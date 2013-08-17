// MiddleWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cxdev.h"
#include "DevInc.h"
#include "MiddleWnd.h"
#include "ActionObj.h"

#include "FormDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMiddleWnd

IMPLEMENT_DYNCREATE(CMiddleWnd, CScrollView)

CMiddleWnd::CMiddleWnd()
{
	m_pView = NULL;
	m_bSelectView = TRUE;
	m_rectTracker.m_nHandleSize = 6;
	m_rectTracker.m_nStyle = CRectTracker::resizeOutside;

	m_bShowRuler = TRUE;
}

CMiddleWnd::~CMiddleWnd()
{
}


BEGIN_MESSAGE_MAP(CMiddleWnd, CScrollView)
	//{{AFX_MSG_MAP(CMiddleWnd)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SELECT_CHANGED, OnSelectChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiddleWnd drawing

void CMiddleWnd::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CMiddleWnd diagnostics

#ifdef _DEBUG
void CMiddleWnd::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMiddleWnd::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMiddleWnd message handlers
BOOL CMiddleWnd::Create(CWnd* pParent)
{
	RECT rect;
	pParent->GetClientRect(&rect);
	return CScrollView::Create(NULL, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE ,rect, pParent, AFX_IDW_PANE_FIRST, NULL);
}

/////////////////////////////////////////////////////////////////
//功能:设置Form窗口的大小,并改变本窗口的滚动范围
//参数:nWidth[in]		对应文档的宽度
//		nHeight[in]		对应文档的高度
/////////////////////////////////////////////////////////////////
void CMiddleWnd::SetFormSize(int nWidth, int nHeight)
{
	CRect rcWindow;
	m_pView->GetWindowRect(&rcWindow);
	rcWindow.right = rcWindow.left + nWidth;
	rcWindow.bottom = rcWindow.top + nHeight;
	
//	CRect rcClient;
//	m_pView->GetClientRect(&rcClient);
	
//	int nDx = nWidth - rcClient.Width();
//	int nDy = nHeight - rcClient.Height();
		
//	rcWindow.InflateRect(0, 0, nDx, nDy);

	CSize szTotal = rcWindow.Size();
	szTotal.cx += 48;
	szTotal.cy += 32;
	SetScrollSizes(MM_TEXT, szTotal);

	ScreenToClient(&rcWindow);

	CPoint ptOrigin = GetScrollPosition();
	int nOffset = m_bShowRuler ? 24 : 6;
	m_pView->SetWindowPos(NULL, nOffset - ptOrigin.x, nOffset - ptOrigin.y, 
			rcWindow.Width(), rcWindow.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	InvalidateRect(NULL);
}

BOOL CMiddleWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	BOOL bRet = CScrollView::PreCreateWindow(cs);
	cs.style |= WS_CLIPCHILDREN;

	return bRet;
}

int CMiddleWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	theApp.m_listSelectNotify.AddTail(m_hWnd);
	
	return 0;
}

void CMiddleWnd::OnDraw(CDC* pDC)
{
	if (m_bShowRuler)
	{
		DrawRuler(pDC);
	}
	else if (m_bSelectView)
	{
		CRect rect;
		
		m_pView->GetWindowRect(&rect);
		ScreenToClient(&rect);
		pDC->DPtoLP(&rect);
		m_rectTracker.m_rect = rect;
		m_rectTracker.m_rect.InflateRect(1, 1);

		//m_rectTracker.Draw(pDC);

		CBrush HighlightBrush(GetSysColor(COLOR_HIGHLIGHT));

		for (int i = 0; i < 8; ++i)
		{
			int nHit = (CRectTracker::TrackerHit)i;

			m_rectTracker.GetHandleRect(nHit, &rect);
			
			if (nHit == CRectTracker::hitBottomRight ||
					nHit == CRectTracker::hitRight ||
					nHit == CRectTracker::hitBottom)
				pDC->FillSolidRect(rect, GetSysColor(COLOR_HIGHLIGHT));
			else
				pDC->FrameRect(&rect, &HighlightBrush);
		}
	}
}

