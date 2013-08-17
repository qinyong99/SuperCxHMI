// SymbolPreviewWnd.h: header file
//
//////////////////////////////////////////////////////////////////////

#ifndef _SYMBOLPREVIEWWND_H__
#define _SYMBOLPREVIEWWND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CSymbolPreviewWnd window

#define WM_OBJECT_SELECTED  WM_USER + 202

class CDevObj;

class CSymbolPreviewWnd : public CWnd
{
// Construction
public:
	CSymbolPreviewWnd(CDevObj* pObj = NULL);

// Attributes
public:
	CDevObj* m_pObject;
	CDevObj* m_pSelectObject;

	CRect m_rcDraw;
	CRect m_rcObject;

// Operations
public:
	void ReCalculatePosition();
	void OnPrepareDC(CDC* pDC);

	void SetSelectObject(CDevObj* pObj);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolPreviewWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSymbolPreviewWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSymbolPreviewWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _SYMBOLPREVIEWWND_H__
