#include "stdafx.h"
#include "ColorDlg.h"
#include "DynamicExpert.h"
#include "ItemEditDialog.h"

///////////////////////////////////////////////////////////////
//功能:设置对话框初始参数
//参数:系统定义
///////////////////////////////////////////////////////////////
LRESULT CItemEditDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HelperSetText(IDC_ITEM_INPUT1, m_pEditItem->m_vaInput1);
	HelperSetText(IDC_ITEM_INPUT2, m_pEditItem->m_vaInput2);

	m_winOutput.Attach(GetDlgItem(IDC_OUTPUT));

	CComPtr<IDispatch> spDispatch;
	m_winOutput.QueryControl(&spDispatch);

	CComDispatchDriver dd(spDispatch);
	CComVariant var = long(m_nOutputType);
	dd.PutProperty(1, &var);
	
	if (m_nOutputType == PROPTYPE_ENUM || m_nOutputType == PROPTYPE_ENUMSTRING)
	{
		ATLASSERT(m_pArrEnumPropItems != NULL);
		for (int i = 0; i < m_pArrEnumPropItems->GetSize(); i++)
		{
			CComVariant var1 = (*m_pArrEnumPropItems)[i].m_bstrDesc;
			CComVariant var2 = (*m_pArrEnumPropItems)[i].m_varValue;
			dd.Invoke2(3, &var1, &var2);
		}
	}

	var = m_pEditItem->m_vaOutput;
	dd.PutProperty(2, &var);

	return 0;
}

LRESULT CItemEditDialog::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled)
{
	CComBSTR bstr;
	GetDlgItemText(IDC_ITEM_INPUT1, (BSTR&)bstr); 
	m_pEditItem->m_vaInput1 = bstr;
	GetDlgItemText(IDC_ITEM_INPUT2, (BSTR&)bstr); 
	m_pEditItem->m_vaInput2 = bstr; 
	
	CComPtr<IDispatch> spDispatch;
	m_winOutput.QueryControl(&spDispatch);

	CComVariant var;
	CComDispatchDriver dd(spDispatch);
	dd.GetProperty(2, &var);

	m_pEditItem->m_vaOutput = var;

	EndDialog(IDOK);    
	return 0;
}

LRESULT CItemEditDialog::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
	return 0;
}

HRESULT CItemEditDialog::HelperSetText(int nIDDlgItem, CComVariant &var)
{
	USES_CONVERSION;

	if (var.vt != VT_BSTR)
		var.ChangeType(VT_BSTR);
		
	if (var.vt != VT_BSTR)
		return S_FALSE;

	SetDlgItemText(nIDDlgItem, W2T(var.bstrVal));

	return S_OK;
}

