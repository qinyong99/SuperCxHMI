// PushbuttonEntryDlg.h: interface for the CPushbuttonEntryDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __PUSHBUTTONENTRYDLG_H_
#define __PUSHBUTTONENTRYDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CDataEntry;

class CPushbuttonEntryDlg :
	public CDialogImpl<CPushbuttonEntryDlg>
{
public:
	CPushbuttonEntryDlg(CDataEntry* pEntry);
	virtual ~CPushbuttonEntryDlg();
	
	enum { IDD = IDD_PUSHBUTTON };
	
	BEGIN_MSG_MAP(CPushbuttonEntryDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_OPEN, OnClickedOpen)
		COMMAND_ID_HANDLER(IDC_CLOSE, OnClickedClose)
	END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
public:
	CDataEntry* m_pDataEntry;

	CButton m_btnOpen;
	CButton m_btnClose;
};

#endif // __PUSHBUTTONENTRYDLG_H_

