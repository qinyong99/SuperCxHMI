// DrawObj.cpp: implementation of the CDrawObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"
#include "ActionObj.h"

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
	m_bNeedLoadOrSaveStorage = FALSE;
}

CDrawObj::~CDrawObj()
{
	
}

BOOL CDrawObj::Create(REFCLSID clsid, LPCTSTR pszName)
{
   return CreateOrLoad(clsid, pszName, NULL);
}

LPUNKNOWN CDrawObj::GetObjectUnknown()
{
	return m_pDrawObject;
	//用下面的不能访问扩展属性
//	LPUNKNOWN lpUnknown = NULL;
//	m_pDrawObject->QueryInterface(IID_IUnknown, (void**)&lpUnknown);
	
//	return lpUnknown;
}

/////////////////////////////////////////////////////////////////////////
//功 能：创建或装载
//////////////////////////////////////////////////////////////////////////
BOOL CDrawObj::CreateOrLoad(REFCLSID clsid, LPCTSTR pszName, IStream* pStream)
{
	HRESULT hResult;
		
	ASSERT(m_pDocument != NULL);

	m_dwObjectNumber = GetNewObjectNumber();

	hResult = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER|
		CLSCTX_INPROC_HANDLER|CLSCTX_LOCAL_SERVER, IID_ICxDrawObject,
		(void**)&m_pDrawObject);
	if (FAILED(hResult))
	{
		ASSERT(FALSE);
		return hResult;
	}
	
	//设置站点
	hResult = m_pDrawObject->SetDrawObjectSite(&m_xDrawObjectSite);
	if (FAILED(hResult))
	{
		ASSERT(FALSE);
		TRACE("设置站点。\n");
	}

	//初始化和装载对象状态
	if (SUCCEEDED(hResult))
	{
		if (pStream == NULL)
		{
			m_pDrawObject->InitNew();
			m_pDocument->CreateUniqueObjectName(this);
			hResult = S_OK;
		}
		else
		{
			hResult = m_pDrawObject->Load(theAppKey, pStream);
		}
	}

	return FinishCreate(hResult);
}


BOOL CDrawObj::FinishCreate(HRESULT hr)
{
	//建立事件连接
	ICxDrawObjectAdviseSink* lpAdviseSink =
		(ICxDrawObjectAdviseSink *)GetInterface(&IID_ICxDrawObjectAdviseSink);
	ASSERT(lpAdviseSink != NULL);
	VERIFY(m_pDrawObject->SetAdvise(lpAdviseSink) == S_OK);

	return TRUE;
}

void CDrawObj::Release()
{
	if (m_pDrawObject != NULL)
	{	
		m_pDrawObject->Release();
		m_pDrawObject = NULL;
	}

	CDevObj::Release();

	CoFreeUnusedLibraries();
}

void CDrawObj::DrawFrontToBack(CDC* pDC)
{
	ASSERT(m_pDrawObject != NULL);
	m_pDrawObject->Draw(pDC->m_hDC);
}

void CDrawObj::Draw(CDC* pDC)
{
	ASSERT(m_pDrawObject != NULL);
	
	if (IsDrawVisible())
		m_pDrawObject->Draw(pDC->m_hDC);
}

void CDrawObj::Print(CDC* pDC)
{
	ASSERT(m_pDrawObject != NULL);
	
	if (IsDrawVisible())
		m_pDrawObject->Print(pDC->m_hDC);
}

void CDrawObj::OnChange()
{
	ASSERT_VALID(this);

	Invalidate(NULL);
//	GetDocument()->UpdateAllViews(NULL);
}

