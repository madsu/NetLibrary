#include "EchoServer.h"
#include <iostream>

EchoServer::EchoServer(EventLoop* loop, int port)
	:server_(loop, port)
{
	server_.SetConectionCallback(
		std::bind(&EchoServer::onConnection, this, std::placeholders::_1));

	server_.SetMessageCallback(
		std::bind(&EchoServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
}

EchoServer::~EchoServer()
{

}

void EchoServer::Start()
{
	server_.Start();
}

void EchoServer::onConnection(const TcpConnectionPtr& conn)
{
	if (conn->GetState() == kConnected) {
		std::cout << "new client:" << conn->name().c_str() << std::endl;
	}
	else {
		std::cout << "client disconnected:" << conn->name().c_str() << std::endl;
	}
}

void EchoServer::onMessage(const TcpConnectionPtr& conn, char* buf)
{
	std::cout << "client:" << conn->name().c_str() << " recv:" << buf << std::endl;
}