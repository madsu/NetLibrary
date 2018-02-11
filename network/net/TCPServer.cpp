#include "TCPServer.h"
#include "EventLoop.h"

TcpServer::TcpServer(EventLoop* loop, const int listen_port)
	: loop_(loop)
	, accept_(loop_, listen_port)
{
	accept_.SetConectionCallback(
		std::bind(&TcpServer::NewConnection, this, std::placeholders::_1));
}

TcpServer::~TcpServer()
{

}

bool TcpServer::Start()
{
	loop_->Start();
	accept_.Start();
	return true;
}

void TcpServer::NewConnection(SOCKET socket)
{
	TcpConnectionPtr pNewConn = new TcpConnection(loop_, socket);
	if (!pNewConn)
		return;

	pNewConn->SetMessageCallback(messageCallback_);
	pNewConn->PostRecv();
}