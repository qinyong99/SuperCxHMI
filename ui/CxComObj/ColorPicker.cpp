// ColorPicker.cpp : Implementation of CColorPicker
#include "stdafx.h"
#include "CxComObj.h"
#include "ColorPicker.h"

CColorPicker::_SystemColorEntry CColorPicker::m_SystemColors[] =
{	
	{0x80000000,_T("卷动条")},
	{0x80000001,_T("桌面")},
	{0x80000002,_T("活动标题栏")},
	{0x80000003,_T("非活动标题栏")},
	{0x80000004,_T("菜单条")},
	{0x80000005,_T("窗口背景")},
	{0x80000006,_T("窗口框架")},
	{0x80000007,_T("菜单文本")},
	{0x80000008,_T("窗口文本")},
	{0x80000009,_T("活动标题栏文本")},
	{0x8000000A,_T("活动边框")},
	{0x8000000B,_T("非活动边框")},
	{0x8000000C,_T("应用程序工作区")},
	{0x8000000D,_T("突出显示")},
	{0x8000000E,_T("突出显示文本")},
	{0x8000000F,_T("按钮表面")},
	{0x80000010,_T("按钮阴影")},
	{0x80000011,_T("无效文本")},
	{0x80000012,_T("按钮文本")},
	{0x80000013,_T("非活动标题栏文本")},
	{0x80000014,_T("按钮突出显示")},
	{0x80000015,_T("按钮亮颜色")},
	{0x80000016,_T("按钮暗颜色")},
	{0x80000017,_T("工具提示文本")},
	{0x80000018,_T("工具提示")},
	{0x00000000,0}
};
OLE_COLOR CColorPicker::m_clrPalette[] =
{
0x00FFFFFF,0x00C0C0ff,0x00C0E0ff,0x00C0FFFF,0x00C0FFC0,0x00FFFFC0,0x00FFC0C0,0x00FFC0FF,
0x00E0E0E0,0x008080FF,0x0080C0FF,0x0080FFFF,0x0080FF80,0x00FFFF80,0x00FF8080,0x00FF80FF,
0x00C0C0C0,0x000000FF,0x000080FF,0x0000FFFF,0x0000FF00,0x00FFFF00,0x00FF0000,0x00FF00FF,
0x00808080,0x000000C0,0x000040C0,0x0000C0C0,0x0000C000,0x00C0C000,0x00C00000,0x00C000C0,
0x00404040,0x00000080,0x00004080,0x00008080,0x00008000,0x00808000,0x00800000,0x00800080,
0x00000000,0x00000040,0x00404080,0x00004040,0x00004000,0x00404000,0x00400000,0x00400040,
0x00FFFFFF,0x00FFFFFF,0x00FFFFFF,0x00FFFFFF,0x00FFFFFF,0x00FFFFFF,0x00FFFFFF,0x00FFFFFF,
0x00FFFFFF,0x00FFFFFF,0x00FFFFFF,0x00FFFFFF,0x00FFFFFF,0x00FFFFFF,0x00FFFFFF,0x00FFFFFF
};

HRESULT CColorPicker::FinalConstruct()
{
	return CoCreateFreeThreadedMarshaler(GetControllingUnknown(), &m_pUnkMarshaler.p);
}
void CColorPicker::FinalRelease()
{
	m_pUnkMarshaler.Release();
}

