// DataRecordsetPage.cpp : Implementation of CDataRecordsetPage
#include "stdafx.h"
#include "CxExtObj.h"
#include "DataRecordsetPage.h"
#include "SQLBuildDlg.h"
#include "OEMSetup.h"

/////////////////////////////////////////////////////////////////////////////
// CDataRecordsetPage

void CDataRecordsetPage::UpdateWindowsState()
{
	int nSel = m_cmbCommandType.GetCurSel();
	BOOL bDatabaseObject = (nSel == 2 || nSel == 3);

	m_cmdDatabaseObject.EnableWindow(bDatabaseObject);
	m_editCommandText.EnableWindow(!bDatabaseObject);

	if (bDatabaseObject)
		RefreshDatabaseObject();
}

HRESULT CDataRecordsetPage::GetConnectionString(CComBSTR& bstrText)
{
	if (m_nObjects > 0)
	{
		CComQIPtr<ICxDataRecordset, &IID_ICxDataRecordset> pRecordset(m_ppUnk[0]);
		
		CComVariant var;
		pRecordset->get_ActiveConnection(&var);
		
		if ((var.vt == VT_DISPATCH) || (var.vt == VT_UNKNOWN))
		{
			if (var.pdispVal != NULL)
			{
				CComDispatchDriver dd(var.pdispVal);
				CComVariant varT;
				dd.GetPropertyByName(L"ConnectionString", &varT);
				if (varT.vt == VT_BSTR)
				{
					bstrText = varT.bstrVal;
					return S_OK;
				}
			}
		}
	}

	return S_FALSE;
}


void CDataRecordsetPage::RefreshDatabaseObject()
{
	m_cmdDatabaseObject.ResetContent();
	
	CComBSTR bstrConnectionString;
	HRESULT hr = GetConnectionString(bstrConnectionString);
	if (hr != S_OK)
		return;

	CDataSource source;

	if (wcsncmp(bstrConnectionString, L"FILE NAME=", 10) == 0)
		hr = source.OpenFromFileName(bstrConnectionString + 10);
	else
		hr = source.OpenFromInitializationString(bstrConnectionString);

	if (hr != S_OK)
		return;

	CComPtr<ICxDataSource> spCxDataSource;
	hr = source.m_spInit->QueryInterface(IID_ICxDataSource, (void**)&spCxDataSource);
	if (spCxDataSource !=NULL)
	{
		BOOL b = TRUE;
		spCxDataSource->SetUseLocalSetup(&b);
	}

	CSession session;
	hr = session.Open(source);
	if (hr != S_OK)
		return;
	
	int nSel = m_cmbCommandType.GetCurSel();
	// 表格
	if (nSel == 2)
	{
		CTables* pTableSet = new CTables;

		// (can simply hard code max size)
		char lpszType[64];
		
		// Set up type type to be retrieved
		strcpy(lpszType, "TABLE");

		if (pTableSet->Open(session, NULL, NULL, NULL, lpszType) == S_OK)
		{
			while(pTableSet->MoveNext() == S_OK)
			{
				// We do not handle multi-word tables or procedures.  Normally,
				// only Access handles these.
				if (_tcschr(pTableSet->m_szName, _T(' ')) != NULL)
					continue;

				m_cmdDatabaseObject.AddString(pTableSet->m_szName);
			}
		}

		delete pTableSet;
		pTableSet = NULL;
	}
	// 存贮过程
	else if (nSel == 3)
	{
		CProcedures* pProcedureSet = new CProcedures;

		// Get all procedure names
		if (pProcedureSet->Open(session) == S_OK)
		{
			while(pProcedureSet->MoveNext() == S_OK)
			{
				// We do not handle multi-word tables or procedures.  Normally,
				// only Access handles these.
				if (_tcschr(pProcedureSet->m_szName, _T(' ')) != NULL)
					continue;
				
				m_cmdDatabaseObject.AddString(pProcedureSet->m_szName);
			}
		}
				
		delete pProcedureSet;
		pProcedureSet = NULL;
	}
}

