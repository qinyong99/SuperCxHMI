// FloatPoint.h : Declaration of the CFloatPoint

#ifndef __FLOATPOINT_H_
#define __FLOATPOINT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFloatPoint
class ATL_NO_VTABLE CFloatPoint : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFloatPoint, &CLSID_FloatPoint>,
	public IDispatchImpl<IFloatPoint, &IID_IFloatPoint, &LIBID_CXHMIOBJLib>
{
public:
	CFloatPoint()
	{
		m_X = 0;
		m_Y = 0;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FLOATPOINT)
DECLARE_NOT_AGGREGATABLE(CFloatPoint)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFloatPoint)
	COM_INTERFACE_ENTRY(IFloatPoint)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IFloatPoint
public:
	STDMETHOD(get_Y)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_Y)(/*[in]*/ float newVal);
	STDMETHOD(get_X)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_X)(/*[in]*/ float newVal);
protected:
	float m_X;
	float m_Y;
};

#endif //__FLOATPOINT_H_