STDMETHODIMP CColorPicker::Pick(long hParent, long X, long Y, OLE_COLOR clr, OLE_COLOR* pclr)
{	
	m_hEvent = CreateEvent(0, 0, 0, 0);
	if (!m_hEvent) 
		return E_FAIL;

	if (!Create((HWND) hParent))
		return E_FAIL;
	
	m_clr = clr;

	BOOL b;
	int idx = SetStartColor(m_clr);		
	TabCtrl_SetCurSel(m_winTab, idx);		
	OnTabSelChange(idx, 0, b);

	int nSizeX = 0;
	HDC hdc = GetDC();
		
	HFONT hFont = (HFONT)::SelectObject(hdc, m_hFont);

	for(int i = 0; m_SystemColors[i].text; i++)
	{
		SIZE sizeText;
		::GetTextExtentPoint32(hdc, m_SystemColors[i].text, lstrlen(m_SystemColors[i].text), &sizeText);
		if(nSizeX < sizeText.cx + sizeText.cy)	nSizeX = sizeText.cx + sizeText.cy;
	}

	::SelectObject(hdc, hFont);
	::ReleaseDC(m_hWnd, hdc);

	nSizeX += GetSystemMetrics(SM_CXVSCROLL);
	nSizeX += GetSystemMetrics(SM_CXBORDER) * 2;
	m_nCellSize = MulDiv(nSizeX, 100, 800);
	
	RECT rc = {0, 0, m_nCellSize * 8, m_nCellSize * 8};
	
	TabCtrl_AdjustRect(m_winTab, TRUE, &rc);
	
	if(rc.left < 0)
	{
		rc.right -= rc.left; rc.left = 0;
	}
	if(rc.top < 0)
	{
		rc.bottom -= rc.top; rc.top = 0;
	}

	rc.bottom += ::GetSystemMetrics(SM_CYEDGE) * 2 + 2;
	rc.right += ::GetSystemMetrics(SM_CXEDGE) * 2 + 2;

	SetWindowPos(m_hWnd, &rc, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	
	GetClientRect(&rc);

	m_winTab.MoveWindow(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
	TabCtrl_AdjustRect(m_winTab, FALSE, &rc); 	

	m_winList.MoveWindow(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
	m_wndPalette.MoveWindow(&rc, FALSE);	


	GetWindowRect(&rc);		
	int height = rc.bottom - rc.top;
	int cyScreen = ::GetSystemMetrics(SM_CYSCREEN);						
	if(Y + height >= cyScreen)
		Y = cyScreen -  height; 		
	int left = max(0, X - (rc.right - rc.left));
	
	SetWindowPos(HWND_TOPMOST, left, Y, 0, 0, SWP_NOSIZE);
	
	ShowWindow(SW_SHOW);
//////////////////////////////////////////////
/*	for (;;)
	{
		MSG msg;
		::GetMessage(&msg, NULL, 0, 0);
		HWND hCaptureWnd = ::GetCapture();
		if (hCaptureWnd && (hCaptureWnd != m_hWnd && ::GetParent(hCaptureWnd) != m_hWnd))
			break;
	}
*/
	AtlWaitWithMessageLoop(m_hEvent);
///////////////////////////////////////////////
	
	HWND hWndGetFocus = GetFocus();
	HWND hWndParent = GetParent();
	DestroyWindow();
	if (hWndGetFocus != hWndParent && ::GetParent(hWndGetFocus) != hWndParent)
		::SetFocus(hWndParent);
	CloseHandle(m_hEvent);
	m_hEvent = 0;	
	*pclr = m_clr;
	return S_OK;
}

LRESULT CColorPicker::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT rc = {0, 0, 0, 0};		

	m_wndPalette.Create(m_hWnd, rc, _T(""), WS_CHILD |  BS_NOTIFY  | BS_OWNERDRAW, 0, 1);
	m_winTab.SubclassWindow(GetDlgItem(IDC_TAB));
	m_winList.SubclassWindow(GetDlgItem(IDC_LIST));

	TC_ITEM tci;
	memset(&tci, 0, sizeof TC_ITEM);
	tci.mask = TCIF_TEXT;

	tci.pszText = _T("调色板");
	TabCtrl_InsertItem(m_winTab, 0, &tci);

	tci.pszText = _T("系统");
	TabCtrl_InsertItem(m_winTab, 1, &tci);
	
	for(int i = 0; m_SystemColors[i].clr; i++)
	{
		int nIdx = m_winList.SendMessage(LB_ADDSTRING, 0, (LPARAM)_T(""));
		m_winList.SendMessage(LB_SETITEMDATA, nIdx, i);
	}
		
	m_hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		
	m_winTab.SendMessage(WM_SETFONT, (WPARAM)m_hFont, true);
	m_winList.SendMessage(WM_SETFONT,(WPARAM)m_hFont, true);
	m_wndPalette.SetFont(m_hFont, true);
		
	return 0;
}
LRESULT CColorPicker::OnTabSelChange(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int nSel = TabCtrl_GetCurSel(m_winTab);
	m_winList.ShowWindow(nSel ? SW_SHOW : SW_HIDE);
	m_wndPalette.ShowWindow(nSel ? SW_HIDE : SW_SHOW);
	
	if (nSel)	m_winList.SetFocus();
	else	m_wndPalette.SetFocus();
	return 0;
}		
LRESULT CColorPicker::OnSelChangeList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	return 0;
}		
LRESULT CColorPicker::OnEraseBkGndPalette(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}		
LRESULT CColorPicker::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(LOWORD(wParam) == WA_INACTIVE)
	{	
//		ShowWindow(SW_HIDE);
		Close();	
	}
	return 0;
}

