// CtrlObj.cpp: implementation of the CCtrlObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RunInc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CCtrlObj, CDevObj, 0)

CCtrlObj::CCtrlObj(CDevObjDoc* pContainer) :
	CDevObj(pContainer)
{
	m_position = CRect(10, 10, 10, 10);
	m_pClientItem = NULL;
}

CCtrlObj::~CCtrlObj()
{
//	if (m_pClientItem != NULL)
//	{
//		m_pClientItem->Release();
//		delete m_pClientItem;
//		m_pClientItem = NULL;
//	}
}

////////////////////////////////////////////////////////////////////////
//功 能：从前到后绘制控件对象
////////////////////////////////////////////////////////////////////////
void CCtrlObj::DrawFrontToBack(CDC* pDC)
{
    ASSERT_VALID(this);

	CCtrlItem* pItem = m_pClientItem;
	if (pItem != NULL)
	{
		// draw the OLE item itself
		pItem->DrawFrontToBack(pDC, &m_position.GetRECT());
	}
}

////////////////////////////////////////////////////////////////////////
//功 能：从后到前绘制控件对象
////////////////////////////////////////////////////////////////////////
void CCtrlObj::DrawBackToFront(CDC* pDC)
{
	ASSERT_VALID(this);

	CCtrlItem* pItem = m_pClientItem;
	if (pItem != NULL)
	{
		// draw the OLE item itself
		pItem->DrawBackToFront(pDC, &m_position.GetRECT());
	}
}

////////////////////////////////////////////////////////////////////////
//功 能：绘制控件对象
////////////////////////////////////////////////////////////////////////
void CCtrlObj::Draw(CDC* pDC)
{
	ASSERT_VALID(this);

	if (m_bVisible)
	{
		CCtrlItem* pItem = m_pClientItem;
		if (pItem != NULL)
		{
			// draw the OLE item itself
			pItem->Draw(pDC, &m_position.GetRECT());
		}
	}
}

////////////////////////////////////////////////////////////////////////
//功 能：移动控件对象
////////////////////////////////////////////////////////////////////////
void CCtrlObj::MoveTo(const CRectF& position, BOOL bUniformScale, CRunView* pView)
{
	BOOL bSizeChanged;
	ASSERT_VALID(this);

	if (position == m_position)
		return;

	Invalidate(pView);

	bSizeChanged = (m_position.Size() != position.Size());
	m_position = position;
	
	if (bSizeChanged)
		SetExtent(pView);

	if (m_pClientItem->IsInPlaceActive())
		m_pClientItem->SetItemRects();

	Invalidate(pView);
}

////////////////////////////////////////////////////////////////////////
//功 能：得到对象接口指针
////////////////////////////////////////////////////////////////////////
LPUNKNOWN CCtrlObj::GetObjectUnknown()
{
	return m_pClientItem->m_lpObject;
}

IDispatch* CCtrlObj::GetObjectDispatch()
{
	return m_pClientItem->m_pDispatch;
}

void CCtrlObj::Invalidate(CRunView* pView)
{
	CDevObj::Invalidate(pView);
//	m_pClientItem->InvalidateItem();
}

////////////////////////////////////////////////////////////
//功 能：释放控件对象
//描 述：先要把项目释放
////////////////////////////////////////////////////////////
void CCtrlObj::Release()
{
	CDevObj::Release();

	if (m_pClientItem != NULL)
		m_pClientItem->Release();
}

////////////////////////////////////////////////////////////////////////
//功 能：控件对象序列化
////////////////////////////////////////////////////////////////////////
void CCtrlObj::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);
	ASSERT(ar.IsLoading());

	CDevObj::Serialize(ar);

	ar >> m_position;
	CCtrlItem* pItem = NULL;
	ar >> pItem;

	if (m_dwVer >= _SUPERCX_VER_0206)
	{
		ar >> m_nTabIndex;
		ar >> m_bTabStop;
		ar >> m_bDefault;
		ar >> m_bCancel;

// 		if (m_bDefault)
// 			ASSERT(FALSE);
	}
	else
	{
		m_nTabIndex = m_pDocument->GetCtrlItemCount();
		m_bTabStop = !pItem->IsLabelControl();
		m_bDefault = FALSE;
		m_bCancel = FALSE;
	}

	FinishCreate(pItem);

	//序列化动态属性
	SerializeDynamicProperty(ar);
}

