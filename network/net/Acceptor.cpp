#include "Acceptor.h"

Acceptor::Acceptor(EventLoop* loop, int port)
	: socket_(WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED))
	, port_(port)
	, channel_(loop, socket_)
{
	channel_.SetAcceptCallback(
		std::bind(&Acceptor::HandleAccept, this));
}

Acceptor::~Acceptor()
{
}

void Acceptor::Start()
{
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port_);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	int nRet = bind(socket_, (LPSOCKADDR)&serverAddr, sizeof(serverAddr));
	if (nRet == SOCKET_ERROR)
		return ;

	nRet = listen(socket_, SOMAXCONN);
	if (nRet == SOCKET_ERROR)
		return ;

	DWORD dwBytes = 0;
	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	nRet = WSAIoctl(socket_, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidAcceptEx, sizeof(GuidAcceptEx),
		&acceptEx_, sizeof(acceptEx_), &dwBytes, NULL, NULL);
	if (nRet == SOCKET_ERROR)
		return ;

	PostAccept();
}

void Acceptor::HandleAccept()
{
	if (newConnCallback_)
	{
		newConnCallback_(cxt_.client);
	}

	//继续投递事件
	PostAccept();
}

void Acceptor::PostAccept()
{
	//投递接受连接事件
	SOCKET clientsock_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (clientsock_ == INVALID_SOCKET)
		return ;

	memset(&cxt_, 0, sizeof(cxt_));
	cxt_.ioType = IO_ACCEPT;
	cxt_.client = clientsock_;
	DWORD dwBytes = 0;
	if (FALSE == acceptEx_(socket_, clientsock_, &cxt_.buffer, MAX_BUFFER_LEN - ((sizeof(SOCKADDR_IN) + 16) * 2),
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwBytes, &cxt_.overlapped))
		return ;
}