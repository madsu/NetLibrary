#pragma once
#include "SocketCommon.h"
#include "Channel.h"

class EventLoop;
class Channel;
class Acceptor
{
public:
	typedef std::function<void(SOCKET)> NewConnCallback;

	Acceptor(EventLoop* loop, int port);
	~Acceptor();

	void Start();

	void HandleAccept();

	void SetConectionCallback(const NewConnCallback& cb)
	{
		newConnCallback_ = cb;
	}

protected:
	void PostAccept();

private:
	int     port_;
	SOCKET  socket_;
	PER_IO_CONTEXT cxt_;
	LPFN_ACCEPTEX acceptEx_;
	LPFN_GETACCEPTEXSOCKADDRS getAcceptExSockAddrs_;

	Channel channel_;

	NewConnCallback newConnCallback_;
};
