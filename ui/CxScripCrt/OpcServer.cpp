// OPCServer.cpp: implementation of the COPCServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OPCServer.h"
#include "../../drv/opc/opcda_i.c"

HRESULT TurnOffAuthenticationOnThisInterface( IUnknown* pUnk )
{
    IClientSecurity* pcs;
    HRESULT hr = pUnk->QueryInterface( IID_IClientSecurity, (void**)&pcs );
    if ( FAILED( hr ) )
        return hr;
	
    // Use QueryBlanket to obtain all the other settings that
    // we want to leave the same (don't just call SetBlanket blindly!)
    // Also, since we are turning *off* authentication, we don't care about
    // the RPC_AUTH_IDENTITY_HANDLE parameter.
    DWORD dwAuthnSvc, dwAuthzSvc, dwImpLevel, grfCaps;
    OLECHAR* pszServerPrincipal;
    hr = pcs->QueryBlanket( pUnk, &dwAuthnSvc, &dwAuthzSvc,
		&pszServerPrincipal,
		0,
		&dwImpLevel, 0, &grfCaps );
	
    // Important note: SetBlanket on the proxy manager (IUnknown) only
    //                 controls the settings for QueryInterface in NT4SP3.
    //                 In NT4SP4 and NT5, this will control Release as well.
    if ( SUCCEEDED( hr ) )
        hr = pcs->SetBlanket( pUnk, dwAuthnSvc, dwAuthzSvc,
		pszServerPrincipal,
		RPC_C_AUTHN_LEVEL_NONE,
		dwImpLevel, 0, grfCaps );
    pcs->Release();
	
    return hr;
}

//////////////////////////////////////////////////////////////////////
// COPCItem
//////////////////////////////////////////////////////////////////////

COPCItem::COPCItem()
{
	m_pParentGroup = NULL;
	m_pServer = NULL;
}

COPCItem::~COPCItem()
{
	
}

//////////////////////////////////////////////////////////////////////
// COPCGroup
//////////////////////////////////////////////////////////////////////

COPCGroup::COPCGroup(COPCServer* pServer)
{
	m_pServer = pServer;
	m_pParentGroup = NULL;
	m_bstrItemID = L"";

	m_bRefreshGroups = FALSE;
	m_bRefreshItems = FALSE;
}

COPCGroup::~COPCGroup()
{
	
}

CSimpleArray<COPCGroup *>& COPCGroup::GetGroups()
{
	if (!m_bRefreshGroups)
		RefreshGroups();
	
	return m_arrGroups;
}

CSimpleArray<COPCItem *>& COPCGroup::GetItems()
{
	if (!m_bRefreshItems)
		RefreshItems();
	
	return m_arrItems;
}

void COPCGroup::RefreshGroups()
{
	HRESULT hr;

	ATLASSERT(m_pServer != NULL);

	if (m_bRefreshGroups)
		return;
	
	m_bRefreshGroups = TRUE;
	
	if (m_pServer->GetBrowseServerAddressSpace() == NULL)
		return;

//	OPCNAMESPACETYPE OPCNameSpaceType;
//	hr = m_pServer->m_spOPCBrowseServerAddressSpace->QueryOrganization(&OPCNameSpaceType);
//	if (FAILED(hr))
//		return;
//	if (OPCNameSpaceType != OPC_NS_HIERARCHIAL)
//		return;

	CComPtr<IEnumString> spEnumString;
	
	hr = m_pServer->m_spOPCBrowseServerAddressSpace->ChangeBrowsePosition(
			OPC_BROWSE_TO, m_bstrItemID);
	if (FAILED(hr))
		return;

	hr = m_pServer->m_spOPCBrowseServerAddressSpace->BrowseOPCItemIDs(
			OPC_BRANCH, L"", VT_EMPTY, OPC_READABLE | OPC_WRITEABLE, &spEnumString);
	if (FAILED(hr))
		return;
	
	LPOLESTR pszItem;
	while (spEnumString->Next(1, &pszItem, NULL) == S_OK)
	{
		COPCGroup* pGroup = new COPCGroup;
		pGroup->m_bstrName = pszItem;
		pGroup->m_pServer = m_pServer;
		
		LPWSTR szItemID = NULL;
		m_pServer->m_spOPCBrowseServerAddressSpace->GetItemID(pGroup->m_bstrName, &szItemID);
		pGroup->m_bstrItemID = szItemID;
		if (szItemID != NULL)
			CoTaskMemFree(szItemID);
		
		m_arrGroups.Add(pGroup);
	}
}

