// FormObj.cpp: implementation of the CFormObj class.
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

CDevObj* CFormObj::m_pObjectWithCapture = NULL;

IMPLEMENT_SERIAL(CFormObj, CDevObj, 0)

const IID IID_IDrawVariable = {0x07C9658C,0xB87E,0x4E62,{0x84,0x60,0xB1,0x70,0x0B,0xA3,0x35,0x66}};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//功能:构造函数
//参数:pParentDev[in]		所属的DevDoc对象(该参数用于保存对象时使用)
//							如该参数为NULL则表示保在时用其它一般方式
/////////////////////////////////////////////////////////////////////
CFormObj::CFormObj(CDevObjDoc* pContainerDoc) :
	CDevObj(pContainerDoc),
	m_pFormDoc(NULL),
	m_dwSubformMoudleNumber(-1)
{
	EnableAutomation();
	EnableConnections();

	m_bParseScript = FALSE;
	//
	AfxOleLockApp();
}

CFormObj::~CFormObj()
{
	AfxOleUnlockApp();
}

BOOL CFormObj::CreateFormDoc()
{
	m_pFormDoc = new CFormDoc();
	m_pFormDoc->m_pFormObj = this;
	
	if (!m_pFormDoc->OnNewDocument())
	{
		delete m_pFormDoc;
		m_pFormDoc = NULL;
		return FALSE;
	}
	
	return TRUE;
}

void CFormObj::Serialize(CArchive &ar)
{
	ASSERT(ar.IsLoading());

	USES_CONVERSION;
	HRESULT hResult;
	TCHAR szObjectName[64];
	IStoragePtr pSubStorage;

	CDevObj::Serialize(ar);

	GetObjectName(szObjectName);

	ar >> m_position;
	ar >> m_ptRotateBase;
	long lTemp;
	ar >> lTemp;
	m_enumScaleBase = enumScaleBasePoint(lTemp);
	ar >> m_bUniformScale;
	ar >> m_fHorScale;							
	ar >> m_fVerScale;							
	ar >> m_fRotateAngle; 
	ar >> m_bTransparency; 

	hResult = ((CFormDoc *)m_pDocument)->m_lpRootStg->OpenStorage(T2COLE(szObjectName), NULL, STGM_READWRITE|
			STGM_SHARE_EXCLUSIVE, NULL, 0, &pSubStorage);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	//注意CFormObj与CFormDoc的对等关系
	VERIFY(CreateFormDoc() == TRUE);
	m_pFormDoc->ReadFromStorage(pSubStorage);

	CFormDoc* pTopFormDoc = GetTopFormDoc();
	m_dwSubformMoudleNumber = pTopFormDoc->m_listSubformModule.GetCount();
	m_strSubformMoudleName.Format(_T("subformmodule%d"), m_dwSubformMoudleNumber);
	pTopFormDoc->m_listSubformModule.AddTail(this);

	//序列化动态属性
	SerializeDynamicProperty(ar);
}

void CFormObj::Draw(CDC* pDC)
{
	if (!m_bVisible)
		return;

	ASSERT(m_pFormDoc != NULL);

	if (!m_bTransparency)
	{
		CRect client = m_position.GetRECT();
		if (m_pFormDoc->m_pImageBackGround != NULL)
		{	
			using namespace Gdiplus;

			TextureBrush tBrush(m_pFormDoc->m_pImageBackGround);
			Graphics graphics(pDC->GetSafeHdc());
			graphics.FillRectangle(&tBrush, 
					Rect(client.left, client.top, client.Width(), client.Height()));
		}
		else
		{
			// paint background
			COLORREF clrBackColor;
			OleTranslateColor(m_pFormDoc->m_clrBackColor, 0, &clrBackColor);
			CBrush backBrush(clrBackColor);
			pDC->FillRect(client, &backBrush);
		}
	}

	m_pFormDoc->DrawDoc(pDC);
}

void CFormObj::Print(CDC* pDC)
{
	ASSERT(m_pFormDoc != NULL);
	
	CRect client = m_position.GetRECT();
	if (m_pFormDoc->m_pImageBackGround != NULL)
	{	
		using namespace Gdiplus;
		
		TextureBrush tBrush(m_pFormDoc->m_pImageBackGround);
		Graphics graphics(pDC->GetSafeHdc());
		graphics.SetPageUnit(UnitPixel);
		graphics.FillRectangle(&tBrush, 
			Rect(client.left, client.top, client.Width(), client.Height()));
	}
	else
	{
		// paint background
		COLORREF clrBackColor;
		OleTranslateColor(m_pFormDoc->m_clrBackColor, 0, &clrBackColor);
		CBrush backBrush(clrBackColor);
		pDC->FillRect(client, &backBrush);
	}
	
	m_pFormDoc->PrintDoc(pDC);
}

IDispatch* CFormObj::GetObjectDispatch()
{
	return GetIDispatch(FALSE);
}

LPUNKNOWN CFormObj::GetObjectUnknown()
{
	return GetInterface(&IID_IUnknown);

//	ASSERT(m_pFormDoc != NULL);

//	return m_pFormDoc->GetInterface(&IID_IUnknown);
}

void CFormObj::Release()
{
	CDevObj::Release();

	ASSERT(m_pFormDoc != NULL);

	if (m_pFormDoc != NULL)
	{
		m_pFormDoc->m_pFormObj = NULL;
		m_pFormDoc->OnCloseDocument();
		m_pFormDoc = NULL;
	}
}

BOOL CFormObj::HitTest(CPoint point, CRunView* pView)
{
	return CDevObj::HitTest(point, pView);
//	if (m_bVisible)
//	{
//		int nCount = m_arrSubObj.GetSize();
//		for (int i = 0; i < nCount; i++)
//		{
//			if (m_arrSubObj[i]->HitTest(point, pView))
//				return TRUE;
//		}
//	}
//
//	return FALSE;
}

