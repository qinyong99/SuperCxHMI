// PrintView.cpp : implementation file
//

#include "stdafx.h"
#include "RunInc.h"
#include "RunCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintView

IMPLEMENT_DYNCREATE(CPrintView, CView)

CPrintView::CPrintView()
{
	m_pRunCtrl = NULL;
	m_pView = NULL;

	m_bPrinting = FALSE;
}

CPrintView::~CPrintView()
{
}


BEGIN_MESSAGE_MAP(CPrintView, CView)
	//{{AFX_MSG_MAP(CPrintView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintView drawing

void CPrintView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CPrintView diagnostics

#ifdef _DEBUG
void CPrintView::AssertValid() const
{
	CView::AssertValid();
}

void CPrintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CReportView printing

BOOL CPrintView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	if (m_pView != NULL)
		return m_pView->DoPreparePrinting(pInfo);
	
	return DoPreparePrinting(pInfo);
}

void CPrintView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_pView != NULL)
	{
		m_pView->OnBeginPrinting(pDC, pInfo);
		return;
	}

	CView::OnBeginPrinting(pDC, pInfo);
}

void CPrintView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_pView != NULL)
	{
		m_pView->OnEndPrinting(pDC, pInfo);
		return;
	}

	CView::OnEndPrinting(pDC, pInfo);
}

void CPrintView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_pView != NULL)
	{
		m_pView->OnPrint(pDC, pInfo);
		return;
	}
	
	CView::OnPrint(pDC, pInfo);
}

BOOL CALLBACK _AfxPreviewCloseProc2(CFrameWnd* pFrameWnd)
{
	ASSERT_VALID(pFrameWnd);
	
	CPreviewViewEx* pView = (CPreviewViewEx*) pFrameWnd->GetDlgItem(AFX_IDW_PANE_FIRST);
	ASSERT_KINDOF(CPreviewView, pView);
	
	pView->OnPreviewClose();
	return FALSE;
}

void CPrintView::OnFilePrintPreview(CRunCtrl* pRunCtrl, CRunView* pView)
{
	// In derived classes, implement special window handling here
	// Be sure to Unhook Frame Window close if hooked.
	m_pRunCtrl = pRunCtrl;
	m_pView = pView;

	// must not create this on the frame.  Must outlive this function
	CPrintPreviewState* pState = new CPrintPreviewState;
	pState->lpfnCloseProc = _AfxPreviewCloseProc2;

	// DoPrintPreview's return value does not necessarily indicate that
	// Print preview succeeded or failed, but rather what actions are necessary
	// at this point.  If DoPrintPreview returns TRUE, it means that
	// OnEndPrintPreview will be (or has already been) called and the
	// pState structure will be/has been deleted.
	// If DoPrintPreview returns FALSE, it means that OnEndPrintPreview
	// WILL NOT be called and that cleanup, including deleting pState
	// must be done here.

	if (!DoPrintPreview(IDD_PREVIEW_TOOLBAR, this,
							RUNTIME_CLASS(CPreviewViewEx), pState))
	{
		// In derived classes, reverse special window handling here for
		// Preview failure case

		TRACE0("Error: DoPrintPreview failed.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		delete pState;      // preview failed to initialize, delete State now
	}
}

// OnEndPrintPreview is here for swap tuning reasons
//  (see viewprev.cpp for complete preview mode implementation)
void CPrintView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo,
			POINT point, CPreviewView* pView)
{
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
  
	m_pRunCtrl->m_bPrintPreview = FALSE;
	GetParentFrame()->EndModalLoop(0);
}

