#include "net/EventLoop.h"
#include "net/TCPServer.h"
#include "EchoServer.h"

int main()
{
	EventLoop loop;
	EchoServer server(&loop, 7002);
	server.Start();
	loop.Loop();
}