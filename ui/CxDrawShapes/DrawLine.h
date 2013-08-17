// DrawLine.h : Declaration of the CDrawLine

#ifndef __DRAWLINE_H_
#define __DRAWLINE_H_

#include "resource.h"       // main symbols
#include "CxDispid.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"


/////////////////////////////////////////////////////////////////////////////
// CDrawLine
class ATL_NO_VTABLE CDrawLine : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawLine, &CLSID_DrawLine>,
	public ICxDispatchImpl<CDrawLine, IDrawLine, &IID_IDrawLine, &LIBID_CXHMIOBJLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawLine>,
	public ICxCategorizePropertiesImpl<CDrawLine>,
	public IProvideClassInfo2Impl<&CLSID_DrawLine, &DIID__ICxDrawObjectEvents, &LIBID_CXHMIOBJLib>,
	public CProxy_ICxDrawObjectEvents<CDrawLine>,
	public IConnectionPointContainerImpl<CDrawLine>

{
public:
	CDrawLine()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRAWLINE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawLine)
	COM_INTERFACE_ENTRY(IDrawLine)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CXPAGE_MAP()
	CXPAGE_ENTRY(CLSID_CxShapeGenPage)
END_CXPAGE_MAP()

BEGIN_CONNECTION_POINT_MAP(CDrawLine)
	CONNECTION_POINT_ENTRY(DIID__ICxDrawObjectEvents)
END_CONNECTION_POINT_MAP()

BEGIN_CXPROP_MAP()
	CXPROP_ENTRY("EdgeStyle", CXPROPID_EdgeStyle)
	CXPROP_ENTRY("EdgeWidth", CXPROPID_EdgeWidth)
	CXPROP_ENTRY("EdgeColor", CXPROPID_EdgeColor)
	CXPROP_ENTRY("HighLightEdgeColor", CXPROPID_HighLightEdgeColor)
	CXPROP_ENTRY("RotateAngle", CXPROPID_RotateAngle)
	CXPROP_ENTRY("RoateBasePointX", CXPROPID_RoateBasePointX)
	CXPROP_ENTRY("RoateBasePointY", CXPROPID_RoateBasePointY)
	CXPROP_ENTRY("JoinStyle", CXPROPID_LineJoinStyle)
	CXPROP_ENTRY("MousePointer", CXPROPID_MousePointer)
END_CXPROP_MAP()

//属性目录
BEGIN_CATEGORY_MAP(CDrawLine)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
//	IMPLEMENTED_CATEGORY(CATID_Insertable)
//	IMPLEMENTED_CATEGORY(CATID_Control)
	IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawLine) 	
	CATEGORY_PROP_NAME(1, _T("颜色"))
	CATEGORY_PROP_NAME(3, _T("变换"))
	CATEGORY_PROP_NAME(4, _T("线型"))
END_CATEGORY_PROP_NAME_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawLine)	
	PROPERTY_TO_CATEGORY(CXPROPID_EdgeColor,    1)
	PROPERTY_TO_CATEGORY(CXPROPID_HighLightEdgeColor,    1)
	
	PROPERTY_TO_CATEGORY(CXPROPID_RotateAngle,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_RoateBasePointX,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_RoateBasePointY,    3)

	PROPERTY_TO_CATEGORY(CXPROPID_EdgeStyle,  4)
	PROPERTY_TO_CATEGORY(CXPROPID_EdgeWidth,  4)
	PROPERTY_TO_CATEGORY(CXPROPID_LineJoinStyle,  4)

	PROPERTY_TO_CATEGORY(CXPROPID_StartX,    -4)	
	PROPERTY_TO_CATEGORY(CXPROPID_StartY,    -4)	
	PROPERTY_TO_CATEGORY(CXPROPID_EndX,    -4)	
	PROPERTY_TO_CATEGORY(CXPROPID_EndY,    -4)

	PROPERTY_TO_CATEGORY(CXPROPID_HorPosition,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_VerPosition,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_Height,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_Width,    -4)
END_PROPERTY_TO_CATEGORY_MAP()

// IDrawRect
public:
	STDMETHOD(get_EndY)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_EndY)(/*[in]*/ float newVal);
	STDMETHOD(get_EndX)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_EndX)(/*[in]*/ float newVal);
	STDMETHOD(get_StartY)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_StartY)(/*[in]*/ float newVal);
	STDMETHOD(get_StartX)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_StartX)(/*[in]*/ float newVal);

	HRESULT DoDraw(HDC hdcDest);
	HRESULT DoPrint(HDC hdcDest);
	HRESULT DoLoad(DWORD dwVer, LPSTREAM pStm);
	HRESULT DoSave(DWORD dwVer, LPSTREAM pStm);
			
	STDMETHOD(MoveHandleTo)(long nHandle, POINT point);
	STDMETHOD(GetHandleCount)(long* pHandleCount);
	STDMETHOD(GetHandle)(long nHandle, POINT* pHandlePoint);
	STDMETHOD(GetHandleCursor)(long nHandle, HCURSOR* pHandleCursor);
	STDMETHOD(GetClassName)(BSTR* pbstrClassName);
	
	STDMETHOD(HitTest)(POINT point, BOOL* bHitTest);
	STDMETHOD(Intersects)(RECT rect, BOOL* bIntersects);
	void AddObjectToPath();
	STDMETHOD(DrawTrackerShape)(HDC hdcDest, POINT* points, int ptcount);
	STDMETHOD(DrawTrackerHandleShape)(HDC hdcDest, long nHandle, POINT point);
};

#endif //__DRAWLINE_H_
