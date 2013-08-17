// DynamicFrm.cpp : Implementation of CDynamicFrm
#include "stdafx.h"

#include "CxHmiObj.h"
#include "CxHmiMisc.h"

#include "CxDynObjs.h"
#include "DynamicFrm.h"

#include "ChineseVarUtils.h"

#include "../CxScripCrt/CxScripCrt.h"



/////////////////////////////////////////////////////////////////////////////
// CDynamicFrm

void CDynamicFrm::GetDynamicClassID(IUnknown* pDynUnk, CLSID* pClassID)
{
	*pClassID = CLSID_NULL;
	
	if (pDynUnk != NULL)
	{
		CComQIPtr<ICxDynamicObject> spDynamicObject = pDynUnk;
		
		CComBSTR bstr;
		spDynamicObject->GetClassName(&bstr);
		
		if (bstr == OLESTR("Lookup"))
			*pClassID = CLSID_LookupDynamic;
		else if (bstr == OLESTR("Linear"))
			*pClassID = CLSID_LinearDynamic;
		else if (bstr == OLESTR("Format"))
			*pClassID = CLSID_FormatDynamic;
		else if (bstr == OLESTR("Direct"))
			*pClassID = CLSID_DirectDynamic;
	}
}

LRESULT CDynamicFrm::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;
	USES_CHINESEVAR_CONVERSION;

	TCHAR szCaption[128];
	wsprintf(szCaption, _T("动态关联（%s）"), OLE2T(m_pPropInfo->bstrPropertyName));
	SetWindowText(szCaption);

	int nDynamic = m_nCurDyn;

	if (nDynamic == -1)
	{
		switch (m_pPropInfo->nType)
		{
		case PROPTYPE_UNKNOWN:
		case PROPTYPE_COLOR:
		case PROPTYPE_ENUM:
		case PROPTYPE_ENUMSTRING:
		case PROPTYPE_FAKEFILE:
		case PROPTYPE_FAKEFOLDER:	
			nDynamic = GetDynamic(CLSID_LookupDynamic);
			break;
		case PROPTYPE_NUMBER:
			nDynamic = GetDynamic(CLSID_LinearDynamic);
			break;
		case PROPTYPE_STRING:
		case PROPTYPE_FAKEFONTNAME:
		case PROPTYPE_TAGGROUP:
		case PROPTYPE_SECURITYAREA:
			nDynamic = GetDynamic(CLSID_FormatDynamic);
			break;
		default:
			nDynamic = GetDynamic(CLSID_DirectDynamic);
		}
	}

	int nIDCheckButton = -1;
	
	if (nDynamic != -1)
	{
		CDynamicInfo* pInfo = m_apDynInfos[nDynamic];

		if (InlineIsEqualGUID(pInfo->clsid, CLSID_LookupDynamic))
			nIDCheckButton = IDC_LOOKUP;
		else if (InlineIsEqualGUID(pInfo->clsid, CLSID_LinearDynamic))
			nIDCheckButton = IDC_LINEAR;
		else if (InlineIsEqualGUID(pInfo->clsid, CLSID_FormatDynamic))
			nIDCheckButton = IDC_FORMAT;
		else if (InlineIsEqualGUID(pInfo->clsid, CLSID_DirectDynamic))
			nIDCheckButton = IDC_DIRECT;

		CComDispatchDriver dd(pInfo->spUnkDynamic);
		CComVariant var;
		dd.GetPropertyByName(L"Source", &var);
		if (var.vt != VT_BSTR)
			var.ChangeType(VT_BSTR);
	
		if (m_pPropInfo->pfnExpression)
			m_pPropInfo->pfnExpression(&var.bstrVal, 1);

		var = VARA2C(W2T(var.bstrVal));
		
		if (var.vt == VT_BSTR)
			SetDlgItemText(IDC_EXPRESSION, W2T(var.bstrVal));
			
	}

	CheckRadioButton(IDC_LOOKUP, IDC_DIRECT, nIDCheckButton);
	ShowDynamicPage(nDynamic);
	
	return 1;
}

void CDynamicFrm::OnFinalMessage(HWND hWnd)
{
	for (int i = 0; i < m_apDynInfos.GetSize(); i++)
	{
		CDynamicInfo* pInfo = m_apDynInfos[i];
		if (pInfo->spPropPage != NULL)
			pInfo->spPropPage->Deactivate();
		
		delete pInfo;
	}
	
	m_apDynInfos.RemoveAll();
}

