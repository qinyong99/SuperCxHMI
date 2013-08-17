// CxGenItem.cpp : implementation of the CCtrlItem class
//

#include "stdafx.h"
#include "DevInc.h"

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
	m_dwPropertyCookie(0),
	m_bWindowless(FALSE),
	m_dwAspectPass2(DVASPECT_CONTENT)
{
	m_pCtrlObj = pCtrlObj;
}

CCtrlItem::~CCtrlItem()
{
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
		if (m_pCtrlObj)
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
	m_pCtrlObj->m_position = rectPos;
	m_pCtrlObj->Invalidate();

	// Mark document as dirty.
	GetDocument()->SetModifiedFlag();

	return TRUE;
}

void CCtrlItem::OnGetItemPosition(CRect& rPosition)
{
	ASSERT_VALID(this);

	// update to extent of item if m_position is not initialized
	if (m_pCtrlObj->m_position.IsRectEmpty())
		UpdateExtent();

	// copy m_position, which is in document coordinates
	CLayoutView* pView = GetActiveView();
	ASSERT_VALID(pView);
	rPosition = m_pCtrlObj->m_position.GetRECT();
	//在位激活并且为窗口模式时变更坐标
	if (IsInPlaceActive() && !m_bWindowless)
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
//功 能：得到类名
//描 述：ProgID格式为<Vendor>.<Component>.<Version>，取<Component>作为类名
/////////////////////////////////////////////////////////////////////////////
CString CCtrlItem::GetNameOfClass()
{
	CComPtr<IProvideClassInfo> spProvideClassInfo;
	CComPtr<ITypeInfo> spClassInfo;

	// 获取类信息
	HRESULT hr = m_lpObject->QueryInterface(&spProvideClassInfo);
	if (SUCCEEDED(hr))
	{
		hr = spProvideClassInfo->GetClassInfo(&spClassInfo);
		if (SUCCEEDED(hr))
		{
			CComBSTR bstrName;
			spClassInfo->GetDocumentation(MEMBERID_NIL, &bstrName, NULL, NULL, NULL);
			return bstrName;
		}
	}

	return _T("Control");
	
//	USES_CONVERSION;
//
//	BSTR bstrProgID;
//	CLSID clsid = CLSID_NULL;
//	GetClassID(&clsid);
//	ProgIDFromCLSID(clsid, &bstrProgID);
//	CString strProgID = OLE2T(bstrProgID);
//	int index = strProgID.Find('.');
//	CString strCompAndNum = strProgID.Mid(index + 1);
//	index = strCompAndNum.Find('.');
//	strClassName = strCompAndNum.Left(index);
//
//	return strClassName;
}

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

//#include "OLEACC.H"
////////////////////////////////////////////////////////////////////////////
//功 能：完成创建
////////////////////////////////////////////////////////////////////////////
BOOL CCtrlItem::FinishCreate(HRESULT hr)
{
	BOOL tSuccess;
	IConnectionPointContainerPtr pCPContainer;
	HRESULT hResult;

	tSuccess = COleClientItem::FinishCreate(hr);
	if (!tSuccess)
	{
		return tSuccess;
	}

	hResult = m_lpObject->QueryInterface( IID_IConnectionPointContainer,
	  (void**)&pCPContainer );
   if ( SUCCEEDED( hResult ) )
   {
	//  if( m_dwEventCookie == 0 )
	//  {
	//	 hResult = pCPContainer->FindConnectionPoint( m_infoEvents.GetIID(),
	//		&m_pEventCP );
	//	 if( SUCCEEDED( hResult ) )
	//	 {
	//		m_pEventCP->Advise( &m_xEventHandler, &m_dwEventCookie );
	//	 }
	 // }

	  if (m_dwPropertyCookie == 0)
	  {
		 hResult = pCPContainer->FindConnectionPoint( IID_IPropertyNotifySink,
			&m_pPropertyCP );
		 if (SUCCEEDED(hResult))
		 {
			m_pPropertyCP->Advise(&m_xPropertyNotifySink,
			   &m_dwPropertyCookie );
		 }
	  }
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

/*	IAccessible* pAccessible = NULL;
	hResult = m_lpObject->QueryInterface(IID_IAccessible,
		(void**)&pAccessible);
	if (SUCCEEDED(hResult))
	{
		pAccessible->put_accName(COleVariant(long(0)), L"aa");
//		TRACE("Control supports IViewObjectEx.\n");
//		m_pViewObjectEx->GetViewStatus(&m_dwViewStatus);
	}
	if (pAccessible != NULL)
		pAccessible->Release();
*/
//	UpdateLink();
	

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

BOOL CCtrlItem::ActsLikeButton() const
{
   return (m_dwMiscStatus&OLEMISC_ACTSLIKEBUTTON);
}

BOOL CCtrlItem::IsLabelControl() const
{
   return (m_dwMiscStatus&OLEMISC_ACTSLIKELABEL);
}

///////////////////////////////////////////////////////////////////////
//功 能：释放项目使用的资源
//描 述：必须在此释放项目使用的资源，否则程序无法完全退出
///////////////////////////////////////////////////////////////////////
void CCtrlItem::Release(OLECLOSE dwCloseOption)
{
	HRESULT hResult;
	IConnectionPointContainerPtr pConnectionPointContainer;

	UINT nState = GetItemState();
	if ((nState == activeUIState) || (nState == activeState) || (nState == openState))
	{
		Close( OLECLOSE_NOSAVE);
	}

    if (m_pOleInPlaceObjectWindowless != NULL)
	{
		m_pOleInPlaceObjectWindowless.Release();
	}

	if (m_pViewObjectEx != NULL)
    {
		m_pViewObjectEx.Release();
    }
   
	if (m_pExtendedControl != NULL)
	{
		// Salvage the display name from the extended control before it goes
		// away.
//		m_strDisplayName = BSTR( m_pExtendedControl->Name );
		m_pExtendedControl.Release();
	}

	if ((m_lpObject != NULL) && (m_dwPropertyCookie != 0))

//	if( (m_lpObject != NULL) && ((m_dwEventCookie != 0) ||
//	  (m_dwPropertyCookie != 0)) )
	{
		m_lpObject->QueryInterface(IID_IConnectionPointContainer,
			(void**)&pConnectionPointContainer);
	}

	if (m_dwPropertyCookie != 0)
	{
		if (m_pPropertyCP == NULL)
		{
			 // We must have quick activated
			hResult = pConnectionPointContainer->FindConnectionPoint(
				IID_IPropertyNotifySink, &m_pPropertyCP);
			if (FAILED(hResult))
			{
				TRACE("QuickActivated control connected to property notify sink, but doesn't support that connection point.\n" );
			}
		}
		if (m_pPropertyCP != NULL)
		{
			m_pPropertyCP->Unadvise(m_dwPropertyCookie);
			m_dwPropertyCookie = 0;
			m_pPropertyCP.Release();
		}
	}

	if (pConnectionPointContainer != NULL)
	{
		pConnectionPointContainer.Release();
	}

	COleClientItem::Release(dwCloseOption);

//  CoFreeUnusedLibraries();
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
	if (GetCachedExtent(&size)) 
	{
		// OLE returns the extent in HIMETRIC units -- we need pixels.
		CClientDC dc(NULL);
		dc.HIMETRICtoDP(&size);

		// Only invalidate if it has actually changed and also only if it is not
		// in-place active.
		CSize szTmp;
		szTmp.cx = int(m_pCtrlObj->m_position.Size().cx);
		szTmp.cy = int(m_pCtrlObj->m_position.Size().cy);
		if ((size != szTmp) && !IsInPlaceActive())
		{
			// Invalidate old, update, invalidate new.
			m_pCtrlObj->Invalidate();
			m_pCtrlObj->m_position.bottom = m_pCtrlObj->m_position.top + size.cy;
			m_pCtrlObj->m_position.right = m_pCtrlObj->m_position.left + size.cx;
			m_pCtrlObj->Invalidate();

			// mark document as modified.
			GetDocument()->SetModifiedFlag();
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

   	CLayoutView* pView = GetActiveView();
	CRect rectTemp = m_pCtrlObj->m_position.GetRECT();
	//在位激活并且为窗口模式时变更坐标
	if (IsInPlaceActive() && !m_bWindowless)
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
	CRect rcPos = m_pCtrlObj->m_position.GetRECT();

	if (!rcPos.PtInRect(point))
	{
		// Trivially reject the point.
		return HITRESULT_OUTSIDE;
	}
	//自行加入
	else
	{
	   return HITRESULT_HIT;
	}

	hResult = m_lpObject->QueryInterface(IID_IViewObjectEx,
		(void**)&pViewObjectEx);
	if (SUCCEEDED(hResult))
	{
		hResult = pViewObjectEx->QueryHitPoint(DVASPECT_CONTENT, &rcPos, point,
			0, &dwHitResult);
		if (SUCCEEDED(hResult))
		{
			return dwHitResult;
		}
	}

	// If we got here, either the object doesn't support hit testing, or the hit
	// test call failed.  Since we would have rejected the point if it didn't
	// lie within the object's bounding rect, we know that the point lies within
	// the object.

	return HITRESULT_HIT;
}

BEGIN_INTERFACE_MAP(CCtrlItem, COleClientItem)
	INTERFACE_PART(CCtrlItem, IID_IPropertyNotifySink, PropertyNotifySink)
	INTERFACE_PART(CCtrlItem, IID_IDispatch, AmbientProperties)
	INTERFACE_PART(CCtrlItem, IID_IOleInPlaceSiteEx, OleInPlaceSiteWindowless)
	INTERFACE_PART(CCtrlItem, IID_IOleInPlaceSiteWindowless, OleInPlaceSiteWindowless)
END_INTERFACE_MAP()

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

	return pThis->m_xOleIPSite.OnInPlaceActivate();
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

	return S_OK ;
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

	return S_OK;
}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::GetCapture()
{
	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);

//   if (pThis->GetActiveView()->GetItemWithCapture() == pThis)
//   {
//		return S_OK;
//   }
//   else
//   {
		return S_FALSE;
//   }
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
		return( S_OK );
/*	METHOD_PROLOGUE( CCtrlItem, OleInPlaceSiteWindowless );

	if( pThis->GetActiveView()->GetItemWithFocus() == pThis )
	{
		return( S_OK );
	}
	else
	{
		return( S_FALSE );
	}
*/
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

		if (rect != *prect)
		{

		}
	}

	if (!rect.IsRectEmpty())
	{
		pThis->GetActiveView()->InvalidateRect(&rect, tErase);
		// 2006.10.22 注释掉下面语句，解决控件在组里的情况下移动刷新的问题
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
	
   return S_OK;
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
	BOOL tCapture)
{
	return S_FALSE;
//   HRESULT hResult;

//	METHOD_PROLOGUE(CCtrlItem, OleInPlaceSiteWindowless);
//
//	return S_OK;

}

STDMETHODIMP CCtrlItem::XOleInPlaceSiteWindowless::SetFocus(
	BOOL tFocus )
{
	return S_FALSE;
	
/*	HRESULT hResult;

	METHOD_PROLOGUE( CCtrlItem, OleInPlaceSiteWindowless );

	if( tFocus )
	{
		hResult = pThis->GetActiveView()->OnWindowlessSetFocus( pThis );
		if( FAILED( hResult ) )
		{
			return( hResult );
		}
	}
	else
	{
		hResult = pThis->GetActiveView()->OnWindowlessReleaseFocus();
		if( FAILED( hResult ) )
		{
			return( hResult );
		}
   }

   return( hResult );
*/
}

///////////////////////////////////////////////////////////////////////////////
// CCtrlItem::XPropertyNotifySink
///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_( ULONG ) CCtrlItem::XPropertyNotifySink::AddRef()
{
   METHOD_PROLOGUE(CCtrlItem, PropertyNotifySink)

   return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CCtrlItem::XPropertyNotifySink::Release()
{
	METHOD_PROLOGUE(CCtrlItem, PropertyNotifySink )

    return pThis->ExternalRelease();
}

STDMETHODIMP CCtrlItem::XPropertyNotifySink::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CCtrlItem, PropertyNotifySink)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CCtrlItem::XPropertyNotifySink::OnChanged(
	DISPID dispid )
{
  	return S_OK;
}

STDMETHODIMP CCtrlItem::XPropertyNotifySink::OnRequestEdit(
	DISPID dispid )
{
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

	TRACE("XAmbientProperties::Release().\n");

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
		TRACE("Control asked for ambient property %s, but the property was disabled.\n",
			LPCTSTR( pProp->GetName()));
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
			if (!(m_dwMiscStatus&OLEMISC_SETCLIENTSITEFIRST) )
			{
				hResult = m_lpObject->SetClientSite( GetClientSite() );
				if( FAILED( hResult ) )
				{
					TRACE( "SetClientSite failed.\n" );
				}
			}
		}
	}

	CheckGeneral(hResult);
}

//////////////////////////////////////////////////////////////////////////
//功 能：写入项目
//////////////////////////////////////////////////////////////////////////
void CCtrlItem::WriteItem(CArchive& ar)
{
	USES_CONVERSION;
	IStoragePtr pStorage;
	IStreamPtr pStream;
	IPersistStreamInitPtr pPSI;
	TCHAR szItemName[64];
	HRESULT hResult;
	IPersistStoragePtr pPersistStorage;
	CFormDoc* pDoc;
	CLSID clsid;

	(void)ar;

	pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	ASSERT(pDoc->m_lpRootStg != NULL);
	ASSERT(pDoc->m_bCompoundFile);
	ASSERT(m_lpStorage != NULL);
	ASSERT(!ar.m_bForceFlat);

	GetItemName(szItemName);

	//相当于另存为
	if (!pDoc->m_bSameAsLoad)
	{
		hResult = pDoc->m_lpRootStg->CreateStorage(T2COLE(szItemName),
			STGM_CREATE|STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE, 0, 0,
			&pStorage);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}
		m_lpNewStorage = pStorage;
		m_lpNewStorage->AddRef();
		m_bNeedCommit = TRUE;
	}
	else
	{
		pStorage = m_lpStorage;
	}

	pPersistStorage = m_lpObject;
	if (pPersistStorage != NULL)
	{
//		if (!pDoc->m_bSameAsLoad || pPersistStorage->IsDirty() == S_OK)
		{
			pPersistStorage->GetClassID(&clsid);
			hResult = WriteClassStg(pStorage, clsid);
			if (FAILED(hResult))
			{
				AfxThrowOleException(hResult);
			}

			// 2006.10.23 改成始终FALSE 
			hResult = pPersistStorage->Save(pStorage, FALSE/*pDoc->m_bSameAsLoad*/);
			if (FAILED(hResult))
			{
				AfxThrowOleException(hResult);
			}
		}
	}
	else
	{
		pPSI = m_lpObject;
		if (pPSI == NULL)
		{
			AfxThrowOleException(E_NOINTERFACE);
		}

		// Set the storage's CLSID to CLSID_NULL, so we know that we actually
		// used a stream
		hResult = WriteClassStg(pStorage, CLSID_NULL);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}

		hResult = pStorage->CreateStream(L"Contents", STGM_CREATE|
			STGM_READWRITE|STGM_SHARE_EXCLUSIVE, 0, 0, &pStream);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}

		pPSI->GetClassID(&clsid);
		hResult = WriteClassStm(pStream, clsid);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}

		hResult = pPSI->Save(pStream, TRUE);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}

		pStream.Release();
	}

	m_bNeedCommit = TRUE;
	pStorage->Commit(STGC_ONLYIFCURRENT);
}

