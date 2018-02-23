#include "TcpConnection.h"

TcpConnection::TcpConnection(EventLoop* loop, SOCKET socket,
	const InetAddress& localAddr, const InetAddress& peerAddr)
	: channel_(loop, socket)
	, localAddr_(localAddr)
	, peerAddr_(peerAddr)
{
	channel_.SetReadCallback(
		std::bind(&TcpConnection::HandleRead, this, std::placeholders::_1));
}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::HandleRead(Buffer* buf)
{
	if (buf->GetReadableBytes() == 0) {
		closeCallback_(this);
		return;
	}

	messageCallback_(this, buf);
	PostRecv();
}

void TcpConnection::PostRecv()
{
	DWORD flags = 0;
	ctx_.ioType = IO_READ;
	ctx_.wsaBuff.buf = ctx_.buf.GetWriterBuf();
	ctx_.wsaBuff.len = ctx_.buf.GetWriteableBytes();

    INT rc = WSARecv(channel_.GetSocket(), &ctx_.wsaBuff, 1, NULL, &flags, &ctx_.overlapped, NULL);
	if (rc == SOCKET_ERROR && WSA_IO_PENDING != WSAGetLastError())
	{
		closeCallback_(this);
		return;
	}
}

void TcpConnection::OnEstablished()
{
	PostRecv();
	SetState(kConnected);
	connectionCallback_(this);
}

void TcpConnection::OnDestroyed()
{
	closesocket(channel_.GetSocket());
	SetState(kDisconnected);
	connectionCallback_(this);
}