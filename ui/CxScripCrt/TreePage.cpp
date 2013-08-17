// TreePage.cpp: implementation of the CTreePageBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxScripCrt.h"
#include "TreePage.h"

STDMETHODIMP CTreePageBase::get_Caption(BSTR *pVal)
{
	*pVal = m_strCaption.AllocSysString();

	return S_OK;
}

STDMETHODIMP CTreePageBase::put_Caption(BSTR newVal)
{
	m_strCaption = newVal;

	return S_OK;
}

STDMETHODIMP CTreePageBase::ExpandNode(OLE_HANDLE hItem)
{
	return E_NOTIMPL;
}

STDMETHODIMP CTreePageBase::InsertNode(OLE_HANDLE hParentItem, BSTR bstrName, 
	IDispatch* pObject, IUnknown* punkEumProp, OLE_HANDLE* phItem)
{
	if (m_pRootNode == NULL)
		m_pRootNode = new CTreeNode;

	CTreeNode* pParentNode = (CTreeNode *)hParentItem;
	if (pParentNode == NULL)
		pParentNode = m_pRootNode;
	
	CTreeNode* pNode = new CTreeNode(pParentNode);
	pNode->m_strName = bstrName;
	pNode->m_spObject = pObject;
	pNode->m_spunkEumProp = punkEumProp;
	pParentNode->AddChildNode(pNode);

	*phItem = (OLE_HANDLE)pNode;

//
//	USES_CONVERSION;
//
//	HTREEITEM hItem = InsertItem(OLE2T(bstrName), 0, 0, (HTREEITEM)hParentItem, TVI_SORT);
//
//	int i;
//	CStringArray arrProperties; 
//	CStringArray arrMethods;
//
//	if (GetObjectMembers(pObject, (ICxEnumExtendProperty *)punkEumProp, arrProperties, arrMethods))
//	{
//		for (i = 0; i < (int)arrProperties.GetCount(); i++)
//			InsertItem(arrProperties[i], 1, 1, hItem, TVI_SORT);
//		
//		for (i = 0; i < (int)arrMethods.GetCount(); i++)
//			InsertItem(arrMethods[i], 2, 2, hItem, TVI_SORT);
//	}
//
//	*phItem = (OLE_HANDLE)hItem;

	return S_OK;
}

HRESULT CTreePageBase::LoadProps(HTREEITEM hParentItem, IDispatch *pIObject)
{
	USES_CONVERSION;
	
	CComPtr<ITypeInfo> spTypeInfo;	
	HRESULT hr = pIObject->GetTypeInfo(0, LOCALE_SYSTEM_DEFAULT, &spTypeInfo);	
	if (FAILED(hr)) 
		return hr;
	if (spTypeInfo == NULL) 
		return E_UNEXPECTED;

	TYPEATTR* pta = 0;
	hr = spTypeInfo->GetTypeAttr(&pta);
	if (FAILED(hr)) 
		return hr;
	if (!pta) 
		return E_UNEXPECTED;
	for (int i = 0; i < pta->cFuncs; i++)
	{
		FUNCDESC* pfd = 0;
		spTypeInfo->GetFuncDesc(i, &pfd);

		if (pfd && (pfd->invkind == INVOKE_PROPERTYGET/* || pfd->invkind == INVOKE_PROPERTYPUT || pfd->invkind == INVOKE_PROPERTYPUTREF*/)
			&& ((pfd->wFuncFlags & (FUNCFLAG_FRESTRICTED | FUNCFLAG_FHIDDEN | FUNCFLAG_FNONBROWSABLE)) == 0))
		{	
			// 下列语句会使部分控件属性无法显示
//			if(pfd->cParams != 0)
//				continue;

			CComBSTR bstrName;
			spTypeInfo->GetDocumentation(pfd->memid, &bstrName, NULL, 0, 0);
//			m_list.AddString(W2T(bstrName));
		}
		spTypeInfo->ReleaseFuncDesc(pfd);			
	} 
	
	for (i = 0; i < pta->cVars; i++)
	{
		VARDESC* pvd = NULL;
		spTypeInfo->GetVarDesc(i, &pvd);
		
		if(((pvd->wVarFlags & VARFLAG_FREADONLY) == 0) && 
			((pvd->wVarFlags & (VARFLAG_FHIDDEN | VARFLAG_FNONBROWSABLE)) == 0))
		{
			CComBSTR bstrName; 
			CComBSTR bstrDocString;
			spTypeInfo->GetDocumentation(pvd->memid, &bstrName, &bstrDocString, 0, 0);

//			m_list.AddString(W2T(bstrName));
		}
		spTypeInfo->ReleaseVarDesc(pvd);	
	}

	spTypeInfo->ReleaseTypeAttr(pta);

	return S_OK;
}

