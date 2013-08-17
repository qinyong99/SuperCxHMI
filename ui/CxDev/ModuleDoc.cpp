// ModuleDoc.cpp : implementation file

#include "stdafx.h"
#include "DevInc.h"
#include "ModuleDoc.h"

#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////
//静态量及常量
const TCHAR CModuleDoc::m_szModuleLibFileExt[] = _T("mlb"); //扩展名

/////////////////////////////////////////////////////////////////////////////
// CModuleDoc

IMPLEMENT_DYNCREATE(CModuleDoc, CDevObjDoc)

CModuleDoc::CModuleDoc()
{
	m_bAutoDelete = FALSE;
	m_dwNextModuleNumber = 0;

	EnableCompoundFile();
}


CModuleDoc::~CModuleDoc()
{
}


BEGIN_MESSAGE_MAP(CModuleDoc, COleDocument)
	//{{AFX_MSG_MAP(CModuleDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModuleDoc diagnostics

#ifdef _DEBUG
void CModuleDoc::AssertValid() const
{
	COleDocument::AssertValid();
}

void CModuleDoc::Dump(CDumpContext& dc) const
{
	COleDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CModuleDoc serialization

void CModuleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		int nCount = m_arrModule.GetSize();
		ar << nCount;
		for (int i = 0; i < nCount; i++)
		{
			CModule* pModule = (CModule *)m_arrModule[i]; 
			pModule->Serialize(ar);
		}

		m_listImage.Write(&ar);
		m_listImageLarge.Write(&ar);
	}
	else
	{
		int nCount;
		ar >> nCount;
		for (int i = 0; i < nCount; i++)
		{
			CModule* pModule = new CModule;
			pModule->Serialize(ar);
			m_arrModule.Add(pModule);
		}

		m_listImage.Read(&ar);
		m_listImageLarge.Read(&ar);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CModuleDoc commands

IStorage* CModuleDoc::CreateModuleStorage(CModule* pModule)
{
	USES_CONVERSION;
	ASSERT(m_lpRootStg != NULL);
	ASSERT(pModule != NULL);

	TCHAR szModuleName[64];
	IStorage* pStorage;

	pModule->GetModuleName(szModuleName);

	HRESULT hResult = m_lpRootStg->CreateStorage(T2COLE(szModuleName), STGM_READWRITE|
		STGM_CREATE|STGM_SHARE_EXCLUSIVE, 0, 0, &pStorage);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
		delete pModule;
		return NULL;
	}

	return pStorage;
}

///////////////////////////////////////////////////////////////////
//功 能：创建唯一的子文档名
///////////////////////////////////////////////////////////////////
void CModuleDoc::CreateUniqueName(LPCTSTR pszBaseName, CString& strUniqueName)
{
	ULONG nTag;
	BOOL bCollisionFound;

	ASSERT(pszBaseName != NULL);

	// Assume the name is already unique
	strUniqueName.Format("%s", pszBaseName);

	nTag = 1;
	do
	{
		bCollisionFound = FALSE;

		int nCount = m_arrModule.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			CModule* pModule = (CModule*)m_arrModule[i];
			if (strUniqueName == pModule->m_strName)
			{
				bCollisionFound = TRUE;
				strUniqueName.Format("%s%u", pszBaseName, nTag);
				nTag++;
			}
		}
	} while (bCollisionFound);
}