CRect CFormObj::GetBoundingRect()
{
	CDevObj* pObj;
	CRect rcObj;
	long lLeft;
	long lTop;
	long lRight;
	long lBottom;

	rcObj = m_position.GetRECT();
	lLeft = rcObj.left;
	lTop = rcObj.top;
	lRight = rcObj.right;
	lBottom = rcObj.bottom;

	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		rcObj = pObj->GetBoundingRect();
		if (lLeft > rcObj.left)
			lLeft = rcObj.left;
		if (lTop > rcObj.top)
			lTop = rcObj.top;
		if (lRight < rcObj.right)
			lRight = rcObj.right;
		if (lBottom < rcObj.bottom)
			lBottom = rcObj.bottom;
	}
	return CRect(lLeft, lTop, lRight, lBottom);
}

void CFormObj::RecalcPosition()
{
	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		CRectF rcObj = pObj->GetPositionRect();
		if (rcObj.left < m_position.left)
			m_position.left = rcObj.left;
		if (rcObj.top < m_position.top)
			m_position.top = rcObj.top;
		if (rcObj.right > m_position.right)
			m_position.right = rcObj.right;
		if (rcObj.bottom > m_position.bottom)
			m_position.bottom = rcObj.bottom;
	}
}

/////////////////////////////////////////////////////////////////
//功能: 移动组合对象
//////////////////////////////////////////////////////////////////
void CFormObj::MoveTo(const CRectF &position, BOOL bUniformScale, CRunView* pView)
{
	ASSERT_VALID(this);

	if (m_bUniformScale)
		bUniformScale = TRUE;

	CRectF rcObj;
	Invalidate(pView);

	float fScaleX = position.Width() / m_position.Width();
	float fScaleY = position.Height() / m_position.Height();

	if (bUniformScale)
	{
		if (ABSOLUTEVALUE(fScaleX) < ABSOLUTEVALUE(fScaleY))
			fScaleX = fScaleX >= 0 ? ABSOLUTEVALUE(fScaleY) : -ABSOLUTEVALUE(fScaleY);
		else
			fScaleY = fScaleY >= 0 ? ABSOLUTEVALUE(fScaleX) : -ABSOLUTEVALUE(fScaleX);
	}

	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		rcObj = pObj->GetPositionRect();
		rcObj.OffsetRect(-m_position.TopLeft());
		rcObj.left = rcObj.left * fScaleX;
		rcObj.right = rcObj.right * fScaleX;
		rcObj.top = rcObj.top * fScaleY;
		rcObj.bottom = rcObj.bottom * fScaleY;
		rcObj.OffsetRect(position.TopLeft());
		pObj->MoveTo(rcObj, bUniformScale, pView);
	}
	
	m_ptRotateBase.x = position.left + (m_ptRotateBase.x - m_position.left) * fScaleX;
	m_ptRotateBase.y = position.top + (m_ptRotateBase.y - m_position.top) * fScaleY;

	m_position = position;
//	m_position.NormalizeRect();
	RecalcPosition();
			
	Invalidate(pView);
}

void CFormObj::FakeParseDynamic()
{
	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		pObj->FakeParseDynamic();
	}
	
	CDevObj::FakeParseDynamic();
}

void CFormObj::ParseDynamic()
{
	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		pObj->ParseDynamic();
	}

	CDevObj::ParseDynamic();
}

void CFormObj::InitRun()
{
	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		pObj->InitRun();
	}

	int nCount = m_arLinkInfo.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		m_arLinkInfo[i].pDynamicObject->InitRun(&m_xScriptEngineSite, GetObjectDispatch(), m_arLinkInfo[i].dispid);
	}

	//都加入动态组
	//if (nCount > 0)
	((CFormDoc *)m_pDocument)->m_dynamicobjects.AddTail(this);

	FireEvent(CXEVENID_INITIALIZE, EVENT_PARAM(VTS_NONE));
	
	//CDevObj::InitRun();
}

void CFormObj::Active(CRunView* pView)
{
	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		pObj->Active(pView);
	}
}

CDevDoc* CFormObj::GetDevDoc()
{
	CFormDoc* pTopFormDoc = GetTopFormDoc();
	if (pTopFormDoc != NULL)
		return pTopFormDoc->m_pDocument; 

	return NULL;
}

CFormDoc* CFormObj::GetTopFormDoc()
{
	if (!m_pDocument->IsKindOf(RUNTIME_CLASS(CFormDoc)))
		return NULL;

	CFormDoc* pTopDoc = (CFormDoc *)m_pDocument;
	while (pTopDoc->m_pFormObj != NULL)
		pTopDoc = (CFormDoc *)pTopDoc->m_pFormObj->m_pDocument;

	return pTopDoc;
}

CView* CFormObj::GetRunView()
{
	CFormDoc* pTopFormDoc = GetTopFormDoc();
	if (pTopFormDoc != NULL)
		return pTopFormDoc->GetRunView(); 

	return NULL;
}

void CFormObj::RotateAt(CPointF point, float fAngle, CRunView* pView)
{
	ASSERT_VALID(this);

	Invalidate(pView);

	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		pObj->RotateAt(point, fAngle);
	}

	m_ptRotateBase = CalculateRotatePoint(point, m_ptRotateBase, fAngle);

	RecalcPosition();
	Invalidate(pView);
}

void CFormObj::ScaleAt(CPointF point, float fHorScale, float fVerScale, CRunView* pView)
{
	ASSERT_VALID(this);

	Invalidate(pView);

	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		pObj->ScaleAt(point, fHorScale, fVerScale);
	}

	m_ptRotateBase -= point;
	m_ptRotateBase.x *= fHorScale; 
	m_ptRotateBase.y *= fVerScale; 
	m_ptRotateBase += point;

	RecalcPosition();
	Invalidate(pView);
}

CPoint CFormObj::GetRotateBasePoint()
{
	return CPoint(int(m_ptRotateBase.x), int(m_ptRotateBase.y));
}

void CFormObj::SetRotateBasePoint(CPoint point)
{
	m_ptRotateBase.x = (float)point.x;
	m_ptRotateBase.y = (float)point.y;
}

