// TagPickCtrl.cpp : Implementation of CTagPickCtrl

#include "stdafx.h"
#include "CxTagPick.h"
#include "TagPickCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTagPickCtrl

static TCHAR szTags[] = _T("Tags");
static TCHAR szTagGroup[] = _T("TagGroup");
static TCHAR szThisNode[] = _T("ThisNode");
bool CTagPickCtrl::m_bSortAscending = false;

LRESULT CTagPickCtrl::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DWORD dwStyle = BMPBTN_SHAREIMAGELISTS | BMPBTN_AUTO3D_DOUBLE/* | BMPBTN_AUTOSIZE*/;
	m_ImageList2.Create(IDB_BUTTON, 32, 1, RGB(0, 128, 128));

	m_btnShowGroupTree.SetBitmapButtonExtendedStyle(dwStyle);
	m_btnShowGroupTree.SetImageList((HIMAGELIST)m_ImageList2); 
    m_btnShowGroupTree.SetImages(0);
    m_btnShowGroupTree.SubclassWindow(GetDlgItem(IDC_SHOW_GROUP));
	m_btnShowGroupTree.SetSelected(m_bShowGroupTree);
	m_btnShowGroupTree.SetToolTipText(_T("显示标签组"));
//	m_winSplitter.SetSinglePaneMode(m_bShowGroupTree ? SPLIT_PANE_NONE : SPLIT_PANE_RIGHT);

	m_btnUseAssocGroup.SetBitmapButtonExtendedStyle(dwStyle);
	m_btnUseAssocGroup.SetImageList((HIMAGELIST)m_ImageList2); 
    m_btnUseAssocGroup.SetImages(1, -1, -1, 2);
    m_btnUseAssocGroup.SubclassWindow(GetDlgItem(IDC_USE_GROUP));
//	m_btnUseAssocGroup.SetSelected(TRUE);
	m_btnUseAssocGroup.SetToolTipText(_T("使用关联标签组"));

	m_btnViewListMode.SetBitmapButtonExtendedStyle(dwStyle);
	m_btnViewListMode.SetImageList((HIMAGELIST)m_ImageList2); 
    m_btnViewListMode.SetImages(3);
    m_btnViewListMode.SubclassWindow(GetDlgItem(IDC_VIEW_LISTMODE));
	m_btnViewListMode.SetToolTipText(_T("显示模式"));
	
	m_btnAddGroup.SetBitmapButtonExtendedStyle(dwStyle);
	m_btnAddGroup.SetImageList((HIMAGELIST)m_ImageList2); 
    m_btnAddGroup.SetImages(4);
    m_btnAddGroup.SubclassWindow(GetDlgItem(IDC_ADD_GROUP));
	m_btnAddGroup.SetToolTipText(_T("新建标签组"));

	m_btnAddTag.SetBitmapButtonExtendedStyle(dwStyle);
	m_btnAddTag.SetImageList((HIMAGELIST)m_ImageList2); 
    m_btnAddTag.SetImages(5);
    m_btnAddTag.SubclassWindow(GetDlgItem(IDC_ADD_TAG));
	m_btnAddTag.SetToolTipText(_T("新建标签"));
	
	m_btnAnalog.SetBitmapButtonExtendedStyle(dwStyle);
	m_btnAnalog.SetImageList((HIMAGELIST)m_ImageList2); 
    m_btnAnalog.SetImages(6);
	m_btnAnalog.SubclassWindow(GetDlgItem(IDC_ANALOG));
	m_btnAnalog.SetSelected(m_bShowAnalog);
	m_btnAnalog.SetToolTipText(_T("显示模拟量"));

	m_btnDigital.SetBitmapButtonExtendedStyle(dwStyle);
	m_btnDigital.SetImageList((HIMAGELIST)m_ImageList2); 
    m_btnDigital.SetImages(7);
	m_btnDigital.SubclassWindow(GetDlgItem(IDC_DIGITAL));
	m_btnDigital.SetSelected(m_bShowDigital);
	m_btnDigital.SetToolTipText(_T("显示开关量"));

	m_btnText.SetBitmapButtonExtendedStyle(dwStyle);
	m_btnText.SetImageList((HIMAGELIST)m_ImageList2); 
    m_btnText.SetImages(8);
	m_btnText.SubclassWindow(GetDlgItem(IDC_TEXT));
	m_btnText.SetSelected(m_bShowText);
	m_btnText.SetToolTipText(_T("显示字符串"));

	m_btnFilter.SetBitmapButtonExtendedStyle(dwStyle);
	m_btnFilter.SetImageList((HIMAGELIST)m_ImageList2); 
    m_btnFilter.SetImages(9, -1, -1, 10);
	m_btnFilter.SubclassWindow(GetDlgItem(IDC_FILTER));
	m_btnFilter.SetToolTipText(_T("过滤"));

 	m_tree.Attach(GetDlgItem(IDC_TAGGROUP));
	m_listTag.Attach(GetDlgItem(IDC_TAG));
	m_cmbProp.Attach(GetDlgItem(IDC_PROP));

	m_ImageList.Create(IDB_TAGTREE, 16, 1, RGB(255, 255, 255));
	m_tree.SetImageList((HIMAGELIST)m_ImageList, TVSIL_NORMAL);

	m_cmbFilterPattern.Attach(GetDlgItem(IDC_FILTER_PATTERN));
	UpdateFilterPatter();

	UpdateViewListMode();

	m_LargeImageList.Create(IDB_LARGE, 32, 1, RGB(0, 128, 128));
	m_listTag.SetImageList((HIMAGELIST)m_LargeImageList, LVSIL_NORMAL);
	m_SmallImageList.Create(IDB_SMALL, 16, 1, RGB(0, 128, 128));
	m_listTag.SetImageList((HIMAGELIST)m_SmallImageList, LVSIL_SMALL);

