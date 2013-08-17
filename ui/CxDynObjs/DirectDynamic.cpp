// DirectDynamic.cpp : Implementation of CDirectDynamic
#include "stdafx.h"
#include "CxDynObjs.h"
#include "DirectDynamic.h"

/////////////////////////////////////////////////////////////////////////////
// CDirectDynamic

STDMETHODIMP CDirectDynamic::GetClassID(CLSID* pClassID)
{
	*pClassID = GetObjectCLSID();
	return S_OK;
}

STDMETHODIMP CDirectDynamic::IsDirty()
{
	return (m_bRequiresSave) ? S_OK : S_FALSE;
}

STDMETHODIMP CDirectDynamic::InitNew(long lPropInfo)
{
	m_bEnableMouseInput = FALSE;
	
	return S_OK;
}

STDMETHODIMP CDirectDynamic::Load(LPSTREAM pStm)
{
	m_bstrSource.ReadFromStream(pStm);
	pStm->Read(&m_bEnableMouseInput, sizeof(m_bEnableMouseInput), NULL);
	return S_OK;
}

STDMETHODIMP CDirectDynamic::Save(LPSTREAM pStm, BOOL fClearDirty)
{
	m_bstrSource.WriteToStream(pStm);
	pStm->Write(&m_bEnableMouseInput, sizeof(m_bEnableMouseInput), NULL);
	return S_OK;
}

STDMETHODIMP CDirectDynamic::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = ::SysAllocString(L"Direct");
	return S_OK;
}

STDMETHODIMP CDirectDynamic::GetCustomPage(CLSID* pclsidPropPage)
{
	*pclsidPropPage = CLSID_DirectPage;
	return S_OK;
}

STDMETHODIMP CDirectDynamic::get_Source(BSTR *pVal)
{
	*pVal = m_bstrSource.Copy();
	return S_OK;
}

STDMETHODIMP CDirectDynamic::put_Source(BSTR newVal)
{
	m_bstrSource = newVal;
	return S_OK;
}

STDMETHODIMP CDirectDynamic::get_EnableMouseInput(BOOL *pVal)
{
	*pVal = m_bEnableMouseInput;
	
	return S_OK;
}

STDMETHODIMP CDirectDynamic::put_EnableMouseInput(BOOL newVal)
{
	m_bEnableMouseInput = newVal;
	
	return S_OK;
}

STDMETHODIMP CDirectDynamic::Clone(IDirectDynamic** ppDynamic)
{
	HRESULT hr = ::CoCreateInstance(
		CLSID_DirectDynamic,         
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IDirectDynamic,
		reinterpret_cast<void**>(ppDynamic));
	if (FAILED(hr))
		return hr;
	
	(*ppDynamic)->put_Source(m_bstrSource);
	(*ppDynamic)->put_EnableMouseInput(m_bEnableMouseInput ? VARIANT_TRUE : VARIANT_FALSE);
	
	return S_OK;
}
