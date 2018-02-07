#include "EchoServer.h"

EchoServer::EchoServer(EventLoop* loop, int port)
	:server_(loop, port)
{

}

EchoServer::~EchoServer()
{

}

void EchoServer::Start()
{
	server_.Start();
}