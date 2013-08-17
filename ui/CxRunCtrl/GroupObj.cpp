// GroupItem.cpp: implementation of the CGroupObj class.
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

IMPLEMENT_SERIAL(CGroupObj, CDevObj, 0)

CGroupObj::CGroupObj(CDevObjDoc* pContainerDoc) :
	CDevObj(pContainerDoc)
{
	EnableAutomation();
	EnableConnections();
	//
	AfxOleLockApp();

	m_pFillBoundRect = NULL;
}

CGroupObj::~CGroupObj()
{
	AfxOleUnlockApp();
}

void CGroupObj::RemoveAllObjects()
{
	m_arrSubObj.RemoveAll();
}

BOOL CGroupObj::CreateOrLoad(LPCTSTR pszName,
   REFIID iidPersistanceMedium, IUnknown* pPersistanceMedium)
{
	m_pDocument->CreateUniqueObjectName(this, "GroupObject",
				m_strDisplayName);
	m_strClassName = "GroupObject";

	return TRUE;
}

void CGroupObj::DrawFrontToBack(CDC* pDC)
{
	for (int i = 0; i < m_arrSubObj.GetSize(); i++)
	{	
		m_arrSubObj[i]->DrawFrontToBack(pDC);
	}
}

void CGroupObj::Draw(CDC* pDC)
{
	if (m_bVisible)
	{
		for(int i = 0; i < m_arrSubObj.GetSize(); i++)
		{
			m_arrSubObj[i]->Draw(pDC);
		}
	}
}

