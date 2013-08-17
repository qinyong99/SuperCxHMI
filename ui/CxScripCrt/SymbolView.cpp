// SymbolView.cpp: implementation of the CSymbolView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxScripCrt.h"
#include "SymbolView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSymbolView::CSymbolView()
{
	m_nCurrentPage = 0;
}

LRESULT CSymbolView::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_ImageList.Create(16, 16, ILC_MASK, 2, 2);

	ImageList_AddIcon(m_ImageList, AtlLoadIcon(IDI_OBJECT));
	ImageList_AddIcon(m_ImageList, AtlLoadIcon(IDI_PROP));
	ImageList_AddIcon(m_ImageList, AtlLoadIcon(IDI_METHOD));

//	m_ImageList.Add((HICON)AtlLoadIcon(IDI_PROP));
//	m_ImageList.Add(AtlLoadIcon(IDI_METHOD));
	
	CRect rect;
	rect.SetRect(0, 0, 0, 0);
	m_ctrlTab.Create(m_hWnd, &rect, NULL, 
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | /*WS_CLIPCHILDREN | WS_CLIPSIBLINGS |*/ TCS_BOTTOM | TCS_FIXEDWIDTH, WS_EX_CONTROLPARENT, 100);
	m_ctrlTab.SetFont(AtlGetStockFont(DEFAULT_GUI_FONT));

	TC_ITEM tci;
	memset(&tci, NULL, sizeof TC_ITEM);
	tci.mask = TCIF_TEXT;

	for (int i = 0; i < m_apTreePages.GetSize(); i++)
	{
		CTreePage* pTreePage = m_apTreePages[i];
		
		tci.pszText = (LPTSTR)(LPCTSTR)pTreePage->m_strCaption;
		m_ctrlTab.InsertItem(i, &tci);

		if (m_ctrlTab.GetItemCount() == 1)
		{
			CRect rcItem;
			m_ctrlTab.GetItemRect(0, &rcItem);
			m_ctrlTab.SetItemSize(CSize(40, rcItem.Height()));
		}

		pTreePage->Create(m_ctrlTab);
		pTreePage->MoveWindow(&rect);
	
//		pTreePage->Create(m_ctrlTab, rect, 
//			WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP | 
//			TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_DISABLEDRAGDROP,
//			WS_EX_CLIENTEDGE, 100 + i);

		if (pTreePage->m_bDisplayIcon)
		{
			pTreePage->m_tree.SetImageList((HIMAGELIST)m_ImageList, TVSIL_NORMAL);
			pTreePage->m_list.SetImageList((HIMAGELIST)m_ImageList, LVSIL_SMALL);
		}

		pTreePage->OnInit();

		if (m_nCurrentPage == i)
			pTreePage->ShowWindow(SW_SHOW);
	}

	return 0;
}

void CSymbolView::GetPageDisplayRect(RECT* pRect)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rect = rcClient;
	m_ctrlTab.AdjustRect(false, &rect);
	pRect->left = rcClient.left;
	pRect->top = rcClient.top;
	pRect->right = rcClient.right;
	pRect->bottom = rect.bottom - ::GetSystemMetrics(SM_CYEDGE) - 2; 
}

