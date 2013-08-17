// PolyTool.h : Declaration of the CPolyTool

#ifndef __POLYTOOL_H_
#define __POLYTOOL_H_

#include "resource.h"       // main symbols
#include "CxDrawTool.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CPolyTool
class ATL_NO_VTABLE CPolyTool : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPolyTool, &CLSID_PolyTool>,
	public CCxDrawToolBase,
	public IConnectionPointContainerImpl<CPolyTool>,
	public CProxy_ICxDrawToolEvents<CPolyTool>,
	public ICxDrawToolImpl<CPolyTool>

{
	
public:
	CPolyTool()
	{
		m_bDrawing = FALSE;
		m_points.clear();
	}

DECLARE_REGISTRY_RESOURCEID(IDR_POLYTOOL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPolyTool)
	COM_INTERFACE_ENTRY(ICxDrawTool)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CPolyTool)
	CONNECTION_POINT_ENTRY(IID_ICxDrawToolEvents)
END_CONNECTION_POINT_MAP()

// IPolyTool
public:
	STDMETHOD(DoLButtonDown)(UINT nFlags, POINT point);
	STDMETHOD(DoMouseMove)(UINT nFlags, POINT point);
	STDMETHOD(DoLButtonDblClk)(UINT nFlags, POINT point);
	HRESULT XorDoDraw(HDC hdcDest);
	STDMETHODIMP DoCancel();
	STDMETHOD(DoKeyDown)(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (m_bDrawing)
			PressCursorKey(nChar);

		return S_OK;
	}
private:
	std::vector<POINT> m_points;
	POINT m_point;
	BOOL m_bDrawing;
};

#endif //__POLYTOOL_H_
