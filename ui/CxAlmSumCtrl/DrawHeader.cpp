// DrawHeader.cpp: implementation of the CDrawHeader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxAlmSumCtrl.h"
#include "Alarm.h"
#include "DrawHeader.h"
#include "AlmSum.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawHeader::CDrawHeader()
{
	m_pAlmSum = NULL;

	m_nColumnCatpture = -1;

	m_nDragDivideOffset = 0;
	m_nResizingColumn = -1;
	m_bColumnResizing = FALSE;
	m_nResizeCaptureRange = 3;
	m_hColDivCursor = ::CreateCursor(_Module.GetModuleInstance(), _DivideColumn_CursorData.xHotSpot, _DivideColumn_CursorData.yHotSpot, _DivideColumn_CursorData.cxWidth, _DivideColumn_CursorData.cyHeight, _DivideColumn_CursorData.arrANDPlane, _DivideColumn_CursorData.arrXORPlane);
}

CDrawHeader::~CDrawHeader()
{
	if (m_hColDivCursor != NULL)
		::DestroyCursor(m_hColDivCursor);
}

void CDrawHeader::RecalcRect(RECT& rect)
{
	int nHeight = GetFontHeight(m_pAlmSum->m_spHeaderFont);
	
	::CopyRect(&m_rect, &rect);
	m_rect.bottom = min(m_rect.top + nHeight + 3, rect.bottom);
	rect.top = m_rect.bottom;
}

void CDrawHeader::DoDraw(HDC hdc)
{
	int iOldMode = ::SetBkMode(hdc, TRANSPARENT);
	COLORREF clrFore;
	OleTranslateColor(m_pAlmSum->m_clrHeaderFore, 0, &clrFore);
	COLORREF clrOldColor = ::SetTextColor(hdc, clrFore);
	
	HFONT hFont;
	HFONT hOldFont;
	if (m_pAlmSum->m_spHeaderFont != NULL)
	{
		CComPtr<IFont> pFont;
		m_pAlmSum->m_spHeaderFont->QueryInterface(IID_IFont, (void**)&pFont);
		pFont->get_hFont(&hFont);
	}
	else
	{
		hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	}
	hOldFont = (HFONT)::SelectObject(hdc, hFont);

	//Ìî³äÇ°¾°
	COLORREF clrFill;
	OleTranslateColor(m_pAlmSum->m_clrHeaderBack, 0, &clrFill);
	HBRUSH brush = ::CreateSolidBrush(clrFill);
	::FillRect(hdc, &m_rect, brush);
	::DeleteObject((HGDIOBJ)brush);

	CSimpleValArray<CColumn *>& apColumns = m_pAlmSum->m_apColumns;

	int nFirstColumn = m_pAlmSum->GetFirstVisibleColumn();
	int nCount = apColumns.GetSize();
	RECT rect = m_rect;
	
	BOOL bBreak = FALSE;
	for (int i = nFirstColumn; i < nCount; i++)
	{
		rect.right = rect.left + apColumns[i]->m_nWidth;
		if (rect.right > m_rect.right)
		{
			rect.right = m_rect.right;
			bBreak = TRUE;
		}
			
		apColumns[i]->Draw(hdc, rect, m_nColumnCatpture == i);
		
		if (bBreak)
			break;
		rect.left = rect.right;
	}

	rect.left = rect.right;
	rect.right = m_rect.right;
	DrawEdge(hdc, &rect, BDR_RAISEDINNER, BF_RECT);

	::SetBkMode(hdc, iOldMode);
	::SetTextColor(hdc, clrOldColor);
	::SelectObject(hdc, hOldFont);
}