HRESULT CTreePageBase::LoadProps(HTREEITEM hParentItem, ICxEnumExtendProperty* pEumProp)
{
	USES_CONVERSION;
	LONG nIndex;
	HRESULT hr;

	pEumProp->Reset();
	while (S_FALSE != pEumProp->Next(&nIndex))
	{
		CComBSTR bstrName; 
		hr = pEumProp->GetName(&bstrName);
		if (FAILED(hr))
		{
			return hr;
		}

//		m_list.AddString(W2T(bstrName));
	}

	return S_OK;
}

STDMETHODIMP CTreePageBase::Populate()
{
	if (!::IsWindow(m_hWnd))
		return S_FALSE;

	m_tree.DeleteAllItems();

	LoadObjectItems(NULL);

	m_HasNodeExpanded = FALSE;

	return S_OK;
}

STDMETHODIMP CTreePageBase::SetSymbolTreeSite(IUnknown *pSite)
{
	m_pSymbolTreeSite = (ICxSymbolTreeSite *)pSite;

	return S_OK;
}

STDMETHODIMP CTreePageBase::SetDefaultNameSpace(BSTR bstrName)
{
	m_strDefaultNameSpace = bstrName;
	return S_OK;
}

STDMETHODIMP CTreePageBase::HasNodeExpanded()
{
	return m_HasNodeExpanded ? S_OK : S_FALSE;
}

BOOL CTreePageBase::GetObjectMembers(ITypeInfo* pTypeInfo, 
		CStringArray& arrProperties, CStringArray& arrMethods)
{
	USES_CONVERSION;
	BOOL bResult = FALSE;

	TYPEATTR* pTypeAttr = NULL;
	HRESULT hr = pTypeInfo->GetTypeAttr(&pTypeAttr);
	if (FAILED(hr))
		return FALSE;

	for (int i = 0; i < pTypeAttr->cFuncs; i++)
	{
		FUNCDESC* pfd = 0;
		pTypeInfo->GetFuncDesc(i, &pfd);

		if ((pfd->wFuncFlags & (FUNCFLAG_FRESTRICTED | 
			FUNCFLAG_FHIDDEN/* | FUNCFLAG_FNONBROWSABLE*/)) == 0)
		{
			CComBSTR bstrName;
			pTypeInfo->GetDocumentation(pfd->memid, &bstrName, NULL, NULL, NULL);
			CString str = OLE2T(bstrName);
			if (pfd->invkind == INVOKE_FUNC)
			{
				arrMethods.Add(OLE2T(bstrName));
				bResult = TRUE;
			}
			else if (pfd->invkind == INVOKE_PROPERTYGET)
			{
				arrProperties.Add(str);
				bResult = TRUE;
			}
		}

		pTypeInfo->ReleaseFuncDesc(pfd);
	}


	for (i = 0; i < pTypeAttr->cVars; i++)
	{
		VARDESC* pvd;
		pTypeInfo->GetVarDesc(i, &pvd);
		if (pvd->varkind == VAR_DISPATCH && (pvd->wVarFlags & (VARFLAG_FHIDDEN | 
			VARFLAG_FRESTRICTED/* | VARFLAG_FNONBROWSABLE*/)) == 0   )
		{
			CComBSTR bstrName;
			pTypeInfo->GetDocumentation(pvd->memid, &bstrName, NULL, NULL, NULL);
			CString str = OLE2T(bstrName);
			arrProperties.Add(str);
			bResult = TRUE;
		}
		pTypeInfo->ReleaseVarDesc(pvd);
	}

	pTypeInfo->ReleaseTypeAttr(pTypeAttr);

	return bResult;
}

BOOL CTreePageBase::GetObjectMembers(IDispatch* pDisp, ICxEnumExtendProperty* pEnumProp,
		CStringArray& arrProperties, CStringArray& arrMethods)
{
	USES_CONVERSION;
	HRESULT hr;

	UINT nInfoCount = 0;
	pDisp->GetTypeInfoCount(&nInfoCount);

	for (UINT iInfo = 0; iInfo < nInfoCount; iInfo++)
	{
		CComPtr<ITypeInfo> spTypeInfo;	
		hr = pDisp->GetTypeInfo(0, LOCALE_SYSTEM_DEFAULT, &spTypeInfo);
		if (FAILED(hr)) 
			return FALSE;
		
		GetObjectMembers(spTypeInfo, arrProperties, arrMethods);
	}

	if (pEnumProp)
	{
		LONG nIndex;
		pEnumProp->Reset();
		while (S_FALSE != pEnumProp->Next(&nIndex))
		{
			CComBSTR bstrName; 
			hr = pEnumProp->GetName(&bstrName);
			if (FAILED(hr))
				break;
				
			arrProperties.Add(W2T(bstrName));
		}
	}

	return TRUE;
}

