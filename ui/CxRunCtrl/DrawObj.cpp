// DrawItem.cpp: implementation of the CDrawObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RunInc.h"

#include "CxHmiMisc_i.c"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



IMPLEMENT_SERIAL(CDrawObj, CDevObj, 0)

CDrawObj::CDrawObj(CDevObjDoc* pContainerDoc) :
	CDevObj(pContainerDoc)	
{
	m_pDrawObject = NULL;
	m_pDispatch = NULL;

	m_bNeedLoadOrSaveStorage = FALSE;
}

CDrawObj::~CDrawObj()
{
}

BEGIN_INTERFACE_MAP(CDrawObj, CDevObj)
	INTERFACE_PART(CDrawObj, IID_ICxDrawObjectAdviseSink, DrawObjectAdviseSink)
	INTERFACE_PART(CDrawObj, IID_ICxDrawObjectSite, DrawObjectSite)
	INTERFACE_PART(CDrawObj, IID_IDispatch, AmbientProperties)
END_INTERFACE_MAP()

///////////////////////////////////////////////////////////////////////////

STDMETHODIMP_( ULONG ) CDrawObj::XDrawObjectSite::AddRef()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

//	TRACE("XDrawObjectSite::AddRef().\n");

	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CDrawObj::XDrawObjectSite::Release()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

//	TRACE("XDrawObjectSite::Release().\n");

	return pThis->ExternalRelease();
}

STDMETHODIMP CDrawObj::XDrawObjectSite::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CDrawObj::XDrawObjectSite::SetCapture(BOOL bCapture)
{
	HRESULT hResult;
	
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite);

	if (bCapture)
	{
		hResult = ((CRunView *)pThis->m_pDocument->GetRunView())->OnWindowlessSetCapture(pThis);
		if (FAILED(hResult))
		{
			return hResult;
		}
	}
	else
	{
		hResult = ((CRunView *)pThis->m_pDocument->GetRunView())->OnWindowlessReleaseCapture(pThis);
		if (FAILED(hResult))
		{
			return hResult;
		}
	}

	return hResult;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetCapture()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	if (((CRunView *)pThis->m_pDocument->GetRunView())->GetObjectWithCapture() == pThis)
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

STDMETHODIMP CDrawObj::XDrawObjectSite::SetFocus(BOOL bFocus)
{
	HRESULT hResult;
	
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite);

	if (bFocus)
	{
		hResult = ((CRunView *)pThis->m_pDocument->GetRunView())->OnWindowlessSetFocus(pThis);
		if (FAILED(hResult))
		{
			return hResult;
		}
	}
	else
	{
		hResult = ((CRunView *)pThis->m_pDocument->GetRunView())->OnWindowlessReleaseFocus();
		if (FAILED(hResult))
		{
			return hResult;
		}
	}

	return hResult;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetFocus()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	if (((CRunView *)pThis->m_pDocument->GetRunView())->GetObjectWithFocus() == pThis)
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetEventObject(ICxEventObj** ppEvent)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	CRunView* pView = (CRunView *)pThis->m_pDocument->GetRunView();
	CFormScrEng* pEngine = pView->GetDocument()->GetScriptEngine();
	pEngine->ActiveEventObject(pView);
	*ppEvent = pEngine->m_pEventObj;

	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::IsRunMode()
{
	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetClientName(BSTR *pbstrName)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	*pbstrName = pThis->GetDisplayName().AllocSysString();
	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::SetClientName(BSTR bstrName)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	return E_NOTIMPL;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetClientLayer(long *plLayer)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	*plLayer = long(pThis->m_nLayer + 1);
	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::SetClientLayer(long lLayer)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	if(lLayer > 30 || lLayer < 1)
		return E_INVALIDARG;

	pThis->m_nLayer = lLayer - 1;
	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetClientLocked(BOOL* pbLocked)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	*pbLocked = pThis->m_bLocked;
	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::SetClientLocked(BOOL bLocked)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	pThis->m_bLocked = bLocked;
	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetClientVisibility(BOOL* pbVisible)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	*pbVisible = pThis->m_bVisible;
	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::SetClientVisibility(BOOL bVisible)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	pThis->m_bVisible = bVisible;
	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetClientToolTip(BSTR *pbstrText)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	*pbstrText = pThis->m_strToolTip.AllocSysString();
	return S_OK;
};

