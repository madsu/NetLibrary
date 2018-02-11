#include "TcpConnection.h"

TcpConnection::TcpConnection(EventLoop* loop, SOCKET socket)
	:channel_(loop, socket)
{
	channel_.SetReadCallback(
		std::bind(&TcpConnection::HandleRead, this, std::placeholders::_1));
}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::HandleRead(char* buf)
{

	messageCallback_(this, buf);

	PostRecv();
}

void TcpConnection::PostRecv()
{
	DWORD dwBytes = 0;
	DWORD flags = 0;

	memset(&ctx_, 0, sizeof(ctx_));
	ctx_.ioType = IO_READ;
	ctx_.wsaBuff.buf = ctx_.buffer;
	ctx_.wsaBuff.len = MAX_BUFFER_LEN;

    WSARecv(channel_.GetSocket(), &ctx_.wsaBuff, 1, &dwBytes, &flags, &ctx_.overlapped, NULL);
}