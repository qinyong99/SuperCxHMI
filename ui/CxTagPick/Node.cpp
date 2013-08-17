// Node.cpp: implementation of the CNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Node.h"


static WCHAR szTags[] = L"Tags";

// SetBlanket.cpp - sample code for explicitly turning off authentication
//                  on various interfaces of a proxy.

// The following function uses QueryBlanket and SetBlanket to correctly
// drop the authentication level to NONE, without mucking up any other
// security settings.
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

// The following code snippet demonstrates that you need to explicitly
// use SetBlanket on each interface you want to make calls through,
// since security settings are stored separately for individual
// interface proxies.
// After calling this function, we can use both IUnknown and IFoo to make
// unauthenticated calls. If you needed to also make calls via IBar,
// just repeat for that interface as well.
//HRESULT TurnOffAuthenticationForMultipleInterfaces( IFoo* pFoo )
//{
//    // turn off authentication on the IFoo interface proxy
//    HRESULT hr = TurnOffAuthenticationOnThisInterface( pFoo );
//    
//    // turn off authentication for IUnknown (on the proxy manager)
//    IUnknown* pUnk;
//    hr = pFoo->QueryInterface( IID_IUnknown, (void**)&pUnk );
//    if ( SUCCEEDED( hr ) )
//    {
//        hr = TurnOffAuthenticationOnThisInterface( pUnk );
//        pUnk->Release();
//    }
//    return hr;


//////////////////////////////////////////////////////////////////////
// CTag
//////////////////////////////////////////////////////////////////////

CTag::CTag(CNode* pNode, LPCWSTR szName)
{
	m_pNode = pNode;
	m_bstrName = szName;
	m_bRefreshProps = FALSE;
	m_nTagType = -1;
}

CTag::~CTag()
{

}

CSimpleArray<CComBSTR>& CTag::GetProps()
{
	if (m_bRefreshProps == FALSE)
		RefreshProps();

	return m_arrProps;
}

const CLSID CLSID_AnalogBlock = {0x872D0536,0x027A,0x4F54,{0x8D,0x1C,0x66,0x8F,0x12,0x8A,0x75,0x4B}};
const CLSID CLSID_DigitalBlock = {0x9C039F35,0xB303,0x4BB5,{0xA5,0x0E,0x61,0x07,0x87,0x4E,0x1F,0x9D}};
const CLSID CLSID_TextBlock = {0x38C1CD13,0xF083,0x4CAD,{0x9E,0xA8,0xB6,0x23,0x39,0xCD,0x5C,0x6D}};

HRESULT CTag::LoadFieldsOfTagType()
{
	USES_CONVERSION;
	
	CLSID clsid;
	
	switch (m_nTagType)
	{
	case TAG_ANALOG:
		clsid = CLSID_AnalogBlock;
		break;
		
	case TAG_DIGITAL:
		clsid = CLSID_DigitalBlock;
		break;
		
	case TAG_TEXT:
		clsid = CLSID_TextBlock;
		break;
		
	default:
		ATLASSERT(FALSE);
	}
	
	CComPtr<IDispatch> spObject;
	HRESULT hr = spObject.CoCreateInstance(clsid);
	if (FAILED(hr)) 
		return hr;
	
	CComPtr<ITypeInfo> spTypeInfo;	
	hr = spObject->GetTypeInfo(0, LOCALE_SYSTEM_DEFAULT, &spTypeInfo);	
	if (FAILED(hr)) 
		return hr;
	if (spTypeInfo == NULL) 
		return E_UNEXPECTED;
	
	TYPEATTR* pta = 0;
	hr = spTypeInfo->GetTypeAttr(&pta);
	if (FAILED(hr)) 
		return hr;
	if (!pta) 
		return E_UNEXPECTED;
	for (int i = 0; i < pta->cFuncs; i++)
	{
		FUNCDESC* pfd = 0;
		spTypeInfo->GetFuncDesc(i, &pfd);
		
		if (pfd && (pfd->invkind == INVOKE_PROPERTYGET/* || pfd->invkind == INVOKE_PROPERTYPUT || pfd->invkind == INVOKE_PROPERTYPUTREF*/)
			&& ((pfd->wFuncFlags & (FUNCFLAG_FRESTRICTED | FUNCFLAG_FHIDDEN | FUNCFLAG_FNONBROWSABLE)) == 0))
		{	
			if(pfd->cParams != 0)
				continue;
			
			CComBSTR bstrName;
			spTypeInfo->GetDocumentation(pfd->memid, &bstrName, NULL, 0, 0);
			m_arrProps.Add(bstrName);
		}
		spTypeInfo->ReleaseFuncDesc(pfd);			
	} 
	
	spTypeInfo->ReleaseTypeAttr(pta);
	
	return S_OK;
}

