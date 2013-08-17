// CxGenItem.cpp : implementation of the CCtrlItem class
//

#include "stdafx.h"
#include "RunInc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCtrlItem implementation

IMPLEMENT_SERIAL(CCtrlItem, COleClientItem, 0)

CCtrlItem::CCtrlItem(CDevObjDoc* pContainer, CCtrlObj* pCtrlObj) :
	COleClientItem(pContainer),
	m_bWindowless(FALSE),
	m_pDC(NULL),
	m_bDisplayAsDefault(FALSE),
	m_dwAspectPass2(DVASPECT_CONTENT)
{
	m_pCtrlObj = pCtrlObj;
	m_pDispatch = NULL;
	m_nLeakRefCount = 0;
}

CCtrlItem::~CCtrlItem()
{
	// 释放泄漏的引用，2006.8.8 加入，解决控件对象不能正确释放的问题
	if (m_nLeakRefCount)
	{
		TRY
		{
			while (m_pDispatch->Release());
		}
		CATCH_ALL(e)
		{
			THROW_LAST();
		}
		END_CATCH_ALL
	}

	ASSERT(m_pDC == NULL);
}

void CCtrlItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	COleClientItem::OnChange(nCode, dwParam);

	// When an item is being edited (either in-place or fully open)
	//  it sends OnChange notifications for changes in the state of the
	//  item or visual appearance of its content.

	switch (nCode)
	{
	case OLE_CHANGED:
		m_pCtrlObj->Invalidate();
		UpdateExtent();
		break;

	case OLE_CHANGED_STATE:
	case OLE_CHANGED_ASPECT:
		m_pCtrlObj->Invalidate();
		break;
	}

//	GetDocument()->UpdateAllViews(NULL);
		// for now just update ALL views/no hints
}

////////////////////////////////////////////////////////////////////////////////////
//功 能：
////////////////////////////////////////////////////////////////////////////////////
BOOL CCtrlItem::OnChangeItemPosition(const CRect& rectPos)
{
	ASSERT_VALID(this);

	// During in-place activation CCtrlItem::OnChangeItemPosition
	//  is called by the server to change the position of the in-place
	//  window.  Usually, this is a result of the data in the server
	//  document changing such that the extent has changed or as a result
	//  of in-place resizing.
	//
	// The default here is to call the base class, which will call
	//  COleClientItem::SetItemRects to move the item
	//  to the new position.

	if (!COleClientItem::OnChangeItemPosition(rectPos))
	{
		return FALSE;
	}

	m_pCtrlObj->Invalidate();
	
	CRect rcTemp = rectPos;
	CRunView* pView = GetActiveView();
	ASSERT_VALID(pView);
	pView->ClientToDoc(rcTemp);
	m_pCtrlObj->m_position = rcTemp;

	m_pCtrlObj->Invalidate();

	return TRUE;
}

void CCtrlItem::OnGetItemPosition(CRect& rPosition)
{
	ASSERT_VALID(this);

	// update to extent of item if m_position is not initialized
//	if (m_pCtrlObj->m_position.IsRectEmpty())
//		UpdateExtent();

	// copy m_position, which is in document coordinates
	CRunView* pView = GetActiveView();
	ASSERT_VALID(pView);
	rPosition = m_pCtrlObj->m_position.GetRECT();

	//在位激活并且为窗口模式时变更坐标
	if (/*IsInPlaceActive() && */!m_bWindowless)
		pView->DocToClient(rPosition);
}

////////////////////////////////////////////////////////////////////////
//功 能：对象序列化
////////////////////////////////////////////////////////////////////////
void CCtrlItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	COleClientItem::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CCtrlItem diagnostics

#ifdef _DEBUG
void CCtrlItem::AssertValid() const
{
	COleClientItem::AssertValid();
}

void CCtrlItem::Dump(CDumpContext& dc) const
{
	COleClientItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
//功 能：创建或者导入控件项目
/////////////////////////////////////////////////////////////////////////////
BOOL CCtrlItem::CreateOrLoad(REFCLSID clsid, 
	REFIID iidPersistanceMedium, IUnknown* pPersistanceMedium)
{
	HRESULT hResult;
	BOOL bSuccess;
	IPersistStreamInitPtr pPersistStreamInit;
	IPersistStreamPtr pPersistStream;
	IPersistStoragePtr pPersistStorage;
	IStreamPtr pStream;
	IStoragePtr pStorage;

	ASSERT(m_lpObject == NULL);
	ASSERT(m_pDocument != NULL);

	m_dwItemNumber = GetNewItemNumber();
	GetItemStorage();
	ASSERT(m_lpStorage != NULL);

   	m_bExtendedControl = TRUE;
	hResult = CExtendedControl::CreateInstance(clsid, this, NULL,
		IID_IOleObject, (void**)&m_lpObject);
	if (FAILED(hResult))
	{
		m_bExtendedControl = FALSE;
		hResult = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER|
			CLSCTX_INPROC_HANDLER|CLSCTX_LOCAL_SERVER, IID_IOleObject,
			(void**)&m_lpObject);

		if (SUCCEEDED(hResult))
		{
			TRACE("Could not aggregate on the control, so it won't support extended properties.\n");
		}
	}

	if (m_bExtendedControl)
	{
		m_pExtendedControl = m_lpObject;
		ASSERT(m_pExtendedControl != NULL);
	}

	//设置客户站点
	if (SUCCEEDED(hResult))
	{
		m_lpObject->GetMiscStatus(DVASPECT_CONTENT, &m_dwMiscStatus);
		if (m_dwMiscStatus&OLEMISC_SETCLIENTSITEFIRST)
		{
			//GetClientSite 得到接口
			hResult = m_lpObject->SetClientSite(GetClientSite());
			if (FAILED(hResult))
			{
			   TRACE("SetClientSite failed.\n");
			}
		 }
	}

	//初始化或装载对象的状态
	if (SUCCEEDED(hResult))
	{
		if( iidPersistanceMedium == IID_NULL )
		{
			pPersistStreamInit = m_lpObject;
			if (pPersistStreamInit != NULL)
			{
				hResult = pPersistStreamInit->InitNew();
				if (hResult == E_NOTIMPL)
				{
					//没有实现 InitNew 为 Ok。
					hResult = S_OK;
				}
			}
			else
			{
				pPersistStorage = m_lpObject;
				if (pPersistStorage != NULL)
				{
					hResult = pPersistStorage->InitNew(m_lpStorage);
				}
				else
				{
					// 假设控件不支持持久性。
					hResult = S_OK;
				}
			}
		}
		else if (iidPersistanceMedium == IID_IStream)
		{
			pStream = pPersistanceMedium;
			ASSERT(pStream != NULL);

			pPersistStreamInit = m_lpObject;
			if (pPersistStreamInit != NULL)
			{
				hResult = pPersistStreamInit->Load(pStream);
			}
			else
			{
				pPersistStream = m_lpObject;
				if (pPersistStream != NULL)
				{
					hResult = pPersistStream->Load(pStream);
				}
				else
				{
					hResult = E_NOINTERFACE;
				}
			}
		}
		else if (iidPersistanceMedium == IID_IStorage)
		{
			pStorage = pPersistanceMedium;
			ASSERT(pStorage != NULL);

			pPersistStorage = m_lpObject;
			if (pPersistStorage != NULL)
			{
				hResult = pStorage->CopyTo(0, NULL, NULL, m_lpStorage);
				if (SUCCEEDED(hResult))
				{
					hResult = pPersistStorage->Load(m_lpStorage);
				}
			}
			else
			{
				hResult = E_NOINTERFACE;
			}
		}
	}

	if (SUCCEEDED(hResult))
	{
		if (!(m_dwMiscStatus&OLEMISC_SETCLIENTSITEFIRST))
		{
			hResult = m_lpObject->SetClientSite(GetClientSite());
			if (FAILED(hResult))
			{
				TRACE("SetClientSite failed.\n");
			}
		}
	}
	//SetClientSite 必须被使用

	bSuccess = FinishCreate(hResult);

	return bSuccess;
}