void CMiddleWnd::LogicToRuler(CPoint& point)
{
	HDC hDCScreen = ::GetDC(NULL);
	ASSERT(hDCScreen != NULL);
	int cxPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSX);
	int cyPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSY);
	::ReleaseDC(NULL, hDCScreen);

	point.x = MulDiv(point.x, 254, cxPixelsPerInch);
	point.y = MulDiv(point.y, 254, cxPixelsPerInch);
}

void CMiddleWnd::RulerToLogic(CPoint& point)
{
	HDC hDCScreen = ::GetDC(NULL);
	ASSERT(hDCScreen != NULL);
	int cxPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSX);
	int cyPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSY);
	::ReleaseDC(NULL, hDCScreen);
	
	point.x = MulDiv(point.x, cxPixelsPerInch, 254);
	point.y = MulDiv(point.y, cxPixelsPerInch, 254);	
}

void CMiddleWnd::LogicToRuler(CRect& rect)
{
	HDC hDCScreen = ::GetDC(NULL);
	ASSERT(hDCScreen != NULL);
	int cxPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSX);
	int cyPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSY);
	::ReleaseDC(NULL, hDCScreen);

	rect.left = MulDiv(rect.left, 254, cxPixelsPerInch);
	rect.top = MulDiv(rect.top, 254, cxPixelsPerInch);
	rect.right = MulDiv(rect.right, 254, cxPixelsPerInch);
	rect.bottom = MulDiv(rect.bottom, 254, cxPixelsPerInch);
}

void CMiddleWnd::RulerToLogic(CRect& rect)
{
	HDC hDCScreen = ::GetDC(NULL);
	ASSERT(hDCScreen != NULL);
	int cxPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSX);
	int cyPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSY);
	::ReleaseDC(NULL, hDCScreen);

	rect.left = MulDiv(rect.left, cxPixelsPerInch, 254);
	rect.top = MulDiv(rect.top, cxPixelsPerInch, 254);
	rect.right = MulDiv(rect.right, cxPixelsPerInch, 254);
	rect.bottom = MulDiv(rect.bottom, cxPixelsPerInch, 254);	
}

void CMiddleWnd::DrawNumbers(CDC* pDC, CRect rect, int nInc, int nTPU, BOOL bHorizontal)
{
	TCHAR buf[10];

	m_pView->ClientToDoc(rect);
	LogicToRuler(rect);
	
	if (bHorizontal)
	{
		for (int nX = nInc; nX < rect.Width() + nInc; nX += nInc)
		{
			wsprintf(buf, _T("%d"), nX/nTPU);
			int nLen = lstrlen(buf);
			
			CPoint point(nX + rect.left, (rect.top + rect.bottom) / 2);
			RulerToLogic(point);
			m_pView->DocToClient(point);
			
			CSize sz = pDC->GetTextExtent(buf, nLen);
			point.Offset(-sz.cx / 2, -sz.cy / 2);
			
			pDC->ExtTextOut(point.x, point.y, 0, NULL, buf, nLen, NULL);
		}	
	}
	else
	{
		for (int nY = nInc; nY < rect.Height() + nInc; nY += nInc)
		{
			wsprintf(buf, _T("%d"), nY/nTPU);
			int nLen = lstrlen(buf);
			
			CPoint point((rect.left + rect.right) / 2, nY + rect.top);
			RulerToLogic(point);
			m_pView->DocToClient(point);
			
			CSize sz = pDC->GetTextExtent(buf, nLen);
			point.Offset(-sz.cx / 2, -sz.cy / 2);
			
			pDC->ExtTextOut(point.x, point.y, 0, NULL, buf, nLen, NULL);
		}	
	}
}

