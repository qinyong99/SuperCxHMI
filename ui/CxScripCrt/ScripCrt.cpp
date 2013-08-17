// ScripCrt.cpp : Implementation of CScripCrt
#include "stdafx.h"
#include "CxScripCrt.h"
#include "ScripCrt.h"

/////////////////////////////////////////////////////////////////////////////
// CScripCrt

STDMETHODIMP CScripCrt::SetSite(ICxScriptEditorSite* pSite)
{
	m_spSite = pSite;
	m_dwSymbolMask = 0;
	
	return S_OK;
}

STDMETHODIMP CScripCrt::SetScriptText(LPCWSTR lpszCode)
{
	m_bstrScriptText = lpszCode;

	return S_OK;
}

STDMETHODIMP CScripCrt::GetScriptText(LPWSTR* ppszCode)
{
	int cb = (m_bstrScriptText.Length() + 1) * sizeof(WCHAR);
	*ppszCode = (LPWSTR)CoTaskMemAlloc(cb);
	wcscpy(*ppszCode, m_bstrScriptText);
	
	return S_OK;
}

STDMETHODIMP CScripCrt::EditObjectEventHandler(LPCWSTR lpszObject, LPCWSTR lpszEvent)
{
	USES_CONVERSION;

	CMainFrame wndFrame;
	CString strText = m_bstrScriptText;
	BOOL bModified = FALSE;
	wndFrame.Show(m_hParentWnd, m_spSite, m_dwSymbolMask, strText, bModified, W2CT(lpszObject), W2CT(lpszEvent));
	if (bModified)
		m_bstrScriptText = strText;

	return bModified ? S_OK : S_FALSE;
}


STDMETHODIMP CScripCrt::SetParentWindow(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;

	return S_OK;
}

STDMETHODIMP CScripCrt::SetSupportSymbol(DWORD dwSymbolMask)
{
	m_dwSymbolMask = dwSymbolMask;

	return S_OK;
}