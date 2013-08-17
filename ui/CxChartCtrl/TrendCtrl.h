// TrendCtrl.h : Declaration of the CTrendCtrl

#ifndef __CXTRENDCTRL_H_
#define __CXTRENDCTRL_H_

#include "resource.h"       // main symbols
#include <atlctl.h>

#include "Chart.h"
#include "TrendDispatchImpl.h"
#include "ToolPane.h"

#include "Label.h"
#include "DataNotation.h"


/////////////////////////////////////////////////////////////////////////////
// CTrendCtrl
class ATL_NO_VTABLE CTrendCtrl :
	public CChart,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CTrendDispatchImpl<CTrendCtrl, ICxTrendCtrl, &IID_ICxTrendCtrl, &LIBID_CXCHARTCTRLLib>,
	public CComControl<CTrendCtrl>,
	public IPersistStreamInitImpl<CTrendCtrl>,
	public IOleControlImpl<CTrendCtrl>,
	public IOleObjectImpl<CTrendCtrl>,
	public IOleInPlaceActiveObjectImpl<CTrendCtrl>,
	public IViewObjectExImpl<CTrendCtrl>,
	public IOleInPlaceObjectWindowlessImpl<CTrendCtrl>,
	public IConnectionPointContainerImpl<CTrendCtrl>,
	public IPersistStorageImpl<CTrendCtrl>,
	public ISpecifyPropertyPagesImpl<CTrendCtrl>,
	public IQuickActivateImpl<CTrendCtrl>,
	public IDataObjectImpl<CTrendCtrl>,
	public IProvideClassInfo2Impl<&CLSID_TrendCtrl, &DIID__ICxTrendCtrlEvents, &LIBID_CXCHARTCTRLLib>,
	public IPropertyNotifySinkCP<CTrendCtrl>,
	public CComCoClass<CTrendCtrl, &CLSID_TrendCtrl>
{
public:
	CTrendCtrl() :
		m_ptCurrentPoint(-1, -1),
		m_ptLastPoint(-1, -1),
		m_nSnappedCurve(-1)
	{
		m_bWindowOnly = TRUE;

		m_apChartObject.Add(&m_lblCursorLegend); 
		m_pCurrentObject = NULL;

		m_dtDefaultCurrentTime = COleDateTime::GetCurrentTime();

		m_nTimer = -1;

		m_nTimeFactor = 0;
	}
	virtual ~CTrendCtrl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CXTRENDCTRL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTrendCtrl)
	COM_INTERFACE_ENTRY(ICxTrendCtrl)
	COM_INTERFACE_ENTRY(ICxTrend)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_PROP_MAP(CTrendCtrl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	PROP_PAGE(CLSID_CxGenTrendPage)
	PROP_PAGE(CLSID_CxTrendCtrlPage)
	PROP_ENTRY("ScrollDirection", 101, CLSID_NULL)
	PROP_ENTRY("ZoomMode", 102, CLSID_NULL)
	PROP_ENTRY("RefreshInterval", 103, CLSID_NULL)
	PROP_ENTRY("ForeColor", 104, CLSID_NULL)
	PROP_ENTRY("BackColor", 105, CLSID_NULL)
	PROP_ENTRY("FrameStyle", 106, CLSID_NULL)
	PROP_ENTRY("FrameColor", 107, CLSID_NULL)
	PROP_ENTRY("Transparency", 108, CLSID_NULL)
	PROP_ENTRY("ShowMutiGrid", 109, CLSID_NULL)
	PROP_ENTRY("ShowMutiTimeAxis", 110, CLSID_NULL)
	PROP_ENTRY("ShowMutiValueAxis", 111, CLSID_NULL)
	PROP_ENTRY("ShowLegend", 112, CLSID_NULL)
	PROP_ENTRY("Caption", 113, CLSID_NULL)
	PROP_ENTRY("CaptionFont", 114, CLSID_NULL)
	PROP_ENTRY("CaptionColor", 115, CLSID_NULL)
	PROP_ENTRY("ScrollPercentage", 116, CLSID_NULL)
	PROP_ENTRY("TrendMode", 117, CLSID_NULL)
	PROP_ENTRY("ShowToolPane", 155, CLSID_NULL)
	PROP_ENTRY("ShowList", 156, CLSID_NULL)
	PROP_ENTRY("ShowCursor", 157, CLSID_NULL)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CTrendCtrl)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CTrendCtrl)
	CHAIN_MSG_MAP(CComControl<CTrendCtrl>)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUP)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// window message
private:
	LRESULT OnCreate(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize (UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IPersistStreamInit
public:
	STDMETHOD(InitNew)();
	STDMETHOD(Load)(LPSTREAM pStm);
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty);

// ICxTrendCtrl
public:
	STDMETHOD(EditProperties)();
	STDMETHOD(EditSerials)();
	STDMETHOD(InsertDataNotation)(/*[in]*/ int nCurve, /*[in]*/ int nIndex);

	STDMETHOD(get_ShowToolPane)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowToolPane)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ShowCursor)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowCursor)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ShowList)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowList)(/*[in]*/ VARIANT_BOOL newVal);

	STDMETHOD(Zoom)(/*[in]*/ float fYHi, /*[in]*/ float fYLo, /*[in]*/ float fXHi, /*[in]*/ float fXLo);
	STDMETHOD(put_ZoomMode)(/*[in]*/ enumZoomMode newVal);
	STDMETHOD(put_TrendMode)(/*[in]*/ enumTrendMode newVal);

public:
	BOOL PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet);
	HRESULT OnDraw(ATL_DRAWINFO& di);

	void DrawCursor(HDC hdcDest);
	void DrawCursorLegend(HDC hdcDest);

	void CreateToolPane();
	
	virtual HRESULT FireViewChange();
	virtual BOOL ClientIsRunMode();
	virtual BOOL RecalcRects();

	UINT m_nTimer;

	CToolPane m_wndToolPane;
	
	VARIANT_BOOL m_bShowToolPane;
	VARIANT_BOOL m_bShowList;

	VARIANT_BOOL m_bShowCursor;
	CPoint m_ptCurrentPoint;
	CPoint m_ptLastPoint;

	int m_nSnappedCurve;
	double m_dSnappedXVal;
	double m_dSnappedYVal;

	CLabel m_lblCursorLegend;

	CSimpleValArray<CChartObject *> m_apChartObject;
	CChartObject* m_pCurrentObject;

	COleDateTime m_dtDefaultCurrentTime;

	int m_nTimeFactor;
};

#endif //__CXTRENDCTRL_H_
