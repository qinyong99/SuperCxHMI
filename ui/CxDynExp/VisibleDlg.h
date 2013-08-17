// VisibleDlg.h: interface for the CVisibleDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __VISIBLEDLG_H_
#define __VISIBLEDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"

class CDynamicExpert;

class CVisibleDlg  :
	public CDialogImpl<CVisibleDlg> 
{
public:
	CVisibleDlg();
	virtual ~CVisibleDlg();
	
	enum { IDD = IDD_VISIBLE };
	
	BEGIN_MSG_MAP(CVisibleDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	virtual void OnShow();
	virtual void OnOK();
	
	CDynamicExpert* m_pExpert;
	CComPtr<IUnknown> m_spUnkDynamic;
};

#endif // __VISIBLEDLG_H_
