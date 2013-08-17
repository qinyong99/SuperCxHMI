// FormScrEng.cpp.cpp : implementation file
//

#include "stdafx.h"
#include "RunInc.h"
#include "RunCtrl.h"
#include "LoginDlg.h"
#include "ReturnValue.h"
#include "BitmapView.h"
#include "BitmapDoc.h"
#include "KeyBoardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void GetMuduleDirectory(LPTSTR szPath);

/////////////////////////////////////////////////////////////////////////////
// CFormScrEng

IMPLEMENT_DYNCREATE(CFormScrEng, CScriptEngine)

CFormScrEng::CFormScrEng(CFormDoc* pFormDoc) :
	m_pFormDoc(pFormDoc)
{
	// 初始化数据源为空
	m_nTagProviderCount = 0;
	for (int i = 0; i < 8; i++)
		m_pTagProvider[i] = NULL;

	m_pdpCurrentEvent = NULL;

	EnableAutomation();
}

CFormScrEng::~CFormScrEng()
{
	m_nTagProviderCount = 0;
	for (int i = 0; i < 8; i++)
	{
		if (m_pTagProvider[i] != NULL)
		{
			m_pTagProvider[i]->Release();
			m_pTagProvider[i] = NULL;
		}
	}
}

BEGIN_INTERFACE_MAP(CFormScrEng, CScriptEngine)
	INTERFACE_PART(CFormScrEng, IID_IDispatch, DispatchExtend)
	INTERFACE_PART(CFormScrEng, IID_ICxScriptEngineSite, ScriptEngineSite)
END_INTERFACE_MAP()