//	m_listTag.SetImageList()
	m_listTag.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 110, -1);
	m_listTag.InsertColumn(1, _T("描述"), LVCFMT_LEFT, 120, -1);
	m_listTag.InsertColumn(2, _T("类型"), LVCFMT_LEFT, 50, -1);
	m_listTag.InsertColumn(3, _T("主设备"), LVCFMT_LEFT, 60, -1);
	m_listTag.InsertColumn(4, _T("主地址"), LVCFMT_LEFT, 60, -1);

	m_nLastSortColumn = -1;

	CRect rcClient;
	GetClientRect(&rcClient);
	m_winSplitter.Create(m_hWnd, rcClient);

	CRect rcTree;
	m_tree.GetWindowRect(&rcTree);
	ScreenToClient(&rcTree);

	CRect rcList;
	m_listTag.GetWindowRect(&rcList);
	ScreenToClient(&rcList);

	CRect rcWindow(rcTree.TopLeft(), rcList.BottomRight());
	m_winSplitter.SetSplitterRect(&rcWindow);

	m_winSplitter.SetSplitterPanes(m_tree, m_listTag);
	m_winSplitter.SetSplitterPos(rcTree.Width());

	OnBrowsedChanged();
	
	return 1;
}

LRESULT CTagPickCtrl::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT rc = {0, 0, LOWORD(lParam), HIWORD(lParam)};

	CRect rcTemp;
	if (m_winSplitter.IsWindow())
	{
		CRect rect;
		m_winSplitter.GetSplitterRect(&rect);
		rcTemp = rect;
	//	rcTemp.top = 20;
	//	rcTemp.left = 20;
		rcTemp.right = rc.right /* + GetSystemMetrics(SM_CXEDGE)*/;
		rcTemp.bottom = rc.bottom;

		m_winSplitter.SetSplitterRect(&rcTemp);
//		m_winSplitter.SetWindowPos(NULL, &rcTemp, SWP_NOZORDER);
	}

	return 0;
}

void CTagPickCtrl::InitGroupTree(BOOL bAssoc)
{
	USES_CONVERSION;

	if (bAssoc)
	{
		m_pGroupNode = FindMatchNode(m_pRootNode, T2W(m_strAssocTagGroup));
		if (m_pGroupNode == NULL)
			return;

//		HTREEITEM hRoot = InsertNode(NULL, m_pGroupNode);
//		m_tree.Expand(hRoot);

		if (m_pGroupNode->GetNodeType() >= CNode::nodeNet)
		{
			if (m_pGroupNode->GetNodeType() == CNode::nodeNet &&
				(!((CNetNode *)m_pGroupNode)->m_bstrComputer || ((CNetNode *)m_pGroupNode)->m_bUseSACProxy))
				m_bThisNodeAssoc = TRUE;

			HTREEITEM hRoot = InsertNode(NULL, m_pGroupNode);
			m_tree.Expand(hRoot);
		}
		else
		{
			CSimpleArray<CNode *>& apChildNodes = m_pGroupNode->GetChildNodes();
			for (int i = 0; i < apChildNodes.GetSize(); i++)
			{
				CNode* pChildNode = apChildNodes[i];
				HTREEITEM hChildItem = InsertNode(NULL, pChildNode);
				
				if (pChildNode->GetNodeType() > CNode::nodeNet)
					ExpandNode(hChildItem);
			}	
		}
	}
	else
	{
		HTREEITEM hRoot = InsertNode(NULL, m_pRootNode);
		m_tree.Expand(hRoot);
	}
}

LRESULT CTagPickCtrl::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_pRootNode)
	{
		m_pRootNode->RemoveAll();
		delete m_pRootNode;
		m_pRootNode = NULL;
	}
	
	return 0;
}

HTREEITEM CTagPickCtrl::InsertNode(HTREEITEM hParent, CNode* pNode)
{
	USES_CONVERSION;

	TV_INSERTSTRUCT tvis;
	tvis.hParent = hParent;
	tvis.hInsertAfter = TVI_SORT;
	tvis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	if (pNode == m_pGroupNode)
	{
		if (!m_bDisableAssoc)
			tvis.item.pszText = _T("TagGroup");
		else if (m_bThisNodeAssoc)
			tvis.item.pszText = _T("标签");
		else 
			tvis.item.pszText = _T("");
	}
	else
	{
		tvis.item.pszText = OLE2T(pNode->m_bstrName);
	}

	BOOL bInsertVirtual = TRUE;

	switch (pNode->GetNodeType())
	{
	case CNode::nodeRoot:
		tvis.item.iImage = 0;
		tvis.item.iSelectedImage = 1;
		bInsertVirtual = TRUE;
		break;
	case CNode::nodeNet:
		if (pNode->m_bstrName == L"ThisNode")
		{
			if (m_bThisNodeAssoc)
			{
				tvis.item.iImage = 8;
				tvis.item.iSelectedImage = 9;
			}
			else
			{
				tvis.item.iImage = 2;
				tvis.item.iSelectedImage = 3;
			}
		}
		else
		{
			tvis.item.iImage = 4;
			tvis.item.iSelectedImage = 5;
		}
		tvis.hInsertAfter = TVI_LAST;
		bInsertVirtual = TRUE;
		break;
	case CNode::nodeTag:
		tvis.item.iImage = 6;
		tvis.item.iSelectedImage = 7;
		break;
	default:
		ATLASSERT(FALSE);
	}

	tvis.item.lParam = (long)pNode;

	HTREEITEM hItem = m_tree.InsertItem(&tvis);

	if (bInsertVirtual)
	{
		//因为网络节点没有打开，先假设有一个项目
		HTREEITEM hVirtuleItem = m_tree.InsertItem(_T("subitem"), hItem, TVI_LAST); 
		m_tree.SetItemData(hVirtuleItem, 0);
	}

	return hItem;
}