void CGroupObj::Print(CDC* pDC)
{
	if (m_bVisible)
	{
		for(int i = 0; i < m_arrSubObj.GetSize(); i++)
		{
			m_arrSubObj[i]->Print(pDC);
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//功 能：当最后引用被释放，删除对象
//描 述：与CCmdTarget::OnFinalRelease()实现不一样，如调用CCmdTarget::OnFinalRelease()会出错
/////////////////////////////////////////////////////////////////////////
void CGroupObj::OnFinalRelease()
{
	// When the last reference for an automation object is released
	//  OnFinalRelease is called.  This implementation deletes the
	//  object.  Add additional cleanup required for your object before
	//  deleting it from memory.

	delete this;
}

BEGIN_MESSAGE_MAP(CGroupObj, CDevObj)
	//{{AFX_MSG_MAP(CGroupObj)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_CONNECTION_MAP(CGroupObj, CDevObj)
   CONNECTION_PART(CGroupObj, __uuidof(IGroupObjectEvents), GroupObjectCP)
END_CONNECTION_MAP()

BEGIN_DISPATCH_MAP(CGroupObj, CDevObj)
	//{{AFX_DISPATCH_MAP(CGroupObj)
	DISP_PROPERTY_EX_ID(CGroupObj, "HorizontalPosition", CXPROPID_HorPosition, GetHorizontalPosition, SetHorizontalPosition, VT_R4)
	DISP_PROPERTY_EX_ID(CGroupObj, "VerticalPosition", CXPROPID_VerPosition, GetVerticalPosition, SetVerticalPosition, VT_R4)
	DISP_PROPERTY_EX_ID(CGroupObj, "Width", CXPROPID_Width, GetWidth, SetWidth, VT_R4)
	DISP_PROPERTY_EX_ID(CGroupObj, "Height", CXPROPID_Height, GetHeight, SetHeight, VT_R4)
	DISP_PROPERTY_EX_ID(CGroupObj, "RoateBasePointX", CXPROPID_RoateBasePointX, GetRoateBasePointX, SetRoateBasePointX, VT_R4)
	DISP_PROPERTY_EX_ID(CGroupObj, "RoateBasePointY", CXPROPID_RoateBasePointY, GetRoateBasePointY, SetRoateBasePointY, VT_R4)
	DISP_PROPERTY_EX_ID(CGroupObj, "RotateAngle", CXPROPID_RotateAngle, GetRotateAngle, SetRotateAngle, VT_R4)
	DISP_PROPERTY_EX_ID(CGroupObj, "ScaleBasePoint", CXPROPID_ScaleBasePoint, GetScaleBasePoint, SetScaleBasePoint, VT_I4)
	DISP_PROPERTY_EX_ID(CGroupObj, "HorizontalScale", CXPROPID_HorScale, GetHorizontalScale, SetHorizontalScale, VT_R4)
	DISP_PROPERTY_EX_ID(CGroupObj, "VerticalScale", CXPROPID_VerScale, GetVerticalScale, SetVerticalScale, VT_R4)
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "UniformScale", CXPROPID_UniformScale, m_bUniformScale, OnUniformScaleChanged, VT_BOOL)
	DISP_PROPERTY_EX_ID(CGroupObj, "Layer", CXPROPID_Layer, GetLayer, SetLayer, VT_I4)
	DISP_PROPERTY_EX_ID(CGroupObj, "Locked", CXPROPID_Locked, GetLocked, SetLocked, VT_BOOL)
	DISP_PROPERTY_EX_ID(CGroupObj, "Visible", CXPROPID_Visible, GetVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "ToolTipText", CXPROPID_ToolTipText, m_strToolTip, OnToolTipTextChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "Description", CXPROPID_Description, m_strDescription, OnDescriptionChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "MousePointer", CXPROPID_MousePointer, m_enumMousePointer, OnMousePointerChanged, VT_I4)
	DISP_PROPERTY_EX_ID(CGroupObj, "Privilege", CXPROPID_Privilege, GetPrivilege, SetPrivilege, VT_I4)
	
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "FillStyle", CXPROPID_FillStyle, m_enumFillStyle, OnFillStyleChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "HorizontalFillDirection", CXPROPID_HorFillDirection, m_enumHorFillDirection, OnHorizontalFillDirectionChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "VerticalFillDirection", CXPROPID_VerFillDirection, m_enumVerFillDirection, OnVerticalFillDirectionChanged, VT_I4)
	DISP_PROPERTY_EX_ID(CGroupObj, "HorizontalFillPercent", CXPROPID_HorFillPercent, GetHorizontalFillPercent, SetHorizontalFillPercent, VT_I4)
	DISP_PROPERTY_EX_ID(CGroupObj, "VerticalFillPercent", CXPROPID_VerFillPercent, GetVerticalFillPercent, SetVerticalFillPercent, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "BrushImageFileName", CXPROPID_BrushImageFileName, m_strBrushImageFileName, OnBrushImageFileNameChanged, VT_BSTR)
	
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "EdgeStyle", CXPROPID_EdgeStyle, m_enumEdgeStyle, OnEdgeStyleChanged, VT_I4)
	DISP_PROPERTY_EX_ID(CGroupObj, "EdgeWidth", CXPROPID_EdgeWidth, GetEdgeWidth, SetEdgeWidth, VT_R4)
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "LineJoinStyle", CXPROPID_LineJoinStyle, m_enumLineJoinStyle, OnLineJoinStyleChanged, VT_I4)
	
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "EdgeColor", CXPROPID_EdgeColor, m_clrEdgeColor, OnEdgeColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "HighLightEdgeColor", CXPROPID_HighLightEdgeColor, m_clrHighLightEdge, OnHighLightEdgeColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "FillColor", CXPROPID_FillColor, m_clrFillColor, OnFillColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "HighLightFillColor", CXPROPID_HighLightFillColor, m_clrHighLightFill, OnHighLightFillColorChanged, VT_COLOR)

	DISP_FUNCTION_ID(CGroupObj, "GetObjectByName", CXMETHID_GetObjectByName, GetObjectByName, VT_UNKNOWN, VTS_BSTR)
	DISP_PROPERTY_EX_ID(CGroupObj, "Objects", CXPROPID_Objects, GetObjectCollection, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CGroupObj, "Name", CXPROPID_Name, GetName, SetNotSupported, VT_BSTR)
	DISP_FUNCTION_ID(CGroupObj, "IsConnected", CXMETHID_IsConnected, IsConnected, VT_I2, VTS_BSTR)
	DISP_FUNCTION_ID(CGroupObj, "ConnectObject", CXMETHID_ConnectObject, ConnectObject, VT_EMPTY, VTS_BSTR VTS_UNKNOWN)
	DISP_FUNCTION_ID(CGroupObj, "ConnectDirect", CXMETHID_ConnectDirect, ConnectDirect, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CGroupObj, "GetConnectObject", CXMETHID_GetConnectObject, GetConnectObject, VT_UNKNOWN, VTS_BSTR)
	DISP_FUNCTION_ID(CGroupObj, "Disconnect", CXMETHID_Disconnect, Disconnect, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CGroupObj, "GetConnectState", CXMETHID_GetConnectState, GetConnectState, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CGroupObj, "GetConnectState", 0x80010006, GetConnectState, VT_I4, VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

//ACF61CEA-3805-11D5-A468-8E961CC7BB0B
//const IID LIBID_CXHMIOBJLib = {0xACF61CEA,0x3805,0x11D5,{0xA4,0x68,0x8E,0x96,0x1C,0xC7,0xBB,0x0B}};


UINT CGroupObj::GetTypeInfoCount()
{ 
	return 1; 
} 

HRESULT CGroupObj::GetTypeLib(LCID lcid, LPTYPELIB* ppTypeLib)
{ 
	return m_pDocument->GetTypeLib(lcid, ppTypeLib);
//	return ::LoadRegTypeLib(tlid, 1, 0, lcid, ppTypeLib); 
} 

CTypeLibCache* CGroupObj::GetTypeLibCache() 
{ 
	return m_pDocument->GetTypeLibCache();
//	AFX_MANAGE_STATE(m_pModuleState); 
//	return AfxGetTypeLibCache(&LIBID_CXHMIOBJLib); } 
}


//IMPLEMENT_OLETYPELIB(CGroupObj, LIBID_CXHMIOBJLib, 1, 0)

const IID IID_IGroupObject = 
{0x2BE0E00C,0x3871,0x4d3a,{0x96,0xF3,0x76,0xA8,0x06,0xC0,0x6D,0x3E}};

const IID IID_IGroupObject2 = 
{0x4D765B89,0x2E46,0x4cff,{0x94,0x15,0xF1,0x6C,0x41,0xE1,0x6C,0x0B}};

BEGIN_INTERFACE_MAP(CGroupObj, CDevObj)
	INTERFACE_PART(CGroupObj, IID_IGroupObject, Dispatch)
	INTERFACE_PART(CGroupObj, IID_IGroupObject2, Dispatch)
	INTERFACE_PART(CGroupObj, IID_IConnectionPointContainer, ConnPtContainer)
	INTERFACE_PART(CGroupObj, IID_IProvideClassInfo, ProvideClassInfo)
	INTERFACE_PART(CGroupObj, IID_IProvideClassInfo2, ProvideClassInfo)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupObj::XProvideClassInfo

STDMETHODIMP_(ULONG) CGroupObj::XProvideClassInfo::AddRef()
{
	METHOD_PROLOGUE_EX_(CGroupObj, ProvideClassInfo)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CGroupObj::XProvideClassInfo::Release()
{
	METHOD_PROLOGUE_EX_(CGroupObj, ProvideClassInfo)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CGroupObj::XProvideClassInfo::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CGroupObj, ProvideClassInfo)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CGroupObj::XProvideClassInfo::GetClassInfo(
	LPTYPEINFO* ppTypeInfo)
{
	METHOD_PROLOGUE_EX(CGroupObj, ProvideClassInfo)

//	return theApp.m_pExtTypeLib->GetTypeInfoOfGuid(__uuidof(GroupObject), ppTypeInfo);
	//使用下句存在内存泄露
	return pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), __uuidof(GroupObject), ppTypeInfo);
//	return pThis->m_pDocument->GetTypeInfoOfGuid(GetUserDefaultLCID(), __uuidof(GroupObject), ppTypeInfo);
}

