// CxDev.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DevInc.h"

#include "PageGen.h"
#include "PageMainForm.h"
#include "PageSubForm.h"
#include "PageObject.h"
#include "FileNewDlg.h"

#include "OPCDataSource.h"

#include "hyperlink.h"

#include <direct.h>
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDevApp

int CDevApp::m_nSelectChangedMsg = RegisterWindowMessage(_T("CxDevSelectChanged"));
int CDevApp::m_nDocumentChangeMsg = RegisterWindowMessage(_T("CxDevDocumentChange"));
//通知工具栏等当前击活的窗体或其它窗口改变,wparam为击活对能者多对象的类弄,lparam为对象数据
//wparam可以为 enum CDevApp::ActivateType, lparam与类型关
int CDevApp::m_nDocumentActiveMsg = RegisterWindowMessage(_T("CxDevDocumentActivate"));
int CDevApp::m_nProjectOpendMsg = RegisterWindowMessage(_T("SuperCxProjectOpend"));

BEGIN_MESSAGE_MAP(CDevApp, CWinApp)
	//{{AFX_MSG_MAP(CDevApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_PICTURE_OPCSERVER, OnPictureOpcserver)
	ON_COMMAND(ID_EXECUTE_APPMC, OnExecuteAppMC)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_PICTURE_RUN, OnPictureRun)
	ON_COMMAND(ID_PICTURE_OPTIONS, OnPictureOptions)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDevApp construction

CDevApp::CDevApp(LPCTSTR lpszAppName) :
	CWinApp(lpszAppName),
	m_settingGen(_T("General")),
	m_settingMainForm(_T("MainForm")),
	m_settingSubForm(_T("SubForm")),
	m_settingObject(_T("Object"))
{
	theAppKey = _SUPERCX_VER & 0xffff0000;

	CString strFileName = GetCurrentAppDirectory();
	strFileName += _T("OPCDataSource.ini");
	m_iniOpcDataSource.SetFileName(strFileName);
}

CDevApp::~CDevApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDevApp object

CDevApp theApp(COEMSetup::GetSystemName() + " 画面开发环境");
DWORD theAppKey;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {8CCCD9F3-E47C-11D4-8C60-5254AB20ED22}
static const CLSID clsid =
{ 0x8cccd9f3, 0xe47c, 0x11d4, { 0x8c, 0x60, 0x52, 0x54, 0xab, 0x20, 0xed, 0x22 } };

/////////////////////////////////////////////////////////////////////////////
// CDevApp initialization
using namespace Gdiplus;
static ULONG_PTR gdiplusToken = NULL;
static GdiplusStartupInput gdiStartup;

