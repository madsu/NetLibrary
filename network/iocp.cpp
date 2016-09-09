#include "Iocp.h"
#include "Client.h"

CNetWorkSession::CNetWorkSession()
	: m_hCompletionPort(INVALID_HANDLE_VALUE)
	, m_listenSocket(INVALID_SOCKET)
	, m_phWorkThreads(NULL)
	, m_pfnAcceptEx(NULL)
	, m_nWorkThreads(0)
{

}

CNetWorkSession::~CNetWorkSession()
{

}

bool CNetWorkSession::Start(int port)
{
	int nRet = 0;

	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (m_hCompletionPort == INVALID_HANDLE_VALUE)
		return false;

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	m_nWorkThreads = sysInfo.dwNumberOfProcessors * 2;

	m_phWorkThreads = new HANDLE[m_nWorkThreads];
	if (m_phWorkThreads == NULL)
	{
		CloseHandle(m_hCompletionPort);
		return false;
	}

	for (int i = 0; i < m_nWorkThreads; ++i)
	{
		m_phWorkThreads[i] = CreateThread(NULL, 0, _WorkerThreadFunc, this, 0, NULL);
	}


	m_listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_listenSocket == INVALID_SOCKET)
		return false;

	if (NULL == CreateIoCompletionPort((HANDLE)m_listenSocket, m_hCompletionPort, NULL, 0))
		return false;

	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	nRet = bind(m_listenSocket, (LPSOCKADDR)&serverAddr, sizeof(serverAddr));
	if (nRet == SOCKET_ERROR)
		return false;

	nRet = listen(m_listenSocket, SOMAXCONN);
	if (nRet == SOCKET_ERROR)
		return false;

	DWORD dwBytes = 0;
	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	nRet = WSAIoctl(m_listenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidAcceptEx, sizeof(GuidAcceptEx),
		&m_pfnAcceptEx, sizeof(m_pfnAcceptEx), &dwBytes, NULL, NULL);
	if (nRet == SOCKET_ERROR)
		return false;

	for (int i = 0; i < MAX_POST_ACCEPT; ++i)
	{
		CClient* pClient = new CClient;
		pClient->InilizeContext();
		pClient->PostAcceptEx(m_pfnAcceptEx, m_listenSocket);
	}

	return true;
}

bool CNetWorkSession::Stop()
{
	return true;
}

DWORD WINAPI CNetWorkSession::_WorkerThreadFunc(LPVOID param)
{
	return 0;
}