LRESULT CDataRecordsetPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	CString strText;
	CComBSTR bstr;
	GetDlgItemText(IDC_BUILD_SQL, (BSTR&)bstr);
	strText = bstr;
	strText.Replace(_T("SuperCx"), COEMSetup::GetSystemName());
	SetDlgItemText(IDC_BUILD_SQL, strText);

	m_cmbCommandType.Attach(GetDlgItem(IDC_COMMAND_TYPE));
	m_cmdDatabaseObject.Attach(GetDlgItem(IDC_DATABASE_OBJECT));
	m_editCommandText.Attach(GetDlgItem(IDC_COMMAND_TEXT));
	m_btnSQLBuild.Attach(GetDlgItem(IDC_BUILD_SQL));

	m_cmbCommandType.AddString(_T("8 - 未知"));
	m_cmbCommandType.AddString(_T("1 - 文本"));
	m_cmbCommandType.AddString(_T("2 - 表格"));
	m_cmbCommandType.AddString(_T("4 - 存贮过程"));
	
	if (m_nObjects > 0)
	{
		CComQIPtr<ICxDataRecordset, &IID_ICxDataRecordset> pRecordset(m_ppUnk[0]);
		
		enumCommandType nCommandType;
		pRecordset->get_CommandType(&nCommandType);
		
		TCHAR szT[8];
		sprintf(szT, _T("%d"), nCommandType);
		m_cmbCommandType.SelectString(-1, szT);

		UpdateWindowsState();

		CComBSTR bstr;
		pRecordset->get_Source(&bstr);
		if (nCommandType == 2 || nCommandType == 4)
			m_cmdDatabaseObject.SetWindowText(OLE2T(bstr));
		else
			m_editCommandText.SetWindowText(OLE2T(bstr));

		CComBSTR bstrConnectionString;
		HRESULT hr = GetConnectionString(bstrConnectionString);
		if (hr != S_OK || 
			(wcsncmp(bstrConnectionString, L"Provider=CxDBProv.RealtimeData.1", 32) != 0 &&
			wcsncmp(bstrConnectionString, L"Provider=CxDBProv.HistoryData.1", 31) != 0))
		{
			m_btnSQLBuild.EnableWindow(FALSE);
		}
	}
	
	return 0;
}

STDMETHODIMP CDataRecordsetPage::Apply()
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<ICxDataRecordset, &IID_ICxDataRecordset> pRecordset(m_ppUnk[0]);
		
		enumCommandType nCommandType;
		int nSel = m_cmbCommandType.GetCurSel();

		CComBSTR bstrText;
		
		switch (nSel) 
		{
		case 0:
			nCommandType = CommandTypeUnknown;
			m_editCommandText.GetWindowText((BSTR&)bstrText);
			break;
		case 1:
			nCommandType = CommandTypeText;
			m_editCommandText.GetWindowText((BSTR&)bstrText);
			break;
		case 2:
			nCommandType = CommandTypeTable;
			m_cmdDatabaseObject.GetWindowText((BSTR&)bstrText);
			break;
		case 3:
			nCommandType = CommandTypeStoredProc;
			m_cmdDatabaseObject.GetWindowText((BSTR&)bstrText);
			break;
		default:
			nCommandType = CommandTypeUnknown;
		} 

		pRecordset->put_CommandType(nCommandType);
		pRecordset->put_Source(bstrText);
	}	
	
	return S_OK;
}

LRESULT CDataRecordsetPage::OnSelchangeCommandType(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdateWindowsState();

	return 0;
}

LRESULT CDataRecordsetPage::OnClickedBuildSql(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	CSQLBuildDlg dlg;
	GetConnectionString(dlg.m_bstrConnectionString);

	if (IDOK == dlg.DoModal())
		m_editCommandText.SetWindowText(OLE2T(dlg.m_bstrSQL));

	return 0;
}