STDMETHODIMP CGroupObj::XProvideClassInfo::GetGUID(DWORD dwGuidKind,
	GUID* pGUID)
{
	METHOD_PROLOGUE_EX_(CGroupObj, ProvideClassInfo)

	if (dwGuidKind == GUIDKIND_DEFAULT_SOURCE_DISP_IID)
	{
		*pGUID = __uuidof(IGroupObjectEvents);
		return NOERROR;
	}
	else
	{
		*pGUID = GUID_NULL;
		return E_INVALIDARG;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGroupObj message handlers

BOOL CGroupObj::GetDispatchIID(IID* piid)
{
	ASSERT( piid != NULL );

	*piid = __uuidof(IGroupObject2);

	return TRUE;
}

void CGroupObj::OnUniformScaleChanged()
{
}

void CGroupObj::OnMousePointerChanged()
{
}

int CGroupObj::GetPrivilege()
{
	return m_nPrivilege;
}

void CGroupObj::SetPrivilege(int iNewValue)
{
	if (iNewValue < 0 || iNewValue > 1000)
		return;
	
	m_nPrivilege = iNewValue;
}

void CGroupObj::OnToolTipTextChanged()
{
}

void CGroupObj::OnDescriptionChanged()
{
}

float CGroupObj::GetHorizontalPosition()
{
	return m_position.left;
}

void CGroupObj::SetHorizontalPosition(float fNewValue)
{
	if (m_position.left == fNewValue)
		return;

	CRectF rect = m_position;
	rect.OffsetRect(fNewValue - m_position.left, 0);
	MoveTo(rect, FALSE);
}

float CGroupObj::GetVerticalPosition()
{
	return m_position.top;
}

void CGroupObj::SetVerticalPosition(float fNewValue)
{
	if (m_position.top == fNewValue)
		return;

	CRectF rect = m_position;
	rect.OffsetRect(0, fNewValue - m_position.top);
	MoveTo(rect, FALSE);
}

float CGroupObj::GetWidth()
{
	return m_position.Width();
}

void CGroupObj::SetWidth(float fNewValue)
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

float CGroupObj::GetHeight()
{
	return m_position.Height();
}

void CGroupObj::SetHeight(float fNewValue)
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

float CGroupObj::GetRoateBasePointX()
{
	return m_ptRotateBase.x;
}

void CGroupObj::SetRoateBasePointX(float fNewValue)
{
	m_ptRotateBase.x = fNewValue;
}

float CGroupObj::GetRoateBasePointY()
{
	return m_ptRotateBase.y;
}

void CGroupObj::SetRoateBasePointY(float fNewValue)
{
	m_ptRotateBase.y = fNewValue;
}

float CGroupObj::GetRotateAngle()
{
	return m_fRotateAngle;
}

void CGroupObj::SetRotateAngle(float fNewValue)
{
	if (m_fRotateAngle == fNewValue)
		return;

	RotateAt(m_ptRotateBase, fNewValue - m_fRotateAngle);		
	m_fRotateAngle = fNewValue;
}

BSTR CGroupObj::GetName()
{
	CString strResult;
	strResult = m_strDisplayName;

	return strResult.AllocSysString();
}

enumScaleBasePoint CGroupObj::GetScaleBasePoint()
{
	return m_enumScaleBase;
}

void CGroupObj::SetScaleBasePoint(enumScaleBasePoint fNewValue)
{
	m_enumScaleBase = fNewValue;
}

float CGroupObj::GetHorizontalScale()
{
	return m_fHorScale;
}

void CGroupObj::SetHorizontalScale(float fNewValue)
{
	if (fNewValue == m_fHorScale)
		return;

	if (fNewValue == 0)
		fNewValue = 1 / m_position.Width();
	float fHorScale = fNewValue / m_fHorScale;		
	m_fHorScale = fNewValue;
	ScaleAt(GetScaleBasePointF(), fHorScale, 1);
}

float CGroupObj::GetVerticalScale()
{
	return m_fVerScale;
}

void CGroupObj::SetVerticalScale(float fNewValue)
{	
	if (fNewValue == m_fVerScale)
		return;

	if (fNewValue == 0)
		fNewValue = 1 / m_position.Height();
	float fVerScale = fNewValue / m_fVerScale;		
	m_fVerScale = fNewValue;
	ScaleAt(GetScaleBasePointF(), 1, fVerScale);
}

CPointF CGroupObj::GetScaleBasePointF()
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

void CGroupObj::OnFillStyleChanged()
{
	CComVariant var = m_enumFillStyle;
	PutProperty(CXPROPID_FillStyle, &var);
}

void CGroupObj::OnHorizontalFillDirectionChanged()
{
	RecalcFillBoundRect();
	Invalidate();
}

void CGroupObj::OnVerticalFillDirectionChanged()
{
	RecalcFillBoundRect();
	Invalidate();
}

int CGroupObj::GetHorizontalFillPercent()
{
	return m_nHorFillPercent;
}

void CGroupObj::SetHorizontalFillPercent(int iNewValue)
{
	if ((iNewValue > 100) || (iNewValue < 0))
		return;
	if (m_nHorFillPercent == iNewValue)
		return;
	
	m_nHorFillPercent = iNewValue;
	
	RecalcFillBoundRect();
	Invalidate();
}

int CGroupObj::GetVerticalFillPercent()
{
	return m_nVerFillPercent;
}

void CGroupObj::SetVerticalFillPercent(int iNewValue)
{
	if ((iNewValue > 100) || (iNewValue < 0))
		return;
	if (m_nVerFillPercent == iNewValue)
		return;
	
	m_nVerFillPercent = iNewValue;
	
	RecalcFillBoundRect();
	Invalidate();
}

void CGroupObj::OnBrushImageFileNameChanged()
{
	CComVariant var = m_strBrushImageFileName;
	PutProperty(CXPROPID_BrushImageFileName, &var);
}

void CGroupObj::OnEdgeStyleChanged()
{
	CComVariant var = m_enumEdgeStyle;
	PutProperty(CXPROPID_EdgeStyle, &var);
}

float CGroupObj::GetEdgeWidth()
{
	return m_fEdgeWidth;
}

void CGroupObj::SetEdgeWidth(float fNewValue)
{
	if ((fNewValue > 60) || (fNewValue < 1))
		return;
	if (m_fEdgeWidth == fNewValue)
		return;
	
	m_fEdgeWidth = fNewValue;
	
	CComVariant var = m_fEdgeWidth;
	PutProperty(CXPROPID_EdgeWidth, &var);
}

void CGroupObj::OnLineJoinStyleChanged()
{
	CComVariant var = m_enumLineJoinStyle;
	PutProperty(CXPROPID_LineJoinStyle, &var);
}

void CGroupObj::OnEdgeColorChanged()
{
	CComVariant var = (long)m_clrEdgeColor;
	PutProperty(CXPROPID_EdgeColor, &var);
}

void CGroupObj::OnHighLightEdgeColorChanged()
{
	CComVariant var = (long)m_clrHighLightEdge;
	PutProperty(CXPROPID_HighLightEdgeColor, &var);
}

void CGroupObj::OnFillColorChanged()
{
	CComVariant var = (long)m_clrFillColor;
	PutProperty(CXPROPID_FillColor, &var);
}

void CGroupObj::OnHighLightFillColorChanged()
{
	CComVariant var = (long)m_clrHighLightFill;
	PutProperty(CXPROPID_HighLightFillColor, &var);
}

HRESULT CGroupObj::PutProperty(DISPID dwDispID, VARIANT* pVar)
{
	int nCount = m_arrSubObj.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CDevObj* pObj = m_arrSubObj[i];
		pObj->PutProperty(dwDispID, pVar);
	}
	
	return S_OK;
}

int CGroupObj::GetLayer()
{
	return m_nLayer + 1;
}

void CGroupObj::SetLayer(int lLayer)
{
	if (lLayer > 30 || lLayer < 1)
		return;

	if (m_nLayer == lLayer - 1)
		return;
	m_nLayer = lLayer - 1;
	Invalidate();
}

BOOL CGroupObj::GetLocked()
{
	return m_bLocked;
}

void CGroupObj::SetLocked(BOOL bNewVal)
{
	if (m_bLocked == bNewVal)
		return;
	
	m_bLocked = bNewVal;
	
	Invalidate();
}

BOOL CGroupObj::GetVisible()
{
	return m_bVisible;
}

void CGroupObj::SetVisible(BOOL bNewVal)
{
	if (m_bVisible == bNewVal)
		return;
	
	m_bVisible = bNewVal;

	Invalidate();
}

VARIANT_BOOL CGroupObj::IsConnected(LPCTSTR pszPropName)
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
		ASSERT(FALSE);
	}

	return bHasConnection;
}

