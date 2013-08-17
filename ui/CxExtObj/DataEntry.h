// DataEntry.h : Declaration of the CDataEntry

#ifndef __CXDATAENTRY_H_
#define __CXDATAENTRY_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDataEntry
class ATL_NO_VTABLE CDataEntry : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDataEntry, &CLSID_CxDataEntry>,
	public IDispatchImpl<ICxDataEntry, &IID_ICxDataEntry, &LIBID_CXHMIOBJLib>
{
public:
	CDataEntry()
	{
		m_enumDataEntryMode = DataEntryModeNumeric;
		m_varMin = 0;
		m_varMax = 100;
		m_bstrOpenCaption = L"´ò¿ª";
		m_bstrCloseCaption = L"¹Ø±Õ";
		m_vbFetch = FALSE;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CXDATAENTRY)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDataEntry)
	COM_INTERFACE_ENTRY(ICxDataEntry)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ICxDataEntry
public:
	STDMETHOD(SetScriptEngineSite)(IUnknown *punkSite);
	STDMETHOD(show)();
	STDMETHOD(get_openCaption)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_openCaption)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_closeCaption)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_closeCaption)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_max)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_max)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_min)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_min)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_var)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_var)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_mode)(/*[in]*/ enumDataEntryMode *pVal);
	STDMETHOD(put_mode)(/*[out, retval]*/ enumDataEntryMode newVal);
	STDMETHOD(get_fetch)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_fetch)(/*[in]*/ VARIANT_BOOL newVal);

public:
	
	BOOL GetValue(VARIANT* pVar);
	BOOL PutValue(VARIANT* pVar);


	enumDataEntryMode m_enumDataEntryMode;
	CComBSTR m_bstrVal;
	CComVariant m_varMin;
	CComVariant m_varMax;
	CComBSTR m_bstrOpenCaption;
	CComBSTR m_bstrCloseCaption;
	VARIANT_BOOL m_vbFetch;

	CComQIPtr<ICxScriptEngineSite> m_spScriptEngineSite;
};

#endif //__CXDATAENTRY_H_
