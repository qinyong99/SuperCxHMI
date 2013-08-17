// SymbolView.h: interface for the CSymbolView class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SYMBOLVIEW_H_
#define __SYMBOLVIEW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TreePage.h"
#include "GeneralPage.h"
#include "OPCServerPage.h"

class CSymbolView : public CWindowImpl<CSymbolView>
{
public:
	CSymbolView();

BEGIN_MSG_MAP(CSymbolView)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	NOTIFY_HANDLER(100, TCN_SELCHANGE, OnSelchangeTab)
	MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSelchangeTab(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	void AddGeneralPage(DWORD dwSymbolMask, HWND hWndNotify = NULL);
	void AddOPCServerPage(HWND hWndNotify = NULL);
	CTreePage* AddTreePage(LPCTSTR lpszCaption, BOOL bDisplayIcon = TRUE, HWND hWndNotify = NULL);
	void AddTreePage(CTreePage* pTreePage);
	
	void ShowPage(int nIndex);
	CTreePage* GetCurrentTreePage();
	void SetNotifyWindow(HWND hWndNotify);

	void GetPageDisplayRect(RECT* pRect);
	void UpdateLayout();

	CImageList m_ImageList;

	CSimpleArray<CTreePage *> m_apTreePages;
	int m_nCurrentPage;

	CTabCtrl m_ctrlTab;
};

#endif // __SYMBOLVIEW_H_
