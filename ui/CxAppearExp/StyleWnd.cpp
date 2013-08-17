// StyleWnd.cpp: implementation of the CStyleWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StyleWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStyleWnd::CStyleWnd()
{

}

CStyleWnd::~CStyleWnd()
{

}

LRESULT CStyleWnd::OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 1;   // no background needed
}

LRESULT CStyleWnd::OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	DRAWSTYLEWINDOWSTRUCT dsws;
	dsws.hDC = dc.m_hDC;
	GetClientRect(&dsws.rcItem);
	::SendMessage(GetParent(), WM_DRAWSTYLEWINDOW, GetDlgCtrlID(), (LPARAM)&dsws);

	return 0;
}

LRESULT CStyleWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	::SendMessage(GetParent(), WM_SELECTSTYLEWINDOW, GetDlgCtrlID(), NULL);
	return 0;
}
