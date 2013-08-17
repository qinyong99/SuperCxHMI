// Slider.h: interface for the CSlider class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SLIDER_H__
#define _SLIDER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_SLIDERCHANGE	WM_USER + 230

class CSlider : public CWindowImpl<CSlider>    
{
	enum Slider_HitTest
	{
		Slider_Nowhere,      
		Slider_Top_Arrow,    
		Slider_Top_Rect,     
		Slider_Thumb,        
		Slider_Bottom_Rect,  
		Slider_Bottom_Arrow  
	};

	enum
	{
		ID_TIMER_FIRST = 1000,
		ID_TIMER_REPEAT = 1001
	};

public:
	CSlider();
	virtual ~CSlider();

	BEGIN_MSG_MAP(CFillStyleWnd)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDown)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
	END_MSG_MAP()
		
	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void DrawArrows(CDCHandle dc, CRect rect, BOOL bVertical,
		int nArrowSize, BOOL bTopPressed, BOOL bBottomPressed);
	void DrawInterior(CDCHandle dc, CRect rect, int nArrowSize,
		int nThumbSize, int nThumbPos, BOOL bVertical);
	void GetItemRect(CRect* pRect, int* pArrowSize, int* pThumbSize, int *pThumbPos);
	int HitTest(POINT point);
	void SetRange(int nMin, int nMax);
	void SetCur(int nCur);
	int GetCur();
	
	int m_nTracking;
	int m_bPressed;
	int m_nTrackOffset;
	
	int m_nMin;
	int m_nMax;
	int m_nCur;
};

#endif // _SLIDER_H__
