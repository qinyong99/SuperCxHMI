// SymbolLibWnd.cpp : implementation file

#include "stdafx.h"
#include "DevInc.h"
#include "cxdev.h"
#include "BarListCtrl.h"
#include "SymbolLibWnd.h"
#include "SymbolLibDoc.h"
#include "FolderNameDlg.h"

#include "LayoutView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef WM_NCMOUSELEAVE
#define WM_NCMOUSELEAVE                 0x02A2
#endif
#ifndef TME_NONCLIENT
#define TME_NONCLIENT   0x00000010
#endif

#define GLW_ICON_CLOSE	0
#define GLW_ICON_OPEN	1
#define GLW_HEADERSIZE	20
#define GLW_TOOLWIDTH	16
#define GLW_TOOLSTEP	4

/////////////////////////////////////////////////////////////////////////////
// CSymbolLibWnd
CSymbolLibWnd::CSymbolLibWnd()
{
	m_pCurrentDocument = NULL;
	m_isDragBeginer = FALSE;
}

CSymbolLibWnd::~CSymbolLibWnd()
{
}


BEGIN_MESSAGE_MAP(CSymbolLibWnd, CBarListCtrl)
	//{{AFX_MSG_MAP(CSymbolLibWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, CBARLISTCTRL_LISTID, OnItemdblclick)
	ON_NOTIFY(LVN_KEYDOWN, CBARLISTCTRL_LISTID, OnListKeyDown)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_LIB_NEW, OnLibNew)
	ON_COMMAND(ID_LIB_RENAME, OnLibRename)
	ON_COMMAND(ID_LIB_DELETE, OnLibDelete)
	ON_COMMAND(ID_LIB_OPEN, OnLibOpen)
	ON_COMMAND(ID_LIB_PROPERTY, OnLibProperty)
	ON_COMMAND(ID_EDIT_COPY, OnLibCopy)
	ON_COMMAND(ID_EDIT_CUT, OnLibCut)
	ON_COMMAND(ID_EDIT_PASTE, OnLibPaste)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_GETDISPINFO, CBARLISTCTRL_LISTID, OnGetDispInfo)
	ON_NOTIFY(LVN_BEGINDRAG, CBARLISTCTRL_LISTID, OnBegindrag)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolLibWnd message handlers

int CSymbolLibWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBarListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_ImageContentLarge.Create(64, 64, ILC_COLOR24, 1, 1);
	m_ImageContentLarge.SetImageCount(1);
	m_ImageContentSmall.Create(32, 32, ILC_COLOR24, 1, 1);
	m_ImageContentSmall.SetImageCount(1);

	OnUpdate();

	return 0;
}

//////////////////////////////////////////////////////////////
//功能：更新列表中的显示内容
//////////////////////////////////////////////////////////////
void CSymbolLibWnd::OnUpdate()
{
	GetListCtrl()->DeleteAllItems();
	CString strName;
	int i = 0;

	//显示库目录
	if (m_pCurrentDocument == NULL)
	{	
		strName = CSymbolLibDoc::GetDefaultPath() + _T("\\*.");
		strName += CSymbolLibDoc::m_szGLibFileExt;
		CFileFind finder;
		BOOL bOk = finder.FindFile(strName);

		while(bOk)
		{
			bOk = finder.FindNextFile();
			strName = finder.GetFileTitle();
			GetListCtrl()->InsertItem(i++, strName, GLW_ICON_CLOSE);
		}
	}
	//显示库内容
	else
	{	
		POSITION pos = m_pCurrentDocument->GetFirstObjPosition();
		while(pos != NULL)
		{
			CDevObj* pObj = m_pCurrentDocument->GetNextObj(pos);
			int index = GetListCtrl()->InsertItem(i++, pObj->GetDisplayName(), I_IMAGECALLBACK);
			GetListCtrl()->SetItemData(index, (DWORD)pObj);
		}
	}
}


BOOL CSymbolLibWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CBarListCtrl::PreCreateWindow(cs);
}