void CGroupObj::ConnectObject(LPCTSTR pszPropName, LPUNKNOWN punkObject)
{
	USES_CONVERSION;

	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
	if(SUCCEEDED(((IDispatch *)(&m_xDispatch))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		AddDynamic(dispid, punkObject);
	}
	else
	{
		ASSERT(FALSE);
	}
}

void CGroupObj::ConnectDirect(LPCTSTR pszPropName, LPCTSTR pszDataSource)
{
}

LPUNKNOWN CGroupObj::GetConnectObject(LPCTSTR pszPropName)
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
		ASSERT(FALSE);
	}

	return punkObject;
}

void CGroupObj::Disconnect(LPCTSTR pszPropName)
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
		ASSERT(FALSE);
	}
}

int CGroupObj::GetConnectState(DISPID dispid)
{
	int nState = 0;
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

	return nState;
}

LPUNKNOWN CGroupObj::GetObjectByName(LPCTSTR lpszName)
{
	LPUNKNOWN lpUnknown = NULL;

	for (int i = 0; i < m_arrSubObj.GetSize(); i++)
	{
		CDevObj* pObj = m_arrSubObj[i];
		if (pObj->GetDisplayName().CompareNoCase(lpszName) == 0)
		{
			pObj->GetObjectUnknown()->QueryInterface(IID_IUnknown, (void**)&lpUnknown);
			break;
		}
	}

	return lpUnknown;
}

