// Legend.h: Definition of the Legend class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEGEND_H__14318303_E5B2_4AF1_B298_C6F8A5F7CE0C__INCLUDED_)
#define AFX_LEGEND_H__14318303_E5B2_4AF1_B298_C6F8A5F7CE0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

class CSerial;

/////////////////////////////////////////////////////////////////////////////
// Legend

class CLegend : 
	public IDispatchImpl<ILegend, &IID_ILegend, &LIBID_CXCHARTCTRLLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot
{
public:
	CLegend() 
	{
		m_pSerial = NULL;
	}
BEGIN_COM_MAP(CLegend)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ILegend)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(Legend) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_NOT_AGGREGATABLE(CLegend)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	STDMETHOD(InitNew)();
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty); 
	STDMETHOD(Load)(LPSTREAM pStm);

// ILegend
public:
	STDMETHOD(get_Show)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Show)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Description)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Description)(/*[in]*/ BSTR newVal);
	STDMETHOD(put_TextFont)(IFontDisp* pFont);
	STDMETHOD(putref_TextFont)(IFontDisp* pFont);
	STDMETHOD(get_TextFont)(IFontDisp** ppFont);
	STDMETHOD(get_TextColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_TextColor)(/*[in]*/ OLE_COLOR newVal);

	void RecalcRect(RECT& rect);
	void CalcLargestWidth(int& nWidth);
	void AdjustHeight(int nHeight);
	
	void DoDraw(HDC hdcDest);

	RECT m_rect;

	VARIANT_BOOL m_bShow;
	CComBSTR m_bstrDesc;
	CComPtr<IFontDisp> m_spTextFont;
	OLE_COLOR m_clrText;

	CSerial* m_pSerial;
};

#endif // !defined(AFX_LEGEND_H__14318303_E5B2_4AF1_B298_C6F8A5F7CE0C__INCLUDED_)
