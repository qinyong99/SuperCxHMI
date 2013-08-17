// DrawGrid.cpp: implementation of the CDrawGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxAlmSumCtrl.h"
#include "Alarm.h"
#include "DrawGrid.h"
#include "AlmSum.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawGrid::CDrawGrid()
{
	m_penGrid.CreatePen(PS_SOLID, 1, RGB(196, 196, 196)); 
}

CDrawGrid::~CDrawGrid()
{

}

void CDrawGrid::DoDraw(HDC hdc)
{
	//填充前景
	COLORREF clrFill;
	OleTranslateColor(m_pAlmSum->m_clrBack, 0, &clrFill);
	HBRUSH brush = ::CreateSolidBrush(clrFill);
	::FillRect(hdc, &m_rect, brush);
	::DeleteObject((HGDIOBJ)brush);

	CAtlArray<CAlarm *>& apAlarms = m_pAlmSum->m_apAlarms;
	CSimpleValArray<CColumn *>& apColumns = m_pAlmSum->m_apColumns;

	int nFirstRow = m_pAlmSum->GetFirstVisibleRow();
	int nFirstColumn = m_pAlmSum->GetFirstVisibleColumn();

	RECT rect;
	rect.top = rect.bottom = m_rect.top;

	int nColumnsWidth = 0; 
	for (int nCol = nFirstColumn; nCol < apColumns.GetSize(); nCol++)
		nColumnsWidth += apColumns[nCol]->m_nWidth;

	HPEN hOldPen = (HPEN)::SelectObject(hdc, m_penGrid);
	for (int nRow = nFirstRow; nRow < (int)apAlarms.GetCount(); nRow++)
	{
		rect.top = rect.bottom;
		rect.bottom = rect.top + m_pAlmSum->m_lRowHeight;
		if (rect.bottom > m_rect.bottom)
			rect.bottom = m_rect.bottom;
	
		rect.left = m_rect.left;
		rect.right = min(rect.left + nColumnsWidth, m_rect.right);
		apAlarms[nRow]->DrawBackground(hdc, rect);

		rect.left = rect.right = m_rect.left;
		for (nCol = nFirstColumn; nCol < apColumns.GetSize(); nCol++)
		{
			rect.left = rect.right;
			rect.right += apColumns[nCol]->m_nWidth;
			if (rect.right > m_rect.right)
				rect.right = m_rect.right;
		
			apAlarms[nRow]->DrawField(apColumns[nCol]->m_enumAlarmField, hdc, rect);
		
			if (rect.right >= m_rect.right)
				break;
			if (m_pAlmSum->m_bShowGrid == VARIANT_TRUE)
			{
				::MoveToEx(hdc, rect.right - 1, rect.top, 0);
				::LineTo(hdc, rect.right - 1, rect.bottom - 1);
			}
		}

		// 绘制选择框
		if (m_pAlmSum->m_nSelectAlarm == nRow)
		{
			int nOldDrawMode = ::SetROP2(hdc, R2_NOT);
		//	::SelectObject(hdc, GetStockObject(BLACK_PEN));
			::MoveToEx(hdc, m_rect.left, rect.bottom - 2, 0);
			::LineTo(hdc, m_rect.left, rect.top);
			::LineTo(hdc, m_rect.right - 2, rect.top);
			::LineTo(hdc, m_rect.right - 2, rect.bottom - 2);
			::SetROP2(hdc, nOldDrawMode);
		//	::SelectObject(hdc, m_penGrid);
		}

		if (rect.bottom >= m_rect.bottom)
			break;

		//画下面网格线
		if (m_pAlmSum->m_bShowGrid == VARIANT_TRUE)
		{
			::MoveToEx(hdc, m_rect.left, rect.bottom - 1, 0);
			::LineTo(hdc, rect.right - 1, rect.bottom - 1);
		}

		// 绘制选择框
		if (m_pAlmSum->m_nSelectAlarm == nRow)
		{
			int nOldDrawMode = ::SetROP2(hdc, R2_NOT);
		//	::SelectObject(hdc, GetStockObject(BLACK_PEN));
			::MoveToEx(hdc, m_rect.left, rect.bottom - 2, 0);
			::LineTo(hdc, m_rect.right - 2, rect.bottom - 2);
			::SetROP2(hdc, nOldDrawMode);
		//	::SelectObject(hdc, m_penGrid);
		}
	}
	::SelectObject(hdc, hOldPen);
}

LRESULT CDrawGrid::DoLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT point;
	point.x = GET_X_LPARAM(lParam);
	point.y = GET_Y_LPARAM(lParam);

	int nFirstRow = m_pAlmSum->GetFirstVisibleRow();
	

	int nOffset = point.y - m_rect.top;
	int nSelectedRow = nFirstRow + nOffset / m_pAlmSum->m_lRowHeight;
	m_pAlmSum->SelectAlarm(nSelectedRow);

	// 是否点中响应列
	if (m_pAlmSum->m_nSelectAlarm != -1)
	{
		int nFirstColumn = m_pAlmSum->GetFirstVisibleColumn();
		if (nFirstColumn == 0 && point.x - m_rect.left < m_pAlmSum->m_apColumns[0]->m_nWidth)
		{
			m_pAlmSum->AckAlarm(m_pAlmSum->m_apAlarms[m_pAlmSum->m_nSelectAlarm]);
		}
	}

	bHandled = FALSE;
	
	return 0;
}

LRESULT CDrawGrid::DoLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDrawGrid::DoMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	return 0;
}



