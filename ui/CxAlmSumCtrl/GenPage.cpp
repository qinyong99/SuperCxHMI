// GenPage.cpp : Implementation of CGenPage
#include "stdafx.h"
#include "CxAlmSumCtrl.h"
#include "GenPage.h"

/////////////////////////////////////////////////////////////////////////////
// CGenPage

LRESULT CGenPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	m_btnShowHeader.Attach(GetDlgItem(IDC_HEADER));
	m_btnShowScrollBar.Attach(GetDlgItem(IDC_SCROOLBAR));
	m_btnShowGrid.Attach(GetDlgItem(IDC_GRID));

	m_btnBackColor.SubclassWindow(GetDlgItem(IDC_BACKCOLOR));
	m_btnHeaderForeColor.SubclassWindow(GetDlgItem(IDC_HEADER_FORECOLOR));
	m_btnHeaderBackColor.SubclassWindow(GetDlgItem(IDC_HEADER_BACKCOLOR));
	m_btnBackColor.SetDefaultColor(GetSysColor(COLOR_WINDOW));
	m_btnHeaderForeColor.SetDefaultColor(GetSysColor(COLOR_BTNTEXT));
	m_btnHeaderBackColor.SetDefaultColor(GetSysColor(COLOR_BTNFACE));

	m_editHeaderFont.Attach(GetDlgItem(IDC_HEADER_FONT));

	m_bLoadPropery = TRUE;
	
	if (m_nObjects > 0)
	{
		CComQIPtr<ICxAlmSum, &IID_ICxAlmSum> spAlmSum(m_ppUnk[0]);
		
		VARIANT_BOOL b;
		spAlmSum->get_ShowHeader(&b);
		m_btnShowHeader.SetCheck(b == VARIANT_TRUE);
		
		spAlmSum->get_ShowScrollBar(&b);
		m_btnShowScrollBar.SetCheck(b == VARIANT_TRUE);
		
		spAlmSum->get_ShowGrid(&b);
		m_btnShowGrid.SetCheck(b == VARIANT_TRUE);

		OLE_COLOR clr;
		spAlmSum->get_BackColor(&clr);
		m_btnBackColor.SetColor(clr);

		spAlmSum->get_HeaderBackColor(&clr);
		m_btnHeaderBackColor.SetColor(clr);

		spAlmSum->get_HeaderForeColor(&clr);
		m_btnHeaderForeColor.SetColor(clr);

		spAlmSum->get_HeaderFont(&m_spHeaderFont);
		if (m_spHeaderFont != NULL)
		{	
			CComDispatchDriver dd(m_spHeaderFont);
			CComVariant var;
			dd.GetProperty(0, &var);
			if (var.vt == VT_BSTR)
				m_editHeaderFont.SetWindowText(OLE2T(var.bstrVal));
		}
		else
		{
			m_editHeaderFont.SetWindowText(_T(""));
		}

		long l;
		spAlmSum->get_RowHeight(&l);
		SetDlgItemInt(IDC_ROW_HEIGHT, l);
	}
	
	m_bLoadPropery = FALSE;
	
	return 0;
}

STDMETHODIMP CGenPage::Apply()
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<ICxAlmSum, &IID_ICxAlmSum> spAlmSum(m_ppUnk[i]);
		
		spAlmSum->put_ShowHeader(m_btnShowHeader.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
		spAlmSum->put_ShowScrollBar(m_btnShowScrollBar.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
		spAlmSum->put_ShowGrid(m_btnShowGrid.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);

		spAlmSum->put_BackColor(m_btnBackColor.GetColor());
		spAlmSum->put_HeaderBackColor(m_btnHeaderBackColor.GetColor());
		spAlmSum->put_HeaderForeColor(m_btnHeaderForeColor.GetColor());

		spAlmSum->put_HeaderFont(m_spHeaderFont);

		spAlmSum->put_RowHeight(GetDlgItemInt(IDC_ROW_HEIGHT));
	}
	
	m_bDirty = FALSE;
	return S_OK;
}

LRESULT CGenPage::OnClickedHeaderfontPick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;
	
	if (PickFont(m_hWnd, &m_spHeaderFont.p))
		OnSelchangeValue(wNotifyCode, wID, hWndCtl, bHandled);
	
	if (m_spHeaderFont != NULL)
	{	
		CComDispatchDriver dd(m_spHeaderFont);
		CComVariant var;
		dd.GetProperty(0, &var);
		if (var.vt == VT_BSTR)
			m_editHeaderFont.SetWindowText(OLE2T(var.bstrVal));
	}
	else
	{
		m_editHeaderFont.SetWindowText(_T(""));
	}
	
	return 0;
}

