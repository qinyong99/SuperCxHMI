
#ifndef __SCRIPTFRAME_H__
#define __SCRIPTFRAME_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptFrame.h : header file

/////////////////////////////////////////////////////////////////////////////
// CScriptFrame frame
class CScriptFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CScriptFrame)
protected:
	CScriptFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

	virtual ~CScriptFrame();

	// Generated message map functions
	//{{AFX_MSG(CScriptFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnClose();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !__SCRIPTFRAME_H__
