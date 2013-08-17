// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__E221AA13_187B_428C_B651_1EC63B7E70E8__INCLUDED_)
#define AFX_STDAFX_H__E221AA13_187B_428C_B651_1EC63B7E70E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include <atlctl.h>

#define _WIN32_IE 0x0400
#include <commctrl.h>
#include <atlwin.h>
#pragma comment (lib, "Comctl32.lib")

#define _WTL_USE_CSTRING

#include <atlapp.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atlsplit.h>
#include <atlmisc.h>


#include "../../svr/CxSac/CxSac.h"
#include "../../svr/CxDatAcs/CxDatDef.h"
#include "../../svr/CxDatAcs/CxDatAcs.h"
#include <atlhost.h>


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E221AA13_187B_428C_B651_1EC63B7E70E8__INCLUDED)
