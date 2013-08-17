// SQLBuildDlg.cpp: implementation of the CSQLBuildDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxExtObj.h"
#include "SQLBuildDlg.h"
#include "PopupList.h"

#include "../../svr/CxDBProv/CxDBProv_i.c"

static const TCHAR* szTimeFormat[] = 
{
	_T(""),
	_T("GeneralDate"), // 2000-11-30 12:00:00
	_T("LongDate"),	// 2000-11-30 12:00:00.000 
	_T("MediumDate"), // 2000-11-30
	_T("ShortDate"), // 00-11-30
	_T("LongTime"), // 12:00:00.000
	_T("MediumTime"), // 12:00:00
	_T("ShortTime"), // 12:00
};

static const TCHAR* szTimeFormatDesc[] = 
{
	_T("不格式化"),
	_T("常规日期 2000-11-30 12:00:00"), // 2000-11-30 12:00:00
	_T("长日期   2000-11-30 12:00:00.000"),	// 2000-11-30 12:00:00.000 
	_T("中日期   2000-11-30"), // 2000-11-30
	_T("短日期   00-11-30"), // 00-11-30
	_T("长时间   12:00:00.000"), // 12:00:00.000
	_T("中时间   12:00:00"), // 12:00:00
	_T("短时间   12:00"), // 12:00
};

