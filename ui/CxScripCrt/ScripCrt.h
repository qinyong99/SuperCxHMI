// ScripCrt.h : Declaration of the CScripCrt

#ifndef __CxScriptCreator_H_
#define __CxScriptCreator_H_

#include "resource.h"       // main symbols
#include "MainFrame.h"

/////////////////////////////////////////////////////////////////////////////
// CScripCrt
class ATL_NO_VTABLE CScripCrt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CScripCrt, &CLSID_CxScriptCreator>,
	public ICxScriptCreator
{
public:
	CScripCrt()
	{
		m_hParentWnd = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CXSCRIPTCREATOR)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CScripCrt)
	COM_INTERFACE_ENTRY(ICxScriptCreator)
END_COM_MAP()

// ICxScriptCreator
public:
	STDMETHOD(SetParentWindow)(HWND hParentWnd);
	STDMETHOD(SetSite)(ICxScriptEditorSite* pSite);
	STDMETHOD(SetSupportSymbol)(DWORD dwSymbolMask);
	STDMETHOD(SetScriptText)(LPCWSTR lpszCode);
	STDMETHOD(GetScriptText)(LPWSTR* ppszCode);
	STDMETHOD(EditObjectEventHandler)(LPCWSTR lpszObject, LPCWSTR lpszEvent);

	CComPtr<ICxScriptEditorSite> m_spSite;
	CComBSTR m_bstrScriptText;
	HWND m_hParentWnd;
	DWORD m_dwSymbolMask;
};

#endif //__CxScriptCreator_H_