BEGIN_DISPATCH_MAP(CFormScrEng, CScriptEngine)
	//{{AFX_DISPATCH_MAP(CFormScrEng)
	DISP_PROPERTY_EX_ID(CFormScrEng, "EventObject", 1, GetEventObject, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CFormScrEng, "DataEntry", 2, GetDataEntry, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CFormScrEng, "PictureDirectory", 3, GetPictureDirectory, SetPictureDirectory, VT_BSTR)
	DISP_PROPERTY_EX_ID(CFormScrEng, "SetupDirectory", 25, GetSetupDirectory, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_PARAM_ID(CFormScrEng, "Application", 4, GetApplication, SetApplication, VT_VARIANT, VTS_BSTR)
	DISP_PROPERTY_PARAM_ID(CFormScrEng, "Frame", 5, GetFrame, SetFrame, VT_VARIANT, VTS_BSTR)
	DISP_FUNCTION_ID(CFormScrEng, "ShowPicture", 6, ShowPicture, VT_EMPTY, VTS_BSTR)
// 	DISP_FUNCTION_ID(CFormScrEng, "DoEvent", 6, DoEvent, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CFormScrEng, "ExitApplication", 8, ExitApplication, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CFormScrEng, "ContainerContext", 9, ContainerContext, VT_EMPTY, VTS_VARIANT)
	DISP_FUNCTION_ID(CFormScrEng, "Login", 10, Login, VT_BOOL, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CFormScrEng, "Logout", 11, Logout, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CFormScrEng, "ManageUser", 12, ManageUser, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CFormScrEng, "ChangePassword", 13, ChangePassword, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CFormScrEng, "GetUserName", 14, GetUserName, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CFormScrEng, "GetUserSecurityArea", 15, GetUserSecurityArea, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CFormScrEng, "GetUserPrivilege", 16, GetUserPrivilege, VT_I4, VTS_NONE)
//	DISP_FUNCTION_ID(CFormScrEng, "MsgBox", 2, MsgBox, VT_I4, VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
//	DISP_FUNCTION_ID(CFormScrEng, "DoEvent", 2, DoEvent, VT_EMPTY, VTS_NONE)
//	DISP_FUNCTION_ID(CFormScrEng, "PopupPicture", 3, PopupPicture, VT_DISPATCH, VTS_BSTR)
//	DISP_FUNCTION_ID(CFormScrEng, "CreateForm", 4, CreateForm, VT_DISPATCH, VTS_BSTR)
	DISP_FUNCTION_ID(CFormScrEng, "SetEventSubPara", 17, SetEventSubPara, VT_VARIANT, VTS_I4 VTS_VARIANT)
	DISP_FUNCTION_ID(CFormScrEng, "PrintWindow", 18, PrintWindow, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CFormScrEng, "CreateServerObject", 19, CreateServerObject, VT_VARIANT, VTS_VARIANT)
	DISP_PROPERTY_EX_ID(CFormScrEng, "FrameWindow", 20, GetFrameWindow, SetNotSupported, VT_VARIANT)
	DISP_FUNCTION_ID(CFormScrEng, "LoadPicutreFile", 21, LoadPicutreFile, VT_VARIANT, VTS_BSTR)
	DISP_FUNCTION_ID(CFormScrEng, "SaveTempPicutreFile", 22, SaveTempPicutreFile, VT_BSTR, VTS_VARIANT)
	DISP_FUNCTION_ID(CFormScrEng, "ShowSoftKeyboard", 23, ShowSoftKeyboard, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_PROPERTY_PARAM_ID(CFormScrEng, "OpenParameter", 24, GetOpenParameter, SetNotSupported, VT_BSTR, VTS_VARIANT)
	//}}AFX_DISPATCH_MAP
// Note the change to the MsgBox handler: in order to support this overide, we have to define
// our own dispatch id and use VT_MFCVALUE so that it matches the value of zero. Note that you
// can only do this for ONE item, and it becomes the DEFAULT item, which is why we put it as a seperate dispatch object.
// To support optional parms, use the VARIANT types.

END_DISPATCH_MAP()

CFormDoc* CFormScrEng::GetDocument()
{
	return m_pFormDoc;
}

CDevDoc* CFormScrEng::GetDevDoc()
{
	if (m_pFormDoc != NULL)
		return m_pFormDoc->m_pDocument;

	return NULL;
}

LPDISPATCH CFormScrEng::GetEventObject() 
{
	m_pEventObj->AddRef();
	return m_pEventObj;
}

LPDISPATCH CFormScrEng::GetDataEntry()
{
	m_pDataEntry->AddRef();
	return m_pDataEntry;
}

BOOL CFormScrEng::Login(const VARIANT FAR& UserName, const VARIANT FAR& Password, const VARIANT FAR& Prompt)
{
	USES_CONVERSION;

	CString strUserName;
	CString strPassword;
	BOOL bNoPrompt = FALSE;
	HRESULT hr;

	if (UserName.vt != VT_ERROR || Password.vt != VT_ERROR || Prompt.vt != VT_ERROR)
	{
		CComVariant var = UserName;

		hr = var.ChangeType(VT_BSTR);
		if (hr != S_OK)
			return hr;
		strUserName = var.bstrVal;

		var = Password;
		hr = var.ChangeType(VT_BSTR);
		if (hr == S_OK)
			strPassword = var.bstrVal;

		var = Prompt;
		hr = var.ChangeType(VT_BOOL);
		if (hr == S_OK)
			bNoPrompt = var.boolVal != VARIANT_FALSE;
	}
	else 
	{
		CLoginDlg dlg;

		if (UserName.vt != VT_ERROR)
		{
			CComVariant var = UserName;
			var.ChangeType(VT_BSTR);
			dlg.m_strUserName = var.bstrVal;
		}

		BOOL bMode = EnableModeless(FALSE);

		int nRet = dlg.DoModal();
		
		if (bMode)
			EnableModeless(TRUE);

		if (nRet != IDOK)
			return FALSE;

		strUserName = dlg.m_strUserName;
		strPassword = dlg.m_strPassword;
	}

	// 用户名不能为空
	if (strUserName.IsEmpty())
		return FALSE;

	LPWSTR pUserName = T2OLE(strUserName);
	LPWSTR pSecurityAreas = NULL;
	DWORD dwUserPrivilege = 0;
	hr = GetDevDoc()->m_pIDataSession->GetUserAccessRight(&pUserName, T2OLE(strPassword), &pSecurityAreas, &dwUserPrivilege);
	if (SUCCEEDED(hr))
	{
		theApp.m_strUserName = strUserName;
		theApp.m_dwUserPrivilege = dwUserPrivilege & 0xffff;
		theApp.m_bAdministrator = ((dwUserPrivilege & 0x80000) != 0);
		if (pSecurityAreas != NULL)
		{
			theApp.m_strUserSecurityAreas = OLE2T(pSecurityAreas);
			CoTaskMemFree(pSecurityAreas);
		}
	}
	else if (!bNoPrompt)
	{
		BOOL bMode = EnableModeless(FALSE);
		GetFrameWnd()->MessageBox(_T("错误的用户名或密码。"), _T("登录"), MB_ICONERROR);
		if (bMode)
			EnableModeless(TRUE);
	}

	return hr == S_OK;
}

void CFormScrEng::Logout()
{
	theApp.m_strUserName.Empty();
	theApp.m_strUserSecurityAreas.Empty();
	theApp.m_dwUserPrivilege = 0;
	theApp.m_bAdministrator = FALSE;
}

#include "UserManagerDlg.h"

void CFormScrEng::ManageUser()
{
	if (!theApp.m_bAdministrator)
	{
		BOOL bMode = EnableModeless(FALSE);
		GetFrameWnd()->MessageBox(_T("请先用有系统管理权限的用户名登录。"), _T("用户管理"), MB_ICONERROR);
		if (bMode)
			EnableModeless(TRUE);
		return;
	}

	CComPtr<ICxDataServer> spDataServer;
	HRESULT hr = GetDevDoc()->m_pIDataSession->GetNodeServer(NULL, IID_ICxDataServer, (IUnknown **)&spDataServer);
	if (FAILED(hr))
		return;

	CComPtr<ICxUserManager> spUserManager;
	hr = spDataServer->CreateUserManager(IID_ICxUserManager, (IUnknown **)&spUserManager);
	if (FAILED(hr))
		return;

	CUserManagerDlg dlg;

	dlg.m_pUserManager = spUserManager;

	CComPtr<IEnumString> spEnumString;
	hr = spDataServer->BrowseSecurityAreas(&spEnumString);
	if (SUCCEEDED(hr))
	{
		LPOLESTR szArea;
		while (spEnumString->Next(1, &szArea, NULL) == S_OK)
		{
			CString strArea = szArea;
			dlg.m_arrSecurityAreas.Add(strArea);
		}
	}

	BOOL bMode = EnableModeless(FALSE);
	dlg.DoModal();
	if (bMode)
		EnableModeless(TRUE);
}

#include "ChangePasswordDlg.h"

void CFormScrEng::ChangePassword()
{
	CComPtr<ICxDataServer> spDataServer;
	HRESULT hr = GetDevDoc()->m_pIDataSession->GetNodeServer(NULL, IID_ICxDataServer, (IUnknown **)&spDataServer);
	if (FAILED(hr))
		return;
	
	CComPtr<ICxUserManager> spUserManager;
	hr = spDataServer->CreateUserManager(IID_ICxUserManager, (IUnknown **)&spUserManager);
	if (FAILED(hr))
		return;
	
	CChangePasswordDlg dlg;
	dlg.m_strUserName = theApp.m_strUserName;
	dlg.m_pUserManager = spUserManager;
	
	BOOL bMode = EnableModeless(FALSE);
	dlg.DoModal();
	if (bMode)
		EnableModeless(TRUE);
}

BSTR CFormScrEng::GetUserSecurityArea()
{
	return theApp.m_strUserSecurityAreas.AllocSysString();
}

long CFormScrEng::GetUserPrivilege()
{
	return (long)theApp.m_dwUserPrivilege;
}

BSTR CFormScrEng::GetUserName()
{
	return theApp.m_strUserName.AllocSysString();
}

// We override this so that we pump it out to the log and always indicate OK.
void CFormScrEng::ShowPicture(LPCTSTR pszPicture)
{
	GetDevDoc()->m_pRunCtrl->SetPicture(pszPicture);
}

void CFormScrEng::ContainerContext(const VARIANT FAR& Tag)
{
	CRunCtrl* pRunCtrl = GetDevDoc()->m_pRunCtrl;

	CComVariant var;
	if (Tag.vt == (VT_VARIANT | VT_BYREF))
	{
		var = Tag;
	}
	else
	{
		var.vt = (VT_VARIANT | VT_BYREF);
		var.pvarVal = (tagVARIANT *)&Tag;
	}

	pRunCtrl->FireContainerContext(var);
}

void CFormScrEng::ExitApplication()
{
	CRunCtrl* pRunCtrl = GetDevDoc()->m_pRunCtrl;
	pRunCtrl->FireOnQuit();

//	CWnd* pFrameWnd = AfxGetMainWnd();
//	pFrameWnd->PostMessage(WM_CLOSE, 0, 0);

	AfxPostQuitMessage(0);
}

VARIANT CFormScrEng::GetApplication(LPCTSTR pszItem) 
{
	return theApp.GetApplication(pszItem);
}

void CFormScrEng::SetApplication(LPCTSTR pszItem, const VARIANT FAR& newValue) 
{
	theApp.SetApplication(pszItem, newValue);
}

VARIANT CFormScrEng::GetFrame(LPCTSTR pszItem) 
{
	return GetDevDoc()->GetFrame(pszItem);
}

void CFormScrEng::SetFrame(LPCTSTR pszItem, const VARIANT FAR& newValue) 
{
	GetDevDoc()->SetFrame(pszItem, newValue);

	CRunCtrl* pRunCtrl = GetDevDoc()->m_pRunCtrl;
	pRunCtrl->FireFrameItemChanged(pszItem);
}

BSTR CFormScrEng::GetPictureDirectory()
{
	CRunCtrl* pRunCtrl = GetDevDoc()->m_pRunCtrl;
	return pRunCtrl->GetCurrentPath();
}

void CFormScrEng::SetPictureDirectory(LPCTSTR lpszNewValue)
{
	CRunCtrl* pRunCtrl = GetDevDoc()->m_pRunCtrl;
	pRunCtrl->SetCurrentPath(lpszNewValue);
}

BSTR CFormScrEng::GetSetupDirectory()
{
	USES_CONVERSION;

	TCHAR szPath[MAX_PATH];
	GetMuduleDirectory(szPath);
	return ::SysAllocString(T2CW(szPath));
}

void CFormScrEng::DoEvent()
{
	MSG msg;

	// Process existing messages in the application's message queue.
	// When the queue is empty, do clean up and return.
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (!AfxGetThread()->PumpMessage())
			return;
	}
}

const CLSID CLSID_CxEventObj = {0x6D977D5E,0x8793,0x458C,{0xAB,0xD3,0xEA,0x6C,0xCF,0x33,0x9C,0xED}};
const CLSID CLSID_CxDataEntry = {0x71185291,0x1637,0x4E03,{0xBA,0xD8,0xB6,0x30,0x02,0xAB,0x7E,0x81}};

HRESULT CFormScrEng::AddItemsName()
{
	USES_CONVERSION;

	HRESULT hResult;

	hResult = m_pActiveScript->AddNamedItem(L"Event", SCRIPTITEM_ISVISIBLE|SCRIPTITEM_CODEONLY);
	if (FAILED(hResult))
		return hResult;

	hResult = m_pActiveScript->AddNamedItem(L"commfunc", SCRIPTITEM_ISVISIBLE|SCRIPTITEM_GLOBALMEMBERS);
	if (FAILED(hResult))
		return hResult;

	//扩充函数
	hResult = m_pActiveScript->AddNamedItem(L"window", SCRIPTITEM_ISVISIBLE|SCRIPTITEM_GLOBALMEMBERS);
	if (FAILED(hResult))
		return hResult;

	hResult = m_pActiveScript->AddNamedItem(T2COLE(m_pFormDoc->GetDisplayName()), SCRIPTITEM_ISVISIBLE|SCRIPTITEM_ISSOURCE);
	if (FAILED(hResult))
		return hResult;	

	for (int i = 0; i < m_pFormDoc->m_listSubformModule.GetCount(); i++)
	{
		CString strSubformMoudleName;
		strSubformMoudleName.Format(_T("subformmodule%d"), i);
		m_pActiveScript->AddNamedItem(T2COLE(strSubformMoudleName), SCRIPTITEM_ISSOURCE);
	}

	return S_OK;
}

HRESULT CFormScrEng::FinishInitialize()
{
	USES_CONVERSION;

	m_pActiveScript->AddTypeLib(LIBID_CXHMIOBJLib, 1, 0, 0);

	//创建事件对象
	HRESULT hResult = m_pEventObj.CreateInstance(CLSID_CxEventObj);
	if (FAILED(hResult))
	{
		return hResult;
	}

	hResult = m_pDataEntry.CreateInstance(CLSID_CxDataEntry);
	if (FAILED(hResult))
	{
		return hResult;
	}
	m_pDataEntry->SetScriptEngineSite(&m_xScriptEngineSite);

	hResult = m_pActiveScript->SetScriptState(SCRIPTSTATE_STARTED);
	hResult = m_pActiveScript->SetScriptState(SCRIPTSTATE_CONNECTED);

#ifdef _DEBUG
	IDispatchPtr pDispatch;
	hResult = m_pActiveScript->GetScriptDispatch(L"MainForm", &pDispatch);
	if (hResult == S_OK)
	{
		CComDispatchDriver dd(pDispatch);

		CReturnValue value;
		CComVariant var(value.GetIDispatch(false));
	
		hResult = dd.Invoke1(L"TextBox1_KeyPress", &var);
		if (hResult == S_OK)
		{
			long l;
			ASSERT(l != 100);
		}
	}
#endif

	return S_OK;
}

int CFormScrEng::FindTagProvider(LPCTSTR lpszName)
{
	if (m_nTagProviderCount == 0)
	{
		WCHAR szSource[80];
		GetDevDoc()->m_pIDataSession->GetDataSourceCount((DWORD *)&m_nTagProviderCount);
		
		for (int i = 0; i < m_nTagProviderCount; i++)
		{
			GetDevDoc()->m_pIDataSession->GetDataSourceInfo(i, szSource, &m_pTagProvider[i]);
			m_strTagProvider[i] = szSource;
		}
	}

	for (int i = 0; i < m_nTagProviderCount; i++)
	{
		if (m_strTagProvider[i].CompareNoCase(lpszName) == 0)
			return i;
	}
	
	return -1;
}

IDispatch* CFormScrEng::GetTagProvider(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_nTagProviderCount);

	return m_pTagProvider[nIndex];
}

