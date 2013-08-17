// InputPasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cxrun.h"
#include "InputPasswordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputPasswordDlg dialog


CInputPasswordDlg::CInputPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputPasswordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputPasswordDlg)
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
}


void CInputPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputPasswordDlg)
	DDX_Text(pDX, IDC_EDIT1, m_strPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputPasswordDlg, CDialog)
	//{{AFX_MSG_MAP(CInputPasswordDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputPasswordDlg message handlers
