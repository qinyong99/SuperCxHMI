#if !defined(AFX_REPORTFIELDDLG_H__FF6EDF0F_8CC4_4AF1_99DF_76BAD4408564__INCLUDED_)
#define AFX_REPORTFIELDDLG_H__FF6EDF0F_8CC4_4AF1_99DF_76BAD4408564__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportFieldDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportFieldDlg dialog

class CReportWizardDlg;

class CReportField
{
public:
	CReportField()
	{
	}
	// CSimpleArray 会使用下面的构造函数
	CReportField(const CReportField& field)
	{
		InternalCopy(field);
	}
	void InternalCopy(const CReportField& field)
	{
		m_strName = field.m_strName;
		m_nAlign = field.m_nAlign;
		m_nSize = field.m_nSize;
	}
	
	CString m_strName;
	enumTextAlign m_nAlign;
	int m_nSize;	
};
 
class CReportFieldDlg : public CDialog
{
// Construction
public:
	DECLARE_DYNCREATE(CReportFieldDlg)

	CReportFieldDlg(CWnd* pParent = NULL);   // standard constructor

// Attributes
public:
	CReportWizardDlg* m_pReportWizardDlg;
	CArray<CReportField, CReportField> m_arrFields;

	void LoadList();

// Dialog Data
	//{{AFX_DATA(CReportFieldDlg)
	enum { IDD = IDD_REPORT_FIELD };
	CListBox	m_listRight;
	CListBox	m_listLeft;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportFieldDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReportFieldDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRightOne();
	afx_msg void OnRightAll();
	afx_msg void OnLeftOne();
	afx_msg void OnLeftAll();
	//}}AFX_MSG
	afx_msg void OnApply();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTFIELDDLG_H__FF6EDF0F_8CC4_4AF1_99DF_76BAD4408564__INCLUDED_)
