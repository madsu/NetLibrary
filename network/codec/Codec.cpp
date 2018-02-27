#include "codec.h"

ProtobufCodec::ProtobufCodec(const ProtobufMessageCallback& messageCb)
	: messageCallback_(messageCb)
	, errorCallback_(defaultErrorCallback)
{

}

ProtobufCodec::ProtobufCodec(const ProtobufMessageCallback& messageCb, const ErrorCallback& errorCb)
	: messageCallback_(messageCb)
	, errorCallback_(errorCb)
{

}

ProtobufCodec::~ProtobufCodec()
{

}

int32_t asInt32(const char* buf)
{
	int32_t be32 = 0;
	::memcpy(&be32, buf, sizeof(be32));
	return ntohl(be32);
}

void ProtobufCodec::OnMessage(const TcpConnectionPtr& conn, Buffer* buf)
{
	while (buf->readableBytes() >= kMinMessageLen + kHeaderLen)
	{
		const int32_t len = buf->peekInt32();
		if (len > kMaxMessageLen || len < kMinMessageLen)
		{
			errorCallback_(conn, buf, kInvalidLength);
			break;
		}
		else if (buf->readableBytes() >= static_cast<size_t>(len + kHeaderLen))
		{
			ErrorCode errorCode = kNoError;
			MessagePtr message = parse(buf->peek() + kHeaderLen, len, &errorCode);
			if (errorCode == kNoError && message)
			{
				messageCallback_(conn, message);
				buf->retrieve(kHeaderLen + len);
			}
			else
			{
				errorCallback_(conn, buf, errorCode);
				break;
			}
		}
		else
		{
			break;
		}
	}
}

void ProtobufCodec::Send()
{

}

MessagePtr ProtobufCodec::parse(const char* buf, int len, ErrorCode* error)
{
	MessagePtr message;

	// check sum
	int32_t expectedCheckSum = asInt32(buf + len - kHeaderLen);
	// get message type name
	int32_t nameLen = asInt32(buf);
	if (nameLen >= 2 && nameLen <= len - 2 * kHeaderLen)
	{
		std::string typeName(buf + kHeaderLen, buf + kHeaderLen + nameLen - 1);
		// create message object
		message = createMessage(typeName);
		if (message)
		{
			// parse from buffer
			const char* data = buf + kHeaderLen + nameLen;
			int32_t dataLen = len - nameLen - 2 * kHeaderLen;
			if (message->ParseFromArray(data, dataLen))
			{
				*error = kNoError;
			}
			else
			{
				*error = kParseError;
			}
		}
		else
		{
			*error = kUnknownMessageType;
		}
	}
	else
	{
		*error = kInvalidNameLen;
	}

	return message;
}

MessagePtr ProtobufCodec::createMessage(const std::string& type_name)
{
	google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptor* descriptor =
		google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
	if (descriptor)
	{
		const google::protobuf::Message* prototype =
			google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype)
		{
			message = prototype->New();
		}
	}
	return message;
}

const std::string kNoErrorStr = "NoError";
const std::string kInvalidLengthStr = "InvalidLength";
const std::string kCheckSumErrorStr = "CheckSumError";
const std::string kInvalidNameLenStr = "InvalidNameLen";
const std::string kUnknownMessageTypeStr = "UnknownMessageType";
const std::string kParseErrorStr = "ParseError";
const std::string kUnknownErrorStr = "UnknownError";

const std::string& ProtobufCodec::errorCodeToString(ErrorCode errorCode)
{
	switch (errorCode)
	{
	case kNoError:
		return kNoErrorStr;
	case kInvalidLength:
		return kInvalidLengthStr;
	case kCheckSumError:
		return kCheckSumErrorStr;
	case kInvalidNameLen:
		return kInvalidNameLenStr;
	case kUnknownMessageType:
		return kUnknownMessageTypeStr;
	case kParseError:
		return kParseErrorStr;
	default:
		return kUnknownErrorStr;
	}
}


void ProtobufCodec::defaultErrorCallback(const TcpConnectionPtr& conn, Buffer* buf, ErrorCode err)
{

}