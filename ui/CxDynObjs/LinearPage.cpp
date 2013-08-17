// LinearPage.cpp : Implementation of CLinearPage
#include "stdafx.h"
#include "CxDynObjs.h"
#include "LinearPage.h"

//#include "TagPaser.h"

/////////////////////////////////////////////////////////////////////////////
// CLinearPage

HRESULT CLinearPage::SetPageSite(IPropertyPageSite *pPageSite)
{
	HRESULT hr = IPropertyPageImpl<CLinearPage>::SetPageSite(pPageSite);
	if (FAILED(hr))
		return hr;

	hr = CrackPropertyType();

	return hr;		
}

//获取页面类型
HRESULT CLinearPage::CrackPropertyType()
{
	IDynamicFrm* pDynamicFrm = NULL;
	HRESULT hr = m_pPageSite->QueryInterface(IID_IDynamicFrm, (void **)&pDynamicFrm);
	if (FAILED(hr))
		return hr;

	CDynamicPropInfo* pPropInfo = NULL;
	pDynamicFrm->get_PropertyInfo((long *)&pPropInfo);
	
	//获取接口属性的页面类型
	m_pPropInfo = pPropInfo;

	pDynamicFrm->Release();

	return S_OK;
}

HRESULT CLinearPage::SetObjects(ULONG nObjects, IUnknown **ppUnk)
{
	HRESULT hr = IPropertyPageImpl<CLinearPage>::SetObjects(nObjects, ppUnk);
	if (SUCCEEDED(hr))
	{
		ILinearDynamic* pDynamic;
		m_ppUnk[0]->QueryInterface(IID_ILinearDynamic, (void**)&pDynamic);
		if (pDynamic != NULL)
		{
			CComVariant var;
			pDynamic->get_HiInValue(&var);
			if (var.vt == VT_BSTR && m_pPropInfo->pfnExpression)
				m_pPropInfo->pfnExpression(&var.bstrVal, 1);
			HelperSetText(IDC_HIINVALUE, var);
			pDynamic->get_HiOutValue(&var);
			if (var.vt == VT_BSTR && m_pPropInfo->pfnExpression)
				m_pPropInfo->pfnExpression(&var.bstrVal, 1);
			HelperSetText(IDC_HIOUTVALUE, var);
			pDynamic->get_LoInValue(&var);
			if (var.vt == VT_BSTR && m_pPropInfo->pfnExpression)
				m_pPropInfo->pfnExpression(&var.bstrVal, 1);
			HelperSetText(IDC_LOINVALUE, var);
			pDynamic->get_LoOutValue(&var);
			if (var.vt == VT_BSTR && m_pPropInfo->pfnExpression)
				m_pPropInfo->pfnExpression(&var.bstrVal, 1);
			HelperSetText(IDC_LOOUTVALUE, var);
			BOOL b;
			pDynamic->get_FetchInput(&b);
			::SendMessage(GetDlgItem(IDC_FETCHINPUT), BM_SETCHECK, b, 0L); 
			pDynamic->get_UseOffset(&b);
			::SendMessage(GetDlgItem(IDC_USEOFFSET), BM_SETCHECK, b, 0L); 

			if (m_pPropInfo->bSupportMouseInput)
			{
				pDynamic->get_EnableMouseInput(&b);
				::SendMessage(GetDlgItem(IDC_ENABLEMOUSEINPUT), BM_SETCHECK, b, 0L); 
			}
			else
			{
				::ShowWindow(GetDlgItem(IDC_ENABLEMOUSEINPUT), SW_HIDE);
			}
			pDynamic->Release();
		}
	}

	return hr;
}

