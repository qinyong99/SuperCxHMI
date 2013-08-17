// ToolPane.cpp: implementation of the CToolPane class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxChartCtrl.h"
#include "TrendCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolPane::CToolPane()
{
	SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);

	m_dtsScrollTimeSpan = COleDateTimeSpan(0, 0, 10, 0);
	m_fScaleFactor = 1;

	m_pTrend = NULL;
}

CToolPane::~CToolPane()
{

}

#define NUM_COLUMNS 9

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
{
	_T(""), _T("颜色"), _T("变量"), _T("描述"),
		_T("单位"), _T("最大值"), _T("最小值"), _T("平均值"), _T("显示/隐藏")
};

static int _gnColumnFmt[NUM_COLUMNS] =
{
	LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_LEFT, 
		LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_CENTER
};

static int _gnColumnWidth[NUM_COLUMNS] =
{
	40, 40, 220, 120, 40, 60, 60, 60, 70
};

LRESULT CToolPane::OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	CreateButtons();

	m_wndList.Create(this, 2, m_hWnd, &rcDefault, NULL,WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_OWNERDRAWFIXED, WS_EX_CLIENTEDGE, 101);
	SetClient(m_wndList);
	
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for (int i = 0; i < NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx = _gnColumnWidth[i];
		lvc.fmt = _gnColumnFmt[i];
		m_wndList.InsertColumn(i,&lvc);
	}
	
	UpdateZoomRadioEdit();
	UpdateToolBar();

	bHandled = FALSE;
	return 1;
}

LRESULT CToolPane::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
	
	//是客户窗口，由父窗口处理
	int nCtrl = (int)wParam;
	if (m_wndClient.IsWindow() && nCtrl == m_wndClient.GetDlgCtrlID())
	{
		DrawListItem(lpdis);
		return 0;
	}

	CDCHandle dc = lpdis->hDC;
	RECT rect = lpdis->rcItem;

	dc.FillRect(&rect, (HBRUSH)LongToPtr(COLOR_3DFACE + 1));
	InflateRect(&rect, -4, 0);
	dc.DrawEdge(&rect, BDR_SUNKENOUTER, BF_RECT);

    TCHAR szBuf[16];
	sprintf(szBuf, _T("%02d:%02d:%02d"), m_dtsScrollTimeSpan.GetHours(), 
			m_dtsScrollTimeSpan.GetMinutes(), m_dtsScrollTimeSpan.GetSeconds());

	HFONT hFontOld = dc.SelectFont(AtlGetStockFont(DEFAULT_GUI_FONT));
	dc.DrawText(szBuf, -1, &rect, DT_SINGLELINE | DT_CENTER| DT_VCENTER);
	dc.SelectFont(hFontOld);
//	dc.FillRect(&rect, (HBRUSH)LongToPtr(COLOR_WINDOW + 1));
		
	return 0;
}

void DrawCheckbox(CDCHandle dc, CRect rect, BOOL bCheck)
{
	dc.FillSolidRect(&rect, GetSysColor(COLOR_WINDOW));
	dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);

	if (bCheck)
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(0,0,0));
		HPEN hOldPen = dc.SelectPen(pen);

		// draw the checkmark
		int x = rect.left + 9;
		int y = rect.top + 3;
		int i;
		for (i = 0; i < 4; i++)
		{
			dc.MoveTo(x, y);
			dc.LineTo(x, y+3);
			x--;
			y++;
		}
		for (i = 0; i < 3; i++)
		{
			dc.MoveTo(x, y);
			dc.LineTo(x, y+3);
			x--;
			y--;
		}
		
		dc.SelectPen(hOldPen);
	}
}

