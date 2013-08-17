// UserManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RunInc.h"
#include "UserManagerDlg.h"
#include "UserItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserManagerDlg dialog


CUserManagerDlg::CUserManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserManagerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUserManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserManagerDlg)
	DDX_Control(pDX, IDC_USERS, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CUserManagerDlg)
	ON_BN_CLICKED(IDC_ADD_USER, OnAddUser)
	ON_BN_CLICKED(IDC_DELETE_USER, OnDeleteUser)
	ON_BN_CLICKED(IDC_MODIFY_USER, OnModifyUser)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_USERS, OnItemchangedUsers)
	ON_NOTIFY(NM_DBLCLK, IDC_USERS, OnDblclkUsers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserManagerDlg message handlers

void CUserManagerDlg::OnAddUser() 
{
	USES_CONVERSION;

	CUserItemDlg dlg;
	CreateUniqueName(_T("newuser"), dlg.m_strName);
	dlg.m_parrSecurityAreas = &m_arrSecurityAreas;
	
	while (1)
	{
		if (dlg.DoModal() != IDOK)
			return;
		
		if (IsUserUniqueName(-1, dlg.m_strName))
			break;
		else
			MessageBox("用户名称重复。", "错误！", MB_ICONERROR);
	}
	
	HRESULT hr = m_pUserManager->SetUserInfo(
		L"", 
		T2CW(dlg.m_strName),
		T2CW(dlg.m_strDesc),
		T2CW(dlg.m_strPassword),
		T2CW(dlg.m_strGroup),
		dlg.m_nPrivilege,
		T2CW(dlg.m_strAccessSecrityAreas));
	if (FAILED(hr))
		return;

	int nItem = m_list.InsertItem(m_list.GetItemCount(), dlg.m_strName);
	m_list.SetItemText(nItem, 1, dlg.m_strDesc);
}

void CUserManagerDlg::OnDeleteUser() 
{
	USES_CONVERSION;
	
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;
	
	int nSel = m_list.GetNextSelectedItem(pos);
	
	CString strItem;
	strItem = m_list.GetItemText(nSel, 0);

	HRESULT hr = m_pUserManager->SetUserInfo(
		T2CW(strItem), 
		L"",
		L"",
		L"",
		L"",
		0,
		L"");
	if (FAILED(hr))
		return;

	m_list.DeleteItem(nSel);
	OnSelchangeUsers();
}

void CUserManagerDlg::OnModifyUser() 
{
	USES_CONVERSION;

	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;

	int nSel = m_list.GetNextSelectedItem(pos);

	CString strItem;
	strItem = m_list.GetItemText(nSel, 0);

	LPWSTR szDesc = NULL;
	LPWSTR szPassword = NULL;
	LPWSTR szGroup = NULL;
	DWORD dwPrivilege;
	LPWSTR szAccessSecrityAreas = NULL;

	HRESULT hr = m_pUserManager->GetUserInfo(
		T2CW(strItem), 
		&szDesc,
		&szPassword,
		&szGroup,
		&dwPrivilege,
		&szAccessSecrityAreas);
	if (FAILED(hr))
		return;
	
	CUserItemDlg dlg;
	dlg.m_strName = strItem;
	dlg.m_strDesc = szDesc;
	dlg.m_strPassword = szPassword;
	dlg.m_strGroup = szGroup;
	dlg.m_nPrivilege = dwPrivilege;
	dlg.m_strAccessSecrityAreas = szAccessSecrityAreas;
	dlg.m_parrSecurityAreas = &m_arrSecurityAreas;

	if (szDesc != NULL)
		CoTaskMemFree(szDesc);
	if (szPassword != NULL)
		CoTaskMemFree(szPassword);
	if (szGroup != NULL)
		CoTaskMemFree(szGroup);
	if (szAccessSecrityAreas != NULL)
		CoTaskMemFree(szAccessSecrityAreas);

	while (1)
	{
		if (dlg.DoModal() != IDOK)
			return;
		
		if (IsUserUniqueName(nSel, dlg.m_strName))
			break;
		else
			MessageBox("用户名称重复。", "错误！", MB_ICONERROR);
	}
	
	hr = m_pUserManager->SetUserInfo(
		T2CW(strItem),
		T2CW(dlg.m_strName),
		T2CW(dlg.m_strDesc),
		T2CW(dlg.m_strPassword),
		T2CW(dlg.m_strGroup),
		dlg.m_nPrivilege,
		T2CW(dlg.m_strAccessSecrityAreas));
	if (FAILED(hr))
		return;
	
 	m_list.SetItemText(nSel, 0, dlg.m_strName);
 	m_list.SetItemText(nSel, 1, dlg.m_strDesc);
}

BOOL CUserManagerDlg::OnInitDialog() 
{
	USES_CONVERSION;

	CDialog::OnInitDialog();
	
	if (m_pUserManager == NULL)
		return FALSE;

	m_SmallImageList.Create(IDB_SMALLICONS, 16, 1, RGB(0, 128, 128));
	m_list.SetImageList(&m_SmallImageList, LVSIL_SMALL);

	m_list.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 120, -1);
	m_list.InsertColumn(1, _T("描述"), LVCFMT_LEFT, 130, -1);

	CComPtr<IEnumString> spEnumString;
	CComPtr<IEnumString> spEnumDesc;
	m_pUserManager->BrowseUsers(&spEnumString, &spEnumDesc);

	int nItem = 0;
	LPOLESTR pszItem;
	while (spEnumString->Next(1, &pszItem, NULL) == S_OK)
	{
		m_list.InsertItem(nItem, OLE2T(pszItem), 0);

		spEnumDesc->Next(1, &pszItem, NULL);
		m_list.SetItemText(nItem, 1, OLE2T(pszItem));

		nItem++;
	}

	OnSelchangeUsers();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserManagerDlg::OnOK() 
{
	if (FAILED(m_pUserManager->SaveUsersInfo()))
		MessageBox(_T("可能其它程序正在使用用户信息。"), _T("用户信息保存失败"), MB_ICONERROR);
			
	CDialog::OnOK();
}

BOOL CUserManagerDlg::IsUserUniqueName(int nItem, LPCTSTR lpszName)
{
	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		if (nItem != i)
		{
			CString strItem;
			strItem = m_list.GetItemText(i, 0);
			if (strItem.CompareNoCase(lpszName) == 0)
				return FALSE;
		}
	}

	return TRUE;
}

void CUserManagerDlg::CreateUniqueName(LPCTSTR lpszBaseName, CString& strName)
{
	long lTag;
	BOOL bCollisionFound;
	
	//假定名字已经唯一
	strName.Format(_T("%s"), lpszBaseName);
	
	lTag = 1;
	do
	{
		bCollisionFound = FALSE;
		if (!IsUserUniqueName(-1, strName))
		{
			bCollisionFound = TRUE;
			strName.Format(_T("%s%d"), lpszBaseName, lTag);
			lTag++;
		}
		
	} while (bCollisionFound);
}

void CUserManagerDlg::OnSelchangeUsers() 
{
	int nSel = -1;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos != NULL)
		nSel = m_list.GetNextSelectedItem(pos);

	GetDlgItem(IDC_MODIFY_USER)->EnableWindow(nSel != -1);
	GetDlgItem(IDC_DELETE_USER)->EnableWindow(nSel != -1 && nSel != 0);
}

void CUserManagerDlg::OnItemchangedUsers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if (pNMListView->uNewState & LVIS_SELECTED)
		OnSelchangeUsers();
	
	*pResult = 0;
}

void CUserManagerDlg::OnDblclkUsers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnModifyUser();
	
	*pResult = 0;
}
