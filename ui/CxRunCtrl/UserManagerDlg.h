#if !defined(AFX_USERMANAGERDLG_H__684F8F27_60EA_41F4_8B2C_A70A645F0F83__INCLUDED_)
#define AFX_USERMANAGERDLG_H__684F8F27_60EA_41F4_8B2C_A70A645F0F83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserManagerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserManagerDlg dialog

_COM_SMARTPTR_TYPEDEF(ICxUserManager, __uuidof(ICxUserManager));

class CUserManagerDlg : public CDialog
{
// Construction
public:
	CUserManagerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserManagerDlg)
	enum { IDD = IDD_USER_MANAGER };
	CListCtrl	m_list;
	//}}AFX_DATA

	CImageList m_SmallImageList;

	CArray<CString, CString> m_arrSecurityAreas;
	ICxUserManagerPtr m_pUserManager;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserManagerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL IsUserUniqueName(int nItem, LPCTSTR lpszName);
	void CreateUniqueName(LPCTSTR lpszBaseName, CString& strName);

	// Generated message map functions
	//{{AFX_MSG(CUserManagerDlg)
	afx_msg void OnAddUser();
	afx_msg void OnDeleteUser();
	afx_msg void OnModifyUser();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeUsers();
	afx_msg void OnItemchangedUsers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkUsers(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERMANAGERDLG_H__684F8F27_60EA_41F4_8B2C_A70A645F0F83__INCLUDED_)
