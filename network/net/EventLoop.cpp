#include "EventLoop.h"
#include "Channel.h"

EventLoop::EventLoop()
	:iocp_(CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0))
{
}

EventLoop::~EventLoop()
{
}

void EventLoop::Loop()
{
	if (iocp_ == INVALID_HANDLE_VALUE)
		return;

	DWORD dwTranceBytes = 0;
	ULONG_PTR uComKey = NULL;
	LPOVERLAPPED pOverlapped = NULL;

	BOOL bRet = GetQueuedCompletionStatus(iocp_, &dwTranceBytes, &uComKey, &pOverlapped, INFINITE);
	if (!bRet && !pOverlapped)
	{
		return;
	}

	if (uComKey == NULL)
	{
		// TODO
		return;
	}

	Channel* channel = reinterpret_cast<Channel*>(uComKey);
	PER_IO_CONTEXT*  io = CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, overlapped);
	channel->HandleIoMessage(io);
}

void EventLoop::Register(HANDLE h, ULONG_PTR key)
{
	if (iocp_ == INVALID_HANDLE_VALUE)
		return;

	CreateIoCompletionPort(h, iocp_, key, 0);
}