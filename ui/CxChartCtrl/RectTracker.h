// RectTracker1.h: interface for the CRectTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECTTRACKER1_H__BB36F1D5_029D_4005_BAA8_D05C8FD7D3AC__INCLUDED_)
#define AFX_RECTTRACKER1_H__BB36F1D5_029D_4005_BAA8_D05C8FD7D3AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRectTracker
{
public:
	// Constructors
	CRectTracker();
	CRectTracker(LPCRECT lpSrcRect, UINT nStyle);
	
	// Style Flags
	enum StyleFlags
	{
		solidLine = 1, dottedLine = 2, hatchedBorder = 4,
			resizeInside = 8, resizeOutside = 16, hatchInside = 32,
	};
	
	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,
			hitTopLeft = 0, hitTopRight = 1, hitBottomRight = 2, hitBottomLeft = 3,
			hitTop = 4, hitRight = 5, hitBottom = 6, hitLeft = 7, hitMiddle = 8
	};
	
	// Attributes
	UINT m_nStyle;      // current state
	CRect m_rect;       // current position (always in pixels)
	CSize m_sizeMin;    // minimum X and Y size during track operation
	int m_nHandleSize;  // size of resize handles (default from WIN.INI)
	
	// Operations
	void Draw(HDC hDC) const;
	void GetTrueRect(LPRECT lpTrueRect) const;
	BOOL SetCursor(HWND hWnd, UINT nHitTest) const;
	BOOL Track(HWND hWnd, CPoint point, BOOL bAllowInvert = FALSE,
		HWND hWndClipTo = NULL);
	BOOL TrackRubberBand(HWND hWnd, CPoint point, BOOL bAllowInvert = TRUE);
	int HitTest(CPoint point) const;
	int NormalizeHit(int nHandle) const;
	
	// Overridables
	virtual void DrawTrackerRect(LPCRECT lpRect, HWND hWndClipTo,
		HDC hDC, HWND hWnd);
	virtual void AdjustRect(int nHandle, LPRECT lpRect);
	virtual void OnChangedRect(const CRect& rectOld);
	virtual UINT GetHandleMask() const;
	
	// Implementation
public:
	virtual ~CRectTracker();
	
protected:
	BOOL m_bAllowInvert;    // flag passed to Track or TrackRubberBand
	CRect m_rectLast;
	CSize m_sizeLast;
	BOOL m_bErase;          // TRUE if DrawTrackerRect is called for erasing
	BOOL m_bFinalErase;     // TRUE if DragTrackerRect called for final erase
	
	// implementation helpers
	int HitTestHandles(CPoint point) const;
	void GetHandleRect(int nHandle, CRect* pHandleRect) const;
	void GetModifyPointers(int nHandle, int**ppx, int**ppy, int* px, int*py);
	virtual int GetHandleSize(LPCRECT lpRect = NULL) const;
	BOOL TrackHandle(int nHandle, HWND hWnd, CPoint point, HWND hWndClipTo);
	void Construct();
};

#endif // !defined(AFX_RECTTRACKER1_H__BB36F1D5_029D_4005_BAA8_D05C8FD7D3AC__INCLUDED_)

