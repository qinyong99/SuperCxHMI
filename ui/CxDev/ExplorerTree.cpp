// ExplorerTree.cpp : implementation file
//

#include "stdafx.h"
#include "DevInc.h"
#include "ExplorerTree.h"
#include "DirWatcher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExplorerTree

CExplorerTree::CExplorerTree()
{
	m_htiSelect = NULL;
	m_bDownSelectItem = FALSE;
	m_bNoSelectedChganded = FALSE;

	m_pDirWatcher = NULL;
}

CExplorerTree::~CExplorerTree()
{
	if (m_pDirWatcher != NULL)
		delete m_pDirWatcher;
}


BEGIN_MESSAGE_MAP(CExplorerTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CExplorerTree)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_COMMAND(ID_ADD_FORM, OnAddForm)
	ON_COMMAND(ID_ADD_BLANK_REPORT, OnAddBlankReport)
	ON_COMMAND(ID_ADD_REPORT_WIZARD, OnAddReportWizard)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_COMMAND(ID_ADD_CODE, OnAddCode)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CDevApp::m_nDocumentChangeMsg, OnDocumentChangedMsg)
	ON_REGISTERED_MESSAGE(CDevApp::m_nDocumentActiveMsg, OnDocumentActivateMsg)
	ON_REGISTERED_MESSAGE(CDirWatcher::m_nWatchNotifyMsg, OnWatchNotifyMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExplorerTree message handlers

int CExplorerTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	theApp.m_listFileOpenNotify.AddTail(m_hWnd);
	theApp.m_listActivateMsgNotify.AddTail(m_hWnd);

	InitExplorerTree();	
	
	m_dropTarget.Register(this);

	return 0;
}

void CExplorerTree::InitExplorerTree()
{
	m_ImageList.Create(16, 16, ILC_MASK, 0, 16);
//	m_ImageList.Create (16, 16, FALSE, 2, 0);
	m_ImageList.Add(::LoadIcon ( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PROJECT)));
	m_ImageList.Add(::LoadIcon ( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PICTURES)));
	m_ImageList.Add(::LoadIcon ( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PICTURE)));
	m_ImageList.Add(::LoadIcon ( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MAINFORM)));
	m_ImageList.Add(::LoadIcon ( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_FORM)));
	m_ImageList.Add(::LoadIcon ( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SUBFORM)));
	m_ImageList.Add(::LoadIcon ( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_REPORT)));
	m_ImageList.Add(::LoadIcon ( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SUBREPORT)));
	m_ImageList.Add(::LoadIcon ( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_CODEMODULE)));
	
	SetImageList(&m_ImageList, TVSIL_NORMAL);

	PopulateTree();	
}

void CExplorerTree::PopulateTree()
{
//	TV_ITEM         tvi; 
//	TVINSERTSTRUCT  tvins;
	
//	memset(&tvi, 0, sizeof(TV_ITEM));
//	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
//	tvi.iImage = tvi.iSelectedImage = IID_PROJECT;
//	tvi.pszText = "演示工程";
//	tvins.item = tvi;
//	tvins.hParent = NULL;
//	HTREEITEM htiProject = InsertItem(&tvins);

//	tvi.pszText = "画面";
//	tvi.iImage = tvi.iSelectedImage = IID_PICTURES;
//	tvins.item = tvi;
//	tvins.hParent = NULL;
//	HTREEITEM htiPictures = InsertItem(&tvins);

	HTREEITEM htiPictures = InsertItem(_T("画面"), IID_PICTURES, IID_PICTURES);

	RefreshPictureItems();

	Expand(htiPictures, TVE_EXPAND);
}

void CExplorerTree::RefreshPictureItems()
{
	HTREEITEM hPictureRoot = GetPictureRoot();
	ASSERT(hPictureRoot != NULL);
	if (hPictureRoot == NULL)
		return;

//	CMapStringToOb map;

	HTREEITEM hNextItem;
   	HTREEITEM hChildItem = GetChildItem(hPictureRoot);
	while (hChildItem != NULL)
	{
		hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
//		map.SetAt(GetItemText(hChildItem), NULL);
		DeleteItem(hChildItem);
		hChildItem = hNextItem;
 	}

	TV_ITEM         tvi; 
	TVINSERTSTRUCT  tvins;
	
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	
	//枚举存放画面目录的所有画面文件

	CString strFilter;
	static CString strLastFilter;
	strFilter = theApp.GetIniStartPictureFolder();
	strFilter += _T("*.grf");
	if (strLastFilter != strFilter)
	{
//		map.RemoveAll();
		strLastFilter = strFilter;
	}

	CFileFind finder;
	BOOL bWorking = finder.FindFile(strFilter);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		CString strFileName = finder.GetFileName();
		strFileName = strFileName.Left(strFileName.GetLength() - 4);

		tvins.hParent = hPictureRoot;
		tvi.pszText = (LPSTR)(LPCTSTR)strFileName;
		tvi.iImage = tvi.iSelectedImage = IID_PICTURE;
		tvins.item = tvi;

		HTREEITEM htiPicture = InsertItem(&tvins);
		
		//插入主窗体子结点, 要显示+在项前是必要的
		tvins.hParent = htiPicture;
		tvi.pszText = "MainForm";
		tvi.iImage = tvi.iSelectedImage = IID_MAINFORM;
		InsertItem(&tvins);

//		CObject* pObj = NULL;
//		if (map.Lookup(GetItemText(htiPicture), pObj))
//			Expand(htiPicture, TVE_EXPAND);
	}

	if (m_pDirWatcher != NULL)
		delete m_pDirWatcher;

	m_pDirWatcher = new CDirWatcher;
	m_pDirWatcher->StartWatch(GetSafeHwnd(), theApp.GetIniStartPictureFolder());
}

