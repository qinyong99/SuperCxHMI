// TrendPage.cpp : Implementation of CTrendPage
#include "stdafx.h"
#include "CxChartCtrl.h"
#include "TrendPage.h"

/////////////////////////////////////////////////////////////////////////////
// CTrendPage

LRESULT CTrendPage::OnInitDialog(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CTabPage* pPage = new CComObjectStack<CPenTabPage>;
	pPage->m_strCaption = _T("画笔");
	m_apPages.Add(pPage);

	pPage = new CComObjectStack<CTimeAxisTabPage>;
	pPage->m_strCaption = _T("时间轴");
	m_apPages.Add(pPage);

	pPage = new CComObjectStack<CValueAxisTabPage>;
	pPage->m_strCaption = _T("值轴");
	m_apPages.Add(pPage);

	pPage = new CComObjectStack<CGridTabPage>;
	pPage->m_strCaption = _T("网格");
	m_apPages.Add(pPage);

	pPage = new CComObjectStack<CLegendTabPage>;
	pPage->m_strCaption = _T("标识");
	m_apPages.Add(pPage);

	m_ctrlTab.Attach(GetDlgItem(IDC_TAB));

//	RECT rcTab;
//	m_ctrlTab.GetWindowRect(&rcTab);
//	ScreenToClient(&rcTab);

//	RECT rc;
//	GetClientRect(&rc);
//	rc.top = rcTab.top;
//	m_ctrlTab.MoveWindow(&rc, FALSE);
	RECT rc;
	m_ctrlTab.GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.bottom += 24;
	m_ctrlTab.MoveWindow(&rc, FALSE);
	m_ctrlTab.AdjustRect(FALSE, &rc); 

	for (int i = 0; i < m_apPages.GetSize(); i++)
	{
		pPage = m_apPages[i];
		pPage->Create(m_hWnd);
		pPage->MoveTo(rc.left, rc.top + 20, rc.right - rc.left, rc.bottom - rc.top - 20);
	
	}

	for (i = 0; i < 5; i++)
	{
		pPage = m_apPages[i];
		
		TC_ITEM tci;
		memset(&tci, NULL, sizeof TC_ITEM);
		tci.mask = TCIF_TEXT;
		tci.pszText = (LPTSTR)(LPCTSTR)pPage->m_strCaption; 
		m_ctrlTab.InsertItem(i, &tci);
		pPage->Show(i == 0);
	}

	if (m_nObjects > 0)
	{
		CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[0]);

		m_winTagList.Init(GetDlgItem(IDC_LIST_TAGS));

		VARIANT_BOOL b;
		spTrend->get_RunMode(&b);
		m_winTagList.m_bLocalSetup = (b == VARIANT_FALSE);

		long lCount;
		spTrend->GetSerialsCount(&lCount);
		for (int i = 0; i < lCount; i++)
		{
			CComPtr<ISerial> spSerial;
			spTrend->GetSerial(i, &spSerial);
			CComBSTR bstrSource;
			spSerial->get_Source(&bstrSource);
			m_winTagList.AddTag(bstrSource);
		}

		CComQIPtr<ICxDrawObject, &IID_ICxDrawObject> spDrawObject(m_ppUnk[0]);
		if (spDrawObject != NULL)
		{
			CComPtr<ICxDrawObjectSite> spDrawObjectSite;
			spDrawObject->GetDrawObjectSite(&spDrawObjectSite);
			if (spDrawObjectSite != NULL)
			{
				ExpressionCreatorProc pfn = NULL;
				spDrawObjectSite->GetExpressionCreatorProc(long(&pfn));
				m_winTagList.SetExpressionCreatorProc(pfn);
			}
		}

		m_nCurPage = 0;
	
		{
			BOOL b;
//			m_winTagList.SetCurSel(0);
			OnSelchangeList_tags(0, 0, 0, b);
		}
	}

	if (m_nObjects > 1)
		EnableWindow(FALSE);

	return 0;
}

LRESULT CTrendPage::OnSelchangeTab(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	m_apPages[m_nCurPage]->ApplySerial();

	int nSel = m_ctrlTab.GetCurSel();
	if (m_ctrlTab.GetItemCount() == 3)
		nSel++;

	for (int i = 0; i < m_apPages.GetSize(); i++)
	{
		CTabPage* pPage = m_apPages[i];
		pPage->Show(nSel == i);
	}

	m_nCurPage = nSel;

	return 0;
}

