#pragma once
#include "SocketCommon.h"

class InetAddress
{
public:
	explicit InetAddress(const SOCKADDR_IN& addr);

	ADDRESS_FAMILY family() const { return addr_.sin_family; }
	std::string toIp() const;
	std::string toIpPort() const;
	USHORT toPort() const;

private:
	SOCKADDR_IN addr_;
};
