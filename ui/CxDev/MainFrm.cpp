// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DevInc.h"
#include "ExplorerTree.h"
#include "Splash.h"
#include "GridSettingDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_COMMAND(ID_WINDOW_CLOSE, OnWindowClose)
	ON_COMMAND(ID_WINDOW_CLOSEALL, OnWindowCloseall)
	ON_COMMAND(ID_WINDOW_NEXT, OnWindowNext)
	ON_COMMAND(ID_WINDOW_PREVIOUS, OnWindowPrevious)
	ON_UPDATE_COMMAND_UI_RANGE(ID_WINDOW_CLOSE, ID_WINDOW_PREVIOUS, OnUpdateWindow)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_VIEW_EXPLORER_BAR, OnViewExplorerBar)
	ON_COMMAND(ID_VIEW_PROPERTY_BAR, OnViewPropertyBar)
	ON_COMMAND(ID_VIEW_TOOLBOX, OnViewToolbox)
	ON_COMMAND(ID_CHAR_COLOR, OnCharColor)
	ON_COMMAND(ID_VIEW_FULL_SCREEN, OnViewFullScreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULL_SCREEN, OnUpdateViewFullScreen)
	ON_COMMAND(ID_VIEW_OBJECT, OnViewObject)
	ON_COMMAND(ID_VIEW_SCRIPT, OnViewCode)
	ON_WM_CLOSE()
	ON_WM_ENDSESSION()
	ON_COMMAND(ID_VIEW_DYNAMIC_EXPERT, OnViewDynamicExpert)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DYNAMIC_EXPERT, OnUpdateViewDynamicExpert)
	ON_COMMAND(ID_VIEW_APPEAR_EXPERT, OnViewAppearExpert)
	ON_UPDATE_COMMAND_UI(ID_VIEW_APPEAR_EXPERT, OnUpdateViewAppearExpert)
	ON_COMMAND(ID_VIEW_ACTION_EXPERT, OnViewActionExpert)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ACTION_EXPERT, OnUpdateViewActionExpert)
	ON_COMMAND(ID_VIEW_STAND_BAR, OnViewStand)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STAND_BAR, OnUpdateViewStand)
	ON_COMMAND(ID_VIEW_LAYOUT_BAR, OnViewLayout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LAYOUT_BAR, OnUpdateViewLayout)
	ON_COMMAND(ID_HELP_CONTENT, OnHelpContent)
	ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	ON_COMMAND(ID_HELP_SEARCH, OnHelpSearch)
	ON_COMMAND(ID_ADD_CONTROL, OnAddControl)
	ON_MESSAGE(WM_INSERTCONTROL, OnInsertControl)
	ON_COMMAND(ID_GRID_SETTING, OnGridSetting)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_DRAW_SELECT, ID_DRAW_ALMSUM, OnTool)
	ON_UPDATE_COMMAND_UI_RANGE(ID_DRAW_SELECT, ID_DRAW_ALMSUM, OnUpdateTool)
	ON_REGISTERED_MESSAGE(CDevApp::m_nProjectOpendMsg, OnProjectOpendMsg)
END_MESSAGE_MAP()

static UINT BASED_CODE toolbox[] =
{
	// same order as in the bitmap 'palette.bmp'
	ID_DRAW_SELECT,
	ID_DRAW_LINE,
	ID_DRAW_RECT,
	ID_DRAW_ROUNDRECT,
	ID_DRAW_ELLIPSE,
	ID_DRAW_POLYLINE,
	ID_DRAW_PIPE,
	ID_DRAW_POLYGON,
	ID_DRAW_ARC,
	ID_DRAW_PIE,
	ID_DRAW_CHORD,
	ID_DRAW_TEXT,
	ID_DRAW_DATALINK,
	ID_DRAW_VARIABLE,
	ID_DRAW_BUTTON,
	ID_DRAW_MENU,
	ID_DRAW_TIMER,
	ID_DRAW_CHART,
	ID_DRAW_ALMSUM,
	ID_INSERT_BITMAP,
};

static UINT BASED_CODE format[] =
{
	// same order as in the bitmap 'format.bmp'
		ID_SEPARATOR, // font name combo box
		ID_SEPARATOR,
		ID_SEPARATOR, // font size combo box
		ID_SEPARATOR,
	ID_CHAR_BOLD,
	ID_CHAR_ITALIC,
	ID_CHAR_UNDERLINE,
	ID_CHAR_COLOR,
		ID_SEPARATOR,
		ID_SEPARATOR, //文本颜色
/*	ID_PARA_LEFT,
	ID_PARA_CENTER,
	ID_PARA_RIGHT,
		ID_SEPARATOR,
	ID_INSERT_BULLET,
*/
};

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CURSOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()

