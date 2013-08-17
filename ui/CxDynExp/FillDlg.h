// FillDlg.h: interface for the CFillDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __FILLDLG_H_
#define __FILLDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Resource.h"

class CDynamicExpert;

class CFillDlg :
	public CDialogImpl<CFillDlg> 
{
public:
	CFillDlg();
	virtual ~CFillDlg();
	
	enum { IDD = IDD_FILL };
	
	BEGIN_MSG_MAP(CFillDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	virtual void OnShow();
	virtual void OnOK();

	void FillDirectionCombo(BSTR bstrProp);
	
	CDynamicExpert* m_pExpert;
	CComPtr<IUnknown> m_spUnkDynamic;

	CComboBox m_cmbDirection;
};

#endif // __FILLDLG_H_
