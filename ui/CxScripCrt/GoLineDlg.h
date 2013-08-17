// GoLineDlg.h: interface for the CGoLineDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _GOLINEDLG_H__
#define _GOLINEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"

class CGoLineDlg :
	public CDialogImpl<CGoLineDlg>,
	public CWinDataExchange<CGoLineDlg>   
{
public:
	CGoLineDlg();

	enum { IDD = IDD_GOLINE };
	
	BEGIN_MSG_MAP(CGoLineDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CGoLineDlg)
		DDX_INT(IDC_EDIT1, m_nLine);
	END_DDX_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);

	int	m_nLine;
	int m_nLineCount;
};

#endif // _GOLINEDLG_H__
