// FillStyleWnd.h: interface for the CFillStyleWnd class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FILLSTYLEWND_H__
#define _FILLSTYLEWND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAppearanceExpert;

class CFillStyleWnd : public CWindowImpl<CFillStyleWnd>  
{
public:
	CFillStyleWnd();
	virtual ~CFillStyleWnd();

	BEGIN_MSG_MAP(CFillStyleWnd)
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

	void DrawItem(CDCHandle dc, CRect& rect, int nFillStyle, BOOL bSelected);
	void GetItemRect(int nFillStyle, CRect& rect);

	CAppearanceExpert* m_pExpert;
	CBitmap m_bitmapImage;
	int m_nSize;
	CToolTipCtrl m_tip;
};

#endif // _FILLSTYLEWND_H__