void CToolPane::DrawListItem(LPDRAWITEMSTRUCT lpdis)
{
	CDCHandle dc = lpdis->hDC;
	BOOL bSelected = lpdis->itemState & ODS_SELECTED;

	switch (lpdis->itemAction)
	{
	case ODA_DRAWENTIRE:
	case ODA_SELECT:
		{
			USES_CONVERSION;
			
			int iIndex = lpdis->itemID;
			CRect rect = lpdis->rcItem;
			
			CSerial& serial = *m_pTrend->m_apSerials[iIndex];
			
			dc.FillRect(&lpdis->rcItem, (HBRUSH)LongToPtr(bSelected ? (COLOR_HIGHLIGHT + 1) : (COLOR_WINDOW + 1)));
		//	dc.FillRect(&rect, (HBRUSH)LongToPtr(COLOR_WINDOW + 1));
			dc.SetTextColor(::GetSysColor(COLOR_BTNTEXT));
			dc.SetBkMode(TRANSPARENT);
			HFONT hFontOld = dc.SelectFont(AtlGetStockFont(DEFAULT_GUI_FONT));
			
			//绘制序号
			TCHAR szBuf[32];
			sprintf(szBuf, _T("%d"), iIndex);
			int nWidth = m_wndList.GetColumnWidth(0);
			rect.right = rect.left + nWidth;
			dc.DrawEdge(&rect, BDR_RAISEDOUTER, BF_RECT | BF_ADJUST);
			dc.FillRect(&rect, (HBRUSH)LongToPtr(COLOR_BTNFACE + 1));
			dc.DrawText(szBuf, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS);
			
			dc.SetTextColor(bSelected ? ::GetSysColor(COLOR_HIGHLIGHTTEXT) : ::GetSysColor(COLOR_BTNTEXT));
			
			//绘制颜色块
			dc.SelectStockPen(BLACK_PEN);
			HBRUSH hColorBrush = ::CreateSolidBrush(serial.m_clrLineColor);	
			HBRUSH hOldBrush = dc.SelectBrush(hColorBrush);
			m_wndList.GetSubItemRect(iIndex, 1, LVIR_BOUNDS, &rect);
			rect.InflateRect(0, 0, -1, -1);
			dc.Rectangle(rect.left, rect.top, rect.right, rect.bottom);
			
			m_wndList.GetSubItemRect(iIndex, 2, LVIR_BOUNDS, &rect);
			rect.left += 2;
			dc.DrawText(OLE2T(serial.m_bstrSource), -1, &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS);
			
			m_wndList.GetSubItemRect(iIndex, 3, LVIR_BOUNDS, &rect);
			rect.left += 2;
			dc.DrawText(OLE2T(serial.m_bstrSourceDesc), -1, &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS);
			
			m_wndList.GetSubItemRect(iIndex, 4, LVIR_BOUNDS, &rect);
			//rect.left += 2;
			dc.DrawText(OLE2T(serial.m_bstrSourceUnit), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS);
			
			CString psz = "%.2f";//serial.m_pValueAxis->m_bstrDisplayFormat;

			if (serial.m_dMaxVal != -DBL_MAX)
			{
				m_wndList.GetSubItemRect(iIndex, 5, LVIR_BOUNDS, &rect);
				rect.right -= 2;
				memset(szBuf, 0, 32);
				sprintf(szBuf, psz, serial.m_dMaxVal);
				dc.DrawText(szBuf, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS);
			}

			if (serial.m_dMinVal != DBL_MAX)
			{
				m_wndList.GetSubItemRect(iIndex, 6, LVIR_BOUNDS, &rect);
				rect.right -= 2;
				memset(szBuf, 0, 32);
				sprintf(szBuf, psz, serial.m_dMinVal);
				dc.DrawText(szBuf, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS);
			}
			
			if (serial.m_values.GetSize() > 0)
			{
				m_wndList.GetSubItemRect(iIndex, 7, LVIR_BOUNDS, &rect);
				rect.right -= 2;
				memset(szBuf, 0, 32);
				sprintf(szBuf, psz, serial.m_dValTotal / serial.m_values.GetSize());
				dc.DrawText(szBuf, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS);
			}

			m_wndList.GetSubItemRect(iIndex, 8, LVIR_BOUNDS, &rect);
			CPoint pt = rect.CenterPoint();
			rect.left = pt.x - 6;
			rect.top = pt.y - 6;
			rect.right = pt.x + 6;
			rect.bottom = pt.y + 6;
			DrawCheckbox(dc, rect, serial.m_bShow);
					
			//画网格
			HPEN hGridPen = CreatePen(PS_SOLID, 1, RGB(196, 196, 196)); 
			HPEN hOldPen = dc.SelectPen(hGridPen);
			for (int i = 3; i < NUM_COLUMNS; i++)
			{
				m_wndList.GetSubItemRect(iIndex, i, LVIR_BOUNDS, &rect);
				dc.MoveTo(rect.left - 1, rect.top, 0);
				dc.LineTo(rect.left - 1, rect.bottom - 1);
			}
			
			//底边和右边网格
			dc.SelectPen(hGridPen);
			dc.MoveTo(lpdis->rcItem.left, lpdis->rcItem.bottom - 1, 0);
			dc.LineTo(lpdis->rcItem.right - 1, lpdis->rcItem.bottom - 1);
			dc.LineTo(lpdis->rcItem.right - 1, lpdis->rcItem.top);
			
			dc.SelectBrush(hOldBrush);
			dc.SelectPen(hOldPen);
			::DeleteObject(hColorBrush);
			::DeleteObject(hGridPen);
			dc.SelectFont(hFontOld);
		}
	}	
}

