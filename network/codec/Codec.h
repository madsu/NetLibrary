#pragma once

#include "../net/TcpConnection.h"
#include "google/protobuf/message.h"

typedef std::shared_ptr<google::protobuf::Message> MessagePtr;

class ProtobufCodec
{
public:
	enum ErrorCode
	{
		kNoError = 0,
		kInvalidLength,
		kCheckSumError,
		kInvalidNameLen,
		kUnknownMessageType,
		kParseError,
	};

	typedef std::function<void(const TcpConnectionPtr&, const MessagePtr&)> ProtobufMessageCallback;
	typedef std::function<void(const TcpConnectionPtr&, Buffer*, ErrorCode)> ErrorCallback;

	explicit ProtobufCodec(const ProtobufMessageCallback& messageCb);
	ProtobufCodec(const ProtobufMessageCallback& messageCb, const ErrorCallback& errorCb);
	~ProtobufCodec();

	void OnMessage(const TcpConnectionPtr& conn, Buffer* buf);
	void Send();

	static const std::string& errorCodeToString(ErrorCode errorCode);
	static void fillEmptyBuffer(Buffer* buf, const google::protobuf::Message& message);
	static google::protobuf::Message* createMessage(const std::string& type_name);
	static MessagePtr parse(const char* buf, int len, ErrorCode* errorCode);

private:
	static void defaultErrorCallback(const TcpConnectionPtr& conn, Buffer* buf, ErrorCode err);

	ProtobufMessageCallback messageCallback_;
	ErrorCallback  errorCallback_;

	const static int kHeaderLen = sizeof(int32_t);
	const static int kMinMessageLen = 2 * kHeaderLen + 2; // nameLen + typeName + checkSum
	const static int kMaxMessageLen = 64 * 1024 * 1024; 
};
