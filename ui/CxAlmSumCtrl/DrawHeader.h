// DrawHeader.h: interface for the CDrawHeader class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DRAWHEADER_H_
#define __DRAWHEADER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DrawCtrl.h"

__declspec(selectany) struct
{
	enum { cxWidth = 32, cyHeight = 32 };
	int xHotSpot;
	int yHotSpot;
	unsigned char arrANDPlane[cxWidth * cyHeight / 8];
	unsigned char arrXORPlane[cxWidth * cyHeight / 8];
} _DivideColumn_CursorData = 
{
	8, 8,
	{
		0Xfe, 0X1f, 0Xff, 0Xff, 0Xfe, 0X1f, 0Xff, 0Xff,
			0Xfe, 0X1f, 0Xff, 0Xff, 0Xfe, 0X1f, 0Xff, 0Xff,
			0Xfe, 0X1f, 0Xff, 0Xff, 0Xe6, 0X19, 0Xff, 0Xff,
			0Xc6, 0X18, 0Xff, 0Xff, 0X80, 0X00, 0X7f, 0Xff,
			0X00, 0X00, 0X3f, 0Xff, 0X00, 0X00, 0X3f, 0Xff,
			0X80, 0X00, 0X7f, 0Xff, 0Xc6, 0X18, 0Xff, 0Xff,
			0Xe6, 0X19, 0Xff, 0Xff, 0Xfe, 0X1f, 0Xff, 0Xff,
			0Xfe, 0X1f, 0Xff, 0Xff, 0Xfe, 0X1f, 0Xff, 0Xff,
			0Xfe, 0X1f, 0Xff, 0Xff, 0Xfe, 0X1f, 0Xff, 0Xff,
			0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff,
			0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff,
			0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff,
			0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff,
			0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff,
			0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff,
			0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff, 0Xff
	},
	{
			0X01, 0Xe0, 0X00, 0X00, 0X01, 0X20, 0X00, 0X00,
				0X01, 0X20, 0X00, 0X00, 0X01, 0X20, 0X00, 0X00,
				0X01, 0X20, 0X00, 0X00, 0X19, 0X26, 0X00, 0X00,
				0X29, 0X25, 0X00, 0X00, 0X4f, 0X3c, 0X80, 0X00,
				0X80, 0X00, 0X40, 0X00, 0X80, 0X00, 0X40, 0X00,
				0X4f, 0X3c, 0X80, 0X00, 0X29, 0X25, 0X00, 0X00,
				0X19, 0X26, 0X00, 0X00, 0X01, 0X20, 0X00, 0X00,
				0X01, 0X20, 0X00, 0X00, 0X01, 0X20, 0X00, 0X00,
				0X01, 0X20, 0X00, 0X00, 0X01, 0Xe0, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00
		}
};

class CAlmSum;

class CDrawHeader : public CDrawCtrl  
{
public:
	CDrawHeader();
	virtual ~CDrawHeader();

BEGIN_ACT_MAP(CDrawGrid)
	ACTION_HANDLER(WM_LBUTTONDOWN, DoLButtonDown)
	ACTION_HANDLER(WM_LBUTTONUP, DoLButtonUp)
	ACTION_HANDLER(WM_MOUSEMOVE, DoMouseMove)
END_ACT_MAP()

	LRESULT DoLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT DoLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT DoMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BOOL MouseOverColumnResizeArea(POINT& point);
	BOOL GetResizeColumn(POINT point, int& nCol, int& nOffsetX);
	int GetColumn(POINT point);
	
	virtual void DoDraw(HDC hdc);
	virtual void RecalcRect(RECT& rect);

public:
	CAlmSum* m_pAlmSum;

	int m_nColumnCatpture;

	BOOL m_bColumnResizing;
	int m_nResizeCaptureRange;
	HCURSOR m_hColDivCursor;
	int m_nResizingColumn;
	int m_nDragDivideOffset;
	CPoint m_ptLastLButtonDown;
	CPoint m_ptLastMousePoint;
};

#endif // __DRAWHEADER_H_
