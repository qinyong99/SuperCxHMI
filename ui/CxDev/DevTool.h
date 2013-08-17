#ifndef __DEVTOO_H__
#define __DEVTOO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DevTool.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDevTool command target

class CDevTool : public CCmdTarget
{
// Constructors
public:
	CDevTool(DRAW_SHAPE drawShape);           // protected constructor used by dynamic creation

// Overrides
	virtual void Initialize();
	virtual void DeInitialize();
	virtual void OnDraw(CLayoutView* pView, CDC* pDC);
	virtual void OnLButtonDown(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnRButtonDown(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnChar(CLayoutView* pView, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnKeyDown(CLayoutView* pView, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnCancel();
	virtual void OnInit();

	virtual void OnDragScroll(CLayoutView* pView, CPoint point);

	UINT m_nTimerID;        // != MAKEWORD(-1, -1) when in scroll area
	DWORD m_dwLastTick;     // only valid when m_nTimerID valid
	UINT m_nScrollDelay;    // time to next scroll

	// metrics for drag-scrolling
	static int c_nScrollInset;
	static UINT c_nScrollDelay;
	static UINT c_nScrollInterval;

// Attributes
	DRAW_SHAPE m_drawShape;
	static CDevObj::TrackerState c_enumTrackerState;

	CDC* m_pDC;

	void DrawRubberBand(CLayoutView* pView, CRect rect);
	BOOL TrackRubberBand(CLayoutView* pView, const CPoint& point, CRect& rect);
		
	static void InitTools();
	static void DeInitTools();
	static CDevTool* FindTool(DRAW_SHAPE drawShape);
	static void SetTrackerState(CDevObj::TrackerState state, BOOL bUpdate = TRUE);
	static CPtrList c_tools;
	static DRAW_SHAPE c_drawShape;
	static CPoint c_down;
	static CLayoutView* c_pActiveView;
};

/////////////////////////////////////////////////////////////////////////////


class CDrawTool : public CDevTool  
{
public:
	CDrawTool(DRAW_SHAPE nDRAW_SHAPE, CLSID clsid);
	virtual ~CDrawTool();

// Operations
public:
	CLSID m_clsid;
	ICxDrawTool* m_pDrawTool;
	IConnectionPoint* m_pIConnectionPoint;
	static CDrawObj* c_pDrawingObject;
		
	HRESULT DrawObjCreate();
	HRESULT DrawObjCancel();
	HRESULT DrawAspectChange(RECT* pInvalidRect);

	// Overridables
	virtual void Initialize();
	virtual void DeInitialize();
	virtual void OnDraw(CLayoutView* pView, CDC* pDC);
	virtual void OnLButtonDown(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnRButtonDown(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnChar(CLayoutView* pView, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnKeyDown(CLayoutView* pView, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnCancel();
	virtual void OnInit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawTool)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCxDrawTool)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

		// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(COlePropPage)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	
	DECLARE_OLECREATE(CDrawTool)

    BEGIN_INTERFACE_PART(CxDrawToolSite, ICxDrawToolSite)
		STDMETHOD(CaptureMouse)();
		STDMETHOD(ReleaseMouse)();
		STDMETHOD(GetDrawObject)(REFCLSID clsid, IUnknown **ppUnk);
		STDMETHOD(GetDrawShape)(DRAW_SHAPE* pDrawShape);
		STDMETHOD(GetWindow)(HWND* phWindow);
		STDMETHOD(GetDC)(HDC* phDC);
		STDMETHOD(ReleaseDC)(HDC hDC);
		STDMETHOD(DocToClient)(POINT* pPoint);
		STDMETHOD(ClientToDoc)(POINT* pPoint);
		STDMETHOD(GetExpressionCreatorProc)(long lProc);
		STDMETHOD(GetGridSpace)(SIZE* pSize);
	END_INTERFACE_PART(CxDrawToolSite)

	BEGIN_INTERFACE_PART(CxDrawToolEvents, ICxDrawToolEvents)
		STDMETHOD(OnViewChange)(RECT* pInvalidRect);	
		STDMETHOD(OnCreate)();
		STDMETHOD(OnCancel)();
    END_INTERFACE_PART(CxDrawToolEvents)
 
	DECLARE_INTERFACE_MAP()

};


class CRectItemTool : public CDevTool  
{
public:
	CRectItemTool();
	virtual ~CRectItemTool();

// Implementation
	virtual void OnLButtonDown(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnKeyDown(CLayoutView* pView, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnCancel();
	
	static CLSID c_clsid;

	CRect m_rect;
	BOOL m_bMouseDown;
};

class CSelectTool : public CDevTool  
{
public:
	void SetupSelectionTracker(CLayoutView* pView, CRectTracker* pTracker);
	CSelectTool();
	virtual ~CSelectTool();

// Implementation
	virtual void OnLButtonDown(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CLayoutView* pView, UINT nFlags, const CPoint& point);
	virtual void OnKeyDown(CLayoutView* pView, UINT nChar, UINT nRepCnt, UINT nFlags);

	BOOL TrackHandle(CLayoutView* pView, CDevObj* pObj, int nHandle, CPoint point);

	static BOOL c_bTrackingHandle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __DEVTOO_H__