BOOL CDevApp::InitInstance()
{
	if (m_pDocManager == NULL)
		m_pDocManager = new CDocManager2;

// 	m_nCmdShow = SW_SHOWMAXIMIZED;

	GetUserInfo();

	// CG: The following block was added by the Splash Screen component.z
\
//	{
\
		CCommandLineInfo cmdInfo;
\
		ParseCommandLine(cmdInfo);
\

\
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
\
//	}
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	COleMessageFilter* pFilter = AfxOleGetMessageFilter();
	pFilter->SetMessagePendingDelay(15000);
//	pFilter->EnableBusyDialog(FALSE);
//	pFilter->EnableNotRespondingDialog(FALSE);

	// Turn security off so that everyone has access to us
	CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_NONE,
		RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

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


	m_settingGen.LoadSettings();
	m_settingMainForm.LoadSettings();
	m_settingSubForm.LoadSettings();
	m_settingObject.LoadSettings();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_pLayoutTemplate = new CMultiDocTemplate(
		IDR_LAYOUTFRAME,
		RUNTIME_CLASS(CFormDoc),
		RUNTIME_CLASS(CLayoutFrame), // custom MDI child frame
		RUNTIME_CLASS(CLayoutView));
	m_pLayoutTemplate->SetContainerInfo(IDR_CXDEVTYPE_CNTR_IP);
	AddDocTemplate(m_pLayoutTemplate);

	m_pScriptTemplate = new CMultiDocTemplate(
		IDR_SCRIPTFRAME,
		RUNTIME_CLASS(CFormDoc),
		RUNTIME_CLASS(CScriptFrame),
		RUNTIME_CLASS(CScriptView));
	AddDocTemplate(m_pScriptTemplate);

	m_pReportTemplate = new CMultiDocTemplate(
		IDR_REPORTFRAME,
		RUNTIME_CLASS(CReportDoc),
		RUNTIME_CLASS(CLayoutFrame),
		RUNTIME_CLASS(CReportView));
	AddDocTemplate(m_pReportTemplate);

	//	m_pCodeModuleTmpl = new CMultiDocTemplate(
//		IDR_LAYOUTFRAME,
//		RUNTIME_CLASS(CCodeModuleDoc),
//		RUNTIME_CLASS(CScriptFrame),
//		RUNTIME_CLASS(CCodeView));
//	AddDocTemplate(m_pCodeModuleTmpl);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_server.ConnectTemplate(clsid, m_pLayoutTemplate, FALSE);

	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line.

	if (!LoadExtTypeLib())
		AfxMessageBox(_T("导入类型库出错！"));

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
//	CCommandLineInfo cmdInfo;
//	ParseCommandLine(cmdInfo);

	// Check to see if launched as OLE server
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
	COleObjectFactory::UpdateRegistryAll();

	GdiplusStartup(&gdiplusToken, &gdiStartup, NULL);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	NotifySelectChanged();

	//对工具初始化
	CDevTool::InitTools();

	CDevObj::c_imageGeneralHandle.Create(IDB_HANDLE_GENERAL, 10, 1, RGB(255, 0, 0));
	CDevObj::c_imageRotateHandle.Create(IDB_HANDLE_ROTATE, 15, 1, RGB(255, 255, 255));


//////////////////////////////////////////////////////////////////////////////

// #define _SOFT_PROTECT

#ifdef _SOFT_PROTECT
	
	SYSTEMTIME st;
	GetLocalTime(&st);

	CString strExpiredTime;
	GetPrivateProfileString(_T("Expired"), _T("Time"),
		_T(""), strExpiredTime.GetBuffer(32), 32, _T("C:\\WINNT\\cxfreever.ini"));
	strExpiredTime.ReleaseBuffer();
	
	SYSTEMTIME stLast;
	memcpy(&stLast, &st, sizeof(SYSTEMTIME));
	if (!strExpiredTime.IsEmpty())
	{
		sscanf(strExpiredTime, _T("%04d-%02d-%2d"), 
			&stLast.wYear, &stLast.wMonth, &stLast.wDay);
	}

	if (strExpiredTime.IsEmpty() ||
		st.wYear > stLast.wYear || 
		(st.wYear == stLast.wYear && st.wMonth > stLast.wMonth) || 
		(st.wYear == stLast.wYear && st.wMonth == stLast.wMonth && st.wDay > stLast.wDay))
	{
		TCHAR szBuf[32];
		sprintf(szBuf, _T("%04d-%02d-%2d"), 
			st.wYear, st.wMonth, st.wDay);
		WritePrivateProfileString(_T("Expired"), _T("Time"), 
			szBuf, _T("C:\\WINNT\\cxfreever.ini"));
	}
	else
	{
		memcpy(&st, &stLast, sizeof(SYSTEMTIME));
	}

	SYSTEMTIME stExpired;
	memset(&stExpired, 0, sizeof(SYSTEMTIME));
	stExpired.wYear = 2007;
	stExpired.wMonth = 12;
	stExpired.wDay = 31;

	if (st.wYear > stExpired.wYear || 
		(st.wYear == stExpired.wYear && st.wMonth > stExpired.wMonth) || 
		(st.wYear == stExpired.wYear && st.wMonth == stExpired.wMonth && st.wDay > stExpired.wDay))
	{ 
		PostThreadMessage(WM_QUIT, 0, NULL);
	}	

//	srand((unsigned)time(NULL));
//	int nDay = rand() % 30 + 1;
//
//	CTime time(2005, 11, 30, 0, 0, 0);
//	if (CTime::GetCurrentTime() > time)
//	{ 
//		PostThreadMessage(WM_QUIT, 0, NULL);
//	}

#endif

//////////////////////////////////////////////////////////////////////////////

	return TRUE;
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
	CHyperLink	m_www;
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
	DDX_Control(pDX, IDC_WWW, m_www);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Set HyperLink for Home Page
	CString strURL;
	strURL.LoadString(IDS_HOMEPAGEADDR);
	strURL.Replace(_T("supercx"), COEMSetup::GetSystemName());
	strURL.MakeLower();
	m_www.SetURL(strURL);
	m_www.SetUnderline(TRUE);
	m_www.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HANDCUR));

	CString strText;
	GetWindowText(strText);
	strText.Replace(_T("SuperCx"), COEMSetup::GetSystemName());
	SetWindowText(strText);

	GetDlgItemText(IDC_APP_NAME, strText);
	strText.Replace(_T("SuperCx"), COEMSetup::GetSystemName());
	SetDlgItemText(IDC_APP_NAME, strText);

	GetDlgItemText(IDC_WWW, strText);
	strText.Replace(_T("http://www.supercx.com"), COEMSetup::GetCommonString(_T("WebSite"), _T("http://www.supercx.com")));
	SetDlgItemText(IDC_WWW, strText);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// App command to run the dialog
