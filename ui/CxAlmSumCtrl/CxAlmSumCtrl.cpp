// CxAlmSumCtrl.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f CxAlmSumCtrlps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "CxAlmSumCtrl.h"

#include "CxHmiObj_i.c"
#include "CxHmiMisc_i.c"
#include "CxAlmSumCtrl_i.c"

#include "../../svr/CxDatAcs/CxDatAcs_i.c"
#include "../../svr/CxSac/CxSac_i.c"
#include "../../svr/CxDatSes/CxDatSes_i.c"

#include "DrawAlmSum.h"
#include "AlmSumCtrl.h"

#include "ColumPage.h"
#include "FormatPage.h"
#include "GenPage.h"
#include "FilterPage.h"
#include "OperatorPage.h"
#include "SortPage.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_DrawAlmSum, CDrawAlmSum)
OBJECT_ENTRY(CLSID_AlmSumCtrl, CAlmSumCtrl)
OBJECT_ENTRY(CLSID_CxColumnAlmSumPage, CColumPage)
OBJECT_ENTRY(CLSID_CxFormatAlmSumPage, CFormatPage)
OBJECT_ENTRY(CLSID_CxGenAlmSumPage, CGenPage)
OBJECT_ENTRY(CLSID_CxFilterAlmSumPage, CFilterPage)
OBJECT_ENTRY(CLSID_CxOperatorAlmSumPage, COperatorPage)
OBJECT_ENTRY(CLSID_CxSortAlmSumPage, CSortPage)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_CXALMSUMCTRLLib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}


