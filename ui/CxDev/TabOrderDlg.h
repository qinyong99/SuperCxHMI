#if !defined(AFX_TABORDERDLG_H__CF917B4B_723D_4F2B_9641_7B70C9923AF3__INCLUDED_)
#define AFX_TABORDERDLG_H__CF917B4B_723D_4F2B_9641_7B70C9923AF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabOrderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabOrderDlg dialog

class CTabOrderDlg : public CDialog
{
// Construction
public:
	CTabOrderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabOrderDlg)
	enum { IDD = IDD_TAB_ORDER };
	CListBox	m_listOrder;
	//}}AFX_DATA

	CDevObjList* m_pObjList;

	void UpdateButtonState();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabOrderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabOrderDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnSelchangeList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABORDERDLG_H__CF917B4B_723D_4F2B_9641_7B70C9923AF3__INCLUDED_)
