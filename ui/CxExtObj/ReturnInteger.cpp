// ReturnInteger.cpp : Implementation of CReturnInteger
#include "stdafx.h"
#include "CxExtObj.h"
#include "ReturnInteger.h"

/////////////////////////////////////////////////////////////////////////////
// CReturnInteger


STDMETHODIMP CReturnInteger::get_Value(int *pVal)
{
	*pVal = m_Value;

	return S_OK;
}

STDMETHODIMP CReturnInteger::put_Value(int newVal)
{
	m_Value = newVal;

	return S_OK;
}
