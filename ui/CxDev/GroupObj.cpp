// GroupItem.cpp: implementation of the CGroupObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"
#include "CxDevIF.h"
#include "ActionObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//XProviderClassInfo
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

	return theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(GroupObject), ppTypeInfo);
	
//	return pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), __uuidof(UserForm), ppTypeInfo);
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

//////////////////////////////////////////////////////////////////////
//CGroupObj Members
IMPLEMENT_SERIAL(CGroupObj, CDevObj, 0)

CGroupObj::CGroupObj(CDevObjDoc* pContainerDoc) :
	CDevObj(pContainerDoc),
	m_fHorScale(1),							//水平缩放因子
	m_fVerScale(1),
	m_fRotateAngle(0)
{
	m_strClassName = "Group";
	m_enumScaleBase = ScaleBasePointCenter;
	m_enumMousePointer = MousePointerArrow;
	m_bUniformScale = FALSE;

	m_enumLineJoinStyle = LineJoinStyleRound;
	m_enumFillStyle = FillStyleSolid;
	m_enumEdgeStyle = EdgeStyleSolid;				
	m_fEdgeWidth = 1;										
	m_nHorFillPercent = 100;								
	m_nVerFillPercent = 100;								
	m_enumVerFillDirection = VerticalDirectionFromBottom;		
	m_enumHorFillDirection = HorizontalDirectionFromLeft;		
	
	m_enumMousePointer = MousePointerArrow;
	
	m_clrEdgeColor = RGB(0, 0, 0);
	m_clrFillColor = RGB(0, 0, 255);
	m_clrHighLightEdge = RGB(255, 255, 255);
	m_clrHighLightFill = RGB(255, 255, 255);

	m_pBoundRect = NULL;
	m_pFillBoundRect = NULL;

	EnableAutomation();

	AfxOleLockApp();
}

CGroupObj::~CGroupObj()
{
	AfxOleUnlockApp();
}

void CGroupObj::AddSubObject(CDevObj *pObj)
{
	m_arrSubObj.Add(pObj);	
	pObj->m_pGroupObj = this;
	RecalcPosition();
	m_ptRotateBase = m_position.CenterPoint();
	
	DWORD id = 0;
	BOOL bFlag = TRUE;
	while(bFlag)
	{
		bFlag = FALSE;
		for(int i = 0; i < m_arrSubObj.GetSize(); i++)
		{
			if(id == m_arrSubObj[i]->m_dwSubObjNum)
			{
				id++;
				bFlag = TRUE;
				break;
			}
		}
	}

	pObj->m_dwSubObjNum = id;
}

void CGroupObj::RemoveObject(CDevObj* pObj)
{
	ASSERT_VALID(pObj);    // must be valid before detach
	ASSERT(pObj->m_pDocument == m_pDocument);     // formerly attached

	for (int i = 0; i < m_arrSubObj.GetSize(); i++)
	{
		if (m_arrSubObj[i] == pObj)
		{
			m_arrSubObj.RemoveAt(i);
			break;
		}
	}
	
	pObj->m_pGroupObj = NULL;
}