{
	m_bFullScreenMode = FALSE;
	m_pwndFullScreenBar = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_strTitle.Replace(_T("SuperCx"), COEMSetup::GetSystemName());

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!CreateStatusBar())
		return -1;
	EnableDocking(CBRS_ALIGN_ANY);
	if (!CreateToolBar())
		return -1;
	if (!CreateLayoutBar())
		return -1;
	if (!CreateToolBox())
		return -1;
	if (!CreateExplorerBar())
		return -1;
	if (!CreatePropertyBar())
		return -1;
	if (!CreateExpertBar())
		return -1;

	CSizingControlBar::GlobalLoadState(_T("BarState"));
	LoadBarState(_T("BarState"));

	if (theApp.IsFirstRun())
	{	
		//该用户第一次使用
		FirstPosition();
	}
	
	// CG: The following line was added by the Splash Screen component.
	if (!COEMSetup::IsSetupFileExist())
		CSplashWnd::ShowSplashScreen(this);

	return 0;
}

const TCHAR szCxDevClass[] = _T("CxDevClass");

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
 	WNDCLASS wndcls;
 	
 	BOOL bRes = CMDIFrameWnd::PreCreateWindow(cs);
 	HINSTANCE hInst = AfxGetInstanceHandle();
 	
 	// see if the class already exists
 	if (!::GetClassInfo(hInst, szCxDevClass, &wndcls))
 	{
 		// get default stuff
 		::GetClassInfo(hInst, cs.lpszClass, &wndcls);
 		wndcls.style &= ~(CS_HREDRAW|CS_VREDRAW);
		// register a new class
 		wndcls.lpszClassName = szCxDevClass;
		wndcls.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
 		ASSERT(wndcls.hIcon != NULL);
 		if (!AfxRegisterClass(&wndcls))
 			AfxThrowResourceException();
 	}
 	cs.lpszClass = szCxDevClass;
//	cs.style |= FWS_PREFIXTITLE;
	
 	return bRes;
 }

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
BOOL CMainFrame::CreateToolBar()
{
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;      // fail to create
	}

	m_wndToolBar.SetWindowText(_T("标准"));
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);

	return TRUE;
}

BOOL CMainFrame::CreateLayoutBar()
{
	if (!m_wndLayoutBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP |
		CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, 
		CRect(0, 0, 0, 0), AFX_IDW_CONTROLBAR_FIRST + 33) ||
        !m_wndLayoutBar.LoadToolBar(IDR_LAYOUTBAR))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }

	m_wndLayoutBar.SetWindowText(_T("布局"));
	m_wndLayoutBar.EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndLayoutBar, AFX_IDW_DOCKBAR_TOP);
	
	return TRUE;
}

BOOL CMainFrame::CreateToolBox()
{
	if (!m_wndToolBox.Create("工具箱", this, CSize(70, 360),
            TRUE, ID_TOOLBOX))
    {
		TRACE0("Failed to create explorerbar\n");
        return FALSE;      
    }

	m_wndToolBox.SetBarStyle(m_wndToolBox.GetBarStyle() |
        SCBS_SHOWEDGES | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBox.EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndToolBox, AFX_IDW_DOCKBAR_RIGHT);

	return TRUE;
}

BOOL CMainFrame::CreateStatusBar()
{
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return FALSE;      // fail to create
	}

	return TRUE;
}

BOOL CMainFrame::CreateExplorerBar()
{
	if (!m_wndExplorerBar.Create("资源浏览器", this, CSize(240, 160),
            TRUE, ID_EXPLORER_BAR))
    {
		TRACE0("Failed to create explorerbar\n");
        return FALSE;      
    }

	m_wndExplorerBar.SetBarStyle(m_wndExplorerBar.GetBarStyle() |
        SCBS_SHOWEDGES | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndExplorerBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndExplorerBar, AFX_IDW_DOCKBAR_LEFT);

	return TRUE;
}

