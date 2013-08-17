// DynamicFrm.h : Declaration of the CDynamicFrm

#ifndef __DYNAMICFRM_H_
#define __DYNAMICFRM_H_

#include "resource.h"       // main symbols
#include "PropInfo.h"

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
};

/////////////////////////////////////////////////////////////////////////////
// CDynamicFrm

class CDynamicInfo
{
public:
	CDynamicInfo()
	{
		clsid = CLSID_NULL;
	}

	CLSID clsid;
	CComPtr<IPropertyPage> spPropPage;
	CComPtr<IUnknown> spUnkDynamic;
};

class ATL_NO_VTABLE CDynamicFrm : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDynamicFrm, &CLSID_DynamicFrm>,
	public CDialogImpl<CDynamicFrm>,
	public IPropertyPageSiteImpl<CDynamicFrm>,
	public IDynamicFrm
{
public:
	CDynamicFrm()
	{
		m_pPropInfo = NULL;
		m_nCurDyn = -1;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DYNAMICFRM)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDynamicFrm)
	COM_INTERFACE_ENTRY(IDynamicFrm)
	COM_INTERFACE_ENTRY(IPropertyPageSite)
END_COM_MAP()

enum {IDD = IDD_DYNAMICFRM};

BEGIN_MSG_MAP(CDynamicFrm)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_HANDLER(IDC_LINEAR, BN_CLICKED, OnClickedLinear)
	COMMAND_HANDLER(IDC_LOOKUP, BN_CLICKED, OnClickedLookup)
	COMMAND_HANDLER(IDC_FORMAT, BN_CLICKED, OnClickedFormat)
	COMMAND_HANDLER(IDC_DIRECT, BN_CLICKED, OnClickedDirect)
	COMMAND_HANDLER(IDC_CREATE_EXPRESSION, BN_CLICKED, OnClickedCreateExpression)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnClickedLinear(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedLookup(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedFormat(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedDirect(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCreateExpression(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	void OnFinalMessage(HWND hWnd);

// IDynamicFrm
public:
	STDMETHOD(EditDynamic)(long hParent, long X, long Y);
	STDMETHOD(get_Source)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Source)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_PropertyInfo)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_PropertyInfo)(/*[in]*/ long newVal);
	STDMETHOD(OnCreateExpression)(BSTR* pbstr);

// IPropertyPageSite
public:
	HRESULT PropertyChange()
	{
		return (HRESULT)NOERROR;
	}

	void GetDynamicClassID(IUnknown* pDynUnk, CLSID* pClassID);
	int GetDynamic(CLSID clsid);
	void ShowDynamicPage(int nDynamic);

protected:
	CSimpleArray<CDynamicInfo *> m_apDynInfos;
	int m_nCurDyn;

	CDynamicPropInfo* m_pPropInfo;
	CComBSTR m_bstrSource;
};

#endif //__DYNAMICFRM_H_
