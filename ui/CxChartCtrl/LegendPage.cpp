// LegendPage.cpp: implementation of the CLegendPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LegendPage.h"
#include "CxChartCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LRESULT CLegendPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_btnShow.Attach(GetDlgItem(IDC_SHOW));
	m_editDesc.Attach(GetDlgItem(IDC_DESC));
	m_editTextFont.Attach(GetDlgItem(IDC_TEXT_FONT));

	m_btnTextColor.SubclassWindow(GetDlgItem(IDC_TEXT_COLOR));
	m_btnTextColor.SetDefaultColor(COLOR_WINDOWTEXT);
	
	return 0;
}

STDMETHODIMP CLegendPage::SetObjects(ULONG nObjects, IUnknown **ppUnk)
{
	USES_CONVERSION;

	m_bLoadPropery = TRUE;

	IPropertyPageImpl<CLegendPage>::SetObjects(nObjects, ppUnk);

	if (m_nObjects > 0)
	{
		CComQIPtr<ISerial, &IID_ISerial> spSerial(m_ppUnk[0]);
		CComPtr<ILegend> spLegend;
		spSerial->get_Legend(&spLegend);

		VARIANT_BOOL b;
		spLegend->get_Show(&b);
		m_btnShow.SetCheck(b == VARIANT_TRUE);

		CComBSTR bstr;
		spLegend->get_Description(&bstr);
		m_editDesc.SetWindowText(OLE2T(bstr));

		m_spTextFont.Release();
		spLegend->get_TextFont(&m_spTextFont);
		if (m_spTextFont != NULL)
		{	
			CComDispatchDriver dd(m_spTextFont);
			CComVariant var;
			dd.GetProperty(0, &var);
			if (var.vt == VT_BSTR)
				m_editTextFont.SetWindowText(OLE2T(var.bstrVal));
		}
		else
		{
			m_editTextFont.SetWindowText(_T(""));
		}
		
		OLE_COLOR clr;
		spLegend->get_TextColor(&clr);
		m_btnTextColor.SetColor(clr);
	}

	m_bLoadPropery = FALSE;

	return S_OK;
}

STDMETHODIMP CLegendPage::Apply()
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<ISerial, &IID_ISerial> spSerial(m_ppUnk[i]);
		CComPtr<ILegend> spLegend;
		spSerial->get_Legend(&spLegend);
		
		spLegend->put_Show(m_btnShow.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);

		CComBSTR bstr;
		m_editDesc.GetWindowText((BSTR&)bstr);
		spLegend->put_Description(bstr);

		spLegend->put_TextFont(m_spTextFont);
		spLegend->put_TextColor(m_btnTextColor.GetColor());
	}
	
	m_bDirty = FALSE;
	return S_OK;
}

LRESULT CLegendPage::OnClickedTextfontPick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	if (PickFont(m_hWnd, &m_spTextFont.p))
		OnSelchangeValue(wNotifyCode, wID, hWndCtl, bHandled);

	if (m_spTextFont != NULL)
	{	
		CComDispatchDriver dd(m_spTextFont);
		CComVariant var;
		dd.GetProperty(0, &var);
		if (var.vt == VT_BSTR)
			m_editTextFont.SetWindowText(OLE2T(var.bstrVal));
	}
	else
	{
		m_editTextFont.SetWindowText(_T(""));
	}
	
	return 0;
}

