// MovementDlg.h: interface for the CMovementDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __MOVEMENTDLG_H_
#define __MOVEMENTDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Resource.h"

class CDynamicExpert;

class CMovementDlg :
	public CDialogImpl<CMovementDlg> 
{
public:
	CMovementDlg();
	virtual ~CMovementDlg();
	
	enum { IDD = IDD_MOVEMENT };
	
	BEGIN_MSG_MAP(CMovementDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	virtual void OnShow();
	virtual void OnOK();
	
	CDynamicExpert* m_pExpert;
	CComPtr<IUnknown> m_spUnkDynamic;
};

#endif // __MOVEMENTDLG_H_
