// DrawPipe.h : Declaration of the CDrawPipe

#ifndef __DRAWPIPE_H_
#define __DRAWPIPE_H_

#include "resource.h"       // main symbols

#include "CxDispid.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"

class CNodes;

/////////////////////////////////////////////////////////////////////////////
// CDrawPipe
class ATL_NO_VTABLE CDrawPipe : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawPipe, &CLSID_DrawPipe>,
	public ICxDispatchImpl<CDrawPipe, IDrawPipe, &IID_IDrawPipe, &LIBID_CXHMIOBJLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawPipe>,
	public ICxCategorizePropertiesImpl<CDrawPipe>,
	public IProvideClassInfo2Impl<&CLSID_DrawPipe, &DIID__ICxDrawObjectEvents, &LIBID_CXHMIOBJLib>,
	public CProxy_ICxDrawObjectEvents<CDrawPipe>,
	public IConnectionPointContainerImpl<CDrawPipe>
{
	
public:
	typedef struct _LINESARRAY
	{	//一个线段的集合体
		int nArraySize;	//int bytes
		int nCount;		//items count
		COLORREF clrColor;
		RECT* pLines;
	} LINESARRAY;

	typedef struct _CIRCLEARRAY
	{	//接头的集合体
		int nArraySize;
		int nCount;
		RECT* pCircles;
	} CIRCLEARRAY;


public:
	CDrawPipe()
	{
		m_apLines = NULL;
		m_apCircles = NULL;
		m_nAllocWidth = 0;

		m_nFlowStartLenth = 0;
		m_enumEdgeStyle = EdgeStylePipe;

		m_nTimeFactor = 0;

		m_bNeedRecalGeometry = TRUE;
	}

	~CDrawPipe()
	{
		if (m_apLines != NULL)
		{
			for(int i = 0; i < m_nAllocWidth; i++)
			{
				if (m_apLines[i].pLines != NULL)
					delete m_apLines[i].pLines;
			}

			delete [] m_apLines;
		}
		
		if (m_apCircles != NULL)
		{
			for(int i = 0; i < m_nAllocWidth; i++)
			{
				if (m_apCircles[i].pCircles != NULL)
					delete m_apCircles[i].pCircles;
			}

			delete [] m_apCircles;
		}
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRAWPIPE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawPipe)
	COM_INTERFACE_ENTRY(IDrawPipe)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDrawPipe)
	CONNECTION_POINT_ENTRY(DIID__ICxDrawObjectEvents)
END_CONNECTION_POINT_MAP()

BEGIN_CXPROP_MAP()
	CXPROP_ENTRY("LiquidColor", CXPROPID_LiquidColor)
	CXPROP_ENTRY("Flow", CXPROPID_Flow)
	CXPROP_ENTRY("FlowDirection", CXPROPID_FlowDirection)
	CXPROP_ENTRY("FlowSpeed", CXPROPID_FlowSpeed)
	
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

BEGIN_CATEGORY_MAP(CDrawPipe)
//	IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
//	IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
//	IMPLEMENTED_CATEGORY(CATID_Insertable)
//	IMPLEMENTED_CATEGORY(CATID_Control)
	IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawPolygon) 	
	CATEGORY_PROP_NAME(1, _T("颜色"))
	CATEGORY_PROP_NAME(3, _T("变换"))
	CATEGORY_PROP_NAME(4, _T("线型"))
	CATEGORY_PROP_NAME(5, _T("液体"))
END_CATEGORY_PROP_NAME_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawPipe)	
	PROPERTY_TO_CATEGORY(CXPROPID_EdgeColor,    1)
	PROPERTY_TO_CATEGORY(CXPROPID_HighLightEdgeColor,    1)
	
	PROPERTY_TO_CATEGORY(CXPROPID_RotateAngle,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_ScaleBasePoint,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_HorScale,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_VerScale,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_RoateBasePointX,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_RoateBasePointY,    3)

	PROPERTY_TO_CATEGORY(CXPROPID_EdgeWidth,  4)
	PROPERTY_TO_CATEGORY(CXPROPID_LineJoinStyle,  4)
	PROPERTY_TO_CATEGORY(CXPROPID_LineJoinStyle,  4)

	PROPERTY_TO_CATEGORY(CXPROPID_LiquidColor,    5)
	PROPERTY_TO_CATEGORY(CXPROPID_Flow,    5)
	PROPERTY_TO_CATEGORY(CXPROPID_FlowDirection,    5)
	PROPERTY_TO_CATEGORY(CXPROPID_FlowSpeed,    5)

	PROPERTY_TO_CATEGORY(CXPROPID_HorPosition,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_VerPosition,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_Height,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_Width,    -4)
