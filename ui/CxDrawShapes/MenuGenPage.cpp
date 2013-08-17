// MenuGenPage.cpp : Implementation of CMenuGenPage
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "MenuGenPage.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuGenPage

typedef struct tagKEYNAME
{
	TCHAR szName[32];
	int nVirtKey;
} KEYNAME;

LPCTSTR GetKeyName(int nVirtKey)
{
	static KEYNAME keys[]=
	{
		{"Left mouse button", VK_LBUTTON},
		{"Right mouse button", VK_RBUTTON},
		{"Control-break processing", VK_CANCEL},
		{"Middle mouse button", VK_MBUTTON},
		{"Back Space", VK_BACK},
		{"Tab", VK_TAB},
		{"Clear", VK_CLEAR},
		{"Enter", VK_RETURN},
		{"Shift", VK_SHIFT},
		{"Ctrl", VK_CONTROL},
		{"Alt", VK_MENU},
		{"Pause", VK_PAUSE},
		{"Caps Lock", VK_CAPITAL},
		{"Esc", VK_ESCAPE},
		{"Space", VK_SPACE},
		{"Page Up", VK_PRIOR},  
		{"Page Down", VK_NEXT},
		{"End", VK_END},
		{"Home", VK_HOME},
		{"Left", VK_LEFT},
		{"Up", VK_UP},
		{"Right", VK_RIGHT},
		{"Down", VK_DOWN},
		{"Select", VK_SELECT},
		{"Excute", VK_EXECUTE},
		{"Print Screen", VK_SNAPSHOT},
		{"Insert", VK_INSERT},
		{"Delete", VK_DELETE},
		{"Help", VK_HELP},
		{"0",'0'},
		{"1",'1'},
		{"2",'2'},
		{"3",'3'},
		{"4",'4'},
		{"5",'5'},
		{"6",'6'},
		{"7",'7'},
		{"8",'8'},
		{"9",'9'},
		{"A",'A'},
		{"B",'B'},
		{"C",'C'},
		{"D",'D'},
		{"E",'E'},
		{"F",'F'},
		{"G",'G'},
		{"H",'H'},
		{"I",'I'},
		{"J",'J'},
		{"K",'K'},
		{"L",'L'},
		{"M",'M'},
		{"N",'N'},
		{"O",'O'},
		{"P",'P'},
		{"Q",'Q'},
		{"R",'R'},
		{"S",'S'},
		{"T",'T'},
		{"U",'U'},
		{"V",'V'},
		{"W",'W'},
		{"X",'X'},
		{"Y",'Y'},
		{"Z",'Z'},
		{"Left windows", VK_LWIN},
		{"Right windows", VK_RWIN},
		{"Applications", VK_APPS},
		{"Numeric keypad 0", VK_NUMPAD0},
		{"Numeric keypad 1", VK_NUMPAD1},
		{"Numeric keypad 2", VK_NUMPAD2},
		{"Numeric keypad 3", VK_NUMPAD3},
		{"Numeric keypad 4", VK_NUMPAD4},
		{"Numeric keypad 5", VK_NUMPAD5},
		{"Numeric keypad 6", VK_NUMPAD6},
		{"Numeric keypad 7", VK_NUMPAD7},
		{"Numeric keypad 8", VK_NUMPAD8},
		{"Numeric keypad 9", VK_NUMPAD9}, 
		{"Multiply", VK_MULTIPLY},
		{"Add", VK_ADD},
		{"Separator", VK_SEPARATOR},
		{"Subtract", VK_SUBTRACT},
		{"Decimal Point", VK_DECIMAL},
		{"Divide", VK_DIVIDE},
		{"F1", VK_F1},
		{"F2", VK_F2},
		{"F3", VK_F3},
		{"F4", VK_F4},
		{"F5", VK_F5},
		{"F6", VK_F6},
		{"F7", VK_F7},
		{"F8", VK_F8},
		{"F9", VK_F9},
		{"F10", VK_F10},
		{"F11", VK_F11},
		{"F12", VK_F12},
		{"F13", VK_F13},
		{"F14", VK_F14},
		{"F15", VK_F15},
		{"F16", VK_F16},
		{"F17", VK_F17},
		{"F18", VK_F18},
		{"F19", VK_F19},
		{"F20", VK_F20},
		{"F21", VK_F21},
		{"F22", VK_F22},
		{"F23", VK_F23},
		{"F24", VK_F24},
		{"Attn", VK_ATTN},
		{"CrSel", VK_CRSEL},
		{"ExSel", VK_EXSEL},
		{"Erase", VK_EREOF},
		{"Play", VK_PLAY},
		{"Zoom", VK_ZOOM},
		{"Reserved for future use", VK_NONAME},
		{"PA1", VK_PA1},
		{"Clear(OEM)", VK_OEM_CLEAR},
		{"\\", '\\'},
		{"-", '-'},
		{"=", '='},
		{"[", '['},
		{"]", ']'},
		{";", ';'},
		{"\'", '\''},
		{",", ','},
		{".", '.'},
		{"/", '/'},
		{"`", '`'}
	};

	for (int i = 0; i < sizeof(keys) / sizeof(KEYNAME); i++)
	{
		if (keys[i].nVirtKey == nVirtKey)
			return keys[i].szName;
	}

	return NULL;
}

