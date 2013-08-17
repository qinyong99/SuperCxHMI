// DataLinkTool.h : Declaration of the CDataLinkTool

#ifndef __DATALINKTOOL_H_
#define __DATALINKTOOL_H_

#include "resource.h"       // main symbols
#include "CxDrawTool.h"

/////////////////////////////////////////////////////////////////////////////
// CDataLinkTool
class ATL_NO_VTABLE CDataLinkTool : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDataLinkTool, &CLSID_DataLinkTool>,
	public CCxDrawToolBase,
	public IConnectionPointContainerImpl<CDataLinkTool>,
	public CProxy_ICxDrawToolEvents<CDataLinkTool>,
	public ICxDrawToolImpl<CDataLinkTool>
{
public:
	CDataLinkTool()
	{
		m_hCursor = NULL;
	}
	~CDataLinkTool()
	{
		if (m_hCursor != NULL)
			::DeleteObject(m_hCursor);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DATALINKTOOL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDataLinkTool)
	COM_INTERFACE_ENTRY(ICxDrawTool)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDataLinkTool)
	CONNECTION_POINT_ENTRY(IID_ICxDrawToolEvents)
END_CONNECTION_POINT_MAP()

// IDataLinkTool
public:
	STDMETHOD(DoDraw)(HDC hdcDest);
	STDMETHOD(DoInit)();
	STDMETHOD(DoLButtonDown)(UINT nFlags, POINT point);
	STDMETHOD(DoMouseMove)(UINT nFlags, POINT point);
	STDMETHOD(DoCancel)();

private:
	CComPtr<IUnknown> m_spFormatDynamic;

	CComBSTR m_bstrSample;

	HCURSOR m_hCursor;
};

#endif //__DATALINKTOOL_H_
