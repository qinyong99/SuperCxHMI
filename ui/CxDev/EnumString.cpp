// EnumString.cpp : implementation file
//

#include "stdafx.h"
#include "cxdev.h"
#include "EnumString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnumString

CEnumString::CEnumString(LPOLESTR* pStrings, int nSize)
{
	m_nCurPos = 0;
	m_nSize = nSize;

	m_pStrings = new LPOLESTR[m_nSize];
	for (int i = 0; i < m_nSize; i++)
		m_pStrings[i] = CoTaskMemAllocString(pStrings[i]);
}

CEnumString::CEnumString(CStringArray& arrStrings)
{
	USES_CONVERSION;

	m_nCurPos = 0;
	m_nSize = arrStrings.GetSize();

	m_pStrings = new LPOLESTR[m_nSize];
	for (int i = 0; i < m_nSize; i++)
		m_pStrings[i] = CoTaskMemAllocString(T2CW(arrStrings[i]));

}

CEnumString::~CEnumString()
{
	for (int i = 0; i < m_nSize; i++)
		CoTaskMemFree(m_pStrings[i]);
	delete [] m_pStrings;
}

BEGIN_INTERFACE_MAP(CEnumString, CCmdTarget)
	INTERFACE_PART(CEnumString, IID_IEnumString, EnumString)
END_INTERFACE_MAP()

//XEnumString members
STDMETHODIMP_( ULONG ) CEnumString::XEnumString::AddRef()
{
	METHOD_PROLOGUE(CEnumString, EnumString)

    return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CEnumString::XEnumString::Release()
{
	METHOD_PROLOGUE(CEnumString, EnumString)

	return pThis->ExternalRelease();
}

STDMETHODIMP CEnumString::XEnumString::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CEnumString, EnumString)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CEnumString::XEnumString::Reset()
{
	METHOD_PROLOGUE(CEnumString, EnumString)

	pThis->m_nCurPos = 0;

	return S_OK;
}

STDMETHODIMP CEnumString::XEnumString::Next(ULONG celt, LPOLESTR* rgelt, ULONG* pceltFetched)
{
	METHOD_PROLOGUE(CEnumString, EnumString)

	if (rgelt == NULL || (celt != 1 && pceltFetched == NULL))
		return E_POINTER;
	if (pThis->m_nCurPos == pThis->m_nSize)
		return E_FAIL;

	ULONG nRem = (ULONG)(pThis->m_nSize - pThis->m_nCurPos);
	HRESULT hRes = S_OK;
	if (nRem < celt)
		hRes = S_FALSE;
	ULONG nMin = min(celt, nRem);
	if (pceltFetched != NULL)
		*pceltFetched = nMin;
	while (nMin--)
	{
		*rgelt = CoTaskMemAllocString(pThis->m_pStrings[pThis->m_nCurPos]);
		rgelt++;
		pThis->m_nCurPos++;
	}

	return hRes;
}

STDMETHODIMP CEnumString::XEnumString::Skip(ULONG celt)
{
	METHOD_PROLOGUE(CEnumString, EnumString)

	int nNew = pThis->m_nCurPos - celt;
	if(nNew < 0 || nNew > pThis->m_nSize - 1)
		return TYPE_E_OUTOFBOUNDS;
	pThis->m_nCurPos = nNew;

	return S_OK;
}

STDMETHODIMP CEnumString::XEnumString::Clone(IEnumString **ppenum)
{
	METHOD_PROLOGUE(CEnumString, EnumString)

	CEnumString* pEnumString = new CEnumString(pThis->m_pStrings, pThis->m_nSize);
	if (pEnumString == NULL)
		return E_OUTOFMEMORY;

	*ppenum = &pEnumString->m_xEnumString;

	return S_OK;
}

HRESULT CEnumString::CreateEnumString(CStringArray& arrStrings, IEnumString** ppEnumString)
{
	CEnumString* pEnumString = new CEnumString(arrStrings);
	if (pEnumString == NULL)
		return E_OUTOFMEMORY;
	
	*ppEnumString = &pEnumString->m_xEnumString;

	return S_OK;
}