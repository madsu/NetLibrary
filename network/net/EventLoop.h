#pragma once
#include "SocketCommon.h"

class EventLoop
{
public:
	typedef std::function<void(PER_HANDLE_DATA*, PER_IO_CONTEXT*)> ConnectionCallback;

public:
	EventLoop();
	~EventLoop();

public:
	void Loop();
	void Register(HANDLE h, ULONG_PTR key);
	void SetConnectionCallback(ConnectionCallback func)
	{
		connection_ = func;
	}

private:
	HANDLE iocp_;
	ConnectionCallback connection_;
};