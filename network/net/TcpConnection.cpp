#include "TcpConnection.h"

TcpConnection::TcpConnection(EventLoop* loop, SOCKET socket)
	:channel_(loop, socket)
{
	channel_.SetReadCallback(
		std::bind(&TcpConnection::OnRecv, this, std::placeholders::_1));
}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::OnRecv(char* buf)
{

}