BOOL CMainFrame::CreatePropertyBar()
{
	if (!m_wndPropertyBar.Create("属性", this, CSize(240, 180),
            TRUE, ID_PROPERTY_BAR))
    {
		TRACE0("Failed to create propertybar\n");
        return FALSE;      
    }

	m_wndPropertyBar.SetBarStyle(m_wndPropertyBar.GetBarStyle() |
        SCBS_SHOWEDGES | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndPropertyBar.EnableDocking(CBRS_ALIGN_ANY);
//	FloatControlBar(&m_wndPropertyBar, CPoint(600, 200));
	DockControlBar(&m_wndPropertyBar, AFX_IDW_DOCKBAR_LEFT);
	
	return TRUE;
}

BOOL CMainFrame::DestroyWindow() 
{
	FullScreenModeOff();

	CSizingControlBar::GlobalSaveState(_T("BarState"));
    SaveBarState(_T("BarState"));

	return CMDIFrameWnd::DestroyWindow();
}

void CMainFrame::OnViewExplorerBar() 
{
	BOOL bShow = m_wndExplorerBar.IsVisible();
	if (!bShow)
		ShowControlBar(&m_wndExplorerBar, !bShow, FALSE);
}

void CMainFrame::OnViewPropertyBar() 
{
	BOOL bShow = m_wndPropertyBar.IsVisible();
	if (!bShow)
		ShowControlBar(&m_wndPropertyBar, !bShow, FALSE);
}

void CMainFrame::OnViewToolbox() 
{
	BOOL bShow = m_wndToolBox.IsVisible();
	if (!bShow)
		ShowControlBar(&m_wndToolBox, !bShow, FALSE);
}

void CMainFrame::OnCharColor()
{
//	CColorMenu colorMenu;
//	CRect rc;
//	int index = m_wndFormatBar.CommandToIndex(ID_CHAR_COLOR);
//	m_wndFormatBar.GetItemRect(index, &rc);
//	m_wndFormatBar.ClientToScreen(rc);
//	colorMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,rc.left,rc.bottom, this);
}

void CMainFrame::FullScreenModeOn()
{
	GetWindowPlacement(&m_OldWndPlacement);

	CRect WindowRect,ClientRect;
	GetWindowRect(&WindowRect);
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery,&ClientRect);
	ClientToScreen(&ClientRect);

	int nFullWidth = GetSystemMetrics(SM_CXSCREEN);
	int nFullHeight = GetSystemMetrics(SM_CYSCREEN);

	m_FullScreenRect.left = WindowRect.left - ClientRect.left - 1;
	m_FullScreenRect.top = WindowRect.top - ClientRect.top - 1;
	m_FullScreenRect.right = WindowRect.right - ClientRect.right + nFullWidth + 2;
	m_FullScreenRect.bottom = WindowRect.bottom - ClientRect.bottom + nFullHeight + 2;
    
	m_bFullScreenMode = TRUE;

	WINDOWPLACEMENT wndpl;
	wndpl.length = sizeof(WINDOWPLACEMENT);
	wndpl.flags = 0;
	wndpl.showCmd = SW_SHOWNORMAL;
	wndpl.rcNormalPosition = m_FullScreenRect;
	SetWindowPlacement(&wndpl);

	m_pwndFullScreenBar= new CToolBar;
	m_pwndFullScreenBar->Create(this);
	m_pwndFullScreenBar->LoadToolBar(IDR_FULLSCREEN);
	m_pwndFullScreenBar->SetBarStyle(m_pwndFullScreenBar->GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	// to look better:
	m_pwndFullScreenBar->ModifyStyle(0, TBSTYLE_FLAT);
	m_pwndFullScreenBar->EnableDocking(0);
	// place the full-screen off button somewhere:
	CPoint pt(300,200);
	FloatControlBar(m_pwndFullScreenBar, pt);
}

void CMainFrame::FullScreenModeOff()
{
	if (m_bFullScreenMode)
	{
	    m_bFullScreenMode = FALSE;
		delete m_pwndFullScreenBar;
		m_pwndFullScreenBar = NULL;
		ShowWindow(SW_HIDE);
		SetWindowPlacement(&m_OldWndPlacement);
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
		lpMMI->ptMaxSize.x = m_FullScreenRect.Width();
        lpMMI->ptMaxSize.y = m_FullScreenRect.Height();
        lpMMI->ptMaxPosition.x = m_FullScreenRect.Width();
        lpMMI->ptMaxPosition.y = m_FullScreenRect.Height();
        lpMMI->ptMaxTrackSize.x = m_FullScreenRect.Width();
      	lpMMI->ptMaxTrackSize.y = m_FullScreenRect.Height();
	}
	else
	{
  		lpMMI->ptMinTrackSize.x = 320;
      	lpMMI->ptMinTrackSize.y = 240;
   	}

	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

CMDIChildWnd* CMainFrame::CreateOrActivateFrame(CDocTemplate* pTemplate,
	CRuntimeClass* pViewClass)
{
	//如果布局视图或者脚本视图已经存在，激或包含它的子窗体。
	//否则，为文档创建一个新视图
	
	CMDIChildWnd* pMDIActive = MDIGetActive();
	ASSERT(pMDIActive != NULL);
	CDocument* pDoc = pMDIActive->GetActiveDocument();
	ASSERT(pDoc != NULL);

	CView* pView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(pViewClass))
		{
			CMDIChildWnd* pFrame = (CMDIChildWnd *)pView->GetParentFrame();
			pFrame->ActivateFrame();
			return pFrame;
		}
	}

	CMDIChildWnd* pNewFrame
		= (CMDIChildWnd*)(pTemplate->CreateNewFrame(pDoc, NULL));
	if (pNewFrame == NULL)
		return NULL;     // not created
	ASSERT_KINDOF(CMDIChildWnd, pNewFrame);
	pTemplate->InitialUpdateFrame(pNewFrame, pDoc);
//	MDITile(MDITILE_HORIZONTAL);
	return pNewFrame;
}