HRESULT CLinearPage::Apply(void)
{
	ATLTRACE(_T("CLinearPage::Apply\n"));
	for (UINT i = 0; i < m_nObjects; i++)
	{
		// Do something interesting here
		// ICircCtl* pCirc;
		// m_ppUnk[i]->QueryInterface(IID_ICircCtl, (void**)&pCirc);
		// pCirc->put_Caption(CComBSTR("something special"));
		// pCirc->Release();
		ILinearDynamic* pDynamic;
		m_ppUnk[i]->QueryInterface(IID_ILinearDynamic, (void**)&pDynamic);
		CComBSTR bstr;
		GetDlgItemText(IDC_LOINVALUE, (BSTR&)bstr); 
		if (bstr.m_str == NULL || bstr == "")
		{
			::MessageBox(m_hWnd, _T("请输入最小输入值！"),  _T("提示"), MB_OK);
			::SetFocus(GetDlgItem(IDC_LOINVALUE));
			return S_FALSE;
		}
		if (m_pPropInfo->pfnExpression)
				m_pPropInfo->pfnExpression(&bstr, 2);
		pDynamic->put_LoInValue(CComVariant(bstr));

		GetDlgItemText(IDC_HIINVALUE, (BSTR&)bstr); 
		if (bstr.m_str == NULL || bstr == "")
		{
			::MessageBox(m_hWnd, _T("请输入最大输入值！"),  _T("提示"), MB_OK);
			::SetFocus(GetDlgItem(IDC_HIINVALUE));
			return S_FALSE;
		}
		if (m_pPropInfo->pfnExpression)
				m_pPropInfo->pfnExpression(&bstr, 2);
		pDynamic->put_HiInValue(CComVariant(bstr));

		GetDlgItemText(IDC_LOOUTVALUE, (BSTR&)bstr);
		if (bstr.m_str == NULL || bstr == "")
		{
			::MessageBox(m_hWnd, _T("请输入最小输出值！"),  _T("提示"), MB_OK);
			::SetFocus(GetDlgItem(IDC_LOOUTVALUE));
			return S_FALSE;
		}
		if (m_pPropInfo->pfnExpression)
				m_pPropInfo->pfnExpression(&bstr, 2);
		pDynamic->put_LoOutValue(CComVariant(bstr));

		GetDlgItemText(IDC_HIOUTVALUE, (BSTR&)bstr);
		if (bstr.m_str == NULL || bstr == "")
		{
			::MessageBox(m_hWnd, _T("请输入最大输出值！"),  _T("提示"), MB_OK);
			::SetFocus(GetDlgItem(IDC_HIOUTVALUE));
			return S_FALSE;
		}
		if (m_pPropInfo->pfnExpression)
				m_pPropInfo->pfnExpression(&bstr, 2);
		pDynamic->put_HiOutValue(CComVariant(bstr));

		BOOL b;
		b = ::SendMessage(GetDlgItem(IDC_FETCHINPUT), BM_GETCHECK, 0, 0L);
		pDynamic->put_FetchInput(b);
		b = ::SendMessage(GetDlgItem(IDC_USEOFFSET), BM_GETCHECK, 0, 0L);
		pDynamic->put_UseOffset(b);
		if (m_pPropInfo->bSupportMouseInput)
		{
			b = ::SendMessage(GetDlgItem(IDC_ENABLEMOUSEINPUT), BM_GETCHECK, 0, 0L);
			pDynamic->put_EnableMouseInput(b);
		}
		pDynamic->Release();
	}
	m_bDirty = FALSE;
	return S_OK;
}

HRESULT CLinearPage::HelperSetText(int nIDDlgItem, CComVariant &var)
{
	USES_CONVERSION;

	if (var.vt != VT_BSTR)
		var.ChangeType(VT_BSTR);
		
	if (var.vt != VT_BSTR)
		return S_FALSE;

	SetDlgItemText(nIDDlgItem, W2T(var.bstrVal));

	return S_OK;
}

#include "../CxScripCrt/CxScripCrt.h"
#include "../CxScripCrt/CxScripCrt_i.c"

LRESULT CLinearPage::OnClickedExp(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	USES_CONVERSION;

	CComBSTR bstr;
	GetDlgItemText(IDC_LOINVALUE + wID - IDC_LOINVALUE_EXP, (BSTR&)bstr);
	
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
	
	SetDlgItemText(IDC_LOINVALUE + wID - IDC_LOINVALUE_EXP, W2T(bstr));

	return 0;		
}

#include "../../svr/CxDatSes/CxDatSes.h"
#include "../../svr/CxDatSes/CxDatSes_i.c"

///////////////////////////////////////////////////////////////////////////////////
//功 能：取标签值范围
///////////////////////////////////////////////////////////////////////////////////
HRESULT GetTagValueRange(BSTR bstrTag, VARIANT* pvaMin, VARIANT* pvaMax)
{
	LPWSTR p = bstrTag;
	if ((p[0] == OLECHAR('S') || p[0] == OLECHAR('s')) &&
		(p[1] == OLECHAR('U') || p[1] == OLECHAR('u')) &&
		(p[2] == OLECHAR('P') || p[2] == OLECHAR('p')) &&
		(p[3] == OLECHAR('E') || p[3] == OLECHAR('e')) &&
		(p[4] == OLECHAR('R') || p[4] == OLECHAR('r')) &&
		(p[5] == OLECHAR('C') || p[5] == OLECHAR('c')) &&
		(p[6] == OLECHAR('X') || p[6] == OLECHAR('x')) &&
		(p[7] == OLECHAR('.')))
	{
		CComPtr<ICxDataSession> spDataSession;
		HRESULT hr = spDataSession.CoCreateInstance(CLSID_CxDataSession);
		if (FAILED(hr)) 
			return hr;

		return spDataSession->GetTagRange(p + 8, pvaMin, pvaMax);
	}
	
	return S_FALSE;
}

LRESULT CLinearPage::OnClickedFetchinput(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	BOOL bFrechInput;
	bFrechInput = ::SendMessage(GetDlgItem(IDC_FETCHINPUT), BM_GETCHECK, 0, 0L);		
	::EnableWindow(GetDlgItem(IDC_LOINVALUE), !bFrechInput);
	::EnableWindow(GetDlgItem(IDC_HIINVALUE), !bFrechInput);

	if (!bFrechInput)
		return 0;

	CComQIPtr<IDynamicFrm> spDynamicFrm(m_pPageSite);
	if (spDynamicFrm != NULL)
	{
		CComBSTR bstrSource;
		spDynamicFrm->get_Source(&bstrSource);

		CComVariant varMin;
		CComVariant varMax;
		if (GetTagValueRange(bstrSource, &varMin, &varMax) == S_OK)
		{
			HelperSetText(IDC_LOINVALUE, varMin);
			HelperSetText(IDC_HIINVALUE, varMax);
		}
	}

	return 0;
}
