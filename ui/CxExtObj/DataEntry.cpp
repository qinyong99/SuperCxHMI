// DataEntry.cpp : Implementation of CDataEntry
#include "stdafx.h"
#include "CxExtObj.h"
#include "DataEntry.h"
#include "AlphaNumEntryDlg.h"
#include "SliderEntryDlg.h"
#include "PushbuttonEntryDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDataEntry


STDMETHODIMP CDataEntry::get_mode(enumDataEntryMode *pVal)
{
	*pVal = m_enumDataEntryMode;		
	
	return S_OK;
}

STDMETHODIMP CDataEntry::put_mode(enumDataEntryMode newVal)
{
	m_enumDataEntryMode = newVal;

	return S_OK;
}

STDMETHODIMP CDataEntry::get_var(BSTR *pVal)
{
	*pVal = m_bstrVal;

	return S_OK;
}

STDMETHODIMP CDataEntry::put_var(BSTR newVal)
{
	m_bstrVal = newVal;

	return S_OK;
}

STDMETHODIMP CDataEntry::get_min(VARIANT *pVal)
{
	*pVal = m_varMin;

	return S_OK;
}

STDMETHODIMP CDataEntry::put_min(VARIANT newVal)
{
	m_varMin = newVal;

	return S_OK;
}

STDMETHODIMP CDataEntry::get_max(VARIANT *pVal)
{
	*pVal = m_varMax;

	return S_OK;
}

STDMETHODIMP CDataEntry::put_max(VARIANT newVal)
{
	m_varMax = newVal;
	
	return S_OK;
}

STDMETHODIMP CDataEntry::get_closeCaption(BSTR *pVal)
{
	*pVal = m_bstrCloseCaption;

	return S_OK;
}

STDMETHODIMP CDataEntry::put_closeCaption(BSTR newVal)
{
	m_bstrCloseCaption = newVal;

	return S_OK;
}

STDMETHODIMP CDataEntry::get_openCaption(BSTR *pVal)
{
	*pVal = m_bstrOpenCaption;

	return S_OK;
}

STDMETHODIMP CDataEntry::put_openCaption(BSTR newVal)
{
	m_bstrOpenCaption = newVal;

	return S_OK;
}

HWND GetSafeOwner_(HWND hParent, HWND* pWndTop)
{
	// get window to start with
	HWND hWnd = hParent;
	if (hWnd == NULL)
		hWnd = ::GetActiveWindow();
	
	// a popup window cannot be owned by a child window
	while (hWnd != NULL && (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD))
		hWnd = ::GetParent(hWnd);
	
	// determine toplevel window to disable as well
	HWND hWndTop = hWnd, hWndTemp = hWnd;
	for (;;)
	{
		if (hWndTemp == NULL)
			break;
		else
			hWndTop = hWndTemp;
		hWndTemp = ::GetParent(hWndTop);
	}
	
	// get last active popup of first non-child that was found
	if (hParent == NULL && hWnd != NULL)
		hWnd = ::GetLastActivePopup(hWnd);
	
	// disable and store top level parent window if specified
	if (pWndTop != NULL)
	{
		if (hWndTop != NULL && ::IsWindowEnabled(hWndTop) && hWndTop != hWnd)
		{
			*pWndTop = hWndTop;
			::EnableWindow(hWndTop, FALSE);
		}
		else
			*pWndTop = NULL;
	}
	
	return hWnd;    // return the owner as HWND
}

STDMETHODIMP CDataEntry::show()
{
	if (m_vbFetch == VARIANT_TRUE)
		m_spScriptEngineSite->PreTreatExpressionEx(m_bstrVal, NULL, &m_varMin, &m_varMax);

	CComQIPtr<IActiveScriptSiteWindow> spActiveScriptSiteWindow = m_spScriptEngineSite;
	BOOL bEnableMode = spActiveScriptSiteWindow->EnableModeless(false) == S_OK;

	HWND hWndTop;
	HWND hWndParent = GetSafeOwner_(NULL, &hWndTop);
	BOOL bEnableParent = FALSE;
	if (hWndParent != NULL && ::IsWindowEnabled(hWndParent))
	{
		::EnableWindow(hWndParent, FALSE);
		bEnableParent = TRUE;
	}

	switch (m_enumDataEntryMode)
	{
	case DataEntryModeNumeric:
		{
			CAlphaNumEntryDlg dlg(this);
			dlg.DoModal();
		}
		break;
	case DataEntryModeText:
		{
			CAlphaNumEntryDlg dlg(this, FALSE);
			dlg.DoModal();
		}
		break;
	case DataEntryModeSlider:
		{
			CSliderEntryDlg dlg(this);
			dlg.DoModal();
		}
		break;
	case DataEntryModePushbutton:
		{
			CPushbuttonEntryDlg dlg(this);
			dlg.DoModal();
		}
		break;
	default:
		break;
	}

	if (bEnableParent)
		::EnableWindow(hWndParent, TRUE);
	//	if (hWndParent != NULL && ::GetActiveWindow() == hWndDialog)
	//		::SetActiveWindow(hWndParent);
	
	if (hWndTop != NULL)
		::EnableWindow(hWndTop, TRUE);

	if (bEnableMode)
		spActiveScriptSiteWindow->EnableModeless(true);

	return S_OK;
}

STDMETHODIMP CDataEntry::SetScriptEngineSite(IUnknown *punkSite)
{
	m_spScriptEngineSite = punkSite;

	return S_OK;
}

BOOL CDataEntry::GetValue(VARIANT* pVar)
{
	ATLASSERT(m_spScriptEngineSite != NULL);
	//求表达式值
	HRESULT hr = m_spScriptEngineSite->EvaluateExpression(m_bstrVal, pVar);
	return (hr == S_OK);
}

BOOL CDataEntry::PutValue(VARIANT* pVar)
{
	USES_CONVERSION;
	USES_CHINESEVAR_CONVERSION;

	ATLASSERT(m_spScriptEngineSite != NULL);

	CComVariant var(*pVar);
	var.ChangeType(VT_BSTR);
	
	CComBSTR bstrStatement = m_bstrVal;
	bstrStatement = VARC2A(W2T(bstrStatement));
	bstrStatement.Append("=");
	bstrStatement.AppendBSTR(var.bstrVal);
	
	return (m_spScriptEngineSite->Execute(bstrStatement) == S_OK);
}

STDMETHODIMP CDataEntry::get_fetch(VARIANT_BOOL *pVal)
{
	*pVal = m_vbFetch;

	return S_OK;
}

STDMETHODIMP CDataEntry::put_fetch(VARIANT_BOOL newVal)
{
	m_vbFetch = newVal;

	return S_OK;
}
