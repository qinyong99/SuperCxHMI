// RangeToFormat.cpp : Implementation of CRangeToFormat
#include "stdafx.h"
#include "CxScripCrt.h"
#include "RangeToFormat.h"

/////////////////////////////////////////////////////////////////////////////
// CRangeToFormat

CRangeToFormat::CRangeToFormat()
{
	memset(&m_rf, 0, sizeof(RangeToFormat));
}

STDMETHODIMP CRangeToFormat::get_DC(long *pVal)
{
	*pVal = (long)m_rf.hdc;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::put_DC(long newVal)
{
	m_rf.hdc = (HDC)newVal;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::get_TargetDC(long *pVal)
{
	*pVal = (long)m_rf.hdcTarget;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::put_TargetDC(long newVal)
{
	m_rf.hdcTarget = (HDC)newVal;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::get_Left(long *pVal)
{
	*pVal = m_rf.rc.left;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::put_Left(long newVal)
{
	m_rf.rc.left = newVal;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::get_Top(long *pVal)
{
	*pVal = m_rf.rc.top;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::put_Top(long newVal)
{
	m_rf.rc.top = newVal;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::get_Right(long *pVal)
{
	*pVal = m_rf.rc.right;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::put_Right(long newVal)
{
	m_rf.rc.right = newVal;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::get_Bottom(long *pVal)
{
	*pVal = m_rf.rc.bottom;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::put_Bottom(long newVal)
{
	m_rf.rc.bottom = newVal;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::get_PageLeft(long *pVal)
{
	*pVal = m_rf.rcPage.left;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::put_PageLeft(long newVal)
{
	m_rf.rcPage.left = newVal;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::get_PageTop(long *pVal)
{
	*pVal = m_rf.rcPage.top;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::put_PageTop(long newVal)
{
	m_rf.rcPage.top = newVal;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::get_PageRight(long *pVal)
{
	*pVal = m_rf.rcPage.right;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::put_PageRight(long newVal)
{
	m_rf.rcPage.right = newVal;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::get_PageBottom(long *pVal)
{
	*pVal = m_rf.rcPage.bottom;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::put_PageBottom(long newVal)
{
	m_rf.rcPage.bottom = newVal;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::get_Min(long *pVal)
{
	*pVal = m_rf.chrg.cpMin;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::put_Min(long newVal)
{
	m_rf.chrg.cpMin = newVal;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::get_Max(long *pVal)
{
	*pVal = m_rf.chrg.cpMax;

	return S_OK;
}

STDMETHODIMP CRangeToFormat::put_Max(long newVal)
{
	m_rf.chrg.cpMax = newVal;

	return S_OK;
}