void CTagPickCtrl::ExpandNode(HTREEITEM hItem)
{
	//删除子节点
	DeleteAllChildItem(hItem);

	CNode* pNode = (CNode *)m_tree.GetItemData(hItem);

	CSimpleArray<CNode *>& apChildNodes = pNode->GetChildNodes();
	for (int i = 0; i < apChildNodes.GetSize(); i++)
	{
		CNode* pChildNode = apChildNodes[i];
		HTREEITEM hChildItem = InsertNode(hItem, pChildNode);

		if (pChildNode->GetNodeType() > CNode::nodeNet)
			ExpandNode(hChildItem);
	}	
}

LRESULT CTagPickCtrl::OnItemexpandingGroup(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pnmh;

	HTREEITEM hItem = pnmtv->itemNew.hItem;
	
	//节点已经被展开过返回，由子项目是否是正式节点判断
	HTREEITEM hChildItem = m_tree.GetChildItem(hItem);
	if (hChildItem != NULL && m_tree.GetItemData(hChildItem) != 0)
		return 0;

	ExpandNode(hItem);
	
	return 0;
}

CString CTagPickCtrl::GetTagGroup(HTREEITEM hItem)
{
	CString strTagGroup;

	//取节点名称
	TV_ITEM tvi;
	memset(&tvi, 0, sizeof(TV_ITEM));
	TCHAR szBuf[128];
	tvi.mask = TVIF_TEXT;
	tvi.pszText = szBuf;
	tvi.cchTextMax = 80;

	while (hItem)
	{
		HTREEITEM hParentItem = m_tree.GetParentItem(hItem);

		if (hParentItem == NULL && m_bThisNodeAssoc)
			break;

		tvi.hItem = hItem;
		m_tree.GetItem(&tvi);
		
		if (!strTagGroup.IsEmpty() && szBuf[0] != _T('\0'))
		{
			strTagGroup.Insert(0, _T('.'));
		}

		strTagGroup.Insert(0, szBuf);
		
		hItem = hParentItem;
		
//		strTagGroup.Insert(0, _T('.'));
	}

//	if (!strTagGroup.IsEmpty() && strTagGroup[0] == _T('.'))
//		strTagGroup.Delete(0);

	return strTagGroup;
}

////////////////////////////////////////////////////////////////////
//功 能：选择的标签组改变
////////////////////////////////////////////////////////////////////
LRESULT CTagPickCtrl::OnSelchangedGroup(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
//	if (m_bNoNotifyTreeSelect)
//	{		
//		m_bNoNotifyTreeSelect = FALSE;
//		m_tree.SelectItem(NULL);
//		return 0;
//	}
	
	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pnmh;

	HTREEITEM hItem = pnmtv->itemNew.hItem;
	DWORD dwNodeType = pnmtv->itemNew.lParam;

	if (hItem == NULL)
		return 0;

	m_listTag.DeleteAllItems();
	m_cmbProp.ResetContent();
	m_cmbProp.EnableWindow(FALSE);

	m_strTagGroup = GetTagGroup(hItem);

	m_strLastSelect = m_strTagGroup;

	Fire_Changed();

	LoadTagsOfGroup(m_strTagGroup);

	return 0;
}

void CTagPickCtrl::UpdateFilterPatter()
{
	m_cmbFilterPattern.ResetContent();

	for (int i = 0; i < m_arrFilterPattern.GetSize(); i++)
	{
		if (m_arrFilterPattern[i].IsEmpty())
			break;

		m_cmbFilterPattern.AddString(m_arrFilterPattern[i]);
	}
	m_cmbFilterPattern.AddString(_T(""));
}

