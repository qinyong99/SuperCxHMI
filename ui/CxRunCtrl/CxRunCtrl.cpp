// CxRunCtrl.cpp : Implementation of CCxRunCtrlApp and DLL registration.

#include "stdafx.h"
#include "RunInc.h"
#include "KeyBoardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCxRunCtrlApp NEAR theApp;
DWORD theAppKey;
DWORD theAppStartTime;

const GUID CDECL BASED_CODE _tlid =
		{ 0x277d304b, 0xedc4, 0x4d2c, { 0xa0, 0xae, 0x2a, 0x85, 0x5f, 0x8c, 0xb3, 0x99 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 6;

const char szAppName[] = _T("画面运行");

HHOOK g_hMessageHook = NULL;

////////////////////////////////////////////////////////////////////////////
// CCxRunCtrlApp::InitInstance - DLL initialization

CCxRunCtrlApp::CCxRunCtrlApp()
{
	theAppKey = _SUPERCX_VER & 0xffff0000;
	theAppStartTime = GetTickCount();

	m_bHighlightMouseObject = FALSE;

	m_dwUserPrivilege = 0;
	m_bAdministrator = FALSE;

	if (m_pszAppName == NULL)
		m_pszAppName = _tcsdup(szAppName);
}

// Hook procedure for WH_GETMESSAGE hook type.
LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// Switch the module state for the correct handle to be used.
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// If this is a keystrokes message, translate it in controls'
	// PreTranslateMessage().
	LPMSG pMsg = (LPMSG)lParam;
	if (nCode >= 0 && PM_REMOVE == wParam)
	{
		if (((pMsg->message >= WM_KEYFIRST) && (pMsg->message <= WM_KEYLAST)) ||
			((pMsg->message >= WM_MOUSEFIRST) && (pMsg->message <= WM_MOUSELAST)))
		{
			HWND hWndStop = AfxGetMainWnd()->GetSafeHwnd();
			
			for (HWND hWnd = pMsg->hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd))
			{
				CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);
				// 只处理 CRunView
				if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CRunView)))
				{
					if (pWnd->PreTranslateMessage(pMsg))
					{
						// The value returned from this hookproc is ignored, and it cannot
						// be used to tell Windows the message has been handled. To avoid
						// further processing, convert the message to WM_NULL before
						// returning.
						pMsg->message = WM_NULL;
						pMsg->lParam = 0L;
						pMsg->wParam = 0;

						break;
					}

					pWnd = pWnd->GetParent();
					if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CPopupFrame)) && pWnd->PreTranslateMessage(pMsg))
					{
						pMsg->message = WM_NULL;
						pMsg->lParam = 0L;
						pMsg->wParam = 0;
					}

					break;
				}
				
				// got to hWndStop window without interest
				if (hWnd == hWndStop)
					break;
			}
		}
	}
	
	// Passes the hook information to the next hook procedure in
	// the current hook chain.
	return ::CallNextHookEx(g_hMessageHook, nCode, wParam, lParam);
}

BOOL CCxRunCtrlApp::InitInstance()
{
	if (m_pCacheFileManager == NULL)
		m_pCacheFileManager = new CCacheFileManager;

	g_hMessageHook = ::SetWindowsHookEx(
			WH_GETMESSAGE,
			GetMessageProc,
			AfxGetInstanceHandle(),
			GetCurrentThreadId());
	ASSERT(g_hMessageHook != NULL);

	return COleControlModule::InitInstance();
}

int CCxRunCtrlApp::ExitInstance()
{
	if (m_pCacheFileManager != NULL)
	{
		delete m_pCacheFileManager;
		m_pCacheFileManager = NULL;
	}

	if (g_hMessageHook != NULL)
	{
		::UnhookWindowsHookEx(g_hMessageHook);
		g_hMessageHook = NULL;
	}

	CKeyBoardDlg::HideSoftKeyBoard();

	return COleControlModule::ExitInstance();
}

void CCxRunCtrlApp::ClearApplicationVariables()
{
	CString strItem;
	VARIANT* pVar;
	POSITION pos = m_mapApplicationItems.GetStartPosition();
	while (pos != NULL)
	{
		m_mapApplicationItems.GetNextAssoc(pos, strItem, pVar);
		VariantClear(pVar);
		delete pVar;
	}
	m_mapApplicationItems.RemoveAll();
}

