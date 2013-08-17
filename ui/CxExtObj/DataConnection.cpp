// DataConnection.cpp : Implementation of CDataConnection
#include "stdafx.h"
#include "CxExtObj.h"
#include "DataConnection.h"

/////////////////////////////////////////////////////////////////////////////
// CDataConnection


STDMETHODIMP CDataConnection::get_ConnectionString(BSTR *pVal)
{
	*pVal = m_bstrConnectionString.Copy();

	return S_OK;
}

STDMETHODIMP CDataConnection::put_ConnectionString(BSTR newVal)
{
	m_bstrConnectionString = newVal;

	return S_OK;
}

STDMETHODIMP CDataConnection::get_UserName(BSTR *pVal)
{
	*pVal = m_bstrUserName.Copy();

	return S_OK;
}

STDMETHODIMP CDataConnection::put_UserName(BSTR newVal)
{
	m_bstrUserName = newVal;

	return S_OK;
}

STDMETHODIMP CDataConnection::get_Password(BSTR *pVal)
{
	*pVal = m_bstrPassword.Copy();

	return S_OK;
}

STDMETHODIMP CDataConnection::put_Password(BSTR newVal)
{
	m_bstrPassword = newVal;

	return S_OK;
}

STDMETHODIMP CDataConnection::get_ConnectionTimeout(long *pVal)
{
	*pVal = m_lConnectionTimeout;

	return S_OK;
}

STDMETHODIMP CDataConnection::put_ConnectionTimeout(long newVal)
{
	m_lConnectionTimeout = newVal;

	return S_OK;
}

STDMETHODIMP CDataConnection::get_CommandTimeout(long *pVal)
{
	*pVal = m_lCommandTimeout;

	return S_OK;
}

STDMETHODIMP CDataConnection::put_CommandTimeout(long newVal)
{
	m_lCommandTimeout = newVal;

	return S_OK;
}
