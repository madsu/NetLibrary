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

	void SetCloseCallback(const CloseCallback& cb)
	{
		closeCallback_ = cb;
	}

	void PostRecv();
	void HandleRead(char* buf, DWORD len);

private:
	Channel channel_;
	PER_IO_CONTEXT ctx_;
	MessageCallback messageCallback_;
	CloseCallback   closeCallback_;
};