void CMiddleWnd::DrawDiv(CDC* pDC, CRect rect, int nInc, int nLargeDiv, int nLength, BOOL bHorizontal)
{
	m_pView->ClientToDoc(rect);
	LogicToRuler(rect);

 	if (bHorizontal)
	{
		for (int nX = nInc; nX < rect.Width(); nX += nInc)
		{
			if (nX % nLargeDiv == 0)
				continue;
			
			CPoint point(nX + rect.left, (rect.top + rect.bottom) / 2);
			RulerToLogic(point);
			m_pView->DocToClient(point);
			
			point.y -= nLength / 2;
			pDC->MoveTo(point);
			
			point.y += nLength;
			pDC->LineTo(point);
		}
	}
	else
	{
		for (int nY = nInc; nY < rect.Height(); nY += nInc)
		{
			if (nY % nLargeDiv == 0)
				continue;
			
			CPoint point((rect.left + rect.right) / 2, nY + rect.top);
			RulerToLogic(point);
			m_pView->DocToClient(point);
			
			point.x -= nLength / 2;
			pDC->MoveTo(point);
			
			point.x += nLength;
			pDC->LineTo(point);
		}
	}
}

void CMiddleWnd::DrawRuler(CDC* pDC)
{
	CRect rect/*, rectSave*/, rcT;

	CPen pen(PS_SOLID, 0, GetSysColor(COLOR_WINDOWTEXT));
	CPen* pOldPen = pDC->SelectObject(&pen);
	
	CFont font;
	font.CreateStockObject(DEFAULT_GUI_FONT);
	CFont* pOldFont = pDC->SelectObject(&font);	
	
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);

	m_pView->GetWindowRect(&rect);
	ScreenToClient(&rect);

	// 边界
	rcT = rect;
	pDC->DPtoLP(rcT);
	pDC->MoveTo(rcT.left, rcT.bottom);
	pDC->LineTo(rcT.right, rcT.bottom);
	pDC->LineTo(rcT.right, rcT.top);

	CRect rectClient;
	GetClientRect(&rectClient);
	rect.right = rectClient.right;
	rect.bottom = rectClient.bottom;

	pDC->DPtoLP(&rect);

	rcT = rect;
	rcT.bottom = rcT.top;
	rcT.top = 0;
	rcT.InflateRect(0, -5, 0, -6);
	
	// 水平标尺
	DrawTickMarks(pDC, rcT, FALSE, TRUE);

	rcT = rect;
	rcT.right = rcT.left;
	rcT.left = 0;
	rcT.InflateRect(-5, 0, -6, 0);

	// 垂直标尺
	if (m_pView->IsKindOf(RUNTIME_CLASS(CReportView)))
	{
		CReportDoc* pReportDoc = (CReportDoc *)m_pView->GetDocument();
		CRect rcSection;
		int nTop = rcT.top; 
		BOOL bDiv = FALSE;
		
		if (pReportDoc->m_secReportHeader.m_bVisible)
		{
			rcSection = pReportDoc->GetSectionRect(&pReportDoc->m_secReportHeader).GetRECT();
			m_pView->DocToClient(rcSection);

			rcSection.top = rcT.top + rcSection.top;
			rcSection.bottom = rcT.top + rcSection.bottom;
			rcSection.left = rcT.left;
			rcSection.right = rcT.right;

			DrawTickMarks(pDC, rcSection, bDiv, FALSE);
			bDiv = TRUE;

			nTop = rcSection.bottom;
		}
		if (pReportDoc->m_secPageHeader.m_bVisible)
		{
			rcSection = pReportDoc->GetSectionRect(&pReportDoc->m_secPageHeader).GetRECT();
			m_pView->DocToClient(rcSection);
			
			rcSection.top = rcT.top + rcSection.top;
			rcSection.bottom = rcT.top + rcSection.bottom;
			rcSection.left = rcT.left;
			rcSection.right = rcT.right;
			
			DrawTickMarks(pDC, rcSection, bDiv, FALSE);
			bDiv = TRUE;

			nTop = rcSection.bottom;
		}
		if (pReportDoc->m_secDetail.m_bVisible)
		{
			rcSection = pReportDoc->GetSectionRect(&pReportDoc->m_secDetail).GetRECT();
			m_pView->DocToClient(rcSection);
			
			rcSection.top = rcT.top + rcSection.top;
			rcSection.bottom = rcT.top + rcSection.bottom;
			rcSection.left = rcT.left;
			rcSection.right = rcT.right;
			
			DrawTickMarks(pDC, rcSection, bDiv, FALSE);
			bDiv = TRUE;

			nTop = rcSection.bottom;
		}
		if (pReportDoc->m_secPageFooter.m_bVisible)
		{
			rcSection = pReportDoc->GetSectionRect(&pReportDoc->m_secPageFooter).GetRECT();
			m_pView->DocToClient(rcSection);
			
			rcSection.top = rcT.top + rcSection.top;
			rcSection.bottom = rcT.top + rcSection.bottom;
			rcSection.left = rcT.left;
			rcSection.right = rcT.right;
			
			DrawTickMarks(pDC, rcSection, bDiv, FALSE);
			bDiv = TRUE;

			nTop = rcSection.bottom;
		}
		if (pReportDoc->m_secReportFooter.m_bVisible)
		{
			rcSection = pReportDoc->GetSectionRect(&pReportDoc->m_secReportFooter).GetRECT();
			m_pView->DocToClient(rcSection);
			
			rcSection.top = rcT.top + rcSection.top;
			rcSection.bottom = rcT.top + rcSection.bottom;
			rcSection.left = rcT.left;
			rcSection.right = rcT.right;
			
			DrawTickMarks(pDC, rcSection, bDiv, FALSE);
			bDiv = TRUE;

			nTop = rcSection.bottom;
		}

		rcT.top = nTop;
		DrawTickMarks(pDC, rcT, bDiv, FALSE);
	}
	else
	{
		DrawTickMarks(pDC, rcT, FALSE, FALSE);
	}

	// 分界线
	DrawAllCaps(pDC, rect);

	rcT.left = 0;
	rcT.top = 0;
	rcT.right = rect.left;
	rcT.bottom = rect.top;

	CBrush brush(::GetSysColor(COLOR_BTNFACE));
	rcT.InflateRect(-3, -3, -3, -3);
	pDC->FillRect(rcT, &brush);
	rcT.InflateRect(-1, -1, -1, -1);
	pDC->DrawEdge(&rcT, BDR_SUNKENOUTER, BF_RECT);
	
	rcT.InflateRect(-5, -5);
	if (m_bSelectView)
	{
	//	CBrush brushHighlight(GetSysColor(COLOR_WINDOWTEXT));
	//	pDC->FrameRect(&rcT, &brushHighlight);
		pDC->FillSolidRect(&rcT, GetSysColor(COLOR_WINDOWTEXT));
	}

	pDC->SetBkMode(nOldBkMode);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
}

