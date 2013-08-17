// DrawAlmSum.h : Declaration of the CDrawAlmSum

#ifndef __DRAWALMSUM_H_
#define __DRAWALMSUM_H_

#include "resource.h"       // main symbols
#include "CxDispid.h"
#include "CxDrawObj.h"
#include "CxCatProp.h"
#include "AlmSumCP.h"

#include "AlmSum.h"
#include "AlmSumDispatchImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawAlmSum

class ATL_NO_VTABLE CDrawAlmSum : 
	public CAlmSum, 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrawAlmSum, &CLSID_DrawAlmSum>,
	public CAlmSumDispatchImpl<CDrawAlmSum, IDrawAlmSum, &IID_IDrawAlmSum, &LIBID_CXALMSUMCTRLLib>,
//	public ICxDispatchImpl<CDrawAlmSum, IDrawAlmSum, &IID_IDrawAlmSum, &LIBID_CXDRAWALMSUMLib>,
	public CCxDrawObjectBase,
	public ICxDrawObjectImpl<CDrawAlmSum>,
	public ICxCategorizePropertiesImpl<CDrawAlmSum>,
	public IProvideClassInfo2Impl<&CLSID_DrawAlmSum, &DIID__ICxDrawObjectEvents, &LIBID_CXALMSUMCTRLLib>,
	public CProxy_ICxAlmSumEvents<CDrawAlmSum>,
	public IConnectionPointContainerImpl<CDrawAlmSum>
{
public:
	CDrawAlmSum() 
	{
		m_hDC = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRAWALMSUM)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDrawAlmSum)
	COM_INTERFACE_ENTRY(IDrawAlmSum)
	COM_INTERFACE_ENTRY(ICxAlmSum)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICxDrawObject)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
	COM_INTERFACE_ENTRY/*_IMPL*/(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDrawAlmSum)
	CONNECTION_POINT_ENTRY(DIID__ICxAlmSumCtrlEvents)
END_CONNECTION_POINT_MAP()

BEGIN_CXPROP_MAP()
//	CXPROP_ENTRY("ShowScrollBar", 18)
//	CXPROP_ENTRY("ShowGrid", 101)
//	CXPROP_ENTRY("AutoScroll", 102)

	CXPROP_ENTRY("MousePointer", CXPROPID_MousePointer)
	CXPROP_ENTRY("Description", CXPROPID_Description)
END_CXPROP_MAP()

BEGIN_CATEGORY_MAP(CDrawAlmSum)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit)
	IMPLEMENTED_CATEGORY(CATID_PersistsToStorage)
	IMPLEMENTED_CATEGORY(CATID_Insertable)
	IMPLEMENTED_CATEGORY(CATID_Control)
	IMPLEMENTED_CATEGORY(CATID_Programmable)
END_CATEGORY_MAP ()

BEGIN_CATEGORY_PROP_NAME_MAP(CDrawAlmSum) 
	CATEGORY_PROP_NAME(1, _T("外观"))	
	CATEGORY_PROP_NAME(2, _T("排序"))
	CATEGORY_PROP_NAME(3, _T("过滤"))	
	CATEGORY_PROP_NAME(4, _T("操作"))	
END_CATEGORY_PROP_NAME_MAP()

BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(CDrawAlmSum)
	PROPERTY_TO_CATEGORY(114, 1)
	PROPERTY_TO_CATEGORY(115, 1)
	PROPERTY_TO_CATEGORY(116, 1)
	PROPERTY_TO_CATEGORY(101, 1)
	PROPERTY_TO_CATEGORY(103, 1)
	PROPERTY_TO_CATEGORY(104, 1)
	PROPERTY_TO_CATEGORY(105, 2)
	PROPERTY_TO_CATEGORY(106, 2)
	PROPERTY_TO_CATEGORY(107, 2)
	PROPERTY_TO_CATEGORY(108, 3)
	PROPERTY_TO_CATEGORY(109, 3)
	PROPERTY_TO_CATEGORY(110, 3)
	PROPERTY_TO_CATEGORY(111, 4)
	PROPERTY_TO_CATEGORY(112, 4)
	PROPERTY_TO_CATEGORY(113, 4)
	PROPERTY_TO_CATEGORY(117, 1)
	PROPERTY_TO_CATEGORY(118, 1)
