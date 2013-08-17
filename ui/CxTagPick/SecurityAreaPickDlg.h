// SecurityAreaPickDlg.h: interface for the CSecurityAreaPickDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECURITYAREAPICKDLG_H__818DDC7E_40E6_452B_9B12_FC006A06D3DB__INCLUDED_)
#define AFX_SECURITYAREAPICKDLG_H__818DDC7E_40E6_452B_9B12_FC006A06D3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

class CSecurityAreaPickDlg : public CDialogImpl<CSecurityAreaPickDlg>  
{
public:
	CSecurityAreaPickDlg(BOOL bUseLocalSetup = TRUE);
	virtual ~CSecurityAreaPickDlg();

	enum {IDD = IDD_SECURITY_AREA};
	
	BEGIN_MSG_MAP(CSecurityAreaPickDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_SELECT_ALL, BN_CLICKED, OnClickedSelectAll)
		COMMAND_HANDLER(IDC_SELECT_NONE, BN_CLICKED, OnClickedSelectNone)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedSelectAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedSelectNone(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	HRESULT LoadSecurityAreaFormServer();
	HRESULT LoadSecurityAreaFormSetup();

	ICxDataServer* GetDataServer();
	
	CComPtr<ICxDataServer> m_spDataServer;
	CComBSTR m_bstrComputer; //¼ÆËã¼ÆÃû³Æ

	CCheckListViewCtrl m_listArea;
	CSimpleArray<CComBSTR> m_arrAreas;

	CString m_strPickArea;
	BOOL m_bUseLocalSetup;
};

#endif // !defined(AFX_SECURITYAREAPICKDLG_H__818DDC7E_40E6_452B_9B12_FC006A06D3DB__INCLUDED_)
