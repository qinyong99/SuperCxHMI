// DataRecordset.cpp : Implementation of CDataRecordset
#include "stdafx.h"
#include "CxExtObj.h"
#include "DataRecordset.h"
#include "SQLBuildDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDataRecordset

STDMETHODIMP CDataRecordset::InitNew()
{
	m_enumCommandType = CommandTypeUnknown;
	m_lMaxRecords = 0;

	return S_OK;
}

STDMETHODIMP CDataRecordset::get_CommandType(enumCommandType *pVal)
{
	*pVal = m_enumCommandType;
		
	return S_OK;
}

STDMETHODIMP CDataRecordset::put_CommandType(enumCommandType newVal)
{
	m_enumCommandType = newVal;
	
	return S_OK;
}

STDMETHODIMP CDataRecordset::get_Source(BSTR *pVal)
{
	*pVal = m_bstrSource.Copy();

	return S_OK;
}

STDMETHODIMP CDataRecordset::put_Source(BSTR newVal)
{
	m_bstrSource = newVal;

	return S_OK;
}

STDMETHODIMP CDataRecordset::Open()
{
	HRESULT hr;

	Close();

	// 释放
	if (m_Session.m_spOpenRowset != NULL)
		m_Session.m_spOpenRowset.Release();

	if (m_Connect.m_spInit != NULL)
		m_Connect.m_spInit.Release();

	CComBSTR bstrConnectionString;
	hr = GetConnectionString(bstrConnectionString);
	if (hr != S_OK)
		return hr;

	// 连接数据源
	if (wcsncmp(bstrConnectionString, L"FILE NAME=", 10) == 0)
		hr = m_Connect.OpenFromFileName(bstrConnectionString + 10);
	else
		hr = m_Connect.OpenFromInitializationString(bstrConnectionString);

	if (hr != S_OK)
		return hr;

	if (m_vbUseLocalSetup)
	{
		CComPtr<ICxDataSource> spCxDataSource;
		hr = m_Connect.m_spInit->QueryInterface(IID_ICxDataSource, (void**)&spCxDataSource);
		if (spCxDataSource !=NULL)
		{
			BOOL b = TRUE;
			spCxDataSource->SetUseLocalSetup(&b);
		}
	}
		
	// 创建会话
	hr = m_Session.Open(m_Connect);
	if (hr != S_OK)
		return hr;
	
	ExecutCommand();

	return S_OK;
}

HRESULT CDataRecordset::GetConnectionString(CComBSTR& bstrText)
{
	if ((m_varActiveConnection.vt == VT_DISPATCH) || 
		(m_varActiveConnection.vt == VT_UNKNOWN))
	{
		if (m_varActiveConnection.pdispVal != NULL)
		{
			CComDispatchDriver dd(m_varActiveConnection.pdispVal);
			CComVariant varT;
			dd.GetPropertyByName(L"ConnectionString", &varT);
			if (varT.vt == VT_BSTR)
			{
				bstrText = varT.bstrVal;
				return S_OK;
			}
		}
	}
	
	return S_FALSE;
}