void CSymbolView::UpdateLayout()
{
	CRect rect;
	GetClientRect(&rect);

	m_ctrlTab.SetWindowPos(NULL, rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,	SWP_NOZORDER | SWP_NOACTIVATE);

	GetPageDisplayRect(&rect);

	for (int i = 0; i < m_apTreePages.GetSize(); i++)
	{
		CTreePage* pTreePage = m_apTreePages[i];
		
		CRect rectT = rect;
		::AdjustWindowRectEx(&rectT, GetStyle(), FALSE, GetExStyle());
		pTreePage->SetWindowPos(NULL, rectT.left, rectT.top,
			rectT.right - rectT.left, rectT.bottom - rectT.top,	SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

LRESULT CSymbolView::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	UpdateLayout();

	return 0;
}

LRESULT CSymbolView::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1;
}

LRESULT CSymbolView::OnSelchangeTab(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int nSel = m_ctrlTab.GetCurSel();
	ShowPage(nSel);

	NMHDR nmhdr = { m_hWnd, GetDlgCtrlID(), TCN_SELCHANGE };
	::SendMessage(GetParent(), WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmhdr);

	return 0;
}

void CSymbolView::ShowPage(int nIndex)
{
	m_nCurrentPage = nIndex;
	for (int i = 0; i < m_apTreePages.GetSize(); i++)
	{
		CTreePage* pTreePage = m_apTreePages[i];
		pTreePage->ShowWindow(m_nCurrentPage == i ? SW_SHOW : SW_HIDE);
	}
}

CTreePage* CSymbolView::GetCurrentTreePage()
{
	if (m_nCurrentPage >= 0 && m_nCurrentPage < m_apTreePages.GetSize())
		return m_apTreePages[m_nCurrentPage];

	return NULL;
}

void CSymbolView::AddTreePage(CTreePage* pTreePage)
{
	if (::IsWindow(m_hWnd))
	{
		TC_ITEM tci;
		memset(&tci, NULL, sizeof TC_ITEM);
		tci.mask = TCIF_TEXT;
		tci.pszText = (LPTSTR)(LPCTSTR)pTreePage->m_strCaption;
		m_ctrlTab.InsertItem(m_apTreePages.GetSize(), &tci);

		if (m_ctrlTab.GetItemCount() == 1)
		{
			CRect rcItem;
			m_ctrlTab.GetItemRect(0, &rcItem);
			m_ctrlTab.SetItemSize(CSize(40, rcItem.Height()));
		}

		pTreePage->Create(m_ctrlTab);

		CRect rect;
		GetPageDisplayRect(rect);
		::AdjustWindowRectEx(&rect, GetStyle(), FALSE, GetExStyle());
		pTreePage->MoveWindow(&rect);
		
// 		pTreePage->Create(m_ctrlTab, rect,
// 			WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP | 
// 			TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_DISABLEDRAGDROP,
// 			WS_EX_CLIENTEDGE, 100 + m_apTreePages.GetSize());
 
 		if (pTreePage->m_bDisplayIcon)
		{
			pTreePage->m_tree.SetImageList((HIMAGELIST)m_ImageList, TVSIL_NORMAL);
			pTreePage->m_list.SetImageList((HIMAGELIST)m_ImageList, LVSIL_SMALL);
		}

		pTreePage->OnInit();
	
		if (m_nCurrentPage == m_apTreePages.GetSize())
			pTreePage->ShowWindow(SW_SHOW);
	}

	pTreePage->AddRef();
	m_apTreePages.Add(pTreePage);
}

CTreePage* CSymbolView::AddTreePage(LPCTSTR lpszCaption, BOOL bDisplayIcon, HWND hWndNotify)
{
	CTreePage* pTreePage = new CTreePage;
	pTreePage->m_strCaption = lpszCaption;
	pTreePage->m_bDisplayIcon = bDisplayIcon;
	pTreePage->m_hNotifyWindow = hWndNotify;

	AddTreePage(pTreePage);
	
	return pTreePage;
} 
void CSymbolView::AddGeneralPage(DWORD dwSymbolMask, HWND hWndNotify)
{
	CGeneralPage* pGeneralPage = new CGeneralPage;
	pGeneralPage->m_strCaption = _T("³£¹æ");
	pGeneralPage->m_bDisplayIcon = false;
	pGeneralPage->m_hNotifyWindow = hWndNotify;
	pGeneralPage->m_dwSymbolMask = dwSymbolMask;
	AddTreePage(pGeneralPage);
}

void CSymbolView::AddOPCServerPage(HWND hWndNotify)
{
	COPCServerPage* pOPCServerPage = new COPCServerPage;
	pOPCServerPage->m_strCaption = _T("OPC");
	pOPCServerPage->m_bDisplayIcon = false;
	pOPCServerPage->m_hNotifyWindow = hWndNotify;
	AddTreePage(pOPCServerPage);
}

void CSymbolView::SetNotifyWindow(HWND hWndNotify)
{
	for (int i = 0; i < m_apTreePages.GetSize(); i++)
		m_apTreePages[i]->m_hNotifyWindow = hWndNotify;
}

LRESULT CSymbolView::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	for (int i = 0; i < m_apTreePages.GetSize(); i++)
	{
		CTreePage* pTreePage = m_apTreePages[i];
		pTreePage->Release();
	}
	
	return 0;
}

LRESULT CSymbolView::OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
