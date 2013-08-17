// CxPropSheet.cpp : Defines the initialization routines for the DLL.
// 所属项目: AutoCx的CxPropSheet工程
// 创建日期: 2001.10.8
// 创 建 人：孙越洋 
// 功能描述：AutoCx系统部件，用于为系统中的各种属性页提供主窗口。 
// 修改记录：
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxPropSheet.h"
#include "PropSheet.h"

#include "../CxOle/CxHmiMisc_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropSheetApp
BEGIN_MESSAGE_MAP(CCxPropSheetApp, CWinApp)
	//{{AFX_MSG_MAP(CPropSheetApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropSheetApp construction

CCxPropSheetApp::CCxPropSheetApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPropSheetApp object
CCxPropSheetApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPropSheetApp initialization

BOOL CCxPropSheetApp::InitInstance()
{
	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Special entry points required for inproc servers

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}

// by exporting DllRegisterServer, you can use regsvr.exe
STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	COleObjectFactory::UpdateRegistryAll();
	return S_OK;
}

/////////////////////////////////////////////////////////////////
//功能:全局属性页生成函数
//参数:同OleCreatePropertyFrame的参数,功能见下面
////////////////////////////////////////////////////////////////
DllExport HRESULT CxCreatePropertyFrame(
  HWND hwndOwner,    //Parent window of property sheet dialog box
  UINT x,            //Horizontal position for dialog box
  UINT y,            //Vertical position for dialog box
  LPCOLESTR lpszCaption,
                     //Pointer to the dialog box caption
  ULONG cObjects,    //Number of object pointers in lplpUnk
  LPUNKNOWN FAR* lplpUnk,
                     //Pointer to the objects for property sheet
  ULONG cPages,      //Number of property pages in lpPageClsID
  LPCLSID lpPageClsID,
                     //Array of CLSIDs for each property page
  LCID lcid,         //Locale identifier for property sheet locale
  DWORD dwReserved,  //Reserved
  LPVOID lpvReserved //Reserved
)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	CString str;
	str.LoadString(IDS_DEFAULTCAPTION);
	CString strCap(lpszCaption);
	CPropSheet* pSheet = new CPropSheet(strCap, CWnd::FromHandle(hwndOwner));
	if(pSheet == NULL)
		return E_FAIL;

	if(!pSheet->AddObjects(cObjects, lplpUnk))
		return E_FAIL;
	if(!pSheet->AddPageCLSID(cPages, lpPageClsID))
		return E_FAIL;

	//use for modalless dialog
	//CWnd* wnd = CWnd::FromHandle(hwndOwner);
	//if(!pSheet->Create(wnd))
	//	return E_FAIL;

	pSheet->m_psh.hwndParent = hwndOwner;
//	pSheet->SetParent(CWnd::FromHandle(hwndOwner));
	return pSheet->DoModal() == IDOK ? S_OK : S_FALSE;
};