LRESULT CColorPicker::OnMouseMovePalette(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = {LOWORD(lParam), HIWORD(lParam)};
	
	if (!m_bTrackPaletteColor)
		return 0;
	int nHit = HitTestPalette(pt);
	if(nHit >= 0 && m_nTrackIndex != nHit)
	{
		m_clr = m_clrPalette[nHit];
		m_nTrackIndex = nHit;
		m_wndPalette.Invalidate();
	}
	else
	{
		m_nTrackIndex = nHit;
	}
	return 0;
}	
LRESULT CColorPicker::OnLButtonUpPalette(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = {LOWORD(lParam), HIWORD(lParam)};
	if(::GetCapture() == m_wndPalette.m_hWnd)
		::ReleaseCapture();

	int nHit = HitTestPalette(pt);
	if(m_bTrackPaletteColor && nHit >= 0)
	{
		m_clr = m_clrPalette[nHit];
		Close();
	}

	m_bTrackPaletteColor = FALSE;	
	return 0;
}	
LRESULT CColorPicker::OnLButtonDownPalette(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = {LOWORD(lParam), HIWORD(lParam)};	
	m_bTrackPaletteColor = FALSE;
	m_nTrackIndex = HitTestPalette(pt);	 
	if(m_nTrackIndex < 0) 
		return 0;	
	m_wndPalette.SetCapture();	
	m_bTrackPaletteColor = TRUE;	
	m_clr = m_clrPalette[m_nTrackIndex];
	m_wndPalette.Invalidate();
	return 0;
}	
LRESULT CColorPicker::OnRButtonDownPalette(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = {LOWORD(lParam), HIWORD(lParam)};
	int nIdx = HitTestPalette(pt);
	if(nIdx > 47)
	{
		CHOOSECOLOR cc;
		memset(&cc, 0, sizeof CHOOSECOLOR);
		cc.lStructSize = sizeof CHOOSECOLOR;
		cc.hwndOwner = GetParent();		
		cc.rgbResult = m_clrPalette[nIdx];
		cc.lpCustColors = &m_clrPalette[48];
		cc.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;
		if(ChooseColor(&cc))
		{			
			m_clr = cc.rgbResult;
		}
	}
	return 0;
}	

