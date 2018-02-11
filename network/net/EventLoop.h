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
	EventLoop();
	~EventLoop();

public:
	void Start(INT worker_nums = 0);
	void Stop();
	void Loop();
	void Register(HANDLE h, ULONG_PTR key);

private:
	void Worker();

private:
	const int maxSize = 20;
	std::mutex mtx_;
	std::condition_variable produce_;
	std::condition_variable consume_;
	std::deque<Task> tasks_;

	HANDLE iocp_;

	std::vector<std::thread> workers_;
};