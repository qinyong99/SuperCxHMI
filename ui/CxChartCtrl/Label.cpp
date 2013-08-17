// Label.cpp: implementation of the CLabel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Label.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLabel::CLabel()
{
}

CLabel::~CLabel()
{

}

void CLabel::DoDraw(HDC hdcDest)
{
	USES_CONVERSION;

	int nOldBkMode = SetBkMode(hdcDest, TRANSPARENT);

	::SetBkColor(hdcDest, RGB(255,255,255));
	::ExtTextOut(hdcDest, 0, 0, ETO_OPAQUE, &m_rect, NULL, 0, NULL);
	Rectangle(hdcDest, m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	
	HFONT hFontOld = (HFONT)SelectObject(hdcDest, AtlGetStockFont(DEFAULT_GUI_FONT));

	RECT rect = m_rect;
	rect.left += 4;
	::DrawText(hdcDest, m_strText, m_strText.GetLength(), &rect, DT_LEFT);
	
	SelectObject(hdcDest, hFontOld);
	SetBkMode(hdcDest, nOldBkMode);
}
