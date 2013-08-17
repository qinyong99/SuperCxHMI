// DevDoc.cpp : implementation file
//

#include "stdafx.h"
#include "DevInc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDevDoc

UINT CDevDoc::m_nUntitledCount = 0;
CPtrList CDevDoc::c_listDevDoc;
CLIPFORMAT CDevDoc::m_cfDocData = (CLIPFORMAT)::RegisterClipboardFormat(_T("CxDev DocData"));

//UINT CDevDoc::m_nUntitledCount = 0;

IMPLEMENT_DYNCREATE(CDevDoc, COleDocument)

CDevDoc::CDevDoc() 
{
	m_lpOrigStg = NULL;
	m_dwVer = 0;
	m_bNeedSave = FALSE;
	m_dwNextDocumentNumber = 0;
	EnableCompoundFile();
}

CDevDoc* CDevDoc::CreateNewDocument(BOOL bAddMainForm)
{
	CDevDoc* pDoc = new CDevDoc();
	if (!pDoc->OnNewDocument())
	{
		delete pDoc;
		return NULL;
	}

	c_listDevDoc.AddTail(pDoc);

	if (bAddMainForm)
		pDoc->AddSubDocument(enumFormDoc);
	
	return pDoc;
}

void CDevDoc::CloseAllDocuments(BOOL bEndSession)
{
	//注意以下的方式是避免一些更新资源浏览器刷新的问题
	POSITION pos = c_listDevDoc.GetTailPosition();
	while (pos != NULL)
	{
		CDevDoc* pDoc = (CDevDoc*)c_listDevDoc.GetPrev(pos);
		c_listDevDoc.RemoveTail();
		pDoc->RemoveAllSubDocument();
		pDoc->OnCloseDocument(); 
	}
//	c_listDevDoc.RemoveAll();
}

/////////////////////////////////////////////////////////////////
//功能:恢复所有的子文档为要求保存状态
/////////////////////////////////////////////////////////////////
void CDevDoc::ResetNeedSaveAll()
{
	POSITION pos = c_listDevDoc.GetHeadPosition();
	while(pos != NULL)
	{
		CDevDoc* pDoc = (CDevDoc *)c_listDevDoc.GetNext(pos);
		pDoc->m_bNeedSave = FALSE;
		POSITION subPos = pDoc->m_listSubDoc.GetHeadPosition();
		while(subPos != NULL)
		{
			CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc*)(pDoc->m_listSubDoc.GetNext(subPos));
			pSubDoc->m_bNeedSave = TRUE;
		}
	}
}

///////////////////////////////////////////////////////////////////
//功 能：保存所有工程文档
//描 述：弹出保存提示对话框，选择需要保存的工程和子文件
///////////////////////////////////////////////////////////////////
BOOL CDevDoc::SaveAllModified(BOOL bDoPrompt)
{
	BOOL bSave = FALSE;
	CSavePromptDlg dlg;
	int iRet = IDYES;

	if (bDoPrompt) //用户要指定选择保存的子文档
	{	
		iRet = dlg.DoModal();
	}

	switch (iRet)
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		bSave = TRUE;
		// If so, either Save or Update, as appropriate
//		if (!DoFileSave())
//			return FALSE;       // don't continue
		break;

	case IDNO:
		return TRUE;
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	if (bSave)
	{
		POSITION pos = c_listDevDoc.GetHeadPosition();
		while (pos != NULL)
		{
			CDevDoc* pDoc = (CDevDoc*)c_listDevDoc.GetNext(pos);
			if (pDoc->m_bNeedSave)
			{
				if (!pDoc->DoFileSave())
				{
					ResetNeedSaveAll();
					return FALSE;
				}
			}
		}
	}

	ResetNeedSaveAll();

	return TRUE;
}

///////////////////////////////////////////////////////////////////
//功 能：打开画面文件
//描 述：首先在以打开的画面文件查找是否相同文件已经打开，如果没有打开，
//打开此文件，否则激活已经打开的文件框架
///////////////////////////////////////////////////////////////////
CDocument* CDevDoc::OpenDocumentFile(LPCTSTR lpszFileName)
{
	CDocument* pDoc = FindDocument(lpszFileName);

	//文档已经打开
	if (pDoc != NULL)
	{
		POSITION pos = pDoc->GetFirstViewPosition();
		if (pos != NULL)
		{
			CView* pView = pDoc->GetNextView(pos); // get first one
			ASSERT_VALID(pView);
			CFrameWnd* pFrame = pView->GetParentFrame();
			if (pFrame != NULL)
				pFrame->ActivateFrame();
		}
	
		return pDoc;
	}

	pDoc = new CDevDoc();
	if (pDoc == NULL)
		return NULL;
	ASSERT_VALID(pDoc);

	// open an existing document
	CWaitCursor wait;
	if (!pDoc->OnOpenDocument(lpszFileName))
	{
		delete pDoc;
		return NULL;
	}
	
	c_listDevDoc.AddTail(pDoc);

	return pDoc;
}

///////////////////////////////////////////////////////////////////
//功 能：根据文件路径查找此文档是否打开，如果打开，返回此文档
///////////////////////////////////////////////////////////////////
CDocument* CDevDoc::FindDocument(LPCTSTR lpszPathName)
{
	POSITION pos = c_listDevDoc.GetHeadPosition();
	while (pos != NULL)
	{
		CDevDoc* pDoc = (CDevDoc*)c_listDevDoc.GetNext(pos);
		if (pDoc->GetPathName().CompareNoCase(lpszPathName) == 0)
		{
			//已经打开
			return pDoc;
		}
	}

	CString strDisplayName = lpszPathName;
	int i = strDisplayName.ReverseFind('/');
	int j = strDisplayName.ReverseFind('\\');
	if (i < j)
		i = j;
	if (i < 0)
		return NULL;

	strDisplayName = strDisplayName.Mid(i + 1, strDisplayName.GetLength() - i - 1);
	LPTSTR lpszExt = _tcsrchr(strDisplayName, '.');
	if (lpszExt != NULL)
		*lpszExt = 0;       // no suffix

	pos = c_listDevDoc.GetHeadPosition();
	while (pos != NULL)
	{
		CDevDoc* pDoc = (CDevDoc*)c_listDevDoc.GetNext(pos);
		if (pDoc->GetDisplayName().CompareNoCase(strDisplayName) == 0)
		{
			//已经打开
			return pDoc;
		}
	}

	return NULL;
}

