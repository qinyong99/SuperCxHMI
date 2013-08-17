// DataField.cpp : Implementation of CDataField
#include "stdafx.h"
#include "CxExtObj.h"
#include "DataField.h"
#include "DataRecordset.h"

/////////////////////////////////////////////////////////////////////////////
// CDataField

STDMETHODIMP CDataField::get_Name(BSTR *pVal)
{
	ATLASSERT(m_pDataRecordset != NULL);
	*pVal = m_bstrName.Copy();

	return S_OK;
}

STDMETHODIMP CDataField::get_Value(VARIANT *pVal)
{
	ATLASSERT(m_pDataRecordset != NULL);
	
	return ::VariantCopy(pVal, &m_varValue);

//	BOOL b = m_pDataRecordset->GetValue(m_lField, pVal);

//	return b ? S_OK : E_FAIL;
}

STDMETHODIMP CDataField::put_Value(VARIANT newVal)
{
	ATLASSERT(m_pDataRecordset != NULL);
	
	return ::VariantCopy(&m_varValue, &newVal);
}

STDMETHODIMP CDataField::get_Avg(VARIANT *pVal)
{
	ReCalcStatParameter();

	return ::VariantCopy(pVal, &m_varAvg);
}

STDMETHODIMP CDataField::get_Max(VARIANT *pVal)
{
	ReCalcStatParameter();

	return ::VariantCopy(pVal, &m_varMax);
}


STDMETHODIMP CDataField::get_Min(VARIANT *pVal)
{
	ReCalcStatParameter();

	return ::VariantCopy(pVal, &m_varMin);
}

STDMETHODIMP CDataField::get_Sum(VARIANT *pVal)
{
	ReCalcStatParameter();

	return ::VariantCopy(pVal, &m_varSum);
}

STDMETHODIMP CDataField::get_Count(long *pVal)
{
	ReCalcStatParameter();
	*pVal = m_lCount;
	
	return S_OK;
}

STDMETHODIMP CDataField::get_DefinedSize(/*[out, retval]*/ long *pVal)
{
	*pVal = m_lDefinedSize;

	return S_OK;
}

STDMETHODIMP CDataField::get_Type(/*[out, retval]*/ enumDataType *pVal)
{
	*pVal = m_enumDataType;

	return S_OK;
}

void CDataField::ReCalcStatParameter()
{
	if (m_bCalcStatParameter)
		return;

	m_bCalcStatParameter = TRUE;

	m_lCount = 0;
	m_varSum.Clear();
	m_varMin.Clear();
	m_varMax.Clear();
	m_varAvg.Clear();
	
	enumPosition pos;
	m_pDataRecordset->get_AbsolutePosition(&pos);

	HRESULT hr = m_pDataRecordset->MoveFirst();
	while (hr == S_OK)
	{
		if (m_varValue.vt != VT_EMPTY)
		{
			m_lCount++;

			if (m_varMin.vt == VT_EMPTY || m_varMin > m_varValue)
				m_varMin = m_varValue;
			if (m_varMax.vt == VT_EMPTY || m_varMax < m_varValue)
				m_varMax = m_varValue;

			if (m_varSum.vt == VT_EMPTY)
				m_varSum = m_varValue;
			else
				VarAdd(&m_varSum, &m_varValue, &m_varSum);
		}

		hr = m_pDataRecordset->MoveNext();
	}

	if (m_lCount > 0)
	{
		CComVariant var = m_lCount;
		VarDiv(&m_varSum, &var, &m_varAvg);
	}

	m_pDataRecordset->MoveFirst();
	m_pDataRecordset->put_AbsolutePosition(pos);
}