void CTag::RefreshProps()
{
	ATLASSERT(m_pNode != NULL);

	if (m_nTagType == -1)
	{
		if (m_pNode->m_pNetNode != NULL)
		{
			ICxDataServer* pDataServer = m_pNode->m_pNetNode->GetDataServer();
			if (pDataServer->IsTagAvailableProperty() == S_OK)
			{
				CComBSTR bstrTag;
				bstrTag.m_str = GetQualityTag();
				DWORD dwCount = 0;
				LPWSTR* pNames;
				HRESULT hr = pDataServer->QueryTagAvailableProperties(bstrTag, 
						&dwCount, &pNames);
				for (DWORD dw = 0; dw < dwCount; dw++)
				{
					m_arrProps.Add(CComBSTR(pNames[dw]));
					CoTaskMemFree(pNames[dw]);
				}
				CoTaskMemFree(pNames);
			}
		}
	}
	else
	{
		LoadFieldsOfTagType();
	}
	
	m_bRefreshProps = TRUE;
}

BSTR CTag::GetQualityTag()
{
	CComBSTR bstrTagGroup;
	bstrTagGroup.m_str = m_pNode->GetQualityTagGroup();

	OLECHAR szTagGroup[MAX_TAG_NAME];
	memset(szTagGroup, 0, MAX_TAG_NAME);
	LPWSTR p = szTagGroup;
	int cb;
	if (bstrTagGroup)
	{
		cb = bstrTagGroup.Length();
		memcpy(p, bstrTagGroup, cb * 2);
		p += cb;
		*p = OLECHAR('.');
		p++;
	}
	
	cb = m_bstrName.Length();
	memcpy(p , m_bstrName, cb * 2);

	return SysAllocString(szTagGroup);
}

//////////////////////////////////////////////////////////////////////
// CNode
//////////////////////////////////////////////////////////////////////

CNode::CNode(LPCWSTR szName, CNetNode* pNetNode)
{
	m_pParentNode = NULL;
	m_bstrName = szName;
	m_pNetNode = pNetNode;

	m_bRefresh = FALSE;
	m_bRefreshTag = FALSE;

	m_dwID = 0;
}

CNode::~CNode()
{

}

inline CNode* CNode::FindChildNode(LPCWSTR szName)
{
	if (m_bRefresh == FALSE)
		RefreshChildren();

	CNode* pNode;
	int nCount = m_apChildNodes.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		pNode = m_apChildNodes[i];
		if (lstrcmpiW(pNode->m_bstrName, szName) == 0)
			return pNode;
	}

	return NULL;
}

inline CTag* CNode::FindTag(LPCWSTR szTag)
{
	if (m_bRefreshTag == FALSE)
		RefreshTags();

	CTag* pTag;
	int nCount = m_apTags.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		pTag = m_apTags[i];
		if (lstrcmpiW(pTag->m_bstrName, szTag) == 0)
			return pTag;
	}

	return NULL;
}

inline LPCWSTR NextToken(LPCWSTR szToken)
{
//	ATLASSERT(*szToken != NULL);
		
	while (*szToken != NULL)
	{
		if (*szToken == OLECHAR('.'))
			return szToken;

		szToken = CharNextW(szToken);
	}

	return szToken;
}

CNode* CNode::FindMatchNode(LPCWSTR szTagGroup)
{
	CNode* pNode = this;
	LPCWSTR szStart = szTagGroup;
	LPCWSTR szEnd;
	
	while (1)
	{
		if (*szStart == NULL)
			return pNode;
		szEnd = NextToken(szStart);
		
		CComBSTR bstrToken(szEnd - szStart, szStart);
		
		pNode = pNode->FindChildNode(bstrToken);
		if (pNode == NULL)
			return NULL;
		
		if (*szEnd == NULL)
			return pNode;
		szStart = szEnd + 1;
	}

	return pNode;
}

