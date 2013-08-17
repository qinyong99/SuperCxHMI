#if !defined(AFX_PAGESUBFORM_H__01FE2834_3DA3_4913_B9DF_BD45F9CC06E4__INCLUDED_)
#define AFX_PAGESUBFORM_H__01FE2834_3DA3_4913_B9DF_BD45F9CC06E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSubForm.h : header file
//
#include "ClrButton.h"

/////////////////////////////////////////////////////////////////////////////
// CPageSubForm dialog

class CPageSubForm : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageSubForm)

// Construction
public:
	CPageSubForm();
	~CPageSubForm();

// Dialog Data
	//{{AFX_DATA(CPageSubForm)
	enum { IDD = IDD_PAGE_SUBFORM };
	CClrButton	m_btGridColor;
	CClrButton	m_btForeColor;
	CClrButton	m_btBackColor;
	BOOL	m_bGrid;
	int		m_nHeight;
	int		m_nWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageSubForm)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageSubForm)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESUBFORM_H__01FE2834_3DA3_4913_B9DF_BD45F9CC06E4__INCLUDED_)
