#pragma once

#include "net/TCPServer.h"
#include "codec/codec.h"
#include "codec/Dispatcher.h"

#include "proto/1.pb.h"

typedef std::shared_ptr<test::SearchRequest> SearchRequestPtr;

class EchoServer
{
public:
	EchoServer(EventLoop* loop, int port);
	~EchoServer();

	void Start();

	void onConnection(const TcpConnectionPtr& conn);
	void onSearchRequest(const TcpConnectionPtr& conn, const SearchRequestPtr& msg);

private:
	TcpServer server_;
	ProtobufCodec codec_;
	ProtobufDispatcher dispatcher_;
};
