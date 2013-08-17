// LinePreviewWnd.cpp: implementation of the CLinePreviewWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxAppearExp.h"
#include "AppearanceExpert.h"
#include "SampleObject.h"
#include "LinePreviewWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinePreviewWnd::CLinePreviewWnd()
{
	m_pExpert = NULL;
}

CLinePreviewWnd::~CLinePreviewWnd()
{
	
}

LRESULT CLinePreviewWnd::OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 1;   // no background needed
}

#undef SelectBitmap

void CLinePreviewWnd::DoPaint(CDCHandle dc, CRect& rect)
{
	CRect r = rect;
	r.right = r.left + r.Height() / 2;
	DrawJoin(dc.m_hDC, r);
	
	r.left = r.right - 1;
	r.right = r.left + 12;
	CRect rcDetail = r;
	
	r.left = r.right;
	r.right = rect.right;
	DrawLine(dc.m_hDC, r);
	
	DrawDetail(dc.m_hDC, rcDetail);
}

LRESULT CLinePreviewWnd::OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	using namespace Gdiplus;

	ATLASSERT(m_pExpert != NULL);
	if (m_pExpert->m_pSampleObject == NULL)
		return 0;
	
	CPaintDC dc(m_hWnd);
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC dcMem;
	dcMem.CreateCompatibleDC(dc);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, rcClient.Width(), rcClient.Height());
	ATLASSERT(bmp.m_hBitmap != NULL);
	// draw image into memory DC--fill BG white first
	HBITMAP hBmpOld = dcMem.SelectBitmap(bmp);

	DoPaint(dcMem.m_hDC, rcClient);

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), dcMem, 0, 0, SRCCOPY);
	dcMem.SelectBitmap(hBmpOld);		// restore
	
	return 0;
}

LRESULT CLinePreviewWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	ATLASSERT(m_pExpert != NULL);
	if (m_pExpert->m_pSampleObject == NULL)
		return 0;

	if (m_pExpert->m_pSampleObject->m_enumEdgeStyle == (enumEdgeStyle)-1)
		return 0;

	POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rect = rcClient;
	rect.right = rect.left + rect.Height() / 2;
	if (rect.PtInRect(point))
	{
		if (m_pExpert->m_pSampleObject->m_enumEdgeStyle == enumEdgeStyle(-1))
			return 0;
		m_pExpert->m_pSampleObject->m_enumLineJoinStyle = 
			(m_pExpert->m_pSampleObject->m_enumLineJoinStyle == LineJoinStyleRound ? LineJoinStyleSquare : LineJoinStyleRound);
		m_pExpert->OnProertyChanged(CXPROPID_LineJoinStyle, this);
		Invalidate();
		return 0;
	}
	
	rect.left = rect.right - 1;
	rect.right = rect.left + 12;
	if (rect.PtInRect(point))
	{
		if (m_pExpert->m_pSampleObject->m_enumEdgeStyle == enumEdgeStyle(-1))
			return 0;
		if (m_pExpert->m_pSampleObject->m_enumEdgeStyle == EdgeStylePipe)
		{
			rect.InflateRect(0, -rect.Height() / 4);
			if (rect.PtInRect(point))
			{
				SetSelectColor(SelectColor_Highlight);
				Invalidate();
				return 0;
			}
		}
		
		SetSelectColor(SelectColor_Color);
		Invalidate();
		return 0;
	}
	
	return 0;
}

#undef SelectBrush
#undef SelectPen

void CLinePreviewWnd::DrawJoin(CDCHandle dc, CRect& rect)
{
	dc.FillRect(&rect, COLOR_3DFACE);

	if (m_pExpert->m_pSampleObject->m_enumEdgeStyle == enumEdgeStyle(-1))
		return;

	COLORREF color(RGB(255, 255, 255));
//	OleTranslateColor(m_pExpert->m_pSampleObject->m_clrEdgeColor, 0, &color);

	CBrush brush;
	brush.CreateSolidBrush(color); 
	HBRUSH hOldBrush = dc.SelectBrush(brush);
	CPen pen;
	pen.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_3DSHADOW));
	HPEN hOldPen = dc.SelectPen(pen);

	if (m_pExpert->m_pSampleObject->m_enumLineJoinStyle == LineJoinStyleRound)
	{
		CRect r = rect;
		r.right += r.Width();
		dc.Pie(&r, CPoint(rect.right, rect.top), CPoint(rect.right, rect.bottom));
	}
	else
	{
		dc.Rectangle(&rect);
	}

	dc.SelectBrush(hOldBrush);
	dc.SelectPen(hOldPen);
}

