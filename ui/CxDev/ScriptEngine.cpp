// ScriptEngine.cpp : implementation file
//

#include "stdafx.h"
#include "ScriptEngine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptEngine

IMPLEMENT_DYNCREATE(CScriptEngine, CCmdTarget)

CScriptEngine::CScriptEngine()
{
}

CScriptEngine::~CScriptEngine()
{
	UnloadScript();
}

BEGIN_INTERFACE_MAP(CScriptEngine, CCmdTarget)
	INTERFACE_PART(CScriptEngine, IID_IActiveScriptSite, ActiveScriptSite)
	INTERFACE_PART(CScriptEngine, IID_IActiveScriptSiteWindow, ActiveScriptSiteWindow)
END_INTERFACE_MAP()

///////////////////////////////////////////////////////////////////////////////
// IActiveScriptSite
///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) CScriptEngine::XActiveScriptSite::AddRef()
{
	METHOD_PROLOGUE_(CScriptEngine, ActiveScriptSite)

	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CScriptEngine::XActiveScriptSite::Release()
{
	METHOD_PROLOGUE_(CScriptEngine, ActiveScriptSite)
   
	return pThis->ExternalRelease();
}

STDMETHODIMP CScriptEngine::XActiveScriptSite::QueryInterface(REFIID iid,
	void** ppInterface)
{
	METHOD_PROLOGUE_(CScriptEngine, ActiveScriptSite)

	return pThis->ExternalQueryInterface(&iid, ppInterface);
}

STDMETHODIMP CScriptEngine::XActiveScriptSite::GetDocVersionString(
	BSTR* pbstrVersion)
{
	(void)pbstrVersion;

    return E_NOTIMPL;
}

STDMETHODIMP CScriptEngine::XActiveScriptSite::GetItemInfo(LPCOLESTR pszName,
	DWORD dwReturnMask, IUnknown** ppItem, ITypeInfo** ppTypeInfo)
{

	HRESULT hResult;

	METHOD_PROLOGUE(CScriptEngine, ActiveScriptSite)

	if (ppItem != NULL)
	{
		*ppItem = NULL;
	}
	if (ppTypeInfo != NULL)
	{
		*ppTypeInfo = NULL;
	}
		
	if (dwReturnMask&SCRIPTINFO_IUNKNOWN)
	{
		hResult = pThis->GetItemDispatch(pszName, ppItem);
		if (FAILED(hResult))
		{
			return hResult;
		}
	}
	if (dwReturnMask&SCRIPTINFO_ITYPEINFO)
	{
		pThis->GetItemTypeInfo(pszName, ppTypeInfo);
	}

	return S_OK;
}

STDMETHODIMP CScriptEngine::XActiveScriptSite::GetLCID(LCID* plcid)
{
	METHOD_PROLOGUE(CScriptEngine, ActiveScriptSite)

	if (plcid == NULL)
	{
		return E_POINTER;
	}

	*plcid = GetUserDefaultLCID();

	return S_OK;
}

STDMETHODIMP CScriptEngine::XActiveScriptSite::OnEnterScript()
{
	METHOD_PROLOGUE(CScriptEngine, ActiveScriptSite)

	return S_OK;
}

STDMETHODIMP CScriptEngine::XActiveScriptSite::OnLeaveScript()
{
	METHOD_PROLOGUE(CScriptEngine, ActiveScriptSite)

	return S_OK;
}

STDMETHODIMP CScriptEngine::XActiveScriptSite::OnScriptError(
	IActiveScriptError* pError)
{
	USES_CONVERSION;
	HRESULT hResult;
//	CExcepInfo excep;
	CString strMessage;
//	int nResult;
	DWORD dwContext;
	ULONG iLine;
	LONG iChar;
	BSTR bstrSourceLineText;

	METHOD_PROLOGUE(CScriptEngine, ActiveScriptSite)

	TRACE("XActiveScriptSite::OnScriptError()\n");

	ASSERT(pError != NULL);

	hResult = pError->GetSourcePosition(&dwContext, &iLine, &iChar);
	if (SUCCEEDED(hResult))
	{
		TRACE("Error at line %u, character %d\n", iLine, iChar);
	}
	bstrSourceLineText = NULL;
	hResult = pError->GetSourceLineText(&bstrSourceLineText);
	if (SUCCEEDED(hResult))
	{
		TRACE("Source Text: %s\n", OLE2CT(bstrSourceLineText));
		SysFreeString(bstrSourceLineText);
		bstrSourceLineText = NULL;
	}
//	hResult = pError->GetExceptionInfo(&excep);
//	if (SUCCEEDED(hResult))
//	{
//		AfxFormatString2(strMessage, IDS_SCRIPTERRORFORMAT, OLE2CT(
//			excep.bstrSource), OLE2CT( excep.bstrDescription));
//		nResult = AfxMessageBox(strMessage, MB_YESNO);
//		if (nResult == IDYES)
//		{
//			return S_OK;
//		}
//		else
//		{
//			return E_FAIL;
//		}
//	}

	return E_FAIL;
}

STDMETHODIMP CScriptEngine::XActiveScriptSite::OnScriptTerminate(
	const VARIANT* pvarResult, const EXCEPINFO* pExcepInfo)
{
	(void)pvarResult;
	(void)pExcepInfo;

	METHOD_PROLOGUE(CScriptEngine, ActiveScriptSite)

	return S_OK;
}

STDMETHODIMP CScriptEngine::XActiveScriptSite::OnStateChange(
   SCRIPTSTATE eState)
{
	METHOD_PROLOGUE(CScriptEngine, ActiveScriptSite)

	TRACE("XActiveScriptSite::OnStateChange()\n");

	switch (eState)
	{
	case SCRIPTSTATE_UNINITIALIZED:
		TRACE( "\tSCRIPTSTATE_UNINITIALIZED\n" );
		break;

	case SCRIPTSTATE_INITIALIZED:
		TRACE( "\tSCRIPTSTATE_INITIALIZED\n" );
		break;

   case SCRIPTSTATE_STARTED:
	  TRACE( "\tSCRIPTSTATE_STARTED\n" );
	  break;

	case SCRIPTSTATE_CONNECTED:
		TRACE( "\tSCRIPTSTATE_CONNECTED\n" );
		break;

	case SCRIPTSTATE_DISCONNECTED:
		TRACE( "\tSCRIPTSTATE_DISCONNECTED\n" );
		break;

	case SCRIPTSTATE_CLOSED:
		TRACE( "\tSCRIPTSTATE_CLOSED\n" );
		break;

	default:
		TRACE( "\tUnknown SCRIPTSTATE value\n" );
		ASSERT( FALSE );
		break;
	}

	return S_OK;
}


///////////////////////////////////////////////////////////////////////////////
// IActiveScriptSiteWindow
///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_( ULONG ) CScriptEngine::XActiveScriptSiteWindow::AddRef()
{
	METHOD_PROLOGUE_(CScriptEngine, ActiveScriptSiteWindow)

    return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CScriptEngine::XActiveScriptSiteWindow::Release()
{
	METHOD_PROLOGUE_( CScriptEngine, ActiveScriptSiteWindow )

   	return pThis->ExternalRelease();
}

STDMETHODIMP CScriptEngine::XActiveScriptSiteWindow::QueryInterface(
	REFIID iid, void** ppInterface)
{
	METHOD_PROLOGUE_(CScriptEngine, ActiveScriptSiteWindow)

	return pThis->ExternalQueryInterface(&iid, ppInterface);
}

STDMETHODIMP CScriptEngine::XActiveScriptSiteWindow::EnableModeless(
	BOOL tEnable)
{
	(void)tEnable;

	METHOD_PROLOGUE(CScriptEngine, ActiveScriptSiteWindow)

	return E_NOTIMPL;
}

STDMETHODIMP CScriptEngine::XActiveScriptSiteWindow::GetWindow(
	HWND* phWindow)
{
	METHOD_PROLOGUE(CScriptEngine, ActiveScriptSiteWindow)

	if (phWindow == NULL)
	{
		return E_POINTER;
	}

   *phWindow = AfxGetMainWnd()->GetSafeHwnd();

	return S_OK;
}
//////////////////////////////////////////////////////////////////////////

void CScriptEngine::UnloadScript()
{
	if (m_pActiveScript != NULL)
	{
		m_pActiveScript->SetScriptState(SCRIPTSTATE_DISCONNECTED);
		m_pActiveScript->Close();
		m_pActiveScript.Release();
	}
	if (m_pActiveScriptParse != NULL)
	{
		m_pActiveScriptParse.Release();
	}
}

HRESULT CScriptEngine::Initialize()
{
	HRESULT hResult;
	CLSID clsid;

	UnloadScript();

	hResult = CLSIDFromProgID(L"VBScript", &clsid);
	if (FAILED(hResult))
	{
		 return hResult;
	}
	
	hResult = m_pActiveScript.CreateInstance(clsid, NULL,
		CLSCTX_INPROC_SERVER);
	if (FAILED(hResult))
	{
		return hResult;
	}

	m_pActiveScriptParse = m_pActiveScript;
	if (m_pActiveScriptParse == NULL)
	{
		return E_NOINTERFACE;
	}

	hResult = m_pActiveScript->SetScriptSite(&m_xActiveScriptSite);
	if (FAILED(hResult))
	{
	  return hResult;
	}

	hResult = m_pActiveScriptParse->InitNew();
	if (FAILED(hResult))
	{
		return hResult;
	}

	hResult = AddItemsName();
	if (FAILED(hResult))
	{
		return hResult;
	}

	FinishInitialize();
	
	return S_OK;
}

HRESULT CScriptEngine::FinishInitialize()
{
	m_pActiveScript->SetScriptState(SCRIPTSTATE_STARTED);
//	m_pActiveScript->SetScriptState(SCRIPTSTATE_CONNECTED);

	return S_OK;
}

HRESULT CScriptEngine::AddItemsName()
{
	return S_OK;
}

HRESULT CScriptEngine::GetItemDispatch(LPCOLESTR pszName,
	IUnknown** ppDispatch)
{
	ASSERT(FALSE);

	return TYPE_E_ELEMENTNOTFOUND;
}

HRESULT CScriptEngine::GetItemTypeInfo(LPCOLESTR pszName,
	ITypeInfo** ppTypeInfo)
{
	ASSERT(FALSE);

	return TYPE_E_ELEMENTNOTFOUND;
}

/////////////////////////////////////////////////////////////////////////
//功能：计算表达式
/////////////////////////////////////////////////////////////////////////
HRESULT CScriptEngine::Evaluation(BSTR bstrExpression, VARIANT* pvaValue, BSTR bstrItemName)
{
	return m_pActiveScriptParse->ParseScriptText(bstrExpression, bstrItemName, NULL,
		NULL, DWORD(this), 0, SCRIPTTEXT_ISEXPRESSION, pvaValue, NULL);
}

//////////////////////////////////////////////////////////
//功 能：执行脚本语句
void CScriptEngine::Execute(BSTR bstrStatement, BSTR bstrItemName)
{
	m_pActiveScriptParse->ParseScriptText(bstrStatement, bstrItemName, NULL,
		NULL, DWORD(this), 0, 0, NULL, NULL);
}


