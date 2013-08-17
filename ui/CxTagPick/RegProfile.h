// RegProfile.h: interface for the CRegProfile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _REGPROFILE_H__
#define _REGPROFILE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ATLASSERT 
	#define ASSERTATLMFC ATLASSERT
#else
	#define ASSERTATLMFC ASSERT
#endif

class CRegProfile 
{
public:
	CRegProfile();
	virtual ~CRegProfile();

	// Attributes
	LPCTSTR m_pszRegistryKey;   // used for registry entries
	LPCTSTR m_pszProfileName;   // default based on app name

public:
	void SetRegistryKey(LPCTSTR lpszRegistryKey, LPCTSTR lpszProfileName = NULL);

// Helper Operations - usually done in InitInstance
public:
	// Profile settings (to the app specific .INI file, or registry)
	UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault);
	BOOL WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);
	CString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,
		LPCTSTR lpszDefault = NULL);
	BOOL WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,
		LPCTSTR lpszValue);
	BOOL GetProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry,
		LPBYTE* ppData, UINT* pBytes);
	BOOL WriteProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry,
		LPBYTE pData, UINT nBytes);

// public for implementation access
public: 
	// helpers for registration
	HKEY GetSectionKey(LPCTSTR lpszSection);
	HKEY GetSetupRegistryKey();
};

#endif // _REGPROFILE_H__
