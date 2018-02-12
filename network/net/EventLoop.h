#pragma once
#include "SocketCommon.h"
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class Channel;
struct Task
{
	PPER_IO_CONTEXT ctx;
	Channel*        channel;
};

class EventLoop
{
public:
	typedef std::function<void()> Functor;

	EventLoop();
	~EventLoop();

public:
	void Loop(int time_out = 100);
	void Register(HANDLE h, ULONG_PTR key);
	void RunInLoop(const Functor& cb);

private:
	void Start(INT worker_nums = 0);
	void Stop();
	void Worker();
	void DoPendingFunctor();

private:
	const int maxSize = 20;
	std::mutex mtx_;
	std::condition_variable produce_;
	std::condition_variable consume_;
	std::deque<Task> tasks_;

	//Delay call
	std::vector<Functor> pendingFunctors_;

	HANDLE iocp_;
	std::vector<std::thread> workers_;
};