void CDrawObj::OnUngrouped()
{
	ASSERT(m_pDrawObject != NULL);

	m_pDrawObject->OnRunModeChange();
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

CRectF CDrawObj::GetPositionRect()
{
	ASSERT(m_pDrawObject != NULL);

	CRectF rect;
	rect.SetRectEmpty();
	m_pDrawObject->GetPositionRect(&rect);
	return rect;
}

CRect CDrawObj::GetBoundingRect()
{
	ASSERT(m_pDrawObject != NULL);

	CRect rect;
	rect.SetRectEmpty();
	m_pDrawObject->GetBoundingRect(&rect);
	return rect;
}

int CDrawObj::GetHandleCount(TrackerState state)
{
	if (!IsDrawVisible())	
		return 0;

	ASSERT(m_pDrawObject != NULL);

	long nCount = 0;
	m_pDrawObject->GetHandleCount(&nCount);

	state = GetTrackerState(state);
	switch (state)
	{
	case reshape:
		if (nCount < 0)
			nCount = 0;
		break;
	case resize:
		if (nCount >= 0)
			nCount = 8;
		else 
			nCount = -nCount;
		break;
	default:
		return CDevObj::GetHandleCount(state);
	}

	return (int)nCount;
}

CPoint CDrawObj::GetHandle(int nHandle, TrackerState state, CLayoutView* pView)
{
	ASSERT(m_pDrawObject != NULL);
	CPoint point;

	state = GetTrackerState(state);
	switch (state)
	{
	case reshape:
		m_pDrawObject->GetHandle(nHandle, &point);
		break;
	case resize:
		m_pDrawObject->GetHandle(-nHandle, &point);
		break;
	default:
		return CDevObj::GetHandle(nHandle, state, pView);
	}

	return point;
}	

HCURSOR CDrawObj::GetHandleCursor(int nHandle, TrackerState state)
{
	ASSERT(m_pDrawObject != NULL);
	HCURSOR hHandleCursor;
	
	state = GetTrackerState(state);
	switch (state)
	{
	case reshape:
		m_pDrawObject->GetHandleCursor(nHandle, &hHandleCursor);
		break;
	case resize:
		m_pDrawObject->GetHandleCursor(-nHandle, &hHandleCursor);
		break;
	default:
		return CDevObj::GetHandleCursor(nHandle, state);
	}

	return hHandleCursor;
}

BOOL CDrawObj::HitTest(CPoint point, CLayoutView* pView)
{
	ASSERT(m_pDrawObject != NULL);
	if (!IsDrawVisible())
		return FALSE;

	BOOL bHitTest;
	m_pDrawObject->HitTest(point, &bHitTest);
	return bHitTest;
}

BOOL CDrawObj::HaveDynamic()
{
	ASSERT(m_pDrawObject != NULL);
	
	return m_pDrawObject->HaveDynamic() == S_OK;
}

BOOL CDrawObj::Intersects(const CRect& rect)
{
	ASSERT(m_pDrawObject != NULL);
	if (!IsDrawVisible())
		return FALSE;

	BOOL bIntersects;
	m_pDrawObject->Intersects(rect, &bIntersects);
	return bIntersects;
}

void CDrawObj::MoveTo(const CRectF& rect, BOOL bUniformScale, CLayoutView* pView)
{
	ASSERT(m_pDrawObject != NULL);

	Invalidate(pView);
	m_pDrawObject->MoveTo(rect, bUniformScale);
	Invalidate(pView);
}

void CDrawObj::Rotate(int lAngle, CLayoutView* pView)
{
	ASSERT(m_pDrawObject != NULL);
	
	Invalidate(pView);
	m_pDrawObject->Rotate(lAngle);
	Invalidate(pView);
}

void CDrawObj::RotateAt(CPointF point, float fAngle, CLayoutView* pView )
{
	ASSERT(m_pDrawObject != NULL);

	m_pDrawObject->RotateAt(point, fAngle);
}

void CDrawObj::DrawRotateAtShape(CDC* pDC, CPointF point, long lAngle)
{
	ASSERT(m_pDrawObject != NULL);

	m_pDrawObject->DrawRotateAtShape(pDC->m_hDC, point, lAngle);
}

void CDrawObj::DrawTrackerShape(CDC* pDC)
{
	ASSERT(m_pDrawObject != NULL);
	
	m_pDrawObject->DrawTrackerShape(pDC->m_hDC, NULL, 0);
}

void CDrawObj::ScaleAt(CPointF point, float fHorScale, float fVerScale, CLayoutView* pView)
{
	ASSERT(m_pDrawObject != NULL);

	m_pDrawObject->ScaleAt(point, fHorScale, fVerScale);
}

CPoint CDrawObj::GetRotateBasePoint()
{
	ASSERT(m_pDrawObject != NULL);
	CPoint point;
	m_pDrawObject->GetRotateBasePoint(&point);

	return point;
}

void CDrawObj::SetRotateBasePoint(CPoint point) 
{
	ASSERT(m_pDrawObject != NULL);
	m_pDrawObject->SetRotateBasePoint(point);
}

void CDrawObj::MoveHandleTo(int nHandle, TrackerState state, CPoint point, CLayoutView* pView)
{
	ASSERT(m_pDrawObject != NULL);
	state = GetTrackerState(state);
	switch (state)
	{
	case reshape:
		m_pDrawObject->MoveHandleTo(nHandle, point);
		break;
	case resize:
		m_pDrawObject->MoveHandleTo(-nHandle, point);
		break;
	case rotate:
		{
			if (nHandle == 1)
			{
				SetRotateBasePoint(point);
				break;
			}
			CPoint ptBase = GetHandle(1, state, pView);
			CPoint ptOrign = GetHandle(nHandle, state, pView);
			m_pDrawObject->Rotate(CalculateAngle(ptBase, ptOrign) - CalculateAngle(ptBase, point));
		}
		break;
	default:
		ASSERT(FALSE);
	}
}

CDevObj::TrackerState CDrawObj::GetTrackerState(TrackerState state)
{
	long nCount = 0;
	m_pDrawObject->GetHandleCount(&nCount);

	if (state == general || state == reshape)
	{
		if (nCount > 0)
			return reshape;
		else
			return resize;
	}

	if (state == rotate && nCount == 0)
		return resize;

	return state;
}

void CDrawObj::DrawTrackerHandleShape(CDC* pDC, int nHandle, TrackerState state, CPoint point, CLayoutView* pView)
{
	ASSERT(m_pDrawObject != NULL);

	state = GetTrackerState(state);
	switch (state)
	{
	case reshape:
		m_pDrawObject->DrawTrackerHandleShape(pDC->m_hDC, nHandle, point);
		break;
	case resize:
		m_pDrawObject->DrawTrackerHandleShape(pDC->m_hDC, -nHandle, point);
		break;
	case rotate:
		{
			if (nHandle == 1)
			{
				CDevObj::DrawTrackerHandleShape(pDC, nHandle, state, point, pView);
				return;
			}
			CPoint ptBase = GetHandle(1, state, pView);
			CPoint ptOrign = GetHandle(nHandle, state, pView);
			m_pDrawObject->DrawRotateShape(pDC->m_hDC, CalculateAngle(ptBase, ptOrign) - CalculateAngle(ptBase, point));
			CDevObj::DrawTrackerHandleShape(pDC, nHandle, state, point, pView);
		}
		break;
	default:
		ASSERT(FALSE);
	}
}

BEGIN_INTERFACE_MAP(CDrawObj, CDevObj)
	INTERFACE_PART(CDrawObj, IID_ICxDrawObjectSite, DrawObjectSite)
	INTERFACE_PART(CDrawObj, IID_ICxDrawObjectAdviseSink, DrawObjectAdviseSink)
	INTERFACE_PART(CDrawObj, IID_IDispatch, AmbientProperties)
END_INTERFACE_MAP()

///////////////////////////////////////////////////////////////
STDMETHODIMP_( ULONG ) CDrawObj::XDrawObjectSite::AddRef()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	TRACE("XAmbientProperties::AddRef().\n");

	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CDrawObj::XDrawObjectSite::Release()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	TRACE("XAmbientProperties::Release().\n");

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
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetCapture()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::SetFocus(BOOL bFocus)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetFocus()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	return S_OK;
}
	