HTREEITEM CExplorerTree::FindPictureItem(LPCTSTR szName)
{
	HTREEITEM hPictureRoot = GetPictureRoot();
	ASSERT(hPictureRoot != NULL);

   	HTREEITEM hChildItem = GetChildItem(hPictureRoot);
	while (hChildItem != NULL)
	{
		if (GetItemText(hChildItem) == szName)
			return hChildItem;
		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
 	}

	return NULL;
}

HTREEITEM CExplorerTree::FindPictureSubItem(LPCTSTR szName, LPCTSTR szSubName)
{
	HTREEITEM hPicture = FindPictureItem(szName);
	if (hPicture == NULL)
		return NULL;
	
   	HTREEITEM hChildItem = GetChildItem(hPicture);
	while (hChildItem != NULL)
	{
		if (GetItemText(hChildItem) == szSubName)
			return hChildItem;
		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
 	}

	return NULL;
}

HTREEITEM CExplorerTree::FindPictureSubItem(HTREEITEM hItem, DWORD dwData)
{
	HTREEITEM hChildItem = GetChildItem(hItem);
	while (hChildItem != NULL)
	{
		if (GetItemData(hChildItem) == dwData)
			return hChildItem;
		HTREEITEM hSearchItem = FindPictureSubItem(hChildItem, dwData);
		if (hSearchItem != NULL)
			return hSearchItem;
		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
 	}

	return NULL;
}

HTREEITEM CExplorerTree::FindPictureSubItem(LPCTSTR szName, DWORD dwData)
{
	HTREEITEM hPicture = FindPictureItem(szName);
	if (hPicture == NULL)
		return NULL;
	
   	return FindPictureSubItem(hPicture, dwData);
}

CDevDoc* CExplorerTree::GetPictureItemDocument(HTREEITEM hPicture)
{
	CString szPathName = theApp.GetIniStartPictureFolder();
	szPathName += GetItemText(hPicture);
	szPathName += ".grf";

	CDevDoc* pDoc =  (CDevDoc *)CDevDoc::FindDocument(szPathName);

	if (pDoc == NULL)
	{
		if (IsFileExist(szPathName))
			pDoc = (CDevDoc *)theApp.OpenDocumentFile(szPathName);
	}

	return pDoc;
}

CDevDoc::CSubDoc* CExplorerTree::GetPictureSubItemDocument(HTREEITEM hPictureSubItem, CDevDoc*& pDoc)
{
	HTREEITEM hPicture = GetParentItem(hPictureSubItem);
	pDoc = GetPictureItemDocument(hPicture);

	if (pDoc == NULL)
		return NULL;

	CString strItem = GetItemText(hPictureSubItem);

	return pDoc->GetSubDocument(strItem);
}

HTREEITEM CExplorerTree::FindChildItemByData(HTREEITEM hItem, DWORD dwData)
{
   	HTREEITEM hChildItem = GetChildItem(hItem);
	while (hChildItem != NULL)
	{
		if (GetItemData(hChildItem) == dwData)
			return hChildItem;

		HTREEITEM hFindItem = FindChildItemByData(hChildItem, dwData);
		if (hFindItem != NULL)
			return hFindItem;

		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
 	}

	return NULL;
}

void CExplorerTree::GetExpandItemsData(HTREEITEM hItem, CArray<DWORD, DWORD&>& arrData)
{
	HTREEITEM hChildItem = GetChildItem(hItem);
	while (hChildItem != NULL)
	{
		if (GetItemState(hChildItem, TVIS_EXPANDED) & TVIS_EXPANDED)
		{
			DWORD dwData = GetItemData(hChildItem);
			arrData.Add(dwData);
			GetExpandItemsData(hChildItem, arrData);
		}

		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
 	}
}