BOOL CDevDoc::IsUniqueSubDocumentName(LPCTSTR szName)
{
	POSITION pos = m_listSubDoc.GetHeadPosition();
	while (pos != NULL)
	{
		CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc *)m_listSubDoc.GetNext(pos); 
		if (szName == pSubDoc->m_strName)
			return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////
//功 能：创建唯一的子文档名
///////////////////////////////////////////////////////////////////
void CDevDoc::CreateUniqueSubDocumentName(LPCTSTR pszBaseName, CString& strUniqueName)
{
	ULONG nTag;
	BOOL bCollisionFound;

	ASSERT(pszBaseName != NULL);

	// Assume the name is already unique
	strUniqueName.Format("%s%u", pszBaseName, 1);

	nTag = 1;
	do
	{
		bCollisionFound = FALSE;

		POSITION pos = m_listSubDoc.GetHeadPosition();
		while (pos != NULL && !bCollisionFound)
		{
			CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc *)m_listSubDoc.GetNext(pos); 
			if (strUniqueName == pSubDoc->m_strName)
			{
				bCollisionFound = TRUE;
				strUniqueName.Format("%s%u", pszBaseName, nTag);
				nTag++;
			}
		}
	} while (bCollisionFound);
}

/////////////////////////////////////////////////////////
//功 能：重载此函数实现工程文件保存处理
//描 述：原来保存的文件后缀名有文档模板产生
/////////////////////////////////////////////////////////
BOOL CDevDoc::DoSave(LPCTSTR pszPathName, BOOL bReplace /*=TRUE*/)
	// Save the document data to a file
	// pszPathName = path name where to save document file
	// if pszPathName is NULL then the user will be prompted (SaveAs)
	// note: pszPathName can be different than 'm_strPathName'
	// if 'bReplace' is TRUE will change file name if successful (SaveAs)
	// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	CString newName = pszPathName;
	if (newName.IsEmpty())
	{
		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(" #%;/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

		//	newName += ".grf"; //加入画面文件的后缀名

			CString strFullPath = theApp.GetIniStartPictureFolder();
			strFullPath += newName;
			newName = strFullPath;
		}

		if (!AfxGetApp()->DoPromptFileName(newName,
			  bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
			  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, NULL))
				return FALSE;       // don't even attempt to save
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (pszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
				do 
				{ 
					e->Delete(); 
				} while (0);
			}
			END_CATCH_ALL
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
	{
		CString strOld = GetDisplayName();
		SetPathName(newName);

		SetDefaultTitle();
		
		// 子文档同时更新
		CDevDoc::CSubDoc* pSubDoc;
		POSITION pos = m_listSubDoc.GetHeadPosition();
		while (pos != NULL)
		{
			pSubDoc = (CDevDoc::CSubDoc *)m_listSubDoc.GetNext(pos);
			pSubDoc->OnParentDocTitleChanged();
		}
	
		CString strNew = GetDisplayName();

		if (strOld != strNew)
		{
			CDocHit hit(CDocHit::changeRename, strNew, strOld);
			theApp.NotifyDocumentChanged(DWORD(this), DWORD(&hit));
		}
	}

	return TRUE;        // success
}

BOOL AFXAPI AfxComparePath(LPCTSTR lpszPath1, LPCTSTR lpszPath2)
{
	// use case insensitive compare as a starter
	if (lstrcmpi(lpszPath1, lpszPath2) != 0)
		return FALSE;
	
	// on non-DBCS systems, we are done
	if (!GetSystemMetrics(SM_DBCSENABLED))
		return TRUE;
	
	// on DBCS systems, the file name may not actually be the same
	// in particular, the file system is case sensitive with respect to
	// "full width" roman characters.
	// (ie. fullwidth-R is different from fullwidth-r).
	int nLen = lstrlen(lpszPath1);
	if (nLen != lstrlen(lpszPath2))
		return FALSE;
	ASSERT(nLen < _MAX_PATH);
	
	// need to get both CT_CTYPE1 and CT_CTYPE3 for each filename
	LCID lcid = GetThreadLocale();
	WORD aCharType11[_MAX_PATH];
	VERIFY(GetStringTypeEx(lcid, CT_CTYPE1, lpszPath1, -1, aCharType11));
	WORD aCharType13[_MAX_PATH];
	VERIFY(GetStringTypeEx(lcid, CT_CTYPE3, lpszPath1, -1, aCharType13));
	WORD aCharType21[_MAX_PATH];
	VERIFY(GetStringTypeEx(lcid, CT_CTYPE1, lpszPath2, -1, aCharType21));
#ifdef _DEBUG
	WORD aCharType23[_MAX_PATH];
	VERIFY(GetStringTypeEx(lcid, CT_CTYPE3, lpszPath2, -1, aCharType23));
#endif
	
	// for every C3_FULLWIDTH character, make sure it has same C1 value
	int i = 0;
	for (LPCTSTR lpsz = lpszPath1; *lpsz != 0; lpsz = _tcsinc(lpsz))
	{
		// check for C3_FULLWIDTH characters only
		if (aCharType13[i] & C3_FULLWIDTH)
		{
#ifdef _DEBUG
			ASSERT(aCharType23[i] & C3_FULLWIDTH); // should always match!
#endif
			
			// if CT_CTYPE1 is different then file system considers these
			// file names different.
			if (aCharType11[i] != aCharType21[i])
				return FALSE;
		}
		++i; // look at next character type
	}
	return TRUE; // otherwise file name is truly the same
}

// helper for reliable and small Release calls
DWORD AFXAPI _AfxRelease(LPUNKNOWN* plpUnknown);
#ifndef _DEBUG
// generate smaller code in release build
#define RELEASE(lpUnk) _AfxRelease((LPUNKNOWN*)&lpUnk)
#else
// generate larger but typesafe code in debug build
#define RELEASE(lpUnk) do \
	{ if ((lpUnk) != NULL) { (lpUnk)->Release(); (lpUnk) = NULL; } } while (0)
#endif

DWORD AFXAPI _AfxRelease(LPUNKNOWN* lplpUnknown)
{
	ASSERT(lplpUnknown != NULL);
	if (*lplpUnknown != NULL)
	{
		DWORD dwRef = (*lplpUnknown)->Release();
		*lplpUnknown = NULL;
		return dwRef;
	}
	return 0;
}

#define DELETE_EXCEPTION(e)// do { e->Delete(); } while (0)

