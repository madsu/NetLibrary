#pragma once

#include "net/TCPServer.h"

class EchoServer
{
public:
	EchoServer(EventLoop* loop, int port);
	~EchoServer();

	void Start();

	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn, Buffer* buf);

private:
	TcpServer server_;
};