STDMETHODIMP CDrawObj::XDrawObjectSite::IsRunMode()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	
	return pThis->m_pGroupObj != NULL ? S_OK: S_FALSE;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetEventObject(ICxEventObj** ppEvent)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	*ppEvent = NULL;

	return S_FALSE;
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

	CString strName(bstrName); 
	pThis->PutDisplayName(strName);
		
	return S_OK;
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
}

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

STDMETHODIMP CDrawObj::XDrawObjectSite::OnMouseInput(const WCHAR* szPropName, BOOL bProces)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	return S_FALSE;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::SetTimer(UINT nElapse)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	return S_FALSE;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::TrackMouseLeave()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	return S_FALSE;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::TrackMouse(BOOL bTrack)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	return S_FALSE;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetExpressionCreatorProc(long lProc)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)

	CFormDoc::m_pExpCrtDoc = (CFormDoc *)pThis->m_pDocument;
	*(ExpressionCreatorProc *)lProc = CFormDoc::ExpressionCreatorProc;

	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetWindow(HWND* phWindow)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
	
	*phWindow = ((CFormDoc *)pThis->m_pDocument)->GetLayoutView()->GetSafeHwnd();
	
	return S_OK;   
}

STDMETHODIMP CDrawObj::XDrawObjectSite::InvalidateRect(LPCRECT prect, BOOL tErase)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
		
		CRect rect;
	
	if (prect == NULL)
		rect = pThis->GetBoundingRect();
	// Intersect the given rectangle with the bounds of the object.
	else
		rect.IntersectRect(prect, pThis->GetBoundingRect());
	
	
	if (!rect.IsRectEmpty())
	{
		((CFormDoc *)pThis->m_pDocument)->GetLayoutView()->InvalidateRect(&rect, tErase);
		//		pThis->GetActiveView()->UpdateWindow();
	}
	
	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::InvalidateRgn(HRGN hRgn, BOOL tErase)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite)
		
	CRgn rgn;
	CRgn* pRgn;
	CRect rectRgn;
	CRect rect;
	
	// The object wants its full extent invalidated.  It's much easier to do
	// this with a rect than a region.
	if (hRgn == NULL)
		return InvalidateRect(NULL, tErase);
	
	pRgn = CRgn::FromHandle(hRgn);
	pRgn->GetRgnBox(&rectRgn);
	rect.IntersectRect(&rectRgn, &pThis->GetBoundingRect());
	if (rect != rectRgn)
		return InvalidateRect(NULL, tErase);
	
	((CFormDoc *)pThis->m_pDocument)->GetLayoutView()->InvalidateRgn(pRgn, tErase);
	
	return S_OK;
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetDC(HDC* phDC)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite); 
	
	if (phDC == NULL)
		return E_INVALIDARG;
	
	if (pThis->m_pDC == NULL)
	{
		pThis->m_pDC = new CDC;
		((CFormDoc *)pThis->m_pDocument)->GetLayoutView()->PrepareRopDC(pThis->m_pDC);
	}
	ASSERT(pThis->m_pDC != NULL);
	
	*phDC = pThis->m_pDC->GetSafeHdc();
	
	return S_OK;   
}

