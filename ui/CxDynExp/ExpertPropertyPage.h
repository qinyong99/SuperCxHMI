// ExpertPropertyPage.h: Definition of the CExpertPropertyPage class
//
//////////////////////////////////////////////////////////////////////

#ifndef _EXPERTPROPERTYPAGE_H__
#define _EXPERTPROPERTYPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DialogSizeHelper.h"

template<class T>
class CExpertPropertyPage : public IPropertyPageImpl<T>
{
protected:
	CExpertPropertyPage()
	{
		m_ppEnumProp = NULL; 
		m_bShowHelpString = FALSE;
		m_bSupportMultiObject = TRUE;
	}
	~CExpertPropertyPage()
	{
		for (UINT iObj = 0; iObj < m_nObjects; iObj++)
		{
			if (m_ppEnumProp[iObj] != NULL)
				m_ppEnumProp[iObj]->Release();
		}
		
		delete [] m_ppEnumProp;
	}
	STDMETHOD(GetPageInfo)(PROPPAGEINFO *pPageInfo)
	{
		T* pT = static_cast<T*>(this);
		ATLTRACE2(atlTraceControls,2,_T("IPropertyPageImpl::GetPageInfo\n"));
		
		if (pPageInfo == NULL)
		{
			ATLTRACE2(atlTraceControls,2,_T("Error : PROPPAGEINFO passed == NULL\n"));
			return E_POINTER;
		}
		
		HRSRC hRsrc = FindResource(_Module.GetResourceInstance(),
			MAKEINTRESOURCE(T::IDD), RT_DIALOG);
		if (hRsrc == NULL)
		{
			ATLTRACE2(atlTraceControls,2,_T("Could not find resource template\n"));
			return E_UNEXPECTED;
		}
		
		HGLOBAL hGlob = LoadResource(_Module.GetResourceInstance(), hRsrc);
		DLGTEMPLATE* pDlgTempl = (DLGTEMPLATE*)LockResource(hGlob);
		if (pDlgTempl == NULL)
		{
			ATLTRACE2(atlTraceControls,2,_T("Could not load resource template\n"));
			return E_UNEXPECTED;
		}
		CDialogSizeHelper::GetDialogSize2(pDlgTempl, &m_size);
		
		pPageInfo->cb = sizeof(PROPPAGEINFO);
		pPageInfo->pszTitle = LoadStringHelper(pT->m_dwTitleID);
		pPageInfo->size = m_size;
		pPageInfo->pszHelpFile = LoadStringHelper(pT->m_dwHelpFileID);
		pPageInfo->pszDocString = LoadStringHelper(pT->m_dwDocStringID);
		pPageInfo->dwHelpContext = pT->m_dwHelpContext;
		
		return S_OK;
	}
	STDMETHOD(ShowHelpString)(BOOL bShow)
	{
		m_bShowHelpString = bShow;
		return S_OK;
	}

	STDMETHOD(IsSupportMultiObject)()
	{
		return m_bSupportMultiObject ? S_OK : S_FALSE;
	}

	STDMETHOD(SetExtendObjects)(ULONG nObjects, IUnknown **ppUnk, ICxEnumExtendProperty** ppEnumProp)
	{
		if (nObjects == 0 && m_nObjects == 0)
			return S_OK;

		if (m_nObjects > 0)
		{
			for (UINT iObj = 0; iObj < m_nObjects; iObj++)
			{
				m_ppUnk[iObj]->Release();
				if (m_ppEnumProp[iObj] != NULL)
					m_ppEnumProp[iObj]->Release();
			}
			
			delete [] m_ppUnk;
			delete [] m_ppEnumProp;
			
			m_ppUnk = NULL;
			m_ppEnumProp = NULL;
			m_nObjects = 0;
		}
		
		ATLTRY(m_ppUnk = new IUnknown*[nObjects]);
		if (m_ppUnk == NULL)
			return E_OUTOFMEMORY;

		ATLTRY(m_ppEnumProp = new ICxEnumExtendProperty*[nObjects]);
		if (m_ppEnumProp == NULL)
		{
			delete [] m_ppUnk;
			m_ppUnk = NULL;
			return E_OUTOFMEMORY;
		}
		
		for (UINT i = 0; i < nObjects; i++)
		{
			ppUnk[i]->AddRef();
			m_ppUnk[i] = ppUnk[i];
			if (ppEnumProp[i] != NULL)
				ppEnumProp[i]->AddRef();
			m_ppEnumProp[i] = ppEnumProp[i];
		}
		
		m_nObjects = nObjects;
			
		T* pT = static_cast<T*>(this);
		pT->OnObjectChanged();
		
		return S_OK;
	}

	STDMETHOD(SetObjects)(ULONG nObjects, IUnknown **ppUnk)
	{
		HRESULT hr = S_OK;
		
		if (nObjects == 0 || ppUnk == NULL)
		{
			if (m_nObjects == 0 || m_ppUnk == NULL)
				return S_OK;
			
			if (m_ppUnk != NULL && m_nObjects > 0)
			{
				for (UINT iObj = 0; iObj < m_nObjects; iObj++)
					m_ppUnk[iObj]->Release();
				
				delete [] m_ppUnk;
				
				m_ppUnk = NULL;
				m_nObjects = 0;
			}
		}
		else
		{
			hr = IPropertyPageImpl<T>::SetObjects(nObjects, ppUnk);
		}
		
		T* pT = static_cast<T*>(this);
		pT->OnObjectChanged();
		
		return hr;
	}

	HRESULT SetPropertyValue(DISPID dwDispID, VARIANT* pVar)
	{
		HRESULT hr;
		BOOL bChanged = FALSE;
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_VALIDATE);
		ULONG nObjects = m_nObjects;
		if (m_bSupportMultiObject && nObjects > 1)
			nObjects = 1;
		for (ULONG iObj = 0; iObj < nObjects; iObj++)
		{
			CComDispatchDriver dd(m_ppUnk[iObj]);
			hr = dd.PutProperty(dwDispID, pVar);
			if (SUCCEEDED(hr))
				bChanged = TRUE;
		}
		if (bChanged)
			m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);

		return S_OK;		
	}

	HRESULT GetPropertyVale(DISPID dwDispID, VARIANT* pVar)
	{
		BOOL bGet = FALSE;
		ULONG nObjects = m_nObjects;
		if (m_bSupportMultiObject && nObjects > 1)
			nObjects = 1;
		for (ULONG iObj = 0; iObj < nObjects; iObj++)
		{
			CComVariant var;
			CComDispatchDriver dd(m_ppUnk[iObj]);
			HRESULT hr = dd.GetProperty(dwDispID, &var);
			if (SUCCEEDED(hr))
			{
				//因为CComVariant重载的==操作没有VT_UI4类型
				if (var.vt = VT_UI4)
					var.vt = VT_I4;
				
				if (!bGet)
					::VariantCopy(pVar, &var);
				else if (var != *pVar)
					return S_FALSE;
				
				bGet = TRUE; 
			}
		}
		
		return bGet ? S_OK : S_FALSE;
	}

	ICxEnumExtendProperty** m_ppEnumProp;
	BOOL m_bShowHelpString;
	BOOL m_bSupportMultiObject;
};

#endif // _EXPERTPROPERTYPAGE_H__