BOOL CTreePageBase::GetObjectMembers(LPCTSTR lpszObject, 
	CStringArray& arrObjects, CStringArray& arrProperties, CStringArray& arrMethods)
{
	USES_CONVERSION;

	CComPtr<IEnumString> spEnumObjects;
	CComPtr<IEnumString> spEnumProperties;
	CComPtr<IEnumString> spEnumMethods;
	
	HRESULT hr = m_pSymbolTreeSite->GetObjectMembers(T2CW(lpszObject), 
		&spEnumObjects, &spEnumProperties, &spEnumMethods);
	if (hr == S_OK)
	{
		LPOLESTR pszItem;
		while (spEnumObjects->Next(1, &pszItem, NULL) == S_OK)
		{
			arrObjects.Add(OLE2T(pszItem));
			CoTaskMemFree(pszItem);
		}
		while (spEnumProperties->Next(1, &pszItem, NULL) == S_OK)
		{
			arrProperties.Add(OLE2T(pszItem));
			CoTaskMemFree(pszItem);
		}
		while (spEnumMethods->Next(1, &pszItem, NULL) == S_OK)
		{
			arrMethods.Add(OLE2T(pszItem));
			CoTaskMemFree(pszItem);
		}

		return TRUE;
	}

	return FALSE;
}

void CTreePageBase::DeleteAllChildItem(HTREEITEM hItem)
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

HTREEITEM CTreePageBase::FindChildItemByName(HTREEITEM hItem, LPCTSTR szItem)
{
	//取节点名称
	TV_ITEM tvi;
	memset(&tvi, 0, sizeof(TV_ITEM));
	TCHAR szBuf[128];
	tvi.mask = TVIF_TEXT;
	tvi.pszText = szBuf;
	tvi.cchTextMax = 128;

	hItem = (hItem == NULL ? m_tree.GetRootItem() : m_tree.GetChildItem(hItem));
	while (hItem)
	{
		tvi.hItem = hItem;
		m_tree.GetItem(&tvi);

		if (lstrcmpi(szItem, szBuf) == 0)
			return hItem;

		hItem = m_tree.GetNextSiblingItem(hItem);
	}

	return NULL;
}

void CTreePageBase::InsertNode(HTREEITEM hParentItem, CTreeNode* pNode)
{
	int i;
	HTREEITEM hItem = m_tree.InsertItem(pNode->m_strName, 0, 0, (HTREEITEM)hParentItem, TVI_SORT);
	m_tree.SetItemData(hItem, DWORD_PTR(pNode));

	if (!m_bWithList)
	{
		CStringArray arrProperties; 
		CStringArray arrMethods;

		if (GetObjectMembers(pNode->m_spObject, (ICxEnumExtendProperty *)pNode->m_spunkEumProp.p, 
			arrProperties, arrMethods))
		{
			for (i = 0; i < (int)arrProperties.GetCount(); i++)
				m_tree.InsertItem(arrProperties[i], 1, 1, hItem, TVI_SORT);
			
			for (i = 0; i < (int)arrMethods.GetCount(); i++)
				m_tree.InsertItem(arrMethods[i], 2, 2, hItem, TVI_SORT);
		}
	}

	for (i = 0; i < pNode->GetChildNodeCount(); i++)
	{
		CTreeNode* pChildNode = pNode->GetChildNode(i);
		InsertNode(hItem, pChildNode);
	}
}