STDMETHODIMP CDrawObj::XDrawObjectSite::SetClientToolTip(BSTR bstrText)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	pThis->m_strToolTip = bstrText;

	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetClientPrivilege(long *plPrivilege)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	*plPrivilege = long(pThis->m_nPrivilege);
	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::SetClientPrivilege(long lPrivilege)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	if(lPrivilege > 1000 || lPrivilege < 0)
		return E_INVALIDARG;

	pThis->m_nPrivilege = lPrivilege;
	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::OnMouseInput(const WCHAR* szPropName, BOOL bProcess)
{
	USES_CONVERSION;
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	return pThis->OnMouseInput(W2T(szPropName), bProcess) ? S_OK : S_FALSE;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::SetTimer(UINT nElapse)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	CDevObjList* pObjList = ((CFormDoc *)pThis->m_pDocument)->GetTimerObjects();
	POSITION pos = pObjList->Find(pThis);

	if (nElapse != 0)
	{
		if (pos == NULL)
			pObjList->AddTail(pThis);
	}
	else
	{
		if (pos != NULL)
			pObjList->RemoveAt(pos);
	}

	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::TrackMouseLeave()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	CRunView* pView = ((CRunView *)pThis->m_pDocument->GetRunView());
	pView->TrackObjectMouseLeave(pThis);

	return S_FALSE;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetExpressionCreatorProc(long lProc)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	lProc = 0;

	return S_FALSE;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetWindow(HWND* phWindow)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	
	*phWindow = ((CRunView *)pThis->m_pDocument->GetRunView())->GetSafeHwnd();
	
	return S_OK;   
}

STDMETHODIMP CDrawObj::XDrawObjectSite::InvalidateRect(LPCRECT prect, BOOL tErase)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	CRunView* pView = (CRunView *)pThis->m_pDocument->GetRunView();
	CRect rect(prect);
	rect.NormalizeRect();
	pView->InvalidateRectEx(&rect);
//
//	CRect rect;
//	
//	if (prect == NULL)
//		rect = pThis->GetBoundingRect();
//	// Intersect the given rectangle with the bounds of the object.
//	else
//		rect.IntersectRect(prect, pThis->GetBoundingRect());
//
//	
//	if (!rect.IsRectEmpty())
//	{
//		CRunView* pView = (CRunView *)pThis->m_pDocument->GetRunView();
//		pView->InvalidateRect(&rect, tErase);
////		pView->UpdateWindow();
//	}

	
	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::InvalidateRgn(HRGN hRgn, BOOL tErase)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

//	CRgn rgn;
	CRgn* pRgn;
//	CRect rectRgn;
//	CRect rect;
	
	// The object wants its full extent invalidated.  It's much easier to do
	// this with a rect than a region.
	if (hRgn == NULL)
		return InvalidateRect(NULL, tErase);
	
	pRgn = CRgn::FromHandle(hRgn);
//	pRgn->GetRgnBox(&rectRgn);
//	rect.IntersectRect(&rectRgn, &pThis->GetBoundingRect());
//	if (rect != rectRgn)
//		return InvalidateRect(NULL, tErase);
	
//	((CRunView *)pThis->m_pDocument->GetRunView())->InvalidateRgn(pRgn, tErase);
	
	return S_OK;
}


STDMETHODIMP CDrawObj::XDrawObjectSite::GetDC(HDC* phDC)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite); 
	
	if (phDC == NULL)
		return E_INVALIDARG;
	
	if (pThis->m_pDC == NULL)
	{
		CRunView * pView = (CRunView *)pThis->m_pDocument->GetRunView();
		pThis->m_pDC = pView->GetDC();
		pView->OnPrepareDC(pThis->m_pDC);
	}
	ASSERT(pThis->m_pDC != NULL);
	
	*phDC = pThis->m_pDC->GetSafeHdc();
	
	return S_OK;   
}

