
#ifndef __INSERTCONTROLDLG_H__
#define __INSERTCONTROLDLG_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// InsertControlDlg.H : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInsertControlDlg dialog

class CInsertControlDlg : public CDialog
{
// Construction
public:
	CInsertControlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInsertControlDlg)
	enum { IDD = IDD_INSERTCONTROL };
	CStatic	m_staticServerPath;
   CCheckListBox m_lbControls;
	//}}AFX_DATA

   CList< CLSID, CLSID& > m_aryClsid;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsertControlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL IsCLSIDInUse(CLSID& clsid);
	void GetSelectCLSIDs();
   void RefreshControlList();
   ICatInformationPtr m_pCatInfo;
   CArray< CATID, CATID& > m_aImplementedCategories;
   CArray< CATID, CATID& > m_aRequiredCategories;
   CList< CLSID, CLSID& > m_lControls;

	// Generated message map functions
	//{{AFX_MSG(CInsertControlDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSelchangeControls();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__INSERTCONTROLDLG_H__)
