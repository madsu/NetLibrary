#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop* loop, SOCKET s)
	: loop_(loop)
	, socket_(s)
{
	loop_->Register((HANDLE)socket_, (ULONG_PTR)this);
}

Channel::~Channel()
{
}

void Channel::HandleIoMessage(PER_IO_CONTEXT* cxt)
{
	switch (cxt->ioType)
	{
	case IO_ACCEPT:
	{
		acceptCallback_();
	}
	break;

	case IO_READ:
	{
		readCallback_(&cxt->buf);
	}
	break;

	case IO_WRITE:
	{
		writeCallback_();
	}
	break;

	default:
		break;
	}
}