STDMETHODIMP CDrawObj::XDrawObjectSite::ReleaseDC(HDC hDC)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite); 
	
	ASSERT(hDC == pThis->m_pDC->GetSafeHdc());
	
	((CRunView *)pThis->m_pDocument->GetRunView())->ReleaseDC(pThis->m_pDC);
	pThis->m_pDC = NULL;
	
	return S_OK;   
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetDefaultServer(BSTR *pbstrComputer)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite); 
	
	*pbstrComputer = ((CFormDoc *)pThis->m_pDocument)->GetDefaultServer().AllocSysString();
	
	return S_OK;  
}

STDMETHODIMP CDrawObj::XDrawObjectSite::SetNeedStorage()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite); 
	
	pThis->m_bNeedLoadOrSaveStorage = TRUE;
	
	return S_OK;  
}

STDMETHODIMP CDrawObj::XDrawObjectSite::DocToClient(RECT* pRect)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite); 
	
	((CRunView *)pThis->m_pDocument->GetRunView())->DocToClient((CRect&)(*pRect));
	
	return S_OK; 
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetFillBoundRect(RECT** ppRect)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
		
		CGroupObj* pGroupObj = pThis->m_pGroupObj;
	if (pGroupObj != NULL)
		*ppRect = pGroupObj->GetFillBoundRect();
	
	return S_OK;
}

////////////////////////////////////////////////////////////////
STDMETHODIMP_( ULONG ) CDrawObj::XDrawObjectAdviseSink::AddRef()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectAdviseSink)

//	TRACE("XDrawObjectAdviseSink::AddRef().\n");

	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CDrawObj::XDrawObjectAdviseSink::Release()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectAdviseSink)

//	TRACE("XDrawObjectAdviseSink::Release().\n");

	return pThis->ExternalRelease();
}

STDMETHODIMP CDrawObj::XDrawObjectAdviseSink::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectAdviseSink)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CDrawObj::XDrawObjectAdviseSink::OnViewChange()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectAdviseSink)

	pThis->OnChange();
	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectAdviseSink::OnDynamicChange()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectAdviseSink)

	pThis->OnDynamicChange();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// CDrawObj::XAmbientProperties
///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_( ULONG ) CDrawObj::XAmbientProperties::AddRef()
{
	METHOD_PROLOGUE(CDrawObj, AmbientProperties)

	TRACE("XAmbientProperties::AddRef().\n");

	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CDrawObj::XAmbientProperties::Release()
{
	METHOD_PROLOGUE(CDrawObj, AmbientProperties)

	TRACE("XAmbientProperties::Release().\n");

	return pThis->ExternalRelease();
}

STDMETHODIMP CDrawObj::XAmbientProperties::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CDrawObj, AmbientProperties)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CDrawObj::XAmbientProperties::GetIDsOfNames(
	REFIID iid, LPOLESTR* ppszNames, UINT nNames, LCID lcid, DISPID* pDispIDs )
{
	CDevObjDoc* pDoc;
	CAmbientProperty* pProp;
	UINT iName;
	HRESULT hResult;

	(void)lcid;

	METHOD_PROLOGUE(CDrawObj, AmbientProperties)
	ASSERT_VALID( pThis );

	if (!IsEqualIID(iid, IID_NULL))
	{
		return E_INVALIDARG;
	}
	if (ppszNames == NULL)
	{
		return E_INVALIDARG;
	}
	if (nNames < 1)
	{
		return E_INVALIDARG;
	}
	if (pDispIDs == NULL)
	{
	  return E_POINTER;
	}

	hResult = S_OK;
	pDoc = pThis->m_pDocument;
	pProp = pDoc->FindAmbientProperty(ppszNames[0]);
	if (pProp == NULL)
	{
		pDispIDs[0] = DISPID_UNKNOWN;
		hResult = DISP_E_UNKNOWNNAME;
	}

	pDispIDs[0] = pProp->GetID();

	if (nNames > 1)
	{
		for (iName = 1; iName < nNames; iName++)
		{
			pDispIDs[iName] = DISPID_UNKNOWN;
		}
		hResult = DISP_E_UNKNOWNNAME;
	}

	return( hResult );
}