LRESULT CToolPane::OnMeasureItem(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (101 != wParam)
		return 0;
	
	MEASUREITEMSTRUCT *pmis = (MEASUREITEMSTRUCT *)lParam;
	pmis->itemHeight = 16;
	
	return 1;
}

void CToolPane::CalcSize()
{
	m_cxyHeader = 30;
}

void CToolPane::UpdateLayout(int cxWidth, int cyHeight)
{
	ATLASSERT(::IsWindow(m_hWnd));
	RECT rect;

	::SetRect(&rect, 0, 0, cxWidth, m_cxyHeader);
	if (m_wndToolBar.m_hWnd != NULL)
		m_wndToolBar.SetWindowPos(NULL, m_cxyBorder + m_cxyBtnOffset, m_cxyBorder + m_cxyBtnOffset, 
				cxWidth - m_cxyBorder - m_cxyBtnOffset, 20, SWP_NOZORDER | SWP_NOACTIVATE);
	
//	TBBUTTONINFO bi;
//	memset(&bi, 0, sizeof(bi));
//	bi.cbSize = sizeof(TBBUTTONINFO);
//	bi.dwMask = TBIF_SIZE;
//	bi.cx = max((cxWidth - 530) * 3 / 5 + 120, 120);
//	m_wndToolBar.SetButtonInfo(ID_SCROLL_BACK + 1, &bi);
//
//	m_wndToolBar.GetItemRect(1, &rect);
//    m_btnTimeRange.SetWindowPos(NULL, &rect, SWP_NOZORDER);
//
//	bi.cx = max((cxWidth - 530) * 2 / 5 + 80, 80);
//	m_wndToolBar.SetButtonInfo(ID_SCROLL_BACK + 4, &bi);
//
//	m_wndToolBar.GetItemRect(4, &rect);
//	rect.left += 2;
//	rect.right -= 12;
//    m_editZoomRadio.SetWindowPos(NULL, &rect, SWP_NOZORDER);


	if (m_wndClient.m_hWnd != NULL)
	{
		if (m_pTrend->m_bShowToolPane == VARIANT_TRUE)
			m_wndClient.SetWindowPos(NULL, 0, m_cxyHeader, cxWidth, cyHeight - m_cxyHeader, SWP_NOZORDER);
		else
			m_wndClient.SetWindowPos(NULL, 0, 0, cxWidth, cyHeight, SWP_NOZORDER);
	}
	else
	{
		rect.bottom = cyHeight;
	}
	
	InvalidateRect(&rect);
}

void CToolPane::UpdateLayout()
{
	RECT rcClient;
	GetClientRect(&rcClient);
	UpdateLayout(rcClient.right, rcClient.bottom);
}

