// UserItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cxrunctrl.h"
#include "UserItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserItemDlg dialog


CUserItemDlg::CUserItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserItemDlg)
	m_strName = _T("");
	m_strDesc = _T("");
	m_strPassword = _T("");
	m_strGroup = _T("标准用户");
	m_nPrivilege = 0;
	//}}AFX_DATA_INIT
}


void CUserItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserItemDlg)
	DDX_Control(pDX, IDC_AREA, m_listArea);
	DDX_Control(pDX, IDC_GROUP, m_cmbGroup);
	DDX_Text(pDX, IDC_USER_NAME, m_strName);
	DDX_Text(pDX, IDC_DESC, m_strDesc);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_GROUP, m_strGroup);
	DDX_Text(pDX, IDC_PRIVILEGE, m_nPrivilege);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserItemDlg, CDialog)
	//{{AFX_MSG_MAP(CUserItemDlg)
	ON_BN_CLICKED(IDC_SELECT_ALL, OnSelectAll)
	ON_BN_CLICKED(IDC_SELECT_NONE, OnSelectNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserItemDlg message handlers

void CUserItemDlg::OnSelectAll() 
{
	int nCount = m_listArea.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		m_listArea.SetCheckState(i, TRUE);
	}
}

void CUserItemDlg::OnSelectNone() 
{
	int nCount = m_listArea.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		m_listArea.SetCheckState(i, FALSE);
	}
}

void CUserItemDlg::LoadSecurityAreas()
{
	m_listArea.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 120, -1);

	for (int i = 0; i < m_parrSecurityAreas->GetSize(); i++)
		m_listArea.InsertItem(i, (*m_parrSecurityAreas)[i]);
}

BOOL CUserItemDlg::OnInitDialog() 
{
	USES_CONVERSION;
	
	CDialog::OnInitDialog();

	m_cmbGroup.AddString(_T("系统管理员"));
	m_cmbGroup.AddString(_T("标准用户"));
	if (!m_strGroup)
		m_strGroup = L"标准用户";

	m_cmbGroup.SelectString(-1, m_strGroup);

	m_listArea.SetCheckStyle();
	LoadSecurityAreas();

	for (int i = 0; i < m_listArea.GetItemCount(); i++)
	{
		CString strText;
		strText = m_listArea.GetItemText(i, 0);
 		if (StrInStrlist(m_strAccessSecrityAreas, strText))
			m_listArea.SetCheckState(i, TRUE);
	} 

	if (m_strName.CompareNoCase(_T("System")) == 0)
	{
		GetDlgItem(IDC_USER_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_GROUP)->EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserItemDlg::OnOK() 
{
	UpdateData();

	m_strName.TrimLeft();
	m_strName.TrimRight();
	if (m_strName.IsEmpty())
	{
		MessageBox("用户名称不能为空。", "错误！", MB_ICONERROR);
		GetDlgItem(IDC_USER_NAME)->SetFocus();
		return;
	}

	m_strAccessSecrityAreas.Empty();
	int nCount = m_listArea.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		if (m_listArea.GetCheckState(i))
		{
			CString strText = m_listArea.GetItemText(i, 0);
			
			if (m_strAccessSecrityAreas.GetLength() > 0)
				m_strAccessSecrityAreas += _T(", ");
			m_strAccessSecrityAreas += strText;
		}
	}

	CDialog::OnOK();
}