/////////////////////////////////////////////////////////////////////////////
//功 能：创建或者导入控件对象
/////////////////////////////////////////////////////////////////////////////
BOOL CCtrlObj::Create(REFCLSID clsid)
{
	CCtrlItem* pItem = new CCtrlItem(m_pDocument);

	try
	{
		if (!pItem->CreateOrLoad(clsid, IID_NULL, NULL))
		{
			AfxThrowOleException(E_FAIL);
		}
	}
	catch (...)
	{
		pItem->Delete();
		throw;
	}

	return FinishCreate(pItem);
}

BOOL CCtrlObj::FinishCreate(CCtrlItem* pItem)
{
	ASSERT_VALID(pItem);

	m_pClientItem = pItem;
	m_pClientItem->m_pCtrlObj = this;

	return TRUE;	
}

void CCtrlObj::SetExtent(CRunView* pView)
{
	if (pView == NULL)
		pView = (CRunView *)((CFormDoc*)m_pDocument)->GetTopFormDoc()->GetRunView();
	ASSERT(pView != NULL);
	if (pView == NULL)
		return;
	
	CSizeF sizeF = m_position.Size();
	CSize size;
	size.cx = int(sizeF.cx);
	size.cy = int(sizeF.cy);
	pView->DocToClient(size);

	CClientDC dc(NULL);
	dc.DPtoHIMETRIC(&size);

	BOOL bSuccess = TRUE;
	TRY
	{
		CRectF position = m_position; // 可能在改变宽度的时候通过CCtrlItem::OnChangeItemPosition改变位置
		m_pClientItem->SetExtent(size);
		m_position = position;
		m_pClientItem->UpdateExtent();
	}
	CATCH(CException, e)
	{
		bSuccess = FALSE;
	}
	END_CATCH
}

void CCtrlObj::OnShow(CRunView* pView)
{
	if (m_pClientItem->ShouldActivateWhenVisible())
	{
		if (m_pClientItem->IsInvisibleAtRuntime() || !m_bVisible)
		{
			m_pClientItem->DoVerb(OLEIVERB_HIDE, pView);
		}
		else
		{
			m_pClientItem->DoVerb(OLEIVERB_INPLACEACTIVATE, pView);
		}
	}
}

void CCtrlObj::Active(CRunView* pView)
{
	if (pView == NULL)
		pView = (CRunView *)((CFormDoc*)m_pDocument)->GetTopFormDoc()->GetRunView();
	ASSERT(pView != NULL);
	if (pView == NULL)
		return;

	SetExtent(pView);

	OnShow(pView);

	CDevObj::Active(pView);
}

void CCtrlObj::Deactive()
{

}

////////////////////////////////////////////////////////////////////////
//功 能：控件对象处理消息
//参 数：2、如果是顶级控件对象，则有CRunView调用，参数为true，
//否则为组对象函数嵌套调用，参数为false
//描 述：控件对象目前不支持连接鼠标输入动态属性，所以并不处理bSubObj
//为true的情况
BOOL CCtrlObj::OnWindowMessage(const MSG* pMessage, BOOL bSubObj)
{
	if (!bSubObj && m_pClientItem->IsWindowless())
		return m_pClientItem->OnWindowMessage(pMessage);
	
	return FALSE;
}

BOOL CCtrlObj::TranslateAccelerator(const MSG* pMessage)
{
	IOleInPlaceActiveObjectPtr pInPlaceActiveObject;

	pInPlaceActiveObject = m_pClientItem->m_lpObject;
	if (pInPlaceActiveObject != NULL)
	{
		HRESULT hr = pInPlaceActiveObject->TranslateAccelerator((MSG *)pMessage);
		if (hr == S_OK)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CCtrlObj::HitTest(CPoint point, const CRunView* pView)
{
	if (m_bVisible)
	{
		if (m_pClientItem->HitTest(point) == HITRESULT_HIT)
			return TRUE;
	}
	
	return FALSE;
}

void CCtrlObj::SetTabIndex(int nTabIndex)
{
	if (nTabIndex < 0)
		nTabIndex = 0;

	m_nTabIndex = m_pDocument->SetItemTabOrder(m_pClientItem, nTabIndex);
}