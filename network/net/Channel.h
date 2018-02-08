#pragma once
#include "SocketCommon.h"

class EventLoop;
class Channel
{
public:
	typedef std::function<void()> EventCallback;
	typedef std::function<void(char* buf)> ReadEventCallback;

	explicit Channel(EventLoop* loop, SOCKET s);
	~Channel();
	
	void HandleIoMessage(PER_IO_CONTEXT* cxt);
	void SetReadCallback(const ReadEventCallback& cb)
	{
		readCallback_ = cb;
	}
	void SetWriteCallback(const EventCallback& cb)
	{
		writeCallback_ = cb;
	}
	void SetCloseCallback(const EventCallback& cb)
	{
		closeCallback_ = cb;
	}
	void SetErrorCallback(const EventCallback& cb)
	{
		errorCallback_ = cb;
	}
	void SetAcceptCallback(const EventCallback& cb)
	{
		acceptCallback_ = cb;
	}

private:
	SOCKET socket_;
	EventLoop* loop_;

	ReadEventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback closeCallback_;
	EventCallback errorCallback_;
	EventCallback acceptCallback_;
};
