#pragma once
#include "Callbacks.h"
#include "Channel.h"
#include "InetAddress.h"

enum Estate
{
	kConnected = 0,
	kDisconnected
};

class TcpConnection
{
public:
	TcpConnection(EventLoop* loop, SOCKET socket,
		const InetAddress& localAddr,
		const InetAddress& peerAddr);
	~TcpConnection();

public:
	const std::string name() const { return peerAddr_.toIpPort(); }
	const InetAddress& localAddress() const { return localAddr_; }
	const InetAddress& peerAddress() const { return peerAddr_; }
	Estate GetState() const { return state_; }

	void SetMessageCallback(const MessageCallback& cb)
	{
		messageCallback_ = cb;
	}

	void SetConnectionCallback(const ConnectionCallback& cb)
	{
		connectionCallback_ = cb;
	}

	//for tcpserver use
	void SetCloseCallback(const CloseCallback& cb)
	{
		closeCallback_ = cb;
	}

	void OnEstablished();
	void OnDestroyed();
	void HandleRead(Buffer* buf);

	//Send data
	void Send(const char* buf, int len);
	void HandleWrite();

private:
	void PostRecv();
	void SetState(Estate state) { state_ = state; };

private:
	Channel channel_;
	//connetion info
	Estate state_;
	const InetAddress localAddr_;
	const InetAddress peerAddr_;
	//send recv buffer
	PER_IO_CONTEXT recvCtx_;
	PER_IO_CONTEXT sendCtx_;
	//user callback
	MessageCallback messageCallback_;
	ConnectionCallback connectionCallback_;
	CloseCallback   closeCallback_;
};
