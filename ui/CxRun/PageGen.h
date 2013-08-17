#ifndef __PAGEAPP_H__
#define __PAGEAPP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageApp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageGen dialog

class CPageGen : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageGen)

// Construction
public:
	CPageGen();
	~CPageGen();

// Dialog Data
	//{{AFX_DATA(CPageGen)
	enum { IDD = IDD_PAGE_GEN };
	CString	m_strServer;
	CEdit	m_edtServer;
	CString	m_strServer2;
	CEdit	m_edtServer2;
	CString	m_strStartPicture;
	CEdit	m_edtStartPicture;
	BOOL	m_bFullScreen;
	BOOL	m_bHighlightMouseObject;
	int		m_nUseSetting;
	BOOL	m_bPromptLocalServerModified;
	BOOL	m_bUseDefaultUserAndPassword;
	CButton m_checkUseDefaultUserAndPassword;
	CString	m_strDefaultUser;
	CString	m_strDefaultPassword;
	CEdit	m_edtDefaultUser;
	CEdit	m_edtDefaultPassword;
	//}}AFX_DATA

	void UpdateEnableState(int nUseSetting);


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageGen)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageGen)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonServer();
	afx_msg void OnButtonServer2();
	afx_msg void OnButtonPicture();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnCheck4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PAGEAPP_H__)