STDMETHODIMP CDrawObj::XAmbientProperties::GetTypeInfo(
	UINT iTypeInfo, LCID lcid, ITypeInfo** ppTypeInfo )
{
	(void)iTypeInfo;
	(void)lcid;
	(void)ppTypeInfo;

	METHOD_PROLOGUE(CDrawObj, AmbientProperties)
	ASSERT_VALID(pThis);

	return E_NOTIMPL;
}

STDMETHODIMP CDrawObj::XAmbientProperties::GetTypeInfoCount(
	UINT* pnInfoCount )
{
	METHOD_PROLOGUE(CDrawObj, AmbientProperties)
	ASSERT_VALID(pThis);

	*pnInfoCount = 0;

	return S_OK;
}

STDMETHODIMP CDrawObj::XAmbientProperties::Invoke(
	DISPID dispidMember, REFIID iid, LCID lcid, WORD wFlags,
	DISPPARAMS* pdpParams, VARIANT* pvarResult, EXCEPINFO* pExceptionInfo,
	UINT* piArgErr )
{
	CDevObjDoc* pDoc;
	CAmbientProperty* pProp;
	HRESULT hResult;
	COleVariant varValue;

	(void)iid;
	(void)lcid;
	(void)wFlags;
	(void)pdpParams;
	(void)pExceptionInfo;
	(void)piArgErr;

	METHOD_PROLOGUE(CDrawObj, AmbientProperties)
	ASSERT_VALID( pThis );

	pDoc = pThis->m_pDocument;

	pProp = pDoc->FindAmbientProperty(dispidMember);
	if (pProp == NULL)
	{
		TRACE("Control asked for unknown ambient property.  DISPID=%d\n", dispidMember );
		return DISP_E_MEMBERNOTFOUND;
	}

	if (!pProp->IsEnabled())
	{
		TRACE("Control asked for ambient property %s, but the property was disabled.\n",
			LPCTSTR(pProp->GetName()));
		return DISP_E_MEMBERNOTFOUND;
	}

	TRACE("Control got ambient property %s.\n", LPCTSTR(pProp->GetName()));

	varValue = pProp->GetValue();
	hResult = VariantCopy( pvarResult, &varValue );
	if (FAILED(hResult))
	{
		return hResult;
	}

	return S_OK;
}

////////////////////////////////////////////////////////////////////////

BOOL CDrawObj::FinishCreate(HRESULT hr)
{
	ICxDrawObjectAdviseSink* lpAdviseSink =
		(ICxDrawObjectAdviseSink *)GetInterface(&IID_ICxDrawObjectAdviseSink);
	ASSERT(lpAdviseSink != NULL);
	VERIFY(m_pDrawObject->SetAdvise(lpAdviseSink) == S_OK);

	m_pDrawObject->QueryInterface(IID_IDispatch, (void **)&m_pDispatch);

	return TRUE;
}

void CDrawObj::Release()
{
	CDevObj::Release();

	if (m_pDispatch != NULL)
	{	
		m_pDispatch->Release();
		m_pDispatch = NULL;
	}

	if (m_pDrawObject != NULL)
	{	
		m_pDrawObject->Release();
		m_pDrawObject = NULL;
	}

	CoFreeUnusedLibraries();
}

void CDrawObj::DrawFrontToBack(CDC* pDC)
{
	ASSERT(m_pDrawObject != NULL);
	m_pDrawObject->Draw(pDC->m_hDC);
}

inline void CDrawObj::Draw(CDC* pDC)
{
	if (m_bVisible)
		m_pDrawObject->Draw(pDC->m_hDC);
}

inline void CDrawObj::Print(CDC* pDC)
{
	if (m_bVisible)
		m_pDrawObject->Print(pDC->m_hDC);
}

void CDrawObj::OnChange()
{
	ASSERT_VALID(this);

	Invalidate(NULL);
//	GetDocument()->UpdateAllViews(NULL);
}

void CDrawObj::Serialize(CArchive& ar)
{
	CDevObj::Serialize(ar);

	if (ar.IsStoring())
	{
		//保存对象
		WriteObject(ar);
	}
	else
	{
		//读入对象
		ReadObject(ar);

		// 完成对象创建过程，事件连接等
		if (!FinishCreate(S_OK))
			AfxThrowArchiveException(CArchiveException::generic);
	}
}

