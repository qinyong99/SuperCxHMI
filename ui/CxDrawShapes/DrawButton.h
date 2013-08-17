// DrawButton.h : Declaration of the CDrawButton

#ifndef __DRAWBUTTON_H_
#define __DRAWBUTTON_H_

#include "resource.h"       // main symbols
#include "CxDispid.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawButton
class ATL_NO_VTABLE CDrawButton : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawButton, &CLSID_DrawButton>,
	public ICxDispatchImpl<CDrawButton, IDrawButton, &IID_IDrawButton, &LIBID_CXHMIOBJLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawButton>,
	public ICxCategorizePropertiesImpl<CDrawButton>,
	public IProvideClassInfo2Impl<&CLSID_DrawButton, &DIID__ICxDrawObjectEvents, &LIBID_CXHMIOBJLib>,
	public CProxy_ICxDrawObjectEvents<CDrawButton>,
	public IConnectionPointContainerImpl<CDrawButton>
{
	enum enumButtonState
	{
		ButtonStateUp,
		ButtonStateDown
	};
public:
	CDrawButton()
	{
		m_bMouseOn = FALSE;
		m_enumButtonState = ButtonStateUp;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRAWBUTTON)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawButton)
	COM_INTERFACE_ENTRY(IDrawButton)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY/*_IMPL*/(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDrawButton)
	CONNECTION_POINT_ENTRY(DIID__ICxDrawObjectEvents)
END_CONNECTION_POINT_MAP()

BEGIN_CXPROP_MAP()
	CXPROP_ENTRY("MousePointer", CXPROPID_MousePointer)
	CXPROP_ENTRY("ForeColor", CXPROPID_ForeColor)
	CXPROP_ENTRY("BackColor", CXPROPID_BackColor)
	CXPROP_ENTRY("BackColor", CXPROPID_Caption)
	CXPROP_ENTRY("Font", CXPROPID_Font)
	CXPROP_ENTRY("PictureNormal", 1)
	CXPROP_ENTRY("PictureHover", 2)
	CXPROP_ENTRY("PicturePushed", 3)
END_CXPROP_MAP()

BEGIN_CATEGORY_MAP(CDrawButton)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
	IMPLEMENTED_CATEGORY(CATID_Insertable)
	IMPLEMENTED_CATEGORY(CATID_Control)
	IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawButton) 
	CATEGORY_PROP_NAME(-5, _T("Õ‚π€"))
	CATEGORY_PROP_NAME(2, _T("Õº∆¨"))	
END_CATEGORY_PROP_NAME_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawButton)	
	PROPERTY_TO_CATEGORY(CXPROPID_Caption, -5)
	PROPERTY_TO_CATEGORY(CXPROPID_BackColor, -5)
	PROPERTY_TO_CATEGORY(CXPROPID_ForeColor, -5)
	PROPERTY_TO_CATEGORY(CXPROPID_Font, -5)

	PROPERTY_TO_CATEGORY(1, 2)
	PROPERTY_TO_CATEGORY(2, 2)
	PROPERTY_TO_CATEGORY(3, 2)
	PROPERTY_TO_CATEGORY(4, 2)
END_PROPERTY_TO_CATEGORY_MAP()

BEGIN_CXPAGE_MAP()
	CXPAGE_ENTRY(CLSID_CxButtonGenPage)
END_CXPAGE_MAP()

