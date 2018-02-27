#include "EchoServer.h"
#include <iostream>

EchoServer::EchoServer(EventLoop* loop, int port)
	: server_(loop, port)
	, codec_(std::bind(&ProtobufDispatcher::OnMessage, &dispatcher_,
		std::placeholders::_1, std::placeholders::_2))
{
	server_.SetConectionCallback(
		std::bind(&EchoServer::onConnection, this, std::placeholders::_1));

	server_.SetMessageCallback(
		std::bind(&ProtobufCodec::OnMessage, &codec_, std::placeholders::_1, std::placeholders::_2));
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

void EchoServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf)
{
	std::string text = buf->retrieveAllAsString();
	std::cout << "client:" << conn->name().c_str() << " recv:" << text << std::endl;
	conn->Send(text.c_str(), text.length());
}