// ExtendedControl.h: interface for the CExtendedControl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __EXTENDEDCONTROL_H__
#define __EXTENDEDCONTROL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCtrlItem;
class CLayoutView;

extern const IID IID_ICxExtendedControl;

#define DISPID_EXT_NAME 0x80010000
//#define DISPID_EXT_POSITIONX 0x80010001
//#define DISPID_EXT_POSITIONY 0x80010002

class CExtendedControl : public ICxExtendedButtonControl  
{
public:
	CExtendedControl();
	~CExtendedControl();

	HRESULT Init(REFCLSID clsidControl, CCtrlItem* pItem);

	static HRESULT CreateInstance(REFCLSID clsidControl,
		CCtrlItem* pItem, IUnknown* pOuterUnknown, REFIID iid,
		void** ppInterface);

public:
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
	STDMETHOD(QueryInterface)(REFIID iid, void** ppInterface);
   
public:
	STDMETHOD(GetIDsOfNames)(REFIID iid, LPOLESTR* ppszNames, UINT nNames,
		 LCID lcid, DISPID* pDispIDs);
	STDMETHOD(GetTypeInfo)(UINT iTypeInfo, LCID lcid,
		ITypeInfo** ppTypeInfo);
	STDMETHOD(GetTypeInfoCount)(UINT* pnInfoCount);
	STDMETHOD(Invoke)(DISPID dispidMember, REFIID iid, LCID lcid,
		 WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult,
		  EXCEPINFO* pExceptionInfo, UINT* piArgError);

public:
	STDMETHOD(get_Name)(BSTR* pbstrName);
	STDMETHOD(put_Name)(BSTR bstrName);

	STDMETHOD(get_HorizontalPosition)(float* px);
	STDMETHOD(put_HorizontalPosition)(float x);
	STDMETHOD(get_VerticalPosition)(float* py);
	STDMETHOD(put_VerticalPosition)(float y);
	STDMETHOD(get_Height)(float* px);
	STDMETHOD(put_Height)(float x);
	STDMETHOD(get_Width)(float* py);
	STDMETHOD(put_Width)(float y);
	STDMETHOD(get_Visible)(VARIANT_BOOL *pVal);
	STDMETHOD(put_Visible)(VARIANT_BOOL newVal);
	STDMETHOD(get_ToolTipText)(BSTR *pVal);
	STDMETHOD(put_ToolTipText)(BSTR newVal);
	STDMETHOD(get_Layer)(int *pVal);
	STDMETHOD(put_Layer)(int newVal);
	STDMETHOD(get_Locked)(VARIANT_BOOL *pVal);
	STDMETHOD(put_Locked)(VARIANT_BOOL newVal);
	STDMETHOD(get_Privilege)(int *pVal);
	STDMETHOD(put_Privilege)(int newVal);
	STDMETHOD(get_Description)(BSTR *pVal);
	STDMETHOD(put_Description)(BSTR newVal);
	STDMETHOD(get_TabIndex)(int *pVal);
	STDMETHOD(put_TabIndex)(int newVal);
	STDMETHOD(get_TabStop)(VARIANT_BOOL *pVal);
	STDMETHOD(put_TabStop)(VARIANT_BOOL newVal);
	STDMETHOD(get_Default)(VARIANT_BOOL *pVal);
	STDMETHOD(put_Default)(VARIANT_BOOL newVal);
	STDMETHOD(get_Cancel)(VARIANT_BOOL *pVal);
	STDMETHOD(put_Cancel)(VARIANT_BOOL newVal);

	STDMETHOD(IsConnected)(BSTR bstrPropertyName, VARIANT_BOOL* pbHasConnection);
	STDMETHOD(ConnectObject)(BSTR bstrPropertyName, IUnknown* punkObject);
	STDMETHOD(ConnectDirect)(BSTR bstrPropertyName, BSTR bstrDataSource);
	STDMETHOD(GetConnectObject)(BSTR bstrPropertyName, IUnknown** ppunkObject);
	STDMETHOD(Disconnect)(BSTR bstrPropertyName);
	STDMETHOD(GetConnectState)(DISPID dispid, int* pState);

	STDMETHOD(SetFocus)();

protected:
	HRESULT InternalInvoke(DISPID dispidMember, REFIID iid, LCID lcid,
		WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult,
		EXCEPINFO* pExceptionInfo, UINT* piArgError);

protected:
	ULONG m_nRefCount;
	IUnknown* m_pUnknown;
	IDispatchPtr m_pInnerDispatch;
	static ITypeInfoPtr m_pTypeInfo;
	static ITypeInfoPtr m_pButtonTypeInfo;
	CCtrlItem* m_pItem;
//	CLayoutView* m_pView;
};

#endif // !defined(__EXTENDEDCONTROL_H__)
