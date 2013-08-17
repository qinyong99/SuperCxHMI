#if !defined(AFX_CHANGEPASSWORDDLG_H__4BEADF4B_DBE9_4A32_97F0_D001F9083F64__INCLUDED_)
#define AFX_CHANGEPASSWORDDLG_H__4BEADF4B_DBE9_4A32_97F0_D001F9083F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangePasswordDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChangePasswordDlg dialog

_COM_SMARTPTR_TYPEDEF(ICxUserManager, __uuidof(ICxUserManager));

class CChangePasswordDlg : public CDialog
{
// Construction
public:
	CChangePasswordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChangePasswordDlg)
	enum { IDD = IDD_CHANGE_PASSWORD };
	CString	m_strUserName;
	CString	m_strOldPassword;
	CString	m_strNewPassword;
	CString	m_strVerifyPassword;
	//}}AFX_DATA

	ICxUserManagerPtr m_pUserManager;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangePasswordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangePasswordDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGEPASSWORDDLG_H__4BEADF4B_DBE9_4A32_97F0_D001F9083F64__INCLUDED_)
