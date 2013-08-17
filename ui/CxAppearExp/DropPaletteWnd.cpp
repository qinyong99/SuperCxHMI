// DropPaletteWnd.cpp: implementation of the CDropPaletteWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DropPaletteWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDropPaletteWnd::CDropPaletteWnd()
{
	m_pColorControl = NULL;
	pNotifyWnd = NULL;
}

CDropPaletteWnd::~CDropPaletteWnd()
{
	if (m_pColorControl != NULL)
		delete m_pColorControl;
}

LRESULT CDropPaletteWnd::OnCancelMode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_pColorControl->OnCancelMode();	
	return 0;
}
LRESULT CDropPaletteWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
LRESULT CDropPaletteWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetCapture();
	SetFocus();
	OnMouseMove(uMsg, wParam, lParam, bHandled);
	return 0;
}
LRESULT CDropPaletteWnd::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPoint point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	if (GetCapture() == m_hWnd)
		m_pColorControl->OnMouseMove(wParam,point);
	
	return 0;
}

LRESULT CDropPaletteWnd::OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetCapture();
	SetFocus();
	CPoint point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	if (GetCapture() == m_hWnd)
		m_pColorControl->OnLButtonDblClk(wParam,point);
	return 0;
}

LRESULT CDropPaletteWnd::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (GetCapture() == m_hWnd)
		::ReleaseCapture();
	CPoint point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	m_pColorControl->OnLButtonUp(wParam,point);

	return 0;
}
LRESULT CDropPaletteWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPaintDC dcPaint(m_hWnd); // device context for painting
	CRect rectClient;
	GetClientRect(rectClient);
	CDC dc;
	dc.CreateCompatibleDC(dcPaint);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(dcPaint, rectClient.Width(), rectClient.Height());
	CBitmap pBmpOld = (CBitmap)dc.SelectBitmap(bmpMem);
	
	m_pColorControl->OnDraw(dc.m_hDC);
	dcPaint.BitBlt(rectClient.left, rectClient.top,	rectClient.Width(), rectClient.Height(), dc, 0, 0, SRCCOPY);
	
	dc.SelectBitmap(pBmpOld);

	return 0;
}
LRESULT CDropPaletteWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CRect rc;
	GetClientRect(&rc);

	if (m_pColorControl != NULL)
	{
		m_pColorControl->SetRect(&rc);
		m_pColorControl->UpdateAll();
	}

	return 0;
}
LRESULT CDropPaletteWnd::OnSysColorChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	globalData.OnSysColorChange();
	Invalidate ();
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CDropPaletteWnd message handlers
BOOL CDropPaletteWnd::Create(HWND hWndParent, RECT rcPos, DWORD dwStyle, UINT nID, COLORREF crColor, BOOL bPopup)
{
	LPVOID lp = (LPVOID)NULL;
	
	CWindowImpl<CDropPaletteWnd>::Create(hWndParent, rcPos, NULL, dwStyle, 0, nID);
	pNotifyWnd = hWndParent;
	m_pColorControl = new CDropPalette;
	CRect rc;
	GetClientRect(&rc);
	m_pColorControl->Create(this, crColor, rc, bPopup);
    UpdateWindow();
    return TRUE;
}

LONG CDropPaletteWnd::OnSelectDayOK(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostMessage(pNotifyWnd, WM_SELECTCOLOROK, wParam, lParam );
	if(m_pColorControl->IsPopupStyle())
	{
		ShowWindow(SW_HIDE);
	}
	return 0L;
}

LONG CDropPaletteWnd::OnSelectDayCancel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostMessage(pNotifyWnd, WM_SELECTCOLORCANCEL, wParam, lParam );
	if (m_pColorControl->IsPopupStyle())
	{
		ShowWindow(SW_HIDE);
	}
	return 0L;
}

LONG CDropPaletteWnd::OnSelectCustom(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostMessage(pNotifyWnd, WM_SELECTCOLORCUSTOM, wParam, lParam );
	if (m_pColorControl->IsPopupStyle())
	{
		ShowWindow(SW_HIDE);
	}
	return 0L;
}

void CDropPaletteWnd::SetCurColor(COLORREF clrCur)
{
	m_pColorControl->m_crCurColor = clrCur;
}
