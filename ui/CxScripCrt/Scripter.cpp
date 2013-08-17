// Scripter.cpp: implementation of the CScripter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Scripter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScripter::CScripter()
{

}

CScripter::~CScripter()
{
	UnloadScript();
}

/////////////////////////////////////////////////////////////////////
// IActiveScriptSite

STDMETHODIMP CScripter::GetLCID(
	/* [out] */ LCID __RPC_FAR *plcid)
{
	*plcid = GetUserDefaultLCID();
	
	return S_OK;
}
        
STDMETHODIMP CScripter::GetItemInfo(
	/* [in] */ LPCOLESTR pszName,
	/* [in] */ DWORD dwReturnMask,
	/* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppItem,
	/* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTypeInfo)
{
	if (dwReturnMask & SCRIPTINFO_IUNKNOWN)
		return GetItemDispatch(pszName, ppItem);
	
	if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
		return GetItemTypeInfo(pszName, ppTypeInfo);
	
	return E_FAIL;
}
        
STDMETHODIMP CScripter::GetDocVersionString(
	/* [out] */ BSTR __RPC_FAR *pbstrVersion)
{
	return E_NOTIMPL;
}
        
STDMETHODIMP CScripter::OnScriptTerminate(
	/* [in] */ const VARIANT __RPC_FAR *pvarResult,
	/* [in] */ const EXCEPINFO __RPC_FAR *pexcepinfo)
{
	return S_OK;
}
        
STDMETHODIMP CScripter::OnStateChange(/* [in] */ SCRIPTSTATE ssScriptState)
{
	return S_OK;
}
        
STDMETHODIMP CScripter::OnScriptError(/* [in] */ IActiveScriptError __RPC_FAR *pscripterror)
{
	return S_OK;
}
        
STDMETHODIMP CScripter::OnEnterScript( void)
{
	return S_OK;
}
        
STDMETHODIMP CScripter::OnLeaveScript( void)
{
	return S_OK;
}

/////////////////////////////////////////////////////////////////////
// IActiveScriptSiteWindow

STDMETHODIMP CScripter::GetWindow(/*[out]*/HWND __RPC_FAR *phwnd)
{
	phwnd = NULL;
	return S_OK;
}
        
STDMETHODIMP CScripter::EnableModeless(/*[in]*/BOOL fEnable)
{
	return S_OK;
}

////////////////////////////////////////////////////////

HRESULT CScripter::AddItemsName()
{
	return S_OK;
}

HRESULT CScripter::GetItemDispatch(LPCOLESTR pszName, IUnknown** ppDispatch)
{
	ATLASSERT(FALSE);
	
	return TYPE_E_ELEMENTNOTFOUND;
}

HRESULT CScripter::GetItemTypeInfo(LPCOLESTR pszName, ITypeInfo** ppTypeInfo)
{
	//		CComBSTR bstrName = pszName;
	ATLASSERT(FALSE);
	
	return TYPE_E_ELEMENTNOTFOUND;
}

void CScripter::UnloadScript()
{
	if (m_spActiveScript != NULL)
	{
		m_spActiveScript->Close();
		m_spActiveScript.Release();
	}
	if (m_spActiveScriptParse != NULL)
		m_spActiveScriptParse.Release();

	if (m_spTypeLib != NULL)
		m_spTypeLib.Release();

	if (m_spTypeInfo != NULL)
		m_spTypeInfo.Release();
}

const GUID CLSID_VBScript = { 0xb54f3741, 0x5b07, 0x11cf, { 0xa4, 0xb0,  0x0,  0xaa,  0x0,  0x4a,  0x55,  0xe8 } };

HRESULT CScripter::Initialize()
{
	HRESULT hResult;
	
	UnloadScript();
	
	hResult = m_spActiveScript.CoCreateInstance(CLSID_VBScript);
	if (FAILED(hResult))
		return hResult;
	
	hResult = LoadTypeLib(L"vbscript.dll", &m_spTypeLib);
	if (FAILED(hResult))
		return hResult;
	
	hResult = m_spTypeLib->GetTypeInfo(0, &m_spTypeInfo);
	if (FAILED(hResult))
		return hResult;
	
	m_spActiveScript->QueryInterface(IID_IActiveScriptParse, (void**)&m_spActiveScriptParse);
	if (m_spActiveScriptParse == NULL)
		return E_NOINTERFACE;
	
	CComQIPtr<IActiveScriptSite> spActiveScriptSite(GetUnknown());
	hResult = m_spActiveScript->SetScriptSite(spActiveScriptSite); 
	if (FAILED(hResult))
		return hResult;
	spActiveScriptSite.p = NULL;
		
	hResult = m_spActiveScriptParse->InitNew();
	if (FAILED(hResult))
		return hResult;
	
	hResult = AddItemsName();
	if (FAILED(hResult))
		return hResult;
	
	FinishInitialize();
	
	return S_OK;
}

HRESULT CScripter::FinishInitialize()
{
	m_spActiveScript->SetScriptState(SCRIPTSTATE_STARTED);
	
	return S_OK;
}

BOOL CScripter::IsLangFunction(CString& strFuction)
{
	USES_CONVERSION;
	TYPEATTR *pta;
	m_spTypeInfo->GetTypeAttr(&pta);
	
	CComBSTR bstrTest(strFuction);
	for (int i = 0; i < pta->cFuncs; i++)
	{
		FUNCDESC* pfd;
		m_spTypeInfo->GetFuncDesc(i, &pfd);
		CComBSTR bstrName;
		m_spTypeInfo->GetDocumentation(pfd->memid, &bstrName, NULL, NULL, NULL);
		if (lstrcmpiW(bstrName, bstrTest) == 0)
		{
			m_spTypeInfo->ReleaseTypeAttr(pta);
			m_spTypeInfo->ReleaseFuncDesc(pfd);
			strFuction = bstrName;
			return i;
		}
		m_spTypeInfo->ReleaseFuncDesc(pfd);
	}
	m_spTypeInfo->ReleaseTypeAttr(pta);
	
	return -1;
}

BOOL CScripter::GetLangFuncParams(CString& strFuction, CStringArray &arrParams)
{
	USES_CONVERSION;
	int index = IsLangFunction(strFuction);
	if (index == -1)
		return FALSE;
	
	FUNCDESC* pfd;
	m_spTypeInfo->GetFuncDesc(index, &pfd);
	
	BSTR bstrParams[20];
	UINT numParams;
	BOOL result = FALSE;
	m_spTypeInfo->GetNames(pfd->memid, bstrParams, 20, &numParams);
	for (UINT i = 1; i < numParams; i++)
	{
		CString str = bstrParams[i];
		if ((int)i > pfd->cParams - pfd->cParamsOpt)
			str = "[" + str + "]";
		arrParams.Add(str);
		SysFreeString(bstrParams[i]);
		result = TRUE;
	}
	m_spTypeInfo->ReleaseFuncDesc(pfd);

	return result;
}

BOOL CScripter::IsLangKeyWord(CString &strWord)
{
	USES_CONVERSION;
	BOOL bFound;
	CComBSTR bstrName(strWord);
	m_spTypeLib->IsName(bstrName, 0, &bFound);
	if (bFound)
	{
		strWord = bstrName;
		return TRUE;
	}
	
	return FALSE;
}