///////////////////////////////////////////////////////////////////
//功 能：保存文档
///////////////////////////////////////////////////////////////////
BOOL CDevDoc::OnSaveDocument(LPCTSTR lpszPathName) 
	// lpszPathName must be fully qualified
{
	USES_CONVERSION;
	
	ASSERT(lpszPathName == NULL || AfxIsValidString(lpszPathName));
	
	// use default implementation if 'docfile' not enabled
	if (!m_bCompoundFile && m_lpRootStg == NULL)
	{
		ASSERT(lpszPathName != NULL);
		return CDocument::OnSaveDocument(lpszPathName);
	}
	
	ASSERT(m_lpOrigStg == NULL);
	if (lpszPathName != NULL)
		m_bSameAsLoad = AfxComparePath(m_strPathName, lpszPathName);
	
	BOOL bResult = FALSE;
	TRY
	{
		// open new root storage if necessary
		if (lpszPathName != NULL && !m_bSameAsLoad)
		{
			// temporarily detach current storage
			m_lpOrigStg = m_lpRootStg;
			m_lpRootStg = NULL;
			
			LPSTORAGE lpStorage;
			SCODE sc = ::StgCreateDocfile(T2COLE(lpszPathName),
				STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_DENY_WRITE|STGM_CREATE,
				0, &lpStorage);
			if (sc != S_OK)
				AfxThrowOleException(sc);
			
			ASSERT(lpStorage != NULL);
			m_lpRootStg = lpStorage;
		}
		ASSERT(m_lpRootStg != NULL);
		
		// use helper to save to root storage
		SaveToStorage();
		
		if (lpszPathName != NULL)
		{
			// commit each of the items
			CommitItems(m_bRemember && !m_bSameAsLoad);
			
			// mark document as clean if remembering the storage
			if (m_bRemember)
				SetModifiedFlag(FALSE);
			
			// remember correct storage or release save copy as storage
			if (!m_bSameAsLoad)
			{
				if (m_bRemember)
				{
					// Save As case -- m_stgRoot is new storage, forget old storage
					m_lpOrigStg->Release();
				}
				else
				{
					// Save Copy As case -- m_stgRoot should hook up to m_stgOrig.
					m_lpRootStg->Release();
					m_lpRootStg = m_lpOrigStg;
				}
			}
		}
		
		bResult = TRUE;
	}
	CATCH_ALL(e)
	{
		if (m_lpOrigStg != NULL)
		{
			// save as failed: abort new storage, and re-attach original
			RELEASE(m_lpRootStg);
			m_lpRootStg = m_lpOrigStg;
		}
		
		if (lpszPathName == NULL)
		{
			THROW_LAST();
			ASSERT(FALSE);  // not reached
		}
		
		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		}
		END_TRY
			DELETE_EXCEPTION(e);
	}
	END_CATCH_ALL
		
	// cleanup
	m_lpOrigStg = NULL;
	m_bSameAsLoad = TRUE;
	m_bRemember = TRUE;
	
	return bResult;
}

void CDevDoc::SaveToStorage(CObject* pObject)
{
	ASSERT(m_lpRootStg != NULL);

	// create Contents stream
	COleStreamFile file;
	CFileException fe;
	if (!file.CreateStream(m_lpRootStg, _T("Contents"),
			CFile::modeReadWrite|CFile::shareExclusive|CFile::modeCreate, &fe))
	{
		if (fe.m_cause == CFileException::fileNotFound)
			AfxThrowArchiveException(CArchiveException::badSchema);
		else
			AfxThrowFileException(fe.m_cause, fe.m_lOsError);
	}

	// save to Contents stream
	CArchive saveArchive(&file, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_pDocument = this;
	saveArchive.m_bForceFlat = FALSE;

	TRY
	{
		// save the contents
		if (pObject != NULL)
			pObject->Serialize(saveArchive);
		else
			Serialize(saveArchive);
		saveArchive.Close();
		file.Close();

		// commit the root storage
		SCODE sc = m_lpRootStg->Commit(STGC_ONLYIFCURRENT | STGC_CONSOLIDATE); // 加入STGC_CONSOLIDATE后流保存后重新打开超过4096个字节有问题
		if (sc != S_OK)
			AfxThrowOleException(sc);
	}
	CATCH_ALL(e)
	{
		file.Abort();   // will not throw an exception
		CommitItems(FALSE); // abort save in progress
		saveArchive.Abort();
		THROW_LAST();
	}
	END_CATCH_ALL
}

///////////////////////////////////////////////////////////////////
//功 能：移除所有子文档
///////////////////////////////////////////////////////////////////
void CDevDoc::RemoveAllSubDocument()
{
	while (m_listSubDoc.GetCount() > 0)
	{
		CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc *)m_listSubDoc.GetHead();
		m_listSubDoc.RemoveHead();
		if (pSubDoc->m_pChildDoc != NULL)
		{
			pSubDoc->m_pChildDoc->OnCloseDocument();
			delete pSubDoc->m_pChildDoc;
		}

		delete pSubDoc;
	}
}