////////////////////////////////////////////////////////////////////////////
//功 能：完成创建
////////////////////////////////////////////////////////////////////////////
BOOL CCtrlItem::FinishCreate(HRESULT hr)
{
	BOOL bSuccess;
	HRESULT hResult;

	bSuccess = COleClientItem::FinishCreate(hr);
	if (!bSuccess)
	{
		return bSuccess;
	}

	hResult = m_lpObject->QueryInterface( IID_IOleControl,
		(void**)&m_pOleControl);
	if (FAILED(hResult))
	{
		// That's OK, it might not be a control
		TRACE("Could not get object's IOleControl interface\n");
	}

	hResult = m_lpObject->QueryInterface( IID_IPointerInactive,
		(void**)&m_pPointerInactive );
	if( SUCCEEDED(hResult))
	{
		TRACE("Control supports IPointerInactive.\n");
	}

	hResult = m_lpObject->QueryInterface(IID_IViewObjectEx,
		(void**)&m_pViewObjectEx);
	if (SUCCEEDED(hResult))
	{
		TRACE("Control supports IViewObjectEx.\n");
		m_pViewObjectEx->GetViewStatus(&m_dwViewStatus);
	}
	else
	{
		m_dwViewStatus = 0;
	}

	hResult = m_lpObject->QueryInterface(IID_IDispatch,
		(void**)&m_pDispatch);
	if (SUCCEEDED(hResult))
	{
		TRACE("Control supports IDispatch.\n");
	}

	return TRUE;
}

BOOL CCtrlItem::DrawFrontToBack(CDC* pDC, LPCRECT prectBounds)
{
	HRESULT hResult;
	RECTL rclBounds;
	RECTL* prclBounds;
	DWORD dwViewStatus;
	RECTL rclClip;
	CRect rectClip;
	CSize size;

	if ((m_lpObject == NULL) || (m_lpViewObject == NULL))
	{
		return FALSE;
	}

	//如果是在位激活且窗口模式退出
	if (IsInPlaceActive() && !m_bWindowless)
	{
		return FALSE;
	}

	//不支持两遍绘制
	if (m_pViewObjectEx == NULL)
	{
		// The control doesn't understand two-pass drawing, so just draw it on
		// pass 2.
		return FALSE ;
	}

	//不可见退出
	if(IsInvisibleAtRuntime() )
	{
		// We are invisible.
		return FALSE;
	}

	// convert RECT lpBounds to RECTL rclBounds
	rclBounds.left = prectBounds->left;
	rclBounds.top = prectBounds->top;
	rclBounds.right = prectBounds->right;
	rclBounds.bottom = prectBounds->bottom;

	if (m_bWindowless)
	{
		prclBounds = NULL;
	}
	else
	{
		prclBounds = &rclBounds;
	}

	// Assume that we won't be able to draw the first pass.
	m_dwAspectPass2 = DVASPECT_CONTENT;

	m_pViewObjectEx->GetViewStatus( &dwViewStatus );
	m_dwViewStatus = dwViewStatus;
	ASSERT( m_dwViewStatus == dwViewStatus );
	//不支持不透明性退出
	if (!(m_dwViewStatus&VIEWSTATUS_DVASPECTOPAQUE))
	{
		return FALSE;
	}

	//返回不透明区域
	hResult = m_pViewObjectEx->GetRect(DVASPECT_OPAQUE, &rclClip);
	if (FAILED(hResult))
	{
		return FALSE;
	}

	hResult = m_lpViewObject->Draw(DVASPECT_OPAQUE, -1, NULL, NULL, NULL,
		pDC->m_hDC, prclBounds, NULL, NULL, 0);
	if (SUCCEEDED(hResult))
	{
		// Just draw the transparent part in pass 2.
		m_dwAspectPass2 = DVASPECT_TRANSPARENT;
	}

	size = CSize(rclClip.left, rclClip.top);
	pDC->HIMETRICtoDP(&size);
	rectClip.left = int(m_pCtrlObj->m_position.left+size.cx);
	rectClip.top = int(m_pCtrlObj->m_position.top+size.cy);
	size = CSize(rclClip.right, rclClip.bottom);
	pDC->HIMETRICtoDP(&size);
	rectClip.right = int(m_pCtrlObj->m_position.left+size.cx);
	rectClip.bottom = int(m_pCtrlObj->m_position.top+size.cy);
	//将此区域切掉
	pDC->ExcludeClipRect(&rectClip);

	return TRUE;
}

