// DirectPage.cpp : Implementation of CDirectPage
#include "stdafx.h"
#include "CxDynObjs.h"
#include "DirectPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDirectPage

HRESULT CDirectPage::SetPageSite(IPropertyPageSite *pPageSite)
{
	HRESULT hr = IPropertyPageImpl<CDirectPage>::SetPageSite(pPageSite);
	if (FAILED(hr))
		return hr;

	hr = CrackPropertyType();

	return hr;		
}

//获取页面类型
HRESULT CDirectPage::CrackPropertyType()
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

HRESULT CDirectPage::SetObjects(ULONG nObjects, IUnknown **ppUnk)
{
	HRESULT hr = IPropertyPageImpl<CDirectPage>::SetObjects(nObjects, ppUnk);
	if (SUCCEEDED(hr))
	{
		IDirectDynamic* pDynamic;
		m_ppUnk[0]->QueryInterface(IID_IDirectDynamic, (void**)&pDynamic);
		if (pDynamic != NULL)
		{
			BOOL b;

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

HRESULT CDirectPage::Apply(void)
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		IDirectDynamic* pDynamic;
		m_ppUnk[i]->QueryInterface(IID_IDirectDynamic, (void**)&pDynamic);
		
		BOOL b;
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
