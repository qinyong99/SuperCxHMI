// CxSheetTest.cpp : Defines the class behaviors for the application.
// 所属项目: AutoCx的CxSheetTest工程
// 创建日期: 2001.10.8
// 创 建 人：孙越洋 
// 功能描述：用于对属性页工程进行测试的主程序类,用于测试。 
// 修改记录：
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxSheetTest.h"
#include "CxSheetTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCxSheetTestApp

BEGIN_MESSAGE_MAP(CCxSheetTestApp, CWinApp)
	//{{AFX_MSG_MAP(CCxSheetTestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCxSheetTestApp construction

CCxSheetTestApp::CCxSheetTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCxSheetTestApp object

CCxSheetTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCxSheetTestApp initialization

BOOL CCxSheetTestApp::InitInstance()
{
	AfxEnableControlContainer();

	OleInitialize(NULL);
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CCxSheetTestDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CCxSheetTestApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	OleUninitialize();
	return CWinApp::ExitInstance();
}