HRESULT CTagPickCtrl::LoadTagsOfGroup(LPCTSTR szGroupName)
{
	USES_CONVERSION;

	m_listTag.DeleteAllItems();

	CString strGroupName = szGroupName;
	if (m_pGroupNode != NULL)
	{
		if (!m_bDisableAssoc)
		{
			strGroupName.Delete(0, 8);
			strGroupName.Insert(0, m_strAssocTagGroup);
		}
		else
		{
			strGroupName.Insert(0, _T('.'));
			strGroupName.Insert(0, m_strAssocTagGroup);
		}
	}
	
	CNode* pNode = FindMatchNode(m_pRootNode, T2W(strGroupName));
	if (pNode == NULL)
		return S_FALSE;

	CClientDC dc(m_listTag);
	HFONT hOldFont = dc.SelectFont(m_listTag.GetFont());

	CString str;
	CSize size;
// 	int nNameColumnWidth = 90;
	int nDescColumnWidth = 120;

	CComBSTR bstrPattern;
	m_cmbFilterPattern.GetWindowText((BSTR&)bstrPattern);

	CString strPattern = bstrPattern;
	if (!m_arrFilterPattern.Remove(strPattern))
		m_arrFilterPattern.RemoveAt(m_arrFilterPattern.GetSize() - 1);
	m_arrFilterPattern.Add(strPattern);
	UpdateFilterPatter();
				
	CSimpleArray<CTag *>& apTags = pNode->GetTags();

	for (int i = 0; i < apTags.GetSize(); i++)
	{
		CTag* pTag = apTags[i];

		if ((pTag->m_nTagType == TAG_ANALOG && !m_bShowAnalog) || 
			(pTag->m_nTagType == TAG_DIGITAL && !m_bShowDigital) || 
			(pTag->m_nTagType == TAG_TEXT && !m_bShowText))
			continue;

		if (!MatchPattern(pTag->m_bstrName, bstrPattern))
			continue;

		str = pTag->m_bstrName;
 		int iItem = m_listTag.InsertItem(i, str, 3);

		m_listTag.SetItemData(iItem, (DWORD)pTag);

		str = pTag->m_bstrDesc;
		m_listTag.SetItemText(iItem, 1, str);
		dc.GetTextExtent(str, -1, &size);
		if (nDescColumnWidth < size.cx + 16)
			nDescColumnWidth = size.cx + 16;

		switch (pTag->m_nTagType)
		{
		case TAG_ANALOG:
			str = _T("模拟量");
			break;
		case TAG_DIGITAL:
			str = _T("开关量");
			break;
		case TAG_TEXT:
			str = _T("字符串");
			break;
		default:
			ATLASSERT(FALSE);
			str = _T("");
		}

		m_listTag.SetItemText(iItem, 2, str);

		str = pTag->m_bstrDevice;
		m_listTag.SetItemText(iItem, 3, str);

		str = pTag->m_bstrAddress;
		m_listTag.SetItemText(iItem, 4, str);
	}
	
	dc.SelectFont(hOldFont);

	// 改变描述的宽度
//	m_listTag.SetColumnWidth(1, nDescColumnWidth);

	return S_OK;
}

HRESULT CTagPickCtrl::LoadFieldsOfTag(LPCTSTR szTagName)
{
	USES_CONVERSION;

	m_cmbProp.ResetContent();

	m_cmbProp.AddString(_T("(无)"));

	CString strTagName = szTagName;
	if (m_pGroupNode != NULL)
	{
		if (!m_bDisableAssoc)
		{
			strTagName.Delete(0, 8);
			strTagName.Insert(0, m_strAssocTagGroup);
		}
		else
		{
			strTagName.Insert(0, _T('.'));
			strTagName.Insert(0, m_strAssocTagGroup);
		}
	}

	CTag* pTag = FindMatchTag(m_pRootNode, T2W(strTagName));
	if (pTag == NULL)
		return S_FALSE;

	CSimpleArray<CComBSTR>& apProps = pTag->GetProps();
	for (int i = 0; i < apProps.GetSize(); i++)
	{
		CString str = apProps[i];
//		if (str.CompareNoCase(_T("Value")) == 0)
//			str += _T("(缺省)");
		m_cmbProp.AddString(str);
	}	

	return S_OK;
}

LRESULT CTagPickCtrl::OnItemchangedTag(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pnmh;
	
	if (pNMListView->uNewState & LVIS_FOCUSED)
	{
		int nItem = pNMListView->iItem;
		m_listTag.GetItemText(nItem, 0, m_strTag);

		if (!m_strTagGroup.IsEmpty())
			m_strLastSelect = m_strTagGroup + _T(".") + m_strTag;
		else
			m_strLastSelect = m_strTag;

		Fire_Changed();

		LoadFieldsOfTag(m_strLastSelect);
		m_cmbProp.SetCurSel(0);
		m_cmbProp.EnableWindow(TRUE);
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
//功能：选中属性列表项目
//////////////////////////////////////////////////////////////////////////////////
LRESULT CTagPickCtrl::OnSelchangeProp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{	
	m_cmbProp.GetLBText(m_cmbProp.GetCurSel(), m_strProp);
	int nFind = m_strProp.Find('(');
	if (nFind != -1)
		m_strProp = m_strProp.Left(nFind);

	if (!m_strTagGroup.IsEmpty())
		m_strLastSelect = m_strTagGroup + _T(".") + m_strTag;
	else
		m_strLastSelect = m_strTag;
	if (!m_strProp.IsEmpty())
		m_strLastSelect += _T(".");
	m_strLastSelect += m_strProp;

	Fire_Changed();

	return 0;
}

////////////////////////////////////////////////////////////////////
//功 能：删除所有子节点
////////////////////////////////////////////////////////////////////
void CTagPickCtrl::DeleteAllChildItem(HTREEITEM hItem)
{
	HTREEITEM hItemChild, hItemChildNext;
	ATLASSERT(hItem);

	hItemChild = m_tree.GetChildItem(hItem);
	while (hItemChild)
	{
		hItemChildNext = m_tree.GetNextSiblingItem(hItemChild);
		m_tree.DeleteItem(hItemChild);
		hItemChild = hItemChildNext;
	}	
}

LRESULT CTagPickCtrl::OnClickedShowGroup(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_bShowGroupTree = m_btnShowGroupTree.GetSelected();
	m_winSplitter.SetSinglePaneMode(m_bShowGroupTree ? SPLIT_PANE_NONE : SPLIT_PANE_RIGHT);
//	m_winSplitter.SetSinglePaneMode(m_winSplitter.GetSinglePaneMode() == SPLIT_PANE_RIGHT ? SPLIT_PANE_NONE : SPLIT_PANE_RIGHT);

	return 0;
}

LRESULT CTagPickCtrl::OnClickedUseGroup(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_bUseAssocGroup = m_btnUseAssocGroup.GetSelected();
	OnBrowsedChanged();

	return 0;
}

LRESULT CTagPickCtrl::OnClickedViewListmode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CMenu menu;
	CMenuHandle menuPopup;
	CRect rect;

	m_btnViewListMode.GetWindowRect(&rect);
	
	// the font popup is stored in a resource
	menu.LoadMenu(IDR_VIEW_POPUP);
	menuPopup = menu.GetSubMenu(0);

	UINT nID;

	switch (m_listTag.GetViewType())
	{
	case LVS_SMALLICON:
		nID = ID_VIEW_SMALLICONS;
		break;

	case LVS_ICON:
		nID = ID_VIEW_SMALLICONS;
		break;

	case LVS_LIST:
		nID = ID_VIEW_LIST;
		break;

	case LVS_REPORT:
		nID = ID_VIEW_DETAILS;
		break;
	}

	TCHAR szBuff[128];
	CMenuItemInfo mii;
	mii.fMask = MIIM_TYPE | MIIM_STATE | MIIM_SUBMENU;
	mii.dwTypeData = szBuff;
	mii.cch = sizeof(szBuff) / sizeof(TCHAR);
	menuPopup.GetMenuItemInfo(nID, FALSE, &mii);

	mii.fState |= MFS_CHECKED;
	mii.fMask |= MIIM_TYPE;
	mii.fType = MFT_RADIOCHECK;
	mii.dwTypeData = szBuff;
	menuPopup.SetMenuItemInfo(nID, FALSE, &mii);

	menuPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.bottom + 1, m_hWnd);
		
	return 0;
}

