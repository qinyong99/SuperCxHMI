// ColorCell.cpp: implementation of the CColorCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ColorCell.h"
#include <math.h>

#define FORMMIN(X,Y) (((X) < (Y)) ? (X) : (Y))
#define FORMMAX(X,Y) (((X) > (Y)) ? (X) : (Y))

const double TAN30 = 0.57735026918962;

_GLOBAL_DATA globalData;

_GLOBAL_DATA::_GLOBAL_DATA()
{
	OnUpdateMetrics();
	OnSysColorChange();
	
	OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	
	::GetVersionEx (&osvi);
	bNT4 = ((osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
		(osvi.dwMajorVersion < 5));
}

_GLOBAL_DATA::~_GLOBAL_DATA()
{
}

void _GLOBAL_DATA::OnSysColorChange()
{
	m_crBtnFace = GetSysColor(COLOR_BTNFACE);
	m_crBtnShadow = GetSysColor(COLOR_BTNSHADOW);
	m_crBtnDkShadow = GetSysColor(COLOR_3DDKSHADOW);
	m_crBtnLight = GetSysColor(COLOR_3DLIGHT);
	m_crBtnHilite = GetSysColor(COLOR_BTNHIGHLIGHT);
	m_crBtnText = GetSysColor(COLOR_BTNTEXT);
	m_crTextGrayed = GetSysColor (COLOR_GRAYTEXT);
	m_crWindowFrame = GetSysColor(COLOR_WINDOWFRAME);
	
	m_crHilite = GetSysColor(COLOR_HIGHLIGHT);
	m_crTextHilite = GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_crWindowText = GetSysColor(COLOR_WINDOWTEXT);
	m_crBtn3dShadow = GetSysColor(COLOR_3DSHADOW);
	m_crBtn3dHiLight = GetSysColor(COLOR_3DHILIGHT);
	m_crAcCaption = GetSysColor(COLOR_ACTIVECAPTION);
	m_crInAcCaption = GetSysColor(COLOR_INACTIVECAPTION);
	m_crInAcCaptionText = GetSysColor(COLOR_INACTIVECAPTIONTEXT);
	m_crDesktop = GetSysColor(COLOR_DESKTOP);
	
	//m_cr3dFace = gfxData.m_cr3dFace;
#ifdef COLOR_HOTLIGHT
	
	OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	
	::GetVersionEx (&osvi);
	if (osvi.dwMajorVersion >= 5)
	{
		m_crTextHot = GetSysColor (COLOR_HOTLIGHT);
	}
	else
#endif
	{
		m_crTextHot = RGB (0, 0, 255);	// Light blue
	}
	
	CWindowDC dc (NULL);
	m_nBitsPerPixel = dc.GetDeviceCaps (BITSPIXEL);
	
}

CSize _GLOBAL_DATA::GetScreenDimensions()
{
	OnUpdateMetrics();
	return CSize(m_cxScreen, m_cyScreen);
}

void _GLOBAL_DATA::OnUpdateMetrics()
{
	m_cxEdge = ::GetSystemMetrics(SM_CXEDGE);
	m_cyEdge = ::GetSystemMetrics(SM_CYEDGE);
	m_cxBorder = ::GetSystemMetrics(SM_CXBORDER);
	m_cxScreen = ::GetSystemMetrics(SM_CXSCREEN);
	m_cyScreen = ::GetSystemMetrics(SM_CYSCREEN);
	m_cxVScroll = ::GetSystemMetrics(SM_CXVSCROLL);
	m_cxFrame = ::GetSystemMetrics(SM_CXFRAME);
	m_cyFrame = ::GetSystemMetrics(SM_CYFRAME);
	m_cxFixedFrame = ::GetSystemMetrics(SM_CXFIXEDFRAME);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColorCell::CColorCell()
{
	m_crCell = RGB(255, 255, 255);
	m_nCellBorderSize = 1;
	m_bAtFirstType = TRUE;
}

CColorCell::~CColorCell()
{
}

CRect CColorCell::GetRect() const
{
	if (!m_bAtFirstType)
		return m_rcPosition;
	
	CRect rcBounds;
	
	rcBounds.left = rcBounds.right  = m_Points[0].x;
	rcBounds.top  = rcBounds.bottom = m_Points[0].y;
	
	for (int i = 1; i < 6; i++)
	{
		rcBounds.left   = FORMMIN(rcBounds.left,   m_Points[i].x);
		rcBounds.top    = FORMMIN(rcBounds.top,    m_Points[i].y);
		rcBounds.right  = FORMMAX(rcBounds.right,  m_Points[i].x);
		rcBounds.bottom = FORMMAX(rcBounds.bottom, m_Points[i].y);
	}
	
	return rcBounds;
}

void CColorCell::SetCellColor(const COLORREF &cr) 
{ 
	m_crCell = cr; 
}

void CColorCell::OnDraw(CDCHandle dc)
{
	CBrush brush;
	brush.CreateSolidBrush(m_crCell);
	CBrushHandle brushOld = dc.SelectBrush(brush);
	CPenHandle penOld = dc.SelectStockPen(NULL_PEN);

	CRect rcPos;
	rcPos = m_rcPosition;
	dc.FillSolidRect(rcPos, GetSysColor(COLOR_BTNFACE));

	rcPos.DeflateRect(m_nCellBorderSize, m_nCellBorderSize);
	if (!m_bAtFirstType)
	{
		dc.Rectangle(rcPos);
	}
	else
	{
		dc.Polygon (m_Points, 6);
	}

	dc.SelectBrush(brushOld);
	dc.SelectPen(penOld);
}

void CColorCell::OnDrawSelect(CDCHandle dc)
{
	CBrushHandle brushWhite = (HBRUSH) ::GetStockObject (WHITE_BRUSH);
	CBrushHandle brushBlank = (HBRUSH) ::GetStockObject (BLACK_BRUSH);
	
	if (m_bAtFirstType)
	{
		if (m_crCell == RGB(255,255,255))
		{
			CRgn rgnOne;
			rgnOne.CreatePolygonRgn(m_Points, 6, ALTERNATE);
			dc.FrameRgn(rgnOne, brushWhite, 3, 3);

			dc.FrameRgn(rgnOne, brushBlank, 2, 2);
		}
		else
		{
			CRgn rgnOne;
			rgnOne.CreatePolygonRgn(m_Points, 6, ALTERNATE);
			dc.FrameRgn(rgnOne, brushWhite, 3, 3);

			dc.FrameRgn(rgnOne, brushBlank, 1, 1);
		}
	}
	else
	{
		CRgn rgnOne, rgnTwo, rgnThree;
		
		CRect rcPos;
		rcPos = m_rcPosition;
		rcPos.DeflateRect(1, 1, 1, 1);
		
		rgnTwo.CreateRectRgnIndirect(&rcPos);
		dc.FrameRgn(rgnTwo, brushWhite, 2, 2);
		rcPos.InflateRect(2, 2, 2, 2);
		
		rgnThree.CreateRectRgnIndirect(&rcPos);
		dc.FrameRgn(rgnThree, brushBlank, 1, 1);
	}
}

int CColorCell::MoreNear(float fNear)
{
	int nRound = (int)fabs(fNear);
	nRound = (fabs(fNear) - (float)nRound >= 0.5) ? nRound+1 : nRound;
	return (fNear < 0) ? -nRound : nRound;
}

void CColorCell::AdjustCellPosition(int x, int y, int nWidth)
{
	float nSideLength = (float)((float)nWidth * TAN30);
	m_Points[0] = CPoint(MoreNear(x - (float)(nWidth / 2)),MoreNear(y - (float)(nSideLength/2)));
	m_Points[1] = CPoint(MoreNear((float)x),MoreNear(y - (float)(nWidth / 2)));
	m_Points[2] = CPoint(MoreNear(x + (float)(nWidth / 2)),MoreNear(y - (float)(nSideLength/2)));
	m_Points[3] = CPoint(MoreNear(x + (float)(nWidth / 2)),MoreNear(y + (float)(nSideLength/2))+1);
	m_Points[4] = CPoint(MoreNear((float)x),MoreNear(y + (float)(nWidth / 2))+1);
	m_Points[5] = CPoint(MoreNear(x - (float)(nWidth / 2)),MoreNear(y + (float)(nSideLength/2))+1);
}

BOOL CColorCell::HitTest(CPoint point)
{
	if (m_bAtFirstType)
	{
		CRgn rgn;
		rgn.CreatePolygonRgn(m_Points, 6, ALTERNATE);
		return rgn.PtInRegion(point);
	}
	else
	{
		CRgn rgn;
		rgn.CreateRectRgn(m_rcPosition.left, m_rcPosition.top, m_rcPosition.right, m_rcPosition.bottom);
		return rgn.PtInRegion(point);
	}
}
