// DataEntryDlg.h: interface for the CDataEntryDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DATAENTRYDLG_H_
#define __DATAENTRYDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "TabPage.h"
#include "NumericEntryDlg.h"
#include "PushbuttonEntryDlg.h"

#define CNumericEntryPage CTabPageImpl<CNumericEntryDlg>
#define CPushbuttonEntryPage CTabPageImpl<CPushbuttonEntryDlg>

class CEventExpert;

class CDataEntryDlg  :
	public CDialogImpl<CDataEntryDlg>,
	public CWinDataExchange<CDataEntryDlg>
{
public:
	CDataEntryDlg();
	virtual ~CDataEntryDlg();
	
	enum { IDD = IDD_DATAENTRY };
	
	BEGIN_MSG_MAP(CDataEntryDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_RANGE_HANDLER(IDC_RADIO1, IDC_RADIO4, OnClickedEntryMode)
		COMMAND_HANDLER(IDC_VAR, BN_CLICKED, OnClickedVar)
		COMMAND_HANDLER(IDC_EXPRESSION, EN_CHANGE, OnChangeExpression)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDataEntryDlg)
		DDX_RADIO(IDC_RADIO1, m_nMode)
	END_DDX_MAP()
			
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedEntryMode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedVar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeExpression(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		
	void ActiveEntryModePage(int nMode);

	virtual void OnFinalMessage(HWND hWnd);

	virtual void OnShow();
	virtual void OnOK();
	
	CEventExpert* m_pExpert;

	CTabPage* m_apPages[4];
	int m_nMode;

};

#endif // __DATAENTRYDLG_H_
