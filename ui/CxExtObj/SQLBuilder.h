// SQLBuilder.h : Declaration of the CSQLBuilder

#ifndef __CXSQLBUILDER_H_
#define __CXSQLBUILDER_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSQLBuilder
class ATL_NO_VTABLE CSQLBuilder : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSQLBuilder, &CLSID_CxSQLBuilder>,
	public IDispatchImpl<ICxSQLBuilder, &IID_ICxSQLBuilder, &LIBID_CXEXTOBJLib>
{
public:
	CSQLBuilder()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CXSQLBUILDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSQLBuilder)
	COM_INTERFACE_ENTRY(ICxSQLBuilder)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ICxSQLBuilder
public:
	STDMETHOD(Build)(/*[in]*/ long ParentWnd, /*[out, retval]*/ BSTR* pbstrSQL);
};

#endif //__CXSQLBUILDER_H_