CString GetTimeFormatByDesc(LPCTSTR lpszDesc)
{
	for (int i = 0; i < 8; i++)
	{
		if (_tcsicmp(szTimeFormatDesc[i], lpszDesc) == 0)
			return szTimeFormat[i];
	}

	return _T("");
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSQLBuildDlg::CSQLBuildDlg() :
	m_listField(this, 1),
	m_editInplace(this, 2)
{
	m_bHistoryData = TRUE;
}

LRESULT CSQLBuildDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_font = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);

	LOGFONT lf;
	m_font.GetLogFont(lf);
	lf.lfWeight = FW_BOLD;
	m_fontBold.CreateFontIndirect(&lf);

	m_penGrid.CreatePen(PS_SOLID, 1, RGB(196, 196, 196));

	m_cmbStatement.Attach(GetDlgItem(IDC_STATEMENT));
	m_cmbTab.Attach(GetDlgItem(IDC_TAB));
	m_btnAllField.Attach(GetDlgItem(IDC_ALL_FIELD));
	m_listField.SubclassWindow(GetDlgItem(IDC_FIELD));
	m_btnTime.Attach(GetDlgItem(IDC_TIME));
	m_dtpcDateFrom.Attach(GetDlgItem(IDC_DATETIMEPICKER1));
	m_dtpcTimeFrom.Attach(GetDlgItem(IDC_DATETIMEPICKER2));
	m_dtpcDateTo.Attach(GetDlgItem(IDC_DATETIMEPICKER3));
	m_dtpcTimeTo.Attach(GetDlgItem(IDC_DATETIMEPICKER4));
	m_btnName.Attach(GetDlgItem(IDC_NAME));
	m_editName.Attach(GetDlgItem(IDC_NAME_QUALIFY));
	m_btnType.Attach(GetDlgItem(IDC_TYPE));
	m_btnAnalog.Attach(GetDlgItem(IDC_ANALOG));
	m_btnDigital.Attach(GetDlgItem(IDC_DIGITAL));
	m_btnString.Attach(GetDlgItem(IDC_STRING));
	m_btnSource.Attach(GetDlgItem(IDC_SOURCE));
	m_editSource.Attach(GetDlgItem(IDC_SOURCE_QUALIFY));
	m_btnArea.Attach(GetDlgItem(IDC_AREA));
	m_editArea.Attach(GetDlgItem(IDC_AREA_QUALIFY));
	m_btnActor.Attach(GetDlgItem(IDC_ACTOR));
	m_editActor.Attach(GetDlgItem(IDC_ACTOR_QUALIFY));
	m_btnSeverity.Attach(GetDlgItem(IDC_SEVERITY));
	m_editSeverityForm.Attach(GetDlgItem(IDC_SEVERITY_LOW));
	m_editSeverityTo.Attach(GetDlgItem(IDC_SEVERITY_HIGH));
	m_btnInterval.Attach(GetDlgItem(IDC_INTERVAL));
	m_editInterval.Attach(GetDlgItem(IDC_INTERVAL_QUALIFY));
	m_btnOrder.Attach(GetDlgItem(IDC_ORDER));

	RECT rc = {0, 0, 0, 0};
	m_editInplace.Create(m_listField, &rc, NULL, WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_NOHIDESEL);
	m_editInplace.SetFont((HFONT)::GetStockObject(DEFAULT_GUI_FONT));

 	m_cmbStatement.AddString(_T("Select"));
	m_cmbStatement.SetCurSel(0);

	m_listField.InsertColumn(0, _T(""), LVCFMT_LEFT, 21, -1);
	m_listField.InsertColumn(1, _T("名称"), LVCFMT_LEFT, 90, -1);
	m_listField.InsertColumn(2, _T("别名"), LVCFMT_LEFT, 90, -1);
	m_listField.InsertColumn(3, _T("格式"), LVCFMT_LEFT, 60, -1);

	SYSTEMTIME st;
	GetSystemTime(&st);
	st.wHour = st.wMinute = st.wSecond = st.wMilliseconds = 0;
	m_dtpcDateFrom.SetSystemTime(GDT_VALID, &st);
	m_dtpcTimeFrom.SetSystemTime(GDT_VALID, &st);
	st.wDay += 1;
	m_dtpcDateTo.SetSystemTime(GDT_VALID, &st);
	m_dtpcTimeTo.SetSystemTime(GDT_VALID, &st);

	m_spDataConnection.CoCreateInstance(CLSID_CxDataConnection);
	CComQIPtr<IPersistStreamInit, &IID_IPersistStreamInit> spPersistStreamInit = m_spDataConnection;
	spPersistStreamInit->InitNew();

	m_spDataRecordset.CoCreateInstance(CLSID_CxDataRecordset);
	CComVariant var = (IUnknown *)m_spDataRecordset;
	m_spDataRecordset->put_ActiveConnection(var);
	spPersistStreamInit = m_spDataConnection;
	spPersistStreamInit->InitNew();

	if (wcsncmp(m_bstrConnectionString, L"Provider=CxDBProv.HistoryData.1", 31) != 0)
		m_bHistoryData = FALSE;

	m_btnTime.EnableWindow(m_bHistoryData);
	m_dtpcDateFrom.EnableWindow(m_bHistoryData);
	m_dtpcTimeFrom.EnableWindow(m_bHistoryData);
	m_dtpcDateTo.EnableWindow(m_bHistoryData);
	m_dtpcTimeTo.EnableWindow(m_bHistoryData);
	m_btnName.EnableWindow(!m_bHistoryData);
	m_editName.EnableWindow(!m_bHistoryData);
	m_btnType.EnableWindow(!m_bHistoryData);
	m_btnAnalog.EnableWindow(!m_bHistoryData);
	m_btnDigital.EnableWindow(!m_bHistoryData);
	m_btnString.EnableWindow(!m_bHistoryData);
	m_btnSource.EnableWindow(m_bHistoryData);
	m_editSource.EnableWindow(m_bHistoryData);
	m_btnArea.EnableWindow(m_bHistoryData);
	m_editArea.EnableWindow(m_bHistoryData);
	m_btnActor.EnableWindow(m_bHistoryData);
	m_editActor.EnableWindow(m_bHistoryData);
	m_btnSeverity.EnableWindow(m_bHistoryData);
	m_editSeverityForm.EnableWindow(m_bHistoryData);
	m_editSeverityTo.EnableWindow(m_bHistoryData);
	m_btnInterval.EnableWindow(m_bHistoryData);
	m_editInterval.EnableWindow(m_bHistoryData);
	m_btnOrder.EnableWindow(m_bHistoryData);

	m_btnAnalog.SetCheck(1);
	m_btnDigital.SetCheck(1);
	m_btnString.SetCheck(1);

	LoadTabs();

//	if (!m_bHistoryData && m_cmbTab.GetCount() > 0)
//		m_cmbTab.SetCurSel(0);

	CenterWindow();
	
	return 1;  // Let the system set the focus
}

void CSQLBuildDlg::LoadTabs()
{
	CDataSource source;
	
	HRESULT hr = source.OpenFromInitializationString(m_bstrConnectionString);
	if (hr != S_OK)
		return;

	CComPtr<ICxDataSource> spCxDataSource;
	hr = source.m_spInit->QueryInterface(IID_ICxDataSource, (void**)&spCxDataSource);
	if (spCxDataSource !=NULL)
	{
		BOOL b = TRUE;
		spCxDataSource->SetUseLocalSetup(&b);
	}
	
	CSession session;
	hr = session.Open(source);
	if (hr != S_OK)
		return;
	
	CTables* pTableSet = new CTables;
		
	// (can simply hard code max size)
	char lpszType[64];
	
	// Set up type type to be retrieved
	strcpy(lpszType, "TABLE");
	
	if (pTableSet->Open(session, NULL, NULL, NULL, lpszType) == S_OK)
	{
		while(pTableSet->MoveNext() == S_OK)
		{
			// We do not handle multi-word tables or procedures.  Normally,
			// only Access handles these.
			if (_tcschr(pTableSet->m_szName, _T(' ')) != NULL)
				continue;
			
			m_cmbTab.AddString(pTableSet->m_szName);
		}
	}
	
	delete pTableSet;
	pTableSet = NULL;
}

