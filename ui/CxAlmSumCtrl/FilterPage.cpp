// FilterPage.cpp : Implementation of CFilterPage
#include "stdafx.h"
#include "CxAlmSumCtrl.h"
#include "NetNode.h"
#include "FilterPage.h"

/////////////////////////////////////////////////////////////////////////////
// CFilterPage

LRESULT CFilterPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_listNode.SubclassWindow(GetDlgItem(IDC_NODE));
	m_listNode.SetExtendedListViewStyle(m_listNode.GetExtendedLVStyle() | LVS_EX_CHECKBOXES);
	m_listArea.SubclassWindow(GetDlgItem(IDC_AREA));
	m_listArea.SetExtendedListViewStyle(m_listArea.GetExtendedLVStyle() | LVS_EX_CHECKBOXES);

	m_bLoadPropery = TRUE;

	if (m_nObjects > 0)
	{
		CComQIPtr<ICxAlmSum, &IID_ICxAlmSum> spAlmSum(m_ppUnk[0]);

		VARIANT_BOOL b;
		spAlmSum->get_RunMode(&b);
		m_bRunMode = (b != VARIANT_FALSE);
//#ifdef _DEBUG
//		m_bRunMode = TRUE;
//#endif

		LoadNetNodeList();
		
		LoadAreaList();
		CComBSTR bstrNodeFilter;
		spAlmSum->get_NodeFilterList(&bstrNodeFilter);
	
		for (int i = 0; i < m_listNode.GetItemCount(); i++)
		{
			CComBSTR bstrText;
			m_listNode.GetItemText(i, 0, (BSTR&)bstrText);
			if (StrInStrlist(bstrNodeFilter, bstrText))
				m_listNode.SetCheckState(i, TRUE);
		}

		CComBSTR bstrAreaFilter;
		spAlmSum->get_AreaFilterList(&bstrAreaFilter);
		
		for (i = 0; i < m_listArea.GetItemCount(); i++)
		{
			CComBSTR bstrText;
			m_listArea.GetItemText(i, 0, (BSTR&)bstrText);
			if (StrInStrlist(bstrAreaFilter, bstrText))
				m_listArea.SetCheckState(i, TRUE);
		}

		CComBSTR bstrPriorityFilter;
		spAlmSum->get_PriorityFilterList(&bstrPriorityFilter);

		if (StrInStrlist(bstrPriorityFilter, L"低"))
			CheckDlgButton(IDC_LOW, BST_CHECKED);
		if (StrInStrlist(bstrPriorityFilter, L"中"))
			CheckDlgButton(IDC_MIDDLE, BST_CHECKED);
		if (StrInStrlist(bstrPriorityFilter, L"高"))
			CheckDlgButton(IDC_HIGH, BST_CHECKED);
	}

	m_bLoadPropery = FALSE;

	return 0;
}

