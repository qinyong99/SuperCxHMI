// TabOrderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DevInc.h"
#include "TabOrderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabOrderDlg dialog


CTabOrderDlg::CTabOrderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabOrderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabOrderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pObjList = NULL;
}


void CTabOrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabOrderDlg)
	DDX_Control(pDX, IDC_LIST, m_listOrder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabOrderDlg, CDialog)
	//{{AFX_MSG_MAP(CTabOrderDlg)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_LBN_SELCHANGE(IDC_LIST, OnSelchangeList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabOrderDlg message handlers

BOOL CTabOrderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_pObjList != NULL)
	{
		int nIndex = 0;
		while (1)
		{
			BOOL bFind = FALSE;

			POSITION pos = m_pObjList->GetHeadPosition();
			while (pos != NULL)
			{
				CDevObj* pObj = (CDevObj *)m_pObjList->GetNext(pos);
				if (pObj->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
				{
					CCtrlObj* pCtrlObj = (CCtrlObj *)pObj;
					if (pCtrlObj->m_nTabIndex == nIndex)
					{
						m_listOrder.InsertString(nIndex, pCtrlObj->GetDisplayName());
						ASSERT(nIndex != LB_ERR);
						m_listOrder.SetItemData(nIndex, (DWORD)pCtrlObj);

						bFind = TRUE;
						break;
					}
				}
			}

			if (!bFind)
				break;

			nIndex++;
		}
	}

	if (m_listOrder.GetCount())
		m_listOrder.SetCurSel(0);
	UpdateButtonState();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabOrderDlg::OnOK() 
{
	for (int i = 0; i < m_listOrder.GetCount(); i++)
	{
		CCtrlObj* pCtrlObj = (CCtrlObj *)m_listOrder.GetItemData(i);
		pCtrlObj->m_nTabIndex = i;
	}
	
	CDialog::OnOK();
}

void CTabOrderDlg::OnUp() 
{
	int nSel = m_listOrder.GetCurSel();
	if (nSel != LB_ERR)
	{
		if (nSel > 0)
		{
			CString strItem;
			m_listOrder.GetText(nSel, strItem);
			DWORD dwData = m_listOrder.GetItemData(nSel);
			m_listOrder.DeleteString(nSel);
			nSel--; 
			int nIndex = m_listOrder.InsertString(nSel, strItem);
			ATLASSERT(nIndex == nSel);
			m_listOrder.SetItemData(nIndex, dwData);
			m_listOrder.SetCurSel(nIndex);
		}
	}

	UpdateButtonState();
}

void CTabOrderDlg::OnDown() 
{
	int nSel = m_listOrder.GetCurSel();
	if (nSel != LB_ERR)
	{
		if (nSel < m_listOrder.GetCount() - 1)
		{
			CString strItem;
			m_listOrder.GetText(nSel, strItem);
			DWORD dwData = m_listOrder.GetItemData(nSel);
			m_listOrder.DeleteString(nSel);
			nSel++; 
			int nIndex = m_listOrder.InsertString(nSel, strItem);
			ATLASSERT(nIndex == nSel);
			m_listOrder.SetItemData(nIndex, dwData);
			m_listOrder.SetCurSel(nIndex);
		}
	}

	UpdateButtonState();
}

void CTabOrderDlg::UpdateButtonState()
{
	int nSel = m_listOrder.GetCurSel();
	if (nSel != LB_ERR)
	{
		GetDlgItem(IDC_UP)->EnableWindow(nSel > 0);
		GetDlgItem(IDC_DOWN)->EnableWindow(nSel < m_listOrder.GetCount() - 1);
	}
	else
	{
		GetDlgItem(IDC_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_DOWN)->EnableWindow(FALSE);
	}
}

void CTabOrderDlg::OnSelchangeList() 
{
	UpdateButtonState();
}
