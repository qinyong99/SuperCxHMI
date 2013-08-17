// OEMSetup.h: interface for the COEMSetup class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _OEMSETUP_H__
#define _OEMSETUP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COEMSetup  
{
public:
	COEMSetup();
	virtual ~COEMSetup();

	static CString GetString(LPCTSTR lpszSection, LPCTSTR lpszEntry, 
		LPCTSTR lpszDefault = NULL);
	static CString GetCommonString(LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL);
	static CString GetModuleString(LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL);
	static CString GetSystemName();

	static BOOL IsSetupFileExist();

	CString m_strSetupFile;
	CString m_strModuleSection;

	static COEMSetup s_setup;
};

#endif // _OEMSETUP_H__
