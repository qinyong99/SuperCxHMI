// FolderNameDlg.cpp : implementation file
//

#include "stdafx.h"

#include "DevInc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFolderNameDlg dialog


CFolderNameDlg::CFolderNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFolderNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFolderNameDlg)
	m_strFolderName = _T("");
	//}}AFX_DATA_INIT
	m_strTitle = _T("ÊäÈë×éÃû");
}


void CFolderNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFolderNameDlg)
	DDX_Text(pDX, IDC_FOLDERNAME, m_strFolderName);
	DDV_MaxChars(pDX, m_strFolderName, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFolderNameDlg, CDialog)
	//{{AFX_MSG_MAP(CFolderNameDlg)
	ON_EN_CHANGE(IDC_FOLDERNAME, OnChangeFoldername)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFolderNameDlg message handlers

void CFolderNameDlg::OnChangeFoldername() 
{
	CString str;
	GetDlgItem(IDC_FOLDERNAME)->GetWindowText(str);
	GetDlgItem(IDOK)->EnableWindow(!str.IsEmpty() && str != m_strFolderName);
}

BOOL CFolderNameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_strTitle);
	CEdit* pWnd = (CEdit*)GetDlgItem(IDC_FOLDERNAME);
	if(pWnd != NULL)
	{
		pWnd->SetSel(0, -1);
	}
	if (m_strFolderName.IsEmpty())
		GetDlgItem(IDOK)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