LPDISPATCH CGroupObj::GetObjectCollection()
{
	CObjectCollection* pObjCol = new CObjectCollection;

	for (int i = 0; i < m_arrSubObj.GetSize(); i++)
	{
		CDevObj* pObj = m_arrSubObj[i];

		IDispatchPtr pDispatch = NULL;
		pObj->GetObjectUnknown()->QueryInterface(IID_IDispatch, (void**)&pDispatch);
		
		CComVariant var = (IDispatch *)pDispatch;
		pObjCol->m_arrObjects.Add(var);
	}

	return (IDispatch *)pObjCol->GetInterface(&IID_IDispatch);
}

////////////////////////////////////////////////////////////////////
//功 能：重新计算位置
//描 述：原来使用CRect::UnionRect出问题 
////////////////////////////////////////////////////////////////////
void CGroupObj::RecalcPosition()
{
	CDevObj* pObj;
	CRectF rcObj;
	float fLeft;
	float fTop;
	float fRight;
	float fBottom;

	int nCount = m_arrSubObj.GetSize();
	ASSERT(nCount > 0);
	pObj = m_arrSubObj[0];
	rcObj = pObj->GetPositionRect();
	fLeft = rcObj.left;
	fTop = rcObj.top;
	fRight = rcObj.right;
	fBottom = rcObj.bottom;

	for(int i = 0; i < m_arrSubObj.GetSize(); i++)
	{
		pObj = m_arrSubObj[i];
		rcObj = pObj->GetPositionRect();
		if (fLeft > rcObj.left)
			fLeft = rcObj.left;
		if (fTop > rcObj.top)
			fTop = rcObj.top;
		if (fRight < rcObj.right)
			fRight = rcObj.right;
		if (fBottom < rcObj.bottom)
			fBottom = rcObj.bottom;
	}
	m_position.SetRect(fLeft, fTop, fRight, fBottom);

	RecalcFillBoundRect();
}

BOOL CGroupObj::HitTest(CPoint point, const CRunView* pView)
{
	if (m_bVisible)
	{
		int nCount = m_arrSubObj.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			if (m_arrSubObj[i]->HitTest(point, pView))
				return TRUE;
		}
	}

	return FALSE;
}