void CExplorerTree::RefreshPictureItem(HTREEITEM hPicture, CDevDoc* pDoc)
{
	HTREEITEM hItem;

	//防止重新进入，在此函数选中活动项可能回导致OnItemexpanding再次调用
	static BOOL bEnter = FALSE;
	if (bEnter)
		return;
	bEnter = TRUE;

	if (pDoc == NULL)
	{
		pDoc = GetPictureItemDocument(hPicture);
	}

	if (pDoc == NULL)
	{
		bEnter = FALSE;
		return;
	}

	ASSERT(pDoc != NULL);
	SetRedraw(FALSE);

	CArray<DWORD, DWORD&> arrExpandItemsData;
	GetExpandItemsData(hPicture, arrExpandItemsData);

	DWORD dwOldSelectItemData = 0;
	hItem = GetSelectedItem();
	if (hItem != NULL)
		dwOldSelectItemData = GetItemData(hItem);

	m_bNoSelectedChganded = TRUE; //如果选择地的节点也被删除，则可能改变选择的节点

	HTREEITEM hNextItem;
   	HTREEITEM hChildItem = GetChildItem(hPicture);
	while (hChildItem != NULL)
	{
		hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
		DeleteItem(hChildItem);
		hChildItem = hNextItem;
 	}

	m_bNoSelectedChganded = FALSE;

	CMDIFrameWnd* pParentFrame = (CMDIFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd* pMDIActive = pParentFrame->MDIGetActive();
	CDocument* pActiveDoc = NULL;
	if (pMDIActive != NULL)
		pActiveDoc = pMDIActive->GetActiveDocument();

	CPtrList& listSubDoc = pDoc->m_listSubDoc;
	POSITION pos = listSubDoc.GetHeadPosition();
	while (pos != NULL)
	{
		CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc* )(listSubDoc.GetNext(pos));
		CString strCaption = pSubDoc->m_strName; //GetCaption();
		if (pSubDoc->GetType() == CDevDoc::enumFormDoc || pSubDoc->GetType() == CDevDoc::enumReportDoc)
		{
			if (pSubDoc->IsFirstSubDoc())
			{
				hItem = InsertItem(strCaption, IID_MAINFORM, IID_MAINFORM, hPicture);
			}
			else
			{
				if (pSubDoc->GetType() == CDevDoc::enumReportDoc)
					hItem = InsertItem(strCaption, IID_REPORT, IID_REPORT, hPicture);
				else
					hItem = InsertItem(strCaption, IID_FORM, IID_FORM, hPicture);
			}
			SetItemData(hItem, DWORD(pSubDoc));

			CFormDoc* pFormDoc = (CFormDoc*)pSubDoc->m_pChildDoc;
			if (pFormDoc != NULL)
				InsertFormDocItem(pFormDoc, hItem);
		}
		else if (pSubDoc->GetType() == CDevDoc::enumCodeDoc)
		{
			hItem = InsertItem(strCaption, IID_CODEMODULE, IID_CODEMODULE, hPicture);
		}
		else
		{
			ASSERT(FALSE);
		}

		if (pActiveDoc != NULL && pActiveDoc == pSubDoc->m_pChildDoc)
		{
			m_bNoSelectedChganded = TRUE;
			SelectItem(hItem);	
			m_bNoSelectedChganded = FALSE;

			dwOldSelectItemData = 0;
		}
	}

	for (int i = 0; i < arrExpandItemsData.GetSize(); i++)
	{
		hItem = FindChildItemByData(hPicture, arrExpandItemsData[i]);
		if (hItem != NULL)
			Expand(hItem, TVE_EXPAND);
	}

	if (dwOldSelectItemData != 0)
	{
		hItem = FindChildItemByData(hPicture, dwOldSelectItemData);
		if (hItem != NULL)
		{
			m_bNoSelectedChganded = TRUE;
			SelectItem(hItem);	
			m_bNoSelectedChganded = FALSE;
		}
	}

	SetRedraw(TRUE);

	bEnter = FALSE;
}

void CExplorerTree::InsertFormDocItem(CFormDoc* pDoc, HTREEITEM hItem)
{
	POSITION posObj = pDoc->GetFirstObjPosition();
	while (posObj)
	{
		CDevObj* pObj = pDoc->GetNextObj(posObj);
		if (pObj->IsKindOf(RUNTIME_CLASS(CFormObj)))
		{
			CString strCaption = pObj->GetDisplayName();

			HTREEITEM hFormItem;
			if (pObj->IsKindOf(RUNTIME_CLASS(CReportObj)))
				hFormItem = InsertItem(strCaption, IID_SUBREPORT, IID_SUBREPORT, hItem, TVI_SORT);
			else
				hFormItem = InsertItem(strCaption, IID_SUBFORM, IID_SUBFORM, hItem, TVI_SORT);
			
			SetItemData(hFormItem, DWORD(pObj));
			CFormDoc* pFormDoc = ((CFormObj *)pObj)->m_pFormDoc;
			if (pFormDoc != NULL)
				InsertFormDocItem(pFormDoc, hFormItem);
		}
	}
}

BOOL CExplorerTree::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= /*TVS_EDITLABELS |*/ TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;

	return CTreeCtrl::PreCreateWindow(cs);
}

void CExplorerTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pNMHDR;
	
	HTREEITEM htiSelect = pnmtv->itemNew.hItem;
	m_htiSelect = htiSelect;

	if (m_bNoSelectedChganded == TRUE)
		return;

	if (htiSelect == NULL)
		return;

	int nImage, nSelectedImage;
	if (!GetItemImage(htiSelect, nImage, nSelectedImage))
		return;

	CWnd* pOld = GetFocus();

	Expand(htiSelect, TVE_EXPAND);

	if (nImage == IID_MAINFORM || nImage == IID_FORM || nImage == IID_REPORT || 
		nImage == IID_CODEMODULE)		//窗体
	{
		CDevDoc* pDoc = NULL;
		CDevDoc::CSubDoc* pSubDoc = GetPictureSubItemDocument(htiSelect, pDoc);
		if (pDoc != NULL && pSubDoc != NULL)
			pDoc->ActivateSubDocument(pSubDoc);
	}
	else if (nImage == IID_SUBFORM || nImage == IID_SUBREPORT)
	{
		CFormObj* pFormObj = (CFormObj *)GetItemData(htiSelect);
		pFormObj->OnEdit();
//		CFormDoc* pFormDoc = ((CFormObj *)pFormObj)->m_pFormDoc;
//		CMDIChildWnd* pNewFrame
//				= (CMDIChildWnd*)(theApp.m_pLayoutTemplate->CreateNewFrame(pFormDoc, NULL));
//		ASSERT(pNewFrame != NULL);
//		ASSERT_KINDOF(CMDIChildWnd, pNewFrame);
//		pNewFrame->InitialUpdateFrame(pFormDoc, TRUE);
	}

	if (pOld != NULL)
		pOld->SetFocus();

	*pResult = 0;
}

