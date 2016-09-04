#pragma once
#include <string.h>

class CStream
{
public:
	CStream();
	~CStream();

private:
	unsigned char *m_pBuffer;
	unsigned int  m_nLen;

public:
	bool SetBuffer(unsigned char* buffer);
	unsigned int GetLength();

	template<typename T>
	bool WriteData(const T& data);

	template<typename T>
	bool ReadData(T& data);
};

template<typename T>
bool CStream::WriteData(const T& data)
{
	memcpy(m_pBuffer + m_nLen, (void*)&data, sizeof(T));

	m_nLen += sizeof(T);
	return true;
}

template<typename T>
bool CStream::ReadData(T& data)
{
	memcpy((void*)&data, m_pBuffer + m_nLen, sizeof(T));

	m_nLen += sizeof(T);
	return true;
}