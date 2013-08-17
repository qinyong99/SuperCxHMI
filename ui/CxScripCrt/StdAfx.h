// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__B135ACC6_388A_400E_96E7_2B200954031D__INCLUDED_)
#define AFX_STDAFX_H__B135ACC6_388A_400E_96E7_2B200954031D__INCLUDED_

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

#include <atldbcli.h>
#include <atldbsch.h>

#include <atlapp.h>
#include <AtlMisc.h>
#include <atlwin.h>
#include <AtlCtrls.h>
#include <AtlCtrlW.h>
#include <AtlCtrlx.h>
#include <AtlSplit.h>
#include <atlframe.h>
#include <AtlDdx.h>
#include <AtlHost.h>

#include <activscp.h>

#include "atlcoll.h"
#include <atlctl.h>

#include "ChineseVarUtils.h"

typedef CAtlArray<CString> CStringArray;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B135ACC6_388A_400E_96E7_2B200954031D__INCLUDED)