void CTagPickCtrl::UpdateViewListMode()
{
	DWORD dwType;

	switch (m_nViewListMode)
	{
	case 0:
		dwType = LVS_ICON;
		break;
	case 1:
		dwType = LVS_SMALLICON;
		break;
	case 2:
		dwType = LVS_LIST;
		break;
	case 3:
		dwType = LVS_REPORT;
		break;
	default:
		ATLASSERT(false);
	}

	if (m_listTag.GetViewType() != dwType)
		m_listTag.SetViewType(dwType);
}

LRESULT CTagPickCtrl::OnViewSmallIcons(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_nViewListMode = 1;
	UpdateViewListMode();

	return 0;
}

LRESULT CTagPickCtrl::OnViewLargeIcons(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_nViewListMode = 0;
	UpdateViewListMode();

	return 0;
}

LRESULT CTagPickCtrl::OnViewList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_nViewListMode = 2;
	UpdateViewListMode();

	return 0;
}

LRESULT CTagPickCtrl::OnViewDetails(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_nViewListMode = 3;
	UpdateViewListMode();

	return 0;
}

LRESULT CTagPickCtrl::OnClickedAnalog(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_bShowAnalog = m_btnAnalog.GetSelected();
	LoadTagsOfGroup(m_strTagGroup);

	return 0;
}

LRESULT CTagPickCtrl::OnClickedDigital(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_bShowDigital = m_btnDigital.GetSelected();
	LoadTagsOfGroup(m_strTagGroup);

	return 0;
}

LRESULT CTagPickCtrl::OnClickedText(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_bShowText = m_btnText.GetSelected();
	LoadTagsOfGroup(m_strTagGroup);

	return 0;
}

int CALLBACK CTagPickCtrl::CompareItems(CTag* pTag1, CTag* pTag2, LPARAM nCol)
{
	int nCmp = 0;
	
	switch (nCol)
	{
	case 0:
		nCmp = wcscmp(pTag1->m_bstrName, pTag2->m_bstrName);
		break;
	case 1:
		nCmp = wcscmp(pTag1->m_bstrDesc, pTag2->m_bstrDesc);
		break;
	case 2:
		{
			if (pTag1->m_nTagType > pTag2->m_nTagType)
				nCmp = 1;
			else if (pTag1->m_nTagType < pTag2->m_nTagType)
				nCmp = -1;
		}
		break;
	case 3:
		nCmp = wcscmp(pTag1->m_bstrDevice, pTag2->m_bstrDevice);
		break;
	case 4:
		nCmp = wcscmp(pTag1->m_bstrAddress, pTag2->m_bstrAddress);
		break;
	}
	
	if (!m_bSortAscending)
		nCmp = -nCmp;
	
	return nCmp;
}

LRESULT CTagPickCtrl::OnColumnclickTag(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW *)pnmh;

	if (m_nLastSortColumn != pNMListView->iSubItem)
		m_bSortAscending = true;
	else
		m_bSortAscending = !m_bSortAscending;
	m_nLastSortColumn = pNMListView->iSubItem;

	m_listTag.SortItems((PFNLVCOMPARE)CompareItems, pNMListView->iSubItem);
	
	return 0;
}

inline int ConvertCase(int c, BOOL bCaseSensitive)
{
	return bCaseSensitive ? c : toupper(c);
}