//////////////////////////////////////////////////////////////
//功能:执行按钮命令
//参数:nButton[in]		第几个按钮
//////////////////////////////////////////////////////////////
void CSymbolLibWnd::DoCommand(int nButton)
{
	switch(nButton)
	{
	case 0:		//返回库目录
		OnBackToRoot();
		break;
	case 1:		//新建图库
		OnLibNew();
		return;
	case 2:		//大图标
		GetListCtrl()->ModifyStyle(LVS_SMALLICON | LVS_LIST | LVS_REPORT, LVS_ICON);
		break;
	case 3:		//小图标
		GetListCtrl()->ModifyStyle(LVS_LIST | LVS_REPORT | LVS_ICON, LVS_SMALLICON);
		break;
	case 4:		//报表形式
		GetListCtrl()->ModifyStyle(LVS_REPORT | LVS_ICON | LVS_SMALLICON, LVS_LIST);
		break;
	case 5:		//详细资料
		break;
	default:	//未知
		break;
	}

	CBarListCtrl::DoCommand(nButton);
}

//////////////////////////////////////////////////////////
//功能:创建一个新图库
//////////////////////////////////////////////////////////
void CSymbolLibWnd::OnLibNew()
{
	if (m_pCurrentDocument != NULL)	//显示图库内容时不能创建新图库
		return;

	CSymbolLibDoc* pNewDoc = NULL;
	CFolderNameDlg dlg;
	dlg.m_strTitle = _T("输入图符库名称");

	while (dlg.DoModal() == IDOK)
	{
		LVFINDINFO info;
		info.flags = LVFI_STRING;
		info.psz = dlg.m_strFolderName.LockBuffer();

		if(GetListCtrl()->FindItem(&info) != -1)	//有同名文件
		{
			dlg.m_strFolderName.UnlockBuffer();
			continue;
		}
		dlg.m_strFolderName.UnlockBuffer();

		pNewDoc = CSymbolLibDoc::NewLib(dlg.m_strFolderName);
		break;
	}

	if (pNewDoc != NULL) 
	{
		if (m_pCurrentDocument != NULL)
		{
			m_pCurrentDocument->SaveLib();
			m_pCurrentDocument->CloseLib();
			delete m_pCurrentDocument;
		}
		m_pCurrentDocument = pNewDoc;
	}

	OnBackToRoot();
}

void CSymbolLibWnd::OnDestroy() 
{
	CBarListCtrl::OnDestroy();
	
	if (m_pCurrentDocument != NULL)
	{
// 		m_pCurrentDocument->SaveLib();
		m_pCurrentDocument->CloseLib();
		delete m_pCurrentDocument;
		m_pCurrentDocument = NULL;
	}
}

void CSymbolLibWnd::OnItemdblclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE* phdn = (NMITEMACTIVATE *)pNMHDR;
	
	*pResult = 0;
	if (phdn->iItem < 0)
		return;

	if (m_pCurrentDocument == NULL)
		OnLibOpen();
	else
		OnLibProperty();
}

void CSymbolLibWnd::OnListKeyDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	
	if (pLVKeyDow->wVKey == VK_DELETE)
		OnLibDelete();

	pResult = 0;
}

void CSymbolLibWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menu;
	menu.CreatePopupMenu();

	int nSelectCount = GetListCtrl()->GetSelectedCount();

	//图符库文档没有打开
	if (m_pCurrentDocument == NULL)
	{
		menu.AppendMenu(MF_STRING, ID_LIB_NEW, _T("新建(&N)"));
		if (nSelectCount == 0)
		{
		}
		else if (nSelectCount == 1)
		{
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_LIB_OPEN, _T("打开(&O)"));
			menu.AppendMenu(MF_STRING, ID_LIB_DELETE, _T("删除(&D)"));
			menu.AppendMenu(MF_STRING, ID_LIB_RENAME, _T("重命名(&R)"));
		}
		else
		{
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_LIB_DELETE, _T("删除(&D)"));
		}
	}
	else
	{
		if (nSelectCount > 0)
		{
			menu.AppendMenu(MF_STRING, ID_EDIT_CUT, _T("剪切(&T)"));
			menu.AppendMenu(MF_STRING, ID_EDIT_COPY, _T("拷贝(&C)"));
		}
		if (IsClipboardFormatAvailable(CLayoutView::m_cfItemData))
			menu.AppendMenu(MF_STRING, ID_EDIT_PASTE, _T("粘贴(&P)"));
		if (nSelectCount > 0)
			menu.AppendMenu(MF_STRING, ID_LIB_DELETE, _T("删除(&D)"));
		
		if (nSelectCount == 1)
		{
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_LIB_RENAME, _T("重命名(&R)"));
			POSITION pos = GetListCtrl()->GetFirstSelectedItemPosition();
			int nItem = GetListCtrl()->GetNextSelectedItem(pos);
			CDevObj* pObj = (CDevObj *)(GetListCtrl()->GetItemData(nItem));
			if (pObj->IsKindOf(RUNTIME_CLASS(CSymbolObj)))
				menu.AppendMenu(MF_STRING, ID_LIB_PROPERTY, "设计图符(&R)");
			else
				menu.AppendMenu(MF_STRING, ID_LIB_PROPERTY, "生成图符(&B)");
		}
	}

	menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

