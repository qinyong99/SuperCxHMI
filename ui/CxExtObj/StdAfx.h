// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__ACF61CED_3805_11D5_A468_8E961CC7BB0B__INCLUDED_)
#define AFX_STDAFX_H__ACF61CED_3805_11D5_A468_8E961CC7BB0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
#include <atldbcli.h>
#include <atldbsch.h>

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include <atlwin.h>

#include <atlapp.h>
#include <atlmisc.h>
#include <atlctrls.h>
#include <atlctl.h>

#include <activscp.h>

#include "ChineseVarUtils.h"

#include "PropertyPageImpl2.h"

const WCHAR szSuperCxRealtimeConnectionString[] = L"Provider=CxDBProv.RealtimeData.1";
const WCHAR szSuperCxHistoryConnectionString[] = L"Provider=CxDBProv.HistoryData.1";

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__ACF61CED_3805_11D5_A468_8E961CC7BB0B__INCLUDED)
