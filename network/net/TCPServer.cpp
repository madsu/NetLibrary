#include "TCPServer.h"

TcpServer::TcpServer(EventLoop* loop, const int listen_port)
	: loop_(loop)
	, port_(listen_port)
{
	loop_->SetConnectionCallback(std::bind(&TcpServer::NewConnection, 
		this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer()
{

}

//将监听套接字绑定到eventloop
bool TcpServer::Start()
{
	//创建一个监听套接字
	listensock_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listensock_ == INVALID_SOCKET)
		return false;

	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port_);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	int nRet = bind(listensock_, (LPSOCKADDR)&serverAddr, sizeof(serverAddr));
	if (nRet == SOCKET_ERROR)
		return false;

	nRet = listen(listensock_, SOMAXCONN);
	if (nRet == SOCKET_ERROR)
		return false;

	DWORD dwBytes = 0;
	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	nRet = WSAIoctl(listensock_, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidAcceptEx, sizeof(GuidAcceptEx),
		&acceptEx_, sizeof(acceptEx_), &dwBytes, NULL, NULL);
	if (nRet == SOCKET_ERROR)
		return false;

	//注册监听套接字至完成端口
	handleData_.socket = listensock_;
	loop_->Register((HANDLE)listensock_, (ULONG_PTR)&handleData_);

	//投递接受连接事件
	SOCKET clientsock_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (clientsock_ == INVALID_SOCKET)
		return false;

	memset(&ioContext_, 0, sizeof(ioContext_));
	ioContext_.ioType = IO_ACCEPT;
	ioContext_.client = clientsock_;
	if (FALSE == acceptEx_(listensock_, clientsock_, &ioContext_.buffer, ioContext_.len - ((sizeof(SOCKADDR_IN) + 16) * 2),
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwBytes, &ioContext_.overlapped))
		return false;

	return true;
}

void TcpServer::NewConnection(PER_HANDLE_DATA* data, PER_IO_CONTEXT* io)
{

}