#if !defined(AFX_OPCSERVERDLG_H__5085A392_ECE7_4345_AC11_1D3CC818BBAB__INCLUDED_)
#define AFX_OPCSERVERDLG_H__5085A392_ECE7_4345_AC11_1D3CC818BBAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OPCServerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COPCServerDlg dialog

class COPCDataSource;

class COPCServerDlg : public CDialog
{
// Construction
public:
	COPCServerDlg(BOOL bAdd = TRUE, CWnd* pParent = NULL);   // standard constructor

	int FindOpcClsid(CLSID clsid);
	BOOL InitOPCServers();
	BOOL InitOPCServers(CATID cid);

// Dialog Data
	//{{AFX_DATA(COPCServerDlg)
	enum { IDD = IDD_OPC_SERVER };
	CComboBox	m_cbOPCServer;
	CEdit	m_editComputer;
	CString	m_strName;
	CEdit	m_editName;
	CString	m_strOPCServer;
	CString	m_strComputer;
	//}}AFX_DATA

	BOOL m_bAdd;

	COPCDataSource* m_pOPCDataSource;
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COPCServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COPCServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPCSERVERDLG_H__5085A392_ECE7_4345_AC11_1D3CC818BBAB__INCLUDED_)
