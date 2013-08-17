#ifndef __SCRIPTENGINE_H__
#define __SCRIPTENGINE_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptEngine.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScriptEngine command target


class CScriptEngine : public CCmdTarget
{
	DECLARE_DYNCREATE(CScriptEngine)

	CScriptEngine();           // protected constructor used by dynamic creation
	virtual ~CScriptEngine();

// Attributes
public:
	IActiveScriptPtr m_pActiveScript;
	IActiveScriptParsePtr m_pActiveScriptParse;

	
// Operations
public:
	void UnloadScript();
	HRESULT Initialize();
	HRESULT Evaluation(BSTR bstrExpression, VARIANT* pvaValue, BSTR bstrItemName = NULL);
	void Execute(BSTR bstrStatement, BSTR bstrItemName = NULL);

	virtual HRESULT FinishInitialize();
	virtual HRESULT AddItemsName();
	virtual HRESULT GetItemDispatch(LPCOLESTR pszName, IUnknown** ppDispatch);
	virtual HRESULT GetItemTypeInfo(LPCOLESTR pszName, ITypeInfo** ppTypeInfo);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptEngine)
	//}}AFX_VIRTUAL

// Implementation
protected:
	BEGIN_INTERFACE_PART(ActiveScriptSite, IActiveScriptSite)
		STDMETHOD(GetDocVersionString)(BSTR* pbstrVersion);
		STDMETHOD(GetItemInfo)(LPCOLESTR pszName, DWORD dwReturnMask,
			IUnknown** ppItem, ITypeInfo** ppTypeInfo);
		STDMETHOD(GetLCID)(LCID* plcid);
		STDMETHOD(OnEnterScript)();
		STDMETHOD(OnLeaveScript)();
		STDMETHOD(OnScriptError)(IActiveScriptError* pError);
		STDMETHOD(OnScriptTerminate)(const VARIANT* pvarResult,
			const EXCEPINFO* pExcepInfo);
		STDMETHOD(OnStateChange)(SCRIPTSTATE eState);
	END_INTERFACE_PART(ActiveScriptSite)

	BEGIN_INTERFACE_PART(ActiveScriptSiteWindow, IActiveScriptSiteWindow)
		STDMETHOD(EnableModeless)(BOOL tEnable);
		STDMETHOD(GetWindow)(HWND* phWindow);
	END_INTERFACE_PART(ActiveScriptSiteWindow)

	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SCRIPTENGINE_H__)
