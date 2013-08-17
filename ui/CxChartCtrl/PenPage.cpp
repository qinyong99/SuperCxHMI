// PenPage.cpp: implementation of the CPenPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PenPage.h"
#include "CxChartCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LRESULT CPenPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_cmbLineStyle.Init(GetDlgItem(IDC_LINE_STYLE));
	m_cmbMarkerStyle.Init(GetDlgItem(IDC_MARKER_STYLE));

	m_btnLineColor.SubclassWindow(GetDlgItem(IDC_LINE_COLOR));

	m_editLineWidth.Attach(GetDlgItem(IDC_LINE_WIDTH));
	m_editPointsNum.Attach(GetDlgItem(IDC_POINTS_NUM));
	m_btnShow.Attach(GetDlgItem(IDC_SHOW));

	return 0;
}

STDMETHODIMP CPenPage::SetObjects(ULONG nObjects, IUnknown **ppUnk)
{
	m_bLoadPropery = TRUE;

	HRESULT hr = IPropertyPageImpl<CPenPage>::SetObjects(nObjects, ppUnk);

	if (m_nObjects > 0)
	{
		CComQIPtr<ISerial, &IID_ISerial> spSerial(m_ppUnk[0]);

		TCHAR szBuf[8];

//		double d;
//		spSerial->get_MaxValue(&d);
//		sprintf(szBuf, "%.2f", (float)d);
//		m_editMaxValue.SetWindowText(szBuf);
//
//		spSerial->get_MinValue(&d);
//		sprintf(szBuf, "%.2f", (float)d);
//		m_editMinValue.SetWindowText(szBuf);

		VARIANT_BOOL b;
		spSerial->get_Show(&b);
		m_btnShow.SetCheck(b == VARIANT_TRUE);

		long l;
		spSerial->get_MaxNumDisplayPoints(&l);
		sprintf(szBuf, "%d", l);
		m_editPointsNum.SetWindowText(szBuf);

		OLE_COLOR clr;
		spSerial->get_LineColor(&clr);
		m_btnLineColor.SetColor(clr);
		m_btnLineColor.SetDefaultColor(clr);

		spSerial->get_LineWidth(&l);
		sprintf(szBuf, "%d", l);
		m_editLineWidth.SetWindowText(szBuf);

		int iSel;
		spSerial->get_LineStyle((enumLineStyle *)&iSel);
		m_cmbLineStyle.SetCurSel(iSel);

		spSerial->get_MarkerStyle((enumMarkerStyle *)&iSel);
		m_cmbMarkerStyle.SetCurSel(iSel);

		spSerial->get_MinValueLimit(&m_dMinValLimit);
		spSerial->get_MaxValueLimit(&m_dMaxValLimit);
		DoDataExchange();
	}

	m_bLoadPropery = FALSE;

	return hr;
}

STDMETHODIMP CPenPage::Apply()
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<ISerial, &IID_ISerial> spSerial(m_ppUnk[i]);

		spSerial->put_Show(m_btnShow.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
		
		spSerial->put_LineColor(m_btnLineColor.GetColor());
		spSerial->put_LineStyle((enumLineStyle)m_cmbLineStyle.GetCurSel());
		spSerial->put_MarkerStyle((enumMarkerStyle)m_cmbMarkerStyle.GetCurSel());

		CComBSTR bstr;
//		m_editMaxValue.GetWindowText((BSTR&)bstr);
		CComVariant var = bstr;
//		var.ChangeType(VT_R4);
//		if (var.vt == VT_R4)
//			spSerial->put_MaxValue(var.fltVal);
//
//		m_editMinValue.GetWindowText((BSTR&)bstr);
//		var = bstr;
//		var.ChangeType(VT_R4);
//		if (var.vt == VT_R4)
//			spSerial->put_MinValue(var.fltVal);


		m_editPointsNum.GetWindowText((BSTR&)bstr);
		var = bstr;
		var.ChangeType(VT_I4);
		if (var.vt == VT_I4 && var.lVal >= 2)
			spSerial->put_MaxNumDisplayPoints(var.lVal);

		m_editLineWidth.GetWindowText((BSTR&)bstr);
		var = bstr;
		var.ChangeType(VT_I4);
		if (var.vt == VT_I4)
			spSerial->put_LineWidth(var.lVal);

		DoDataExchange(TRUE);
		spSerial->put_MinValueLimit(m_dMinValLimit);
		spSerial->put_MaxValueLimit(m_dMaxValLimit);
	}
	m_bDirty = FALSE;
	return S_OK;
}
