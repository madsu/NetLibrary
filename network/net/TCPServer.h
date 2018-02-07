#pragma once
#include "EventLoop.h"

class TcpServer 
{
public:
	TcpServer(EventLoop* loop, const int listen_port);
	~TcpServer();

public:
	bool Start();

private:
	EventLoop* loop_;
	CInitSocket init_;

	void NewConnection(PER_HANDLE_DATA* data, PER_IO_CONTEXT* io);
	//TODO Acceptor
	int port_;
	SOCKET listensock_;
	LPFN_ACCEPTEX acceptEx_;
	PER_HANDLE_DATA handleData_;
	PER_IO_CONTEXT  ioContext_;
};
