#if !defined(AFX_SCRIPTERRORDLG_H__A503545F_043A_45FB_A3E3_06A364B2A566__INCLUDED_)
#define AFX_SCRIPTERRORDLG_H__A503545F_043A_45FB_A3E3_06A364B2A566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptErrorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScriptErrorDlg dialog

class CScriptErrorDlg : public CDialog
{
// Construction
public:
	CScriptErrorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScriptErrorDlg)
	enum { IDD = IDD_SCRIPT_ERROR };
	CString	m_strInfoText;
	//}}AFX_DATA

	void AddErrorInfoMessage(CString strInfo);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptErrorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<CString, CString> m_arrInfoText;

	// Generated message map functions
	//{{AFX_MSG(CScriptErrorDlg)
	afx_msg void OnIgnore();
	afx_msg void OnIgnoreAll();
	afx_msg void OnBreak();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTERRORDLG_H__A503545F_043A_45FB_A3E3_06A364B2A566__INCLUDED_)
