// Slider.cpp: implementation of the CSlider class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Slider.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSlider::CSlider()
{
	m_nMin = m_nMax = m_nCur = 0;
	m_nTracking = Slider_Nowhere;
	m_bPressed = FALSE;
	m_nTrackOffset = 0;
	m_nCur = 0;
}

CSlider::~CSlider()
{

}

LRESULT CSlider::OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
//	CDCHandle dc = (HDC)wParam;
//	CRect rcClient;
//	GetClientRect(&rcClient);
//	dc.FillRect(&rcClient, COLOR_3DFACE);


	return 1;   // no background needed
}

LRESULT CSlider::OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);
	
	CRect rect;
	int nArrowSize = 0;
	int nThumbSize = 0;
	int nThumbPos = 0;
	GetItemRect(&rect, &nArrowSize, &nThumbSize, &nThumbPos);
	DrawArrows(dc.m_hDC, rect, FALSE, nArrowSize, 
		m_bPressed && m_nTracking == Slider_Top_Arrow, m_bPressed && m_nTracking == Slider_Bottom_Arrow);
	DrawInterior(dc.m_hDC, rect, nArrowSize, nThumbSize, nThumbPos, FALSE);
		
	return 0;
}

void CSlider::DrawArrows(CDCHandle dc, CRect rect, BOOL bVertical,
		int nArrowSize, BOOL bTopPressed, BOOL bBottomPressed)
{
	CRect r = rect;
	if (bVertical)
		r.bottom = r.top + nArrowSize;
	else
		r.right = r.left + nArrowSize;
	
	DrawFrameControl(dc, &r, DFC_SCROLL,
		(bVertical ? DFCS_SCROLLUP : DFCS_SCROLLLEFT) | 
		(bTopPressed ? (DFCS_PUSHED | DFCS_FLAT) : 0 ));
	
	r = rect;
	if (bVertical)
		r.top = r.bottom - nArrowSize;
	else
		r.left = r.right - nArrowSize;
	
	DrawFrameControl(dc, &r, DFC_SCROLL,
		(bVertical ? DFCS_SCROLLDOWN : DFCS_SCROLLRIGHT) | 
		(bBottomPressed ? (DFCS_PUSHED | DFCS_FLAT) : 0));
}

void CSlider::DrawInterior(CDCHandle dc, CRect rect, int nArrowSize,
		int nThumbSize, int nThumbPos, BOOL bVertical)
{
	CRect r = rect;
	if (bVertical)
	{
		r.top += nArrowSize;
		r.bottom -= nArrowSize;
	}
	else
	{
		r.left += nArrowSize;
		r.right -= nArrowSize;
	}

	COLORREF clrTextOld = dc.SetTextColor(::GetSysColor(COLOR_BTNFACE));
	COLORREF clrBkOld = dc.SetBkColor(::GetSysColor(COLOR_BTNHILIGHT));
	CBrush brush(CDCHandle::GetHalftoneBrush());
	dc.SetBrushOrg(r.left, r.top);
	dc.FillRect(&r, brush);
	dc.SetTextColor(clrTextOld);
	dc.SetBkColor(clrBkOld);

	if (bVertical)
	{
		r.top = nThumbPos;
		r.bottom = r.top + nThumbSize;
	}
	else
	{
		r.left = nThumbPos;
		r.right = r.left + nThumbSize;
	}

	DrawEdge(dc, &r, EDGE_RAISED, BF_RECT|BF_ADJUST);
//	DrawEdge(dc, &r, EDGE_SUNKEN, BF_RECT|BF_ADJUST);
	dc.FillRect(&r, COLOR_3DFACE);

	HFONT hOldFont = dc.SelectFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	dc.SetBkMode(TRANSPARENT);

	CString strPos;
	strPos.Format(_T("%d"), m_nCur);
	dc.DrawText(strPos, strPos.GetLength(), &r, DT_CENTER|DT_VCENTER);

	dc.SelectFont(hOldFont);
}