LRESULT CDynamicFrm::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;
	USES_CHINESEVAR_CONVERSION;

	CComBSTR bstrExpression;
	GetDlgItemText(IDC_EXPRESSION, (BSTR&)bstrExpression); 
	if (m_pPropInfo->pfnExpression)
		m_pPropInfo->pfnExpression(&bstrExpression, 2);

	bstrExpression = VARC2A(OLE2T(bstrExpression));

	BOOL bValid = FALSE;
	for (UINT i = 0; i < bstrExpression.Length(); i++)
	{
		if (!iswspace(bstrExpression.m_str[i]))
		{
			bValid = TRUE;
			break;
		}
	}
	if (!bValid)
	{
		m_nCurDyn = -1;
		m_pPropInfo->spUnkDynamic.Release();
		EndDialog(wID);
		return 0;
	}

//	if (bstrExpression.m_str == NULL || bstrExpression == "")
//	{
//		::MessageBox(m_hWnd, _T("请输入完整公式/表达式！"),  _T("提示"), MB_OK);
//		::SetFocus(GetDlgItem(IDC_EXPRESSION));
//		m_pCurDyn = NULL;
//		EndDialog(wID);
//		return 0;
//	}

	ATLASSERT(m_nCurDyn != -1);
	CDynamicInfo* pInfo = m_apDynInfos[m_nCurDyn];

	ATLASSERT(pInfo->spUnkDynamic != NULL);
	CComDispatchDriver dd(pInfo->spUnkDynamic);
	CComVariant var(bstrExpression);
	dd.PutPropertyByName(L"Source", &var);

	ATLASSERT(pInfo->spPropPage != NULL);
	HRESULT hr = pInfo->spPropPage->Apply();
	if (hr != S_OK)
		return 0;

	m_pPropInfo->spUnkDynamic = pInfo->spUnkDynamic;

	EndDialog(wID);
	return 0;	
}

LRESULT CDynamicFrm::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

STDMETHODIMP CDynamicFrm::EditDynamic(long hParent, long X, long Y)
{
	if (DoModal((HWND)hParent) != IDOK)
		return S_FALSE;

	return S_OK;
}

LRESULT CDynamicFrm::OnClickedLinear(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nDynamic = GetDynamic(CLSID_LinearDynamic);
	ShowDynamicPage(nDynamic);
	
	return 0;
}

LRESULT CDynamicFrm::OnClickedLookup(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nDynamic = GetDynamic(CLSID_LookupDynamic);
	ShowDynamicPage(nDynamic);
	
	return 0;
}

LRESULT CDynamicFrm::OnClickedFormat(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nDynamic = GetDynamic(CLSID_FormatDynamic);
	ShowDynamicPage(nDynamic);
	
	return 0;
}

LRESULT CDynamicFrm::OnClickedDirect(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nDynamic = GetDynamic(CLSID_DirectDynamic);
	ShowDynamicPage(nDynamic);
	
	return 0;
}

void CDynamicFrm::ShowDynamicPage(int nDynamic)
{
	CDynamicInfo* pInfo;
	
	if (nDynamic != m_nCurDyn)
	{
		if (m_nCurDyn != -1)
		{
			pInfo = m_apDynInfos[m_nCurDyn];
			if (pInfo->spPropPage != NULL)
				pInfo->spPropPage->Show(SW_HIDE);
		}
	}
	
	m_nCurDyn = nDynamic;
	
	if (m_nCurDyn == -1)
		return;
	
	pInfo = m_apDynInfos[m_nCurDyn];
	
	if (pInfo->spPropPage == NULL)
	{
		CComQIPtr<ICxDynamicObject> spDynamicObject = pInfo->spUnkDynamic;
		if (spDynamicObject == NULL)
		{
			ATLASSERT(FALSE);
			return;
		}
		
		CLSID clsidPage = CLSID_NULL;
		spDynamicObject->GetCustomPage(&clsidPage);
		
		CComPtr<IPropertyPage> spPropPage;
		HRESULT hr = spPropPage.CoCreateInstance(clsidPage);
		if (FAILED(hr))
		{
			ATLASSERT(FALSE);
			return;
		}
		
		CComPtr<IPropertyPageSite> spPageSite;
		_InternalQueryInterface(IID_IPropertyPageSite, (void**)&spPageSite);
		spPropPage->SetPageSite(spPageSite);
		
		PROPPAGEINFO ppi;
		spPropPage->GetPageInfo(&ppi);
		SIZE size = ppi.size;
		
		RECT rcDisplay;
		::GetWindowRect(GetDlgItem(IDC_RECT), &rcDisplay);
		ScreenToClient(&rcDisplay);
		rcDisplay.left = rcDisplay.left + (rcDisplay.right - rcDisplay.left - size.cx) / 2;
		rcDisplay.top = rcDisplay.top + (rcDisplay.bottom - rcDisplay.top - size.cy) / 2;
		rcDisplay.right = rcDisplay.left + size.cx;
		rcDisplay.bottom = rcDisplay.top + size.cy;
		OffsetRect(&rcDisplay, 2, 12);
		spPropPage->Activate(m_hWnd, &rcDisplay, TRUE);
		
 		spPropPage->SetObjects(1, &pInfo->spUnkDynamic.p);
		
		pInfo->spPropPage = spPropPage;
	}
	
	
	pInfo->spPropPage->Show(SW_SHOW);
}

