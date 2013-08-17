#if !defined(AFX_INPUTPASSWORDDLG_H__564E18EA_4BE0_4470_8DF3_B5AD30E8AE03__INCLUDED_)
#define AFX_INPUTPASSWORDDLG_H__564E18EA_4BE0_4470_8DF3_B5AD30E8AE03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputPasswordDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputPasswordDlg dialog

class CInputPasswordDlg : public CDialog
{
// Construction
public:
	CInputPasswordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputPasswordDlg)
	enum { IDD = IDD_INPUT_PASSWORD };
	CString	m_strPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputPasswordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputPasswordDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTPASSWORDDLG_H__564E18EA_4BE0_4470_8DF3_B5AD30E8AE03__INCLUDED_)
