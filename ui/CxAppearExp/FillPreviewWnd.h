// FillPreviewWnd.h: interface for the CFillPreviewWnd class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FILLPREVIEWWND_H__
#define _FILLPREVIEWWND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAppearanceExpert;

class CFillPreviewWnd : public CWindowImpl<CFillPreviewWnd>  
{
	enum 
	{
		SelectColor_None = 0,
		SelectColor_Color = 1,
		SelectColor_Highlight = 2,
	};

public:
	CFillPreviewWnd();
	virtual ~CFillPreviewWnd();

	BEGIN_MSG_MAP(CFillPreviewWnd)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	END_MSG_MAP()
		
	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);

	void DrawColorEdge(CDCHandle dc, CRect& rect, int nFillStyle, COLORREF color);
	int GetSelectColor();
	void SetSelectColor(int nColor);
	
	CAppearanceExpert* m_pExpert;
};

#endif // _FILLPREVIEWWND_H__
