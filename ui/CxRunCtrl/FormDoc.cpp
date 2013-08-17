// RunDoc.cpp : implementation of the CFormDoc class
//
#include "stdafx.h"
#include "RunInc.h"
#include "RunCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int GetShiftState()
{
	BOOL bShift = (GetKeyState(VK_SHIFT) < 0);
	BOOL bCtrl  = (GetKeyState(VK_CONTROL) < 0);
	BOOL bAlt   = (GetKeyState(VK_MENU) < 0);

	return (int)(bShift + (bCtrl << 1) + (bAlt << 2));
}

/////////////////////////////////////////////////////////////////////////////
// CFormDoc

IMPLEMENT_DYNCREATE(CFormDoc, CDevObjDoc)

BEGIN_MESSAGE_MAP(CFormDoc, CDevObjDoc)
	//{{AFX_MSG_MAP(CFormDoc)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_CONNECTION_MAP(CFormDoc, CDevObjDoc)
   CONNECTION_PART(CFormDoc, __uuidof(IUserFormEvents), UserFormCP)
END_CONNECTION_MAP()

BEGIN_DISPATCH_MAP(CFormDoc, CDevObjDoc)
	//{{AFX_DISPATCH_MAP(CFormDoc)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "VisibleLayer", CXPROPID_VisibleLayer, m_ulVisibleLayer, OnVisibleLayerChanged, VT_I4)
	
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "StartUpPosition", CXPROPID_StartUpPosition, m_enumStartUpPosition, OnStartUpPositionChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "BorderStyle", CXPROPID_BorderStyle, m_enumBorderStyle, OnBorderStyleChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "ControlBox", CXPROPID_ControlBox, m_bControlBox, OnControlBoxChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "AllowDirectDraw", CXPROPID_AllowDirectDraw, m_bAllowDirectDraw, OnAllowDirectDrawChanged, VT_BOOL)

	DISP_FUNCTION_ID(CFormDoc, "Show", 4, Show, VT_EMPTY, VTS_VARIANT)
	DISP_FUNCTION_ID(CFormDoc, "Hide", 5, Hide, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CFormDoc, "Close", 6, Close, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CFormDoc, "GetHitObject", CXMETHID_GetHitObject, GetHitObject, VT_UNKNOWN, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CFormDoc, "GetObjectByName", CXMETHID_GetObjectByName, GetObjectByName, VT_UNKNOWN, VTS_BSTR)
	DISP_PROPERTY_EX_ID(CFormDoc, "Objects", CXPROPID_Objects, GetObjectCollection, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CFormDoc, "WindowHandle", CXPROPID_WindowHandle, GetWindowHandle, SetNotSupported, VT_HANDLE)
	DISP_FUNCTION_ID(CFormDoc, "ForceRefresh", CXMETHID_ForceRefresh, ForceRefresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CFormDoc, "BindDataLinksToTagGroup", CXMETHID_BindDataLinksToTagGroup, BindDataLinksToTagGroup, VT_EMPTY, VTS_BSTR)

	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "SecurityArea", CXPROPID_SecurityArea, m_strSecurityArea, OnSecurityAreaChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "ScanInterval", CXPROPID_ScanInterval, m_nScanInterval, OnScanIntervalChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "Description", CXPROPID_Description, m_strDescription, OnDescriptionChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "MousePointer", CXPROPID_MousePointer, m_enumMousePointer, OnMousePointerChanged, VT_I4)
	
	DISP_PROPERTY_EX_ID(CFormDoc, "HorizontalPosition", CXPROPID_HorPosition, GetHorizontalPosition, SetHorizontalPosition, VT_R4)
	DISP_PROPERTY_EX_ID(CFormDoc, "VerticalPosition", CXPROPID_VerPosition, GetVerticalPosition, SetVerticalPosition, VT_R4)
	DISP_PROPERTY_EX_ID(CFormDoc, "Width", CXPROPID_Width, GetWidth, SetWidth, VT_R4)
	DISP_PROPERTY_EX_ID(CFormDoc, "Height", CXPROPID_Height, GetHeight, SetHeight, VT_R4)
	
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "BackColor", CXPROPID_BackColor, m_clrBackColor, OnBackColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "Caption", CXPROPID_Caption, m_strCaption, OnCaptionChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CGroupObj, "Description", CXPROPID_Description, m_strDescription, OnDescriptionChanged, VT_BSTR)
	DISP_PROPERTY_EX_ID(CFormDoc, "Font", CXPROPID_Font, GetFont, SetFont, VT_FONT)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "ForeColor", CXPROPID_ForeColor, m_clrForeColor, OnForeColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "BrushImageFile", CXPROPID_BrushImageFileName, m_strBrushImageFile, OnBrushImageFileChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "TagGroup", CXPROPID_TagGroup, m_strTagGroup, OnTagGroupChanged, VT_BSTR)
	DISP_PROPERTY_EX_ID(CFormDoc, "AssocTagGroup", CXPROPID_AssocTagGroup, GetAssocTagGroup, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX_ID(CFormDoc, "Menu", CXPROPID_Menu, GetMenu, SetMenu, VT_BSTR)

	DISP_PROPERTY_EX_ID(CFormDoc, "Name", CXPROPID_Name, GetName, SetNotSupported, VT_BSTR)
	DISP_FUNCTION_ID(CFormDoc, "IsConnected", CXMETHID_IsConnected, IsConnected, VT_I2, VTS_BSTR)
	DISP_FUNCTION_ID(CFormDoc, "ConnectObject", CXMETHID_ConnectObject, ConnectObject, VT_EMPTY, VTS_BSTR VTS_UNKNOWN)
	DISP_FUNCTION_ID(CFormDoc, "ConnectDirect", CXMETHID_ConnectDirect, ConnectDirect, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CFormDoc, "GetConnectObject", CXMETHID_GetConnectObject, GetConnectObject, VT_UNKNOWN, VTS_BSTR)
	DISP_FUNCTION_ID(CFormDoc, "Disconnect", CXMETHID_Disconnect, Disconnect, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CFormDoc, "GetConnectState", CXMETHID_GetConnectState, GetConnectState, VT_I4, VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// {80008000-8000-8000-8000-800080008205}
static const IID IID_IUserForm =
{ 0x80008000, 0x8000, 0x8000, { 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x82, 0x05 } };

const IID IID_ISubUserForm = 
{0x1AE68DD5,0x7CFD,0x42f9,{0xAA,0xA1,0xAB,0xED,0x37,0x6D,0x4E,0x85}};

// {80008000-8000-8000-8000-800080008206}
static const IID IID_IUserFormEvents =
{ 0x80008000, 0x8000, 0x8000, { 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x82, 0x06 } };

BEGIN_INTERFACE_MAP(CFormDoc, CDevObjDoc)
	INTERFACE_PART(CFormDoc, IID_IDispatch, DispatchExtend)
	INTERFACE_PART(CFormDoc, IID_IUserForm, DispatchExtend)
	INTERFACE_PART(CFormDoc, IID_ISubUserForm, DispatchExtend)
	INTERFACE_PART(CFormDoc, IID_ICxScriptEngineSite, ScriptEngineSite)
	INTERFACE_PART(CFormDoc, IID_IConnectionPointContainer, ConnPtContainer)
	INTERFACE_PART(CFormDoc, IID_IProvideClassInfo, ProvideMultipleClassInfo)
	INTERFACE_PART(CFormDoc, IID_IProvideClassInfo2, ProvideMultipleClassInfo)
	INTERFACE_PART(CFormDoc, IID_IProvideMultipleClassInfo, ProvideMultipleClassInfo)
	INTERFACE_PART(CFormDoc, IID_IOleContainer, OleContainer)
END_INTERFACE_MAP()

//18703A54-2F8D-11D5-A468-B85CCDCD2108
const IID LIBID_CXHMIOBJLib = {0xD75F0066,0xDC61,0x4679,{0xA7,0x39,0x5B,0x04,0x6E,0x2C,0x90,0x88}};

IMPLEMENT_OLETYPELIB(CFormDoc, LIBID_CXHMIOBJLib, 1, 0)

////////////////////////////////////////////////////////////////
STDMETHODIMP_( ULONG ) CFormDoc::XScriptEngineSite::AddRef()
{
	METHOD_PROLOGUE(CFormDoc, ScriptEngineSite)

	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CFormDoc::XScriptEngineSite::Release()
{
	METHOD_PROLOGUE(CFormDoc, ScriptEngineSite)

	return pThis->ExternalRelease();
}

STDMETHODIMP CFormDoc::XScriptEngineSite::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CFormDoc, ScriptEngineSite)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CFormDoc::XScriptEngineSite::EvaluateExpression(BSTR bstrSource, VARIANT* pvaValue)
{
	METHOD_PROLOGUE(CFormDoc, ScriptEngineSite)

	BSTR bstrName = pThis->GetDisplayName().AllocSysString();

	HRESULT hr = pThis->GetScriptEngine()->Evaluation(bstrSource, pvaValue, bstrName);	
	::SysFreeString(bstrName);
	return hr;
}

STDMETHODIMP CFormDoc::XScriptEngineSite::Execute(BSTR bstrStatement)
{
	METHOD_PROLOGUE(CFormDoc, ScriptEngineSite)

	BSTR bstrName = pThis->GetDisplayName().AllocSysString();
	pThis->GetScriptEngine()->Execute(bstrStatement, bstrName);
	::SysFreeString(bstrName);
	
	return S_OK;
}

STDMETHODIMP CFormDoc::XScriptEngineSite::PreTreatExpression(BSTR bstrSource, BSTR* pbstrSource)
{
	METHOD_PROLOGUE(CFormDoc, ScriptEngineSite)

	pThis->GetScriptEngine()->PreTreatExpression(bstrSource, pbstrSource);	
	
	return S_OK;
}

STDMETHODIMP CFormDoc::XScriptEngineSite::PreTreatExpressionEx(BSTR bstrSource, BSTR* pbstrSource, VARIANT* pvaMin, VARIANT* pvaMax)
{
	METHOD_PROLOGUE(CFormDoc, ScriptEngineSite)

	pThis->GetScriptEngine()->PreTreatExpression(bstrSource, pbstrSource, pvaMin, pvaMax);	

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// CCtrlItem::XOleContainer
///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) CFormDoc::XOleContainer::AddRef()
{
	METHOD_PROLOGUE_EX_(CFormDoc, OleContainer)
	return (ULONG)pThis->InternalAddRef();
}

STDMETHODIMP_(ULONG) CFormDoc::XOleContainer::Release()
{
	METHOD_PROLOGUE_EX_(CFormDoc, OleContainer)
	return (ULONG)pThis->InternalRelease();
}

STDMETHODIMP CFormDoc::XOleContainer::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CFormDoc, OleContainer)
	return (HRESULT)pThis->InternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CFormDoc::XOleContainer::ParseDisplayName(LPBINDCTX,
	LPOLESTR, ULONG*, LPMONIKER*)
{
	return E_NOTIMPL;
}

STDMETHODIMP CFormDoc::XOleContainer::EnumObjects(DWORD dwFlags,
	LPENUMUNKNOWN* ppEnumUnknown)
{
	METHOD_PROLOGUE_EX_(CFormDoc, OleContainer)

	*ppEnumUnknown = NULL;
	HRESULT hr = S_OK;
	CEnumUnknown* pEnum = NULL;
	UINT cObjects = 0;
	LPUNKNOWN* ppUnk = NULL;

	TRY
	{
		if (dwFlags & OLECONTF_EMBEDDINGS)
		{
			cObjects = pThis->m_objects.GetCount();
			ppUnk = new LPUNKNOWN[cObjects];
			UINT i = 0;
			POSITION pos = pThis->m_objects.GetHeadPosition();
			CDevObj* pObj;
			while (pos != NULL)
			{
				pObj = pThis->m_objects.GetNext(pos);
				pObj->GetObjectUnknown()->AddRef();
				ppUnk[i++] = pObj->GetObjectUnknown();
			}

			ASSERT(cObjects == i);
		}
		pEnum = new CEnumUnknown(ppUnk, cObjects);
	}
	CATCH_ALL(e)
	{
		// Note: DELETE_EXCEPTION(e) not necessary
		hr = E_OUTOFMEMORY;
	}
	END_CATCH_ALL

	// clean up in case of failure
	if (SUCCEEDED(hr))
	{
		ASSERT(pEnum != NULL);
		*ppEnumUnknown = (IEnumUnknown*)&pEnum->m_xEnumVOID;
	}
	else
	{
		ASSERT(pEnum == NULL);
		ASSERT(*ppEnumUnknown == NULL);

		if (ppUnk != NULL)
		{
			for (UINT i = 0; i < cObjects; i++)
			{
				ASSERT(ppUnk[i] != NULL);
				ppUnk[i]->Release();
			}
		}
	}

	return hr;
}

STDMETHODIMP CFormDoc::XOleContainer::LockContainer(BOOL)
{
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
// CFormDoc::XDispatchExtend

STDMETHODIMP_( ULONG ) CFormDoc::XDispatchExtend::AddRef()
{
	METHOD_PROLOGUE(CFormDoc, DispatchExtend)

    return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CFormDoc::XDispatchExtend::Release()
{
	METHOD_PROLOGUE(CFormDoc, DispatchExtend)

	return pThis->ExternalRelease();
}

STDMETHODIMP CFormDoc::XDispatchExtend::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CFormDoc, DispatchExtend)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CFormDoc::XDispatchExtend::GetIDsOfNames(
	REFIID iid, LPOLESTR* ppszNames, UINT nNames, LCID lcid, DISPID* pDispIDs)
{
	USES_CONVERSION;
	USES_CHINESEVAR_CONVERSION;

	(void)lcid;

	METHOD_PROLOGUE(CFormDoc, DispatchExtend)
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

		// 窗体中对象
		void *pTemp;
		CString strName = *ppszNames;
		strName.MakeUpper();
		if (pThis->m_mapNameToDispid.Lookup(strName, pTemp))
		{
			*pDispIDs = DISPID(pTemp);
			return S_OK;
		}

		// 画面文档中所有子文档
		CDevDoc::CSubDoc* pSubDoc = pThis->m_pDocument->FindSubDocumentByName(VARA2C(strName));
		if (pSubDoc != NULL)
		{
			*pDispIDs = pSubDoc->m_dwDocumentNumber + 0x80080000;
			return S_OK;	
		}

		if (strName == _T("THIS"))
		{
			*pDispIDs = 0;
			return S_OK;
		}
	}
	
	return ((IDispatch *)(&pThis->m_xDispatch))->GetIDsOfNames(iid, ppszNames, nNames, lcid, pDispIDs);
}

STDMETHODIMP CFormDoc::XDispatchExtend::GetTypeInfo(
	UINT iTypeInfo, LCID lcid, ITypeInfo** ppTypeInfo)
{
	(void)iTypeInfo;
	(void)lcid;
	(void)ppTypeInfo;

	METHOD_PROLOGUE(CFormDoc, DispatchExtend)
	ASSERT_VALID(pThis);

	return ((IDispatch *)(&pThis->m_xDispatch))->GetTypeInfo(iTypeInfo, lcid, ppTypeInfo);
}

STDMETHODIMP CFormDoc::XDispatchExtend::GetTypeInfoCount(
   UINT* pnInfoCount )
{
	METHOD_PROLOGUE(CFormDoc, DispatchExtend)
	ASSERT_VALID(pThis);

	return ((IDispatch *)(&pThis->m_xDispatch))->GetTypeInfoCount(pnInfoCount);
}

STDMETHODIMP CFormDoc::XDispatchExtend::Invoke(
	DISPID dispidMember, REFIID iid, LCID lcid, WORD wFlags,
	DISPPARAMS* pdpParams, VARIANT* pvarResult, EXCEPINFO* pExceptionInfo,
	UINT* piArgErr)
{
	(void)iid;
	(void)lcid;
	(void)wFlags;
	(void)pdpParams;
	(void)pExceptionInfo;
	(void)piArgErr;

	METHOD_PROLOGUE(CFormDoc, DispatchExtend)
	ASSERT_VALID(pThis);

	if (wFlags & DISPATCH_PROPERTYGET)
	{
		if ((dispidMember & 0x80080000) == 0x80080000)
		{
			dispidMember &= 0xffff; 
			CDevDoc::CSubDoc* pSubDoc = pThis->m_pDocument->FindSubDocumentByNumber(dispidMember);
			ASSERT(pSubDoc != NULL);
			
			if (pSubDoc != NULL/* && pSubDoc->GetType() == CDevDoc::enumFormDoc*/)
			{
				CFormDoc* pFormDoc = (CFormDoc*)pSubDoc->m_pChildDoc;
				if (pFormDoc == NULL)
				{
					pFormDoc = (CFormDoc *)pSubDoc->LoadChildDocument();
					pFormDoc->ReadyToRun();
				}
				if (pFormDoc != NULL)
				{
					pvarResult->vt = VT_DISPATCH;
					pvarResult->pdispVal = NULL;

//					CFormScrEng* pEngine = pFormDoc->GetScriptEngine();
//					if (pEngine != NULL)
//					{
//						USES_CONVERSION;
//						pEngine->GetScriptDispatch(T2COLE(pFormDoc->GetDisplayName()), &pvarResult->pdispVal);
//					}
					if (pvarResult->pdispVal == NULL)
						pvarResult->pdispVal = pFormDoc->GetIDispatch(TRUE);
				
					return S_OK;
				}
			}

			return DISP_E_MEMBERNOTFOUND;
		}

		if (dispidMember == 0)
		{
			if (pvarResult)	
			{
				pvarResult->vt = VT_DISPATCH;
				IDispatch* pdisp = pThis->GetIDispatch(FALSE);
				//IDispatch* pdisp = pThis->GetScriptDispatch();
				pvarResult->pdispVal = pdisp;
				if (pdisp != NULL)
					pdisp->AddRef();		// We're returning this disp ptr
				return S_OK;
			}
			
			return DISP_E_MEMBERNOTFOUND;
		}

		if (dispidMember == CXPROPID_TagGroup)
		{
			if (pvarResult)	
			{
				pvarResult->vt = VT_DISPATCH;
				IDispatch* pdisp = pThis->GetTagProvider();
				pvarResult->pdispVal = pdisp;
				if (pdisp != NULL)
					pdisp->AddRef();		// We're returning this disp ptr
				return S_OK;
			}
			
			return DISP_E_MEMBERNOTFOUND;
		}
	}

	if ((dispidMember & CXDISPID_EXTPROP) != CXDISPID_EXTPROP)
	{
//		if (pThis->m_bHasClosed && (wFlags & DISPATCH_METHOD) != 0)
//			return DISP_E_MEMBERNOTFOUND;

		return ((IDispatch *)(&pThis->m_xDispatch))->Invoke(dispidMember, iid, lcid, wFlags, 
			pdpParams, pvarResult, pExceptionInfo, piArgErr);
	}

	if ((wFlags & DISPATCH_PROPERTYGET) != 0 || (wFlags & DISPATCH_PROPERTYPUT) != 0)
	{
		dispidMember &= 0x7fff;
	
		if (dispidMember >= pThis->GetObjects()->GetCount())
			return DISP_E_MEMBERNOTFOUND;
			
		IDispatch* pdisp = pThis->m_ppObjects[dispidMember];
		
		if ((wFlags & DISPATCH_PROPERTYGET) != 0)
		{
			if (pvarResult)	
			{
				pvarResult->vt = VT_DISPATCH;
				pvarResult->pdispVal = pdisp;
				//if (pdisp != NULL)
				pdisp->AddRef();		// We're returning this disp ptr
				return S_OK;
			}
		}
		else
		{
			return pdisp->Invoke(0, iid, lcid, wFlags,
				pdpParams, pvarResult, pExceptionInfo, piArgErr);
		}
	}

  	return DISP_E_MEMBERNOTFOUND;
}

/////////////////////////////////////////////////////////////////////////////
// CFormDoc::XProvideMultipleClassInfo

STDMETHODIMP_(ULONG) CFormDoc::XProvideMultipleClassInfo::AddRef()
{
	METHOD_PROLOGUE_EX_(CFormDoc, ProvideMultipleClassInfo)

	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CFormDoc::XProvideMultipleClassInfo::Release()
{
	METHOD_PROLOGUE_EX_(CFormDoc, ProvideMultipleClassInfo)

	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CFormDoc::XProvideMultipleClassInfo::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CFormDoc, ProvideMultipleClassInfo)

	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CFormDoc::XProvideMultipleClassInfo::GetClassInfo(
	LPTYPEINFO* ppTypeInfo)
{
	METHOD_PROLOGUE_EX(CFormDoc, ProvideMultipleClassInfo)

	if (pThis->IsMainForm())
		return pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), __uuidof(UserForm), ppTypeInfo);

//	return theApp.m_pExtTypeLib->GetTypeInfoOfGuid(__uuidof(UserForm), ppTypeInfo);
	return pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), __uuidof(SubUserForm), ppTypeInfo);
}

