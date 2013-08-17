// OperatorAPage.cpp : Implementation of COperatorAPage
#include "stdafx.h"
#include "CxAlmSumCtrl.h"
#include "OperatorPage.h"

/////////////////////////////////////////////////////////////////////////////
// COperatorAPage

LRESULT COperatorPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_btnAck.Attach(GetDlgItem(IDC_ACK));
	m_btnContextMenu.Attach(GetDlgItem(IDC_CONTEXT_MENU));
	m_btnOnlineConfigure.Attach(GetDlgItem(IDC_ONLINE_CONFIGURE));

	m_bLoadPropery = TRUE;

	if (m_nObjects > 0)
	{
		CComQIPtr<ICxAlmSum, &IID_ICxAlmSum> spAlmSum(m_ppUnk[0]);
		
		VARIANT_BOOL b;
		spAlmSum->get_AckEnabled(&b);
		m_btnAck.SetCheck(b == VARIANT_TRUE);
		
		spAlmSum->get_ContextMenuEnabled(&b);
		m_btnContextMenu.SetCheck(b == VARIANT_TRUE);
		
		spAlmSum->get_OnlineConfigureEnabled(&b);
		m_btnOnlineConfigure.SetCheck(b == VARIANT_TRUE);
	}

	m_bLoadPropery = FALSE;

	return 0;
}

STDMETHODIMP COperatorPage::Apply()
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<ICxAlmSum, &IID_ICxAlmSum> spAlmSum(m_ppUnk[i]);
		
		spAlmSum->put_AckEnabled(m_btnAck.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
		spAlmSum->put_ContextMenuEnabled(m_btnContextMenu.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
		spAlmSum->put_OnlineConfigureEnabled(m_btnOnlineConfigure.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
	}
	
	m_bDirty = FALSE;
	return S_OK;
}