void CDataRecordset::ExecutCommandText(LPCTSTR lpszCommandText, BOOL bSQL)
{
	CCommand<CManualAccessor>* pCommand = new CCommand<CManualAccessor>;
	if (pCommand == NULL)
		return;
	
	CDBPropSet propset(DBPROPSET_ROWSET);
	propset.AddProperty(DBPROP_CANSCROLLBACKWARDS, true);
	propset.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	
	HRESULT hr = pCommand->Open(m_Session, lpszCommandText, &propset, NULL, 
			bSQL ? DBGUID_DBSQL : DBGUID_DEFAULT, FALSE);
	if (FAILED(hr))
	{
		delete pCommand;
		return;
	}
	
	ULONG ulColumns = 0;
	DBCOLUMNINFO* pColumnInfo = NULL;
	LPOLESTR pColumnNames = NULL;
	if (pCommand->GetColumnInfo(&ulColumns, &pColumnInfo, &pColumnNames) != S_OK)
	{
		delete pCommand;
		return;
	}
	
	// 为每列创建字段对象
	for (ULONG l = 0; l < ulColumns; l++)
	{
		CDataFieldObject dfo;
		m_arrDataFileds.Add(dfo);
	}
	
	pCommand->CreateAccessor(ulColumns, m_arrDataFileds.GetData(), sizeof(CDataFieldObject) * ulColumns);
	
	for (l = 0; l < ulColumns; l++)
	{
		CDataFieldObject& dfo = m_arrDataFileds[l];
		dfo.m_pDataRecordset = this;
		dfo.m_bstrName = pColumnInfo[l].pwszName;
		dfo.m_lField = pColumnInfo[l].iOrdinal;
		dfo.m_enumDataType = (enumDataType)pColumnInfo[l].wType;
		dfo.m_lDefinedSize = pColumnInfo[l].ulColumnSize;
				
		pCommand->AddBindEntry(pColumnInfo[l].iOrdinal, DBTYPE_VARIANT, sizeof(VARIANT), &dfo.m_varValue,
			NULL, &dfo.m_dwStatus);
		
		dfo.AddRef();
		// 0 为书签
		if (pColumnInfo[l].iOrdinal != 0)
			m_lFieldCount++;
	}
	pCommand->Bind();
	
	m_pCommand = pCommand;
	m_pRowset = (CRowset *)pCommand;
	
	CoTaskMemFree(pColumnInfo);
	CoTaskMemFree(pColumnNames);

}

void CDataRecordset::FetchTable(LPCTSTR lpszTableName)
{
	ATLASSERT(lpszTableName != NULL);
	if (lpszTableName == NULL || lpszTableName[0] == '\0')
		return;

	CDatabaseTable<CManualAccessor>* pTable = new CDatabaseTable<CManualAccessor>;
	if (pTable == NULL)
		return;

	HRESULT hr = pTable->Open(m_Session, lpszTableName, NULL, FALSE);
	if (FAILED(hr))
	{
		delete pTable;
		return;
	}

	ULONG ulColumns = 0;
	DBCOLUMNINFO* pColumnInfo = NULL;
	LPOLESTR pColumnNames = NULL;
	if (pTable->GetColumnInfo(&ulColumns, &pColumnInfo, &pColumnNames) != S_OK)
	{
		delete pTable;
		return;
	}

	// 为每列创建字段对象
	for (ULONG l = 0; l < ulColumns; l++)
	{
		CDataFieldObject dfo;
		m_arrDataFileds.Add(dfo);
	}

	pTable->CreateAccessor(ulColumns, m_arrDataFileds.GetData(), sizeof(CDataFieldObject) * ulColumns);

	for (l = 0; l < ulColumns; l++)
	{
		CDataFieldObject& dfo = m_arrDataFileds[l];
		dfo.m_pDataRecordset = this;
		dfo.m_bstrName = pColumnInfo[l].pwszName;
		dfo.m_lField = pColumnInfo[l].iOrdinal; 
		dfo.m_enumDataType = (enumDataType)pColumnInfo[l].wType;
		dfo.m_lDefinedSize = pColumnInfo[l].ulColumnSize;

		pTable->AddBindEntry(pColumnInfo[l].iOrdinal, DBTYPE_VARIANT, sizeof(VARIANT), &dfo.m_varValue,
			NULL, &dfo.m_dwStatus);
		
		dfo.AddRef();
		// 0 为书签
		if (pColumnInfo[l].iOrdinal != 0)
			m_lFieldCount++;
	}
	pTable->Bind();

	m_pTable = pTable;
	m_pRowset = (CRowset *)pTable;
	
	CoTaskMemFree(pColumnInfo);
	CoTaskMemFree(pColumnNames);
}

void CDataRecordset::CallProcedure(LPCTSTR lpszProcName)
{
	TCHAR szCommandText[128];
	ATLASSERT(lpszProcName != NULL);
	
	sprintf(szCommandText, _T("{ Call %s }"), lpszProcName);
	ExecutCommandText(szCommandText, FALSE);
}

void CDataRecordset::ExecutCommand()
{
	USES_CONVERSION;

	switch (m_enumCommandType) 
	{
	case CommandTypeUnknown:
	case CommandTypeText:
		ExecutCommandText(OLE2T(m_bstrSource), m_enumCommandType == CommandTypeText);
		break;
	case CommandTypeTable:
		FetchTable(OLE2T(m_bstrSource));
		break;
	case CommandTypeStoredProc:
		CallProcedure(OLE2T(m_bstrSource));
		break;
	default:
		break;
	}

	if (m_pRowset != NULL)
	{
		HRESULT hr = m_pRowset->GetApproximatePosition(NULL, NULL, (ULONG *)&m_lRecordCount);
		if (FAILED(hr))
		{
			while (m_pRowset->MoveNext() == S_OK)
				m_lRecordCount++;

			MoveFirst();
		}
	}
}

