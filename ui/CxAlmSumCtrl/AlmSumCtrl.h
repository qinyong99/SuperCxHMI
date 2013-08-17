// AlmSumCtrl.h : Declaration of the CAlmSumCtrl

#ifndef __CXALMSUMCTRL_H_
#define __CXALMSUMCTRL_H_

#include "resource.h"       // main symbols
#include <atlctl.h>

#include "AlmSum.h"
#include "AlmSumDispatchImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CAlmSumCtrl
class ATL_NO_VTABLE CAlmSumCtrl : 
	public CAlmSum,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAlmSumDispatchImpl<CAlmSumCtrl, ICxAlmSumCtrl, &IID_ICxAlmSumCtrl, &LIBID_CXALMSUMCTRLLib>,
	public CComControl<CAlmSumCtrl>,
	public IPersistStreamInitImpl<CAlmSumCtrl>,
	public IOleControlImpl<CAlmSumCtrl>,
	public IOleObjectImpl<CAlmSumCtrl>,
	public IOleInPlaceActiveObjectImpl<CAlmSumCtrl>,
	public IViewObjectExImpl<CAlmSumCtrl>,
	public IOleInPlaceObjectWindowlessImpl<CAlmSumCtrl>,
	public IConnectionPointContainerImpl<CAlmSumCtrl>,
	public IPersistStorageImpl<CAlmSumCtrl>,
	public ISpecifyPropertyPagesImpl<CAlmSumCtrl>,
	public IQuickActivateImpl<CAlmSumCtrl>,
	public IDataObjectImpl<CAlmSumCtrl>,
	public IProvideClassInfo2Impl<&CLSID_AlmSumCtrl, &DIID__ICxAlmSumCtrlEvents, &LIBID_CXALMSUMCTRLLib>,
	public IPropertyNotifySinkCP<CAlmSumCtrl>,
	public CComCoClass<CAlmSumCtrl, &CLSID_AlmSumCtrl>
{
public:
	CAlmSumCtrl() 
	{
		m_bWindowOnly = TRUE;
		m_hDC = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CXALMSUMCTRL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CAlmSumCtrl)
	COM_INTERFACE_ENTRY(ICxAlmSumCtrl)
	COM_INTERFACE_ENTRY(ICxAlmSum)
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

BEGIN_PROP_MAP(CAlmSumCtrl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)

	PROP_PAGE(CLSID_CxGenAlmSumPage)
	PROP_PAGE(CLSID_CxOperatorAlmSumPage)
	PROP_PAGE(CLSID_CxSortAlmSumPage)
	PROP_PAGE(CLSID_CxFilterAlmSumPage)
	PROP_PAGE(CLSID_CxFormatAlmSumPage)
	PROP_PAGE(CLSID_CxColumnAlmSumPage)

	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CAlmSumCtrl)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CAlmSumCtrl)
	CHAIN_MSG_MAP(CComControl<CAlmSumCtrl>)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, DoWindowMouseMessage)
	MESSAGE_HANDLER(WM_LBUTTONUP, DoWindowMouseMessage)
	MESSAGE_HANDLER(WM_RBUTTONUP, DoWindowMouseMessage)
	MESSAGE_HANDLER(WM_MOUSEMOVE, DoWindowMouseMessage)
	MESSAGE_HANDLER(WM_MOUSEWHEEL, DoWindowMouseMessage)
	MESSAGE_HANDLER(WM_KEYDOWN, DoKeyDown)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnSize (UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IPersistStreamInit
public:
	STDMETHOD(InitNew)();
	STDMETHOD(Load)(LPSTREAM pStm);
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty);

public:
	BOOL PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet);
	virtual void OnFinalMessage(HWND /*hWnd*/);
	
	HRESULT OnDraw(ATL_DRAWINFO& di);

	virtual HRESULT FireViewChange();
	virtual BOOL ClientIsRunMode();
	virtual BOOL RecalcRects();
	virtual void SetCapture(BOOL bCapture, CDrawCtrl* pCtrl = NULL);
	virtual HDC GetDC();
	virtual void ReleaseDC(HDC hDC);
	virtual HWND GetClientWindow();
	virtual IUnknown* GetObjectUnknown() { return GetUnknown(); }

	virtual HRESULT OnEditProperties();

	HDC m_hDC;
};

#endif //__CXALMSUMCTRL_H_
