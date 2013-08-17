#if !defined(AFX_RUNCTRLPPG_H__3674A260_31A8_4F99_83D0_B053EE05F2E1__INCLUDED_)
#define AFX_RUNCTRLPPG_H__3674A260_31A8_4F99_83D0_B053EE05F2E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// RunCtrlPpg.h : Declaration of the CRunCtrlPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CRunCtrlPropPage : See RunCtrlPpg.cpp.cpp for implementation.

class CRunCtrlPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CRunCtrlPropPage)
	DECLARE_OLECREATE_EX(CRunCtrlPropPage)

// Constructor
public:
	CRunCtrlPropPage();

// Dialog Data
	//{{AFX_DATA(CRunCtrlPropPage)
	enum { IDD = IDD_PROPPAGE_RUNCTRL };
	int m_nBorderStyle;
	BOOL	m_bRefreshEnabled;
	BOOL	m_bZoomFitWindow;
	CString	m_strPicture;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CRunCtrlPropPage)
	afx_msg void OnButtonPicture();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNCTRLPPG_H__3674A260_31A8_4F99_83D0_B053EE05F2E1__INCLUDED)