////////////////////////////////////////////////////////////////////////////
//功 能：创建新文档
////////////////////////////////////////////////////////////////////////////
BOOL CDevDoc::OnNewDocument()
{
	if (!COleDocument::OnNewDocument())
		return FALSE;

	SetDefaultTitle();

	CDocHit hit(CDocHit::changeNew, GetDisplayName());
	theApp.NotifyDocumentChanged(DWORD(this), (DWORD)&hit);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
//功 能：打开文档
////////////////////////////////////////////////////////////////////////////
BOOL CDevDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!COleDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	SetPathName(lpszPathName);
	SetDefaultTitle();
	
	CFormDoc::CFormSubDoc* pSubDoc = (CFormDoc::CFormSubDoc *)m_listSubDoc.GetHead();
	TRY
	{
		pSubDoc->LoadChildDocument();
//		theApp.NotifyDocumentChanged(DWORD(this));
	}
	CATCH_ALL(e)
	{
		RemoveAllSubDocument();

		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY
		DELETE_EXCEPTION(e);

//		THROW_LAST();
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}

CDevDoc::~CDevDoc()
{

}


BEGIN_MESSAGE_MAP(CDevDoc, COleDocument)
	//{{AFX_MSG_MAP(CDevDoc)
	ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
	ON_COMMAND(ID_ADD_CODE, OnAddCode)
	ON_COMMAND(ID_FILE_SAVE_PICTURE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_PICTURE_AS, OnFileSaveAs)
	ON_COMMAND(ID_ADD_FORM, OnAddForm)
	ON_COMMAND(ID_ADD_BLANK_REPORT, OnAddBlankReport)
	ON_COMMAND(ID_ADD_REPORT_WIZARD, OnAddReportWizard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDevDoc diagnostics

#ifdef _DEBUG
void CDevDoc::AssertValid() const
{
	COleDocument::AssertValid();
}

void CDevDoc::Dump(CDumpContext& dc) const
{
	COleDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDevDoc serialization

void CDevDoc::Serialize(CArchive& ar)
{
	int nType;
	CDevDoc::CSubDoc* pSubDoc;

	if (ar.IsStoring())
	{
		m_dwVer = _SUPERCX_VER; 
		ar << m_dwVer;

		m_nSubformModule = 0;

		int nCount = m_listSubDoc.GetCount();
		ar << nCount;
	
		POSITION pos = m_listSubDoc.GetHeadPosition();
		while (pos != NULL)
		{
			pSubDoc = (CDevDoc::CSubDoc *)m_listSubDoc.GetNext(pos); 
			nType = pSubDoc->GetType();
			ar << nType;
			pSubDoc->Serialize(ar);
		}
	}
	else
	{
		ar >> m_dwVer;

		int nCount;
		ar >> nCount;

		for (int i = 0; i < nCount; i++)
		{
			ar >> nType;

			if ((enum SUBDOCTYPE)nType == enumFormDoc)
				pSubDoc = new CFormDoc::CFormSubDoc;
			else if ((enum SUBDOCTYPE)nType == enumReportDoc) 
				pSubDoc = new CReportDoc::CReportSubDoc;
			else 
				pSubDoc = new CCodeModuleDoc::CCodeSubDoc;
			
			pSubDoc->Serialize(ar);
			m_listSubDoc.AddTail(pSubDoc);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDevDoc commands


//////////////////////////////////////////////////////////////
//功 能：得到指定子项目号码的存储名称
//参 数：子项目号码
//返 回：存储名称
////////////////////////////////////////////////////////////
CString CDevDoc::GetStorageName(DWORD dwSubDocumentNuber) const
{
	CString strRet;
	strRet.Format( _T("SubDoc%lu"), dwSubDocumentNuber);
	
	return strRet;
}

//////////////////////////////////////////////////////////////
//功 能：打开子文档的存储
//参 数：所要创建存储的子文档指针
//返 回：如果子文档的存储存在并能被打开，返回其存储
////////////////////////////////////////////////////////////
LPSTORAGE CDevDoc::OpenSubDocumentStorage(CDevDoc::CSubDoc* pSubDoc, LPSTORAGE lpRootStg)
{
	USES_CONVERSION;

	CString strStorageName = GetStorageName(pSubDoc->m_dwDocumentNumber);
	if (lpRootStg == NULL)
		lpRootStg = m_lpRootStg;

	// 打开子项目存储
	LPSTORAGE lpStorage = NULL;
	SCODE sc = lpRootStg->OpenStorage(T2CW(strStorageName), NULL,
		STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE, 0, 0, &lpStorage);
	if (sc == STG_E_FILENOTFOUND)
	{
		sc = lpRootStg->CreateStorage(T2CW(strStorageName), STGM_CREATE |
			STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE, 0, 0, &lpStorage);
	}

	if (sc != S_OK)
	{
		TRACE1( "Warning: unable to open child storage %s.\n", strStorageName);
		AfxThrowOleException(sc);
	}
	ASSERT(lpStorage != NULL);
	
	return lpStorage;
}

//////////////////////////////////////////////////////////////
//功 能：创建子文档的存储
//参 数：所要创建存储的子文档指针
//返 回：被创建的存储
////////////////////////////////////////////////////////////
LPSTORAGE CDevDoc::CreateSubDocumentStorage(CSubDoc* pSubDoc)
{
	USES_CONVERSION;
	CString strStorageName = GetStorageName(pSubDoc->m_dwDocumentNumber);

	LPSTORAGE lpStorage = NULL;
	SCODE sc = m_lpRootStg->CreateStorage(T2CW(strStorageName),
		STGM_CREATE|STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE,
		0, 0, &lpStorage);
	if (sc != S_OK)
	{
		TRACE("Warning: unable to create child storage %s.\n");
		AfxThrowOleException(sc);
		return NULL;
	}
	ASSERT(lpStorage != NULL);

	return lpStorage;
}


//////////////////////////////////////////////////////////////
//功 能：新增窗口都用此函数
//////////////////////////////////////////////////////////////
CDevDoc::CSubDoc* CDevDoc::AddSubDocument(SUBDOCTYPE doctype)
{
	CDevDoc::CSubDoc* pSubDoc = NULL;
	if (doctype == enumFormDoc)
		pSubDoc = new CFormDoc::CFormSubDoc;
	else if (doctype == enumReportDoc) 
		pSubDoc = new CReportDoc::CReportSubDoc;
	else if (doctype == enumCodeDoc) 
		pSubDoc = new CCodeModuleDoc::CCodeSubDoc;
	else
		return NULL;

	pSubDoc->m_dwDocumentNumber = m_dwNextDocumentNumber++;

	pSubDoc->m_pParentDoc = this;
	if (pSubDoc->CreateChildDocument(this) != NULL)
	{
		m_listSubDoc.AddTail(pSubDoc);
		if (!pSubDoc->IsFirstSubDoc())
			pSubDoc->m_pChildDoc->SetModifiedFlag();
		SetModifiedFlag();

		CDocHit hit(CDocHit::changeContent, GetDisplayName());
		theApp.NotifyDocumentChanged(DWORD(this), DWORD(&hit));
		OnSubDocumentChanged();
	}
	else
	{
		delete pSubDoc;
		AfxMessageBox(_T("创建文档失败！"));
		return NULL;
	}

	return pSubDoc;
}

BOOL IsFileExist(LPCTSTR szName)
{
	HANDLE hFile = CreateFile(
		szName,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		// If the handle is valid then the file exists.
		CloseHandle(hFile);
		return TRUE;
	}
	
	return FALSE;
}

/////////////////////////////////////////////////////////////////////
//功 能：设置工程文档标题
//描 述：格式为 画面_未命名X
/////////////////////////////////////////////////////////////////////
void CDevDoc::SetDefaultTitle()
{
	CString strDocName;
	TCHAR szNum[8];
	UINT nUntitledCount;

	if (m_strPathName.IsEmpty())
	{
		nUntitledCount = m_nUntitledCount/*c_listDevDoc.GetCount()*/;
		CString strFileName;

		do 
		{
			nUntitledCount++;
			wsprintf(szNum, _T("%d"), nUntitledCount);
			strFileName = theApp.GetIniStartPictureFolder() + _T("未命名");
			strFileName = strFileName + szNum;
			strFileName += _T(".grf");
		} while (FindDocument(strFileName) != NULL || IsFileExist(strFileName));
		

	//	WIN32_FIND_DATA FindFileData;
  	//	while (FindFirstFile(strFileName, &FindFileData) != INVALID_HANDLE_VALUE)
	//	{
	//		nUntitledCount++;
	//		wsprintf(szNum, _T("%d"), nUntitledCount + 1);
	//		strFileName = theApp.GetStartPicturePath() + _T("未命名");
	//		strFileName = strFileName + szNum + _T(".grf");
	//	}

	//	VERIFY(strDocName.LoadString(AFX_IDS_UNTITLED));
		strDocName = "未命名";
		strDocName += szNum;

		m_nUntitledCount++;
	}
	else
	{
//		strDocName = "画面_";
		CString strFileName = m_strPathName;
		GetFileTitle(m_strPathName, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
		strFileName.ReleaseBuffer();
	
		strDocName += strFileName;//.Mid(5);
	}

	SetTitle(strDocName);
}

CString CDevDoc::GetDisplayName()
{ 
	return GetTitle();//.Mid(5); 
}

///////////////////////////////////////////////////////////////////////////////

CDevDoc::CSubDoc::CSubDoc()
{
	m_dwDocumentNumber = -1;
	m_pParentDoc = NULL;
	m_pChildDoc = NULL;
	m_bNeedSave = TRUE;

	m_pStorage = NULL;
}

CDocument* CDevDoc::CSubDoc::CreateChildDocument(CDevDoc* pDoc)
{
	ASSERT(FALSE);

	return NULL;
}

CDocument* CDevDoc::CSubDoc::LoadChildDocument(BOOL bInitFrame)
{
	ASSERT(FALSE);

	return NULL;
}

BOOL CDevDoc::DeleteSubDocument(CDevDoc::CSubDoc* pSubDoc)
{
	if (pSubDoc->m_pChildDoc != NULL)
	{
		pSubDoc->m_pChildDoc->OnCloseDocument();
		delete pSubDoc->m_pChildDoc;
	}
	
	CString strStorageName = GetStorageName(pSubDoc->m_dwDocumentNumber);
	BSTR bstr = strStorageName.AllocSysString();
	m_lpRootStg->DestroyElement(bstr);
	SysFreeString(bstr);

	delete pSubDoc;
	m_listSubDoc.RemoveAt(m_listSubDoc.Find(pSubDoc));
	SetModifiedFlag();

	CDocHit hit(CDocHit::changeContent, GetDisplayName());
	theApp.NotifyDocumentChanged(DWORD(this), DWORD(&hit));
	OnSubDocumentChanged();
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//功能:返回对应于ID号的子文档指针,如不存在则返回NULL
//////////////////////////////////////////////////////////////////////
CDevDoc::CSubDoc* CDevDoc::GetSubDocument(DWORD dwSubDocumentNuber)
{
	POSITION pos = m_listSubDoc.GetHeadPosition();
	while (pos != NULL)
	{
		CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc*)m_listSubDoc.GetNext(pos);
		if(pSubDoc->m_dwDocumentNumber == dwSubDocumentNuber)
			return pSubDoc;
	}

	return NULL;
}

////////////////////////////////////////////////////////////////
//功能:返回一个可用的不重复ID号
////////////////////////////////////////////////////////////////
DWORD CDevDoc::GetNewDocumentNumber()
{
	DWORD dwRet = m_dwNextDocumentNumber;
	m_dwNextDocumentNumber++;
	return dwRet;
}


void CDevDoc::CSubDoc::GetDocumentName(LPTSTR lpszDocumentName) const
{
	ASSERT_VALID(this);
	ASSERT(lpszDocumentName != NULL);

	wsprintf(lpszDocumentName, _T("SubDoc%lu"), m_dwDocumentNumber);
}

//////////////////////////////////////////////////////////////
//功能:关闭子文档对象
//////////////////////////////////////////////////////////////
BOOL CDevDoc::CSubDoc::CloseChildDoc()
{
	m_pChildDoc->OnCloseDocument();
	return TRUE;
}

//////////////////////////////////////////////////////////////
//功能:是否为第一个子文档
//////////////////////////////////////////////////////////////
BOOL CDevDoc::CSubDoc::IsFirstSubDoc()
{
	return (m_dwDocumentNumber == 0);
}

void CDevDoc::OnFileSaveAll() 
{
	SaveAllModified(TRUE);
}

////////////////////////////////////////////////////////////
//功能:从一个文件中引入一个新窗体
////////////////////////////////////////////////////////////
CDevDoc::CSubDoc* CDevDoc::ImportForm(CString strFileName)
{
	CFormDoc::CFormSubDoc* pSubDoc = new CFormDoc::CFormSubDoc;
	pSubDoc->m_dwDocumentNumber = m_dwNextDocumentNumber ++;
	LPTSTR strBuffer = strFileName.LockBuffer();
	pSubDoc->m_pParentDoc = this;

	if (pSubDoc->LoadChildDocumentFromFile(strBuffer) != NULL)
	{
		m_listSubDoc.AddTail(pSubDoc);
		SetModifiedFlag(TRUE);
		theApp.NotifyDocumentChanged(DWORD(this));
		OnSubDocumentChanged();
	}
	else
	{
		delete pSubDoc;
		AfxMessageBox("不能创建文档！");
		pSubDoc = NULL;
	}

	strFileName.UnlockBuffer();
	if(pSubDoc != NULL)
		SaveModified();

	return pSubDoc;
}

CDocument* CDevDoc::CSubDoc::LoadChildDocumentFromFile(LPTSTR fileName)
{
	ASSERT(FALSE);

	return NULL;
}

////////////////////////////////////////////////////////////////////
//功 能：得到子文挡数据
////////////////////////////////////////////////////////////////////
void CDevDoc::GetSubDocumentData(CDevDoc::CSubDoc** ppSubDoc, int nCount, LPSTGMEDIUM lpStgMedium)
{
	ASSERT(ppSubDoc != NULL);

	COleStreamFile file;
	IStoragePtr pStorage;
	HRESULT hResult;

	hResult = StgCreateDocfile(NULL, STGM_CREATE|STGM_READWRITE|
		STGM_DELETEONRELEASE|STGM_SHARE_EXCLUSIVE, 0, &pStorage);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	if (!file.CreateStream(pStorage, _T("Contents"), CFile::modeReadWrite|
		CFile::shareExclusive|CFile::modeCreate))
	{
		AfxThrowOleException(E_FAIL);
	}

	CArchive ar(&file, CArchive::store);

	ar << nCount;
	for (int i = 0; i < nCount; i++)
	{
		ar << ppSubDoc[i]->GetType();
		ppSubDoc[i]->CopyToClipboard(ar, pStorage);
	}
	
	lpStgMedium->tymed = TYMED_ISTORAGE;
	lpStgMedium->pstg = pStorage;
	lpStgMedium->pstg->AddRef();
	lpStgMedium->pUnkForRelease = NULL;

	ar.Flush();
	file.Close();
}

////////////////////////////////////////////////////////////////////
//功 能：得到子文挡数据
////////////////////////////////////////////////////////////////////
void CDevDoc::GetSubDocumentData(CDevDoc::CSubDoc* pSubDoc, LPSTGMEDIUM lpStgMedium)
{
	ASSERT(pSubDoc != NULL);

	GetSubDocumentData(&pSubDoc, 1, lpStgMedium);
}

void CDevDoc::GetSubDocumentData(DWORD dwSubDocumentNuber, LPSTGMEDIUM lpStgMedium)
{
	CSubDoc* pSubDoc = GetSubDocument(dwSubDocumentNuber);
	ASSERT(pSubDoc != NULL);

	GetSubDocumentData(pSubDoc, lpStgMedium);
}

BOOL CDevDoc::DoPasteSubDocument(COleDataObject* pDataObject)
{
	STGMEDIUM stgmedium;
	IStoragePtr pStorage;
	COleStreamFile file;
	int nCount;
	int nType;
	
	COleDataObject clipboardData;
	if (pDataObject == NULL)
	{
		if (!clipboardData.AttachClipboard())
			return FALSE;
		pDataObject = &clipboardData;
	}
	
	if (!pDataObject->GetData(m_cfDocData, &stgmedium))
	{
		return FALSE;
	}

	ASSERT(stgmedium.tymed == TYMED_ISTORAGE);

	pStorage = stgmedium.pstg;
	ReleaseStgMedium(&stgmedium);

	if (!file.OpenStream(pStorage, _T("Contents"), STGM_READ|
		STGM_SHARE_EXCLUSIVE))
	{
		return FALSE;
	}

	CArchive ar(&file, CArchive::load|CArchive::bNoFlushOnDelete);

	ar >> nCount;
	CSubDoc* pSubDoc;
	for (int i = 0; i < nCount; i++)
	{
		ar >> nType;
		switch ((enum SUBDOCTYPE)nType)
		{
		case enumFormDoc:
			pSubDoc = new CFormDoc::CFormSubDoc();
			break;
		case enumReportDoc:
			pSubDoc = new CReportDoc::CReportSubDoc();
			break;
		case enumCodeDoc:
			pSubDoc = new CCodeModuleDoc::CCodeSubDoc();
			break;
		default:
			ASSERT(FALSE);
		}

		pSubDoc->m_pParentDoc = this;

		TRY
		{
			if (!pSubDoc->CreateFromClipboard(ar, pStorage))
			{
				AfxThrowOleException(E_FAIL);
			}
		}
		CATCH_ALL(e)
		{
			file.Abort();   // will not throw an exception
			ar.Abort();
			
			pStorage.Release();

			delete pSubDoc;

			THROW_LAST();
		}
		END_CATCH_ALL

		m_listSubDoc.AddTail(pSubDoc);
	}
		
	ar.Flush();
	file.Close();
	
	SetModifiedFlag();

	CDocHit hit(CDocHit::changeContent, GetDisplayName());
	theApp.NotifyDocumentChanged(DWORD(this), DWORD(&hit));
	OnSubDocumentChanged();

	return TRUE;
}

////////////////////////////////////////////////////////////////
//功能:击活指定的子文档为当前编辑子文档
////////////////////////////////////////////////////////////////
void CDevDoc::ActivateSubDocument(CDevDoc::CSubDoc* pSubDoc)
{
	if (m_listSubDoc.Find(pSubDoc) == NULL)
		return;
	
	TRY
	{
		if (pSubDoc->LoadChildDocument() == NULL)
			return;

		// 主要用于包含模块对象的窗体刷新子节点
		CDocHit hit(CDocHit::changeContent, GetDisplayName());
		theApp.NotifyDocumentChanged(DWORD(this), DWORD(&hit));
	}
	CATCH_ALL(e)
	{
		TRY
		{
			ReportSaveLoadException(m_strPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY
		DELETE_EXCEPTION(e);
		return;
	}
	END_CATCH_ALL

	POSITION pos = pSubDoc->m_pChildDoc->GetFirstViewPosition();
	if (pos == NULL)
		return;

	CView* pView = pSubDoc->m_pChildDoc->GetNextView(pos);
	if (pView == NULL)
		return;

	CMDIChildWnd* pFrame = (CMDIChildWnd *)pView->GetParentFrame();
	CMDIChildWnd* pActiveFrame = ((CMainFrame *)AfxGetMainWnd())->MDIGetActive();
	if (pFrame != NULL && pFrame != pActiveFrame)
		((CMainFrame *)AfxGetMainWnd())->MDIActivate(pFrame);

}

//////////////////////////////////////////////////////////
//功能:删除文件
//参数:strPathName[in]		要删除的文件名,有或没有路径,
//							如没有则使用默认路径
//返回:成功TRUE,失败FALSE
//说明:对应文档应已被关闭
//////////////////////////////////////////////////////////
BOOL CDevDoc::DeleteDocumentFile(LPCTSTR lpszFileName)
{
	CDevDoc* pDoc = (CDevDoc *)FindDocument(lpszFileName);
	if (pDoc != NULL)
	{
		if (!pDoc->CloseDocument())
			return FALSE;
	}
	else
	{
		CString strDisplayName = lpszFileName;
		int i = strDisplayName.ReverseFind('/');
		int j = strDisplayName.ReverseFind('\\');
		if (i < j)
			i = j;
		if (i < 0)
			return NULL;

		strDisplayName = strDisplayName.Mid(i + 1, strDisplayName.GetLength() - i - 1 - 4);

		CDocHit hit(CDocHit::changeDelete, strDisplayName);
		theApp.NotifyDocumentChanged(0, DWORD(&hit));
	}

	return DeleteFile(lpszFileName);
}

///////////////////////////////////////////////////////////////////
//功能:关闭画面文档
//返回:成功TRUE,失败FALSE
///////////////////////////////////////////////////////////////////
BOOL CDevDoc::CloseDocument()
{
	CDocHit hit(CDocHit::changeDelete, GetDisplayName());
	theApp.NotifyDocumentChanged(DWORD(this), DWORD(&hit));

	c_listDevDoc.RemoveAt(c_listDevDoc.Find(this));

	RemoveAllSubDocument();
	OnCloseDocument(); 
		
	return TRUE;
}

////////////////////////////////////////////////////////////////////
//功能：
////////////////////////////////////////////////////////////////////
BOOL CDevDoc::SaveSubDocument(CDevDoc::CSubDoc *pDoc)
{
	CArray<BOOL, BOOL> arrSave;

	//保存原需要保存needsave标志
	POSITION pos = m_listSubDoc.GetHeadPosition();
	while (pos != NULL)
	{
		CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc*)m_listSubDoc.GetNext(pos);
		arrSave.Add(pSubDoc->m_bNeedSave);
		if (pDoc != pSubDoc)
			pSubDoc->m_bNeedSave = FALSE;
	}

	DoFileSave(); 	//保存主文档

	//恢复标志
	pos = m_listSubDoc.GetHeadPosition();
	int i = 0;
	while (pos != NULL)
	{
		CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc*)m_listSubDoc.GetNext(pos);
		pSubDoc->m_bNeedSave = arrSave[i++];
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////
//功能:返回对应于当前击活窗体的文档
//返回:成功TRUE,失败FALSE
/////////////////////////////////////////////////////////////
CDevDoc* CDevDoc::GetActivateDoc()
{
	CFormDoc* pSubDoc = GetActivateSubDoc();
	if(pSubDoc != NULL)
	{
		if(pSubDoc->m_pFormSubDoc != NULL)
		{
			CDevDoc* pDevDoc = (CDevDoc *)(pSubDoc->m_pFormSubDoc->m_pParentDoc);
			return pDevDoc;
		}
	}
	
	return NULL;
}

//////////////////////////////////////////////////////////
//功能:返回当前击活的窗体
//返回:窗体文档指针(如有), NULL(如无)
//////////////////////////////////////////////////////////
CFormDoc* CDevDoc::GetActivateSubDoc()
{
	CDevApp* pApp = (CDevApp*)AfxGetApp();
	ASSERT(pApp->m_pMainWnd);

	CMainFrame* pFrame = (CMainFrame *)(pApp->m_pMainWnd);
	CMDIChildWnd* pChildWnd = pFrame->MDIGetActive();
	if(pChildWnd != NULL)
	{
		CView* pView = pChildWnd->GetActiveView();
		if(pView != NULL)
		{
			CDocument* pDoc = pView->GetDocument();
			if(pDoc != NULL && pDoc->IsKindOf(RUNTIME_CLASS(CFormDoc)))
			{
				if (((CFormDoc*)pDoc)->m_pFormObj != NULL)
					return ((CFormDoc*)pDoc)->m_pFormObj->GetFormDoc();

				ASSERT(((CFormDoc*)pDoc)->m_pFormSubDoc);
				ASSERT(((CFormDoc*)pDoc)->m_pFormSubDoc->m_pParentDoc);
				return (CFormDoc*)pDoc;
			}
		}
	}
	return NULL;
}

void CDevDoc::OnAddCode() 
{
	//暂时不支持
	return;
	AddSubDocument(enumCodeDoc);
}


void CDevDoc::OnAddForm() 
{
	AddSubDocument(CDevDoc::enumFormDoc);	
}

void CDevDoc::OnAddBlankReport()
{
	AddSubDocument(CDevDoc::enumReportDoc);
}

CDevDoc::CSubDoc* CDevDoc::GetSubDocument(LPCTSTR szName)
{
	POSITION pos = m_listSubDoc.GetHeadPosition();
	while (pos != NULL)
	{
		CSubDoc* pSubDoc = (CSubDoc* )(m_listSubDoc.GetNext(pos));
		if (pSubDoc->m_strName.CollateNoCase(szName) == 0)
			return pSubDoc;
	}

	return NULL;
}

#include "ReportFieldDlg.h"
#include "ReportWizardDlg.h"

#define TITLEFONTSTYLE FontStyleBold

void CDevDoc::OnAddReportWizard() 
{
	CReportWizardDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CReportDoc* pReportDoc = (CReportDoc *)AddSubDocument(CDevDoc::enumReportDoc)->m_pChildDoc;
		pReportDoc->m_pDataConnection = dlg.m_pDataConnection;
		pReportDoc->m_pDataRecordset = dlg.m_pDataRecordset;
		if (dlg.m_nReportOrientation == CReportWizardDlg::ReportOrientation_Landscape)
			pReportDoc->SetLandscapeWidth();
		
		// 报眉
		CRectF rcSection = pReportDoc->GetSectionRect(&pReportDoc->m_secReportHeader);
		CPointF point  = rcSection.CenterPoint();
		point.y -= 5;
		pReportDoc->AddLabel(_T("报表标题"), point, 0, TextAlignCenter, 24, TITLEFONTSTYLE);
		
		// 页眉
		if (dlg.m_nReportLayout != CReportWizardDlg::ReportLayout_Justified)
		{
			rcSection = pReportDoc->GetSectionRect(&pReportDoc->m_secPageHeader);
			pReportDoc->AddLine(rcSection.left, rcSection.bottom - 2, 
				rcSection.right, rcSection.bottom - 2, 2, RGB(0, 0, 255));
		}
		else
		{
			pReportDoc->m_secPageHeader.m_fHeight = 4.0f;
		}
		
		if (dlg.m_nReportLayout == CReportWizardDlg::ReportLayout_Columnar)
		{
			// 详细内容
			pReportDoc->m_secDetail.m_bKeepTogether = TRUE;
			rcSection = pReportDoc->GetSectionRect(&pReportDoc->m_secDetail);

			//	if (rcSection.Height() < dlg.m_arrFields.GetSize() * 40)
			pReportDoc->m_secDetail.m_fHeight = (float)dlg.m_arrFields.GetSize() * 30 + 10;
			
	// 		point.x = (rcSection.left + rcSection.right) / 2;
			point.x = rcSection.left;
			point.y = rcSection.top + 20;
			
			for (int i = 0; i < dlg.m_arrFields.GetSize(); i++)
			{
				CReportField& field = dlg.m_arrFields[i];

				pReportDoc->AddLabel(field.m_strName, point, 0, TextAlignLeft, 12, TITLEFONTSTYLE);
				CPointF ptTemp = point;
				ptTemp.Offset(200, 0);
				pReportDoc->AddFieldDataLink(field.m_strName, ptTemp, 0, TextAlignLeft, 12);
				point.Offset(0, 30);
			}
		}
		else if (dlg.m_nReportLayout == CReportWizardDlg::ReportLayout_Tabular)
		{
			point.x = rcSection.left;
			point.y = rcSection.top + 12;

			int nTotalSize = 0;
			for (int i = 0; i < dlg.m_arrFields.GetSize(); i++)
			{
				int nSize = min(dlg.m_arrFields[i].m_nSize, 30);
				nTotalSize += nSize;
			}
			if (dlg.m_arrFields.GetSize() > 0)
				nTotalSize += (dlg.m_arrFields.GetSize() - 1);
			nTotalSize *= 8;
			
			int nWidth = rcSection.Width();

			for (i = 0; i < dlg.m_arrFields.GetSize(); i++)
			{
				CReportField& field = dlg.m_arrFields[i];

				int nSize = min(field.m_nSize, 30) * 8;
				int nOffsetTemp = nTotalSize <= nWidth ? nSize : MulDiv(nSize, nWidth, nTotalSize);
							
				pReportDoc->AddLabel(field.m_strName, point, nOffsetTemp, field.m_nAlign, 12, TITLEFONTSTYLE);
				
				nSize = nSize + 8;
				int nOffset = nTotalSize <= nWidth ? nSize : MulDiv(nSize, nWidth, nTotalSize);
				point.Offset(nOffset, 0);
			}

			rcSection = pReportDoc->GetSectionRect(&pReportDoc->m_secDetail);

			point.x = rcSection.left;
			point.y = rcSection.top + 10;
			
			for (i = 0; i < dlg.m_arrFields.GetSize(); i++)
			{
				CReportField& field = dlg.m_arrFields[i];

				int nSize = min(field.m_nSize, 30) * 8;
				int nOffsetTemp = nTotalSize <= nWidth ? nSize : MulDiv(nSize, nWidth, nTotalSize);
				
				pReportDoc->AddFieldDataLink(field.m_strName, point, nOffsetTemp, field.m_nAlign, 12);

				nSize = nSize + 8;
				int nOffset = nTotalSize <= nWidth ? nSize : MulDiv(nSize, nWidth, nTotalSize);
				point.Offset(nOffset, 0);
			}
			
			pReportDoc->m_secDetail.m_bKeepTogether = TRUE;
			pReportDoc->m_secDetail.m_fHeight = 26.0f;
		}
		else if (dlg.m_nReportLayout == CReportWizardDlg::ReportLayout_Justified)
		{
			rcSection = pReportDoc->GetSectionRect(&pReportDoc->m_secDetail);
			
			int nLeft = rcSection.left + 2;
			point.x = nLeft;
			point.y = rcSection.top + 20;
			
			for (int i = 0; i < dlg.m_arrFields.GetSize(); i++)
			{
				CReportField& field = dlg.m_arrFields[i];
				
				// 换行
				BOOL bDrawLine = i == 0;
				if (point.x != nLeft && point.x + field.m_nSize * 8 > rcSection.right)
				{
					point.x = nLeft;
					point.y += 40;
					bDrawLine = TRUE;
				}

				// 画横线
				if (bDrawLine)
				{
					if (i != 0)
					{
						pReportDoc->AddLine(rcSection.left + 2, point.y - 10, 
							rcSection.right - 2, point.y - 10, 1);
					}
					pReportDoc->AddLine(rcSection.left + 2, point.y + 10, 
						rcSection.right - 2, point.y + 10, 1);
				}

				if (point.x != nLeft)
				{
					pReportDoc->AddLine(point.x - 4, point.y - 10, 
						point.x - 4, point.y + 30, 1);
				}
				else
				{
					point.x += 4;
				}

				// 本行留有残余空白
				BOOL bLeftSpace = FALSE;
				if (i < dlg.m_arrFields.GetSize() - 1)
				{
					CReportField& fieldT = dlg.m_arrFields[i + 1];
					if (point.x + (field.m_nSize + fieldT.m_nSize) * 8 > rcSection.right)
						bLeftSpace = TRUE;
				}
				else
				{
					bLeftSpace = TRUE;
				}

				int nOffsetTemp = bLeftSpace ? (rcSection.right - point.x) : field.m_nSize * 8;
				
				pReportDoc->AddLabel(field.m_strName, point, nOffsetTemp, field.m_nAlign, 12, TITLEFONTSTYLE);
				CPointF ptTemp = point;
				ptTemp.Offset(0, 20);
				pReportDoc->AddFieldDataLink(field.m_strName, ptTemp, nOffsetTemp, field.m_nAlign, 12);

				int nOffset = (field.m_nSize + 1) * 8;
				point.Offset(nOffset, 0);
			}

			pReportDoc->m_secDetail.m_bKeepTogether = TRUE;
			pReportDoc->m_secDetail.m_fHeight = point.y + 40 - rcSection.top;
		}
		
		rcSection = pReportDoc->GetSectionRect(&pReportDoc->m_secDetail);

		if (dlg.m_nReportLayout != CReportWizardDlg::ReportLayout_Justified)
		{
			pReportDoc->AddLine(rcSection.left, rcSection.bottom - 1, 
				rcSection.right, rcSection.bottom - 1, 1);
		}
		else
		{
			CDevObj* pObj = pReportDoc->AddRectangle(rcSection.left + 2, rcSection.top + 10, 
				rcSection.right - 2, rcSection.bottom - 10, 2);
			pReportDoc->BringToFront(pObj);
		}
		
		// 页脚
		rcSection = pReportDoc->GetSectionRect(&pReportDoc->m_secPageFooter);
		
		pReportDoc->AddLine(rcSection.left, rcSection.bottom - 22, 
			rcSection.right, rcSection.bottom - 22, 2, RGB(192, 192, 192255));
		
		point.y = rcSection.bottom - 10;
		point.x = rcSection.left + 20;
		pReportDoc->AddDataLink(_T("Date()"), point, _T("=Date()"), TextAlignLeft, 10, TITLEFONTSTYLE);
		
		point.x = rcSection.right - 20;
		pReportDoc->AddDataLink(_T("\"共 \" & PageCount & \" 页，第 \" & Page & \" 页\""), point, 
			_T("=\"共 \" & PageCount & \" 页，第 \" & Page & \" 页\""), TextAlignRight, 10, TITLEFONTSTYLE);
		
		pReportDoc->ReCalcReportHeight(TRUE);
	}	
}

void CDevDoc::OnSubDocumentChanged()
{
	CDevDoc::CSubDoc* pSubDoc;
	POSITION pos = m_listSubDoc.GetHeadPosition();
	while (pos != NULL)
	{
		pSubDoc = (CDevDoc::CSubDoc *)m_listSubDoc.GetNext(pos);
		pSubDoc->OnParentDocChanged();
	}
}