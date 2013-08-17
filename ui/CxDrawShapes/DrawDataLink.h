// DrawDataLink.h : Declaration of the CDrawDataLink

#ifndef __DRAWDATALINK_H_
#define __DRAWDATALINK_H_

#include "resource.h"       // main symbols
#include "CxDispid.h"
#include "ExpressSerializer.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"
#include <comdef.h>


typedef CContainedWindowT<CEdit> CContainedEdit;

/////////////////////////////////////////////////////////////////////////////
// CDrawDataLink
class ATL_NO_VTABLE CDrawDataLink : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawDataLink, &CLSID_DrawDataLink>,
	public ICxDispatchImpl<CDrawDataLink, IDrawDataLink, &IID_IDrawDataLink, &LIBID_CXHMIOBJLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawDataLink>,
	public ICxCategorizePropertiesImpl<CDrawDataLink>,
	public IProvideClassInfo2Impl<&CLSID_DrawDataLink, &DIID_IDrawDataLinkEvents, &LIBID_CXHMIOBJLib>,
	public CProxy_ICxObjectBaseEvents<CDrawDataLink, &DIID_IDrawDataLinkEvents>,	
//	public CProxy_ICxDrawObjectEvents<CDrawDataLink>,
	public IConnectionPointContainerImpl<CDrawDataLink>,
	public CMessageMap
{
public:
	CDrawDataLink()
	{
		m_bAutoSize = TRUE;
		m_bWordWrap = TRUE;

		m_pInputEdit = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRAWDATALINK)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawDataLink)
	COM_INTERFACE_ENTRY(IDrawDataLink)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY/*_IMPL*/(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDrawDataLink)
	CONNECTION_POINT_ENTRY(DIID_IDrawDataLinkEvents)
END_CONNECTION_POINT_MAP()

BEGIN_CXPAGE_MAP()
	CXPAGE_ENTRY(CLSID_CxDataLinkGenPage)
END_CXPAGE_MAP()	

BEGIN_CXPROP_MAP()
//  CXPROP_ENTRY("MousePointer", CXPROPID_MousePointer)
END_CXPROP_MAP()

BEGIN_MSG_MAP(CDrawDataLink)
ALT_MSG_MAP(1) 
	MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocusEdit)
	MESSAGE_HANDLER(WM_KEYDOWN,	OnKeyDownEdit)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCodeEdit)
END_MSG_MAP()

BEGIN_CATEGORY_MAP(CDrawDataLink)
//	IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
//	IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
//	IMPLEMENTED_CATEGORY(CATID_Insertable)
//	IMPLEMENTED_CATEGORY(CATID_Control)
IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawDataLink) 	
	CATEGORY_PROP_NAME(5, _T("文本"))
	CATEGORY_PROP_NAME(6, _T("格式"))
END_CATEGORY_PROP_NAME_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawDataLink)
	PROPERTY_TO_CATEGORY(CXPROPID_Value,    5)
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

// pStm->Read(&m_enumMousePointer, sizeof(m_enumMousePointer), NULL);
// m_bstrDescription.ReadFromStream(pStm);
// 
// pStm->Read(&m_dFontHeight, sizeof(double), NULL);
// pStm->Read(&m_dFontWidth, sizeof(double), NULL);
// pStm->Read(&m_lWeight, sizeof(long), NULL);
// pStm->Read(&m_bItalic, sizeof(BOOL), NULL);
// pStm->Read(&m_bUnderLine, sizeof(BOOL), NULL);
// pStm->Read(&m_bStrikethrough, sizeof(BOOL), NULL);
// m_bstrFontName.ReadFromStream(pStm);
// m_bstrValue.ReadFromStream(pStm);
// pStm->Read(&m_clrForeColor, sizeof(OLE_COLOR), NULL);
// pStm->Read(&m_clrBackColor, sizeof(OLE_COLOR), NULL);
// pStm->Read(&m_nTextAlign, sizeof(UINT), NULL);
// pStm->Read(&m_bsBackStyle, sizeof(enumBackStyle), NULL);
// pStm->Read(&m_rcPosition, sizeof(RECTF), NULL);
// pStm->Read(&m_bAutoSize, sizeof(BOOL), NULL);
// pStm->Read(&m_bWordWrap, sizeof(BOOL), NULL);