void CTreePageBase::LoadObjectItems(HTREEITEM hParentItem)
{
	int i;
	CStringArray arrObjects; 
	CStringArray arrProperties; 
	CStringArray arrMethods;

	if (m_pRootNode != NULL)
	{
		CTreeNode* pParentNode = m_pRootNode;
		for (int i = 0; i < pParentNode->GetChildNodeCount(); i++)
		{
			CTreeNode* pNode = pParentNode->GetChildNode(i);
			InsertNode(hParentItem, pNode);
		}

		if (m_pPopulatedRootNode)
			delete m_pPopulatedRootNode;
		m_pPopulatedRootNode = m_pRootNode;
		m_pRootNode = NULL;
	}

	if (m_pSymbolTreeSite != NULL)
	{
		if (hParentItem != NULL)
			DeleteAllChildItem(hParentItem);

		GetObjectMembers(GetQualifyNameSpace(hParentItem), arrObjects, arrProperties, arrMethods);

		if (m_bWithList)
		{
			for (i = (int)arrObjects.GetCount() - 1; i >= 0 ; i--)
				m_tree.InsertItem(arrObjects[i], 0, 0, hParentItem, TVI_SORT/*TVI_FIRST*/);

			m_list.DeleteAllItems();

			for (i = 0; i < (int)arrProperties.GetCount(); i++)
				m_list.InsertItem(0, arrProperties[i], 1);
	
			for (i = 0; i < (int)arrMethods.GetCount(); i++)
				m_list.InsertItem(0, arrMethods[i], 2);
		}
		else
		{
			for (i = 0; i < (int)arrProperties.GetCount(); i++)
				m_tree.InsertItem(arrProperties[i], 1, 1, hParentItem, TVI_SORT);
	
			for (i = 0; i < (int)arrMethods.GetCount(); i++)
				m_tree.InsertItem(arrMethods[i], 2, 2, hParentItem, TVI_SORT);

			for (i = (int)arrObjects.GetCount() - 1; i >= 0 ; i--)
			{
				HTREEITEM hItem = m_tree.InsertItem(arrObjects[i], 0, 0, hParentItem, TVI_SORT/*TVI_FIRST*/);
				m_tree.InsertItem(_T("VirtualChild"), hItem, TVI_LAST);
			}
		}

		CString strText = GetQualifyNameSpace(hParentItem);

		if (!m_strDefaultNameSpace.IsEmpty() && strText.CompareNoCase(m_strDefaultNameSpace.Left(strText.GetLength())) == 0)
		{
			CString strT = m_strDefaultNameSpace.Mid(strText.GetLength());
			if ((!strT.IsEmpty() && strT[0] == _T('.')) || strText.IsEmpty())
			{
				if (strT[0] == _T('.'))
					strT.Remove(0);
				int nFind = strT.Find(_T('.'));
				if (nFind != -1)
					strT = strT.Left(nFind);

				HTREEITEM hItem = FindChildItemByName(hParentItem, strT);
				m_tree.SetItemState(hItem, TVIS_BOLD, TVIS_BOLD);
			}
		}
	}
	else if (m_pPopulatedRootNode != NULL)
	{
		CTreeNode* pNode = (CTreeNode*)m_tree.GetItemData(hParentItem);

		if (pNode && m_bWithList)
		{
			if (GetObjectMembers(pNode->m_spObject, (ICxEnumExtendProperty *)pNode->m_spunkEumProp.p, 
				arrProperties, arrMethods))
			{
				m_list.DeleteAllItems();

				for (i = 0; i < (int)arrProperties.GetCount(); i++)
					m_list.InsertItem(i, arrProperties[i], 1);
		
				for (i = 0; i < (int)arrMethods.GetCount(); i++)
					m_list.InsertItem(i, arrMethods[i], 2);
			}
		}
	}
}

CString CTreePageBase::GetQualifyNameSpace(HTREEITEM hItem)
{
	CString strRet;

	//取节点名称
	TV_ITEM tvi;
	memset(&tvi, 0, sizeof(TV_ITEM));
	TCHAR szBuf[256];
	tvi.mask = TVIF_TEXT;
	tvi.pszText = szBuf;
	tvi.cchTextMax = 255;

	while (hItem)
	{
		tvi.hItem = hItem;
		m_tree.GetItem(&tvi);
		strRet.Insert(0, szBuf);

		if (!m_bObjectHiberarchy)
			break;

		hItem = m_tree.GetParentItem(hItem);
		if (hItem)
			strRet.Insert(0, _T('.'));
	}

	return strRet;
}

void CTreePageBase::TruncateNameSpace(CString& strText)
{
	if (!m_strDefaultNameSpace.IsEmpty() && m_strDefaultNameSpace.CompareNoCase(strText.Left(m_strDefaultNameSpace.GetLength())) == 0)
	{
		strText = strText.Mid(m_strDefaultNameSpace.GetLength());
		if (!strText.IsEmpty() && strText[0] == _T('.'))
			strText = strText.Mid(1);
	}
}

CString CTreePageBase::GetSelectNameSpace()
{
	return m_strSelectNameSpace;
}

LRESULT CTreePageBase::OnItemexpandingNode(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pnmh;

	HTREEITEM hItem = pnmtv->itemNew.hItem;
	LoadObjectItems(hItem);

	m_HasNodeExpanded = TRUE;
	
	return 0;
}

