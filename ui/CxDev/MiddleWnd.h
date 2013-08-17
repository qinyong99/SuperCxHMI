#if !defined(AFX_MIDDLEWND_H__5DD83CD0_459E_4C22_A400_67B8748EB214__INCLUDED_)
#define AFX_MIDDLEWND_H__5DD83CD0_459E_4C22_A400_67B8748EB214__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MiddleWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMiddleWnd view

#define WM_SELECT_CHANGED  WM_USER + 102 

class CMiddleWnd : public CScrollView
{
public:
	CMiddleWnd();           // protected constructor used by dynamic creation
	virtual ~CMiddleWnd();
	Create(CWnd* pParent);

	class CTracker : public CRectTracker
	{
		friend class CMiddleWnd;
	};

protected:
	DECLARE_DYNCREATE(CMiddleWnd)

// Attributes
public:
	CLayoutView* m_pView;
	BOOL m_bSelectView;
	CTracker m_rectTracker;

	BOOL m_bShowRuler;

// Operations
public:
	void SetFormSize(int nWidth, int nHeight);

	void DrawTickMarks(CDC* pDC, CRect rect, BOOL bDiv, BOOL bHorizontal);
	void DrawFace(CDC* pDC, CRect rect, BOOL bDiv, BOOL bHorizontal);
	void DrawNumbers(CDC* pDC, CRect rect, int nInc, int nTPU, BOOL bHorizontal);
	void DrawDiv(CDC* pDC, CRect rect, int nInc, int nLargeDiv, int nLength, BOOL bHorizontal);
	void DrawRuler(CDC* pDC);
	void DrawAllCaps(CDC* pDC, CRect rect);
	void DrawGap(CDC* pDC, CRect rect, int nBar);

	void LogicToRuler(CPoint& point);
	void RulerToLogic(CPoint& point);
	void LogicToRuler(CRect& rect);
	void RulerToLogic(CRect& rect);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiddleWnd)
	protected:
	virtual void OnInitialUpdate();     // first time after construct
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	virtual void OnDraw(CDC* pDC);

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMiddleWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg LRESULT OnSelectChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDDLEWND_H__5DD83CD0_459E_4C22_A400_67B8748EB214__INCLUDED_)