void CMainFrame::OnViewObject() 
{
	CMDIChildWnd* pMDIActive = MDIGetActive();
	if (pMDIActive != NULL)
		CreateOrActivateFrame(theApp.m_pLayoutTemplate,
				RUNTIME_CLASS(CLayoutView));	
}

void CMainFrame::OnViewCode() 
{
	CMDIChildWnd* pMDIActive = MDIGetActive();
	if (pMDIActive != NULL)
	{
		CScriptFrame* pFrame = (CScriptFrame *)CreateOrActivateFrame(theApp.m_pScriptTemplate,
				RUNTIME_CLASS(CScriptView));
		if (pFrame != NULL && pFrame != pMDIActive)
		{
			CFormDoc* pDoc = (CFormDoc *)pFrame->GetActiveDocument();
			ASSERT(pDoc != NULL);

			((CScriptView*)pFrame->GetActiveView())->EditObjectEventHandler(NULL, NULL, FALSE);
		}
	}
}

/////////////////////////////////////////////////////////////
//功 能：关闭主窗口处理
//描 述：此函数实现代码参考void ::OnEndSession(BOOL bEnding)
////////////////////////////////////////////////////////////
void CMainFrame::OnEndSession(BOOL bEnding) 
{
	if (!bEnding)
		return;

	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL && pApp->m_pMainWnd == this)
	{
		AfxOleSetUserCtrl(TRUE);    // keeps from randomly shutting down
		CDevDoc::CloseAllDocuments(FALSE); //关闭工程文档
		pApp->CloseAllDocuments(TRUE);

		// allow application to save settings, etc.
		pApp->ExitInstance();
	}
}

/////////////////////////////////////////////////////////////
//功 能：关闭主窗口处理
//描 述：此函数实现代码参考void CFrameWnd::OnClose()
////////////////////////////////////////////////////////////
void CMainFrame::OnClose() 
{
	if (m_lpfnCloseProc != NULL && !(*m_lpfnCloseProc)(this))
		return;

	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL && pApp->m_pMainWnd == this)
	{
		// attempt to save all documents
		if (!pApp->SaveAllModified())
			return;     // don't close it

		// hide the application's windows before closing all the documents
		pApp->HideApplication();

		// close all documents first
		CDevDoc::CloseAllDocuments(FALSE);  //关闭工程文档
		pApp->CloseAllDocuments(FALSE);

		//close all barwnd
		m_wndExplorerBar.DestroyAllChild();

		// don't exit if there are outstanding component objects
		if (!AfxOleCanExitApp())
		{
			// take user out of control of the app
			AfxOleSetUserCtrl(FALSE);

			// don't destroy the main window and close down just yet
			//  (there are outstanding component (OLE) objects)
			return;
		}

		// there are cases where destroying the documents may destroy the
		//  main window of the application.
		if (!afxContextIsDLL && pApp->m_pMainWnd == NULL)
		{
			AfxPostQuitMessage(0);
			return;
		}
	}

	// then destroy the window
	DestroyWindow();
}