CRect CGroupObj::GetBoundingRect()
{
	CDevObj* pObj;
	CRect rcObj;
	long lLeft;
	long lTop;
	long lRight;
	long lBottom;

	int nCount = m_arrSubObj.GetSize();
	ASSERT(nCount > 0);
	pObj = m_arrSubObj[0];
	rcObj = pObj->GetBoundingRect();
	lLeft = rcObj.left;
	lTop = rcObj.top;
	lRight = rcObj.right;
	lBottom = rcObj.bottom;

	for(int i = 1; i < nCount; i++)
	{
		pObj = m_arrSubObj[i];
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

void CGroupObj::MoveTo(const CRectF &position, BOOL bUniformScale, CRunView* pView)
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

	for (int i = 0; i < m_arrSubObj.GetSize(); i++)
	{
		rcObj = m_arrSubObj[i]->GetPositionRect();
		rcObj.OffsetRect(- m_position.TopLeft());
		rcObj.left = rcObj.left * fScaleX;
		rcObj.right = rcObj.right * fScaleX;
		rcObj.top = rcObj.top * fScaleY;
		rcObj.bottom = rcObj.bottom * fScaleY;
		rcObj.OffsetRect(position.TopLeft());
		m_arrSubObj[i]->MoveTo(rcObj, bUniformScale, pView);
	}
	
	m_ptRotateBase.x = position.left + (m_ptRotateBase.x - m_position.left) * fScaleX;
	m_ptRotateBase.y = position.top + (m_ptRotateBase.y - m_position.top) * fScaleY;

	RecalcPosition();
	Invalidate(pView);
}

void CGroupObj::RotateAt(CPointF point, float fAngle, CRunView* pView)
{
	ASSERT_VALID(this);

	Invalidate(pView);

	for (int i = 0; i < m_arrSubObj.GetSize(); i++)
	{
		m_arrSubObj[i]->RotateAt(point, fAngle);
	}

	m_ptRotateBase = CalculateRotatePoint(point, m_ptRotateBase, fAngle);

	RecalcPosition();
	Invalidate(pView);
}

void CGroupObj::ScaleAt(CPointF point, float fHorScale, float fVerScale, CRunView* pView)
{
	ASSERT_VALID(this);

	Invalidate(pView);

	for (int i = 0; i < m_arrSubObj.GetSize(); i++)
	{
		m_arrSubObj[i]->ScaleAt(point, fHorScale, fVerScale);
	}

	m_ptRotateBase -= point;
	m_ptRotateBase.x *= fHorScale; 
	m_ptRotateBase.y *= fVerScale; 
	m_ptRotateBase += point;

	RecalcPosition();
	Invalidate(pView);
}

////////////////////////////////////////////////////////////////
//功能：释放对象
//描述：先要释放所有子对象
////////////////////////////////////////////////////////////////
void CGroupObj::Release()
{
	for(int i = 0; i < m_arrSubObj.GetSize(); i++)
	{
		m_arrSubObj[i]->Release();
		delete m_arrSubObj[i];
	}
	m_arrSubObj.RemoveAll();

	if (m_pFillBoundRect != NULL)
	{
		delete m_pFillBoundRect;
		m_pFillBoundRect = NULL;
	}

	CDevObj::Release();
}

void CGroupObj::InitRun()
{
	for(int i = 0; i < m_arrSubObj.GetSize(); i++)
		m_arrSubObj[i]->InitRun();

	CDevObj::InitRun();
}

void CGroupObj::Serialize(CArchive& ar)
{
	ASSERT(ar.IsLoading());

	CDevObj::Serialize(ar);

	m_arrSubObj.Serialize(ar);
	for(int i = 0; i < m_arrSubObj.GetSize(); i++)
	{
		m_arrSubObj[i]->m_pGroupObj = this;
	}
	
	ar >> m_position;
	ar >> m_ptRotateBase;
	long lTemp;
	ar >> lTemp;
	m_enumScaleBase = enumScaleBasePoint(lTemp);
	ar >> m_bUniformScale;
	ar >> lTemp;
	m_enumMousePointer = enumMousePointer(lTemp);
	ar >> m_fHorScale;							
	ar >> m_fVerScale;							
	ar >> m_fRotateAngle; 
	
	if (m_dwVer > _SUPERCX_VER_0204)
	{
		ar >> m_clrEdgeColor;
		ar >> m_clrHighLightEdge;
		ar >> lTemp;
		m_enumEdgeStyle = enumEdgeStyle(lTemp);	
		ar >> lTemp;
		m_enumLineJoinStyle = enumLineJoinStyle(lTemp);	
		ar >> m_fEdgeWidth;	
		
		ar >> m_clrFillColor;
		ar >> m_clrHighLightFill;
		ar >> lTemp;
		m_enumFillStyle = enumFillStyle(lTemp);
		ar >> m_strBrushImageFileName;
		
		ar >> lTemp;
		m_enumHorFillDirection = enumHorizontalDirection(lTemp);
		ar >> lTemp;
		m_enumVerFillDirection = enumVerticalDirection(lTemp);
		ar >> m_nHorFillPercent;
		ar >> m_nVerFillPercent;
		
		BOOL b;
		ar >> b;
		if (b)
		{
			if (m_pFillBoundRect == NULL)
				m_pFillBoundRect = new CRect;
			ar >> *m_pFillBoundRect;
		}
	}
	
	SerializeDynamicProperty(ar);
}

IDispatch* CGroupObj::GetObjectDispatch()
{
	return GetIDispatch(FALSE);
}

LPUNKNOWN CGroupObj::GetObjectUnknown()
{
	return GetInterface(&IID_IUnknown);
}

void CGroupObj::FireEventV(DISPID dispid, BYTE* pbParams,
	va_list argList)
{
	COleDispatchDriver driver;

	POSITION pos = m_xGroupObjectCP.GetStartPosition();
	LPDISPATCH pDispatch;
	while (pos != NULL)
	{
		pDispatch = (LPDISPATCH)m_xGroupObjectCP.GetNextConnection(pos);
		ASSERT(pDispatch != NULL);
		driver.AttachDispatch(pDispatch, FALSE);
		TRY
			driver.InvokeHelperV(dispid, DISPATCH_METHOD, VT_EMPTY, NULL,
				pbParams, argList);
		END_TRY
		driver.DetachDispatch();
	}
}

void AFX_CDECL CGroupObj::FireEvent(DISPID dispid, BYTE* pbParams, ...)
{
	va_list argList;
	va_start(argList, pbParams);
	FireEventV(dispid, pbParams, argList);
	va_end(argList);
}

inline void CGroupObj::FireEvent(DISPID dispid)
{
	FireEvent(dispid, EVENT_PARAM(VTS_NONE));
}

inline ICxDynamicObject* CGroupObj::GetMouseInputynamic(DISPID dispid)
{
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		if (m_arLinkInfo[i].dispid == dispid && m_arLinkInfo[i].pDynamicObject && 
				m_arLinkInfo[i].pDynamicObject->IsEnableMouseInput() == S_OK)
			return m_arLinkInfo[i].pDynamicObject;
	}

	return NULL;
}

