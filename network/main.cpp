#include "Stream.h"

int main()
{
	unsigned char* pBuffer = new unsigned char[100]();

	int A = 100;
	char ch = 'a';

	CStream stream;
	stream.SetBuffer(pBuffer);
	stream.WriteData(A);
	stream.WriteData(ch);

	int A2 = 0;
	char ch2 = 0;
	stream.SetBuffer(pBuffer);
	stream.ReadData(A2);
	stream.ReadData(ch2);

	delete pBuffer;
	return 0;
}