/////////////////////////////////////////////////////////////////////////
//功 能：当最后引用被释放，删除对象
//描 述：与CCmdTarget::OnFinalRelease()实现不一样，如调用CCmdTarget::OnFinalRelease()会出错
/////////////////////////////////////////////////////////////////////////
void CFormObj::OnFinalRelease()
{
 	// When the last reference for an automation object is released
 	//  OnFinalRelease is called.  This implementation deletes the
 	//  object.  Add additional cleanup required for your object before
 	//  deleting it from memory.
 
 	delete this;
}

BEGIN_CONNECTION_MAP(CFormObj, CDevObj)
   CONNECTION_PART(CFormObj, __uuidof(IFormObjectEvents), FormObjectCP)
END_CONNECTION_MAP()

BEGIN_DISPATCH_MAP(CFormObj, CDevObj)
	//{{AFX_DISPATCH_MAP(CFormObj)
	DISP_PROPERTY_EX_ID(CFormObj, "ForeColor", CXPROPID_ForeColor, GetForeColor, SetForeColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CFormObj, "BackColor", CXPROPID_BackColor, GetBackColor, SetBackColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CFormObj, "BrushImageFile", CXPROPID_BrushImageFileName, GetBrushImageFile, SetBrushImageFile, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CFormObj, "Transparency", CXPROPID_Transparency, m_bTransparency, OnTransparencyChanged, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFormObj, "VisibleLayer", CXPROPID_VisibleLayer, GetVisibleLayer, SetVisibleLayer, VT_I4)
	
	DISP_PROPERTY_EX_ID(CFormObj, "HorizontalPosition", CXPROPID_HorPosition, GetHorizontalPosition, SetHorizontalPosition, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "VerticalPosition", CXPROPID_VerPosition, GetVerticalPosition, SetVerticalPosition, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "Width", CXPROPID_Width, GetWidth, SetWidth, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "Height", CXPROPID_Height, GetHeight, SetHeight, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "RoateBasePointX", CXPROPID_RoateBasePointX, GetRoateBasePointX, SetRoateBasePointX, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "RoateBasePointY", CXPROPID_RoateBasePointY, GetRoateBasePointY, SetRoateBasePointY, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "RotateAngle", CXPROPID_RotateAngle, GetRotateAngle, SetRotateAngle, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "ScaleBasePoint", CXPROPID_ScaleBasePoint, GetScaleBasePoint, SetScaleBasePoint, VT_I4)
	DISP_PROPERTY_EX_ID(CFormObj, "HorizontalScale", CXPROPID_HorScale, GetHorizontalScale, SetHorizontalScale, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "VerticalScale", CXPROPID_VerScale, GetVerticalScale, SetVerticalScale, VT_R4)
	DISP_PROPERTY_NOTIFY_ID(CFormObj, "UniformScale", CXPROPID_UniformScale, m_bUniformScale, OnUniformScaleChanged, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFormObj, "Layer", CXPROPID_Layer, GetLayer, SetLayer, VT_I4)
	DISP_PROPERTY_EX_ID(CFormObj, "Locked", CXPROPID_Locked, GetLocked, SetLocked, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFormObj, "Visible", CXPROPID_Visible, GetVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CFormObj, "ToolTipText", CXPROPID_ToolTipText, m_strToolTip, OnToolTipTextChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CFormObj, "Description", CXPROPID_Description, m_strDescription, OnDescriptionChanged, VT_BSTR)
	DISP_PROPERTY_EX_ID(CFormObj, "MousePointer", CXPROPID_MousePointer, GetMousePointer, SetMousePointer, VT_I4)
	DISP_PROPERTY_EX_ID(CFormObj, "Privilege", CXPROPID_Privilege, GetPrivilege, SetPrivilege, VT_I4)
	DISP_PROPERTY_EX_ID(CFormObj, "TagGroup", CXPROPID_TagGroup, GetTagGroup, SetTagGroup, VT_BSTR)
	DISP_PROPERTY_EX_ID(CFormObj, "AssocTagGroup", CXPROPID_AssocTagGroup, GetAssocTagGroup, SetNotSupported, VT_BSTR)

	DISP_FUNCTION_ID(CFormObj, "GetObjectByName", CXMETHID_GetObjectByName, GetObjectByName, VT_UNKNOWN, VTS_BSTR)
	DISP_PROPERTY_EX_ID(CFormObj, "Objects", CXPROPID_Objects, GetObjectCollection, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CFormObj, "Name", CXPROPID_Name, GetName, SetNotSupported, VT_BSTR)
	DISP_FUNCTION_ID(CFormObj, "IsConnected", CXMETHID_IsConnected, IsConnected, VT_I2, VTS_BSTR)
	DISP_FUNCTION_ID(CFormObj, "ConnectObject", CXMETHID_ConnectObject, ConnectObject, VT_EMPTY, VTS_BSTR VTS_UNKNOWN)
	DISP_FUNCTION_ID(CFormObj, "ConnectDirect", CXMETHID_ConnectDirect, ConnectDirect, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CFormObj, "GetConnectObject", CXMETHID_GetConnectObject, GetConnectObject, VT_UNKNOWN, VTS_BSTR)
	DISP_FUNCTION_ID(CFormObj, "Disconnect", CXMETHID_Disconnect, Disconnect, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CFormObj, "GetConnectState", CXMETHID_GetConnectState, GetConnectState, VT_I4, VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

//const extern IID LIBID_CXHMIOBJLib;
//IMPLEMENT_OLETYPELIB(CFormObj, LIBID_CXHMIOBJLib, 1, 0)

//ACF61CEA-3805-11D5-A468-8E961CC7BB0B
//const IID LIBID_CXHMIOBJLib = {0xACF61CEA,0x3805,0x11D5,{0xA4,0x68,0x8E,0x96,0x1C,0xC7,0xBB,0x0B}};


UINT CFormObj::GetTypeInfoCount()
{ 
	return 1; 
} 

HRESULT CFormObj::GetTypeLib(LCID lcid, LPTYPELIB* ppTypeLib)
{ 
	return m_pDocument->GetTypeLib(lcid, ppTypeLib);
//	return ::LoadRegTypeLib(tlid, 1, 0, lcid, ppTypeLib); 
} 

CTypeLibCache* CFormObj::GetTypeLibCache() 
{ 
	return m_pDocument->GetTypeLibCache();
//	AFX_MANAGE_STATE(m_pModuleState); 
//	return AfxGetTypeLibCache(&LIBID_CXHMIOBJLib); } 
}

// {80008000-8000-8000-8000-800080008202}
static const IID IID_IFormObject =
{ 0x80008000, 0x8000, 0x8000, { 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x82, 0x08 } };

BEGIN_INTERFACE_MAP(CFormObj, CDevObj)
	INTERFACE_PART(CFormObj, IID_IDispatch, DispatchExtend)
	INTERFACE_PART(CFormObj, IID_IFormObject, DispatchExtend)
	INTERFACE_PART(CFormObj, IID_IConnectionPointContainer, ConnPtContainer)
	INTERFACE_PART(CFormObj, IID_IProvideClassInfo, ProvideMultipleClassInfo)
	INTERFACE_PART(CFormObj, IID_IProvideClassInfo2, ProvideMultipleClassInfo)
	INTERFACE_PART(CFormObj, IID_IProvideMultipleClassInfo, ProvideMultipleClassInfo)
END_INTERFACE_MAP()

////////////////////////////////////////////////////////////////
STDMETHODIMP_( ULONG ) CFormObj::XDispatchExtend::AddRef()
{
	METHOD_PROLOGUE(CFormObj, DispatchExtend)

    return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CFormObj::XDispatchExtend::Release()
{
	METHOD_PROLOGUE(CFormObj, DispatchExtend)

	return pThis->ExternalRelease();
}

STDMETHODIMP CFormObj::XDispatchExtend::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CFormObj, DispatchExtend)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CFormObj::XDispatchExtend::GetIDsOfNames(
	REFIID iid, LPOLESTR* ppszNames, UINT nNames, LCID lcid, DISPID* pDispIDs)
{
	USES_CONVERSION;

	(void)lcid;

	METHOD_PROLOGUE(CFormObj, DispatchExtend)
	ASSERT_VALID(pThis);

	if (nNames == 1)
	{
		if (!IsEqualIID( iid, IID_NULL))
		{
			return E_INVALIDARG;
		}
		if (ppszNames == NULL)
		{
			return E_INVALIDARG;
		}
		if (pDispIDs == NULL)
		{
			return E_POINTER;
		}

		void *pTemp;
		CString strName = *ppszNames;
		strName.MakeUpper();
		if (pThis->m_pFormDoc->m_mapNameToDispid.Lookup(strName, pTemp))
		{
			*pDispIDs = DISPID(pTemp);
			return S_OK;
		}
	}
	
	return ((IDispatch *)(&pThis->m_xDispatch))->GetIDsOfNames(iid, ppszNames, nNames, lcid, pDispIDs);
}

STDMETHODIMP CFormObj::XDispatchExtend::GetTypeInfo(
	UINT iTypeInfo, LCID lcid, ITypeInfo** ppTypeInfo)
{
	(void)iTypeInfo;
	(void)lcid;
	(void)ppTypeInfo;

	METHOD_PROLOGUE(CFormObj, DispatchExtend)
	ASSERT_VALID(pThis);

	return ((IDispatch *)(&pThis->m_xDispatch))->GetTypeInfo(iTypeInfo, lcid, ppTypeInfo);
}

STDMETHODIMP CFormObj::XDispatchExtend::GetTypeInfoCount(
   UINT* pnInfoCount )
{
	METHOD_PROLOGUE(CFormObj, DispatchExtend)
	ASSERT_VALID(pThis);

	return ((IDispatch *)(&pThis->m_xDispatch))->GetTypeInfoCount(pnInfoCount);
}

STDMETHODIMP CFormObj::XDispatchExtend::Invoke(
	DISPID dispidMember, REFIID iid, LCID lcid, WORD wFlags,
	DISPPARAMS* pdpParams, VARIANT* pvarResult, EXCEPINFO* pExceptionInfo,
	UINT* piArgErr)
{
//	HRESULT hResult;
	COleVariant varValue;

	(void)iid;
	(void)lcid;
	(void)wFlags;
	(void)pdpParams;
	(void)pExceptionInfo;
	(void)piArgErr;

	METHOD_PROLOGUE(CFormObj, DispatchExtend)
	ASSERT_VALID(pThis);

	if ((dispidMember & CXDISPID_EXTPROP) != CXDISPID_EXTPROP)
		return ((IDispatch *)(&pThis->m_xDispatch))->Invoke(dispidMember, iid, lcid, wFlags, 
			pdpParams, pvarResult, pExceptionInfo, piArgErr);

	if ((wFlags & DISPATCH_PROPERTYGET) != 0 || (wFlags & DISPATCH_PROPERTYPUT) != 0)
	{
		dispidMember &= 0x7fff;
		
		if (dispidMember >= pThis->m_pFormDoc->GetObjects()->GetCount())
			return DISP_E_MEMBERNOTFOUND;
		
		IDispatch* pdisp = pThis->m_pFormDoc->m_ppObjects[dispidMember];
		
		if ((wFlags & DISPATCH_PROPERTYGET) != 0)
		{
			if (pvarResult)	
			{
				pvarResult->vt = VT_DISPATCH;
				pvarResult->pdispVal = pdisp;
				//if (pdisp != NULL)
				pdisp->AddRef();		// We're returning this disp ptr
				return S_OK;
			}
		}
		else
		{
			return pdisp->Invoke(0, iid, lcid, wFlags,
				pdpParams, pvarResult, pExceptionInfo, piArgErr);
		}
	}

/*
	dispidMember &= 0x7fff;

//	if (dispidMember >= pThis->m_apObjectDispatchs.GetSize())
//		return DISP_E_MEMBERNOTFOUND;
	if (dispidMember >= pThis->m_pFormDoc->GetObjects()->GetCount())
		return DISP_E_MEMBERNOTFOUND;

	
//	m_ppObjects

//	if (dispidMember >= pThis->m_apObjectDispatchs.GetSize())
//		return DISP_E_MEMBERNOTFOUND;

	if (pdpParams->cArgs)
		return DISP_E_BADPARAMCOUNT;

	if (pdpParams->cNamedArgs)
		return DISP_E_NONAMEDARGS;

	IDispatch* pdisp = pThis->m_pFormDoc->m_ppObjects[dispidMember];
	
	if (pvarResult)	
    {
//		if (dispidMember == 0)
//			return DISP_E_MEMBERNOTFOUND; 
		pvarResult->vt = VT_DISPATCH;
		pvarResult->pdispVal = pdisp;
		//if (pdisp != NULL)
			pdisp->AddRef();		// We're returning this disp ptr
	//	return DISP_E_MEMBERNOTFOUND;
    }*/


  	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CFormObj::XProvideMultipleClassInfo

STDMETHODIMP_(ULONG) CFormObj::XProvideMultipleClassInfo::AddRef()
{
	METHOD_PROLOGUE_EX_(CFormObj, ProvideMultipleClassInfo)

	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CFormObj::XProvideMultipleClassInfo::Release()
{
	METHOD_PROLOGUE_EX_(CFormObj, ProvideMultipleClassInfo)

	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CFormObj::XProvideMultipleClassInfo::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CFormObj, ProvideMultipleClassInfo)

	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CFormObj::XProvideMultipleClassInfo::GetClassInfo(
	LPTYPEINFO* ppTypeInfo)
{
	METHOD_PROLOGUE_EX(CFormObj, ProvideMultipleClassInfo)

	return pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), __uuidof(FormObject), ppTypeInfo);
}

STDMETHODIMP CFormObj::XProvideMultipleClassInfo::GetGUID(DWORD dwGuidKind,
	GUID* pGUID)
{
	METHOD_PROLOGUE_EX_(CFormObj, ProvideMultipleClassInfo)

	if (dwGuidKind == GUIDKIND_DEFAULT_SOURCE_DISP_IID)
	{
		*pGUID = __uuidof(IFormObjectEvents);
		return NOERROR;
	}
	else
	{
		*pGUID = GUID_NULL;
		return E_INVALIDARG;
	}
}

STDMETHODIMP CFormObj::XProvideMultipleClassInfo::GetMultiTypeInfoCount(
	ULONG *pcti)
{
	METHOD_PROLOGUE(CFormObj, ProvideMultipleClassInfo)

	*pcti = 2;

	return S_OK;
}

STDMETHODIMP CFormObj::XProvideMultipleClassInfo::GetInfoOfIndex(
	ULONG iti, DWORD dwFlags, ITypeInfo** pptiCoClass, 
	DWORD* pdwTIFlags, ULONG* pcdispidReserved, IID* piidPrimary, 
	IID* piidSource)
{
	METHOD_PROLOGUE(CFormObj, ProvideMultipleClassInfo)

	HRESULT    hr        = S_OK;
	ITypeInfo *ptinfoOut = NULL;

//	if (pThis->m_bClose)
//		return E_UNEXPECTED;

	if (iti > 1)
		hr = E_FAIL;

	if (pThis->m_pFormDoc->m_ptinfoCls == NULL)
		pThis->m_pFormDoc->BuildTypeInfo();

	if (dwFlags & MULTICLASSINFO_GETTYPEINFO)
    {
		if (!pptiCoClass || !pdwTIFlags)
			hr = E_INVALIDARG;
		else
		{
			*pdwTIFlags  = 0L;
			*pptiCoClass = NULL;
			if (iti == 0)
			{
				ptinfoOut = pThis->m_pFormDoc->m_ptinfoCls;
			}
			else
			{
			//	theApp.m_pExtTypeLib->GetTypeInfoOfGuid(__uuidof(UserForm), &ptinfoOut);

				pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), __uuidof(FormObject), &ptinfoOut);
			}
		}
    }

	if (dwFlags & MULTICLASSINFO_GETNUMRESERVEDDISPIDS)
    {
		if (!pcdispidReserved)
			hr = E_INVALIDARG;
		else
		*pcdispidReserved = 256;
    }

	if (dwFlags & MULTICLASSINFO_GETIIDPRIMARY)
    {
		if (!piidPrimary)
			hr = E_INVALIDARG;
		else if (iti == 0)
			*piidPrimary = pThis->m_pFormDoc->m_iidSubObj;
		else
			*piidPrimary = IID_IFormObject;
    }

	if (dwFlags & MULTICLASSINFO_GETIIDSOURCE)
    {
		if (!piidSource)
			hr = E_INVALIDARG;
		else if (iti == 0)
			*piidSource = IID_NULL;
		else
			*piidSource = __uuidof(IFormObjectEvents);
    }

	// Fill in ptinfo out param
	if (hr)
    {
		if (pptiCoClass)
		*pptiCoClass = NULL;
    }
	else if (pptiCoClass)
    {
		*pptiCoClass = ptinfoOut;
		(*pptiCoClass)->AddRef();
    }

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// CFormObj message handlers

