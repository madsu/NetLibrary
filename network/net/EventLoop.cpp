#include "EventLoop.h"
#include "Channel.h"

EventLoop::EventLoop()
	:iocp_(CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0))
{
}

EventLoop::~EventLoop()
{
}

void EventLoop::Start(INT worker_nums /* = 0 */)
{
	if (worker_nums == 0) 
	{
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);

		worker_nums = sysInfo.dwNumberOfProcessors * 2;
	}

	//创建worker线程
	for (INT i = 0; i < worker_nums; i++)
	{
		workers_.push_back(std::thread(
			std::bind(&EventLoop::Worker, this)));
	}
}

void EventLoop::Stop()
{
	if (iocp_ != INVALID_HANDLE_VALUE)
		CloseHandle(iocp_);

	std::vector<std::thread>::iterator iter = workers_.begin();
	for (; iter != workers_.end(); ++iter)
		iter->join();
}

void EventLoop::Loop()
{
	while (true)
	{
		std::unique_lock<std::mutex> lcx(mtx_);
		consume_.wait(lcx, [this] {return tasks_.size() != 0; });

		Task task = tasks_.front();
		tasks_.pop_front();
		Channel* channel = task.channel;
		PER_IO_CONTEXT* io = task.ctx;
		channel->HandleIoMessage(io);

		produce_.notify_all();
	}
}

void EventLoop::Register(HANDLE h, ULONG_PTR key)
{
	if (iocp_ == INVALID_HANDLE_VALUE)
		return;

	CreateIoCompletionPort(h, iocp_, key, 0);
}

void EventLoop::Worker()
{
	if (iocp_ == INVALID_HANDLE_VALUE)
		return;

	DWORD dwTranceBytes = 0;
	ULONG_PTR uComKey = NULL;
	LPOVERLAPPED pOverlapped = NULL;
	BOOL bRet = FALSE;

	while (true)
	{
		bRet = GetQueuedCompletionStatus(iocp_, &dwTranceBytes, &uComKey, &pOverlapped, INFINITE);
		if (!bRet && !pOverlapped)
		{
			return;
		}

		Task task;
		task.channel = reinterpret_cast<Channel*>(uComKey);
		task.ctx = CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, overlapped);

		std::unique_lock<std::mutex> lcx(mtx_);
		produce_.wait(lcx, [this] {return tasks_.size() != maxSize; });
		
		tasks_.push_back(task);

		consume_.notify_all();
	}
}