LRESULT CColorPicker::OnKeyDownList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (wParam == VK_ESCAPE)
	{			
		Close();
	}	
	else if (wParam == VK_RETURN)
	{
		int nSel = m_winList.SendMessage(LB_GETCURSEL);
		if(nSel >= 0) m_clr = m_SystemColors[nSel].clr;
		Close();
	}
	else if (wParam == VK_TAB)
	{
		BOOL b;
		TabCtrl_SetCurSel(m_winTab, 0);
		OnTabSelChange(0, 0, b);
	}
	return 0;
}	
LRESULT CColorPicker::OnLButtonUpList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	int nSel = m_winList.SendMessage(LB_GETCURSEL);
	if(nSel >= 0) m_clr = m_SystemColors[nSel].clr;
	Close();	
	return 0;
}
LRESULT CColorPicker::OnKeyDownButton(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	switch (wParam) {			
		case VK_RETURN: 
			{	
				if ((m_nTrackIndex >=	 0) && (m_nTrackIndex < 64))
					m_clr = m_clrPalette[m_nTrackIndex];
			}
		case VK_ESCAPE:
			Close();
			break;

		case VK_TAB:
			{
				BOOL b;
				TabCtrl_SetCurSel(m_winTab, 1);
				OnTabSelChange(0, 0, b);
				break;
			}
		case VK_LEFT:
			m_nTrackIndex = max(0, m_nTrackIndex - 1);
			m_wndPalette.Invalidate();
			break;

		case VK_RIGHT:
			m_nTrackIndex = min(63, m_nTrackIndex + 1);
			m_wndPalette.Invalidate();
			break;

		case VK_UP:
			m_nTrackIndex = max(0, m_nTrackIndex - 8);
			m_wndPalette.Invalidate();
			break;

		case VK_DOWN:
			m_nTrackIndex = min(63, m_nTrackIndex + 8);
			m_wndPalette.Invalidate();
			break;

		default: 
			break;
	}
	return 0;
}

