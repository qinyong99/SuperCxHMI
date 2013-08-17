// DataSourceDlg.cpp: implementation of the CDataSourceDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataSourceDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataSourceDlg::CDataSourceDlg()
{

}

LRESULT CDataSourceDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CheckDlgButton(IDC_RADIO2, BST_CHECKED);

	CenterWindow();

	return 1;  // Let the system set the focus
}

LRESULT CDataSourceDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled)
{
	if (IsDlgButtonChecked(IDC_RADIO1))
		m_bstrConnectionString = szSuperCxRealtimeConnectionString;
	else
		m_bstrConnectionString = szSuperCxHistoryConnectionString;

	EndDialog(IDOK);
	
	return 0;
}

LRESULT CDataSourceDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
	
	return 0;
}