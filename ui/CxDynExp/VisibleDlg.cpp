// VisibleDlg.cpp: implementation of the CVisibleDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VisibleDlg.h"
#include "DynamicExpert.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVisibleDlg::CVisibleDlg()
{
	m_pExpert = NULL;
}

CVisibleDlg::~CVisibleDlg()
{
}

LRESULT CVisibleDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	
// 	m_editPicture.Attach(GetDlgItem(IDC_PICTURE));
	
	return S_OK;
}

void CVisibleDlg::OnShow()
{
	USES_CONVERSION;
	CComVariant var;

	m_spUnkDynamic.Release();

	CPropInfo prop;
	m_pExpert->GetSelectProp(prop);
		
	IUnknown* pUnkDyn = NULL;
	m_pExpert->GetDynamic(prop.m_bstrName, &pUnkDyn);
	m_spUnkDynamic = pUnkDyn;
		
	if (m_spUnkDynamic == NULL)
	{
		HRESULT hr = m_spUnkDynamic.CoCreateInstance(CLSID_DirectDynamic);
		if (SUCCEEDED(hr))
		{
			CComQIPtr<ICxDynamicObject> spDynamicObject(m_spUnkDynamic);
			if (spDynamicObject != NULL)
			{
				m_pExpert->InitDynamicPropInfo();
				spDynamicObject->InitNew(long(m_pExpert->m_pPropInfo));
			}
		}
	}
	
	if (m_spUnkDynamic != NULL)
	{
		CLSID clsid;
		CDynamicExpert::GetDynamicClassID(m_spUnkDynamic, &clsid);
 		if (!InlineIsEqualGUID(clsid, CLSID_DirectDynamic))
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
	}
}

void CVisibleDlg::OnOK()
{
	USES_CONVERSION;
	CPropInfo prop;
	m_pExpert->GetSelectProp(prop);

	CComBSTR bstr;
	BOOL bValid = m_pExpert->GetSource((BSTR&)bstr);
	
	if (!bValid)
	{
		m_pExpert->DeleteDynamic(prop.m_bstrName);
		return;
	}

	ATLASSERT(m_spUnkDynamic != NULL);

	CComDispatchDriver dd(m_spUnkDynamic);

	CComVariant var(bstr);
	dd.PutPropertyByName(L"Source", &var);

	m_pExpert->PutDynamic(prop.m_bstrName, m_spUnkDynamic);
}
