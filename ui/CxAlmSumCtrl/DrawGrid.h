// DrawGrid.h: interface for the CDrawGrid class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DRAWGRID_H_
#define __DRAWGRID_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DrawCtrl.h"

class CAlmSum;

class CDrawGrid : public CDrawCtrl  
{
public:
	CDrawGrid();
	virtual ~CDrawGrid();

BEGIN_ACT_MAP(CDrawGrid)
	ACTION_HANDLER(WM_LBUTTONDOWN, DoLButtonDown)
	ACTION_HANDLER(WM_LBUTTONUP, DoLButtonUp)
	ACTION_HANDLER(WM_MOUSEMOVE, DoMouseMove)
END_ACT_MAP()

	LRESULT DoLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT DoLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT DoMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	virtual void DoDraw(HDC hdc);

	CAlmSum* m_pAlmSum;
	CPen m_penGrid;
};

#endif // __DRAWGRID_H_