STDMETHODIMP CFormDoc::XProvideMultipleClassInfo::GetGUID(DWORD dwGuidKind,
	GUID* pGUID)
{
	METHOD_PROLOGUE_EX_(CFormDoc, ProvideMultipleClassInfo)

	if (dwGuidKind == GUIDKIND_DEFAULT_SOURCE_DISP_IID)
	{
		*pGUID = __uuidof(IUserFormEvents);
		return NOERROR;
	}
	else
	{
		*pGUID = GUID_NULL;
		return E_INVALIDARG;
	}
}

STDMETHODIMP CFormDoc::XProvideMultipleClassInfo::GetMultiTypeInfoCount(
	ULONG *pcti)
{
	METHOD_PROLOGUE(CFormDoc, ProvideMultipleClassInfo)

	*pcti = 2;

	return S_OK;
}

STDMETHODIMP CFormDoc::XProvideMultipleClassInfo::GetInfoOfIndex(
	ULONG iti, DWORD dwFlags, ITypeInfo** pptiCoClass, 
	DWORD* pdwTIFlags, ULONG* pcdispidReserved, IID* piidPrimary, 
	IID* piidSource)
{
	METHOD_PROLOGUE(CFormDoc, ProvideMultipleClassInfo)

	HRESULT    hr        = S_OK;
	ITypeInfo *ptinfoOut = NULL;

//	if (pThis->m_bClose)
//		return E_UNEXPECTED;

	if (iti > 1)
		hr = E_FAIL;

	if (pThis->m_ptinfoCls == NULL)
		pThis->BuildTypeInfo();

	if (dwFlags & MULTICLASSINFO_GETTYPEINFO)
    {
		if (!pptiCoClass || !pdwTIFlags)
			hr = E_INVALIDARG;
		else
		{
			*pdwTIFlags  = 0L;
			*pptiCoClass = NULL;
			if (iti == 0)
			{
				ptinfoOut = pThis->m_ptinfoCls;
			}
			else
			{
			//	theApp.m_pExtTypeLib->GetTypeInfoOfGuid(__uuidof(UserForm), &ptinfoOut);

				if (pThis->IsMainForm())
					pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), __uuidof(UserForm), &ptinfoOut);
				else
					pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), __uuidof(SubUserForm), &ptinfoOut);
			}
		}
    }

	if (dwFlags & MULTICLASSINFO_GETNUMRESERVEDDISPIDS)
    {
		if (!pcdispidReserved)
			hr = E_INVALIDARG;
		else
		*pcdispidReserved = 256;
    }

	if (dwFlags & MULTICLASSINFO_GETIIDPRIMARY)
    {
		if (!piidPrimary)
			hr = E_INVALIDARG;
		else if (iti == 0)
			*piidPrimary = pThis->m_iidSubObj;
		else
		{
			if (pThis->IsMainForm())
				*piidPrimary = IID_IUserForm;
			else
				*piidPrimary = IID_ISubUserForm;
		}
    }

	if (dwFlags & MULTICLASSINFO_GETIIDSOURCE)
    {
		if (!piidSource)
			hr = E_INVALIDARG;
		else if (iti == 0)
			*piidSource = IID_NULL;
		else
			*piidSource = __uuidof(IUserFormEvents);
    }

	// Fill in ptinfo out param
	if (hr)
    {
		if (pptiCoClass)
		*pptiCoClass = NULL;
    }
	else if (pptiCoClass)
    {
		*pptiCoClass = ptinfoOut;
		(*pptiCoClass)->AddRef();
    }

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// CFormDoc construction/destruction