void CMainFrame::OnTool(UINT nID)
{
	CDevTool::c_drawShape = DRAW_SHAPE(nID - ID_DRAW_SELECT);
	CDevTool* pTool = CDevTool::FindTool(CDevTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnInit();

	CDevTool::SetTrackerState(CDevObj::general);
}

void CMainFrame::OnUpdateTool(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(pCmdUI->m_nID - ID_DRAW_SELECT == (unsigned)CDevTool::c_drawShape);

//	pCmdUI->SetCheck(pCmdUI->m_nID - ID_DRAW_SELECT == (unsigned)CDevTool::c_drawShape);
}

/////////////////////////////////////////////////////////////////////
//功 能：插入控件
////////////////////////////////////////////////////////////////////////
LRESULT CMainFrame::OnInsertControl(WPARAM wParam, LPARAM lParam)
{
	CDevTool::c_drawShape = DRAW_RECTITEM;
	CDevTool::SetTrackerState(CDevObj::general);
	CRectItemTool::c_clsid = *(CLSID *)wParam;

	return 0;
}

const CLSID CLSID_CxAppearanceExpert = {0x90EFE451,0x2B10,0x40D3,{0x9C,0xDE,0xDA,0x0B,0x12,0x2F,0xDF,0xB3}};
const CLSID CLSID_CxDynamicExpert = {0xA5034306,0x39CC,0x42FD,{0xB5,0xE6,0xC3,0x94,0xA9,0x47,0xF8,0x8E}};
const CLSID CLSID_CxEventExpert = {0xD4C4E688,0x44D3,0x4781,{0xB4,0x61,0x3F,0xFC,0x8B,0x86,0x2B,0x68}};

///////////////////////////////////////////////////////////////
//功能:创建专家工具窗口
///////////////////////////////////////////////////////////////
BOOL CMainFrame::CreateExpertBar()
{
	if (!m_wndAppearExpert.Create(CLSID_CxAppearanceExpert, _T("颜色和线条专家"), ID_APPEAR_EXPERT, this))
		return FALSE;
	m_wndAppearExpert.EnableDocking(0);

	if (!m_wndDynamicExpert.Create(CLSID_CxDynamicExpert, _T("动画专家"), ID_DYNAMIC_EXPERT, this))
		return FALSE;
	m_wndDynamicExpert.EnableDocking(0);

	if (!m_wndActionExpert.Create(CLSID_CxEventExpert, _T("动作专家"), ID_EVENT_EXPERT, this))
		return FALSE;
	m_wndActionExpert.EnableDocking(0);

	return TRUE;
}

/////////////////////////////////////////////////////////
//功能:返回图符工具窗口, 或NULL(失败)
/////////////////////////////////////////////////////////
CWnd* CMainFrame::GetToolSymbol()
{
	return m_wndExplorerBar.GetToolWnd(EXPBAR_SYMBOL);
}


/////////////////////////////////////////////////////////
//功能:返回模版窗口,或NULL(失败)
/////////////////////////////////////////////////////////
CWnd* CMainFrame::GetToolModule()
{
	return m_wndExplorerBar.GetToolWnd(EXPBAR_MODULE);
}

///////////////////////////////////////////////////////////
//显示填充专家窗口
///////////////////////////////////////////////////////////
void CMainFrame::OnViewDynamicExpert() 
{
	ShowControlBar(&m_wndDynamicExpert, !m_wndDynamicExpert.IsVisible(), FALSE);
	m_wndDynamicExpert.UpdateWindow();

//	m_wndDynamicExpert.PostMessage(CDevApp::m_nSelectChangedMsg, NULL, 0);
 	m_wndDynamicExpert.OnSelectChangedMsg(NULL, 0);
}

void CMainFrame::OnUpdateViewDynamicExpert(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndDynamicExpert.IsVisible());
}

void CMainFrame::OnViewAppearExpert() 
{
	ShowControlBar(&m_wndAppearExpert, !m_wndAppearExpert.IsVisible(), FALSE);
	m_wndAppearExpert.OnSelectChangedMsg(NULL, 0);
}

void CMainFrame::OnUpdateViewAppearExpert(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndAppearExpert.IsVisible());	
}