LRESULT CDrawHeader::DoLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT point;
	point.x = GET_X_LPARAM(lParam);
	point.y = GET_Y_LPARAM(lParam);

	m_ptLastLButtonDown = point;
	
	if (!m_bColumnResizing)
	{
		m_pAlmSum->SetCapture(TRUE, this);
		if (MouseOverColumnResizeArea(point))
		{
			SetCursor(m_hColDivCursor); 
			m_bColumnResizing = TRUE;
		}
		else
		{
			m_nColumnCatpture = GetColumn(point);

			CDCHandle dcHandle = m_pAlmSum->GetDC();
			DoDraw(dcHandle);
			m_pAlmSum->ReleaseDC(dcHandle);
		}
	}

	if (m_bColumnResizing)
	{
		if (!GetResizeColumn(point, m_nResizingColumn, m_nDragDivideOffset))
			return 1;
		
		CRect rect = m_pAlmSum->m_rect;
		CRect invertedRect(point.x - 1 - m_nDragDivideOffset, rect.top, 
			point.x - m_nDragDivideOffset, rect.bottom);
		
		CDCHandle dcHandle = m_pAlmSum->GetDC();
		dcHandle.InvertRect(&invertedRect);
		m_pAlmSum->ReleaseDC(dcHandle);
	}
		
	return 0;
}

LRESULT CDrawHeader::DoLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT point;
	point.x = GET_X_LPARAM(lParam);
	point.y = GET_Y_LPARAM(lParam);

	m_pAlmSum->SetCapture(FALSE, this);

	if (m_bColumnResizing)
	{
		int nMinX = m_ptLastLButtonDown.x - m_nDragDivideOffset - m_pAlmSum->GetColumnWidth(m_nResizingColumn);
		int nXPos = max(m_ptLastMousePoint.x, nMinX);

		CRect rect = m_pAlmSum->m_rect;
		CRect invertedRect(nXPos - 1 - m_nDragDivideOffset, rect.top, 
			nXPos - m_nDragDivideOffset, rect.bottom);
		CDCHandle dcHandle = m_pAlmSum->GetDC();
		dcHandle.InvertRect(&invertedRect);
		m_pAlmSum->ReleaseDC(dcHandle);
		
		if (m_ptLastLButtonDown != point) 
		{   
			CPoint start;
			nXPos = max(point.x, nMinX);
			m_pAlmSum->SetColumnWidth(m_nResizingColumn, nXPos - m_ptLastLButtonDown.x + m_nDragDivideOffset + m_pAlmSum->GetColumnWidth(m_nResizingColumn));
			m_pAlmSum->FireViewChange();
		}
	
		m_bColumnResizing = FALSE;
		m_nResizingColumn = -1;
	}
	else if (m_nColumnCatpture != -1)
	{
		int nCol = GetColumn(point);
		if (nCol == m_nColumnCatpture)
		{
			if (m_pAlmSum->m_nSortField == m_pAlmSum->m_apColumns[nCol]->m_enumAlarmField &&
				m_pAlmSum->m_nSecondSortField == (enumAlarmField)-1)
			{
				m_pAlmSum->m_bSortAscending = !m_pAlmSum->m_bSortAscending;
			}
			else
			{
				m_pAlmSum->m_nSortField = m_pAlmSum->m_apColumns[nCol]->m_enumAlarmField;
				m_pAlmSum->m_nSecondSortField = (enumAlarmField)-1;
			}
			
			CAlarm* pSelectAlarm = m_pAlmSum->GetSelectAlarm();
			m_pAlmSum->ReSortAlarms();
			m_pAlmSum->SelectAlarm(pSelectAlarm);

			m_nColumnCatpture = -1;
		}
		else
		{
			m_nColumnCatpture = -1;

			CDCHandle dcHandle = m_pAlmSum->GetDC();
			DoDraw(dcHandle);
			m_pAlmSum->ReleaseDC(dcHandle);
		}
	}
	
	return 0;
}

