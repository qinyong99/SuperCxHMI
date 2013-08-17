#if !defined(AFX_USERITEMDLG_H__B544435D_66A3_46FC_8C5B_354E4092319D__INCLUDED_)
#define AFX_USERITEMDLG_H__B544435D_66A3_46FC_8C5B_354E4092319D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserItemDlg dialog

#include "CheckListCtrl.h"

class CUserItemDlg : public CDialog
{
// Construction
public:
	CUserItemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserItemDlg)
	enum { IDD = IDD_USER_ITEM };
	CCheckListCtrl m_listArea;
	CComboBox	m_cmbGroup;
	CString	m_strName;
	CString	m_strDesc;
	CString	m_strPassword;
	CString		m_strGroup;
	UINT	m_nPrivilege;
	//}}AFX_DATA

	CString m_strAccessSecrityAreas;
	CArray<CString, CString>* m_parrSecurityAreas;

	void LoadSecurityAreas();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserItemDlg)
	afx_msg void OnSelectAll();
	afx_msg void OnSelectNone();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERITEMDLG_H__B544435D_66A3_46FC_8C5B_354E4092319D__INCLUDED_)
