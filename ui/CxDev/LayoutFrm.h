// ChildFrm.h : interface of the CLayoutFrame class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __CHILDFRM_H__
#define __CHILDFRM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLayoutView;
#include "MiddleWnd.h"

class CLayoutFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CLayoutFrame)
public:
	CLayoutFrame();

// Attributes
public:
	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayoutFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLayoutFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

	//{{AFX_MSG(CLayoutFrame)
	afx_msg void OnClose();
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CMiddleWnd* m_pMiddleWnd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //__CHILDFRM_H__
