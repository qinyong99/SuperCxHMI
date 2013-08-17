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

	int m_nUseSetting;

// Dialog Data
	//{{AFX_DATA(CPageGen)
	enum { IDD = IDD_PAGE_GEN };
	CString	m_strPictureFolder;
	CEdit	m_edtPictureFolder;
	CString	m_strStartPicture;
	CEdit	m_edtStartPicture;
	BOOL	m_bFullScreen;
	BOOL	m_bHighlightMouseObject;
	BOOL	m_bUseSetting;
	CButton	m_btnUseSetting;
	//}}AFX_DATA


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
	afx_msg void OnButtonFolder();
	afx_msg void OnButtonPicture();
	afx_msg void OnUseSetting();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PAGEAPP_H__)