LRESULT CDrawHeader::DoMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT point;
	point.x = GET_X_LPARAM(lParam);
	point.y = GET_Y_LPARAM(lParam);

	UINT nFlags = wParam;

	if (!(nFlags & MK_LBUTTON))
	{
		if (MouseOverColumnResizeArea(point) && m_nColumnCatpture == -1)
		{
			SetCursor(m_hColDivCursor); 
		}
		else
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}
	}

	if (nFlags & MK_LBUTTON) 
	{
		if (m_bColumnResizing)
		{
			//SetCursor(m_hColDivCursor); 
			
			CRect rect = m_pAlmSum->m_rect;
			CDCHandle dcHandle = m_pAlmSum->GetDC();

			int nMinX = m_ptLastLButtonDown.x - m_nDragDivideOffset - m_pAlmSum->GetColumnWidth(m_nResizingColumn);
			int nXPos = max(m_ptLastMousePoint.x, nMinX);
			CRect oldInvertedRect(nXPos - 1 - m_nDragDivideOffset, rect.top, 
				nXPos - m_nDragDivideOffset, rect.bottom);

			dcHandle.InvertRect(&oldInvertedRect);
			nXPos = max(point.x, nMinX);
			CRect newInvertedRect(nXPos - 1 - m_nDragDivideOffset, rect.top, 
				nXPos - m_nDragDivideOffset, rect.bottom);
			dcHandle.InvertRect(&newInvertedRect);
			m_pAlmSum->ReleaseDC(dcHandle);
		}	
		else if (m_nColumnCatpture != -1)
		{
			int nCol = GetColumn(point);
			if (nCol != m_nColumnCatpture)
			{
				m_nColumnCatpture = -1;
				
				CDCHandle dcHandle = m_pAlmSum->GetDC();
				DoDraw(dcHandle);
				m_pAlmSum->ReleaseDC(dcHandle);
				
				m_pAlmSum->SetCapture(FALSE, this);
			}
		}
	}

	m_ptLastMousePoint = point;

	return 0;
}

int CDrawHeader::GetColumn(POINT point)
{
	if (point.y < m_rect.top || point.y > m_rect.bottom)
		return -1;
	
	int nFirstColumn = m_pAlmSum->GetFirstVisibleColumn();
	CSimpleValArray<CColumn *>& apColumns = m_pAlmSum->m_apColumns;
	int nColumnCount = apColumns.GetSize();
	int nStartX = m_rect.left;
	for (int i = nFirstColumn; i < nColumnCount; i++)
	{
		nStartX += apColumns[i]->m_nWidth;
		if (nStartX > point.x)
			return i;
	}

	return -1;
}

BOOL CDrawHeader::GetResizeColumn(POINT point, int& nCol, int& nOffsetX)
{
	int nFirstColumn = m_pAlmSum->GetFirstVisibleColumn();
	CSimpleValArray<CColumn *>& apColumns = m_pAlmSum->m_apColumns;
	int nColumnCount = apColumns.GetSize();
	int nStartX = m_pAlmSum->m_rect.left;
	for (int i = nFirstColumn; i < nColumnCount; i++)
	{
		nStartX += apColumns[i]->m_nWidth;
		if (nStartX > point.x - m_nResizeCaptureRange)
			break;
	}
	if (nCol >= nColumnCount)
		return FALSE;
	
	if (abs(point.x - nStartX) <= m_nResizeCaptureRange)
	{
		nOffsetX = point.x - nStartX;
		while (nOffsetX >= 0)
		{
			if (i + 1 >= nColumnCount)
				break;
			nStartX += apColumns[i + 1]->m_nWidth;
			if (abs(point.x - nStartX) <= m_nResizeCaptureRange)
			{
				i++;
				nOffsetX = point.x - nStartX;
			}
			else
			{
				break;
			}
		}
		nCol = i;
		return TRUE;
	}
	
	return FALSE;
}

BOOL CDrawHeader::MouseOverColumnResizeArea(POINT& point)
{
	int nCol = 0;
	int nOffset = 0;
	return GetResizeColumn(point, nCol, nOffset);
}

