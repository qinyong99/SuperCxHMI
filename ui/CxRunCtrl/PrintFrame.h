#if !defined(AFX_PRINTFRAME_H__0768EE8B_2218_4CE7_8A13_C583965FF024__INCLUDED_)
#define AFX_PRINTFRAME_H__0768EE8B_2218_4CE7_8A13_C583965FF024__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintFrame frame

class CPrintFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CPrintFrame)
public:
	CPrintFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	virtual BOOL ContinueModal();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintFrame)
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPrintFrame();

protected:

	// Generated message map functions
	//{{AFX_MSG(CPrintFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTFRAME_H__0768EE8B_2218_4CE7_8A13_C583965FF024__INCLUDED_)
