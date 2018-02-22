#include "InetAddress.h"
#include <sstream>

InetAddress::InetAddress(const SOCKADDR_IN& addr)
	: addr_(addr)
{

}

std::string InetAddress::toIp() const
{
	char str[INET_ADDRSTRLEN] = {};
	inet_ntop(AF_INET, (PVOID)&addr_.sin_addr, str, sizeof(str));
	return str;
}

std::string InetAddress::toIpPort() const
{
	std::stringstream str("");
	str << toIp() << ":" << toPort();
	return str.str();
}

USHORT InetAddress::toPort() const
{
	return ntohs(addr_.sin_port);
}