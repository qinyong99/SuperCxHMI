// KeyBoardWnd.cpp: implementation of the CKeyBoardWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KeyBoardWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyBoardWnd::CKeyBoardWnd()
{

}

CKeyBoardWnd::~CKeyBoardWnd()
{

}


LRESULT CKeyBoardWnd::OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 1;   // no background needed
}


LRESULT CKeyBoardWnd::OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);
	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillRect(&rcClient, COLOR_3DFACE);

	for (int i = 0; i < 23; i++)
	{
		CRect rect;
//		GetItemRect(i, rect);
//		DrawItem(dc.m_hDC, rect, i, i == m_pExpert->m_pSampleObject->m_enumFillStyle);
	}
		
	return 0;
}

LRESULT CKeyBoardWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	
	
	return 0;
}
