// KeyBoardWnd.h: interface for the CKeyBoardWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYBOARDWND_H__4AE96230_9110_42BB_8098_A058FB7C8D88__INCLUDED_)
#define AFX_KEYBOARDWND_H__4AE96230_9110_42BB_8098_A058FB7C8D88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKeyBoardWnd : public CWindowImpl<CKeyBoardWnd>    
{
public:
	CKeyBoardWnd();
	virtual ~CKeyBoardWnd();
	
BEGIN_MSG_MAP(CKeyBoardWnd)
	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDown)
END_MSG_MAP()

	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	
//	void DrawItem(CDCHandle dc, CRect& rect, LPCTSTR lpszText, );
//	void GetItemRect(int nFillStyle, CRect& rect);
};

#endif // !defined(AFX_KEYBOARDWND_H__4AE96230_9110_42BB_8098_A058FB7C8D88__INCLUDED_)
