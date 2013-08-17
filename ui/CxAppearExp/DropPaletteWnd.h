// DropPaletteWnd.h: interface for the CDropPaletteWnd class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DROPPALETTEWND_H__
#define _DROPPALETTEWND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DropPalette.h"

class CDropPaletteWnd : public CWindowImpl<CDropPaletteWnd>
{
	// Construction
public:
	CDropPaletteWnd();
	
	// Attributes
public:
	
	//Create
	BOOL Create(HWND hWndParent, RECT rcPos, DWORD dwStyle, UINT nID, COLORREF crColor, BOOL bPopup);
	
	// Operations
	//Get ColorControl
	CDropPalette* GetColorControl()	{ return m_pColorControl; }
	
protected:
	CDropPalette* m_pColorControl;
	
public:
	
	// Destructor
	virtual ~CDropPaletteWnd();
	void SetCurColor(COLORREF clrCur);
	
	// Generated message map functions
protected:
	BEGIN_MSG_MAP(CDropPaletteWnd)
		MESSAGE_HANDLER(WM_CANCELMODE, OnCancelMode)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUP)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SYSCOLORCHANGE, OnSysColorChange)
		
		MESSAGE_HANDLER(WM_SELECTCOLOROK, OnSelectDayOK)
		MESSAGE_HANDLER(WM_SELECTCOLORCANCEL, OnSelectDayCancel)
		MESSAGE_HANDLER(WM_SELECTCOLORCUSTOM, OnSelectCustom)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_SYSCOLORCHANGE, OnSysColorChange)
	END_MSG_MAP()
		
protected:
	
	// The pointer to Notify Window
	HWND  pNotifyWnd;
	
	LRESULT OnCancelMode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysColorChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = true;
		return 0;
	}
	
	LRESULT OnSelectDayOK(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSelectDayCancel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSelectCustom(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
};

#endif // _DROPPALETTEWND_H__