public:
	STDMETHOD(GetClassName)(BSTR* pbstrClassName);
	STDMETHOD(MoveHandleTo)(long nHandle, POINT point);
	STDMETHOD(DrawTrackerHandleShape)(HDC hdcDest, long nHandle, POINT point);
	STDMETHOD(DrawTrackerShape)(HDC hdcDest, POINT* pPoints, int nCount);
	STDMETHOD(RotateAt)(POINTF point, float fAngle);
	STDMETHOD(ScaleAt)(POINTF point, float fHorScale, float fVerScale);
	STDMETHOD(DrawRotateAtShape)(HDC hdcDest, POINTF point, long lAngle);

	STDMETHOD(InitNew)();
	STDMETHOD(SaveOldVer)(LPSTREAM pStm, BOOL fClearDirty); 
	STDMETHOD(LoadOldVer)(LPSTREAM pStm);

	STDMETHOD(HitTest)(POINT point, BOOL* bHitTest);
	STDMETHOD(Intersects)(RECT rect, BOOL* bIntersects);
	STDMETHOD(MoveTo)(RECTF rect, BOOL bUniformScale);
	STDMETHOD(GetPositionRect)(RECTF* pPositionRect);
	STDMETHOD(GetBoundingRect)(RECT* pBoundingRect);
	STDMETHOD(GetHandleCount)(long* pHandleCount);

	STDMETHOD(OnMouseLeave)();
	STDMETHOD(OnWindowMessage)(UINT msg, WPARAM wParam, LPARAM lParam, BOOL bSubObj, LRESULT *plResult);
	
	HRESULT DoDraw(HDC hdcDest);
	HRESULT DoPrint(HDC hdcDest) { return DoDraw(hdcDest); }
	HRESULT DoLoad(DWORD dwVer, LPSTREAM pStm);
	HRESULT DoSave(DWORD dwVer, LPSTREAM pStm);
	
// IDrawButton
public:
	STDMETHOD(get_Height)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_Height)(/*[in]*/ float newVal);
	STDMETHOD(get_Width)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_Width)(/*[in]*/ float newVal);
	STDMETHOD(get_VerticalPosition)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_VerticalPosition)(/*[in]*/ float newVal);
	STDMETHOD(get_HorizontalPosition)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_HorizontalPosition)(/*[in]*/ float newVal);
	STDMETHOD(get_Caption)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Caption)(/*[in]*/ BSTR newVal);
	STDMETHOD(put_BackColor)(OLE_COLOR clrBackground);
	STDMETHOD(get_BackColor)(OLE_COLOR* pclrBackground);
	STDMETHOD(put_ForeColor)(OLE_COLOR clrForeground);
	STDMETHOD(get_ForeColor)(OLE_COLOR* pclrForeground);
	STDMETHOD(put_Font)(IFontDisp* pFont);
	STDMETHOD(putref_Font)(IFontDisp* pFont);
	STDMETHOD(get_Font)(IFontDisp** ppFont);

	STDMETHOD(put_PictureNormal)(IPictureDisp* pPicture);
	STDMETHOD(putref_PictureNormal)(IPictureDisp* pPicture);
	STDMETHOD(get_PictureNormal)(IPictureDisp** ppPicture);

	STDMETHOD(put_PictureHover)(IPictureDisp* pPicture);
	STDMETHOD(putref_PictureHover)(IPictureDisp* pPicture);
	STDMETHOD(get_PictureHover)(IPictureDisp** ppPicture);

	STDMETHOD(put_PicturePushed)(IPictureDisp* pPicture);
	STDMETHOD(putref_PicturePushed)(IPictureDisp* pPicture);
	STDMETHOD(get_PicturePushed)(IPictureDisp** ppPicture);

	STDMETHOD(put_PictureDisabled)(IPictureDisp* pPicture);
	STDMETHOD(putref_PictureDisabled)(IPictureDisp* pPicture);
	STDMETHOD(get_PictureDisabled)(IPictureDisp** ppPicture);

	STDMETHOD(put_Enabled)(VARIANT_BOOL newVal);
	STDMETHOD(get_Enabled)(VARIANT_BOOL *pVal);

	RECTF m_rcPosition;	
	CComBSTR m_bstrCaption;
	OLE_COLOR m_clrBackground;
	OLE_COLOR m_clrForeground;
	CComPtr<IFontDisp> m_pFont;

	CComPtr<IPictureDisp> m_pPictureNormal;
	CComPtr<IPictureDisp> m_pPictureHover;
	CComPtr<IPictureDisp> m_pPicturePushed;
	CComPtr<IPictureDisp> m_pPictureDisabled;

	VARIANT_BOOL m_bEnabled;
	
	enumButtonState m_enumButtonState;
	BOOL m_bMouseOn;
};

#endif //__DRAWBUTTON_H_