STDMETHODIMP CDrawObj::XDrawObjectSite::ReleaseDC(HDC hDC)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite); 
	
	ASSERT(hDC == pThis->m_pDC->GetSafeHdc());
	
	((CFormDoc *)pThis->m_pDocument)->GetLayoutView()->ReleaseDC(pThis->m_pDC);
	delete pThis->m_pDC;
	pThis->m_pDC = NULL;
	
	return S_OK;   
}

STDMETHODIMP CDrawObj::XDrawObjectSite::GetDefaultServer(BSTR *pbstrComputer)
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectSite); 

	*pbstrComputer = NULL;
		
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
	
	((CFormDoc *)pThis->m_pDocument)->GetLayoutView()->DocToClient((CRect&)(*pRect));
	
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

	TRACE("XDrawObjectAdviseSink::AddRef().\n");

	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CDrawObj::XDrawObjectAdviseSink::Release()
{
	METHOD_PROLOGUE(CDrawObj, DrawObjectAdviseSink)

	TRACE("XDrawObjectAdviseSink::Release().\n");

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
/////////////////////////////////////////////////////////////////////////
//功 能：拷贝对象到剪贴板
//参 数：
/////////////////////////////////////////////////////////////////////////
void CDrawObj::CopyToClipboard(CArchive& ar, IStorage* pStorage)
{
	CLSID clsid;
	HRESULT hResult;
	
	ASSERT_VALID(this);
	ASSERT(ar.IsStoring());

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
	m_bNeedLoadOrSaveStorage = FALSE;
	hResult = m_pDrawObject->Save(pStream, TRUE);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	if (m_bNeedLoadOrSaveStorage)
	{
		USES_CONVERSION;
		TCHAR szObjectName[64];
		IStoragePtr pSubStorage;

		ar << m_dwObjectNumber;
		
		GetObjectName(szObjectName);
		
		//创建子存储
		hResult = pStorage->CreateStorage(T2COLE(szObjectName), STGM_READWRITE|
			STGM_CREATE|STGM_SHARE_EXCLUSIVE, 0, 0, &pSubStorage);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}
		
		hResult = m_pDrawObject->SaveStorage(pStorage, false);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}
	}

	CDevObj::CopyToClipboard(ar, pStorage);
}