CNode* FindMatchNode(CNode* pNode, LPCWSTR szTagGroup)
{
	LPCWSTR szStart = szTagGroup;
	LPCWSTR szEnd;

	if (*szStart == NULL)
		return NULL;

	szEnd = NextToken(szStart);
	CComBSTR bstrToken(szEnd - szStart, szStart);

	if (lstrcmpiW(pNode->m_bstrName, bstrToken) == 0)
	{
		if (*szEnd == NULL)
			return pNode;

		szStart = szEnd + 1;
		return pNode->FindMatchNode(szStart);
	}

	return NULL;
}

CTag* CNode::FindMatchTag(LPCWSTR szTag)
{
	CNode* pNode = this;
	
	LPCWSTR szStart = szTag;
	LPCWSTR szEnd = NextToken(szStart);

	while (*szEnd != NULL)
	{
		CComBSTR bstrToken(szEnd - szStart, szStart);
		
		pNode = pNode->FindChildNode(bstrToken);
		if (pNode == NULL)
			return NULL;
		
		szStart = szEnd + 1;
		szEnd = NextToken(szStart);
	}

	CComBSTR bstrToken(szEnd - szStart, szStart);
	
	return pNode->FindTag(bstrToken);
}

CTag* FindMatchTag(CNode* pNode, LPCWSTR szTag)
{
	LPCWSTR szStart = szTag;
	LPCWSTR szEnd;

	if (*szStart == NULL)
		return NULL;

	szEnd = NextToken(szStart);
	CComBSTR bstrToken(szEnd - szStart, szStart);

	if (lstrcmpiW(pNode->m_bstrName, bstrToken) == 0)
	{
		if (*szEnd == NULL)
			return NULL;

		szStart = szEnd + 1;
		return pNode->FindMatchTag(szStart);
	}

	return NULL;
}

void CNode::AddChildNode(CNode* pNode)
{
	pNode->m_pParentNode = this;
	m_apChildNodes.Add(pNode);
}

void CNode::RefreshChildren()
{
	m_bRefresh = TRUE;
}

void CNode::RefreshTags()
{
	m_bRefreshTag = TRUE;
}

BSTR CNode::GetQualityTagGroup()
{
	if (GetNodeType() != nodeTag)
		return NULL;

	OLECHAR szTagGroup[MAX_TAG_NAME];
	LPWSTR p = szTagGroup + MAX_TAG_NAME - 1;
	*p = OLECHAR('\0');
	
	CNode* pNode = this;
	while (1)
	{
		int n = pNode->m_bstrName.Length();
		p -= n;
		if (p < szTagGroup)
			return NULL;
		memcpy(p, pNode->m_bstrName, n * 2);

		pNode = pNode->GetParent();
		if (pNode == NULL || pNode == m_pNetNode)
			break;

		if (--p < szTagGroup)
			return NULL;
		*p = OLECHAR('.');
	}

	return SysAllocString(p);
}

CSimpleArray<CNode *>& CNode::GetChildNodes()
{
	if (m_bRefresh == FALSE)
		RefreshChildren();

	return m_apChildNodes;
}

CSimpleArray<CTag *>& CNode::GetTags()
{
	if (m_bRefreshTag == FALSE)
		RefreshTags();

	return m_apTags;
}

void CNode::RemoveAll()
{
	//删除子节点
	int nCount = m_apChildNodes.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		m_apChildNodes[i]->RemoveAll();
		delete m_apChildNodes[i];
	}
	m_apChildNodes.RemoveAll();

	nCount = m_apTags.GetSize();
	for (i = 0; i < nCount; i++)
	{
		delete m_apTags[i];
	}
	m_apTags.RemoveAll();
}


//////////////////////////////////////////////////////////////////////
// CRootNode
//////////////////////////////////////////////////////////////////////

CRootNode::CRootNode(BOOL bUseLocalSetup)
{
	m_bstrName = szTags;
	m_bUseLocalSetup = bUseLocalSetup;
	m_pOnlyNetNode = NULL;
}