void CMiddleWnd::DrawAllCaps(CDC* pDC, CRect rect)
{
	CRect rcT;

	rcT = rect;
	rcT.left = 0;
	rcT.right += 2;
	rcT.bottom = rcT.top;
	rcT.top = rcT.bottom - 6;
	DrawGap(pDC, rcT, 0);

	rcT.OffsetRect(0, -rcT.top - 1);
	DrawGap(pDC, rcT, 0);
	
	rcT = rect;
	rcT.top = 0;
	rcT.bottom += 2;
	rcT.right = rcT.left;
	rcT.left = rcT.right - 6;
	DrawGap(pDC, rcT, 0);

	rcT.OffsetRect(-rcT.left - 1, 0);
	DrawGap(pDC, rcT, 0);
	
	rcT = rect;
	rcT.left = 0;
	rcT.right += 2;
	rcT.bottom = rcT.top;
	rcT.top = rcT.bottom - 6;
	DrawGap(pDC, rcT, 1);

	rcT.OffsetRect(0, -rcT.top - 1);
	DrawGap(pDC, rcT, 1);
	
	rcT = rect;
	rcT.top = 0;
	rcT.bottom += 2;
	rcT.right = rcT.left;
	rcT.left = rcT.right - 6;
	DrawGap(pDC, rcT, 1);

	rcT.OffsetRect(-rcT.left - 1, 0);
	DrawGap(pDC, rcT, 1);
	
	rcT = rect;
	rcT.left = 0;
	rcT.right += 2;
	rcT.bottom = rcT.top;
	rcT.top = rcT.bottom - 6;
	DrawGap(pDC, rcT, 2);

	rcT.OffsetRect(0, -rcT.top - 1);
	DrawGap(pDC, rcT, 2);
	
	rcT = rect;
	rcT.top = 0;
	rcT.bottom += 2;
	rcT.right = rcT.left;
	rcT.left = rcT.right - 6;
	DrawGap(pDC, rcT, 2);

	rcT.OffsetRect(-rcT.left - 1, 0);
	DrawGap(pDC, rcT, 2);
}

