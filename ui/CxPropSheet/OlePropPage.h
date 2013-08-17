#if !defined(AFX_OLEPROPPAGE_H__469F345B_1A8B_11D3_B009_00105AA22F40__INCLUDED_)
#define AFX_OLEPROPPAGE_H__469F345B_1A8B_11D3_B009_00105AA22F40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OlePropPage.h : header file
//
#include "resource.h"

extern const IID IOlePropPage;

/////////////////////////////////////////////////////////////////////////////
// COlePropPage dialog

class COlePropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COlePropPage)

// Construction
public:
	CString GetPageTitle();
	void ResetPropRect(RECT* pRect);
	SIZE GetPageSize();
	IPropertyPage* m_pPropPage;
	LPUNKNOWN* m_pObject;
	CLSID m_clsidPage;
	COlePropPage(CLSID clsidPage, LPUNKNOWN* lpAryUnk, int& count);
	COlePropPage();
	~COlePropPage();

// Dialog Data
	//{{AFX_DATA(COlePropPage)
	enum { IDD = IDD_OLE_PROPPAGE_LARGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COlePropPage)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COlePropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(COlePropPage)
	afx_msg long SetDynamicIcon(long index);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

    BEGIN_INTERFACE_PART(PropertySite, IPropertyPageSite)
        STDMETHOD(GetLocaleID)(LCID* pLocaleID);
        STDMETHOD(OnStatusChange)(DWORD dwFlags);
        STDMETHOD(GetPageContainer)(IUnknown **ppUnk);
        STDMETHOD(TranslateAccelerator)(MSG *pMsg);
	END_INTERFACE_PART(PropertySite)

	BEGIN_INTERFACE_PART(OlePropSiteCtrl, IOlePropPage)
	END_INTERFACE_PART(OlePropSiteCtrl)
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OLEPROPPAGE_H__469F345B_1A8B_11D3_B009_00105AA22F40__INCLUDED_)
