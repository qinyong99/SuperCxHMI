////////////////////////////////////////////////////////////////
// ExpropItemDlg.h : header file

#if !defined(AFX_EXPROPITEMDLG_H__37A0D047_C67E_4279_B28B_BE6EF6FF8E33__INCLUDED_)
#define AFX_EXPROPITEMDLG_H__37A0D047_C67E_4279_B28B_BE6EF6FF8E33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSymbolPropDlg dialog

class CSymbolPropList;

class CSymbolPropDlg : public CDialog
{
// Construction
public:
	CSymbolPropDlg(CSymbolProp* pProp, CWnd* pParent = NULL);   // standard constructor

public:
// Dialog Data
	//{{AFX_DATA(CSymbolPropDlg)
	enum { IDD = IDD_SYMBOL_PROP };
	CEdit	m_editValid;
	CButton	m_btnEnum;
	CComboBox	m_listType;
	CString	m_strDes;
	CString	m_strName;
	CString m_strValidScript;
	//}}AFX_DATA
	enumPropType m_type;

	_variant_t m_value;
	
	CSymbolObj* m_pObject;
	CSymbolPropList* m_pPropList;
	CSymbolProp* m_pProp;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CSymbolPropDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboType();
	afx_msg void OnButtonEnum();
	afx_msg void OnButtonValexp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPROPITEMDLG_H__37A0D047_C67E_4279_B28B_BE6EF6FF8E33__INCLUDED_)