void CExplorerTree::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pNMHDR;
	
	if (pnmtv->action != TVE_EXPAND ||
		(pnmtv->itemNew.state & TVIS_EXPANDEDONCE))
		return ;

	//当展开画面名称结点
	if (pnmtv->itemNew.iImage == IID_PICTURE)
		RefreshPictureItem(pnmtv->itemNew.hItem, NULL);

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
//功 能：往所选画面添加窗体
//描 述：
//////////////////////////////////////////////////////////////////////////////
void CExplorerTree::OnAddForm() 
{
	if (m_htiSelect == NULL)
		return;
	
	HTREEITEM hPictureItem = m_htiSelect;
	int nImage, nSelectedImage;
	if (!GetItemImage(hPictureItem, nImage, nSelectedImage))
		return;
	while (nImage != IID_PICTURE && GetParentItem(hPictureItem) != NULL)
	{
		hPictureItem = GetParentItem(hPictureItem);
		if (!GetItemImage(hPictureItem, nImage, nSelectedImage))
			return;
	}
	
	ASSERT(nImage == IID_PICTURE);

	CDevDoc* pDoc = GetPictureItemDocument(hPictureItem);
	ASSERT(pDoc != NULL);

	pDoc->OnAddForm();
}

void CExplorerTree::OnAddBlankReport()
{
	if (m_htiSelect == NULL)
		return;
	
	HTREEITEM hPictureItem = m_htiSelect;
	int nImage, nSelectedImage;
	if (!GetItemImage(hPictureItem, nImage, nSelectedImage))
		return;
	while (nImage != IID_PICTURE && GetParentItem(hPictureItem) != NULL)
	{
		hPictureItem = GetParentItem(hPictureItem);
		if (!GetItemImage(hPictureItem, nImage, nSelectedImage))
			return;
	}
	
	ASSERT(nImage == IID_PICTURE);
	
	CDevDoc* pDoc = GetPictureItemDocument(hPictureItem);
	ASSERT(pDoc != NULL);
	
	pDoc->OnAddBlankReport();
}

void CExplorerTree::OnAddReportWizard()
{
	if (m_htiSelect == NULL)
		return;
	
	HTREEITEM hPictureItem = m_htiSelect;
	int nImage, nSelectedImage;
	if (!GetItemImage(hPictureItem, nImage, nSelectedImage))
		return;
	while (nImage != IID_PICTURE && GetParentItem(hPictureItem) != NULL)
	{
		hPictureItem = GetParentItem(hPictureItem);
		if (!GetItemImage(hPictureItem, nImage, nSelectedImage))
			return;
	}
	
	ASSERT(nImage == IID_PICTURE);
	
	CDevDoc* pDoc = GetPictureItemDocument(hPictureItem);
	ASSERT(pDoc != NULL);

	pDoc->OnAddReportWizard();
}