CModuleDoc::CModule* CModuleDoc::AddFormModule(CFormDoc* pDoc)
{
	USES_CONVERSION;
	ASSERT(pDoc != NULL);
	ASSERT(m_lpRootStg != NULL);

	CString strName;
	CreateUniqueName(pDoc->GetDisplayName(), strName);
	CModule* pModule = new CModule(this, 
		pDoc->IsKindOf(RUNTIME_CLASS(CReportDoc)) ? CDevDoc::enumReportDoc : CDevDoc::enumFormDoc, strName);
	ASSERT(pModule != NULL);

////////////////////////////////////////////////////////////////
//  为禁止成都电业局自行添加具有反事故演习功能的模块而增加的代码
//	在非我的计算机上含 ActionGame 调用的模块脚本中增加全角空格使脚本解释报错

	if (!IsFileExist(_T("D:\\WorkDir\\CxDev\\CxDev.dsp")))
	{
		CString strScript;
		CScriptView* pScriptView = pDoc->GetScriptView();
		if (pScriptView != NULL)
			pScriptView->GetText(strScript);
		else
			strScript = pDoc->m_strScript;

		strScript.MakeUpper();
		
		if (strScript.Find(_T("ACTIONGAME")) != -1)
		{
			if (pScriptView != NULL)
			{	
				pScriptView->SetSelectionStart(strScript.GetLength());
				pScriptView->SetSelectionEnd(strScript.GetLength());
				pScriptView->ReplaceSel(_T("　"));
			}
			else
			{
				pDoc->m_strScript += (0, _T("　"));
			}
		}
	}

////////////////////////////////////////////////////////////////

	IStorage* pStorage = CreateModuleStorage(pModule);
	pDoc->WriteToStorage(pStorage);
	m_arrModule.Add(pModule);

	pStorage->Release();
	
	SaveLib();
	
	CreateModuleBitmap(pModule);	

	return pModule;
}

void CModuleDoc::CreateModuleBitmap(CModuleDoc::CModule* pModule)
{
	USES_CONVERSION;

	CFormDoc* pFormDoc = NULL;

	switch (pModule->m_type) 
	{
	case CDevDoc::enumFormDoc:
		pFormDoc = new CFormDoc;
		break;
	case CDevDoc::enumReportDoc:
		pFormDoc = new CReportDoc;
		break;
	default:
		ASSERT(false);
		return;
	}

	if (!pFormDoc->OnNewDocument())
	{
		delete pFormDoc;
		return;
	}
	
	TCHAR szModuleName[64];
	IStorage* pStorage;
	pModule->GetModuleName(szModuleName);
	// 打开存储
	SCODE sc = m_lpRootStg->OpenStorage(T2COLE(szModuleName), NULL,
		STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE, 0, 0, &pStorage);
	if (sc != S_OK)
	{
		AfxThrowOleException(sc);
	}
	ASSERT(pStorage != NULL);

	pFormDoc->ReadFromStorage(pStorage);

	pStorage->Release();

	pModule->m_rcPosition = pFormDoc->m_position;

	CDC *pDC = AfxGetMainWnd()->GetDC();

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	//小图标
	CBitmap bitmap32;
	bitmap32.CreateCompatibleBitmap(pDC, 32, 32);
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap32);

	CRect rect32(0, 0, 32, 32);
	memDC.FillSolidRect(&rect32, GetSysColor(COLOR_WINDOW));
	pFormDoc->DisplayAsIcon(&memDC, &rect32);
	memDC.SelectObject(pOldBitmap);
	m_listImage.Add(&bitmap32, (CBitmap *)NULL);

	//大图标
	CBitmap bitmap64;
	bitmap64.CreateCompatibleBitmap(pDC, 64, 64);
	pOldBitmap = memDC.SelectObject(&bitmap64);

	CRect rect64(0, 0, 64, 64);
	memDC.FillSolidRect(&rect64, GetSysColor(COLOR_WINDOW));
	pFormDoc->DisplayAsIcon(&memDC, &rect64);
	memDC.SelectObject(pOldBitmap);
	m_listImageLarge.Add(&bitmap64, (CBitmap *)NULL);

	pFormDoc->OnCloseDocument();

	SaveLib();

	AfxGetMainWnd()->ReleaseDC(pDC);
}

BOOL CModuleDoc::DeleteModule(LPCSTR szName)
{
	USES_CONVERSION;

	int nCount = m_arrModule.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CModule* pModule = (CModule*)m_arrModule[i];
		if (pModule->m_strName == szName)
		{
			if (pModule->m_type == CDevDoc::enumFormDoc)
			{
				TCHAR szModuleName[64];
				pModule->GetModuleName(szModuleName);
				m_lpRootStg->DestroyElement(T2OLE(szModuleName));
			}
			m_arrModule.RemoveAt(i);

			m_listImage.Remove(i);
			m_listImageLarge.Remove(i);

			delete pModule;
			
			return TRUE;
		}
	}

	return FALSE;
}