//BOOL MatchPattern(LPCTSTR String, LPCTSTR Pattern, BOOL bCaseSensitive)
//{
//	TCHAR c, p, l;
//	for (;;)
//	{
//	switch (p = ConvertCase(*Pattern++, bCaseSensitive))
//	{
//		case 0: // end of pattern
//		return *String ? FALSE : TRUE; // if end of string TRUE
//
//	case _T('*'):
//		while (*String)
//		{	
//			// match zero or more char
//			if (MatchPattern (String++, Pattern, bCaseSensitive))
//				return TRUE;
//		}
//		return MatchPattern (String, Pattern, bCaseSensitive);
//
//	case _T('?'):
//		if (*String++ == 0) // match any one char
//			return FALSE; // not end of string
//		break;
//
//	case _T('['):
//		// match char set
//		if ((c = ConvertCase(*String++, bCaseSensitive)) == 0)
//			return FALSE; // syntax
//		l = 0;
//		if (*Pattern == _T('!')) // match a char if NOT in set []
//		{
//			++Pattern;
//			while ((p = ConvertCase(*Pattern++, bCaseSensitive)) != _T('\0'))
//			{
//				if (p == _T(']')) // if end of char set, then
//					break; // no match found
//				if (p == _T('-'))
//				{ 
//					// check a range of chars?
//					p = ConvertCase(*Pattern, bCaseSensitive);
//					// get high limit of range
//					if (p == 0 || p == _T(']'))
//						return FALSE; // syntax
//					if (c >= l && c <= p)
//						return FALSE; // if in range, return FALSE
//				}
//
//				l = p;
//				if (c == p) // if char matches this element
//				return FALSE; // return false
//			}
//		}
//		else // match if char is in set []
//		{
//			while ((p = ConvertCase(*Pattern++, bCaseSensitive)) != _T('\0'))
//			{
//				if (p == _T(']')) // if end of char set, then
//					return FALSE; // no match found
//				if (p == _T('-'))
//				{ 
//					// check a range of chars?
//					p = ConvertCase(*Pattern, bCaseSensitive);
//					// get high limit of range
//					if (p == 0 || p == _T(']'))
//						return FALSE; // syntax
//					if (c >= l && c <= p)
//						break; // if in range, move on
//				}
//				l = p;
//				if (c == p) // if char matches this element
//				break; // move on
//			}
//			while (p && p != _T(']')) // got a match in char set
//				p = *Pattern++; // skip to end of set
//		}
//		break;
//
//	case _T('#'):
//		c = *String++;
//		if (!_istdigit(c))
//			return FALSE; // not a digit
//		break;
//
//	default:
//		c = ConvertCase(*String++, bCaseSensitive);
//		if (c != p) // check for exact char
//			return FALSE; // not a match
//		break;
//	}
//}


BOOL MatchPattern(LPCWSTR lpszString, LPCWSTR lpszPattern, BOOL bCaseSensitive)
{
	if (!lpszString)
		return FALSE;

	if (!lpszPattern || lpszPattern[0] == '\0')
		return TRUE;

	WCHAR c,p,l;
	for (;;)
	{
		switch (p = ConvertCase(*lpszPattern++, bCaseSensitive))
		{
		case 0: // end of pattern
			return *lpszString ? FALSE : TRUE;	// if end of string TRUE

		case WCHAR('*'):
			while (*lpszString)
			{	// match zero or more char
				if (MatchPattern(lpszString++, lpszPattern, bCaseSensitive))
					return TRUE;
			}
			return MatchPattern(lpszString, lpszPattern, bCaseSensitive);
			
		case WCHAR('?'):
			if (*lpszString++ == 0) //match anyone char 
				return FALSE;	//not end of string
			break;
			
		case WCHAR('['):
			if ((c = ConvertCase(*lpszString++, bCaseSensitive))==0) //match charset 
				return FALSE;	//syntax
			l = 0;
			if (*lpszPattern == WCHAR('!'))//match a char if NOT in set[]
			{
				++lpszPattern;

				while ((p = ConvertCase(*lpszPattern++, bCaseSensitive)) != WCHAR('\0'))
				{
					if (p == WCHAR(']'))//if end of charset,then 
						break;	//no match found
					
					if (p == WCHAR('-'))
					{	//check a range of chars?
						p = ConvertCase(*lpszPattern, bCaseSensitive); //get high limit of range
						if (p == 0 || p == WCHAR(']'))
							return FALSE;	//syntax 
						if ( c>= l && c <= p) 
							return FALSE;	//	if in range, return FALSE
					}

					l = p;
					if (c == p)//if char matches this element 
						return FALSE;	//return false
				}
			}
			else	//match if char is in set[]
			{
				while ((p = ConvertCase(*lpszPattern++, bCaseSensitive)) != WCHAR('\0'))
				{
					if (p == WCHAR(']'))	//if end of charset, then
						return FALSE;	//no match found
					
					if (p == WCHAR('-'))
					{	//check a range of chars?
						p = ConvertCase(*lpszPattern, bCaseSensitive); //get high limit of range
						if (p == 0 || p == WCHAR(']'))
							return	FALSE;//syntax

						if (c >= l && c <= p)
							break;	//if in range,move on
					}
					l = p;
					if (c == p)	//if char matches this element
						break;	//move on
				} 

				while (p && p != WCHAR(']')) //got a match in charset 
					p = *lpszPattern++;	//skip to end of set
			}

			break;

			case WCHAR('#'):
				c= *lpszString++;
				if (!iswdigit(c))
					return FALSE;	//not a digit

				break;

			default: 
				c = ConvertCase(*lpszString++, bCaseSensitive);
				if (c != p)	//check for exact char
					return FALSE;	//not a match 

				break;
		}
	}
}

LRESULT CTagPickCtrl::OnEditchangeFilterPattern(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled)
{
	CComBSTR bstrText;
	m_cmbFilterPattern.GetWindowText((BSTR&)bstrText);
	CString strText = bstrText;

	m_btnFilter.EnableWindow(!strText.IsEmpty());

	return 0;
}

LRESULT CTagPickCtrl::OnClickedFilter(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	LoadTagsOfGroup(m_strTagGroup);

	return 0;
}

