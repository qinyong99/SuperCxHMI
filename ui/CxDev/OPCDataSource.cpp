// OPCDataSource.cpp : implementation file
//

#include "stdafx.h"
#include "cxdev.h"
#include "OPCDataSource.h"
#include "OPCServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COPCDataSource dialog


COPCDataSource::COPCDataSource(CWnd* pParent /*=NULL*/)
	: CDialog(COPCDataSource::IDD, pParent)
{
	//{{AFX_DATA_INIT(COPCDataSource)
	//}}AFX_DATA_INIT
}


void COPCDataSource::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COPCDataSource)
	DDX_Control(pDX, IDC_LIST1, m_lbName);
	DDX_Control(pDX, IDC_EDIT1, m_editOPCServer);
	DDX_Control(pDX, IDC_EDIT2, m_editComputer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COPCDataSource, CDialog)
	//{{AFX_MSG_MAP(COPCDataSource)
	ON_BN_CLICKED(IDC_ADD_SERVER, OnAddServer)
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDC_REMOVE_SERVER, OnRemoveServer)
	ON_BN_CLICKED(IDC_EDIT_SERVER, OnEditServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COPCDataSource message handlers

void COPCDataSource::OnAddServer() 
{
	COPCServerDlg dlg;
	dlg.m_pOPCDataSource = this;

	CreateUniqueSeverName(dlg.m_strName);
	int nRet = dlg.DoModal();
	if (nRet != IDOK)
		return;

	COPCServerItem* pItem = new COPCServerItem;
	pItem->m_strName = dlg.m_strName;
	pItem->m_strOPCServer = dlg.m_strOPCServer;
	pItem->m_strComputer = dlg.m_strComputer;
	int iIndex = m_lbName.AddString(dlg.m_strName);
	m_lbName.SetItemData(iIndex, DWORD(pItem));
	
	m_lbName.SetCurSel(iIndex);
	OnSelchangeList1();
}

BOOL COPCDataSource::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CIni& ini = theApp.m_iniOpcDataSource;
	
	CString* pNames = NULL;
	int nCount = ini.GetSectionNames(&pNames);
	for (int i = 0; i < nCount; i++)
	{
		COPCServerItem* pItem = new COPCServerItem;
		pItem->m_strName = pNames[i];
		ini.SetSection(pItem->m_strName);
		pItem->m_strOPCServer = ini.GetString(CString(_T("OPCSERVER")));
		pItem->m_strComputer = ini.GetString(CString(_T("COMPUTER")));

		int iIndex = m_lbName.AddString(pItem->m_strName);
		m_lbName.SetItemData(iIndex, DWORD(pItem));
	}
	delete [] pNames;

	if (m_lbName.GetCount() > 0)
	{
		m_lbName.SetCurSel(0);
		OnSelchangeList1();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COPCDataSource::OnDestroy() 
{
	CDialog::OnDestroy();
	
	int nCount = m_lbName.GetCount();
	for (int iIndex = 0; iIndex < nCount; iIndex++)
		delete (COPCServerItem *)m_lbName.GetItemData(iIndex);
}

void COPCDataSource::OnOK() 
{
	CIni& ini = theApp.m_iniOpcDataSource;

	//删除所有的区
	CString* pNames = NULL;
	int nDevCount = ini.GetSectionNames(&pNames);
	for (int i = 0; i < nDevCount; i++)
	{
		ini.DeleteSection(pNames[i]);
	}
	delete [] pNames;

	int nCount = m_lbName.GetCount();
	for (int iIndex = 0; iIndex < nCount; iIndex++)
	{
		COPCServerItem* pItem = (COPCServerItem *)m_lbName.GetItemData(iIndex);
		ini.SetSection(pItem->m_strName);
		ini.WriteString(CString(_T("OPCSERVER")), pItem->m_strOPCServer);
		ini.WriteString(CString(_T("COMPUTER")), pItem->m_strComputer);
	}

	CDialog::OnOK();
}

void COPCDataSource::OnSelchangeList1() 
{
	int iIndex = m_lbName.GetCurSel();
	if (iIndex != LB_ERR)
	{
		COPCServerItem* pItem = (COPCServerItem *)m_lbName.GetItemData(iIndex);
		m_editOPCServer.SetWindowText(pItem->m_strOPCServer);
		m_editComputer.SetWindowText(pItem->m_strComputer);
	}
	else
	{
		m_editOPCServer.SetWindowText(_T(""));
		m_editComputer.SetWindowText(_T(""));
	}
}

void COPCDataSource::OnRemoveServer() 
{
	int iIndex = m_lbName.GetCurSel();
	delete (COPCServerItem *)m_lbName.GetItemData(iIndex);
	m_lbName.DeleteString(iIndex);

	if (iIndex > 1)
		m_lbName.SetCurSel(iIndex - 1);
	else if (m_lbName.GetCount() > 0)
		m_lbName.SetCurSel(0);

	OnSelchangeList1();
}

void COPCDataSource::OnEditServer() 
{
	int iIndex = m_lbName.GetCurSel();
	if (iIndex == LB_ERR)
		return;

	COPCServerItem* pItem = (COPCServerItem *)m_lbName.GetItemData(iIndex);
	m_lbName.SetItemData(iIndex, NULL);

	COPCServerDlg dlg(FALSE);
	dlg.m_pOPCDataSource = this;

	dlg.m_strName = pItem->m_strName;
	dlg.m_strOPCServer = pItem->m_strOPCServer;
	dlg.m_strComputer = pItem->m_strComputer;
	
	int nRet = dlg.DoModal();
	if (nRet != IDOK)
	{
		m_lbName.SetItemData(iIndex, DWORD(pItem));
		return;
	}

	pItem->m_strName = dlg.m_strName;
	pItem->m_strOPCServer = dlg.m_strOPCServer;
	pItem->m_strComputer = dlg.m_strComputer;

	m_lbName.DeleteString(iIndex);
	iIndex = m_lbName.InsertString(iIndex, dlg.m_strName);
	m_lbName.SetItemData(iIndex, DWORD(pItem));

	m_lbName.SetCurSel(iIndex);
	OnSelchangeList1();
}