void CModuleDoc::SetModuleName(LPCSTR szName, LPCSTR szNewName)
{
	int nCount = m_arrModule.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CModule* pModule = (CModule*)m_arrModule[i];
		if (pModule->m_strName == szName)
		{
			pModule->m_strName = szNewName;
			SaveLib();
		}
	}
}

CModuleDoc::CModule* CModuleDoc::GetModule(LPCSTR szName)
{
	int nCount = m_arrModule.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CModule* pModule = (CModule*)m_arrModule[i];
		if (pModule->m_strName == szName)
			return pModule;
	}

	return NULL;
}

int CModuleDoc::GetModuleCount()
{
	return m_arrModule.GetSize();
}

CModuleDoc::CModule* CModuleDoc::GetModule(int iIndex)
{
	return (CModule *)m_arrModule[iIndex];
}

///////////////////////////////////////////////////////
//功 能：由模块对象创建一个子文挡对象
//描 述：使用完对象由调用者释放]
///////////////////////////////////////////////////////
CDevDoc::CSubDoc* CModuleDoc::GetSubDocument(LPCSTR szName)
{
	USES_CONVERSION;

	CModule* pModule = GetModule(szName);

	CDevDoc::CSubDoc* pSubDoc = NULL;
	switch (pModule->m_type) 
	{
	case CDevDoc::enumFormDoc:
		pSubDoc = new CFormDoc::CFormSubDoc;
		break;
	case CDevDoc::enumReportDoc:
		pSubDoc = new CReportDoc::CReportSubDoc;
		break;
	default:
		ASSERT(false);
		return NULL;
	}
	
	pSubDoc->m_strName = pModule->m_strName;
	pSubDoc->m_dwDocumentNumber = pModule->m_dwModuleNumber;

	TCHAR szModuleName[64];
	IStorage* pStorage;
	pModule->GetModuleName(szModuleName);
	// 打开存储
	SCODE sc = m_lpRootStg->OpenStorage(T2COLE(szModuleName), NULL,
		STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE, 0, 0, &pStorage);
	if (sc != S_OK)
	{
		AfxThrowOleException(sc);
	}
	ASSERT(pStorage != NULL);

	pSubDoc->m_pStorage = pStorage;
	
	return pSubDoc;
}

BOOL CModuleDoc::DoPaste(COleDataObject* pDataObject)
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
	
	if (!pDataObject->GetData(CDevDoc::m_cfDocData, &stgmedium))
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
	CDevDoc::CSubDoc* pSubDoc;
	for (int i = 0; i < nCount; i++)
	{
		ar >> nType;
		switch ((CDevDoc::SUBDOCTYPE)nType)
		{
		case CDevDoc::enumFormDoc:
			pSubDoc = new CFormDoc::CFormSubDoc();
			break;
		case CDevDoc::enumReportDoc:
			pSubDoc = new CReportDoc::CReportSubDoc();
			break;
		default:
			ASSERT(FALSE);
		}

		CModule* pModule = new CModule(this, (CDevDoc::SUBDOCTYPE)nType, NULL);
		ASSERT(pModule != NULL);

		IStorage* pModuleStorage = CreateModuleStorage(pModule);
		pSubDoc->m_dwDocumentNumber = pModule->m_dwModuleNumber;
		pSubDoc->m_pStorage = pModuleStorage;
		if (!pSubDoc->CreateFromClipboard(ar, pStorage))
		{
			AfxThrowOleException(E_FAIL);
		}
		CString strName;
		CreateUniqueName(pSubDoc->m_strName, strName);
		pModule->m_strName = strName;
		m_arrModule.Add(pModule);
		pModuleStorage->Release();
		delete pSubDoc;
	}
	
	SaveLib();
	
	ar.Flush();
	file.Close();

	for (i = m_arrModule.GetSize() - nCount; i < m_arrModule.GetSize(); i++)
	{
		CModule* pModule = (CModule *)m_arrModule[i];
		CreateModuleBitmap(pModule);
	}

	return TRUE;
}

