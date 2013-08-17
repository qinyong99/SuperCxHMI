// Scripter.h: interface for the CScripter class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SCRIPTER_H__
#define _SCRIPTER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ATL_NO_VTABLE CScripter :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IActiveScriptSite,
	public IActiveScriptSiteWindow
{
public:
	CScripter();
	virtual ~CScripter();

	DECLARE_PROTECT_FINAL_CONSTRUCT()
		
BEGIN_COM_MAP(CScripter)
	COM_INTERFACE_ENTRY(IActiveScriptSite)
	COM_INTERFACE_ENTRY(IActiveScriptSiteWindow)
END_COM_MAP()

// IActiveScriptSite
public:
	STDMETHOD(GetLCID)( 
		/* [out] */ LCID __RPC_FAR *plcid);
	STDMETHOD(GetItemInfo)( 
		/* [in] */ LPCOLESTR pstrName,
		/* [in] */ DWORD dwReturnMask,
		/* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppiunkItem,
		/* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppti);
	STDMETHOD(GetDocVersionString)( 
		/* [out] */ BSTR __RPC_FAR *pbstrVersion);
	STDMETHOD(OnScriptTerminate)( 
		/* [in] */ const VARIANT __RPC_FAR *pvarResult,
		/* [in] */ const EXCEPINFO __RPC_FAR *pexcepinfo);
	STDMETHOD(OnStateChange)( 
		/* [in] */ SCRIPTSTATE ssScriptState);
	STDMETHOD(OnScriptError)( 
		/* [in] */ IActiveScriptError __RPC_FAR *pscripterror);
	STDMETHOD(OnEnterScript)(void);
	STDMETHOD(OnLeaveScript)(void);
	
// IActiveScriptSiteWindow
public:	
	STDMETHOD(GetWindow)( 
		/* [out] */ HWND __RPC_FAR *phwnd);
        
    STDMETHOD(EnableModeless)( 
		/* [in] */ BOOL fEnable);

// Operations
public:
	BOOL GetLangFuncParams(CString& strFuction, CStringArray &arrParams);
	BOOL IsLangFunction(CString& strFuction);
	BOOL IsLangKeyWord(CString &strWord);
	
	virtual void UnloadScript();
	HRESULT Initialize();
	virtual HRESULT FinishInitialize();

	virtual HRESULT AddItemsName();
	virtual HRESULT GetItemDispatch(LPCOLESTR pszName, IUnknown** ppDispatch);
	virtual HRESULT GetItemTypeInfo(LPCOLESTR pszName, ITypeInfo** ppTypeInfo);

// Attributes
public:
	CComPtr<IActiveScript> m_spActiveScript;
	CComPtr<IActiveScriptParse> m_spActiveScriptParse;
	
	CComPtr<ITypeInfo> m_spTypeInfo;
	CComPtr<ITypeLib> m_spTypeLib;
};

#endif // SCRIPTER_H__
