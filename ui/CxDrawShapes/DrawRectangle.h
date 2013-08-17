// DrawRectangle.h : Declaration of the CDrawRectangle

#ifndef __DRAWRECTANGLE_H_
#define __DRAWRECTANGLE_H_

#include "resource.h"       
#include "CxDispid.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"
#include "gdiplus.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawRectangle
class ATL_NO_VTABLE CDrawRectangle : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawRectangle, &CLSID_DrawRectangle>,
	public ICxDispatchImpl<CDrawRectangle, IDrawRectangle, &IID_IDrawRectangle, &LIBID_CXHMIOBJLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawRectangle>,
//	public ICxPerPropertyEditImpl<CDrawRectangle>,
	public ICxCategorizePropertiesImpl<CDrawRectangle>,
	public IProvideClassInfo2Impl<&CLSID_DrawRectangle, &DIID__ICxDrawObjectEvents, &LIBID_CXHMIOBJLib>,
	public CProxy_ICxDrawObjectEvents<CDrawRectangle>,
	public IConnectionPointContainerImpl<CDrawRectangle>
{
public:
	~CDrawRectangle()
	{
	}
	CDrawRectangle()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRAWRECTANGLE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawRectangle)
	COM_INTERFACE_ENTRY(IDrawRectangle)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
//	COM_INTERFACE_ENTRY(ICxPerPropertyEdit)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CXPAGE_MAP()
	CXPAGE_ENTRY(CLSID_CxShapeGenPage)
END_CXPAGE_MAP()	

BEGIN_CONNECTION_POINT_MAP(CDrawRectangle)
	CONNECTION_POINT_ENTRY(DIID__ICxDrawObjectEvents)
END_CONNECTION_POINT_MAP()

BEGIN_CXPROP_MAP()
	CXPROP_ENTRY("FillStyle", CXPROPID_FillStyle)
	CXPROP_ENTRY("HorizontalFillDirection", CXPROPID_HorFillDirection)
	CXPROP_ENTRY("VerticalFillDirection", CXPROPID_VerFillDirection)
	CXPROP_ENTRY("VerticalFillPercent", CXPROPID_VerFillPercent)
	CXPROP_ENTRY("HorizontalFillPercent", CXPROPID_HorFillPercent)
	CXPROP_ENTRY("BrushImageFileName", CXPROPID_BrushImageFileName)

	CXPROP_ENTRY("EdgeStyle", CXPROPID_EdgeStyle)
	CXPROP_ENTRY("EdgeWidth", CXPROPID_EdgeWidth)
	CXPROP_ENTRY("JoinStyle", CXPROPID_LineJoinStyle)

	CXPROP_ENTRY("EdgeColor", CXPROPID_EdgeColor)
	CXPROP_ENTRY("FillColor", CXPROPID_FillColor)
	CXPROP_ENTRY("HighLightEdgeColor", CXPROPID_HighLightEdgeColor)
	CXPROP_ENTRY("HighLightFillColor", CXPROPID_HighLightFillColor)

	CXPROP_ENTRY("ScaleBasePoint", CXPROPID_ScaleBasePoint)
	CXPROP_ENTRY("RoateBasePointX", CXPROPID_RoateBasePointX)
	CXPROP_ENTRY("RoateBasePointY", CXPROPID_RoateBasePointY)

	CXPROP_ENTRY("MousePointer", CXPROPID_MousePointer)
END_CXPROP_MAP()

BEGIN_CATEGORY_MAP(CDrawRectangle)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
	IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawRectangle) 	
	CATEGORY_PROP_NAME(1, _T("颜色"))
	CATEGORY_PROP_NAME(2, _T("填充"))
	CATEGORY_PROP_NAME(3, _T("变换"))
	CATEGORY_PROP_NAME(4, _T("线型"))
END_CATEGORY_PROP_NAME_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawRectangle)
	PROPERTY_TO_CATEGORY(CXPROPID_FillColor,    1)
	PROPERTY_TO_CATEGORY(CXPROPID_EdgeColor,    1)
	PROPERTY_TO_CATEGORY(CXPROPID_HighLightEdgeColor,    1)
	PROPERTY_TO_CATEGORY(CXPROPID_HighLightFillColor,    1)

	PROPERTY_TO_CATEGORY(CXPROPID_FillStyle,    2)
	PROPERTY_TO_CATEGORY(CXPROPID_HorFillPercent,    2)
	PROPERTY_TO_CATEGORY(CXPROPID_HorFillDirection,    2)
	PROPERTY_TO_CATEGORY(CXPROPID_VerFillPercent,    2)
	PROPERTY_TO_CATEGORY(CXPROPID_VerFillDirection,    2)
	PROPERTY_TO_CATEGORY(CXPROPID_BrushImageFileName,    2)
	//PROPERTY_TO_CATEGORY(CXPROPID_BackgroundStyle,    2)

	PROPERTY_TO_CATEGORY(CXPROPID_RotateAngle,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_ScaleBasePoint,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_HorScale,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_VerScale,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_RoateBasePointX,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_RoateBasePointY,    3)
	
	PROPERTY_TO_CATEGORY(CXPROPID_EdgeStyle,  4)
	PROPERTY_TO_CATEGORY(CXPROPID_EdgeWidth,  4)
	PROPERTY_TO_CATEGORY(CXPROPID_LineJoinStyle,  4)

	PROPERTY_TO_CATEGORY(CXPROPID_HorPosition,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_VerPosition,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_Height,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_Width,    -4)
END_PROPERTY_TO_CATEGORY_MAP()

// IDrawRectangle
public:
	HRESULT DoLoad(DWORD dwVer, LPSTREAM pStm);
	HRESULT DoSave(DWORD dwVer, LPSTREAM pStm);

	STDMETHOD(GetClassName)(BSTR* pbstrClassName);

	virtual void AddObjectToPath();
};

#endif //__DRAWRECTANGLE_H_
