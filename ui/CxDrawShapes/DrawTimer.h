// DrawTimer.h : Declaration of the CDrawTimer

#ifndef __DRAWTIMER_H_
#define __DRAWTIMER_H_

#include "resource.h"       // main symbols
#include "CxDispid.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"
#include "gdiplus.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawTimer

class ATL_NO_VTABLE CDrawTimer : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawTimer, &CLSID_DrawTimer>,
	public ICxDispatchImpl<CDrawTimer, IDrawTimer, &IID_IDrawTimer, &LIBID_CXHMIOBJLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawTimer>,
//	public ICxPerPropertyEditImpl<CDrawTimer>,
	public ICxCategorizePropertiesImpl<CDrawTimer>,
	public IProvideClassInfo2Impl<&CLSID_DrawTimer, &DIID_IDrawTimerEvents, &LIBID_CXHMIOBJLib>,
	public CProxy_ICxObjectBaseEvents<CDrawTimer, &DIID_IDrawTimerEvents>,
	public IConnectionPointContainerImpl<CDrawTimer>,
	public CWindowImpl<CDrawTimer, CWindow, CNullTraits>
{
public:
	CDrawTimer()
	{
		m_uTimer = -1; 
		m_bRun = FALSE;
	}

	virtual ~CDrawTimer()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRAWTIMER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawTimer)
	COM_INTERFACE_ENTRY(IDrawTimer)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CXPAGE_MAP()
  	CXPAGE_ENTRY(CLSID_CxTimerGenPage)
END_CXPAGE_MAP()	

BEGIN_CONNECTION_POINT_MAP(CDrawTimer)
	CONNECTION_POINT_ENTRY(DIID_IDrawTimerEvents)
END_CONNECTION_POINT_MAP()

BEGIN_CXPROP_MAP()
	CXPROP_ENTRY("Enabled", CXPROPID_Enabled)
	CXPROP_ENTRY("Interval", 2)
END_CXPROP_MAP()

BEGIN_CATEGORY_MAP(CDrawTimer)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
	IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawTimer) 	
 	CATEGORY_PROP_NAME(1, _T("¶¨Ê±"))
END_CATEGORY_PROP_NAME_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawTimer)
	PROPERTY_TO_CATEGORY(CXPROPID_Enabled,    1)
	PROPERTY_TO_CATEGORY(2,    1)
END_PROPERTY_TO_CATEGORY_MAP()

BEGIN_MSG_MAP(CTimerWnd)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
END_MSG_MAP()

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

// IDrawTimer
public:
	STDMETHOD(get_Enabled)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Enabled)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Interval)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_Interval)(/*[in]*/ int newVal);

public:
	HRESULT DoLoad(DWORD dwVer, LPSTREAM pStm);
	HRESULT DoSave(DWORD dwVer, LPSTREAM pStm);

	STDMETHOD(GetClassName)(BSTR* pbstrClassName);
	STDMETHOD(MoveHandleTo)(long nHandle, POINT point);
	STDMETHOD(DrawTrackerHandleShape)(HDC hdcDest, long nHandle, POINT point);
	STDMETHOD(DrawTrackerShape)(HDC hdcDest, POINT* pPoints, int nCount);
	STDMETHOD(RotateAt)(POINTF point, float fAngle);
	STDMETHOD(ScaleAt)(POINTF point, float fHorScale, float fVerScale);
	STDMETHOD(DrawRotateAtShape)(HDC hdcDest, POINTF point, long lAngle);

	STDMETHOD(HitTest)(POINT point, BOOL* bHitTest);
	STDMETHOD(Intersects)(RECT rect, BOOL* bIntersects);
	STDMETHOD(MoveTo)(RECTF rect, BOOL bUniformScale);
	STDMETHOD(GetPositionRect)(RECTF* pPositionRect);
	STDMETHOD(GetBoundingRect)(RECT* pBoundingRect);
	STDMETHOD(GetHandleCount)(long* pHandleCount);

	virtual HRESULT DoDraw(HDC hdcDest);

	STDMETHOD(InitNew)();
	STDMETHOD(InitRun)(BOOL* pbHasDynamic, ICxScriptEngineSite* pScriptSite);

	void ResetTimer();
	void FinalRelease();

	VARIANT_BOOL m_bEnabled;
	int m_nInterval;

	UINT m_uTimer;
	BOOL m_bRun;

	RECTF m_rcPosition;	
};

#endif //__DRAWTIMER_H_
