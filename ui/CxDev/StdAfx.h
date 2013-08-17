// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__18703A5B_2F8D_11D5_A468_B85CCDCD2108__INCLUDED_)
#define AFX_STDAFX_H__18703A5B_2F8D_11D5_A468_B85CCDCD2108__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#ifdef _RICHEDIT_VER
#undef _RICHEDIT_VER
#endif
#define _RICHEDIT_VER	0x0200
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxrich.h>        //MFC对CRichEidt的支持
#endif // _AFX_NO_AFXCMN_SUPPORT
//以下为自己添加的可能用到的头文件
#include <afxpriv.h>
#include <afxctl.h>
#include <afxtempl.h>
#include <afxconv.h>
#include <comcat.h>
#include <windowsx.h>
#include <activscp.h>
#include <objsafe.h>
//必须在工程设置/链接/对象库模块中加入shlwapi.lib 
#include <shlwapi.h>
#include <atlbase.h>
#undef SubclassWindow  // Conflicts with CWnd::SubclassWindow
#include <comdef.h>

#include  <io.h>
#include <math.h>

#include "ChineseVarUtils.h"

int roundleast(int n);
void RoundRect(LPRECT r1);
void MulDivRect(LPRECT r1, LPRECT r2, int num, int div);

OLECHAR* CoTaskMemAllocString(const OLECHAR* psz);


//以上为自己添加的可能用到的头文件
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__18703A5B_2F8D_11D5_A468_B85CCDCD2108__INCLUDED_)
