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

void TcpServer::Start()
{
	accept_.Start();
}

void TcpServer::NewConnection(SOCKET socket)
{
	TcpConnectionPtr pNewConn = new TcpConnection(loop_, socket);
	if (!pNewConn)
		return;

	pNewConn->SetMessageCallback(messageCallback_);
	pNewConn->SetCloseCallback(
		std::bind(&TcpServer::RemoveConnection, this, std::placeholders::_1));
	pNewConn->PostRecv();
}

void TcpServer::RemoveConnection(TcpConnectionPtr conn)
{
	delete conn;
}