void CFormScrEng::PreTreatExpression(BSTR bstrSource, BSTR* pbstrSource, VARIANT* pvaMin, VARIANT* pvaMax)
{
	if (GetDevDoc()->m_pIDataSession != NULL)
	{
		LPWSTR p = bstrSource;
		if ((p[0] == OLECHAR('T') || p[0] == OLECHAR('t')) &&
			(p[1] == OLECHAR('A') || p[1] == OLECHAR('a')) &&
			(p[2] == OLECHAR('G') || p[2] == OLECHAR('g')) &&
			(p[3] == OLECHAR('S') || p[3] == OLECHAR('s')) &&
			(p[4] == OLECHAR('.')))
		{
			GetDevDoc()->m_pIDataSession->GetTagRange(p + 5, pvaMin, pvaMax);
		}
	}
}

HRESULT CFormScrEng::GetItemDispatch(LPCOLESTR pszName,
	IUnknown** ppDispatch)
{
	USES_CONVERSION;

	if (wcscmp(pszName, L"window") == 0)
	{
		*ppDispatch = GetIDispatch(TRUE);

		return S_OK;
	}

	if (wcscmp(pszName, L"commfunc") == 0)
		return GetDevDoc()->m_pIDataSession->GetCommomFunctions((IDispatch **)ppDispatch);

	if (m_pFormDoc->GetDisplayName() == OLE2T(pszName))
		return m_pFormDoc->GetItemDispatch(ppDispatch);

	POSITION pos = m_pFormDoc->m_listSubformModule.GetHeadPosition();
	while (pos != NULL)
	{
		CFormObj* pFormObj = (CFormObj *)m_pFormDoc->m_listSubformModule.GetNext(pos);
		if (pFormObj->m_strSubformMoudleName == OLE2T(pszName))
			return pFormObj->m_pFormDoc->GetItemDispatch(ppDispatch);
	}

	return TYPE_E_ELEMENTNOTFOUND;
}