CRootNode::~CRootNode()
{
	if (m_pOnlyNetNode != NULL)
	{
		delete m_pOnlyNetNode;
		m_pOnlyNetNode = NULL;
	}
}

const CLSID CLSID_CxNodeAccess = {0x7672914C,0x79C8,0x48C5,{0x8B,0x61,0xD8,0x34,0x8E,0x4F,0x3E,0x9F}};

// 直接从配置文件中读取节点信息
HRESULT CRootNode::LoadChildrenFormSetup()
{
	CComPtr<ICxDataAccess> spNodeAccess;
	
	HRESULT hr = spNodeAccess.CoCreateInstance(CLSID_CxNodeAccess);
	if (FAILED(hr))
		return hr;
	
	hr = spNodeAccess->Open(OPENMODE_READ);
	if (FAILED(hr))
	{
		CNetNode* pNode = new CNetNode(L"ThisNode");
		AddChildNode(pNode);
		pNode->LoadLocalNodeFormSetup();

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
		CNetNode* pNode = new CNetNode(bstrName, bstrComputer);
		AddChildNode(pNode);

		if (!pNode->m_bstrComputer)
			pNode->LoadLocalNodeFormSetup();
	}
	
	spNodeAccess->Close();

	return S_OK;
}

HRESULT CRootNode::LoadChildrenFormServer()
{
	HRESULT hr;
	DWORD dwCount = 0;
	CXNETNODE* pNodes = NULL;
	
	//添加本地节点
	CNetNode* pThisNode = new CNetNode;
	
	ICxDataServer* pThisDataServer = pThisNode->GetDataServer();
	if (pThisDataServer == NULL)
		goto OnQuit;
	
	//列举网络节点
	hr = pThisDataServer->GetNetNodes(&dwCount, &pNodes);
	if (FAILED(hr))
		goto OnQuit;

	if (dwCount > 0)
	{
		for (DWORD dw = 0; dw < dwCount; dw++)
		{
			if (dw == 0)
			{
				pThisNode->m_bstrName = pNodes[dw].szNode;
				AddChildNode(pThisNode);
				pThisNode = NULL;
			}
			else
			{
				CNetNode* pNode = new CNetNode(pNodes[dw].szNode, pNodes[dw].szComputer);
				AddChildNode(pNode);
			}
			
			//释放字符串
			CoTaskMemFree(pNodes[dw].szNode);
			CoTaskMemFree(pNodes[dw].szComputer);
			CoTaskMemFree(pNodes[dw].szBackupComputer);
		}
	}
	else
	{
		pThisNode->m_bstrName = L"ThisNode";
		AddChildNode(pThisNode);
		pThisNode = NULL;
	}

	if (pNodes != NULL)
		CoTaskMemFree(pNodes);
	
OnQuit:
	if (pThisNode != NULL)
		delete pThisNode;

	return hr;
}

void CRootNode::RefreshChildren()
{
	CNode::RefreshChildren();

	if (m_pOnlyNetNode != NULL)
	{
		AddChildNode(m_pOnlyNetNode);
		m_pOnlyNetNode = NULL;
	}
	else
	{
		if (m_bUseLocalSetup)
			LoadChildrenFormSetup();
		else
			LoadChildrenFormServer();
	}
}

//////////////////////////////////////////////////////////////////////
// CNetNode
//////////////////////////////////////////////////////////////////////

CNetNode::CNetNode(LPCWSTR szName, LPCWSTR szComputer, BOOL bUseSACProxy, long lSACProxyPort) :
	CNode(szName, this)
{
	m_bstrComputer = szComputer;
	m_bUseSACProxy = bUseSACProxy;
	m_lSACProxyPort = lSACProxyPort;
}

CNetNode::~CNetNode()
{

}

