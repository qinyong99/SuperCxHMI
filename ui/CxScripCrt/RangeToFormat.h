// RangeToFormat.h : Declaration of the CRangeToFormat

#ifndef __RANGETOFORMAT_H_
#define __RANGETOFORMAT_H_

#include "resource.h"       // main symbols
#include <Platform.h>
#include <Scintilla.h>
#include <SciLexer.h>

/////////////////////////////////////////////////////////////////////////////
// CRangeToFormat
class ATL_NO_VTABLE CRangeToFormat : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRangeToFormat, &CLSID_CxRangeToFormat>,
	public IDispatchImpl<ICxRangeToFormat, &IID_ICxRangeToFormat, &LIBID_CXSCRIPCRTLib>
{
public:
	CRangeToFormat();

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CRangeToFormat)
	COM_INTERFACE_ENTRY(ICxRangeToFormat)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IRangeToFormat
public:
	STDMETHOD(get_Max)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Max)(/*[in]*/ long newVal);
	STDMETHOD(get_Min)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Min)(/*[in]*/ long newVal);
	STDMETHOD(get_PageBottom)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_PageBottom)(/*[in]*/ long newVal);
	STDMETHOD(get_PageRight)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_PageRight)(/*[in]*/ long newVal);
	STDMETHOD(get_PageTop)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_PageTop)(/*[in]*/ long newVal);
	STDMETHOD(get_PageLeft)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_PageLeft)(/*[in]*/ long newVal);
	STDMETHOD(get_Bottom)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Bottom)(/*[in]*/ long newVal);
	STDMETHOD(get_Right)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Right)(/*[in]*/ long newVal);
	STDMETHOD(get_Top)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Top)(/*[in]*/ long newVal);
	STDMETHOD(get_Left)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Left)(/*[in]*/ long newVal);
	STDMETHOD(get_TargetDC)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_TargetDC)(/*[in]*/ long newVal);
	STDMETHOD(get_DC)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_DC)(/*[in]*/ long newVal);

public:
	RangeToFormat m_rf;
};

#endif //__RANGETOFORMAT_H_
