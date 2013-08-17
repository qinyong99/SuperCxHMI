// DrawScrollBar.cpp: implementation of the CDrawScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxAlmSumCtrl.h"
#include "Alarm.h"
#include "DrawHeader.h"
#include "DrawGrid.h"
#include "DrawScrollBar.h"
#include "DrawAlmSum.h"
#include "DrawAlmSum.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawScrollBar::CDrawScrollBar(BOOL bVertical)
{
	m_nCatpture = -1;
	m_bVertical = bVertical;
	m_nMin = m_nMax = m_nPos = m_nPage = 0;
	m_bEnable = TRUE;
	m_hBmpEnable = ::LoadBitmap(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_BARDISABLE));
	m_hBmpDisable = ::LoadBitmap(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_BARSCROLL));
}

CDrawScrollBar::~CDrawScrollBar()
{
	if (m_hBmpEnable != NULL)
		DeleteObject(m_hBmpEnable);
	if (m_hBmpDisable != NULL)
		DeleteObject(m_hBmpDisable);
}

void CDrawScrollBar::GetItemRects(RECT rcTotal, RECT& rcMinButton, RECT& rcMaxButton, RECT& rcMidButton)
{
	int nRange = m_nMax - m_nMin;
	
	if (m_bVertical)
	{
		int nTotalLength = rcTotal.bottom - rcTotal.top;
		int nButtonHeight = rcTotal.right - rcTotal.left;

		rcMinButton.left = rcMaxButton.left = rcMidButton.left = rcTotal.left;
		rcMinButton.right = rcMaxButton.right = rcMidButton.right = rcTotal.right;
	
		rcMinButton.top = rcTotal.top;
		rcMaxButton.bottom = rcTotal.bottom;

		if (nTotalLength <= 2 * nButtonHeight)
		{
			rcMidButton.top = rcMidButton.bottom = rcMaxButton.top = rcMinButton.bottom = rcTotal.top + nTotalLength / 2;
		}
		else
		{
			rcMinButton.bottom = rcMinButton.top + nButtonHeight;
			rcMaxButton.top = rcMaxButton.bottom - nButtonHeight;
			
			if (m_nPage < nRange)
			{
				int nMidHeight = nTotalLength - 2 * nButtonHeight;
				int nMidButtonHeight = nMidHeight * m_nPage / nRange;
				if (nMidButtonHeight < 4)
					nMidButtonHeight = min(nMidHeight, 4);
				rcMidButton.top = rcMinButton.bottom + (nMidHeight - nMidButtonHeight) * m_nPos / (nRange - m_nPage);
				rcMidButton.bottom = rcMidButton.top + nMidButtonHeight;
			}
			else
			{
				rcMidButton.top = rcMinButton.bottom;
				rcMidButton.bottom = rcMaxButton.top;
			}
		}
	}
	else
	{
		int nTotalLength = rcTotal.right - rcTotal.left;
		int nButtonWidth = rcTotal.bottom - rcTotal.top;
		
		rcMinButton.top = rcMaxButton.top = rcMidButton.top = rcTotal.top;
		rcMinButton.bottom = rcMaxButton.bottom = rcMidButton.bottom = rcTotal.bottom;
		
		rcMinButton.left = rcTotal.left;
		rcMaxButton.right = rcTotal.right;
		
		if (nTotalLength <= 2 * nButtonWidth)
		{
			rcMidButton.left = rcMidButton.right = rcMaxButton.left = rcMinButton.right = rcTotal.left + nTotalLength / 2;
		}
		else
		{
			rcMinButton.right = rcMinButton.left + nButtonWidth;
			rcMaxButton.left = rcMaxButton.right - nButtonWidth;
					
			if (m_nPage < nRange)
			{
				int nMidWidth = nTotalLength - 2 * nButtonWidth;
				int nMidButtonWidth = nMidWidth * m_nPage / nRange;
				if (nMidButtonWidth < 4)
					nMidButtonWidth = min(nMidWidth, 4);

				rcMidButton.left = rcMinButton.right + (nMidWidth - nMidButtonWidth) * m_nPos / (nRange - m_nPage);
				rcMidButton.right = rcMidButton.left + nMidButtonWidth;
			}
			else
			{
				rcMidButton.left = rcMinButton.right;
				rcMidButton.right = rcMaxButton.left;
			}
		}
	}
}