VARIANT CCxRunCtrlApp::GetApplication(LPCTSTR pszItem) 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	
	if (pszItem != NULL)
	{
		VARIANT* pVar = NULL;
		if (m_mapApplicationItems.Lookup(pszItem, pVar))
			VariantCopy(&vaResult, pVar);
	}
	
	return vaResult;
}

void CCxRunCtrlApp::SetApplication(LPCTSTR pszItem, const VARIANT FAR& newValue) 
{
	VARIANT* pVar = NULL;
	if (m_mapApplicationItems.Lookup(pszItem, pVar))
	{
		VariantClear(pVar);
		if (newValue.vt == VT_NULL)
		{
			delete pVar;
			m_mapApplicationItems.RemoveKey(pszItem);
			return;
		}
	}
	else
	{
		if (newValue.vt == VT_NULL)
			return;
		
		pVar = new VARIANT;
		VariantInit(pVar);
	}
	
	VariantCopy(pVar, (VARIANT *)&newValue);
	m_mapApplicationItems.SetAt(pszItem, pVar);
}

BOOL CCxRunCtrlApp::VerifySecurityArea(LPCTSTR lpszArea)
{
	LPCTSTR p1;
	LPCTSTR p2;

	p1 = lpszArea;

	for (;;)
	{
		while (*p1 == ' ')
		{
			p1++;
		}
		p2 = p1;
		
		while (__iscsym(*p2))
			p2++;
		
		p2++;

		TCHAR szT[128];
		lstrcpyn(szT, p1, p2 - p1);
		if (StrInStrlist(m_strUserSecurityAreas, szT))
			return TRUE;
		
		if (*p2 == '\0')
			return FALSE;
		
		p1 = p2;
	}

	return FALSE;
}

#include "comcat.h"

const CLSID CLSID_RunCtrl = {0xC162A5CC,0x6BB1,0x4C0A,{0x90,0x81,0xC5,0x20,0xA4,0xEA,0xDD,0xA7}};

//////////////////////////////////////////////////////////////////////

// 创建组件种类
HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription)
{
    ICatRegister* pcr = NULL ;
    HRESULT hr = S_OK ;

    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
            NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (FAILED(hr))
        return hr;

    // Make sure the HKCR\Component Categories\{..catid...}
    // key is registered.
    CATEGORYINFO catinfo;
    catinfo.catid = catid;
    catinfo.lcid = 0x0409 ; // english

    // Make sure the provided description is not too long.
    // Only copy the first 127 characters if it is.
    int len = wcslen(catDescription);
    if (len>127)
        len = 127;
    wcsncpy(catinfo.szDescription, catDescription, len);
    // Make sure the description is null terminated.
    catinfo.szDescription[len] = '\0';

    hr = pcr->RegisterCategories(1, &catinfo);
        pcr->Release();

    return hr;
}

// 注册组件种类
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
    // Register your component categories information.
    ICatRegister* pcr = NULL ;
    HRESULT hr = S_OK ;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
                NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (SUCCEEDED(hr))
    {
       // Register this category as being "implemented" by the class.
       CATID rgcatid[1] ;
       rgcatid[0] = catid;
       hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
    }
    if (pcr != NULL)
        pcr->Release();
    return hr;
}
// 卸载组件种类
HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
    ICatRegister* pcr = NULL ;
    HRESULT hr = S_OK ;

    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
            NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (SUCCEEDED(hr))
    {
       // Unregister this category as being "implemented" by the class.
       CATID rgcatid[1] ;
       rgcatid[0] = catid;
       hr = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid);
    }

    if (pcr != NULL)
        pcr->Release();

    return hr;
}

extern const CLSID CLSID_RunCtrl;

