// GridPage.cpp: implementation of the CGridPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridPage.h"
#include "CxChartCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LRESULT CGridPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_btnHorColor.SubclassWindow(GetDlgItem(IDC_HOR_COLOR));
	m_btnVerColor.SubclassWindow(GetDlgItem(IDC_VER_COLOR));
	m_btnHorColor.SetDefaultColor(RGB(224 ,224, 224));
	m_btnVerColor.SetDefaultColor(RGB(224 ,224, 224));

	m_cmbHorStyle.Init(GetDlgItem(IDC_HOR_STYLE));
	m_cmbVerStyle.Init(GetDlgItem(IDC_VER_STYLE));
	
	m_editHorNum.Attach(GetDlgItem(IDC_HOR_NUM));
	m_editVerNum.Attach(GetDlgItem(IDC_VER_NUM));
	m_btnHorShow.Attach(GetDlgItem(IDC_HOR_SHOW));
	m_btnVerShow.Attach(GetDlgItem(IDC_VER_SHOW));

	return 0;
}

STDMETHODIMP CGridPage::SetObjects(ULONG nObjects, IUnknown **ppUnk)
{
	USES_CONVERSION;

	m_bLoadPropery = TRUE;

	IPropertyPageImpl<CGridPage>::SetObjects(nObjects, ppUnk);

	if (m_nObjects > 0)
	{
		CComPtr<IGrid> spGrid;
		CComQIPtr<ISerial, &IID_ISerial> spSerial(m_ppUnk[0]);
		CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[0]);
		
		if (spTrend != NULL)
			spTrend->get_Grid(&spGrid);
		else
			spSerial->get_Grid(&spGrid);

		TCHAR szBuf[8];

		long l;
		spGrid->get_NumberOfHorizontalLines(&l);
		sprintf(szBuf, "%d", l);
		m_editHorNum.SetWindowText(szBuf);
		
		spGrid->get_NumberOfVerticalLines(&l);
		sprintf(szBuf, "%d", l);
		m_editVerNum.SetWindowText(szBuf);

		OLE_COLOR clr;
		spGrid->get_HorizontalColor(&clr);
		m_btnHorColor.SetColor(clr);

		spGrid->get_VerticalColor(&clr);
		m_btnVerColor.SetColor(clr);

		VARIANT_BOOL b;
		spGrid->get_ShowHorizontal(&b);
		m_btnHorShow.SetCheck(b == VARIANT_TRUE);

		spGrid->get_ShowVertical(&b);
		m_btnVerShow.SetCheck(b == VARIANT_TRUE);

		int iSel;
		spGrid->get_HorizontalStyle((enumLineStyle *)&iSel);
		m_cmbHorStyle.SetCurSel(iSel);

		spGrid->get_VerticalStyle((enumLineStyle *)&iSel);
		m_cmbVerStyle.SetCurSel(iSel);
	}

	m_bLoadPropery = FALSE;

	return S_OK;
}

STDMETHODIMP CGridPage::Apply()
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComPtr<IGrid> spGrid;
		CComQIPtr<ISerial, &IID_ISerial> spSerial(m_ppUnk[0]);
		CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[0]);
		
		if (spTrend != NULL)
			spTrend->get_Grid(&spGrid);
		else
			spSerial->get_Grid(&spGrid);
		
		spGrid->put_HorizontalColor(m_btnHorColor.GetColor());
		spGrid->put_VerticalColor(m_btnVerColor.GetColor());

		spGrid->put_HorizontalStyle((enumLineStyle)m_cmbHorStyle.GetCurSel());
		spGrid->put_VerticalStyle((enumLineStyle)m_cmbVerStyle.GetCurSel());

		CComBSTR bstr;
		m_editHorNum.GetWindowText((BSTR&)bstr);
		CComVariant var = bstr;
		var.ChangeType(VT_I4);
		if (var.vt == VT_I4)
			spGrid->put_NumberOfHorizontalLines(var.lVal);

		m_editVerNum.GetWindowText((BSTR&)bstr);
		var = bstr;
		var.ChangeType(VT_I4);
		if (var.vt == VT_I4)
			spGrid->put_NumberOfVerticalLines(var.lVal);

		spGrid->put_ShowHorizontal(m_btnHorShow.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
		spGrid->put_ShowVertical(m_btnVerShow.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
	}
	
	m_bDirty = FALSE;
	return S_OK;
}
