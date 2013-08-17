// ServerPromptModifiedDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CxRun.h"
#include "ServerPromptModifiedDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerPromptModifiedDlg dialog


CServerPromptModifiedDlg::CServerPromptModifiedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerPromptModifiedDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerPromptModifiedDlg)
	m_bNotPrompt = FALSE;
	//}}AFX_DATA_INIT
}


void CServerPromptModifiedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerPromptModifiedDlg)
	DDX_Check(pDX, IDC_CHECK1, m_bNotPrompt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerPromptModifiedDlg, CDialog)
	//{{AFX_MSG_MAP(CServerPromptModifiedDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerPromptModifiedDlg message handlers