BEGIN_SERIALIZE_MAP(CDrawDataLink)
	SERIALIZE_ENTRY_INT(m_enumMousePointer, MousePointerArrow)
	SERIALIZE_ENTRY_BSTR(m_bstrDescription, "")
	SERIALIZE_ENTRY_DOUBLE(m_dFontHeight, 12)
	SERIALIZE_ENTRY_DOUBLE(m_dFontWidth, 0)
	SERIALIZE_ENTRY_LONG(m_lWeight, FW_NORMAL)
	SERIALIZE_ENTRY_BOOL(m_bItalic, FALSE)
	SERIALIZE_ENTRY_BOOL(m_bUnderLine, FALSE)
	SERIALIZE_ENTRY_BOOL(m_bStrikethrough, FALSE)
	SERIALIZE_ENTRY_BSTR(m_bstrFontName, "宋体")
	SERIALIZE_ENTRY_BSTR(m_bstrValue, "")
	SERIALIZE_ENTRY_ULONG(m_clrForeColor, 0x80000000 | COLOR_WINDOWTEXT)
	SERIALIZE_ENTRY_ULONG(m_clrBackColor, 0x80000000 | COLOR_WINDOW)
	SERIALIZE_ENTRY_ULONG(m_nTextAlign, TextAlignLeft)
	SERIALIZE_ENTRY_INT(m_bsBackStyle, BackStyleTransparent)
	SERIALIZE_ENTRY_BLOB(m_rcPosition)
	SERIALIZE_ENTRY_BOOL(m_bAutoSize, TRUE)
	SERIALIZE_ENTRY_BOOL(m_bWordWrap, TRUE)
END_SERIALIZE_MAP()	

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
	STDMETHOD(ExpressLoad)(LPSTREAM pStm, DWORD dwVer);
	STDMETHOD(ExpressSave)(LPSTREAM pStm, DWORD dwVer);
	
	STDMETHOD(DrawTrackerHandleShape)(HDC hdcDest, long nHandle, POINT point);
	STDMETHOD(DrawTrackerShape)(HDC hdcDest, POINT* pPoints, int nCount);
	STDMETHOD(RotateAt)(POINTF point, float fAngle);
	STDMETHOD(ScaleAt)(POINTF point, float fHorScale, float fVerScale);
	STDMETHOD(DrawRotateAtShape)(HDC hdcDest, POINTF point, long lAngle);

	STDMETHOD(OnWindowMessage)(UINT msg, WPARAM wParam, LPARAM lParam, BOOL bSubObj, LRESULT *plResult);
	
	void CalculateBound(RECTF& rcCaption);
	void ReCalcBound();
	void ReCalcFontSize(double& lFontHeight, double& lFontWidth);

	HFONT GetCacheFont();
	HFONT CreateTextFont(int nHeight, int nWidth);
	int GetAveCharWidth(int nFontHeight);

	HWND GetClientWindow();
	void ShowInputEdit();

	BOOL IsEnableInput();
	virtual int GetDynamicState(DISPID dispid);

	void FinalRelease();

	LRESULT OnGetDlgCodeEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	
	LRESULT OnKeyDownEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocusEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
// IDrawDataLink
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
	STDMETHOD(get_Value)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Value)(/*[in]*/ BSTR newVal);
	
	STDMETHOD(get_BackStyle)(/*[out, retval]*/ enumBackStyle *pVal);
	STDMETHOD(put_BackStyle)(/*[in]*/ enumBackStyle newVal);
	STDMETHOD(get_BackColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_BackColor)(/*[in]*/ OLE_COLOR newVal);
	
	STDMETHOD(get_FontColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_FontColor)(/*[in]*/ OLE_COLOR newVal);
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
	CComBSTR m_bstrValue;		//显示的文本
	RECTF m_rcPosition;		//边界矩形
	OLE_COLOR m_clrForeColor;		//前景颜色
	OLE_COLOR m_clrBackColor;
	enumBackStyle m_bsBackStyle;		//背景模式
	enumTextAlign m_nTextAlign;

	CFont m_fontCache;
	
	CContainedEdit* m_pInputEdit; // 数据输入
	CFont m_fontInputEdit;
};

#endif //__DRAWDATALINK_H_
