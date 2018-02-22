#pragma once
#include "Callbacks.h"
#include "Channel.h"
#include "InetAddress.h"

enum 
{

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

	void PostRecv();
	void HandleRead(char* buf, DWORD len);

private:
	Channel channel_;

	const InetAddress localAddr_;
	const InetAddress peerAddr_;

	PER_IO_CONTEXT ctx_;
	MessageCallback messageCallback_;
	ConnectionCallback connectionCallback_;
	CloseCallback   closeCallback_;
};
