#include "Stream.h"

CStream::CStream()
	: m_pBuffer(NULL)
	, m_nLen(0)
{

}

CStream::~CStream()
{

}

bool CStream::SetBuffer(unsigned char* buffer)
{
	if (!buffer)
	{
		return false;
	}

	m_pBuffer = buffer;
	m_nLen = 0;
	return true;
}

unsigned int CStream::GetLength()
{
	return m_nLen;
}

