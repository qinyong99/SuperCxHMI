// DcomErrorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CxRun.h"
#include "DcomErrorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDcomErrorDlg dialog


CDcomErrorDlg::CDcomErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDcomErrorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDcomErrorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDcomErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDcomErrorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDcomErrorDlg, CDialog)
	//{{AFX_MSG_MAP(CDcomErrorDlg)
	ON_BN_CLICKED(IDRETRY, OnRetry)
	ON_BN_CLICKED(IDIGNORE, OnIgnore)
	ON_BN_CLICKED(IDABORT, OnAbort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDcomErrorDlg message handlers

void CDcomErrorDlg::OnRetry() 
{
	EndDialog(IDRETRY);
}

void CDcomErrorDlg::OnIgnore() 
{
	EndDialog(IDIGNORE);
}

void CDcomErrorDlg::OnAbort() 
{
	EndDialog(IDABORT);
}