//////////////////////////////////////////////////////
//功能:关闭文档
//返回:成功TRUE,失败FALSE
//////////////////////////////////////////////////////
BOOL CModuleDoc::CloseLib()
{
	OnCloseDocument();
	DeleteContents();
	
	return TRUE;
}

////////////////////////////////////////////////////////
//功能:取默认的图库路径
////////////////////////////////////////////////////////
CString CModuleDoc::GetDefaultPath()
{
	CString strRet = theApp.GetCurrentAppDirectory();
	strRet += _T("Module");
	return strRet;
}

///////////////////////////////////////////////////////////
//功能:保存当前库文件到磁盘
//返回:成功TRUE, 失败FALSE
///////////////////////////////////////////////////////////
BOOL CModuleDoc::SaveLib()
{
	return DoSave(m_strPathName, TRUE);
}

///////////////////////////////////////////////////////////
//功能:创建新的库
//参数:strName[in]		库名
//返回:成功库文档指针,失败NULL
////////////////////////////////////////////////////////////
CModuleDoc* CModuleDoc::NewLib(CString strName)
{
	CModuleDoc* pDoc = new CModuleDoc();
	if (pDoc == NULL)
		AfxThrowMemoryException();
	if (!pDoc->OnNewDocument())
		return NULL;

	pDoc->m_listImage.Create(32, 32, ILC_COLOR24, 0, 1);
	pDoc->m_listImageLarge.Create(64, 64, ILC_COLOR24, 0, 1);
//	pDoc->m_listImage.SetImageCount(0);
//	pDoc->m_listImageLarge.SetImageCount(0);

	CString strNameUse = GetDefaultPath();
	strNameUse += _T("\\");
	strNameUse += strName + ".";
	strNameUse += m_szModuleLibFileExt;

	pDoc->DoSave(strNameUse, TRUE);
	pDoc->SetPathName(strNameUse, FALSE);

	return pDoc;
}

//////////////////////////////////////////////////////
//功能:则除strLibName指定的库
//参数:strLibName[in]		库名
//返回:成功TRUE,失败FALSE
//////////////////////////////////////////////////////
BOOL CModuleDoc::DeleteLib(CString strLibName)
{
	CString strPath = GetDefaultPath();
	strPath += "\\";
	strPath += strLibName + ".";
	strPath += m_szModuleLibFileExt;

	return DeleteFile(strPath);
}

////////////////////////////////////////////////////////
//功能:重命名库文档
//参数:strLibName[in]		图库名
//返回:成功图库文档指针,失败NULL
/////////////////////////////////////////////////////////
void CModuleDoc::RenameLib(LPCTSTR lpszOldName, LPCTSTR lpszNewName)
{
	CString strPath = GetDefaultPath();
	strPath += _T("\\");

	CString strOldName = strPath + lpszOldName + _T(".") + m_szModuleLibFileExt;
	CString strNewName = strPath + lpszNewName + _T(".") + m_szModuleLibFileExt;

	CFile::Rename(strOldName, strNewName);
}

BOOL CModuleDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
		ASSERT(FALSE);		//名称必须存在
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
			}
			END_CATCH_ALL
		}
		return FALSE;
	}

	return TRUE;        // success
}

////////////////////////////////////////////////////////
//功能:打开为库文档
//参数:strLibName[in]		库名
//返回:成功库文档指针,失败NULL
/////////////////////////////////////////////////////////
BOOL CModuleDoc::OpenLib(CString strLibName)
{
	CString strPath = GetDefaultPath();
	strPath += _T("\\");
	strPath += strLibName + _T(".");
	strPath += m_szModuleLibFileExt;
	m_strPathName = strPath;

	return OnOpenDocument(strPath);
}

void CModuleDoc::DeleteContents() 
{
	int nCount = m_arrModule.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CModule* pModule = (CModule*)m_arrModule[i];
		delete pModule;
	}
	m_arrModule.RemoveAll();

	COleDocument::DeleteContents();
}