void CGroupObj::RemoveAllObjects()
{
	m_arrSubObj.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////
//功 能：创建或导入对象]
/////////////////////////////////////////////////////////////////////////
BOOL CGroupObj::CreateOrLoad(LPCTSTR pszName,
   REFIID iidPersistanceMedium, IUnknown* pPersistanceMedium)
{
	m_dwObjectNumber = GetNewObjectNumber();
	m_pDocument->CreateUniqueObjectName(this);
	return TRUE;
}

LPUNKNOWN CGroupObj::GetObjectUnknown()
{
	return GetInterface(&IID_IUnknown);
}

void CGroupObj::DrawFrontToBack(CDC* pDC)
{
	for(int i = 0; i < m_arrSubObj.GetSize(); i++)
	{	
		m_arrSubObj[i]->DrawFrontToBack(pDC);
	}
}

void CGroupObj::Draw(CDC* pDC)
{
	if (IsDrawVisible())
	{
		for(int i = 0; i < m_arrSubObj.GetSize(); i++)
		{
			m_arrSubObj[i]->Draw(pDC);
		}
	}
}

void CGroupObj::Print(CDC* pDC)
{
	if (IsDrawVisible())
	{
		for (int i = 0; i < m_arrSubObj.GetSize(); i++)
		{
			m_arrSubObj[i]->Print(pDC);
		}
	}
}

void CGroupObj::SetChildObjectDocument()
{
	for (int i = 0; i < m_arrSubObj.GetSize(); i++)
	{
		CDevObj* pObj = m_arrSubObj[i];
		pObj->m_pDocument = m_pDocument;
		pObj->SetChildObjectDocument();
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
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "Locked", CXPROPID_Locked, m_bLocked, OnLockedChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "Visible", CXPROPID_Visible, m_bVisible, OnVisibleChanged, VT_BOOL)
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

	DISP_PROPERTY_EX_ID(CGroupObj, "Name", CXPROPID_Name, GetName, SetName, VT_BSTR)
	DISP_FUNCTION_ID(CGroupObj, "IsConnected", CXMETHID_IsConnected, IsConnected, VT_I2, VTS_BSTR)
	DISP_FUNCTION_ID(CGroupObj, "ConnectObject", CXMETHID_ConnectObject, ConnectObject, VT_EMPTY, VTS_BSTR VTS_UNKNOWN)
	DISP_FUNCTION_ID(CGroupObj, "ConnectDirect", CXMETHID_ConnectDirect, ConnectDirect, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CGroupObj, "GetConnectObject", CXMETHID_GetConnectObject, GetConnectObject, VT_UNKNOWN, VTS_BSTR)
	DISP_FUNCTION_ID(CGroupObj, "Disconnect", CXMETHID_Disconnect, Disconnect, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CGroupObj, "GetConnectState", CXMETHID_GetConnectState, GetConnectState, VT_I4, VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

const extern IID LIBID_CXHMIOBJLib;
IMPLEMENT_OLETYPELIB(CGroupObj, LIBID_CXHMIOBJLib, 1, 0)

const IID IID_IGroupObject = 
{0x2BE0E00C,0x3871,0x4d3a,{0x96,0xF3,0x76,0xA8,0x06,0xC0,0x6D,0x3E}};

const IID IID_IGroupObject2 = 
{0x4D765B89,0x2E46,0x4cff,{0x94,0x15,0xF1,0x6C,0x41,0xE1,0x6C,0x0B}};

BEGIN_INTERFACE_MAP(CGroupObj, CDevObj)
	INTERFACE_PART(CGroupObj, IID_IGroupObject, Dispatch)
	INTERFACE_PART(CGroupObj, IID_IGroupObject2, Dispatch)
	INTERFACE_PART(CGroupObj, IID_ICategorizeProperties, CategorizeProperties)
	INTERFACE_PART(CGroupObj, IID_IProvideClassInfo, ProvideClassInfo)
	INTERFACE_PART(CGroupObj, IID_IProvideClassInfo2, ProvideClassInfo)
END_INTERFACE_MAP()

////////////////////////////////////////////////////////////////
STDMETHODIMP_( ULONG ) CGroupObj::XCategorizeProperties::AddRef()
{
	METHOD_PROLOGUE(CGroupObj, CategorizeProperties)

	TRACE("XCategorizeProperties::AddRef().\n");

	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CGroupObj::XCategorizeProperties::Release()
{
	METHOD_PROLOGUE(CGroupObj, CategorizeProperties)

	TRACE("XCategorizeProperties::Release().\n");

	return pThis->ExternalRelease();
}

STDMETHODIMP CGroupObj::XCategorizeProperties::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CGroupObj, CategorizeProperties)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CGroupObj::XCategorizeProperties::MapPropertyToCategory(DISPID dispid, PROPCAT* ppropcat)
{
	METHOD_PROLOGUE(CGroupObj, CategorizeProperties)

	return pThis->MapPropertyToCategory(dispid, ppropcat);
}

STDMETHODIMP CGroupObj::XCategorizeProperties::GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName)
{
	METHOD_PROLOGUE(CGroupObj, CategorizeProperties)

	return pThis->GetCategoryName(propcat, lcid, pbstrName);
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
	CRectF rect = m_position;
	rect.OffsetRect(fNewValue - m_position.left, 0);
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
	return 0;
}

void CGroupObj::SetRotateAngle(float fNewValue)
{
	if (IsRunMode())
	{
		RotateAt(m_ptRotateBase, fNewValue - m_fRotateAngle);		
		m_fRotateAngle = fNewValue;
	}
	else
	{
		RotateAt(m_ptRotateBase, fNewValue);
	}
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
	return 1;
}

void CGroupObj::SetHorizontalScale(float fNewValue)
{
	if (IsRunMode())
	{
		if (fNewValue == 0)
			fNewValue = 1 / m_position.Width();
		float fHorScale = fNewValue / m_fHorScale;		
		m_fHorScale = fNewValue;
		ScaleAt(GetScaleBasePointF(), fHorScale, 1);
	}
	else
	{
		ScaleAt(GetScaleBasePointF(), fNewValue, 1);
	}
}

float CGroupObj::GetVerticalScale()
{
	return 1;
}

void CGroupObj::SetVerticalScale(float fNewValue)
{
	if (IsRunMode())
	{
		if (fNewValue == 0)
			fNewValue = 1 / m_position.Height();
		float fVerScale = fNewValue / m_fVerScale;		
		m_fVerScale = fNewValue;
		ScaleAt(GetScaleBasePointF(), 1, fVerScale);
	}
	else
	{
		ScaleAt(GetScaleBasePointF(), 1, fNewValue);
	}
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

	m_nLayer = lLayer - 1;
	Invalidate();
}

void CGroupObj::OnLockedChanged()
{
	Invalidate();
}

void CGroupObj::OnVisibleChanged()
{
	Invalidate();
}

BSTR CGroupObj::GetName()
{
	return GetDisplayName().AllocSysString();
}

void CGroupObj::SetName(LPCTSTR lpszNewValue)
{
	CString strOldName = GetDisplayName();
	PutDisplayName(lpszNewValue);
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
	if (SUCCEEDED(((IDispatch *)(&m_xDispatch))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
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

// 此函数在开发环境无用, 没有实现
LPUNKNOWN CGroupObj::GetObjectByName(LPCTSTR lpszName)
{
	return NULL;
}

// 此函数在开发环境无用, 没有实现
LPDISPATCH CGroupObj::GetObjectCollection()
{
	return NULL;
}

float CGroupObj::GetVerticalPosition()
{
	return m_position.top;
}

void CGroupObj::SetVerticalPosition(float fNewValue)
{
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


CDevObj::TrackerState CGroupObj::GetTrackerState(TrackerState state)
{
	if (state == general || state == reshape)
		return resize;
	return state;
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

	for(int i = 1; i < m_arrSubObj.GetSize(); i++)
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

	// 重新计算边界
	if (m_pBoundRect != NULL)
	{
		delete m_pBoundRect;
		m_pBoundRect = NULL;
	}
	GetBoundingRect();

	RecalcFillBoundRect();
}

BOOL CGroupObj::HitTest(CPoint point, CLayoutView* pView)
{
	if (IsDrawVisible())
	{
		if (GetBoundingRect().PtInRect(point))
		{
			if (CDevObj::HitTest(point, pView))
				return TRUE;

			int nCount = m_arrSubObj.GetSize();
			for (int i = 0; i < nCount; i++)
			{
				if (m_arrSubObj[i]->HitTest(point, pView))
					return TRUE;
			}
		}
	}

	return FALSE;
	

//	return CDevObj::HitTest(point, pView);
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

CRect CGroupObj::GetBoundingRect()
{
	CDevObj* pObj;
	CRect rcObj;
	long lLeft;
	long lTop;
	long lRight;
	long lBottom;

	if (m_pBoundRect == NULL)
	{	
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

		m_pBoundRect = new CRect(lLeft, lTop, lRight, lBottom);
	}

	return *m_pBoundRect;
}

//////////////////////////////////////////////////////////////////
//功能: 移动组合对象
//////////////////////////////////////////////////////////////////
void CGroupObj::MoveTo(const CRectF &position, BOOL bUniformScale, CLayoutView* pView)
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

void CGroupObj::DrawRotateAtShape(CDC* pDC, CPointF point, long lAngle)
{
	for (int i = 0; i < m_arrSubObj.GetSize(); i++)
	{
		m_arrSubObj[i]->DrawRotateAtShape(pDC, point, lAngle);
	}
}

void CGroupObj::Rotate(int lAngle, CLayoutView* pView)
{
	RotateAt(m_ptRotateBase, (float)lAngle, pView);
}

void CGroupObj::RotateAt(CPointF point, float fAngle, CLayoutView* pView)
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

void CGroupObj::ScaleAt(CPointF point, float fHorScale, float fVerScale, CLayoutView* pView)
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

void CGroupObj::MoveHandleTo(int nHandle, TrackerState state, CPoint point, CLayoutView* pView)
{
	switch (state)
	{
	case general:
	case resize:
		CDevObj::MoveHandleTo(nHandle, state, point, pView);
		break;
	case rotate:
		{
			if (nHandle == 1)
			{
				CDevObj::MoveHandleTo(nHandle, state, point, pView);
				return;
			}
			CPoint ptBase = GetHandle(1, state, pView);
			CPoint ptOrign = GetHandle(nHandle, state, pView);
			for (int i = 0; i < m_arrSubObj.GetSize(); i++)
			{
				m_arrSubObj[i]->RotateAt(m_ptRotateBase, float(CalculateAngle(ptBase, ptOrign) - CalculateAngle(ptBase, point)), pView);
			}
			RecalcPosition();
		}
		break;
	default:
		ASSERT(FALSE);
	}

}

void CGroupObj::DrawTrackerHandleShape(CDC* pDC, int nHandle, TrackerState state, CPoint point, CLayoutView* pView)
{
	switch (state)
	{
	case general:
	case resize:
		CDevObj::DrawTrackerHandleShape(pDC, nHandle, state, point, pView);
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
			for (int i = 0; i < m_arrSubObj.GetSize(); i++)
			{
				m_arrSubObj[i]->DrawRotateAtShape(pDC, m_ptRotateBase, CalculateAngle(ptBase, ptOrign) - CalculateAngle(ptBase, point));
			}
//			m_pDrawObject->DrawRotateShape(pDC->m_hDC, CalculateAngle(ptBase, ptOrign) - CalculateAngle(ptBase, point));
			CDevObj::DrawTrackerHandleShape(pDC, nHandle, state, point, pView);
		}
		break;
	default:
		ASSERT(FALSE);
	}
}

CPoint CGroupObj::GetRotateBasePoint()
{
	return CPoint(int(m_ptRotateBase.x), int(m_ptRotateBase.y));
}

void CGroupObj::SetRotateBasePoint(CPoint point)
{
	m_ptRotateBase.x = (float)point.x;
	m_ptRotateBase.y = (float)point.y;
}

void CGroupObj::Delete(BOOL bAutoDelete)
{
	CDevObjArray arrObjs;

	//保存子对象指针(因子对象删除时会自动把自己从m_arrSubObj中删除)
	for(int i = 0; i < m_arrSubObj.GetSize(); i++)
	{
		arrObjs.Add(m_arrSubObj[i]); //->Delete(bAutoDelete);
	}

	//删除子对象
	for(i = 0; i < arrObjs.GetSize(); i++)
	{
		arrObjs[i]->Delete(bAutoDelete);
	}

	CDevObj::Delete(bAutoDelete);
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

	if (m_pBoundRect != NULL)
	{
		delete m_pBoundRect;
		m_pBoundRect = NULL;
	}
	if (m_pFillBoundRect != NULL)
	{
		delete m_pFillBoundRect;
		m_pFillBoundRect = NULL;
	}

	CDevObj::Release();
}

void CGroupObj::Serialize(CArchive& ar)
{
	CDevObj::Serialize(ar);

	m_arrSubObj.Serialize(ar);
	
	if (ar.IsLoading())
	{
		for(int i = 0; i < m_arrSubObj.GetSize(); i++)
		{
			m_arrSubObj[i]->m_pGroupObj = this;
		}
	}

	if (ar.IsStoring())
	{
		ar << m_position;
		ar << m_ptRotateBase;
		ar << m_enumScaleBase;
		ar << m_bUniformScale;
		ar << m_enumMousePointer;
		ar << m_fHorScale;							
		ar << m_fVerScale;							
		ar << m_fRotateAngle; 
		
		ar << m_clrEdgeColor;
		ar << m_clrHighLightEdge;
		ar << m_enumEdgeStyle;			
		ar << m_enumLineJoinStyle;
		ar << m_fEdgeWidth;	
		
		ar << m_clrFillColor;
		ar << m_clrHighLightFill;
		ar << m_enumFillStyle;
		ar << m_strBrushImageFileName;
		
		ar << m_enumHorFillDirection;
		ar << m_enumVerFillDirection;
		ar << m_nHorFillPercent;
		ar << m_nVerFillPercent;

		ar << (m_pFillBoundRect != NULL);
		if (m_pFillBoundRect != NULL)
			ar << *m_pFillBoundRect;
	}
	else
	{
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
	}

	//序列化动态属性
	SerializeDynamicProperty(ar);
}

void CGroupObj::CopyToClipboard(CArchive& ar, IStorage* pStorage)
{
	ar << m_position;
	ar << m_ptRotateBase;
	ar << m_enumScaleBase;
	ar << m_enumMousePointer;
	ar << m_bUniformScale;
	ar << m_fHorScale;							
	ar << m_fVerScale;							
	ar << m_fRotateAngle;
	
	ar << m_clrEdgeColor;
	ar << m_clrHighLightEdge;
	ar << m_enumEdgeStyle;			
	ar << m_enumLineJoinStyle;
	ar << m_fEdgeWidth;	
	
	ar << m_clrFillColor;
	ar << m_clrHighLightFill;
	ar << m_enumFillStyle;
	ar << m_strBrushImageFileName;
	
	ar << m_enumHorFillDirection;
	ar << m_enumVerFillDirection;
	ar << m_nHorFillPercent;
	ar << m_nVerFillPercent;
	
	ar << (m_pFillBoundRect != NULL);
	if (m_pFillBoundRect != NULL)
		ar << *m_pFillBoundRect;

	ar << m_arrSubObj.GetSize();
	for(int i = 0; i < m_arrSubObj.GetSize(); i++)
	{
		ar << int(m_arrSubObj[i]->GetObjectType());
		m_arrSubObj[i]->CopyToClipboard(ar, pStorage);
	}
	
	//拷贝动态属性
	SerializeDynamicProperty(ar);

	CDevObj::CopyToClipboard(ar, pStorage);
}

BOOL CGroupObj::CreateFromClipboard(CArchive& ar, IStorage* pStorage)
{
	CDevObj* pObj;
	int nObjects;
	int nObjType;
	
	ASSERT(m_pDocument != NULL);

	m_dwObjectNumber = GetNewObjectNumber();

	ar >> m_position;
	ar >> m_ptRotateBase;
	long lTemp;
	ar >> lTemp;
	m_enumScaleBase = enumScaleBasePoint(lTemp);
	ar >> lTemp;
	m_enumMousePointer = enumMousePointer(lTemp);
	ar >> m_bUniformScale;
	ar >> m_fHorScale;							
	ar >> m_fVerScale;							
	ar >> m_fRotateAngle; 

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

	ar >> nObjects;
	for (int i = 0; i < nObjects; i++)
	{
		ar >> nObjType;
		switch (nObjType)
		{
		case drawobj:
			pObj = new CDrawObj(m_pDocument);
			break;
		case groupobj:
			pObj = new CGroupObj(m_pDocument);
			break;
		case symbolobj:
			pObj = new CSymbolObj(m_pDocument);
			break;
		case ctrlobj:
			pObj = new CCtrlObj(m_pDocument);
			break;
		case formobj:
			pObj = new CFormObj(m_pDocument);
			break;
		case reportobj:
			pObj = new CReportObj(m_pDocument);
			break;
		default:
			ASSERT(FALSE);
		}

		if (!pObj->CreateFromClipboard(ar, pStorage))
		{
			AfxThrowOleException(E_FAIL);
		}

		pObj->m_pGroupObj = this;
		m_pDocument->RemoveObject(pObj);
		pObj->m_pDocument = m_pDocument;
		m_arrSubObj.Add(pObj);
	}

	//创建动态属性
	SerializeDynamicProperty(ar);

	return CDevObj::CreateFromClipboard(ar, pStorage);
}

HRESULT CGroupObj::MapPropertyToCategory(DISPID dispid, PROPCAT* ppropcat)
{
	switch (dispid)
	{
		case CXPROPID_HorPosition:
		case CXPROPID_VerPosition:
		case CXPROPID_Width:
		case CXPROPID_Height:
			*ppropcat = -4;
			break;
		case CXPROPID_RotateAngle:
		case CXPROPID_RoateBasePointX:
		case CXPROPID_RoateBasePointY:
		case CXPROPID_ScaleBasePoint:
		case CXPROPID_HorScale:
		case CXPROPID_VerScale:
		case CXPROPID_UniformScale:
			*ppropcat = 3;
			break;
		case CXPROPID_FillColor:
		case CXPROPID_EdgeColor:
		case CXPROPID_HighLightEdgeColor:
		case CXPROPID_HighLightFillColor:
			*ppropcat = 1;
			break;
		case CXPROPID_FillStyle:
		case CXPROPID_HorFillPercent:
		case CXPROPID_HorFillDirection:
		case CXPROPID_VerFillPercent:
		case CXPROPID_VerFillDirection:
		case CXPROPID_BrushImageFileName:
			*ppropcat = 2;
			break;
		case CXPROPID_EdgeStyle:
		case CXPROPID_EdgeWidth:
		case CXPROPID_LineJoinStyle:
			*ppropcat = 4;
			break;
		default:
			*ppropcat = -2;
			break;
	}

	return S_OK;
}

HRESULT CGroupObj::GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName)
{
	switch (propcat)
	{
	case -2:
		*pbstrName = ::SysAllocString(OLESTR("杂项"));
		break;
	case 3:
		*pbstrName = ::SysAllocString(OLESTR("变换"));
		break;
	case -4:
		*pbstrName = ::SysAllocString(OLESTR("位置"));
		break;
	case 1:
		*pbstrName = ::SysAllocString(OLESTR("颜色"));
		break;
	case 2:
		*pbstrName = ::SysAllocString(OLESTR("填充"));
		break;
	case 4:
		*pbstrName = ::SysAllocString(OLESTR("线型"));
		break;
	default:
		return S_FALSE;
	}	

	return S_OK;
}

void CGroupObj::OnUngrouped()
{
	m_fHorScale = 1;							
	m_fVerScale = 1;							
	m_fRotateAngle = 0;     
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