// LineStyleWnd.h: interface for the CLineStyleWnd class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _LINESTYLEWND_H__
#define _LINESTYLEWND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAppearanceExpert;

class CLineStyleWnd : public CWindowImpl<CLineStyleWnd>  
{
public:
	CLineStyleWnd();
	virtual ~CLineStyleWnd();
	
	BEGIN_MSG_MAP(CLineStyleWnd)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	END_MSG_MAP()
		
	LRESULT OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	
	void DrawItem(CDCHandle dc, CRect& rect, int nLineStyle, BOOL bSelected);
	void GetItemRect(int nLineStyle, CRect& rect);

	BOOL StartTrackMouseLeave();
	
	CAppearanceExpert* m_pExpert;
	int m_nSize;
	CToolTipCtrl m_tip;
};

#endif // _LINESTYLEWND_H__
