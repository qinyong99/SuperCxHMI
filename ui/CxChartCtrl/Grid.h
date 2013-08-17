// Grid.h: Definition of the CGrid class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRID_H__98D3DF92_9D1A_4BDA_A20A_92DBB0F14C33__INCLUDED_)
#define AFX_GRID_H__98D3DF92_9D1A_4BDA_A20A_92DBB0F14C33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGrid

class CGrid : 
	public IDispatchImpl<IGrid, &IID_IGrid, &LIBID_CXCHARTCTRLLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot
{
public:
	CGrid() 
	{
		m_pChart = NULL;
	}
BEGIN_COM_MAP(CGrid)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IGrid)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CGrid) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_NOT_AGGREGATABLE(CGrid)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IGrid
public:
	STDMETHOD(get_ShowVertical)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowVertical)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ShowHorizontal)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowHorizontal)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_NumberOfVerticalLines)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_NumberOfVerticalLines)(/*[in]*/ long newVal);
	STDMETHOD(get_NumberOfHorizontalLines)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_NumberOfHorizontalLines)(/*[in]*/ long newVal);
	STDMETHOD(get_VerticalStyle)(/*[out, retval]*/ enumLineStyle *pVal);
	STDMETHOD(put_VerticalStyle)(/*[in]*/ enumLineStyle newVal);
	STDMETHOD(get_HorizontalStyle)(/*[out, retval]*/ enumLineStyle *pVal);
	STDMETHOD(put_HorizontalStyle)(/*[in]*/ enumLineStyle newVal);
	STDMETHOD(get_Width)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Width)(/*[in]*/ long newVal);
	STDMETHOD(get_VerticalColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_VerticalColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_HorizontalColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_HorizontalColor)(/*[in]*/ OLE_COLOR newVal);

public:
	STDMETHOD(InitNew)();
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty); 
	STDMETHOD(Load)(LPSTREAM pStm);

	void DoDraw(HDC hdcDest, RECT& rect);

	VARIANT_BOOL m_bShowVertical;
	VARIANT_BOOL m_bShowHorizontal;
	long m_lNumberOfVerticalLines;
	long m_lNumberOfHorizontalLines;
	enumLineStyle m_enumVerticalStyle;
	enumLineStyle m_enumHorizontalStyle;
	long m_nWidth;
	OLE_COLOR m_clrVerticalColor;
	OLE_COLOR m_clrHorizontalColor;

	CChart* m_pChart;
};

#endif // !defined(AFX_GRID_H__98D3DF92_9D1A_4BDA_A20A_92DBB0F14C33__INCLUDED_)