STDMETHODIMP CDataRecordset::Close()
{
	m_lCurrentRecord = PositionUnknown;
	m_lRecordCount = 0;

	m_arrDataFileds.RemoveAll();
	m_lFieldCount = 0;

	FreeRecordMemory();
	if (m_pTable != NULL)
	{
		delete m_pTable;
		m_pTable = NULL;
	}
	if (m_pCommand != NULL)
	{
		delete m_pCommand;
		m_pCommand = NULL;
	}
	m_pRowset = NULL;

	return S_OK;
}

STDMETHODIMP CDataRecordset::MoveNext()
{
	if (m_pRowset == NULL)
		return E_FAIL;
	
	ATLASSERT(m_pRowset != NULL);

	FreeRecordMemory();
	
	HRESULT hr = m_pRowset->MoveNext();
	if (hr == S_OK)
	{
		if (m_lCurrentRecord < 0)
			m_lCurrentRecord = 0;
		else
			m_lCurrentRecord++;
	}
	else
	{
		m_lCurrentRecord = PositionEOF;
	}
	
	return hr;
}

STDMETHODIMP CDataRecordset::MovePrevious()
{
	if (m_pRowset == NULL)
		return E_FAIL;
	
	ATLASSERT(m_pRowset != NULL);
	
	FreeRecordMemory();

	HRESULT hr = m_pRowset->MovePrev();
	if (hr == S_OK)
	{
		if (m_lCurrentRecord == PositionEOF)
			m_lCurrentRecord = m_lRecordCount - 1;
		m_lCurrentRecord--;
	}
	else
	{
		m_lCurrentRecord = PositionBOF;
	}
	
	return hr;
}

STDMETHODIMP CDataRecordset::MoveFirst()
{
	if (m_pRowset == NULL)
		return E_FAIL;
	
	ATLASSERT(m_pRowset != NULL);
	
	FreeRecordMemory();

	HRESULT hr = m_pRowset->MoveFirst();
	if (hr == S_OK)
	{
		m_lCurrentRecord = 0;
	}
	else
	{
		m_lCurrentRecord = PositionUnknown;
	}
	
	return hr;
}

STDMETHODIMP CDataRecordset::MoveLast()
{
	if (m_pRowset == NULL)
		return E_FAIL;
	
	ATLASSERT(m_pRowset != NULL);

	FreeRecordMemory();

	HRESULT hr = m_pRowset->MoveLast();
	if (hr == S_OK)
	{
		m_lCurrentRecord = m_lRecordCount - 1;
	}
	else
	{
		m_lCurrentRecord = PositionUnknown;
	}

	return hr;
}

