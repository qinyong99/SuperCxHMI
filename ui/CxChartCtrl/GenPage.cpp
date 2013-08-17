// GenPage.cpp : Implementation of CGenPage
#include "stdafx.h"
#include "CxChartCtrl.h"
#include "GenPage.h"

/////////////////////////////////////////////////////////////////////////////
// CGenPage

LRESULT CGenPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	m_editCaption.Attach(GetDlgItem(IDC_CAPTION));
	m_editRefreshInterval.Attach(GetDlgItem(IDC_REFRESH_INTERVAL));
	m_btnTransparency.Attach(GetDlgItem(IDC_TRANSPARENCY));

	m_btnForeColor.SubclassWindow(GetDlgItem(IDC_FORE_COLOR));
	m_btnBackColor.SubclassWindow(GetDlgItem(IDC_BACK_COLOR));
	m_btnFrameColor.SubclassWindow(GetDlgItem(IDC_FRAME_COLOR));
	m_btnForeColor.SetDefaultColor(GetSysColor(COLOR_WINDOW));
	m_btnBackColor.SetDefaultColor(GetSysColor(COLOR_BTNFACE));
	m_btnFrameColor.SetDefaultColor(GetSysColor(COLOR_BTNFACE));

	m_btnShowLegend.Attach(GetDlgItem(IDC_SHOW_LEGEND));
	m_btnShowMutiTimeAxis.Attach(GetDlgItem(IDC_SHOW_MUTITIMEAXIS));
	m_btnShowMutiValueAxis.Attach(GetDlgItem(IDC_SHOW_MUTIVALEAXIS));
	m_btnShowMutiGrid.Attach(GetDlgItem(IDC_SHOW_MUTIGRID));
	m_btnShowToolPane.Attach(GetDlgItem(IDC_SHOW_TOOLPANE));
	m_btnShowList.Attach(GetDlgItem(IDC_SHOW_LIST));

	m_cmbFrameType.Attach(GetDlgItem(IDC_FRAME_TYPE));
	m_cmbFrameType.ResetContent();
	m_cmbFrameType.InsertString(0, _T("ÎÞ"));
	m_cmbFrameType.InsertString(1, _T("Í»³ö"));
	m_cmbFrameType.InsertString(2, _T("ÏÂ½µ"));
	m_cmbFrameType.InsertString(3, _T("Ê´¿Ì"));
	m_cmbFrameType.InsertString(4, _T("×²»÷"));
	m_cmbFrameType.InsertString(5, _T("¾ØÐÎ"));
	
	m_bLoadPropery = TRUE;

	if (m_nObjects > 0)
	{
		CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[0]);
		
		TCHAR szBuf[8];

		long l;
		spTrend->get_RefreshInterval(&l);
		sprintf(szBuf, "%d", l);
		m_editRefreshInterval.SetWindowText(szBuf);

		CComBSTR bstr;
		spTrend->get_Caption(&bstr);
		m_editCaption.SetWindowText(OLE2T(bstr));

		OLE_COLOR clr;
		spTrend->get_ForeColor(&clr);
		m_btnForeColor.SetColor(clr);
		
		spTrend->get_BackColor(&clr);
		m_btnBackColor.SetColor(clr);

		spTrend->get_FrameColor(&clr);
		m_btnFrameColor.SetColor(clr);

		VARIANT_BOOL b;
		spTrend->get_ShowLegend(&b);
		m_btnShowLegend.SetCheck(b == VARIANT_TRUE);

		spTrend->get_Transparency(&b);
		m_btnTransparency.SetCheck(b == VARIANT_TRUE);

		spTrend->get_ShowMutiTimeAxis(&b);
		m_btnShowMutiTimeAxis.SetCheck(b == VARIANT_TRUE);

		spTrend->get_ShowMutiValueAxis(&b);
		m_btnShowMutiValueAxis.SetCheck(b == VARIANT_TRUE);

		spTrend->get_ShowMutiGrid(&b);
		m_btnShowMutiGrid.SetCheck(b == VARIANT_TRUE);

		spTrend->get_ScrollDirection((enumScrollDirection *)&l);
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1 + l);

		spTrend->get_ZoomMode((enumZoomMode *)&l);
		CheckRadioButton(IDC_RADIO3, IDC_RADIO5, IDC_RADIO3 + l);

		spTrend->get_TrendMode((enumTrendMode *)&l);
		CheckRadioButton(IDC_RADIO6, IDC_RADIO7, IDC_RADIO6 + l);

		spTrend->get_FrameStyle((enumFrameStyle *)&l);
		m_cmbFrameType.SetCurSel(l);

		CComQIPtr<ICxTrendCtrl, &IID_ICxTrendCtrl> spTrendCtrl(m_ppUnk[0]);
		if (spTrendCtrl != NULL)
		{
			spTrendCtrl->get_ShowToolPane(&b);
			m_btnShowToolPane.SetCheck(b == VARIANT_TRUE);
			
			spTrendCtrl->get_ShowList(&b);
			m_btnShowList.SetCheck(b == VARIANT_TRUE);
		}
		else
		{
			m_btnShowToolPane.EnableWindow(FALSE);
			m_btnShowList.EnableWindow(FALSE);
		}
	}

	m_bLoadPropery = FALSE;

	return 0;
}

STDMETHODIMP CGenPage::Apply()
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[i]);
		
		CComBSTR bstr;
		m_editRefreshInterval.GetWindowText((BSTR&)bstr);
		CComVariant var = bstr;
		var.ChangeType(VT_I4);
		if (var.vt == VT_I4)
			spTrend->put_RefreshInterval(var.lVal);

		m_editCaption.GetWindowText((BSTR&)bstr);
		spTrend->put_Caption(bstr);

		spTrend->put_ForeColor(m_btnForeColor.GetColor());
		spTrend->put_BackColor(m_btnBackColor.GetColor());
		spTrend->put_FrameColor(m_btnFrameColor.GetColor());

		spTrend->put_Transparency(m_btnTransparency.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
		spTrend->put_ShowLegend(m_btnShowLegend.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
		spTrend->put_ShowMutiTimeAxis(m_btnShowMutiTimeAxis.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
		spTrend->put_ShowMutiValueAxis(m_btnShowMutiValueAxis.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
		spTrend->put_ShowMutiGrid(m_btnShowMutiGrid.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
			
		spTrend->put_ScrollDirection(IsDlgButtonChecked(IDC_RADIO1) ? ScrollDirectionLeft : ScrollDirectionRight);
		
		if (IsDlgButtonChecked(IDC_RADIO3))
			spTrend->put_ZoomMode(ZoomModeHorizontal);
		else if (IsDlgButtonChecked(IDC_RADIO4))
			spTrend->put_ZoomMode(ZoomModeVertical);
		else 
			spTrend->put_ZoomMode(ZoomModeBoth);

		spTrend->put_TrendMode(IsDlgButtonChecked(IDC_RADIO6) ? TrendModeRealTime : TrendModeHistory);

		int iIndex = m_cmbFrameType.GetCurSel();
		spTrend->put_FrameStyle((enumFrameStyle)iIndex);

		CComQIPtr<ICxTrendCtrl, &IID_ICxTrendCtrl> spTrendCtrl(m_ppUnk[0]);
		if (spTrendCtrl != NULL)
		{
			spTrendCtrl->put_ShowToolPane(m_btnShowToolPane.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
			spTrendCtrl->put_ShowList(m_btnShowList.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
		}
	}
	m_bDirty = FALSE;

	return S_OK;
}

