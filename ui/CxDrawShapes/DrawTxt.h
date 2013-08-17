// DrawTxt.h : Declaration of the CDrawTxt

#ifndef __DRAWTXT_H_
#define __DRAWTXT_H_

#include "resource.h"       // main symbols
#include "CxDispid.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"
#include <comdef.h>


/////////////////////////////////////////////////////////////////////////////
// CDrawTxt
class ATL_NO_VTABLE CDrawTxt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawTxt, &CLSID_DrawTxt>,
	public ICxDispatchImpl<CDrawTxt, IDrawTxt, &IID_IDrawTxt, &LIBID_CXHMIOBJLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawTxt>,
	public ICxCategorizePropertiesImpl<CDrawTxt>,
	public IProvideClassInfo2Impl<&CLSID_DrawTxt, &DIID__ICxDrawObjectEvents, &LIBID_CXHMIOBJLib>,
	public CProxy_ICxDrawObjectEvents<CDrawTxt>,
	public IConnectionPointContainerImpl<CDrawTxt>
{
public:
	CDrawTxt()
	{
		m_bAutoSize = TRUE;
		m_bWordWrap = TRUE;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRAWTXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawTxt)
	COM_INTERFACE_ENTRY(IDrawTxt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY/*_IMPL*/(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDrawTxt)
	CONNECTION_POINT_ENTRY(DIID__ICxDrawObjectEvents)
END_CONNECTION_POINT_MAP()

BEGIN_CXPAGE_MAP()
	CXPAGE_ENTRY(CLSID_CxTextGenPage)
	CXPAGE_ENTRY(CLSID_CxTextTextPage)
END_CXPAGE_MAP()	

BEGIN_CXPROP_MAP()
	CXPROP_ENTRY("MousePointer", CXPROPID_MousePointer)
END_CXPROP_MAP()

BEGIN_CATEGORY_MAP(CDrawTxt)
//	IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
//	IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
//	IMPLEMENTED_CATEGORY(CATID_Insertable)
//	IMPLEMENTED_CATEGORY(CATID_Control)
	IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawTxt) 	
	CATEGORY_PROP_NAME(5, _T("文本"))
	CATEGORY_PROP_NAME(6, _T("格式"))
END_CATEGORY_PROP_NAME_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawTxt)
	PROPERTY_TO_CATEGORY(CXPROPID_Caption,    5)
	PROPERTY_TO_CATEGORY(CXPROPID_FontName,    5)
	PROPERTY_TO_CATEGORY(CXPROPID_FontSize,    5)
	PROPERTY_TO_CATEGORY(CXPROPID_FontColor,    5)

	PROPERTY_TO_CATEGORY(CXPROPID_FontStyle,    6)
	PROPERTY_TO_CATEGORY(CXPROPID_UnderLine,    6)
	PROPERTY_TO_CATEGORY(CXPROPID_StrikeThrough,   6)
//	PROPERTY_TO_CATEGORY(CXPROPID_Alignment,   6)
	PROPERTY_TO_CATEGORY(CXPROPID_BackStyle,    6)
	PROPERTY_TO_CATEGORY(CXPROPID_BackColor,    6)

	PROPERTY_TO_CATEGORY(CXPROPID_Alignment,    -6)
	PROPERTY_TO_CATEGORY(CXPROPID_AutoSize,    -6)
	PROPERTY_TO_CATEGORY(CXPROPID_WordWrap,    -6)
END_PROPERTY_TO_CATEGORY_MAP()

// ICxDrawObject
public:
	HRESULT DoDraw(HDC hdcDest);
	HRESULT DoPrint(HDC hdcDest) { return DoDraw(hdcDest); }
	HRESULT DoLoad(DWORD dwVer, LPSTREAM pStm);
	HRESULT DoSave(DWORD dwVer, LPSTREAM pStm);

	virtual HRESULT ICxDrawObject_GetPositionRect(RECTF* pPositionRect);