CFormDoc::CFormDoc(CDevDoc* pDoc) :
	m_pDocument(pDoc),
	m_pFormSubDoc(NULL),
	m_pFormObj(NULL),
	m_pFontHolder(NULL),
	m_pTagProvider(NULL),
	m_pScriptEngine(NULL)
{
	m_descFont.lpstrName = NULL;

	m_ptinfoInt = NULL;
	m_ptinfoCls = NULL;
	m_ptlSubObj = NULL;

	m_ppObjects = NULL;

	m_bParsedScript = FALSE;

	m_pImageBackGround = NULL;

	//如果不是子文档，支持符合文件
//	if (m_pDocument == NULL)
		EnableCompoundFile();

	//自动化支持
	EnableAutomation();
	//连接点支持
	EnableConnections();

	AfxOleLockApp();
}

CFormDoc::~CFormDoc()
{
//	m_pFormSubDoc->m_pChildDoc = NULL;
	m_pDocument = NULL;

	if (m_pImageBackGround)
		delete m_pImageBackGround;

	AfxOleUnlockApp();
}

BOOL CFormDoc::OnNewDocument()
{
	if (m_pDocument != NULL)
	{
		if(m_lpRootStg == NULL)
			m_lpRootStg = m_pDocument->OpenSubDocumentStorage(m_pFormSubDoc);
	}
	else
	{
		if (!CDevObjDoc::OnNewDocument())
			return FALSE;
	}

	return Initialize();
}