LRESULT CTreePageBase::OnDblclkSymbol(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if (!m_bDblClickSelect)
		return 0;

	m_strSelectNameSpace.Empty();

	if (idCtrl == IDC_TREE)
	{
		UINT nFlags;
		CPoint pt;
		GetCursorPos(&pt);
		m_tree.ScreenToClient(&pt);

		HTREEITEM hSelItem = m_tree.HitTest(pt, &nFlags);
		if (hSelItem != NULL)
			m_strSelectNameSpace = GetQualifyNameSpace(hSelItem);
	}
	else
	{
		HTREEITEM hSelItem = m_tree.GetSelectedItem();
		if (hSelItem != NULL)
		{
			m_strSelectNameSpace = GetQualifyNameSpace(hSelItem);

			int nIndex = m_list.GetSelectedIndex();
			if (nIndex != -1)
			{
				CString strTemp;
				m_list.GetItemText(nIndex, 0, strTemp);
				if (!m_strSelectNameSpace.IsEmpty())
					m_strSelectNameSpace += _T(".");
				m_strSelectNameSpace += strTemp;
			}
		}
	}

	TruncateNameSpace(m_strSelectNameSpace);

	::PostMessage(m_hNotifyWindow ? m_hNotifyWindow : GetParent(), WM_SYMBOL_SELECT, GetDlgCtrlID(), (LPARAM)&m_hWnd);

	return 0;
}

LRESULT CTreePageBase::OnSelchangedTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pnmh;

	HTREEITEM hItem = pnmtv->itemNew.hItem;

	if (m_bWithList)
	{
		m_strSelectNameSpace = GetQualifyNameSpace(hItem);
		TruncateNameSpace(m_strSelectNameSpace);

		::PostMessage(m_hNotifyWindow ? m_hNotifyWindow : GetParent(), WM_SYMBOL_SELECT, GetDlgCtrlID(), (LPARAM)&m_hWnd);

		LoadObjectItems(hItem);
	}

	return 0;
}

LRESULT CTreePageBase::OnItemchangedList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pnmh;
	
	if (pNMListView->uNewState & LVIS_FOCUSED)
	{
		m_strSelectNameSpace.Empty();

		HTREEITEM hSelItem = m_tree.GetSelectedItem();
		if (hSelItem != NULL)
		{
			m_strSelectNameSpace = GetQualifyNameSpace(hSelItem);

			int nIndex = m_list.GetSelectedIndex();
			if (nIndex != -1)
			{
				CString strTemp;
				m_list.GetItemText(nIndex, 0, strTemp);
				if (!m_strSelectNameSpace.IsEmpty())
					m_strSelectNameSpace += _T(".");
				m_strSelectNameSpace += strTemp;
			}
		}

		TruncateNameSpace(m_strSelectNameSpace);

		::PostMessage(m_hNotifyWindow ? m_hNotifyWindow : GetParent(), WM_SYMBOL_SELECT, GetDlgCtrlID(), (LPARAM)&m_hWnd);
	}
	
	return 0;
}

HWND CTreePageBase::Create(HWND hWndParent)
{
	return CDialogImpl<CTreePageBase>::Create(hWndParent, NULL);
}

LRESULT CTreePageBase::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_tree.Attach(GetDlgItem(IDC_TREE));
	m_list.Attach(GetDlgItem(IDC_LIST));

	m_list.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 240, -1);
	
	CRect rcClient;
	GetClientRect(&rcClient);
	m_winSplitter.Create(m_hWnd, rcClient, "", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP, WS_EX_CONTROLPARENT);

	CRect rcTree;
	m_tree.GetWindowRect(&rcTree);
	ScreenToClient(&rcTree);

	CRect rcList;
	m_list.GetWindowRect(&rcList);
	ScreenToClient(&rcList);

	CRect rcWindow(rcTree.TopLeft(), rcList.BottomRight());
	m_winSplitter.SetSplitterRect(&rcWindow);

	m_winSplitter.SetSplitterPanes(m_tree, m_list);
	m_winSplitter.SetSplitterPos(rcTree.Width());

	if (!m_bWithList)
		m_winSplitter.SetSinglePaneMode(0);
		
	return 0;
}

LRESULT CTreePageBase::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT rc = {0, 0, LOWORD(lParam), HIWORD(lParam)};

	if (m_winSplitter.IsWindow())
	{
	//	rc.left = 10;
		m_winSplitter.SetWindowPos(NULL, rc.left, rc.top,
			rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE | SWP_NOZORDER);
		m_winSplitter.SetSplitterRect(&rc);
	}

	return 0;
}

void CTreePageBase::OnInit()
{
	Populate();
}
