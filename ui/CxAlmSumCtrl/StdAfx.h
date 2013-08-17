// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__7DA031E5_ACBE_4729_9A47_86298D3A6D47__INCLUDED_)
#define AFX_STDAFX_H__7DA031E5_ACBE_4729_9A47_86298D3A6D47__INCLUDED_

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
#include <atlmisc.h>
#include <atlgdi.h>
#include <atlctrls.h>
#include <atlctrlx.h>

#include "../../svr/CxSAC/CxSAC.h"
#include "../../svr/CxDatAcs/CxDatDef.h"
#include "../../svr/CxDatAcs/CxDatAcs.h"
#include "../../svr/CxDatSes/CxDatSes.h"

BOOL StrInStrlist(const OLECHAR *szList, const OLECHAR *szSub);

HRESULT GetDefaultFont(IFontDisp** ppFont);
int GetFontHeight(IFontDisp* pFontDisp);
BOOL PickFont(HWND hWndParant, IFontDisp** ppFont);
HRESULT SaveFontToStream(IFontDisp* pFont, IStream* pStm);
HRESULT ReadFontFromStream(IFontDisp** ppFont, IStream* pStm);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7DA031E5_ACBE_4729_9A47_86298D3A6D47__INCLUDED)
