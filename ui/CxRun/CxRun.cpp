// CxRun.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CxRun.h"

#include "MainFrm.h"
#include "RunDoc.h"
#include "RunView.h"

#include "PageGen.h"
#include "DocManager2.h"
#include "ServerNotifyWnd.h"

#include "Service.h"

#include "ServerPromptModifiedDlg.h"

#include "OleMessageFilter2.h"

#include "../../svr/CxDatSes/CxDatSes_i.c"

#include "../../svr/CxSac/CxSac.h"
#include "../../svr/CxSac/CxSac_i.c"

#include   "../WinLockDll/winlockdll.h"
#include "Splash.h"

#include "OEMSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunApp

BEGIN_MESSAGE_MAP(CRunApp, CWinApp)
	//{{AFX_MSG_MAP(CRunApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_OPTION, OnAppOption)
	ON_COMMAND(ID_TOOL_MC, OnToolMC)
	ON_COMMAND(ID_TOOL_DEV, OnToolDev)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunApp construction

CRunApp::CRunApp(LPCTSTR lpszAppName) :
	CWinApp(lpszAppName)
{
	m_bAbortApplication = FALSE;

	m_cmdMainWindowShow = SW_SHOW;

	m_nUseSetting = 0;
	m_bPromptLocalServerModified = FALSE;
	m_bStopServerWhenExit = FALSE;
	m_bFullScreen = FALSE;
	m_bHighlightMouseObject = TRUE;
	m_bUseDefaultUserAndPassword = FALSE;

	m_strCaption = _T("SuperCx 运行系统");
	m_bSystemMenu = TRUE;
	m_bMaximizeBox = TRUE;
	m_bMinimizeBox = TRUE;
	m_bMainMenu = TRUE;
	m_bThickFrame = TRUE;
		
	m_bDisableSwitchFullScreen = FALSE;
	m_dwDisableKeyMask = 0;
	
	m_bLockTaskSwitching = FALSE;
	m_bLockCtrlAltDel = FALSE;

	m_bFreeLicense = FALSE;

	if (m_pDocManager == NULL)
		m_pDocManager = new CDocManager2;

	m_bCanActiveMainFrame = TRUE;

	m_pServerNotifyWnd = NULL;

	m_pServerService = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRunApp object

CRunApp theApp(COEMSetup::GetSystemName() + _T(" 运行系统"));

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {EB197719-4C10-11D5-A46A-B043DCC54377}
static const CLSID clsid =
{ 0xeb197719, 0x4c10, 0x11d5, { 0xa4, 0x6a, 0xb0, 0x43, 0xdc, 0xc5, 0x43, 0x77 } };

/////////////////////////////////////////////////////////////////////////////
// CRunApp initialization

CLSID CRunApp::GetAppCLSID()
{
	return clsid;
}

BOOL CRunApp::InitInstance()
{
	HWND hWndFirst = FindWindow(szCxRunClass, NULL);
	if (hWndFirst != NULL)
	{
		SetForegroundWindow(hWndFirst);
		if (IsIconic(hWndFirst))
			::ShowWindow(hWndFirst, SW_RESTORE);
		return FALSE;
	}

	m_strCaption.Replace(_T("SuperCx"), COEMSetup::GetSystemName());

	CWinThread* pThread = AfxGetThread();
	
	ASSERT(m_pMessageFilter == NULL);
	// allocate and initialize default message filter
	if (pThread->m_pMessageFilter == NULL)
		pThread->m_pMessageFilter = (COleMessageFilter *)-1;
	
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		pThread->m_pMessageFilter = NULL;
		return FALSE;
	}

	if (pThread->m_pMessageFilter == (COleMessageFilter *)-1)
	{
		COleMessageFilter2* pFilter = new COleMessageFilter2;
		pFilter->Register2();
		pThread->m_pMessageFilter = pFilter;
		ASSERT(AfxOleGetMessageFilter() != NULL);
	}

	COleMessageFilter* pFilter = AfxOleGetMessageFilter();
	pFilter->SetMessagePendingDelay(48000);
//	pFilter->EnableBusyDialog(FALSE);
//	pFilter->EnableNotRespondingDialog(FALSE);

#ifndef _DEBUG
	// Turn security off so that everyone has access to us
	CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_NONE,
		RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
#endif

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(COEMSetup::GetSystemName());

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	LoadSetting();

	if (m_bAbortApplication)
		return FALSE;

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CRunDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CRunView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
//	EnableShellOpen();
//	RegisterShellFileTypes(TRUE);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_server.ConnectTemplate(clsid, pDocTemplate, TRUE);
		// Note: SDI applications register server objects only if /Embedding
		//   or /Automation is present on the command line.

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	//不创建新文档
	if ((!m_strStartPicture.IsEmpty() || m_lpCmdLine[0] != '\0') && cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash && m_bFreeLicense);
	
	// Check to see if launched as OLE server
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Register all OLE server (factories) as running.  This enables the
		//  OLE libraries to create objects from other applications.
		COleTemplateServer::RegisterAll();

		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
	COleObjectFactory::UpdateRegistryAll();

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_bCanActiveMainFrame = FALSE;

	//如果没有指定打开文件，用系统设置的文件
	if (m_lpCmdLine[0] == '\0')
	{
 		if (!m_strStartPicture.IsEmpty())
			OpenDocumentFile(m_strStartPicture);
	}
	//打开命令行指定的文件
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNothing)
	{
		//	ASSERT(cmdInfo.m_nShellCommand == CCommandLineInfo::FileDDE);
		OpenDocumentFile(m_lpCmdLine);
	}

	if (m_bFullScreen)
		((CMainFrame *)m_pMainWnd)->FullScreenModeOn();

	m_bCanActiveMainFrame = TRUE;

	((CMainFrame *)m_pMainWnd)->ActivateFrame(m_cmdMainWindowShow);

	// The one and only window has been initialized, so show and update it.
  	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