CString GetAccelKeyName(DWORD dwShift, DWORD dwKeyCode)
{
	CString strText;

	if ((dwShift & 0x2) != 0)
	{
		strText += _T("Ctrl");
	}
	if ((dwShift & 0x4) != 0)
	{
		if (!strText.IsEmpty())
			strText += _T(" + ");
		strText += _T("Alt");
	}
	if ((dwShift & 0x1) != 0)
	{
		if (!strText.IsEmpty())
			strText += _T(" + ");
		strText += _T("Shift");
	}
	
	if (dwKeyCode != VK_CONTROL && dwKeyCode != VK_MENU && dwKeyCode != VK_SHIFT)
	{
		LPCTSTR pKeyName = GetKeyName(dwKeyCode);
		if (pKeyName != NULL)
		{
			if (!strText.IsEmpty())
				strText += _T(" + ");
			
			strText += GetKeyName(dwKeyCode);
		}
	}

	return strText;
}

LRESULT CMenuGenPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	m_btnButtonDisplayed.Attach(GetDlgItem(IDC_BUTTON_DISPLAYED));
	m_editButtonCaption.Attach(GetDlgItem(IDC_BUTTON_CAPTION));
	m_btnSeparator.Attach(GetDlgItem(IDC_SEPARATOR));
	m_btnCheck.Attach(GetDlgItem(IDC_CHECK));
	m_btnDisable.Attach(GetDlgItem(IDC_DISABLE));
	m_tree.SubclassWindow(GetDlgItem(IDC_TREE));
	m_btnAccelKeyEnable.Attach(GetDlgItem(IDC_ACCEL_KEY_ENABLE));
	m_editAccelKey.SubclassWindow(GetDlgItem(IDC_ACCEL_KEY));
	m_editID.Attach(GetDlgItem(IDC_NAME));

	m_ImageList.Create(IDB_MENU, 16, 1, RGB(255, 255, 255));
	m_tree.SetImageList((HIMAGELIST)m_ImageList, TVSIL_NORMAL);

	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_UP), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	::SendMessage(GetDlgItem(IDC_UP), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
	
	hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_DOWN), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		::SendMessage(GetDlgItem(IDC_DOWN), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);

	hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_LEFT), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	::SendMessage(GetDlgItem(IDC_LEFT), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
	
	hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_RIGHT), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	::SendMessage(GetDlgItem(IDC_RIGHT), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);

	m_bLoadPropery = TRUE;

	if (m_nObjects > 0)
	{
		CComQIPtr<IDrawMenu, &IID_IDrawMenu> spMenu(m_ppUnk[0]);
		
		VARIANT_BOOL b;
		spMenu->get_ButtonDisplayed(&b);
		m_btnButtonDisplayed.SetCheck(b != VARIANT_FALSE);

	 	CComBSTR bstr;
		spMenu->get_ButtonCaption(&bstr);
		m_editButtonCaption.SetWindowText(OLE2T(bstr));

		long lCount;
		spMenu->get_ItemCount(&lCount);

		int nIndex = 0;
		AddMenuItems(NULL, spMenu, nIndex, lCount, 0);
	}

	if (m_tree.GetRootItem() == NULL)
	{
		::EnableWindow(GetDlgItem(IDC_UP), FALSE);
		::EnableWindow(GetDlgItem(IDC_DOWN), FALSE);
		::EnableWindow(GetDlgItem(IDC_LEFT), FALSE);
		::EnableWindow(GetDlgItem(IDC_RIGHT), FALSE);
		::EnableWindow(GetDlgItem(IDC_REMOVE), FALSE);
		
		m_btnSeparator.EnableWindow(FALSE);
		m_btnCheck.EnableWindow(FALSE);
		m_btnDisable.EnableWindow(FALSE);
		m_btnAccelKeyEnable.EnableWindow(FALSE);
		m_editAccelKey.EnableWindow(FALSE);
		m_editID.EnableWindow(FALSE);
	}

	m_bLoadPropery = FALSE;
	
	return 0;
}

