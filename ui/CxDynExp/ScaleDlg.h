// ScaleDlg.h: interface for the CScaleDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SCALEDLG_H_
#define __SCALEDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Resource.h"

class CDynamicExpert;

class CScaleDlg :
	public CDialogImpl<CScaleDlg> 
{
public:
	CScaleDlg();
	virtual ~CScaleDlg();
	
	enum { IDD = IDD_SCALE };
	
	BEGIN_MSG_MAP(CScaleDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	virtual void OnShow();
	virtual void OnOK();

	void FillBasePointCombo();
	
	CDynamicExpert* m_pExpert;
	CComPtr<IUnknown> m_spUnkDynamic;

	CComboBox m_cmbBase;
};

#endif // __SCALEDLG_H_