void CSQLBuildDlg::LoadFields()
{
	m_listField.DeleteAllItems();
	m_arrFieldNames.RemoveAll();

	CDataSource source;
	
	HRESULT hr = source.OpenFromInitializationString(m_bstrConnectionString);
	if (hr != S_OK)
		return;

	CComPtr<ICxDataSource> spCxDataSource;
	hr = source.m_spInit->QueryInterface(IID_ICxDataSource, (void**)&spCxDataSource);
	if (spCxDataSource !=NULL)
	{
		BOOL b = TRUE;
		spCxDataSource->SetUseLocalSetup(&b);
	}
	
	CSession session;
	hr = session.Open(source);
	if (hr != S_OK)
		return;
	
	CColumns* pColumnSet = new CColumns;
	
	// (can simply hard code max size)
	char lpszType[64];
	
	// Set up type type to be retrieved
	CString strTabName;
	m_cmbTab.GetLBText(m_cmbTab.GetCurSel(), strTabName);
	strcpy(lpszType, strTabName);
	
	if (pColumnSet->Open(session, NULL, NULL, NULL, lpszType) == S_OK)
	{
		while(pColumnSet->MoveNext() == S_OK)
		{
			// We do not handle multi-word tables or procedures.  Normally,
			// only Access handles these.
			if (pColumnSet->m_nOrdinalPosition == 0 || _tcschr(pColumnSet->m_szColumnName, _T(' ')) != NULL)
				continue;
			
			CFieldItem* pField = new CFieldItem;
			pField->m_strName = pColumnSet->m_szColumnName;
			pField->m_bTime = pColumnSet->m_nDataType == DBTYPE_FILETIME;
			if (pField->m_bTime)
				pField->m_strTimeFormat = _T("不格式化");
			int nItem = m_listField.InsertItem(m_listField.GetItemCount(), _T(""));
			m_listField.SetItemData(nItem, (DWORD)pField);

			m_arrFieldNames.Add(pField->m_strName);
			m_arrFieldTimeProps.Add(pField->m_bTime);
		}
	}
	
	delete pColumnSet;
	pColumnSet = NULL;

	UpdateFieldButtons();
}

