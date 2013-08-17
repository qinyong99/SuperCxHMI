#if !defined(AFX_CXRUNCTRL_H__A707CD4A_2DED_4CEB_8EFC_6A4A3C38228D__INCLUDED_)
#define AFX_CXRUNCTRL_H__A707CD4A_2DED_4CEB_8EFC_6A4A3C38228D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CxRunCtrl.h : main header file for CXRUNCTRL.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols
#include "CacheFileManager.h"

/////////////////////////////////////////////////////////////////////////////
// CCxRunCtrlApp : See CxRunCtrl.cpp for implementation.

#define WM_REPLACE_PICTURE  WM_USER + 1

class CCxRunCtrlApp : public COleControlModule
{
public:
	CCxRunCtrlApp();

	BOOL InitInstance();
	int ExitInstance();

public:
	BOOL VerifySecurityArea(LPCTSTR lpszArea);

	VARIANT GetApplication(LPCTSTR pszItem); 
	void SetApplication(LPCTSTR pszItem, const VARIANT FAR& newValue); 

	void ClearApplicationVariables();

	BOOL m_bHighlightMouseObject;

	CString m_strLastPicturePath;
	CCacheFileManager* m_pCacheFileManager; 

	CString m_strUserName;
	CString m_strUserSecurityAreas;
	DWORD m_dwUserPrivilege;
	DWORD m_bAdministrator;

	// 应用程序对象，可以用来保存全局变量
	CTypedPtrMap<CMapStringToPtr, CString, VARIANT*> m_mapApplicationItems;
};

#define APPTIMELIMIT 1000 * 60 * 120

extern CCxRunCtrlApp theApp;
extern DWORD theAppKey;
extern DWORD theAppStartTime;

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CXRUNCTRL_H__A707CD4A_2DED_4CEB_8EFC_6A4A3C38228D__INCLUDED)