LRESULT CSlider::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

	CRect rect;
	int nArrowSize = 0;
	int nThumbSize = 0;
	int nThumbPos = 0;

	int nHitTest = HitTest(point);
	if (nHitTest == Slider_Top_Rect || nHitTest == Slider_Bottom_Rect)
	{
		GetItemRect(&rect, &nArrowSize, &nThumbSize, &nThumbPos);
		int nPixels = rect.Width() - 2 * nArrowSize - nThumbSize;
		int nPos = point.x - nArrowSize - rect.left - nThumbSize / 2;
		if (nPos < 0)
			nPos = 0;
		else if (nPos > nPixels)
			nPos = nPixels;
		int nCur = m_nMin + MulDiv(m_nMax - m_nMin, nPos, nPixels);
		SetCur(nCur);
		::SendMessage(GetParent(), WM_SLIDERCHANGE, GetDlgCtrlID(), 0L);
		Invalidate();
		return 0;
	}

	SetCapture();
	if (::GetCapture() == m_hWnd)
	{
		m_nTracking = nHitTest;

		if (nHitTest == Slider_Top_Arrow || nHitTest == Slider_Bottom_Arrow)
		{
			SetCur(GetCur() + (m_nTracking == Slider_Top_Arrow ? -1 : 1));
			::SendMessage(GetParent(), WM_SLIDERCHANGE, GetDlgCtrlID(), 0L);
			m_bPressed = TRUE;
			Invalidate();
			UpdateWindow();

			int nElapse = 250;
			int nDelay = 0;
			if(::SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &nDelay, 0))
				nElapse += nDelay * 250;   // all milli-seconds
			SetTimer(ID_TIMER_FIRST, nElapse);

			return 0;
		}

		GetItemRect(&rect, &nArrowSize, &nThumbSize, &nThumbPos);
		m_nTrackOffset = point.x - nThumbPos;
	}

	return 0;
}

LRESULT CSlider::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	if (::GetCapture() == m_hWnd)
	{
		POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		int nHitTest = HitTest(point);
		if (m_nTracking == Slider_Top_Arrow || m_nTracking == Slider_Bottom_Arrow)
		{
			BOOL bPressed = m_nTracking == nHitTest;
			if (m_bPressed != bPressed)
			{
				m_bPressed = bPressed;
				Invalidate();
				UpdateWindow();
			}
		}
		else if (m_nTracking == Slider_Thumb)
		{
			CRect rect;
			int nArrowSize = 0;
			int nThumbSize = 0;
			int nThumbPos = 0;
			GetItemRect(&rect, &nArrowSize, &nThumbSize, &nThumbPos);

			int nPixels = rect.Width() - 2 * nArrowSize - nThumbSize;
			int nPos = point.x - m_nTrackOffset - nArrowSize - rect.left;
			if (nPos < 0)
				nPos = 0;
			else if (nPos > nPixels)
				nPos = nPixels;
			int nCur = m_nMin + MulDiv(m_nMax - m_nMin, nPos, nPixels);
			SetCur(nCur);
			::SendMessage(GetParent(), WM_SLIDERCHANGE, GetDlgCtrlID(), 0L);
			Invalidate();
		}
	}

	bHandled = FALSE;
	return 1;
}

LRESULT CSlider::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::GetCapture() == m_hWnd)
	{
//		if (m_fPressed == 1)
//			::SendMessage(GetParent(), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
		::ReleaseCapture();
	}

	return 0;
}

LRESULT CSlider::OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (m_nTracking != Slider_Nowhere)
	{
		m_nTracking = Slider_Nowhere;
		m_bPressed = FALSE;
		Invalidate();
		UpdateWindow();
	}
	bHandled = FALSE;
	return 1;
}