STDMETHODIMP CDataRecordset::get_BOF(VARIANT_BOOL *pVal)
{
	*pVal = m_lCurrentRecord == PositionBOF ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CDataRecordset::get_EOF(VARIANT_BOOL *pVal)
{
	*pVal = m_lCurrentRecord == PositionEOF ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CDataRecordset::get_Field(VARIANT Index, ICxDataField** ppvObject)
{
	USES_CONVERSION;

	if (!m_pRowset)
		return E_FAIL;

	ATLASSERT(m_pRowset != NULL);

	int nColumn = -1;

	if (Index.vt == VT_BSTR)
	{
		for (int i = 0; i < m_arrDataFileds.GetSize(); i++)
		{
			CDataFieldObject& dfo = m_arrDataFileds[i];
			if (dfo.m_lField == 0)
				continue;
			if (lstrcmpiW(dfo.m_bstrName, Index.bstrVal) == 0)
			{	
				nColumn = i;
				break;
			}
		}
	}
	else
	{
		CComVariant var = Index;
		var.ChangeType(VT_I4);

		if (var.vt == VT_I4)
		{
			nColumn = var.lVal - 1;
			if (m_arrDataFileds.GetSize() > 0 && m_arrDataFileds[0].m_lField == 0)
				nColumn += 1;
		}
	}
	

	if (nColumn < 0 || nColumn >= m_arrDataFileds.GetSize())
		return E_INVALIDARG;
	
	return m_arrDataFileds[nColumn].QueryInterface(IID_ICxDataField, (void **)ppvObject);
}

STDMETHODIMP CDataRecordset::get_MaxRecords(long *pVal)
{
	*pVal = m_lMaxRecords;

	return S_OK;
}

STDMETHODIMP CDataRecordset::put_MaxRecords(long newVal)
{
	m_lMaxRecords = newVal;

	return S_OK;
}

STDMETHODIMP CDataRecordset::get_FieldCount(long *pVal)
{
	if (m_pRowset == NULL)
	{
		*pVal = 0;
		return E_FAIL;
	}

	ATLASSERT(m_pRowset != NULL);
	*pVal = m_lFieldCount;

	return S_OK;
}

STDMETHODIMP CDataRecordset::get_ActiveConnection(VARIANT *pVal)
{
	VariantCopy(pVal, &m_varActiveConnection);

	return S_OK;
}

STDMETHODIMP CDataRecordset::put_ActiveConnection(VARIANT newVal)
{
	VariantCopy(&m_varActiveConnection, &newVal);

	return S_OK;
}

STDMETHODIMP CDataRecordset::get_RecordCount(/*[out, retval]*/ long *pVal)
{
	if (m_pRowset == NULL)
	{
		*pVal = 0;
		return E_FAIL;
	}

	*pVal = m_lRecordCount;

	return S_OK;
}

STDMETHODIMP CDataRecordset::put_RecordCount(/*[in]*/ long newVal)
{
	m_lRecordCount = newVal;

	return S_OK;
}

LPOLESTR CDataRecordset::GetFieldName(long lField)
{
	if (m_pRowset == NULL)
		return NULL;
	
	ATLASSERT(m_pRowset != NULL);

	if (lField < 1 || lField > m_lFieldCount)
		return NULL;

	return m_arrDataFileds[lField].m_bstrName;
}

STDMETHODIMP CDataRecordset::get_AbsolutePosition(/*[out, retval]*/ enumPosition *pVal)
{
	long lPos = m_lCurrentRecord;
	// 从1开始计数
	if (lPos >= 0)
		lPos++;
	*pVal = (enumPosition)lPos;

	return S_OK;
}

STDMETHODIMP CDataRecordset::put_AbsolutePosition(/*[in]*/ enumPosition newVal)
{
	if (m_pRowset == NULL || m_lCurrentRecord == PositionUnknown)
		return E_FAIL;
	
	ATLASSERT(m_pRowset != NULL);
	
	if (newVal < 1 || newVal > m_lRecordCount)
		return E_INVALIDARG;

	FreeRecordMemory();

	// 从1开始计数
	newVal = enumPosition(newVal - 1);

	HRESULT hr;
	switch (m_lCurrentRecord) 
	{
	case PositionEOF:
		hr = m_pRowset->MoveNext(newVal - m_lRecordCount - 1, true);
		break;
	case PositionBOF:
		hr = m_pRowset->MoveNext(newVal, true);
		break;
	default:
		hr = m_pRowset->MoveNext(newVal - m_lCurrentRecord - 1, true);
		break;
	}

	m_lCurrentRecord = newVal;
	return hr;
}

void CDataRecordset::FreeRecordMemory()
{
	if (m_pTable != NULL)
		m_pTable->FreeRecordMemory();
	if (m_pCommand != NULL)
		m_pCommand->FreeRecordMemory();
}

STDMETHODIMP CDataRecordset::get_UseLocalSetup(/*[out, retval]*/ VARIANT_BOOL *pVal)
{
	*pVal = m_vbUseLocalSetup;

	return S_OK;
}

STDMETHODIMP CDataRecordset::put_UseLocalSetup(/*[in]*/ VARIANT_BOOL newVal)
{
	m_vbUseLocalSetup = newVal;

	return S_OK;
}

//STDMETHODIMP CDataRecordset::get_Bookmark(/*[out, retval]*/ VARIANT *pVal)
//{
//	
//}
//
//STDMETHODIMP CDataRecordset::put_Bookmark(/*[in]*/ VARIANT newVal)
//{
//	