LRESULT CColorPicker::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam == 1) //pallete
		DrawPalette((DRAWITEMSTRUCT *)lParam);
	else if(wParam == IDC_LIST)
		DrawListItem((DRAWITEMSTRUCT *)lParam);
	return 0;
}	
void CColorPicker::DrawListItem(LPDRAWITEMSTRUCT di)
{
	HDC hdc = di->hDC;
	RECT &rcItem = di->rcItem;

	FillRect(hdc, &di->rcItem, GetSysColorBrush(COLOR_BTNFACE)); 

	RECT rc;
	int i = (int)(di->itemData);
	
	::CopyRect(&rc, &rcItem);	
	rc.right = rc.left + rc.bottom - rc.top;
	::InflateRect(&rc, -1, -1);
	
	COLORREF clr;
	OleTranslateColor(m_SystemColors[i].clr, 0, &clr);
	
	COLORREF clrBack = ::GetBkColor(hdc);
	COLORREF clrText = ::GetTextColor(hdc);

	HBRUSH hBr = ::CreateSolidBrush(clr);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBr);

	FillRect(hdc, &rc, hBr); 
	DrawEdge(hdc, &rc, BDR_SUNKENINNER, BF_RECT);

	::SelectObject(hdc, hOldBrush);
	::DeleteObject(hBr);
	
	::CopyRect(&rc, &rcItem);
	rc.left += rc.bottom - rc.top;
	if(di->itemState & ODS_SELECTED)
	{
		::SetBkColor(hdc, ::GetSysColor(COLOR_HIGHLIGHT));
		::SetTextColor(hdc, ::GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else
	{
		::SetBkColor(hdc, ::GetSysColor(COLOR_BTNFACE));
		::SetTextColor(hdc, ::GetSysColor(COLOR_HIGHLIGHT));
	}
	
	::ExtTextOut(hdc, rc.left, rc.top, ETO_OPAQUE | ETO_CLIPPED, &rc,
			m_SystemColors[i].text, lstrlen(m_SystemColors[i].text), 0);
	
	::SetBkColor(hdc, clrBack);
	::SetTextColor(hdc, clrText);
}	
void CColorPicker::DrawPalette(LPDRAWITEMSTRUCT di)
{
	HDC hdc = ::CreateCompatibleDC(di->hDC);
	HBITMAP hbmp = ::CreateCompatibleBitmap(di->hDC, di->rcItem.right - di->rcItem.left, di->rcItem.bottom - di->rcItem.top);
	HBITMAP hbmp_old = (HBITMAP)::SelectObject(hdc, hbmp);

	FillRect(hdc, &di->rcItem, GetSysColorBrush(COLOR_BTNFACE)); 
//	HPEN hOldPen = (HPEN)::SelectObject(hdc, ::GetStockObject(BLACK_PEN));
	RECT rc;	

	for(int i = 0; i < sizeof(m_clrPalette) / sizeof(m_clrPalette[0]); i++)
	{		
		rc.left = m_nCellSize * ( i % 8 );
		rc.top = m_nCellSize * ( i / 8 );
		rc.right = rc.left + m_nCellSize - 1;
		rc.bottom = rc.top + m_nCellSize - 1;
		
		HBRUSH hBr = ::CreateSolidBrush(m_clrPalette[i]);
		::InflateRect(&rc, -1, -1);
		FillRect(hdc, &rc, hBr); 

		if(m_nTrackIndex == i)
		{
			DrawEdge(hdc, &rc, EDGE_SUNKEN, BF_RECT);
		}
		else
		{
			DrawEdge(hdc, &rc, BDR_SUNKENINNER, BF_RECT);
		}						

		::DeleteObject(hBr);
	}
	::BitBlt(di->hDC, di->rcItem.left, di->rcItem.top, di->rcItem.right - di->rcItem.left,
			di->rcItem.bottom - di->rcItem.top,	hdc, 0, 0, SRCCOPY);
//	::SelectObject(hdc, hOldPen);
	::DeleteObject(::SelectObject(hdc, hbmp_old));
	::DeleteDC(hdc);
}				
int CColorPicker::GetPaletteItemRect(int nIdx, RECT* rc)
{	
	if(nIdx < 0 || nIdx >= sizeof(m_clrPalette) / sizeof(m_clrPalette[0]))
	{
		ATLASSERT(FALSE);
		return -1;
	}
	m_wndPalette.GetClientRect(rc);
	rc->left += m_nCellSize * (nIdx % 8);
	rc->top  += m_nCellSize * ( nIdx / 8);
	rc->right = rc->left + m_nCellSize - 1;
	rc->bottom = rc->top + m_nCellSize - 1;
	return nIdx;
}		
int CColorPicker::HitTestPalette(POINT p)
{	
	RECT rc;
	m_wndPalette.GetClientRect(&rc);
	int x = (p.x - rc.left) / m_nCellSize;
	int y = (p.y - rc.top) / m_nCellSize;
	if ((x < 0) || (x > 7) || (y < 0) || (y > 7))
		return -1;
	return (x + (y * 8));		
}			
int CColorPicker::SetStartColor(OLE_COLOR clr)
{
	for(int i = 0; m_SystemColors[i].clr; i++)
		if(clr == m_SystemColors[i].clr)
		{
			m_winList.SendMessage(LB_SETCURSEL, i);
			return 1;
		}
	
	for(i = 0; i < sizeof(m_clrPalette) / sizeof(m_clrPalette[0]); i++)
		if(clr == m_clrPalette[i])
		{
			m_nTrackIndex = i;			
			return 0;
		}

	for(i = 48; i < sizeof(m_clrPalette) / sizeof(m_clrPalette[0]); i++)
		if(m_clrPalette[i] == 0x00FFFFFF)
		{
			m_clrPalette[i] = clr;
			m_nTrackIndex = i;
			return 0;
		}

	int nLastChanceIdx = sizeof(m_clrPalette) / sizeof(m_clrPalette[0]) - 1;
	m_clrPalette[nLastChanceIdx] = clr;
	m_nTrackIndex = nLastChanceIdx;
	return 0;
}	

STDMETHODIMP CColorPicker::Close()
{
	SetEvent(m_hEvent);

	return S_OK;
}
