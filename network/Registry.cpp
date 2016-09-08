#include "Registry.h"

CRegistry::CRegistry()
	: m_openKey(NULL)
	, m_bIsOpen(false)
{

}

CRegistry::~CRegistry()
{

}

bool CRegistry::Open(HKEY rootKey, const TCHAR* subKey)
{
	if (!subKey)
		return false;

	if (m_bIsOpen)
		return false;

	if (RegOpenKey(rootKey, subKey, &m_openKey) != ERROR_SUCCESS)
		return false;

	m_bIsOpen = true;
	return true;
}

bool CRegistry::Close()
{
	if (RegCloseKey(m_openKey) != ERROR_SUCCESS)
		return false;

	m_bIsOpen = false;
	return true;
}

bool CRegistry::SetValue(const TCHAR* valueName, const TCHAR* value)
{
	if (!valueName || !value)
		return false;

	if (!m_bIsOpen)
		return false;

	if (RegSetValueEx(m_openKey, valueName, 0, REG_SZ, (BYTE*)&value, _tcslen(value)) != ERROR_SUCCESS)
		return false;

	return true;
}

bool CRegistry::SetValue(const TCHAR* valueName, DWORD value)
{
	if (!valueName)
		return false;

	if (!m_bIsOpen)
		return false;

	if (RegSetValueEx(m_openKey, valueName, 0, REG_DWORD, (BYTE*)&value, sizeof(DWORD)) != ERROR_SUCCESS)
		return false;

	return true;
}

bool CRegistry::GetValue(const TCHAR* valueName, TCHAR* value, LPDWORD bufferLength)
{
	DWORD valueType = 0;

	if (!valueName || !value)
		return false;

	if (!m_bIsOpen)
		return false;

	if (RegQueryValueEx(m_openKey, valueName, 0, &valueType, (BYTE*)value, bufferLength) != ERROR_SUCCESS)
		return false;

	return true;
}

bool CRegistry::GetValue(const TCHAR* valueName, LPDWORD value)
{
	DWORD valueType = 0;
	DWORD bufferLength = sizeof(DWORD);

	if (!valueName)
		return false;

	if (!m_bIsOpen)
		return false;

	if (RegQueryValueEx(m_openKey, valueName, 0, &valueType, (BYTE*)value, &bufferLength) != ERROR_SUCCESS)
		return false;

	return true;
}

bool CRegistry::CreateKey(HKEY rootKey, const TCHAR* subKey)
{
	if (!subKey)
		return false;

	if (m_bIsOpen)
		return false;

	if (RegCreateKey(rootKey, subKey, &m_openKey) != ERROR_SUCCESS)
		return false;

	m_bIsOpen = true;
	return true;
}

bool CRegistry::DeleteKey(HKEY rootKey, const TCHAR* subKey)
{
	if (!subKey)
		return false;

	if (RegDeleteKey(rootKey, subKey) != ERROR_SUCCESS)
		return false;

	return true;
}
