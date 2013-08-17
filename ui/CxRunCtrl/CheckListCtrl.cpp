// CheckListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "cxrunctrl.h"
#include "CheckListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckListCtrl

CCheckListCtrl::CCheckListCtrl()
{
}

CCheckListCtrl::~CCheckListCtrl()
{
}


BEGIN_MESSAGE_MAP(CCheckListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CCheckListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckListCtrl message handlers

void CCheckListCtrl::SetCheckStyle()
{
	SetExtendedListViewStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

// 	m_StateImageList.Create(IDB_STATEICONS, 16, 1, RGB(255, 0, 0));
// 	SetImageList(&m_StateImageList, LVSIL_STATE);
}

// dwExMask = 0 means all styles
DWORD CCheckListCtrl::SetExtendedListViewStyle(DWORD dwExStyle, DWORD dwExMask)
{
	ATLASSERT(::IsWindow(m_hWnd));
	return (DWORD)::SendMessage(m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, dwExMask, dwExStyle);
}

void CCheckListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UINT uFlags = 0;
	int nHitItem = HitTest(point, &uFlags);
	
	if (uFlags & LVHT_ONITEMSTATEICON && ::GetKeyState(VK_CONTROL) >= 0)
		CheckSelectedItems(nHitItem);
	
	CListCtrl::OnLButtonDown(nFlags, point);
}

BOOL CCheckListCtrl::GetCheckState(int nIndex)
{
	UINT uRet = GetItemState(nIndex, LVIS_STATEIMAGEMASK);
	return (uRet >> 12) - 1;
}

BOOL CCheckListCtrl::SetCheckState(int nItem, BOOL bCheck)
{
	int nCheck = bCheck ? 2 : 1;	// one based index
	return SetItemState(nItem, INDEXTOSTATEIMAGEMASK(nCheck), LVIS_STATEIMAGEMASK);
}

void CCheckListCtrl::CheckSelectedItems(int nCurrItem)
{
	// first check if this item is selected
	LVITEM lvi;
	lvi.iItem = nCurrItem;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_STATE;
	lvi.stateMask = LVIS_SELECTED;
	GetItem(&lvi);
	// if item is not selected, don't do anything
	if(!(lvi.state & LVIS_SELECTED))
		return;
	// new check state will be reverse of the current state,
	BOOL bCheck = !GetCheckState(nCurrItem);
	int nItem = -1;
	int nOldItem = -1;
	while((nItem = GetNextItem(nOldItem, LVNI_SELECTED)) != -1)
	{
		if(nItem != nCurrItem)
			SetCheckState(nItem, bCheck);
		nOldItem = nItem;
	}
}

void CCheckListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_SPACE)
	{
		int nCurrItem = GetNextItem(-1, LVNI_FOCUSED);
		if(nCurrItem != -1  && ::GetKeyState(VK_CONTROL) >= 0)
			CheckSelectedItems(nCurrItem);
	}
	
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}
