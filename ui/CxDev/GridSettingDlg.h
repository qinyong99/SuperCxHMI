#if !defined(AFX_GRIDSETTINGDLG_H__DA592E6D_22EC_4D5F_99AA_9D9806EC18EE__INCLUDED_)
#define AFX_GRIDSETTINGDLG_H__DA592E6D_22EC_4D5F_99AA_9D9806EC18EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridSetting.h : header file
//

#include "ClrButton.h"

/////////////////////////////////////////////////////////////////////////////
// CGridSettingDlg dialog

class CGridSettingDlg : public CDialog
{
// Construction
public:
	CGridSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGridSettingDlg)
	enum { IDD = IDD_GRID_SETTING };
	CEdit	m_editGridSpaceHeight;
	CEdit	m_editGridSpaceWidth;
	CClrButton	m_clrGridColor;
	BOOL	m_bGridEnable;
	UINT	m_nGridSpaceWidth;
	UINT	m_nGridSpaceHeight;
	BOOL	m_bAlignToGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGridSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDSETTINGDLG_H__DA592E6D_22EC_4D5F_99AA_9D9806EC18EE__INCLUDED_)
