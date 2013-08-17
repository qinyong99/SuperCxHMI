// ScaleDlg.cpp: implementation of the CScaleDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScaleDlg.h"
#include "DynamicExpert.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScaleDlg::CScaleDlg()
{
	m_pExpert = NULL;
}

CScaleDlg::~CScaleDlg()
{
}

LRESULT CScaleDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	
		
	return S_OK;
}

void CScaleDlg::FillBasePointCombo()
{
	USES_CONVERSION;
	ATLASSERT(m_cmbBase.m_hWnd == NULL);
	
	CEnumPropItemArray* pArrEnumItem = NULL;
	CTypeInfoBrowse::CrackPropertyType(m_pExpert->m_ppUnk[0], L"ScaleBasePoint", (LPARAM)&pArrEnumItem);
	ATLASSERT(pArrEnumItem != NULL);
	
	if (pArrEnumItem != NULL)
	{
		m_cmbBase.Attach(GetDlgItem(IDC_BASE));

		for (int i = 0; i < pArrEnumItem->GetSize(); i++)
		{
			CEnumPropItem& item = (*pArrEnumItem)[i];
// 			wsprintf(szBuf, _T("%d - %s"), item.m_lValue, OLE2T(item.m_bstrDesc));
			m_cmbBase.AddString(OLE2T(item.m_bstrDesc));
		}
	}
}

void CScaleDlg::OnShow()
{
	USES_CONVERSION;
	CComVariant var;

	m_spUnkDynamic.Release();

	CPropInfo prop;
	m_pExpert->GetSelectProp(prop);
	SetDlgItemText(IDC_PROP, OLE2T(prop.m_bstrDesc));

	if (m_cmbBase.m_hWnd == NULL)
		FillBasePointCombo();

	if (m_cmbBase.m_hWnd != NULL)
	{
		CComDispatchDriver dd(m_pExpert->m_ppUnk[0]);
		dd.GetPropertyByName(L"ScaleBasePoint", &var);
		m_cmbBase.SetCurSel(var.lVal);
	}
	
	BOOL bShow = prop.m_dispid != CXPROPID_RotateAngle ? SW_SHOW : SW_HIDE;
	::ShowWindow(GetDlgItem(IDC_POSITON_TYPE), bShow);
	::ShowWindow(GetDlgItem(IDC_OFFSET), bShow);
	::ShowWindow(GetDlgItem(IDC_ABSOLUTE), bShow);
		
	IUnknown* pUnkDyn = NULL;
	m_pExpert->GetDynamic(prop.m_bstrName, &pUnkDyn);
	m_spUnkDynamic = pUnkDyn;
		
	if (m_spUnkDynamic == NULL)
	{
		HRESULT hr = m_spUnkDynamic.CoCreateInstance(CLSID_LinearDynamic);
		if (SUCCEEDED(hr))
		{
			CComQIPtr<ICxDynamicObject> spDynamicObject(m_spUnkDynamic);
			if (spDynamicObject != NULL)
			{
				m_pExpert->InitDynamicPropInfo();
				spDynamicObject->InitNew(long(m_pExpert->m_pPropInfo));

				CComDispatchDriver dd(m_spUnkDynamic);
//				var = TRUE;
//				dd.PutPropertyByName(L"UseOffset", &var);
			}
		}
	}
	
	if (m_spUnkDynamic != NULL)
	{
		CLSID clsid;
		CDynamicExpert::GetDynamicClassID(m_spUnkDynamic, &clsid);
 		if (!InlineIsEqualGUID(clsid, CLSID_LinearDynamic))
		{
			m_pExpert->SelectAdvacedProp(prop.m_dispid);
			return;
		}
		
		CComDispatchDriver dd(m_spUnkDynamic);

		var.Clear();
		dd.GetPropertyByName(L"Source", &var);
		if (var.vt != VT_BSTR)
			var.ChangeType(VT_BSTR);
		if (var.vt == VT_BSTR)
			m_pExpert->SetSource(var.bstrVal);

		var.Clear();
		dd.GetPropertyByName(L"LoInValue", &var);
		HelperSetText(m_hWnd, IDC_LOINVALUE, var);

		var.Clear();
		dd.GetPropertyByName(L"HiInValue", &var);
		HelperSetText(m_hWnd, IDC_HIINVALUE, var);

		var.Clear();
		dd.GetPropertyByName(L"LoOutValue", &var);
		HelperSetText(m_hWnd, IDC_LOOUTVALUE, var);

		var.Clear();
		dd.GetPropertyByName(L"HiOutValue", &var);
		HelperSetText(m_hWnd, IDC_HIOUTVALUE, var);

		var.Clear();
		dd.GetPropertyByName(L"FetchInput", &var);
		::SendMessage(GetDlgItem(IDC_FETCHINPUT), BM_SETCHECK, var.boolVal != VARIANT_FALSE, 0L); 
	}
}

