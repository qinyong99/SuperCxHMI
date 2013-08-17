// DrawMenu.h : Declaration of the CDrawMenu

#ifndef __DRAWMENU_H_
#define __DRAWMENU_H_

#include "resource.h"       // main symbols
#include "CxDispid.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"
#include "gdiplus.h"
#include "SimpleArray2.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawMenu
class ATL_NO_VTABLE CDrawMenu : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawMenu, &CLSID_DrawMenu>,
	public ICxDispatchImpl<CDrawMenu, IDrawMenu, &IID_IDrawMenu, &LIBID_CXHMIOBJLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawMenu>,
//	public ICxPerPropertyEditImpl<CDrawMenu>,
	public ICxCategorizePropertiesImpl<CDrawMenu>,
	public IProvideClassInfo2Impl<&CLSID_DrawMenu, &DIID_IDrawMenuEvents, &LIBID_CXHMIOBJLib>,
	public CProxy_ICxObjectBaseEvents<CDrawMenu, &DIID_IDrawMenuEvents>,
	public IConnectionPointContainerImpl<CDrawMenu>
{
public:
	CDrawMenu()
	{
		m_bMouseOn = FALSE;
		m_enumButtonState = ButtonStateUp;
	}

	class CMenuItem
	{
	public:
		CMenuItem()
		{
			m_dwState = 0;
			m_dwShiftState = 0;
			m_dwKeyCode = 0;
		}
		CMenuItem(const CMenuItem& item)
		{
			InternalCopy(item);
		}
		void InternalCopy(const CMenuItem& item)
		{
			m_bstrText = item.m_bstrText;
			m_dwState = item.m_dwState;
			m_bstrID = item.m_bstrID;
			m_dwShiftState = item.m_dwShiftState;
			m_dwKeyCode = item.m_dwKeyCode;
		}
	
		CComBSTR m_bstrText;
		DWORD m_dwState;
		CComBSTR m_bstrID;
		DWORD m_dwShiftState;
		DWORD m_dwKeyCode;
	};

DECLARE_REGISTRY_RESOURCEID(IDR_DRAWMENU)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawMenu)
	COM_INTERFACE_ENTRY(IDrawMenu)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CXPAGE_MAP()
  	CXPAGE_ENTRY(CLSID_CxMenuGenPage)
END_CXPAGE_MAP()	

BEGIN_CONNECTION_POINT_MAP(CDrawMenu)
	CONNECTION_POINT_ENTRY(DIID_IDrawMenuEvents)
END_CONNECTION_POINT_MAP()

BEGIN_CXPROP_MAP()
END_CXPROP_MAP()

BEGIN_CATEGORY_MAP(CDrawMenu)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
	IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawMenu) 
	CATEGORY_PROP_NAME(5, _T("²Ëµ¥"))	
END_CATEGORY_PROP_NAME_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawMenu)
	PROPERTY_TO_CATEGORY(20,    5)
	PROPERTY_TO_CATEGORY(21,    5)
END_PROPERTY_TO_CATEGORY_MAP()

// IDrawMenu
public:
	STDMETHOD(Popup)(/*[in, optional] */VARIANT X, /*[in, optional] */VARIANT Y);
	STDMETHOD(AddItem)(/*[in] */BSTR Item, /*[in, optional] */VARIANT Index, /*[in, optional] */VARIANT State, /*[in, optional] */VARIANT ID,
					   /*[in, optional] */VARIANT AccelShift, /*[in, optional] */VARIANT AccelCode);
	STDMETHOD(RemoveItem)(/*[in] */int Index);
	STDMETHOD(GetItemState)(/*[in] */int Index, /*[out, retval] */long* pVal);
	STDMETHOD(SetItemState)(/*[in] */int Index, /*[in] */long State);
	STDMETHOD(GetItemText)(/*[in] */int Index, /*[out, retval] */BSTR* pVal);
	STDMETHOD(SetItemText)(/*[in] */int Index, /*[in] */BSTR Item);
	STDMETHOD(GetItemID)(/*[in] */int Index, /*[out, retval] */BSTR* ID);
	STDMETHOD(SetItemID)(/*[in] */int Index, /*[in] */BSTR ID);
	STDMETHOD(get_ItemCount)(/*[out, retval] */long *pVal);
	STDMETHOD(Clear)();
	STDMETHOD(SelectItem)(/*[in] */int Index);

	STDMETHOD(GetItemAccelShift)(/*[in] */int Index, /*[out, retval] */long* pVal);
	STDMETHOD(SetItemAccelShift)(/*[in] */int Index, /*[in] */long Shift);
	STDMETHOD(GetItemAccelKeyCode)(/*[in] */int Index, /*[out, retval] */long* pVal);
	STDMETHOD(SetItemAccelKeyCode)(/*[in] */int Index, /*[in] */long KeyCode);

	STDMETHOD(get_ButtonDisplayed)(/*[out, retval] */VARIANT_BOOL *pVal);
	STDMETHOD(put_ButtonDisplayed)(/*[in] */VARIANT_BOOL newVal);
	STDMETHOD(get_ButtonCaption)(/*[out, retval] */BSTR *pVal);
	STDMETHOD(put_ButtonCaption)(/*[in] */BSTR newVal);

public:
	HRESULT DoLoad(DWORD dwVer, LPSTREAM pStm);
	HRESULT DoSave(DWORD dwVer, LPSTREAM pStm);

	STDMETHOD(GetClassName)(BSTR* pbstrClassName);
	STDMETHOD(MoveHandleTo)(long nHandle, POINT point);
	STDMETHOD(DrawTrackerHandleShape)(HDC hdcDest, long nHandle, POINT point);
	STDMETHOD(DrawTrackerShape)(HDC hdcDest, POINT* pPoints, int nCount);
	STDMETHOD(RotateAt)(POINTF point, float fAngle);
	STDMETHOD(ScaleAt)(POINTF point, float fHorScale, float fVerScale);
	STDMETHOD(DrawRotateAtShape)(HDC hdcDest, POINTF point, long lAngle);

	STDMETHOD(HitTest)(POINT point, BOOL* bHitTest);
	STDMETHOD(Intersects)(RECT rect, BOOL* bIntersects);
	STDMETHOD(MoveTo)(RECTF rect, BOOL bUniformScale);
	STDMETHOD(GetPositionRect)(RECTF* pPositionRect);
	STDMETHOD(GetBoundingRect)(RECT* pBoundingRect);
	STDMETHOD(GetHandleCount)(long* pHandleCount);

	STDMETHOD(OnMouseLeave)();
	STDMETHOD(OnWindowMessage)(UINT msg, WPARAM wParam, LPARAM lParam, BOOL bSubObj, LRESULT *plResult);
	
	virtual HRESULT DoDraw(HDC hdcDest);

	STDMETHOD(InitNew)();

	HWND GetClientWindow();
	VOID Fire_MenuClick(int Item, BSTR ID);

	void GetSystemSettings();

	void AddMenuItems(HMENU hMenu, int& nIndex, int nDepth);

	RECTF m_rcPosition;	

	CSimpleArray2<CMenuItem> m_arrItems;

	BOOL m_bButtonDisplayed;
	CComBSTR m_bstrButtonCaption;

	enumButtonState m_enumButtonState;
	BOOL m_bMouseOn;
	CFont m_fontMenu;
};

#endif //__DRAWMENU_H_
