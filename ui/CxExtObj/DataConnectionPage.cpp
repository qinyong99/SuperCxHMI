// DataConnectionPage.cpp : Implementation of CDataConnectionPage
#include "stdafx.h"
#include "CxExtObj.h"
#include "DataConnectionPage.h"
#include "OEMSetup.h"

/////////////////////////////////////////////////////////////////////////////
// CDataConnectionPage

HRESULT CDataConnectionPage::PromptSourceFileName(LPOLESTR* ppwszSelectedFile)
{
	USES_CONVERSION;
	CComPtr<IDBPromptInitialize> spDBInit;
	
	HRESULT hr = CoCreateInstance(CLSID_DataLinks, NULL, CLSCTX_INPROC_SERVER,
		IID_IDBPromptInitialize, (void**) &spDBInit);
	if (FAILED(hr))
		return hr;
	
	hr = spDBInit->PromptFileName(m_hWnd, DBPROMPTOPTIONS_NONE, NULL, L"*.udl", ppwszSelectedFile);
	
	return hr;
}

HRESULT CDataConnectionPage::PromptSource(LPOLESTR* ppwszInitString)
{
	CComPtr<IDBPromptInitialize> spDBInit;
	
	HRESULT hr = CoCreateInstance(CLSID_DataLinks, NULL, CLSCTX_INPROC_SERVER,
		IID_IDBPromptInitialize, (void**) &spDBInit);
	if (FAILED(hr))
		return hr;
	
	CComPtr<IDBProperties> spIDBProperties;
	hr = spDBInit->PromptDataSource(NULL, m_hWnd, DBPROMPTOPTIONS_WIZARDSHEET, 0, NULL, NULL,
		IID_IDBProperties, (IUnknown**)&spIDBProperties);

	if (hr == S_OK)
	{
		CComPtr<IDataInitialize> spDataInit;
		
		HRESULT hr = CoCreateInstance(CLSID_MSDAINITIALIZE, NULL, CLSCTX_INPROC_SERVER,
			IID_IDataInitialize, (void**)&spDataInit);
		if (FAILED(hr))
			return hr;
		
		hr = spDataInit->GetInitializationString(spIDBProperties, TRUE, ppwszInitString);
	}
	else if (hr == S_FALSE)
		hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_CANCELLED);  // The user clicked cancel
	
	return hr;
}

LRESULT CDataConnectionPage::OnClickedBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	LPOLESTR szSelected;
	if (PromptSourceFileName(&szSelected) == S_OK)
		SetDlgItemText(IDC_FILE_NAME, OLE2T(szSelected));
	
	return 0;
}

LRESULT CDataConnectionPage::OnClickedBuilder(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	LPOLESTR szInitString;
	if (PromptSource(&szInitString) == S_OK)
		SetDlgItemText(IDC_CONNECTION_STRING, OLE2T(szInitString));

	return 0;
}

LRESULT CDataConnectionPage::OnClickedRadio1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdateWindowsState();

	return 0;	
}

LRESULT CDataConnectionPage::OnClickedRadio2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdateWindowsState();

	return 0;	
}

LRESULT CDataConnectionPage::OnClickedRadio3(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdateWindowsState();
	
	return 0;
}

LRESULT CDataConnectionPage::OnClickedRadio4(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdateWindowsState();
	
	return 0;
}

void CDataConnectionPage::UpdateWindowsState()
{
	BOOL b = IsDlgButtonChecked(IDC_RADIO1);
	::EnableWindow(GetDlgItem(IDC_FILE_NAME), b);
	::EnableWindow(GetDlgItem(IDC_BROWSE), b);

	b = IsDlgButtonChecked(IDC_RADIO2);
	::EnableWindow(GetDlgItem(IDC_CONNECTION_STRING), b);
	::EnableWindow(GetDlgItem(IDC_BUILDE), b);
}

//const WCHAR szSuperCxRealtimeConnectionString[] = L"Provider=CxDBProv.RealtimeData.1";
//const WCHAR szSuperCxHistoryConnectionString[] = L"Provider=CxDBProv.HistoryData.1";
//const WCHAR szSuperCxRealtimeConnectionString[] = L"Provider=CxDBProv.RealtimeData.1;Persist Security Info=False;User ID=\"\";Data Source=\"\";Extended Properties=\"\"";
//const WCHAR szSuperCxHistoryConnectionString[] = L"Provider=CxDBProv.HistoryData.1;Persist Security Info=False;User ID=\"\";Data Source=\"\";Extended Properties=\"\"";

LRESULT CDataConnectionPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	CString strText;
	CComBSTR bstr;
	GetDlgItemText(IDC_RADIO3, (BSTR&)bstr);
	strText = bstr;
	strText.Replace(_T("SuperCx"), COEMSetup::GetSystemName());
	SetDlgItemText(IDC_RADIO3, strText);

	GetDlgItemText(IDC_RADIO4, (BSTR&)bstr);
	strText = bstr;
	strText.Replace(_T("SuperCx"), COEMSetup::GetSystemName());
	SetDlgItemText(IDC_RADIO4, strText);
	
	if (m_nObjects > 0)
	{
		CComQIPtr<ICxDataConnection, &IID_ICxDataConnection> pConnection(m_ppUnk[0]);

		CComBSTR bstrConnectionString;
		pConnection->get_ConnectionString(&bstrConnectionString);

		int nMode = 3;
		if (wcsncmp(bstrConnectionString, L"FILE NAME=", 10) == 0)
		{
			nMode = 0;
		}
		else if (bstrConnectionString.Length() > 0) 
		{
			if (wcsstr(bstrConnectionString, szSuperCxRealtimeConnectionString) != NULL)
				nMode = 2;
			else if (wcsstr(bstrConnectionString, szSuperCxHistoryConnectionString) == NULL)
				nMode = 1;
		}

		switch (nMode) 
		{
		case 0:
			SetDlgItemText(IDC_FILE_NAME, OLE2T(bstrConnectionString + 10));
			break;
		case 1:
			SetDlgItemText(IDC_CONNECTION_STRING, OLE2T(bstrConnectionString));
			break;
		default:
			break;
		}

		::CheckRadioButton(m_hWnd, IDC_RADIO1, IDC_RADIO4, IDC_RADIO1 + nMode);
		UpdateWindowsState();
	}
	
	return 0;
}

STDMETHODIMP CDataConnectionPage::Apply()
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<ICxDataConnection, &IID_ICxDataConnection> pConnection(m_ppUnk[0]);

		CComBSTR bstrConnectionString;
		CComBSTR bstrText;

		if (IsDlgButtonChecked(IDC_RADIO1))
		{
			bstrConnectionString = L"FILE NAME=";
			GetDlgItemText(IDC_FILE_NAME, (BSTR&)bstrText);
		}
		else if (IsDlgButtonChecked(IDC_RADIO2))
		{
			GetDlgItemText(IDC_CONNECTION_STRING, (BSTR&)bstrText);
		}
		else if (IsDlgButtonChecked(IDC_RADIO3))
		{
			bstrText = szSuperCxRealtimeConnectionString;
		}
		else
		{
			bstrText = szSuperCxHistoryConnectionString;
		}

		bstrConnectionString += bstrText;

		pConnection->put_ConnectionString(bstrConnectionString);
	}

	return S_OK;
}