void CFormScrEng::GetSourceContextText(DWORD dwCookie, CString& strText)
{
	strText += GetDevDoc()->GetPathName();

	if (dwCookie == -1)
	{
		strText += _T(" -> 窗体\"");
		strText += m_pFormDoc->GetDisplayName();
		strText += _T("\"");
	}
	else
	{
		POSITION pos = m_pFormDoc->m_listSubformModule.GetHeadPosition();
		while (pos != NULL)
		{
			CFormObj* pFormObj = (CFormObj *)m_pFormDoc->m_listSubformModule.GetNext(pos);
			if (pFormObj->m_dwSubformMoudleNumber == dwCookie - 1)
			{
				CString strTextT;

				CFormDoc* pTopDoc = (CFormDoc *)pFormObj->m_pDocument;
				while (1)
				{
					CString strT;
					strT = _T(" -> 子模块\"");
					strT += pFormObj->GetDisplayName();
					strT += _T("\"");
					strTextT.Insert(0, strT);

					if (pTopDoc->m_pFormObj == NULL)
						break;

					pFormObj = pTopDoc->m_pFormObj;
					pTopDoc = (CFormDoc *)pFormObj->m_pDocument;
				}
				
				strText += _T(" -> 窗体\"");
				strText += pTopDoc->GetDisplayName();
				strText += _T("\"");

				strText += strTextT;
			
				break;
			}
		}
	}
}