LRESULT CSQLBuildDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CString strText;

	m_cmbStatement.GetLBText(m_cmbStatement.GetCurSel(), strText);
	m_bstrSQL = strText;
	m_bstrSQL += L" ";

	if (m_cmbTab.GetCurSel() == CB_ERR)
	{
		MessageBox(_T("请选择表格！"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	if (m_btnAllField.GetCheck() == 0)
	{
		if (m_listField.GetItemCount() == 0)
		{
			MessageBox(_T("请至少插入一个字段！"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}

		for (int i = 0; i < m_listField.GetItemCount(); i++)
		{
			CFieldItem* pField = (CFieldItem*)m_listField.GetItemData(i);

			if (pField->m_bTime)
			{
				strText = GetTimeFormatByDesc(pField->m_strTimeFormat);
				if (!strText.IsEmpty())
				{
					m_bstrSQL += L"FormatTime(";
					m_bstrSQL += (LPCTSTR)pField->m_strName;
					m_bstrSQL += L", ";
					m_bstrSQL += (LPCTSTR)strText;
					m_bstrSQL += L")";
				}
				else
				{
					m_bstrSQL += (LPCTSTR)pField->m_strName;
				}
			}
			else
			{
				m_bstrSQL += (LPCTSTR)pField->m_strName;
			}
			
			if (!pField->m_strAlias.IsEmpty())
			{
				m_bstrSQL += L" ";
				m_bstrSQL += L"As ";
				m_bstrSQL += (LPCTSTR)pField->m_strAlias;
			}

			if (i < m_listField.GetItemCount() - 1)
				m_bstrSQL += L", ";
		}
	}
	else
	{
		m_bstrSQL += L"*";
	}
	m_bstrSQL += L" ";

	m_bstrSQL += L"From ";
	m_cmbTab.GetLBText(m_cmbTab.GetCurSel(), strText);
	m_bstrSQL += (LPCTSTR)strText;
	
	BOOL bWhere = FALSE;

	if (m_btnTime.GetCheck() != 0)
	{
		SYSTEMTIME st1;
		SYSTEMTIME st2;
		TCHAR szBuf[32];

		if (!bWhere)
		{
			bWhere = TRUE;
			m_bstrSQL += L" Where ";
		}
		else
		{
			m_bstrSQL += L" And ";
		}

		m_dtpcDateFrom.GetSystemTime(&st1);
		m_dtpcTimeFrom.GetSystemTime(&st2);
		st1.wHour = st2.wHour;
		st1.wMinute = st2.wMinute;
		st1.wSecond = st2.wSecond;
		st1.wMilliseconds = st2.wMilliseconds;

		wsprintf(szBuf, _T("%04d-%02d-%02d %02d:%02d:%02d"), 
			st1.wYear, st1.wMonth, st1.wDay,
			st1.wHour, st1.wMinute, st1.wSecond); 

		m_bstrSQL += L"Time Between #";
		m_bstrSQL += szBuf;
		m_bstrSQL += L"# And #";

		m_dtpcDateTo.GetSystemTime(&st1);
		m_dtpcTimeTo.GetSystemTime(&st2);
		st1.wHour = st2.wHour;
		st1.wMinute = st2.wMinute;
		st1.wSecond = st2.wSecond;
		st1.wMilliseconds = st2.wMilliseconds;

		wsprintf(szBuf, _T("%04d-%02d-%02d %02d:%02d:%02d"), 
			st1.wYear, st1.wMonth, st1.wDay,
			st1.wHour, st1.wMinute, st1.wSecond); 

		m_bstrSQL += szBuf;
		m_bstrSQL += L"#";
	}

	if (m_btnType.GetCheck() != 0)
	{
		int nType = 0;
		if (m_btnAnalog.GetCheck() != 0)
			nType |= 1;
		if (m_btnDigital.GetCheck() != 0)
			nType |= 2;
		if (m_btnString.GetCheck() != 0)
			nType |= 4;

		if (nType != 7)
		{
			if (!bWhere)
			{
				bWhere = TRUE;
				m_bstrSQL += L" Where ";
			}
			else
			{
				m_bstrSQL += L" And ";
			}

			TCHAR szBuf[32];
			wsprintf(szBuf, _T("%d"), nType); 
			
			m_bstrSQL += L"Type = ";
			m_bstrSQL += szBuf;
		}
	}

	if (m_btnName.GetCheck() != 0)
	{
		int nLen = m_editName.GetWindowTextLength() + 1;
		m_editName.GetWindowText(strText.GetBuffer(nLen), nLen);
		strText.ReleaseBuffer();
		
		strText.TrimLeft();
		strText.TrimRight();
		if (!strText.IsEmpty())
		{
			if (!bWhere)
			{
				bWhere = TRUE;
				m_bstrSQL += L" Where ";
			}
			else
			{
				m_bstrSQL += L" And ";
			}
			
			m_bstrSQL += L"Name Like \"";
			m_bstrSQL += (LPCTSTR)strText;
			m_bstrSQL += L"\"";
		}
	}

	if (m_btnSource.GetCheck() != 0)
	{
		int nLen = m_editSource.GetWindowTextLength() + 1;
		m_editSource.GetWindowText(strText.GetBuffer(nLen), nLen);
		strText.ReleaseBuffer();
		
		strText.TrimLeft();
		strText.TrimRight();
		if (!strText.IsEmpty())
		{
			if (!bWhere)
			{
				bWhere = TRUE;
				m_bstrSQL += L" Where ";
			}
			else
			{
				m_bstrSQL += L" And ";
			}
			
			m_bstrSQL += L"Source Like \"";
			m_bstrSQL += (LPCTSTR)strText;
			m_bstrSQL += L"\"";
		}
	}

	if (m_btnArea.GetCheck() != 0)
	{
		int nLen = m_editArea.GetWindowTextLength() + 1;
		m_editArea.GetWindowText(strText.GetBuffer(nLen), nLen);
		strText.ReleaseBuffer();
		
		strText.TrimLeft();
		strText.TrimRight();
		if (!strText.IsEmpty())
		{
			if (!bWhere)
			{
				bWhere = TRUE;
				m_bstrSQL += L" Where ";
			}
			else
			{
				m_bstrSQL += L" And ";
			}
			
			m_bstrSQL += L"Area Like \"";
			m_bstrSQL += (LPCTSTR)strText;
			m_bstrSQL += L"\"";
		}
	}

	if (m_btnActor.GetCheck() != 0)
	{
		int nLen = m_editActor.GetWindowTextLength() + 1;
		m_editActor.GetWindowText(strText.GetBuffer(nLen), nLen);
		strText.ReleaseBuffer();
		
		strText.TrimLeft();
		strText.TrimRight();
		if (!strText.IsEmpty())
		{
			if (!bWhere)
			{
				bWhere = TRUE;
				m_bstrSQL += L" Where ";
			}
			else
			{
				m_bstrSQL += L" And ";
			}
			
			m_bstrSQL += L"Actor Like \"";
			m_bstrSQL += (LPCTSTR)strText;
			m_bstrSQL += L"\"";
		}
	}

	if (m_btnSeverity.GetCheck() != 0)
	{
		int nSeverityLow = GetDlgItemInt(IDC_SEVERITY_LOW);
		int nSeverityHigh = GetDlgItemInt(IDC_SEVERITY_HIGH);
		
		if (!bWhere)
		{
			bWhere = TRUE;
			m_bstrSQL += L" Where ";
		}
		else
		{
			m_bstrSQL += L" And ";
		}
		
		strText.Format(_T("Severity Between %d And %d"), nSeverityLow, nSeverityHigh);
		m_bstrSQL += (LPCTSTR)strText;
	}

	if (m_btnInterval.GetCheck() != 0)
	{
		BOOL bTranslated;
		int nInterval = GetDlgItemInt(IDC_INTERVAL_QUALIFY, &bTranslated);
		if (bTranslated)
		{
			strText.Format(_T(" Interval By %d"), nInterval);
			m_bstrSQL += (LPCTSTR)strText;
		}
	}

	if (m_btnOrder.GetCheck() != 0)
	{
		m_bstrSQL += L" Order By Time Desc";
	}

	EndDialog(IDOK);

	return 0;
}

LRESULT CSQLBuildDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
	
	return 0;
}

LRESULT CSQLBuildDlg::OnClickedAllField(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_listField.EnableWindow(m_btnAllField.GetCheck() == 0);
	return 0;
}

LRESULT CSQLBuildDlg::OnSelchangeTab(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	LoadFields();

	int nSel = m_cmbTab.GetCurSel();
	if (nSel != CB_ERR)
	{
		if (m_bHistoryData)
		{
			CString strText;
			m_cmbTab.GetLBText(nSel, strText);
			BOOL b = strText.Left(3).CompareNoCase(_T("evt")) == 0;
			m_btnSource.EnableWindow(b);
			m_editSource.EnableWindow(b);
			BOOL b1 = b && strText.Left(9).CompareNoCase(_T("evtOperation")) != 0;
			m_btnSeverity.EnableWindow(b1);
			m_editSeverityForm.EnableWindow(b1);
			m_editSeverityTo.EnableWindow(b1);
			m_btnInterval.EnableWindow(!b);
			m_editInterval.EnableWindow(!b);
			if (b)
			{
				m_btnInterval.SetCheck(0);
			}
			else
			{
				m_btnSource.SetCheck(0);
				m_btnSeverity.SetCheck(0);
			}

			b = strText.Left(8).CompareNoCase(_T("evtAlarm")) == 0 || 
				strText.Left(6).CompareNoCase(_T("evtAll")) == 0;
			m_btnArea.EnableWindow(b);
			m_editArea.EnableWindow(b);
			if (!b)
				m_btnArea.SetCheck(0);

			b = b || strText.Left(9).CompareNoCase(_T("evtOperation")) == 0;
			m_btnActor.EnableWindow(b);
			m_editActor.EnableWindow(b);
			if (!b)
				m_btnActor.SetCheck(0);
		}
	}
	
	return 0;
}

LRESULT CSQLBuildDlg::OnMeasureItem(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MEASUREITEMSTRUCT* lpMeasureItemStruct = (MEASUREITEMSTRUCT *)lParam;
	lpMeasureItemStruct->itemHeight = 18;
	
	return 1;
}

LRESULT CSQLBuildDlg::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;
	
	DRAWITEMSTRUCT* lpDrawItemStruct = (DRAWITEMSTRUCT *)lParam;
	CDCHandle dc = lpDrawItemStruct->hDC;
	const RECT& rcItem = lpDrawItemStruct->rcItem;
	BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED) != 0;
	int itemID = lpDrawItemStruct->itemID;
	CFieldItem* pField = (CFieldItem*)lpDrawItemStruct->itemData;
	
	switch (lpDrawItemStruct->itemAction)
	{
	case ODA_SELECT:
	case ODA_DRAWENTIRE:
		{		
			HPEN hOldPen = (HPEN)dc.SelectPen(m_penGrid);
					
			dc.FillRect(&rcItem, m_listField.IsWindowEnabled() ? COLOR_WINDOW : COLOR_BTNFACE);
			
			// 画序号列
			CRect rcT = rcItem;
			int nColumn = 0;
			rcT.right = rcT.left + m_listField.GetColumnWidth(nColumn++);
			CRect r = rcT;
			
			dc.DrawEdge(&r, bSelected ? BDR_RAISEDINNER : EDGE_RAISED, BF_RECT|BF_ADJUST);
			dc.FillRect(&r, COLOR_3DFACE);
			
			HFONT hOldFont = dc.SelectFont(bSelected ? m_fontBold : m_font);

			TCHAR szText[16];
			sprintf(szText, "%d", itemID + 1);
			dc.DrawText(szText, -1, &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			
			dc.SelectFont(m_font);

			// 绘制名称
			rcT.left = rcT.right;
			rcT.right += m_listField.GetColumnWidth(nColumn++);
			r = rcT;
			r.InflateRect(-4, 0, -4, 0);
			
			dc.DrawText(pField->m_strName, -1, &r, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_WORD_ELLIPSIS);
			
			// 绘制别名
			rcT.left = rcT.right;
			rcT.right += m_listField.GetColumnWidth(nColumn++);
			r = rcT;
			r.InflateRect(-4, 0, -4, 0);
			
			dc.DrawText(pField->m_strAlias, -1, &r, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_WORD_ELLIPSIS);

			// 绘制别名
			rcT.left = rcT.right;
			rcT.right += m_listField.GetColumnWidth(nColumn++);
			r = rcT;
			r.InflateRect(-4, 0, -4, 0);
			
			dc.DrawText(pField->m_strTimeFormat, -1, &r, DT_LEFT | DT_SINGLELINE | DT_VCENTER/* | DT_WORD_ELLIPSIS*/);
						
			if (bSelected)
			{
 				m_listField.GetSubItemRect(itemID, m_nSelectColumn, LVIR_BOUNDS, &rcT);
				dc.FrameRect(&rcT, (HBRUSH)::GetStockObject(BLACK_BRUSH));
				if (m_nSelectColumn != m_nEditColumn)
				{
					rcT.InflateRect(-1, -1, -1, -1);
					dc.FrameRect(&rcT, (HBRUSH)::GetStockObject(BLACK_BRUSH));
				}
			}
			
			dc.SelectFont(hOldFont);
			dc.SelectPen(hOldPen);
		}
		
		break;
	}	
	
	return 0;
}

LRESULT CSQLBuildDlg::OnDeleteitemField(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pnmh;
	
	LV_ITEM lvi;
	lvi.mask = LVIF_PARAM;
	lvi.iItem = pNMListView->iItem;
	lvi.iSubItem = 0;
	if (m_listField.GetItem(&lvi) == TRUE)
	{
		CFieldItem* pData = (CFieldItem*)(lvi.lParam);
		delete pData;
	}

	return 0;
}

LRESULT CSQLBuildDlg::OnLButtonDownList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT ptMsg = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	LVHITTESTINFO lvh;
	lvh.pt = ptMsg;
	
	if (m_listField.SubItemHitTest(&lvh) != -1)
	{
		if (m_nSelectColumn == lvh.iSubItem)
		{
			if (m_listField.GetSelectedIndex() == lvh.iItem)
			{
				if (OnEditCell(lvh.iItem, lvh.iSubItem))
					return 1;
			}
		}
		else
		{
			m_nSelectColumn = lvh.iSubItem;
			if (m_listField.GetSelectedIndex() == lvh.iItem)
				m_listField.RedrawItems(lvh.iItem, lvh.iItem);
		}
	}
	
	LRESULT lRet = m_listField.DefWindowProc(uMsg, wParam, lParam);
	
	UpdateFieldButtons();
	
	return lRet;
}

LRESULT CSQLBuildDlg::OnGetDlgCodeList(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return DLGC_WANTTAB |  DLGC_WANTARROWS | DLGC_WANTALLKEYS;
}

LRESULT CSQLBuildDlg::OnGetDlgCodeEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)	
{
	return DLGC_WANTTAB |  DLGC_WANTARROWS | DLGC_WANTALLKEYS;
}

