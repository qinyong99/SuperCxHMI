// LineStyleWnd.cpp: implementation of the CLineStyleWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxAppearExp.h"
#include "AppearanceExpert.h"
#include "SampleObject.h"
#include "LineStyleWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineStyleWnd::CLineStyleWnd()
{
	m_pExpert = NULL;
	m_nSize = 24;
}

CLineStyleWnd::~CLineStyleWnd()
{
	
}

const LPCTSTR szLineStyleTooltipText[] = 
{
	_T("实线"),
	_T("虚线"),
	_T("点线"),
	_T("点划线"),
	_T("双点划线"),
	_T("管道线"),
	_T("透明线")
};	

LRESULT CLineStyleWnd::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	m_tip.Create(m_hWnd);
	ATLASSERT(m_tip.IsWindow());
	
	for (int i = 0; i < 7; i++)
	{
		CRect rect;
		GetItemRect(i, rect);
// 		CToolInfo toolInfo(0, m_hWnd, 1, &rect, (char *)szLineStyleTooltipText[i]);
		m_tip.AddTool(m_hWnd, szLineStyleTooltipText[i], &rect, 1);
// 		m_tip.AddTool(&toolInfo);
	}

	return 1;
}

LRESULT CLineStyleWnd::OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CDCHandle dc = (HDC)wParam;
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillRect(&rcClient, COLOR_3DFACE);

	return 1;   // no background needed
}

LRESULT CLineStyleWnd::OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	ATLASSERT(m_pExpert != NULL);
	if (m_pExpert->m_pSampleObject == NULL)
		return 0;
	
	CPaintDC dc(m_hWnd);
	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillRect(&rcClient, COLOR_3DFACE);

	for (int i = 0; i < 7; i++)
	{
		CRect rect;
		GetItemRect(i, rect);
		DrawItem(dc.m_hDC, rect, i, i == m_pExpert->m_pSampleObject->m_enumEdgeStyle);
	}
		
	return 0;
}

void CLineStyleWnd::GetItemRect(int nLineStyle, CRect& rect)
{
	if (nLineStyle == -1)
	{
		rect.SetRectEmpty();
		return;
	}

	ATLASSERT(nLineStyle >= EdgeStyleSolid && nLineStyle <= EdgeStyleTransparence);
	
	rect.left = 0;
	rect.top = m_nSize * nLineStyle;
	rect.right = m_nSize * 4;
	rect.bottom = rect.top + m_nSize;
}

void CLineStyleWnd::DrawItem(CDCHandle dc, CRect& rect, int nLineStyle, BOOL bSelected)
{
	using namespace Gdiplus;

	dc.DrawEdge(&rect, bSelected ? BDR_RAISEDOUTER : BDR_SUNKENOUTER, BF_RECT|BF_ADJUST);

	CSampleObject object;
	object.ICxDrawObject_InitNew();
	object.m_enumEdgeStyle = (enumEdgeStyle)nLineStyle;
	object.m_clrEdgeColor = RGB(0, 0, 0);
	object.m_enumLineJoinStyle = LineJoinStyleSquare;
	if (object.m_enumEdgeStyle == EdgeStylePipe)
	{
		object.m_fEdgeWidth = 6;
		object.m_clrHighLightEdge = RGB(255, 255, 255);
	}
	else
	{
		object.m_fEdgeWidth = 2;
	}
	
	GraphicsPath path;
	path.AddLine(rect.left + 8, (rect.top + rect.bottom) / 2, 
		rect.right - 8, (rect.top + rect.bottom) / 2);	
	
	Graphics graphics(dc.m_hDC);
	object.DrawObjectEdge(&graphics, &path);
	
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

LRESULT CLineStyleWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	ATLASSERT(m_pExpert != NULL);
	if (m_pExpert->m_pSampleObject == NULL)
		return 0;

	POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

	for (int i = 0; i < 7; i++)
	{
		CRect rect;
		GetItemRect(i, rect);
		if (rect.PtInRect(point))
		{
			if (i != m_pExpert->m_pSampleObject->m_enumEdgeStyle)
			{
				CRect rectT;
				GetItemRect(m_pExpert->m_pSampleObject->m_enumEdgeStyle, rectT);
				InvalidateRect(rectT, FALSE);
				
				m_pExpert->m_pSampleObject->m_enumEdgeStyle = (enumEdgeStyle)i;
				InvalidateRect(rect, FALSE);
				
	 			m_pExpert->OnProertyChanged(CXPROPID_EdgeStyle, this);
			}
			
			break;
		}
	}
	
	return 0;
}

LRESULT CLineStyleWnd::OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MSG msg = { m_hWnd, uMsg, wParam, lParam };
	if(m_tip.IsWindow())
		m_tip.RelayEvent(&msg);
	bHandled = FALSE;
	return 1;
}