// 	// Enable drag/drop open
 	m_pMainWnd->DragAcceptFiles();

	if ((m_dwDisableKeyMask & DISABLE_KEY_ALL) != 0)
	{
		if (TaskSwitching_Enable_Disable(FALSE, m_dwDisableKeyMask))
			m_bLockTaskSwitching = TRUE;
	}
	
	if ((m_dwDisableKeyMask & DISABLE_KEY_CTRL_SHIFT_ESC) != 0)
	{
		if (CtrlAltDel_Enable_Disable(FALSE))
			m_bLockCtrlAltDel = TRUE;
	}

	if (!CSplashWnd::IsValidBitmap())
		AfxPostQuitMessage(0);

	return TRUE;
}

//////////////////////////////////////////////////////////////////
//功能：返回本程序所在目录
//////////////////////////////////////////////////////////////////
CString CRunApp::GetCurrentAppDirectory()
{
	TCHAR szBuff[_MAX_PATH];
	VERIFY(::GetModuleFileName(m_hInstance, szBuff, _MAX_PATH));
	
	LPTSTR lpszTemp = (LPTSTR)szBuff;
	for (LPCTSTR lpsz = szBuff; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
	{
		// remember last directory/drive separator
		if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
			lpszTemp = (LPTSTR)_tcsinc(lpsz);
	}
	
	*lpszTemp = 0;
	
	return CString(szBuff);
}

CString CRunApp::GetSystemSetupFile()
{
	CString strFile = GetCurrentAppDirectory();
	strFile += _T("System.ini");
	
	return strFile;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CRunApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CRunApp message handlers

void CRunApp::OnAppOption() 
{
	CPageGen pageGen;
	CPropertySheet sheet(_T("选项"), m_pMainWnd);

	sheet.AddPage(&pageGen);

	sheet.m_psh.dwFlags &= ~PSH_HASHELP;
	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.DoModal();
}

void EnsureLocalServerRunning(LPCTSTR lpszServer)
{
	if (!IsLocalComputer(lpszServer))
		return;

	CService serviceSAC(szCxSACService, CLSID_CxDataServer);
	if (serviceSAC.Open() == 0)
	{
		if (serviceSAC.GetCurrentState() == SERVICE_PAUSED ||
			serviceSAC.GetCurrentState() == SERVICE_STOPPED)
		{
			serviceSAC.Start();
		}
		
		// 等待5秒直到服务刷新完毕
		DWORD dwWaitTimes = 100;
		while (dwWaitTimes--)
		{
			if (serviceSAC.GetCurrentState() == SERVICE_RUNNING)
				break;
			Sleep(50);
		}
	}
}

void CRunApp::LoadSetting()
{
	TCHAR szBuf[256];
	CString strIniFile = GetSystemSetupFile();

	m_bUseDefaultUserAndPassword = ::GetPrivateProfileInt(szIniRunSection, szIniUseDefaultUserAndPassword, 0, strIniFile) != 0;
	
	szBuf[0] = _T('\0');
	::GetPrivateProfileString(szIniRunSection, szIniDefaultUser, _T(""), szBuf, 256, strIniFile);
	m_strDefaultUser = szBuf;
	
	szBuf[0] = _T('\0');
	::GetPrivateProfileString(szIniRunSection, szIniDefaultPassword, _T(""), szBuf, 256, strIniFile);
	m_strDefaultPassword = szBuf;
	
	m_nUseSetting = ::GetPrivateProfileInt(szIniRunSection, szIniUseSettingEntry, 0, strIniFile);

	if (m_nUseSetting == 0)
	{
		m_bPromptLocalServerModified = ::GetPrivateProfileInt(szIniRunSection, szIniPromptLocalServerModified, 1, strIniFile) != 0;
		
		if (COEMSetup::GetLocalServerUseSACProxy())
		{	
			m_strServer = "localhost";
			EnsureLocalServerRunning(m_strServer);
		
			if (CreateDataSession(m_pDataSession, m_strServer))
				GetRunSettingFromServer(m_pDataSession);
		}
		else
		{
			GetRunSettingFromServer((LPCTSTR)NULL, m_bPromptLocalServerModified);
		}
	}
	else if (m_nUseSetting == 1)
	{
		szBuf[0] = _T('\0');
		::GetPrivateProfileString(szIniRunSection, szIniServerEntry, _T(""), szBuf, 256, strIniFile);
		m_strServer = szBuf;

		szBuf[0] = _T('\0');
		::GetPrivateProfileString(szIniRunSection, szIniServer2Entry, _T(""), szBuf, 256, strIniFile);
		m_strServer2 = szBuf;

		EnsureLocalServerRunning(m_strServer);

		BOOL b = FALSE;
		if (CreateDataSession(m_pDataSession, m_strServer))
			b = GetRunSettingFromServer(m_pDataSession);
	
		if (!b)
		{
			m_pDataSession.Release();

			EnsureLocalServerRunning(m_strServer2);

			if (CreateDataSession(m_pDataSession2, m_strServer2))
				b = GetRunSettingFromServer(m_pDataSession2);

			if (!b)
				m_pDataSession2.Release();
		}

		MonitorServer();

		// 直接连接站点
		m_strStartPicture = _T("Server://../");
	}
	else if (m_nUseSetting == 2)
	{
		szBuf[0] = _T('\0');
		::GetPrivateProfileString(szIniRunSection, szIniStartPictureEntry, _T(""), szBuf, 256, strIniFile);
		m_strStartPicture = szBuf;
		
		m_bFullScreen = ::GetPrivateProfileInt(szIniRunSection, szIniFullScreenEntry, 0, strIniFile) != 0;
		m_bHighlightMouseObject = ::GetPrivateProfileInt(szIniRunSection, szIniHighlightMouseObject, 1, strIniFile) != 0;
	}
}

BOOL CRunApp::CreateDataSession(ICxDataSessionPtr& pDataSession, LPCTSTR lpszServer)
{
	USES_CONVERSION;
	HRESULT hr;
	
	hr = pDataSession.CreateInstance(CLSID_CxDataSession/*clsid*/, NULL,
		CLSCTX_INPROC_SERVER);
	if (FAILED(hr))
		return FALSE;
	
	if (lpszServer != NULL)
		pDataSession->SetThisNode(T2CW(lpszServer), NULL, FALSE);

	if (m_bUseDefaultUserAndPassword)
		pDataSession->SetDefaultUserPassword(T2CW(m_strDefaultUser), T2CW(m_strExitPassword));

	return TRUE;
}

BOOL CRunApp::GetRunSettingFromServer(LPCTSTR lpszServer, BOOL bCheckModified)
{
	ICxDataSessionPtr pDataSession;
	if (CreateDataSession(pDataSession, lpszServer))
	{
		pDataSession->SetAutoLogin(FALSE, TRUE);
//		pDataSession->SetSACProxy(COEMSetup::GetLocalServerUseSACProxy(), TRUE, 8081);
		return GetRunSettingFromServer(pDataSession, bCheckModified);
	}

	return FALSE;
}

void GetThisNodeName(ICxDataSession* pDataSession, LPTSTR szName)
{
	USES_CONVERSION;
	
	LPWSTR pszNode = NULL;
	pDataSession->GetThisNodeName(&pszNode);
	if (pszNode != NULL)
		strcpy(szName, OLE2T(pszNode));
	else
		szName[0] = '\0';
	if (pszNode != NULL)
		CoTaskMemFree(pszNode);
}

void CRunApp::CheckServerModified(ICxDataServer* pServer, BOOL bCheckModified)
{
	CComPtr<ICxDataServer> spDataServer;
	HRESULT hr;

	if (pServer == NULL)
	{
		ICxDataSessionPtr pDataSession;
		if (!CreateDataSession(pDataSession))
			return;

		USES_CONVERSION;
		TCHAR szThisNode[128];
		GetThisNodeName(pDataSession, szThisNode);
		
		hr = pDataSession->GetNodeServer(T2OLE(szThisNode), 
			IID_ICxDataServer, (IUnknown **)&spDataServer);
		if (FAILED(hr))
			return;

		pServer = spDataServer;
	}

	while (1)
	{
		DWORD dwLockStatus = -1;
		DWORD dwEncryptData = 0;
		hr = pServer->VerifySoftLock(FALSE, &dwLockStatus, &dwEncryptData);
		if (dwLockStatus == 2)
			m_bFreeLicense = TRUE;

		if (dwLockStatus == -1 || dwLockStatus == -2)
		{
			CString strText;
			if (dwLockStatus == -1)
				strText = _T("没有检测到加密锁，请安装好加密锁后重试，单击忽略进入演示模式！");
			else
				strText = _T("加密锁支持的点数不够，请重新安装好加密锁后重试，单击忽略进入演示模式！");

			int nRet = MessageBox(NULL, strText, _T("错误"), MB_ICONEXCLAMATION | MB_ABORTRETRYIGNORE);
			if (nRet == IDRETRY)
				continue;

			if (nRet == IDABORT)
				m_bAbortApplication = TRUE;
//				AfxPostQuitMessage(0);
//				ExitProcess(0);
			else
				pServer->VerifySoftLock(TRUE, &dwLockStatus, &dwEncryptData);
		}

		break;
	}

	if (m_bFreeLicense)
	{
		ReadFreeLicense();
//		ASSERT(m_bFreeLicense == TRUE);
	}

	if (bCheckModified && pServer->IsModified() == S_OK)
	{
		if (m_pServerService == NULL)
		{
			m_pServerService = new CService(szCxSACService, CLSID_CxDataServer);
			if (m_pServerService->Open() != 0)
				return;
		}

		CServerPromptModifiedDlg dlg;
		int nRet = dlg.DoModal();

		if (dlg.m_bNotPrompt)
		{
			m_bPromptLocalServerModified = FALSE;

			CString strIniFile = GetSystemSetupFile();
	
			CString str;
			str.Format(_T("%d"), m_bPromptLocalServerModified);
			::WritePrivateProfileString(szIniRunSection, szIniPromptLocalServerModified, str, strIniFile);

		}

		if (nRet == IDOK)
		{
			m_pServerService->Refresh();

			// 等待5秒直到服务刷新完毕
			DWORD dwWaitTimes = 100;
			while (dwWaitTimes--)
			{
				if (m_pServerService->GetCurrentState() == SERVICE_RUNNING)
					break;
				Sleep(50);
			}
		}
	}
}

BOOL CRunApp::GetRunSettingFromServer(ICxDataSession* pDataSession, BOOL bCheckModified)
{
	USES_CONVERSION;

	TCHAR szThisNode[128];
	GetThisNodeName(pDataSession, szThisNode);
	
	CComPtr<ICxDataServer> spDataServer;
	HRESULT hr = pDataSession->GetNodeServer(T2OLE(szThisNode), 
		IID_ICxDataServer, (IUnknown **)&spDataServer);
	if (FAILED(hr))
		return FALSE;

	CheckServerModified(spDataServer, bCheckModified);
	
	return GetRunSettingFromServer(spDataServer);
}

BOOL CRunApp::GetRunSettingFromServer(ICxDataServer* pServer)
{
	CXRUNPICSETTING setting;
	HRESULT hr = pServer->GetRunPictureSetting(&setting);
	if (hr == S_OK)
	{
		if (setting.szSplashImage != NULL)
		{
			m_strSplashImage = setting.szSplashImage;
			CoTaskMemFree(setting.szSplashImage);
		}
		
		if (setting.szStartPicture != NULL)
		{
			m_strStartPicture = setting.szStartPicture;
			CoTaskMemFree(setting.szStartPicture);
		}
		
		m_bFullScreen = setting.bFullScreen;
		m_bDisableSwitchFullScreen = setting.bDisableSwitchFullScreen;
		m_bSystemMenu = setting.bSystemMenu;
		m_bStopServerWhenExit = setting.bStopServerWhenExit;
		
		if (setting.szCaption != NULL)
		{
			m_strCaption = setting.szCaption;
			CoTaskMemFree(setting.szCaption);
		}
		
		m_bMaximizeBox = setting.bMaximizeBox;
		m_bMinimizeBox = setting.bMinimizeBox;
		m_bMainMenu = setting.bMainMenu;
		m_bThickFrame = setting.bThickFrame;
		
		if (setting.szExitPassword != NULL)
		{
			m_strExitPassword = setting.szExitPassword;
			CoTaskMemFree(setting.szExitPassword);
		}
		
		m_bHighlightMouseObject = setting.bHighlightMouseObject;
		m_dwDisableKeyMask = setting.dwDisableKeyMask;
		
		return TRUE;
	}
	
	return FALSE;
}

BOOL CRunApp::GetActiveProject(LPTSTR lpszProject)
{
	CString strFile = GetCurrentAppDirectory();
	CString strDefault = strFile;
	
	strFile += _T("System.ini");
	strDefault += _T("Demo\\Demo.scp");
	
	DWORD dw = ::GetPrivateProfileString(_T("Project"), _T("ActiveProject"),
		strDefault, lpszProject, MAX_PATH, strFile);
	ASSERT(dw < 4095);
	
	if (_access(lpszProject, 0) == 0)
		return TRUE;
	
	return FALSE;
}

BOOL CRunApp::GetPojectFolder(LPCTSTR lpszEntry, LPTSTR lpszFolder)
{
	TCHAR szProject[4096];
	GetActiveProject(szProject);
	
	DWORD dw = ::GetPrivateProfileString(_T("Path"), lpszEntry,
		NULL, lpszFolder, 4096, szProject);
	ASSERT(dw < 4096);
	
	GetQualityPojectFoler(szProject, lpszFolder);
	
	return TRUE;
}

void CRunApp::GetQualityPojectFoler(LPTSTR lpszProject, LPTSTR lpszFolder)
{
	LPTSTR lpszFind = strstr(lpszFolder, _T("<工程路径>"));
	if (lpszFind != NULL)
	{
		TCHAR szT[4096];
		lstrcpy(szT, lpszFolder);
		
		// always capture the complete file name including extension (if present)
		LPTSTR lpszTemp = (LPTSTR)lpszProject;
		for (LPCTSTR lpsz = lpszProject; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
		{
			// remember last directory/drive separator
			if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
				lpszTemp = (LPTSTR)_tcsinc(lpsz);
		}
		
		lstrcpyn(lpszFolder, lpszProject, lpszTemp - lpszProject);
		lstrcat(lpszFolder, szT + 10);
	}
}

BOOL CRunApp::GetPictureFolder(LPTSTR lpszFolder)
{
	return GetPojectFolder(_T("Picture"), lpszFolder);
}

int CRunApp::ExitInstance() 
{
	if (m_bStopServerWhenExit)
	{
		if (m_pServerService == NULL)
		{
			m_pServerService = new CService(szCxSACService, CLSID_CxDataServer);
			if (m_pServerService->Open() == 0)
				m_pServerService->Stop();
		}
	}
		
	if (m_pServerService != NULL)
	{
		delete m_pServerService;
		m_pServerService = NULL;
	}

	if (m_pServerNotifyWnd != NULL)
	{
		m_pServerNotifyWnd->DestroyWindow();
		delete m_pServerNotifyWnd;
		m_pServerNotifyWnd = NULL;
	}

	// 如果不在此释放，则 Release 版本退出报错
	if (m_pDataSession != NULL)
		m_pDataSession.Release();
	if (m_pDataSession2 != NULL)
		m_pDataSession2.Release();

	if (m_bLockTaskSwitching)
		TaskSwitching_Enable_Disable(TRUE, 0);
	
	if (m_bLockCtrlAltDel)
		CtrlAltDel_Enable_Disable(TRUE);
	
	return CWinApp::ExitInstance();
}

void CRunApp::SwitchServer()
{
	BOOL b = FALSE;
	if (m_pDataSession != NULL)
	{
		m_pDataSession->SetThisNode(NULL, NULL, FALSE);
		m_pDataSession.Release();

		ASSERT(m_pDataSession2 == NULL);
		if (CreateDataSession(m_pDataSession2, m_strServer2))
			b = GetRunSettingFromServer(m_pDataSession2);
	}
	else if (m_pDataSession2 != NULL)
	{
		m_pDataSession2->SetThisNode(NULL, NULL, FALSE);
		m_pDataSession2.Release();
		
		ASSERT(m_pDataSession == NULL);
		if (CreateDataSession(m_pDataSession, m_strServer))
			b = GetRunSettingFromServer(m_pDataSession);
	}

	if (b)
	{
		CRunView* pView = (CRunView *)((CMainFrame *)m_pMainWnd)->GetActiveView();
		if (pView != NULL)
			pView->SetServer(_T(""));
	}
	
	// 直接连接站点
//	if (b)
//	{
//		m_strStartPicture = _T("Server://ThisNode/");
//		OpenDocumentFile(m_strStartPicture);
//	}

	MonitorServer();
}

void CRunApp::MonitorServer()
{
	USES_CONVERSION;

	if (m_pServerNotifyWnd == NULL)
	{
		m_pServerNotifyWnd = new CServerNotifyWnd;
		if (!m_pServerNotifyWnd->CreateEx(0, AfxRegisterWndClass(0),
			_T("Socket Notification Sink"),
			WS_OVERLAPPED, 0, 0, 0, 0, NULL, NULL))
		{
			TRACE0("Warning: unable to create socket notify window!\n");
			AfxThrowResourceException();
		}
		ASSERT(m_pServerNotifyWnd->m_hWnd != NULL);
	}

//	if (m_pDataSession != NULL)
//		m_pDataSession->StartServerMonitor(T2CW(m_strServer), m_pServerNotifyWnd->m_hWnd, WM_SERVER_DISCONNECTED);
//	else if (m_pDataSession2 != NULL)
//		m_pDataSession2->StartServerMonitor(T2CW(m_strServer2), m_pServerNotifyWnd->m_hWnd, WM_SERVER_DISCONNECTED);
}

const TCHAR szCxMCClass[] = _T("CxMCClass");

void CRunApp::OnToolMC() 
{
	BOOL bOk;
	
	HWND hWndFirst = FindWindow(szCxMCClass, NULL);
	if (hWndFirst != NULL)
	{
		bOk = SetForegroundWindow(hWndFirst);
		// 		ATLASSERT(bOk == TRUE);
		return;
	}
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	// Execute the command with a call to the CreateProcess API call.
	memset(&si,0,sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	
	CString strExeFile = GetCurrentAppDirectory();
	strExeFile += _T("CxMC.exe");
	
	bOk = CreateProcess(strExeFile, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	// 	ATLASSERT(bOk == TRUE);
	
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);	
}

const TCHAR szCxDevClass[] = _T("CxDevClass");

void CRunApp::OnToolDev() 
{
	BOOL bOk;
	
	HWND hWndFirst = FindWindow(szCxDevClass, NULL);
	if (hWndFirst != NULL)
	{
		bOk = SetForegroundWindow(hWndFirst);
		// 		ATLASSERT(bOk == TRUE);
		return;
	}
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	// Execute the command with a call to the CreateProcess API call.
	memset(&si,0,sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	
	CString strExeFile = GetCurrentAppDirectory();
	strExeFile += _T("CxDev.exe");
	
	bOk = CreateProcess(strExeFile, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	// 	ATLASSERT(bOk == TRUE);
	
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

BOOL CRunApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

const TCHAR szVerInfo[] = _T("supercx 3.0 free 64 point general version; ");
const TCHAR szTime[] = _T("time:");
const TCHAR szUser[] = _T("license to:");
const TCHAR szCompany[] = _T("company:");

void CRunApp::ReadFreeLicense()
{
	m_bFreeLicense = FALSE;

	CString strLicenseFile = GetCurrentAppDirectory();
#ifdef _DEBUG
	strLicenseFile = _T("C:\\Program Files\\SuperCx\\");
#endif
	strLicenseFile += _T("license.txt");

	HANDLE hFile = ::CreateFile(
		strLicenseFile,
		GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	
	DWORD nSize = GetFileSize(hFile, NULL);
	
	// 分配缓冲区
	LPSTR pBuffer = (LPSTR)alloca(nSize + 1);
	pBuffer[nSize] = '\0';

	DWORD nBytesRead = 0;
	BOOL bSuccess = ::ReadFile(hFile, pBuffer, nSize, &nBytesRead, NULL);
	CloseHandle(hFile);

	if (nBytesRead != nSize)
		return;

	// 解密字符串
	LPSTR p = pBuffer + 256;
	LPSTR pEnd = p + nBytesRead - 258;

	for (int i = 0; p < pEnd; i++)
	{
		if (IsDBCSLeadByte(*p))
			p++;

		*p = pBuffer[i] ^ *p;
					
		p++;
	}

	p = pBuffer + 256;

	// 版本
	if (strncmp(p, szVerInfo, strlen(szVerInfo)) != 0)
		return;
	p += strlen(szVerInfo);

	// 授权时间
	if (strncmp(p, szTime, strlen(szTime)) != 0)
		return;
	p += strlen(szTime);

	memset(&m_stLicenseTime, sizeof(SYSTEMTIME), 0);

	m_stLicenseTime.wYear = (WORD)strtol(p, &p, 10);
	if (*p != '-')
		return;
	p++;

	m_stLicenseTime.wMonth = (WORD)strtol(p, &p, 10);
	if (*p != '-')
		return;
	p++;

	m_stLicenseTime.wDay = (WORD)strtol(p, &p, 10);
	if (*p != ';')
		return;
	p++;

	if (*p != ' ')
		return;
	p++;

	// 用户
	if (strncmp(p, szUser, strlen(szUser)) != 0)
		return;
	p += strlen(szUser);

	LPSTR p1 = p;
	int nLen = strlen(szCompany);
	while (*p1 != '\0')
	{
		if (strncmp(p1, szCompany, nLen) == 0)
			break;
		p1++;
	}

	if (*p1 == '\0')
		return;

	if (*(p1 - 2) != ';' && *(p1 - 1) != ' ')
		return;

	m_strLicenseUser = CComBSTR(p1 - p - 1, p);

	p = p1 + nLen;
	p1 = pBuffer + nSize - 1;

	while (*p1 == ';' || *p1 == '\r' || *p1 == '\n')
		p1--;

	m_strLicenseCompany = CComBSTR(p1 - p + 2, p);

	m_bFreeLicense = TRUE;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strText;
	GetWindowText(strText);
	strText.Replace(_T("SuperCx"), COEMSetup::GetSystemName());
	SetWindowText(strText);

	GetDlgItemText(IDC_APP_NAME, strText);
	strText.Replace(_T("SuperCx"), COEMSetup::GetSystemName());
	SetDlgItemText(IDC_APP_NAME, strText);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
