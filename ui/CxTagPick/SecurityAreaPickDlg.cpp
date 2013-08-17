// SecurityAreaPickDlg.cpp: implementation of the CSecurityAreaPickDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SecurityAreaPickDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSecurityAreaPickDlg::CSecurityAreaPickDlg(BOOL bUseLocalSetup)
{
	m_bUseLocalSetup = bUseLocalSetup;
}

CSecurityAreaPickDlg::~CSecurityAreaPickDlg()
{

}

BOOL StrInStrlist(LPCTSTR lpszList, LPCTSTR lpszSub)
{
	if (lpszList == NULL)
		return FALSE;
	
	LPCTSTR p1;
	LPCTSTR p2;
	p1 = lpszList;
	
	for (;;)
	{
		while (*p1 == ' ')
		{
			p1++;
		}
		p2 = p1;
		
		while (__iscsym(*p2))
			p2++;
		
		p2++;
		
		TCHAR szT[128];
		lstrcpyn(szT, p1, p2 - p1);
		if (lstrcmpi(szT, lpszSub) == 0)
			return TRUE;
		
		if (*p2 == '\0')
			return FALSE;
		
		p1 = p2;
	}
	
	return FALSE;
}

LRESULT CSecurityAreaPickDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	m_listArea.SubclassWindow(GetDlgItem(IDC_AREA));
	m_listArea.SetExtendedListViewStyle(m_listArea.GetExtendedLVStyle() | LVS_EX_CHECKBOXES);
	
	if (m_bUseLocalSetup)
		LoadSecurityAreaFormSetup();
	else
		LoadSecurityAreaFormServer();

	m_listArea.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 120, -1);
	
	for (int i = 0; i < m_arrAreas.GetSize(); i++)
	{
		CString strArea = OLE2T(m_arrAreas[i]);
		m_listArea.InsertItem(i, strArea);

		if (StrInStrlist(m_strPickArea, strArea))
			m_listArea.SetCheckState(i, TRUE);
	}
	
	return 1;
}

LRESULT CSecurityAreaPickDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_strPickArea = "";

	int nCount = m_listArea.GetItemCount();
	TCHAR szText[256];
	for (int i = 0; i < nCount; i++)
	{
		if (m_listArea.GetCheckState(i))
		{
			m_listArea.GetItemText(i, 0, szText, 256);
			
			if (!m_strPickArea.IsEmpty())
				m_strPickArea += _T(", ");
			m_strPickArea += szText;
		}
	}
	
	EndDialog(wID);
	return 0;	
}

LRESULT CSecurityAreaPickDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

ICxDataServer* CSecurityAreaPickDlg::GetDataServer()
{
	HRESULT hr;
	
	if (m_spDataServer == NULL)
	{
		//本地节点
		if (!m_bstrComputer)
		{
			hr = m_spDataServer.CoCreateInstance(CLSID_CxDataServer);
		}
		//网络节点
		else
		{
			COSERVERINFO info;
			
			MULTI_QI qi;
			int result = 0;
			info.dwReserved1 = NULL;
			info.dwReserved2 = NULL;
			info.pAuthInfo   = NULL;
			info.pwszName	= m_bstrComputer;//L"128.128.10.193";//max
			
			qi.pIID = &IID_ICxDataServer;
			qi.hr	= 0;
			qi.pItf = NULL;
			
			hr = CoCreateInstanceEx(CLSID_CxDataServer,
				NULL,
				CLSCTX_REMOTE_SERVER,  
				&info,
				1,
				&qi);
			if (SUCCEEDED(qi.hr))
			{
				m_spDataServer = (ICxDataServer *)qi.pItf;
				qi.pItf->Release();
				
//				TurnOffAuthenticationOnThisInterface(m_spDataServer);
			}
		}
	}
	
	return m_spDataServer;
}

HRESULT CSecurityAreaPickDlg::LoadSecurityAreaFormServer()
{
	GetDataServer();
	if (m_spDataServer == NULL)
		return E_FAIL;
	
	CComPtr<IEnumString> spEnumString;
	
	HRESULT hr = m_spDataServer->BrowseSecurityAreas(&spEnumString);
	if (FAILED(hr))
		return hr;
	
	LPOLESTR szArea;
	while (spEnumString->Next(1, &szArea, NULL) == S_OK)
	{
		CComBSTR bstr = szArea;
		m_arrAreas.Add(bstr);
		
		CoTaskMemFree(szArea);
	}

	return S_OK;
}

const CLSID CLSID_CxSecurityAreaAccess = {0x6B529F75,0x3406,0x4926,{0x89,0xA1,0x91,0x8B,0xF4,0x71,0x8B,0x88}};

HRESULT CSecurityAreaPickDlg::LoadSecurityAreaFormSetup()
{
	CComPtr<ICxDataAccess> spSecurityAreaAccess;
	HRESULT hr = spSecurityAreaAccess.CoCreateInstance(CLSID_CxSecurityAreaAccess);
	if (FAILED(hr))
		return hr;
	
	hr = spSecurityAreaAccess->Open(OPENMODE_READ);
	if (FAILED(hr))
		return hr;

	long lCount;
	CAlarmAreaData data;
	spSecurityAreaAccess->GetCount(&lCount);
	for (int i = 0; i < lCount; i++)
	{
		CComPtr<ICxDataRecord> spArea;
		spSecurityAreaAccess->GetRecord(i, &spArea);
		spArea->GetData(long(&data));
		CComBSTR bstr = data.szName;
		m_arrAreas.Add(bstr);
	}

	spSecurityAreaAccess->Close();

	return S_OK;
}

LRESULT CSecurityAreaPickDlg::OnClickedSelectAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nCount = m_listArea.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		m_listArea.SetCheckState(i, TRUE);
	}

	return 0;
}
LRESULT CSecurityAreaPickDlg::OnClickedSelectNone(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nCount = m_listArea.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		m_listArea.SetCheckState(i, FALSE);
	}

	return 0;
}
