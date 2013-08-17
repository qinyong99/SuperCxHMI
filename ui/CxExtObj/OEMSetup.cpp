// OEMSetup.cpp: implementation of the COEMSetup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OEMSetup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COEMSetup COEMSetup::s_setup;

COEMSetup::COEMSetup()
{
	TCHAR szModule[MAX_PATH];
	GetModuleFileName(NULL, szModule, MAX_PATH);
	
	LPTSTR lpszTemp = (LPTSTR)szModule;
	for (LPCTSTR lpsz = szModule; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
	{
		// remember last directory/drive separator
		if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
			lpszTemp = (LPTSTR)_tcsinc(lpsz);
	}

	m_strModuleSection = lpszTemp;

	*lpszTemp = 0;

	m_strSetupFile = szModule;
	m_strSetupFile += _T("OEMSetup.ini");

#ifdef _DEBUG
//	m_strSetupFile = _T("d:\\workdir\\cxdemo\\OEMSetup.ini");
#endif
}

COEMSetup::~COEMSetup()
{
	
}

CString COEMSetup::GetString(LPCTSTR lpszSection, LPCTSTR lpszEntry,
	LPCTSTR lpszDefault)
{
	static TCHAR chNil = '\0';

	if (lpszDefault == NULL)
		lpszDefault = &chNil;    // don't pass in NULL
	TCHAR szT[4096];
	DWORD dw = ::GetPrivateProfileString(lpszSection, lpszEntry,
		lpszDefault, szT, sizeof(szT) / sizeof(szT[0]), s_setup.m_strSetupFile);
//	ASSERT(dw < 4095);
	return szT;
}

CString COEMSetup::GetCommonString(LPCTSTR lpszEntry, LPCTSTR lpszDefault)
{
	return GetString(_T("Common"), lpszEntry, lpszDefault);
}

CString COEMSetup::GetModuleString(LPCTSTR lpszSection, LPCTSTR lpszEntry, 
		LPCTSTR lpszDefault)
{
	return GetString(s_setup.m_strModuleSection, lpszEntry, lpszDefault);
}

CString COEMSetup::GetSystemName()
{
	return GetCommonString(_T("SystemName"), _T("SuperCx"));
}

BOOL COEMSetup::IsSetupFileExist()
{
	HANDLE hFile = CreateFile(
		s_setup.m_strSetupFile,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		// If the handle is valid then the file exists.
		CloseHandle(hFile);
		return TRUE;
	}
	
	return FALSE;
}