/////////////////////////////////////////////////////////////////////
//功能:根据击活项的改变而做相应显示上的变动
//参数:wparam[in]		击活对象的类型
//		lparam[in]		对应击活对象类型的参数
//返回:0
/////////////////////////////////////////////////////////////////////
LONG CExplorerTree::OnDocumentActivateMsg(DWORD dwNotify, DWORD dwData)
{
	CDevDoc* pDoc = (CDevDoc*)dwNotify;

	if (pDoc != NULL)
	{
	//	CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc *)dwData;

		CString strFileName = pDoc->GetDisplayName();
		int nIndex = strFileName.ReverseFind('.');
		if (nIndex != -1)
			strFileName = strFileName.Left(nIndex);
		
		HTREEITEM hItem = FindPictureSubItem(strFileName, dwData);
		m_bNoSelectedChganded = TRUE;
		SelectItem(hItem);	
		m_bNoSelectedChganded = FALSE;
	}
	else
	{
		SelectItem(NULL);
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////
//功能:画面文档增加或删除窗体或窗体的ID及名称改变后激发,用于更改对应的显示内容
//////////////////////////////////////////////////////////////
LONG CExplorerTree::OnDocumentChangedMsg(DWORD dwNotify, DWORD dwData)
{
	CDevDoc* pDoc = (CDevDoc*)dwNotify;
	CDocHit* pHit = (CDocHit *)dwData;

	if (pHit == NULL)
	{
		if (pDoc == NULL)
		{
			theApp.SaveAllModified();
			SetRedraw(FALSE);
			CDevDoc::CloseAllDocuments(FALSE);
			RefreshPictureItems();
			SetRedraw(TRUE);
		}
		
		return 0;
	}

	if (pHit->m_changeType == CDocHit::changeNew)
	{
		ASSERT(!pHit->m_strNewName.IsEmpty());

		CString strFileName = pHit->m_strNewName;
		int nIndex = strFileName.ReverseFind('.');
		if (nIndex != -1)
			strFileName = strFileName.Left(nIndex);
	
		HTREEITEM hPicture = FindPictureItem(strFileName);
		if (hPicture == NULL)
		{
			hPicture = InsertItem(strFileName, IID_PICTURE, IID_PICTURE, GetPictureRoot());
			InsertItem(_T("MainForm"), IID_MAINFORM, IID_MAINFORM, hPicture);
		}
	}
	else if (pHit->m_changeType == CDocHit::changeContent)
	{
		ASSERT(!pHit->m_strNewName.IsEmpty());

		CString strFileName = pHit->m_strNewName;
		int nIndex = strFileName.ReverseFind('.');
		if (nIndex != -1)
			strFileName = strFileName.Left(nIndex);
		
		HTREEITEM hPicture = FindPictureItem(strFileName);
		if (hPicture != NULL)
			RefreshPictureItem(hPicture, pDoc);
	}
	else if (pHit->m_changeType == CDocHit::changeDelete)
	{
		ASSERT(!pHit->m_strNewName.IsEmpty());

		if (!IsPictureExist(pHit->m_strNewName))
		{
			CString strItem = pHit->m_strNewName;
			
			LPTSTR lpszExt = _tcsrchr(strItem, '.');
			if (lpszExt != NULL)
				*lpszExt = 0;       // no suffix
			
			HTREEITEM hPicture = FindPictureItem(strItem);
			if (hPicture != NULL)
				DeleteItem(hPicture);
		}
	}
	else if (pHit->m_changeType == CDocHit::changeRename)
	{
		HTREEITEM hPicture;
	
		ASSERT(!pHit->m_strOldName.IsEmpty());
		if (!IsPictureExist(pHit->m_strOldName))
		{
			CString strItem = pHit->m_strOldName;
			LPTSTR lpszExt = _tcsrchr(strItem, '.');
			if (lpszExt != NULL)
				*lpszExt = 0;       // no suffix
			
			hPicture = FindPictureItem(strItem);
			if (hPicture != NULL)
				DeleteItem(hPicture);
		}

		ASSERT(!pHit->m_strNewName.IsEmpty());
		if (IsPictureExist(pHit->m_strNewName))
		{
			CString strItem = pHit->m_strNewName;
			
			LPTSTR lpszExt = _tcsrchr(strItem, '.');
			if (lpszExt != NULL)
				*lpszExt = 0;       // no suffix
			
			hPicture = FindPictureItem(strItem);
			if (hPicture == NULL)
				hPicture = InsertItem(strItem, IID_PICTURE, IID_PICTURE, GetPictureRoot());
			
			if (pDoc != NULL)
			{
				ASSERT(hPicture != NULL);
				RefreshPictureItem(hPicture, pDoc);
			}
		}
	}
	
	return 0;
}

BOOL CExplorerTree::IsPictureExist(LPCTSTR szName)
{
	CString strPathName = theApp.GetIniStartPictureFolder();
	strPathName += szName;
	if (_tcsrchr(strPathName, '.') == NULL) // 没有后缀名，加上后缀
		strPathName += ".grf";

	return IsFileExist(strPathName);
}

LONG CExplorerTree::OnWatchNotifyMsg(WPARAM wParam, LPARAM lParam)
{
	CDirWatcher::CFileChangeInfo* pFileChangeInfo = (CDirWatcher::CFileChangeInfo *)lParam;
	
	switch (pFileChangeInfo->nType) 
	{
	case CDirWatcher::FileChange_Add:
		if (pFileChangeInfo->strName.Right(4).CompareNoCase(_T(".grf")) == 0)
		{
			CDocHit hit(CDocHit::changeNew, pFileChangeInfo->strName);
			theApp.NotifyDocumentChanged(NULL, (DWORD)&hit);
		}
		break;
	case CDirWatcher::FileChange_Remove:
		if (pFileChangeInfo->strName.Right(4).CompareNoCase(_T(".grf")) == 0)
		{
			CDocHit hit(CDocHit::changeDelete, pFileChangeInfo->strName);
			theApp.NotifyDocumentChanged(NULL, DWORD(&hit));
		}
		break;
	case CDirWatcher::FileChange_Rename:
		{
			CDocHit hit(CDocHit::changeRename, pFileChangeInfo->strNewName, pFileChangeInfo->strName);
			theApp.NotifyDocumentChanged(NULL, DWORD(&hit));
		}
		break;
	default:
		break;
	}
	
	delete pFileChangeInfo;
	
	return 0;
}

////////////////////////////////////////////////////////////
//功能:返回画面项的根位置
////////////////////////////////////////////////////////////
HTREEITEM CExplorerTree::GetPictureRoot()
{
	return GetRootItem();
}

////////////////////////////////////////////////////////////////
//功能:从全路径名中取出主文件名
////////////////////////////////////////////////////////////////
CString CExplorerTree::MainFileNameFromFullPath(CString strPath)
{
	CString name;

	int len = strPath.GetLength();
	int stage = 0;
	while(len--)
	{
		if(stage == 0)
		{
			if(strPath[len] == '.')
			{
				stage = 1;
				continue;
			}

			if(strPath[len] == '\\' || strPath[len] == ':' || len == 0)
			{	//无扩展名
				name = strPath.Right(strPath.GetLength() - len - 1);
				break;
			}

			continue;
		}
		else
		{
			if(strPath[len] == '\\' || strPath[len] == ':')
			{
				CString Tmp = name;
				len = name.GetLength();
				int i = len;
				while(i--)
				{
					Tmp.SetAt(i, name[len - i -1]);
				}
				name = Tmp;
				break;
			}
			name += strPath[len];
		}
	}

	return name;
}

void CExplorerTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DELETE)
	{
		m_htiSelect = GetSelectedItem();
		OnEditClear();
		return;
	}

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////
//功能:返回由hItem指定的窗体项或画面项对应的文档指针, 及该项对应的ID号
//参数:hItem[in]		树项
//		formID[out]		窗体的ID号码
//返回:成功为对应画面对象的指针, 失败为NULL;ID中返回对应子文档的ID号
//说明:如hItem是画面项则返回的ID号为-1
//////////////////////////////////////////////////////////////
CDocument* CExplorerTree::GetDocument(HTREEITEM hItem, DWORD& formID)
{
	CDocument* pDoc = NULL;
	ASSERT(hItem);
	int nImage, nSelectedImage;
	if( (hItem == NULL) || (!GetItemImage(hItem, nImage, nSelectedImage)) )
		return pDoc;

	HTREEITEM hParent = NULL;
	if (nImage == IID_MAINFORM || nImage == IID_FORM || nImage == IID_REPORT)
	{	//窗体或主窗体
		hParent = GetParentItem(hItem);
		formID = GetItemData(hItem);
	}
	else if(nImage == IID_PICTURE)
	{	//画面
		hParent = hItem;
		hItem = GetChildItem(hParent);
		formID = -1;
	}
	else
		return pDoc;
	
	if(hParent == NULL || hItem == NULL)
		return pDoc;

	if (GetItemImage(hParent, nImage, nSelectedImage))
	{
		if (nImage != IID_PICTURE)
			return pDoc;

		DWORD ID = GetItemData(hItem);
		pDoc = (CDevDoc*)GetItemData(hParent);
	}

	return pDoc;
}

void CExplorerTree::OnEditDelete(BOOL bPrompt)
{
	if (m_htiSelect == NULL)
		return;

	int nImage, nSelectedImage;
	if (GetItemImage(m_htiSelect, nImage, nSelectedImage) == FALSE)	//没有选中项
		return;

	switch(nImage)
	{
	case IID_PICTURE:
		{
			if (bPrompt && AfxMessageBox(_T("要删除选择的画面吗？"), MB_YESNO | MB_ICONQUESTION) != IDYES)
				return;

			CString szPathName = theApp.GetIniStartPictureFolder();
			szPathName += GetItemText(m_htiSelect);
			szPathName += ".grf";
			
			SelectItem(NULL);
			CDevDoc::DeleteDocumentFile(szPathName);
		}
		break;
	case IID_FORM:
	case IID_CODEMODULE:
	case IID_REPORT:
		{
			CString strPrompt;
	
			if (nImage == IID_FORM)
				strPrompt = _T("要删除选择的窗体吗？");
			else
				strPrompt = _T("要删除选择的报表吗？");
	
			if (bPrompt && AfxMessageBox(strPrompt, MB_YESNO | MB_ICONQUESTION) != IDYES)
				return;

			CDevDoc* pDoc = NULL;
			CDevDoc::CSubDoc* pSubDoc = GetPictureSubItemDocument(m_htiSelect, pDoc);
			pDoc->DeleteSubDocument(pSubDoc);
		}
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////
//功能:删除当前选中的项
////////////////////////////////////////////////////////////////////
void CExplorerTree::OnEditClear() 
{
	OnEditDelete();
}

////////////////////////////////////////////////////////////////////
//功能:判定指定项是否可以进行名称编缉,并允许或禁止该操作
////////////////////////////////////////////////////////////////////
void CExplorerTree::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

//	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
//	TVITEM* pItem = &(pTVDispInfo->item);
	// TODO: Add your control notification handler code here
//	*pResult = 1;
//	int nImage, nSelectedImage;
//	if(GetItemImage(pItem->hItem, nImage, nSelectedImage))
//	{	//有该存在
//		if(nImage == IID_FORM)
//		{	//子窗体
//			*pResult = 0;
//		}
//	}

}

///////////////////////////////////////////////////////////////////
//功能:对改动后的名称进行检查和保存
///////////////////////////////////////////////////////////////////
void CExplorerTree::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
//	TVITEM* pItem = &(pTVDispInfo->item);
//	// TODO: Add your control notification handler code here
//	int nImage, nSelectedImage;
//	if(GetItemImage(pItem->hItem, nImage, nSelectedImage))
//	{	//有该存在
//		if(nImage == IID_FORM)
//		{	//窗体
//			DWORD ID;
//			CDevDoc* pDevDoc = (CDevDoc *)(GetDocument(pItem->hItem, ID));
//			if(pDevDoc != NULL && pItem->pszText != NULL)
//				pDevDoc->SetSubDocTitle(ID, pItem->pszText);
//		}
//	}
	
	*pResult = 0;
}

BOOL CExplorerTree::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if( pMsg->message == WM_KEYDOWN )
    {
		if( GetEditControl() &&
			(pMsg->wParam == VK_RETURN ||  pMsg->wParam == VK_ESCAPE )
		  )
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;                            // DO NOT process further
		}
	}

	return CTreeCtrl::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////