void CMiddleWnd::DrawGap(CDC* pDC, CRect rect, int nBar)
{
	switch (nBar) 
	{
	case 0:
		pDC->Draw3dRect(rect, ::GetSysColor(COLOR_BTNFACE), ::GetSysColor(COLOR_WINDOWFRAME));
		break;
	case 1:
		rect.InflateRect(-1, -1);
		pDC->Draw3dRect(rect, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_BTNSHADOW));
		break;
	case 2:
		rect.InflateRect(-2, -2);
		pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));
		break;
	default:
		ASSERT(FALSE);
	}
}

void CMiddleWnd::DrawTickMarks(CDC* pDC, CRect rect, BOOL bDiv, BOOL bHorizontal)
{
	DrawFace(pDC, rect, bDiv, bHorizontal);
	DrawDiv(pDC, rect, 25, 100, 2, bHorizontal);
	DrawDiv(pDC, rect, 50, 100, 5, bHorizontal);
	DrawNumbers(pDC, rect, 100, 100, bHorizontal);
}

void CMiddleWnd::DrawFace(CDC* pDC, CRect rect, BOOL bDiv, BOOL bHorizontal)
{
	CBrush brush(RGB(255, 255, 255));
	pDC->FillRect(rect, &brush);

	if (bDiv)
	{
		if (bHorizontal)
		{
			pDC->MoveTo(rect.left, rect.top);
			pDC->LineTo(rect.left, rect.bottom);
		}
		else
		{
			pDC->MoveTo(rect.left, rect.top);
			pDC->LineTo(rect.right, rect.top);
		}
	}
}

