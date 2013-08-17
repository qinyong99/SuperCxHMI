// DrawVariable.h : Declaration of the CDrawVariable

#ifndef __DRAWVARIABLE_H_
#define __DRAWVARIABLE_H_

#include "resource.h"       
#include "CxDispid.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"
#include "gdiplus.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawVariable
class ATL_NO_VTABLE CDrawVariable : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawVariable, &CLSID_DrawVariable>,
	public ICxDispatchImpl<CDrawVariable, IDrawVariable, &IID_IDrawVariable, &LIBID_CXHMIOBJLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawVariable>,
//	public ICxPerPropertyEditImpl<CDrawVariable>,
	public ICxCategorizePropertiesImpl<CDrawVariable>,
	public IProvideClassInfo2Impl<&CLSID_DrawVariable, &DIID_IDrawVariableEvents, &LIBID_CXHMIOBJLib>,
	public CProxy_ICxObjectBaseEvents<CDrawVariable, &DIID_IDrawVariableEvents>,
	public IConnectionPointContainerImpl<CDrawVariable>
{
public:
	CDrawVariable()
	{
		m_bParsingDynamic = FALSE;
	}
	~CDrawVariable()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRAWVARIABLE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawVariable)
	COM_INTERFACE_ENTRY(IDrawVariable)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CXPAGE_MAP()
 	CXPAGE_ENTRY(CLSID_CxVariableGenPage)
END_CXPAGE_MAP()	

BEGIN_CONNECTION_POINT_MAP(CDrawVariable)
	CONNECTION_POINT_ENTRY(DIID_IDrawVariableEvents)
END_CONNECTION_POINT_MAP()

BEGIN_CXPROP_MAP()
	CXPROP_ENTRY("Value", CXPROPID_Value)
	CXPROP_ENTRY("VariableType", CXPROPID_VariableType)
END_CXPROP_MAP()

BEGIN_CATEGORY_MAP(CDrawVariable)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
	IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawVariable) 	
	CATEGORY_PROP_NAME(1, _T("±‰¡ø"))
END_CATEGORY_PROP_NAME_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawVariable)
	PROPERTY_TO_CATEGORY(CXPROPID_Value,    1)
	PROPERTY_TO_CATEGORY(CXPROPID_VariableType,    1)
END_PROPERTY_TO_CATEGORY_MAP()

// IDrawVariable
public:
	STDMETHOD(get_Value)(/*[out, retval]*/ CXVARIABLETYPE *pVal);
	STDMETHOD(put_Value)(/*[in]*/ CXVARIABLETYPE newVal);
	STDMETHOD(get_Type)(/*[out, retval]*/ enumPropertyType *pVal);
	STDMETHOD(put_Type)(/*[in]*/ enumPropertyType newVal);

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

	virtual HRESULT DoDraw(HDC hdcDest);
	virtual int GetDynamicState(DISPID dispid);

	STDMETHOD(InitNew)();
	STDMETHOD(InitRun)(BOOL* pbHasDynamic, ICxScriptEngineSite* pScriptSite);
	STDMETHOD(ParseDynamic)();

	CComVariant m_vaOrigin;
	CComVariant m_value;
	enumPropertyType m_type;
	BOOL m_bParsingDynamic;

	RECTF m_rcPosition;	
};

#endif //__DRAWVARIABLE_H_
