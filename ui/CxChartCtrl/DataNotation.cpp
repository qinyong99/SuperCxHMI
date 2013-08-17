// DataNotation.cpp: implementation of the CDataNotation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxChartCtrl.h"
#include "TrendCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataNotation::CDataNotation()
{
	m_pTrend = NULL;
	m_bPositioned = FALSE;
}

CDataNotation::~CDataNotation()
{

}

void CDataNotation::DoDraw(HDC hdcDest)
{
	DATAPOINT datapoint;
	datapoint.dXVal = m_dXVal;
	datapoint.dYVal = m_dYVal;
	
	POINT point;
	m_pTrend->m_apSerials[m_nCurve]->GetTimeAxis()->GetPointForValue(&datapoint, &point);
	m_pTrend->m_apSerials[m_nCurve]->GetValueAxis()->GetPointForValue(&datapoint, &point);

	if (!m_bPositioned)
	{
		m_ptLast = point;

		m_rect.left = point.x;
		m_rect.top = point.y - 20;
		m_rect.right = m_rect.left + 1;
		m_rect.bottom = point.y;
		DrawText(hdcDest, m_strText, m_strText.GetLength(), 
				&m_rect, DT_CENTER | DT_CALCRECT);
		m_rect.bottom = point.y - 8;
		InflateRect(&m_rect, 2, 0);
		OffsetRect(&m_rect, point.x - m_rect.left, point.y - m_rect.top - 20);
		m_bPositioned = TRUE;
	}
	
	OffsetRect(&m_rect, point.x - m_ptLast.x, point.y - m_ptLast.y);
	
//	InflateRect(&m_rect, 1, 1);
	Rectangle(hdcDest, m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
//	::FillRect(hdcDest, &m_rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	DrawText(hdcDest, m_strText, m_strText.GetLength(), 
			&m_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	
//	pDC->FillSolidRect (nXPos - 1, nYPos - 1, 3, 3, 0);
	DrawMarker(hdcDest, point.x, point.y, MarkerStyleCross, 5);
	MoveToEx(hdcDest, point.x, point.y, NULL);
	LineTo(hdcDest, m_rect.left, m_rect.bottom);

	m_ptLast = point;
}