/////////////////////////////////////////////////////////////////////////
//功 能：从剪贴板中创建对象
//参 数：
/////////////////////////////////////////////////////////////////////////
BOOL CDrawObj::CreateFromClipboard(CArchive& ar, IStorage* pStorage)
{
	HRESULT hResult;
	CLSID clsid;
	
	CArchiveStream stm(&ar);
	LPSTREAM pStream = &stm;

	hResult = ReadClassStm(pStream, &clsid);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	hResult = CreateOrLoad(clsid, NULL, pStream);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	if (m_bNeedLoadOrSaveStorage)
	{
		USES_CONVERSION;
		TCHAR szObjectName[64];
		IStoragePtr pSubStorage;

		DWORD dwObjectNumber = m_dwObjectNumber;
		ar >> m_dwObjectNumber;
		
		GetObjectName(szObjectName);
		m_dwObjectNumber = dwObjectNumber;
		
		//打开子存储
		hResult = pStorage->OpenStorage(T2COLE(szObjectName), NULL, STGM_READ|
			STGM_SHARE_EXCLUSIVE, NULL, 0, &pSubStorage);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}
		
		hResult = m_pDrawObject->LoadStorage(pStorage);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}
	}

	return CDevObj::CreateFromClipboard(ar, pStorage);
}

CString CDrawObj::GetClassName()
{
	BSTR bstrClassName;
  
	ASSERT(m_pDrawObject != NULL);
	m_pDrawObject->GetClassName(&bstrClassName);
	return CString(bstrClassName);
}

BOOL CDrawObj::GetPropertyPageCLSIDs(CArray<CLSID, CLSID&>& aclsidPages)
{
	CAUUID pages;
	HRESULT hResult;
	ULONG iPage;
//	CLSID clsid;

	if (m_pDrawObject != NULL )
	{
		pages.cElems = 0;
		pages.pElems = NULL;
		hResult = m_pDrawObject->GetPages( &pages );
		if (FAILED(hResult))
		{
			TRACE("ICxDrawObject::GetPages() failed\n");
			return FALSE;
		}

		for (iPage = 0; iPage < pages.cElems; iPage++)
		{
			aclsidPages.Add(pages.pElems[iPage]);
		}

		CoTaskMemFree(pages.pElems);
	}
  
	return TRUE;
}