STDMETHODIMP CTagPickCtrl::get_Value(BSTR *pVal)
{
	*pVal = m_strLastSelect.AllocSysString();

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_Value(BSTR newVal)
{
	m_strLastSelect = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_AssocGroup(BSTR *pVal)
{
	*pVal = m_strAssocTagGroup.AllocSysString();

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_AssocGroup(BSTR newVal)
{
	m_strAssocTagGroup = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_UseLocalSetup(BOOL *pVal)
{
	*pVal = m_bUseLocalSetup;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_UseLocalSetup(BOOL newVal)
{
	m_bUseLocalSetup = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_DisableAssoc(BOOL *pVal)
{
	*pVal = m_bDisableAssoc;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_DisableAssoc(BOOL newVal)
{
	m_bDisableAssoc = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_Browsed(BOOL *pVal)
{
	*pVal = m_bBrowsed;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_Browsed(BOOL newVal)
{
	if (m_bBrowsed != newVal)
	{
		m_bBrowsed = newVal;
		OnBrowsedChanged();
	}

	return S_OK;
}

void CTagPickCtrl::OnBrowsedChanged()
{
	if (m_bBrowsed)
	{
		if (m_pRootNode == NULL)
		{
			m_pRootNode = new CRootNode(m_bUseLocalSetup);

			if (m_strAssocTagGroup.Left(7) == _T("NetNode"))
			{
				CComBSTR bstrHostAddr;
				long lHostPort;
				CComBSTR bstrUserName;
				CComBSTR bstrPassword;

				int nIndex1 = m_strAssocTagGroup.Find(_T("HostAddr:"), 0);
				int nIndex2 = m_strAssocTagGroup.Find(_T(";"), nIndex1);
				bstrHostAddr = m_strAssocTagGroup.Mid(nIndex1 + 9, nIndex2 - nIndex1 - 9);

				nIndex1 = m_strAssocTagGroup.Find(_T("HostPort:"), 0);
				nIndex2 = m_strAssocTagGroup.Find(_T(";"), nIndex1);
				lHostPort = atoi(m_strAssocTagGroup.Mid(nIndex1 + 9, nIndex2 - nIndex1 - 9));

				nIndex1 = m_strAssocTagGroup.Find(_T("UserName:"), 0);
				nIndex2 = m_strAssocTagGroup.Find(_T(";"), nIndex1);
				bstrUserName = m_strAssocTagGroup.Mid(nIndex1 + 9, nIndex2 - nIndex1 - 9);

				nIndex1 = m_strAssocTagGroup.Find(_T("Password:"), 0);
				nIndex2 = m_strAssocTagGroup.Find(_T(";"), nIndex1);
				bstrPassword = m_strAssocTagGroup.Mid(nIndex1 + 9, nIndex2 - nIndex1 - 9);

				m_pRootNode->m_pOnlyNetNode = new CNetNode(L"ThisNode", bstrHostAddr, TRUE, lHostPort);
				m_pRootNode->m_pOnlyNetNode->m_bstrUserName = bstrUserName;
				m_pRootNode->m_pOnlyNetNode->m_bstrPassword = bstrPassword;
				m_strAssocTagGroup = _T("Tags.ThisNode");
			}
		}

		BOOL bAssoc = !m_strAssocTagGroup.IsEmpty();
		if (!bAssoc || m_bDisableAssoc)
			m_btnUseAssocGroup.EnableWindow(FALSE);
		else
			bAssoc = m_bUseAssocGroup; // 使用最近使用过的关联关系

		m_btnUseAssocGroup.SetSelected(bAssoc);

		InitGroupTree(bAssoc);

		DisplayMatchTag(m_strLastSelect);
	}
}

inline LPCTSTR NextToken(LPCTSTR szToken)
{
	while (*szToken != NULL)
	{
		if (*szToken == _T('.'))
			return szToken;

		szToken = CharNext(szToken);
	}

	return szToken;
}

HTREEITEM CTagPickCtrl::FindChildItemByName(HTREEITEM hItem, LPCTSTR szItem)
{
	//取节点名称
	TV_ITEM tvi;
	memset(&tvi, 0, sizeof(TV_ITEM));
	TCHAR szBuf[128];
	tvi.mask = TVIF_TEXT;
	tvi.pszText = szBuf;
	tvi.cchTextMax = 128;

	HTREEITEM hChildItem = (hItem == NULL ? m_tree.GetRootItem() : m_tree.GetChildItem(hItem));
	if (hChildItem != NULL && m_tree.GetItemData(hChildItem) == 0)
		ExpandNode(hItem);

	while (hChildItem)
	{
		tvi.hItem = hChildItem;
		m_tree.GetItem(&tvi);

		if (lstrcmpi(szItem, szBuf) == 0)
			return hChildItem;

		hChildItem = m_tree.GetNextSiblingItem(hChildItem);
	}

	return NULL;
}

int CTagPickCtrl::FindTagItemByName(LPCTSTR szItem)
{
	int nCount = m_listTag.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		CString strItem;
		m_listTag.GetItemText(i, 0, strItem);
		if (strItem.CompareNoCase(szItem) == 0)
			return i;
	}

	return -1;
}

int CTagPickCtrl::FindPropItemByName(LPCTSTR szItem)
{
	int nCount = m_cmbProp.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CString strItem;
		m_cmbProp.GetLBText(i, strItem);
		if (strItem.CompareNoCase(szItem) == 0)
			return i;
	}

	return -1;
}

void CTagPickCtrl::DisplayMatchTag(LPCTSTR szTagName)
{
	USES_CONVERSION;

	ATLASSERT(m_pRootNode != NULL);
	if (m_pRootNode == NULL)
		return;

	// 顺着标签组树根节点往下搜索
	CString strTagName = szTagName;
	if (m_pGroupNode != NULL && m_bDisableAssoc)
	{
		strTagName.Insert(0, _T('.'));
		strTagName.Insert(0, _T("标签"));
	}

	HTREEITEM hItem = NULL;
	HTREEITEM hLastItem = NULL;

	LPCTSTR szStart = strTagName;
	LPCTSTR szEnd = NextToken(szStart);
	while (1)
	{
		CString strToken(szStart, szEnd - szStart);

		hItem = FindChildItemByName(hItem, strToken);
		if (hItem == NULL)
			break;

		hLastItem = hItem;

		if (*szEnd == NULL)
			break;

		szStart = szEnd + 1;
		szEnd = NextToken(szStart);
	}

	if (hLastItem != NULL)
	{
		m_tree.Expand(hLastItem);
		m_tree.SelectItem(hLastItem);

		if (szEnd == NULL)
			return;

		CString strToken(szStart, szEnd - szStart);
		int nIndex = FindTagItemByName(strToken);
		if (nIndex != -1)
		{
			m_listTag.SelectItem(nIndex);

			szStart = szEnd + 1;
			szEnd = NextToken(szStart);
			if (szEnd == NULL)
				return;

			CString strToken(szStart, szEnd - szStart);
			nIndex = FindPropItemByName(strToken);
			m_cmbProp.SetCurSel(nIndex == -1 ? 0 : nIndex);
		}
	}
}



STDMETHODIMP CTagPickCtrl::get_ShowGroupTree(BOOL *pVal)
{
	*pVal = m_bShowGroupTree;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_ShowGroupTree(BOOL newVal)
{
	m_bShowGroupTree = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_UseAssocGroup(BOOL *pVal)
{
	*pVal = m_bUseAssocGroup;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_UseAssocGroup(BOOL newVal)
{
	m_bUseAssocGroup = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_ViewListMode(long *pVal)
{
	*pVal = m_nViewListMode;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_ViewListMode(long newVal)
{
	m_nViewListMode = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_ShowAnalog(BOOL *pVal)
{
	*pVal = m_bShowAnalog;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_ShowAnalog(BOOL newVal)
{
	m_bShowAnalog = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_ShowDigital(BOOL *pVal)
{
	*pVal = m_bShowDigital;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_ShowDigital(BOOL newVal)
{
	m_bShowDigital = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_ShowText(BOOL *pVal)
{
	*pVal = m_bShowText;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_ShowText(BOOL newVal)
{
	m_bShowText = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_FilterPattern1(BSTR *pVal)
{
	if (m_arrFilterPattern.GetSize() > 0)
		*pVal = m_arrFilterPattern[0].AllocSysString();
	
	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_FilterPattern1(BSTR newVal)
{
	if (m_arrFilterPattern.GetSize() > 0)
		m_arrFilterPattern[0] = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_FilterPattern2(BSTR *pVal)
{
	if (m_arrFilterPattern.GetSize() > 1)
		*pVal = m_arrFilterPattern[1].AllocSysString();

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_FilterPattern2(BSTR newVal)
{
	if (m_arrFilterPattern.GetSize() > 1)
		m_arrFilterPattern[1] = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_FilterPattern3(BSTR *pVal)
{
	if (m_arrFilterPattern.GetSize() > 2)
		*pVal = m_arrFilterPattern[2].AllocSysString();

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_FilterPattern3(BSTR newVal)
{
	if (m_arrFilterPattern.GetSize() > 2)
		m_arrFilterPattern[2] = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_FilterPattern4(BSTR *pVal)
{
	if (m_arrFilterPattern.GetSize() > 3)
		*pVal = m_arrFilterPattern[3].AllocSysString();

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_FilterPattern4(BSTR newVal)
{
	if (m_arrFilterPattern.GetSize() > 3)
		m_arrFilterPattern[3] = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_FilterPattern5(BSTR *pVal)
{
	if (m_arrFilterPattern.GetSize() > 4)
		*pVal = m_arrFilterPattern[4].AllocSysString();

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_FilterPattern5(BSTR newVal)
{
	if (m_arrFilterPattern.GetSize() > 4)
		m_arrFilterPattern[4] = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_FilterPattern6(BSTR *pVal)
{
	if (m_arrFilterPattern.GetSize() > 5)
		*pVal = m_arrFilterPattern[5].AllocSysString();

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_FilterPattern6(BSTR newVal)
{
	if (m_arrFilterPattern.GetSize() > 5)
		m_arrFilterPattern[5] = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_FilterPattern7(BSTR *pVal)
{
	if (m_arrFilterPattern.GetSize() > 6)
		*pVal = m_arrFilterPattern[6].AllocSysString();

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_FilterPattern7(BSTR newVal)
{
	if (m_arrFilterPattern.GetSize() > 6)
		m_arrFilterPattern[6] = newVal;

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::get_FilterPattern8(BSTR *pVal)
{
	if (m_arrFilterPattern.GetSize() > 7)
		*pVal = m_arrFilterPattern[7].AllocSysString();

	return S_OK;
}

STDMETHODIMP CTagPickCtrl::put_FilterPattern8(BSTR newVal)
{
	if (m_arrFilterPattern.GetSize() > 7)
		m_arrFilterPattern[7] = newVal;

	return S_OK;
}