void CMenuGenPage::AddMenuItems(HTREEITEM hParentItem, IDrawMenu* pMenu, int& nIndex, int nCount, int nDepth)
{
	USES_CONVERSION;
	HTREEITEM hItem = NULL;

	while (nIndex < nCount)
	{
		CComBSTR bstrText;
		pMenu->GetItemText(nIndex + 1, &bstrText);
		
		int nItemDepth = 0;
		LPCWSTR p = bstrText;
		while (*p++ == '.')
			nItemDepth++;

		if (nItemDepth < nDepth)
		{
			nIndex--;
			break;
		}
		
		if (nItemDepth > nDepth)
		{
			ATLASSERT(hItem != NULL);
			AddMenuItems(hItem, pMenu, nIndex, nCount, nDepth + 1);
			m_tree.Expand(hItem);
		}
		else
		{
			hItem = m_tree.InsertItem(OLE2T(bstrText + nDepth), 0, 0, hParentItem, TVI_LAST);
			
			CItemInfo* pInfo = new CItemInfo;

			long l;
			pMenu->GetItemState(nIndex + 1, &l);
			pInfo->m_dwState = l;

			CComBSTR bstrID;
			pMenu->GetItemID(nIndex + 1, &bstrID);
			pInfo->m_strID = bstrID;

			pMenu->GetItemAccelShift(nIndex + 1, &l);
			pInfo->m_dwShiftState = l;

			pMenu->GetItemAccelKeyCode(nIndex + 1, &l);
			pInfo->m_dwKeyCode = l;

			m_tree.SetItemData(hItem, (DWORD)pInfo);
		}
		
		nIndex++;
	}
}

STDMETHODIMP CMenuGenPage::Apply(void)
{
	m_bDirty = FALSE;
	
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<IDrawMenu, &IID_IDrawMenu> spMenu(m_ppUnk[i]);

		spMenu->put_ButtonDisplayed(m_btnButtonDisplayed.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
			
		CComBSTR bstr;
		m_editButtonCaption.GetWindowText((BSTR&)bstr);
		spMenu->put_ButtonCaption(bstr);
	
		spMenu->Clear();

		int nIndex = 1;
		AddMenuItems(NULL, spMenu, nIndex, 0);
	}
	
	return S_OK;
}

void CMenuGenPage::AddMenuItems(HTREEITEM hItem, IDrawMenu* pMenu, int& nIndex, int nDepth)
{
	USES_CONVERSION;
	CString strItemText;
	CString strDepthPrefix;
	
	if (hItem == NULL)
		hItem = m_tree.GetRootItem();
	else
		hItem = m_tree.GetChildItem(hItem);

	// 深度前缀
	for (int i = 0; i < nDepth; i++)
		strDepthPrefix += _T(".");

	while (hItem != NULL)
	{
		CItemInfo* pInfo = (CItemInfo*)m_tree.GetItemData(hItem);

		CComVariant varIndex(nIndex++);
		CComVariant varState = (long)pInfo->m_dwState;
		CComVariant varID = pInfo->m_strID;
		CComVariant varShift = (long)pInfo->m_dwShiftState;
		CComVariant varKeyCode = (long)pInfo->m_dwKeyCode;
		m_tree.GetItemText(hItem, strItemText);

		strItemText = strDepthPrefix + strItemText;
		pMenu->AddItem(T2OLE(strItemText), varIndex, varState, varID, varShift, varKeyCode);

		AddMenuItems(hItem, pMenu, nIndex, nDepth + 1);
		
		hItem = m_tree.GetNextSiblingItem(hItem);
	}
}