//功能:添加代码
/////////////////////////////////////////////////////////////////////////
void CExplorerTree::OnAddCode() 
{
	//暂时不支持
	return;
	
	if (m_htiSelect == NULL)
		return;

	HTREEITEM hItem = m_htiSelect;
	m_htiSelect = NULL;
	CDevDoc* pDoc = (CDevDoc *)GetItemData(hItem);

	if(pDoc != NULL)
	{	//对应文档已打开
		ASSERT(pDoc != NULL);
		if(pDoc == NULL)
			return;
		pDoc->AddSubDocument(CDevDoc::enumCodeDoc);
	}
	else
	{	//文档未打开
		CString strName = GetItemText(m_htiSelect);
		CString strDocPath = theApp.GetIniStartPictureFolder();
		strDocPath += strName;
		strDocPath += ".grf";
	
		CDevDoc* pDoc = (CDevDoc *)CDevDoc::FindDocument(strDocPath);
		ASSERT(pDoc != NULL);
		if(pDoc == NULL)
			return;

		pDoc->AddSubDocument(CDevDoc::enumCodeDoc);
		theApp.OpenDocumentFile(strDocPath); //激活窗口
	}
}

void CExplorerTree::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_htiSelect = NULL;

	CPoint curPoint;
	GetCursorPos(&curPoint);
	CPoint ptHit = curPoint;
	ScreenToClient(&ptHit);
	HTREEITEM htiHit = HitTest(ptHit, &nFlags);

	m_bDownSelectItem = (htiHit == GetSelectedItem());

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CExplorerTree::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	HTREEITEM htiSelect = GetSelectedItem();
	
	if (m_bDownSelectItem && htiSelect != NULL && htiSelect != GetRootItem())
	{
		CMDIFrameWnd* pParentFrame = (CMDIFrameWnd*)AfxGetMainWnd();
		CMDIChildWnd* pMDIActive = pParentFrame->MDIGetActive();
		
		if (pMDIActive != NULL)
		{
			pParentFrame->SendMessage(WM_COMMAND, 
				pMDIActive->IsKindOf(RUNTIME_CLASS(CLayoutFrame)) ? ID_VIEW_SCRIPT : ID_VIEW_OBJECT, 0);
		}
	}
	
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}