LRESULT CSlider::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	switch (wParam)   // timer ID
	{
	case ID_TIMER_FIRST:
		KillTimer(ID_TIMER_FIRST);
		if (m_bPressed && (m_nTracking == Slider_Top_Arrow || m_nTracking == Slider_Bottom_Arrow))
		{
			SetCur(GetCur() + (m_nTracking == Slider_Top_Arrow ? -1 : 1));
			::SendMessage(GetParent(), WM_SLIDERCHANGE, GetDlgCtrlID(), 0L);
			Invalidate();
			int nElapse = 250;
			int nRepeat = 40;
			if(::SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &nRepeat, 0))
				nElapse = 10000 / (10 * nRepeat + 25);   // milli-seconds, approximated
			SetTimer(ID_TIMER_REPEAT, nElapse);
		}
		break;
	case ID_TIMER_REPEAT:
		if (m_bPressed && (m_nTracking == Slider_Top_Arrow || m_nTracking == Slider_Bottom_Arrow))
		{
			SetCur(GetCur() + (m_nTracking == Slider_Top_Arrow ? -1 : 1));
			::SendMessage(GetParent(), WM_SLIDERCHANGE, GetDlgCtrlID(), 0L);
			Invalidate();
		}
		else if (::GetCapture() != m_hWnd)
			KillTimer(ID_TIMER_REPEAT);
		break;
	default:	// not our timer
		break;
	}
	return 0;
}

int CSlider::HitTest(POINT point)
{
	BOOL bVertical = FALSE;

	CRect rect;
	int nArrowSize = 0;
	int nThumbSize = 0;
	int nThumbPos = 0;
	GetItemRect(&rect, &nArrowSize, &nThumbSize, &nThumbPos);

	if (!rect.PtInRect(point))
		return Slider_Nowhere;

	if (bVertical)
	{
		if (point.y < rect.top + nArrowSize)
			return Slider_Top_Arrow;
		if (point.y > rect.bottom - nArrowSize)
			return Slider_Bottom_Arrow;
		point.y -= rect.top;
		if (point.y < nThumbPos)
			return Slider_Top_Rect;
		if (point.y >= nThumbPos + nThumbSize) 
			return Slider_Bottom_Rect;
	}
	else
	{
		if (point.x < rect.left + nArrowSize)
			return Slider_Top_Arrow;
		if (point.x > rect.right - nArrowSize)
			return Slider_Bottom_Arrow;
		point.y -= rect.left;
		if (point.x < nThumbPos)
			return Slider_Top_Rect;
		if (point.x >= nThumbPos + nThumbSize) 
			return Slider_Bottom_Rect;
	}

	return Slider_Thumb;
}

void CSlider::GetItemRect(CRect* pRect, int* pArrowSize, int* pThumbSize, int *pThumbPos)
{
	CDCHandle dc = ::GetDC(NULL);
	CString strPos(_T("64"));
	CSize szText;
	dc.GetTextExtent(strPos, strPos.GetLength(), &szText);
	*pThumbSize = szText.cx + 4;
	::ReleaseDC(NULL, dc);

	CRect rcClient;
	GetClientRect(&rcClient);

	pRect->left = rcClient.left;
	pRect->top = rcClient.top;
	pRect->right = rcClient.right;
	pRect->bottom = GetSystemMetrics(SM_CYHSCROLL);
	
	int nPixels = pRect->right - pRect->left;
	if (nPixels < 2 * GetSystemMetrics(SM_CXVSCROLL) + *pThumbSize)
	{
		if (nPixels > *pThumbSize)
            *pArrowSize = (nPixels - *pThumbSize) / 2;
        else
            *pArrowSize = 0;
        *pThumbPos = 0;
	}
	else
	{
	    *pArrowSize = GetSystemMetrics(SM_CXVSCROLL);
        nPixels -= ((2 * (GetSystemMetrics(SM_CXVSCROLL))) + *pThumbSize);
		       
		int nRange = m_nMax - m_nMin;
		if (nRange == 0)
			*pThumbPos = *pArrowSize;
		else
			*pThumbPos = *pArrowSize + MulDiv(nPixels, m_nCur - m_nMin, nRange);
 	}
}

void CSlider::SetRange(int nMin, int nMax)
{
	m_nMin = nMin;
	m_nMax = nMax;
}

void CSlider::SetCur(int nCur)
{
	m_nCur = nCur;
	if (m_nCur < m_nMin)
		m_nCur = m_nMin;
	else if (m_nCur > m_nMax)
		m_nCur = m_nMax;
}

int CSlider::GetCur()
{
	return m_nCur;
}