BOOL CCtrlItem::DrawBackToFront(CDC* pDC, LPCRECT prectBounds)
{
	RECTL rclBounds;
	RECTL* prclBounds;
	DWORD dwViewStatus;

	if (m_lpObject == NULL || m_lpViewObject == NULL)
	{
		return FALSE;
	}

	if (IsInvisibleAtRuntime())
	{
		// We are invisible.
		return FALSE;
	}

    if (IsInPlaceActive() && !m_bWindowless)
    {
 		return FALSE;
    }

	// convert RECT lpBounds to RECTL rclBounds
	rclBounds.left = prectBounds->left;
	rclBounds.top = prectBounds->top;
	rclBounds.right = prectBounds->right;
	rclBounds.bottom = prectBounds->bottom;

	if (m_bWindowless)
	{
		prclBounds = NULL;
	}
	else
	{
		prclBounds = &rclBounds;
	}

	if (m_pViewObjectEx != NULL)
	{
		m_pViewObjectEx->GetViewStatus(&dwViewStatus);
		ASSERT(dwViewStatus == m_dwViewStatus);
	}
	else
	{
		ASSERT(m_dwViewStatus == 0);
	}

	if ((m_dwAspectPass2 == DVASPECT_TRANSPARENT)&&(m_dwViewStatus&
		VIEWSTATUS_DVASPECTTRANSPARENT))
	{
		m_lpViewObject->Draw( DVASPECT_TRANSPARENT, -1, NULL, NULL, NULL,
			pDC->m_hDC, prclBounds, NULL, NULL, 0 );
	}
	else if (m_dwAspectPass2 == DVASPECT_CONTENT)
	{
		m_lpViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, pDC->m_hDC,
			prclBounds, NULL, NULL, 0);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//功 能：绘制项目
//////////////////////////////////////////////////////////////////////////
BOOL CCtrlItem::Draw(CDC* pDC, LPCRECT prectBounds,
	DVASPECT nDrawAspect)
{
	RECTL rclBounds;
	RECTL* prclBounds;
	CRect rectClip;
	CSize size;

	if (m_lpObject == NULL || m_lpViewObject == NULL)
	{
		return FALSE;
	}

	//项目为窗体控件，返回
	if (IsInPlaceActive() && !m_bWindowless)
    {
 		return FALSE;
    }

	// convert RECT lpBounds to RECTL rclBounds
	rclBounds.left = prectBounds->left;
	rclBounds.top = prectBounds->top;
	rclBounds.right = prectBounds->right;
	rclBounds.bottom = prectBounds->bottom;

	if (m_bWindowless)
	{
		prclBounds = NULL;
	}
	else
	{
		prclBounds = &rclBounds;
	}

	m_lpViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, pDC->m_hDC,
		prclBounds, NULL, NULL, 0);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////
//功 能：运行时是否可见？
///////////////////////////////////////////////////////////////////////
BOOL CCtrlItem::IsInvisibleAtRuntime() const
{
	return (m_dwMiscStatus & OLEMISC_INVISIBLEATRUNTIME);
}

///////////////////////////////////////////////////////////////////////
//功 能：释放项目使用的资源
//描 述：必须在此释放项目使用的资源，否则程序无法完全退出
///////////////////////////////////////////////////////////////////////
void CCtrlItem::Release(OLECLOSE dwCloseOption)
{
	UINT eState;

	//如果处于活动状态，先进行关闭
	eState = GetItemState();
	if (eState == activeUIState || 
		eState == activeState || 
		eState == openState)
	{
		Close(OLECLOSE_NOSAVE);
	}

	if( m_pPointerInactive != NULL )
	{
		m_pPointerInactive.Release();
	}

    if (m_pOleInPlaceObjectWindowless != NULL)
	{
		m_pOleInPlaceObjectWindowless.Release();
	}

	//此处是5
	if (m_pViewObjectEx != NULL)
    {
		m_pViewObjectEx.Release();
    }

//	if (m_pDispatch != NULL)
//    {
//		m_pDispatch->Release();
//    }
	   
	if (m_pOleControl != NULL)
	{
		m_pOleControl.Release();
	}

	if (m_pExtendedControl != NULL)
	{
		m_pExtendedControl.Release();
	}

	COleClientItem::Release(dwCloseOption);

	if (m_pDispatch != NULL)
    {
		m_nLeakRefCount = m_pDispatch->Release();
    }

	CoFreeUnusedLibraries();
}

//////////////////////////////////////////////////////////////////////////////////
//功 能：是否用户界面激活
//////////////////////////////////////////////////////////////////////////////////
BOOL CCtrlItem::IsUIActive() const
{
	return (GetItemState() == activeUIState);
}

//////////////////////////////////////////////////////////////////////////////////
//功 能：
//////////////////////////////////////////////////////////////////////////////////
BOOL CCtrlItem::UpdateExtent()
{
	if (m_lpViewObject == NULL)
		return TRUE;

	CSize size;

	//下行调用m_lpViewObject，此时m_lpViewObject对部分控件可能为空
	if (GetExtent(&size))
	{
		// OLE returns the extent in HIMETRIC units -- we need pixels.
		CClientDC dc(NULL);
		dc.HIMETRICtoDP(&size);

		CRunView* pView = GetActiveView();
		if (pView == NULL)
			pView = (CRunView *)((CFormDoc*)m_pCtrlObj->m_pDocument)->GetTopFormDoc()->GetRunView();
		if (pView != NULL)
			pView->ClientToDoc(size);
		
		// Only invalidate if it has actually changed and also only if it is not
		// in-place active.
		CSize szTmp;
		szTmp.cx = int(m_pCtrlObj->m_position.Size().cx);
		szTmp.cy = int(m_pCtrlObj->m_position.Size().cy);
		if ((size != szTmp) /*&& !IsInPlaceActive()*/)
		{
			// Invalidate old, update, invalidate new.
			m_pCtrlObj->Invalidate();
			m_pCtrlObj->m_position.bottom = m_pCtrlObj->m_position.top + size.cy;
			m_pCtrlObj->m_position.right = m_pCtrlObj->m_position.left + size.cx;
			m_pCtrlObj->Invalidate();
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//功 能：获取外围裁剪矩形
//////////////////////////////////////////////////////////////////////////////////////////
void CCtrlItem::OnGetClipRect(CRect& rectClip)
{
	ASSERT_VALID(this);

   	CRunView* pView = GetActiveView();
	CRect rectTemp = m_pCtrlObj->m_position.GetRECT();
	//在位激活并且为窗口模式时变更坐标
	if (/*IsInPlaceActive() && */!m_bWindowless)
		pView->DocToClient(rectTemp);

	rectClip = rectTemp;
}

//////////////////////////////////////////////////////////////////////////////////////////
//功 能：点中测试
//////////////////////////////////////////////////////////////////////////////////////////
DWORD CCtrlItem::HitTest(CPoint point)
{
	HRESULT hResult;
	IViewObjectExPtr pViewObjectEx;
	DWORD dwHitResult;
	CRect rcItem;

	rcItem = m_pCtrlObj->m_position.GetRECT();
	if (!rcItem.PtInRect(point))
	{
		return HITRESULT_OUTSIDE;
	}
	
	hResult = m_lpObject->QueryInterface(IID_IViewObjectEx,
		(void**)&pViewObjectEx);
	if (SUCCEEDED(hResult))
	{
		hResult = pViewObjectEx->QueryHitPoint(DVASPECT_CONTENT, &rcItem, point,
			0, &dwHitResult);
		if (SUCCEEDED(hResult))
		{
			return dwHitResult;
		}
	}

	return HITRESULT_HIT;
}

//////////////////////////////////////////////////////////////////
//功 能：控件的焦点设置
//描 述：直接有视图处理
//////////////////////////////////////////////////////////////////
void CCtrlItem::OnFocus(BOOL bGotFocus)
{
	CRunView* pView = GetActiveView();
	if (pView != NULL)
		pView->OnFocus(m_pCtrlObj, bGotFocus);
}

BOOL CCtrlItem::TranslateAccelerator(LPMSG pMessage, DWORD dwModifiers)
{
	CRunView* pView = GetActiveView();
	if (pView != NULL)
		return pView->TranslateAccelerator(this, pMessage, dwModifiers);

	return FALSE;
}

BEGIN_INTERFACE_MAP(CCtrlItem, COleClientItem)
	INTERFACE_PART(CCtrlItem, IID_IDispatch, AmbientProperties)
	INTERFACE_PART(CCtrlItem, IID_IAxWinAmbientDispatch, AmbientProperties)
	INTERFACE_PART(CCtrlItem, IID_IOleControlSite, OleControlSite)
	INTERFACE_PART(CCtrlItem, IID_IOleInPlaceSiteEx, OleInPlaceSiteWindowless)
	INTERFACE_PART(CCtrlItem, IID_IOleInPlaceSiteWindowless, OleInPlaceSiteWindowless)
END_INTERFACE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CCtrlItem::XOleControlSite
///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) CCtrlItem::XOleControlSite::AddRef()
{
	METHOD_PROLOGUE(CCtrlItem, OleControlSite)

    return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CCtrlItem::XOleControlSite::Release()
{
	METHOD_PROLOGUE(CCtrlItem, OleControlSite)

    return pThis->ExternalRelease();
}

STDMETHODIMP CCtrlItem::XOleControlSite::QueryInterface(REFIID iid,
	LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CCtrlItem, OleControlSite)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CCtrlItem::XOleControlSite::GetExtendedControl(
	IDispatch** ppExtendedControl )
{
	METHOD_PROLOGUE(CCtrlItem, OleControlSite)

	if (ppExtendedControl == NULL)
	{
		return E_POINTER;
	}

	*ppExtendedControl = pThis->m_pExtendedControl;
	if (*ppExtendedControl == NULL)
	{
		return E_NOTIMPL;
	}
	else
	{
		(*ppExtendedControl)->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CCtrlItem::XOleControlSite::LockInPlaceActive(
	BOOL tLock )
{
	(void)tLock;

	METHOD_PROLOGUE(CCtrlItem, OleControlSite)

	return E_NOTIMPL;
}

STDMETHODIMP CCtrlItem::XOleControlSite::OnControlInfoChanged()
{
	METHOD_PROLOGUE(CCtrlItem, OleControlSite)

	return E_NOTIMPL;
}

STDMETHODIMP CCtrlItem::XOleControlSite::OnFocus(BOOL bGotFocus)
{
	METHOD_PROLOGUE(CCtrlItem, OleControlSite)

	pThis->OnFocus(bGotFocus);

	return S_OK;
}

STDMETHODIMP CCtrlItem::XOleControlSite::ShowPropertyFrame()
{
	return E_NOTIMPL;
}

STDMETHODIMP CCtrlItem::XOleControlSite::TransformCoords(
   POINTL* pptHiMetric, POINTF* pptContainer, DWORD dwFlags )
{
   (void)pptHiMetric;
   (void)pptContainer;
   (void)dwFlags;

   METHOD_PROLOGUE(CCtrlItem, OleControlSite)

   return E_NOTIMPL;
}

STDMETHODIMP CCtrlItem::XOleControlSite::TranslateAccelerator(
	LPMSG pMessage, DWORD dwModifiers)
{
	(void)pMessage;
	(void)dwModifiers;

	METHOD_PROLOGUE(CCtrlItem, OleControlSite)

	return pThis->TranslateAccelerator(pMessage, dwModifiers) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//CCtrlItem::XOleInPlaceSiteWindowless
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(ULONG) CCtrlItem::XOleInPlaceSiteWindowless::AddRef()
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless)

	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CCtrlItem::XOleInPlaceSiteWindowless::Release()
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless)

	return pThis->ExternalRelease();
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::
	ContextSensitiveHelp(BOOL tEnterMode)
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	return pThis->m_xOleIPSite.ContextSensitiveHelp(tEnterMode);
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::GetWindow(
	HWND* phWnd)
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	return pThis->m_xOleIPSite.GetWindow(phWnd);
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::
	CanInPlaceActivate()
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	return pThis->m_xOleIPSite.CanInPlaceActivate();
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::
	DeactivateAndUndo()
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	return pThis->m_xOleIPSite.DeactivateAndUndo();
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::
	DiscardUndoState()
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	return pThis->m_xOleIPSite.DiscardUndoState();
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::GetWindowContext(
	IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT prectPos,
	LPRECT prectClip, LPOLEINPLACEFRAMEINFO pFrameInfo)
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	return pThis->m_xOleIPSite.GetWindowContext(ppFrame, ppDoc, prectPos,
		prectClip, pFrameInfo);
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::
	OnInPlaceActivate()
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	return pThis->m_xOleIPSite.OnInPlaceActivate();
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::
	OnInPlaceDeactivate()
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	return pThis->m_xOleIPSite.OnInPlaceDeactivate();
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::OnPosRectChange(
	LPCRECT prectPos)
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	return pThis->m_xOleIPSite.OnPosRectChange(prectPos);
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::OnUIActivate()
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	return pThis->m_xOleIPSite.OnUIActivate();
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::OnUIDeactivate(
	BOOL tUndoable )
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	return pThis->m_xOleIPSite.OnUIDeactivate(tUndoable);
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::Scroll(
	SIZE sizeExtent)
{
	METHOD_PROLOGUE( CCtrlItem, OleInPlaceSiteWindowless );

	return pThis->m_xOleIPSite.Scroll(sizeExtent);
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::
	OnInPlaceActivateEx( BOOL* ptNoRedraw, DWORD dwFlags )
{
	HRESULT hResult;

	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	ASSERT(ptNoRedraw != NULL);

	ASSERT(pThis->m_bWindowless == FALSE);

	if (dwFlags&ACTIVATE_WINDOWLESS)
	{
		pThis->m_bWindowless = TRUE;
		hResult = pThis->m_lpObject->QueryInterface(
			IID_IOleInPlaceObjectWindowless,
			(void**)&pThis->m_pOleInPlaceObjectWindowless);
		ASSERT(SUCCEEDED(hResult));
	}

	hResult = pThis->m_xOleIPSite.OnInPlaceActivate();
	if (FAILED(hResult))
	{
		return hResult;
	}

	if (ptNoRedraw != NULL)
	{
		if (pThis->GetActiveView()->GetUpdateRect(NULL))
		{
			*ptNoRedraw = FALSE;
		}
		else
		{
			*ptNoRedraw = TRUE;
		}
	}

	return S_OK;
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::
	OnInPlaceDeactivateEx(BOOL tNoRedraw)
{
	(void)tNoRedraw;

	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	pThis->OnDeactivate(tNoRedraw);

	return S_OK ;
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::
	RequestUIActivate()
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	return S_OK;
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::AdjustRect(
	LPRECT prect )
{
	(void)prect;

	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	return E_NOTIMPL;
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::
	CanWindowlessActivate()
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

//#ifdef _DEBUG

	IPersistStoragePtr pPersistStorage;
	const CLSID CLSID_ApexGisCtrl = {0x10FAA75E,0xCA52,0x4B4F,{0xA0,0x7B,0x81,0x6F,0x47,0x38,0xA2,0xC0}};

	pPersistStorage = pThis->m_lpObject;
	if (pPersistStorage != NULL)
	{
		CLSID clsid;
		pPersistStorage->GetClassID(&clsid);

		if (clsid == CLSID_ApexGisCtrl)
			return S_OK;
	}

//#endif

	return S_FALSE;
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::GetCapture()
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	if (pThis->GetActiveView()->GetObjectWithCapture() == pThis->m_pCtrlObj)
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::GetDC(
	LPCRECT prect, DWORD dwFlags, HDC* phDC )
{
	CView* pView;
	CRgn rgn;
	CRect rect;

	(void)dwFlags;

	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	ASSERT(phDC != NULL);
	*phDC = NULL;

	if (pThis->m_pDC != NULL)
	{
	   return E_FAIL;
	}

	pView = pThis->GetActiveView();
	ASSERT(pView != NULL);

	pThis->m_pDC = pView->GetDC();
	if (pThis->m_pDC == NULL)
	{
	   return E_FAIL;
	}

	if (prect == NULL)
	{
		// The clipping rectangle is the object's extent rectangle.
		rect = pThis->m_pCtrlObj->m_position.GetRECT();
	}
	else
	{
		// The clipping rectangle is the object's extent rectangle intersected
		// with the rectangle requested by the object.
		rect.IntersectRect(&pThis->m_pCtrlObj->m_position.GetRECT(), prect);
	}

	rgn.CreateRectRgnIndirect(&rect);
	pThis->m_pDC->SelectClipRgn(&rgn, RGN_AND);

	*phDC = pThis->m_pDC->GetSafeHdc();

	return S_OK;
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::GetFocus()
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	if (pThis->GetActiveView()->GetObjectWithFocus() == pThis->m_pCtrlObj)
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::InvalidateRect(
	LPCRECT prect, BOOL tErase)
{
	CRect rect;

	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	if (prect == NULL)
	{
		rect = pThis->m_pCtrlObj->m_position.GetRECT();
	}
	else
	{
		// Intersect the given rectangle with the bounds of the object.
		rect.IntersectRect(prect, &pThis->m_pCtrlObj->m_position.GetRECT());
	}

	if (!rect.IsRectEmpty())
	{
		pThis->GetActiveView()->InvalidateRect(&rect, tErase);
//		pThis->GetActiveView()->UpdateWindow();
	}

	return S_OK;
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::InvalidateRgn(
    HRGN hRgn, BOOL tErase)
{
	CRgn rgn;
	CRgn* pRgn;
	CRect rectRgn;
	CRect rect;

	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	if (hRgn == NULL)
	{
		// The object wants its full extent invalidated.  It's much easier to do
		// this with a rect than a region.
		return InvalidateRect(NULL, tErase);
	}

	pRgn = CRgn::FromHandle(hRgn);
	pRgn->GetRgnBox(&rectRgn);
	rect.IntersectRect(&rectRgn, &pThis->m_pCtrlObj->m_position.GetRECT());
	if (rect != rectRgn)
	{
		return InvalidateRect(NULL, tErase);
	}

	pThis->GetActiveView()->InvalidateRgn(pRgn, tErase);

	return S_OK;
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::
	OnDefWindowMessage(UINT nMessage, WPARAM wParam, LPARAM lParam,
	LRESULT* plResult)
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);
	
    return pThis->GetActiveView()->OnWindowlessDefWindowMessage(nMessage,
	  wParam, lParam, plResult);
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::ReleaseDC(
	HDC hDC )
{
	CView* pView;

	UNUSED(hDC);

	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	pView = pThis->GetActiveView();
	ASSERT(pView != NULL);

	ASSERT(hDC == pThis->m_pDC->GetSafeHdc());

	pView->ReleaseDC(pThis->m_pDC);

	pThis->m_pDC = NULL;

	return S_OK;
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::ScrollRect(
	int dx, int dy, LPCRECT prectScroll, LPCRECT prectClip )
{
	(void)dx;
	(void)dy;
	(void)prectScroll;
	(void)prectClip;

	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	return E_NOTIMPL;
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::SetCapture(
	BOOL bCapture)
{
	HRESULT hResult;
	
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	if (bCapture)
	{
		hResult = pThis->GetActiveView()->OnWindowlessSetCapture(pThis->m_pCtrlObj);
		if (FAILED(hResult))
		{
			return hResult;
		}
	}
	else
	{
		hResult = pThis->GetActiveView()->OnWindowlessReleaseCapture(pThis->m_pCtrlObj);
		if (FAILED(hResult))
		{
			return hResult;
		}
	}

	return hResult;
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::SetFocus(
	BOOL bFocus )
{
	HRESULT hResult;

	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

	if (!bFocus)
	{
		hResult = pThis->GetActiveView()->OnWindowlessReleaseFocus();
		return S_OK;
	}
	else if (!pThis->IsWindowless())
	{
		pThis->GetActiveView()->SetFocus();
		return S_OK;
	}
	else 
	{
		hResult = pThis->GetActiveView()->OnWindowlessSetFocus(pThis->m_pCtrlObj);
		if (FAILED(hResult))
		{
			return hResult;
		}
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// CCtrlItem::XAmbientProperties
///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(ULONG) CCtrlItem::XAmbientProperties::AddRef()
{
	METHOD_PROLOGUE(CCtrlItem, AmbientProperties)

	TRACE("XAmbientProperties::AddRef().\n");

	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CCtrlItem::XAmbientProperties::Release()
{
	METHOD_PROLOGUE(CCtrlItem, AmbientProperties)

//	TRACE("XAmbientProperties::Release().\n");

	return pThis->ExternalRelease();
}

STDMETHODIMP CCtrlItem::XAmbientProperties::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CCtrlItem, AmbientProperties)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CCtrlItem::XAmbientProperties::GetIDsOfNames(
	REFIID iid, LPOLESTR* ppszNames, UINT nNames, LCID lcid, DISPID* pDispIDs)
{
	CFormDoc* pDoc;
	CAmbientProperty* pProp;
	UINT iName;
	HRESULT hResult;

	(void)lcid;

	METHOD_PROLOGUE(CCtrlItem, AmbientProperties)
	ASSERT_VALID(pThis);

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
	pDoc = pThis->GetDocument();
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

	return hResult;
}

STDMETHODIMP CCtrlItem::XAmbientProperties::GetTypeInfo(
	UINT iTypeInfo, LCID lcid, ITypeInfo** ppTypeInfo)
{
	(void)iTypeInfo;
	(void)lcid;
	(void)ppTypeInfo;

	METHOD_PROLOGUE(CCtrlItem, AmbientProperties)
	ASSERT_VALID(pThis);

	return E_NOTIMPL;
}

STDMETHODIMP CCtrlItem::XAmbientProperties::GetTypeInfoCount(
	UINT* pnInfoCount)
{
	METHOD_PROLOGUE(CCtrlItem, AmbientProperties)
	ASSERT_VALID(pThis);

	*pnInfoCount = 0;

	return S_OK;
}

STDMETHODIMP CCtrlItem::XAmbientProperties::Invoke(
	DISPID dispidMember, REFIID iid, LCID lcid, WORD wFlags,
	DISPPARAMS* pdpParams, VARIANT* pvarResult, EXCEPINFO* pExceptionInfo,
	UINT* piArgErr)
{

	CFormDoc* pDoc;
	CAmbientProperty* pProp;
	HRESULT hResult;
	COleVariant varValue;

	(void)iid;
	(void)lcid;
	(void)wFlags;
	(void)pdpParams;
	(void)pExceptionInfo;
	(void)piArgErr;

	METHOD_PROLOGUE(CCtrlItem, AmbientProperties)
	ASSERT_VALID(pThis);

	if (dispidMember == DISPID_AMBIENT_DISPLAYASDEFAULT)
	{
		VariantClear(pvarResult);
		pvarResult->vt = VT_BOOL;
		if (pThis->m_bDisplayAsDefault)
		{
			pvarResult->boolVal = VAR_TRUE;
		}
		else
		{
			pvarResult->boolVal = VAR_FALSE;
		}

		return S_OK;
	}

	pDoc = pThis->GetDocument();

	pProp = pDoc->FindAmbientProperty(dispidMember);
	if (pProp == NULL)
	{
		TRACE("Control asked for unknown ambient property.  DISPID=%d\n",
			dispidMember);
		return DISP_E_MEMBERNOTFOUND;
	}

	if (!pProp->IsEnabled())
	{
//		TRACE("Control asked for ambient property %s, but the property was disabled.\n",
//			LPCTSTR( pProp->GetName()));
		return DISP_E_MEMBERNOTFOUND;
	}

	TRACE("Control got ambient property %s.\n", LPCTSTR( pProp->GetName()));

	varValue = pProp->GetValue();
	hResult = VariantCopy(pvarResult, &varValue);
	if (FAILED(hResult))
	{
		return hResult;
	}

	return S_OK;
}

////////////////////////////////////////////////////////////////////////////////
//功 能：读入项目
////////////////////////////////////////////////////////////////////////////////
void CCtrlItem::ReadItem(CArchive& ar)
{
	USES_CONVERSION;
	BOOL tUsedStream;
	IStoragePtr pStorage;
	IPersistStreamInitPtr pPSI;
	IStreamPtr pStream;
	TCHAR szItemName[64];
	HRESULT hResult;
	BOOL tQuickActivated;
	IPersistStoragePtr pPersistStorage;
	CLSID clsid;

	(void)ar;

	CFormDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	ASSERT(pDoc->m_lpRootStg != NULL);
	ASSERT(pDoc->m_bCompoundFile);
	ASSERT(m_lpStorage == NULL);
	ASSERT(m_lpLockBytes == NULL);
	ASSERT(!ar.m_bForceFlat);

	// 得到项目名称
	GetItemName(szItemName);

	// 打开此项目存储
	hResult = pDoc->m_lpRootStg->OpenStorage(T2COLE(szItemName), NULL,
		STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE, 0, 0, &pStorage);
	if (hResult != S_OK)
	{
		TRACE1("Warning: unable to open child storage %s.\n", szItemName);
		// upon failure throw file exception (item will be cleaned up)
		AfxThrowOleException( hResult );
	}
	ASSERT(pStorage != NULL);

	// remember the storage
	m_lpStorage = pStorage;
	ASSERT(m_lpStorage != NULL);
	m_lpStorage->AddRef();

	// attempt to load the object from the storage
	hResult = ReadClassStg(m_lpStorage, &clsid);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	tUsedStream = FALSE;
	if (clsid == CLSID_NULL)
	{
		hResult = m_lpStorage->OpenStream(L"Contents", NULL, STGM_READ|
			STGM_SHARE_EXCLUSIVE, 0, &pStream);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}

		hResult = ReadClassStm(pStream, &clsid);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}

		tUsedStream = TRUE;
	}

	m_bExtendedControl = TRUE;
	hResult = CExtendedControl::CreateInstance(clsid, this, NULL,
		IID_IOleObject, (void**)&m_lpObject);
	if (FAILED(hResult))
	{
		m_bExtendedControl = FALSE;
		// The control may not like being aggregated on, so just try to create it
		// directly and punt the extended control stuff.
		hResult = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER|
			CLSCTX_INPROC_HANDLER|CLSCTX_LOCAL_SERVER, IID_IOleObject,
			(void**)&m_lpObject);
		if (SUCCEEDED(hResult))
		{
			TRACE("Could not aggregate on the control, so it won't support extended properties.\n");
		}
	}
	
	if (m_bExtendedControl)
	{
		m_pExtendedControl = m_lpObject;
		ASSERT(m_pExtendedControl != NULL);
	}

	tQuickActivated = FALSE;
	if (SUCCEEDED(hResult))
	{
//		tQuickActivated = QuickActivate();

		if( !tQuickActivated )
		{
			m_lpObject->GetMiscStatus(DVASPECT_CONTENT, &m_dwMiscStatus);
			if (m_dwMiscStatus&OLEMISC_SETCLIENTSITEFIRST)
			{
				hResult = m_lpObject->SetClientSite(GetClientSite());
				if (FAILED(hResult))
				{
					TRACE("SetClientSite failed.\n");
				}
			}
		}
	}

	if (SUCCEEDED(hResult))
	{
		pPersistStorage = m_lpObject;
		if (pPersistStorage != NULL)
		{
			hResult = pPersistStorage->Load(m_lpStorage);
		}
		else
		{
			if (!tUsedStream)
			{
				hResult = E_NOINTERFACE;
			}
			else
			{
				pPSI = m_lpObject;
				if (pPSI == NULL)
				{
					hResult = E_NOINTERFACE;
				}
				else
				{
					hResult = pPSI->Load(pStream);
				}
			}
		}
	}

	if (SUCCEEDED(hResult))
	{
		if (!tQuickActivated)
		{
			if( !(m_dwMiscStatus&OLEMISC_SETCLIENTSITEFIRST) )
			{
				hResult = m_lpObject->SetClientSite( GetClientSite() );
				if( FAILED( hResult ) )
				{
					TRACE("SetClientSite failed.\n");
				}
			}
		}
	}

	CheckGeneral(hResult);
}

IUnknown* CCtrlItem::GetInterfaceHook( const void* pv )
{
	const IID* piid;

#ifdef _DEBUG
	// The standard MFC implementation of QueryInterface doesn't switch module
	// states, since most objects just use static data to implement QI.  Since
	// we're calling GetDocument(), though, we wind up calling ASSERT_VALID on
	// a bunch of MFC objects, and that requires the correct module state.
	// Because ASSERT_VALID doesn't do anything in release builds, we only need
	// to switch module states in debug builds.
	AFX_MANAGE_STATE(m_pModuleState);
#endif  // _DEBUG

	piid = (const IID*)pv;

	if (*piid == IID_IOleInPlaceSiteEx)
	{
		return &m_xOleInPlaceSiteWindowless;
	}
	else if (*piid == IID_IOleInPlaceSiteWindowless)
	{
		return &m_xOleInPlaceSiteWindowless;
	}

	return NULL;
}

HWND CCtrlItem::GetInPlaceObjectWindow()
{
	HWND hWnd = NULL;
	LPOLEINPLACEOBJECT lpInPlaceObject = NULL;

	m_lpObject->QueryInterface(IID_IOleInPlaceObject, (void **)&lpInPlaceObject);
	if (lpInPlaceObject != NULL)
	{
		if (lpInPlaceObject->GetWindow(&hWnd) != S_OK)
			hWnd = NULL;
		lpInPlaceObject->Release();
	}

	return hWnd;
}

void CCtrlItem::BringWindowTop()
{
	HWND hWnd = GetInPlaceObjectWindow();
	if (hWnd != NULL)
		::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}

void CCtrlItem::SetFocus()
{
	HWND hWnd = GetInPlaceObjectWindow();
	if (hWnd != NULL)
		::SetFocus(hWnd);
}

/////////////////////////////////////////////////////////////
//功 能：窗口项目进入非活动状态，
//描 述：必须调用基类实现
/////////////////////////////////////////////////////////////
void CCtrlItem::OnDeactivate(BOOL bNoRedraw)
{
	if (m_bWindowless)
	{
		if (GetActiveView()->GetObjectWithFocus() == m_pCtrlObj)
		{
			GetActiveView()->OnWindowlessReleaseFocus();
		}
		m_bWindowless = FALSE;
		m_pOleInPlaceObjectWindowless.Release();
	}

	if (!bNoRedraw)
	{
		m_pCtrlObj->Invalidate();
	}

	COleClientItem::OnDeactivate();
}

void CCtrlItem::OnDeactivate()
{
	OnDeactivate(FALSE);
}

void CCtrlItem::OnActivateUI()
{
	CCtrlItem* pItem;
	CFormDoc* pDoc;
	POSITION posItem;

	// Allow only one UIActive item per frame.
	pDoc = GetDocument();
	posItem = pDoc->GetStartPosition();
	while (posItem != NULL)
	{
		pItem = (CCtrlItem*)pDoc->GetNextItem( posItem );
		if (pItem->IsUIActive())
		{
			ASSERT(pItem != this);
			pItem->DeactivateUI();
		}
	}

   COleClientItem::OnActivateUI();
}

////////////////////////////////////////////////////////////////////////////
//功 能：用户界面关闭
//描 述：一些种类控件必须在此关闭
////////////////////////////////////////////////////////////////////////////
void CCtrlItem::OnDeactivateUI(BOOL bUndoable)
{
	COleClientItem::OnDeactivateUI(bUndoable);

    // Also InPlaceDeactivate the object if it doesn't have the
	// ACTIVATEWHENVISIBLE flag set.
	if (!ShouldActivateWhenVisible())
	{
		Close();
	}
}

////////////////////////////////////////////////////////////////////////////
//功 能：显示时是否应该可见
//描 述：msform的控件要依据此关闭，否则无法完全释放
////////////////////////////////////////////////////////////////////////////
BOOL CCtrlItem::ShouldActivateWhenVisible()
{
	DWORD dwMiscStatus;

	dwMiscStatus = 0;
	m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMiscStatus);

	if (!(dwMiscStatus&OLEMISC_ACTIVATEWHENVISIBLE))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCtrlItem::IsWindowless() const
{
	return m_bWindowless;
}

BOOL CCtrlItem::SupportsIPointerInactive() const
{
	return (m_pPointerInactive != NULL);
}

BOOL CCtrlItem::OnWindowMessage(const MSG* pMessage)
{
	LRESULT lResult;
	HRESULT hResult;
	
	LPARAM lParam = pMessage->lParam;
	
	if (pMessage->message >= WM_MOUSEFIRST && pMessage->message <= WM_MOUSELAST)
	{
		CPoint point((DWORD)lParam);
		((CRunView *)m_pCtrlObj->m_pDocument->GetRunView())->ClientToDoc(point);
		lParam = MAKELPARAM(point.x, point.y);
	}
	
	hResult = m_pOleInPlaceObjectWindowless->OnWindowMessage(pMessage->message,
		pMessage->wParam, lParam, &lResult);

	return (hResult == S_OK);
}

BOOL CCtrlItem::ActsLikeButton() const
{
   return (m_dwMiscStatus&OLEMISC_ACTSLIKEBUTTON);
}

BOOL CCtrlItem::IsLabelControl() const
{
   return (m_dwMiscStatus&OLEMISC_ACTSLIKELABEL);
}

DWORD CCtrlItem::GetActivationPolicy() const
{
	DWORD dwPolicy;
	HRESULT hResult;

	ASSERT(m_pPointerInactive != NULL);
	ASSERT(!IsInPlaceActive());

	hResult = m_pPointerInactive->GetActivationPolicy(&dwPolicy);
	if (FAILED(hResult))
	{
		return 0;
	}

	return dwPolicy;
}

DWORD CCtrlItem::OnPointerInactiveMove(CPoint point,
	DWORD dwKeyState)
{
	DWORD dwPolicy;

	ASSERT(m_pPointerInactive != NULL);
	ASSERT(!IsInPlaceActive());

	dwPolicy = GetActivationPolicy();

	if (dwPolicy&POINTERINACTIVE_ACTIVATEONENTRY)
	{
		return dwPolicy;
	}

	m_pPointerInactive->OnInactiveMouseMove(&m_pCtrlObj->m_position.GetRECT(), point.x, point.y,
			dwKeyState);

	return 0 ;
}

DWORD CCtrlItem::OnPointerInactiveSetCursor(CPoint point,
	DWORD dwMouseMessage, BOOL* ptHandled)
{
	HRESULT hResult;
	DWORD dwPolicy;

	ASSERT(ptHandled != NULL);
	*ptHandled = FALSE;

	ASSERT(m_pPointerInactive != NULL);
	ASSERT(!IsInPlaceActive());

	dwPolicy = GetActivationPolicy();

	if (dwPolicy&POINTERINACTIVE_ACTIVATEONENTRY)
	{
		return dwPolicy;
	}

	hResult = m_pPointerInactive->OnInactiveSetCursor(&m_pCtrlObj->m_position.GetRECT(), point.x,
	  point.y, dwMouseMessage, FALSE);
	if (hResult == S_OK)
	{
		*ptHandled = TRUE;
	}

	return 0;
}

void CCtrlItem::DisplayAsDefault(BOOL bDefault)
{
   ASSERT(ActsLikeButton());

   if (m_bDisplayAsDefault != bDefault)
   {
		m_bDisplayAsDefault = bDefault;
		
		// The default setting changed.
		m_pOleControl->OnAmbientPropertyChange(
			DISPID_AMBIENT_DISPLAYASDEFAULT);
   }
}

BOOL CCtrlItem::IsEnabled()
{
	BOOL bEnabled = TRUE;

	if (m_pDispatch != NULL)
	{
		COleDispatchDriver dd(m_pDispatch, FALSE);
		TRY
			dd.GetProperty(DISPID_ENABLED, VT_BOOL, &bEnabled);
		END_TRY
	}

	return bEnabled;
}