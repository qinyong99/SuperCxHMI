// ReturnInteger.h : Declaration of the CReturnInteger

#ifndef __CXRETURNINTEGER_H_
#define __CXRETURNINTEGER_H_

#include "resource.h"       // main symbols
//#include "AtlCtl.h"

/////////////////////////////////////////////////////////////////////////////
// CReturnInteger
class ATL_NO_VTABLE CReturnInteger : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CReturnInteger, &CLSID_CxReturnInteger>,
	public IDispatchImpl<ICxReturnInteger, &IID_ICxReturnInteger, &LIBID_CXHMIOBJLib>
//	public IObjectSafetyImpl<CReturnInteger, INTERFACESAFE_FOR_UNTRUSTED_CALLER>
{
public:
	CReturnInteger()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CXRETURNINTEGER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CReturnInteger)
	COM_INTERFACE_ENTRY(ICxReturnInteger)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ICxReturnInteger
public:
	STDMETHOD(get_Value)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_Value)(/*[in]*/ int newVal);

	int m_Value;
};

#endif //__CXRETURNINTEGER_H_