STDMETHODIMP CFilterPage::Apply(void)
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<ICxAlmSum, &IID_ICxAlmSum> spAlmSum(m_ppUnk[i]);

		// 节点
		CComBSTR bstrNodeFilter;
		BOOL bAllCheck = TRUE;
		for (int j = 0; j < m_listNode.GetItemCount(); j++)
		{
			if (m_listNode.GetCheckState(j))
			{	
				CComBSTR bstrText;
				m_listNode.GetItemText(j, 0, (BSTR&)bstrText);

				if (bstrNodeFilter != NULL)
					bstrNodeFilter += L" ";
				bstrNodeFilter += bstrText;
			}
			else
			{
				bAllCheck = FALSE;
			}
		}

		if (bAllCheck)
			bstrNodeFilter = L"*";

		spAlmSum->put_NodeFilterList(bstrNodeFilter);

		// 报警区
		CComBSTR bstrAreaFilter;
		bAllCheck = TRUE;
		for (j = 0; j < m_listArea.GetItemCount(); j++)
		{
			if (m_listArea.GetCheckState(j))
			{	
				CComBSTR bstrText;
				m_listArea.GetItemText(j, 0, (BSTR&)bstrText);
				
				if (bstrAreaFilter != NULL)
					bstrAreaFilter += L" ";
				bstrAreaFilter += bstrText;
			}
			else
			{
				bAllCheck = FALSE;
			}
		}
		
		if (bAllCheck)
			bstrAreaFilter = L"*";
		
		spAlmSum->put_AreaFilterList(bstrAreaFilter);

		// 优先级
		CComBSTR bstrPriorityFilter;
		bAllCheck = TRUE;

		if (IsDlgButtonChecked(IDC_LOW))
		{
			if (bstrPriorityFilter != NULL)
				bstrPriorityFilter += L" ";
			bstrPriorityFilter += L"低";
		}
		else
		{
			bAllCheck = FALSE;
		}
		if (IsDlgButtonChecked(IDC_MIDDLE))
		{
			if (bstrPriorityFilter != NULL)
				bstrPriorityFilter += L" ";
			bstrPriorityFilter += L"中";
		}
		else
		{
			bAllCheck = FALSE;
		}
		if (IsDlgButtonChecked(IDC_HIGH))
		{
			if (bstrPriorityFilter != NULL)
				bstrPriorityFilter += L" ";
			bstrPriorityFilter += L"高";
		}
		else
		{
			bAllCheck = FALSE;
		}
		
		if (bAllCheck)
			bstrPriorityFilter = L"*";
		
		spAlmSum->put_PriorityFilterList(bstrPriorityFilter);
		
	}

	m_bDirty = FALSE;

	return S_OK;
}

const CLSID CLSID_CxNodeAccess = {0x7672914C,0x79C8,0x48C5,{0x8B,0x61,0xD8,0x34,0x8E,0x4F,0x3E,0x9F}};

HRESULT CFilterPage::EnumNetNodesFormSetup()
{
	CComPtr<ICxDataAccess> spNodeAccess;
	
	HRESULT hr = spNodeAccess.CoCreateInstance(CLSID_CxNodeAccess);
	if (FAILED(hr))
		return hr;
	
	hr = spNodeAccess->Open(OPENMODE_READ);
	if (FAILED(hr))
	{
		CNetNode* pThisNode = new CComObject<CNetNode>;
		pThisNode->m_bstrName = L"ThisNode";
		m_apNetNodes.Add(pThisNode);
		pThisNode->AddRef();
		
		return S_OK;
	}
	
	long lCount;
	CNodeData data;
	spNodeAccess->GetCount(&lCount);
	for (int i = 0; i < lCount; i++)
	{
		CComPtr<ICxDataRecord> spNode;
		spNodeAccess->GetRecord(i, &spNode);
		spNode->GetData(long(&data));
		CComBSTR bstrName(50, data.szName);
		CComBSTR bstrComputer(50, data.szComputer);
		
		if (i == 0) 
			bstrComputer.Empty();

		CNetNode* pNode = new CComObject<CNetNode>;
		pNode->m_bstrName = bstrName;
 		pNode->m_bstrComputer = bstrComputer;
		m_apNetNodes.Add(pNode);
		pNode->AddRef();
	}
	
	spNodeAccess->Close();
	
	return S_OK;
}

const CLSID CLSID_CxAlarmAreaAccess = {0xF9ED2DBC,0x8B78,0x4DBB,{0xA4,0x78,0xCE,0x76,0x83,0xEA,0x78,0x58}};

HRESULT CFilterPage::EnumLocalAlarmAreasFormSetup()
{
	CComPtr<ICxDataAccess> spAlarmAreaAccess;
	HRESULT hr = spAlarmAreaAccess.CoCreateInstance(CLSID_CxAlarmAreaAccess);
	if (FAILED(hr))
		return hr;
	
	hr = spAlarmAreaAccess->Open(OPENMODE_READ);
	if (FAILED(hr))
		return hr;
	
	long lCount;
	CAlarmAreaData data;
	spAlarmAreaAccess->GetCount(&lCount);
	for (int i = 0; i < lCount; i++)
	{
		CComPtr<ICxDataRecord> spArea;
		spAlarmAreaAccess->GetRecord(i, &spArea);
		spArea->GetData(long(&data));
		CComBSTR bstr = data.szName;
		m_arrAlarmAreas.Add(bstr);
	}
	
	spAlarmAreaAccess->Close();
	
	return S_OK;
}

