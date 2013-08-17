// DevDoc.cpp : implementation file
//

#include "stdafx.h"
#include "RunInc.h"
#include "RunCtrl.h"

#include "../../svr/CxSAC/CxSAC_i.c"
#include "../../svr/CxDatSes/CxDatSes_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDevDoc


CPtrList CDevDoc::c_listDevDoc;

IMPLEMENT_DYNCREATE(CDevDoc, COleDocument)

CDevDoc::CDevDoc()
{
	m_dwVer = 0;
	m_pRunCtrl = NULL;

	m_pIDataSession = NULL;
	m_pMainForm = NULL;

	EnableCompoundFile();
}

BOOL CDevDoc::OnNewDocument()
{
	if (!COleDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CDevDoc::~CDevDoc()
{
	POSITION pos = m_mapFrameItems.GetStartPosition();
	while (pos != NULL)
	{
		CString strItem;
		VARIANT* pVar;
		m_mapFrameItems.GetNextAssoc(pos, strItem, pVar);
		VariantClear(pVar);
		delete pVar;
	}
	m_mapFrameItems.RemoveAll();
}


BEGIN_MESSAGE_MAP(CDevDoc, COleDocument)
	//{{AFX_MSG_MAP(CDevDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
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
	ASSERT(ar.IsLoading());
	
	ar >> m_dwVer;

#if _SUPERCX_DEVELOP_LICENSE
	if ((m_dwVer & 0x10000) == 0)
	{
		MessageBox(NULL, _T("非法的画面文件格式！"), _T("错误"), MB_OK | MB_ICONERROR);
		AfxThrowArchiveException(CArchiveException::badSchema);
	}

	m_dwVer = 0xFFFF & m_dwVer;
#endif

	int nCount;
	ar >> nCount;

#if _SUPERCX_DEVELOP_LICENSE
	nCount = CSoftLock::DecryptDataBak(nCount);
#endif

	int nType;
	CDevDoc::CSubDoc* pSubDoc;
	for (int i = 0; i < nCount; i++)
	{
		ar >> nType;

		if ((enum SUBDOCTYPE)nType == enumFormDoc)
			pSubDoc = new CFormDoc::CFormSubDoc;
		else if ((enum SUBDOCTYPE)nType == enumReportDoc) 
			pSubDoc = new CReportDoc::CReportSubDoc;
		else 
		//	pSubDoc = new CCodeModuleDoc::CCodeSubDoc;
			continue;

		pSubDoc->Serialize(ar);
		m_listSubDoc.AddTail(pSubDoc);

		CString strName = pSubDoc->m_strName;
		strName.MakeUpper();
		m_mapNameToSubDocument.SetAt(strName, pSubDoc);
		m_mapNumberToSubDocument.SetAt((WORD)pSubDoc->m_dwDocumentNumber, pSubDoc);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDevDoc commands

CDevDoc::CSubDoc* CDevDoc::FindSubDocumentByName(LPCTSTR lpszName)
{
	CSubDoc* pSubDoc = NULL;
	m_mapNameToSubDocument.Lookup(lpszName, (void* &)pSubDoc);

	return pSubDoc;
}

CDevDoc::CSubDoc* CDevDoc::FindSubDocumentByNumber(DWORD dwNumber)
{
	CSubDoc* pSubDoc = NULL;
	m_mapNumberToSubDocument.Lookup((WORD)dwNumber, (void* &)pSubDoc);
	
	return pSubDoc;
}

CDocument* CDevDoc::ShowPicture(CRunCtrl* pRunCtrl, LPCTSTR lpszFileName)
{
	CDevDoc* pDoc = new CDevDoc();
	if (pDoc == NULL)
		return NULL;
	ASSERT_VALID(pDoc);
	
	pDoc->m_pRunCtrl = pRunCtrl;
	pRunCtrl->m_pPictureDoc = pDoc;

	// open an existing document
	CWaitCursor wait;
	if (!pDoc->OnOpenDocument(lpszFileName))
	{
		delete pDoc;
		pRunCtrl->m_pPictureDoc = NULL;
		return NULL;
	}
	
	c_listDevDoc.AddTail(pDoc);

	return pDoc;
}

///////////////////////////////////////////////////////////////////
//功 能：打开指定文件路径文件
///////////////////////////////////////////////////////////////////
CDocument* CDevDoc::OpenDocumentFile(LPCTSTR lpszFileName)
{
	CDevDoc* pDoc = new CDevDoc();
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

void CDevDoc::CloseAllDocuments(BOOL bEndSession)
{
	POSITION pos = c_listDevDoc.GetHeadPosition();
	while (pos != NULL)
	{
		CDevDoc* pDoc = (CDevDoc*)c_listDevDoc.GetNext(pos);
	//	pDoc->RemoveAllSubDocument();
		pDoc->OnCloseDocument(); 
	}
//	c_listDevDoc.RemoveAll();
}

///////////////////////////////////////////////////////////////////
//功 能：移除所有子文档
///////////////////////////////////////////////////////////////////
void CDevDoc::RemoveAllSubDocument()
{
	POSITION pos = m_listSubDoc.GetHeadPosition();
	while (pos != NULL)
	{
		CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc *)m_listSubDoc.GetNext(pos); 
		if (pSubDoc->m_pChildDoc != NULL)
			pSubDoc->m_pChildDoc->OnCloseDocument();
		delete pSubDoc;
	}
	m_listSubDoc.RemoveAll();
	m_mapNameToSubDocument.RemoveAll();
	m_mapNumberToSubDocument.RemoveAll();
}

void CDevDoc::ResetAllRunView()
{
	POSITION pos = m_listSubDoc.GetHeadPosition();
	while (pos != NULL)
	{
		CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc *)m_listSubDoc.GetNext(pos);
		CFormDoc* pFormDoc = (CFormDoc *)pSubDoc->m_pChildDoc;
		if (pFormDoc != NULL)
		{
			CRunView* pView = (CRunView *)pFormDoc->GetRunView();
			if (pView != NULL)
				pView->ReSetTimer(TRUE, TRUE, FALSE);
		}
	}
}

void CDevDoc::SetAllRunView(BOOL bTimerEnable, BOOL bZoomFitWindow, 
	float fZoomRatio, float fZoomX, float fZoomY, BOOL bAllowMouseZoom, BOOL bAllowScrollBars, BOOL bDragMode)
{
	POSITION pos = m_listSubDoc.GetHeadPosition();
	while (pos != NULL)
	{
		CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc *)m_listSubDoc.GetNext(pos);
		CFormDoc* pFormDoc = (CFormDoc *)pSubDoc->m_pChildDoc;
		if (pFormDoc != NULL)
		{
			CRunView* pView = (CRunView *)pFormDoc->GetRunView();
			if (pView != NULL)
			{
				pView->SetTimerEnabled(bTimerEnable);
				if (pFormDoc->IsMainForm())
				{
					pView->SetZoomFitWindow(bZoomFitWindow, fZoomRatio, fZoomX, fZoomY);
					pView->SetAllowMouseZoom(bAllowMouseZoom);
					pView->SetAllowScrollBars(bAllowScrollBars);
					pView->SetDragMode(bDragMode);
				}
			}
		}
	}
}

CRunView* CDevDoc::GetActiveRunView()
{
	CWnd* pActiveWnd = CWnd::GetActiveWindow();
	if (pActiveWnd != NULL && pActiveWnd->IsKindOf(RUNTIME_CLASS(CPopupFrame)))
	{
		CPopupFrame* pFrame = (CPopupFrame *)pActiveWnd;
		return (CRunView *)pFrame->GetActiveView();
	}

	if (m_listSubDoc.GetCount() > 0)
	{
		CFormDoc::CFormSubDoc* pSubDoc = (CFormDoc::CFormSubDoc *)m_listSubDoc.GetHead(); 
		CFormDoc* pFormDoc = (CFormDoc *)pSubDoc->m_pChildDoc;
		return (CRunView *)pFormDoc->GetRunView();
	}

	return NULL;
}

DWORD AFXAPI _AfxRelease2(LPUNKNOWN* lplpUnknown)
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

// helper for reliable and small Release calls
DWORD AFXAPI _AfxRelease2(LPUNKNOWN* plpUnknown);
#ifndef _DEBUG
// generate smaller code in release build
#define RELEASE(lpUnk) _AfxRelease2((LPUNKNOWN*)&lpUnk)
#else
// generate larger but typesafe code in debug build
#define RELEASE(lpUnk) do \
{ if ((lpUnk) != NULL) { (lpUnk)->Release(); (lpUnk) = NULL; } } while (0)
#endif

// special exception handling just for MFC library implementation
#ifndef _AFX_OLD_EXCEPTIONS

// MFC does not rely on auto-delete semantics of the TRY..CATCH macros,
//  therefore those macros are mapped to something closer to the native
//  C++ exception handling mechanism when building MFC itself.

#undef TRY
#define TRY { try {

#undef CATCH
#define CATCH(class, e) } catch (class* e) \
{ ASSERT(e->IsKindOf(RUNTIME_CLASS(class))); UNUSED(e);

#undef AND_CATCH
#define AND_CATCH(class, e) } catch (class* e) \
{ ASSERT(e->IsKindOf(RUNTIME_CLASS(class))); UNUSED(e);

#undef CATCH_ALL
#define CATCH_ALL(e) } catch (CException* e) \
{ { ASSERT(e->IsKindOf(RUNTIME_CLASS(CException))); UNUSED(e);

#undef AND_CATCH_ALL
#define AND_CATCH_ALL(e) } catch (CException* e) \
{ { ASSERT(e->IsKindOf(RUNTIME_CLASS(CException))); UNUSED(e);

#undef END_TRY
#define END_TRY } catch (CException* e) \
{ ASSERT(e->IsKindOf(RUNTIME_CLASS(CException))); e->Delete(); } }

#undef THROW_LAST
#define THROW_LAST() throw

// Because of the above definitions of TRY...CATCH it is necessary to
//  explicitly delete exception objects at the catch site.

#define DELETE_EXCEPTION(e)// do { e->Delete(); } while (0)
#define NO_CPP_EXCEPTION(expr)

#else   //!_AFX_OLD_EXCEPTIONS

// In this case, the TRY..CATCH macros provide auto-delete semantics, so
//  it is not necessary to explicitly delete exception objects at the catch site.

#define DELETE_EXCEPTION(e)
#define NO_CPP_EXCEPTION(expr) expr

#endif  //_AFX_OLD_EXCEPTIONS

BOOL CDevDoc::OpenCacheFile(LPCTSTR lpszPathName)
{
	USES_CONVERSION;
	
	ASSERT(lpszPathName == NULL || AfxIsValidString(lpszPathName));
	ASSERT(theApp.m_pCacheFileManager != NULL);

	// abort changes to current docfile
	if (lpszPathName != NULL)
	{
		DeleteContents();
		RELEASE(m_lpRootStg);
	}
	SetModifiedFlag();  // dirty during de-serialize

	CCacheFile* pCacheFile = theApp.m_pCacheFileManager->OpenCacheFile(lpszPathName, m_pIDataSession);
	if (pCacheFile == NULL)
		return FALSE;
	
	BOOL bResult = FALSE;
	TRY
	{
		if (m_lpRootStg == NULL)
		{
			DWORD dwBytes = pCacheFile->GetFileSize();
			// allocate enough memory to read entire block
			HGLOBAL hStorage = ::GlobalAlloc(GMEM_SHARE|GMEM_MOVEABLE, dwBytes);
			if (hStorage == NULL)
				AfxThrowMemoryException();
			
			LPVOID lpBuf = ::GlobalLock(hStorage);
			ASSERT(lpBuf != NULL);
			memcpy(lpBuf, pCacheFile->GetBuffer(), dwBytes);
			::GlobalUnlock(hStorage);
			
			LPLOCKBYTES lpLockBytes = NULL;
			SCODE sc = CreateILockBytesOnHGlobal(hStorage, TRUE, &lpLockBytes);
			if (sc != S_OK)
			{
				::GlobalFree(hStorage);
				AfxThrowOleException(sc);
			}
			ASSERT(lpLockBytes != NULL);
			ASSERT(::StgIsStorageILockBytes(lpLockBytes) == S_OK);
			
			sc = ::StgOpenStorageOnILockBytes(lpLockBytes, NULL,
				STGM_SHARE_EXCLUSIVE|STGM_READWRITE, NULL, 0, &m_lpRootStg);
			if (sc != S_OK)
			{
				VERIFY(lpLockBytes->Release() == 0);
				lpLockBytes = NULL;
				// ILockBytes::Release will GlobalFree the hStorage
				AfxThrowOleException(sc);
			}
							
			// 必须立即释放，否则内存泄漏
			lpLockBytes->Release();
			ASSERT(m_lpRootStg != NULL);
		}
		
		// use helper to read document from storage
		LoadFromStorage();

		SetModifiedFlag(FALSE); // start off with unmodified
		bResult = TRUE;
	}
	CATCH_ALL(e)
	{
		DeleteContents();   // removed failed contents
		RELEASE(m_lpRootStg);
		
		// if not file-based load, return exceptions to the caller
		if (lpszPathName == NULL)
		{
			THROW_LAST();
			ASSERT(FALSE);  // not reached
		}
		
		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY
			
		DELETE_EXCEPTION(e);
	}
	END_CATCH_ALL
		
	return bResult;
}

///////////////////////////////////////////////////////////////
//功 能：打开画面
//描 述：此文挡已经打开过画面的处理
///////////////////////////////////////////////////////////////
BOOL CDevDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	USES_CONVERSION;

	ASSERT(m_pRunCtrl != NULL);
	if (!m_strPathName.IsEmpty())
		m_pRunCtrl->FirePictureClose(m_strPathName);
	
	BOOL bCancel = FALSE;
	m_pRunCtrl->FireBeforeShowPicture(lpszPathName, &bCancel);
	if (bCancel)
		return FALSE;

	//数据会话重置
	if (m_pIDataSession != NULL)
		m_pIDataSession->ResetTags();

	CDocument* pCurDoc = NULL;
	CDevDoc::CSubDoc* pCurSubDoc = NULL;
	if (m_listSubDoc.GetCount() >= 1)
		pCurSubDoc = (CDevDoc::CSubDoc *)m_listSubDoc.GetHead();
	if (pCurSubDoc != NULL)
	{	
		pCurDoc = pCurSubDoc->m_pChildDoc;	//得到主窗口文挡对象
		delete pCurSubDoc;
	}

	//跳过主窗口关闭子窗口
	POSITION pos = m_listSubDoc.GetHeadPosition();
	if (pos != NULL)
		m_listSubDoc.GetNext(pos);	
	while (pos != NULL)
	{
		CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc *)m_listSubDoc.GetNext(pos); 
		if (pSubDoc->m_pChildDoc != NULL)
		{
			pSubDoc->m_pChildDoc->OnCloseDocument();
		}
		delete pSubDoc;
	}

	m_listSubDoc.RemoveAll();
	m_mapNameToSubDocument.RemoveAll();
	m_mapNumberToSubDocument.RemoveAll();

	BOOL bCreateDataSession = FALSE;

	// 创建数据会话
	if (m_pIDataSession == NULL)
	{
		HRESULT hResult = ::CoCreateInstance(
			CLSID_CxDataSession,         
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_ICxDataSession,
			reinterpret_cast<void**>(&m_pIDataSession));
		if (FAILED(hResult))
		{
			AfxMessageBox(_T("创建数据会话失败！"));
			ASSERT(FALSE);
		}

		bCreateDataSession = TRUE;

	//	if (!m_pRunCtrl->m_server.IsEmpty())
	//			theApp.m_strLastServer = m_pRunCtrl->m_server;
	
			
		m_pIDataSession->SetThisNode(T2CW(m_pRunCtrl->m_server), NULL, FALSE);

		static bool bGlobalSet = false;

		//if (!bGlobalSet)
		{
			bGlobalSet = true;

			m_pIDataSession->SetCreateServerObjectAsLocal(m_pRunCtrl->m_bCreateServerObjectAsLocal);
			if (!m_pRunCtrl->m_strDefaultUser.IsEmpty())
				m_pIDataSession->SetDefaultUserPassword(T2CW(m_pRunCtrl->m_strDefaultUser), T2CW(m_pRunCtrl->m_strDefaultPassword));
			m_pIDataSession->SetSACProxy(m_pRunCtrl->m_bLocalServerUseSACProxy, TRUE, m_pRunCtrl->m_lPort);
		}

		//#ifdef _DEBUG
//		m_pIDataSession->SetThisNode(L"QinYong", NULL);
//#endif
	}

//	DWORD dwTick = GetTickCount();
	// 使用缓冲文件方式打开文件
	if (!OpenCacheFile(lpszPathName))
//	if (!COleDocument::OnOpenDocument(lpszPathName))
	{
		if (pCurDoc != NULL)
			pCurDoc->OnCloseDocument();
		return FALSE;
	}

//	dwTick = GetTickCount() - dwTick;
//	CString strText;
//	strText.Format("%d", dwTick);
//	AfxMessageBox(strText);

	m_strPathName = lpszPathName;
	if (m_strPathName.Left(7).CompareNoCase(_T("Server:")) != 0)
		SetPathName(lpszPathName);
//	SetDefaultTitle();

	//由前一个主窗口打开主窗口
	CFormDoc::CFormSubDoc* pSubDoc = (CFormDoc::CFormSubDoc *)m_listSubDoc.GetHead(); 
	pSubDoc->m_pChildDoc = pCurDoc;

	CFormDoc* pFormDoc = NULL;
	TRY
	{
		pFormDoc = (CFormDoc *)pSubDoc->LoadChildDocument();
		if (pFormDoc == NULL)
			return FALSE;
	}
	CATCH_ALL(e)
	{
		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY

		DELETE_EXCEPTION(e);

		return FALSE;
	}
	END_CATCH_ALL

	// 2005.7.7 加入判断是否为运行模式
	if (m_pRunCtrl->AmbientUserMode())
	{
		pFormDoc->ReadyToRun();
	
		if (bCreateDataSession)
		{
			LPWSTR pUserName = NULL;
			LPWSTR pSecurityAreas = NULL;
			DWORD dwUserPrivilege = 0;
			HRESULT hr = m_pIDataSession->GetUserAccessRight(&pUserName, NULL, &pSecurityAreas, &dwUserPrivilege);
			if (SUCCEEDED(hr))
			{
				if (pUserName != NULL)
				{
					theApp.m_strUserName = pUserName;
					CoTaskMemFree(pUserName);
				}
				theApp.m_dwUserPrivilege = dwUserPrivilege & 0xffff;
				theApp.m_bAdministrator = ((dwUserPrivilege & 0x80000) != 0);
				
				if (pSecurityAreas != NULL)
				{
					theApp.m_strUserSecurityAreas = OLE2T(pSecurityAreas);
					CoTaskMemFree(pSecurityAreas);
				}
			}
		}
	}

	m_pMainForm = pFormDoc;
	pFormDoc->ShowFrameWindow(TRUE, FALSE);

	m_pRunCtrl->FireShowPictureComplete(lpszPathName);

	return TRUE;
}

void CDevDoc::UpdateDataSession()
{
	m_pIDataSession->SetReadTagMode(ReadTagMode_Update);
	
	POSITION pos = m_listSubDoc.GetHeadPosition();
	while (pos != NULL)
	{
		CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc *)m_listSubDoc.GetNext(pos);
		pSubDoc->UpdateDataSession();
	}

	m_pIDataSession->CommitTags(TRUE);
	m_pIDataSession->SetReadTagMode(ReadTagMode_Normal);
}

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
LPSTORAGE CDevDoc::OpenSubDocumentStorage(CSubDoc* pSubDoc)
{
	USES_CONVERSION;
	CString strStorageName = GetStorageName(pSubDoc->m_dwDocumentNumber);

	// 打开子项目存储
	LPSTORAGE lpStorage = NULL;
	SCODE sc = m_lpRootStg->OpenStorage(T2OLE(strStorageName), NULL,
		STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE, 0, 0, &lpStorage );
	if (sc != S_OK)
	{
		TRACE1( "Warning: unable to open child storage %s.\n", strStorageName );
		AfxThrowOleException(sc);
	}
	ASSERT(lpStorage != NULL);
	
	return lpStorage;
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
		strDocName = "画面_未命名";
	//	VERIFY(strDocName.LoadString(AFX_IDS_UNTITLED));
		nUntitledCount = c_listDevDoc.GetCount();
		wsprintf(szNum, _T("%d"), nUntitledCount + 1);
		strDocName += szNum;
	}
	else
	{
		strDocName = "画面_";
		CString strFileName = m_strPathName;
		GetFileTitle(m_strPathName, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
		strFileName.ReleaseBuffer();
	
		strDocName += strFileName;//.Mid(5);
	}

	SetTitle(strDocName);
}

///////////////////////////////////////////////////////////////////////////////
//

CDevDoc::CSubDoc::CSubDoc()
{
	m_dwDocumentNumber = -1;
//	m_strFormName;
	m_pParentDoc = NULL;
	m_pChildDoc = NULL;
}

//////////////////////////////////////////////////////////////
//功能:是否为第一个子文档
//////////////////////////////////////////////////////////////
BOOL CDevDoc::CSubDoc::IsFirstSubDoc()
{
	return (m_dwDocumentNumber == 0);
}

CDocument* CDevDoc::CSubDoc::LoadChildDocument()
{
	ASSERT(FALSE);

	return NULL;
}

void CDevDoc::CSubDoc::UpdateDataSession()
{
}

void CDevDoc::OnCloseDocument() 
{
	ASSERT(m_pRunCtrl != NULL);
	if (!m_strPathName.IsEmpty())
		m_pRunCtrl->FirePictureClose(m_strPathName);

	m_pMainForm = NULL;

	RemoveAllSubDocument();
	c_listDevDoc.RemoveAt(c_listDevDoc.Find(this));

	if (m_pIDataSession != NULL)
	{
		m_pIDataSession->Release();
		m_pIDataSession = NULL;
	}

	COleDocument::OnCloseDocument();
}

BOOL CDevDoc::CanReplacePicture()
{
	POSITION pos = m_listSubDoc.GetHeadPosition();
	while (pos != NULL)
	{
		CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc *)m_listSubDoc.GetNext(pos); 
		if (pSubDoc->GetType() == CDevDoc::enumFormDoc && pSubDoc->m_pChildDoc != NULL)
		{
			CFormScrEng* pEngine = ((CFormDoc *)pSubDoc->m_pChildDoc)->GetScriptEngine();
			if (pEngine != NULL && pEngine->IsScriptRunning())
				return FALSE;
		} 
	}

	return TRUE;
}

VARIANT CDevDoc::GetFrame(LPCTSTR pszItem) 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	
	if (pszItem != NULL)
	{
		VARIANT* pVar = NULL;
		if (m_mapFrameItems.Lookup(pszItem, pVar))
			VariantCopy(&vaResult, pVar);
	}
	
	return vaResult;
}

void CDevDoc::SetFrame(LPCTSTR pszItem, const VARIANT FAR& newValue) 
{
	VARIANT* pVar = NULL;
	if (m_mapFrameItems.Lookup(pszItem, pVar))
	{
		VariantClear(pVar);
		if (newValue.vt == VT_NULL)
		{
			delete pVar;
			m_mapFrameItems.RemoveKey(pszItem);
			return;
		}
	}
	else
	{
		if (newValue.vt == VT_NULL)
			return;
		
		pVar = new VARIANT;
		VariantInit(pVar);
	}
	
	VariantCopy(pVar, (VARIANT *)&newValue);
	m_mapFrameItems.SetAt(pszItem, pVar);
}

