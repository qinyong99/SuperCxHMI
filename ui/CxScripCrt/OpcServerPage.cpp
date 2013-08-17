// OPCServerPage.cpp: implementation of the COPCServerPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxScripCrt.h"
#include "OPCServerPage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void COPCServerPage::OnInit()
{
	USES_CONVERSION;

	m_ImageList.Create(IDB_TAGTREE, 16, 1, RGB(255, 255, 255));
	m_tree.SetImageList((HIMAGELIST)m_ImageList, TVSIL_NORMAL);

	LoadOPCServers();

	CTreePage::OnInit();
}

void GetMuduleDirectory(LPTSTR szPath)
{
	TCHAR szBuf[_MAX_PATH];
	::GetModuleFileName(_Module.m_hInst, szBuf, _MAX_PATH);
	
	LPTSTR lpszTemp = (LPTSTR)szBuf;
	for (LPCTSTR lpsz = szBuf; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
	{
		// remember last directory/drive separator
		if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
			lpszTemp = (LPTSTR)_tcsinc(lpsz);
	}
	
	*lpszTemp = 0;
	
	lstrcpy(szPath, szBuf);
}

void COPCServerPage::LoadOPCServers()
{
	TCHAR szIniFile[_MAX_PATH];
	GetMuduleDirectory(szIniFile);
	lstrcat(szIniFile, _T("OPCDataSource.ini")); 
	
	TCHAR szBuf[1024];
	DWORD dwSize = GetPrivateProfileSectionNames(szBuf, 1024, szIniFile);
	if (dwSize == 0)
		return;
	
	LPTSTR lpszSection = (LPTSTR)szBuf;
	LPTSTR lpszTemp = (LPTSTR)szBuf; 
	while (1)
	{
		while (*lpszTemp != '\0')
			lpszTemp++;
		
		COPCServer* pServer = new COPCServer;

		// 数据源名称
		pServer->m_bstrName = lpszSection;
			
		// 服务器名称
		TCHAR szT[80];
		GetPrivateProfileString(lpszSection, _T("OPCSERVER"),
			NULL, szT, 80, szIniFile);
		pServer->m_bstrOPCServerName = szT;
		
		// 计算机名称
		GetPrivateProfileString(lpszSection, _T("COMPUTER"),
			NULL, szT, 80, szIniFile);
		pServer->m_bstrComputer = szT;
		
		// 增加节点
		m_apOPCServers.Add(pServer);
		
		lpszTemp++;
		if (*lpszTemp == '\0')
			break;
		lpszSection = lpszTemp;
	}
}

void COPCServerPage::LoadObjectItems(HTREEITEM hParentItem)
{
	USES_CONVERSION;

	if (hParentItem == NULL)
	{
		for (int i = 0; i < m_apOPCServers.GetSize(); i++)
		{
			COPCServer* pServer = m_apOPCServers[i];
			HTREEITEM hItem = m_tree.InsertItem(OLE2T(pServer->m_bstrName), NULL, NULL);
			m_tree.SetItemData(hItem, (DWORD)&pServer->m_group);
			m_tree.InsertItem(_T("VirtualChild"), hItem, TVI_SORT);
		}

		return;
	}

	//节点已经被展开过返回，由子项目是否是正式节点判断
	HTREEITEM hChildItem = m_tree.GetChildItem(hParentItem);
	if (hChildItem != NULL && m_tree.GetItemData(hChildItem) != 0)
		return;

	//删除子节点
	DeleteAllChildItem(hParentItem);

	COPCGroup* pGroup = (COPCGroup *)m_tree.GetItemData(hParentItem);
	if (pGroup == NULL)
		return;

	CSimpleArray<COPCGroup *>& arrGroups = pGroup->GetGroups();
	CSimpleArray<COPCItem *>& arrItems = pGroup->GetItems();

	TV_INSERTSTRUCT tvis;
	tvis.hParent = hParentItem;
	tvis.hInsertAfter = TVI_SORT;
	tvis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	
	tvis.item.iImage = 6;
	tvis.item.iSelectedImage = 7;
	
	for (int i = 0; i < arrGroups.GetSize(); i++)
	{
		tvis.item.pszText = OLE2T(arrGroups[i]->m_bstrName);
		HTREEITEM hGroupItem = m_tree.InsertItem(&tvis);

		m_tree.SetItemData(hGroupItem, (DWORD)arrGroups[i]);
		m_tree.InsertItem(_T("VirtualChild"), hGroupItem, TVI_SORT);
	}

	tvis.item.iImage = 8;
	tvis.item.iSelectedImage = 9;

	for (i = 0; i < arrItems.GetSize(); i++)
	{
		tvis.item.pszText = OLE2T(arrItems[i]->m_bstrName);
		m_tree.InsertItem(&tvis);
	}
}