LRESULT CMenuGenPage::OnSelchangedTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)pnmh;
	HTREEITEM hSelect = pnmtv->itemNew.hItem;

	::EnableWindow(GetDlgItem(IDC_UP), m_tree.GetPrevSiblingItem(hSelect) != NULL);
	::EnableWindow(GetDlgItem(IDC_DOWN), m_tree.GetNextSiblingItem(hSelect) != NULL);
	::EnableWindow(GetDlgItem(IDC_LEFT), m_tree.GetParentItem(hSelect) != NULL);
	::EnableWindow(GetDlgItem(IDC_RIGHT), m_tree.GetPrevSiblingItem(hSelect) != NULL);

	::EnableWindow(GetDlgItem(IDC_REMOVE), TRUE);

	CString strItemText;
	m_tree.GetItemText(hSelect, strItemText);
	
	BOOL bSeparator = strItemText == _T("-");
	
	m_btnSeparator.EnableWindow(TRUE);
	m_btnCheck.EnableWindow(!bSeparator);
	m_btnDisable.EnableWindow(!bSeparator);
	m_btnAccelKeyEnable.EnableWindow(!bSeparator);
	m_editAccelKey.EnableWindow(!bSeparator);
	m_editID.EnableWindow(!bSeparator);
	
	m_btnSeparator.SetCheck(bSeparator);
	if (!bSeparator)
	{
		CItemInfo* pInfo = (CItemInfo*)m_tree.GetItemData(hSelect);

		m_btnDisable.SetCheck((pInfo->m_dwState & MenuStateDisable) != 0);
		m_btnCheck.SetCheck((pInfo->m_dwState & MenuStateCheck) != 0);

		SetDlgItemText(IDC_NAME, pInfo->m_strID);

		BOOL bHasAccel = pInfo->m_dwShiftState != 0 || pInfo->m_dwKeyCode != 0;
		m_btnAccelKeyEnable.SetCheck(bHasAccel);
		m_editAccelKey.EnableWindow(bHasAccel);
		
		CString strText = GetAccelKeyName(pInfo->m_dwShiftState, pInfo->m_dwKeyCode);
		m_editAccelKey.SetWindowText(strText);
	}
	else
	{
		m_editID.SetWindowText(_T(""));
		m_editAccelKey.SetWindowText(_T(""));
	}

	return 0;
}

HTREEITEM CMenuGenPage::CopyItemTo(HTREEITEM hItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	HTREEITEM hNewItem = NULL;

	ATLASSERT(hItem != NULL);
	
	CString strText;
	m_tree.GetItemText(hItem, strText);
	DWORD dwData = m_tree.GetItemData(hItem);
	m_tree.SetItemData(hItem, 0);

	hNewItem = m_tree.InsertItem(strText, 0, 0, hParent, hInsertAfter);
	ATLASSERT(hNewItem != NULL);
	m_tree.SetItemData(hNewItem, dwData);

	HTREEITEM hChildItem = m_tree.GetChildItem(hItem);
	while (hChildItem != NULL)
	{
		CopyItemTo(hChildItem, hNewItem, TVI_LAST);

		hChildItem = m_tree.GetNextSiblingItem(hChildItem);
	}

	return hNewItem;
}

void CMenuGenPage::MoveItemTo(HTREEITEM hItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	m_tree.SetRedraw(FALSE);
	
	HTREEITEM hNewItem = CopyItemTo(hItem, hParent, hInsertAfter);
	m_tree.SelectItem(hNewItem);
	m_tree.Expand(hNewItem);
	m_tree.DeleteItem(hItem);
	
	m_tree.SetRedraw(TRUE);
}