BYTE GetDistinctionValue(BYTE b1, BYTE b2)
{
	BYTE b = (b1 + b2) / 2;
	if (abs(b1 - b2) < 128)
		return b + 128;

	return b;
}

void CLinePreviewWnd::DrawDetail(CDCHandle dc, CRect& rect)
{
	if (m_pExpert->m_pSampleObject->m_enumEdgeStyle == enumEdgeStyle(-1))
	{
		dc.FillRect(&rect, COLOR_3DFACE);
		return;
	}
	
	COLORREF color;
	OleTranslateColor(m_pExpert->m_pSampleObject->m_clrEdgeColor, 0, &color);
	CBrush brush;
	brush.CreateSolidBrush(color); 
	HBRUSH hOldBrush = dc.SelectBrush(brush);

	COLORREF clrHighLight;
	OleTranslateColor(m_pExpert->m_pSampleObject->m_clrHighLightEdge, 0, &clrHighLight);
	
	COLORREF clrLine = RGB(GetDistinctionValue(GetRValue(color), GetRValue(clrHighLight)),
		GetDistinctionValue(GetGValue(color), GetGValue(clrHighLight)),
		GetDistinctionValue(GetBValue(color), GetBValue(clrHighLight)));

	CPen pen;
	pen.CreatePen(PS_SOLID, 0, clrLine);
	CPen pen3;
	pen3.CreatePen(PS_SOLID, 3, clrLine);
	HPEN hOldPen = NULL; 

	if (m_pExpert->m_pSampleObject->m_enumEdgeStyle == EdgeStylePipe)
	{
		hOldPen = dc.SelectPen(GetSelectColor() == SelectColor_Color ? pen3 : pen);

		CRect r = rect;
		r.bottom = r.top + rect.Height() / 4;
		dc.Rectangle(&r);
		r.OffsetRect(0, 3 * rect.Height() / 4);
		dc.Rectangle(&r);

		COLORREF clrHighLight;
		OleTranslateColor(m_pExpert->m_pSampleObject->m_clrHighLightEdge, 0, &clrHighLight);
		CBrush brushHighLight;
		brushHighLight.CreateSolidBrush(clrHighLight); 

		dc.SelectBrush(brushHighLight);
		dc.SelectPen(GetSelectColor() == SelectColor_Highlight ? pen3 : pen);

		r = rect;
		r.InflateRect(0, -rect.Height() / 4);
		dc.Rectangle(&r);
	}
	else
	{
		if (m_pExpert->m_pSampleObject->m_enumEdgeStyle != EdgeStyleTransparence)
		{
			hOldPen = dc.SelectPen(GetSelectColor() == SelectColor_Color ? pen3 : pen);
			dc.Rectangle(&rect);
		}
	}
	
	dc.SelectBrush(hOldBrush);
	if (hOldPen != NULL)
		dc.SelectPen(hOldPen);
}

void CLinePreviewWnd::DrawLine(CDCHandle dc, CRect& rect)
{
	using namespace Gdiplus;

	int nSavedDC = dc.SaveDC();
	dc.IntersectClipRect(rect);

	dc.FillRect(&rect, COLOR_3DFACE);

	GraphicsPath path;
	path.AddLine(rect.left, (rect.top + rect.bottom) / 2, 
		rect.right, (rect.top + rect.bottom) / 2);	
	
	Graphics graphics(dc.m_hDC);
	float fEdgeWidth = m_pExpert->m_pSampleObject->m_fEdgeWidth;
	if (fEdgeWidth > 40.0)
		m_pExpert->m_pSampleObject->m_fEdgeWidth = 40.0;
	m_pExpert->m_pSampleObject->DrawObjectEdge(&graphics, &path);
	m_pExpert->m_pSampleObject->m_fEdgeWidth = fEdgeWidth;

	dc.RestoreDC(nSavedDC);
}

int CLinePreviewWnd::GetSelectColor()
{
	if (m_pExpert->m_dwColorPropDispID == CXPROPID_EdgeColor)
		return SelectColor_Color;
	if (m_pExpert->m_dwColorPropDispID == CXPROPID_HighLightEdgeColor)
		return SelectColor_Highlight;
	
	return SelectColor_None;
}

void CLinePreviewWnd::SetSelectColor(int nColor)
{
	m_pExpert->OnColorPropSelecting(this);

	switch (nColor) 
	{
	case SelectColor_Color:
		m_pExpert->m_dwColorPropDispID = CXPROPID_EdgeColor;
		break;
	case SelectColor_Highlight:
		m_pExpert->m_dwColorPropDispID = CXPROPID_HighLightEdgeColor;
		break;
	default:
		return;
	}
	
	m_pExpert->OnColorPropSelected(this);
}

