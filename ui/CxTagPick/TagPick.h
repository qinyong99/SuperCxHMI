// TagPick.h : Declaration of the CTagPick

#ifndef __CXTAGPICKER_H_
#define __CXTAGPICKER_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTagPick
class ATL_NO_VTABLE CTagPick : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTagPick, &CLSID_CxTagPicker>,
	public IDispatchImpl<ICxTagPicker, &IID_ICxTagPicker, &LIBID_CXTAGPICKLib>
{
public:
	CTagPick()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CXTAGPICKER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTagPick)
	COM_INTERFACE_ENTRY(ICxTagPicker)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ICxTagPicker
public:
	STDMETHOD(PickTag)(long hParent, long nX, long nY, BOOL bUseLocalSetup, BOOL bDisableAssoc, BSTR bstrAssocGroup, BSTR bstrTag, BSTR* pbstrPickTag);
	STDMETHOD(PickTagGroup)(long hParent, long nX, long nY, BOOL bUseLocalSetup, BOOL bDisableAssoc, BSTR bstrAssocGroup, BSTR bstrGroup, BSTR* pbstrPickGroup);
	STDMETHOD(PickSecurityArea)(long hParent, long nX, long nY, BOOL bUseLocalSetup, BSTR bstrArea, BSTR* pbstrPickArea);
};

#endif //__CXTAGPICKER_H_