void CToolPane::CreateButtons()
{
//	ATLASSERT(m_wndToolBar.m_hWnd == NULL);
	
	m_wndToolBar.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | CCS_NODIVIDER | CCS_NOPARENTALIGN | CCS_NOMOVEY | TBSTYLE_TOOLTIPS | TBSTYLE_LIST, 0, ID_TOOL);
	ATLASSERT(m_wndToolBar.IsWindow());
	m_wndToolBar.SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	
	if (m_wndToolBar.m_hWnd != NULL)
	{
		int nButtons = 15;
		TBBUTTON* pButton = new TBBUTTON[nButtons];

		m_wndToolBar.SetButtonStructSize();

	//	m_wndToolBar.LoadImages(IDB_TOOL);
		m_wndToolBar.SetBitmapSize(16, 15);
		m_wndToolBar.SetButtonSize(28, 22);
		m_wndToolBar.AddBitmap(nButtons + 1, IDR_TOOL);

		for (int i = 0; i < nButtons; i++)
		{
			pButton[i].fsState   = TBSTATE_ENABLED;
			pButton[i].dwData    = 0;
			pButton[i].iString   = 0;
			pButton[i].iBitmap   = i;
			pButton[i].fsStyle = TBSTYLE_BUTTON;
			pButton[i].idCommand = i + ID_SCROLL_BACK;
		}

		m_wndToolBar.AddButtons(nButtons, pButton);
		delete [] pButton;
	}

	// 时间段使用按钮
	TBBUTTONINFO bi;
	memset(&bi, 0, sizeof(bi));
	bi.cbSize = sizeof(TBBUTTONINFO);
	bi.dwMask = TBIF_SIZE | TBIF_STYLE;
	bi.cx = 135;
	bi.fsStyle = TBSTYLE_SEP;
	m_wndToolBar.SetButtonInfo(ID_SCROLL_BACK + 1, &bi);
	m_wndToolBar.SetButtonInfo(ID_SCROLL_BACK + 3, &bi);

	RECT rect;
	m_wndToolBar.GetItemRect(1, &rect);
	m_dtpcFromTimePicker.Create(m_wndToolBar, &rect, NULL, WS_CHILD | WS_VISIBLE, 0, ID_PICKER_FROM_TIME);
//	DWORD dw = GetLastError();
	m_dtpcFromTimePicker.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	m_wndToolBar.GetItemRect(3, &rect);
	m_dtpcToTimePicker.Create(m_wndToolBar, &rect, NULL, WS_CHILD | WS_VISIBLE, 0, ID_PICKER_TO_TIME);
	m_dtpcToTimePicker.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));

	COleDateTime dtNow = COleDateTime::GetCurrentTime();
	dtNow -= m_dtsScrollTimeSpan;
	SYSTEMTIME stForm;
	dtNow.GetAsSystemTime(stForm);
	m_dtpcFromTimePicker.SetSystemTime(GDT_VALID, &stForm);

	// 显示比例
	bi.cx = 70;
	bi.fsStyle = TBSTYLE_BUTTON | TBSTYLE_DROPDOWN;
	
	m_wndToolBar.SetButtonInfo(ID_SCROLL_BACK + 6, &bi);

	m_wndToolBar.GetItemRect(6, &rect);
	rect.left += 2;
	rect.right -= 12;
	m_editZoomRadio.Create(this, 1, m_wndToolBar, &rect, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS 
			| ES_NOHIDESEL | ES_CENTER | WS_VISIBLE, WS_EX_CLIENTEDGE, ID_EDIT_ZOOM_RADIO);
	m_editZoomRadio.SetFont(AtlGetStockFont(DEFAULT_GUI_FONT));
}

void CToolPane::DrawPaneTitle(CDCHandle dc)
{
	RECT rect;
	GetClientRect(&rect);
	
	rect.bottom = rect.top + m_cxyHeader;
//	InflateRect(&rect, -10, -6);
	dc.DrawEdge(&rect, EDGE_RAISED, BF_TOP | /*BF_RIGHT |*/ BF_ADJUST);
	dc.FillRect(&rect, (HBRUSH)LongToPtr(COLOR_3DFACE + 1));
}

LRESULT CToolPane::OnScrollBack(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CComVariant var = (double)m_dtsScrollTimeSpan;
	m_pTrend->ScrollBack(var);
	
	return 0;
}

LRESULT CToolPane::OnScrollForward(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CComVariant var = (double)m_dtsScrollTimeSpan;
	m_pTrend->ScrollForward(var);
	
	return 0;
}

LRESULT CToolPane::OnScrollForwardMost(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pTrend->ScrollForwardMost();

	return 0;
}

