#if !defined(AFX_PAGEOBJECT_H__5F018E70_D1CB_4C1E_8C28_2F1C0D861643__INCLUDED_)
#define AFX_PAGEOBJECT_H__5F018E70_D1CB_4C1E_8C28_2F1C0D861643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageObject.h : header file
//
#include "ClrButton.h"

/////////////////////////////////////////////////////////////////////////////
// CPageObject dialog

class CPageObject : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageObject)

// Construction
public:
	CPageObject();
	~CPageObject();

// Dialog Data
	//{{AFX_DATA(CPageObject)
	enum { IDD = IDD_PAGE_OBJECT };
	CClrButton	m_btForeColor;
	CClrButton	m_btBackColor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageObject)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageObject)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEOBJECT_H__5F018E70_D1CB_4C1E_8C28_2F1C0D861643__INCLUDED_)
