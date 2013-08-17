// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CxRun.h"

#include "MainFrm.h"
#include "InputPasswordDlg.h"
#include "Splash.h"
#include "HtmlHelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_VIEW_FULL_SCREEN, OnViewFullScreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULL_SCREEN, OnUpdateViewFullScreen)
	ON_WM_CLOSE()
	ON_COMMAND(ID_HELP_CONTENT, OnHelpContent)
	ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	ON_COMMAND(ID_HELP_SEARCH, OnHelpSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_bFullScreenMode = FALSE;
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	WNDCLASS wndcls;
	
	BOOL bRes = CFrameWnd::PreCreateWindow(cs);
	HINSTANCE hInst = AfxGetInstanceHandle();
	
	// see if the class already exists
	if (!::GetClassInfo(hInst, szCxRunClass, &wndcls))
	{
		// get default stuff
		::GetClassInfo(hInst, cs.lpszClass, &wndcls);
		wndcls.style &= ~(CS_HREDRAW|CS_VREDRAW);
		// register a new class
		wndcls.lpszClassName = szCxRunClass;
		wndcls.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
		ASSERT(wndcls.hIcon != NULL);
		if (!AfxRegisterClass(&wndcls))
			AfxThrowResourceException();
	}
	cs.lpszClass = szCxRunClass;

	cs.style |= WS_MAXIMIZE;
	if (!theApp.m_bSystemMenu)
		cs.style &= ~WS_SYSMENU;
 	if (!theApp.m_bMaximizeBox)
		cs.style &= ~WS_MAXIMIZEBOX;
 	if (!theApp.m_bMinimizeBox)
		cs.style &= ~WS_MINIMIZEBOX;

	// 如果不现实菜单，现实平板
	if (!theApp.m_bMainMenu)
	{
		cs.hMenu = NULL;
//		cs.dwExStyle = WS_EX_WINDOWEDGE;
	}

//	cs.dwExStyle = WS_EX_WINDOWEDGE;
 	if (!theApp.m_bThickFrame)
		cs.style &= ~WS_THICKFRAME;

	return bRes;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::FullScreenModeOn()
{
	GetWindowPlacement(&m_wpOldWndPlacement);

	CRect WindowRect,ClientRect;
	GetWindowRect(&WindowRect);
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery,&ClientRect);
	ClientToScreen(&ClientRect);

	int nFullWidth = GetSystemMetrics(SM_CXSCREEN);
	int nFullHeight = GetSystemMetrics(SM_CYSCREEN);

	m_rcFullScreen.left = WindowRect.left - ClientRect.left;
	m_rcFullScreen.top = WindowRect.top - ClientRect.top;
	m_rcFullScreen.right = WindowRect.right - ClientRect.right + nFullWidth;
	m_rcFullScreen.bottom = WindowRect.bottom - ClientRect.bottom + nFullHeight;
    
	m_bFullScreenMode = TRUE;

	WINDOWPLACEMENT wndpl;
	wndpl.length = sizeof(WINDOWPLACEMENT);
	wndpl.flags = 0;
	wndpl.showCmd = SW_SHOWNORMAL;
	wndpl.rcNormalPosition = m_rcFullScreen;
	SetWindowPlacement(&wndpl);
}

void CMainFrame::FullScreenModeOff()
{
	if (theApp.m_bFullScreen && theApp.m_bDisableSwitchFullScreen)
		return;

	if(m_bFullScreenMode)
	{
	    m_bFullScreenMode = FALSE;
		SetWindowPlacement(&m_wpOldWndPlacement);
	}
}

void CMainFrame::OnViewFullScreen() 
{
	if(m_bFullScreenMode)
		FullScreenModeOff();
	else FullScreenModeOn();	
}

void CMainFrame::OnUpdateViewFullScreen(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bFullScreenMode);	
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if (m_bFullScreenMode)
	{
		lpMMI->ptMaxSize.x = m_rcFullScreen.Width();
        lpMMI->ptMaxSize.y = m_rcFullScreen.Height();
        lpMMI->ptMaxPosition.x = m_rcFullScreen.Width();
        lpMMI->ptMaxPosition.y = m_rcFullScreen.Height();
        lpMMI->ptMaxTrackSize.x = m_rcFullScreen.Width();
      	lpMMI->ptMaxTrackSize.y = m_rcFullScreen.Height();
	}
	else
	{
  		lpMMI->ptMinTrackSize.x = 320;
      	lpMMI->ptMinTrackSize.y = 240;
   	}

	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

/////////////////////////////////////////////////////////////
//功 能：关闭主窗口处理
//描 述：此函数实现代码参考void CFrameWnd::OnClose()
////////////////////////////////////////////////////////////
void CMainFrame::OnClose() 
{
	if (!theApp.m_strExitPassword.IsEmpty())
	{
		CInputPasswordDlg dlg;
		if (dlg.DoModal() != IDOK)
			return;
		
		if (dlg.m_strPassword != theApp.m_strExitPassword)
		{
			MessageBox(_T("您输入的退出密码不正确！"), _T("密码错误"), MB_ICONEXCLAMATION | MB_OK);
			return;
		}
	}

	CFrameWnd::OnClose();
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	if (theApp.m_bCanActiveMainFrame)
		CFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	SetWindowText(theApp.m_strCaption);
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);

	return 0;
}

void CMainFrame::OnHelpContent() 
{
	CString strHelpFile = theApp.GetCurrentAppDirectory();
	strHelpFile += _T("CxHlp.chm");
	HWND hwndHelp = HtmlHelp(NULL, strHelpFile, HH_DISPLAY_TOC, 0);
}

void CMainFrame::OnHelpIndex() 
{
	CString strHelpFile = theApp.GetCurrentAppDirectory();
	strHelpFile += _T("CxHlp.chm");
	HWND hwndHelp = HtmlHelp(NULL, strHelpFile, HH_DISPLAY_INDEX, 0);
}

void CMainFrame::OnHelpSearch() 
{
	HH_FTS_QUERY que;
	que.cbStruct = sizeof(que);
	que.fUniCodeStrings = FALSE;
	que.pszSearchQuery = "abc";
	que.iProximity = 0;
	que.fStemmedSearch = FALSE;
	que.fTitleOnly = TRUE;
	que.fExecute = TRUE;
	que.pszWindow = "";
	
	CString strHelpFile = theApp.GetCurrentAppDirectory();
	strHelpFile += _T("CxHlp.chm");
	HWND hwndHelp = HtmlHelp(NULL, strHelpFile, HH_DISPLAY_SEARCH, (DWORD)&que);
}
