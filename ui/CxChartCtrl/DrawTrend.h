// DrawTrend.h : Declaration of the CDrawTrend

#ifndef __DRAWTREND_H_
#define __DRAWTREND_H_

#include "resource.h"       // main symbols
#include "CxDispid.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"

#include "Chart.h"
#include "TrendDispatchImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawTrend
class ATL_NO_VTABLE CDrawTrend :
	public CChart, 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawTrend, &CLSID_DrawTrend>,
	public CTrendDispatchImpl<CDrawTrend, IDrawTrend, &IID_IDrawTrend, &LIBID_CXCHARTCTRLLib>,
//	public ICxDispatchImpl<CDrawTrend, IDrawTrend, &IID_IDrawTrend, &LIBID_CXDRAWCHARTLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawTrend>,
	public ICxCategorizePropertiesImpl<CDrawTrend>,
	public IProvideClassInfo2Impl<&CLSID_DrawTrend, &DIID__ICxDrawObjectEvents, &LIBID_CXCHARTCTRLLib>,
	public CProxy_ICxDrawObjectEvents<CDrawTrend>,
	public IConnectionPointContainerImpl<CDrawTrend>
{
public:
	CDrawTrend()
	{
		m_enumMousePointer = MousePointerArrow;
		m_nTimeFactor = 0;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRAWTREND)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawTrend)
	COM_INTERFACE_ENTRY(IDrawTrend)
	COM_INTERFACE_ENTRY(ICxTrend)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY/*_IMPL*/(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDrawTrend)
	CONNECTION_POINT_ENTRY(DIID__ICxDrawObjectEvents)
END_CONNECTION_POINT_MAP()

BEGIN_CXPROP_MAP()
	CXPROP_ENTRY("ScrollDirection", 101)
	CXPROP_ENTRY("ZoomMode", 102)
	CXPROP_ENTRY("RefreshInterval", 103)
	CXPROP_ENTRY("ForeColor", 104)
	CXPROP_ENTRY("BackColor", 105)
	CXPROP_ENTRY("FrameStyle", 106)
	CXPROP_ENTRY("FrameColor", 107)
	CXPROP_ENTRY("Transparency", 108)
	CXPROP_ENTRY("ShowMutiGrid", 109)
	CXPROP_ENTRY("ShowMutiTimeAxis", 110)
	CXPROP_ENTRY("ShowMutiValueAxis", 111)
	CXPROP_ENTRY("ShowLegend", 112)
	CXPROP_ENTRY("Caption", 113)
	CXPROP_ENTRY("ScrollPercentage", 116)
	CXPROP_ENTRY("ChartMode", 117)

	CXPROP_ENTRY("MousePointer", CXPROPID_MousePointer)
	CXPROP_ENTRY("Description", CXPROPID_Description)
END_CXPROP_MAP()

BEGIN_CATEGORY_MAP(CDrawTrend)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
	IMPLEMENTED_CATEGORY(CATID_Insertable)
	IMPLEMENTED_CATEGORY(CATID_Control)
	IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawTrend) 
	CATEGORY_PROP_NAME(1, _T("Õ‚π€"))
END_CATEGORY_PROP_NAME_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawTrend)	
	PROPERTY_TO_CATEGORY(104, 1)
	PROPERTY_TO_CATEGORY(105, 1)
	PROPERTY_TO_CATEGORY(106, 1)
	PROPERTY_TO_CATEGORY(107, 1)
	PROPERTY_TO_CATEGORY(108, 1)
	PROPERTY_TO_CATEGORY(109, 1)
	PROPERTY_TO_CATEGORY(110, 1)
	PROPERTY_TO_CATEGORY(111, 1)
	PROPERTY_TO_CATEGORY(112, 1)
	PROPERTY_TO_CATEGORY(113, 1)
	PROPERTY_TO_CATEGORY(114, 1)
	PROPERTY_TO_CATEGORY(115, 1)
	PROPERTY_TO_CATEGORY(116, 1)
END_PROPERTY_TO_CATEGORY_MAP()

BEGIN_CXPAGE_MAP()
	CXPAGE_ENTRY(CLSID_CxGenTrendPage)
	CXPAGE_ENTRY(CLSID_CxTrendCtrlPage)
END_CXPAGE_MAP()

