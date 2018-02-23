#include "Acceptor.h"
#include <iostream>

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

	GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;
	nRet = WSAIoctl(socket_, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidGetAcceptExSockAddrs, sizeof(GuidGetAcceptExSockAddrs),
		&getAcceptExSockAddrs_, sizeof(getAcceptExSockAddrs_), &dwBytes, NULL, NULL);
	if (nRet == SOCKET_ERROR)
		return;

	PostAccept();
}

void Acceptor::HandleAccept()
{
	SOCKADDR_IN* ClientAddr = NULL;
	SOCKADDR_IN* LocalAddr = NULL;
	int remoteLen = sizeof(SOCKADDR_IN);
	int localLen = sizeof(SOCKADDR_IN);

	getAcceptExSockAddrs_(cxt_.buf.GetReaderBuf(), cxt_.buf.GetReadableBytes(), localLen + 16, remoteLen + 16,
		(LPSOCKADDR*)&LocalAddr, &localLen, (LPSOCKADDR*)&ClientAddr, &remoteLen);

	////新连接到达
	//char str[INET_ADDRSTRLEN];
	//std::cout << "new client:" << inet_ntop(AF_INET, &ClientAddr->sin_addr, str, sizeof(str));
	//std::cout << "  port" << ntohs(ClientAddr->sin_port) << std::endl;

	if (newConnCallback_)
	{
		InetAddress peerAddr(*ClientAddr);
		InetAddress localAddr(*LocalAddr);
		newConnCallback_(cxt_.client, peerAddr, localAddr);
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

	cxt_.ioType = IO_ACCEPT;
	cxt_.client = clientsock_;
	cxt_.buf.RetriveAll();

	DWORD dwBytes = 0;
	INT   len = sizeof(SOCKADDR_IN) + 16;
	BOOL ret = acceptEx_(socket_, clientsock_, cxt_.buf.GetWriterBuf(), 0, len, len, &dwBytes, &cxt_.overlapped);
	if (FALSE == ret && ERROR_IO_PENDING != GetLastError()) {
		std::cout << "Accept failed:" << std::endl;
	}
	return;
}