//	STDMETHOD(ICxDrawObject_GetPositionRect)(RECTF* pPositionRect);
	STDMETHOD(GetBoundingRect)(RECT* pBoundingRect);
	STDMETHOD(MoveHandleTo)(long nHandle, POINT point);
	STDMETHOD(GetHandleCount)(long* pHandleCount);
	virtual HRESULT ICxDrawObject_MoveTo(RECTF rect, BOOL bUniformScale);
	STDMETHOD(HitTest)(POINT point, BOOL* bHitTest);
	STDMETHOD(Intersects)(RECT rect, BOOL* bIntersects);
	STDMETHOD(GetClassName)(BSTR* pbstrClassName);

	STDMETHOD(InitNew)();
	STDMETHOD(SaveOldVer)(LPSTREAM pStm, BOOL fClearDirty); 
	STDMETHOD(LoadOldVer)(LPSTREAM pStm);

	STDMETHOD(DrawTrackerHandleShape)(HDC hdcDest, long nHandle, POINT point);
	STDMETHOD(DrawTrackerShape)(HDC hdcDest, POINT* pPoints, int nCount);
	STDMETHOD(RotateAt)(POINTF point, float fAngle);
	STDMETHOD(ScaleAt)(POINTF point, float fHorScale, float fVerScale);
	STDMETHOD(DrawRotateAtShape)(HDC hdcDest, POINTF point, long lAngle);
	
	void CalculateBound(RECTF& rcCaption);
	void ReCalcBound();
	void ReCalcFontSize(double& lFontHeight, double& lFontWidth);

	HFONT CreateTextFont(int nHeight, int nWidth);
	int GetAveCharWidth(int nFontHeight);

// IDrawTxt
public:
	STDMETHOD(GetCharPosition)(long lIndex, long* plX, long* plY);

	STDMETHOD(get_Height)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_Height)(/*[in]*/ float newVal);
	STDMETHOD(get_Width)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_Width)(/*[in]*/ float newVal);
	STDMETHOD(get_VerticalPosition)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_VerticalPosition)(/*[in]*/ float newVal);
	STDMETHOD(get_HorizontalPosition)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_HorizontalPosition)(/*[in]*/ float newVal);
	STDMETHOD(get_AutoSize)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_AutoSize)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_WordWrap)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_WordWrap)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_UnderLine)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_UnderLine)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_StrikeThrough)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_StrikeThrough)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_FontStyle)(/*[out, retval]*/ enumFontStyle *pVal);
	STDMETHOD(put_FontStyle)(/*[in]*/ enumFontStyle newVal);
	STDMETHOD(get_FontSize)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_FontSize)(/*[in]*/ long newVal);
	STDMETHOD(get_FontName)(/*[out, retval]*/ CXFONTNAME *pVal);
	STDMETHOD(put_FontName)(/*[in]*/ CXFONTNAME newVal);
	STDMETHOD(get_Alignment)(/*[out, retval]*/ enumTextAlign *pVal);
	STDMETHOD(put_Alignment)(/*[in]*/ enumTextAlign newVal);
	STDMETHOD(get_Caption)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Caption)(/*[in]*/ BSTR newVal);

	STDMETHOD(get_BackStyle)(/*[out, retval]*/ enumBackStyle *pVal);
	STDMETHOD(put_BackStyle)(/*[in]*/ enumBackStyle newVal);
	STDMETHOD(get_BackColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_BackColor)(/*[in]*/ OLE_COLOR newVal);

	STDMETHOD(get_FontColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_FontColor)(/*[in]*/ OLE_COLOR newVal);

	STDMETHOD(get_IdealHeight)(/*[out, retval]*/ float *pVal);
		
private:
	BOOL m_bAutoSize;
	BOOL m_bWordWrap;
	double m_dFontHeight;		//字体高度
	double m_dFontWidth;		//字体宽度
	long m_lWeight;		//
	BOOL m_bItalic;		//是否斜体
	CComBSTR m_bstrFontName;		//字体名称
	BOOL m_bUnderLine;		//是否下划线
	BOOL m_bStrikethrough;		//是否删除线
	CComBSTR m_bstrCaption;		//显示的文本
	RECTF m_rcPosition;		//位置矩形
	OLE_COLOR m_clrForeColor;		//前景颜色
	OLE_COLOR m_clrBackColor;
	enumBackStyle m_bsBackStyle;		//背景模式
	enumTextAlign m_nTextAlign;
};

#endif //__DRAWTXT_H_