void CDrawScrollBar::DrawBack(HDC hdc, RECT &rect)
{
	HBRUSH hBrush;
	if (m_bEnable)
		hBrush = ::CreatePatternBrush(m_hBmpEnable);
	else
		hBrush = ::CreatePatternBrush(m_hBmpDisable);

	HGDIOBJ hOld = ::SelectObject(hdc, hBrush);
	::PatBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATCOPY);
	::SelectObject(hdc, hOld);

	::DeleteObject(hBrush);
}

void CDrawScrollBar::DrawArrowBtn(HDC hdc, RECT &rect, enum enumDirection way)
{
	UINT state = m_bEnable ? 0 : DFCS_INACTIVE;
	
	switch(way)
	{
	case LEFT:
		state |= DFCS_SCROLLLEFT;
		break;
	case RIGHT:
		state |= DFCS_SCROLLRIGHT;
		break;
	case TOP:
		state |= DFCS_SCROLLUP;
		break;
	case BOTTOM:
		state |= DFCS_SCROLLDOWN;
		break;
	case CENTER:
		::DrawFrameControl(hdc, &rect, DFC_BUTTON, DFCS_BUTTONPUSH);
		return;
	}

	::DrawFrameControl(hdc, &rect, DFC_SCROLL, state);
}

void CDrawScrollBar::DoDraw(HDC hdc)
{
	DrawBack(hdc, m_rect);
	
	RECT rcMin, rcMax, rcMid;
	GetItemRects(m_rect, rcMin, rcMax, rcMid);
	if (m_bVertical)
	{
		DrawArrowBtn(hdc, rcMin, TOP);
		DrawArrowBtn(hdc, rcMax, BOTTOM);
		DrawArrowBtn(hdc, rcMid, CENTER);
	}
	else
	{
		DrawArrowBtn(hdc, rcMin, LEFT);
		DrawArrowBtn(hdc, rcMax, RIGHT);
		DrawArrowBtn(hdc, rcMid, CENTER);
	}
}

///////////////////////////////////////////////////////////////
//功能: 设置滚动条的范围
///////////////////////////////////////////////////////////////
void CDrawScrollBar::SetRange(int nMin, int nMax)
{
//	int nOldRange = m_nMax - m_nMin;

	m_nMin = nMin;
	m_nMax = nMax;

//	int nRange = m_nMax - m_nMin;
//
//	if (nRange != nOldRange)
//	{
//		if (m_bVertical)
//		{
//			int nHeight = m_rect.bottom - m_rect.top;
//			if ((nOldRange <= nHeight && nRange > nHeight) ||
//				(nOldRange > nHeight && nRange <= nHeight))
//				m_pAlmSum->RecalcRects();	
//		}
//		else
//		{
//			int nWidth = m_rect.right - m_rect.left;
//			if ((nOldRange <= nWidth && nRange > nWidth) ||
//				(nOldRange > nWidth && nRange <= nWidth))
//				m_pAlmSum->RecalcRects();	
//		}
//	}

	if (m_nPos > m_nMax - m_nPage)
		m_nPos = m_nMax - m_nPage;
	if (m_nPos < m_nMin)
		m_nPos = m_nMin;

	m_pAlmSum->FireViewChange();
}

///////////////////////////////////////////////////////////////
//功能: 设置当前位置
///////////////////////////////////////////////////////////////
void CDrawScrollBar::SetPos(int nPos)
{
	m_nPos = nPos;

	if (m_nPos > m_nMax - m_nPage)
		m_nPos = m_nMax - m_nPage;
	if (m_nPos < m_nMin)
		m_nPos = m_nMin;

	m_pAlmSum->FireViewChange();
}

void CDrawScrollBar::SetPage(int nPage)
{
	m_nPage = nPage;
}

