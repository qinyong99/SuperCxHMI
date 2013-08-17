// PropertyPageDlg.h: interface for the CPropertyPageDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __PROPERTYPAGEDLG_H_
#define __PROPERTYPAGEDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CPropertyPageDlg : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CDialogImpl<CPropertyPageDlg>,
	public IPropertyPageSite
{
public:
	CPropertyPageDlg();
	CPropertyPageDlg(CLSID clsidPage, IUnknown* pUnk);
	virtual ~CPropertyPageDlg();

	enum { IDD = IDD_PAGE_DLG };
	
BEGIN_MSG_MAP(CPropertyPageDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()

BEGIN_COM_MAP(CPropertyPageDlg)
	COM_INTERFACE_ENTRY(IPropertyPageSite)
END_COM_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	void OnFinalMessage(HWND hWnd);

	// IPropertyPageSite
public:
	STDMETHOD(GetLocaleID)(LCID* pLocaleID)
	{
		*pLocaleID = ::GetThreadLocale();
		return (HRESULT)NOERROR;   
	}
    STDMETHOD(OnStatusChange)(DWORD dwFlags)
	{
		return (HRESULT)E_NOTIMPL;
	}
    STDMETHOD(GetPageContainer)(IUnknown **ppUnk)
	{
		return (HRESULT)E_NOTIMPL;
	}
    STDMETHOD(TranslateAccelerator)(MSG *pMsg)
	{
		return (HRESULT)E_NOTIMPL;   
	}

public:
	HRESULT ShowPage();

	CLSID m_clsidPage;
	CComPtr<IUnknown> m_spObject;
	CComBSTR m_bstrCaption;

	CComPtr<IPropertyPage> m_spPropertyPage;
};

#endif // __PROPERTYPAGEDLG_H_