// SortPage.cpp : Implementation of CSortPage
#include "stdafx.h"
#include "CxAlmSumCtrl.h"
#include "SortPage.h"
#include "Alarm.h"

/////////////////////////////////////////////////////////////////////////////
// CSortPage

static enumAlarmField _gnSortField[8] =
{
	AlarmField_TimeIn, AlarmField_TagName, AlarmField_Priority,
	AlarmField_Node, AlarmField_Ack, AlarmField_Ack,
	AlarmField_TimeOut, AlarmField_Ack
};

static enumAlarmField _gnSecondSortField[8] =
{
	enumAlarmField(-1), enumAlarmField(-1), enumAlarmField(-1),
	enumAlarmField(-1), AlarmField_TimeIn, AlarmField_Priority,
	enumAlarmField(-1), AlarmField_TimeOut
};

static _TCHAR *_gszSortField[8] =
{
	_T("开始时间"), _T("标签"), _T("优先级"),
	_T("节点"), _T("确认/开始时间"), _T("确认/优先级"),
	_T("结束时间"), _T("确认/结束时间"),
};

LRESULT CSortPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bLoadPropery = FALSE;

	m_cmbSortField.Attach(GetDlgItem(IDC_SORTFIELD));
	for (int i = 0; i < 8; i++)
		m_cmbSortField.AddString(_gszSortField[i]);

	if (m_nObjects > 0)
	{
		CComQIPtr<ICxAlmSum, &IID_ICxAlmSum> spAlmSum(m_ppUnk[0]);

		long l;
		spAlmSum->get_SortOrder((enumSortOrder *)&l);
		CheckRadioButton(IDC_SORTASCENDING, IDC_SORTDESCENDING, IDC_SORTASCENDING + l);

		long lSortColumn, lSecondSortColumn;
		spAlmSum->get_SortColumn(&lSortColumn);
		spAlmSum->get_SecondSortColumn(&lSecondSortColumn);
		for (i = 0; i < 8; i++)
		{
			if (lSortColumn == (long)_gnSortField[i] && 
				lSecondSortColumn == (long)_gnSecondSortField[i])
			{
				m_cmbSortField.SetCurSel(i);
				break;
			}
		}
	}

	return 0;
}

STDMETHODIMP CSortPage::Apply()
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<ICxAlmSum, &IID_ICxAlmSum> spAlmSum(m_ppUnk[i]);

		if (IsDlgButtonChecked(IDC_SORTASCENDING))
			spAlmSum->put_SortOrder(SortOrderAscending);
		else 
			spAlmSum->put_SortOrder(SortOrderDescending);

		int nSel = m_cmbSortField.GetCurSel();
		if (nSel != CB_ERR)
		{
			spAlmSum->put_SortColumn(_gnSortField[nSel]);
			spAlmSum->put_SecondSortColumn(_gnSecondSortField[nSel]);
			spAlmSum->Sort();
		}
	}
	m_bDirty = FALSE;
	
	return S_OK;
}
