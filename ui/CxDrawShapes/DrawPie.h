// DrawPie.h : Declaration of the CDrawPie

#ifndef __DRAWPIE_H_
#define __DRAWPIE_H_

#include "resource.h"       // main symbols
#include "CxDispid.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawPie
class ATL_NO_VTABLE CDrawPie : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawPie, &CLSID_DrawPie>,
	public ICxDispatchImpl<CDrawPie, IDrawPie, &IID_IDrawPie, &LIBID_CXHMIOBJLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawPie>,
	public ICxCategorizePropertiesImpl<CDrawPie>,
	public IProvideClassInfo2Impl<&CLSID_DrawPie, &DIID__ICxDrawObjectEvents, &LIBID_CXHMIOBJLib>,
	public CProxy_ICxDrawObjectEvents<CDrawPie>,
	public IConnectionPointContainerImpl<CDrawPie>
{
public:
	CDrawPie()
	{
	}

	
DECLARE_REGISTRY_RESOURCEID(IDR_DRAWPIE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawPie)
	COM_INTERFACE_ENTRY(IDrawPie)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY/*_IMPL*/(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDrawPie)
	CONNECTION_POINT_ENTRY(DIID__ICxDrawObjectEvents)
END_CONNECTION_POINT_MAP()

BEGIN_CXPAGE_MAP()
	CXPAGE_ENTRY(CLSID_CxShapeGenPage)
END_CXPAGE_MAP()

BEGIN_CATEGORY_MAP(CDrawPie)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
	IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawPie) 	
	CATEGORY_PROP_NAME(1, _T("颜色"))
	CATEGORY_PROP_NAME(2, _T("填充"))
	CATEGORY_PROP_NAME(3, _T("变换"))
	CATEGORY_PROP_NAME(4, _T("线型"))
	CATEGORY_PROP_NAME(5, _T("弧"))
END_CATEGORY_PROP_NAME_MAP()

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

	CXPROP_ENTRY("RoateBasePointX", CXPROPID_RoateBasePointX)
	CXPROP_ENTRY("RoateBasePointY", CXPROPID_RoateBasePointY)
	CXPROP_ENTRY("ScaleBasePoint", CXPROPID_ScaleBasePoint)

	CXPROP_ENTRY("MousePointer", CXPROPID_MousePointer)
		
END_CXPROP_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawPie)	
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

	PROPERTY_TO_CATEGORY(CXPROPID_RotateAngle,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_ScaleBasePoint,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_HorScale,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_VerScale,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_RoateBasePointX,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_RoateBasePointY,    3)

	PROPERTY_TO_CATEGORY(CXPROPID_StartAngle,  5)
	PROPERTY_TO_CATEGORY(CXPROPID_SweepAngle,  5)
	PROPERTY_TO_CATEGORY(CXPROPID_Radius,  5)
	PROPERTY_TO_CATEGORY(CXPROPID_StartAngle,  5)
	PROPERTY_TO_CATEGORY(CXPROPID_PieType,  5)

	PROPERTY_TO_CATEGORY(CXPROPID_EdgeStyle,  4)
	PROPERTY_TO_CATEGORY(CXPROPID_EdgeWidth,  4)
	PROPERTY_TO_CATEGORY(CXPROPID_LineJoinStyle,  4)

	PROPERTY_TO_CATEGORY(CXPROPID_CenterX,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_CenterY,    -4)

	PROPERTY_TO_CATEGORY(CXPROPID_HorPosition,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_VerPosition,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_Height,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_Width,    -4)
END_PROPERTY_TO_CATEGORY_MAP()


// ICxDrawObject
public:
	HRESULT DoLoad(DWORD dwVer, LPSTREAM pStm);
	HRESULT DoSave(DWORD dwVer, LPSTREAM pStm);

	STDMETHOD(SaveOldVer)(LPSTREAM pStm, BOOL fClearDirty); 
	STDMETHOD(LoadOldVer)(LPSTREAM pStm);
	STDMETHOD(GetClassName)(BSTR* pbstrClassName);
	STDMETHOD(MoveHandleTo)(long nHandle, POINT point);
	STDMETHOD(MoveHandlesTo)(POINT* pPoints, int nCount);
	STDMETHOD(GetHandleCount)(long* pHandleCount);
	STDMETHOD(GetHandle)(long nHandle, POINT* pHandlePoint);
	STDMETHOD(GetHandleCursor)(long nHandle, HCURSOR* pHandleCursor);
	STDMETHOD(InitNew)();
	STDMETHOD(Intersects)(RECT rect, BOOL* pbIntersects);
	void InitPiePath(Gdiplus::GraphicsPath** ppObjectPath,
		POINT* point, enumPieType type);

// IDrawPie
public:
	STDMETHOD(get_PieType)(/*[out, retval]*/ enumPieType *pVal);
	STDMETHOD(put_PieType)(/*[in]*/ enumPieType newVal);
	STDMETHOD(get_StartAngle)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_StartAngle)(/*[in]*/ float newVal);
	STDMETHOD(get_SweepAngle)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_SweepAngle)(/*[in]*/ float newVal);
	STDMETHOD(get_CenterY)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_CenterY)(/*[in]*/ float newVal);
	STDMETHOD(get_CenterX)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_CenterX)(/*[in]*/ float newVal);
	STDMETHOD(get_Radius)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_Radius)(/*[in]*/ float newVal);
	
	STDMETHOD(DrawTrackerHandleShape)(HDC hdcDest, long nHandle, POINT point);
	STDMETHOD(DrawTrackerShape)(HDC hdcDest, POINT* points, int ptcount);
private:
	//园弧类型 
	enumPieType m_enumPieType;
	
};

#endif //__DRAWPIE_H_
