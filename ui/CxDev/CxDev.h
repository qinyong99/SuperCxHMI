// CxDev.h : main header file for the CXDEV application
//

#ifndef __CXDEV_H__
#define __CXDEV_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#define CXMSG_GRID_SETTING_CHANGED WM_USER + 501

/////////////////////////////////////////////////////////////////////////////
// CDevApp:
// See CxDev.cpp for the implementation of this class
//
#include "UndoStack.h"
#include "Ini.h"
#include "AppSetting.h"

class CDevApp : public CWinApp
{
public:
	CDevApp(LPCTSTR lpszAppName = NULL);
	~CDevApp();

	CGenSetting m_settingGen;
	CFormSetting m_settingMainForm;
	CFormSetting m_settingSubForm;
	CObjectSetting m_settingObject;

	void NotifyDocumentActivate(DWORD dwNotify, DWORD dwData);
	void NotifyDocumentChanged(DWORD dwNotify, DWORD dwData = 0, BOOL bPost = FALSE);

//Attributes
	CMultiDocTemplate* m_pLayoutTemplate;
	CMultiDocTemplate* m_pScriptTemplate;
	CMultiDocTemplate* m_pCodeModuleTmpl;
	CMultiDocTemplate* m_pReportTemplate;

	ITypeLibPtr m_pHMITypeLib;
	ITypeLibPtr m_pComTypeLib;
	BOOL LoadExtTypeLib();

	static int m_nSelectChangedMsg;
	static int m_nDocumentChangeMsg;
	static int m_nDocumentActiveMsg;

	static int m_nProjectOpendMsg; 

	CList<HWND, HWND> m_listSelectNotify;
	CList<HWND, HWND> m_listFileOpenNotify;
	CList<HWND, HWND> m_listActivateMsgNotify;
	void NotifySelectChanged(BOOL bPost = FALSE, DWORD dwNotify = 0, BOOL bUpdateOnly = FALSE);

	int MakeSurePathExists(LPCTSTR lpszPath, 
			BOOL bFilenameIncluded = TRUE);
	BOOL GetCurrentProject(LPTSTR lpszProject);
	CString GetActiveProject();
	BOOL GetPictureFolder(LPTSTR lpszFolder);

	CString m_strCurrentPictureFolder;

	CString GetIniStartPictureFolder(BOOL bCache = TRUE);
	void UpdateIniStartPictureFolder(LPCTSTR lpszPathName);
	BOOL IsFirstRun();
	BOOL UpdateIniFirstRun();

	CString GetCurrentAppDirectory();
	CString GetSystemSetupFile();

	CIni m_iniOpcDataSource;

	CString m_strUserName;
	CString m_strUserCompany;
	CString m_strUserSerial;
	void GetUserInfo();

	void AlignToGrid(CPoint& point);

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDevApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL SaveAllModified();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CDevApp)
	afx_msg void OnFileNew();
	afx_msg void OnAppAbout();
	afx_msg void OnPictureOptions();
	afx_msg void OnPictureRun();
	afx_msg void OnPictureOpcserver();
	afx_msg void OnExecuteAppMC();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

extern CDevApp theApp;
extern DWORD theAppKey;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __CXDEV_H__