void CExplorerTree::OnEditCopy() 
{
	int nImage, nSelectedImage;
	if (!GetItemImage(m_htiSelect, nImage, nSelectedImage))
		return;
	ASSERT(nImage == IID_MAINFORM || nImage == IID_FORM || nImage == IID_REPORT);

	CDevDoc* pDoc = NULL;
	CDevDoc::CSubDoc* pSubDoc = GetPictureSubItemDocument(m_htiSelect, pDoc);
	ASSERT(pDoc != NULL);
	ASSERT(pSubDoc != NULL);

	STGMEDIUM stgMedium;
	COleDataSource* pDataSource = new COleDataSource;

	CDevDoc::GetSubDocumentData(pSubDoc, &stgMedium);
	pDataSource->CacheData(CDevDoc::m_cfDocData, &stgMedium);

	ASSERT_VALID(pDataSource);

	pDataSource->SetClipboard();
}

void CExplorerTree::OnEditCut() 
{
	OnEditCopy();
	OnEditClear();
}

void CExplorerTree::OnEditPaste() 
{
	HTREEITEM hPictureItem = m_htiSelect;
	int nImage, nSelectedImage;
	if (!GetItemImage(hPictureItem, nImage, nSelectedImage))
		return;
	while (nImage != IID_PICTURE && GetParentItem(hPictureItem) != NULL)
	{
		hPictureItem = GetParentItem(hPictureItem);
		if (!GetItemImage(hPictureItem, nImage, nSelectedImage))
			return;
	}
	
	ASSERT(nImage == IID_PICTURE);

	CDevDoc* pDoc = GetPictureItemDocument(hPictureItem);
	ASSERT(pDoc != NULL);

	pDoc->DoPasteSubDocument(NULL);
}

///////////////////////////////////////////////////////////////
//功能:展开画面结点
//参数:hPicture[in]		文档对应项名柄
//返回:打开的文档指针
///////////////////////////////////////////////////////////////
CDocument* CExplorerTree::ExpandPictureItem(HTREEITEM hPicture)
{
	int nImage, nSelectedImage;
	ASSERT(GetItemImage(hPicture, nImage, nSelectedImage));
	ASSERT(nImage == IID_PICTURE);

	CDocument* pDoc = (CDocument *)GetItemData(hPicture);
	//已经打开
	if (pDoc != NULL)
		return pDoc;

	CString strPathName = theApp.GetIniStartPictureFolder();
	strPathName += GetItemText(hPicture);
	strPathName += ".grf";

	//打开画面
	pDoc = theApp.OpenDocumentFile(strPathName);
	SetItemData(hPicture, (DWORD)pDoc);

	return pDoc;
}