LRESULT CSQLBuildDlg::OnKeyDownList(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	int nVirtKey = (int)wParam;
	
	switch (nVirtKey) 
	{
	case VK_TAB:
		PostMessage(WM_KEYDOWN, VK_TAB, 0);
		PostMessage(WM_KEYUP, VK_TAB, 0);
		break;
	case VK_LEFT:
	case VK_RIGHT:
		{
			int nItem = m_listField.GetSelectedIndex();
			if (nItem != -1)
			{
				if (nVirtKey == VK_LEFT)
				{
					if (m_nSelectColumn > 1)
						m_nSelectColumn--;
				}
				else if (nVirtKey == VK_RIGHT)
				{
					if (m_nSelectColumn < m_listField.GetHeader().GetItemCount() - 1)
						m_nSelectColumn++;
				}
			}
			
			m_listField.RedrawItems(nItem, nItem);
		}
		break;
	case VK_RETURN:
		OnEditCell(m_listField.GetSelectedIndex(), m_nSelectColumn);
		break;
	default:
		bHandled = FALSE;
		break;
	}
	
	return 0;
}

LRESULT CSQLBuildDlg::OnVScrollList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_listField.SetFocus();

	bHandled = FALSE;
	return 0;
}

LRESULT CSQLBuildDlg::OnKillFocusEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_nEditItem != -1)
	{
		CComBSTR bstr;
		m_editInplace.GetWindowText((BSTR&)bstr);

		CFieldItem* pItem = (CFieldItem*)m_listField.GetItemData(m_nEditItem);
		switch (m_nEditColumn) 
		{
		case 2:
			pItem->m_strAlias = bstr;
			break;
		default:
			ATLASSERT(FALSE);
			break;
		}
	}
	
	m_nEditItem = -1;
	m_nEditColumn = -1;
	
	m_editInplace.SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	
	return 0;
}

