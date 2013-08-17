// BitmapView.cpp: implementation of the CBitmapView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RunInc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CBitmapView, CPrintView)

BEGIN_MESSAGE_MAP(CBitmapView, CPrintView)
	//{{AFX_MSG_MAP(CBitmapView)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CPrintView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CPrintView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CPrintView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmapView construction/destruction

CBitmapView::CBitmapView()
{
}

CBitmapView::~CBitmapView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CBitmapView diagnostics

#ifdef _DEBUG
void CBitmapView::AssertValid() const
{
	CPrintView::AssertValid();
}

void CBitmapView::Dump(CDumpContext& dc) const
{
	CPrintView::Dump(dc);
}

CBitmapDoc* CBitmapView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBitmapDoc)));
	return (CBitmapDoc *)m_pDocument;
}
#endif //_DEBUG

BOOL CBitmapView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CPrintView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBitmapView drawing
void CBitmapView::OnDraw(CDC* pDC)
{
	CBitmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	BITMAP bm;
	pDoc->m_bitmap.GetBitmap(&bm);

	CPoint pt;
	CRect rectMargin;
	CRect rectPage;

	pDC->Escape(GETPRINTINGOFFSET, 0, NULL, &pt);
	rectMargin.left = pt.x;
	rectMargin.top = pt.y;
	
	pDC->Escape(GETPHYSPAGESIZE, 0, NULL, &pt);
	rectPage.SetRect(0, 0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	
	rectMargin.right  = pt.x                     // total paper width
		- pDC->GetDeviceCaps(HORZRES) // printable width
		- rectMargin.left;   // left unprtable margin
	
	rectMargin.bottom = pt.y                     // total paper height
		- pDC->GetDeviceCaps(VERTRES) // printable ht
		- rectMargin.top;    // rt unprtable margin
	
	pt.x = pDC->GetDeviceCaps(LOGPIXELSX);    // dpi in X direction
	pt.y = pDC->GetDeviceCaps(LOGPIXELSY);    // dpi in Y direction
	
	rectMargin.left = MulDiv(pDoc->m_rcMargin.left, pt.x, 1440)
		- rectMargin.left;
	rectMargin.top  = MulDiv(pDoc->m_rcMargin.top, pt.y, 1440)
		- rectMargin.top;
	rectMargin.right  = MulDiv(pDoc->m_rcMargin.right, pt.x, 1440)
		- rectMargin.right;
	rectMargin.bottom = MulDiv(pDoc->m_rcMargin.bottom, pt.y, 1440)
		- rectMargin.bottom;

	rectPage.InflateRect(-rectMargin.left, -rectMargin.top, -rectMargin.right, -rectMargin.bottom);

	float fXScale;
	float fYScale;
	float fScale;

	if (pDoc->m_fHorizontalScale == 0.0f || pDoc->m_fVerticalScale == 0.0f)
	{
		fXScale = (float)rectPage.Width() / bm.bmWidth;
		fYScale = (float)rectPage.Height() / bm.bmHeight;
		
		fScale = min(fXScale, fYScale); // max(min(nXScale, nYScale), 1);
		if (pDoc->m_bMultipleScale)
			fScale = float(int(fScale));

		fXScale = fScale;
		fYScale = fScale;
	}
	else
	{
		fXScale = pDoc->m_fHorizontalScale;
		fYScale = pDoc->m_fVerticalScale;
	}
	
	int cxBlt = int(bm.bmWidth * fXScale);
	int cyBlt = int(bm.bmHeight * fYScale);
	int xOff = (rectPage.left + rectPage.right) / 2 - cxBlt / 2;
	int yOff = (rectPage.top + rectPage.bottom) / 2 - cyBlt / 2;
	
	CDC dc;
	if (dc.CreateCompatibleDC(pDC))
	{
		// Paint the image.
		CBitmap* pOldBitmap = dc.SelectObject(&pDoc->m_bitmap);
		pDC->StretchBlt(xOff, yOff, cxBlt, cyBlt, &dc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	
		dc.SelectObject(pOldBitmap);
	}
}

void CBitmapView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CPrintView::OnPrepareDC(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CBitmapView printing

BOOL CBitmapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}