void CMainFrame::OnViewActionExpert() 
{
	ShowControlBar(&m_wndActionExpert, !m_wndActionExpert.IsVisible(), FALSE);
	m_wndActionExpert.OnSelectChangedMsg(NULL, 0);
}

void CMainFrame::OnUpdateViewActionExpert(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndActionExpert.IsVisible());	
}

void CMainFrame::FirstPosition()
{
	CPoint point(100, 60);
	FloatControlBar(&m_wndAppearExpert, point);
	FloatControlBar(&m_wndDynamicExpert, point);
	FloatControlBar(&m_wndActionExpert, point);

	ShowControlBar(&m_wndAppearExpert, FALSE, FALSE);
	ShowControlBar(&m_wndDynamicExpert, FALSE, FALSE);	
	ShowControlBar(&m_wndActionExpert, FALSE, FALSE);	

	DelayRecalcLayout(TRUE);

//	CRect rectTemp;
//	m_wndToolBar.GetWindowRect(&rectTemp);
//	ScreenToClient(&rectTemp);
//	rectTemp.left = 400;
//	rectTemp.right = rectTemp.left;
//	(rcClient.left, rcClient.top, rcClient.left, rcClient.top);
//	DockControlBar(&m_wndLayoutBar, AFX_IDW_DOCKBAR_TOP, &rectTemp);

	CRect rcWindow;
	GetWindowRect(&rcWindow);
//	ClientToScreen(&rcWindow);
	CRect rect(rcWindow.left, rcWindow.top, rcWindow.left + 200, rcWindow.top + 100);

	DockControlBar(&m_wndLayoutBar, AFX_IDW_DOCKBAR_TOP, &rect);
	DockControlBar(&m_wndToolBar, AFX_IDW_DOCKBAR_TOP, &rect);

	DockControlBar(&m_wndExplorerBar, AFX_IDW_DOCKBAR_LEFT, &rect);
	rect.top = rcWindow.top + 100;
	rect.bottom = rcWindow.bottom;
	DockControlBar(&m_wndPropertyBar, AFX_IDW_DOCKBAR_LEFT, &rect);
	m_wndLayoutBar.GetWindowRect(&rect);

	RecalcLayout();
}

void CMainFrame::OnViewMousePosChange(CPoint pt)
{
	CString strPos;
	strPos.Format("%d,%d", pt.x, pt.y);
	m_wndStatusBar.SetPaneText(1, strPos);
}

void CMainFrame::ViewCode(CDevObj* pObj, CView* pView, LPCTSTR lpszEvent)
{
	ASSERT(pView->IsKindOf(RUNTIME_CLASS(CLayoutView)));
	CFormDoc* pDoc = (CFormDoc *)pView->GetDocument();
	ASSERT(pDoc != NULL);

	CString strName;
	if (pObj != NULL)
		strName = pObj->GetDisplayName();
	else
		strName = pDoc->GetClassName();

	CScriptView* pScriptView = NULL;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		pScriptView = (CScriptView *)(pDoc->GetNextView(pos));
		if (pScriptView->IsKindOf(RUNTIME_CLASS(CScriptView)))
		{
			CScriptFrame* pFrame = (CScriptFrame *)pScriptView->GetParentFrame();
			BOOL maximized;
			MDIGetActive(&maximized);
			pFrame->ActivateFrame(maximized ? SW_SHOW : SW_SHOWNORMAL);
			pScriptView->EditObjectEventHandler(strName, lpszEvent);
			return;
		}
	}

	CScriptFrame* pNewFrame
		= (CScriptFrame *)(theApp.m_pScriptTemplate->CreateNewFrame(pDoc, NULL));
	if (pNewFrame == NULL)
		return;     // not created
	ASSERT_KINDOF(CMDIChildWnd, pNewFrame);
	theApp.m_pScriptTemplate->InitialUpdateFrame(pNewFrame, pDoc);
	((CScriptView*)pNewFrame->GetActiveView())->EditObjectEventHandler(strName, lpszEvent);
}

void CMainFrame::OnViewStand() 
{
	BOOL bShow = m_wndToolBar.IsWindowVisible();
	ShowControlBar(&m_wndToolBar, !bShow, FALSE);
}

void CMainFrame::OnUpdateViewStand(CCmdUI* pCmdUI) 
{
	BOOL bShow = m_wndToolBar.IsWindowVisible();
	pCmdUI->SetCheck(bShow);
}

