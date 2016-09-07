#include <tchar.h>
#include <assert.h>
#include <windows.h>

#include "Stream.h"
#include "Registry.h"

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

	DWORD value = 0;
	CRegistry registry;
	registry.CreateKey(HKEY_CURRENT_USER, _T("MaDashu"));
	registry.SetValue(_T("madsu"), 100);
	registry.GetValue(_T("madsu"), &value);
	assert(value == 100);
	registry.DeleteKey(HKEY_CURRENT_USER, _T("MaDashu"));
	registry.Close();
	return 0;
}