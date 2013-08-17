// DataSourceDlg.h: interface for the CDataSourceDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATASOURCEDLG_H__0AFDF9CC_0A30_4EF7_910D_EAADE0E4169D__INCLUDED_)
#define AFX_DATASOURCEDLG_H__0AFDF9CC_0A30_4EF7_910D_EAADE0E4169D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

//EXTERN_C const WCHAR szSuperCxRealtimeConnectionString[];
EXTERN_C const WCHAR szSuperCxHistoryConnectionString[];

class CDataSourceDlg :
	public CDialogImpl<CDataSourceDlg>  
{
public:
	CDataSourceDlg();

	enum { IDD = IDD_DATA_SOURCE };
	
	BEGIN_MSG_MAP(CDataSourceDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);

	CComBSTR m_bstrConnectionString;
};

#endif // !defined(AFX_DATASOURCEDLG_H__0AFDF9CC_0A30_4EF7_910D_EAADE0E4169D__INCLUDED_)