LRESULT CToolPane::OnZoomRadio(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	float fFactor;
	switch (wID)
	{
	case ID_ZOOM_1600:
		fFactor = 0.0625f;
		break;
	case ID_ZOOM_800:
		fFactor = 0.125f;
		break;
	case ID_ZOOM_400:
		fFactor = 0.25;
		break;
	case ID_ZOOM_200:
		fFactor = 0.5;
		break;
	case ID_ZOOM_150:
		fFactor = 0.666667f;
		break;
	case ID_ZOOM_100:
		fFactor = 1.0f;
		break;
	case ID_ZOOM_50:
		fFactor = 2.0f;
		break;
	case ID_ZOOM_25:
		fFactor = 4.0f;
		break;
	case ID_ZOOM_12_5:
		fFactor = 8.0f;
		break;
	default:
		fFactor = 1.0f;
	}
	
	Zoom(fFactor);
	
	return 0;
}

void CToolPane::Zoom(float fFactor)
{
	if (m_pTrend == NULL)
		return;

	fFactor /= m_fScaleFactor;
	
	CRect rect = m_pTrend->m_rcSerial;
	POINTF point;
	point.x = (float)(rect.left + rect.right) / 2;
	point.y = (float)(rect.top + rect.bottom) / 2;
	m_pTrend->Zoom(point.y - (float)rect.Height() * fFactor / 2, point.y + (float)rect.Height() * fFactor / 2,
		point.x + (float)rect.Width() * fFactor / 2, point.x - (float)rect.Width() * fFactor / 2);
	
}

LRESULT CToolPane::OnZoomIn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pTrend->ZoomIn();
	
	return 0;
}

LRESULT CToolPane::OnZoomOut(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pTrend->ZoomOut();
	return 0;
}

LRESULT CToolPane::OnCrossCursor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	VARIANT_BOOL b;
	m_pTrend->get_ShowCursor(&b);
	b = (b != VARIANT_FALSE ? VARIANT_FALSE : VARIANT_TRUE);
	m_pTrend->put_ShowCursor(b);

	return 0;
}

LRESULT CToolPane::OnPrint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pTrend->OnPrint();
	
	return 0;
}

LRESULT CToolPane::OnEditSerials(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pTrend->EditSerials();

	return 0;
}

LRESULT CToolPane::OnModeRealtime(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pTrend->put_TrendMode(TrendModeRealTime);

	return 0;
}

LRESULT CToolPane::OnModeHistory(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pTrend->put_TrendMode(TrendModeHistory);
	
	return 0;
}

LRESULT CToolPane::OnEditProperties(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pTrend->EditProperties();

	return 0;
}

LRESULT CToolPane::DoNothing(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	return 0;
}

LRESULT CToolPane::OnDropDown(WPARAM idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	// this function handles the dropdown menus from the toolbar
	NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pnmh;
	CRect rect;
	
	// translate the current toolbar item rectangle into screen coordinates
	// so that we'll know where to pop up the menu
	m_wndToolBar.GetRect(pNMToolBar->iItem, &rect);
	rect.top = rect.bottom;
	::ClientToScreen(pNMToolBar->hdr.hwndFrom, &rect.TopLeft());

	CMenu menu;
	CMenuHandle menuPopup;
	
	menu.LoadMenu(IDR_ZOOM_RADIO);
	menuPopup = menu.GetSubMenu(0);
	menuPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, m_hWnd);

	return 0;
}

LRESULT CToolPane::OnGetDlgCodeEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)	
{
	return DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTALLKEYS;
}

static BOOL _AtlSimpleFloatParse(LPCTSTR lpszText, double& d)
{
	ATLASSERT(lpszText != NULL);
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;
	
	TCHAR chFirst = lpszText[0];
	d = _tcstod(lpszText, (LPTSTR*)&lpszText);
	if (d == 0.0 && chFirst != '0')
		return FALSE;   // could not convert
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;
	
	if (*lpszText != '\0')
		return FALSE;   // not terminated properly
	
	return TRUE;
}

