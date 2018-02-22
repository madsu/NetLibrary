#include "TCPServer.h"
#include "EventLoop.h"

TcpServer::TcpServer(EventLoop* loop, const int listen_port)
	: loop_(loop)
	, accept_(loop_, listen_port)
{
	accept_.SetConectionCallback(
		std::bind(&TcpServer::NewConnection, this, std::placeholders::_1,
			std::placeholders::_2, std::placeholders::_3));
}

TcpServer::~TcpServer()
{

}

void TcpServer::Start()
{
	accept_.Start();
}

void TcpServer::NewConnection(SOCKET socket, const InetAddress& peerAddr, const InetAddress& localAddr)
{
	TcpConnectionPtr pNewConn = new TcpConnection(loop_, socket, localAddr, peerAddr);
	if (!pNewConn)
		return;

	sessions_[pNewConn->name()] = pNewConn;
	pNewConn->SetConnectionCallback(connectionCallback_);
	pNewConn->SetMessageCallback(messageCallback_);
	pNewConn->SetCloseCallback(
		std::bind(&TcpServer::RemoveConnection, this, std::placeholders::_1));
	pNewConn->OnEstablished();
}

void TcpServer::RemoveConnection(TcpConnectionPtr conn)
{
	sessions_.erase(conn->name());
	conn->OnDestroyed();
	delete conn;
}