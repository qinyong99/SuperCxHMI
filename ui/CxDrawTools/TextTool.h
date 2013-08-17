// TextTool.h : Declaration of the CTextTool

#ifndef __TEXTTOOL_H_
#define __TEXTTOOL_H_

#include "resource.h"       // main symbols
#include "CxDrawTool.h"
#include <comdef.h>
#include "string.h"
#define	STR_MAX_LENGTH 4096

/////////////////////////////////////////////////////////////////////////////
// CTextTool
class ATL_NO_VTABLE CTextTool : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTextTool, &CLSID_TextTool>,
	public CCxDrawToolBase,
	public IConnectionPointContainerImpl<CTextTool>,
	public CProxy_ICxDrawToolEvents<CTextTool>,
	public ICxDrawToolImpl<CTextTool>
{
public:
	CTextTool()
	{	
		m_pDrawTxt = NULL;
		ZeroMemory(m_TextBuf, STR_MAX_LENGTH);
		m_hWindow = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TEXTTOOL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTextTool)
	COM_INTERFACE_ENTRY(ICxDrawTool)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CTextTool)
	CONNECTION_POINT_ENTRY(IID_ICxDrawToolEvents)
END_CONNECTION_POINT_MAP()

public:
	STDMETHOD(DoDraw)(HDC hdcDest);
	STDMETHOD(DoChar)(UINT nChar, UINT nRepCnt, UINT nFlags);
	STDMETHOD(DoLButtonDown)(UINT nFlags, POINT point);
	STDMETHOD(DoMouseMove)(UINT nFlags, POINT point);
	STDMETHOD(DoCancel)();

// ITextTool
public:
	char m_TextBuf[STR_MAX_LENGTH];
	IDrawTxt* m_pDrawTxt;
	HWND m_hWindow;
	
	void RePosCaret();
};

#endif //__TEXTTOOL_H_
