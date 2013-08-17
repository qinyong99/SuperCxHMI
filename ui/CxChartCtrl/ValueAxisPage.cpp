// ValueAxisPage.cpp: implementation of the CValueAxisPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueAxisPage.h"
#include "CxChartCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LRESULT CValueAxisPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_btnAxisColor.SubclassWindow(GetDlgItem(IDC_AXIS_COLOR));
	m_btnTitleColor.SubclassWindow(GetDlgItem(IDC_TITLE_COLOR));
	m_editTicksNum.Attach(GetDlgItem(IDC_TICKS_NUM));
	m_editTitle.Attach(GetDlgItem(IDC_TITLE));
	m_btnShowAxis.Attach(GetDlgItem(IDC_AXIS_SHOW));
	m_btnShowTitle.Attach(GetDlgItem(IDC_TITLE_SHOW));
	m_editTickFont.Attach(GetDlgItem(IDC_TICK_FONT));
	m_editTitleFont.Attach(GetDlgItem(IDC_TITLE_FONT));

	return 0;
}

STDMETHODIMP CValueAxisPage::SetObjects(ULONG nObjects, IUnknown **ppUnk)
{
	USES_CONVERSION;

	m_bLoadPropery = TRUE;

	IPropertyPageImpl<CValueAxisPage>::SetObjects(nObjects, ppUnk);

	if (m_nObjects > 0)
	{
		CComPtr<IValueAxis> spAxis;
		CComQIPtr<ISerial, &IID_ISerial> spSerial(m_ppUnk[0]);
		CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[0]);
		
		if (spTrend != NULL)
			spTrend->get_ValueAxis(&spAxis);
		else
			spSerial->get_ValueAxis(&spAxis);

		TCHAR szBuf[8];

		long l;
		spAxis->get_NumTicks(&l);
		sprintf(szBuf, "%d", l);
		m_editTicksNum.SetWindowText(szBuf);

		OLE_COLOR clr;
		spAxis->get_AxisColor(&clr);
		m_btnAxisColor.SetColor(clr);

		spAxis->get_TitleColor(&clr);
		m_btnTitleColor.SetColor(clr);

		clr = RGB(0, 0, 0);
		if (spSerial != NULL)
			spSerial->get_LineColor(&clr);
		m_btnAxisColor.SetDefaultColor(clr);
		m_btnTitleColor.SetDefaultColor(clr);

		CComBSTR bstr;
		spAxis->get_Title(&bstr);
		m_editTitle.SetWindowText(OLE2T(bstr));

		VARIANT_BOOL b;
		spAxis->get_ShowAxis(&b);
		m_btnShowAxis.SetCheck(b == VARIANT_TRUE);

		spAxis->get_ShowTitle(&b);
		m_btnShowTitle.SetCheck(b == VARIANT_TRUE);

		spAxis->get_WholeDigits(&l);
		SetDlgItemInt(IDC_WHOLEDIGITS, l, FALSE);

		spAxis->get_DecimalDigits(&l);
		SetDlgItemInt(IDC_DECIMALDIGITS, l, FALSE);

		spAxis->get_MinValue(&m_dMinVal);
		spAxis->get_MaxValue(&m_dMaxVal);
		DoDataExchange();

//		double d;
//		spSerial->get_MaxValue(&d);
//		sprintf(szBuf, "%.2f", (float)d);
//		m_editMaxValue.SetWindowText(szBuf);


// 		spAxis->get_DecimalDigits(&l);
// 		SetDlgItemInt(IDC_DECIMALDIGITS, l, FALSE);

		m_spTickFont.Release();
		spAxis->get_TickFont(&m_spTickFont);
		if (m_spTickFont != NULL)
		{	
			CComDispatchDriver dd(m_spTickFont);
			CComVariant var;
			dd.GetProperty(0, &var);
			if (var.vt == VT_BSTR)
				m_editTickFont.SetWindowText(OLE2T(var.bstrVal));
		}
		else
		{
			m_editTickFont.SetWindowText(_T(""));
		}
		
		m_spTitleFont.Release();
		spAxis->get_TitleFont(&m_spTitleFont);
		if (m_spTitleFont != NULL)
		{	
			CComDispatchDriver dd(m_spTitleFont);
			CComVariant var;
			dd.GetProperty(0, &var);
			if (var.vt == VT_BSTR)
				m_editTitleFont.SetWindowText(OLE2T(var.bstrVal));
		}
		else
		{
			m_editTitleFont.SetWindowText(_T(""));
		}
	}

	m_bLoadPropery = FALSE;

	return S_OK;
}

STDMETHODIMP CValueAxisPage::Apply()
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComPtr<IValueAxis> spAxis;
		CComQIPtr<ISerial, &IID_ISerial> spSerial(m_ppUnk[0]);
		CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[0]);
		
		if (spTrend != NULL)
			spTrend->get_ValueAxis(&spAxis);
		else
			spSerial->get_ValueAxis(&spAxis);
		
		spAxis->put_AxisColor(m_btnAxisColor.GetColor());
		spAxis->put_TitleColor(m_btnTitleColor.GetColor());

		CComBSTR bstr;
		m_editTicksNum.GetWindowText((BSTR&)bstr);
		CComVariant var = bstr;
		var.ChangeType(VT_I4);
		if (var.vt == VT_I4)
			spAxis->put_NumTicks(var.lVal);

		bstr;
		m_editTitle.GetWindowText((BSTR&)bstr);
		spAxis->put_Title(bstr);

		spAxis->put_ShowAxis(m_btnShowAxis.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
		spAxis->put_ShowTitle(m_btnShowTitle.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);

		long l = GetDlgItemInt(IDC_WHOLEDIGITS);
		spAxis->put_WholeDigits(l);
		l = GetDlgItemInt(IDC_DECIMALDIGITS);
		spAxis->put_DecimalDigits(l);

		spAxis->put_TickFont(m_spTickFont);
		spAxis->put_TitleFont(m_spTitleFont);

		DoDataExchange(TRUE);
		spAxis->put_MinValue(m_dMinVal);
		spAxis->put_MaxValue(m_dMaxVal);
	}
	
	m_bDirty = FALSE;
	return S_OK;
}

LRESULT CValueAxisPage::OnClickedTickfontPick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;
	
	if (PickFont(m_hWnd, &m_spTickFont.p))
		OnSelchangeValue(wNotifyCode, wID, hWndCtl, bHandled);
	
	if (m_spTickFont != NULL)
	{	
		CComDispatchDriver dd(m_spTickFont);
		CComVariant var;
		dd.GetProperty(0, &var);
		if (var.vt == VT_BSTR)
			m_editTickFont.SetWindowText(OLE2T(var.bstrVal));
	}
	else
	{
		m_editTickFont.SetWindowText(_T(""));
	}
	
	return 0;
}

LRESULT CValueAxisPage::OnClickedTitlefontPick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;
	
	if (PickFont(m_hWnd, &m_spTitleFont.p))
		OnSelchangeValue(wNotifyCode, wID, hWndCtl, bHandled);
	
	if (m_spTitleFont != NULL)
	{	
		CComDispatchDriver dd(m_spTitleFont);
		CComVariant var;
		dd.GetProperty(0, &var);
		if (var.vt == VT_BSTR)
			m_editTitleFont.SetWindowText(OLE2T(var.bstrVal));
	}
	else
	{
		m_editTitleFont.SetWindowText(_T(""));
	}
	
	return 0;
}