BOOL CFormObj::GetDispatchIID(IID* piid)
{
	ASSERT( piid != NULL );

	*piid = __uuidof(IFormObject);

	return TRUE;
}

void CFormObj::OnUniformScaleChanged()
{
}

enumMousePointer CFormObj::GetMousePointer()
{
	return m_pFormDoc->m_enumMousePointer;
}

void CFormObj::SetMousePointer(enumMousePointer fNewValue)
{
	m_pFormDoc->m_enumMousePointer = fNewValue;
}

int CFormObj::GetPrivilege()
{
	return m_nPrivilege;
}

void CFormObj::SetPrivilege(int iNewValue)
{
	if (iNewValue < 0 || iNewValue > 1000)
		return;
	
	m_nPrivilege = iNewValue;
}

void CFormObj::OnToolTipTextChanged()
{
}

void CFormObj::OnDescriptionChanged()
{
}

float CFormObj::GetHorizontalPosition()
{
	return m_position.left;
}

void CFormObj::SetHorizontalPosition(float fNewValue)
{
	if (m_position.left == fNewValue)
		return;
	
	CRectF rect = m_position;
	rect.OffsetRect(fNewValue - m_position.left, 0);
	MoveTo(rect, FALSE);
}

float CFormObj::GetRoateBasePointX()
{
	return m_ptRotateBase.x;
}

