// DataLinkGenPage.h : Declaration of the DataLinkGenPage

#ifndef __CXDATALINKGENPAGE_H_
#define __CXDATALINKGENPAGE_H_

#include "resource.h"       // main symbols

#include "../CxDynObjs/PropInfo.h"
#include "../CxDynObjs/CxDynObjs.h"

EXTERN_C const CLSID CLSID_CxDataLinkGenPage;

//////////////////////////////////////////
template <class T>
class ATL_NO_VTABLE IPropertyPageSiteImpl : public IPropertyPageSite
{
public:
	STDMETHOD(GetLocaleID)(LCID* pLocaleID)
	{
		*pLocaleID = ::GetThreadLocale();
		return (HRESULT)NOERROR;   
	}
    STDMETHOD(OnStatusChange)(DWORD dwFlags)
	{
		T* pT = static_cast<T*>(this);
		return pT->PropertyChange();
	}
    STDMETHOD(GetPageContainer)(IUnknown **ppUnk)
	{
		return (HRESULT)E_NOTIMPL;
	}
    STDMETHOD(TranslateAccelerator)(MSG *pMsg)
	{
		return (HRESULT)E_NOTIMPL;   
	}
	
	CComPtr<IUnknown> m_spUnkSite;
};

/////////////////////////////////////////////////////////////////////////////
// CDataLinkGenPage
class ATL_NO_VTABLE CDataLinkGenPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDataLinkGenPage, &CLSID_CxDataLinkGenPage>,
	public IPropertyPageImpl<CDataLinkGenPage>,
	public CDialogImpl<CDataLinkGenPage>,
	public IPropertyPageSiteImpl<CDataLinkGenPage>,
	public IDynamicFrm
{
public:
	CDataLinkGenPage() 
	{
		m_dwTitleID = IDS_TITLECxDataLinkGenPage;
		m_dwHelpFileID = IDS_HELPFILECxDataLinkGenPage;
		m_dwDocStringID = IDS_DOCSTRINGCxDataLinkGenPage;
		
		m_bObjectIsFormmatDynamic = FALSE;
	}
	
	enum {IDD = IDD_CXDATALINKGENPAGE};
	
	DECLARE_REGISTRY_RESOURCEID(IDR_CXDATALINKGENPAGE)
		
	DECLARE_PROTECT_FINAL_CONSTRUCT()
		
	BEGIN_COM_MAP(CDataLinkGenPage) 
		COM_INTERFACE_ENTRY(IPropertyPage)
		COM_INTERFACE_ENTRY(IPropertyPageSite)
		COM_INTERFACE_ENTRY(IDynamicFrm)
	END_COM_MAP()
		
	BEGIN_MSG_MAP(CDataLinkGenPage)
		CHAIN_MSG_MAP(IPropertyPageImpl<CDataLinkGenPage>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_CREATE_EXPRESSION, BN_CLICKED, OnClickedCreateExpression)
	END_MSG_MAP()
		// Handler prototypes:
		//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
		
	LRESULT OnInitDialog(UINT, WPARAM wParam, LPARAM lParam, BOOL&);
	LRESULT OnClickedCreateExpression(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// IDynamicFrm
public:
	STDMETHOD(EditDynamic)(long hParent, long X, long Y);
	STDMETHOD(get_Source)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Source)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_PropertyInfo)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_PropertyInfo)(/*[in]*/ long newVal);
	STDMETHOD(OnCreateExpression)(BSTR* pbstr);
	
	STDMETHOD(Apply)(void);
	
	void OnFinalMessage(HWND hWnd);
	
	HRESULT PropertyChange()
	{
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
		
		return S_OK;
	}
	
private:
	BOOL m_bObjectIsFormmatDynamic; // 
	
	CComPtr<IFormatDynamic> m_spFormatDynamic;
	CComPtr<IPropertyPage> m_spFormatDynamicPage;

	CDynamicPropInfo m_propinfo;
};

#endif //__CXDATALINKGENPAGE_H_
