// DataConnection.h : Declaration of the CDataConnection

#ifndef __CXDATACONNECTION_H_
#define __CXDATACONNECTION_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDataConnection
class ATL_NO_VTABLE CDataConnection : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDataConnection, &CLSID_CxDataConnection>,
	public IPersistStreamInitImpl<CDataConnection>,
	public IDispatchImpl<ICxDataConnection, &IID_ICxDataConnection, &LIBID_CXHMIOBJLib>
{
public:
	CDataConnection()
	{
	}

	unsigned m_bRequiresSave:1;

DECLARE_REGISTRY_RESOURCEID(IDR_CXDATACONNECTION)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDataConnection)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
//	COM_INTERFACE_ENTRY(IPersistStream)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ICxDataConnection)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

BEGIN_PROP_MAP(CDataConnection)
	PROP_ENTRY("ConnectionString", 1, CLSID_NULL)
	PROP_ENTRY("UserName", 2, CLSID_NULL)
	PROP_ENTRY("Password", 3, CLSID_NULL)
	PROP_ENTRY("ConnectionTimeout", 4, CLSID_NULL)
	PROP_ENTRY("CommandTimeout", 5, CLSID_NULL)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

// ICxDataConnection
public:
	STDMETHOD(get_CommandTimeout)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_CommandTimeout)(/*[in]*/ long newVal);
	STDMETHOD(get_ConnectionTimeout)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ConnectionTimeout)(/*[in]*/ long newVal);
	STDMETHOD(get_Password)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Password)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_UserName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_UserName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ConnectionString)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ConnectionString)(/*[in]*/ BSTR newVal);

private:
	CComBSTR m_bstrConnectionString;
	CComBSTR m_bstrUserName;
	CComBSTR m_bstrPassword;
	long m_lConnectionTimeout;
	long m_lCommandTimeout;

	long m_lConnectionMode;
};

#endif //__CXDATACONNECTION_H_
