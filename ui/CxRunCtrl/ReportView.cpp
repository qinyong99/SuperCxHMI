// ReportView.cpp: implementation of the CReportView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RunInc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CReportView, CPrintView)

BEGIN_MESSAGE_MAP(CReportView, CPrintView)
	//{{AFX_MSG_MAP(CReportView)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CPrintView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CPrintView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CPrintView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportView construction/destruction

CReportView::CReportView()
{
	
}

CReportView::~CReportView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CReportView diagnostics

#ifdef _DEBUG
void CReportView::AssertValid() const
{
	CPrintView::AssertValid();
}

void CReportView::Dump(CDumpContext& dc) const
{
	CPrintView::Dump(dc);
}

CReportDoc* CReportView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CReportDoc)));
	return (CReportDoc *)m_pDocument;
}
#endif //_DEBUG

BOOL CReportView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CPrintView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CReportView drawing
void CReportView::OnDraw(CDC* pDC)
{
	CReportDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

//	Graphics graphics(pDC->GetSafeHdc());
//	graphics.SetPageUnit(UnitPixel);
//	Pen pen(Color(0, 0, 0), 10);
//	graphics.DrawRectangle(&pen, Rect(100, 100, 200, 200));

//	pDoc->DrawDoc(pDC);
}

void CReportView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CPrintView::OnPrepareDC(pDC, pInfo);

	pDC->SetMapMode(MM_ANISOTROPIC);

	HDC hDCScreen = ::GetDC(NULL);
	ASSERT(hDCScreen != NULL);
	int cxPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSX);
	int cyPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSY);
	::ReleaseDC(NULL, hDCScreen);
		
	pDC->SetWindowExt(cxPixelsPerInch, cyPixelsPerInch);
	
	cxPixelsPerInch = pDC->GetDeviceCaps(LOGPIXELSX);
	cyPixelsPerInch = pDC->GetDeviceCaps(LOGPIXELSY);
	pDC->SetViewportExt(cxPixelsPerInch, cyPixelsPerInch);
}

/////////////////////////////////////////////////////////////////////////////
// CReportView printing

BOOL CReportView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CReportView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CReportDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	OnPrepareDC(pDC);

	pDoc->PreparePrint(pDC);
	pInfo->SetMaxPage(pDoc->m_nTotalPageCount);
		
	CPrintView::OnBeginPrinting(pDC, pInfo);
}

void CReportView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CPrintView::OnEndPrinting(pDC, pInfo);
}

void CReportView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CReportDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 避免被来自 AbortProc 的调用打断
	if (m_bPrinting)
		return;

	m_bPrinting = TRUE;

	pDoc->m_bPrint = !pInfo->m_bPreview;
	pDoc->DrawReportPage(pDC, pInfo->m_nCurPage);

	m_bPrinting = FALSE;
}