void CFormScrEng::ActiveEventObject(CRunView* pView)
{
	InitEventObject();
}

void CFormScrEng::UnloadScript()
{
	CScriptEngine::UnloadScript();
}

BOOL CFormScrEng::IsScriptRunning()
{
	if (m_pActiveScript != NULL)
	{
		SCRIPTTHREADSTATE sts;
		VERIFY(m_pActiveScript->GetScriptThreadState(SCRIPTTHREADID_CURRENT, &sts) == S_OK);
		if (sts != 0)
			return TRUE;
	}
	
	return FALSE;
}

HRESULT CFormScrEng::InitEventObject()
{
	CRunView* pView = (CRunView *)GetDocument()->GetRunView();
	if (pView != NULL)
		pView->InitEventObject();

	return S_OK;
}

CFrameWnd* CFormScrEng::GetFrameWnd()
{
	for (HWND hWnd = ::GetActiveWindow(); hWnd != NULL; hWnd = ::GetParent(hWnd))
	{
		CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);
		if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
			return (CFrameWnd*)pWnd;
		//	return (CFrameWnd*)pWnd->GetTopLevelParent();
	}

//	return (CFrameWnd*)CWnd::GetActiveWindow();
	return GetDevDoc()->m_pRunCtrl->m_pFrameWnd;
}

HRESULT CFormScrEng::GetScriptDispatch(LPCOLESTR pszItemName, IDispatch **ppdisp)
{
	return m_pActiveScript->GetScriptDispatch(pszItemName, ppdisp);
}

HRESULT CFormScrEng::InvokeScriptDispatch(LPCOLESTR pszItemName, LPCOLESTR pszMethod, REFIID iid, LCID lcid,
		WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult, EXCEPINFO* pExceptionInfo, UINT* piArgError)
{
	IDispatchPtr pScriptDispatch;
	m_pActiveScript->GetScriptDispatch(pszItemName, &pScriptDispatch);
	if (pScriptDispatch != NULL)
	{
		DISPID dwDispID;
		if (SUCCEEDED(pScriptDispatch->GetIDsOfNames(IID_NULL, (LPOLESTR*)&pszMethod, 1, 0, &dwDispID)))
		{
			m_pdpCurrentEvent = pdpParams;
			HRESULT hr = pScriptDispatch->Invoke(dwDispID, iid, lcid,
				wFlags, pdpParams, pvarResult, pExceptionInfo, piArgError);
			m_pdpCurrentEvent = NULL;

			return hr;
		}	
	}

	return S_OK;
}

VARIANT CFormScrEng::SetEventSubPara(int nPara, const VARIANT FAR& newValue)
{
	VARIANT var;
	VariantInit(&var);
	if (newValue.vt == (VT_VARIANT | VT_BYREF))
		VariantCopy(&var, newValue.pvarVal);
	else
		VariantCopy(&var, (VARIANT*)&newValue);

	if (m_pdpCurrentEvent != NULL && nPara >= 0 && nPara < (int)m_pdpCurrentEvent->cArgs)
	{
		VARIANT& varPara = m_pdpCurrentEvent->rgvarg[m_pdpCurrentEvent->cArgs - nPara - 1];

		if ((varPara.vt & VT_BYREF) != 0)
		{
			if (VariantChangeType(&var, &var, 0, varPara.vt & ~VT_BYREF) == S_OK)
			{
				switch (varPara.vt & ~VT_BYREF)
				{
				case VT_BOOL:
				case VT_I1:
				case VT_UI1:
					*varPara.pbVal = var.bVal;
					break;
				case VT_I2:
				case VT_UI2:
					*varPara.piVal = var.iVal;
					break;
				case VT_I4:
				case VT_UI4:
				case VT_R4:
					*varPara.pfltVal = var.fltVal;
					break;
				case VT_R8:
				case VT_DATE:
					*varPara.pdblVal = var.dblVal;
					break;
				case VT_ERROR:
					*varPara.pscode = var.scode;
				case VT_CY:
					*varPara.pcyVal = var.cyVal;
					break;
				case VT_BSTR:
					if (*varPara.pbstrVal != NULL)
						SysFreeString(*varPara.pbstrVal);
					*varPara.pbstrVal = SysAllocString(var.bstrVal);
					break;
				}
			}
		}
		else if (varPara.vt == VT_DISPATCH || varPara.vt == VT_UNKNOWN)
		{
			CComDispatchDriver dd(varPara.punkVal);
			dd.PutProperty(0, &var);
		}
	}

	return var;
}