HRESULT CFilterPage::EnumNetNodes()
{
	if (!m_bRunMode)
		return EnumNetNodesFormSetup();

	HRESULT hr;
	if (m_spDataSession == NULL)
	{
		hr = m_spDataSession.CoCreateInstance(CLSID_CxDataSession);
		if (FAILED(hr))
			return hr;
	}

	DWORD dwCount;
	LPWSTR*	pNames;
	hr = m_spDataSession->GetNetNodes(&dwCount, &pNames);
	if (FAILED(hr))
		return hr;

	for (DWORD dw = 0; dw < dwCount; dw++)
	{
		CNetNode* pNode = new CComObject<CNetNode>;
		pNode->m_bstrName = pNames[dw];
//		pNode->m_bstrComputer = szComputer;
		m_apNetNodes.Add(pNode);
		pNode->AddRef();

		if (pNames[dw] != NULL)
			CoTaskMemFree(pNames[dw]);
	}

	CoTaskMemFree(pNames);

	return S_OK;
}

HRESULT CFilterPage::EnumAlarmAreas()
{
	LPOLESTR szArea;
	HRESULT hr;
	
	for (int i = 0; i < m_apNetNodes.GetSize(); i++)
	{
		// 本地节点使用本地配置
		if (i == 0 && !m_bRunMode)
		{
			EnumLocalAlarmAreasFormSetup();
			continue;
		}

		if (m_spDataSession == NULL)
		{
			hr = m_spDataSession.CoCreateInstance(CLSID_CxDataSession);
			if (FAILED(hr))
				return hr;
		}

		CComPtr<IEnumString> spEnumString;

		CComPtr<ICxDataServer> spDataServer;

		if (m_bRunMode)
		{
			hr = m_spDataSession->GetNodeServer(m_apNetNodes[i]->m_bstrName, IID_ICxDataServer, (IUnknown **)&spDataServer);
			if (FAILED(hr))
				continue;
		}
		else
		{
			spDataServer = m_apNetNodes[i]->GetDataServer();
			if (spDataServer == NULL)
				continue;
		}

		hr = spDataServer->BrowseEventAreas(&spEnumString);
		if (FAILED(hr))
			continue;

		while (spEnumString->Next(1, &szArea, NULL) == S_OK)
		{
			CComBSTR bstr = szArea;
			
			BOOL bUnique = TRUE;
			for (int j = 0; j < m_arrAlarmAreas.GetSize(); j++)
			{
				if (lstrcmpiW(m_arrAlarmAreas[j], bstr) == 0)
				{
					bUnique = FALSE; 
					break;
				}
			}
			if (bUnique)
				m_arrAlarmAreas.Add(bstr);
		}
	}

	return S_OK;
}

HRESULT CFilterPage::LoadNetNodeList()
{
	USES_CONVERSION;

	EnumNetNodes();
	
	m_listNode.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 120, -1);

	for (int i = 0; i < m_apNetNodes.GetSize(); i++)
	{
		m_listNode.InsertItem(i, OLE2T(m_apNetNodes[i]->m_bstrName));
	}
	
	return S_OK;
}

HRESULT CFilterPage::LoadAreaList()
{
	USES_CONVERSION;

	EnumAlarmAreas();

	m_listArea.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 120, -1);

	for (int i = 0; i < m_arrAlarmAreas.GetSize(); i++)
	{
		m_listArea.InsertItem(i, OLE2T(m_arrAlarmAreas[i]));
	}

	return S_OK;
}

LRESULT CFilterPage::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	for (int i = 0; i < m_apNetNodes.GetSize(); i++)
	{
		m_apNetNodes[i]->Release();
	}

	return 0;
}

LRESULT CFilterPage::OnItemchangedArea(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if (!m_bLoadPropery)
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);

	return 0;
}
LRESULT CFilterPage::OnClickedHigh(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);

	return 0;
}
LRESULT CFilterPage::OnClickedLow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);

	return 0;
}
LRESULT CFilterPage::OnClickedMiddle(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);

	return 0;
}
LRESULT CFilterPage::OnItemchangedNode(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if (!m_bLoadPropery)
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);

	return 0;
}