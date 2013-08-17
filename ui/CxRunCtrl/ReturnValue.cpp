// ReturnValue.cpp : implementation file
//

#include "stdafx.h"
#include "ReturnValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReturnValue

IMPLEMENT_DYNCREATE(CReturnValue, CCmdTarget)

CReturnValue::CReturnValue()
{
	EnableAutomation();
}

CReturnValue::~CReturnValue()
{
}


void CReturnValue::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CReturnValue, CCmdTarget)
	//{{AFX_MSG_MAP(CReturnValue)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CReturnValue, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CReturnValue)
	DISP_PROPERTY_EX_ID(CReturnValue, "Value", 0, GetValue, SetValue, VT_VARIANT)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IReturnValueA to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {BC7BE258-665A-448C-998F-01FEA30D6872}
static const IID IID_IReturnValue =
{ 0xbc7be258, 0x665a, 0x448c, { 0x99, 0x8f, 0x1, 0xfe, 0xa3, 0xd, 0x68, 0x72 } };

BEGIN_INTERFACE_MAP(CReturnValue, CCmdTarget)
	INTERFACE_PART(CReturnValue, IID_IReturnValue, DispatchExtend)
	INTERFACE_PART(CReturnValue, IID_IDispatch, DispatchExtend)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReturnValue::XDispatchExtend

STDMETHODIMP_( ULONG ) CReturnValue::XDispatchExtend::AddRef()
{
	METHOD_PROLOGUE(CReturnValue, DispatchExtend)

    return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CReturnValue::XDispatchExtend::Release()
{
	METHOD_PROLOGUE(CReturnValue, DispatchExtend)

	return pThis->ExternalRelease();
}

STDMETHODIMP CReturnValue::XDispatchExtend::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CReturnValue, DispatchExtend)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CReturnValue::XDispatchExtend::GetIDsOfNames(
	REFIID iid, LPOLESTR* ppszNames, UINT nNames, LCID lcid, DISPID* pDispIDs)
{
	USES_CONVERSION;

	(void)lcid;

	METHOD_PROLOGUE(CReturnValue, DispatchExtend)
	ASSERT_VALID(pThis);

	if (nNames == 1)
	{
		if (!IsEqualIID( iid, IID_NULL))
		{
			return E_INVALIDARG;
		}
		if (ppszNames == NULL)
		{
			return E_INVALIDARG;
		}
		if (pDispIDs == NULL)
		{
			return E_POINTER;
		}
	}
	
	return ((IDispatch *)(&pThis->m_xDispatch))->GetIDsOfNames(iid, ppszNames, nNames, lcid, pDispIDs);
}

STDMETHODIMP CReturnValue::XDispatchExtend::GetTypeInfo(
	UINT iTypeInfo, LCID lcid, ITypeInfo** ppTypeInfo)
{
	(void)iTypeInfo;
	(void)lcid;
	(void)ppTypeInfo;

	METHOD_PROLOGUE(CReturnValue, DispatchExtend)
	ASSERT_VALID(pThis);

	return ((IDispatch *)(&pThis->m_xDispatch))->GetTypeInfo(iTypeInfo, lcid, ppTypeInfo);
}

STDMETHODIMP CReturnValue::XDispatchExtend::GetTypeInfoCount(
   UINT* pnInfoCount )
{
	METHOD_PROLOGUE(CReturnValue, DispatchExtend)
	ASSERT_VALID(pThis);

	return ((IDispatch *)(&pThis->m_xDispatch))->GetTypeInfoCount(pnInfoCount);
}

STDMETHODIMP CReturnValue::XDispatchExtend::Invoke(
	DISPID dispidMember, REFIID iid, LCID lcid, WORD wFlags,
	DISPPARAMS* pdpParams, VARIANT* pvarResult, EXCEPINFO* pExceptionInfo,
	UINT* piArgErr)
{
//	HRESULT hResult;
	COleVariant varValue;

	(void)iid;
	(void)lcid;
	(void)wFlags;
	(void)pdpParams;
	(void)pExceptionInfo;
	(void)piArgErr;

	METHOD_PROLOGUE(CReturnValue, DispatchExtend)
	ASSERT_VALID(pThis);

#ifdef _DEBUG
		if (dispidMember == 20000 || dispidMember == 20001)
		{
		//	*pDispIDs = 20000;
			return S_OK;
		}
#endif

//	if (wFlags & DISPATCH_PROPERTYGET)
//	{
//		if (dispidMember == 0)
//		{
//			if (pvarResult)	
//			{
//				pvarResult->vt = VT_DISPATCH;
//				IDispatch* pdisp = pThis->GetIDispatch(FALSE);
//				//IDispatch* pdisp = pThis->GetScriptDispatch();
//				pvarResult->pdispVal = pdisp;
//				if (pdisp != NULL)
//					pdisp->AddRef();		// We're returning this disp ptr
//				return S_OK;
//			}
//			
//			return DISP_E_MEMBERNOTFOUND;
//		}
//	}


	return ((IDispatch *)(&pThis->m_xDispatch))->Invoke(dispidMember, iid, lcid, wFlags, 
		pdpParams, pvarResult, pExceptionInfo, piArgErr);
}

/////////////////////////////////////////////////////////////////////////////
// CReturnValue message handlers

VARIANT CReturnValue::GetValue() 
{
	return m_vaValue;
}

void CReturnValue::SetValue(const VARIANT FAR& newValue) 
{
	m_vaValue = newValue;
}
