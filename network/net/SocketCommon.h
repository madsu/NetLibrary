#pragma once

#include <functional>
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#include <Windows.h>
#pragma comment(lib, "WS2_32")

#include <map>
#include <vector>
#include <list>
#include <string>
#include <assert.h>
#include <algorithm>

#include "Buffer.h"
#define  MAX_POST_ACCEPT 10

enum IO_TYPE
{
	IO_ACCEPT = 0,
	IO_READ,
	IO_WRITE,
	IO_REMOVE
};

typedef struct _PER_IO_CONTEXT_
{
	OVERLAPPED overlapped;
	IO_TYPE    ioType;
	WSABUF     wsaBuff;
	Buffer     buf;
	DWORD      transferBytes;
	SOCKET     client;
}PER_IO_CONTEXT, *PPER_IO_CONTEXT;

class CInitSocket
{
public:
	CInitSocket(BYTE minorVer = 2, BYTE majorVer = 2)
	{
		WSADATA wsaData = {};
		WORD sockVersion = MAKEWORD(minorVer, majorVer);
		WSAStartup(sockVersion, &wsaData);
	}

	~CInitSocket()
	{
		WSACleanup();
	}
};