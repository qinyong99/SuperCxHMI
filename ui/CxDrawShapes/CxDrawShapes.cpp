// CxDrawShapes.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f CxDrawShapesps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "CxDrawShapes.h"

#include "CxHmiObj_i.c"
#include "CxHmiMisc_i.c"
#include "CxDrawShapes_i.c"

#include "DrawLine.h"
#include "DrawRectangle.h"
#include "DrawEllipse.h"
#include "DrawRoundRectangle.h"
#include "DrawPolyLine.h"
#include "DrawPipe.h"
#include "DrawPolygon.h"
#include "DrawArc.h"
#include "DrawPie.h"
#include "DrawChord.h"
#include "DrawTxt.h"
#include "DrawDataLink.h"
#include "DrawButton.h"
#include "DrawImage.h"
#include "FloatPoint.h"

#include "ShapeGenPage.h"
#include "PipeGenPage.h"
#include "ButtonGenPage.h"
#include "ImageGenPage.h"
#include "TextGenPage.h"
#include "TextTextPage.h"
#include "DataLinkGenPage.h"
#include "DrawVariable.h"
#include "VariableGenPage.h"
#include "DrawTimer.h"
#include "TimerGenPage.h"
#include "DrawMenu.h"
#include "MenuGenPage.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_DrawLine, CDrawLine)
	OBJECT_ENTRY(CLSID_DrawRectangle, CDrawRectangle)
	OBJECT_ENTRY(CLSID_DrawEllipse, CDrawEllipse)
	OBJECT_ENTRY(CLSID_DrawRoundRectangle, CDrawRoundRectangle)
	OBJECT_ENTRY(CLSID_DrawPolyline, CDrawPolyline)
	OBJECT_ENTRY(CLSID_DrawPipe, CDrawPipe)
	OBJECT_ENTRY(CLSID_DrawPolygon, CDrawPolygon)
	OBJECT_ENTRY(CLSID_DrawArc, CDrawArc)
	OBJECT_ENTRY(CLSID_DrawPie, CDrawPie)
	OBJECT_ENTRY(CLSID_DrawChord, CDrawChord)
	OBJECT_ENTRY(CLSID_DrawTxt, CDrawTxt)
	OBJECT_ENTRY(CLSID_DrawDataLink, CDrawDataLink)
	OBJECT_ENTRY(CLSID_DrawButton, CDrawButton)
	OBJECT_ENTRY(CLSID_DrawImage, CDrawImage)
	OBJECT_ENTRY(CLSID_FloatPoint, CFloatPoint)
	OBJECT_ENTRY(CLSID_CxShapeGenPage, CShapeGenPage)
	OBJECT_ENTRY(CLSID_CxPipeGenPage, CPipeGenPage)
	OBJECT_ENTRY(CLSID_CxButtonGenPage, CButtonGenPage)
	OBJECT_ENTRY(CLSID_CxImageGenPage, CImageGenPage)
	OBJECT_ENTRY(CLSID_CxTextGenPage, CTextGenPage)
	OBJECT_ENTRY(CLSID_CxTextTextPage, CTextTextPage)
	OBJECT_ENTRY(CLSID_CxDataLinkGenPage, CDataLinkGenPage)
	OBJECT_ENTRY(CLSID_DrawVariable, CDrawVariable)
	OBJECT_ENTRY(CLSID_CxVariableGenPage, CVariableGenPage)
	OBJECT_ENTRY(CLSID_DrawTimer, CDrawTimer)
	OBJECT_ENTRY(CLSID_CxTimerGenPage, CTimerGenPage)
	OBJECT_ENTRY(CLSID_DrawMenu, CDrawMenu)
	OBJECT_ENTRY(CLSID_CxMenuGenPage, CMenuGenPage)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_CXDRAWSHAPESLib);
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