void CFormObj::SetRoateBasePointX(float fNewValue)
{
	m_ptRotateBase.x = fNewValue;
}

float CFormObj::GetRoateBasePointY()
{
	return m_ptRotateBase.y;
}

void CFormObj::SetRoateBasePointY(float fNewValue)
{
	m_ptRotateBase.y = fNewValue;
}

float CFormObj::GetRotateAngle()
{
	return 0;
}

void CFormObj::SetRotateAngle(float fNewValue)
{
	if (m_fRotateAngle == fNewValue)
		return;

	RotateAt(m_ptRotateBase, fNewValue - m_fRotateAngle);		
	m_fRotateAngle = fNewValue;
}

CPointF CFormObj::GetScaleBasePointF()
{
	switch (m_enumScaleBase)
	{
	case ScaleBasePointCenter:
		return m_position.CenterPoint();
	case ScaleBasePointLeftTop:
		return m_position.TopLeft();
	case ScaleBasePointTopMiddle:
		return CPointF((m_position.left + m_position.right) / 2, m_position.top);
	case ScaleBasePointRightTop:
		return CPointF(m_position.right, m_position.top);
	case ScaleBasePointRightMiddle:
		return CPointF(m_position.right, (m_position.top + m_position.bottom) / 2);
	case ScaleBasePointRightBottom:
		return CPointF(m_position.right, m_position.bottom);
	case ScaleBasePointBottomMiddle:
		return CPointF((m_position.left + m_position.right) / 2, m_position.bottom);
	case ScaleBasePointLeftBottom:
		return CPointF(m_position.left, m_position.bottom);
	case ScaleBasePointLeftMiddle:
		return CPointF(m_position.left, (m_position.top + m_position.bottom) / 2);
	default:
		ASSERT(FALSE);
	}

	return CPointF(0, 0);
}

