// RunView.h : interface of the CRunView class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __RUNVIEW_H__
#define __RUNVIEW_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCtrlItem;

#define ID_SCANTIMER 5
#define ID_STANDTIMER 6
#define STANDTIMER_INTERVAL 100
#define WM_PAINTEX WM_USER + 100

#include "PPToolTip.h"

class CRunView : public CScrollView
{
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected: // create from serialization only
	CRunView();
	DECLARE_DYNCREATE(CRunView)

	BOOL m_bDestory;
	BOOL m_bNoClosePicture;

// Attributes
public:
	CFormDoc* GetDocument() const;
	// m_pSelection holds the selection to the current CRunItem.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not CRunItem objects.  This selection
	//  mechanism is provided just to help you get started.

	CPPToolTip m_tooltip;

// Operations
public:
	void InvalidObject(CDevObj *pObj);
	void InvalidateRectEx(LPCRECT lpRect);
	
	void UpdateObjAreaMask(CDevObj *pObj, BOOL bOr = FALSE);

	void Refresh(BOOL bAll = FALSE);
	void OnDrawEx(CDC* pDC);
	void OnDrawDirect(CDC* pDC);

	void DrawHighlightObject(CDC* pDC);

	void Reset(); 

	void TrackObjectMouseLeave(CDevObj* pObj);

	int m_nAreaWidth;
	int m_nAreaHeight;
	WORD m_wInvalidAreaMask[8];
	CRect m_arrAreaRects[8][16];
	CRgn m_arrAreaRgns[8][16];

	CDevObj* m_pObjectHighlight;
	CDevObj* m_pObjectTrackMouseLeave;
	BOOL m_bMouseTracking;

	CDevObj* m_pObjectWithCapture;
	CDevObj* m_pObjectWithFocus;
	
	CPoint m_ptLastMousePosition;
	CCtrlItem* m_pItemToDeactivateOnLeave;

	unsigned m_iButtonState : 3;        // Which buttons are down?
	unsigned m_iDblClkState : 3;        // Which buttons involved in dbl click?
	
	unsigned m_iButton : 3;
	UINT m_iKeyCode;

	BOOL m_bLockDraw;

	static HCURSOR m_hHandCursors[3];
	static void SetStandardCursor(enumMousePointer enumMousePointer);
	static HCURSOR GetHandCursor(int nIndex = 0);
		
	CDevObj* HitTestObjects(CPoint point) const;

	BOOL HandlePointerInactiveMove( CPoint point, DWORD dwKeyState );
	BOOL HandlePointerInactiveSetCursor( DWORD dwMouseMessage );
	BOOL HandleWindowlessMouseMessage(CPoint point);
	BOOL HandleWindowlessKeyboardMessage();
	BOOL HandleWindowlessFocusMessage(BOOL bFoucs);

	void OnFocus(CDevObj* pObj, BOOL bGotFocus);
	BOOL TranslateAccelerator(CCtrlItem* pItem, LPMSG pMsg, DWORD dwModifiers);

	HRESULT OnWindowlessDefWindowMessage(UINT nMessage, WPARAM wParam,
		LPARAM lParam, LRESULT* plResult);
	HRESULT OnWindowlessReleaseCapture(CDevObj* pObj);
	HRESULT OnWindowlessReleaseFocus();
	HRESULT OnWindowlessSetCapture(CDevObj* pObj);
	HRESULT OnWindowlessSetFocus(CDevObj* pObj);
	CDevObj* GetObjectWithCapture() const;
	CDevObj* GetObjectWithFocus() const;
	
	// Firing functions for stock events
	void FireEvent(DISPID);
	void FireKeyDown(USHORT* pnChar, short nShiftState);
	void FireKeyUp(USHORT* pnChar, short nShiftState);
	void FireKeyPress(USHORT* pnChar);
	void FireMouseDown(short nButton, short nShiftState,
		OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y);
	void FireMouseUp(short nButton, short nShiftState,
		OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y);
	void FireMouseMove(short nButton, short nShiftState,
		OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y);
	void FireClick();
	void FireDblClick();
	void FireInitialize();
	void FireActivate();
	void FireDeactivate();

	// events
	void KeyDown(USHORT* pnChar);
	void KeyUp(USHORT* pnChar);
	void ButtonDown(USHORT iButton, UINT nFlags, CPoint point);
	void ButtonUp(USHORT iButton, UINT nFlags, CPoint point);
	void ButtonDblClk(USHORT iButton, UINT nFlags, CPoint point);

	BOOL IsCancelBubble();
	void InitEventObject();
	ICxEventObj* m_pEventObject;

	void UpdateOptimizeDrawPara();

	// zoom factor
	CSize m_zoomNum;
	CSize m_zoomDenom;

	UINT m_nScanTimer;
	UINT m_nStandTimer;
	void SetTimerEnabled(BOOL bEnabled) { m_bTimerEnabled = bEnabled; }

	void ReSetTimer(BOOL bScan, BOOL bStand, BOOL bSet);

	CRunCtrl* m_pRunCtrl;; //用来显示的框架

	void SetZoomFitWindow(BOOL bFitWindow, float fZoomRatio, float fZoomCenterX, float fZoomCenterY);
	void Zoom();
	void SetAllowMouseZoom(BOOL bAllow);
	void SetAllowScrollBars(BOOL bAllow);
	void SetDragMode(BOOL b);

	void Drag();
	
	void SetScrollInfo();
	BOOL SetZoomFactor(CSize zoomNum, CSize zoomDenom);
	void DocToClient(CRect& rect);
	void DocToClient(CPoint& point);
	void DocToClient(CSize& size);
	void ClientToDoc(CPoint& point);
	void ClientToDoc(CRect& rect);
	void ClientToDoc(CSize& size);

	void SetScrollSizes2(int nMapMode, SIZE sizeTotal,
				const SIZE& sizePage = sizeDefault,
				const SIZE& sizeLine = sizeDefault);

	BOOL m_bEnabled;
	  
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRunView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bTimerEnabled;
	BOOL m_bZoomFitWindow;
	float m_fZoomRatio;
	float m_fZoomX;
	float m_fZoomY;
	BOOL m_bAllowMouseZoom;
	BOOL m_bAllowScrollBars;
	BOOL m_bDragMode;

	CMapPtrToPtr m_mapDirectDraw;
	CRect m_rcDirectDraw;
		
// Generated message map functions
protected:
	//{{AFX_MSG(CRunView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCancelMode();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnNcPaint();
	//}}AFX_MSG
	afx_msg void OnPaintEx();
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
//	afx_msg BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void NotifyDisplayTooltip(NMHDR * pNMHDR, LRESULT * result);
	DECLARE_MESSAGE_MAP()

	// button handler helpers
	void OnButtonUp(USHORT nButton, UINT nFlags, CPoint point);
	void OnButtonDown(USHORT nButton, UINT nFlags, CPoint point);
	void OnButtonDblClk(USHORT nButton, UINT nFlags, CPoint point);

//	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	friend class CDevObj;
	friend class CPrintView;
};

#ifndef _DEBUG  // debug version in RunView.cpp
inline CFormDoc* CRunView::GetDocument() const
   { return (CFormDoc*)m_pDocument; }
#endif


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNVIEW_H__EB197727_4C10_11D5_A46A_B043DCC54377__INCLUDED_)