void CDevApp::OnAppAbout()
{
	if (COEMSetup::IsSetupFileExist())
	{
		CAboutDlg aboutDlg;
		aboutDlg.DoModal();
	}
	else
	{
		CSplashWnd::ShowSplashScreen(m_pMainWnd, FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDevApp message handlers

void CDevApp::OnFileNew()
{
//	CFileNewDlg dlg;
//	dlg.m_nWidth = m_settingMainForm.m_nWidth;
//	dlg.m_nHeight = m_settingMainForm.m_nHeight;
//
//	if (dlg.DoModal() == IDOK)
//	{
//		m_settingMainForm.m_nWidth = dlg.m_nWidth;
//		m_settingMainForm.m_nHeight = dlg.m_nHeight;
//
//		CDevDoc::CreateNewDocument();
//	}

	CDevDoc::CreateNewDocument();

	// By default, the framework does not create an actual file for
	// a new document until the user does a File Save As.  But ChkBook
	// requires a file as soon as the user does a File New, because
	// ChkBook updates the file on a per transaction basis.  Upon File
	// New, we prompt the user with a File Open dialog, in which the
	// user specifies a new file (or if she changes her mind, she
	// can specify an existing check book file).  We call the same
	// CWinApp::DoPromptFileName that CWinApp::OnFileOpen calls.
	// But we replace the OFN_FILEMUSTEXIST flag with
	// OFN_CREATEPROMPT.

//	CWinApp::OnFileNew();

/*	CString strNewFileName;

	if (!(DoPromptFileName(strNewFileName, IDS_NEW_PICTURE,
			OFN_HIDEREADONLY | OFN_CREATEPROMPT, TRUE, NULL)))
		return;

	// If file doesn't already exist, then create it.
	CFile file;
	CFileStatus status;
	if (!file.GetStatus(strNewFileName, status))
	{
		if (!file.Open(strNewFileName, CFile::modeCreate))
		{
			CString strMessage;
			AfxFormatString1(strMessage, IDS_CANNOT_CREATE_PICTURE,
				strNewFileName);
			AfxMessageBox(strMessage);
			return;
		}
		file.Close();
	}

	// Open the file now that it has been created.
	OpenDocumentFile(strNewFileName);
*/
}

int CDevApp::ExitInstance() 
{
	//对工具反初始化
	CDevTool::DeInitTools();
//	CDevDoc::RemoveAll();

	m_settingGen.SaveSettings();
	m_settingMainForm.SaveSettings();
	m_settingSubForm.SaveSettings();
	m_settingObject.SaveSettings();

	UpdateIniFirstRun();

	if (gdiplusToken != NULL)
		GdiplusShutdown(gdiplusToken);

	return CWinApp::ExitInstance();
}

////////////////////////////////////////////////////////////////
//功 能：重载此函数以保存所有工程文档（对应CDevDoc）
//描 述：此函数在关闭主窗口及退出应用程序时调用
////////////////////////////////////////////////////////////////
BOOL CDevApp::SaveAllModified() 
{
	if (!CDevDoc::SaveAllModified(TRUE))
		return FALSE;
	
	return CWinApp::SaveAllModified();
}

CDocument* CDevApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	return CDevDoc::OpenDocumentFile(lpszFileName);
	
//	return CWinApp::OpenDocumentFile(lpszFileName);
}

void CDevApp::NotifySelectChanged(BOOL bPost, DWORD dwNotify, BOOL bUpdateOnly)
{
	POSITION pos = m_listSelectNotify.GetHeadPosition();
	while (pos != NULL)
	{
		HWND hWnd = m_listSelectNotify.GetNext(pos);
		if (bPost)
			::PostMessage(hWnd, m_nSelectChangedMsg, dwNotify, bUpdateOnly);
		else
			::SendMessage(hWnd, m_nSelectChangedMsg, dwNotify, bUpdateOnly);
	}
}

//////////////////////////////////////////////////////////////////////
//功能:通知有关工具程序的当前子窗口选项发生变化
//参数:type[in]				被击活对象的类型
//		lparamInfoData[in]	对应type的参数类型
//////////////////////////////////////////////////////////////////////
void CDevApp::NotifyDocumentActivate(DWORD dwNotify, DWORD dwData)
{
	POSITION pos = m_listActivateMsgNotify.GetHeadPosition();
	while (pos != NULL)
	{
		HWND hWnd = m_listActivateMsgNotify.GetNext(pos);
		::SendMessage(hWnd, m_nDocumentActiveMsg, (WPARAM)dwNotify, (LPARAM)dwData);
	}
}

void CDevApp::NotifyDocumentChanged(DWORD dwNotify, DWORD dwData, BOOL bPost)
{
	POSITION pos = m_listFileOpenNotify.GetHeadPosition();
	while (pos != NULL)
	{
		HWND hWnd = m_listFileOpenNotify.GetNext(pos);
		if (bPost)
			::PostMessage(hWnd, m_nDocumentChangeMsg, (WPARAM)dwNotify, (LPARAM)dwData);
		else
			::SendMessage(hWnd, m_nDocumentChangeMsg, (WPARAM)dwNotify, (LPARAM)dwData);
	}
}

////////////////////////////////////////////////////////////////////
//功能:显示全局默认设置的对话框
////////////////////////////////////////////////////////////////////
void CDevApp::OnPictureOptions()
{
	CPageGen pageGen;
//	CPageMainForm pageMainForm;
//	CPageSubForm pageSubForm;
//	CPageObject pageObject;
	CPropertySheet sheet(_T("选项"), m_pMainWnd);

		sheet.AddPage(&pageGen);
//	sheet.AddPage(&pageMainForm);
//	sheet.AddPage(&pageSubForm);
//	sheet.AddPage(&pageObject);

	sheet.m_psh.dwFlags &= ~PSH_HASHELP;
	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.DoModal();
}

// const CLSID CLSID_CxRun = {0xEB197719,0x4C10,0x11D5,{0xA4,0x6A,0xB0,0x43,0xDC,0xC5,0x43,0x77}};

const TCHAR szCxRunClass[] = _T("CxRunClass");

void CDevApp::OnPictureRun() 
{
	CString strFile;

	CDevDoc* pDoc = CDevDoc::GetActivateDoc();
	if (pDoc != NULL)
	{
		BOOL bSave = FALSE;
		strFile = pDoc->GetPathName();
		if (strFile == "")
		{
			if (AfxMessageBox("文档未保存, 是否保存后运行？", MB_OKCANCEL | MB_ICONQUESTION) != IDOK)
				return;

			bSave = TRUE;
		}
		else
		{
			//判定是否要存盘
			CPtrList* pDocList = &pDoc->m_listSubDoc;
			POSITION posSubDoc = pDocList->GetHeadPosition();
			while (posSubDoc != NULL)
			{
				CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc *)pDocList->GetNext(posSubDoc);
				if (pSubDoc->m_pChildDoc && pSubDoc->m_pChildDoc->IsModified())
				{
					int iRet = AfxMessageBox("文档改动, 是否保存后运行？", MB_YESNOCANCEL | MB_ICONQUESTION);
					if (iRet == IDCANCEL)
						return;
					else if (iRet == IDYES)
						bSave = TRUE;
					break;
				}
			}
		}

		if (bSave)
		{
			if (pDoc->DoFileSave() == FALSE)
				return;
			strFile = pDoc->GetPathName();
		}
	}

	// 启动运行环境

	CLSID clsid;
	if (CLSIDFromProgID(OLESTR("CxRun.Document"), &clsid) == S_OK)
	{
		COleDispatchDriver dd;

		LPUNKNOWN lpUnk;
		LPDISPATCH lpDispatch;

		HRESULT hr = GetActiveObject(clsid, NULL, &lpUnk);
		if (hr == NOERROR)
		{
			HRESULT hr = lpUnk->QueryInterface(IID_IDispatch,
				(LPVOID*)&lpDispatch);
			lpUnk->Release();
			if (hr == NOERROR)
				dd.AttachDispatch(lpDispatch, TRUE);

			HWND hWndFirst = FindWindow(szCxRunClass, NULL);
			if (hWndFirst != NULL)
				SetForegroundWindow(hWndFirst);

					if (!strFile.IsEmpty())
			{
				static BYTE parms[] =
					VTS_BSTR;
				dd.InvokeHelper(2, DISPATCH_METHOD,
					VT_EMPTY, NULL, parms, strFile);
			}
			else
			{
				dd.InvokeHelper(3, DISPATCH_METHOD,
					VT_EMPTY, NULL, NULL);
			}

//			dd.SetProperty(0x1, VT_BOOL, TRUE);
		}
	}
	
	// 如果运行环境是第一次启动，无法使用自动化接口调用，则以命令行形式启动

	CString strApplicationName = GetCurrentAppDirectory();
	strApplicationName += _T("CxRun.exe");
	
	strFile.Insert(0, " ");
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	
	if (!CreateProcess(
		strApplicationName,   // pointer to name of executable module
		(LPTSTR)(LPCTSTR)strFile, // pointer to command line string
		NULL,   // pointer to process security attributes
		NULL,   // pointer to thread security attributes
		FALSE,  // handle inheritance flag
		0,      // creation flags
		NULL,   // pointer to new environment block
		NULL,   // pointer to current directory name
		&si,    // pointer to STARTUPINFO
		&pi     // pointer to PROCESS_INFORMATION
		))
	{
		AfxMessageBox("启动运行环境失败！");
	}
}

/////////////////////////////////////////////////////////////////////////////
//功能；读入扩展类型库
/////////////////////////////////////////////////////////////////////////////

const IID LIBID_CXDATSESLib = {0x61ECCDB3,0xFF84,0x47E4,{0xBD,0x62,0xB7,0xB1,0x59,0x9E,0xC0,0x48}};

BOOL CDevApp::LoadExtTypeLib()
{
	USES_CONVERSION;
	HRESULT hResult;
	LPOLESTR pszModuleO;
	TCHAR szModule[MAX_PATH];
	CString strTLBPath;

	GetModuleFileName(NULL, szModule, MAX_PATH);
	strTLBPath = szModule;
	strTLBPath += _T( "\\2" );  // Load the second TLB from the executable

	pszModuleO = T2OLE(strTLBPath);
	hResult = LoadTypeLib(pszModuleO, &m_pHMITypeLib);
	if (FAILED(hResult))
	{
		TRACE("Failed to load typelib for HMI object\n");
		return FALSE;
	}

	hResult = LoadRegTypeLib(LIBID_CXDATSESLib, 1, 0, LOCALE_USER_DEFAULT, &m_pComTypeLib);
	if (FAILED(hResult))
	{
		TRACE("Failed to load typelib for common object\n");
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////
//功能：返回本程序所在目录
//////////////////////////////////////////////////////////////////
CString CDevApp::GetCurrentAppDirectory()
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

CString CDevApp::GetSystemSetupFile()
{
	CString strFile = GetCurrentAppDirectory();
	strFile += _T("System.ini");

	return strFile;
}

//////////////////////////////////////////////////////////////////
//功能:返回当前的工程所在的工作目录
//////////////////////////////////////////////////////////////////

int CDevApp::MakeSurePathExists(LPCTSTR lpszPath, 
		BOOL bFilenameIncluded)
{
	LPTSTR lpszTempPath = (LPTSTR)lpszPath;
	LPTSTR lpszLastDivide = NULL;
	TCHAR cTemp;

	while (lpszTempPath = strpbrk(lpszTempPath + 1, "\\/"))
	{
		cTemp = *lpszTempPath;
		*lpszTempPath = '\0';
		CreateDirectory(lpszPath, NULL);
		*lpszTempPath = cTemp;
		lpszLastDivide = lpszTempPath;
	}

	BOOL bRet = TRUE;
	if (!bFilenameIncluded)
	{
		CreateDirectory(lpszPath, NULL);
		bRet = !_access(lpszPath, 0);
	}
	else
	{
		if (lpszLastDivide != NULL)
		{
			cTemp = *lpszLastDivide;
			*lpszLastDivide = '\0';
			bRet = !_access(lpszPath, 0);
			*lpszLastDivide = cTemp;
		}
	}

	return bRet;
}

BOOL CDevApp::GetCurrentProject(LPTSTR lpszProject)
{
	HKEY hSectionKey = NULL;
	HKEY hAppKey = NULL;
	HKEY hSoftKey = NULL;
	HKEY hCompanyKey = NULL;
	BOOL bRet = FALSE;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("software"), 0, KEY_WRITE|KEY_READ,
		&hSoftKey) == ERROR_SUCCESS)
	{
		DWORD dw;
		if (RegCreateKeyEx(hSoftKey, _T("SuperCx"), 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCompanyKey, &dw) == ERROR_SUCCESS)
		{
			if (RegCreateKeyEx(hCompanyKey, _T("ProjectSetup"), 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hAppKey, &dw) == ERROR_SUCCESS)
			{
				if (RegCreateKeyEx(hAppKey, _T("General"), 0, REG_NONE,
					REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
					&hSectionKey, &dw) == ERROR_SUCCESS)
				{
					DWORD dwType, dwCount;
					LONG lResult = RegQueryValueEx(hSectionKey, _T("CurrentProject"), NULL, &dwType,
						NULL, &dwCount);
					if (lResult == ERROR_SUCCESS)
					{
						ATLASSERT(dwType == REG_SZ);
						ATLASSERT(dwCount < 128);
						lResult = RegQueryValueEx(hSectionKey, _T("CurrentProject"), NULL, &dwType,
								(LPBYTE)lpszProject, &dwCount);
						if (lResult == ERROR_SUCCESS)
							bRet = TRUE;
					}
				}
			}
		}
	}
	
	if (hSectionKey)
		RegCloseKey(hSectionKey);
	if (hAppKey)
		RegCloseKey(hAppKey);
	if (hSoftKey != NULL)
		RegCloseKey(hSoftKey);
	if (hCompanyKey != NULL)
		RegCloseKey(hCompanyKey);

	return bRet;
}

CString CDevApp::GetActiveProject()
{
	CString strFile = theApp.GetSystemSetupFile();

	CString strDefault = theApp.GetCurrentAppDirectory();
	strDefault += _T("Demo\\Demo.scp");

	CString strValue;
	strValue.GetBuffer(MAX_PATH);
	DWORD dw = ::GetPrivateProfileString(_T("Project"), _T("ActiveProject"),
		strDefault, strValue.GetBuffer(MAX_PATH), MAX_PATH, strFile);
	strValue.ReleaseBuffer();
	ASSERT(dw < 4095);

	return strValue;
}

BOOL CDevApp::GetPictureFolder(LPTSTR lpszFolder)
{
	CString strProject;
	// 没有当前工程则使用活动工程，没有活动工程使用演示工程
	if (!GetCurrentProject(strProject.GetBuffer(4096)))
		strProject = GetActiveProject();
	strProject.ReleaseBuffer();

	if (_access(strProject, 0) != 0)
		return FALSE;

	TCHAR szT[4096];
	DWORD dw = ::GetPrivateProfileString(_T("Path"), _T("Picture"),
			_T("<工程路径>\\Picture"), szT, 4096, strProject);

	LPTSTR lpszFind = strstr(szT, _T("<工程路径>"));
	if (lpszFind != NULL)
	{
		// always capture the complete file name including extension (if present)
		LPTSTR lpszTemp = (LPTSTR)(LPCTSTR)strProject;
		for (LPCTSTR lpsz = strProject; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
		{
			// remember last directory/drive separator
		if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
				lpszTemp = (LPTSTR)_tcsinc(lpsz);
		}

		lstrcpyn(lpszFolder, strProject, lpszTemp - strProject);
		lstrcat(lpszFolder, szT + 10);
	}
	else
	{
		lstrcat(lpszFolder, szT);
	}
	
	return TRUE;
}

static TCHAR BASED_CODE szIniOptionSection[] = _T("Option");
static TCHAR BASED_CODE szIniPictureEntry[] = _T("PictureFolder");

CString CDevApp::GetIniStartPictureFolder(BOOL bCache)
{
	if (bCache && !m_strCurrentPictureFolder.IsEmpty())
		return m_strCurrentPictureFolder;

	CString strFolder;
	CString strTemp;
	BOOL b;

	// 使用用户选项设置
	strFolder = GetProfileString(szIniOptionSection, szIniPictureEntry, NULL);
	if (!strFolder.IsEmpty())
		goto OnQuit;

	// 使用OEM设置，画面开发环境作为画图工具使用
	strTemp = COEMSetup::GetModuleString(_T("UseAsPictureTool"));
	if (strTemp.CompareNoCase(_T("Yes")) == 0)
	{
		TCHAR szDir[_MAX_PATH];
		memset(szDir, 0, _MAX_PATH);
		LPITEMIDLIST pidl;
		LPMALLOC pmalloc;
		try
		{
			if (SUCCEEDED(SHGetMalloc(&pmalloc)))
			{
				if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidl)))
				{
					SHGetPathFromIDList(pidl, szDir);
					pmalloc->Free(pidl);
				}
				pmalloc->Release();
			}
		}
		catch(...)
		{
		}

		strFolder = szDir;
		strFolder += _T("\\My SuperCx Files");
		goto OnQuit;
	}

	// 使用工程设置
	b = GetPictureFolder(strFolder.GetBuffer(MAX_PATH));
	strFolder.ReleaseBuffer();
	if (b)
		goto OnQuit;

	strFolder = GetCurrentAppDirectory();
	strFolder += _T("Demo\\Picture");

