// FillStyleWnd.cpp: implementation of the CFillStyleWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxAppearExp.h"
#include "AppearanceExpert.h"
#include "SampleObject.h"
#include "FillStyleWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFillStyleWnd::CFillStyleWnd()
{
	m_pExpert = NULL;
	m_nSize = 24;
}

CFillStyleWnd::~CFillStyleWnd()
{
	
}

const LPCTSTR szFillStyleTooltipText[] = 
{
	_T("透明"),
	_T("实心"),
	_T("水平线"),
	_T("垂直线"),
	_T("左斜线"),
	_T("右斜线"),
	_T("正交线"),
	_T("斜交线"),
	_T("左上亮"),
	_T("右上亮"),
	_T("右下亮"),
	_T("左下亮"),
	_T("水平中亮"), 
	_T("水平边亮"),
	_T("水平左亮"),
	_T("水平右亮"),
	_T("垂直中亮"),
	_T("垂直边亮"), 
	_T("垂直上亮"),
	_T("垂直下亮"),
	_T("中心亮"),
	_T("四边亮"),
	_T("位图")
};	

LRESULT CFillStyleWnd::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	m_tip.Create(m_hWnd);
	ATLASSERT(m_tip.IsWindow());
	
	for (int i = 0; i < 23; i++)
	{
		CRect rect;
		GetItemRect(i, rect);
		CToolInfo toolInfo(0, m_hWnd, 1, &rect, (char *)szFillStyleTooltipText[i]);
		m_tip.AddTool(&toolInfo);
	}

	return 1;
}

LRESULT CFillStyleWnd::OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CDCHandle dc = (HDC)wParam;
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillRect(&rcClient, COLOR_3DFACE);

	return 1;   // no background needed
}

LRESULT CFillStyleWnd::OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	ATLASSERT(m_pExpert != NULL);
	if (m_pExpert->m_pSampleObject == NULL)
		return 0;
	
	CPaintDC dc(m_hWnd);
	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillRect(&rcClient, COLOR_3DFACE);

	for (int i = 0; i < 23; i++)
	{
		CRect rect;
		GetItemRect(i, rect);
		DrawItem(dc.m_hDC, rect, i, i == m_pExpert->m_pSampleObject->m_enumFillStyle);
	}
		
	return 0;
}

void CFillStyleWnd::GetItemRect(int nFillStyle, CRect& rect)
{
	if (nFillStyle == -1)
	{
		rect.SetRectEmpty();
		return;
	}

	ATLASSERT(nFillStyle >= FillStyleSolid && nFillStyle <= FillStyleImage);
	
	if (nFillStyle == FillStyleImage)
		nFillStyle = (nFillStyle + 4) / 4 * 4;
	
	rect.left = nFillStyle % 4 * m_nSize;
	rect.top = nFillStyle / 4 * m_nSize;
	rect.right = rect.left + m_nSize;
	rect.bottom = rect.top + m_nSize;
}

void CFillStyleWnd::DrawItem(CDCHandle dc, CRect& rect, int nFillStyle, BOOL bSelected)
{
	using namespace Gdiplus;

	dc.DrawEdge(&rect, bSelected ? BDR_RAISEDOUTER : BDR_SUNKENOUTER, BF_RECT|BF_ADJUST);
	if (nFillStyle == FillStyleTransparence ||  /*nFillStyle == FillStyleImage || */
		(nFillStyle >= FillStyleHatchHorizontal && nFillStyle <= FillStyleHatchDiagonalCross))
	{
		dc.FillRect(&rect, COLOR_3DFACE);
	}
	else
	{
		CRect rc = rect;
		dc.FrameRect(&rc, ::GetSysColorBrush(COLOR_3DFACE));
		rc.InflateRect(-1, -1);
		dc.FrameRect(&rc, ::GetSysColorBrush(COLOR_3DFACE));
	}
	
	rect.InflateRect(-1, -1, -1, -1);
//	dc.FrameRect(&rect, ::GetSysColorBrush(COLOR_3DFACE));
	
	if (nFillStyle != FillStyleImage)
	{
		CSampleObject object;
		object.ICxDrawObject_InitNew();
		object.m_clrFillColor = RGB(0, 0, 0);
		object.m_clrHighLightFill = RGB(255, 255, 255);
		object.m_enumFillStyle = (enumFillStyle)nFillStyle;
		
		Rect rectGdiplus(rect.left, rect.top, rect.Width(), rect.Height());
		GraphicsPath path;
		path.AddRectangle(rectGdiplus);
		
		Graphics graphics(dc.m_hDC);
		object.FillObjectRgn(&graphics, &path);
	}
	else
	{
		CDC dcMem;
		dcMem.CreateCompatibleDC(dc);
		
		HBITMAP hBmpOld = dcMem.SelectBitmap(m_bitmapImage);
						
		dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), dcMem, 0, 0, SRCCOPY);
		dcMem.SelectBitmap(hBmpOld);		// restore
	}
	
	rect.InflateRect(1, 1, 1, 1);
	if (bSelected)
	{
		dc.FrameRect(&rect, (HBRUSH)::GetStockObject(WHITE_BRUSH));
		rect.InflateRect(-1, -1, -1, -1);
		dc.FrameRect(&rect, (HBRUSH)::GetStockObject(WHITE_BRUSH));
		rect.InflateRect(-1, -1, -1, -1);
		dc.FrameRect(&rect, (HBRUSH)::GetStockObject(WHITE_BRUSH));
	}
}

LRESULT CFillStyleWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	ATLASSERT(m_pExpert != NULL);
	if (m_pExpert->m_pSampleObject == NULL)
		return 0;

	POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

	for (int i = 0; i < 23; i++)
	{
		CRect rect;
		GetItemRect(i, rect);
		if (rect.PtInRect(point))
		{
			if (i != m_pExpert->m_pSampleObject->m_enumFillStyle)
			{
				CRect rectT;
				GetItemRect(m_pExpert->m_pSampleObject->m_enumFillStyle, rectT);
				InvalidateRect(rectT, FALSE);
				
				m_pExpert->m_pSampleObject->m_enumFillStyle = (enumFillStyle)i;
				InvalidateRect(rect, FALSE);
				
				m_pExpert->OnProertyChanged(CXPROPID_FillStyle, this);
			}
			
			break;
		}
	}
	
	return 0;
}

LRESULT CFillStyleWnd::OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MSG msg = { m_hWnd, uMsg, wParam, lParam };
	if(m_tip.IsWindow())
		m_tip.RelayEvent(&msg);
	bHandled = FALSE;
	return 1;
}


