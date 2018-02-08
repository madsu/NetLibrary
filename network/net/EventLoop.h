#pragma once
#include "SocketCommon.h"

class EventLoop
{
public:
	EventLoop();
	~EventLoop();

public:
	void Loop();
	void Register(HANDLE h, ULONG_PTR key);

private:
	HANDLE iocp_;
};