LRESULT CMenuGenPage::OnClickedUp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HTREEITEM hSelect = m_tree.GetSelectedItem();
	ATLASSERT(hSelect != NULL);
	if (hSelect != NULL)
	{
		HTREEITEM hParent = m_tree.GetParentItem(hSelect);
		HTREEITEM hPrev = m_tree.GetPrevSiblingItem(hSelect);
		ATLASSERT(hPrev != NULL);
		hPrev = m_tree.GetPrevSiblingItem(hPrev);

		MoveItemTo(hSelect, hParent, hPrev != NULL ? hPrev : TVI_FIRST);
	}
	
	return 1;
}

LRESULT CMenuGenPage::OnClickedDown(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HTREEITEM hSelect = m_tree.GetSelectedItem();
	ATLASSERT(hSelect != NULL);
	if (hSelect != NULL)
	{
		HTREEITEM hParent = m_tree.GetParentItem(hSelect);
		HTREEITEM hNext = m_tree.GetNextSiblingItem(hSelect);
		ATLASSERT(hNext != NULL);

		MoveItemTo(hSelect, hParent, hNext != NULL ? hNext : TVI_LAST);
	}
	
	return 1;
}

LRESULT CMenuGenPage::OnClickedLeft(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HTREEITEM hSelect = m_tree.GetSelectedItem();
	ATLASSERT(hSelect != NULL);
	if (hSelect != NULL)
	{
		HTREEITEM hPrev = m_tree.GetParentItem(hSelect);
		ATLASSERT(hPrev != NULL);
		HTREEITEM hParent = m_tree.GetParentItem(hPrev);

		MoveItemTo(hSelect, hParent, hPrev != NULL ? hPrev : TVI_FIRST);
	}
	
	return 1;
}

LRESULT CMenuGenPage::OnClickedRight(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HTREEITEM hSelect = m_tree.GetSelectedItem();
	ATLASSERT(hSelect != NULL);
	if (hSelect != NULL)
	{
		HTREEITEM hPrev = m_tree.GetPrevSiblingItem(hSelect);
		ATLASSERT(hPrev != NULL);

		MoveItemTo(hSelect, hPrev, TVI_LAST);
	}
	
	return 1;
}

LRESULT CMenuGenPage::OnClickedInsert(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HTREEITEM hSelect = NULL;
	HTREEITEM hParent = NULL;
	HTREEITEM hItem = NULL;

	m_tree.SetRedraw(FALSE);

	hSelect = m_tree.GetSelectedItem();
	if (hSelect != NULL)
		hParent = m_tree.GetParentItem(hSelect);
	
	hItem = m_tree.InsertItem(_T("未命名菜单项"), 0, 0, hParent, hSelect != NULL ? hSelect : TVI_LAST);
	CItemInfo* pInfo = new CItemInfo;
	m_tree.SetItemData(hItem, (DWORD)pInfo);

	m_tree.SelectItem(hItem);

	m_tree.SetRedraw(TRUE);

	SetDirty();

	m_tree.EditLabel(hItem);

	return 1;
}

//LRESULT CMenuGenPage::OnDblclkTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
//{
//	UINT nFlags;
//	CPoint curPoint;
//	GetCursorPos(&curPoint);
//	m_tree.ScreenToClient(&curPoint);
//	HTREEITEM hSelect = m_tree.HitTest(curPoint, &nFlags);
//	if (hSelect != NULL)
//		m_tree.EditLabel(hSelect);
//
//	return 1;
//}

LRESULT CMenuGenPage::OnLButtonDownTree(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	HTREEITEM hSelect = m_tree.GetSelectedItem();
	
	POINT ptMsg = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	TVHITTESTINFO tvh;
	tvh.pt = ptMsg;
	if (m_tree.HitTest(&tvh) != NULL && 
		(tvh.flags == TVHT_ONITEMICON || tvh.flags == TVHT_ONITEMLABEL))
	{
		if (hSelect == tvh.hItem)
		{
			m_tree.EditLabel(hSelect);
		}
		else
		{
			m_tree.SetFocus();
			m_tree.SelectItem(tvh.hItem);
		}

		return 1;
	}
	
	bHandled = FALSE;

	return 1;
}