LRESULT CSQLBuildDlg::OnKeyDownEdit (UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	int nVirtKey = (int)wParam;
	if (nVirtKey == VK_TAB)
	{
		bHandled = TRUE;
		PostMessage(WM_KEYDOWN, VK_TAB, 0);
		PostMessage(WM_KEYUP, VK_TAB, 0);		
	}
	if ((nVirtKey == VK_RETURN) || (nVirtKey == VK_TAB))
	{
		m_listField.SetFocus();
	}
	else if (nVirtKey == VK_ESCAPE)
	{	
		m_nEditItem = -1;
		m_listField.SetFocus();
	}	
	
	return 0;
}

BOOL CSQLBuildDlg::OnEditCell(int nItem, int nColumn)
{
	CFieldItem* pItem = (CFieldItem*)m_listField.GetItemData(nItem);

	switch (nColumn) 
	{
	case 2:
		{
			m_editInplace.SetWindowText(pItem->m_strAlias);
			
			CRect rect;
			m_listField.GetSubItemRect(nItem, nColumn, LVIR_BOUNDS, &rect);
			
			m_editInplace.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, 
				rect.bottom - rect.top, SWP_SHOWWINDOW);
			m_editInplace.SetSelAll();
			m_editInplace.SetFocus();
		}
		break;
	case 1:
	case 3:
		{
			if (nColumn == 3 && !pItem->m_bTime)
				return FALSE;

			m_nEditItem = nItem;
			m_nEditColumn = nColumn;
			m_listField.RedrawItems(nItem, nItem);

			CPopupList list;
			list.Create(m_listField, m_font);
			
			if (nColumn == 1)
			{
				for (int i = 0; i < m_arrFieldNames.GetSize(); i++)
					list.AddString(m_arrFieldNames[i]);
			}
			else
			{
				for (int i = 0; i < 7; i++)
					list.AddString(szTimeFormatDesc[i]);
			}
			
			CRect rect;
			m_listField.GetSubItemRect(nItem, nColumn, LVIR_BOUNDS, &rect);
			POINT ptTemp = {rect.left, rect.bottom};	
			m_listField.ClientToScreen(&ptTemp);
			
			int nSelect = list.Show(ptTemp.x, ptTemp.y, 
				nColumn == 1 ? rect.right - rect.left : 200, 
				nColumn == 1 ? pItem->m_strName : pItem->m_strTimeFormat);
			if (nSelect != -1)
			{
				if (nColumn == 1)
				{
					pItem->m_strName = m_arrFieldNames[nSelect];
					pItem->m_bTime = m_arrFieldTimeProps[nSelect];
					if (pItem->m_bTime)
						pItem->m_strTimeFormat = _T("不格式化");
					else
						pItem->m_strTimeFormat.Empty();
				}
				else
				{
					pItem->m_strTimeFormat = szTimeFormatDesc[nSelect];
				}
			}

			m_listField.RedrawItems(nItem, nItem);
			
			m_nEditItem = -1;
			m_nEditColumn = -1;

			return TRUE;
		}
	default:
		break;
	}

	m_nEditItem = nItem;
	m_nEditColumn = nColumn;

	return TRUE;
}

