// NetNode.cpp: implementation of the CNetNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NetNode.h"
#include "AlmSum.h"

// SetBlanket.cpp - sample code for explicitly turning off authentication
//                  on various interfaces of a proxy.

// The following function uses QueryBlanket and SetBlanket to correctly
// drop the authentication level to NONE, without mucking up any other
// security settings.
HRESULT TurnOffAuthenticationOnThisInterface(IUnknown* pUnk)
{
	IClientSecurity* pcs;
    HRESULT hr = pUnk->QueryInterface(IID_IClientSecurity, (void**)&pcs );
    if (FAILED(hr))
        return hr;
	
    // Use QueryBlanket to obtain all the other settings that
    // we want to leave the same (don't just call SetBlanket blindly!)
    // Also, since we are turning *off* authentication, we don't care about
    // the RPC_AUTH_IDENTITY_HANDLE parameter.
    DWORD dwAuthnSvc, dwAuthzSvc, dwImpLevel, grfCaps;
    OLECHAR* pszServerPrincipal;
    hr = pcs->QueryBlanket(pUnk, &dwAuthnSvc, &dwAuthzSvc,
		&pszServerPrincipal,
		0,
		&dwImpLevel, 0, &grfCaps);
	
    // Important note: SetBlanket on the proxy manager (IUnknown) only
    //                 controls the settings for QueryInterface in NT4SP3.
    //                 In NT4SP4 and NT5, this will control Release as well.
    if (SUCCEEDED(hr))
        hr = pcs->SetBlanket(pUnk, dwAuthnSvc, dwAuthzSvc,
		pszServerPrincipal,
		RPC_C_AUTHN_LEVEL_NONE,
		dwImpLevel, 0, grfCaps);
	
    pcs->Release();
	
    return hr;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetNode::CNetNode(LPCWSTR szName, LPCWSTR szComputer)
{
	m_bstrName = szName;
	m_bstrComputer = szComputer;
}

CNetNode::~CNetNode()
{

}

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
	
	return m_spDataServer;
}
