// FloatPoint.cpp : Implementation of CFloatPoint
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "FloatPoint.h"

/////////////////////////////////////////////////////////////////////////////
// CFloatPoint


STDMETHODIMP CFloatPoint::get_X(float *pVal)
{
	*pVal = m_X;

	return S_OK;
}

STDMETHODIMP CFloatPoint::put_X(float newVal)
{
	m_X = newVal;

	return S_OK;
}

STDMETHODIMP CFloatPoint::get_Y(float *pVal)
{
	*pVal = m_Y;

	return S_OK;
}

STDMETHODIMP CFloatPoint::put_Y(float newVal)
{
	m_Y = newVal;

	return S_OK;
}
