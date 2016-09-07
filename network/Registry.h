#pragma once
#include <tchar.h>
#include <windows.h>

class CRegistry
{
public:
	CRegistry();
    virtual	~CRegistry();

public:
	bool Open(HKEY rootKey, const TCHAR* subKey);
	bool Close();
	bool SetValue(const TCHAR* valueName, const TCHAR* value);
	bool SetValue(const TCHAR* valueName, DWORD value);
	bool GetValue(const TCHAR* valueName, TCHAR* value, LPDWORD bufferLength);
	bool GetValue(const TCHAR* valueName, LPDWORD value);
	bool CreateKey(HKEY rootKey, const TCHAR* subKey);
	bool DeleteKey(HKEY rootKey, const TCHAR* subKey);

private:
	HKEY m_openKey;
	bool m_bIsOpen;
};
