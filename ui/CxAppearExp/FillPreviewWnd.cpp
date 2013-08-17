// FillPreviewWnd.cpp: implementation of the CFillPreviewWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxAppearExp.h"
#include "AppearanceExpert.h"
#include "SampleObject.h"
#include "FillPreviewWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFillPreviewWnd::CFillPreviewWnd()
{
	m_pExpert = NULL;
}

CFillPreviewWnd::~CFillPreviewWnd()
{
	
}

LRESULT CFillPreviewWnd::OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 1;   // no background needed
}

LRESULT CFillPreviewWnd::OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	using namespace Gdiplus;

	ATLASSERT(m_pExpert != NULL);
	if (m_pExpert->m_pSampleObject == NULL)
		return 0;
	
	CPaintDC dc(m_hWnd);
	CRect rcClient;
	GetClientRect(&rcClient);

	int nFillStyle = m_pExpert->m_pSampleObject->m_enumFillStyle;
	if (nFillStyle == (enumFillStyle)-1 ||
		nFillStyle == FillStyleTransparence ||  nFillStyle == FillStyleImage || 
		(nFillStyle >= FillStyleHatchHorizontal && nFillStyle <= FillStyleHatchDiagonalCross))
	{
		dc.FillRect(&rcClient, COLOR_3DFACE);
		if (nFillStyle == (enumFillStyle)-1)
			return 0;
	}
	else
	{
		CRect rc = rcClient;
		dc.FrameRect(&rc, ::GetSysColorBrush(COLOR_3DFACE));
		rc.InflateRect(1, 1, 1, 1);
		dc.FrameRect(&rc, ::GetSysColorBrush(COLOR_3DFACE));
	}
	rcClient.InflateRect(-1, -1, -2, -2);

	Rect rect(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());
	GraphicsPath path;
	path.AddRectangle(rect);
	
	Graphics graphics(dc.m_hDC);
	m_pExpert->m_pSampleObject->FillObjectRgn(&graphics, &path);
		
	COLORREF clrFill;
	OleTranslateColor(m_pExpert->m_pSampleObject->m_clrFillColor, 0, &clrFill);
	COLORREF clrHighLightFill;
	if (m_pExpert->m_pSampleObject->m_enumFillStyle >= FillStyleGradientLeftTopLight &&
		m_pExpert->m_pSampleObject->m_enumFillStyle <= FillStyleGradientAroundLight)
		OleTranslateColor(m_pExpert->m_pSampleObject->m_clrHighLightFill, 0, &clrHighLightFill);
	else
		clrHighLightFill = clrFill;

	COLORREF clrLine = RGB(128 + (GetRValue(clrFill) + GetRValue(clrHighLightFill)) / 2,
		128 + (GetGValue(clrFill) + GetGValue(clrHighLightFill)) / 2,
		128 + (GetBValue(clrFill) + GetBValue(clrHighLightFill)) / 2);

	DrawColorEdge(dc.m_hDC, rcClient, nFillStyle, clrLine);
	
	return 0;
}

#undef SelectBrush
#undef SelectPen

void DrawLine(CDCHandle dc, int nX1, int nY1, int nX2, int nY2)
{
	dc.MoveTo(nX1, nY1);
	dc.LineTo(nX2, nY2);
}

void DrawRect(CDCHandle dc, RECT rect)
{
	dc.MoveTo(rect.left, rect.top);
	dc.LineTo(rect.right, rect.top);
	dc.LineTo(rect.right, rect.bottom);
	dc.LineTo(rect.left, rect.bottom);
	dc.LineTo(rect.left, rect.top);
}