void CSymbolLibWnd::OnLibRename() 
{
//	ASSERT(m_pCurrentDocument == NULL);	//当前应处于库目录显示状态中
	ASSERT(GetListCtrl()->GetSelectedCount() == 1);

	POSITION pos = GetListCtrl()->GetFirstSelectedItemPosition();
	int nItem = GetListCtrl()->GetNextSelectedItem(pos);
	
	TCHAR szBuf[32];
	GetListCtrl()->GetItemText(nItem, 0, szBuf, 32);
	CString strName = szBuf;

	CFolderNameDlg dlg;
	if (m_pCurrentDocument == NULL)
		dlg.m_strTitle = _T("输入图符库名称");
	else
		dlg.m_strTitle = _T("输入图符名称");

	dlg.m_strFolderName = GetListCtrl()->GetItemText(nItem, 0);

	while (dlg.DoModal() == IDOK)
	{
		LVFINDINFO info;
		info.flags = LVFI_STRING;
		info.psz = dlg.m_strFolderName.LockBuffer();

		int nFindItem = GetListCtrl()->FindItem(&info);
		if (nFindItem != -1 && nFindItem != nItem)	//有同名文件
		{
			dlg.m_strFolderName.UnlockBuffer();
			continue;
		}
		dlg.m_strFolderName.UnlockBuffer();
		if (m_pCurrentDocument == NULL)
		{
			CSymbolLibDoc::RenameLib(strName, dlg.m_strFolderName);
			OnUpdate();
		}
		else
		{
			CDevObj* pObj = (CDevObj*)(GetListCtrl()->GetItemData(nItem));
			ASSERT(pObj != NULL);

			pObj->PutDisplayName(dlg.m_strFolderName, FALSE);
			GetListCtrl()->SetItemText(nItem, 0, pObj->GetDisplayName());
			m_pCurrentDocument->SaveLib();
		}
		
		break;
	}
}

void CSymbolLibWnd::OnLibCopy()
{
	COleDataSource* pDataSource = PrepareDataSource();
	pDataSource->SetClipboard();
}

void CSymbolLibWnd::OnLibCut()
{
	OnLibCopy();
	LibDelete(FALSE);
}

void CSymbolLibWnd::OnLibPaste()
{
	DoPaste();
}