void COPCGroup::RefreshItems()
{
	ATLASSERT(m_pServer != NULL);
	
	if (m_bRefreshItems)
		return;
	
	m_bRefreshItems = TRUE;
	
	if (m_pServer->GetBrowseServerAddressSpace() == NULL)
		return;

//	OPCNAMESPACETYPE OPCNameSpaceType;
//	HRESULT hr = m_pServer->m_spOPCBrowseServerAddressSpace->QueryOrganization(&OPCNameSpaceType);
//	if (FAILED(hr))
//		return;
//	if (OPCNameSpaceType != OPC_NS_HIERARCHIAL)
//		return;
	
	CComPtr<IEnumString> spEnumString;
	
	HRESULT hr = m_pServer->m_spOPCBrowseServerAddressSpace->BrowseOPCItemIDs(
		OPC_LEAF, L"", VT_EMPTY, OPC_READABLE | OPC_WRITEABLE, &spEnumString);
	if (FAILED(hr))
		return;
	
	LPOLESTR pszItem;
	while (spEnumString->Next(1, &pszItem, NULL) == S_OK)
	{
		COPCItem* pItem = new COPCItem;
		pItem->m_bstrName = pszItem;
		
		m_arrItems.Add(pItem);
	}
}

void COPCGroup::RemoveAll()
{
	//删除子节点
	int nCount = m_arrGroups.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		m_arrGroups[i]->RemoveAll();
		delete m_arrGroups[i];
	}
	m_arrGroups.RemoveAll();

	nCount = m_arrItems.GetSize();
	for (i = 0; i < nCount; i++)
	{
		delete m_arrItems[i];
	}
	m_arrItems.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
// COPCServer
//////////////////////////////////////////////////////////////////////

COPCServer::COPCServer() :
	m_group(this)
{
	
}

COPCServer::~COPCServer()
{
	RemoveAll();
}

IOPCServer* COPCServer::GetOPCServer()
{
	HRESULT hr;

	if (m_spOPCServer == NULL)
	{
		CLSID CLSID_OPCServer;
		
		hr = CLSIDFromString(m_bstrOPCServerName, &CLSID_OPCServer);
		if (FAILED(hr))
			return NULL;

		//本地节点
		if (!m_bstrComputer || m_bstrComputer == L"")
		{
			hr = m_spOPCServer.CoCreateInstance(CLSID_OPCServer);
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
			
			qi.pIID = &IID_IOPCServer;
			qi.hr	= 0;
			qi.pItf = NULL;
			
			hr = CoCreateInstanceEx(CLSID_OPCServer,
				NULL,
				CLSCTX_REMOTE_SERVER,  
				&info,
				1,
				&qi);
			if (SUCCEEDED(qi.hr))
			{
				m_spOPCServer = (IOPCServer *)qi.pItf;
				qi.pItf->Release();
			}
		}
	}
	
	return m_spOPCServer;
}

IOPCBrowseServerAddressSpace* COPCServer::GetBrowseServerAddressSpace()
{
	//先创建服务器
	if (GetOPCServer() == NULL)
		return NULL;

	if (m_spOPCBrowseServerAddressSpace == NULL)
	{
		HRESULT hr = m_spOPCServer->QueryInterface(IID_IOPCBrowseServerAddressSpace, 
			(void **)&m_spOPCBrowseServerAddressSpace);
		if (FAILED(hr))
			ATLASSERT(FALSE);

//		if (m_spOPCBrowseServerAddressSpace != NULL)
//			TurnOffAuthenticationOnThisInterface(m_spOPCBrowseServerAddressSpace);
	}
	
	return m_spOPCBrowseServerAddressSpace;
}

void COPCServer::RemoveAll()
{
	m_group.RemoveAll();
}