void CExplorerTree::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	UINT nFlags;
	CPoint curPoint;
	GetCursorPos(&curPoint);
	CPoint ptHit = curPoint;
	ScreenToClient(&ptHit);
	m_htiSelect = HitTest(ptHit, &nFlags);

	TV_ITEM tvi;
	
	tvi.hItem = m_htiSelect;
	tvi.mask = TVIF_HANDLE | TVIF_IMAGE;
	GetItem(&tvi);
	UINT nImageID = tvi.iImage;

	CMenu menu;
	menu.CreatePopupMenu();

	if (nImageID == IID_PICTURE)
	{
		if (IsClipboardFormatAvailable(CDevDoc::m_cfDocData))
			menu.AppendMenu(MF_STRING, ID_EDIT_PASTE, _T("粘贴(&P)"));
		menu.AppendMenu(MF_STRING, ID_EDIT_CLEAR, _T("删除(&D)"));
	}
	else if (nImageID == IID_MAINFORM)
	{
		menu.AppendMenu(MF_STRING, ID_EDIT_COPY, _T("拷贝(&C)"));
		if (IsClipboardFormatAvailable(CDevDoc::m_cfDocData))
			menu.AppendMenu(MF_STRING, ID_EDIT_PASTE, _T("粘贴(&P)"));
	}
	else if (nImageID == IID_FORM || nImageID == IID_REPORT || nImageID == IID_CODEMODULE)
	{
		menu.AppendMenu(MF_STRING, ID_EDIT_CUT, _T("剪切(&T)"));
		menu.AppendMenu(MF_STRING, ID_EDIT_COPY, _T("拷贝(&C)"));
		if (IsClipboardFormatAvailable(CDevDoc::m_cfDocData))
			menu.AppendMenu(MF_STRING, ID_EDIT_PASTE, _T("粘贴(&P)"));
		menu.AppendMenu(MF_STRING, ID_EDIT_CLEAR, _T("删除(&D)"));
	}
	else
	{
		return;
	}

	CMenu menuPopup;
	menuPopup.CreatePopupMenu();
	menuPopup.AppendMenu(MF_STRING, ID_ADD_BLANK_REPORT, _T("空白报表(&B)"));
	menuPopup.AppendMenu(MF_STRING, ID_ADD_REPORT_WIZARD, _T("报表向导(&W)"));

	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, ID_ADD_FORM, _T("插入子窗体(&F)"));
	menu.AppendMenu(MF_STRING|MF_POPUP, (UINT)menuPopup.m_hMenu, _T("插入报表(&R)"));
	menu.AppendMenu(MF_STRING, ID_ADD_CODE, _T("插入脚本模块(&C)"));
	menu.EnableMenuItem(ID_ADD_CODE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	menu.TrackPopupMenu(TPM_LEFTALIGN, curPoint.x, curPoint.y, this);
}

/////////////////////////////////////////////////////////////////////////////
// CExplorerTree drag/drop support

DROPEFFECT CExplorerTree::OnDragScroll(DWORD /*dwKeyState*/, CPoint /*point*/)
{
	return DROPEFFECT_NONE;
}

DROPEFFECT CExplorerTree::OnDragEnter(COleDataObject* pDataObject,
	DWORD dwKeyState, CPoint point)
{
	return OnDragOver(pDataObject, dwKeyState, point);
}

DROPEFFECT CExplorerTree::OnDragOver(COleDataObject* pDataObject,
	DWORD dwKeyState, CPoint /*point*/)
{
	DROPEFFECT de = DROPEFFECT_NONE;

	if (!pDataObject->IsDataAvailable(CDevDoc::m_cfDocData))
		return de;
	
	if ((dwKeyState & MK_CONTROL) == MK_CONTROL)
		de = DROPEFFECT_COPY;
	else
		de = DROPEFFECT_MOVE;

	return de;
}

BOOL CExplorerTree::OnDrop(COleDataObject* pDataObject,
	DROPEFFECT dropEffect, CPoint point)
{
	UINT nFlags;
	HTREEITEM htiSelect = HitTest(point, &nFlags);

	int nImage, nSelectedImage;
	if (!GetItemImage(htiSelect, nImage, nSelectedImage))
		return FALSE;
	if (nImage == IID_MAINFORM || nImage == IID_FORM || nImage == IID_REPORT || nImage == IID_CODEMODULE)
		htiSelect = GetParentItem(htiSelect);
	else if (nImage != IID_PICTURE)
		return FALSE;

	HTREEITEM htiDrag = m_htiDrag;
	if (!GetItemImage(htiDrag, nImage, nSelectedImage))
		return FALSE;
	if (nImage == IID_MAINFORM || nImage == IID_FORM || nImage == IID_REPORT || nImage == IID_CODEMODULE)
		htiDrag = GetParentItem(htiDrag);

	if ((((dropEffect & DROPEFFECT_MOVE) != 0) || ((dropEffect & DROPEFFECT_COPY) != 0)) && htiSelect != htiDrag)
	{
		ASSERT(pDataObject->IsDataAvailable(CDevDoc::m_cfDocData));

		CDevDoc* pDoc = GetPictureItemDocument(htiSelect);
		return pDoc->DoPasteSubDocument(pDataObject);
	}

	return FALSE;
}

DROPEFFECT CExplorerTree::OnDropEx(COleDataObject* /*pDataObject*/,
	DROPEFFECT /*dropEffect*/, DROPEFFECT /*dropEffectList*/, CPoint /*point*/)
{
	return (DROPEFFECT)-1;  // not implemented
}

void CExplorerTree::OnDragLeave()
{
}

void CExplorerTree::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;

	int nImage, nSelectedImage;
	if (!GetItemImage(hItem, nImage, nSelectedImage))
		return;

	if (nImage != IID_MAINFORM && nImage != IID_FORM && nImage != IID_REPORT && nImage != IID_CODEMODULE)
		return;

	CDevDoc* pDoc = NULL;
	CDevDoc::CSubDoc* pSubDoc = GetPictureSubItemDocument(hItem, pDoc);
	ASSERT(pDoc != NULL);
	ASSERT(pSubDoc != NULL);

//	if (pSubDoc->m_pChildDoc == NULL)
//		pDoc->ActivateSubDocument(pSubDoc);

	CSubDocDataSource source;
	source.AddSubDocument(pSubDoc);
	source.DelayRenderData(CDevDoc::m_cfDocData);

	DROPEFFECT dwEffects = DROPEFFECT_COPY;
	if (!pSubDoc->IsFirstSubDoc())
		dwEffects |= DROPEFFECT_MOVE;

	m_htiDrag = hItem;
	dwEffects = source.DoDragDrop(dwEffects);
	if (dwEffects == DROPEFFECT_MOVE)
	{
		m_htiSelect = hItem;
		OnEditDelete(FALSE);
	}
}