void CNetNode::RefreshChildren()
{
	//先创建服务器
	if (GetBrowseServerAddressSpace() == NULL)
		return;

	HRESULT hr = m_spTagBrowser->ChangeBrowsePosition(CX_BROWSE_TO, L"");
	if (FAILED(hr))
		return;

	CComPtr<IEnumString> spEnumString;
	CComPtr<IEnumVARIANT> spEnumAttr;
	hr = m_spTagBrowser->BrowseItemIDs(CX_BRANCH, &spEnumString, &spEnumAttr);
	if (FAILED(hr))
		return;
	TurnOffAuthenticationOnThisInterface(spEnumString);

	LPOLESTR pszGroup;
	while (spEnumString->Next(1, &pszGroup, NULL) == S_OK)
	{
		CTagNode* pNode = new CTagNode(pszGroup, this);
		AddChildNode(pNode);

		CoTaskMemFree(pszGroup);
	}

	CNode::RefreshChildren();
}

void CNetNode::RefreshTags()
{
	//先创建服务器
	if (GetBrowseServerAddressSpace() == NULL)
		return;

	HRESULT hr = m_spTagBrowser->ChangeBrowsePosition(CX_BROWSE_TO, L"");
	if (FAILED(hr))
		return;

	CComPtr<IEnumString> spEnumString;
	CComPtr<IEnumVARIANT> spEnumAttr;
	hr = m_spTagBrowser->BrowseItemIDs(CX_LEAF, &spEnumString, &spEnumAttr);
	if (FAILED(hr))
		return;
	TurnOffAuthenticationOnThisInterface(spEnumString);

	LPOLESTR psz;
	VARIANT var;
	VariantInit(&var);

	while (spEnumString->Next(1, &psz, NULL) == S_OK)
	{
		CTag* pTag = new CTag(this, psz);
		m_apTags.Add(pTag);

		CoTaskMemFree(psz);
		
		spEnumAttr->Next(1, &var, NULL);
		pTag->m_bstrDesc = var.bstrVal;
		VariantClear(&var);

		spEnumAttr->Next(1, &var, NULL);
		pTag->m_nTagType = var.intVal;
		VariantClear(&var);

		spEnumAttr->Next(1, &var, NULL);
		pTag->m_bstrDevice = var.bstrVal;
		VariantClear(&var);

		spEnumAttr->Next(1, &var, NULL);
		pTag->m_bstrAddress = var.bstrVal;
		VariantClear(&var);
	}

	CNode::RefreshTags();
}

void GetLocalIP(CComBSTR& bstrIP)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	TCHAR szName[255];
	PHOSTENT pHost;
	wVersionRequested = MAKEWORD(2, 0);

	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if (gethostname(szName, sizeof(szName)) == 0)
		{
			pHost = gethostbyname(szName);
			for (int nAdapter = 0; pHost->h_addr_list[nAdapter] != NULL; nAdapter++)
			{
				sockaddr_in sa;
				memcpy(&sa.sin_addr.s_addr, pHost->h_addr_list[nAdapter], pHost->h_length);
				CComBSTR bstr = inet_ntoa(sa.sin_addr);
			
				if (bstrIP.Length() > 0)
					bstrIP.Append(L" ");

				bstrIP.Append(bstr);
			}
		}
		
		WSACleanup();
	}
}

BOOL CNetNode::Login(LPCWSTR szUser, LPCWSTR szPassword)
{
	static CComBSTR bstrLocalIP;

	if (GetDataServer() == NULL)
		return FALSE;

	if (bstrLocalIP.m_str == NULL)
		GetLocalIP(bstrLocalIP);

	HRESULT hr;
	BOOL bLocal = m_bstrComputer.Length() < 1;

	CComBSTR bstrIP = bLocal ? L"本地计算机" : bstrLocalIP;

	DWORD dwCookie;
	hr = m_spDataServer->UserLogin(szUser, 
			szPassword, bstrIP, bLocal, &dwCookie);

	return hr == S_OK;
}

const CLSID CLSID_CxSACProxy = {0x6F3CA3A3,0x6922,0x4001,{0xA8,0xFD,0x64,0x25,0x40,0x58,0x22,0x19}};

