// DrawPolyline.h : Declaration of the CDrawPolyline

#ifndef __DRAWPOLYLINE_H_
#define __DRAWPOLYLINE_H_

#include "resource.h"       // main symbols
#include "CxDispid.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawPolyline

class ATL_NO_VTABLE CDrawPolyline : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawPolyline, &CLSID_DrawPolyline>,
	public ICxDispatchImpl<CDrawPolyline, IDrawPolyline, &IID_IDrawPolyline, &LIBID_CXHMIOBJLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawPolyline>,
	public ICxCategorizePropertiesImpl<CDrawPolyline>,
	public IProvideClassInfo2Impl<&CLSID_DrawPolyline, &DIID__ICxDrawObjectEvents, &LIBID_CXHMIOBJLib>,
	public CProxy_ICxDrawObjectEvents<CDrawPolyline>,
	public IConnectionPointContainerImpl<CDrawPolyline>
{
public:
	CDrawPolyline()
	{
	}
	~CDrawPolyline()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRAWPOLYLINE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawPolyline)
	COM_INTERFACE_ENTRY(IDrawPolyline)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY/*_IMPL*/(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDrawPolyline)
	CONNECTION_POINT_ENTRY(DIID__ICxDrawObjectEvents)
END_CONNECTION_POINT_MAP()

BEGIN_CXPAGE_MAP()
	CXPAGE_ENTRY(CLSID_CxShapeGenPage)
END_CXPAGE_MAP()

BEGIN_CATEGORY_MAP(CDrawPolyline)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
	IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawPolygon) 	
	CATEGORY_PROP_NAME(1, _T("颜色"))
	/*CATEGORY_PROP_NAME(2, _T("填充"))*/
	CATEGORY_PROP_NAME(3, _T("变换"))
	CATEGORY_PROP_NAME(4, _T("线型"))
END_CATEGORY_PROP_NAME_MAP()

BEGIN_CXPROP_MAP()
	CXPROP_ENTRY("EdgeStyle", CXPROPID_EdgeStyle)
	CXPROP_ENTRY("EdgeWidth", CXPROPID_EdgeWidth)

	CXPROP_ENTRY("EdgeColor", CXPROPID_EdgeColor)
	CXPROP_ENTRY("HighLightEdgeColor", CXPROPID_HighLightEdgeColor)
	
	CXPROP_ENTRY("ScaleBasePoint", CXPROPID_ScaleBasePoint)
		
	CXPROP_ENTRY("HorizontalPosition", CXPROPID_HorPosition)
	CXPROP_ENTRY("VerticalPosition", CXPROPID_VerPosition)

	CXPROP_ENTRY("JoinStyle", CXPROPID_LineJoinStyle)

	CXPROP_ENTRY("MousePointer", CXPROPID_MousePointer)

	CXPROP_ENTRY("RoateBasePointX", CXPROPID_RoateBasePointX)
	CXPROP_ENTRY("RoateBasePointY", CXPROPID_RoateBasePointY)
END_CXPROP_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawPolygon)	
	PROPERTY_TO_CATEGORY(CXPROPID_EdgeColor,    1)
	PROPERTY_TO_CATEGORY(CXPROPID_HighLightEdgeColor,    1)
	
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

// ICxDrawObject
public:
	STDMETHOD(MoveHandleTo)(long nHandle, POINT point);
	STDMETHOD(GetHandleCount)(long* pHandleCount);
	STDMETHOD(GetHandle)(long nHandle, POINT* pHandlePoint);
	STDMETHOD(GetHandleCursor)(long nHandle, HCURSOR* pHandleCursor);
	STDMETHOD(GetClassName)(BSTR* pbstrClassName);
//	STDMETHOD(GetPositionRect)(RECT* pPositionRect);
//	STDMETHOD(GetBoundingRect)(RECT* pBoundingRect);
//	STDMETHOD(InitNew)();
	STDMETHOD(HitTest)(POINT point, BOOL* bHitTest);
	STDMETHOD(Intersects)(RECT rect, BOOL* pbIntersects);
	void AddObjectToPath();
	
	STDMETHOD(DrawTrackerHandleShape)(HDC hdcDest, long nHandle, POINT point);
	STDMETHOD(DrawTrackerShape)(HDC hdcDest, POINT* points, int ptcount);

	HRESULT DoDraw(HDC hdcDest);
	HRESULT DoPrint(HDC hdcDest);
	HRESULT DoLoad(DWORD dwVer, LPSTREAM pStm);
	HRESULT DoSave(DWORD dwVer, LPSTREAM pStm);

// IDrawPolyline
public:
	STDMETHOD(GetPointAt)(/*[in]*/ long lIndex, /*[out, retval]*/ IFloatPoint** ppPoint);
	STDMETHOD(DeletePoint)(long lIndex);
	STDMETHOD(GetNumberOfPoints)(/*[out, retval]*/ long *pVal);
	STDMETHOD(AddPoint)(IFloatPoint* pPoint);
};

#endif //__DRAWPOLYLINE_H_