END_PROPERTY_TO_CATEGORY_MAP()

BEGIN_CXPAGE_MAP()
	CXPAGE_ENTRY(CLSID_CxGenAlmSumPage)
	CXPAGE_ENTRY(CLSID_CxOperatorAlmSumPage)
	CXPAGE_ENTRY(CLSID_CxSortAlmSumPage)
	CXPAGE_ENTRY(CLSID_CxFilterAlmSumPage)
	CXPAGE_ENTRY(CLSID_CxFormatAlmSumPage)
	CXPAGE_ENTRY(CLSID_CxColumnAlmSumPage)
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

	STDMETHOD(InitRun)(BOOL* pbHasDynamic, ICxScriptEngineSite* pScriptSite);
	STDMETHOD(OnWindowMessage)(UINT msg, WPARAM wParam, LPARAM lParam, BOOL bSubObj, LRESULT *plResult);

	STDMETHOD(HitTest)(POINT point, BOOL* bHitTest);
	STDMETHOD(Intersects)(RECT rect, BOOL* bIntersects);
	STDMETHOD(MoveTo)(RECTF rect, BOOL bUniformScale);
	STDMETHOD(GetPositionRect)(RECTF* pPositionRect);
	STDMETHOD(GetBoundingRect)(RECT* pBoundingRect);
	STDMETHOD(GetHandleCount)(long* pHandleCount);

	void FinalRelease();
	
// IDrawAlmSum
public:
	STDMETHOD(get_Height)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_Height)(/*[in]*/ float newVal);
	STDMETHOD(get_Width)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_Width)(/*[in]*/ float newVal);
	STDMETHOD(get_VerticalPosition)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_VerticalPosition)(/*[in]*/ float newVal);
	STDMETHOD(get_HorizontalPosition)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_HorizontalPosition)(/*[in]*/ float newVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pbstrName);
	STDMETHOD(put_Name)(/*[in]*/ BSTR bstrName);
	STDMETHOD(get_Visible)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Visible)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Layer)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_Layer)(/*[in]*/ int newVal);
	STDMETHOD(get_Locked)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Locked)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Privilege)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_Privilege)(/*[in]*/ int newVal);
	STDMETHOD(get_MousePointer)(/*[out, retval]*/ enumMousePointer *pVal);
	STDMETHOD(put_MousePointer)(/*[in]*/ enumMousePointer newVal);
	STDMETHOD(get_Description)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Description)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ToolTipText)(/*[out, retval]*/ BSTR *pbstrText);
	STDMETHOD(put_ToolTipText)(/*[in]*/ BSTR bstrText);
	
	STDMETHOD(IsConnected)(BSTR bstrPropertyName, VARIANT_BOOL* pbHasConnection);
	STDMETHOD(ConnectObject)(BSTR bstrPropertyName, IUnknown* punkObject);
	STDMETHOD(ConnectDirect)(BSTR bstrPropertyName, BSTR bstrDataSource);
	STDMETHOD(GetConnectObject)(BSTR bstrPropertyName, IUnknown** ppunkObject);
	STDMETHOD(Disconnect)(BSTR bstrPropertyName);
	STDMETHOD(GetConnectState)(DISPID dispid, int* pState);

public:
	HRESULT DoDraw(HDC hdcDest);
	HRESULT DoPrint(HDC hdcDest) { return DoDraw(hdcDest); }
	HRESULT DoLoad(DWORD dwVer, LPSTREAM pStm);
	HRESULT DoSave(DWORD dwVer, LPSTREAM pStm);

	virtual void OnAlarmAcked(LPCTSTR szSource);

	virtual BOOL RecalcRects();
	virtual HRESULT FireViewChange();
	virtual BOOL ClientIsRunMode();
	virtual void SetCapture(BOOL bCapture, CDrawCtrl* pCtrl = NULL);
	virtual HDC GetDC();
	virtual void ReleaseDC(HDC hDC);
	virtual HWND GetClientWindow();
	virtual IUnknown* GetObjectUnknown() { return GetUnknown(); }
	virtual void GetDefaultServer(BSTR* pbstrComputer);

	virtual HRESULT OnEditProperties(); 

	RECTF m_rcPosition;	

	HDC m_hDC;
};

#endif //__DRAWALMSUM_H_
