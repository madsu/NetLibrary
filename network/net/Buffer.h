#pragma once
#include <assert.h>
#include <string>

#define MAX_BUFFER_LEN 10240
class Buffer
{
public: 
	Buffer();
	~Buffer();

	int GetReadableBytes() { assert(writerIndex_ >= readerIndex_); return writerIndex_ - readerIndex_; }
	int GetWriteableBytes() { assert(writerIndex_ <= MAX_BUFFER_LEN); return MAX_BUFFER_LEN - writerIndex_; }
	char* GetReaderBuf() { return &buf_[readerIndex_]; }
	char* GetWriterBuf() { return &buf_[writerIndex_]; }

	//write bytes
	void WriteBytes(int len);

	//Retrive 
	void Retrive(int len);
	void RetriveByte();
	void RetriveInt32();
	void RetriveInt64();
	void RetriveAll();
	std::string RetriveAsString(int len);
	std::string RetriveAllAsString();

	//Append
	void Append(const char* buf, int len);
private:
	void EnsureWritableBytes(int len);
	void MakeSpace(int len);

private:
	char buf_[MAX_BUFFER_LEN];
	int readerIndex_;
	int writerIndex_;
};