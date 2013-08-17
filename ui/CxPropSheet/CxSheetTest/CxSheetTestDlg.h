// CxSheetTestDlg.h : header file
// 所属项目: AutoCx的CxSheetTest工程
// 创建日期: 2001.10.8
// 创 建 人：孙越洋 
// 功能描述：测试工程主对话框窗口类。 
// 修改记录：
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CXSHEETTESTDLG_H__2A2962CC_C24B_4114_8576_D6B7989561E1__INCLUDED_)
#define AFX_CXSHEETTESTDLG_H__2A2962CC_C24B_4114_8576_D6B7989561E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCxSheetTestDlg dialog

class CCxSheetTestDlg : public CDialog
{
// Construction
public:
	CCxSheetTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCxSheetTestDlg)
	enum { IDD = IDD_CXSHEETTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCxSheetTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCxSheetTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CXSHEETTESTDLG_H__2A2962CC_C24B_4114_8576_D6B7989561E1__INCLUDED_)
