// PopupList.h: interface for the CPopupList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _POPUPLIST_H__
#define _POPUPLIST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPopupList : public CWindowImpl<CPopupList, CListBox>
{
public:
	BOOL m_bClose;
	BOOL m_bOked;

	CPopupList() 
	{
		m_bClose = FALSE;
		m_bOked = FALSE;
	}
	
	BOOL Create(HWND hWndParent, HFONT hFont)
	{
		ATLASSERT(m_hWnd == NULL);
		CRect rectWindow(0, 0, 0, 0);
		HWND hWnd = CWindowImpl<CPopupList, CListBox>::Create(hWndParent, rectWindow, NULL, WS_BORDER | WS_POPUP | WS_VSCROLL, WS_EX_TOPMOST);
		ATLASSERT(m_hWnd == hWnd);

		SetFont(hFont);

		return hWnd != NULL;
	}

	int Show(int x, int y, int cx, LPCTSTR lpszSelString)
	{
		int nSel = FindStringExact(-1, lpszSelString);
		return Show(x, y, cx, nSel);
	}

	int Show(int x, int y, int cx, int nSel)
	{		
		SetCurSel(nSel);
		SetTopIndex(nSel);
		
		int nCount = min(GetCount(), 8);
		int cy = GetItemHeight(0) * nCount + GetSystemMetrics(SM_CYBORDER) * 2;
				
		CRect rect(x, y, x + cx, y + cy);
		AdjustPosition(rect);
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
		SetCapture();
		ATLASSERT(m_hWnd == ::GetCapture());
		
		while (!m_bClose)
		{
			MSG msg;
			if (!::GetMessage(&msg, NULL, 0, 0))
			{
				::PostQuitMessage(msg .wParam);
				break;
			}

			if (msg.message == WM_KEYDOWN)
				msg.hwnd  = m_hWnd;
			DispatchMessage(&msg);
		}

		::ReleaseCapture();
	
		nSel = m_bOked ? GetCurSel() : -1;
		DestroyWindow();
		
		return nSel;
	}

	BEGIN_MSG_MAP(CPopupList)
 		MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
 		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)	
	END_MSG_MAP()

	LRESULT OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return MA_NOACTIVATE;
	}

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int nVirtKey = (int)wParam;
		if (nVirtKey == VK_RETURN || nVirtKey == VK_ESCAPE)
			Close(nVirtKey == VK_RETURN);

		bHandled = FALSE;

		return 0;
	}
	
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{	
		POINT ptMsg = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

		BOOL bOutside;
		int nItem = ItemFromPoint(ptMsg, bOutside);
		if (!bOutside)
		{
			SetCurSel(nItem);
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
				SendMessage(WM_NCLBUTTONDOWN, HTVSCROLL,
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
	LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{	
		return 0;
	}	

	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{	
		POINT ptMsg = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		
		BOOL bOutside;
		int nItem = ItemFromPoint(ptMsg, bOutside);
		if (!bOutside)
  			Close(TRUE);

		bHandled = FALSE;
 		
		return 0;
	}	

	void Close(BOOL bOked)
	{
 		m_bClose = TRUE;
		m_bOked = bOked;
	}

	void AdjustPosition(RECT rect)
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
				- (GetItemHeight(0) + (rect.bottom - rect.top)));
		}

		// Set the window size and position
		MoveWindow(&rect, TRUE);
	}

};

#endif // _POPUPLIST_H__
