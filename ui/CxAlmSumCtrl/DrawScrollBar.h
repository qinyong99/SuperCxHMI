// DrawScrollBar.h: interface for the CDrawScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWSCROLLBAR_H__B61961F4_7588_438D_AC3D_BAD446ACE1F4__INCLUDED_)
#define AFX_DRAWSCROLLBAR_H__B61961F4_7588_438D_AC3D_BAD446ACE1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DrawCtrl.h"

class CAlmSum;

enum enumDirection
{
	LEFT,
	RIGHT,
	BOTTOM,
	TOP,
	CENTER
};

class CDrawScrollBar : public CDrawCtrl  
{
public:
	CDrawScrollBar(BOOL bVertical = TRUE);

	virtual ~CDrawScrollBar();

BEGIN_ACT_MAP(CDrawScrollBar)
	ACTION_HANDLER(WM_LBUTTONDOWN, DoLButtonDown)
	ACTION_HANDLER(WM_LBUTTONUP, DoLButtonUp)
	ACTION_HANDLER(WM_MOUSEMOVE, DoMouseMove)
END_ACT_MAP()

	LRESULT DoLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT DoLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT DoMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void Enable(BOOL bEnable);
	int GetPos() { return m_nPos; }
	void SetPos(int nPos);
	void SetRange(int nMin, int nMax);
	void SetPage(int nPage);
	int GetPage() { return m_nPage; }
	int GetMin() { return m_nMin; }
	int GetMax() { return m_nMax; }

	virtual void DoDraw(HDC hdc);
	virtual void RecalcRect(RECT& rect);

	CAlmSum* m_pAlmSum;

protected:
	void DrawBack(HDC hdc, RECT& rect);
	void GetItemRects(RECT rcTotal, RECT& rcMinButton, RECT& rcMaxButton, RECT& rcMidButton);
	void DrawArrowBtn(HDC hdc, RECT& rect, enum enumDirection way);

	int m_nCatpture; //-1无，1上按钮，2中间块，3下按钮，4空白
	POINT m_ptLastLButtonDown;
	int m_nOffestMinButton;

	BOOL m_bVertical;
	BOOL m_bEnable;
	int m_nMin;
	int m_nMax;
	int m_nPos;
	int m_nPage;
	HBITMAP m_hBmpDisable;
	HBITMAP m_hBmpEnable;
};

#endif // !defined(AFX_DRAWSCROLLBAR_H__B61961F4_7588_438D_AC3D_BAD446ACE1F4__INCLUDED_)
