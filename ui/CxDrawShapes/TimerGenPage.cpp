// TimerGenPage.cpp : Implementation of CTimerGenPage
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "TimerGenPage.h"

/////////////////////////////////////////////////////////////////////////////
// CTimerGenPage

LRESULT CTimerGenPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_btnEnabled.Attach(GetDlgItem(IDC_ENABLED));

	m_bLoadPropery = TRUE;
	
	if (m_nObjects > 0)
	{
		CComQIPtr<IDrawTimer, &IID_IDrawTimer> spTimer(m_ppUnk[0]);

		int n;
		spTimer->get_Interval(&n);
		SetDlgItemInt(IDC_INTERVAL, n);
		
		VARIANT_BOOL b;
		spTimer->get_Enabled(&b);
		m_btnEnabled.SetCheck(b != VARIANT_FALSE);
	}

	m_bLoadPropery = FALSE;
	
	return 0;
}

STDMETHODIMP CTimerGenPage::Apply(void)
{
	m_bDirty = FALSE;
	
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<IDrawTimer, &IID_IDrawTimer> spTimer(m_ppUnk[i]);
		
		int n = GetDlgItemInt(IDC_INTERVAL);
		spTimer->put_Interval(n);
		
		spTimer->put_Enabled(m_btnEnabled.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
	}
	
	return S_OK;
}

void CTimerGenPage::SetDirty()
{
	if (!m_bLoadPropery)
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
}

LRESULT CTimerGenPage::OnChangeInterval(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SetDirty();

	return 0;
}

LRESULT CTimerGenPage::OnClickedEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SetDirty();
	
	return 0;
}
