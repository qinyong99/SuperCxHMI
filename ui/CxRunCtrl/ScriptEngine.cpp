// ScriptEngine.cpp : implementation file
//

#include "stdafx.h"
#include "RunInc.h"

#include "ScriptErrorDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////
CExcepInfo::CExcepInfo()
{
	memset(this, 0, sizeof(*this));
}

CExcepInfo::~CExcepInfo()
{
	Clear();
}

void CExcepInfo::Clear()
{
	if (bstrSource != NULL)
	{
		::SysFreeString( bstrSource );
	}
	if (bstrDescription != NULL)
	{
		::SysFreeString( bstrDescription);
	}
	if (bstrHelpFile != NULL)
	{
		::SysFreeString(bstrHelpFile);
	}

	memset(this, 0, sizeof(*this));
}

/////////////////////////////////////////////////////////////////////////////
// CScriptEngine

IMPLEMENT_DYNCREATE(CScriptEngine, CCmdTarget)

CScriptEngine::CScriptEngine()
{
	m_bIgnorAllError = FALSE;
	m_pScriptErrorDlg = NULL;
}

CScriptEngine::~CScriptEngine()
{
	UnloadScript();

	if (m_pScriptErrorDlg != NULL)
	{
		delete m_pScriptErrorDlg;
		m_pScriptErrorDlg = NULL;
	}
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

// 未来加入错误处理功能
STDMETHODIMP CScriptEngine::XActiveScriptSite::OnScriptError(
	IActiveScriptError* pError)
{
	USES_CONVERSION;
	USES_CHINESEVAR_CONVERSION;
	
	HRESULT hResult;
	CExcepInfo excep;
	CString strMessage;
	int nResult;
	DWORD dwContext;
	ULONG iLine;
	LONG iChar;
	CComBSTR bstrSourceLineText;

	METHOD_PROLOGUE(CScriptEngine, ActiveScriptSite)

	TRACE("XActiveScriptSite::OnScriptError()\n");

	if (pThis->m_bIgnorAllError)
		return E_FAIL;

	ASSERT(pError != NULL);

	hResult = pError->GetSourcePosition(&dwContext, &iLine, &iChar);
	if (SUCCEEDED(hResult))
	{
		CString strContextText;
		pThis->GetSourceContextText(dwContext, strContextText);
	
// 		strMessage.Format("第 %u 行, 字符 %d 发生错误。\r\n", iLine+1, iChar+1);
		strMessage.Format("%s 第 %u 行发生错误。\r\n\r\n", strContextText, iLine+1);
	}
	if (dwContext == 0)
		return E_FAIL;

	hResult = pError->GetSourceLineText(&bstrSourceLineText);
	if (SUCCEEDED(hResult))
	{
		CString strTemp;
		strTemp.Format("脚本语句: %s\r\n", VARA2C(W2CT(bstrSourceLineText)));
		strMessage += strTemp;
	//	MessageBox("脚本语句: %s\n", OLE2CT(bstrSourceLineText));
	}
	hResult = pError->GetExceptionInfo(&excep);
	if (SUCCEEDED(hResult))
	{
		CString strTemp;
		CString strTemp1 = excep.bstrSource;
		strTemp1.Replace(_T("Microsoft VBScript"), _T("VBScript"));
	//	strTemp1.Replace(_T("Microsoft"), _T("SuperCx"));
		CString strTemp2 = excep.bstrDescription;
		// 有时描述中会含有脚本, 需进行转换
		strTemp2.Replace("'", "\"'\"");
		strTemp2 = VARA2C(strTemp2);
		strTemp2.Replace("\"'\"", "'");
		AfxFormatString2(strTemp, IDS_SCRIPTERRORFORMAT, strTemp1, strTemp2);
		strMessage += strTemp;

		if (pThis->m_pScriptErrorDlg == NULL)
		{
			BOOL bMode = pThis->EnableModeless(FALSE);
			
			pThis->m_pScriptErrorDlg = new CScriptErrorDlg(pThis->GetFrameWnd());
			pThis->m_pScriptErrorDlg->AddErrorInfoMessage(strMessage);
			nResult = pThis->m_pScriptErrorDlg->DoModal();
			
			if (bMode)
				pThis->EnableModeless(TRUE);

			delete pThis->m_pScriptErrorDlg;
			pThis->m_pScriptErrorDlg = NULL;
		}
		else
		{
			pThis->m_pScriptErrorDlg->AddErrorInfoMessage(strMessage);
		}

		if (nResult == ID_IGNORE_ALL)
		{
			pThis->m_bIgnorAllError = TRUE;
			return E_FAIL;
		}
		else if (nResult == ID_BREAK)
		{
			pThis->m_bIgnorAllError = TRUE;
			AfxPostQuitMessage(0);
		}
	}

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

	return pThis->EnableModeless(tEnable) ? S_OK : S_FALSE;
}

STDMETHODIMP CScriptEngine::XActiveScriptSiteWindow::GetWindow(
	HWND* phWindow)
{
	METHOD_PROLOGUE(CScriptEngine, ActiveScriptSiteWindow)

	if (phWindow == NULL)
	{
		return E_POINTER;
	}

	CFrameWnd* pFrameWnd = pThis->GetFrameWnd();
	if (pFrameWnd != NULL)
		*phWindow = pFrameWnd->GetSafeHwnd();
	else
		*phWindow = AfxGetMainWnd()->GetSafeHwnd();

	return S_OK;
}

CFrameWnd* CScriptEngine::GetFrameWnd()
{
	return NULL;
}

BOOL CScriptEngine::EnableModeless(BOOL bEnable)
{
	CFrameWnd* pFrameWnd = GetFrameWnd();
	if (pFrameWnd != NULL)
	{
		if (!bEnable)
		{
			BOOL bEnabled = pFrameWnd->IsWindowEnabled();
			pFrameWnd->BeginModalState();
			pFrameWnd->EnableWindow(bEnabled);
		}
		else
		{
			pFrameWnd->EndModalState();
		//	pFrameWnd->SetFocus();
		}
		
		return TRUE;
	}
	
	return FALSE;
}

void CScriptEngine::UnloadScript()
{
	if (m_pActiveScript != NULL)
	{
		VERIFY(m_pActiveScript->Close() == S_OK);

		m_pActiveScript.Release();
	}
	if (m_pActiveScriptParse != NULL)
	{
		m_pActiveScriptParse.Release();
	}
}

const GUID CLSID_VBScript = { 0xb54f3741, 0x5b07, 0x11cf, { 0xa4, 0xb0,  0x0,  0xaa,  0x0,  0x4a,  0x55,  0xe8 } };

HRESULT CScriptEngine::Initialize()
{
	HRESULT hResult;
//	CLSID clsid;

	UnloadScript();

//	if (m_pSaveActiveScript == NULL)
//	{
//		hResult = CLSIDFromProgID(L"VBScript", &clsid);
//		if (FAILED(hResult))
//		{
//			return hResult;
//		}
	
		hResult = m_pActiveScript.CreateInstance(CLSID_VBScript/*clsid*/, NULL,
			CLSCTX_INPROC_SERVER);
		if (FAILED(hResult))
		{
			return hResult;
		}
//	}
//	else
//	{
//		hResult = m_pSaveActiveScript->Clone(&m_pActiveScript);
//		if (FAILED(hResult))
//		{
//			return hResult;
//		}
//	}

	hResult = m_pActiveScript->SetScriptSite(&m_xActiveScriptSite);
	if (FAILED(hResult))
	{
		return hResult;
	}

//	if (m_pSaveActiveScript == NULL)
//	{
		
//		m_pActiveScript->Clone(&m_pSaveActiveScript);
//	}

	m_pActiveScriptParse = m_pActiveScript;
	if (m_pActiveScriptParse == NULL)
	{
		return E_NOINTERFACE;
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
		NULL, 0, 0, SCRIPTTEXT_ISEXPRESSION, pvaValue, NULL);
}

//////////////////////////////////////////////////////////
//功 能：执行脚本语句
void CScriptEngine::Execute(BSTR bstrStatement, BSTR bstrItemName)
{
	m_pActiveScriptParse->ParseScriptText(bstrStatement, bstrItemName, NULL,
		NULL, 0, 0, 0, NULL, NULL);
}


