// StyleWnd.h: interface for the CStyleWnd class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _STYLEWND_H__
#define _STYLEWND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_DRAWSTYLEWINDOW			WM_USER+230	
#define WM_SELECTSTYLEWINDOW		WM_USER+231	

typedef struct tagDRAWSTYLEWINDOWSTRUCT
{
    HDC hDC;
    RECT rcItem;
} DRAWSTYLEWINDOWSTRUCT;


class CStyleWnd : public CWindowImpl<CStyleWnd>  
{
public:
	CStyleWnd();
	virtual ~CStyleWnd();

	BEGIN_MSG_MAP(CStyleWnd)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	END_MSG_MAP()

	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
};

#endif // _STYLEWND_H__