void CFormScrEng::PrintWindow(const VARIANT FAR& Window,
	const VARIANT FAR& LeftMargin, const VARIANT FAR& TopMargin, 
	const VARIANT FAR& RightMargin, const VARIANT FAR& BottomMargin,
	const VARIANT FAR& HorizontalScale, const VARIANT FAR& VerticalScale,
	const VARIANT FAR& MultipleScale)
{
	CComVariant var;
	HRESULT hr;
	CString strWindow;
	int nLeftMargin = 900;
	int nTopMargin = 720;
	int nRightMargin = 900;
	int nBottomMargin = 720;
	float fHorizontalScale = 0.0f;
	float fVerticalScale = 0.0f;
	BOOL bMultipleScale = 1;
	HWND hWnd = NULL;

	if (Window.vt != VT_ERROR)
	{
		var = Window;
		hr = VariantChangeType(&var, &var, 0, VT_BSTR);
		if (hr != S_OK)
			return;
		strWindow = var.bstrVal;
	}
	if (LeftMargin.vt != VT_ERROR)
	{
		var = LeftMargin;
		hr = var.ChangeType(VT_I4);	
		if (hr == S_OK)
			nLeftMargin = var.lVal;
	}
	if (TopMargin.vt != VT_ERROR)
	{
		var = TopMargin;
		hr = var.ChangeType(VT_I4);	
		if (hr == S_OK)
			nTopMargin = var.lVal;
	}
	if (RightMargin.vt != VT_ERROR)
	{
		var = RightMargin;
		hr = var.ChangeType(VT_I4);	
		if (hr == S_OK)
			nRightMargin = var.lVal;
	}
	if (BottomMargin.vt != VT_ERROR)
	{
		var = BottomMargin;
		hr = var.ChangeType(VT_I4);	
		if (hr == S_OK)
			nBottomMargin = var.lVal;
	}
	if (HorizontalScale.vt != VT_ERROR)
	{
		var = HorizontalScale;
		hr = var.ChangeType(VT_R4);	
		if (hr == S_OK)
			fHorizontalScale = var.fltVal;
	}
	if (VerticalScale.vt != VT_ERROR)
	{
		var = VerticalScale;
		hr = var.ChangeType(VT_R4);	
		if (hr == S_OK)
			fVerticalScale = var.fltVal;
	}
	if (MultipleScale.vt != VT_ERROR)
	{
		var = MultipleScale;
		hr = var.ChangeType(VT_BOOL);
		if (hr == S_OK)
			bMultipleScale = (var.boolVal != VARIANT_FALSE);
	}

	if (!strWindow.IsEmpty())
	{
		hWnd = FindWindow(NULL, strWindow);
	}
	else
	{
		CRunView* pView = (CRunView *)GetDocument()->GetRunView();
		hWnd = pView->m_hWnd;
	}

	if (hWnd == NULL)
		return;

	CBitmapDoc* pDoc = new CBitmapDoc;

	CRect rcWindow;
	GetWindowRect(hWnd, &rcWindow);

	CWindowDC dcWindow(CWnd::FromHandle(hWnd));
	CDC dc;

	if (dc.CreateCompatibleDC(&dcWindow))
	{
		if (pDoc->m_bitmap.CreateCompatibleBitmap(&dcWindow, rcWindow.Width(), rcWindow.Height()))
		{
			CBitmap* pOldBitmap = dc.SelectObject(&pDoc->m_bitmap);
			dc.BitBlt(0, 0, rcWindow.Width(), rcWindow.Height(), &dcWindow, 0, 0, SRCCOPY);
			dc.SelectObject(pOldBitmap);
		}
	}

	pDoc->m_rcMargin.SetRect(nLeftMargin, nTopMargin, nRightMargin, nBottomMargin);
	pDoc->m_fHorizontalScale = fHorizontalScale;
	pDoc->m_fVerticalScale = fVerticalScale;
	pDoc->m_bMultipleScale = bMultipleScale;

	CRunCtrl* pRunCtrl = GetDevDoc()->m_pRunCtrl;
	pRunCtrl->PrintPreview(NULL, pDoc, RUNTIME_CLASS(CBitmapView));
}

VARIANT CFormScrEng::CreateServerObject(const VARIANT FAR& ClassValue)
{
	VARIANT vaResult;
	VariantInit(&vaResult);

	GetDevDoc()->m_pIDataSession->CreateServerObject(ClassValue, &vaResult, TRUE);
	
	return vaResult;
}

VARIANT CFormScrEng::GetFrameWindow()
{
	CRunCtrl* pRunCtrl = GetDevDoc()->m_pRunCtrl;

	VARIANT vaResult;
	VariantInit(&vaResult);
	vaResult.vt = VT_DISPATCH;
	pRunCtrl->GetInterface(&IID_IUnknown)->QueryInterface(IID_IDispatch, (void **)&vaResult.pdispVal);
	
	return vaResult;
}

