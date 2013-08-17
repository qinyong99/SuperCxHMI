#if !defined(AFX_DCOMERRORDLG_H__DBE4CD57_85B5_4695_B64B_29483E18CB91__INCLUDED_)
#define AFX_DCOMERRORDLG_H__DBE4CD57_85B5_4695_B64B_29483E18CB91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DcomErrorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDcomErrorDlg dialog

class CDcomErrorDlg : public CDialog
{
// Construction
public:
	CDcomErrorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDcomErrorDlg)
	enum { IDD = IDD_DCOM_ERROR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDcomErrorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDcomErrorDlg)
	afx_msg void OnRetry();
	afx_msg void OnIgnore();
	afx_msg void OnAbort();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCOMERRORDLG_H__DBE4CD57_85B5_4695_B64B_29483E18CB91__INCLUDED_)