STDMETHODIMP CDynamicFrm::get_PropertyInfo(long *pVal)
{
	*pVal = long(m_pPropInfo);

	return S_OK;
}

STDMETHODIMP CDynamicFrm::put_PropertyInfo(long newVal)
{
	m_pPropInfo = (CDynamicPropInfo *)newVal;

	if (m_pPropInfo->spUnkDynamic != NULL)
	{
		for (int i = 0; i < m_apDynInfos.GetSize(); i++)
		{
			CDynamicInfo* pInfo = m_apDynInfos[i];
			if (pInfo->spUnkDynamic == m_pPropInfo->spUnkDynamic)
				return S_OK;
		}

		CDynamicInfo* pInfo = new CDynamicInfo;
		pInfo->spUnkDynamic = m_pPropInfo->spUnkDynamic;
		GetDynamicClassID(pInfo->spUnkDynamic, &pInfo->clsid);
		m_apDynInfos.Add(pInfo);
		m_nCurDyn = m_apDynInfos.GetSize() - 1;
	}

	return S_OK;
}

int CDynamicFrm::GetDynamic(CLSID clsid)
{
	for (int i = 0; i < m_apDynInfos.GetSize(); i++)
	{
		CDynamicInfo* pInfo = m_apDynInfos[i];
		if (pInfo->clsid == clsid)
		{
			ATLASSERT(pInfo->spUnkDynamic != NULL);
			return i;
		}
	}
	
	CComPtr<IUnknown> spUnkDynamic;
	HRESULT hr = spUnkDynamic.CoCreateInstance(clsid);
	if (SUCCEEDED(hr))
	{
		CComQIPtr<ICxDynamicObject> spDynamicObject(spUnkDynamic);
		if (spDynamicObject != NULL)
			spDynamicObject->InitNew(long(m_pPropInfo));

		CComDispatchDriver dd(spDynamicObject);
		CComVariant var = m_bstrSource;
		dd.PutPropertyByName(L"Source", &var);
			
		CDynamicInfo* pInfo = new CDynamicInfo;
		pInfo->spUnkDynamic = spUnkDynamic;
		pInfo->clsid = clsid;
		
		m_apDynInfos.Add(pInfo);
		return m_apDynInfos.GetSize() - 1;
	}
	
	return -1;
}

LRESULT CDynamicFrm::OnClickedCreateExpression(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	CComBSTR bstr;
	GetDlgItemText(IDC_EXPRESSION, bstr.m_str); 

	if (m_pPropInfo->pfnExpression)
	{
		m_pPropInfo->pfnExpression(&bstr);
	}
	else
	{
		CComPtr<ICxExpressionCreator> spExprCrt;
		HRESULT hr = spExprCrt.CoCreateInstance(CLSID_CxExpressionCreator);
		if (FAILED(hr)) 
			return hr;

		spExprCrt->put_Expression(bstr);

		spExprCrt->AddGeneralSymbolPage(CX_ALL_SYMBOL);

		hr = spExprCrt->ShowDialog();
		if (FAILED(hr)) 
			return hr;

		bstr.Empty();
		spExprCrt->get_Expression(&bstr);
	}
	
	SetDlgItemText(IDC_EXPRESSION, W2T(bstr));

	return 0;
}

STDMETHODIMP CDynamicFrm::get_Source(BSTR *pbstr)
{
	GetDlgItemText(IDC_EXPRESSION, *pbstr); 

	return S_OK;
}

STDMETHODIMP CDynamicFrm::put_Source(BSTR bstr)
{
	USES_CONVERSION;
	USES_CHINESEVAR_CONVERSION;

	CComBSTR bstrT = VARA2C(OLE2T(bstr));

	if (IsWindow())
		SetDlgItemText(IDC_EXPRESSION, W2T(bstrT));

	m_bstrSource = bstrT;

	return S_OK;
}

STDMETHODIMP CDynamicFrm::OnCreateExpression(BSTR *pbstr)
{
	return E_NOTIMPL;
}
