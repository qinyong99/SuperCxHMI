// TextTextPage.cpp : Implementation of CTextTextPage
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "TextTextPage.h"

/////////////////////////////////////////////////////////////////////////////
// CTextTextPage

LRESULT CTextTextPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;
	
	m_editText.Attach(GetDlgItem(IDC_TEXT));
	
	m_bLoadPropery = TRUE;
	
	if (m_nObjects > 0)
	{
		CComQIPtr<IDrawTxt, &IID_IDrawTxt> spText(m_ppUnk[0]);
		
		CComBSTR bstr;
		spText->get_Caption(&bstr);
		m_editText.SetWindowText(OLE2T(bstr));
	}
	
	m_bLoadPropery = FALSE;
	
	return 0;
}

STDMETHODIMP CTextTextPage::Apply()
{
	USES_CONVERSION;
	
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<IDrawTxt, &IID_IDrawTxt> spText(m_ppUnk[i]);
		
		CComBSTR bstr;
		m_editText.GetWindowText((BSTR&)bstr);
		spText->put_Caption(bstr);
	}
	
	return S_OK;
}