LRESULT CTrendPage::OnSelchangeList_tags(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_nCurPage >= 0)
		m_apPages[m_nCurPage]->ApplySerial();

	IUnknown** ppUnk = (IUnknown**)_alloca(sizeof(IUnknown*) * m_nObjects);

	CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[0]);

	int nPageCount = m_ctrlTab.GetItemCount();
	int iIndex = m_winTagList.GetCurSel();

	if (iIndex != LB_ERR )
	{
		TC_ITEM tci;
		memset(&tci, NULL, sizeof TC_ITEM);

		if (nPageCount == 3)
		{
			TC_ITEM tci;
			memset(&tci, NULL, sizeof TC_ITEM);
			tci.mask = TCIF_TEXT;

			tci.pszText = _T("画笔");
			m_ctrlTab.InsertItem(0, &tci);

			tci.pszText = _T("标识");
			m_ctrlTab.InsertItem(4, &tci);
		}

		for (int i = 0; i < (int)m_nObjects; i++)
		{
			CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[i]);
			CComPtr<ISerial> spSerial;
			spTrend->GetSerial(iIndex, &spSerial);

			ppUnk[i] = spSerial;
		}

		for (i = 0; i < m_apPages.GetSize(); i++)
		{
			CTabPage* pPage = m_apPages[i];
			pPage->SetSerialObjects(m_nObjects, ppUnk);
		}
	}
	else
	{
//		m_ctrlTab.SetFocus();
		int nSel = m_ctrlTab.GetCurSel();

		if (nPageCount > 3)
		{
			m_ctrlTab.DeleteItem(4);
			m_ctrlTab.DeleteItem(0);

			if (nSel == 0 || nSel == 4)
			{
				nSel = 0;
				m_ctrlTab.SetCurSel(nSel);
				nSel++;
				for (int i = 0; i < m_apPages.GetSize(); i++)
				{
					CTabPage* pPage = m_apPages[i];
					pPage->Show(nSel == i);
				}

				m_nCurPage = nSel;
			}
		}

		for (int i = 0; i < (int)m_nObjects; i++)
		{
			CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[i]);
			ppUnk[i] = spTrend;
		}

		for (i = 1; i < 4; i++)
		{
			CTabPage* pPage = m_apPages[i];
			pPage->SetSerialObjects(m_nObjects, ppUnk);
		}
	}

	return 0;
}

LRESULT CTrendPage::OnClickedAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CComBSTR bstr;
	if (m_winTagList.PickTag((BSTR&)bstr))
	{
		CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[0]);

		CComPtr<ISerial> spSerial;
		spTrend->AddSerial(bstr, &spSerial);
		int iIndex = m_winTagList.AddTag(bstr);
	
		m_winTagList.SetCurSel(iIndex);
		BOOL b;
		OnSelchangeList_tags(0, 0, 0, b);

		return 0;
	}

	return -1;
}

LRESULT CTrendPage::OnClickedEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int iIndex = m_winTagList.GetCurSel();
	if (iIndex != LB_ERR )
	{
		m_winTagList.OnEditTag(iIndex);
		m_winTagList.SetCurSel(iIndex);
	}
	
	return 0;
}

LRESULT CTrendPage::OnClickedRemove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int iIndex = m_winTagList.GetCurSel();
	if (iIndex != LB_ERR)
	{
		CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[0]);
		spTrend->RemoveSerial(iIndex);
		m_winTagList.DeleteString(iIndex);

		if (iIndex >= m_winTagList.GetCount())
			iIndex--;
		m_winTagList.SetCurSel(iIndex);
		BOOL b;
		OnSelchangeList_tags(0, 0, 0, b);
	}

	return 0;
}

LRESULT CTrendPage::OnTagItemChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int iIndex = int(wParam);

	CComQIPtr<ICxTrend, &IID_ICxTrend> spTrend(m_ppUnk[0]);
	CComPtr<ISerial> spSerial;
	spTrend->GetSerial(iIndex, &spSerial);

	CComBSTR bstrSource;
	m_winTagList.GetTextBSTR(iIndex, (BSTR&)bstrSource);
	spSerial->put_Source(bstrSource);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// ChartPage

