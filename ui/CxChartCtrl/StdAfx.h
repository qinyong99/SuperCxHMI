// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__4DBF4CA7_30B3_4455_81CA_F2F48C605B93__INCLUDED_)
#define AFX_STDAFX_H__4DBF4CA7_30B3_4455_81CA_F2F48C605B93__INCLUDED_

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
#include <atlapp.h>
#include <atlgdi.h>
#include <atlmisc.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atldlgs.h>
#include <atlprint.h>
#define _ATL_USE_DDX_FLOAT
#include <atlddx.h>

#define ROUND(f) (int)((f) >=0 ? f + .4999999 : f - .4999999) 

#include <math.h>

HRESULT GetDefaultFont(IFontDisp** ppFont);
int GetFontHeight(IFontDisp* pFontDisp);
BOOL PickFont(HWND hWndParant, IFontDisp** ppFont);
HRESULT SaveFontToStream(IFontDisp* pFont, IStream* pStm);
HRESULT ReadFontFromStream(IFontDisp** ppFont, IStream* pStm);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4DBF4CA7_30B3_4455_81CA_F2F48C605B93__INCLUDED)
