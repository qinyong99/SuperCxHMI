// DirectDynamic.h : Declaration of the CDirectDynamic

#ifndef __DIRECTDYNAMIC_H_
#define __DIRECTDYNAMIC_H_

#include "resource.h"       // main symbols
#include "CxdynObj.h"

/////////////////////////////////////////////////////////////////////////////
// CDirectDynamic
class ATL_NO_VTABLE CDirectDynamic : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDirectDynamic, &CLSID_DirectDynamic>,
	public IDispatchImpl<IDirectDynamic, &IID_IDirectDynamic, &LIBID_CXHMIOBJLib>,
	public IPersistStream,
	public CCxDynamicObjectBase,
	public ICxDynamicObjectImpl<CDirectDynamic>
{
public:
	CDirectDynamic()
	{
		m_bEnableMouseInput = FALSE;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DIRECTDYNAMIC)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDirectDynamic)
	COM_INTERFACE_ENTRY(IDirectDynamic)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPersistStream)
	COM_INTERFACE_ENTRY(ICxDynamicObject)
END_COM_MAP()

public:
	STDMETHOD(GetClassID)(CLSID* pClassID);
	STDMETHOD(GetSizeMax)(ULARGE_INTEGER FAR* /* pcbSize */)
	{
		ATLTRACENOTIMPL(_T("IPersistStream::GetSizeMax"));
	}
	STDMETHOD(InitNew)(long lPropInfo);
	STDMETHOD(IsDirty)();
	STDMETHOD(Load)(LPSTREAM pStm);
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty);
	STDMETHOD(GetClassName)(BSTR* pbstrClassName);
	STDMETHOD(GetCustomPage)(CLSID* pclsidPropPage);
	STDMETHOD(IsEnableMouseInput)() { return m_bEnableMouseInput ? S_OK : S_FALSE; }

public:
	STDMETHOD(get_Source)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Source)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_EnableMouseInput)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_EnableMouseInput)(/*[in]*/ BOOL newVal);
	STDMETHOD(Clone)(IDirectDynamic** ppDynamic);

	BOOL m_bEnableMouseInput;
};

#endif //__DIRECTDYNAMIC_H_