LRESULT CSQLBuildDlg::OnClickedFieldDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nIndex = m_listField.GetSelectedIndex();
	if (nIndex != -1)
		m_listField.DeleteItem(nIndex);
	
	if (m_listField.GetItemCount() > 0)
	{
		if (m_listField.GetItemCount() <= nIndex)
			nIndex--;
		m_listField.SelectItem(nIndex);
	}
	
	UpdateFieldButtons();
		
	if (nIndex < m_listField.GetItemCount())
		m_listField.RedrawItems(nIndex, m_listField.GetItemCount() - 1);
	
	return 0;
}

LRESULT CSQLBuildDlg::OnClickedFieldDown(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nIndex = m_listField.GetSelectedIndex();
	if (nIndex != -1)
	{
		CFieldItem* pOldField = (CFieldItem*)m_listField.GetItemData(nIndex);
		CFieldItem* pField = new CFieldItem(*pOldField);
		m_listField.DeleteItem(nIndex);
		nIndex = m_listField.InsertItem(nIndex + 1, _T(""));
		m_listField.SetItemData(nIndex, (DWORD)pField);
		m_listField.SelectItem(nIndex);
	}

	UpdateFieldButtons();
	
	return 0;
}

LRESULT CSQLBuildDlg::OnClickedFieldInsert(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CFieldItem* pField = new CFieldItem;
	int nIndex = m_listField.InsertItem(m_listField.GetSelectedIndex() != -1 ? m_listField.GetSelectedIndex() : 0, _T(""));
	m_listField.SetItemData(nIndex, (DWORD)pField);
	
	if (m_listField.GetSelectedIndex() == -1)
	{
		m_nSelectColumn = 1;
		m_listField.SelectItem(nIndex);
	}
	
	UpdateFieldButtons();
	
	return 0;
}

