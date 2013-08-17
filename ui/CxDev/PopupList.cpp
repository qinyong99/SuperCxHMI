// PopupList.cpp : implementation file
//

#include "stdafx.h"
#include "cxdev.h"
#include "PopupList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopupList

CPopupList::CPopupList()
{
	m_bClose = FALSE;
	m_bOked = FALSE;
}

CPopupList::~CPopupList()
{
}


BEGIN_MESSAGE_MAP(CPopupList, CListBox)
	//{{AFX_MSG_MAP(CPopupList)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupList message handlers

void CPopupList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	BOOL bOutside;
	int nItem = ItemFromPoint(point, bOutside);
	if (!bOutside)
	{
		SetCurSel(nItem);
	}
	else
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		rcClient.right += GetSystemMetrics(SM_CXVSCROLL);
		if (rcClient.PtInRect(point))
		{
			CWnd* pWndOldCapture = GetCapture();
			ReleaseCapture();
			
			POINT ptScreen = point;
			ClientToScreen(&ptScreen);
			SendMessage(WM_NCLBUTTONDOWN, HTVSCROLL,
				MAKELONG(ptScreen.x, ptScreen.y));
			
			if (pWndOldCapture != NULL)
				pWndOldCapture->SetCapture();
		}
		else
		{
			Close(FALSE);
		}
	}
}

void CPopupList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
}

void CPopupList::OnLButtonUp(UINT nFlags, CPoint point) 
{
	BOOL bOutside;
	int nItem = ItemFromPoint(point, bOutside);
	if (!bOutside)
		Close(TRUE);
		
	CListBox::OnLButtonUp(nFlags, point);
}

void CPopupList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_RETURN || nChar == VK_ESCAPE)
		Close(nChar == VK_RETURN);
	
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPopupList::Close(BOOL bOked)
{
	m_bClose = TRUE;
	m_bOked = bOked;
}

BOOL CPopupList::Create(CWnd* pParentWnd, CFont* pFont)
{
	ASSERT(m_hWnd == NULL);

	CRect rect(0, 0, 0, 0);

	CWnd* pWnd = this;
	BOOL b = pWnd->CreateEx(WS_EX_TOPMOST, _T("LISTBOX"), NULL, 
		WS_BORDER | WS_POPUP | WS_VSCROLL, rect, pParentWnd, 0);
	
	SetFont(pFont);
	
	return b;
}

int CPopupList::Show(int x, int y, int cx, int nSel)
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

void CPopupList::AdjustPosition(RECT rect)
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

