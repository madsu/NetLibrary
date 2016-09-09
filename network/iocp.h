#pragma once

#include "SocketCommon.h"

class CNetWorkSession
{
public:
	CNetWorkSession();
	~CNetWorkSession();

public:
	bool Start(int port = 6666);
	bool Stop();

protected:
	static DWORD WINAPI _WorkerThreadFunc(LPVOID param);

private:
	CInitSocket m_initSocket;
	LPFN_ACCEPTEX m_pfnAcceptEx;
	SOCKET      m_listenSocket;

	HANDLE      m_hCompletionPort;
	int         m_nWorkThreads;
	HANDLE*     m_phWorkThreads;
};