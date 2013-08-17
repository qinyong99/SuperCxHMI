// TagPickDlg.cpp: implementation of the CTagPickDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxTagPick.h"
#include "TagPickDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LRESULT CTagPickDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetDlgItemText(IDC_SELECT, m_strPickTag);

	GetDlgControl(IDC_CXTAGPICKCTRL1, IID_IDispatch, (void**)&m_spDispatch);

	CComQIPtr<IPersistPropertyBag> spPersistPropertyBag(m_spDispatch);
	if (spPersistPropertyBag != NULL)
	{
		m_pPickCtrlPropertyBag->LoadFromRegistry();

		CComQIPtr<IPropertyBag> spPropertyBag(m_pPickCtrlPropertyBag->GetUnknown());
		spPersistPropertyBag->Load(spPropertyBag, NULL);
	}

	DispEventAdvise(m_spDispatch);

	CComDispatchDriver dd(m_spDispatch);

	CComVariant var;
	var = m_strPickTag;
	dd.PutPropertyByName(L"Value", &var);

	var = m_bUseLocalSetup ? true : false;
	dd.PutPropertyByName(L"UseLocalSetup", &var);

	var = m_strAssocTagGroup;
	dd.PutPropertyByName(L"AssocGroup", &var);

	var = m_bDisableAssoc ? true : false;
	dd.PutPropertyByName(L"DisableAssoc", &var);

	var = true;
	dd.PutPropertyByName(L"Browsed", &var);

	return 1;
}

LRESULT CTagPickDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CComBSTR bstr;
	GetDlgItemText(IDC_SELECT, (BSTR&)bstr);
	m_strPickTag = bstr;

	EndDialog(wID);
	return 0;	
}

LRESULT CTagPickDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

HRESULT CTagPickDlg::OnChanged()
{
	CComDispatchDriver dd(m_spDispatch);

	CComVariant var;
	dd.GetPropertyByName(L"Value", &var);
	CString strText = var.bstrVal;
	SetDlgItemText(IDC_SELECT, strText);

	return 0;
}

LRESULT CTagPickDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CComQIPtr<IPersistPropertyBag> spPersistPropertyBag(m_spDispatch);
	if (spPersistPropertyBag != NULL)
	{
		CComQIPtr<IPropertyBag> spPropertyBag(m_pPickCtrlPropertyBag->GetUnknown());
		spPersistPropertyBag->Save(spPropertyBag, TRUE, TRUE);

		m_pPickCtrlPropertyBag->SaveToRegistry();
	}

	m_pPickCtrlPropertyBag->Release();
	
	return 0;
}