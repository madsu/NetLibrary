#pragma once

#include "net/TCPServer.h"
#include "codec/codec.h"
#include "codec/Dispatcher.h"

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
	ProtobufCodec codec_;
	ProtobufDispatcher dispatcher_;
};
