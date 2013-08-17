// PreviewViewEx.cpp: implementation of the CPreviewViewEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RunInc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL CALLBACK _AfxPreviewCloseProc2(CFrameWnd* pFrameWnd);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPreviewViewEx, CPreviewView)

CPreviewViewEx::CPreviewViewEx()
{
}

CPreviewViewEx::~CPreviewViewEx()
{
}

BEGIN_MESSAGE_MAP(CPreviewViewEx, CPreviewView)
	//{{AFX_MSG_MAP(CPreviewViewEx)
	ON_COMMAND(AFX_ID_PREVIEW_PRINT, OnPreviewPrint)
	ON_COMMAND(AFX_ID_PREVIEW_CLOSE, OnPreviewClose)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewViewEx diagnostics

#ifdef _DEBUG
void CPreviewViewEx::AssertValid() const
{
	
}

void CPreviewViewEx::Dump(CDumpContext& dc) const
{

}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPreviewViewEx message handlers

void CPreviewViewEx::OnActivateView(BOOL bActivate, CView* pView1, CView *pView2)
{
	CPreviewView::OnActivateView(bActivate, pView1, pView2);
}

void CPreviewViewEx::OnPreviewClose()
{
	m_pToolBar->DestroyWindow();
	m_pToolBar = NULL;

	m_pPreviewInfo->m_nCurPage = m_nCurrentPage;
	((CPrintView*)m_pOrigView)->OnEndPrintPreview(m_pPreviewDC, m_pPreviewInfo,
									CPoint(0, 0), this);
}

void CPreviewViewEx::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPreviewView::OnEndPrintPreview(pDC, pInfo, point, pView);
}

void CPreviewViewEx::OnPreviewPrint()
{
	m_pPrintView->SendMessage(WM_COMMAND, ID_FILE_PRINT);
 	OnPreviewClose();               // force close of Preview
 
 // cause print (can be overridden by catching the command)
//	CWnd* pMainWnd = AfxGetThread()->m_pMainWnd;
//	ASSERT_VALID(pMainWnd);
//	pMainWnd->SendMessage(WM_COMMAND, ID_FILE_PRINT);
}

void CPreviewViewEx::OnDisplayPageNumber(UINT nPage, UINT nPagesDisplayed)
{
	UINT nEndPage = nPage + nPagesDisplayed - 1;
	
	CFrameWnd* pParent = GetParentFrame();
	ASSERT_VALID(pParent);
	ASSERT_KINDOF(CFrameWnd, pParent);
	
	int nSubString = (nPagesDisplayed == 1) ? 0 : 1;
	
	CString s;
	if (AfxExtractSubString(s, m_pPreviewInfo->m_strPageDesc, nSubString))
	{
		TCHAR szBuf[80];
		if (nSubString == 0)
			wsprintf(szBuf, s, nPage);
		else
			wsprintf(szBuf, _T("ตฺ %d-%d าณ"), nPage, nEndPage);

		if (m_pToolBar != NULL)
			m_pToolBar->SetDlgItemText(ID_PREVIEW_PAGE, szBuf);
		
//		pParent->SendMessage(WM_SETMESSAGESTRING, 0, (LPARAM)(LPVOID)szBuf);
	}
	else
	{
		TRACE1("Malformed Page Description string. Could not get string %d.\n",
			nSubString);
	}

}

void CPreviewViewEx::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
//	CPreviewView::OnUpdate(pSender, lHint, pHint);
}

