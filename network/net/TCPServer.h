#pragma once
#include "SocketCommon.h"
#include "TcpConnection.h"
#include "Acceptor.h"

class EventLoop;
class TcpServer 
{
public:
	TcpServer(EventLoop* loop, const int listen_port);
	~TcpServer();

public:
	void Start();

	void SetConectionCallback(const ConnectionCallback& cb)
	{
		connectionCallback_ = cb;
	}

	void SetMessageCallback(const MessageCallback& cb)
	{
		messageCallback_ = cb;
	}

private:
	void NewConnection(SOCKET socket, const InetAddress& peerAddr, const InetAddress& localAddr);
	void RemoveConnection(TcpConnectionPtr conn);

private:
	EventLoop* loop_;
	CInitSocket init_;
	Acceptor   accept_;

	//connection map
	std::map<std::string, TcpConnectionPtr> sessions_;

	//user callback
	ConnectionCallback connectionCallback_;
	MessageCallback    messageCallback_;
};
