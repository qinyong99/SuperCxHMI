// CxExtObj.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f CxExtObjps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "CxExtObj.h"

#include "CxHmiObj_i.c"
#include "CxExtObj_i.c"
#include "EventObj.h"
#include "DataEntry.h"
#include "ReturnInteger.h"
#include "BlankPage.h"
#include "DataConnection.h"
#include "DataRecordset.h"
#include "DataField.h"
#include "DataRecordsetPage.h"
#include "DataConnectionPage.h"
#include "SQLBuilder.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_CxEventObj, CEventObj)
OBJECT_ENTRY(CLSID_CxDataEntry, CDataEntry)
OBJECT_ENTRY(CLSID_CxReturnInteger, CReturnInteger)
OBJECT_ENTRY(CLSID_CxBlankPage, CBlankPage)
OBJECT_ENTRY(CLSID_CxDataConnection, CDataConnection)
OBJECT_ENTRY(CLSID_CxDataRecordset, CDataRecordset)
OBJECT_ENTRY(CLSID_CxDataField, CDataField)
OBJECT_ENTRY(CLSID_CxDataRecordsetPage, CDataRecordsetPage)
OBJECT_ENTRY(CLSID_CxDataConnectionPage, CDataConnectionPage)
OBJECT_ENTRY(CLSID_CxSQLBuilder, CSQLBuilder)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_CXEXTOBJLib);
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


