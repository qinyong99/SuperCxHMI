// DrawImage.h : Declaration of the CDrawImage

#ifndef __DRAWIMAGE_H_
#define __DRAWIMAGE_H_

#include "resource.h"       // main symbols
#include "CxDispid.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"

using namespace Gdiplus;

int GetCodecClsid(const WCHAR* format, CLSID* pClsid);

/////////////////////////////////////////////////////////////////////////////
// CDrawImage
class ATL_NO_VTABLE CDrawImage : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawImage, &CLSID_DrawImage>,
	public ICxDispatchImpl<CDrawImage, IDrawImage, &IID_IDrawImage, &LIBID_CXHMIOBJLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawImage>,
	public ICxCategorizePropertiesImpl<CDrawImage>,
	public IProvideClassInfo2Impl<&CLSID_DrawImage, &DIID__ICxDrawObjectEvents, &LIBID_CXHMIOBJLib>,
	public CProxy_ICxDrawObjectEvents<CDrawImage>,
	public IConnectionPointContainerImpl<CDrawImage>
{
public:
	CDrawImage()
	{
		m_bMouseOn = FALSE;
	
		m_enumEdgeStyle = (enumEdgeStyle)0;					//实线边界
		m_fEdgeWidth = 0;											//边界线宽为1
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRAWIMAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawImage)
	COM_INTERFACE_ENTRY(IDrawImage)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY/*_IMPL*/(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDrawImage)
	CONNECTION_POINT_ENTRY(DIID__ICxDrawObjectEvents)
END_CONNECTION_POINT_MAP()

BEGIN_CXPROP_MAP()
	CXPROP_ENTRY("MousePointer", CXPROPID_MousePointer)
	CXPROP_ENTRY("ScaleBasePoint", CXPROPID_ScaleBasePoint)
	CXPROP_ENTRY("RoateBasePointX", CXPROPID_RoateBasePointX)
	CXPROP_ENTRY("RoateBasePointY", CXPROPID_RoateBasePointY)
	CXPROP_ENTRY("Transparency", 6)
	CXPROP_ENTRY("TransparentColor", 7)
	CXPROP_ENTRY("CurrentImage", 8)
	CXPROP_ENTRY("DownImageDisplayed" , 9)
	CXPROP_ENTRY("SecondaryImageDisplayed", 10)
	CXPROP_ENTRY("ButtonStyle", 11)
	CXPROP_ENTRY("ButtonState", 12)
	CXPROP_ENTRY("FrameStyle", 13)
	CXPROP_ENTRY("FrameColor", 14)
END_CXPROP_MAP()

BEGIN_CATEGORY_MAP(CDrawImage)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
	IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawImage) 	
	CATEGORY_PROP_NAME(3, _T("变换"))
	CATEGORY_PROP_NAME(4, _T("图片"))
	CATEGORY_PROP_NAME(5, _T("按钮"))
END_CATEGORY_PROP_NAME_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawImage)
	PROPERTY_TO_CATEGORY(CXPROPID_RotateAngle,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_ScaleBasePoint,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_HorScale,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_VerScale,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_RoateBasePointX,    3)
	PROPERTY_TO_CATEGORY(CXPROPID_RoateBasePointY,    3)

	PROPERTY_TO_CATEGORY(8,    4)
	PROPERTY_TO_CATEGORY(9,    4)
	PROPERTY_TO_CATEGORY(10,    4)

	PROPERTY_TO_CATEGORY(11,    5)
	PROPERTY_TO_CATEGORY(12,    5)
	
	PROPERTY_TO_CATEGORY(CXPROPID_HorPosition,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_VerPosition,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_Height,    -4)
	PROPERTY_TO_CATEGORY(CXPROPID_Width,    -4)
END_PROPERTY_TO_CATEGORY_MAP()

BEGIN_CXPAGE_MAP()
	CXPAGE_ENTRY(CLSID_CxImageGenPage)
END_CXPAGE_MAP()	

