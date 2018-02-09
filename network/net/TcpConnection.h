#pragma once
#include "Callbacks.h"
#include "Channel.h"

class TcpConnection
{
public:
	TcpConnection(EventLoop* loop, SOCKET socket);
	~TcpConnection();

	void SetMessageCallback(const MessageCallback& cb)
	{
		messageCallback_ = cb;
	}

	void PostRecv();
	void HandleRead(char* buf);

private:
	Channel channel_;
	PER_IO_CONTEXT ctx_;
	MessageCallback messageCallback_;
};
