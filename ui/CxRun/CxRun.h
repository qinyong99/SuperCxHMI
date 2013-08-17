// CxRun.h : main header file for the CXRUN application
//

#if !defined(AFX_CXRUN_H__01EA0722_5D2B_4351_9A6A_2DC338AB3290__INCLUDED_)
#define AFX_CXRUN_H__01EA0722_5D2B_4351_9A6A_2DC338AB3290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "../../svr/CxDatSes/CxDatSes.h"

_COM_SMARTPTR_TYPEDEF(ICxDataSession, _uuidof(ICxDataSession));

/////////////////////////////////////////////////////////////////////////////
// CRunApp:
// See CxRun.cpp for the implementation of this class
//

const TCHAR szCxRunClass[] = _T("CxRunClass");

const TCHAR szIniRunSection[] = _T("Run");
const TCHAR szIniUseSettingEntry[] = _T("UseSetting");
const TCHAR szIniServerEntry[] = _T("Server");
const TCHAR szIniServer2Entry[] = _T("Server2");
const TCHAR szIniStartPictureEntry[] = _T("StartPicture");
const TCHAR szIniFullScreenEntry[] = _T("FullScreen");
const TCHAR szIniHighlightMouseObject[] = _T("HighlightMouseObject");
const TCHAR szIniPromptLocalServerModified[] = _T("PromptLocalServerModified");
const TCHAR szIniUseDefaultUserAndPassword[] = _T("UseDefaultUserAndPassword");
const TCHAR szIniDefaultUser[] = _T("DefaultUser");
const TCHAR szIniDefaultPassword[] = _T("DefaultPassword");

const TCHAR szCxSACService[] = _T("CxSAC");

class CServerNotifyWnd;
class CService;
interface ICxDataServer;

class CRunApp : public CWinApp
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CRunApp(LPCTSTR lpszAppName);

	CLSID GetAppCLSID();

	CString GetCurrentAppDirectory();
	CString GetSystemSetupFile();

	BOOL GetActiveProject(LPTSTR lpszProject);
	BOOL GetPictureFolder(LPTSTR lpszFolder);
	BOOL GetPojectFolder(LPCTSTR lpszEntry, LPTSTR lpszFolder);
	void GetQualityPojectFoler(LPTSTR lpszProject, LPTSTR lpszFolder);

	void LoadSetting();
	BOOL CreateDataSession(ICxDataSessionPtr& pDataSession, LPCTSTR lpszServer = NULL);
	BOOL GetRunSettingFromServer(LPCTSTR lpszServer = NULL, BOOL bCheckModified = FALSE);
	BOOL GetRunSettingFromServer(ICxDataSession* pDataSession, BOOL bCheckModified = FALSE);
	BOOL GetRunSettingFromServer(ICxDataServer* pServer);

	void CheckServerModified(ICxDataServer* pServer = NULL, BOOL bCheckModified = FALSE);

	void SwitchServer();
	void MonitorServer();

	CService* m_pServerService;

	CString	m_strServer;
	CString	m_strServer2;

	int		m_nUseSetting;
	CString	m_strSplashImage;
	CString	m_strStartPicture;
	BOOL	m_bFullScreen;
	BOOL	m_bDisableSwitchFullScreen;
	BOOL	m_bSystemMenu;
	CString	m_strCaption;
	BOOL	m_bMaximizeBox;
	BOOL	m_bMinimizeBox;
	BOOL	m_bMainMenu;
	BOOL	m_bThickFrame;
	CString m_strExitPassword;
	BOOL	m_bHighlightMouseObject;
	DWORD	m_dwDisableKeyMask;
	BOOL	m_bStopServerWhenExit;
	BOOL	m_bPromptLocalServerModified;
	BOOL	m_bUseDefaultUserAndPassword;
	CString m_strDefaultUser;
	CString m_strDefaultPassword;

	void ReadFreeLicense();
	
	BOOL m_bFreeLicense; // Ãâ·ÑÐí¿É
	SYSTEMTIME m_stLicenseTime;
	CString m_strLicenseUser;
	CString m_strLicenseCompany;
	
	ICxDataSessionPtr m_pDataSession;
	ICxDataSessionPtr m_pDataSession2;
	CServerNotifyWnd* m_pServerNotifyWnd;

	int m_cmdMainWindowShow;
	
	BOOL m_bLockTaskSwitching;
	BOOL m_bLockCtrlAltDel;

	BOOL m_bCanActiveMainFrame;

	BOOL m_bAbortApplication;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CRunApp)
	afx_msg void OnAppAbout();
	afx_msg void OnAppOption();
	afx_msg void OnToolMC();
	afx_msg void OnToolDev();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CRunApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CXRUN_H__01EA0722_5D2B_4351_9A6A_2DC338AB3290__INCLUDED_)