void CDrawObj::ReadObject(CArchive& ar)
{
	HRESULT hResult;
	CLSID clsid;

	(void)ar;

	CArchiveStream stm(&ar);
	LPSTREAM pStream = &stm;

	hResult = ReadClassStm(pStream, &clsid);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	hResult = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER|
		CLSCTX_INPROC_HANDLER|CLSCTX_LOCAL_SERVER, IID_ICxDrawObject,
		(void**)&m_pDrawObject);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	//设置站点
	hResult = m_pDrawObject->SetDrawObjectSite(&m_xDrawObjectSite);
	if (FAILED(hResult))
	{
		 TRACE("SetDrawObjectSite failed.\n");
	}

	hResult = m_pDrawObject->Load(theAppKey, pStream);
	
	//简化的错误处理
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}
	
	if (m_bNeedLoadOrSaveStorage)
	{
		USES_CONVERSION;
		IStoragePtr pStorage;
		TCHAR szObjectName[64];
		CFormDoc* pDoc;
		
		pDoc = (CFormDoc*)m_pDocument;
		
		// 得到项目名称
		GetObjectName(szObjectName);
		
		// 打开此项目存储
		hResult = pDoc->m_lpRootStg->OpenStorage(T2COLE(szObjectName), NULL,
			STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE, 0, 0, &pStorage);
		if (hResult != S_OK)
		{
			TRACE1("Warning: unable to open child storage %s.\n", szObjectName);
			// upon failure throw file exception (item will be cleaned up)
			AfxThrowOleException( hResult );
		}
		ASSERT(pStorage != NULL);
		
		hResult = m_pDrawObject->LoadStorage(pStorage);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}
	}
}

void CDrawObj::WriteObject(CArchive& ar)
{
	HRESULT hResult;
	CLSID clsid;

	(void)ar;

	CArchiveStream stm(&ar);
	LPSTREAM pStream = &stm;

	//保存类全局号
	m_pDrawObject->GetClassID(&clsid);
	hResult = WriteClassStm(pStream, clsid);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	//保存数据
//	DWORD dwVer = 0x10001;
//	pStream->Write(&dwVer, 4, NULL);
	hResult = m_pDrawObject->Save(pStream, TRUE);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	if (m_bNeedLoadOrSaveStorage)
	{
		USES_CONVERSION;
		IStoragePtr pStorage;
		TCHAR szObjectName[64];
		HRESULT hResult;
		CFormDoc* pDoc;
		
		pDoc = (CFormDoc*)m_pDocument;
		ASSERT_VALID(pDoc);
		ASSERT(pDoc->m_lpRootStg != NULL);
		ASSERT(pDoc->m_bCompoundFile);
		
		GetObjectName(szObjectName);
		
		//相当于另存为
		hResult = pDoc->m_lpRootStg->CreateStorage(T2COLE(szObjectName),
			STGM_CREATE|STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE, 0, 0,
			&pStorage);
		if (FAILED(hResult))
			AfxThrowOleException(hResult);
		
		hResult = m_pDrawObject->SaveStorage(pStorage, pDoc->m_bSameAsLoad);
		if (FAILED(hResult))
			AfxThrowOleException(hResult);
		
		hResult = pStorage->Commit(STGC_ONLYIFCURRENT);
		if (FAILED(hResult))
			AfxThrowOleException(hResult);
	}
}

void CDrawObj::SaveProperty(CArchive& ar)
{
	HRESULT hResult;
	
	(void)ar;

	CArchiveStream stm(&ar);
	LPSTREAM pStream = &stm;

	//保存数据
	hResult = m_pDrawObject->Save(pStream, TRUE);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}
}

void CDrawObj::RestoreProperty(CArchive& ar) 
{
	HRESULT hResult;

	(void)ar;

	CArchiveStream stm(&ar);
	LPSTREAM pStream = &stm;

	hResult = m_pDrawObject->Load(theAppKey, pStream);

	//简化的错误处理
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}
}

BOOL CDrawObj::IsDefaultMousePointer()
{
	return m_pDrawObject->IsDefaultMousePointer() == S_OK;
}