enumScaleBasePoint CFormObj::GetScaleBasePoint()
{
	return m_enumScaleBase;
}

void CFormObj::SetScaleBasePoint(enumScaleBasePoint fNewValue)
{
	m_enumScaleBase = fNewValue;
}

float CFormObj::GetHorizontalScale()
{
	return 1;
}

void CFormObj::SetHorizontalScale(float fNewValue)
{
	if (m_fHorScale == fNewValue)
		return;
	
	if (fNewValue == 0)
		fNewValue = 1 / m_position.Width();
	float fHorScale = fNewValue / m_fHorScale;		
	m_fHorScale = fNewValue;
	ScaleAt(GetScaleBasePointF(), fHorScale, 1);
}

float CFormObj::GetVerticalScale()
{
	return 1;
}

void CFormObj::SetVerticalScale(float fNewValue)
{
	if (m_fVerScale == fNewValue)
		return;

	if (fNewValue == 0)
		fNewValue = 1 / m_position.Height();
	float fVerScale = fNewValue / m_fVerScale;		
	m_fVerScale = fNewValue;
	ScaleAt(GetScaleBasePointF(), 1, fVerScale);
	
}

int CFormObj::GetLayer()
{
	return m_nLayer + 1;
}

void CFormObj::SetLayer(int lLayer)
{
	if (lLayer > 30 || lLayer < 1)
		return;

	if (m_nLayer == lLayer - 1)
		return;

	m_nLayer = lLayer - 1;
	Invalidate();
}

BOOL CFormObj::GetLocked()
{
	return m_bLocked;
}

void CFormObj::SetLocked(BOOL bNewVal)
{
	if (m_bLocked == bNewVal)
		return;
	
	m_bLocked = bNewVal;
	
	Invalidate();
}

BOOL CFormObj::GetVisible()
{
	return m_bVisible;
}

void CFormObj::SetVisible(BOOL bNewVal)
{
	if (m_bVisible == bNewVal)
		return;

	m_bVisible = bNewVal;
	Invalidate();
}

BSTR CFormObj::GetName()
{
	return GetDisplayName().AllocSysString();
}

BSTR CFormObj::GetTagGroup()
{
	return m_pFormDoc->m_strTagGroup.AllocSysString();	
}

void CFormObj::SetTagGroup(LPCTSTR lpszNewValue)
{
	m_pFormDoc->m_strTagGroup = lpszNewValue;
}

BSTR CFormObj::GetAssocTagGroup()
{
	return m_pFormDoc->GetAssocTagGroupEx(FALSE).AllocSysString();	
}

