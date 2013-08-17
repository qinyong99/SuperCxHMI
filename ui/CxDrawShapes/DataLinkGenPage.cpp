// DataLinkGenPage.cpp : Implementation of DataLinkGenPage
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "../CxDynObjs/CxDynObjs_i.c"
#include "DataLinkGenPage.h"

/////////////////////////////////////////////////////////////////////////////
// DataLinkGenPage

LRESULT CDataLinkGenPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;
	USES_CHINESEVAR_CONVERSION;

	memset(&m_propinfo, 0, sizeof(CDynamicPropInfo));
	m_propinfo.bSupportMouseInput = TRUE;
	
	if (m_nObjects > 0)
	{
		CComQIPtr<IDrawDataLink, &IID_IDrawDataLink> spDataLink(m_ppUnk[0]);

		if (spDataLink != NULL)
		{
			CComPtr<IUnknown> spUnknown;
			spDataLink->GetConnectObject(L"Value", &spUnknown);
			
			if (spUnknown != NULL)
				spUnknown->QueryInterface(IID_IFormatDynamic, (void **)&m_spFormatDynamic);	
		}
		else
		{
			m_ppUnk[0]->QueryInterface(IID_IFormatDynamic, (void **)&m_spFormatDynamic);
			m_bObjectIsFormmatDynamic = TRUE;
		}
	
		CComQIPtr<ICxDynamicObject> spDynamicObject;
		if (m_spFormatDynamic == NULL)
		{
			m_spFormatDynamic.CoCreateInstance(CLSID_FormatDynamic);
			spDynamicObject = m_spFormatDynamic;
			spDynamicObject->InitNew(0);
		}
		else
		{
			//显示格式动态对象属性页
			spDynamicObject = m_spFormatDynamic;
		}
	
		ATLASSERT(m_spFormatDynamic != NULL);

		// 显示表达式
		CComVariant var;
		CComDispatchDriver dd(m_spFormatDynamic);
		dd.GetPropertyByName(L"Source", &var);
		
		if (var.vt != VT_BSTR)
			var.ChangeType(VT_BSTR);

		var = VARA2C(W2T(var.bstrVal));
		
		if (var.vt == VT_BSTR)
			SetDlgItemText(IDC_EXPRESSION, W2T(var.bstrVal));

		CLSID clsidPage = CLSID_NULL;
		spDynamicObject->GetCustomPage(&clsidPage);

		//创建动态属性页面对象
		HRESULT hr = ::CoCreateInstance(
			clsidPage,         
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IPropertyPage,
			reinterpret_cast<void**>(&m_spFormatDynamicPage));
		if (FAILED(hr))
		{
			ATLASSERT(FALSE);
			return 0;
		}
		ATLASSERT(m_spFormatDynamicPage != NULL);
		
		//设置属性页站点
		CComPtr<IPropertyPageSite> spPageSite;
		_InternalQueryInterface(IID_IPropertyPageSite, (void**)&spPageSite);
		m_spFormatDynamicPage->SetPageSite(spPageSite);
			
		PROPPAGEINFO proppageinfo;
		m_spFormatDynamicPage->GetPageInfo(&proppageinfo);
		RECT rcDisplay;
		GetClientRect(&rcDisplay);
		rcDisplay.top += 80;
		m_spFormatDynamicPage->Activate(m_hWnd, &rcDisplay, TRUE);
		IUnknown* pUnk = m_spFormatDynamic;
		m_spFormatDynamicPage->SetObjects(1, (IUnknown **)&pUnk);
		m_spFormatDynamicPage->Show(SW_SHOW);
	}
	
	return 0;
}

#include "../CxDynObjs/PropInfo.h"
#include "../CxScripCrt/CxScripCrt.h"
#include "../CxScripCrt/CxScripCrt_i.c"