LRESULT CSQLBuildDlg::OnClickedFieldUp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nIndex = m_listField.GetSelectedIndex();
	if (nIndex != -1)
	{
		CFieldItem* pOldField = (CFieldItem*)m_listField.GetItemData(nIndex);
		CFieldItem* pField = new CFieldItem(*pOldField);
		m_listField.DeleteItem(nIndex);
		nIndex = m_listField.InsertItem(nIndex - 1, _T(""));
		m_listField.SetItemData(nIndex, (DWORD)pField);
		m_listField.SelectItem(nIndex);
	}
	
	UpdateFieldButtons();
	
	return 0;
}

void CSQLBuildDlg::UpdateFieldButtons()
{
	int nIndex = m_listField.GetSelectedIndex();
	::EnableWindow(GetDlgItem(IDC_FIELD_DELETE), nIndex != -1);
	::EnableWindow(GetDlgItem(IDC_FIELD_UP), nIndex > 0);
	::EnableWindow(GetDlgItem(IDC_FIELD_DOWN), nIndex < m_listField.GetItemCount() - 1);
}

LRESULT CSQLBuildDlg::OnClickedTime(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	BOOL b = m_btnTime.GetCheck() != 0;
	m_dtpcDateFrom.EnableWindow(b);
	m_dtpcTimeFrom.EnableWindow(b);
	m_dtpcDateTo.EnableWindow(b);
	m_dtpcTimeTo.EnableWindow(b);

	return 0;
}

LRESULT CSQLBuildDlg::OnClickedActor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_editActor.EnableWindow(m_btnActor.GetCheck() != 0);
	
	return 0;
}

LRESULT CSQLBuildDlg::OnClickedArea(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_editArea.EnableWindow(m_btnArea.GetCheck() != 0);
	
	return 0;
}

LRESULT CSQLBuildDlg::OnClickedSeverity(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_editSeverityForm.EnableWindow(m_btnSeverity.GetCheck() != 0);
	m_editSeverityTo.EnableWindow(m_btnSeverity.GetCheck() != 0);
	
	return 0;
}

LRESULT CSQLBuildDlg::OnClickedSource(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_editSource.EnableWindow(m_btnSource.GetCheck() != 0);
	
	return 0;
}