void CSymbolLibWnd::LibDelete(BOOL bPrompt)
{
	POSITION pos = GetListCtrl()->GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;
	
	int nItem;
	
	if (m_pCurrentDocument == NULL)
	{	
		if (bPrompt && MessageBox(_T("确实要删除选中的图符库吗？"), _T("确认图符库删除"), MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;

		//删除库
		TCHAR buffer[MAX_PATH];
		CString strName;
		while (pos)
		{
			nItem = GetListCtrl()->GetNextSelectedItem(pos);
			GetListCtrl()->GetItemText(nItem, 0, buffer, MAX_PATH);
			strName = buffer;
			CSymbolLibDoc::DeleteLib(strName);
		}
		OnUpdate();
	}
	else
	{	
		if (bPrompt && MessageBox(_T("确实要删除选中的图符吗？"), _T("确认图符删除"), MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;

		//删除库中的对象
		CDevObj* pObj;
		while (pos)
		{
			nItem = GetListCtrl()->GetNextSelectedItem(pos);
			pObj = (CDevObj *)(GetListCtrl()->GetItemData(nItem));
			if (!m_pCurrentDocument->DeleteObject(pObj))
				MessageBox(_T("删除图符出错！"));
		}
		m_pCurrentDocument->SaveLib();
		OnUpdate();
	}
}

void CSymbolLibWnd::OnLibDelete() 
{
	LibDelete();
}

void CSymbolLibWnd::OnLibOpen() 
{
	ASSERT(m_pCurrentDocument == NULL);	//当前应处于库目录显示状态中
	AfxGetApp()->DoWaitCursor(1);

	//打开库文档
	POSITION pos = GetListCtrl()->GetFirstSelectedItemPosition();
	int index = GetListCtrl()->GetNextSelectedItem(pos);
	if (index < 0)
		return;
	TCHAR buffer[40];
	GetListCtrl()->GetItemText(index, 0, buffer, 40);
	CString strName = buffer;

	CSymbolLibDoc* pDoc = new CSymbolLibDoc();
	if (pDoc == NULL)
		AfxThrowMemoryException();
	pDoc->OpenLib(strName);
	
	//设置新图标大小
	GetListCtrl()->SetImageList(&m_ImageContentSmall, LVSIL_SMALL);
	GetListCtrl()->SetImageList(&m_ImageContentLarge, LVSIL_NORMAL);

	//更新显示
	m_pCurrentDocument = pDoc;
	OnUpdate();
	DoCommand(-1);

	AfxGetApp()->DoWaitCursor(-1);
}

///////////////////////////////////////////////////////
//功能:当前显示内容加到库目录
///////////////////////////////////////////////////////
void CSymbolLibWnd::OnBackToRoot()
{
	if (m_pCurrentDocument != NULL)
	{
//		m_pCurrentDocument->SaveLib();
		if (!m_pCurrentDocument->CloseLib())
			AfxMessageBox(_T("图库文件保存错误!"));
		delete m_pCurrentDocument;
		m_pCurrentDocument = NULL;
	}

	//更改图标大小
	GetListCtrl()->SetImageList(&m_listImage, LVSIL_SMALL);
	GetListCtrl()->SetImageList(&m_listImageLarge, LVSIL_NORMAL);

	OnUpdate();
}

/////////////////////////////////////////////////////////////////////////
//功能:执行放操作
//参数:pWnd[in]			当前光标所在窗口
//		pDataObject[in]	当前要准备放下的数据
//		dropDefault[in]	当前的默认动作
//		dropList[in]	可以执行的动作列表
//		point[in]		当前相对屏幕的光标位置
//返回: 成功TRUE,失败FALSE
/////////////////////////////////////////////////////////////////////////
BOOL CSymbolLibWnd::DoDrop(CWnd *pWnd, COleDataObject *pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point)
{
	BOOL bRet = FALSE;
	if (pDataObject == NULL || m_pCurrentDocument == NULL)
		return bRet;

	STGMEDIUM stgmedium;
	if (!pDataObject->GetData(CLayoutView::m_cfItemData, &stgmedium))
		return bRet;

	ASSERT(stgmedium.tymed == TYMED_ISTORAGE);

	IStoragePtr pStorage;
	COleStreamFile file;
	CDevObj* pObj = NULL;
	int nObjects = 0;
	pStorage = stgmedium.pstg;
	ReleaseStgMedium(&stgmedium);

	if (!file.OpenStream(pStorage, _T("Contents"), STGM_READ|
		STGM_SHARE_EXCLUSIVE))
	{
		return bRet;
	}

	CArchive ar(&file, CArchive::load|CArchive::bNoFlushOnDelete);
	return CreateSymbolObject(m_pCurrentDocument, ar, pStorage);
}

/////////////////////////////////////////////////////////////////////////
//功能:在拖放期间确定光标的型状
//参数:pWnd[in]			当前光标所在窗口
//		pDataObject[in]	当前要准备放下的数据
//		dropDefault[in]	当前的默认动作
//		dropList[in]	可以执行的动作列表
//		point[in]		当前相对屏幕的光标位置
//返回:代表光型状的值
/////////////////////////////////////////////////////////////////////////
DROPEFFECT CSymbolLibWnd::FindDragEffect(CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
{
	DROPEFFECT dfRet = DROPEFFECT_NONE;
	if(m_isDragBeginer)	//是自己开始拖放操作的
		return dfRet;

	if (m_pCurrentDocument != NULL && pDataObject->IsDataAvailable(CLayoutView::m_cfItemData))
		dfRet = DROPEFFECT_COPY;
	return dfRet;
}

////////////////////////////////////////////////////////////////////////
//功能:按指定大小和位置显示对象的辅助函数
//参数:rect[in]		位置
//		pdc[in]		设备
//		pObj[in]	对象
///////////////////////////////////////////////////////////////////////
static void DispObjAsIcon(CDC* pdc, CRect* pRect, CDevObj* pObj)
{
	ASSERT(pdc);
	ASSERT(pRect);
	ASSERT(pObj);

	int nkeep = pdc->SaveDC();

	CRect rcObj = pObj->GetPositionRect().GetRECT();
	int nBorder = pRect->Width() < pRect->Height() ?  pRect->Width() : pRect->Height();
	nBorder -= 2;

	POINT ptViewOrg, ptViewExt, ptWndOrg, ptWndExt;
	ptViewOrg.x = pRect->left + (pRect->Width() - nBorder) / 2;
	ptViewOrg.y = pRect->top + (pRect->Height() - nBorder) / 2;
	ptViewExt.x = nBorder;
	ptViewExt.y = nBorder;
	nBorder = rcObj.Width() > rcObj.Height() ? rcObj.Width() : rcObj.Height();
	ptWndOrg.x = rcObj.left + (rcObj.Width() - nBorder) / 2;
	ptWndOrg.y = rcObj.top + (rcObj.Height() - nBorder) / 2;
	ptWndExt.x = nBorder;
	ptWndExt.y = nBorder;

	pdc->SetMapMode(MM_ANISOTROPIC);
	pdc->SetViewportOrg(ptViewOrg.x, ptViewOrg.y);
	pdc->SetWindowOrg(ptWndOrg.x, ptWndOrg.y);
	pdc->SetViewportExt(ptViewExt.x, ptViewExt.y);
	pdc->SetWindowExt(ptWndExt.x, ptWndExt.y);

	pObj->Draw(pdc);

	pdc->RestoreDC(nkeep);
}

////////////////////////////////////////////////////////////////////////
//功能:在显示图符库内容时, 提供显示图形给对象
////////////////////////////////////////////////////////////////////////
void CSymbolLibWnd::OnGetDispInfo(NMHDR * pNotifyStruct, LRESULT * result)
{
	if (m_pCurrentDocument != NULL)
	{
		NMLVDISPINFO* pInfo = (NMLVDISPINFO *)pNotifyStruct;
		int index = pInfo->item.iItem;

		if ((pInfo->item.mask & LVIF_IMAGE) != 0)
		{	//需要显示图形
			CDevObj* pObj = (CDevObj*)(GetListCtrl()->GetItemData(index));
			UpdateImage(pObj);
			pInfo->item.iImage = 0;
		}
	}

	*result = 0;
}


void CSymbolLibWnd::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW * phdn = (NMLISTVIEW *) pNMHDR;
	
	if (m_pCurrentDocument != NULL)
	{
		COleDataSource* pData = PrepareDataSource(/*phdn->iItem*/);
		if(pData != NULL)
		{
			m_isDragBeginer = TRUE;
			DROPEFFECT efBack = pData->DoDragDrop(DROPEFFECT_COPY);
			delete pData;
			m_isDragBeginer = FALSE;
		}
	}

	*pResult = 0;
}

/////////////////////////////////////////////////////////////
//功能:为拖放操作准备数据对象
//参数:nItem[in]	要拖放的项索引
//返回:成功OLE数据对象指针,失败NULL
//说明:返回的对象使用后要用new删除
/////////////////////////////////////////////////////////////
COleDataSource* CSymbolLibWnd::PrepareDataSource()
{
	ASSERT(m_pCurrentDocument != NULL);

	int nSelectedObjects = GetListCtrl()->GetSelectedCount();
	if (nSelectedObjects < 1)
		return NULL;
	
	STGMEDIUM stgMedium;
	COleDataSource* pDataSource;
	
	pDataSource = new COleDataSource;
	if(pDataSource == NULL)
	{
		AfxThrowMemoryException();
	}

	if (!GetSelectedObjectData(&stgMedium))
	{
		delete pDataSource;
		return NULL;
	}

	pDataSource->CacheData(CLayoutView::m_cfItemData, &stgMedium);

	GetDragInfoData(&stgMedium);
	pDataSource->CacheData(CLayoutView::m_cfDragInfo, &stgMedium);

	return pDataSource;
}

////////////////////////////////////////////////////////////
//功能:获取指定对象的数据到交换格式
//参数:pObj[in]			要获取数据的对象
//		lpStgMedium[in]	数据存放地
////////////////////////////////////////////////////////////
BOOL CSymbolLibWnd::GetSelectedObjectData(LPSTGMEDIUM lpStgMedium)
{
	ASSERT(lpStgMedium != NULL);
	ASSERT(m_pCurrentDocument != NULL);
	
	int nSelectedObjects = GetListCtrl()->GetSelectedCount();
	if (nSelectedObjects < 1)
		return FALSE;
	
	IStoragePtr pStorage;
	COleStreamFile file;

	HRESULT hResult = StgCreateDocfile(NULL, STGM_CREATE|STGM_READWRITE|
		STGM_DELETEONRELEASE|STGM_SHARE_EXCLUSIVE, 0, &pStorage);
	if (FAILED(hResult))
	{
		return FALSE;
	}

	if (!file.CreateStream(pStorage, _T("Contents"), CFile::modeReadWrite|
		CFile::shareExclusive|CFile::modeCreate))
	{
		pStorage->Release();
		return FALSE;
	}

	CArchive ar(&file, CArchive::store);

	ar << nSelectedObjects;
	
	int nItem;
	CDevObj* pObj;
	POSITION pos = GetListCtrl()->GetFirstSelectedItemPosition();
	while (pos)
	{
		nItem = GetListCtrl()->GetNextSelectedItem(pos);
		pObj = (CDevObj *)(GetListCtrl()->GetItemData(nItem));
		ar << int(pObj->GetObjectType());
		pObj->CopyToClipboard(ar, pStorage);
	}

	lpStgMedium->tymed = TYMED_ISTORAGE;
	lpStgMedium->pstg = pStorage;
	lpStgMedium->pstg->AddRef();
	lpStgMedium->pUnkForRelease = NULL;

	ar.Flush();
	file.Close();

	return TRUE;
}

//void CSymbolLibWnd::AlignToGrid(CPoint& point)
//{
//	if (theApp.m_settingGen.m_bAlignToGrid)
//	{
//		int nGrindWidth = theApp.m_settingGen.m_nGridSpaceWidth;
//		point.x = int((point.x + nGrindWidth / 2) / nGrindWidth) * nGrindWidth;
//		int nGrindHeight = theApp.m_settingGen.m_nGridSpaceHeight;
//		point.y = int((point.y + nGrindHeight / 2) / nGrindHeight) * nGrindHeight;
//	}


/////////////////////////////////////////////////////////////////////
//功 能：取拖放信息
/////////////////////////////////////////////////////////////////////
void CSymbolLibWnd::GetDragInfoData(LPSTGMEDIUM lpStgMedium)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpStgMedium, sizeof(STGMEDIUM)));

	int nItem;
	CDevObj* pObj;
	CRect rcBound;
	rcBound.SetRectEmpty();

	CClientDC dc(this);
	dc.BeginPath();

	POSITION pos = GetListCtrl()->GetFirstSelectedItemPosition();
	while (pos)
	{
		nItem = GetListCtrl()->GetNextSelectedItem(pos);
		pObj = (CDevObj *)(GetListCtrl()->GetItemData(nItem));
		CRect rcObj = pObj->GetPositionRect().GetRECT();
		rcObj.NormalizeRect();
		rcBound.UnionRect(rcBound, rcObj);

		pObj->DrawTrackerShape(&dc);
	}

	dc.EndPath();
	

	CPoint ptStart = rcBound.CenterPoint();
	theApp.AlignToGrid(ptStart);
	
	LPSTREAM lpStream;
	if (::CreateStreamOnHGlobal(NULL, TRUE, &lpStream) != S_OK)
		AfxThrowMemoryException();
	
	ASSERT(lpStream != NULL);
	
	lpStream->Write(&rcBound, sizeof(CRect), NULL);
	lpStream->Write(&ptStart, sizeof(CPoint), NULL);
	
	int iNumPoints = dc.GetPath(NULL, NULL, 0);
	
	CPoint* pPoints = new CPoint[iNumPoints];
	if (pPoints == NULL)
		AfxThrowMemoryException();
	BYTE* pTypes = new BYTE[iNumPoints];
	if (pTypes == NULL)
		AfxThrowMemoryException();
	
	iNumPoints = dc.GetPath(pPoints, pTypes, iNumPoints);
	
	lpStream->Write(&iNumPoints, sizeof(int), NULL);
	lpStream->Write(pPoints, sizeof(CPoint) * iNumPoints, NULL);
	lpStream->Write(pTypes, sizeof(BYTE) * iNumPoints, NULL);

	if (pPoints != NULL)
		delete pPoints;
	if (pTypes != NULL)
		delete pTypes;
	
	// setup the STGMEDIUM
	lpStgMedium->tymed = TYMED_ISTREAM;
	lpStgMedium->pstm = lpStream;
	lpStgMedium->pUnkForRelease = NULL;
}

/////////////////////////////////////////////////////////////;
//功能:创建非默认的大小图标
//返回:TRUE已创建图标,不使用默认图标, FALSE未创建,使用默认图标
//////////////////////////////////////////////////////////////
BOOL CSymbolLibWnd::PreCreateImage(CImageList *pImageLarge, CImageList *pImageSmall)
{
	return FALSE;
}

void CSymbolLibWnd::UpdateImage(CDevObj *pObj)
{
	IMAGEINFO info;
	
	//大图标
	m_ImageContentLarge.GetImageInfo(0, &info);
	CDC* pdc = GetDC();
	CDC dc;
	dc.CreateCompatibleDC(pdc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pdc, info.rcImage.right - info.rcImage.left,
		info.rcImage.bottom - info.rcImage.top);
	CGdiObject* pOldBmp = dc.SelectObject(&bmp);
	ReleaseDC(pdc);

	CRect rect = info.rcImage;
	dc.FillSolidRect(&rect, GetSysColor(COLOR_WINDOW));
	DispObjAsIcon(&dc, &rect, pObj);
	dc.SelectObject(pOldBmp);
	m_ImageContentLarge.Replace(0, &bmp, NULL);

	bmp.DeleteObject();

	//小图标
	pdc = GetDC();
	m_ImageContentSmall.GetImageInfo(0, &info);
	bmp.CreateCompatibleBitmap(pdc, info.rcImage.right - info.rcImage.left,
		info.rcImage.bottom - info.rcImage.top);
	ReleaseDC(pdc);
	pOldBmp = dc.SelectObject(&bmp);

	rect = info.rcImage;
	dc.FillSolidRect(&rect, RGB(255, 255, 255));
	DispObjAsIcon(&dc, &rect, pObj);
	dc.SelectObject(pOldBmp);
	m_ImageContentSmall.Replace(0, &bmp, NULL);

	bmp.DeleteObject();

	dc.DeleteDC();
}

/////////////////////////////////////////////////////////
//功能:编辑对象扩展属性
/////////////////////////////////////////////////////////
void CSymbolLibWnd::OnLibProperty() 
{
	//修改图符属性
	if (m_pCurrentDocument != NULL)
	{
		POSITION pos = GetListCtrl()->GetFirstSelectedItemPosition();
		int index = GetListCtrl()->GetNextSelectedItem(pos);
		ASSERT(index >= 0);
		CDevObj* pObj = (CDevObj*)(GetListCtrl()->GetItemData(index));
		ASSERT(pObj != NULL);

		BOOL bChange = FALSE;
		if (!pObj->IsKindOf(RUNTIME_CLASS(CSymbolObj)))
		{
			pObj = m_pCurrentDocument->ConvertObjectToSymbol(pObj);
			GetListCtrl()->SetItemData(index, DWORD(pObj));
			bChange = TRUE;
		}
		ASSERT(pObj->IsKindOf(RUNTIME_CLASS(CSymbolObj)));
		
		if (((CSymbolObj *)pObj)->BeginDesign(this))
			bChange = TRUE;

		if (bChange)
		{
			GetListCtrl()->SetItemText(index, 0, pObj->GetDisplayName());
			UpdateImage(pObj);
			GetListCtrl()->RedrawItems(index, index);
			m_pCurrentDocument->SaveLib();
		}
	}
}

///////////////////////////////////////////////////////////////
//功能:返回是否能加入图符对象
///////////////////////////////////////////////////////////////
BOOL CSymbolLibWnd::CanAddSymbol()
{
	return  m_pCurrentDocument != NULL;
}

///////////////////////////////////////////////////////////////
//功能:从剪贴板上粘贴组合对象
///////////////////////////////////////////////////////////////
BOOL CSymbolLibWnd::DoPaste()
{
	BOOL bRet = FALSE;
	COleDataObject data;
	if (m_pCurrentDocument == NULL || !data.AttachClipboard())
		return bRet;

	STGMEDIUM stgmedium;
	if(!data.GetData(CLayoutView::m_cfItemData, &stgmedium))
		return bRet;

	ASSERT(stgmedium.tymed == TYMED_ISTORAGE);

	IStoragePtr pStorage;
	COleStreamFile file;
	CDevObj* pObj = NULL;
	int nObjects = 0;
	pStorage = stgmedium.pstg;
	ReleaseStgMedium(&stgmedium);

	if (!file.OpenStream(pStorage, _T("Contents"), STGM_READ|
		STGM_SHARE_EXCLUSIVE))
	{
		return bRet;
	}

	CArchive ar(&file, CArchive::load|CArchive::bNoFlushOnDelete);

	CreateSymbolObject(m_pCurrentDocument, ar, pStorage);

	ar.Flush();
	file.Close();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//功能:从Archive的数据中创建图符对象
//参数:pDoc[in]			图符库对象指针
//		arData[in] 		图符数据
//		pStorage[in]	装图符数据的贮存接口
//////////////////////////////////////////////////////////////////////////
BOOL CSymbolLibWnd::CreateSymbolObject(CSymbolLibDoc *pDoc, CArchive &ar, IStoragePtr& pStorage)
{
	ASSERT(pDoc != NULL && pDoc == m_pCurrentDocument);

	int nObjType;
	ar.m_pDocument = pDoc;
	int nObjects;
	CDevObj* pObj;

	ar >> nObjects;
	for (int i = 0; i < nObjects; i++)
	{
		ar >> nObjType;
		switch (nObjType)
		{
		case CDevObj::groupobj:
			pObj = new CGroupObj(pDoc);
			break;
		case CDevObj::symbolobj:
			pObj = new CSymbolObj(pDoc);
			break;
		case CDevObj::ctrlobj:
			pObj = new CCtrlObj(pDoc);
			break;
		case CDevObj::drawobj:
			pObj = new CDrawObj(pDoc);
			break;
		case CDevObj::formobj:
			pObj = new CFormObj(pDoc);
			break;
		case CDevObj::reportobj:
			pObj = new CReportObj(pDoc);
			break;
		default:
			ASSERT(FALSE);
		}
		
		ASSERT(pObj != NULL);
		if (!pObj->CreateFromClipboard(ar, pStorage))
		{
			AfxThrowOleException(E_FAIL);
		}
	}
	
	m_pCurrentDocument->SaveLib(); //立即保存

	OnUpdate();
	EnsureObjectVisible(pObj);

	return TRUE;
}

///////////////////////////////////////////////////////////////
//功能:确保选定项目可见
///////////////////////////////////////////////////////////////
void CSymbolLibWnd::EnsureObjectVisible(CDevObj *pObj)
{
	if (pObj == NULL)
		return;

	CListCtrl* pCtrl = GetListCtrl();
	int nCount = pCtrl->GetItemCount();
	CDevObj* pSearchObj;
	for(int i = 0; i < nCount; i++)
	{
		pSearchObj = (CDevObj*)(GetListCtrl()->GetItemData(i));
		if (pObj == pSearchObj)
		{
			pCtrl->EnsureVisible(i, FALSE);
			return;
		}
	}
}

