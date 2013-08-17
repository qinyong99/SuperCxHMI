// SymbolPreviewWnd.cpp : implementation file
//

#include "stdafx.h"
#include "DevInc.h"
#include "SymbolPreviewWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSymbolPreviewWnd

CSymbolPreviewWnd::CSymbolPreviewWnd(CDevObj* pObj)
{
	m_pObject = pObj;
	m_pSelectObject = NULL;
}

CSymbolPreviewWnd::~CSymbolPreviewWnd()
{
}


BEGIN_MESSAGE_MAP(CSymbolPreviewWnd, CWnd)
	//{{AFX_MSG_MAP(CSymbolPreviewWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSymbolPreviewWnd message handlers

void CSymbolPreviewWnd::ReCalculatePosition()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.InflateRect(-15, -15);

	m_rcObject = m_pObject->GetPositionRect().GetRECT();

	if (rcClient.Width() * m_rcObject.Height() < m_rcObject.Width() * rcClient.Height())
	{
		m_rcDraw.left = rcClient.left;
		m_rcDraw.right = rcClient.right;
		int nHeight = MulDiv(rcClient.Width(), m_rcObject.Height(), m_rcObject.Width());
		m_rcDraw.top = rcClient.top + (rcClient.Height() - nHeight) / 2;
		m_rcDraw.bottom = m_rcDraw.top + nHeight;
	}
	else
	{
		m_rcDraw.top = rcClient.top;
		m_rcDraw.bottom = rcClient.bottom;
		int nWidth = MulDiv(rcClient.Height(), m_rcObject.Width(), m_rcObject.Height());
		m_rcDraw.left = rcClient.left + (rcClient.Width() - nWidth) / 2;
		m_rcDraw.right = m_rcDraw.left + nWidth;
	}
}

void CSymbolPreviewWnd::OnPrepareDC(CDC* pDC)
{
	pDC->SetMapMode(MM_ANISOTROPIC);

	pDC->SetViewportOrg(m_rcDraw.TopLeft());
	pDC->SetWindowOrg(m_rcObject.TopLeft());

	pDC->SetViewportExt(m_rcDraw.Width(), m_rcDraw.Height());
	pDC->SetWindowExt(m_rcObject.Width(), m_rcObject.Height());
}

void CSymbolPreviewWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);
	
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
		
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBmp = dcMem.SelectObject(&bmp);

	CRect rect = rcClient;
	dcMem.DrawEdge(&rect, EDGE_ETCHED, BF_RECT|BF_ADJUST);
	dcMem.FillSolidRect(&rect, ::GetSysColor(COLOR_3DFACE));

	OnPrepareDC(&dcMem);

	m_pObject->Draw(&dcMem);

	dcMem.SetViewportOrg(0, 0);
	dcMem.SetWindowOrg(0, 0);
	dcMem.SetMapMode(MM_TEXT);

	if (m_pSelectObject != NULL)
	{
		rect = m_pSelectObject->GetPositionRect().GetRECT();
		CClientDC dcClient(this);
		OnPrepareDC(&dcClient);
		dcClient.LPtoDP(&rect);

		CRect rcHandle(-4, -4, 4, 4);

		CRect rcT = rcHandle;
		rcT.OffsetRect(rect.left, rect.top);
		dcMem.Rectangle(&rcT);

		rcT = rcHandle;
		rcT.OffsetRect(rect.right, rect.top);
		dcMem.Rectangle(&rcT);

		rcT = rcHandle;
		rcT.OffsetRect(rect.right, rect.bottom);
		dcMem.Rectangle(&rcT);

		rcT = rcHandle;
		rcT.OffsetRect(rect.left, rect.bottom);
		dcMem.Rectangle(&rcT);
	}
	
	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	dcMem.SelectObject(pOldBmp);		// restore
}

int CSymbolPreviewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ReCalculatePosition();
	
	return 0;
}

void CSymbolPreviewWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	if (m_pObject->IsKindOf(RUNTIME_CLASS(CSymbolObj)))
	{
		CDevObjArray& arrObj = ((CSymbolObj *)m_pObject)->GetSubObjects();
		CDevObj* pObj;
		for (int i = arrObj.GetSize() - 1; i >= 0 ; i--)
		{
			pObj = arrObj[i];
			if (pObj->HitTest(point, NULL))
			{
				GetParent()->SendMessage(WM_OBJECT_SELECTED, (WPARAM)pObj, 0L);
				break;
			}
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CSymbolPreviewWnd::SetSelectObject(CDevObj* pObj)
{
	if (m_pSelectObject != pObj)
	{
		m_pSelectObject = pObj;
		Invalidate();
	}
}

BOOL CSymbolPreviewWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
