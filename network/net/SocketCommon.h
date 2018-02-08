#pragma once

#include <functional>
#include <WinSock2.h>
#include <MSWSock.h>
#include <Windows.h>
#pragma comment(lib, "WS2_32")

#include <map>
#include <vector>
#include <list>

#define  MAX_POST_ACCEPT 10
#define  MAX_BUFFER_LEN 10240

enum IO_TYPE
{
	IO_ACCEPT = 0,
	IO_READ,
	IO_WRITE
};

typedef struct _PER_IO_CONTEXT_
{
	OVERLAPPED overlapped;
	IO_TYPE    ioType;
	WSABUF     wsaBuff;
	char       buffer[MAX_BUFFER_LEN];
	int        len;
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