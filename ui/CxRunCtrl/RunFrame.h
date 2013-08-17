#if !defined(AFX_RUNFRAME_H__6C144426_D0B8_484B_9E02_98A3B6C7AA2D__INCLUDED_)
#define AFX_RUNFRAME_H__6C144426_D0B8_484B_9E02_98A3B6C7AA2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RunFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRunFrame frame

class CRunCtrl;

class CRunFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CRunFrame)
protected:
	CRunFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRunFrame();

	// Generated message map functions
	//{{AFX_MSG(CRunFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNFRAME_H__6C144426_D0B8_484B_9E02_98A3B6C7AA2D__INCLUDED_)
