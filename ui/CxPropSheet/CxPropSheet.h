// CxPropSheet.h : main header file for the CXPROPSHEET DLL
// 所属项目: AutoCx的CxPropSheet工程
// 创建日期: 2001.10.8
// 创 建 人：孙越洋 
// 功能描述：AutoCx系统部件，用于为系统中的各种属性页提供主窗口。 
// 修改记录：
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CXPROPSHEET_H__CE30370F_FE4B_4528_AB1B_AFD7275FCD87__INCLUDED_)
#define AFX_CXPROPSHEET_H__CE30370F_FE4B_4528_AB1B_AFD7275FCD87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )

/////////////////////////////////////////////////////////////////////////////
// CCxPropSheetApp
// See CxPropSheet.cpp for the implementation of this class
//
#include "..\CxOle\CxHmiObj.h"
#include "..\CxOle\CxHmiMisc.h"
class CCxPropSheetApp : public CWinApp
{
public:
	CCxPropSheetApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCxPropSheetApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCxPropSheetApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CXPROPSHEET_H__CE30370F_FE4B_4528_AB1B_AFD7275FCD87__INCLUDED_)
