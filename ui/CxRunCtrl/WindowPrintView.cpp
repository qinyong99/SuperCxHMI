// WindowPrintView.cpp: implementation of the CWindowPrintView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WindowPrintView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CWindowPrintView, CPrintView)

BEGIN_MESSAGE_MAP(CWindowPrintView, CPrintView)
	//{{AFX_MSG_MAP(CWindowPrintView)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CPrintView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CPrintView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CPrintView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowPrintView construction/destruction

CWindowPrintView::CWindowPrintView()
{
				
}

CWindowPrintView::~CWindowPrintView()
{
	
}

/////////////////////////////////////////////////////////////////////////////
// CWindowPrintView diagnostics

#ifdef _DEBUG
void CWindowPrintView::AssertValid() const
{
	CPrintView::AssertValid();
}

void CWindowPrintView::Dump(CDumpContext& dc) const
{
	CPrintView::Dump(dc);
}

#endif //_DEBUG

BOOL CWindowPrintView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CPrintView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWindowPrintView drawing
void CWindowPrintView::OnDraw(CDC* pDC)
{
}

void CWindowPrintView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
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
// CWindowPrintView printing

BOOL CWindowPrintView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CWindowPrintView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
		
	CPrintView::OnBeginPrinting(pDC, pInfo);
}

void CWindowPrintView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CPrintView::OnEndPrinting(pDC, pInfo);
}

void CWindowPrintView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	
}