void CFillPreviewWnd::DrawColorEdge(CDCHandle dc, CRect& rect, int nFillStyle, COLORREF color)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, color); 
	CPen pen3;
	pen3.CreatePen(PS_SOLID, 3, color);
	HPEN hOldPen = NULL;

	switch (nFillStyle) 
	{
	case FillStyleGradientCenterLight:
	case FillStyleGradientAroundLight:
		if (nFillStyle == FillStyleGradientCenterLight)
			dc.SelectPen(GetSelectColor() == SelectColor_Color ? pen3 : pen);
		else 
			dc.SelectPen(GetSelectColor() == SelectColor_Highlight ? pen3 : pen);
		DrawRect(dc, rect);
		rect.InflateRect(-rect.Width() / 4, -rect.Height() / 4);
		dc.SelectPen(GetSelectColor() != SelectColor_None ? pen3 : pen);
		DrawRect(dc, rect);
		break;

	case FillStyleGradientHorMiddleLight:
	case FillStyleGradientHorSideLight:
		if ((nFillStyle == FillStyleGradientHorMiddleLight && GetSelectColor() == SelectColor_Color) ||
			(nFillStyle == FillStyleGradientHorSideLight && GetSelectColor() == SelectColor_Highlight))
		{
			dc.SelectPen(pen);
			DrawLine(dc, rect.left + rect.Width() / 4, rect.top, rect.left + 3 * rect.Width() / 4, rect.top);
			DrawLine(dc, rect.left + rect.Width() / 4, rect.bottom, rect.left + 3 * rect.Width() / 4, rect.bottom);

			dc.SelectPen(pen3);
			CRect rectT = rect;
			rectT.right = rect.left + rect.Width() / 4;
			DrawRect(dc, rectT);
			rectT.OffsetRect(3 * rect.Width() / 4, 0);
			DrawRect(dc, rectT);
		}
		else if ((nFillStyle == FillStyleGradientHorMiddleLight && GetSelectColor() == SelectColor_Highlight) ||
			(nFillStyle == FillStyleGradientHorSideLight && GetSelectColor() == SelectColor_Color))
		{
			dc.SelectPen(pen);
			dc.MoveTo(rect.left + rect.Width() / 4, rect.top);
			dc.LineTo(rect.left, rect.top);
			dc.LineTo(rect.left, rect.bottom);
			dc.LineTo(rect.left + rect.Width() / 4, rect.bottom);

			dc.MoveTo(rect.left + 3 * rect.Width() / 4, rect.top);
			dc.LineTo(rect.right, rect.top);
			dc.LineTo(rect.right, rect.bottom);
			dc.LineTo(rect.left + 3 * rect.Width() / 4, rect.bottom);
			
			dc.SelectPen(pen3);
			rect.InflateRect(-rect.Width() / 4, 0);
			DrawRect(dc, rect);
		}
		else
		{
			dc.SelectPen(pen);
			DrawRect(dc, rect);
			DrawLine(dc, rect.left + rect.Width() / 4, rect.top, rect.left + rect.Width() / 4, rect.bottom);
			DrawLine(dc, rect.left + 3 * rect.Width() / 4, rect.top, rect.left + 3 * rect.Width() / 4, rect.bottom);
		}
		break;
	case FillStyleGradientHorLeftLight:
	case FillStyleGradientHorRightLight:
		if ((nFillStyle == FillStyleGradientHorLeftLight && GetSelectColor() == SelectColor_Color) ||
			(nFillStyle == FillStyleGradientHorRightLight && GetSelectColor() == SelectColor_Highlight))
		{
			dc.SelectPen(pen);
			dc.MoveTo(rect.left + rect.Width() / 2, rect.top);
			dc.LineTo(rect.left, rect.top);
			dc.LineTo(rect.left, rect.bottom);
			dc.LineTo(rect.left + rect.Width() / 2, rect.bottom);

			dc.SelectPen(pen3);
			rect.left += rect.Width() / 2;
			DrawRect(dc, rect);
		}
		else if ((nFillStyle == FillStyleGradientHorLeftLight && GetSelectColor() == SelectColor_Highlight) ||
			(nFillStyle == FillStyleGradientHorRightLight && GetSelectColor() == SelectColor_Color))
		{
			dc.SelectPen(pen);
					
			dc.MoveTo(rect.left + rect.Width() / 2, rect.top);
			dc.LineTo(rect.right, rect.top);
			dc.LineTo(rect.right, rect.bottom);
			dc.LineTo(rect.left + rect.Width() / 2, rect.bottom);
			
			dc.SelectPen(pen3);
			rect.right = rect.left + rect.Width() / 2;
			DrawRect(dc, rect);
		}
		else
		{
			dc.SelectPen(pen);
			DrawRect(dc, rect);
			DrawLine(dc, rect.left + rect.Width() / 2, rect.top, rect.left + rect.Width() / 2, rect.bottom);
		}
		break;


	case FillStyleGradientVerMiddleLight:
	case FillStyleGradientVerSideLight:
		if ((nFillStyle == FillStyleGradientVerMiddleLight && GetSelectColor() == SelectColor_Color) ||
			(nFillStyle == FillStyleGradientVerSideLight && GetSelectColor() == SelectColor_Highlight))
		{
			dc.SelectPen(pen);
			DrawLine(dc, rect.left, rect.top + rect.Height() / 4 , rect.left, rect.top + 3 * rect.Height() / 4);
			DrawLine(dc, rect.right, rect.top + rect.Height() / 4 , rect.right, rect.top + 3 * rect.Height() / 4);
			
			dc.SelectPen(pen3);
			CRect rectT = rect;
			rectT.bottom = rect.top + rect.Height() / 4;
			DrawRect(dc, rectT);
			rectT.OffsetRect(0, 3 * rect.Height() / 4);
			DrawRect(dc, rectT);
		}
		else if ((nFillStyle == FillStyleGradientVerMiddleLight && GetSelectColor() == SelectColor_Highlight) ||
			(nFillStyle == FillStyleGradientVerSideLight && GetSelectColor() == SelectColor_Color))
		{
			dc.SelectPen(pen);
			dc.MoveTo(rect.left, rect.top + rect.Height() / 4);
			dc.LineTo(rect.left, rect.top);
			dc.LineTo(rect.right, rect.top);
			dc.MoveTo(rect.right, rect.top + rect.Height() / 4);
			
			dc.MoveTo(rect.left, rect.top + 3 * rect.Height() / 4);
			dc.LineTo(rect.left, rect.bottom);
			dc.LineTo(rect.right, rect.bottom);
			dc.MoveTo(rect.right, rect.top + 3 * rect.Height() / 4);
			
			dc.SelectPen(pen3);
			rect.InflateRect(0, -rect.Height() / 4);
			DrawRect(dc, rect);
		}
		else
		{
			dc.SelectPen(pen);
			DrawRect(dc, rect);
			DrawLine(dc, rect.left, rect.top + rect.Height() / 4, rect.right, rect.top + rect.Height() / 4);
			DrawLine(dc, rect.left, rect.top + 3 * rect.Height() / 4, rect.right, rect.top + 3 * rect.Height() / 4);
		}
		break;
	case FillStyleGradientVerTopLight:
	case FillStyleGradientVerBottomLight:
		if ((nFillStyle == FillStyleGradientVerTopLight && GetSelectColor() == SelectColor_Color) ||
			(nFillStyle == FillStyleGradientVerBottomLight && GetSelectColor() == SelectColor_Highlight))
		{
			dc.SelectPen(pen);
			dc.MoveTo(rect.left, rect.top + rect.Height() / 2);
			dc.LineTo(rect.left, rect.top);
			dc.LineTo(rect.right, rect.top);
			dc.LineTo(rect.right, rect.top + rect.Height() / 2);
			
			dc.SelectPen(pen3);
			rect.top += rect.Height() / 2;
			DrawRect(dc, rect);
		}
		else if ((nFillStyle == FillStyleGradientVerTopLight && GetSelectColor() == SelectColor_Highlight) ||
			(nFillStyle == FillStyleGradientVerBottomLight && GetSelectColor() == SelectColor_Color))
		{
			dc.SelectPen(pen);
			
			dc.MoveTo(rect.left, rect.top + rect.Height() / 2);
			dc.LineTo(rect.left, rect.bottom);
			dc.LineTo(rect.right, rect.bottom);
			dc.LineTo(rect.right, rect.top + rect.Height() / 2);
			
			dc.SelectPen(pen3);
			rect.bottom = rect.top + rect.Height() / 2;
			DrawRect(dc, rect);
		}
		else
		{
			dc.SelectPen(pen);
			DrawRect(dc, rect);
			DrawLine(dc, rect.left, rect.top + rect.Height() / 2, rect.right, rect.top + rect.Height() / 2);
		}
		break;

	case FillStyleGradientLeftTopLight:
	case FillStyleGradientRightBottomLight:
		if ((nFillStyle == FillStyleGradientLeftTopLight && GetSelectColor() == SelectColor_Color) ||
			(nFillStyle == FillStyleGradientRightBottomLight && GetSelectColor() == SelectColor_Highlight))
		{
			dc.SelectPen(pen);
			dc.MoveTo(rect.left, rect.bottom);
			dc.LineTo(rect.left, rect.top);
			dc.LineTo(rect.right, rect.top);

			dc.SelectPen(pen3);
			dc.LineTo(rect.right, rect.bottom);
			dc.LineTo(rect.left, rect.bottom);
			dc.LineTo(rect.right, rect.top);
		}
		else if ((nFillStyle == FillStyleGradientLeftTopLight && GetSelectColor() == SelectColor_Highlight) ||
			(nFillStyle == FillStyleGradientRightBottomLight && GetSelectColor() == SelectColor_Color))
		{
			dc.SelectPen(pen);
			dc.MoveTo(rect.right, rect.top);
			dc.LineTo(rect.right, rect.bottom);
			dc.LineTo(rect.left, rect.bottom);
			
			dc.SelectPen(pen3);
			dc.LineTo(rect.left, rect.top);
			dc.LineTo(rect.right, rect.top);
			dc.LineTo(rect.left, rect.bottom);
		}
		else
		{
			dc.SelectPen(pen);
			dc.MoveTo(rect.left, rect.bottom);
			dc.LineTo(rect.left, rect.top);
			dc.LineTo(rect.right, rect.top);
			dc.LineTo(rect.right, rect.bottom);
			dc.LineTo(rect.left, rect.bottom);
			dc.LineTo(rect.right, rect.top);
		}
		break;

	case FillStyleGradientLeftBottomLight:
	case FillStyleGradientRightTopLight:
		if ((nFillStyle == FillStyleGradientLeftBottomLight && GetSelectColor() == SelectColor_Color) ||
			(nFillStyle == FillStyleGradientRightTopLight && GetSelectColor() == SelectColor_Highlight))
		{
			dc.SelectPen(pen);
			dc.MoveTo(rect.left, rect.top);
			dc.LineTo(rect.left, rect.bottom);
			dc.LineTo(rect.right, rect.bottom);
			
			dc.SelectPen(pen3);
			dc.LineTo(rect.right, rect.top);
			dc.LineTo(rect.left, rect.top);
			dc.LineTo(rect.right, rect.bottom);
		}
		else if ((nFillStyle == FillStyleGradientLeftBottomLight && GetSelectColor() == SelectColor_Highlight) ||
			(nFillStyle == FillStyleGradientRightTopLight && GetSelectColor() == SelectColor_Color))
		{
			dc.SelectPen(pen);
			dc.MoveTo(rect.left, rect.top);
			dc.LineTo(rect.right, rect.top);
			dc.LineTo(rect.right, rect.bottom);
			
			dc.SelectPen(pen3);
			dc.LineTo(rect.left, rect.bottom);
			dc.LineTo(rect.left, rect.top);
			dc.LineTo(rect.right, rect.bottom);
		}
		else
		{
			dc.SelectPen(pen);
			dc.MoveTo(rect.left, rect.top);
			dc.LineTo(rect.left, rect.bottom);
			dc.LineTo(rect.right, rect.bottom);
			dc.LineTo(rect.right, rect.top);
			dc.LineTo(rect.left, rect.top);
			dc.LineTo(rect.right, rect.bottom);
		}
		break;

	default:
		dc.SelectPen(GetSelectColor() == SelectColor_Color ? pen3 : pen);
		DrawRect(dc, rect);
		break;
	}

	if (hOldPen != NULL)
		dc.SelectPen(hOldPen);
}

