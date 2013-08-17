// PushbuttonDataEntry.h: interface for the CPushbuttonDataEntry class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SLIDERENTRYDLG_H_
#define __SLIDERENTRYDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CPushbuttonDataEntry :
public CDialogImpl<CPushbuttonDataEntry>
{
public:
	CPushbuttonDataEntry();
	virtual ~CPushbuttonDataEntry();
	
	enum { IDD = IDD_SLIDER };
	
	BEGIN_MSG_MAP(CPushbuttonDataEntry)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		END_MSG_MAP()
		
		LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
public:
	
};

#endif // __SLIDERENTRYDLG_H_