void ModifyRegForWebRun(CLSID clsid, BOOL bAllow = TRUE)
{
	USES_CONVERSION;
	CString strKey;
	HRESULT hr;
	DWORD dw;
	OLECHAR* szCLSID;
	static TCHAR szKey[] = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Ext\\Stats");

	hr = ::StringFromCLSID(clsid, &szCLSID);
	if (!SUCCEEDED(hr))
		return;

	HKEY hStatsKey = NULL;
	HKEY hClsidKey = NULL;
	HKEY hIexploreKey = NULL;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, szKey, 0, KEY_WRITE|KEY_READ,
		&hStatsKey) == ERROR_SUCCESS)
	{
		if (bAllow)
		{
			if (RegCreateKeyEx(hStatsKey, OLE2T(szCLSID), 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hClsidKey, &dw) == ERROR_SUCCESS)
			{
				if (RegCreateKeyEx(hClsidKey, _T("iexplore"), 0, REG_NONE,
					REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
					&hIexploreKey, &dw) == ERROR_SUCCESS)
				{
					int nValue = 4;
					RegSetValueEx(hIexploreKey, _T("Flags"), NULL, REG_DWORD,
						(LPBYTE)&nValue, sizeof(nValue));
				}
			}
		}
		else
		{
			if (RegOpenKeyEx(hStatsKey, OLE2T(szCLSID), 0, KEY_WRITE|KEY_READ,
				&hClsidKey) == ERROR_SUCCESS)
			{
				LONG lResult = RegDeleteKey(hClsidKey, _T("iexplore"));
				ASSERT(lResult == ERROR_SUCCESS);

				RegCloseKey(hClsidKey);
				hClsidKey = NULL;
			}

			LONG lResult = RegDeleteKey(hStatsKey, OLE2T(szCLSID));
			ASSERT(lResult == ERROR_SUCCESS);
		}
	}
	if (hStatsKey != NULL)
		RegCloseKey(hStatsKey);
	if (hClsidKey != NULL)
		RegCloseKey(hClsidKey);
	if (hIexploreKey != NULL)
		RegCloseKey(hIexploreKey);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	ModifyRegForWebRun(CLSID_RunCtrl);

// 	// 标记控件初始化安全.
//     // 创建初始化安全组件种类
//     HRESULT hr = CreateComponentCategory(CATID_SafeForInitializing,
//                L"Controls safely initializable from persistent data!");
//     if (FAILED(hr))
//         return hr;
//     // 注册初始化安全
//     hr = RegisterCLSIDInCategory(CLSID_RunCtrl, CATID_SafeForInitializing);
//     if (FAILED(hr))
//         return hr;
// 
//     // 标记控件脚本安全
//     // 创建脚本安全组件种类 
//     hr = CreateComponentCategory(CATID_SafeForScripting, L"Controls safely scriptable!");
//     if (FAILED(hr))
//         return hr;
//     // 注册脚本安全组件种类
//     hr = RegisterCLSIDInCategory(CLSID_RunCtrl, CATID_SafeForScripting);
//     if (FAILED(hr))
//         return hr;

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	ModifyRegForWebRun(CLSID_RunCtrl, FALSE);

// 	// 删除控件初始化安全入口.
//     HRESULT hr=UnRegisterCLSIDInCategory(CLSID_RunCtrl, CATID_SafeForInitializing);
//     if (FAILED(hr))
//         return hr;
//     // 删除控件脚本安全入口
//     hr=UnRegisterCLSIDInCategory(CLSID_RunCtrl, CATID_SafeForScripting);
//     if (FAILED(hr))
//         return hr;

	return NOERROR;
}


DWORD CalculateBlockCount()
{
	return 0;
}

void GetMuduleDirectory(LPTSTR szPath)
{
	TCHAR szBuf[_MAX_PATH];
	::GetModuleFileName(theApp.m_hInstance, szBuf, _MAX_PATH);
	
	LPTSTR lpszTemp = (LPTSTR)szBuf;
	for (LPCTSTR lpsz = szBuf; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
	{
		// remember last directory/drive separator
		if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
			lpszTemp = (LPTSTR)_tcsinc(lpsz);
	}
	
	*lpszTemp = 0;
	
	lstrcpy(szPath, szBuf);
}

void LogSystemEvent(LPCTSTR lpszSource, LPCTSTR lpszMessage = NULL, UINT uType = 0)
{
}
