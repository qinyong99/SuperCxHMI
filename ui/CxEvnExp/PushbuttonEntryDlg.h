// PushbuttonEntryDlg.h: interface for the CPushbuttonEntryDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __PUSHBUTTONENTRYDLG_H_
#define __PUSHBUTTONENTRYDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"

class CEventExpert;

class CPushbuttonEntryDlg  :
	public CDialogImpl<CPushbuttonEntryDlg>  
{
public:
	CPushbuttonEntryDlg();
	
	enum { IDD = IDD_PUSHBUTTONENTRY };
	
	BEGIN_MSG_MAP(CPushbuttonEntryDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_OPEN, EN_CHANGE, OnChangeCapion)
		COMMAND_HANDLER(IDC_CLOSE, EN_CHANGE, OnChangeCapion)
	END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnChangeCapion(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	virtual void OnShow();
	virtual void OnOK();

	CEdit m_editOpen;
	CEdit m_editClose;

	CString m_strScript;

	CEventExpert* m_pExpert;
};

#endif // __PUSHBUTTONENTRYDLG_H_
