// ShowPictureDlg.h: interface for the CShowPictureDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SHOWPICTUREDLG_H_
#define __SHOWPICTUREDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"

class CEventExpert;

class CShowPictureDlg  :
	public CDialogImpl<CShowPictureDlg> 
{
public:
	CShowPictureDlg();
	virtual ~CShowPictureDlg();
	
	enum { IDD = IDD_SHOWPICTURE };
	
	BEGIN_MSG_MAP(CShowPictureDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_SELECT_PICTURE, BN_CLICKED, OnClickedSelectPicture)
		COMMAND_HANDLER(IDC_PICTURE, EN_CHANGE, OnChangePicture)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedSelectPicture(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangePicture(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	virtual void OnShow();
	virtual void OnOK();

	CEventExpert* m_pExpert;

	CString m_strPictureFile;
	CEdit m_editPicture;

};

#endif // __SHOWPICTUREDLG_H_