///////////////////////////////////////////////////////////////
//功能: 允许或禁止
///////////////////////////////////////////////////////////////
void CDrawScrollBar::Enable(BOOL bEnable)
{
	m_bEnable = bEnable;
}

LRESULT CDrawScrollBar::DoLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT rcMin, rcMax, rcMid;
	GetItemRects(m_rect, rcMin, rcMax, rcMid);

	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);

	m_ptLastLButtonDown = pt;

	if (PtInRect(&rcMin, pt))
	{
		m_nCatpture = 0;

		if (m_bVertical)
			m_pAlmSum->DoVScroll(SB_LINEUP, 0, this);
		else
			m_pAlmSum->DoHScroll(SB_LINELEFT, 0, this);
	}
	else if (PtInRect(&rcMid, pt))
	{
		m_nCatpture = 1;

		RECT rcMin, rcMax, rcMid;
		GetItemRects(m_rect, rcMin, rcMax, rcMid);
		if (m_bVertical)
			m_nOffestMinButton = pt.y - rcMid.top;
		else
			m_nOffestMinButton = pt.x - rcMid.left;
		
	}
	else if (PtInRect(&rcMax, pt))
	{
		m_nCatpture = 2;

		if (m_bVertical)
			m_pAlmSum->DoVScroll(SB_LINEDOWN, 0, this);
		else
			m_pAlmSum->DoHScroll(SB_LINERIGHT, 0, this);
	}
	else
	{
		m_nCatpture = -1;

		if (m_bVertical)
		{
			if (pt.y > rcMin.bottom && pt.y < rcMid.top)
				m_pAlmSum->DoVScroll(SB_PAGEUP, 0, this);
			else if (pt.y > rcMid.bottom && pt.y < rcMax.top)
				m_pAlmSum->DoVScroll(SB_PAGEDOWN, 0, this);
		}
		else
		{
			if (pt.x > rcMin.right && pt.x < rcMid.left)
				m_pAlmSum->DoHScroll(SB_PAGEUP, 0, this);
			else if (pt.x > rcMid.right && pt.x < rcMax.left)
				m_pAlmSum->DoHScroll(SB_PAGEDOWN, 0, this);
		}	
	}

	if (m_nCatpture != -1)
		m_pAlmSum->SetCapture(TRUE, this);

	return 0;
}

LRESULT CDrawScrollBar::DoLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
//	if (m_nCatpture != -1)
//	{
		m_pAlmSum->SetCapture(FALSE, this);
//	}

	m_nCatpture = -1;

	return 0;
}

LRESULT CDrawScrollBar::DoMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_nCatpture == 1)
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		RECT rcMin, rcMax, rcMid;
		GetItemRects(m_rect, rcMin, rcMax, rcMid);

		int nRange = m_nMax - m_nMin - m_nPage;
		if (nRange > 0)
		{
			if (m_bVertical)
			{
				int nPos = nRange * (pt.y - m_nOffestMinButton - rcMin.bottom) / (rcMax.top - (rcMid.bottom - rcMid.top) - rcMin.bottom);
				SetPos(nPos);
			}
			else
			{
				int nPos = nRange * (pt.x - m_nOffestMinButton - rcMin.right) / (rcMax.left - (rcMid.right - rcMid.left) - rcMin.right);
				SetPos(nPos);
			}
		}
				
		//		m_ptLastLButtonDown
	}

	return 0;
}

void CDrawScrollBar::RecalcRect(RECT& rect)
{
	::CopyRect(&m_rect, &rect);
	if (m_bVertical)
	{
	//	if (rect.bottom - rect.top <= m_nMax - m_nMin)

		m_rect.left = max(m_rect.right - GetSystemMetrics(SM_CXVSCROLL), rect.left);
		m_rect.bottom = max(m_rect.bottom - GetSystemMetrics(SM_CYHSCROLL), rect.top);
		
	//		return;
		
		rect.right = m_rect.left;
	}
	else
	{
		m_rect.top = max(m_rect.bottom - GetSystemMetrics(SM_CYHSCROLL), rect.top);
		rect.bottom = m_rect.top;
	}
}


