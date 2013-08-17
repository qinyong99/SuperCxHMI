// FontCombo.cpp : Implementation of CFontCombo
#include "stdafx.h"
#include "CxComObj.h"
#include "FontCombo.h"

/////////////////////////////////////////////////////////////////////////////
// CFontCombo

BOOL CFontCombo::Create(HWND hWnd, HIMAGELIST hImages)
{
	m_hImages = hImages;

	CRect rect(0, 0, 0, 0);
	return CWindowImpl<CFontCombo>::Create(hWnd, rect, NULL, WS_POPUP) != 0;		
}

int CFontCombo::Show(int x, int y, int cx, int cyItem)
{
	// 创建列表项
	int nCount = m_arrFontDesc.GetSize();
	for (int i = 0; i < nCount; i++)
		m_list.AddString(m_arrFontDesc[i].m_strName);

	// 计算列表项的高度
	CClientDC dc(m_hWnd);
	TEXTMETRIC tm;
	if (dc.GetTextMetrics(&tm))
		cyItem = tm.tmHeight + cyItem;
	else
		cyItem = 15;

	m_list.SetItemHeight(0, cyItem);
	m_list.SetCurSel(m_list.FindStringExact(-1, m_strSel));
		
	//计算列表筐的高度
	nCount = min(m_list.GetCount(), 8);
	int cy = m_list.GetItemHeight(0) * nCount + GetSystemMetrics(SM_CYBORDER) * 2;
				
	CRect rect(x, y, x + cx, y + cy);
	AdjustPosition(rect);
	GetClientRect(&rect);
	m_list.MoveWindow(&rect);

	ShowWindow(SW_SHOWNA);	

	// Purge the message queue of paints
	MSG msg;
	while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
	{
		if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
			return FALSE;
		DispatchMessage(&msg);
	}
	
	// Set capture to the window which received this message
	m_list.SetCapture();
	ATLASSERT(m_list.m_hWnd == ::GetCapture());
	
	while (!m_bClose)
	{
		MSG msg;
		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			::PostQuitMessage(msg .wParam);
			break;
		}
		
		if (msg.message == WM_KEYDOWN)
			msg.hwnd  = m_list.m_hWnd;
		DispatchMessage(&msg);
	}
	
	::ReleaseCapture();
	
	int nSel = m_bOked ? m_list.GetCurSel() : -1;
	if (nSel != -1)
		m_list.GetText(nSel, m_strSel);
	DestroyWindow();
	
	return nSel;
}

LRESULT CFontCombo::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_hImages = ImageList_LoadImage(_Module.GetResourceInstance(), 
		MAKEINTRESOURCE(IDB_FONTTYPE), 16, 3, CLR_DEFAULT, IMAGE_BITMAP, 0); 

	CRect rcClient;
	GetClientRect(&rcClient);
	HWND hWnd = m_list.Create(m_hWnd, rcClient, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL 
		| LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | LBS_OWNERDRAWFIXED | LBS_SORT|LBS_HASSTRINGS , 0, 1);
	ATLASSERT(hWnd != 0);

	ATLTRACE(_T("HWND pickers: 0x%08x, HWND list: 0x%08x\n"));

	EnumFontFamiliesEx();
		 
	return 0;
}

LRESULT CFontCombo::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DRAWITEMSTRUCT* pdis = (DRAWITEMSTRUCT *)lParam;
	
	CDCHandle dc = pdis->hDC;
	CRect rcItem = pdis->rcItem;
	int nItemID = pdis->itemID;
	CRect rc = rcItem;	
	BOOL bSelected = pdis->itemState & ODS_SELECTED;

	dc.SetBkColor(bSelected ? GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_WINDOW));

	if (m_hImages)
	{			
		rc.right = rc.left + rc.bottom - rc.top;
		dc.ExtTextOut(0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
		
		rc.InflateRect(-1, -1);
		
		int ix = GetSystemMetrics(SM_CXSMICON);
		int iy = GetSystemMetrics(SM_CYSMICON);
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top;
		int dx = min(ix, cx);
		int dy = min(iy, cy);
		int x = max(rc.left, (cx - dx) / 2 + rc.left);
		int y = max(rc.top, (cy - dy) / 2 + rc.top);
		
		ImageList_DrawEx(m_hImages, 1, dc.m_hDC, x, y, dx, dy, CLR_NONE, CLR_DEFAULT, 
			bSelected ? ILD_SELECTED : ILD_NORMAL);
	
		rc = rcItem;
		rc.left += rc.bottom - rc.top;		
	}

	HFONT hOldFont = dc.SelectFont((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
	
	dc.SetTextColor(bSelected ? GetSysColor(COLOR_HIGHLIGHTTEXT) : GetSysColor(COLOR_WINDOWTEXT));
	dc.ExtTextOut(0, 0, ETO_OPAQUE, &rc, 0, 0, 0);
	dc.SetBkMode(TRANSPARENT);
	
	CString strItem;
	m_list.GetText(nItemID, strItem);
	dc.DrawText(strItem, -1, &rc, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	if (hOldFont)
		dc.SelectFont(hOldFont);

	return 0;
}

LRESULT CFontCombo::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int nVirtKey = (int)wParam;
	if (nVirtKey == VK_RETURN || nVirtKey == VK_ESCAPE)
		Close(nVirtKey == VK_RETURN);
	
	bHandled = FALSE;

	return 0;
}

LRESULT CFontCombo::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT ptMsg = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	
	BOOL bOutside;
	int nItem = m_list.ItemFromPoint(ptMsg, bOutside);
	if (!bOutside)
		Close(TRUE);
	
	bHandled = FALSE;
	
	return 0;
}

LRESULT CFontCombo::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT ptMsg = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	
	BOOL bOutside;
	int nItem = m_list.ItemFromPoint(ptMsg, bOutside);
	if (!bOutside)
	{
		m_list.SetCurSel(nItem);
	}
	else
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		rcClient.right += GetSystemMetrics(SM_CXVSCROLL);
		if (rcClient.PtInRect(ptMsg))
		{
			HWND hWndOldCapture = GetCapture();
			ReleaseCapture();
			
			POINT ptScreen = ptMsg;
			ClientToScreen(&ptScreen);
			m_list.SendMessage(WM_NCLBUTTONDOWN, HTVSCROLL,
				MAKELONG(ptScreen.x, ptScreen.y));
			
			if (hWndOldCapture != 0)
				::SetCapture(hWndOldCapture);
		}
		else
		{
			Close(FALSE);
		}
	}
	
	return 0;
}

