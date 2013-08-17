// PieTool.h : Declaration of the CPieTool

#ifndef __PIETOOL_H_
#define __PIETOOL_H_

#include "resource.h"       // main symbols
#include "CxDrawTool.h"

/////////////////////////////////////////////////////////////////////////////
// CPieTool
class ATL_NO_VTABLE CPieTool : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPieTool, &CLSID_PieTool>,
	public IConnectionPointContainerImpl<CPieTool>,
	public CCxDrawToolBase,
	public CProxy_ICxDrawToolEvents<CPieTool>,
	public ICxDrawToolImpl<CPieTool>
{
public:
	CPieTool();
	

DECLARE_REGISTRY_RESOURCEID(IDR_PIETOOL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPieTool)
	COM_INTERFACE_ENTRY(ICxDrawTool)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CPieTool)
	CONNECTION_POINT_ENTRY(IID_ICxDrawToolEvents)
END_CONNECTION_POINT_MAP()

// IPieTool
public:
	POINT m_points[4];
	int m_nPoint;
	ICxDrawObject* m_pDrawObject;
	BOOL m_bIsFirstMove3thPt;

	STDMETHOD(DoMouseMove)(UINT nFlags, POINT point);
	STDMETHOD(DoLButtonDown)(UINT nFlags, POINT point);
	STDMETHOD(DoCancel)();
	STDMETHOD(DoKeyDown)(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (m_nPoint > 0)
			PressCursorKey(nChar);

		return S_OK;
	}

};

#endif //__PIETOOL_H_