VARIANT_BOOL CFormObj::IsConnected(LPCTSTR pszPropName)
{
	USES_CONVERSION;

	VARIANT_BOOL bHasConnection = FALSE;
	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
    if (SUCCEEDED(((IDispatch *)(&m_xDispatch))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		HasDynamic(dispid, &bHasConnection);
	}
	else
	{
		CDevObj* pObj = FindVariableObject(pszPropName);
		if (pObj != NULL)
		{
			CComQIPtr<IDrawVariable, &IID_IDrawVariable> spDrawVariable(pObj->GetObjectUnknown());
			spDrawVariable->IsConnected(L"Value", &bHasConnection);
		}
	}

	return bHasConnection;
}

void CFormObj::ConnectObject(LPCTSTR pszPropName, LPUNKNOWN punkObject)
{
	USES_CONVERSION;

	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
	if (SUCCEEDED(((IDispatch *)(&m_xDispatch))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		AddDynamic(dispid, punkObject);
	}
	else
	{
		CDevObj* pObj = FindVariableObject(pszPropName);
		if (pObj != NULL)
		{
			CComQIPtr<IDrawVariable, &IID_IDrawVariable> spDrawVariable(pObj->GetObjectUnknown());
			spDrawVariable->ConnectObject(L"Value", punkObject);
		}
	}
}

void CFormObj::ConnectDirect(LPCTSTR pszPropName, LPCTSTR pszDataSource)
{
}

LPUNKNOWN CFormObj::GetConnectObject(LPCTSTR pszPropName)
{
	USES_CONVERSION;

	LPUNKNOWN punkObject = NULL;

	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
    if (SUCCEEDED(((IDispatch *)(&m_xDispatch))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		
		GetDynamic(dispid, &punkObject);
	}
	else
	{
		CDevObj* pObj = FindVariableObject(pszPropName);
		if (pObj != NULL)
		{
			CComQIPtr<IDrawVariable, &IID_IDrawVariable> spDrawVariable(pObj->GetObjectUnknown());
			spDrawVariable->GetConnectObject(L"Value", &punkObject);
		}
	}

	return punkObject;
}

void CFormObj::Disconnect(LPCTSTR pszPropName)
{
	USES_CONVERSION;

	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
    if (SUCCEEDED(((IDispatch *)(&m_xDispatch))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		RemoveDynamic(dispid);
	}
	else
	{
		CDevObj* pObj = FindVariableObject(pszPropName);
		if (pObj != NULL)
		{
			CComQIPtr<IDrawVariable, &IID_IDrawVariable> spDrawVariable(pObj->GetObjectUnknown());
			spDrawVariable->Disconnect(L"Value");
		}
	}
}

int CFormObj::GetConnectState(DISPID dispid)
{
	int nState = 0;
	
	if ((dispid & CXDISPID_EXTPROP) != CXDISPID_EXTPROP)
	{
		//只有水平位置，垂直位置，旋转角度支持鼠标输入
		if (dispid == CXPROPID_HorPosition || dispid == CXPROPID_VerPosition || dispid == CXPROPID_RotateAngle)
			nState |= 0x1;
		
		for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
		{
			if (m_arLinkInfo[i].dispid == dispid)
			{
				nState |= 0x2; //已经连接
				if (m_arLinkInfo[i].pDynamicObject->IsEnableMouseInput() == S_OK)
					nState |= 0x4;
				break;
			}
		}
	}
	else
	{
		int iIndex = dispid - CXDISPID_EXTPROP;
		
		CDevObjList* pObjects = m_pFormDoc->GetObjects();
		POSITION posObj = pObjects->GetHeadPosition();
		CDevObj* pObj;
		while (posObj != NULL)
		{
			pObj = pObjects->GetNext(posObj);
			if (pObj->IsKindOf(RUNTIME_CLASS(CDrawObj)) && pObj->GetClassName() == _T("Variable"))
			{
				if (iIndex == 0)
				{
					CComQIPtr<IDrawVariable, &IID_IDrawVariable> spDrawVariable(pObj->GetObjectUnknown());
					spDrawVariable->GetConnectState(0, &nState);
				}
				
				iIndex--;
			}
		}
	}
	
	return nState;
}

LPUNKNOWN CFormObj::GetObjectByName(LPCTSTR lpszName)
{
	return m_pFormDoc->GetObjectByName(lpszName);
}

LPDISPATCH CFormObj::GetObjectCollection()
{
	return m_pFormDoc->GetObjectCollection();
}

CDevObj* CFormObj::FindVariableObject(LPCTSTR lpszName)
{
	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		if (pObj->IsKindOf(RUNTIME_CLASS(CDrawObj)) && pObj->GetClassName() == _T("Variable"))
		{
			if (pObj->GetDisplayName().CollateNoCase(lpszName) == 0)
				return pObj;
		}
	}
	
	return NULL;
}

float CFormObj::GetVerticalPosition()
{
	return m_position.top;
}

void CFormObj::SetVerticalPosition(float fNewValue)
{
	if (m_position.top == fNewValue)
		return;
	
	CRectF rect = m_position;
	rect.OffsetRect(0, fNewValue - m_position.top);
	MoveTo(rect, FALSE);
}

float CFormObj::GetWidth()
{
	return m_position.Width();
}

void CFormObj::SetWidth(float fNewValue)
{
	ASSERT(m_position.right >= m_position.left);
	if (fabs(m_position.Width() - fNewValue) < 0.001)
		return;
	
	if (ABSOLUTEVALUE(fNewValue) < 0.1f)
		fNewValue = fNewValue >= 0 ? 0.1f : -0.1f;
	
	CRectF rect = m_position;
	rect.right = m_position.left + fNewValue;
	if (m_bUniformScale)
	{
		float fScaleX = rect.Width() / m_position.Width();
		rect.bottom = rect.top + rect.Height() * fScaleX;
	}
	MoveTo(rect, FALSE);
}

float CFormObj::GetHeight()
{
	return m_position.Height();
}

void CFormObj::SetHeight(float fNewValue)
{
	ASSERT(m_position.bottom >= m_position.top);
	if (fabs(m_position.Height() - fNewValue) < 0.001)
		return;
	
	if (ABSOLUTEVALUE(fNewValue) < 0.1f)
		fNewValue = fNewValue >= 0 ? 0.1f : -0.1f;
	
	CRectF rect = m_position;
	rect.bottom = m_position.top + fNewValue;
	if (m_bUniformScale)
	{
		float fScaleY = rect.Height() / m_position.Height();
		rect.right = rect.left + rect.Width() * fScaleY;
	}
	MoveTo(rect, FALSE);
}

OLE_COLOR CFormObj::GetForeColor()
{
	ASSERT(m_pFormDoc != NULL);

	return m_pFormDoc->m_clrForeColor;
}

void CFormObj::SetForeColor(OLE_COLOR nNewValue)
{
	ASSERT(m_pFormDoc != NULL);

	m_pFormDoc->m_clrForeColor = nNewValue;
	m_pFormDoc->UpdateAllViews(NULL);
}

OLE_COLOR CFormObj::GetBackColor()
{
	ASSERT(m_pFormDoc != NULL);

	return m_pFormDoc->m_clrBackColor;
}

void CFormObj::SetBackColor(OLE_COLOR nNewValue)
{
	ASSERT(m_pFormDoc != NULL);

	m_pFormDoc->m_clrBackColor = nNewValue;
	m_pFormDoc->UpdateAllViews(NULL);
}

BSTR CFormObj::GetBrushImageFile()
{
	ASSERT(m_pFormDoc != NULL);
	
	return m_pFormDoc->m_strBrushImageFile.AllocSysString();
}

void CFormObj::SetBrushImageFile(LPCTSTR lpszNewValue)
{
	ASSERT(m_pFormDoc != NULL);
	
	m_pFormDoc->m_strBrushImageFile = lpszNewValue;
	m_pFormDoc->OnBrushImageFileChanged();	
}

void CFormObj::OnTransparencyChanged()
{
	ASSERT(m_pFormDoc != NULL);
	
	m_pFormDoc->UpdateAllViews(NULL);
}

int CFormObj::GetVisibleLayer()
{
	ASSERT(m_pFormDoc != NULL);

	return m_pFormDoc->m_ulVisibleLayer;
}

void CFormObj::SetVisibleLayer(int iNewValue)
{
	ASSERT(m_pFormDoc != NULL);

	m_pFormDoc->m_ulVisibleLayer = iNewValue;
	m_pFormDoc->UpdateAllViews(NULL);
}

void CFormObj::FireEventV(DISPID dispid, BYTE* pbParams,
	va_list argList)
{
	COleDispatchDriver driver;

	POSITION pos = m_xFormObjectCP.GetStartPosition();
	LPDISPATCH pDispatch;
	while (pos != NULL)
	{
		pDispatch = (LPDISPATCH)m_xFormObjectCP.GetNextConnection(pos);
		ASSERT(pDispatch != NULL);
		driver.AttachDispatch(pDispatch, FALSE);
		TRY
			driver.InvokeHelperV(dispid, DISPATCH_METHOD, VT_EMPTY, NULL,
				pbParams, argList);
		END_TRY
		driver.DetachDispatch();
	}
}

void AFX_CDECL CFormObj::FireEvent(DISPID dispid, BYTE* pbParams, ...)
{
	va_list argList;
	va_start(argList, pbParams);
	FireEventV(dispid, pbParams, argList);
	va_end(argList);
}

inline void CFormObj::FireEvent(DISPID dispid)
{
	FireEvent(dispid, EVENT_PARAM(VTS_NONE));
}

////////////////////////////////////////////////////////////////////////
//功 能：组对象处理消息
//参 数：2、如果是顶级组合对象，则有CRunView调用，参数为true，
//否则为此函数嵌套调用，参数为false
//描 述：要注意处理组对象中嵌套子对象的消息（因为可能连接鼠标输入动态对象）
BOOL CFormObj::OnWindowMessage(const MSG* pMsg, BOOL bSubObj)
{
	CPoint point; 
	BOOL bSubObjHandled = FALSE;
	CRunView* pView = (CRunView *)m_pDocument->GetRunView();
	GetCursorPos(&point);
	pView->ScreenToClient(&point);
	pView->ClientToDoc(point);
	//先有点中的子对象处理可能连接的鼠标输入动态属性

	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetTailPosition();
	CDevObj* pSubObj = NULL;
	CDevObj* pHitSubObj = NULL;

	if ((pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST) && 
		(pView->m_pObjectWithCapture == this && m_pObjectWithCapture != NULL))
	{
		pHitSubObj = m_pObjectWithCapture;
	}
	else
	{
		while (posObj)
		{
			pSubObj = (CDevObj*)pObjects->GetPrev(posObj);
			if (pSubObj->HitTest(point, pView))
			{
				pHitSubObj = pSubObj;
				break;
			}
		}
	}

	if (pHitSubObj != NULL)
		bSubObjHandled = pHitSubObj->OnWindowMessage(pMsg, TRUE);
	
	//如果是子对象，或者子对象已经处理消息
	if (bSubObjHandled /*|| bSubObj*/)
		return TRUE;

	if (((CRunView *)GetRunView())->IsCancelBubble())
		return TRUE;

	switch (pMsg->message)
	{
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			if (pHitSubObj != NULL && pView->m_pObjectWithCapture == pHitSubObj)
				m_pObjectWithCapture = pHitSubObj;
			
			pView->OnWindowlessSetCapture(this);
			
			FireEvent(CXEVENID_MOUSEDOWN);
			if (pMsg->message == WM_LBUTTONDOWN && pHitSubObj == NULL)
				pView->OnWindowlessSetFocus(this);
			break;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			pView->OnWindowlessReleaseCapture(this);
			m_pObjectWithCapture = NULL;

			FireEvent(CXEVENID_MOUSEUP);
			if (HitTest(point, pView))
				FireEvent(CXEVENID_CLICK);
			break;
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			if (pHitSubObj == NULL)
				pView->OnWindowlessSetCapture(this);
			FireEvent(CXEVENID_DBLCLICK);
			break;
		case WM_MOUSEMOVE:
			FireEvent(CXEVENID_MOUSEMOVE);
			// 2011.4.27 注释下面一行
			// if (HitTest(point, pView) && pHitSubObj == NULL)
				pView->TrackObjectMouseLeave(this);
			if (pHitSubObj == NULL || (pHitSubObj != NULL && pHitSubObj->IsDefaultMousePointer()))
				CRunView::SetStandardCursor(GetMousePointer());
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			FireEvent(CXEVENID_KEYDOWN);
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			FireEvent(CXEVENID_KEYUP);
			break;
		case WM_CHAR:
			FireEvent(CXEVENID_KEYPRESS); //没有处理双字节的情形，参看CRunView::OnChar
			break;
		default:
			return FALSE;
	}

//	BOOL bMouseInput = ProcessMouseInput(pMsg->message, pMsg->wParam, pMsg->lParam);
		//如果是子对象，并且没有处理鼠标输入，返回FALSE;
//	return bSubObj && !bMouseInput ? FALSE : TRUE;

	return bSubObj ? FALSE : TRUE;
}

void CFormObj::OnMouseLeave()
{
	FireEvent(CXEVENID_MOUSELEAVE);
}

BOOL CFormObj::IsDefaultMousePointer()
{
	return GetMousePointer() == MousePointerDefault;
}

