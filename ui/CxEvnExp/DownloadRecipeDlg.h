// DownloadRecipeDlg.h: interface for the CDownloadRecipeDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DOWNLOADRECIPEDLG_H_
#define __DOWNLOADRECIPEDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"

class CEventExpert;

class CDownloadRecipeDlg  :
	public CDialogImpl<CDownloadRecipeDlg> 
{
public:
	CDownloadRecipeDlg();
	virtual ~CDownloadRecipeDlg();
	
	enum { IDD = IDD_DOWNLOAD_RECIPE };
	
	BEGIN_MSG_MAP(CDownloadRecipeDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_SELECT_RECIPE, BN_CLICKED, OnClickedSelect)
		COMMAND_HANDLER(IDC_RECIPE, EN_CHANGE, OnChange)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedSelect(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	virtual void OnShow();
	virtual void OnOK();

	CEventExpert* m_pExpert;

	CString m_strFile;
	CEdit m_edit;
};

#endif // __DOWNLOADRECIPEDLG_H_
