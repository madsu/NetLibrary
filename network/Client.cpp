#include "Client.h"

CClient::CClient()
{

}

CClient::~CClient()
{

}

bool CClient::InilizeContext()
{
	m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_socket == INVALID_SOCKET)
		return false;

	ZeroMemory(&m_iRecvIO.overlapped, sizeof(m_iRecvIO.overlapped));
	ZeroMemory(&m_iRecvIO.bufffer, MAX_BUFFER_LEN);
	m_iRecvIO.ioType = IO_ACCEPT;
	m_iRecvIO.wsaBuff.buf = m_iRecvIO.bufffer;
	m_iRecvIO.len = MAX_BUFFER_LEN;

	return false;
}

bool CClient::PostAcceptEx(LPFN_ACCEPTEX acceptexfunc, SOCKET listenSocket)
{
	DWORD dwBytes = 0;
	
	if(acceptexfunc == NULL)
		return false;

	if (FALSE == acceptexfunc(listenSocket, m_socket, &m_iRecvIO.bufffer, m_iRecvIO.len - ((sizeof(SOCKADDR_IN) + 16) * 2),
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwBytes, &m_iRecvIO.overlapped))
		return false;
		
	return true;
}