OnQuit:
	LPCTSTR lpszTemp = (LPCTSTR)strFolder + strFolder.GetLength() - 1;
	if (*lpszTemp != '\\' && *lpszTemp != '/')
		strFolder += _T("\\");
	
	MakeSurePathExists(strFolder);

	m_strCurrentPictureFolder = strFolder;
	return strFolder;
}

void CDevApp::UpdateIniStartPictureFolder(LPCTSTR lpszPathName)
{
	if (GetIniStartPictureFolder() != lpszPathName)
	{
//		if (_access(lpszPathName, 0) == 0)
//		{
//			if (IDYES == AfxMessageBox(_T("指定的文件夹不存在，是否创建?"), MB_YESNO))
//			{
//				if (!MakeSurePathExists(lpszPathName))
//					AfxMessageBox(_T("创建文件夹失败！"));
//			}
//		}
		WriteProfileString(szIniOptionSection, szIniPictureEntry, lpszPathName);
		if (theApp.GetIniStartPictureFolder(FALSE) != theApp.GetIniStartPictureFolder())
			NotifyDocumentChanged(0, 0, TRUE);
	}
}

static TCHAR BASED_CODE szIniAppState[] = _T("AppState");
static TCHAR BASED_CODE szIniFirstRun[] = _T("FirstRun");