LRESULT CFillPreviewWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	ATLASSERT(m_pExpert != NULL);
	if (m_pExpert->m_pSampleObject == NULL)
		return 0;

	int nFillStyle = m_pExpert->m_pSampleObject->m_enumFillStyle;
	if (nFillStyle == -1)
		return 0;

	POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	CRect rect;
	GetClientRect(&rect);

	int nSelectColor = SelectColor_Color;

	switch (nFillStyle) 
	{
	case FillStyleGradientCenterLight:
		rect.InflateRect(-rect.Width() / 4, -rect.Height() / 4);
		if (rect.PtInRect(point))
			nSelectColor = SelectColor_Highlight;
		break;
	case FillStyleGradientAroundLight:
		rect.InflateRect(-rect.Width() / 4, -rect.Height() / 4);
		if (!rect.PtInRect(point))
			nSelectColor = SelectColor_Highlight;
		break;
	case FillStyleGradientHorMiddleLight:
		rect.InflateRect(-rect.Width() / 4, 0);
		if (rect.PtInRect(point))
			nSelectColor = SelectColor_Highlight;
		break;
	case FillStyleGradientHorSideLight:
		rect.InflateRect(-rect.Width() / 4, 0);
		if (!rect.PtInRect(point))
			nSelectColor = SelectColor_Highlight;
		break;
	case FillStyleGradientHorLeftLight:
		rect.right = rect.left + rect.Width() / 2;
		if (rect.PtInRect(point))
			nSelectColor = SelectColor_Highlight;
		break;
	case FillStyleGradientHorRightLight:
		rect.right = rect.left + rect.Width() / 2;
		if (!rect.PtInRect(point))
			nSelectColor = SelectColor_Highlight;
		break;

	case FillStyleGradientVerMiddleLight:
		rect.InflateRect(0, -rect.Height() / 4);
		if (rect.PtInRect(point))
			nSelectColor = SelectColor_Highlight;
		break;
	case FillStyleGradientVerSideLight:
		rect.InflateRect(0, -rect.Height() / 4);
		if (!rect.PtInRect(point))
			nSelectColor = SelectColor_Highlight;
		break;
	case FillStyleGradientVerTopLight:
		rect.bottom = rect.top + rect.Height() / 2;
		if (rect.PtInRect(point))
			nSelectColor = SelectColor_Highlight;
		break;
	case FillStyleGradientVerBottomLight:
		rect.bottom = rect.top + rect.Height() / 2;
		if (!rect.PtInRect(point))
			nSelectColor = SelectColor_Highlight;
		break;
	case FillStyleGradientLeftTopLight:
		{
			CPoint points[3] = 
			{
				CPoint(rect.left, rect.bottom),
				CPoint(rect.left, rect.top),
				CPoint(rect.right, rect.top),
			};

			CRgn rgn;
			rgn.CreatePolygonRgn(points, 3, ALTERNATE);
			if (rgn.PtInRegion(point))
			nSelectColor = SelectColor_Highlight;
		}
		break;
	case FillStyleGradientRightBottomLight:
		{
			CPoint points[3] = 
			{
				CPoint(rect.left, rect.bottom),
					CPoint(rect.left, rect.top),
					CPoint(rect.right, rect.top),
			};
			
			CRgn rgn;
			rgn.CreatePolygonRgn(points, 3, ALTERNATE);
			if (!rgn.PtInRegion(point))
				nSelectColor = SelectColor_Highlight;
		}
		break;
	case FillStyleGradientLeftBottomLight:
		{
			CPoint points[3] = 
			{
				CPoint(rect.left, rect.top),
				CPoint(rect.left, rect.bottom),
				CPoint(rect.right, rect.bottom),
			};
			
			CRgn rgn;
			rgn.CreatePolygonRgn(points, 3, ALTERNATE);
			if (rgn.PtInRegion(point))
				nSelectColor = SelectColor_Highlight;
		}
		break;
	case FillStyleGradientRightTopLight:
		{
			CPoint points[3] = 
			{
				CPoint(rect.left, rect.top),
				CPoint(rect.left, rect.bottom),
				CPoint(rect.right, rect.bottom),
			};
			
			CRgn rgn;
			rgn.CreatePolygonRgn(points, 3, ALTERNATE);
			if (!rgn.PtInRegion(point))
				nSelectColor = SelectColor_Highlight;
		}
		break;
		
	default:
		break;
	}

	if (GetSelectColor() != nSelectColor)
	{
		SetSelectColor(nSelectColor);
		Invalidate();
	}
	
	return 0;
}

int CFillPreviewWnd::GetSelectColor()
{
	if (m_pExpert->m_dwColorPropDispID == CXPROPID_FillColor)
		return SelectColor_Color;
	if (m_pExpert->m_dwColorPropDispID == CXPROPID_HighLightFillColor)
		return SelectColor_Highlight;

	return SelectColor_None;
}

void CFillPreviewWnd::SetSelectColor(int nColor)
{
	m_pExpert->OnColorPropSelecting(this);

	switch (nColor) 
	{
	case SelectColor_Color:
		m_pExpert->m_dwColorPropDispID = CXPROPID_FillColor;
		break;
	case SelectColor_Highlight:
		m_pExpert->m_dwColorPropDispID = CXPROPID_HighLightFillColor;
		break;
	default:
		return;
	}

	m_pExpert->OnColorPropSelected(this);
}

