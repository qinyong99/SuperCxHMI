// ScriptErrorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CxRunCtrl.h"
#include "ScriptErrorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptErrorDlg dialog


CScriptErrorDlg::CScriptErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScriptErrorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScriptErrorDlg)
	m_strInfoText = _T("");
	//}}AFX_DATA_INIT
}


void CScriptErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScriptErrorDlg)
	DDX_Text(pDX, IDC_INFO, m_strInfoText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScriptErrorDlg, CDialog)
	//{{AFX_MSG_MAP(CScriptErrorDlg)
	ON_BN_CLICKED(ID_IGNORE, OnIgnore)
	ON_BN_CLICKED(ID_IGNORE_ALL, OnIgnoreAll)
	ON_BN_CLICKED(ID_BREAK, OnBreak)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptErrorDlg message handlers

void CScriptErrorDlg::AddErrorInfoMessage(CString strInfo)
{
	m_arrInfoText.Add(strInfo);
}

void CScriptErrorDlg::OnIgnore() 
{
	if (m_arrInfoText.GetSize() == 0)
	{
		EndDialog(ID_IGNORE);
		return;
	}

	m_strInfoText = m_arrInfoText[0];
	m_arrInfoText.RemoveAt(0);

	UpdateData(FALSE);
}

void CScriptErrorDlg::OnIgnoreAll() 
{
	EndDialog(ID_IGNORE_ALL);
}

void CScriptErrorDlg::OnBreak() 
{
	EndDialog(ID_BREAK);
}

BOOL CScriptErrorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ATLASSERT(m_arrInfoText.GetSize() > 0);

	m_strInfoText = m_arrInfoText[0];
	m_arrInfoText.RemoveAt(0);
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