END_PROPERTY_TO_CATEGORY_MAP()

BEGIN_CXPAGE_MAP()
	CXPAGE_ENTRY(CLSID_CxPipeGenPage)
END_CXPAGE_MAP()		

public:
	HRESULT DoDraw(HDC hdcDest);
	HRESULT DoPrint(HDC hdcDest) { return DoDraw(hdcDest); }
	HRESULT DoLoad(DWORD dwVer, LPSTREAM pStm);
	HRESULT DoSave(DWORD dwVer, LPSTREAM pStm);

	STDMETHOD(InitNew)();
	STDMETHOD(GetHandleCount)(long* pHandleCount);
	STDMETHOD(GetHandle)(long nHandle, POINT* pHandlePoint);
	STDMETHOD(HitTest)(POINT point, BOOL* bHitTest);
	STDMETHOD(Intersects)(RECT rect, BOOL* pbIntersects);
	STDMETHOD(MoveTo)(RECTF rect, BOOL bUniformScale);
	STDMETHOD(Rotate)(long lAngle);
	STDMETHOD(RotateAt)(POINTF point, float fAngle);
	
// ICxDrawObject
	STDMETHOD(MoveHandleTo)(long nHandle, POINT point);
	STDMETHOD(GetHandleCursor)(long nHandle, HCURSOR* pHandleCursor);
	STDMETHOD(GetClassName)(BSTR* pbstrClassName);

	STDMETHOD(InitRun)(BOOL* pbHasDynamic, ICxScriptEngineSite* pScriptSite);
	STDMETHOD(OnTimer)();
	STDMETHOD(DrawTrackerHandleShape)(HDC hdcDest, long nHandle, POINT point);
	
// IDrawPipe
public:
	STDMETHOD(get_LiquidColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_LiquidColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_FlowSpeed)(/*[out, retval]*/ enumFlowSpeed *pVal);
	STDMETHOD(put_FlowSpeed)(/*[in]*/ enumFlowSpeed newVal);
	STDMETHOD(get_FlowDirection)(/*[out, retval]*/ enumFlowDirection *pVal);
	STDMETHOD(put_FlowDirection)(/*[in]*/ enumFlowDirection newVal);
	STDMETHOD(get_EdgeWidth)(float *pVal);
	STDMETHOD(put_EdgeWidth)(float newVal);
	STDMETHOD(put_EdgeStyle)(enumEdgeStyle newVal);

	STDMETHOD(get_Flow)(VARIANT_BOOL *pVal);
	STDMETHOD(put_Flow)(VARIANT_BOOL newVal);
	
	STDMETHOD(put_EdgeColor)(OLE_COLOR newVal);
	STDMETHOD(put_HighLightEdgeColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(put_JoinStyle)(/*[in]*/ enumLineJoinStyle newVal);

	STDMETHOD(GetPointAt)(/*[in]*/ long lIndex, /*[out, retval]*/ IFloatPoint** ppPoint);
	STDMETHOD(DeletePoint)(long lIndex);
	STDMETHOD(GetNumberOfPoints)(/*[out, retval]*/ long *pVal);
	STDMETHOD(AddPoint)(IFloatPoint* pPoint);

public:
	void AddCircle(RECT& rect, int index);
	void AddLine(int x1, int y1, int x2, int y2, int index);
	void CalcuLineSegment( int x1, int y1, int x2, int y2);

	//针对管道液体流动刷新优化
	HRESULT FireViewChangeEx();

	void RecalGeometry();

	
private:
	int m_nFlowStartLenth;
	BOOL m_bFlowShow;	//是否流动
	enumFlowDirection m_enumFlowDirection; //流动方向
	enumFlowSpeed m_enumFlowSpeed;
	OLE_COLOR m_clrLiquid;

	LINESARRAY* m_apLines;
	CIRCLEARRAY* m_apCircles;
	int m_nAllocWidth;

	BOOL ReAllocWidth(int nWidth);

	BOOL m_bNeedRecalGeometry;
	
//	SIZE m_szLastZoomNum;
//	SIZE m_szLastZoomDenom;

	int m_nTimeFactor;
};

#endif //__DRAWPIPE_H_
