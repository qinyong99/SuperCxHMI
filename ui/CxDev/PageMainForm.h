#if !defined(AFX_PAGEMAINFORM_H__860AFE2A_978E_4066_9845_7B9683D893B1__INCLUDED_)
#define AFX_PAGEMAINFORM_H__860AFE2A_978E_4066_9845_7B9683D893B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageMainForm.h : header file
//
#include "ClrButton.h"

/////////////////////////////////////////////////////////////////////////////
// CPageMainForm dialog

class CPageMainForm : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageMainForm)

// Construction
public:
	CPageMainForm();
	~CPageMainForm();

// Dialog Data
	//{{AFX_DATA(CPageMainForm)
	enum { IDD = IDD_PAGE_MAINFORM };
	CClrButton	m_btGridColor;
	CClrButton	m_btForeColor;
	CClrButton	m_btBackGround;
	BOOL	m_bGrid;
	int		m_nHeight;
	int		m_nWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageMainForm)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageMainForm)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEMAINFORM_H__860AFE2A_978E_4066_9845_7B9683D893B1__INCLUDED_)