VARIANT CFormScrEng::LoadPicutreFile(LPCTSTR lpszFileName)
{
	VARIANT vaResult;
	vaResult.vt = VT_ERROR;
	BYTE* pBuffer = NULL;
	DWORD dwSize;
	DWORD dwBytesRead = 0;
	SAFEARRAY* psa = NULL;
	BOOL bSuccess;

	HANDLE hFile = ::CreateFile(
		lpszFileName,
		GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
	
	if (hFile == INVALID_HANDLE_VALUE)
		goto onexit;

	dwSize = GetFileSize(hFile, NULL);
	pBuffer = new BYTE[dwSize];
	if (pBuffer == NULL)
		goto onexit;
	
	bSuccess = ::ReadFile(hFile, pBuffer, dwSize, &dwBytesRead, NULL);
	CloseHandle(hFile);
	if (!bSuccess || dwBytesRead != dwSize)
		goto onexit;

	psa = SafeArrayCreateVector(VT_UI1, 0, dwSize);
	if (psa != NULL)
	{
		memcpy(psa->pvData, pBuffer, dwSize);
		vaResult.vt = VT_ARRAY | VT_UI1;
		vaResult.parray = psa;
	}

onexit:
	if (pBuffer != NULL)
		delete [] pBuffer;
	return vaResult;
}

BSTR CFormScrEng::SaveTempPicutreFile(const VARIANT FAR& vaData)
{
	USES_CONVERSION;
	TCHAR szFileName[MAX_PATH];

	const VARIANT* pData = &vaData;

	if ((pData->vt & VT_BYREF) != 0)
		pData = pData->pvarVal;

	if (pData->vt != (VT_ARRAY | VT_UI1))
		return NULL;
	
	GetTempPath(MAX_PATH, szFileName);
    GetTempFileName(szFileName, "grf", 0, szFileName);
	strcat(szFileName, ".grf");

	HANDLE hFile = CreateFile(szFileName, GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return NULL;

	BYTE* pBuffer = NULL;
	SafeArrayAccessData(pData->parray, (void HUGEP**)&pBuffer);
	DWORD dwElemsize = SafeArrayGetElemsize(pData->parray);

	long lLBound = 0;
	long lUBound = 0;
	SafeArrayGetLBound(pData->parray, 1, &lLBound);
	SafeArrayGetUBound(pData->parray, 1, &lUBound);
	DWORD nSize = (lUBound - lLBound + 1) * dwElemsize;

	DWORD dwWritten = 0;
	WriteFile(hFile, pBuffer, nSize, &dwWritten, NULL);
	CloseHandle(hFile);

	SafeArrayUnaccessData(pData->parray);

	return SysAllocString(T2CW(szFileName));
}

BSTR CFormScrEng::GetOpenParameter(const VARIANT FAR& Item)
{
	CString strPara = GetDevDoc()->m_pRunCtrl->m_strOpenParameter.AllocSysString();

	if (Item.vt == VT_ERROR)
		return strPara.AllocSysString();

	CMapStringToString mapItems;

	LPCTSTR p1 = strPara;
	LPCTSTR p2 = NULL;
	LPCTSTR p3 = p1;

	while (1)
	{ 
		if (*p3 == _T('='))
		{
			p2 = p3;
		}

		if (*p3 == _T('&') || *p3 == '\0')
		{
			if (p2 != NULL)
			{
				CString strItem(p1, p2 - p1);
				strItem.TrimLeft();
				strItem.TrimRight();
				if (!strItem.IsEmpty())
				{
					CString strValve(p2 + 1, p3 - p2 - 1);
					strValve.TrimLeft();
					strValve.TrimRight();

					mapItems.SetAt(strItem, strValve);
				}
			}

			p2 = NULL;
			p1 = p3 + 1;
		}

		if (*p3 == '\0')
			break;

		p3++;
	}

	CComVariant var = Item;
	var.ChangeType(VT_BSTR);

	if (var.vt = VT_BSTR)
	{
		CString strItem = var.bstrVal;
		CString strValve;
		if (mapItems.Lookup(strItem, strValve))
			return strValve.AllocSysString();
	}

	return NULL;
}

void CFormScrEng::ShowSoftKeyboard(const VARIANT FAR& Hide, const VARIANT FAR& OnlyNumber, const VARIANT FAR& XPos, const VARIANT FAR& YPos)
{
	CComVariant var;
	HRESULT hr;
	BOOL bHide = FALSE;
	BOOL bOnlyNumber = FALSE;
	BOOL bDefaultPos = TRUE;
	int nXPos = -1;
	int nYPos = -1;

	if (Hide.vt != VT_ERROR)
	{
		var = Hide;
		hr = VariantChangeType(&var, &var, 0, VT_BOOL);
		if (hr != S_OK)
			return;
		bHide = var.boolVal != VARIANT_FALSE;
	}
	
	if (OnlyNumber.vt != VT_ERROR)
	{
		var = OnlyNumber;
		hr = VariantChangeType(&var, &var, 0, VT_BOOL);
		if (hr != S_OK)
			return;
		bOnlyNumber = var.boolVal != VARIANT_FALSE;
	}
	
	bDefaultPos = !(XPos.vt != VT_ERROR && YPos.vt != VT_ERROR);

	if (XPos.vt != VT_ERROR)
	{
		var = XPos;
		hr = VariantChangeType(&var, &var, 0, VT_I4);
		if (hr != S_OK)
			return;
		nXPos = var.intVal;
	}

	if (YPos.vt != VT_ERROR)
	{
		var = YPos;
		hr = VariantChangeType(&var, &var, 0, VT_I4);
		if (hr != S_OK)
			return;
		nYPos = var.intVal;
	}

	if (!bHide)
		CKeyBoardDlg::ShowSoftKeyBoard(NULL, bOnlyNumber, bDefaultPos, nXPos, nYPos);
	else
		CKeyBoardDlg::HideSoftKeyBoard();
}

/////////////////////////////////////////////////////////////////////////////
// CFormScrEng::XDispatchExtend

STDMETHODIMP_( ULONG ) CFormScrEng::XDispatchExtend::AddRef()
{
	METHOD_PROLOGUE(CFormScrEng, DispatchExtend)

    return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CFormScrEng::XDispatchExtend::Release()
{
	METHOD_PROLOGUE(CFormScrEng, DispatchExtend)

	return pThis->ExternalRelease();
}

STDMETHODIMP CFormScrEng::XDispatchExtend::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CFormScrEng, DispatchExtend)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP CFormScrEng::XDispatchExtend::GetIDsOfNames(
	REFIID iid, LPOLESTR* ppszNames, UINT nNames, LCID lcid, DISPID* pDispIDs)
{
	METHOD_PROLOGUE(CFormScrEng, DispatchExtend)
	ASSERT_VALID(pThis);

	if (nNames == 1)
	{
		CString strFunName = ppszNames[0];
		int nIndex = pThis->FindTagProvider(strFunName);
		if (nIndex != -1)
		{
			pDispIDs[0] = CXDISPID_EXTPROP + nIndex;
			return S_OK;
		}
	}

	return  ((IDispatch *)&pThis->m_xDispatch)->GetIDsOfNames(iid, ppszNames, nNames, lcid, pDispIDs);
}

STDMETHODIMP CFormScrEng::XDispatchExtend::GetTypeInfo(
	UINT iTypeInfo, LCID lcid, ITypeInfo** ppTypeInfo)
{
	*ppTypeInfo = NULL;
	
	return S_OK;
}

STDMETHODIMP CFormScrEng::XDispatchExtend::GetTypeInfoCount(
   UINT* pnInfoCount )
{
	*pnInfoCount = 0;
	
	return S_OK;
}

STDMETHODIMP CFormScrEng::XDispatchExtend::Invoke(
	DISPID dispidMember, REFIID iid, LCID lcid, WORD wFlags,
	DISPPARAMS* pdpParams, VARIANT* pvarResult, EXCEPINFO* pExceptionInfo,
	UINT* piArgErr)
{

	METHOD_PROLOGUE(CFormScrEng, DispatchExtend)
	ASSERT_VALID(pThis);

	if (dispidMember >= CXDISPID_EXTPROP)
	{
		if (pvarResult)	
		{
			pvarResult->vt = VT_DISPATCH;
			IDispatch* pdisp = pThis->GetTagProvider(dispidMember - CXDISPID_EXTPROP);
			pvarResult->pdispVal = pdisp;
			if (pdisp != NULL)
				pdisp->AddRef();		// We're returning this disp ptr
			return S_OK;
		}
	}
	//	return pThis->m_pTagProvider->Invoke(dispidMember, iid, lcid, wFlags, 
	//		pdpParams, pvarResult, pExceptionInfo, piArgErr);
	
	return ((IDispatch *)(&pThis->m_xDispatch))->Invoke(dispidMember, iid, lcid, wFlags, 
			pdpParams, pvarResult, pExceptionInfo, piArgErr);
}

////////////////////////////////////////////////////////////////
STDMETHODIMP_( ULONG ) CFormScrEng::XScriptEngineSite::AddRef()
{
	METHOD_PROLOGUE(CFormScrEng, ScriptEngineSite)
		
	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CFormScrEng::XScriptEngineSite::Release()
{
	METHOD_PROLOGUE(CFormScrEng, ScriptEngineSite)
		
	return pThis->ExternalRelease();
}

STDMETHODIMP CFormScrEng::XScriptEngineSite::QueryInterface(
			REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CFormScrEng, ScriptEngineSite)
		
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CFormScrEng::XScriptEngineSite::EvaluateExpression(BSTR bstrSource, VARIANT* pvaValue)
{
	METHOD_PROLOGUE(CFormScrEng, ScriptEngineSite)
		
	CComBSTR bstrName;
	bstrName.m_str = pThis->GetDocument()->GetDisplayName().AllocSysString();	
	
	return pThis->Evaluation(bstrSource, pvaValue, bstrName);	
}

STDMETHODIMP CFormScrEng::XScriptEngineSite::Execute(BSTR bstrStatement)
{
	METHOD_PROLOGUE(CFormScrEng, ScriptEngineSite)

	CComBSTR bstrName;
	bstrName.m_str = pThis->GetDocument()->GetDisplayName().AllocSysString();

	pThis->Execute(bstrStatement, bstrName);

	return S_OK;
}

STDMETHODIMP CFormScrEng::XScriptEngineSite::PreTreatExpression(BSTR bstrSource, BSTR* pbstrSource)
{
	METHOD_PROLOGUE(CFormScrEng, ScriptEngineSite)

	pThis->PreTreatExpression(bstrSource, pbstrSource);	
	
	return S_OK;
}

STDMETHODIMP CFormScrEng::XScriptEngineSite::PreTreatExpressionEx(BSTR bstrSource, BSTR* pbstrSource, VARIANT* pvaMin, VARIANT* pvaMax)
{
	METHOD_PROLOGUE(CFormScrEng, ScriptEngineSite)

	pThis->PreTreatExpression(bstrSource, pbstrSource, pvaMin, pvaMax);	

	return S_OK;
}