ICxDataServer* CNetNode::GetDataServer()
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
			if (m_bUseSACProxy)
			{
				CComPtr<IDispatch> spProxyObject;
				hr = spProxyObject.CoCreateInstance(CLSID_CxSACProxy);
				if (SUCCEEDED(hr))
				{
					CComDispatchDriver dd(spProxyObject);

					CComVariant var(m_bstrComputer);
					dd.PutProperty(2, &var);

					var = m_lSACProxyPort;
					dd.PutProperty(1, &var);

					var = false;
					dd.PutProperty(9, &var);

					CComVariant varRet;
					dd.Invoke0(6, &varRet);

					if (varRet.vt == VT_UNKNOWN)
						m_spDataServer = (ICxDataServer*)varRet.punkVal;
				}
			}

			// 使用代理不能成功连接时再使用DCOM连接
			if (m_spDataServer == NULL)
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

					TurnOffAuthenticationOnThisInterface(m_spDataServer);
				}
			}
		}

		if (m_bstrUserName.Length() > 0)
			Login(m_bstrUserName, m_bstrPassword);
	}

	return m_spDataServer;
}

const CLSID CLSID_CxTagAccess = {0x3A2BC483,0xD0EE,0x460A,{0x86,0xF4,0x7D,0x16,0x64,0xCC,0x90,0x92}};

void LoadTags(CNode* pParentNode, ICxDataAccess* pTagAccess)
{
	USES_CONVERSION;

	long lCount;
	pTagAccess->GetCount(&lCount);
	for (long i = 0; i < lCount; i++)
	{
		CComPtr<ICxDataRecord> spTag;
		pTagAccess->GetRecord(i, &spTag);
		
		CTagData data;
		spTag->GetData(long(&data));
		
		if (data.dwParentGroup == pParentNode->m_dwID)
		{
			CTag* pTag = new CTag(pParentNode, T2W(data.szName));
			pTag->m_bstrDesc = data.szDesc;
			pTag->m_nTagType = data.nType;
			pTag->m_bstrDevice = data.szDevice;
			pTag->m_bstrAddress = data.szAddress;
			pParentNode->m_apTags.Add(pTag);
		}
	}
}

void LoadTagNodes(CNode* pParentNode, ICxDataAccess* pTagGroupAccess, ICxDataAccess* pTagAccess)
{
	long lCount;
	pTagGroupAccess->GetCount(&lCount);
	for (long i = 0; i < lCount; i++)
	{
		CComPtr<ICxDataRecord> spTagGroup;
		pTagGroupAccess->GetRecord(i, &spTagGroup);
		
		CTagGroupData data;
		spTagGroup->GetData(long(&data));
		
		if (data.dwParentGroup == pParentNode->m_dwID)
		{
			CTagNode* pNode = new CTagNode();
			pNode->m_dwID = data.dwID;
			pNode->m_bstrName = data.szName;
			pParentNode->AddChildNode(pNode);
			
			LoadTagNodes(pNode, pTagGroupAccess, pTagAccess);

			pNode->m_bRefresh = TRUE;
			pNode->m_bRefreshTag = TRUE;
		}
	}
	
	LoadTags(pParentNode, pTagAccess);
}

HRESULT CNetNode::LoadLocalNodeFormSetup()
{
	m_bRefresh = TRUE;
	m_bRefreshTag = TRUE;

	HRESULT hr;
	CComPtr<ICxMutiDataAccess> spMutiDataAccess;
	
	hr = spMutiDataAccess.CoCreateInstance(CLSID_CxTagAccess);
	if (FAILED(hr))
		return hr;

	spMutiDataAccess->SetOpenMode(TRUE);
	
	CComPtr<ICxDataAccess> spTagGroupDataAccess;
	hr = spMutiDataAccess->GetDataAccess(L"TagGroup", &spTagGroupDataAccess);
	if (FAILED(hr))
		return hr;
	
	hr = spTagGroupDataAccess->Open(OPENMODE_READ);
	if (FAILED(hr))
		return hr;
	
	CComPtr<ICxDataAccess> spTagDataAccess;
	hr = spMutiDataAccess->GetDataAccess(L"Tag", &spTagDataAccess);
	if (FAILED(hr))
		return hr;
	
	hr = spTagDataAccess->Open(OPENMODE_READ);
	if (FAILED(hr))
		return hr;
	
	
	LoadTagNodes(this, spTagGroupDataAccess, spTagDataAccess);
	
	spTagGroupDataAccess->Close();
	
	spTagDataAccess->Close();

	return hr;
}