////////////////////////////////////////////////////////////////////////////
//功 能：初始化
////////////////////////////////////////////////////////////////////////////
BOOL CFormDoc::Initialize()
{
//	HRESULT hResult;
  
//	hResult = InitDefaultAmbientProperties();
//	if (FAILED(hResult))
//	{
//		return FALSE;
//	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
//功 能：初始化缺省环境属性
//描 述：在窗口属性导入后调用（因字体、前景及背景颜色依赖于窗体颜色）
////////////////////////////////////////////////////////////////////////////
HRESULT CFormDoc::InitAmbientProperties()
{
	LPCOLESTR pszFaceName;
	CAmbientProperty* pProp;

	//////////////////////////////////////////////////////////////////////////////////
	//环境属性

	//AllowWindowlessActivation
	pProp = new CAmbientProperty(DISPID_AMBIENT_ALLOWWINDOWLESSACTIVATION, 
		"AllowWindowlessActivation", COleVariant(VAR_TRUE, VT_BOOL), 
		VTI_BOOL, TRUE);
	m_apAmbientProperties.Add(pProp);

	//BackColor，由m_clrBackgroundColor决定
	pProp = new CAmbientProperty(DISPID_AMBIENT_BACKCOLOR, "BackColor",
		COleVariant(long(m_clrBackColor), VT_I4), VTI_COLOR,
		TRUE);
	m_apAmbientProperties.Add(pProp);

	//ForeColor，由m_clrForeColor决定
	pProp = new CAmbientProperty( DISPID_AMBIENT_FORECOLOR, "ForeColor",
		COleVariant(long(m_clrForeColor), VT_I4), VTI_COLOR,
		TRUE);
	m_apAmbientProperties.Add(pProp);

	//LocaleID
	pProp = new CAmbientProperty(DISPID_AMBIENT_LOCALEID, "LocaleID", COleVariant(
		long(GetUserDefaultLCID()), VT_I4), VTI_I4, TRUE);
	m_apAmbientProperties.Add(pProp);

	//UserMode
	pProp = new CAmbientProperty(DISPID_AMBIENT_USERMODE, "UserMode", COleVariant(
		VAR_TRUE, VT_BOOL), VTI_BOOL, TRUE);
	m_apAmbientProperties.Add(pProp);

	//DisplayAsDefault
	pProp = new CAmbientProperty(DISPID_AMBIENT_DISPLAYASDEFAULT, "DisplayAsDefault", COleVariant(
		VAR_FALSE, VT_BOOL), VTI_BOOL, TRUE);
	m_apAmbientProperties.Add(pProp);

	//创建字体
	delete m_pFontHolder;
	m_pFontHolder = new CFontHolder(NULL);
	if( m_pFontHolder == NULL )
	{
		AfxThrowMemoryException();
	}
	m_descFont.cbSizeofstruct = sizeof(m_descFont);
	pszFaceName = L"宋体";
	delete[] m_descFont.lpstrName;
	m_descFont.lpstrName = new OLECHAR[ocslen(pszFaceName)+1];
	if (m_descFont.lpstrName == NULL)
	{
		AfxThrowMemoryException();
	}
	ocscpy( m_descFont.lpstrName, pszFaceName );
	m_descFont.cySize.Lo = 90000L;
	m_descFont.cySize.Hi = 0;
	m_descFont.sWeight = FW_NORMAL;
	m_descFont.sCharset = 0;
	m_descFont.fItalic = FALSE;
	m_descFont.fUnderline = FALSE;
	m_descFont.fStrikethrough = FALSE;
	m_pFontHolder->InitializeFont(&m_descFont);

	COleVariant varFont;
	varFont.vt = VT_DISPATCH;
	varFont.pdispVal = m_pFontHolder->GetFontDispatch();

	//Font
	pProp = new CAmbientProperty(DISPID_AMBIENT_FONT, "Font", varFont,
		VTI_FONT, TRUE);
	m_apAmbientProperties.Add(pProp);

	//MessageReflect，值的设定与CxDev不同
	pProp = new CAmbientProperty(DISPID_AMBIENT_MESSAGEREFLECT, "MessageReflect",
		COleVariant(VARIANT_FALSE, VT_BOOL), VTI_BOOL, TRUE);
	m_apAmbientProperties.Add(pProp);

	//ShowGrabHandles
	pProp = new CAmbientProperty(DISPID_AMBIENT_SHOWGRABHANDLES, "ShowGrabHandles",
		COleVariant(VARIANT_FALSE, VT_BOOL), VTI_BOOL, TRUE);
	m_apAmbientProperties.Add(pProp);

	//ShowHatching
	pProp = new CAmbientProperty(DISPID_AMBIENT_SHOWHATCHING, "ShowHatching",
		COleVariant(VARIANT_FALSE, VT_BOOL), VTI_BOOL, TRUE);
	m_apAmbientProperties.Add(pProp);

	//DocHostFlags
	pProp = new CAmbientProperty(DISPID_AMBIENT_DOCHOSTFLAGS, "DocHostFlags",
		COleVariant(/*DOCHOSTUIFLAG_NO3DBORDER*/long(4), VT_I4), VTI_I4, TRUE);
	m_apAmbientProperties.Add(pProp);

	//DocHostDoubleClickFlags
	pProp = new CAmbientProperty(DISPID_AMBIENT_DOCHOSTDOUBLEFLAGS, "DocHostDoubleClickFlags",
		COleVariant(/*DOCHOSTUIDBLCLK_DEFAULT*/long(0), VT_I4), VTI_I4, TRUE);
	m_apAmbientProperties.Add(pProp);

	//AllowContextMenu
	pProp = new CAmbientProperty(DISPID_AMBIENT_ALLOWCONTEXTMENU, "AllowContextMenu",
		COleVariant(VARIANT_TRUE, VT_BOOL), VTI_BOOL, TRUE);
	m_apAmbientProperties.Add(pProp);

	//AllowShowUI
	pProp = new CAmbientProperty(DISPID_AMBIENT_ALLOWSHOWUI, "AllowShowUI",
		COleVariant(VARIANT_FALSE, VT_BOOL), VTI_BOOL, TRUE);
	m_apAmbientProperties.Add(pProp);

	//OptionKeyPath
	pProp = new CAmbientProperty(DISPID_AMBIENT_OPTIONKEYPATH, "OptionKeyPath",
		COleVariant("", VT_BSTR), VTI_BSTR, TRUE);
	m_apAmbientProperties.Add(pProp);

	return S_OK;
}

////////////////////////////////////////////////////////////////////////////
//功 能：窗体属性序列化
///////////////////////////////////////////////////////////////////////////// 
void CFormDoc::SerializeProperty(CArchive& ar)
{
	//运行模式不支持存储
	ASSERT(ar.IsLoading());
	
	ar >> m_clrBackColor;
	ar >> m_clrForeColor;
	ar >> m_strCaption;
	
	ar >> m_strBrushImageFile;
	ASSERT(m_pImageBackGround == NULL);
	if (m_strBrushImageFile != "")
	{
		BSTR bstr = m_strBrushImageFile.AllocSysString();
		m_pImageBackGround = Gdiplus::Image::FromFile(bstr);
		SysFreeString(bstr);
	}

	ar >> m_position;

	ar >> m_nScanInterval;
	ar >> m_strSecurityArea;
	ar >> m_strTagGroup;

	if (m_dwVer > _SUPERCX_VER_0203)
		ar >> m_strMenu;

	if (m_dwVer != _SUPERCX_OLDVER)
		ar >> m_strDescription;

	if (m_dwVer > _SUPERCX_VER_0100)
	{
		long lTemp;
		ar >> lTemp;
		m_enumMousePointer = enumMousePointer(lTemp);

		if (m_dwVer > _SUPERCX_VER_0101)
		{
			ar >> lTemp;
			m_enumStartUpPosition = enumStartUpPosition(lTemp);
			ar >> lTemp;
			m_enumBorderStyle = enumBorderStyle(lTemp);

			if (m_dwVer > _SUPERCX_VER_0208)
				ar >> m_bControlBox;
			else
				m_bControlBox = TRUE;

			if (m_dwVer < _SUPERCX_VER_0207)
			{
				BOOL bShowModal;
				ar >> bShowModal;
			}

			if (m_dwVer > _SUPERCX_VER_0209)
				ar >> m_bAllowDirectDraw;
			else
				m_bAllowDirectDraw = FALSE;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//功 能：文档序列化
///////////////////////////////////////////////////////////////////////////// 
void CFormDoc::Serialize(CArchive& ar)
{
	//运行模式不支持存储
	ASSERT(ar.IsLoading());

	if (m_pDocument != NULL && m_pDocument->m_dwVer == _SUPERCX_OLDVER)
		m_dwVer = _SUPERCX_OLDVER;
	else	
		ar >> m_dwVer;

	//导入窗体属性
	SerializeProperty(ar);
	//序列化动态属性
	SerializeDynamicProperty(ar);

	//初始化环境属性
	VERIFY(InitAmbientProperties() == S_OK);

	//脚本
	ar >> m_strScript;

	ar >> m_nLayer;
	ar >> m_ulVisibleLayer;
	ar >> m_ulLockLayer;

	//调用基类进行客户项目序列化
	CDevObjDoc::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////
//功 能：保存窗体及所有对象属性
/////////////////////////////////////////////////////////////////////////
void CFormDoc::SaveProperty()
{
	HRESULT hResult;
	COleStreamFile file;

	hResult = StgCreateDocfile(NULL, STGM_CREATE|STGM_READWRITE|
		STGM_DELETEONRELEASE|STGM_SHARE_EXCLUSIVE, 0, &m_pStorage);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	if (!file.CreateStream(m_pStorage, _T("Contents"), CFile::modeReadWrite|
		CFile::shareExclusive|CFile::modeCreate))
	{
		AfxThrowOleException(E_FAIL);
	}

	CArchive ar(&file, CArchive::store);

	SerializeProperty(ar);

	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CDevObj* pObj = m_objects.GetNext(pos);
//		pObj->SaveProperty(ar);
	}

	ar.Flush();
	file.Close();
}

/////////////////////////////////////////////////////////////////////////
//功 能：恢复窗体及所有对象属性
/////////////////////////////////////////////////////////////////////////
void CFormDoc::RestoreProperty()
{
	COleStreamFile file;

	if (!file.OpenStream(m_pStorage, _T("Contents"), STGM_READ|
		STGM_SHARE_EXCLUSIVE))
	{
		return;
	}

	CArchive ar(&file, CArchive::load|CArchive::bNoFlushOnDelete);

	SerializeProperty(ar);
	
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CDevObj* pObj = m_objects.GetNext(pos);
//		pObj->RestoreProperty(ar);
	}

	ar.Flush();
	file.Close();
}

#define USE_CUSTOM_EVENT_HANDLER

//////////////////////////////////////////////////////////////////////////////
//功 能：创建类型信息
//////////////////////////////////////////////////////////////////////////////
HRESULT CFormDoc::BuildTypeInfo()
{
	USES_CONVERSION;
	//标准类型信息库
	static GUID guidStdOle = {0x00020430,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46};

	ITypeLib*        ptlStdOle   = NULL;
	ITypeInfo*       ptinfoIDisp = NULL;
	ICreateTypeLib * pcreatetl   = NULL;
	ICreateTypeInfo* pctiCoClass = NULL;		// For creating the CoClass
	ICreateTypeInfo* pctiProps   = NULL;		// For creating the Primary (Property) Dispinterface
	FUNCDESC         funcdesc    = {0};
	TYPEDESC         tdescUser   = {0};
	ELEMDESC         edArg       = {0};
	HREFTYPE         hreftype;
	int              index;
	int				 nProp;
	HRESULT          hr; 

	POSITION posObj;
	CDevObj* pObj;
	ITypeInfo* ptiObjCls;
	IDispatch* pDispatch;
	IProvideClassInfo* pProvideClassInfo;
	CString strDisplayName;
	DISPID dispid;
	TCHAR szStrBuf[256];
	LPWSTR pOLEStr;

	
	//已经创建退出
	if (m_ptinfoInt && m_ptinfoCls)
		return S_OK;

	CoCreateGuid(&m_libidSubObj);
	CoCreateGuid(&m_clsidSubObj);
	CoCreateGuid(&m_iidSubObj);
	
	// 创建类型信息库
	CHECKSZ(CreateTypeLib(SYS_WIN32, L"" /* szFile */, &pcreatetl), "CreateTypeLib FAILED");

	// 设置类型信息库属性
	CHECKSZ(pcreatetl->SetGuid(m_libidSubObj), "TL SetGuid FAILED");
	CHECKSZ(pcreatetl->SetVersion(1, 0), "TL SetVersion FAILED");
	CHECKSZ(pcreatetl->SetName(L"Sprds"), "TL SetName FAILED");
	CHECKSZ(pcreatetl->SetLcid(LOCALE_SYSTEM_DEFAULT), "TL SetLcid FAILED");

	// 取IDispatch接口信息
	CHECKSZ(LoadRegTypeLib(guidStdOle, STDOLE_MAJORVERNUM, STDOLE_MINORVERNUM, STDOLE_LCID, &ptlStdOle), "LoadRegTypeLib FAILED");
	CHECKSZ(ptlStdOle->GetTypeInfoOfGuid(IID_IDispatch, &ptinfoIDisp), "GetTypeInfoOfGuid (IID_IDispatch) FAILED");

	// Create TypeInfo
	CHECKSZ(pcreatetl->CreateTypeInfo(L"FormSubObjs", TKIND_DISPATCH, &pctiProps), "Create TI FAILED");
	CHECKSZ(pctiProps->SetGuid(m_iidSubObj), "cti SetGuid FAILED");
	CHECKSZ(pctiProps->SetVersion(1, 0), "cti SetVersion FAILED");
	CHECKSZ(pctiProps->AddRefTypeInfo(ptinfoIDisp, &hreftype), "cti AddRefTypeInfoo FAILED"); 	// Reference StdOLE
	CHECKSZ(pctiProps->AddImplType(0, hreftype), "cti AddImplType FAILED");

	funcdesc.funckind = FUNC_DISPATCH;
	funcdesc.invkind  = INVOKE_PROPERTYGET;
	funcdesc.callconv = CC_STDCALL;
	funcdesc.cScodes  = -1;
	funcdesc.wFuncFlags = FUNCFLAG_FSOURCE;
	funcdesc.elemdescFunc.tdesc.vt = VT_PTR;
	funcdesc.elemdescFunc.tdesc.lptdesc = &tdescUser;
	tdescUser.vt = VT_USERDEFINED;
	
	m_ppObjects = (IDispatch **)malloc(sizeof(IDispatch *) * m_objects.GetCount());

	// 遍历窗体对象链表，为每个对象添加以其名字命名的属性
	posObj = m_objects.GetHeadPosition();
	index = 0;
	nProp = 0;
	while (posObj)
	{
		pObj = m_objects.GetNext(posObj);

		//以名字建立快速DISPID索引
		//不使用宏，避免对象多时需要太多堆栈空间
		ChineseToASCII(pObj->GetDisplayName(), szStrBuf);
		strDisplayName = szStrBuf;
		strDisplayName.MakeUpper();
		dispid = CXDISPID_EXTPROP | index;
		m_mapNameToDispid.SetAt(strDisplayName, (void *)dispid);

		//对象自动化接口数组
		pDispatch = pObj->GetObjectDispatch();
		m_ppObjects[index] = pDispatch;
		
		if (pObj->GetEnventSinkStatus())
		{
#ifdef USE_CUSTOM_EVENT_HANDLER
			pObj->AdviseEnventSink();
#else	
			//ATL对象类型信息在一个实例加载之后，就一直驻在内存中
			if (pDispatch != NULL)
				pDispatch->QueryInterface(IID_IProvideClassInfo, (void **)&pProvideClassInfo);
			if (pProvideClassInfo)
			{
				ptiObjCls = NULL;
				pProvideClassInfo->GetClassInfo(&ptiObjCls);
				if (ptiObjCls != NULL)
				{
					CHECKCL(pctiProps->AddRefTypeInfo(ptiObjCls, &tdescUser.hreftype));
					ptiObjCls->Release();
				}
				pProvideClassInfo->Release();
			}
		
			funcdesc.memid = dispid;
			
			CHECKCL(pctiProps->AddFuncDesc(nProp, &funcdesc));
			pOLEStr = T2OLE(strDisplayName);
			CHECKCL(pctiProps->SetFuncAndParamNames(nProp, &pOLEStr, 1));
	
			nProp++;
#endif
		}

		index++;
		continue;

	CheckLoop:
		// Do any clean up here

		CHECKSZ(hr,"BuildTypeInfo FAILED");
		break;
	}
	
	// 完成属性的类型信息
	CHECKSZ(pctiProps->LayOut(), "BuildTypeInfo FAILED");
	CHECKSZ(pctiProps->QueryInterface(IID_ITypeInfo, (void **)&m_ptinfoInt), "BuildTypeInfo FAILED");

	// Now we have to create a coclass for the interface
	CHECKSZ(pcreatetl->CreateTypeInfo(L"Form", TKIND_COCLASS, &pctiCoClass), "BuildTypeInfo FAILED");
	CHECKSZ(pctiCoClass->SetGuid(m_clsidSubObj), "BuildTypeInfo FAILED");
	CHECKSZ(pctiCoClass->SetVersion(1, 0), "BuildTypeInfo FAILED");

	// Add the Page Property dispinterface to coclass
	CHECKSZ(pctiCoClass->AddRefTypeInfo(m_ptinfoInt, &hreftype), "BuildTypeInfo FAILED");
	CHECKSZ(pctiCoClass->AddImplType(0, hreftype), "BuildTypeInfo FAILED");
	CHECKSZ(pctiCoClass->SetImplTypeFlags(0, IMPLTYPEFLAG_FDEFAULT), "BuildTypeInfo FAILED");

	// Finish off the CoClass
	CHECKSZ(pctiCoClass->LayOut(), "BuildTypeInfo FAILED");
	CHECKSZ(pctiCoClass->QueryInterface(IID_ITypeInfo, (void **)&m_ptinfoCls), "BuildTypeInfo FAILED");

	// 完成库创建
	CHECKSZ(pcreatetl->QueryInterface(IID_ITypeLib, (void **)&m_ptlSubObj), "BuildTypeLib FAILED");

CleanUp:
	// Code to release refs aquired during page build.
	if (pctiCoClass)
		pctiCoClass->Release();
	if (pctiProps)
		pctiProps->Release();
	if (ptinfoIDisp)
		ptinfoIDisp->Release();
	if (ptlStdOle)
		ptlStdOle->Release();
	if (pcreatetl)
		pcreatetl->Release();

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// CFormDoc diagnostics

#ifdef _DEBUG
void CFormDoc::AssertValid() const
{
	CDevObjDoc::AssertValid();
}

void CFormDoc::Dump(CDumpContext& dc) const
{
	CDevObjDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormDoc commands

////////////////////////////////////////////////////////////////////////
//功 能：激活事件
////////////////////////////////////////////////////////////////////////
void CFormDoc::FireEventV(DISPID dispid, BYTE* pbParams,
	va_list argList)
{
	COleDispatchDriver driver;

	POSITION pos = m_xUserFormCP.GetStartPosition();
	LPDISPATCH pDispatch;
	while (pos != NULL)
	{
		pDispatch = (LPDISPATCH)m_xUserFormCP.GetNextConnection(pos);
		ASSERT(pDispatch != NULL);
		driver.AttachDispatch(pDispatch, FALSE);
		TRY
			driver.InvokeHelperV(dispid, DISPATCH_METHOD, VT_EMPTY, NULL,
				pbParams, argList);
		END_TRY
		driver.DetachDispatch();
	}
}

void AFX_CDECL CFormDoc::FireEvent(DISPID dispid, BYTE* pbParams, ...)
{
	va_list argList;
	va_start(argList, pbParams);
	FireEventV(dispid, pbParams, argList);
	va_end(argList);
}

//////////////////////////////////////////////////////////////////////////
//功 能：清空一些附带产生的
//描 述：由void CFormDoc::DeleteContents()调用
////////////////////////////////////////////////////////////////////////////
void CFormDoc::Cleanup()
{
	if (m_pImageBackGround)
	{
		delete m_pImageBackGround;
		m_pImageBackGround = NULL;
	}

	m_dynamicobjects.RemoveAll();
	m_timerobjects.RemoveAll();

	if (m_pTagProvider != NULL)
	{
		m_pTagProvider->Release(); 
		m_pTagProvider = NULL;
	}

	if (m_ppObjects != NULL)
	{
		free(m_ppObjects);
		m_ppObjects = NULL;
	}

	m_mapNameToDispid.RemoveAll();

	if (m_ptinfoInt != NULL)
	{
		m_ptinfoInt->Release();
		m_ptinfoInt = NULL;
	}
	if (m_ptinfoCls != NULL)
	{
		m_ptinfoCls->Release();
		m_ptinfoCls = NULL;
	}
	if (m_ptlSubObj != NULL)
	{
		m_ptlSubObj->Release();
		m_ptlSubObj = NULL;
	}

	//清除环境属性
	for (int iProp = 0; iProp < m_apAmbientProperties.GetSize(); iProp++)
	{
		  delete m_apAmbientProperties[iProp];
	}
	m_apAmbientProperties.SetSize(0);

	delete m_pFontHolder;
	m_pFontHolder = NULL;

	delete[] m_descFont.lpstrName;
	m_descFont.lpstrName = NULL;

	m_listSubformModule.RemoveAll();

	//脚本引擎重置
	if (m_pScriptEngine != NULL)
	{
		delete m_pScriptEngine;
		m_pScriptEngine = NULL;
	}

	if (m_pDocument != NULL)
		m_pDocument->m_pIDataSession->BindObjectsToTagGroup(NULL, 0, NULL);
}

void CFormDoc::DeleteContents() 
{
	Cleanup();

	CDevObjDoc::DeleteContents();

	// 2006.8.8 加入，解决控件对象不能正确释放的问题
	POSITION pos = GetStartPosition();
	while (pos != NULL)
	{
		CDocItem* pItem = GetNextItem(pos);
		ASSERT(pItem != NULL);
		delete pItem;
	}
	m_docItemList.RemoveAll();
}

////////////////////////////////////////////
//功 能：这里实现关闭整个画面
////////////////////////////////////////////////
void CFormDoc::OnFileClose()
{
	m_pDocument->OnCloseDocument();
}

BOOL CFormDoc::GetDispatchIID(IID* piid)
{
	ASSERT(piid != NULL);
	if (IsMainForm())
		*piid = __uuidof(IUserForm);
	else
		*piid = __uuidof(ISubUserForm);

	return TRUE;
}

void CFormDoc::OnStartUpPositionChanged()
{
}

void CFormDoc::OnBorderStyleChanged()
{
}

void CFormDoc::OnControlBoxChanged()
{
}

void CFormDoc::OnAllowDirectDrawChanged()
{
}

void CFormDoc::OnScanIntervalChanged()
{
	CRunView* pView = (CRunView *)GetRunView();
	if (pView)
		pView->ReSetTimer(TRUE, FALSE, TRUE);
}

void CFormDoc::OnSecurityAreaChanged()
{
}

void CFormDoc::OnVisibleLayerChanged()
{
	UpdateAllViews(NULL);
}

void CFormDoc::ReSyncFormPosition()
{
	if (!IsMainForm() && !IsKindOf(RUNTIME_CLASS(CReportDoc)))
	{
		CView* pView = GetRunView();
		if (pView != NULL)
		{
			CRect rcWindow; 
			pView->GetParentFrame()->GetWindowRect(&rcWindow);
			m_position = rcWindow;
		}
	}
}

float CFormDoc::GetHorizontalPosition()
{
	ReSyncFormPosition();

	return m_position.left;
}

void CFormDoc::SetHorizontalPosition(float fNewValue)
{
	ReSyncFormPosition();

	if (!IsMainForm() && !IsKindOf(RUNTIME_CLASS(CReportDoc)))
		m_position.OffsetRect(fNewValue - m_position.left, 0);

	OnPositionChanged();
}

float CFormDoc::GetVerticalPosition()
{
	ReSyncFormPosition();

	return m_position.top;
}

void CFormDoc::SetVerticalPosition(float fNewValue)
{
	ReSyncFormPosition();

	if (!IsMainForm() && !IsKindOf(RUNTIME_CLASS(CReportDoc)))
		m_position.OffsetRect(0, fNewValue - m_position.top);

	OnPositionChanged();
}

float CFormDoc::GetWidth() 
{
	ReSyncFormPosition();
	
	return m_position.Width();
}

void CFormDoc::SetWidth(float fNewValue) 
{
	ReSyncFormPosition();
	m_position.right = m_position.left + fNewValue;
	OnPositionChanged();
}

float CFormDoc::GetHeight() 
{
	ReSyncFormPosition();

	return m_position.Height();
}

void CFormDoc::SetHeight(float fNewValue) 
{
	ReSyncFormPosition();
	m_position.bottom = m_position.top + fNewValue;
	OnPositionChanged();
}

void CFormDoc::OnPositionChanged()
{
	if (!IsMainForm() && !IsKindOf(RUNTIME_CLASS(CReportDoc)))
	{
		CRunView* pView = (CRunView *)GetRunView();
		if (pView != NULL)
			pView->GetParentFrame()->MoveWindow(&m_position.GetRECT());
	}
}

void CFormDoc::Show(const VARIANT FAR& Modal) 
{
	CView* pView = GetRunView();
	if (pView != NULL && pView->IsWindowVisible())
	{
		// 如果已经存在, 则进行激活
		CPopupFrame* pFrame = (CPopupFrame *)pView->GetParentFrame();
		pFrame->SetActiveWindow();
		return;
	}

	m_bModalDone = TRUE;

	//如果前面已经打开（让隐藏），则不需要重新连接脚本引擎
//	if (pView == NULL)
//		ReadyToRun();

	BOOL bModal = FALSE;

	if (Modal.vt != VT_ERROR)
	{
		CComVariant var = Modal;

		HRESULT hr = var.ChangeType(VT_BOOL);
		if (hr != S_OK)
			return;
		bModal = var.boolVal;
	}

	if (bModal)
	{
		CPopupFrame* pFrame = NULL;
		BOOL bOldFrameEnabled = TRUE;

		CView* pView = GetRunView();
		if (pView != NULL)
		{
			pFrame = (CPopupFrame *)GetRunView()->GetParentFrame();
			bOldFrameEnabled = pFrame->IsWindowEnabled();
		}

		// 以后可以考虑子窗口可以指定父窗口
		HWND hWndParent = ::GetActiveWindow();
	
		HWND hWndTop = NULL;
		hWndParent = CWnd::GetSafeOwner_(hWndParent, &hWndTop);
		BOOL bEnableParent = FALSE;
		if (hWndParent != NULL && ::IsWindowEnabled(hWndParent))
		{
			::EnableWindow(hWndParent, FALSE);
			bEnableParent = TRUE;
		}

		// 如果pFrame在已经创建且隐藏的情况下上面的语句有可能把自己给禁止了
		if (pFrame != NULL/* && pFrame->IsWindowEnabled() != bOldFrameEnabled*/)
			pFrame->EnableWindow(TRUE);
	
		ShowFrameWindow(FALSE);

		if (pFrame == NULL)
		{
			ASSERT(GetRunView() != NULL);
			pFrame = (CPopupFrame *)GetRunView()->GetParentFrame();
		}

		pFrame->m_bDisableModal = TRUE;
		m_pDocument->m_pRunCtrl->m_pFrameWnd->BeginModalState();

		MSG msg;
		
		for (m_bModalDone = FALSE; !m_bModalDone; WaitMessage())
		{
			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT || (msg.message == WM_CLOSE_FORM && msg.hwnd == pFrame->m_hWnd))
				{
					m_bModalDone = TRUE;
					PostMessage(msg.hwnd, msg.message, 0, 0);
					break;
				}
				
	//			if (!pFrame->IsDialogMessage(&msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}

		m_pDocument->m_pRunCtrl->m_pFrameWnd->EndModalState();
		pFrame->m_bDisableModal = FALSE;
		
		// hide the window before enabling parent window, etc.
		if (pFrame != NULL)
		{
			pFrame->SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|
				SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
		}

		if (bEnableParent)
			::EnableWindow(hWndParent, TRUE);
		if (hWndParent != NULL && ::GetActiveWindow() == pFrame->m_hWnd)
			::SetActiveWindow(hWndParent);
		
		if (hWndTop != NULL)
			::EnableWindow(hWndTop, TRUE);

		Hide();
	}
	else
	{
		ShowFrameWindow(FALSE);
	}
}

const IID IID_IDrawMenu = {0xC6148AC6,0x831C,0x4C57,{0x94,0x26,0xA6,0x79,0x4B,0x6B,0xA2,0xAB}};

CFrameWnd* CFormDoc::CreateNewFrame()
{
	CFrameWnd* pFrame = NULL;
	if (IsMainForm())
	{
		CRunCtrl* pRunCtrl = m_pDocument->m_pRunCtrl;
		ASSERT(pRunCtrl != NULL);
		
		pFrame = pRunCtrl->CreateFrame(this);
	}
	else
	{
		CCreateContext context;
		context.m_pCurrentDoc = this;
		context.m_pNewViewClass = RUNTIME_CLASS(CRunView);
		
		pFrame = (CFrameWnd*)new CPopupFrame;
		if (pFrame == NULL)
		{
			return NULL;
		}
		ASSERT_KINDOF(CFrameWnd, pFrame);
		
		if (!pFrame->LoadFrame(IDR_CXRUNTYPE,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
			GetMainForm()->GetRunView()->GetParent(), &context))
		{
			return NULL;
		}
	

//#ifdef _DEBUG
//		static HMENU m_hMenu;
//		HINSTANCE hInst = AfxFindResourceHandle(
//			MAKEINTRESOURCE(IDR_PRINT), RT_MENU);
//		m_hMenu = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_PRINT));
//		BOOL b  = ::SetMenu(GetMainForm()->GetRunView()->GetParent()->m_hWnd, m_hMenu);
//		DWORD dw = GetLastError();
//#endif
		
	}
	
	return pFrame;
}

void CFormDoc::ShowFrameWindow(BOOL bInitial, BOOL bPopup)
{
	CFrameWnd* pNewFrame = NULL;
	if (GetRunView() == NULL)
	{
		pNewFrame = CreateNewFrame();
		ASSERT(pNewFrame != NULL);

		bInitial = TRUE;
		pNewFrame->InitialUpdateFrame(this, TRUE);
	}
	else
	{
		CFrameWnd* pFrame = (CFrameWnd *)GetRunView()->GetParent();
		if (bInitial)
			pFrame->InitialUpdateFrame(this, TRUE);
		else
			pFrame->ActivateFrame();
	}

	CRunView* pView = (CRunView *)GetRunView();

	//激活对象
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CDevObj* pObj = m_objects.GetNext(pos);
		pObj->Active(pView);
	}
	
	if (m_pDocument->m_pRunCtrl->AmbientUserMode())
	{
		if (!m_strSecurityArea.IsEmpty())
			pView->m_bEnabled = theApp.VerifySecurityArea(m_strSecurityArea);
		
		pView->m_bLockDraw = TRUE;

		if (pView->m_bEnabled)
		{
			if (bInitial)
			{
				pView->m_pEventObject = GetScriptEngine()->m_pEventObj;
				FireEvent(CXEVENID_INITIALIZE, EVENT_PARAM(VTS_NONE));
			}

			pView->Refresh(TRUE);
			pView->ReSetTimer(TRUE, TRUE, TRUE);
		}

		pView->Invalidate();
		pView->m_bLockDraw = FALSE;
	}
}	

void CFormDoc::Hide() 
{
	if (GetRunView() != NULL)
	{
		if (!m_bModalDone)
		{
			m_bModalDone = TRUE;
			return;
		}

		// 设置主窗口为焦点
		CFormDoc* pFormDoc = GetMainForm();
		if (pFormDoc != NULL)
			pFormDoc->GetRunView()->SetFocus();
		
		CRunView* pView = (CRunView *)GetRunView();

		pView->GetParent()->ShowWindow(SW_HIDE);
		pView->ReSetTimer(TRUE, TRUE, FALSE);
	}
}

// 因为是脚本调用，所以不能直接删除，而是采用发送消息的方式延时删除
void CFormDoc::Close() 
{
	CRunView* pView = (CRunView *)GetRunView();
	if (pView != NULL && GetScriptEngine() != NULL)
		GetScriptEngine()->ActiveEventObject(pView);

	FireEvent(CXEVENID_QUERYCLOSE, EVENT_PARAM(VTS_NONE));
	if (pView != NULL)
	{
		if (pView->IsCancelBubble())
			return;

		CPopupFrame* pFrame = (CPopupFrame*)GetRunView()->GetParentFrame();
		pFrame->PostMessage(WM_CLOSE_FORM, 0, 0L);
	}
	
	m_pFormSubDoc->m_pChildDoc = NULL;
}

void CFormDoc::OnCloseForm()
{
	CRunView* pView = (CRunView *)GetRunView();
	pView->m_bNoClosePicture = TRUE;

	m_pFormSubDoc->m_pChildDoc = NULL;
	OnCloseDocument();
}

LPUNKNOWN CFormDoc::GetHitObject(long lXPos, long lYPos)
{
	LPUNKNOWN lpUnknown = NULL;

	CRunView* pView = (CRunView *)GetRunView();
	if (pView != NULL)
	{
		CPoint point(lXPos, lYPos);
		CDevObj* pObjHit = pView->HitTestObjects(point);
		
		if (pObjHit != NULL)
			pObjHit->GetObjectUnknown()->QueryInterface(IID_IUnknown, (void**)&lpUnknown);
	}

	return lpUnknown;
}

LPUNKNOWN CFormDoc::GetObjectByName(LPCTSTR lpszName)
{
	USES_CHINESEVAR_CONVERSION;
	LPUNKNOWN lpUnknown = NULL;

	// 在Hash表存在的情况下使用Hash表加快访问速度
	if (m_ppObjects != NULL)
	{
		void *pTemp;
		CString strName = VARC2A(lpszName);
		strName.MakeUpper();
		if (m_mapNameToDispid.Lookup(strName, pTemp))
		{
			lpUnknown = m_ppObjects[(int)pTemp & 0x7fff];
			lpUnknown->AddRef();
		}
	}
	else
	{
		POSITION posObj = m_objects.GetHeadPosition();
		while (posObj != NULL)
		{
			CDevObj* pObj = m_objects.GetNext(posObj);
			if (pObj->GetDisplayName().CompareNoCase(lpszName) == 0)
			{
				pObj->GetObjectUnknown()->QueryInterface(IID_IUnknown, (void**)&lpUnknown);
				break;
			}
		}
	}
	
	return lpUnknown;
}

LPDISPATCH CFormDoc::GetObjectCollection()
{
	CObjectCollection* pObjCol = new CObjectCollection;

	POSITION posObj = m_objects.GetHeadPosition();
	while (posObj != NULL)
	{
		CDevObj* pObj = m_objects.GetNext(posObj);

		IDispatchPtr pDispatch = NULL;
		pObj->GetObjectUnknown()->QueryInterface(IID_IDispatch, (void**)&pDispatch);
		
		CComVariant var = (IDispatch *)pDispatch;
		pObjCol->m_arrObjects.Add(var);
	}

	return (IDispatch *)pObjCol->GetInterface(&IID_IDispatch);
}

OLE_HANDLE CFormDoc::GetWindowHandle() 
{
	CRunView* pView = (CRunView *)GetRunView();
	if (pView != NULL)
		return (OLE_HANDLE)pView->GetSafeHwnd();

	return NULL;
}

void CFormDoc::ForceRefresh()
{
	CRunView* pView = (CRunView *)GetRunView();
	if (pView != NULL)
		pView->Refresh(TRUE);
}

void CFormDoc::BindDataLinksToTagGroup(LPCTSTR lpszGroup)
{
	USES_CONVERSION;

	IUnknown** ppObjects = new IUnknown*[m_objects.GetCount()];
	int nCount = 0;
	
	POSITION posObj = m_objects.GetHeadPosition();
	while (posObj != NULL)
	{
		CDevObj* pObj = m_objects.GetNext(posObj);
		if (pObj->GetClassName().Compare(_T("DataLink")) == 0)
			ppObjects[nCount++] = pObj->GetObjectUnknown();
	}

	m_pDocument->m_pIDataSession->BindObjectsToTagGroup(ppObjects, nCount, T2CW(lpszGroup));

	delete [] ppObjects;
}

//////////////////////////////////////////////////////////////////////////////////////
//功能：从存贮装载动态属性
//////////////////////////////////////////////////////////////////////////////////////
HRESULT CFormDoc::LoadDynamicProperty(IStream *pStm)
{
	HRESULT hr;

	if(!pStm)
	{
		return E_POINTER;
	}

	int nSize;
	pStm->Read(&nSize, sizeof(nSize), NULL);
	for(int nIndex = 0; nIndex < nSize; nIndex++)
	{
		DISPID dispid;
		pStm->Read(&dispid, sizeof(DISPID), NULL);
		IUnknownPtr pObject;
		hr = OleLoadFromStream(pStm, IID_IUnknown, (void**)&pObject);
		AddDynamic(dispid, pObject);
		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
//功能：往存贮保存动态属性
//////////////////////////////////////////////////////////////////////////////////////
HRESULT CFormDoc::SaveDynamicProperty(IStream *pStm)
{
	HRESULT hr;

	if(!pStm)
	{
		return E_POINTER;
	}

	int nSize = m_arLinkInfo.GetSize();;
	pStm->Write(&nSize, sizeof(nSize), NULL);
	for(int nIndex = 0; nIndex < nSize; nIndex++)
	{
		pStm->Write(&m_arLinkInfo[nIndex].dispid, sizeof(DISPID), NULL);
		ICxDynamicObject* pObject = m_arLinkInfo[nIndex].pDynamicObject;
		if (pObject != NULL)
		{
			IPersistStreamPtr spStream;			
			hr = pObject->QueryInterface(IID_IPersistStream, (void**)&spStream);
			if (FAILED(hr))
				return hr;
			if (spStream != NULL)
			{	
				hr = OleSaveToStream(spStream, pStm);
				if (FAILED(hr))
					return hr;
			}
		}
	}

	return S_OK;
}

void CFormDoc::FakeParseDynamic()
{
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		m_arLinkInfo[i].pDynamicObject->FakeParseConnection();
	}
}

void CFormDoc::ParseDynamic()
{
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		m_arLinkInfo[i].pDynamicObject->ParseConnection();
	}
}

void CFormDoc::SerializeDynamicProperty(CArchive& ar)
{
	CArchiveStream stm(&ar);
	LPSTREAM pStream = &stm;

	if (ar.IsStoring())
	{
		SaveDynamicProperty(pStream);
	}
	else
	{
		LoadDynamicProperty(pStream);
	}
}

HRESULT CFormDoc::AddDynamic(DISPID dispid, IUnknown *pUnknown)
{
	VARIANT_BOOL bHasConnected;
	HasDynamic(dispid, &bHasConnected);
	if (bHasConnected == VARIANT_TRUE)
	{
		RemoveDynamic(dispid);
	}

	LINKINFO linkinfo;
	memset(&linkinfo, 0, sizeof(LINKINFO));
	linkinfo.dispid = dispid;
	//在此增加引用
	ICxDynamicObject* pDynamicObject = NULL;
	pUnknown->QueryInterface(IID_ICxDynamicObject, (void **)&pDynamicObject);
	if (pDynamicObject == NULL)
		return S_FALSE;

	linkinfo.pDynamicObject = pDynamicObject;
	m_arLinkInfo.Add(linkinfo);

	return S_OK;
}

HRESULT CFormDoc::HasDynamic(DISPID dispid, VARIANT_BOOL *pConnected)
{
	*pConnected = VARIANT_FALSE;
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		if (m_arLinkInfo[i].dispid == dispid)
		{
			*pConnected = VARIANT_TRUE;
			break;
		}
	}

	return S_OK;
}

HRESULT CFormDoc::GetDynamic(DISPID dispid, IUnknown **ppUnknown)
{
	*ppUnknown = NULL;
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		if (m_arLinkInfo[i].dispid == dispid)
		{
			m_arLinkInfo[i].pDynamicObject->QueryInterface(IID_IUnknown, (void **)ppUnknown);
			break;
		}
	}

	return S_OK;
}

HRESULT CFormDoc::RemoveDynamic(DISPID dispid)
{
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		if (m_arLinkInfo[i].dispid == dispid)
		{
			m_arLinkInfo[i].pDynamicObject->Release();
			m_arLinkInfo.RemoveAt(i);
			break;
		}
	}

	return S_OK;
}

VARIANT_BOOL CFormDoc::IsConnected(LPCTSTR pszPropName)
{
	USES_CONVERSION;

	VARIANT_BOOL bHasConnection = FALSE;
	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
    if (SUCCEEDED(((IDispatch *)(&m_xDispatch))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		HasDynamic(dispid, &bHasConnection);
	}
	else
	{
		ASSERT(FALSE);
	}

	return bHasConnection;
}

void CFormDoc::ConnectObject(LPCTSTR pszPropName, LPUNKNOWN punkObject)
{
	USES_CONVERSION;

	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
    if (SUCCEEDED(((IDispatch *)(&m_xDispatch))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		AddDynamic(dispid, punkObject);
	}
	else
	{
		ASSERT(FALSE);
	}
}

void CFormDoc::ConnectDirect(LPCTSTR pszPropName, LPCTSTR pszDataSource)
{
}

LPUNKNOWN CFormDoc::GetConnectObject(LPCTSTR pszPropName)
{
	USES_CONVERSION;

	LPUNKNOWN punkObject = NULL;

	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
    if (SUCCEEDED(((IDispatch *)(&m_xDispatch))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		
		GetDynamic(dispid, &punkObject);
	}
	else
	{
		ASSERT(FALSE);
	}

	return punkObject;
}

void CFormDoc::Disconnect(LPCTSTR pszPropName)
{
	USES_CONVERSION;

	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
    if (SUCCEEDED(((IDispatch *)(&m_xDispatch))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		RemoveDynamic(dispid);
	}
	else
	{
		ASSERT(FALSE);
	}
}

int CFormDoc::GetConnectState(DISPID dispid)
{
	int nState = 0;
	VARIANT_BOOL bHasConnection;
	HasDynamic(dispid, &bHasConnection);
	if (bHasConnection == VARIANT_TRUE)
		nState |= 0x2;

	return nState;
}

void CFormDoc::OnBackColorChanged() 
{
	UpdateAllViews(NULL);
}

void CFormDoc::OnForeColorChanged() 
{
	UpdateAllViews(NULL);
}

void CFormDoc::OnCaptionChanged() 
{
	if (GetRunView() != NULL)
	{
		GetRunView()->GetParent()->SetWindowText(m_strCaption);
		GetRunView()->GetParent()->SendMessage(WM_NCPAINT);
	}
}

void CFormDoc::OnMousePointerChanged()
{
}

void CFormDoc::OnDescriptionChanged()
{
}

void CFormDoc::OnBrushImageFileChanged()
{
	using namespace Gdiplus;

	if (m_pImageBackGround)
	{
		delete m_pImageBackGround;
		m_pImageBackGround = NULL;
	}
	
	if (m_strBrushImageFile != "")
	{
		BSTR bstr = m_strBrushImageFile.AllocSysString();
		m_pImageBackGround = Image::FromFile(bstr);
		SysFreeString(bstr);
	}
	
	UpdateAllViews(NULL);
}

void CFormDoc::OnTagGroupChanged()
{
	if (m_pTagProvider != NULL)
	{
		m_pTagProvider->Release(); 
		m_pTagProvider = NULL;
	}

	CFormDoc* pFormDoc = GetTopFormDoc();
	pFormDoc->m_pDocument->UpdateDataSession();

	CRunView* pView = (CRunView *)GetRunView();
	if (pView != NULL)
		pView->Refresh(TRUE);
	UpdateAllViews(NULL);
}

BSTR CFormDoc::GetAssocTagGroup()
{
	return GetAssocTagGroupEx(FALSE).AllocSysString();	
}

IDispatch* CFormDoc::GetTagProvider()
{
	if (m_pTagProvider == NULL)
	{
		ICxDataSession* pIDataSession = GetTopFormDoc()->m_pDocument->m_pIDataSession;
		if (pIDataSession != NULL)
		{
			BSTR bstr = GetAssocTagGroupEx(TRUE).AllocSysString();	
			pIDataSession->GetTagGroup(bstr, &m_pTagProvider);
			SysFreeString(bstr);
		}
	}

	return m_pTagProvider;
}

LPFONTDISP CFormDoc::GetFont()
{
	return m_pFontHolder->GetFontDispatch();
}

void CFormDoc::SetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) ||
		AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	m_pFontHolder->InitializeFont(NULL, pFontDisp);
}

BSTR CFormDoc::GetName()
{
	return GetDisplayName().AllocSysString();
}

inline CString CFormDoc::GetDisplayName()
{
	if (m_pFormObj != NULL)
		return m_pFormObj->m_strSubformMoudleName;
	return m_pFormSubDoc->m_strName;
}

BOOL CFormDoc::PutDisplayName(LPCSTR lpszNewValue, BOOL bValidName)
{
//	CString strName = lpszNewValue;
	//主窗口名称不能改变
//	if (IsMainForm() && strName != _T("MainForm"))
//		return FALSE;

//	if (IsValidObjectName(strName) && m_pDocument->IsUniqueSubDocumentName(strName))
//	{
		m_pFormSubDoc->m_strName = lpszNewValue;
		return TRUE;
//	}

//	return FALSE;
}

BSTR CFormDoc::GetMenu()
{
	return m_strMenu.AllocSysString();
}

void CFormDoc::SetMenu(LPCTSTR lNewValue)
{
	m_strMenu = lNewValue;
}

CDevObj* CFormDoc::GetMenuObject()
{
	if (m_strMenu.IsEmpty())
		return NULL;

	POSITION posObj = m_objects.GetHeadPosition();
	while (posObj != NULL)
	{
		CDevObj* pObj = m_objects.GetNext(posObj);
		if (pObj->IsKindOf(RUNTIME_CLASS(CDrawObj)) && pObj->GetClassName() == _T("Menu"))
		{
			if (pObj->GetDisplayName().CollateNoCase(m_strMenu) == 0)
				return pObj;
		}
	}
	
	return NULL;
}

///////////////////////////////////////////////////////////////////
//功 能：返回CRunView
/////////////////////////////////////////////////////////////////////
CView* CFormDoc::GetRunView() const
{
	if (m_pFormObj != NULL)
		return m_pFormObj->GetRunView();

	POSITION posView;
	CView* pView;

	posView = GetFirstViewPosition();
	if (posView == NULL)
	   return NULL;

	ASSERT(posView != NULL);

	while (posView != NULL)
	{
	  pView = GetNextView(posView);
	  ASSERT(pView != NULL);
	  if (pView->IsKindOf(RUNTIME_CLASS(CRunView)))
	  {
		 return /*STATIC_DOWNCAST(CRunView, pView)*/pView;
	  }
	}

	ASSERT(FALSE);
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//【函  数】CFormDoc* GetTopFormDoc()
//【参  数】无
//【功  能】因为子模块可以嵌套，此函数用于返回顶层模块文档
//【返回值】CFormDoc*
//【说  明】不影响全局变量?
/////////////////////////////////////////////////////////////////////////////
CFormDoc* CFormDoc::GetTopFormDoc()
{
	CFormDoc* pFormDoc = this;
	while (pFormDoc->m_pFormObj != NULL)
	{
		CFormObj* pFormObj = pFormDoc->m_pFormObj;
		pFormDoc = (CFormDoc*)pFormObj->m_pDocument;
	}

	return pFormDoc;
}

///////////////////////////////////////////////////////////////////
//功 能：返回脚本引擎
/////////////////////////////////////////////////////////////////////
CFormScrEng* CFormDoc::GetScriptEngine()
{
	return GetTopFormDoc()->m_pScriptEngine;
}

CString CFormDoc::GetDefaultServer()
{
	CFormDoc* pFormDoc = GetTopFormDoc();
	return pFormDoc->m_pDocument->m_pRunCtrl->m_server;
}

void CFormDoc::ReadFromStorage(IStorage* pStorage)
{
	//临时替换
	IStorage* lpOrginRootRtg = m_lpRootStg;
	m_lpRootStg = pStorage;
	LoadFromStorage();
	m_lpRootStg = lpOrginRootRtg;
}

////////////////////////////////////////////
//功 能：从父文档中读入
//说 明：此函数与COleDocument::LoadFromStorage相似
////////////////////////////////////////////////
void CFormDoc::ReadFromParentDocument()
{
	if(m_lpRootStg == NULL)
		m_lpRootStg = m_pDocument->OpenSubDocumentStorage(m_pFormSubDoc);

	ASSERT(m_lpRootStg != NULL);

	// open Contents stream
	COleStreamFile file;
	CFileException fe;
	if (!file.OpenStream(m_lpRootStg, _T("Contents"),
			CFile::modeRead|CFile::shareExclusive, &fe) &&
		!file.CreateStream(m_lpRootStg, _T("Contents"),
			CFile::modeRead|CFile::shareExclusive|CFile::modeCreate, &fe))
	{
		if (fe.m_cause == CFileException::fileNotFound)
			AfxThrowArchiveException(CArchiveException::badSchema);
		else
			AfxThrowFileException(fe.m_cause, fe.m_lOsError);
	}

	// load it with CArchive (loads from Contents stream)
	CArchive loadArchive(&file, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;

	TRY
	{
		if (file.GetLength() != 0)
			Serialize(loadArchive);     // load main contents
		loadArchive.Close();
		
		file.Close();
	}
	CATCH_ALL(e)
	{
		file.Abort();   // will not throw an exception
		DeleteContents();   // removed failed contents
		loadArchive.Abort();

		if (e->IsKindOf(RUNTIME_CLASS(COleException)))
			e->ReportError(MB_ICONEXCLAMATION);

		THROW_LAST();
	}
	END_CATCH_ALL
}

//////////////////////////////////////////////////////////////////////
//功能：根据DISPID查找环境属性
//////////////////////////////////////////////////////////////////////
CAmbientProperty* CFormDoc::FindAmbientProperty(DISPID dispid)
{
	int iProp;

	for (iProp = 0; iProp < m_apAmbientProperties.GetSize(); iProp++)
	{
		if (m_apAmbientProperties[iProp]->GetID() == dispid)
		{
			return m_apAmbientProperties[iProp];
		}
	}

	// We didn't find the property.
	return NULL;
}

//////////////////////////////////////////////////////////////////////
//功能：根据名字查找环境属性
//////////////////////////////////////////////////////////////////////
CAmbientProperty* CFormDoc::FindAmbientProperty( LPCOLESTR pszName )
{
	USES_CONVERSION;
	int iProp;
	LPCTSTR pszNameT;

	pszNameT = OLE2CT(pszName);
	for (iProp = 0; iProp < m_apAmbientProperties.GetSize(); iProp++)
	{
		if (m_apAmbientProperties[iProp]->GetName() == pszNameT)
		{
			return m_apAmbientProperties[iProp];
		}
	}

	// We didn't find the property.
	return NULL;
}

/////////////////////////////////////////////////////////////
//功 能：重载此函数使所有活动项目处于非激活状态
/////////////////////////////////////////////////////////////
void CFormDoc::PreCloseFrame(CFrameWnd* pFrameArg)
{
	POSITION posItem;
	CCtrlItem* pItem;

	CRunView* pView = (CRunView *)GetRunView();
	pView->ReSetTimer(TRUE, TRUE, FALSE);

	// 如果在事件过程中弹出MsgBox，则出错
	if (m_pScriptEngine != NULL)
		FireEvent(CXEVENID_TERMINATE, EVENT_PARAM(VTS_NONE));

	posItem = GetStartPosition();
	while (posItem != NULL)
	{
		pItem = (CCtrlItem *)GetNextItem(posItem);
		if (pItem != NULL)
		{
			if (pItem->IsInPlaceActive())
			{
				pItem->Deactivate();
				//如果不加入下句，部分控件无法完全释放，如msform的框架等
				pItem->Close(OLECLOSE_NOSAVE);
			}
		}
	}

	POSITION pos = m_listSubformModule.GetHeadPosition();
	while (pos != NULL)
	{
		CFormObj* pFormObj = (CFormObj *)m_listSubformModule.GetNext(pos);
		pFormObj->m_pFormDoc->PreCloseFrame(pFrameArg);	
	}

	// 下列语句不知道起何作用
//	if (IsMainForm())
//		m_pDocument->m_pRunCtrl->m_pFrameWnd = NULL;
		
	CDevObjDoc::PreCloseFrame(pFrameArg);
}

void CFormDoc::UpdateCtrlItemsPosition()
{
	POSITION posItem;
	CCtrlItem* pItem;

	CRunView* pView = (CRunView *)GetRunView();
	
	posItem = GetStartPosition();
	while (posItem != NULL)
	{
		pItem = (CCtrlItem *)GetNextItem(posItem);
		if (pItem != NULL)
		{
			pItem->m_pCtrlObj->SetExtent(pView);
			if (pItem->IsInPlaceActive())
				pItem->SetItemRects();
		}
	}
}

//////////////////////////////////////////////////////////////////
//功能:判断是否为主窗口
//////////////////////////////////////////////////////////////////
BOOL CFormDoc::IsMainForm()
{
	return m_pFormSubDoc->IsFirstSubDoc();
}

CFormDoc* CFormDoc::GetMainForm()
{
	return m_pDocument->m_pMainForm;
}

void CFormDoc::UpdateAllAssocViews(CView* pSender, LPARAM lHint, CObject* pHint)
	// walk through all views
{
	CFormDoc* pFormDoc = this;
	while (pFormDoc->m_pFormObj != NULL)
	{
		CFormObj* pFormObj = pFormDoc->m_pFormObj;
		pFormDoc = (CFormDoc*)pFormObj->m_pDocument;
		((COleDocument *)pFormDoc)->UpdateAllViews(pSender, HINT_UPDATE_DRAWOBJ, pFormObj);
	}
}

//获取关联标签
CString CFormDoc::GetAssocTagGroupEx(BOOL bIncludeThis)
{
	CString strTagGroup;

	CFormDoc* pFormDoc = NULL;
	if (bIncludeThis)
		pFormDoc = this;
	else if (m_pFormObj)
		pFormDoc = (CFormDoc*)m_pFormObj->m_pDocument;
	while (pFormDoc != NULL)
	{
		if (!strTagGroup.IsEmpty())
			strTagGroup.Insert(0, _T('.'));
		strTagGroup.Insert(0, pFormDoc->m_strTagGroup);
		//直到是顶层标签或者没有上级模块止
		if (strTagGroup.Left(4).CompareNoCase(_T("Tags")) == 0 || pFormDoc->m_pFormObj == NULL)
			break;

		pFormDoc = (CFormDoc*)pFormDoc->m_pFormObj->m_pDocument;
	}

	return strTagGroup;
}

BOOL CFormDoc::IsAssocTagGroupValid(LPCSTR lpszTagGroup)
{
	CString strTagGroup(lpszTagGroup);
	if (strTagGroup.IsEmpty())
		return TRUE;
	
	if (strTagGroup.Left(4).CompareNoCase(_T("Tags")) == 0)
	{
		TCHAR ch = strTagGroup.GetAt(4);
		if (ch == _T('.') || ch == _T('\0'))
			return TRUE;
	}
	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//功 能：绘制画面文档
//////////////////////////////////////////////////////////////////////////
void CFormDoc::DrawDoc(CDC *pDC)
{
	POSITION posObj;
	CDevObj* pObj;

//	int n = 0;
	posObj = m_objects.GetHeadPosition();
	while (posObj != NULL)
	{
		pObj = m_objects.GetNext(posObj);
//		ASSERT(pObj->m_nLayer >= n);
//		pObj->m_nLayer = n;
		if ((m_ulVisibleLayer & (1 << pObj->m_nLayer)) != 0)
			pObj->Draw(pDC);
	}
}

void CFormDoc::PrintDoc(CDC *pDC)
{
	POSITION posObj;
	CDevObj* pObj;
	
	//	int n = 0;
	posObj = m_objects.GetHeadPosition();
	while (posObj != NULL)
	{
		pObj = m_objects.GetNext(posObj);
		//		ASSERT(pObj->m_nLayer >= n);
		//		pObj->m_nLayer = n;
		if ((m_ulVisibleLayer & (1 << pObj->m_nLayer)) != 0)
			pObj->Draw(pDC);
	}
}

LPOLEITEMCONTAINER CFormDoc::GetContainer()
{
	return (LPOLEITEMCONTAINER)&m_xOleContainer;
}

// 求动态对象表达式的值，使数据会话中定购的标签更新
void CFormDoc::UpdateDataSession()
{
	int nCount = m_arLinkInfo.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		m_arLinkInfo[i].pDynamicObject->FakeParseConnection();
	}

	POSITION pos = m_dynamicobjects.GetHeadPosition();
	while (pos != NULL)
	{
		CDevObj* pObj = m_dynamicobjects.GetNext(pos);
		pObj->FakeParseDynamic();
	}
}

HRESULT CFormDoc::GetItemDispatch(IUnknown** ppDispatch)
{
	USES_CONVERSION;

	if (m_bParsedScript == FALSE)
	{
		m_bParsedScript = TRUE;

		CString strItemName = GetDisplayName();

		CString strScript = m_strScript;
		CString strNew;
		strNew.Format(_T("Sub %s_"), strItemName);
		if (!IsKindOf(RUNTIME_CLASS(CReportDoc)))
			strScript.Replace(_T("Sub UserForm_"), strNew);
		else
			strScript.Replace(_T("Sub Report_"), strNew);

		GetScriptEngine()->m_pActiveScriptParse->ParseScriptText(T2COLE(strScript), T2COLE(strItemName), NULL,
			NULL, m_pFormObj == NULL ? -1 : m_pFormObj->m_dwSubformMoudleNumber + 1, 0, SCRIPTTEXT_ISVISIBLE, NULL, NULL);
	}

	BuildTypeInfo();
	*ppDispatch = m_pFormObj == NULL ? GetIDispatch(TRUE) : m_pFormObj->GetIDispatch(TRUE);

	return S_OK;
}

void CFormDoc::ReadyToRun()
{
	// 创建脚本引擎并初始化
	if (m_pScriptEngine == NULL)
	{
		m_pScriptEngine = new CFormScrEng(this);
		m_bParsedScript = FALSE;
	}
	m_pScriptEngine->Initialize();

	m_pDocument->m_pIDataSession->SetReadTagMode(ReadTagMode_Add);
	int nCount = m_arLinkInfo.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		m_arLinkInfo[i].pDynamicObject->InitRun(&m_xScriptEngineSite, GetIDispatch(FALSE), m_arLinkInfo[i].dispid);
	}
	
	ASSERT(m_dynamicobjects.GetCount() == 0);
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CDevObj* pObj = m_objects.GetNext(pos);
		pObj->InitRun();
	}

	m_pDocument->m_pIDataSession->CommitTags(TRUE);
	m_pDocument->m_pIDataSession->SetReadTagMode(ReadTagMode_Normal);
}

void CFormDoc::OnDynamicChange(CDevObj* pObj)
{
	POSITION posObj = m_dynamicobjects.Find(pObj);
	if (posObj)
		m_dynamicobjects.RemoveAt(posObj);

	pObj->InitRun();
}

void CFormDoc::OnFinalRelease()
{
	return;
}

///////////////////////////////////////////////////////////////////////////////
//

CFormDoc::CFormSubDoc::CFormSubDoc()
{
}

CFormDoc* CFormDoc::CFormSubDoc::CreateFormDoc()
{
	CFormDoc* pFormDoc = new CFormDoc(m_pParentDoc);
	
	pFormDoc->m_pFormSubDoc = this;
	if (!pFormDoc->OnNewDocument())
	{
		delete pFormDoc;
		return NULL;
	}
	
	m_pChildDoc = pFormDoc;
	
	return pFormDoc;
}

CDocument* CFormDoc::CFormSubDoc::LoadChildDocument()
{
	CFormDoc* pFormDoc = NULL;

	if (m_pChildDoc != NULL)
	{
		pFormDoc = (CFormDoc *)m_pChildDoc;
		pFormDoc->m_pFormSubDoc = this;

		CRunView* pView = (CRunView *)pFormDoc->GetRunView();
		ASSERT_VALID(pView);
		CFrameWnd* pFrame = pView->GetParentFrame();
		ASSERT_VALID(pFrame);

// 		pView->Reset();

		pFormDoc->PreCloseFrame(pFrame);

// 		ASSERT(pView->m_pObjectWithFocus == NULL);
 		pView->m_pObjectWithFocus = NULL;
		pView->m_pObjectTrackMouseLeave = NULL;
// 		ASSERT(pView->m_pObjectHighlight == NULL);
// 		AfxMessageBox("aa");
//  	pView->m_pObjectHighlight = NULL;

		pFormDoc->DeleteContents();

	// 	ASSERT(pView->m_pObjectHighlight == NULL);
		pView->Reset();

		ASSERT(pFormDoc->m_lpRootStg == NULL);
 		if (pFormDoc->m_lpRootStg != NULL)
		{
			pFormDoc->m_lpRootStg->Release();
			pFormDoc->m_lpRootStg = NULL;
		}
	}
	else
	{
		pFormDoc = CreateFormDoc();
		ASSERT(pFormDoc != NULL);
	}

	pFormDoc->ReadFromParentDocument();

	if (pFormDoc->m_lpRootStg != NULL)
	{
		pFormDoc->m_lpRootStg->Release();
		pFormDoc->m_lpRootStg = NULL;
	}

//	TRY
//	{
//		pFormDoc->ReadFromParentDocument();
//	}
//	CATCH_ALL(e)
//	{
//		pFormDoc->OnCloseDocument();
//		// 		delete pFormDoc;
//		pFormDoc = NULL;
//		
//		THROW_LAST();
//	}
//	END_CATCH_ALL
	
	return pFormDoc;
}

void CFormDoc::CFormSubDoc::Serialize(CArchive& ar)
{
	ASSERT(ar.IsLoading());

	m_pParentDoc = (CDevDoc *)ar.m_pDocument;
	ar >> m_dwDocumentNumber;
	ar >> m_strName;

	int nSubformModule;
	ar >> nSubformModule;
}

void CFormDoc::CFormSubDoc::UpdateDataSession()
{
	if (m_pChildDoc != NULL)
	{
		((CFormDoc *)m_pChildDoc)->UpdateDataSession();
	}
}