//LRESULT CDynamicExpert::OnKeyDownModes(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	USES_CONVERSION;
//	HTREEITEM hCurrItem = NULL;
//	
//	if (wParam == VK_SPACE)
//	{
//		hCurrItem = m_treeModes.GetSelectedItem();
//		if (m_treeModes.GetParentItem(hCurrItem) == NULL || !m_treeModes.GetCheckState(hCurrItem))
//		{
//			SyncModePage();
//			return 1;
//		}
//	}
//	
//	LRESULT lRet = m_treeModes.DefWindowProc(uMsg, wParam, lParam);
//	
//	if (hCurrItem != NULL)
//	{
//		if (!m_treeModes.GetCheckState(hCurrItem))
//		{
//			int nPropIndex = m_treeModes.GetItemData(hCurrItem);
//			CPropInfo& prop = m_arrProps[nPropIndex];
//			
//			OnPropertyChanging();
//			DeleteDynamic(prop.m_bstrName);
//			OnPropertyChanged();
//			m_treeModes.SetCheckState(hCurrItem, FALSE);
//		}
//	}
//	
//	SyncModePage();
//	
//	return lRet;
//}

LRESULT CMenuGenPage::OnEndlabeleditTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pnmh;
	if (pTVDispInfo->item.pszText != NULL)
	{
		CString strItemText = pTVDispInfo->item.pszText;

		BOOL bSeparator = strItemText == _T("-");
		
		m_btnSeparator.EnableWindow(TRUE);
		m_btnCheck.EnableWindow(!bSeparator);
		m_btnDisable.EnableWindow(!bSeparator);
		
		m_btnSeparator.SetCheck(bSeparator);
		if (!bSeparator)
		{
			DWORD dw = pTVDispInfo->item.lParam;
			m_btnDisable.SetCheck((dw & MenuStateDisable) != 0);
			m_btnCheck.SetCheck((dw & MenuStateCheck) != 0);
		}

		return 1;
	}

	return 0;
}

LRESULT CMenuGenPage::OnClickedRemove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HTREEITEM hSelect = m_tree.GetSelectedItem();
	if (hSelect != NULL)
	{
		m_tree.DeleteItem(hSelect);
		SetDirty();

		if (m_tree.GetRootItem() == NULL)
		{
			::EnableWindow(GetDlgItem(IDC_UP), FALSE);
			::EnableWindow(GetDlgItem(IDC_DOWN), FALSE);
			::EnableWindow(GetDlgItem(IDC_LEFT), FALSE);
			::EnableWindow(GetDlgItem(IDC_RIGHT), FALSE);
			::EnableWindow(GetDlgItem(IDC_REMOVE), FALSE);
			
			m_btnSeparator.EnableWindow(FALSE);
			m_btnCheck.EnableWindow(FALSE);
			m_btnDisable.EnableWindow(FALSE);
			m_btnAccelKeyEnable.EnableWindow(FALSE);
			m_editAccelKey.EnableWindow(FALSE);
			m_editID.EnableWindow(FALSE);
		}
	}

	return 1;
}

LRESULT CMenuGenPage::OnChangeCaption(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SetDirty();
	
	return 0;
}

LRESULT CMenuGenPage::OnClickedCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{	
	HTREEITEM hSelect = m_tree.GetSelectedItem();
	if (hSelect != NULL)
	{
		CItemInfo* pInfo = (CItemInfo*)m_tree.GetItemData(hSelect);

		if (m_btnCheck.GetCheck())
			pInfo->m_dwState |= MenuStateCheck;
		else
			pInfo->m_dwState &= ~MenuStateCheck;
	
		SetDirty();
	}

	return 0;
}

