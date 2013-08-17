// TagGroupPickDlg.cpp: implementation of the CTagGroupPickDlgDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TagGroupPickDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CTagGroupPickDlg::InitGroupTree(BOOL bAssoc)
{
	if (bAssoc)
	{
		CNode* pNode = FindMatchNode(m_pRootNode, m_bstrAssocGroup);
		if (pNode == NULL)
			return;

		CSimpleArray<CNode *>& apChildNodes = pNode->GetChildNodes();
		for (int i = 0; i < apChildNodes.GetSize(); i++)
		{
			CNode* pChildNode = apChildNodes[i];
			HTREEITEM hChildItem = InsertNode(NULL, pChildNode);

	//		//优先展开本地节点
	//		if (pChildNode->GetNodeType() == CNode::nodeNet && pChildNode->m_bstrName == L"ThisNode")
	//			m_tree.Expand(hChildItem);

			if (pChildNode->GetNodeType() > CNode::nodeRoot)
				ExpandNode(hChildItem);
		}
	}
	else
	{
		HTREEITEM hRoot = InsertNode(NULL, m_pRootNode);
		m_tree.Expand(hRoot);
	}
}

HTREEITEM CTagGroupPickDlg::InsertNode(HTREEITEM hParent, CNode* pNode)
{
	USES_CONVERSION;

	TV_INSERTSTRUCT tvis;
	tvis.hParent = hParent;
	tvis.hInsertAfter = TVI_SORT;
	tvis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvis.item.pszText = OLE2T(pNode->m_bstrName);

	BOOL bInsertVirtual = FALSE;

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
			tvis.item.iImage = 2;
			tvis.item.iSelectedImage = 3;
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

////////////////////////////////////////////////////////////////////
//功 能：删除所有子节点
////////////////////////////////////////////////////////////////////
void CTagGroupPickDlg::DeleteAllChildItem(HTREEITEM hItem)
{
	HTREEITEM hItemChild, hItemChildNext ;
	ATLASSERT(hItem);

	hItemChild = m_tree.GetChildItem(hItem);
	while (hItemChild)
	{
		hItemChildNext = m_tree.GetNextSiblingItem(hItemChild);
		m_tree.DeleteItem(hItemChild);
		hItemChild = hItemChildNext;
	}	
}

void CTagGroupPickDlg::ExpandNode(HTREEITEM hItem)
{
	//删除子节点
	DeleteAllChildItem(hItem);

	CNode* pNode = (CNode *)m_tree.GetItemData(hItem);

	CSimpleArray<CNode *>& apChildNodes = pNode->GetChildNodes();
	for (int i = 0; i < apChildNodes.GetSize(); i++)
	{
		CNode* pChildNode = apChildNodes[i];
		HTREEITEM hChildItem = InsertNode(hItem, pChildNode);

//		//优先展开本地节点
//		if (pChildNode->GetNodeType() == CNode::nodeNet && pChildNode->m_bstrName == L"ThisNode")
//			m_tree.Expand(hChildItem);


		if (pChildNode->GetNodeType() > CNode::nodeNet)
			ExpandNode(hChildItem);
	}	
}

LRESULT CTagGroupPickDlg::OnItemexpandingGroup(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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

CString CTagGroupPickDlg::GetTagGroup(HTREEITEM hItem)
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
		tvi.hItem = hItem;
		m_tree.GetItem(&tvi);
		
		strTagGroup.Insert(0, szBuf);
		hItem = m_tree.GetParentItem(hItem);
		if (hItem)
			strTagGroup.Insert(0, _T('.'));
	}
	
	return strTagGroup;
}

LRESULT CTagGroupPickDlg::OnSelchangedGroup(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pnmh;

	HTREEITEM hItem = pnmtv->itemNew.hItem;

	m_strPickGroup = GetTagGroup(hItem);
	m_editSel.SetWindowText(m_strPickGroup);

	return 0;
}

LRESULT CTagGroupPickDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_tree.Attach(GetDlgItem(IDC_GROUP));
	m_btnAssoc.Attach(GetDlgItem(IDC_ASSOC));
	m_editSel.Attach(GetDlgItem(IDC_SELECT));
	m_editSel.SetWindowText(m_strPickGroup);
	m_editSel.SetSel(0, -1);
	
	m_ImageList.Create(IDB_TAGTREE, 16, 1, RGB(255, 255, 255));
	m_tree.SetImageList((HIMAGELIST)m_ImageList, TVSIL_NORMAL);

	if (m_pRootNode == NULL)
		m_pRootNode = new CRootNode;

	BOOL bAssoc = (m_bstrAssocGroup != NULL && !(m_bstrAssocGroup == L""));
	if (!bAssoc || m_bDisableAssoc)
		m_btnAssoc.EnableWindow(FALSE);
	m_btnAssoc.SetCheck(bAssoc ? BST_CHECKED : BST_UNCHECKED);
	UpdateGroupTree();
		
	return 1;
}

void CTagGroupPickDlg::UpdateGroupTree()
{
	//删除所有根节点
	HTREEITEM hRoot, hRootNext;
	hRoot = m_tree.GetRootItem();
	while (hRoot)
	{
		hRootNext = m_tree.GetNextSiblingItem(hRoot);
		m_tree.DeleteItem(hRoot);
		hRoot = hRootNext;
	}	

	BOOL bAssoc = m_btnAssoc.GetCheck() == BST_CHECKED;
	InitGroupTree(bAssoc);
}

LRESULT CTagGroupPickDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nLen = m_editSel.GetWindowTextLength();
	m_editSel.GetWindowText(m_strPickGroup.GetBufferSetLength(nLen), nLen+1);
	m_strPickGroup.ReleaseBuffer();

	EndDialog(wID);
	return 0;	
}

LRESULT CTagGroupPickDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CTagGroupPickDlg::OnFinalMessage(HWND hWnd)
{
	if (m_pRootNode)
	{
		m_pRootNode->RemoveAll();
		delete m_pRootNode;
		m_pRootNode = NULL;
	}
}

LRESULT CTagGroupPickDlg::OnClickedAssoc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdateGroupTree();

	return 0;
}

