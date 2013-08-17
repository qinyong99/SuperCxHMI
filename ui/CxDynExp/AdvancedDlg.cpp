// AdvancedDlg.cpp: implementation of the CAdvancedDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AdvancedDlg.h"
#include "DynamicExpert.h"

#include "../CxOle/CxHmiObj_i.c"
#include "../CxDynObjs/CxDynObjs_i.c"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAdvancedDlg::CAdvancedDlg()
{
	m_pExpert = NULL;
	m_nCurDyn = -1;
	m_bCanChangeMode = TRUE;
}

CAdvancedDlg::~CAdvancedDlg()
{
}

LRESULT CAdvancedDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	
	
	return S_OK;
}

void CAdvancedDlg::OnShow()
{
	USES_CONVERSION;

	CPropInfo prop;
	m_pExpert->GetSelectProp(prop);
//	SetDlgItemText(IDC_PROP, OLE2T(prop.m_bstrDesc));
	
	m_pExpert->GetSource((BSTR&)m_bstrSource);

	m_pExpert->InitDynamicPropInfo();
	m_pPropInfo = m_pExpert->m_pPropInfo;

	CLSID clsidDesire = CLSID_NULL;
	switch (m_pPropInfo->nType)
	{
	case PROPTYPE_UNKNOWN:
	case PROPTYPE_COLOR:
	case PROPTYPE_ENUM:
	case PROPTYPE_ENUMSTRING:
	case PROPTYPE_FAKEFILE:
	case PROPTYPE_FAKEFOLDER:	
		clsidDesire = CLSID_LookupDynamic;
		break;
	case PROPTYPE_NUMBER:
		clsidDesire = CLSID_LinearDynamic;
		break;
	case PROPTYPE_STRING:
	case PROPTYPE_FAKEFONTNAME:
	case PROPTYPE_TAGGROUP:
	case PROPTYPE_SECURITYAREA:
		clsidDesire = CLSID_FormatDynamic;
		break;
	default:
		clsidDesire = CLSID_DirectDynamic;
	}

	int nDynamic = -1;

	if (m_pPropInfo->spUnkDynamic != NULL)
	{
		CLSID clsidDynamic = CLSID_NULL;
		CDynamicExpert::GetDynamicClassID(m_pPropInfo->spUnkDynamic, &clsidDynamic);
		if (!m_bCanChangeMode && !InlineIsEqualGUID(clsidDesire, clsidDynamic))
		{
			m_pExpert->SelectAdvacedProp(prop.m_dispid);
			return;
		}

		for (int i = 0; i < m_apDynInfos.GetSize(); i++)
		{
			CDynamicInfo* pInfo = m_apDynInfos[i];
			if (pInfo->spUnkDynamic == m_pPropInfo->spUnkDynamic)
			{
				nDynamic = i;
				break;
			}
		}
		
		if (nDynamic == -1)
		{
			CDynamicInfo* pInfo = new CDynamicInfo;
			pInfo->spUnkDynamic = m_pPropInfo->spUnkDynamic;
			pInfo->clsid = clsidDynamic;
			m_apDynInfos.Add(pInfo);
			nDynamic = m_apDynInfos.GetSize() - 1;
		}
	}
	else
	{
		nDynamic = GetDynamic(clsidDesire);
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
	}
	
	int nCmdShow = m_bCanChangeMode ? SW_SHOW : SW_HIDE;
	for (int i = IDC_LOOKUP; i <= IDC_DIRECT; i++)
		::ShowWindow(GetDlgItem(i), nCmdShow);
	::ShowWindow(GetDlgItem(IDC_DIV), nCmdShow);
		
	CheckRadioButton(IDC_LOOKUP, IDC_DIRECT, nIDCheckButton);

	ShowDynamicPage(nDynamic);
}

void CAdvancedDlg::OnOK()
{
	BOOL bValid = m_pExpert->GetSource((BSTR&)m_bstrSource);
			
	if (!bValid)
	{
		m_pExpert->DeleteDynamic(m_pPropInfo->bstrPropertyName);
		return;
	}
		
	ATLASSERT(m_nCurDyn != -1);
	CDynamicInfo* pInfo = m_apDynInfos[m_nCurDyn];
	
	ATLASSERT(pInfo->spUnkDynamic != NULL);
	CComDispatchDriver dd(pInfo->spUnkDynamic);
	CComVariant var(m_bstrSource);
	dd.PutPropertyByName(L"Source", &var);
	
	ATLASSERT(pInfo->spPropPage != NULL);
	HRESULT hr = pInfo->spPropPage->Apply();
	if (hr != S_OK)
		return;

	m_pPropInfo->spUnkDynamic = pInfo->spUnkDynamic;
	m_pExpert->PutDynamic(m_pPropInfo->bstrPropertyName, m_pPropInfo->spUnkDynamic);
}

HRESULT CAdvancedDlg::PropertyChange()
{
	return S_OK;
}

void CAdvancedDlg::HideDynamicPage()
{
	for (int i = 0; i < m_apDynInfos.GetSize(); i++)
	{
		CDynamicInfo* pInfo = m_apDynInfos[i];
		if (pInfo->spPropPage != NULL)
			pInfo->spPropPage->Deactivate();
		
		delete pInfo;
	}
	
	m_apDynInfos.RemoveAll();

	m_nCurDyn = -1;
}

void CAdvancedDlg::OnFinalMessage(HWND hWnd)
{
	HideDynamicPage();	
}


int CAdvancedDlg::GetDynamic(CLSID clsid)
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

LRESULT CAdvancedDlg::OnClickedLinear(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nDynamic = GetDynamic(CLSID_LinearDynamic);
	ShowDynamicPage(nDynamic);
	
	return 0;
}

LRESULT CAdvancedDlg::OnClickedLookup(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nDynamic = GetDynamic(CLSID_LookupDynamic);
	ShowDynamicPage(nDynamic);
	
	return 0;
}

LRESULT CAdvancedDlg::OnClickedFormat(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nDynamic = GetDynamic(CLSID_FormatDynamic);
	ShowDynamicPage(nDynamic);
	
	return 0;
}

LRESULT CAdvancedDlg::OnClickedDirect(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nDynamic = GetDynamic(CLSID_DirectDynamic);
	ShowDynamicPage(nDynamic);
	
	return 0;
}

void CAdvancedDlg::ShowDynamicPage(int nDynamic)
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
		if (!m_bCanChangeMode)
			::OffsetRect(&rcDisplay, 30, 0);
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

STDMETHODIMP CAdvancedDlg::EditDynamic(long hParent, long X, long Y)
{
	return E_NOTIMPL;
}

STDMETHODIMP CAdvancedDlg::get_PropertyInfo(long *pVal)
{
	*pVal = long(m_pPropInfo);
	
	return S_OK;
}

STDMETHODIMP CAdvancedDlg::put_PropertyInfo(long newVal)
{
	return E_NOTIMPL;
}

STDMETHODIMP CAdvancedDlg::get_Source(BSTR *pbstr)
{
	*pbstr = m_bstrSource.Copy();
		
	return S_OK;
}

STDMETHODIMP CAdvancedDlg::put_Source(BSTR bstr)
{
	m_bstrSource = bstr;
	
	return S_OK;
}

STDMETHODIMP CAdvancedDlg::OnCreateExpression(BSTR *pbstr)
{
	return E_NOTIMPL;
}