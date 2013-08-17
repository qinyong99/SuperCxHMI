// AdvancedDlg.h: interface for the CAdvancedDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ADVANCEDDLG_H_
#define __ADVANCEDDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"

class CDynamicPropInfo;
class CDynamicExpert;

#include "../CxDynObjs/CxDynObjs.h"

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

class CAdvancedDlg :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CDialogImpl<CAdvancedDlg>,
	public IPropertyPageSiteImpl<CAdvancedDlg>,
	public IDynamicFrm
{
public:
	CAdvancedDlg();
	virtual ~CAdvancedDlg();
	
	enum { IDD = IDD_ADVANCED };

	BEGIN_COM_MAP(CAdvancedDlg)
		COM_INTERFACE_ENTRY(IDynamicFrm)
		COM_INTERFACE_ENTRY(IPropertyPageSite)
	END_COM_MAP()
	
	BEGIN_MSG_MAP(CAdvancedDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_LINEAR, BN_CLICKED, OnClickedLinear)
		COMMAND_HANDLER(IDC_LOOKUP, BN_CLICKED, OnClickedLookup)
		COMMAND_HANDLER(IDC_FORMAT, BN_CLICKED, OnClickedFormat)
		COMMAND_HANDLER(IDC_DIRECT, BN_CLICKED, OnClickedDirect)
	END_MSG_MAP()

	// IDynamicFrm
public:
	STDMETHOD(EditDynamic)(long hParent, long X, long Y);
	STDMETHOD(get_Source)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Source)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_PropertyInfo)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_PropertyInfo)(/*[in]*/ long newVal);
	STDMETHOD(OnCreateExpression)(BSTR* pbstr);

public:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedLinear(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedLookup(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedFormat(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedDirect(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

 	void OnFinalMessage(HWND hWnd);
	
	virtual void OnShow();
	virtual void OnOK();

	void HideDynamicPage();

	HRESULT PropertyChange();

	int GetDynamic(CLSID clsid);
	void ShowDynamicPage(int nDynamic);
	
	CSimpleArray<CDynamicInfo *> m_apDynInfos;
	int m_nCurDyn;
	CDynamicPropInfo* m_pPropInfo;
	CComBSTR m_bstrSource;
	BOOL m_bCanChangeMode;

	CDynamicExpert* m_pExpert;
};

#endif // __ADVANCEDDLG_H_