// IDrawImage
public:
	STDMETHOD(get_FrameStyle)(/*[out, retval]*/ enumFrameStyle *pVal);
	STDMETHOD(put_FrameStyle)(/*[in]*/ enumFrameStyle newVal);
	STDMETHOD(get_FrameColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_FrameColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_ButtonState)(/*[out, retval]*/ enumButtonState *pVal);
	STDMETHOD(put_ButtonState)(/*[in]*/ enumButtonState newVal);
	STDMETHOD(get_ButtonStyle)(/*[out, retval]*/ enumBitmapButtonStyle *pVal);
	STDMETHOD(put_ButtonStyle)(/*[in]*/ enumBitmapButtonStyle newVal);
	STDMETHOD(get_HoverImageDisplayed)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_HoverImageDisplayed)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_DownImageDisplayed)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_DownImageDisplayed)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ImageCount)(/*[out, retval]*/ long *pVal);
	STDMETHOD(DeleteImage)(long lIndex);
	STDMETHOD(get_CurrentImage)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_CurrentImage)(/*[in]*/ long newVal);
	STDMETHOD(AddImage)(BSTR bstrFileName);
	STDMETHOD(get_TransparentColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_TransparentColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_Transparency)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Transparency)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(LoadFromHBITMAP)(/*[in]*/ VARIANT_BOOL bPrimary, /*[in]*/ short nIndex, /*[in]*/ long hbm, /*[in]*/ long hpal);
	STDMETHOD(LoadImage)(/*[in]*/ VARIANT_BOOL bPrimary, /*[in]*/ short nIndex, /*[in]*/ BSTR bstrFileName);
	STDMETHOD(GetClassName)(BSTR* pbstrClassName);

	
	virtual void AddObjectToPath();
	
	HRESULT Draw(HDC hdcDest, BOOL bPrint = FALSE);
	virtual HRESULT DoDraw(HDC hdcDest);
	virtual HRESULT DoPrint(HDC hdcDest);
	HRESULT DoLoad(DWORD dwVer, LPSTREAM pStm);
	HRESULT DoSave(DWORD dwVer, LPSTREAM pStm);

	STDMETHOD(LoadStorage)(IStorage* pStorage);
	STDMETHOD(SaveStorage)(IStorage* pStorage, BOOL fSameAsLoad);
	
	STDMETHOD(InitNew)()
	{
		m_dwLoadVer = _SUPERCX_VER;

		m_bTransparency = VARIANT_FALSE;
		m_bDownImageDisplayed = VARIANT_FALSE;
		m_bHoverImageDisplayed = VARIANT_FALSE;
		m_clrTransparent = 0;
		m_lCurrentImage = 0;
		m_enumFrameStyle = FrameStyleNone;
		m_clrFrame = RGB(0, 0, 0);
		m_enumButtonState = ButtonStateUp;
		m_enumButtonStyle = BitmapButtonStyleNone;
		
		return ICxDrawObjectImpl<CDrawImage>::InitNew();
	}
	STDMETHOD(OnMouseLeave())
	{
		m_bMouseOn = FALSE;
		FireViewChange();
		return ICxDrawObjectImpl<CDrawImage>::OnMouseLeave();
	}
	STDMETHOD(OnWindowMessage)(UINT msg, WPARAM wParam, LPARAM lParam, BOOL bSubObj, LRESULT *plResult)
	{
		switch (msg)
		{
		case WM_MOUSEMOVE:
			{
				POINT point;
				point.x = GET_X_LPARAM(lParam);
				point.y = GET_Y_LPARAM(lParam);
				BOOL bHit;
				HitTest(point, &bHit);
				if (bHit)
				{
					m_bMouseOn = TRUE;
					m_spDrawObjectSite->TrackMouseLeave();
					FireViewChange();
				}
				else
				{
					m_bMouseOn = FALSE;
					FireViewChange();
				}
				if (m_enumButtonStyle == BitmapButtonStylePush && m_enumButtonState == ButtonStateDown)
				{
					
					if (!bHit)
					{
						m_enumButtonState = ButtonStateUp;
						FireViewChange();
					}
				}
			}
			break;
		case WM_LBUTTONDOWN:
			if (m_enumButtonStyle == BitmapButtonStylePush)
			{
				m_enumButtonState = ButtonStateDown;
				FireViewChange();
			}
			else if (m_enumButtonStyle == BitmapButtonStyleMultiState)
			{
				if (m_enumButtonState == ButtonStateDown)
					m_enumButtonState = ButtonStateUp;
				else
					m_enumButtonState = ButtonStateDown;
				FireViewChange();
			}

			break;
		case WM_LBUTTONUP:
			if (m_enumButtonStyle == BitmapButtonStylePush)
			{
				m_enumButtonState = ButtonStateUp;
				FireViewChange();
			}
		   	break;
		}
		return ICxDrawObjectImpl<CDrawImage>::OnWindowMessage(msg, wParam, lParam, bSubObj, plResult);
	}

	void FinalRelease() 
	{
		int iCount = m_apImageStream.GetSize();
		for (int i = 0; i < iCount; i++)
		{
			IStream* pStream = m_apImageStream[i];
			if (pStream != NULL)
				pStream->Release();
		}
		m_apImageStream.RemoveAll();

		iCount = m_apImage.GetSize();
		for (i = 0; i < iCount; i++)
			delete m_apImage[i];
		m_apImage.RemoveAll();
	}
	

private:
	CSimpleValArray<IStream *> m_apImageStream;
	CSimpleValArray<Image *> m_apImage;
	long m_lCurrentImage;
	OLE_COLOR m_clrTransparent;
	VARIANT_BOOL m_bTransparency;
	VARIANT_BOOL m_bDownImageDisplayed;
	VARIANT_BOOL m_bHoverImageDisplayed;
	BOOL m_bMouseOn;
	enumFrameStyle m_enumFrameStyle;
	OLE_COLOR m_clrFrame;
	enumButtonState m_enumButtonState;
	enumBitmapButtonStyle m_enumButtonStyle;

	DWORD m_dwLoadVer;
};

#endif //__DRAWIMAGE_H_