////////////////////////////////////////////////////////////////////////
//功 能：基本图形对象处理消息
//参 数：2、如果是图形对象，则有CRunView调用，参数为true，
//否则为组对象函数嵌套调用，参数为false
//描 述：要注意处理组对象中嵌套子对象的消息（因为可能连接鼠标输入动态对象）
BOOL CDrawObj::OnWindowMessage(const MSG* pMessage, BOOL bSubObj)
{
	LRESULT lResult;
	HRESULT hResult;

	LPARAM lParam = pMessage->lParam;

	if (pMessage->message >= WM_MOUSEFIRST && pMessage->message <= WM_MOUSELAST)
	{
		CPoint point((DWORD)lParam);
		((CRunView *)m_pDocument->GetRunView())->ClientToDoc(point);
		lParam = MAKELPARAM(point.x, point.y);
	}

	hResult = m_pDrawObject->OnWindowMessage(pMessage->message,
		pMessage->wParam, lParam, bSubObj, &lResult);

	return (hResult == S_OK);
}

BOOL CDrawObj::HitTest(CPoint point, const CRunView* pView)
{
	ASSERT(m_pDrawObject != NULL);

	if (!m_bVisible)
		return FALSE;
	
	BOOL bHitTest;
	m_pDrawObject->HitTest(point, &bHitTest);
	return bHitTest;
}

void CDrawObj::MoveTo(const CRectF& rect, BOOL bUniformScale, CRunView* pView)
{
	ASSERT(m_pDrawObject != NULL);

	Invalidate(pView);
	m_pDrawObject->MoveTo(rect, bUniformScale);
	Invalidate(pView);
}

void CDrawObj::RotateAt(CPointF point, float fAngle, CRunView* pView )
{
	ASSERT(m_pDrawObject != NULL);

	m_pDrawObject->RotateAt(point, fAngle);
}

void CDrawObj::ScaleAt(CPointF point, float fHorScale, float fVerScale, CRunView* pView)
{
	ASSERT(m_pDrawObject != NULL);

	m_pDrawObject->ScaleAt(point, fHorScale, fVerScale);
}

CRect CDrawObj::GetBoundingRect()
{
	ASSERT(m_pDrawObject != NULL);

	CRect rect;
	rect.SetRectEmpty();
	m_pDrawObject->GetBoundingRect(&rect);
	return rect;
}

CRectF CDrawObj::GetPositionRect()
{
	ASSERT(m_pDrawObject != NULL);

	CRectF rect;
	rect.SetRectEmpty();
	m_pDrawObject->GetPositionRect(&rect);
	return rect;
}

IDispatch* CDrawObj::GetObjectDispatch()
{
	return m_pDispatch;
}

void CDrawObj::InitRun()
{
	ASSERT(m_pDrawObject != NULL);

	BOOL bHasDynamic;
	m_pDrawObject->InitRun(&bHasDynamic, &m_xScriptEngineSite);
	if (bHasDynamic)
		((CFormDoc *)m_pDocument)->m_dynamicobjects.AddTail(this);

}

void CDrawObj::FakeParseDynamic()
{
	ASSERT(m_pDrawObject != NULL);
	
	m_pDrawObject->FakeParseDynamic();
}

void CDrawObj::ParseDynamic()
{
	ASSERT(m_pDrawObject != NULL);

	m_pDrawObject->ParseDynamic();
}

void CDrawObj::OnTimer()
{
	ASSERT(m_pDrawObject != NULL);

	m_pDrawObject->OnTimer();
}
void CDrawObj::OnMouseLeave()
{
	ASSERT(m_pDrawObject != NULL);

	m_pDrawObject->OnMouseLeave();
}

LPUNKNOWN CDrawObj::GetObjectUnknown()
{
	return m_pDrawObject;
	//用下面的不能访问扩展属性
//	LPUNKNOWN lpUnknown = NULL;
//	m_pDrawObject->QueryInterface(IID_IUnknown, (void**)&lpUnknown);
	
//	return lpUnknown;
}

CString CDrawObj::GetClassName()
{
	BSTR bstrClassName;
	
	ASSERT(m_pDrawObject != NULL);
	m_pDrawObject->GetClassName(&bstrClassName);
	return CString(bstrClassName);
}

