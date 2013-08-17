// AlphaNumEntryDlg.h: interface for the CAlphaNumEntryDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ALPHANUMENTRYDLG_H_
#define __ALPHANUMENTRYDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CDataEntry;

typedef CContainedWindowT<CEdit> CContainedEdit;

class CAlphaNumEntryDlg :
	public CDialogImpl<CAlphaNumEntryDlg>
{
public:
	CAlphaNumEntryDlg(CDataEntry* pEntry, BOOL bNumberOnly = TRUE);
	virtual ~CAlphaNumEntryDlg();

	enum { IDD = IDD_NUMERIC };
	
	BEGIN_MSG_MAP(CAlphaNumEntryDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	ALT_MSG_MAP(1) 
//		MESSAGE_HANDLER(WM_CHAR, OnCharEdit)
	END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnCharEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
public:
	CContainedEdit m_edit;
	
	BOOL m_bNumberOnly;
	CDataEntry* m_pDataEntry;
};

#endif // __ALPHANUMENTRYDLG_H_
