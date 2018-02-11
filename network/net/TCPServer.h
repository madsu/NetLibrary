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
	bool Start();

	void SetConectionCallback(const ConnectionCallback& cb)
	{
		connectionCallback_ = cb;
	}

	void SetMessageCallback(const MessageCallback& cb)
	{
		messageCallback_ = cb;
	}

private:
	void NewConnection(SOCKET socket);

private:
	EventLoop* loop_;
	CInitSocket init_;
	Acceptor   accept_;
	std::map<UINT32, TcpConnectionPtr> sessions_;

	//用户回调
	ConnectionCallback connectionCallback_;
	MessageCallback    messageCallback_;
};