LRESULT CDataLinkGenPage::OnClickedCreateExpression(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;
	
	CComBSTR bstr;
	GetDlgItemText(IDC_EXPRESSION, (BSTR&)bstr); 

	OnCreateExpression(&bstr);
		
	SetDlgItemText(IDC_EXPRESSION, W2T(bstr));
	
	return 0;
}

STDMETHODIMP CDataLinkGenPage::Apply(void)
{
	USES_CONVERSION;
	USES_CHINESEVAR_CONVERSION;

	HRESULT hr = m_spFormatDynamicPage->Apply();
	if (FAILED(hr))
		return hr;

	CComBSTR bstrExpression;
	GetDlgItemText(IDC_EXPRESSION, (BSTR&)bstrExpression); 
	bstrExpression = VARC2A(W2T(bstrExpression));

	m_spFormatDynamic->put_Source(bstrExpression);

	m_bDirty = FALSE;

	if (m_bObjectIsFormmatDynamic)
		return S_OK;

	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<IDrawDataLink, &IID_IDrawDataLink> spDataLink(m_ppUnk[i]);
		
		CComPtr<IFormatDynamic> spFormatDynamic;
		m_spFormatDynamic->Clone(&spFormatDynamic);
		CComBSTR bstrSample;
		spFormatDynamic->get_DisplaySample(&bstrSample);
		spDataLink->put_Value(bstrSample);
		spDataLink->ConnectObject(L"Value", spFormatDynamic);
	}

	return S_OK;
}

void CDataLinkGenPage::OnFinalMessage(HWND hWnd)
{
	if (m_spFormatDynamicPage)
	{
		m_spFormatDynamicPage->Deactivate();
		m_spFormatDynamicPage.Release();
	}
}

STDMETHODIMP CDataLinkGenPage::EditDynamic(long hParent, long X, long Y)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDataLinkGenPage::get_Source(/*[out, retval]*/ BSTR *pVal)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDataLinkGenPage::put_Source(/*[in]*/ BSTR newVal)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDataLinkGenPage::get_PropertyInfo(/*[out, retval]*/ long *pVal)
{
	*pVal = long(&m_propinfo);
	return S_OK;
}

STDMETHODIMP CDataLinkGenPage::put_PropertyInfo(/*[in]*/ long newVal)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDataLinkGenPage::OnCreateExpression(BSTR* pbstr)
{
	USES_CONVERSION;
	
	CComBSTR bstr;
	if (*pbstr != NULL)
		bstr = *pbstr;
	
	if (m_bObjectIsFormmatDynamic)
	{
		CComQIPtr<ICxDynamicObject> spDynamicObject = m_spFormatDynamic;
		
		CDynamicPropInfo* pPropinfo = NULL;
		spDynamicObject->GetPropInfo((long *)&pPropinfo);
		if (pPropinfo != NULL && pPropinfo->pfnExpression != NULL)
		{
			pPropinfo->pfnExpression(&bstr);
		}
		else
		{
			CComPtr<ICxExpressionCreator> p;		
			HRESULT hr = p.CoCreateInstance(CLSID_CxExpressionCreator); 
			if (FAILED(hr)) 
				return hr;
			
			p->put_Expression(bstr);
			hr = p->ShowDialog();
			if (FAILED(hr)) 
				return hr;
			p->get_Expression(&bstr);
		}
	}
	else 
	{
		CComQIPtr<ICxDrawObject, &IID_ICxDrawObject> spDrawObject(m_ppUnk[0]);
		CComPtr<ICxDrawObjectSite> spDrawObjectSite;
		spDrawObject->GetDrawObjectSite(&spDrawObjectSite);
		if (spDrawObjectSite != NULL)
		{
			ExpressionCreatorProc pfn = NULL;
			spDrawObjectSite->GetExpressionCreatorProc(long(&pfn));
			pfn(&bstr);
		}
	}
	
	if (*pbstr != NULL)
		SysFreeString(*pbstr);
	*pbstr = bstr.Copy();
	
	return S_OK;
}
