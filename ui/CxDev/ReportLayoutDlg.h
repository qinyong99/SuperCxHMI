#if !defined(AFX_REPORTLAYOUTDLG_H__9F9105C0_8DC0_44C6_97E8_12F52739440A__INCLUDED_)
#define AFX_REPORTLAYOUTDLG_H__9F9105C0_8DC0_44C6_97E8_12F52739440A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportLayoutDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportLayoutDlg dialog

class CReportWizardDlg;

class CReportLayoutDlg : public CDialog
{
// Construction
public:
	DECLARE_DYNCREATE(CReportLayoutDlg)

	CReportLayoutDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReportLayoutDlg)
	enum { IDD = IDD_REPORT_LAYOUT };
	CStatic	m_statOrientation;
	CStatic	m_statLayout;
	int		m_nLayout;
	int		m_nOrientation;
	//}}AFX_DATA

	CBitmap m_bitmapColumnar;
	CBitmap m_bitmapTabular;
	CBitmap m_bitmapJustified;

	CReportWizardDlg* m_pReportWizardDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportLayoutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReportLayoutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnColumnar();
	afx_msg void OnTabular();
	afx_msg void OnJustified();
	afx_msg void OnPortrait();
	afx_msg void OnLandscape();
	afx_msg void OnOrientation();
	//}}AFX_MSG
	afx_msg void OnApply();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTLAYOUTDLG_H__9F9105C0_8DC0_44C6_97E8_12F52739440A__INCLUDED_)
