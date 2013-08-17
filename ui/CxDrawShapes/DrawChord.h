// DrawChord.h : Declaration of the CDrawChord

#ifndef __DRAWCHORD_H_
#define __DRAWCHORD_H_

#include "resource.h"       // main symbols
#include "CxDispid.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"


/////////////////////////////////////////////////////////////////////////////
// CDrawChord
class ATL_NO_VTABLE CDrawChord : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawChord, &CLSID_DrawChord>,
	public ICxDispatchImpl<CDrawChord, IDrawChord, &IID_IDrawChord, &LIBID_CXHMIOBJLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawChord>,
	public ICxCategorizePropertiesImpl<CDrawChord>,
	public IProvideClassInfo2Impl<&CLSID_DrawChord, &DIID__ICxDrawObjectEvents, &LIBID_CXHMIOBJLib>,
	public CProxy_ICxDrawObjectEvents<CDrawChord>,
	public IConnectionPointContainerImpl<CDrawChord>
{
public:
	CDrawChord()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRAWCHORD)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawChord)
	COM_INTERFACE_ENTRY(IDrawChord)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY/*_IMPL*/(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CXPAGE_MAP()
	CXPAGE_ENTRY(CLSID_CxShapeGenPage)
END_CXPAGE_MAP()

BEGIN_CONNECTION_POINT_MAP(CDrawChord)
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

	CXPROP_ENTRY("EdgeColor", CXPROPID_EdgeColor)
	CXPROP_ENTRY("FillColor", CXPROPID_FillColor)
	CXPROP_ENTRY("HighLightEdgeColor", CXPROPID_HighLightEdgeColor)
	CXPROP_ENTRY("HighLightFillColor", CXPROPID_HighLightFillColor)

	CXPROP_ENTRY("RoateBasePointX", CXPROPID_RoateBasePointX)
	CXPROP_ENTRY("RoateBasePointY", CXPROPID_RoateBasePointY)
	CXPROP_ENTRY("ScaleBasePoint", CXPROPID_ScaleBasePoint)

	CXPROP_ENTRY("JoinStyle", CXPROPID_LineJoinStyle)

	CXPROP_ENTRY("MousePointer", CXPROPID_MousePointer)
END_CXPROP_MAP()

BEGIN_CATEGORY_MAP(CDrawChord)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
	IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawChord) 	
	CATEGORY_PROP_NAME(1, _T("颜色"))
	CATEGORY_PROP_NAME(2, _T("填充"))
	CATEGORY_PROP_NAME(3, _T("变换"))
	CATEGORY_PROP_NAME(4, _T("线型"))
END_CATEGORY_PROP_NAME_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawChord)	
	PROPERTY_TO_CATEGORY(CXPROPID_FillColor,    1)
	//PROPERTY_TO_CATEGORY(CXPROPID_BackGroundColor,    1)
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

	PROPERTY_TO_CATEGORY(CXPROPID_StartX,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_StartY,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_EndX,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_EndY,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_MiddleX,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_MiddleY,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_HorPosition,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_VerPosition,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_Height,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_Width,    -4)
END_PROPERTY_TO_CATEGORY_MAP()
// IDrawChord

public:
	HRESULT DoLoad(DWORD dwVer, LPSTREAM pStm);
	HRESULT DoSave(DWORD dwVer, LPSTREAM pStm);

	void InitChordPath(Gdiplus::GraphicsPath** ppObjectPath, POINTF* points);
	void InitChordPath(Gdiplus::GraphicsPath** ppObjectPath, POINT* points);

	STDMETHOD(GetClassName)(BSTR* pbstrClassName);
//	HRESULT DoDraw(HDC hdcDest, RECT* pRectBounds);
	STDMETHOD(MoveHandleTo)(long nHandle, POINT point);
	STDMETHOD(GetHandleCount)(long* pHandleCount);
	STDMETHOD(GetHandle)(long nHandle, POINT* pHandlePoint);
	STDMETHOD(GetHandleCursor)(long nHandle, HCURSOR* pHandleCursor);
//	STDMETHOD(GetPositionRect)(RECT* pPositionRect);
//	STDMETHOD(GetBoundingRect)(RECT* pBoundingRect);
//	STDMETHOD(Intersects)(RECT rect, BOOL* pIntersected);
	STDMETHOD(DrawTrackerHandleShape)(HDC hdcDest, long nHandle, POINT point);
	STDMETHOD(DrawTrackerShape)(HDC hdcDest, POINT* points, int ptcount);
	STDMETHOD(MoveHandlesTo)(POINT* pPoints, int nCount);

public:
	STDMETHOD(get_MiddleY)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_MiddleY)(/*[in]*/ long newVal);
	STDMETHOD(get_MiddleX)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_MiddleX)(/*[in]*/ long newVal);
	STDMETHOD(get_EndY)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_EndY)(/*[in]*/ long newVal);
	STDMETHOD(get_EndX)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_EndX)(/*[in]*/ long newVal);
	STDMETHOD(get_StartY)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_StartY)(/*[in]*/ long newVal);
	STDMETHOD(get_StartX)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_StartX)(/*[in]*/ long newVal);
	
private:
	class fPOINT
	{
	public:
		double x;
		double y;
		fPOINT()
		{
		}

		fPOINT(POINT pt)
		{
			x = (double)pt.x;
			y = (double)pt.y;
		}
		void CopyTo(POINT& pt)
		{
			pt.x = (long)x;
			pt.y = (long)y;
		}
		fPOINT& operator=(POINT pt)
		{
			x = (double)pt.x;
			y = (double)pt.y;
			return *this;
		}
	};

private:

	BOOL m_MostSphere;
	BOOL ReCalGeometry();

	//起始点
	fPOINT m_ptStart;
	//结束点
	fPOINT m_ptEnd;
	//弧中点
	fPOINT m_ptMiddle;
	//园弧中心点
	fPOINT m_ptCenter;
	//园弧半径
	float m_lRadius;
	//起始角度
	float m_lStartAngle;
	//chord角度
	float m_lSweepAngle;
	//CHORD的外接园矩形
	RECT m_rcRect;
};

#endif //__DRAWCHORD_H_
