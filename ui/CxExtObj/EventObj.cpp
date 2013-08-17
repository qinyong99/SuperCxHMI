// EventObj.cpp : Implementation of CEventObj
#include "stdafx.h"
#include "CxExtObj.h"
#include "EventObj.h"

/////////////////////////////////////////////////////////////////////////////
// CEventObj


STDMETHODIMP CEventObj::get_altKey(VARIANT_BOOL *pVal)
{
	*pVal = altKey;

	return S_OK;
}

STDMETHODIMP CEventObj::put_altKey(VARIANT_BOOL newVal)
{
	altKey = newVal;

	return S_OK;
}

STDMETHODIMP CEventObj::get_ctrlKey(VARIANT_BOOL *pVal)
{
	*pVal = ctrlKey;

	return S_OK;
}

STDMETHODIMP CEventObj::put_ctrlKey(VARIANT_BOOL newVal)
{
	ctrlKey = newVal;

	return S_OK;
}

STDMETHODIMP CEventObj::get_shiftKey(VARIANT_BOOL *pVal)
{
	*pVal = shiftKey;

	return S_OK;
}

STDMETHODIMP CEventObj::put_shiftKey(VARIANT_BOOL newVal)
{
	shiftKey = newVal;

	return S_OK;
}

STDMETHODIMP CEventObj::get_returnValue(VARIANT *pVal)
{
	::VariantCopy(pVal, &returnValue);

	return S_OK;
}

STDMETHODIMP CEventObj::put_returnValue(VARIANT newVal)
{
	::VariantCopy(&returnValue, &newVal);

	return S_OK;
}

STDMETHODIMP CEventObj::get_cancelBubble(VARIANT_BOOL *pVal)
{
	*pVal = cancelBubble;

	return S_OK;
}

STDMETHODIMP CEventObj::put_cancelBubble(VARIANT_BOOL newVal)
{
	cancelBubble = newVal;

	return S_OK;
}

STDMETHODIMP CEventObj::get_keyCode(long *pVal)
{
	*pVal = keyCode;

	return S_OK;
}

STDMETHODIMP CEventObj::put_keyCode(long newVal)
{
	keyCode = newVal;

	return S_OK;
}

STDMETHODIMP CEventObj::get_button(long *pVal)
{
	*pVal = button;

	return S_OK;
}

STDMETHODIMP CEventObj::put_button(long newVal)
{
	button = newVal;

	return S_OK;
}

STDMETHODIMP CEventObj::get_x(long *pVal)
{
	*pVal = x;

	return S_OK;
}

STDMETHODIMP CEventObj::put_x(long newVal)
{
	x = newVal;

	return S_OK;
}

STDMETHODIMP CEventObj::get_y(long *pVal)
{
	*pVal = y;

	return S_OK;
}

STDMETHODIMP CEventObj::put_y(long newVal)
{
	y = newVal;

	return S_OK;
}

STDMETHODIMP CEventObj::get_clientX(long *pVal)
{
	*pVal = clientX;
	
	return S_OK;
}

STDMETHODIMP CEventObj::put_clientX(long newVal)
{
	clientX = newVal;
	
	return S_OK;
}

STDMETHODIMP CEventObj::get_clientY(long *pVal)
{
	*pVal = clientY;
	
	return S_OK;
}

STDMETHODIMP CEventObj::put_clientY(long newVal)
{
	clientY = newVal;
	
	return S_OK;
}

STDMETHODIMP CEventObj::get_screenX(long *pVal)
{
	*pVal = screenX;
	
	return S_OK;
}

STDMETHODIMP CEventObj::put_screenX(long newVal)
{
	screenX = newVal;
	
	return S_OK;
}

STDMETHODIMP CEventObj::get_screenY(long *pVal)
{
	*pVal = screenY;
	
	return S_OK;
}

STDMETHODIMP CEventObj::put_screenY(long newVal)
{
	screenY = newVal;
	
	return S_OK;
}