inline BOOL CGroupObj::HasMouseInputDynamic()
{
	DISPID dispid;
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		dispid = m_arLinkInfo[i].dispid;
		if ((dispid == CXPROPID_HorPosition || dispid == CXPROPID_VerPosition || dispid == CXPROPID_RotateAngle) &&
				m_arLinkInfo[i].pDynamicObject->IsEnableMouseInput() == S_OK)
			return TRUE;
	}

	if (OnMouseInput(_T("HorizontalPosition"), FALSE) ||
		OnMouseInput(_T("VerticalPosition"), FALSE)  ||
		OnMouseInput(_T("RotateAngle"), FALSE))
			return TRUE;

	return FALSE;
}

inline BOOL CGroupObj::ProcessMouseInput(UINT msg, WPARAM wParam, LPARAM lParam)
{
	static POINTF ptDragOffset; //相对于对象左上角偏移
	static float fAngleOrgin; //原始旋转角度
	static float fAngleStart; //开始旋转角度
	static POINTF ptOrgin;	//旋转原点
	static BOOL bMouseInput = FALSE;

	switch (msg)
	{
	case WM_MOUSEMOVE:
		{
			if (!bMouseInput)
				break;

			POINTF point;
			point.x = (float)GET_X_LPARAM(lParam);
			point.y = (float)GET_Y_LPARAM(lParam);
			CRectF rcPosition = GetPositionRect();
			
			ICxDynamicObject* pDyn = GetMouseInputynamic(CXPROPID_HorPosition);
			BOOL bProcess = OnMouseInput(_T("HorizontalPosition"), FALSE);
			BOOL bHandled = FALSE;
			if (pDyn || bProcess)
			{
				rcPosition.right = point.x - ptDragOffset.x + rcPosition.right - rcPosition.left;
				rcPosition.left = point.x - ptDragOffset.x;
				MoveTo(rcPosition, FALSE);
				if (pDyn)
					pDyn->InvertParseConnection();
				if (bProcess)
					OnMouseInput(_T("HorizontalPosition"), TRUE);
				
				bHandled = TRUE;
			}
			pDyn = GetMouseInputynamic(CXPROPID_VerPosition);
			bProcess = OnMouseInput(_T("VerticalPosition"), FALSE);
			if (pDyn || bProcess)
			{
				rcPosition.bottom = point.y - ptDragOffset.y + rcPosition.bottom - rcPosition.top;
				rcPosition.top = point.y - ptDragOffset.y;
				MoveTo(rcPosition, FALSE);
				if (pDyn)
					pDyn->InvertParseConnection();
				if (bProcess)
					OnMouseInput(_T("VerticalPosition"), TRUE);

				bHandled = TRUE;
			}
			if (bHandled)
			{
				return TRUE;
			}
					
			pDyn = GetMouseInputynamic(CXPROPID_RotateAngle);
			bProcess = OnMouseInput(_T("RotateAngle"), FALSE);
			if (pDyn || bProcess)
			{
				float fRotateAngle = CalculateAngle(ptOrgin, point);
				fRotateAngle = fAngleStart - fRotateAngle + fAngleOrgin;
				fRotateAngle = float(long(fRotateAngle) % 360);
				if (fRotateAngle < 0)
					fRotateAngle += 360;
				ATLASSERT(0 <= fRotateAngle && 360 >= fRotateAngle);
				if (fRotateAngle > 180)
					fRotateAngle -= 360;
				SetRotateAngle(fRotateAngle);
		//		ReSetObjectPath();
		//		FireViewChange();
				if (pDyn)
					pDyn->InvertParseConnection();
				if (bProcess)
					OnMouseInput(_T("RotateAngle"), TRUE);
				
				return TRUE;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if (!HasMouseInputDynamic())
				return FALSE;

			POINTF point;
			point.x = (float)GET_X_LPARAM(lParam);
			point.y = (float)GET_Y_LPARAM(lParam);
		
			CRectF rcPosition = GetPositionRect();
			ptDragOffset.x = point.x - rcPosition.left;
			ptDragOffset.y = point.y - rcPosition.top;

			fAngleOrgin = m_fRotateAngle;
			ptOrgin.x = m_ptRotateBase.x;
			ptOrgin.y = m_ptRotateBase.y;
			fAngleStart = CalculateAngle(ptOrgin, point);

			bMouseInput = TRUE;
			return TRUE;
		}
		break;
	case WM_LBUTTONUP:
		{
			bMouseInput = FALSE;
		}
	}

	return FALSE;
}

BOOL CGroupObj::IsDefaultMousePointer()
{
	return m_enumMousePointer == MousePointerDefault;
}

////////////////////////////////////////////////////////////////////////
//功 能：组对象处理消息
//参 数：2、如果是顶级组合对象，则有CRunView调用，参数为true，
//否则为此函数嵌套调用，参数为false
//描 述：要注意处理组对象中嵌套子对象的消息（因为可能连接鼠标输入动态对象）
BOOL CGroupObj::OnWindowMessage(const MSG* pMsg, BOOL bSubObj)
{
	CPoint point; 
	BOOL bSubObjHandled = FALSE;
	CRunView* pView = (CRunView *)m_pDocument->GetRunView();
	GetCursorPos(&point);
	pView->ScreenToClient(&point);
	pView->ClientToDoc(point);
	//先有点中的子对象处理可能连接的鼠标输入动态属性
	for (int i = m_arrSubObj.GetSize() - 1; i >= 0 ; i--)
	{
		if (m_arrSubObj[i]->HitTest(point, pView))
		{
			bSubObjHandled = m_arrSubObj[i]->OnWindowMessage(pMsg, TRUE);
			break;
		}
	}
	
	//如果是子对象，或者子对象已经处理消息
	if (bSubObjHandled /*|| bSubObj*/)
		return TRUE;

	switch (pMsg->message)
	{
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			pView->OnWindowlessSetCapture(this);
			FireEvent(CXEVENID_MOUSEDOWN);
			if (pMsg->message == WM_LBUTTONDOWN)
				pView->OnWindowlessSetFocus(this);
			break;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			pView->OnWindowlessReleaseCapture(this);
			FireEvent(CXEVENID_MOUSEUP);
			if (HitTest(point, pView))
				FireEvent(CXEVENID_CLICK);
			break;
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			pView->OnWindowlessSetCapture(this);
			FireEvent(CXEVENID_DBLCLICK);
			break;
		case WM_MOUSEMOVE:
			FireEvent(CXEVENID_MOUSEMOVE);
			if (HitTest(point, pView))
				pView->TrackObjectMouseLeave(this);
			CRunView::SetStandardCursor(m_enumMousePointer);
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

	BOOL bMouseInput = ProcessMouseInput(pMsg->message, pMsg->wParam, pMsg->lParam);
		//如果是子对象，并且没有处理鼠标输入，返回FALSE;
	return bSubObj && !bMouseInput ? FALSE : TRUE;
	
//	return TRUE;
}

void CGroupObj::OnMouseLeave()
{
	FireEvent(CXEVENID_MOUSELEAVE);
}

void CGroupObj::RecalcFillBoundRect()
{
	if (m_nVerFillPercent == 100 && m_nHorFillPercent == 100)
	{
		if (m_pFillBoundRect != NULL)
		{
			delete m_pFillBoundRect;
			m_pFillBoundRect = NULL;
		}
	}
	else
	{
		if (m_pFillBoundRect == NULL)
			m_pFillBoundRect = new CRect;
		
		*m_pFillBoundRect = m_position.GetRECT();
		
		if (m_nVerFillPercent != 100)
		{
			int nFillHeight = m_pFillBoundRect->Height() * m_nVerFillPercent / 100;
			
			switch (m_enumVerFillDirection)
			{
			case VerticalDirectionFromTop:
				{
					m_pFillBoundRect->bottom = m_pFillBoundRect->top + nFillHeight;
				}
				break;
			case VerticalDirectionFromBottom:
				{
					m_pFillBoundRect->top = m_pFillBoundRect->bottom - nFillHeight;
				}
				break;
			case VerticalDirectionFromCenter:
				{
					m_pFillBoundRect->top = (m_pFillBoundRect->top + m_pFillBoundRect->bottom - nFillHeight) / 2;
					m_pFillBoundRect->bottom = m_pFillBoundRect->top + nFillHeight;
				}
				break;
			}
		}
		
		if (m_nHorFillPercent != 100)
		{
			int nFillWidth = m_pFillBoundRect->Width() * m_nHorFillPercent / 100;
			
			switch (m_enumHorFillDirection)
			{
			case HorizontalDirectionFromLeft:
				{
					m_pFillBoundRect->right = m_pFillBoundRect->left + nFillWidth;
				}
				break;
			case HorizontalDirectionFromRight:
				{
					m_pFillBoundRect->left = m_pFillBoundRect->right - nFillWidth;
				}
				break;
			case HorizontalDirectionFromCenter:
				{
					m_pFillBoundRect->left = (m_pFillBoundRect->left + m_pFillBoundRect->right - nFillWidth) / 2;
					m_pFillBoundRect->right = m_pFillBoundRect->left + nFillWidth;
				}
				break;
			}
		}
	}
}