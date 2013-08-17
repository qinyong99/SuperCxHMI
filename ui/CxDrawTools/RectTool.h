// RectTool.h : Declaration of the CRectTool

#ifndef __RECTTOOL_H_
#define __RECTTOOL_H_

#include "resource.h"       // main symbols
#include "CxDrawTool.h"

/////////////////////////////////////////////////////////////////////////////
// CRectTool
class ATL_NO_VTABLE CRectTool : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRectTool, &CLSID_RectTool>,
	public CCxDrawToolBase,
	public IConnectionPointContainerImpl<CRectTool>,
	public CProxy_ICxDrawToolEvents<CRectTool>,
	public ICxDrawToolImpl<CRectTool>
{
public:
	CRectTool()
	{
		m_pDrawObject = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_RECTTOOL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CRectTool)
	COM_INTERFACE_ENTRY(ICxDrawTool)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CRectTool)
	CONNECTION_POINT_ENTRY(IID_ICxDrawToolEvents)
END_CONNECTION_POINT_MAP()

// IRectTool
public:
	ICxDrawObject* m_pDrawObject;
	RECTF m_rect;
	POINT m_ptPreMove;
	BOOL m_bIsMouseMoved;

	STDMETHOD(DoLButtonUp)(UINT nFlags, POINT point);
	STDMETHOD(DoMouseMove)(UINT nFlags, POINT point);
	STDMETHOD(DoLButtonDown)(UINT nFlags, POINT point);
	STDMETHOD(DoCancel)();
	STDMETHOD(DoKeyDown)(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (m_pDrawObject)
			PressCursorKey(nChar);

		return S_OK;
	}

};

#endif //__RECTTOOL_H_
