// LinePreviewWnd.h: interface for the CLinePreviewWnd class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _LINEPREVIEWWND_H__
#define _LINEPREVIEWWND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAppearanceExpert;

class CLinePreviewWnd : public CWindowImpl<CLinePreviewWnd>  
{
	enum 
	{
		SelectColor_None = 0,
		SelectColor_Color = 1,
		SelectColor_Highlight = 2,
	};
public:
	CLinePreviewWnd();
	virtual ~CLinePreviewWnd();
	
	BEGIN_MSG_MAP(CLinePreviewWnd)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDown)
	END_MSG_MAP()
		
	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	
	void DoPaint(CDCHandle dc, CRect& rect);
	void DrawJoin(CDCHandle dc, CRect& rect);
	void DrawDetail(CDCHandle dc, CRect& rect);
	void DrawLine(CDCHandle dc, CRect& rect);

	int GetSelectColor();
	void SetSelectColor(int nColor);

	CAppearanceExpert* m_pExpert;
};

#endif // _LINEPREVIEWWND_H__
