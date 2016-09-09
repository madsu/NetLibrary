#pragma once
#include "SocketCommon.h"

class CClient
{
public:
	CClient();
	~CClient();

	bool PostAcceptEx(LPFN_ACCEPTEX acceptexfunc, SOCKET listenSocket);
	bool InilizeContext();

private:
	SOCKET m_socket;
	PER_IO_CONTEXT m_iRecvIO;
	PER_IO_CONTEXT m_iSendIO;
};