LRESULT CToolPane::OnKeyDownEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	int nVirtKey = (int)wParam;

	if (nVirtKey == VK_TAB)
	{
		bHandled = TRUE;
		PostMessage(WM_KEYDOWN, VK_TAB, 0);
		PostMessage(WM_KEYUP, VK_TAB, 0);		
	}

	if (nVirtKey == VK_RETURN || nVirtKey == VK_TAB)
	{
		TCHAR szBuf[32];
		m_wndToolBar.GetDlgItemText(ID_EDIT_ZOOM_RADIO, szBuf, sizeof(szBuf) / sizeof(TCHAR));
		double d = 0;
		if (_AtlSimpleFloatParse(szBuf, d))
			Zoom(100 / (float)d);

		m_pTrend->SetFocus();
	}
	else if (nVirtKey == VK_ESCAPE)
	{	
		m_pTrend->SetFocus();
	}	
	
	return 0;
}

LRESULT CToolPane::OnKillFocusEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	UpdateZoomRadioEdit();
		
	return 0;
}

void CToolPane::UpdateZoomRadioEdit()
{
	TCHAR szBuf[128];
	sprintf(szBuf, _T("%.1f%%"),  1 / m_fScaleFactor * 100);
	m_editZoomRadio.SetWindowText(szBuf);
}

void CToolPane::UpdateToolBar()
{
	if (m_wndToolBar.IsWindow())
	{
//		TBBUTTONINFO bi;
//		memset(&bi, 0, sizeof(bi));
//		bi.cbSize = sizeof(TBBUTTONINFO);
//		bi.dwMask = TBIF_IMAGE;
//		bi.iImage = m_pTrend->m_enumChartMode == ChartModeRealTime ? 14 : 12;
//		m_wndToolBar.SetButtonInfo(ID_CHART_MODE, &bi);
		
		m_wndToolBar.CheckButton(ID_MODE_REAL, m_pTrend->m_enumTrendMode == TrendModeRealTime);
		m_wndToolBar.CheckButton(ID_MODE_HIST, m_pTrend->m_enumTrendMode != TrendModeRealTime);
		m_wndToolBar.CheckButton(ID_CROSS_CURSOR, m_pTrend->m_bShowCursor == VARIANT_TRUE);
	}
}

LRESULT CToolPane::OnTimeUpdate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SYSTEMTIME stFrom;
	SYSTEMTIME stTo;
	
	m_dtpcFromTimePicker.GetSystemTime(&stFrom);
	m_dtpcToTimePicker.GetSystemTime(&stTo);

	COleDateTime dtForm(stFrom);
	COleDateTime dtTo(stTo);

	m_pTrend->put_ScrollDirection(dtForm > dtTo ? ScrollDirectionRight : ScrollDirectionLeft);
	
	CComVariant varForm;
	varForm.vt = VT_DATE;
	varForm.date = (DATE)dtForm;

	CComVariant varTo;
	varTo.vt = VT_DATE;
	varTo.date = (DATE)dtTo;

	m_pTrend->ScrollTo(varForm, varTo);

	return 0;
}

LRESULT CToolPane::OnLButtonDownList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT ptMsg = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

	LVHITTESTINFO lvh;
	lvh.pt = ptMsg;
	if (m_wndList.HitTest(&lvh) != -1)
	{
		int iIndex = lvh.iItem;
		CRect rect;
		m_wndList.GetSubItemRect(iIndex, 8, LVIR_BOUNDS, &rect);
		CPoint pt = rect.CenterPoint();
		rect.left = pt.x - 6;
		rect.top = pt.y - 6;
		rect.right = pt.x + 6;
		rect.bottom = pt.y + 6;

		if (rect.PtInRect(ptMsg))
		{
			CSerial& serial = *m_pTrend->m_apSerials[iIndex];
			serial.put_Show(serial.m_bShow != VARIANT_FALSE ? VARIANT_FALSE : VARIANT_TRUE);
		}
	}
	
	LRESULT lRet = m_wndList.DefWindowProc(uMsg, wParam, lParam);
	
	return lRet;
}

LRESULT CToolPane::OnToolbarNeedText(WPARAM, LPNMHDR pnmh, BOOL&)
{
	LPTOOLTIPTEXT lpToolTipText = (LPTOOLTIPTEXT)pnmh;
	static TCHAR szBuf[128];

	// Get the string with the same ID as the button from the resource
	LoadString(_Module.GetResourceInstance(),
		lpToolTipText->hdr.idFrom,
		szBuf, sizeof(szBuf) * sizeof(TCHAR));
	lpToolTipText->lpszText = szBuf;
	
	return 0;
}
