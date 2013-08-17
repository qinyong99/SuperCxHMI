// ColumPage.cpp : Implementation of CColumPage
#include "stdafx.h"
#include "CxAlmSumCtrl.h"
#include "ColumPage.h"

/////////////////////////////////////////////////////////////////////////////
// CColumPage

LRESULT CColumPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_listAvailable.Attach(GetDlgItem(IDC_LIST1));
	m_listSelected.Attach(GetDlgItem(IDC_LIST2));

	m_bLoadPropery = TRUE;
	
	if (m_nObjects > 0)
	{
		USES_CONVERSION;
		CComQIPtr<ICxAlmSum, &IID_ICxAlmSum> spAlmSum(m_ppUnk[0]);

		long lCount;
		spAlmSum->GetColumnCount(&lCount);
		for (long lCol = 1; lCol < lCount; lCol++)
		{
			if (spAlmSum->IsColumnShow(lCol) == S_OK)
				continue;

			CComBSTR bstr;
			spAlmSum->GetColumnLabel(lCol, &bstr);
			int iIndex = m_listAvailable.AddString(OLE2T(bstr));
			m_listAvailable.SetItemData(iIndex, (DWORD)lCol);
		}

		spAlmSum->GetShowColumnCount(&lCount);
		for (int iIndex = 1; iIndex < lCount; iIndex++)
		{
			spAlmSum->GetShowColumn(iIndex, &lCol);
			CComBSTR bstr;
			spAlmSum->GetColumnLabel(lCol, &bstr);
			int iIndex = m_listSelected.AddString(OLE2T(bstr));
			m_listSelected.SetItemData(iIndex, (DWORD)lCol);
		}
	}
	
	m_bLoadPropery = FALSE;

	return 0;
}

STDMETHODIMP CColumPage::Apply(void)
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<ICxAlmSum, &IID_ICxAlmSum> spAlmSum(m_ppUnk[i]);

		long lCount;
		spAlmSum->GetColumnCount(&lCount);
		for (int i = 1; i < lCount; i++)
			spAlmSum->ShowColumn(i, FALSE);

		lCount = m_listSelected.GetCount();
		for (i = 0; i < lCount; i++)
		{
			long l = (long)m_listSelected.GetItemData(i);
			spAlmSum->ShowColumn(l, TRUE);
		}
	}

	m_bDirty = FALSE;
	return S_OK;
}

LRESULT CColumPage::OnClickedAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	CComBSTR bstr;
	int iIndex = m_listAvailable.GetCurSel();
	if (iIndex == LB_ERR)
		return 0;

	m_listAvailable.GetTextBSTR(iIndex, (BSTR&)bstr);
	int i = (int)m_listAvailable.GetItemData(iIndex);
	m_listAvailable.DeleteString(iIndex);

	iIndex = m_listSelected.AddString(OLE2T(bstr));
	m_listSelected.SetItemData(iIndex, (DWORD)i);

	OnSelchangeValue(wNotifyCode, wID, hWndCtl, bHandled);

	return 0;
}
LRESULT CColumPage::OnClickedRemove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	CComBSTR bstr;
	int iIndex = m_listSelected.GetCurSel();
	if (iIndex == LB_ERR)
		return 0;

	m_listSelected.GetTextBSTR(iIndex, (BSTR&)bstr);
	int i = (int)m_listSelected.GetItemData(iIndex);
	m_listSelected.DeleteString(iIndex);

	iIndex = m_listAvailable.AddString(OLE2T(bstr));
	m_listAvailable.SetItemData(iIndex, (DWORD)i);

	OnSelchangeValue(wNotifyCode, wID, hWndCtl, bHandled);

	return 0;
}

LRESULT CColumPage::OnClickedUp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	CComBSTR bstr;
	int iIndex = m_listSelected.GetCurSel();
	if (iIndex == LB_ERR || iIndex == 0)
		return 0;

	m_listSelected.GetTextBSTR(iIndex, (BSTR&)bstr);
	int i = (int)m_listSelected.GetItemData(iIndex);
	m_listSelected.DeleteString(iIndex);

	iIndex = m_listSelected.InsertString(iIndex - 1, OLE2T(bstr));
	m_listSelected.SetItemData(iIndex, (DWORD)i);
	m_listSelected.SetCurSel(iIndex);

	OnSelchangeValue(wNotifyCode, wID, hWndCtl, bHandled);

	return 0;
}

LRESULT CColumPage::OnClickedDown(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	CComBSTR bstr;
	int iIndex = m_listSelected.GetCurSel();
	if (iIndex == LB_ERR || iIndex == m_listSelected.GetCount() - 1)
		return 0;

	m_listSelected.GetTextBSTR(iIndex, (BSTR&)bstr);
	int i = (int)m_listSelected.GetItemData(iIndex);
	m_listSelected.DeleteString(iIndex);

	iIndex = m_listSelected.InsertString(iIndex + 1, OLE2T(bstr));
	m_listSelected.SetItemData(iIndex, (DWORD)i);
	m_listSelected.SetCurSel(iIndex);

	OnSelchangeValue(wNotifyCode, wID, hWndCtl, bHandled);

	return 0;
}

