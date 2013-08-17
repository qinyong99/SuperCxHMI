// NumericEntryDlg.h: interface for the CNumericEntryDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __NUMERICENTRYDLG_H_
#define __NUMERICENTRYDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"

class CEventExpert;

typedef CContainedWindowT<CEdit> CContainedEdit;

class CNumericEntryDlg  :
	public CDialogImpl<CNumericEntryDlg>  
{
public:
	CNumericEntryDlg();
	
	enum { IDD = IDD_NUMERICENTRY };
	
	BEGIN_MSG_MAP(CNumericEntryDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_MAX, EN_CHANGE, OnChangeLimit)
		COMMAND_HANDLER(IDC_MIN, EN_CHANGE, OnChangeLimit)
		COMMAND_HANDLER(IDC_FETCH_LIMIT, BN_CLICKED, OnClickedFetchLimit)
	ALT_MSG_MAP(1) 
//		MESSAGE_HANDLER(WM_CHAR, OnCharEditMax)
	ALT_MSG_MAP(2) 
//		MESSAGE_HANDLER(WM_CHAR, OnCharEditMin)
	END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnChangeLimit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedFetchLimit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnCharEditMax(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCharEditMin(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	virtual void OnShow();
	virtual void OnOK();

	CEventExpert* m_pExpert;

	CContainedEdit m_editMax;
	CContainedEdit m_editMin;
	CButton m_btnFetchLimit;

	CString m_strScript;

};

#endif // __NUMERICENTRYDLG_H_