LRESULT CMenuGenPage::OnClickedDisable(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HTREEITEM hSelect = m_tree.GetSelectedItem();
	if (hSelect != NULL)
	{
		CItemInfo* pInfo = (CItemInfo*)m_tree.GetItemData(hSelect);
		
		if (m_btnDisable.GetCheck())
			pInfo->m_dwState |= MenuStateDisable;
		else
			pInfo->m_dwState &= ~MenuStateDisable;
			
		SetDirty();
	}

	return 0;
}

LRESULT CMenuGenPage::OnClickedAccelKeyEnable(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HTREEITEM hSelect = m_tree.GetSelectedItem();
	if (hSelect != NULL)
	{
		CItemInfo* pInfo = (CItemInfo*)m_tree.GetItemData(hSelect);
		
		if (m_btnAccelKeyEnable.GetCheck())
		{
			m_editAccelKey.EnableWindow(TRUE);
		}
		else
		{
			pInfo->m_dwShiftState = 0;
			pInfo->m_dwKeyCode = 0;
			m_editAccelKey.EnableWindow(FALSE);
		}
		
		SetDirty();
	}

	return 0;
}

LRESULT CMenuGenPage::OnKillfocusName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HTREEITEM hSelect = m_tree.GetSelectedItem();
	if (hSelect != NULL)
	{
		CItemInfo* pInfo = (CItemInfo*)m_tree.GetItemData(hSelect);
		
		CComBSTR bstr;
		m_editID.GetWindowText((BSTR&)bstr);
		pInfo->m_strID = bstr;
		pInfo->m_strID.TrimLeft();
		pInfo->m_strID.TrimRight();
		
		SetDirty();
	}
	
	return 0;
}

LRESULT CMenuGenPage::OnClickedSeparator(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HTREEITEM hSelect = m_tree.GetSelectedItem();
	if (hSelect != NULL)
	{
		BOOL bSeparator = m_btnSeparator.GetCheck();
		m_btnCheck.EnableWindow(!bSeparator);
		m_btnDisable.EnableWindow(!bSeparator);
		m_btnAccelKeyEnable.EnableWindow(!bSeparator);
		m_editAccelKey.EnableWindow(m_btnAccelKeyEnable.GetCheck());
		m_editID.EnableWindow(!bSeparator);

		if (bSeparator)
		{
			m_btnDisable.SetCheck(false);
			m_btnCheck.SetCheck(false);
			m_btnAccelKeyEnable.SetCheck(false);

			CItemInfo* pInfo = (CItemInfo*)m_tree.GetItemData(hSelect);
			pInfo->m_strID.Empty();
			pInfo->m_dwState = 0;
			pInfo->m_dwShiftState = 0;
			pInfo->m_dwKeyCode = 0;

//			m_editID.SetWindowText(_T(""));
//			m_editAccelKey.SetWindowText(_T(""));
		}

		m_tree.SetItemText(hSelect, bSeparator ? _T("-") : _T(""));
	}

	return 0;
}

LRESULT CMenuGenPage::OnDeleteitemTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pnmh;

	CItemInfo* pInfo = (CItemInfo*)pNMTreeView->itemOld.lParam;
	if (pInfo != NULL)
		delete pInfo;

	return 0;
}

LRESULT CMenuGenPage::OnKeyDownAccelKey(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HTREEITEM hSelect = m_tree.GetSelectedItem();
	if (hSelect != NULL)
	{
		CItemInfo* pInfo = (CItemInfo*)m_tree.GetItemData(hSelect);

		BOOL bShift = (GetKeyState(VK_SHIFT) < 0);
		BOOL bCtrl = (GetKeyState(VK_CONTROL) < 0);
		BOOL bAlt = (GetKeyState(VK_MENU) < 0);
		
		int nVirtKey = (int)wParam;

		pInfo->m_dwShiftState = (DWORD)(bShift + (bCtrl << 1) + (bAlt << 2));
		pInfo->m_dwKeyCode = nVirtKey;

		CString strText = GetAccelKeyName(pInfo->m_dwShiftState, pInfo->m_dwKeyCode);
		m_editAccelKey.SetWindowText(strText);
		m_editAccelKey.SetSel(strText.GetLength(), strText.GetLength());
	}

	return 0;
}

LRESULT CMenuGenPage::OnCharAccelKey(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
