// ChildFrm.cpp : implementation of the CLayoutFrame class
//

#include "stdafx.h"
#include "DevInc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLayoutFrame

IMPLEMENT_DYNCREATE(CLayoutFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CLayoutFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CLayoutFrame)
	ON_WM_CLOSE()
	ON_WM_MDIACTIVATE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayoutFrame construction/destruction

CLayoutFrame::CLayoutFrame()
{
	// TODO: add member initialization code here
	m_pMiddleWnd = NULL;
}

CLayoutFrame::~CLayoutFrame()
{
}

BOOL CLayoutFrame::PreCreateWindow(CREATESTRUCT& cs)
{
//	CRect rcClient;
//	GetMDIFrame()->GetClientRect(&rcClient);
	
//	cs.x = rcClient.left;
//	cs.y = rcClient.top;
	cs.cx = theApp.m_settingMainForm.m_nWidth * 3 / 5;
	cs.cy = theApp.m_settingMainForm.m_nHeight * 3 / 4;

	if (!CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CLayoutFrame diagnostics

#ifdef _DEBUG
void CLayoutFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CLayoutFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLayoutFrame message handlers

///////////////////////////////////////////////////////////////
//功 能：重载此函数更新窗口标题
////////////////////////////////////////////////////////////////
void CLayoutFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// update our parent window first
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);

	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave child window alone!

	CDocument* pDocument = GetActiveDocument();
	if (pDocument == NULL)
		return;

	if (bAddToTitle)
	{
		CString strText;
		strText = pDocument->GetTitle();
		strText += _T("(UserForm)");
		AfxSetWindowText(m_hWnd, strText);
//		TCHAR szText[256+_MAX_PATH];
//		if (pDocument == NULL)
//			lstrcpy(szText, m_strTitle);
//		else
//			lstrcpy(szText, pDocument->GetTitle());
//		wsprintf(szText + lstrlen(szText), _T(":%d"), m_nWindow);
//		if (m_nWindow > 0)
//			wsprintf(szText + lstrlen(szText), _T(":%d"), m_nWindow);

		// set title if changed, but don't remove completely
//		AfxSetWindowText(m_hWnd, szText);
	}
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
void CLayoutFrame::OnClose() 
{
	if (m_lpfnCloseProc != NULL && !(*m_lpfnCloseProc)(this))
		return;

	CDocument* pActiveDoc = GetActiveDocument();
	if (!pActiveDoc->IsKindOf(RUNTIME_CLASS(CFormDoc)))
		return;

	CFormDoc* pFormDoc = (CFormDoc *)pActiveDoc;
	CDevDoc* pDoc = pFormDoc->m_pDocument;
	
	//提示是否保在内容
	if (pFormDoc->m_viewList.GetCount() == 1)
	{
		//提示是否保在内容
		if (pFormDoc->m_pFormObj == NULL && pFormDoc->IsModified() && 
			pFormDoc->m_viewList.GetCount() == 1)
		{
			int iRet = AfxMessageBox("是否保存画面内容？", MB_YESNOCANCEL | MB_ICONQUESTION);
			if (iRet == IDCANCEL)
				return;
			else if(iRet == IDNO)
			{	
				//不保存
				if (pDoc != NULL && pDoc->GetPathName() == "" &&
					pFormDoc->IsMainForm())
				{	
					//主窗口示保存, 画面文档也没有保存,则删除该文档
					pDoc->CloseDocument();
					return;
				}
				
				if (pDoc != NULL)
					pDoc->SetModifiedFlag(FALSE);
			}
			else
			{	
				if (pDoc != NULL)
					pDoc->SaveSubDocument(pFormDoc->m_pFormSubDoc);
			}
		}
		else
		{	
			if (pDoc != NULL && pDoc->GetPathName().Compare("") == 0 &&
				pFormDoc->IsMainForm())
			{	
				//删除该文档
				pDoc->CloseDocument();
				return;
			}
		}
	}
	
	pFormDoc->PreCloseFrame(this);

	// then destroy the window
	DestroyWindow();
}

void CLayoutFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	// 当画面中有控件对象时，原实现菜单不被激活，让 m_pNotifyHook 为空，可使菜单激活
	COleFrameHook* pNotifyHook = m_pNotifyHook;
	m_pNotifyHook = NULL;

	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	m_pNotifyHook = m_pNotifyHook;
	
	CLayoutView* pView = (CLayoutView* )GetActiveView();
	if (pView != NULL)
	{
		SetActiveView(pView);

		CFormDoc* pDoc = pView->GetDocument();
		if (pDoc != NULL && bActivate)
		{
			pDoc->OnActivate();
		}

		pView->m_bActive = bActivate;
		pView->SendMessage(WM_NCACTIVATE, (UINT)bActivate);
		pView->RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
	}
		
	if (pActivateWnd == NULL)
		theApp.NotifyDocumentActivate(0, 0);
}

BOOL CLayoutFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	BOOL bShowRuler = FALSE;

	if (pContext->m_pCurrentDoc != NULL && pContext->m_pCurrentDoc->IsKindOf(RUNTIME_CLASS(CFormDoc)))
	{	
		//如是主窗口,则无处理
		CFormDoc* pDoc = (CFormDoc*)(pContext->m_pCurrentDoc);
				
		if (pDoc->IsMainForm())
			return CMDIChildWnd::OnCreateClient(lpcs, pContext);

		if (pDoc->IsKindOf(RUNTIME_CLASS(CReportDoc)))
			bShowRuler = TRUE;
	}

	m_pMiddleWnd = new CMiddleWnd();
	if (m_pMiddleWnd == NULL)
		return FALSE;

	m_pMiddleWnd->m_bShowRuler = bShowRuler;
	if (!m_pMiddleWnd->Create(this))
	{
		delete m_pMiddleWnd;
		m_pMiddleWnd = NULL;
		return FALSE;
	}

	CLayoutView* pView = (CLayoutView*)(pContext->m_pNewViewClass->CreateObject());
	if (pView == NULL || !pView->Create(NULL, NULL, WS_CHILD, rectDefault, m_pMiddleWnd, 1, pContext))
	{
		m_pMiddleWnd->DestroyWindow();
		return FALSE;
	}

	m_pViewActive = pView;
	m_pMiddleWnd->m_pView = pView;

	return TRUE;
}


void CLayoutFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CMDIChildWnd::OnGetMinMaxInfo(lpMMI);

	CMainFrame* pMainFrame = (CMainFrame *)GetMDIFrame();
	if (pMainFrame->m_bFullScreenMode)
	{
		CRect rect;
		pMainFrame->GetClientRect(&rect);

		lpMMI->ptMaxSize.x = rect.Width();
        lpMMI->ptMaxSize.y = rect.Height();
        lpMMI->ptMaxTrackSize.x = rect.Width();
		lpMMI->ptMaxTrackSize.y = rect.Height();
	}
}