public:
	STDMETHOD(GetClassName)(BSTR* pbstrClassName);
	STDMETHOD(MoveHandleTo)(long nHandle, POINT point);
	STDMETHOD(DrawTrackerHandleShape)(HDC hdcDest, long nHandle, POINT point);
	STDMETHOD(DrawTrackerShape)(HDC hdcDest, POINT* pPoints, int nCount);
	STDMETHOD(RotateAt)(POINTF point, float fAngle);
	STDMETHOD(ScaleAt)(POINTF point, float fHorScale, float fVerScale);
	STDMETHOD(DrawRotateAtShape)(HDC hdcDest, POINTF point, long lAngle);

	STDMETHOD(InitNew)();
	STDMETHOD(SaveOldVer)(LPSTREAM pStm, BOOL fClearDirty); 
	STDMETHOD(LoadOldVer)(LPSTREAM pStm);

	STDMETHOD(InitRun)(BOOL* pbHasDynamic, ICxScriptEngineSite* pScriptSite);
	STDMETHOD(FakeParseDynamic)();
	STDMETHOD(OnTimer)();

	STDMETHOD(HitTest)(POINT point, BOOL* bHitTest);
	STDMETHOD(Intersects)(RECT rect, BOOL* bIntersects);
	STDMETHOD(MoveTo)(RECTF rect, BOOL bUniformScale);
	STDMETHOD(GetPositionRect)(RECTF* pPositionRect);
	STDMETHOD(GetBoundingRect)(RECT* pBoundingRect);
	STDMETHOD(GetHandleCount)(long* pHandleCount);

	virtual BOOL RecalcRects();
	virtual HRESULT FireViewChange();
	virtual BOOL ClientIsRunMode();
	virtual BOOL OnRefreshSerialData(CSerial* pSerial);
	virtual void GetDefaultServer(BSTR* pbstrComputer);
		
	HRESULT DoDraw(HDC hdcDest);
	HRESULT DoPrint(HDC hdcDest) { return DoDraw(hdcDest); }
	HRESULT DoLoad(DWORD dwVer, LPSTREAM pStm);
	HRESULT DoSave(DWORD dwVer, LPSTREAM pStm);

// IDrawTrend
public:
	STDMETHOD(get_Height)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_Height)(/*[in]*/ float newVal);
	STDMETHOD(get_Width)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_Width)(/*[in]*/ float newVal);
	STDMETHOD(get_VerticalPosition)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_VerticalPosition)(/*[in]*/ float newVal);
	STDMETHOD(get_HorizontalPosition)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_HorizontalPosition)(/*[in]*/ float newVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pbstrName);
	STDMETHOD(put_Name)(/*[in]*/ BSTR bstrName);
	STDMETHOD(get_Visible)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Visible)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Layer)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_Layer)(/*[in]*/ int newVal);
	STDMETHOD(get_Locked)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Locked)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Privilege)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_Privilege)(/*[in]*/ int newVal);
	STDMETHOD(get_MousePointer)(/*[out, retval]*/ enumMousePointer *pVal);
	STDMETHOD(put_MousePointer)(/*[in]*/ enumMousePointer newVal);
	STDMETHOD(get_Description)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Description)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ToolTipText)(/*[out, retval]*/ BSTR *pbstrText);
	STDMETHOD(put_ToolTipText)(/*[in]*/ BSTR bstrText);

	STDMETHOD(IsConnected)(BSTR bstrPropertyName, VARIANT_BOOL* pbHasConnection);
	STDMETHOD(ConnectObject)(BSTR bstrPropertyName, IUnknown* punkObject);
	STDMETHOD(ConnectDirect)(BSTR bstrPropertyName, BSTR bstrDataSource);
	STDMETHOD(GetConnectObject)(BSTR bstrPropertyName, IUnknown** ppunkObject);
	STDMETHOD(Disconnect)(BSTR bstrPropertyName);
	STDMETHOD(GetConnectState)(DISPID dispid, int* pState);

	STDMETHOD(EditProperties)();
	STDMETHOD(EditSerials)();

public:
	RECTF m_rcPosition;	

	int m_nTimeFactor;
	
	CComPtr<ICxScriptEngineSite> m_spScriptSite;
};

#endif //__DRAWTREND_H_