//////////////////////////////////////////////////////////////////////////
//功 能：提交项目
//////////////////////////////////////////////////////////////////////////
void CCtrlItem::CommitItem(BOOL bSuccess)
{
	IPersistStoragePtr pPersistStorage;

	if (!m_bNeedCommit)
	{
		return;
	}

	if (m_lpNewStorage != NULL && !bSuccess)
	{
		m_lpNewStorage->Release();
		m_lpNewStorage = NULL;
	}

	pPersistStorage = m_lpObject;
	// If the object doesn't support IPersistStorage, then we used
	// IPersistStreamInit in a stream inside the storage.
	if (pPersistStorage != NULL)
	{
		//以下语句在很多情况下都会出错，MFC的缺省实现即有问题
		VERIFY(pPersistStorage->SaveCompleted(m_lpNewStorage) == S_OK);
	}

	if (m_lpNewStorage != NULL)
	{
		m_lpStorage->Release();
		m_lpStorage = m_lpNewStorage;
		m_lpNewStorage = NULL;
	}

	m_bNeedCommit = FALSE;
}

//////////////////////////////////////////////////////////////////////////
//功 能：得到项目属性页类号
//////////////////////////////////////////////////////////////////////////
BOOL CCtrlItem::GetPropertyPageCLSIDs(CArray<CLSID,
	CLSID& >& aclsidPages)
{
	ISpecifyPropertyPagesPtr pSpecify;
	CAUUID pages;
	HRESULT hResult;
	ULONG iPage;

	pSpecify = m_lpObject;
	if (pSpecify != NULL)
	{
		pages.cElems = 0;
		pages.pElems = NULL;
		hResult = pSpecify->GetPages(&pages);
		if (FAILED(hResult))
		{
			TRACE("ISpecifyPropertyPages::GetPages() failed\n");
			return FALSE;
		}

		for (iPage = 0; iPage < pages.cElems; iPage++)
		{
			aclsidPages.Add(pages.pElems[iPage]);
		}

		CoTaskMemFree(pages.pElems);
	}

	//可以在此加入扩展属性页
//	if( m_pExtendedControl != NULL )
//	{
//		clsid = __uuidof(CxExtPage);
//		clsid = __uuidof(CxPropBrowserPage);
//		aclsidPages.Add(clsid);
//	}
	
	return TRUE;
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

/////////////////////////////////////////////////////////////
//功 能：窗口项目进入非活动状态，
//描 述：必须调用基类实现
/////////////////////////////////////////////////////////////
void CCtrlItem::OnDeactivate(BOOL tNoRedraw)
{
	m_bWindowless = FALSE;
//	if (m_bWindowless)
//	{
//		if (GetActiveView()->GetItemWithFocus() == this)
//		{
//			GetActiveView()->OnWindowlessReleaseFocus();
//		}
//		m_bWindowless = FALSE;
//		m_pOleInPlaceObjectWindowless.Release();
//	}

//	if (!tNoRedraw)
//	{
//		InvalidateItem();
//	}

	COleClientItem::OnDeactivate();
}

////////////////////////////////////////////////////////////////////////
//功 能：
//参 数：
//	1、
//	2、保存信息的父目录       
//描 述：在第二个参数所指定的存储中创建新的子存储保存项目
////////////////////////////////////////////////////////////////////////
void CCtrlItem::CopyToClipboard(CArchive& ar, IStorage* pStorage)
{
	USES_CONVERSION;
	IPersistStoragePtr pPersistStorage;
	CLSID clsid;
	HRESULT hResult;
	TCHAR szItemName[64];
	IStoragePtr pSubStorage;

	ASSERT_VALID(this);
	ASSERT(ar.IsStoring());

	//用于确定所创建的子存储
	ar << m_dwItemNumber;
	GetItemName(szItemName);

	//创建子存储
	hResult = pStorage->CreateStorage(T2COLE(szItemName), STGM_READWRITE|
		STGM_CREATE|STGM_SHARE_EXCLUSIVE, 0, 0, &pSubStorage);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	pPersistStorage = m_lpObject;
	if (pPersistStorage == NULL)
	{
		AfxThrowOleException(E_NOINTERFACE);
	}

//	::OleSave(pPersistStorage, pSubStorage, FALSE);

	pPersistStorage->GetClassID(&clsid);
	hResult = WriteClassStg(pSubStorage, clsid);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

    hResult = pPersistStorage->Save(pSubStorage, FALSE);
    if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	hResult = pPersistStorage->SaveCompleted(NULL);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

}

BOOL CCtrlItem::CreateFromClipboard(CArchive& ar, IStorage* pStorage)
{
	USES_CONVERSION;
	HRESULT hResult;
	CLSID clsid;
	TCHAR szItemName[64];
	IStoragePtr pSubStorage;

	ASSERT(!ar.IsStoring());
	ASSERT(pStorage != NULL);

	ar >> m_dwItemNumber;
	GetItemName(szItemName);

	hResult = pStorage->OpenStorage(T2COLE(szItemName), NULL, STGM_READ|
			STGM_SHARE_EXCLUSIVE, NULL, 0, &pSubStorage);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	hResult = ReadClassStg(pSubStorage, &clsid);
	if (FAILED(hResult))
	{
		return FALSE;
	}

	return CreateOrLoad(clsid, IID_IStorage, pSubStorage);
}

///////////////////////////////////////////////////////////////
//注意：此处与CxRun实现的不同之处
///////////////////////////////////////////////////////////////
void CCtrlItem::OnActivate()
{
	COleClientItem::OnActivate();
}

void CCtrlItem::OnActivateUI()
{
	return;
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