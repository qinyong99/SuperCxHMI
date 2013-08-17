// DataRecordset.h : Declaration of the CDataRecordset

#ifndef __CXDATARECORDSET_H_
#define __CXDATARECORDSET_H_

#include "resource.h"       // main symbols

#include "DataField.h"

struct VARIANTBIND
{
	VARIANTBIND()
	{
		memset(this, 0, sizeof(*this));
	}
	
	VARIANT varValue;
	DWORD dwStatus;
};

template <class TAccessor = CNoAccessor, class TRowset = CRowset>
class CDatabaseTable :
	public CAccessorRowset<TAccessor, TRowset>
{
public:
	// Open a rowset on the passed name
	HRESULT Open(const CSession& session, LPCTSTR szTableName, DBPROPSET* pPropSet = NULL, bool bBind = true)
	{
		USES_CONVERSION;
		DBID    idTable;
		
		idTable.eKind           = DBKIND_NAME;
		idTable.uName.pwszName  = (LPOLESTR)T2COLE(szTableName);
		
		return Open(session, idTable, pPropSet, bBind);
	}
	// Open the a rowset on the passed DBID
	HRESULT Open(const CSession& session, DBID& dbid, DBPROPSET* pPropSet = NULL, bool bBind = true)
	{
		// Check the session is valid
		ATLASSERT(session.m_spOpenRowset != NULL);
		HRESULT hr;
		
		hr = session.m_spOpenRowset->OpenRowset(NULL, &dbid, NULL, GetIID(),
			(pPropSet) ? 1 : 0, pPropSet, (IUnknown**)GetInterfacePtr());
		if (SUCCEEDED(hr))
		{
			SetupOptionalRowsetInterfaces();
			
			// If we have output columns then bind
			if (bBind && _OutputColumnsClass::HasOutputColumns())
				hr = Bind();
		}
		
		return hr;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CDataRecordset
class ATL_NO_VTABLE CDataRecordset : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDataRecordset, &CLSID_CxDataRecordset>,
	public IPersistStreamInitImpl<CDataRecordset>,
	public IDispatchImpl<ICxDataRecordset, &IID_ICxDataRecordset, &LIBID_CXHMIOBJLib>
{
public:
	CDataRecordset()
	{
		m_enumCommandType = CommandTypeUnknown;

		m_pTable = NULL;
		m_pCommand = NULL;
		m_pRowset = NULL;

		m_lFieldCount = 0;
		m_lRecordCount = 0;
		m_lCurrentRecord = PositionUnknown;

		m_vbUseLocalSetup = VARIANT_FALSE;
	}

	~CDataRecordset()
	{
		Close();
	}

	unsigned m_bRequiresSave:1;

DECLARE_REGISTRY_RESOURCEID(IDR_CXDATARECORDSET)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDataRecordset)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
//	COM_INTERFACE_ENTRY(IPersistStream)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ICxDataRecordset)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

BEGIN_PROP_MAP(CDataRecordset)
	PROP_ENTRY("Source", 1, CLSID_NULL)
	PROP_ENTRY("CommandType", 13, CLSID_NULL)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

// IPersistStreamInit
public:
	STDMETHOD(InitNew)();

// ICxDataRecordset
public:
	STDMETHOD(get_CommandType)(/*[out, retval]*/ enumCommandType *pVal);
	STDMETHOD(put_CommandType)(/*[in]*/ enumCommandType newVal);
	STDMETHOD(get_ActiveConnection)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_ActiveConnection)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_FieldCount)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_MaxRecords)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_MaxRecords)(/*[in]*/ long newVal);
	STDMETHOD(get_Field)(/*[in]*/ VARIANT Index, /*[out, retval]*/ ICxDataField** ppvObject);
	STDMETHOD(get_EOF)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_BOF)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(MoveLast)();
	STDMETHOD(MoveFirst)();
	STDMETHOD(MovePrevious)();
	STDMETHOD(MoveNext)();
	STDMETHOD(Close)();
	STDMETHOD(Open)();
	STDMETHOD(get_Source)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Source)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_RecordCount)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_RecordCount)(/*[in]*/ long newVal);
	STDMETHOD(get_AbsolutePosition)(/*[out, retval]*/ enumPosition *pVal);
	STDMETHOD(put_AbsolutePosition)(/*[in]*/ enumPosition newVal);
	STDMETHOD(get_UseLocalSetup)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_UseLocalSetup)(/*[in]*/ VARIANT_BOOL newVal);
//	STDMETHOD(get_Bookmark)(/*[out, retval]*/ VARIANT *pVal);
//	STDMETHOD(put_Bookmark)(/*[in]*/ VARIANT newVal);


public:
	HRESULT GetConnectionString(CComBSTR& bstrText);

	void ExecutCommandText(LPCTSTR lpszCommandText, BOOL bSQL = TRUE);
	void FetchTable(LPCTSTR lpszTableName);
	void CallProcedure(LPCTSTR lpszProcName);
	void ExecutCommand();

	void FreeRecordMemory();

	LPOLESTR GetFieldName(long lField);

	CComBSTR m_bstrSource;
	enumCommandType m_enumCommandType;
	CComVariant m_varActiveConnection;

	long m_lMaxRecords;

	CDataSource m_Connect;
	CSession m_Session;

	CDatabaseTable<CManualAccessor>* m_pTable;
	CCommand<CManualAccessor>* m_pCommand;
	CRowset* m_pRowset;

	CSimpleArray<CDataFieldObject> m_arrDataFileds;

	long m_lFieldCount;
	long m_lRecordCount;
	long m_lCurrentRecord;
	VARIANT_BOOL m_vbUseLocalSetup;
};

#endif //__CXDATARECORDSET_H_
