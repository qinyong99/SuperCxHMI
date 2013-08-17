// DataField.h : Declaration of the CDataField

#ifndef __CXDATAFIELD_H_
#define __CXDATAFIELD_H_

#include "resource.h"       // main symbols

class CDataRecordset;

/////////////////////////////////////////////////////////////////////////////
// CDataField
class ATL_NO_VTABLE CDataField : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDataField, &CLSID_CxDataField>,
	public IDispatchImpl<ICxDataField, &IID_ICxDataField, &LIBID_CXHMIOBJLib>
{
public:
	CDataField()
	{
		m_pDataRecordset = NULL;
		m_dwStatus = 0;
		m_lField = -1;
		m_lDefinedSize = 0;
		m_enumDataType = DataTypeEmpty;

		m_bCalcStatParameter = false;
	}
	
DECLARE_REGISTRY_RESOURCEID(IDR_CXDATAFIELD)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDataField)
	COM_INTERFACE_ENTRY(ICxDataField)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ICxDataField
public:
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Value)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_Value)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_Avg)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_Max)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_Min)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_Sum)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_DefinedSize)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Type)(/*[out, retval]*/ enumDataType *pVal);

public:
	void ReCalcStatParameter();

	CComVariant m_varValue;
	DWORD m_dwStatus;

	CDataRecordset* m_pDataRecordset;
	long m_lField;

	long m_lCount;
	CComVariant m_varSum;
	CComVariant m_varMin;
	CComVariant m_varMax;
	CComVariant m_varAvg;

	BOOL m_bCalcStatParameter;

	long m_lDefinedSize;
	enumDataType m_enumDataType;

	CComBSTR m_bstrName;
};

class CDataFieldObject : public CComObjectCached<CDataField>
{
public:
	CDataFieldObject()
	{
	}
	// CSimpleArray 会使用下面的构造函数
	CDataFieldObject(const CDataFieldObject& dfo)
	{
		InternalCopy(dfo);
	}
	void InternalCopy(const CDataFieldObject& dfo)
	{
		m_pDataRecordset = dfo.m_pDataRecordset;
		m_lField = dfo.m_lField;
		m_lCount = dfo.m_lCount;

		m_varValue = dfo.m_varValue;
		m_dwStatus = dfo.m_dwStatus;
		
		m_varSum = dfo.m_varSum;
		m_varMin = dfo.m_varMin;
		m_varMax = dfo.m_varMax;
		m_varAvg = dfo.m_varAvg;

		m_bstrName = dfo.m_bstrName;
	}
};

//typedef CComObjectCached<CDataField> CDataFieldObject;

#endif //__CXDATAFIELD_H_
