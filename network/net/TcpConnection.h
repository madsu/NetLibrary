#pragma once
#include "Channel.h"

class TcpConnection
{
public:
	TcpConnection(EventLoop* loop, SOCKET socket);
	~TcpConnection();

	void OnRecv(char* buf);

private:
	Channel channel_;
};

typedef TcpConnection* TcpConnectionPtr;