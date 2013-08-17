// EnumProp.cpp : implementation file
//

#include "stdafx.h"
#include "DevInc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnumExtendProp

CEnumExtendProp::CEnumExtendProp(CProp* pProps, UINT nSize)
{
	m_pProps = pProps;
	m_nSize = nSize;
	m_nCurPos = -1;
	EnableAutomation();
}

CEnumExtendProp::~CEnumExtendProp()
{
	if (m_pProps != NULL)
		delete [] m_pProps;
	m_pProps = NULL;
	m_nSize = 0;
	m_nCurPos = 0;
}


BEGIN_INTERFACE_MAP(CEnumExtendProp, CCmdTarget)
	INTERFACE_PART(CEnumExtendProp, IID_ICxEnumExtendProperty, EnumProp)
END_INTERFACE_MAP()

//XEnumProp members
STDMETHODIMP_( ULONG ) CEnumExtendProp::XEnumProp::AddRef()
{
	METHOD_PROLOGUE(CEnumExtendProp, EnumProp)

    return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CEnumExtendProp::XEnumProp::Release()
{
	METHOD_PROLOGUE(CEnumExtendProp, EnumProp)

	return pThis->ExternalRelease();
}

STDMETHODIMP CEnumExtendProp::XEnumProp::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CEnumExtendProp, EnumProp)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CEnumExtendProp::XEnumProp::GetName(
	BSTR *pbstrName)
{
	METHOD_PROLOGUE(CEnumExtendProp, EnumProp)

	if(pThis->m_nCurPos >= pThis->m_nSize)
		return OLE_E_ENUM_NOMORE;

	*pbstrName = pThis->m_pProps[pThis->m_nCurPos].m_strName.AllocSysString();

	return S_OK;
}

STDMETHODIMP CEnumExtendProp::XEnumProp::GetDescription(BSTR *pbstrDescription)
{
	METHOD_PROLOGUE(CEnumExtendProp, EnumProp)

	if(pThis->m_nCurPos >= pThis->m_nSize)
		return OLE_E_ENUM_NOMORE;
	*pbstrDescription = pThis->m_pProps[pThis->m_nCurPos].m_strDescription.AllocSysString();

	return S_OK;
}

STDMETHODIMP CEnumExtendProp::XEnumProp::GetType(LONG *pIndexed)
{
	METHOD_PROLOGUE(CEnumExtendProp, EnumProp)

	if(pThis->m_nCurPos >= pThis->m_nSize)
		return OLE_E_ENUM_NOMORE;
	*pIndexed = (LONG)(pThis->m_pProps[pThis->m_nCurPos].m_proptype);

	return S_OK;
}

STDMETHODIMP CEnumExtendProp::XEnumProp::GetDispid(DISPID *pDispid)
{
	METHOD_PROLOGUE(CEnumExtendProp, EnumProp)

	if(pThis->m_nCurPos >= pThis->m_nSize)
		return OLE_E_ENUM_NOMORE;
	*pDispid = pThis->m_pProps[pThis->m_nCurPos].m_dispid;

	return S_OK;
}

STDMETHODIMP CEnumExtendProp::XEnumProp::Reset()
{
	METHOD_PROLOGUE(CEnumExtendProp, EnumProp)

	pThis->m_nCurPos = -1;

	return S_OK;
}

STDMETHODIMP CEnumExtendProp::XEnumProp::Next(LONG *pIndex)
{
	METHOD_PROLOGUE(CEnumExtendProp, EnumProp)

	pThis->m_nCurPos++;
	
	*pIndex = pThis->m_nCurPos;
	if(pThis->m_nCurPos > pThis->m_nSize - 1)
		return S_FALSE;

	return S_OK;
}

STDMETHODIMP CEnumExtendProp::XEnumProp::Skip(LONG celt)
{
	METHOD_PROLOGUE(CEnumExtendProp, EnumProp)

	int nNew = pThis->m_nCurPos - celt;
	if(nNew < 0 || nNew > pThis->m_nSize - 1)
		return TYPE_E_OUTOFBOUNDS;
	pThis->m_nCurPos = nNew;

	return S_OK;
}

STDMETHODIMP CEnumExtendProp::XEnumProp::Clone(ICxEnumExtendProperty **ppenum)
{
	METHOD_PROLOGUE(CEnumExtendProp, EnumProp)

	CProp* pProps = new CProp[pThis->m_nSize];
	if (pProps == NULL)
		return E_OUTOFMEMORY;

	for(int i = 0; i < pThis->m_nSize; i++)
	{
		pProps[i] = pThis->m_pProps[i];
	}

	CEnumExtendProp* pEnumExtendProp = new CEnumExtendProp(pProps, pThis->m_nSize);
	if (pEnumExtendProp == NULL)
	{
		delete [] pProps;
		return E_OUTOFMEMORY;
	}

	ICxEnumExtendProperty* pRet = (ICxEnumExtendProperty*)&(pEnumExtendProp->m_xEnumProp);
	pRet->AddRef();
	*ppenum = pRet;

	return S_OK;
}