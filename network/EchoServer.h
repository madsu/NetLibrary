#pragma once

#include "net/TCPServer.h"

class EchoServer
{
public:
	EchoServer(EventLoop* loop, int port);
	~EchoServer();

	void Start();

	TcpServer server_;
};