BOOL CDevApp::IsFirstRun()
{
	return GetProfileInt(szIniAppState, szIniFirstRun, 1);	
}

BOOL CDevApp::UpdateIniFirstRun()
{
	return WriteProfileInt(szIniAppState, szIniFirstRun, 0);	
}

void CDevApp::OnPictureOpcserver() 
{
	COPCDataSource dlg;
	dlg.DoModal();
}


BOOL CDevApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	if (CScriptView::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

void CDevApp::GetUserInfo()
{
	HKEY hVerKey = NULL;
	HKEY hProductKey = NULL;
	HKEY hSoftKey = NULL;
	HKEY hCompanyKey = NULL;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("software"), 0, KEY_WRITE|KEY_READ,
			&hSoftKey) == ERROR_SUCCESS)
	{
		if (RegOpenKeyEx(hSoftKey, _T("SuperCx"), 0, KEY_WRITE|KEY_READ, 
				&hCompanyKey) == ERROR_SUCCESS)
		{
			if (RegOpenKeyEx(hCompanyKey, _T("SuperCx 监控软件平台"), 0, KEY_WRITE|KEY_READ, 
					&hProductKey) == ERROR_SUCCESS)
			{
				if (RegOpenKeyEx(hProductKey, _T("3.0"), 0, KEY_WRITE|KEY_READ, 
						&hVerKey) == ERROR_SUCCESS)
				{
					DWORD dwType, dwCount;
					CString strValue;
					LONG lResult;
					lResult = RegQueryValueEx(hVerKey, _T("User"), NULL, &dwType,
						NULL, &dwCount);
					if (lResult == ERROR_SUCCESS)
					{
						ASSERT(dwType == REG_SZ);
						lResult = RegQueryValueEx(hVerKey, _T("User"), NULL, &dwType,
							(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
						strValue.ReleaseBuffer();
						m_strUserName = strValue;
					}

					lResult = RegQueryValueEx(hVerKey, _T("Company"), NULL, &dwType,
						NULL, &dwCount);
					if (lResult == ERROR_SUCCESS)
					{
						ASSERT(dwType == REG_SZ);
						lResult = RegQueryValueEx(hVerKey, _T("Company"), NULL, &dwType,
							(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
						strValue.ReleaseBuffer();
						m_strUserCompany = strValue;
					}

					lResult = RegQueryValueEx(hVerKey, _T("Serial"), NULL, &dwType,
						NULL, &dwCount);
					if (lResult == ERROR_SUCCESS)
					{
						ASSERT(dwType == REG_SZ);
						lResult = RegQueryValueEx(hVerKey, _T("Serial"), NULL, &dwType,
							(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
						strValue.ReleaseBuffer();
						m_strUserSerial = strValue;
					}
				}
			}
		}
	}
	if (hVerKey)
		RegCloseKey(hVerKey);
	if (hProductKey)
		RegCloseKey(hProductKey);
	if (hSoftKey != NULL)
		RegCloseKey(hSoftKey);
	if (hCompanyKey != NULL)
		RegCloseKey(hCompanyKey);
}

const TCHAR szCxMCClass[] = _T("CxMCClass");

void CDevApp::OnExecuteAppMC() 
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

void CDevApp::AlignToGrid(CPoint& point)
{
	if (m_settingGen.m_bAlignToGrid)
	{
		int nGrindWidth = m_settingGen.m_nGridSpaceWidth;
		point.x = ROUND((point.x + nGrindWidth / 2) / nGrindWidth) * nGrindWidth;
		int nGrindHeight = m_settingGen.m_nGridSpaceHeight;
		point.y = ROUND((point.y + nGrindHeight / 2) / nGrindHeight) * nGrindHeight;
	}
}
