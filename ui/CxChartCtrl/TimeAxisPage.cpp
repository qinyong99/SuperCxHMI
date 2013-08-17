// TimeAxisPage.cpp: implementation of the CTimeAxisPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimeAxisPage.h"
#include "CxChartCtrl.h"
#include "OleDateTime.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const static TCHAR* szTimeFormats[] =
{
	_T("%Y-%m-%d\\»»ÐÐ%H:%M:%S"),
	_T("%Y-%m-%d %H:%M:%S"),
	_T("%y-%m-%d\\»»ÐÐ%H:%M:%S"),
	_T("%y-%m-%d %H:%M:%S"),
	_T("%Y-%m-%d"),
	_T("%y-%m-%d"),
	_T("%H:%M:%S"),
	_T("%M:%S"),
	_T("%S")
};

LRESULT CTimeAxisPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_btnAxisColor.SubclassWindow(GetDlgItem(IDC_AXIS_COLOR));
	m_btnTitleColor.SubclassWindow(GetDlgItem(IDC_TITLE_COLOR));
	m_editTicksNum.Attach(GetDlgItem(IDC_TICKS_NUM));
	m_editTitle.Attach(GetDlgItem(IDC_TITLE));
	m_btnShowAxis.Attach(GetDlgItem(IDC_AXIS_SHOW));
	m_btnShowTitle.Attach(GetDlgItem(IDC_TITLE_SHOW));
	m_cbFormat.Attach(GetDlgItem(IDC_FORMAT));
	m_editTickFont.Attach(GetDlgItem(IDC_TICK_FONT));
	m_editTitleFont.Attach(GetDlgItem(IDC_TITLE_FONT));

	COleDateTime datetime = COleDateTime::GetCurrentTime();
	for (int i = 0; i < 9; i++)
	{
		CString strFormat = datetime.Format(szTimeFormats[i]);
		m_cbFormat.AddString(strFormat);
	}

	return 0;
}

STDMETHODIMP CTimeAxisPage::SetObjects(ULONG nObjects, IUnknown **ppUnk)
{
	USES_CONVERSION;

	m_bLoadPropery = TRUE;

	IPropertyPageImpl<CTimeAxisPage>::SetObjects(nObjects, ppUnk);

	if (m_nObjects > 0)
	{
		CComPtr<ITimeAxis> spAxis;
		CComQIPtr<ISerial, &IID_ISerial> spSerial(m_ppUnk[0]);
		CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[0]);

		if (spTrend != NULL)
			spTrend->get_TimeAxis(&spAxis);
		else
			spSerial->get_TimeAxis(&spAxis);
	
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

		bstr.Empty();
		spAxis->get_DisplayFormat(&bstr);
		for (int i = 0; i < 7; i++)
		{
			if (bstr == szTimeFormats[i])
			{
				m_cbFormat.SetCurSel(i);
				break;
			}
		}
	
		COleDateTimeSpan timespan;
		spAxis->get_TimeSpan((double *)&timespan);
		ULONGLONG dSeconds = (ULONGLONG)timespan.GetTotalSeconds();
		
		SetDlgItemInt(IDC_DAY, int(dSeconds / (24 * 60 * 60)), FALSE);
		dSeconds %= 24 * 60 * 60;
		SetDlgItemInt(IDC_HOUR, int(dSeconds / (60 * 60)), FALSE);
		dSeconds %= 60 * 60;
		SetDlgItemInt(IDC_MINUTE, int(dSeconds / 60), FALSE);
		dSeconds %= 60;
		SetDlgItemInt(IDC_SECOND, int(dSeconds), FALSE);

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

STDMETHODIMP CTimeAxisPage::Apply()
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComPtr<ITimeAxis> spAxis;
		CComQIPtr<ISerial, &IID_ISerial> spSerial(m_ppUnk[0]);
		CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[0]);
		
		if (spTrend != NULL)
			spTrend->get_TimeAxis(&spAxis);
		else
			spSerial->get_TimeAxis(&spAxis);
		
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

		int nSel = m_cbFormat.GetCurSel();
		if (nSel != CB_ERR)
		{
			bstr = szTimeFormats[nSel];
			spAxis->put_DisplayFormat(bstr);
		}
		
		spAxis->put_ShowAxis(m_btnShowAxis.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
		spAxis->put_ShowTitle(m_btnShowTitle.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);

		LONG lDay = GetDlgItemInt(IDC_DAY);
		int nHour = GetDlgItemInt(IDC_HOUR);
		int nMinute = GetDlgItemInt(IDC_MINUTE);
		int nSecond = GetDlgItemInt(IDC_SECOND);

		COleDateTimeSpan timespan(lDay, nHour, nMinute, nSecond);
		spAxis->put_TimeSpan(timespan.m_span);

		spAxis->put_TickFont(m_spTickFont);
		spAxis->put_TitleFont(m_spTitleFont);
	}
	
	m_bDirty = FALSE;
	return S_OK;
}

LRESULT CTimeAxisPage::OnClickedTickfontPick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

LRESULT CTimeAxisPage::OnClickedTitlefontPick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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