ICxBrowseServerAddressSpace* CNetNode::GetBrowseServerAddressSpace()
{
	//先创建服务器
	if (GetDataServer() == NULL)
		return NULL;
	
	if (m_spTagBrowser == NULL)
	{
		HRESULT hr = m_spDataServer->CreateTagBrowser(IID_ICxBrowseServerAddressSpace, 
			(IUnknown **)&m_spTagBrowser);
		if (FAILED(hr))
			ATLASSERT(FALSE);

		if (m_spTagBrowser != NULL)
			TurnOffAuthenticationOnThisInterface(m_spTagBrowser);
	}
	
	return m_spTagBrowser;
}


//////////////////////////////////////////////////////////////////////
// CTagNode
//////////////////////////////////////////////////////////////////////

CTagNode::CTagNode(LPCWSTR szName, CNetNode* pNetNode) :
	CNode(szName, pNetNode)
{
	m_bstrTagGroup = L"";
}

CTagNode::~CTagNode()
{

}

void CTagNode::RefreshChildren()
{
	ATLASSERT(m_pNetNode != NULL);

	ICxBrowseServerAddressSpace* pTagBrowser = m_pNetNode->GetBrowseServerAddressSpace();
	if (pTagBrowser == NULL)
		return;

	m_bstrTagGroup.Empty();
	m_bstrTagGroup.m_str = GetQualityTagGroup();
	
	if (!m_bstrTagGroup)
	{
		CNode::RefreshChildren();
		return;
	}

	HRESULT hr = pTagBrowser->ChangeBrowsePosition(CX_BROWSE_TO, m_bstrTagGroup);
	if (FAILED(hr))
		return;

	CComPtr<IEnumString> spEnumString;
	CComPtr<IEnumVARIANT> spEnumAttr;
	hr = pTagBrowser->BrowseItemIDs(CX_BRANCH, &spEnumString, &spEnumAttr);
	if (FAILED(hr))
		return;
	TurnOffAuthenticationOnThisInterface(spEnumString);

	LPOLESTR pszGroup;
	while (spEnumString->Next(1, &pszGroup, NULL) == S_OK)
	{
		CTagNode* pNode = new CTagNode(pszGroup, m_pNetNode);
		AddChildNode(pNode);

		CoTaskMemFree(pszGroup);
	}

	CNode::RefreshChildren();
}

void CTagNode::RefreshTags()
{
	ATLASSERT(m_pNetNode != NULL);

	ICxBrowseServerAddressSpace* pTagBrowser = m_pNetNode->GetBrowseServerAddressSpace();
	if (pTagBrowser == NULL)
		return;

	if (m_bstrTagGroup.m_str == NULL)
		m_bstrTagGroup.m_str = GetQualityTagGroup();
	
	if (!m_bstrTagGroup)
	{
		CNode::RefreshTags();
		return;
	}

	HRESULT hr = pTagBrowser->ChangeBrowsePosition(CX_BROWSE_TO, m_bstrTagGroup);
	if (FAILED(hr))
		return;

	CComPtr<IEnumString> spEnumString;
	CComPtr<IEnumVARIANT> spEnumAttr;
	hr = pTagBrowser->BrowseItemIDs(CX_LEAF, &spEnumString, &spEnumAttr);
	if (FAILED(hr))
		return;
	TurnOffAuthenticationOnThisInterface(spEnumString);

	LPOLESTR psz;
	VARIANT var;
	VariantInit(&var);

	while (spEnumString->Next(1, &psz, NULL) == S_OK)
	{
		CTag* pTag = new CTag(this, psz);
		m_apTags.Add(pTag);
		CoTaskMemFree(psz);

		spEnumAttr->Next(1, &var, NULL);
		pTag->m_bstrDesc = var.bstrVal;
		VariantClear(&var);

		spEnumAttr->Next(1, &var, NULL);
		pTag->m_nTagType = var.intVal;
		VariantClear(&var);

		spEnumAttr->Next(1, &var, NULL);
		pTag->m_bstrDevice = var.bstrVal;
		VariantClear(&var);

		spEnumAttr->Next(1, &var, NULL);
		pTag->m_bstrAddress = var.bstrVal;
		VariantClear(&var);
	}

	CNode::RefreshTags();
}