void CScaleDlg::OnOK()
{
	USES_CONVERSION;
	CComVariant var;

	CPropInfo prop;
	m_pExpert->GetSelectProp(prop);

	CComBSTR bstr;
	BOOL bValid = m_pExpert->GetSource((BSTR&)bstr);
	
	if (!bValid)
	{
		m_pExpert->DeleteDynamic(prop.m_bstrName);
		return;
	}

	if (m_cmbBase.m_hWnd != NULL)
	{
		CComDispatchDriver dd(m_pExpert->m_ppUnk[0]);
		var = m_cmbBase.GetCurSel();
		dd.PutPropertyByName(L"ScaleBasePoint", &var);
	}

	ATLASSERT(m_spUnkDynamic != NULL);

	CComDispatchDriver dd(m_spUnkDynamic);

	var = bstr;
	dd.PutPropertyByName(L"Source", &var);

	GetDlgItemText(IDC_LOINVALUE, (BSTR&)bstr);
	if (!bstr || bstr == "")
	{
		::MessageBox(m_hWnd, _T("请输入表达式最小值！"),  _T("提示"), MB_OK | MB_ICONEXCLAMATION);
		::SetFocus(GetDlgItem(IDC_LOINVALUE));
		return;
	}
	var = bstr;
	dd.PutPropertyByName(L"LoInValue", &var);

	GetDlgItemText(IDC_HIINVALUE, (BSTR&)bstr);
	if (!bstr || bstr == "")
	{
		::MessageBox(m_hWnd, _T("请输入表达式最大值！"),  _T("提示"), MB_OK | MB_ICONEXCLAMATION);
		::SetFocus(GetDlgItem(IDC_HIINVALUE));
		return;
	}
	var = bstr;
	dd.PutPropertyByName(L"HiInValue", &var);

	GetDlgItemText(IDC_LOOUTVALUE, (BSTR&)bstr);
	if (!bstr || bstr == "")
	{
		CString strText;
		strText.Format(_T("请输入%s最小值！"), OLE2T(prop.m_bstrDesc));
		::MessageBox(m_hWnd, strText,  _T("提示"), MB_OK | MB_ICONEXCLAMATION);
		::SetFocus(GetDlgItem(IDC_LOOUTVALUE));
		return;
	}
	var = bstr;
	dd.PutPropertyByName(L"LoOutValue", &var);

	GetDlgItemText(IDC_HIOUTVALUE, (BSTR&)bstr);
	if (!bstr || bstr == "")
	{
		CString strText;
		strText.Format(_T("请输入%s最大值！"), OLE2T(prop.m_bstrDesc));
		::MessageBox(m_hWnd, strText,  _T("提示"), MB_OK | MB_ICONEXCLAMATION);
		::SetFocus(GetDlgItem(IDC_HIOUTVALUE));
		return;
	}
	var = bstr;
	dd.PutPropertyByName(L"HiOutValue", &var);

	var = (BOOL)::SendMessage(GetDlgItem(IDC_FETCHINPUT), BM_GETCHECK, 0, 0L);
	dd.PutPropertyByName(L"FetchInput", &var);

	m_pExpert->PutDynamic(prop.m_bstrName, m_spUnkDynamic);
}