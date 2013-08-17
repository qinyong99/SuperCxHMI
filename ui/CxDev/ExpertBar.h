// ExpertBar.h: interface for the CExpertBar class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPERTBAR_H__CE6D1B73_C22E_485E_8A1F_5FDD488BF385__INCLUDED_)
#define AFX_EXPERTBAR_H__CE6D1B73_C22E_485E_8A1F_5FDD488BF385__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

_COM_SMARTPTR_TYPEDEF(ICxExpertPage, _uuidof(ICxExpertPage));

class CExpertBar : public CSizingControlBar  
{
public:
	CExpertBar();
	virtual ~CExpertBar();

	BOOL Create(const CLSID& clsidPage, LPCTSTR lpszTitle, UINT nID, CWnd* pParentWnd);

	void AdjustPosition();

	BOOL GetEventHandler(LPCTSTR lpszEvent, CString& strSubBody);
	BOOL InsertEventHandler(LPCTSTR lpszEvent, LPCTSTR lpszParams, LPCTSTR lpszSubBody);
	void OnNextEventObject();
	void OnNextDynamicObject();
	void OnEditEventHandler(LPCTSTR lpszEvent);
	BOOL HaveDynamic();
	
	BOOL OnUpdateCaption(LPCTSTR lpszObject);

	void OnPropertyChanging();
	void OnPropertyChanged();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResultView)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	
	BEGIN_INTERFACE_PART(PageSite, ICxExpertPageSite)
		INIT_INTERFACE_PART(CExpertBar, PageSite)
		STDMETHOD(OnStatusChange)(DWORD dwFlags);
		STDMETHOD(GetLocaleID)(LCID* pLocaleID);
		STDMETHOD(GetPageContainer)(IUnknown** ppUnk);
		STDMETHOD(TranslateAccelerator)(LPMSG pMsg);
		STDMETHOD(OnGetDataSource)(BSTR* pbstrSource);
		STDMETHOD(OnNextEventObject)();
		STDMETHOD(OnEditEventHandler)(BSTR bstrEvent);
		STDMETHOD(GetEventHandler)(BSTR bstrEvent, BSTR *pbstrHandler);
		STDMETHOD(InsertEventHandler)(BSTR bstrEvent, BSTR bstrParams, BSTR bstrHandler);
		STDMETHOD(OnNextDynamicObject)();
		STDMETHOD(HaveDynamic)();
	END_INTERFACE_PART(PageSite)

	DECLARE_INTERFACE_MAP()

protected:
	CLayoutView* GetActiveLayoutView();
	CDocument* GetDocument() 
	{ 
		CMDIFrameWnd* pParentFrame = (CMDIFrameWnd*)AfxGetMainWnd();
		CMDIChildWnd* pMDIActive = pParentFrame->MDIGetActive();
		if (pMDIActive == NULL)
			return NULL;
		return pMDIActive->GetActiveDocument();
	}
	
	//{{AFX_MSG(CExpertBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LONG OnSelectChangedMsg(DWORD dwNotify, LONG);
    DECLARE_MESSAGE_MAP()

protected:
	CLSID m_clsidPage;
	IPropertyPagePtr m_pPage;
	CSize m_sizePage;
	BOOL m_bShowPage;

	friend class CMainFrame;
};

#endif // !defined(AFX_EXPERTBAR_H__CE6D1B73_C22E_485E_8A1F_5FDD488BF385__INCLUDED_)


