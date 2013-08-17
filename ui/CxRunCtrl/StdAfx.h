#if !defined(AFX_STDAFX_H__B7C6CBDF_352F_4ED5_8D21_C1197618BE2E__INCLUDED_)
#define AFX_STDAFX_H__B7C6CBDF_352F_4ED5_8D21_C1197618BE2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#define _WIN32_WINNT 0x0400
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>

//以下为自己添加的可能用到的头文件
#include <afxpriv.h>
#include <afxctl.h>
#include <afxtempl.h>
#include <afxconv.h>
#include <afxmt.h>
#include <comcat.h>
#include <windowsx.h>
#include <activscp.h>
#include <objsafe.h>
//必须在工程设置/链接/对象库模块中加入shlwapi.lib 
#include <shlwapi.h>

#undef SubclassWindow  // Conflicts with CWnd::SubclassWindow

#include <comdef.h>
//以上为自己添加的可能用到的头文件

#include <math.h>

#include <Gdiplus.h>
using namespace Gdiplus;

#include "ChineseVarUtils.h"

BOOL StrInStrlist(LPCTSTR lpszList, LPCTSTR lpszSub);

#define _SUPERCX_DEVELOP_LICENSE 0

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B7C6CBDF_352F_4ED5_8D21_C1197618BE2E__INCLUDED_)