BOOL CMiddleWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CClientDC dc(this);

	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	dc.DPtoLP(&point);
	
	CRect rcWindow;
	m_pView->GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);
	dc.DPtoLP(&rcWindow);

	if (m_bShowRuler)
	{
		if (abs(point.x - rcWindow.right) < 2)
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		else if (abs(point.y - rcWindow.bottom) < 2)
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		else
			return CScrollView::OnSetCursor(pWnd, nHitTest, message);

		return TRUE;
	}

	m_rectTracker.m_rect = rcWindow;
	m_rectTracker.m_rect.InflateRect(1, 1);

	if (m_pView->m_selection.IsEmpty())
	{
		int htType = m_rectTracker.HitTest(point);
		if (htType == CRectTracker::hitBottomRight ||
			htType == CRectTracker::hitRight ||
			htType == CRectTracker::hitBottom)
		{
			return m_rectTracker.SetCursor(this, nHitTest);
		}
	}
	
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CMiddleWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bShowRuler)
	{
		ClientToScreen(&point);
		m_pView->ScreenToClient(&point);
		if (!m_pView->DoSpecialLButtonDown(nFlags, point))
		{
			m_pView->Select(NULL);
			if (m_pView->IsKindOf(RUNTIME_CLASS(CReportView)))
				((CReportView *)m_pView)->SelectSection(NULL);
		}
		
		return;
	}

	if (m_pView->m_selection.IsEmpty())
	{
		CRect rcWindow;
		m_pView->GetWindowRect(&rcWindow);
		ScreenToClient(&rcWindow);
		CClientDC dc(this);
		dc.DPtoLP(&rcWindow);
		m_rectTracker.m_rect = rcWindow;
		m_rectTracker.m_rect.InflateRect(1, 1);
				
		int htType = m_rectTracker.HitTest(point);
		if (htType == CRectTracker::hitBottomRight ||
			htType == CRectTracker::hitRight ||
			htType == CRectTracker::hitBottom)
		{
			if ((m_pView->GetStyle() & WS_CAPTION) != 0)
			{
				m_rectTracker.m_sizeMin.cx = GetSystemMetrics(SM_CXMINTRACK) + 2;
				m_rectTracker.m_sizeMin.cy = GetSystemMetrics(SM_CYMINTRACK) + 2;
			}
			
			ModifyStyle(WS_CLIPCHILDREN, 0);
			BOOL b = m_rectTracker.Track(this, point);
			ModifyStyle(0, WS_CLIPCHILDREN);
			
			if (b)
			{
				m_rectTracker.m_rect.InflateRect(-1, -1);
				
				CSize sizeDoc;
				sizeDoc = m_rectTracker.m_rect.Size();
				
				CFormDoc* pFromDoc = m_pView->GetDocument();
				
				CSizeF szOld;
				szOld.cx = pFromDoc->GetWidth();
				szOld.cy = pFromDoc->GetHeight();
				
				pFromDoc->SetDocSize((float)sizeDoc.cx, (float)sizeDoc.cy);
				
				pFromDoc->SetModifiedFlag();
				if (m_bSelectView)
					theApp.NotifySelectChanged(FALSE, 0, TRUE);
				
				CSizeF szNew;
				szNew.cx = pFromDoc->GetWidth();
				szNew.cy = pFromDoc->GetHeight();
				CActionDocSize* pAct = new CActionDocSize(pFromDoc, szNew, szOld);
				
				pFromDoc->m_stackUndo.Push(pAct);
			}
		}
	}
	else
	{
		m_pView->Select(NULL);
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

int CMiddleWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	int nResult = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	if (nResult == MA_NOACTIVATE || nResult == MA_NOACTIVATEANDEAT)
		return nResult;   // frame does not want to activate
	
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame != NULL)
	{
		// eat it if this will cause activation
		ASSERT(pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame));
		
		// either re-activate the current view, or set this view to be active
		CView* pView = pParentFrame->GetActiveView();
		HWND hWndFocus = ::GetFocus();
		if (pView == this &&
			m_hWnd != hWndFocus && !::IsChild(m_hWnd, hWndFocus))
		{
			// re-activate this view
			OnActivateView(TRUE, this, this);
		}
		else
		{
			// activate this view
			pParentFrame->SetActiveView(m_pView);
		}
	}

	return nResult;
}

LRESULT CMiddleWnd::OnSelectChanged(WPARAM wParam, LPARAM lParam)
{
	m_bSelectView = BOOL(wParam);

	if (m_bShowRuler)
	{
		CClientDC dc(this);

		CRect rect(0, 0, 24, 24);
		dc.LPtoDP(rect);
		InvalidateRect(&rect);
	}
	else
	{
		CRect rcWindow;
		m_pView->GetWindowRect(&rcWindow);
		ScreenToClient(&rcWindow);
		
		rcWindow.InflateRect(6, 6);
		InvalidateRect(&rcWindow);
	}
	
	return 0;
}

BOOL CMiddleWnd::OnEraseBkgnd(CDC* pDC) 
{
	ASSERT_VALID(pDC);
	
	// Fill background with APPWORKSPACE
	CBrush backBrush(GetSysColor(COLOR_APPWORKSPACE));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);     // Erase the area needed

	if (m_bShowRuler)
	{
		CRect rcWindow;
		m_pView->GetWindowRect(&rcWindow);
		ScreenToClient(&rcWindow);
		pDC->DPtoLP(&rcWindow);
		if (rect.left < rcWindow.left)
			rect.left = rcWindow.left;
		if (rect.top < rcWindow.top)
			rect.top = rcWindow.top;
	}
		
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}