LRESULT CFontCombo::OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	return 0;
}

void CFontCombo::Close(BOOL bOked)
{
	m_bClose = TRUE;
	m_bOked = bOked;
}	

void CFontCombo::AddFont(ENUMLOGFONT* pelf, DWORD dwType, LPCTSTR lpszScript)
{
	LOGFONT& lf = pelf->elfLogFont;
	if (lf.lfCharSet == MAC_CHARSET) // don't put in MAC fonts, commdlg doesn't either
		return;

	// Don't display vertical font for FE platform
	if ((GetSystemMetrics(SM_DBCSENABLED)) && (lf.lfFaceName[0] == '@'))
		return;

	// don't put in non-printer raster fonts
	for (int i = 0; i < m_arrFontDesc.GetSize(); i++)
	{
		if (m_arrFontDesc[i].Compare(lf.lfFaceName) == 0)
			return;
	}

	CFontDesc fd;
	fd.m_strName = lf.lfFaceName;
	fd.m_strScript = lpszScript;
	fd.m_nCharSet = lf.lfCharSet;
	fd.m_nPitchAndFamily = lf.lfPitchAndFamily;
	fd.m_dwFlags = dwType;
	m_arrFontDesc.Add(fd);
}

BOOL CALLBACK CFontCombo::EnumFamScreenCallBackEx(ENUMLOGFONTEX* pelf,
	NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis)
{
	// don't put in non-printer raster fonts
	if (FontType & RASTER_FONTTYPE)
		return 1;
	
	DWORD dwData = (FontType & TRUETYPE_FONTTYPE) ? TT_FONT : 0;
	((CFontCombo *)pThis)->AddFont((ENUMLOGFONT*)pelf, dwData, (LPCTSTR)pelf->elfScript);
	return 1;
}

void CFontCombo::EnumFontFamiliesEx(BYTE nCharSet)
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfCharSet = nCharSet;
  
	CClientDC dc(m_hWnd);

	::EnumFontFamiliesEx(dc.m_hDC, &lf,
			(FONTENUMPROC)EnumFamScreenCallBackEx, (LPARAM)this, NULL);
}

STDMETHODIMP CFontCombo::PickFont(long hParent, long x, long y, long lWidth, BSTR bstrIn, BSTR* pbstrOut)
{
	USES_CONVERSION;
   
	if (!Create((HWND)hParent, NULL))
		return E_FAIL;

	m_strSel = bstrIn;
	Show(x, y, lWidth, 1);

	if (pbstrOut != NULL)
	{
		if (*pbstrOut != NULL)
			SysFreeString(*pbstrOut);
		*pbstrOut = SysAllocString(T2OLE(m_strSel));
	}
	
	return S_OK;
}

void CFontCombo::AdjustPosition(RECT rect)
{
	// Get the screen rectangle
	CRect rectScreen(CPoint(0, 0), CSize(
		::GetSystemMetrics(SM_CXSCREEN),
		::GetSystemMetrics(SM_CYSCREEN)));
#if (WINVER >= 0x0500)
	HMODULE hUser32 = ::GetModuleHandleA ("USER32.DLL");
	if (hUser32 != NULL)
	{
		typedef HMONITOR (WINAPI *FN_MonitorFromWindow) (HWND hWnd, DWORD dwFlags);
		typedef BOOL (WINAPI *FN_GetMonitorInfo) (HMONITOR hMonitor, LPMONITORINFO lpmi);
		FN_MonitorFromWindow pfnMonitorFromWindow = (FN_MonitorFromWindow)
			::GetProcAddress (hUser32, "MonitorFromWindow");
		FN_GetMonitorInfo pfnGetMonitorInfo = (FN_GetMonitorInfo)
			::GetProcAddress (hUser32, "GetMonitorInfoA");
		if (pfnMonitorFromWindow != NULL && pfnGetMonitorInfo != NULL)
		{
			MONITORINFO mi;
			HMONITOR hMonitor = pfnMonitorFromWindow(m_hWnd, 
				MONITOR_DEFAULTTONEAREST);
			mi .cbSize = sizeof (mi);
			pfnGetMonitorInfo(hMonitor, &mi);
			rectScreen = mi.rcWork;
		}
	}
#endif
	
	// Need to check it'll fit on screen: Too far right?
	if (rect.right > rectScreen.right)
		::OffsetRect(&rect, rectScreen.right -rect.right, 0);
	
	// Too far left?
	if (rect.left < rectScreen.left)
		::OffsetRect(&rect, rectScreen.left - rect.left, 0);
	
	// Bottom falling out of screen?  If so, the move
	// the whole popup above the parents window
	if (rect.bottom > rectScreen.bottom)
	{
		::OffsetRect(&rect, 0,
			- (m_list.GetItemHeight(0) + (rect.bottom - rect.top)));
	}
	
	// Set the window size and position
	MoveWindow(&rect, TRUE);
}