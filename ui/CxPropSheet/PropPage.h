#if !defined(AFX_PROPPAGE_H__9D843BFA_5D38_43EC_BFBE_DA0C83E63D75__INCLUDED_)
#define AFX_PROPPAGE_H__9D843BFA_5D38_43EC_BFBE_DA0C83E63D75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPage.h : header file
// 所属项目: AutoCx的CxPropSheet工程
// 创建日期: 2001.10.8
// 创 建 人：孙越洋 
// 功能描述：属性页类,用于停放ole属性页对象。 
// 修改记录：
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CPropPage dialog
class CPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropPage)

// Construction
public:
	CPropPage();
	CPropPage(CLSID clsidPage, LPUNKNOWN* lpAryUnk, int& count);
	~CPropPage();

// Dialog Data
	//{{AFX_DATA(CPropPage)
	enum { IDD = IDD_OLE_PROPPAGE_LARGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropPage)
	public:
	virtual void OnFinalRelease();
	virtual BOOL OnApply();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

//data
public:
	long m_lImageIndex;
	void ResetPropRect(RECT* pRect);
	CString GetPageTitle();
	SIZE GetPageSize();
	IPropertyPage* m_pPropPage;
	LPUNKNOWN* m_pObject;
	CLSID m_clsidPage;
		
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPage)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CPropPage)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

    BEGIN_INTERFACE_PART(PropertySite, ICxPropertyPageSite)
        STDMETHOD(GetLocaleID)(LCID* pLocaleID);
        STDMETHOD(OnStatusChange)(DWORD dwFlags);
        STDMETHOD(GetPageContainer)(IUnknown **ppUnk);
        STDMETHOD(TranslateAccelerator)(MSG *pMsg);
		STDMETHOD(SetDynamicIcon)(long nIndex);
	END_INTERFACE_PART(PropertySite)
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGE_H__9D843BFA_5D38_43EC_BFBE_DA0C83E63D75__INCLUDED_)
