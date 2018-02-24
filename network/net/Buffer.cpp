#include "Buffer.h"
#include "SocketCommon.h"

Buffer::Buffer()
	: readerIndex_(0)
	, writerIndex_(0)
{
}

Buffer::~Buffer()
{
}

void Buffer::WriteBytes(int len)
{
	assert(writerIndex_ + len <= MAX_BUFFER_LEN);

	writerIndex_ += len;
}

void Buffer::Retrive(int len)
{
	if (len < GetReadableBytes())
	{
		readerIndex_ += len;
	}
	else
	{
		RetriveAll();
	}
}

void Buffer::RetriveByte()
{
	Retrive(sizeof(byte));
}

void Buffer::RetriveInt32()
{
	Retrive(sizeof(int32_t));
}

void Buffer::RetriveInt64()
{
	Retrive(sizeof(int64_t));
}

void Buffer::RetriveAll()
{
	readerIndex_ = 0;
	writerIndex_ = 0;
}

std::string Buffer::RetriveAsString(int len)
{
	assert(len <= GetReadableBytes());
	std::string str(buf_, readerIndex_, len);
	Retrive(len);
	return str;
}

std::string Buffer::RetriveAllAsString()
{
	return RetriveAsString(GetReadableBytes());
}

void Buffer::Append(const char* buf, int len)
{
	EnsureWritableBytes(len);
	memcpy_s(GetWriterBuf(), GetWriteableBytes(), buf, len);
	WriteBytes(len);
}

void Buffer::EnsureWritableBytes(int len)
{
	if (GetWriteableBytes() < len)
	{
		MakeSpace(len);
	}
}

void Buffer::MakeSpace(int len)
{
	//FIX ME
	if (readerIndex_ > 0)
	{
		int readlen = GetReadableBytes();
		memmove(&buf_[0], &buf_[readerIndex_], readlen);
		readerIndex_ = 0;
		writerIndex_ -= readlen;
	}
}