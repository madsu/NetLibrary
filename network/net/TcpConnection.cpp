#include "TcpConnection.h"

TcpConnection::TcpConnection(EventLoop* loop, SOCKET socket,
	const InetAddress& localAddr, const InetAddress& peerAddr)
	: channel_(loop, socket)
	, localAddr_(localAddr)
	, peerAddr_(peerAddr)
{
	channel_.SetReadCallback(
		std::bind(&TcpConnection::HandleRead, this, std::placeholders::_1));
	channel_.SetWriteCallback(
		std::bind(&TcpConnection::HandleWrite, this));
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
	recvCtx_.ioType = IO_READ;
	recvCtx_.wsaBuff.buf = recvCtx_.buf.GetWriterBuf();
	recvCtx_.wsaBuff.len = recvCtx_.buf.GetWriteableBytes();

    INT rc = WSARecv(channel_.GetSocket(), &recvCtx_.wsaBuff, 1, NULL, &flags, &recvCtx_.overlapped, NULL);
	if (rc == SOCKET_ERROR && WSA_IO_PENDING != WSAGetLastError())
	{
		closeCallback_(this);
		return;
	}
}

void TcpConnection::Send(const char* buf, int len)
{
	int readbytes = sendCtx_.buf.GetReadableBytes();
	sendCtx_.buf.Append(buf, len);
	if (readbytes == 0)
	{
		HandleWrite();
	}
}

void TcpConnection::HandleWrite()
{
	if (sendCtx_.buf.GetReadableBytes() == 0)
		return;

	int rc = SOCKET_ERROR;
	do
	{
		DWORD sendBytes = 0;
		sendCtx_.ioType = IO_WRITE;
		sendCtx_.wsaBuff.buf = sendCtx_.buf.GetReaderBuf();
		sendCtx_.wsaBuff.len = sendCtx_.buf.GetReadableBytes();
		rc = WSASend(channel_.GetSocket(), &sendCtx_.wsaBuff, 1, &sendBytes, 0, &sendCtx_.overlapped, NULL);
		if (rc == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING)
		{
			break;
		}
		else if (rc == WSAEACCES)
		{
			sendCtx_.buf.Retrive(sendBytes);
		}

	} while (rc == WSAEACCES && sendCtx_.buf.GetReadableBytes() != 0);
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