#pragma once

#include "../net/TcpConnection.h"
#include "Codec.h"

class ProtobufDispatcher
{
public:
	ProtobufDispatcher();
	~ProtobufDispatcher();

	void OnMessage(const TcpConnectionPtr& conn, const MessagePtr msg);
};
