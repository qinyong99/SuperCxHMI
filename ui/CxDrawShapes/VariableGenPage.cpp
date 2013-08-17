// VariableGenPage.cpp : Implementation of CVariableGenPage
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "VariableGenPage.h"

/////////////////////////////////////////////////////////////////////////////
// CVariableGenPage

void CVariableGenPage::SyncValueType()
{
	if (m_nObjects > 0)
	{
		CComQIPtr<IDrawVariable, &IID_IDrawVariable> spVariable(m_ppUnk[0]);
		
		enumPropertyType nType = (enumPropertyType)m_cmbVarType.GetCurSel();
		
		CComPtr<IDispatch> spDispatch;
		GetDlgControl(IDC_VALUE, IID_IDispatch, (void**)&spDispatch);
		CComDispatchDriver dd(spDispatch);
		CComVariant var;
		switch (nType)
		{
		case PropertyTypeColor:
			var = PROPTYPE_COLOR;
			break;
		case PropertyTypeBool:
			var = PROPTYPE_BOOL;
			break;
		case PropertyTypeString:
		default:
			var = PROPTYPE_STRING;
			break;
		case PropertyTypeNumeric:
			var = PROPTYPE_NUMBER;
			break;
		}
		dd.PutPropertyByName(L"Type", &var);
		
		spVariable->get_Value(&var);
		dd.PutPropertyByName(L"Value", &var);
	}
}

LRESULT CVariableGenPage::OnSelchangeVartype(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SyncValueType();

	return 0;
}

LRESULT CVariableGenPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;
	USES_CHINESEVAR_CONVERSION;

	m_cmbVarType.Attach(GetDlgItem(IDC_VARTYPE));
	m_cmbVarType.AddString(_T("颜色"));
	m_cmbVarType.AddString(_T("布尔"));
	m_cmbVarType.AddString(_T("字符串"));
	m_cmbVarType.AddString(_T("数字"));
	m_cmbVarType.AddString(_T("未知"));

	m_btnEvaluateSource.Attach(GetDlgItem(IDC_EVALUATE_SOURCE));

	m_editName.Attach(GetDlgItem(IDC_NAME));
	m_editDesc.Attach(GetDlgItem(IDC_DESC));

	if (m_nObjects > 0)
	{
		CComQIPtr<IDrawVariable, &IID_IDrawVariable> spVariable(m_ppUnk[0]);

		enumPropertyType type;
		spVariable->get_Type(&type);
		m_cmbVarType.SetCurSel(type);

		SyncValueType();

		CComBSTR bstr;
		spVariable->get_Name(&bstr);
		m_editName.SetWindowText(OLE2T(bstr));

		bstr.Empty();
		spVariable->get_Description(&bstr);
		m_editDesc.SetWindowText(OLE2T(bstr));
	
		CComPtr<IUnknown> spUnknown;
		spVariable->GetConnectObject(L"Value", &spUnknown);
		
		if (spUnknown != NULL)
			spUnknown->QueryInterface(IID_IDirectDynamic, (void **)&m_spDirectDynamic);
		
		CComQIPtr<ICxDynamicObject> spDynamicObject;
		if (m_spDirectDynamic == NULL)
		{
			m_spDirectDynamic.CoCreateInstance(CLSID_DirectDynamic);
			spDynamicObject = m_spDirectDynamic;
			spDynamicObject->InitNew(0);
		}
	
		ATLASSERT(m_spDirectDynamic != NULL);
		
		// 显示表达式
		CComVariant var;
		CComDispatchDriver dd(m_spDirectDynamic);
		dd.GetPropertyByName(L"Source", &var);

		if (var.vt != VT_BSTR)
			var.ChangeType(VT_BSTR);

		var = VARA2C(W2T(var.bstrVal));
		
		if (var.vt == VT_BSTR)
			SetDlgItemText(IDC_EXPRESSION, W2T(var.bstrVal));

		var.Clear();
		dd.GetPropertyByName(L"EnableMouseInput", &var);
		if (var.vt != VT_BOOL)
			var.ChangeType(VT_BOOL);
		m_btnEvaluateSource.SetCheck(var.boolVal != VARIANT_FALSE);
	}
	
	return 0;
}

#include "../CxDynObjs/PropInfo.h"
#include "../CxScripCrt/CxScripCrt.h"

LRESULT CVariableGenPage::OnClickedCreateExpression(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;
	
	CComBSTR bstr;
	GetDlgItemText(IDC_EXPRESSION, bstr.m_str); 
	
	CComQIPtr<ICxDrawObject, &IID_ICxDrawObject> spDrawObject(m_ppUnk[0]);
	CComPtr<ICxDrawObjectSite> spDrawObjectSite;
	spDrawObject->GetDrawObjectSite(&spDrawObjectSite);
	if (spDrawObjectSite != NULL)
	{
		ExpressionCreatorProc pfn = NULL;
		spDrawObjectSite->GetExpressionCreatorProc(long(&pfn));
		pfn(&bstr);
	}
	
	SetDlgItemText(IDC_EXPRESSION, W2T(bstr));
	
	return 0;
}

STDMETHODIMP CVariableGenPage::Apply(void)
{
	USES_CONVERSION;
	USES_CHINESEVAR_CONVERSION;

	CComBSTR bstrExpression;
	GetDlgItemText(IDC_EXPRESSION, (BSTR&)bstrExpression); 
	bstrExpression = VARC2A(W2T(bstrExpression));

	if (bstrExpression.Length() > 0)
	{
		m_spDirectDynamic->put_Source(bstrExpression);
		m_spDirectDynamic->put_EnableMouseInput(m_btnEvaluateSource.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
	}
	else
	{
		m_spDirectDynamic.Release();
	}
	
	m_bDirty = FALSE;
	
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<IDrawVariable, &IID_IDrawVariable> spVariable(m_ppUnk[i]);

		spVariable->put_Type((enumPropertyType)m_cmbVarType.GetCurSel());

		CComPtr<IDispatch> spDispatch;
		GetDlgControl(IDC_VALUE, IID_IDispatch, (void**)&spDispatch);
		CComDispatchDriver dd(spDispatch);
		CComVariant var;
		dd.GetPropertyByName(L"Value", &var);

		spVariable->put_Value(var);

		CComBSTR bstr;
		m_editName.GetWindowText((BSTR&)bstr);
		spVariable->put_Name(bstr);

		bstr.Empty();
		m_editDesc.GetWindowText((BSTR&)bstr);
		spVariable->put_Description(bstr);
				
		if (m_spDirectDynamic != NULL)
		{
			CComPtr<IDirectDynamic> spDirectDynamic;
			m_spDirectDynamic->Clone(&spDirectDynamic);
			spVariable->ConnectObject(L"Value", m_spDirectDynamic);
		}
		else
		{
			spVariable->Disconnect(L"Value");
		}
	}
	
	return S_OK;
}
