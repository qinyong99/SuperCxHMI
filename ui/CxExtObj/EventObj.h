// EventObj.h : Declaration of the CEventObj

#ifndef __CXEVENTOBJ_H_
#define __CXEVENTOBJ_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEventObj
class ATL_NO_VTABLE CEventObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEventObj, &CLSID_CxEventObj>,
	public IDispatchImpl<ICxEventObj, &IID_ICxEventObj, &LIBID_CXHMIOBJLib>
{
public:
	CEventObj()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CXEVENTOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CEventObj)
	COM_INTERFACE_ENTRY(ICxEventObj)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ICxEventObj
public:
	STDMETHOD(get_y)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_y)(/*[in]*/ long newVal);
	STDMETHOD(get_x)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_x)(/*[in]*/ long newVal);
	STDMETHOD(get_clientY)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_clientY)(/*[in]*/ long newVal);
	STDMETHOD(get_clientX)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_clientX)(/*[in]*/ long newVal);
	STDMETHOD(get_screenY)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_screenY)(/*[in]*/ long newVal);
	STDMETHOD(get_screenX)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_screenX)(/*[in]*/ long newVal);
	STDMETHOD(get_button)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_button)(/*[in]*/ long newVal);
	STDMETHOD(get_keyCode)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_keyCode)(/*[in]*/ long newVal);
	STDMETHOD(get_cancelBubble)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_cancelBubble)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_returnValue)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_returnValue)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_shiftKey)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_shiftKey)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ctrlKey)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ctrlKey)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_altKey)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_altKey)(/*[in]*/ VARIANT_BOOL newVal);

public:
	VARIANT_BOOL altKey;
	VARIANT_BOOL ctrlKey;
	VARIANT_BOOL shiftKey;
	VARIANT returnValue;
	VARIANT_BOOL cancelBubble;
	long keyCode;
	long button;
	long x;
	long y;
	long clientX;
	long clientY;
	long screenX;
	long screenY;
};

#endif //__CXEVENTOBJ_H_