void CMainFrame::OnViewLayout() 
{
	BOOL bShow = m_wndLayoutBar.IsWindowVisible();
	ShowControlBar(&m_wndLayoutBar, !bShow, FALSE);
}

void CMainFrame::OnUpdateViewLayout(CCmdUI* pCmdUI) 
{
	BOOL bShow = m_wndLayoutBar.IsWindowVisible();
	pCmdUI->SetCheck(bShow);
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

void CMainFrame::OnAddControl() 
{
	CBoxCtrl& ctrlBox = m_wndToolBox.m_wndBoxCtrl;
	if (ctrlBox.GetFolderCount() < 2)
		ctrlBox.OnAddFolder();
	if (ctrlBox.GetFolderCount() < 2)
		return;

	if (ctrlBox.GetSelFolder() == 0)
		ctrlBox.SetSelFolder(1);

	ctrlBox.OnAddItem();
}

void CMainFrame::OnGridSetting() 
{
	CGridSettingDlg	dlg;
	if (dlg.DoModal() == IDOK)
	{
		SendMessageToDescendants(CXMSG_GRID_SETTING_CHANGED, 0, 0, TRUE, FALSE);
	}
}

#define _AfxGetDlgCtrlID(hWnd)          ((UINT)(WORD)::GetDlgCtrlID(hWnd))

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

BOOL CMainFrame::OnToolTipText(UINT msg, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);
	UNUSED(pNMHDR);

	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;

	UINT nID = pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = _AfxGetDlgCtrlID((HWND)nID);
	}

	BOOL bHandled = FALSE;
	CString strTipText;

	CMDIChildWnd* pMDIActive = MDIGetActive();
	if (pMDIActive != NULL && pMDIActive->GetActiveView()->IsKindOf(RUNTIME_CLASS(CLayoutView)))
	{
		CDocument* pActiveDoc = pMDIActive->GetActiveDocument();
	
		if (pActiveDoc->IsKindOf(RUNTIME_CLASS(CFormDoc)))
		{
			CFormDoc* pFormDoc = (CFormDoc *)pActiveDoc;

			if (nID == ID_EDIT_UNDO)
			{
				if (pFormDoc->m_stackUndo.CanUndo())
				{
					strTipText = _T("撤消");
					strTipText += pFormDoc->m_stackUndo.GetNextUndoCmdStr();
				}
				else
				{
					strTipText = _T("无法撤消");
				}
				
				bHandled = TRUE;
			}
			if (nID == ID_EDIT_REDO)
			{
				if (pFormDoc->m_stackUndo.CanRedo())
				{
					strTipText = _T("恢复");
					strTipText += pFormDoc->m_stackUndo.GetNextRedoCmdStr();
				}
				else
				{
					strTipText = _T("无法恢复");
				}
				
				bHandled = TRUE;
			}
		}
	}

	if (bHandled)
	{
#ifndef _UNICODE
		if (pNMHDR->code == TTN_NEEDTEXTA)
			lstrcpyn(pTTTA->szText, strTipText, _countof(pTTTA->szText));
		else
			_mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#else
		if (pNMHDR->code == TTN_NEEDTEXTA)
			_wcstombsz(pTTTA->szText, strTipText, _countof(pTTTA->szText));
		else
			lstrcpyn(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#endif
		*pResult = 0;

		return TRUE;
	}
		
	// otherwise, handle it ourselves
	return CMDIFrameWnd::OnToolTipText(msg, pNMHDR, pResult);
}

LONG CMainFrame::OnProjectOpendMsg(DWORD, LONG)
{
	if (theApp.GetIniStartPictureFolder(FALSE) != theApp.GetIniStartPictureFolder())
		theApp.NotifyDocumentChanged(0, 0, TRUE);

	return 0;
}

void CMainFrame::OnWindowClose() 
{
	CMDIChildWnd* pWnd = MDIGetActive();
	if (pWnd !=	NULL)
		pWnd->SendMessage(WM_CLOSE);
}

void CMainFrame::OnWindowCloseall() 
{
	theApp.NotifyDocumentChanged(0, 0, TRUE);
}

void CMainFrame::OnWindowNext() 
{
	::SendMessage(m_hWndMDIClient, WM_MDINEXT, 0, 0);
}

void CMainFrame::OnWindowPrevious() 
{
	::SendMessage(m_hWndMDIClient, WM_MDINEXT, 0, 1);
}

void CMainFrame::OnUpdateWindow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(MDIGetActive() != NULL);
}