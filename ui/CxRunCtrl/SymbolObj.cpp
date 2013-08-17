// SymbolObj.cpp: implementation of the CSymbolObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RunInc.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


////////////////////////////////////////////////////////////
//功能:保存和读取扩展属性值
////////////////////////////////////////////////////////////
void CSymbolProp::SerializeExData(CArchive& ar , _variant_t& data)
{
	CArchiveStream stm(&ar);
	LPSTREAM pStream = &stm;
	CComVariant theData;

	if (ar.IsStoring())
	{
		theData = data;
		theData.WriteToStream(pStream);
	}
	else
	{
		theData.ReadFromStream(pStream);
		data = theData;
	}
}

////////////////////////////////////////////////////////////////
//功能:保存高级扩展属性对象
////////////////////////////////////////////////////////////////
void CSymbolProp::SerializeExtendInterface(CArchive& ar, ICxDynamicObjectPtr& pIn, ICxDynamicObjectPtr& pOut)
{
	CArchiveStream stm(&ar);
	LPSTREAM pStream = &stm;
	HRESULT hr;

	if (ar.IsStoring())
	{
		IPersistStreamPtr spStream;			
		hr = pIn->QueryInterface(IID_IPersistStream, (void**)&spStream);
		if (FAILED(hr))
			AfxThrowOleException(hr);
		if (spStream != NULL)
		{	
			hr = OleSaveToStream(spStream, pStream);
			if (FAILED(hr))
				AfxThrowOleException(hr);
		}
	}
	else
	{
		IUnknownPtr pObject;
		hr = OleLoadFromStream(pStream, IID_IUnknown, (void**)&pObject);
		if (FAILED(hr))
			AfxThrowOleException(hr);
		pOut = pObject;
	}
}

void CSymbolProp::Serialize(CArchive &ar)
{
	int i;
	int nCount;
	BOOL bHaveDynamic;

	SerializeExData(ar, m_value);
	
	if (ar.IsStoring())
	{
		ar << m_dispid;
		ar << m_strName;
		ar << m_strDescription;
		ar << m_strValidScript;
		int nTmp = (int)m_proptype;
		ar << nTmp;
		nCount = m_arrObjNum.GetSize();
		ar << nCount;

		for(i = 0; i < nCount; i++)
		{
			ar << m_arrObjNum[i].m_nSubIndex;
			ar << m_arrObjNum[i].m_strPropName;
			if(m_arrObjNum[i].m_pDynamicObject == NULL)
				bHaveDynamic = FALSE;
			else
				bHaveDynamic = TRUE;
			ar << bHaveDynamic;

			if(bHaveDynamic)
				SerializeExtendInterface(ar, m_arrObjNum[i].m_pDynamicObject, m_arrObjNum[i].m_pDynamicObject/*not used*/);
		}
	}
	else
	{
		ar >> m_dispid;
		ar >> m_strName;
		ar >> m_strDescription;
		ar >> m_strValidScript;
		int nTmp;
		ar >> nTmp;
		m_proptype = (enumPropType)nTmp;
		ar >> nCount;
		m_arrObjNum.SetSize(nCount);

		for(i = 0; i < nCount; i++)
		{
			ar >> m_arrObjNum[i].m_nSubIndex;
			ar >> m_arrObjNum[i].m_strPropName;
			ar >> bHaveDynamic;

			if(bHaveDynamic)
				SerializeExtendInterface(ar, m_arrObjNum[i].m_pDynamicObject/*not used*/, m_arrObjNum[i].m_pDynamicObject);
		}
	}
}

HRESULT CSymbolScrEng::AddItemsName()
{
	HRESULT hResult = m_pActiveScript->AddNamedItem(L"symbol", SCRIPTITEM_ISVISIBLE/*|SCRIPTITEM_GLOBALMEMBERS*/);
	if (FAILED(hResult))
		return hResult;
	
	int nCount = m_pObject->m_arrSubObj.GetSize();
	CDevObj* pObj;
	BSTR bstrName;
	CString strName;
	for (int i = 0; i < nCount; i++)
	{
		pObj = m_pObject->m_arrSubObj[i];
		strName.Format("object%03d", i);
		bstrName = strName.AllocSysString();
		hResult = m_pActiveScript->AddNamedItem(bstrName, SCRIPTITEM_ISVISIBLE);
		SysFreeString(bstrName);
		if (FAILED(hResult))
			return hResult;
	}
	
	return S_OK;
}

HRESULT CSymbolScrEng::GetItemDispatch(LPCOLESTR pszName, IUnknown** ppDispatch)
{
	ASSERT(pszName != NULL );
	ASSERT(ppDispatch != NULL );

	*ppDispatch = NULL;

	if (wcscmp(pszName, L"symbol") == 0)
	{
		m_pObject->GetObjectUnknown()->QueryInterface(IID_IDispatch, (void **)ppDispatch);
		return S_OK;
	}
	else 
	{
		int n;
		if (swscanf(pszName, L"object%03d", &n))
		{
			m_pObject->m_arrSubObj[n]->GetObjectUnknown()->QueryInterface(IID_IDispatch, (void **)ppDispatch);
			return S_OK;

		}
	}

	return TYPE_E_ELEMENTNOTFOUND;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CSymbolObj, CGroupObj, 0)

CSymbolObj::CSymbolObj(CDevObjDoc* pContainerDoc) :
	CGroupObj(pContainerDoc),
	m_pScriptEngine(NULL)
{
	m_strClassName = "Symbol";

	m_bParsingDynamic = FALSE;
}

CSymbolObj::~CSymbolObj()
{
}

//////////////////////////////////////////////////////////////////////
//XDispatchExtend members
STDMETHODIMP_( ULONG ) CSymbolObj::XDispatchExtend::AddRef()
{
	METHOD_PROLOGUE(CSymbolObj, DispatchExtend)

    return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CSymbolObj::XDispatchExtend::Release()
{
	METHOD_PROLOGUE(CSymbolObj, DispatchExtend)
	return ((IDispatch *)(&pThis->m_xDispatch))->Release();
}

STDMETHODIMP CSymbolObj::XDispatchExtend::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CSymbolObj, DispatchExtend)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CSymbolObj::XDispatchExtend::GetIDsOfNames(
	REFIID iid, LPOLESTR* ppszNames, UINT nNames, LCID lcid, DISPID* pDispIDs)
{
	USES_CONVERSION;

	METHOD_PROLOGUE(CSymbolObj, DispatchExtend)
	ASSERT_VALID(pThis);

	if (!IsEqualIID( iid, IID_NULL))
	{
		return E_INVALIDARG;
	}
	if (ppszNames == NULL)
	{
		return E_INVALIDARG;
	}
	if (nNames < 1)
	{
		return E_INVALIDARG;
	}
	if (pDispIDs == NULL)
	{
		return E_POINTER;
	}

	int nCount = pThis->m_arrExProp.GetSize();
	if (nNames == 1)
	{
		CString strFunName = ppszNames[0];
		for(int i = 0; i < nCount; i++)
		{
			CSymbolProp& ppt = pThis->m_arrExProp[i];
			if (strFunName.CompareNoCase(ppt.m_strName) == 0)
			{
				pDispIDs[0] = ppt.m_dispid;
				return S_OK;
			}
		}
	}

	return ((IDispatch *)(&pThis->m_xDispatch))->GetIDsOfNames(iid, ppszNames, 
		 nNames, lcid, pDispIDs);
}

STDMETHODIMP CSymbolObj::XDispatchExtend::GetTypeInfo(
	UINT iTypeInfo, LCID lcid, ITypeInfo** ppTypeInfo)
{
	METHOD_PROLOGUE(CSymbolObj, DispatchExtend)
	ASSERT_VALID(pThis);

	//下语句有内存泄露,所以改为后面语句
//	((IDispatch *)(&pThis->m_xDispatch))->GetTypeInfo(iTypeInfo, lcid, ppTypeInfo);

	return pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), __uuidof(GroupObject), ppTypeInfo);

//	return theApp.m_pExtTypeLib->GetTypeInfoOfGuid(__uuidof(IGroupObject), ppTypeInfo);

//	CDevObjDoc* pDoc = pThis->m_pDocument;
//	if (pDoc != NULL && pDoc->IsKindOf(RUNTIME_CLASS(CFormDoc)))
//	{
//		return ((CFormDoc*)pDoc)->GetTypeInfoOfGuid(lcid, __uuidof(IGroupObject), ppTypeInfo);
//	}

//	return E_FAIL;
}

STDMETHODIMP CSymbolObj::XDispatchExtend::GetTypeInfoCount(
   UINT* pnInfoCount )
{
	METHOD_PROLOGUE(CSymbolObj, DispatchExtend)
	ASSERT_VALID(pThis);

	return ((IDispatch *)(&pThis->m_xDispatch))->GetTypeInfoCount(pnInfoCount);
}

STDMETHODIMP CSymbolObj::XDispatchExtend::Invoke(
	DISPID dispidMember, REFIID iid, LCID lcid, WORD wFlags,
	DISPPARAMS* pdpParams, VARIANT* pvarResult, EXCEPINFO* pExceptionInfo,
	UINT* piArgErr)
{
 	METHOD_PROLOGUE(CSymbolObj, DispatchExtend)
	ASSERT_VALID(pThis);

	if ((dispidMember & CXDISPID_EXTPROP) != CXDISPID_EXTPROP)
		return ((IDispatch *)(&pThis->m_xDispatch))->Invoke(dispidMember, iid, lcid, wFlags,
			pdpParams, pvarResult, pExceptionInfo, piArgErr);
	else
	{
		if ((wFlags & DISPATCH_PROPERTYPUT) == DISPATCH_PROPERTYPUT)
			return pThis->PutExtendProperty(dispidMember, pdpParams->rgvarg);
		else if ((wFlags & DISPATCH_PROPERTYGET) == DISPATCH_PROPERTYGET)
			return pThis->GetExtendProperty(dispidMember, pvarResult);
	}

	return DISP_E_MEMBERNOTFOUND;
}

////////////////////////////////////////////////////////////////
STDMETHODIMP_( ULONG ) CSymbolObj::XScriptEngineSite::AddRef()
{
	METHOD_PROLOGUE(CSymbolObj, ScriptEngineSite)

	TRACE("XScriptEngineSite::AddRef().\n");

	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CSymbolObj::XScriptEngineSite::Release()
{
	METHOD_PROLOGUE(CSymbolObj, ScriptEngineSite)

	TRACE("XScriptEngineSite::Release().\n");

	return pThis->ExternalRelease();
}

STDMETHODIMP CSymbolObj::XScriptEngineSite::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CSymbolObj, ScriptEngineSite)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CSymbolObj::XScriptEngineSite::EvaluateExpression(BSTR bstrSource, VARIANT* pvaValue)
{
	METHOD_PROLOGUE(CSymbolObj, ScriptEngineSite)

	return pThis->m_pScriptEngine->Evaluation(bstrSource, pvaValue);	
}

STDMETHODIMP CSymbolObj::XScriptEngineSite::Execute(BSTR bstrStatement)
{
	METHOD_PROLOGUE(CSymbolObj, ScriptEngineSite)

	pThis->m_pScriptEngine->Execute(bstrStatement);	

	return S_OK;
}

STDMETHODIMP CSymbolObj::XScriptEngineSite::PreTreatExpression(BSTR bstrSource, BSTR* pbstrSource)
{
	METHOD_PROLOGUE(CSymbolObj, ScriptEngineSite)

/*	CString strSource = bstrSource;
	char* p = (char *)(LPCTSTR)strSource;
	CString strOut;
	CString strName;
	int iIndex = 0;
	int iLast = 0;
	char szBuf[4];
	int n;
	while ((iIndex = strSource.Find("object", iLast)) != -1)
	{
		strOut += CString(p+iLast, iIndex - iLast);

		iIndex += 6;
		if (isdigit(p[iIndex]) && isdigit(p[iIndex + 1]) && isdigit(p[iIndex + 2]) && !isdigit(p[iIndex + 3]))
		{
			szBuf[3] = 0;
			memcpy(szBuf, p + iIndex, 3);

			n = atoi(szBuf);
			if (n < pThis->m_arrSubObj.GetSize())
			{
				strName = pThis->m_arrSubObj[n]->GetDisplayName();
				strOut += strName;
				iLast = iIndex + 3;
				continue;
			}
		}

		strOut += CString(p+iLast, 6);
		iLast = iIndex;
	}

	strOut += CString(p+iLast, strSource.GetLength() - iLast);

	*pbstrSource = strOut.AllocSysString();
*/
/*	CString strSource = bstrSource;
	CString strName;
	CString strOld;
	int nCount = pThis->m_arrSubObj.GetSize();
	str
	for (int i = 0; i < nCount; i++)
	{
		strName = pThis->m_arrSubObj[i]->GetDisplayName();
		strOld.Format("object%d", i);
		strSource.Replace(strOld, strName);
	}
	*pbstrSource = strSource.AllocSysString();
*/	
	return S_OK;
}

STDMETHODIMP CSymbolObj::XScriptEngineSite::PreTreatExpressionEx(BSTR bstrSource, BSTR* pbstrSource, VARIANT* pvaMin, VARIANT* pvaMax)
{
	METHOD_PROLOGUE(CSymbolObj, ScriptEngineSite)

	return S_OK;
}

BEGIN_DISPATCH_MAP(CSymbolObj, CGroupObj)
	//{{AFX_DISPATCH_MAP(CSymbolObj)
	DISP_FUNCTION_ID(CSymbolObj, "IsConnected", CXMETHID_IsConnected, IsConnected, VT_I2, VTS_BSTR)
	DISP_FUNCTION_ID(CSymbolObj, "ConnectObject", CXMETHID_ConnectObject, ConnectObject, VT_EMPTY, VTS_BSTR VTS_UNKNOWN)
	DISP_FUNCTION_ID(CSymbolObj, "ConnectDirect", CXMETHID_ConnectDirect, ConnectDirect, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CSymbolObj, "GetConnectObject", CXMETHID_GetConnectObject, GetConnectObject, VT_UNKNOWN, VTS_BSTR)
	DISP_FUNCTION_ID(CSymbolObj, "Disconnect", CXMETHID_Disconnect, Disconnect, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CSymbolObj, "GetConnectState", CXMETHID_GetConnectState, GetConnectState, VT_I4, VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CSymbolObj, CGroupObj)
	INTERFACE_PART(CSymbolObj, IID_IDispatch, DispatchExtend)
	INTERFACE_PART(CSymbolObj, IID_ICxScriptEngineSite, ScriptEngineSite)
END_INTERFACE_MAP()

void CSymbolObj::Serialize(CArchive& ar)
{
	CGroupObj::Serialize(ar);

	//序列化扩展属性
	int nCount;
	ar >> nCount;
	m_arrExProp.SetSize(nCount);

	for(int i = 0; i < nCount; i++)
	{
		m_arrExProp[i].Serialize(ar);
	}

	UpdateAllExPropValue();
}

VARIANT_BOOL CSymbolObj::IsConnected(LPCTSTR pszPropName)
{
	USES_CONVERSION;

	VARIANT_BOOL bHasConnection = FALSE;
	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
    if (SUCCEEDED(((IDispatch *)(&m_xDispatchExtend))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
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

void CSymbolObj::ConnectObject(LPCTSTR pszPropName, LPUNKNOWN punkObject)
{
	USES_CONVERSION;

	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
	if(SUCCEEDED(((IDispatch *)(&m_xDispatchExtend))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		AddDynamic(dispid, punkObject);
	}
	else
	{
		ASSERT(FALSE);
	}
}

void CSymbolObj::ConnectDirect(LPCTSTR pszPropName, LPCTSTR pszDataSource)
{
}

LPUNKNOWN CSymbolObj::GetConnectObject(LPCTSTR pszPropName)
{
	USES_CONVERSION;

	LPUNKNOWN punkObject = NULL;

	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
    if (SUCCEEDED(((IDispatch *)(&m_xDispatchExtend))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
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

void CSymbolObj::Disconnect(LPCTSTR pszPropName)
{
	USES_CONVERSION;

	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
    if (SUCCEEDED(((IDispatch *)(&m_xDispatchExtend))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		RemoveDynamic(dispid);
	}
	else
	{
		ASSERT(FALSE);
	}
}

int CSymbolObj::GetConnectState(DISPID dispid)
{
	int nState = 0;
	//扩展属性，水平位置，垂直位置，旋转角度支持鼠标输入
	if (/*HasMouseInputDynamic(dispid)*/ dispid >= CXDISPID_EXTPROP || dispid == CXPROPID_HorPosition || dispid == CXPROPID_VerPosition || dispid == CXPROPID_RotateAngle)
		nState |= 0x1;
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		if (m_arLinkInfo[i].dispid == dispid)
		{
			nState |= 0x2; //已经连接
			if (m_arLinkInfo[i].pDynamicObject->IsEnableMouseInput() == S_OK)
				nState |= 0x4;
			break;
		}
	}

	return nState;
}

BOOL CSymbolObj::HasMouseInputDynamic(DISPID dispid)
{
	ICxDynamicObject* pDynObj;

	int nCount = m_arrExProp.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CSymbolProp& prop = m_arrExProp[i];
		if (prop.m_dispid == dispid || dispid == DISPID_UNKNOWN)
		{
			int nRePropCount = prop.m_arrObjNum.GetSize();
			for (int j = 0; j < nRePropCount; j++)
			{
				CSymbolProp::CRelatedProp& rePpt = prop.m_arrObjNum[j];
				pDynObj = rePpt.m_pDynamicObject;
				if (pDynObj != NULL && pDynObj->IsEnableMouseInput() == S_OK)
					return TRUE;
			}
		}
	}

	return FALSE;
}

HRESULT CSymbolObj::GetExtendProperty(DISPID dwDispID, VARIANT* pVar)
{
	int nCount = m_arrExProp.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CSymbolProp& prop = m_arrExProp[i];
		if (prop.m_dispid == dwDispID)
		{
			VariantCopy(pVar, &prop.m_value);
			return S_OK;
		}
	}

	return DISP_E_MEMBERNOTFOUND;
}

//HRESULT CSymbolObj::PutExtendProperty(DISPID dwDispID, VARIANT* pVar)
//{
//	int nCount = m_arrExProp.GetSize();
//	for (int i = 0; i < nCount; i++)
//	{
//		CSymbolProp& prop = m_arrExProp[i];
//		if (prop.m_dispid == dwDispID)
//		{
//			if (!prop.m_bNeedUpdateValue && VarCmp(pVar, &prop.m_value, LOCALE_USER_DEFAULT, 0) == VARCMP_EQ)
//				return S_OK;
//			
//			prop.m_bNeedUpdateValue = FALSE;
//			
//			DISPID dispID;
//			BSTR bstr;
//			DISPPARAMS dispparams = {NULL, NULL, 1, 1};
//			DISPID dispidPut = DISPID_PROPERTYPUT;
//			dispparams.rgdispidNamedArgs = &dispidPut;
//			ICxDynamicObject* pDynObj;
//			
//			HRESULT hr = DISP_E_TYPEMISMATCH;
//			
//			//保存新值
//			if (IsValidExPropValue(&prop, pVar))
//			{
//				switch (prop.m_proptype)
//				{
//				case PROPTYPE_STRING:
//					hr = VariantChangeType(&prop.m_value, pVar, 0, VT_BSTR);
//					break;
//				case PROPTYPE_NUMBER:
//					hr = VariantChangeType(&prop.m_value, pVar, 0, VT_R4);
//					break;
//				case PROPTYPE_COLOR:
//					hr = VariantChangeType(&prop.m_value, pVar, 0, VT_I4);
//					break;
//				case PROPTYPE_BOOL:
//					hr = VariantChangeType(&prop.m_value, pVar, 0, VT_BOOL);
//					break;
//				}
//				if (FAILED(hr))
//					return hr;
//				
//				pDynObj = GetMouseInputynamic(dwDispID);
//				if (pDynObj != NULL && pDynObj->IsInitRun() == S_OK)
//					pDynObj->InvertParseConnection();
//			}
//			
//			nCount = prop.m_arrObjNum.GetSize();
//			for (i = 0; i < nCount; i++)
//			{
//				CSymbolProp::CRelatedProp& rePpt = prop.m_arrObjNum[i];
//				if (rePpt.m_nSubIndex >= 0)
//				{
//					IDispatchPtr pDisp = m_arrSubObj[rePpt.m_nSubIndex]->GetObjectUnknown();
//					bstr = rePpt.m_strPropName.AllocSysString();
//					hr = pDisp->GetIDsOfNames(IID_NULL, &bstr, 1, LOCALE_SYSTEM_DEFAULT, &dispID);
//					::SysFreeString(bstr);
//					if (FAILED(hr))
//						return hr;
//					
//					pDynObj = rePpt.m_pDynamicObject;
//					if (pDynObj != NULL)
//					{	
//						//高级属性
//						if (pDynObj->IsInitRun() != S_OK)
//						{
//							if (m_pScriptEngine == NULL)
//								InitScriptEngine();
//							pDynObj->InitRun(&m_xScriptEngineSite, pDisp, dispID);
//						}
//						
//						hr = pDynObj->ParseConnection();
//						
//					}
//					else
//					{
//						dispparams.rgvarg = &prop.m_value;
//						hr =  pDisp->Invoke(dispID, IID_NULL,
//							LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT,
//							&dispparams, NULL, NULL, NULL);
//					}
//					
//					if (FAILED(hr))
//						return hr;
//				}
//			}
//			
//			return S_OK;
//		}
//	}
//	
//	return DISP_E_MEMBERNOTFOUND;

// 此实现与开发环境有所不同，多了在新值与旧值相等时的判断处理
HRESULT CSymbolObj::PutExtendProperty(DISPID dwDispID, VARIANT* pVar)
{
	int nCount = m_arrExProp.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CSymbolProp& prop = m_arrExProp[i];
		if (prop.m_dispid == dwDispID)
		{
			BOOL bNeedUpdateValue = prop.m_bNeedUpdateValue;
			prop.m_bNeedUpdateValue = FALSE;

			// 比较新值与旧值是否相等
			if (!bNeedUpdateValue && VarCmp(pVar, &prop.m_value, LOCALE_USER_DEFAULT, 0) == VARCMP_EQ)
				return S_OK;

			ICxDynamicObject* pDynObj;

			HRESULT hr = DISP_E_TYPEMISMATCH;

			// 如果是合法新值
			if (IsValidExPropValue(&prop, pVar))
			{
				CComVariant var = *pVar;

				// 转换为所定义类型
				switch (prop.m_proptype)
				{
				case PROPTYPE_STRING:
					hr = var.ChangeType(VT_BSTR);
					break;
				case PROPTYPE_NUMBER:
					hr = var.ChangeType(VT_R4);
					break;
				case PROPTYPE_COLOR:
					hr = var.ChangeType(VT_I4);
					break;
				case PROPTYPE_BOOL:
					hr = var.ChangeType(VT_BOOL);
					break;
				default:
					ASSERT(FALSE);
				}
				if (FAILED(hr))
					return hr;

				// 再次比较新值与旧值是否相等
				if (!bNeedUpdateValue && VarCmp(&var, &prop.m_value, LOCALE_USER_DEFAULT, 0) == VARCMP_EQ)
					return S_OK;
			
				// 赋新值
				VariantCopy(&prop.m_value, &var);
			
				// 反向处理
				pDynObj = GetMouseInputynamic(dwDispID);
// 				if (/*!m_bParsingDynamic && */pDynObj != NULL && pDynObj->IsInitRun() == S_OK)
// 					pDynObj->InvertParseConnection();
				// 2008.5.6为避免在网络状况不好的情况下“标签组”演示画面能畅通运行，由上面语句改为下面语句
				if (!m_bParsingDynamic && pDynObj != NULL && pDynObj->IsInitRun() == S_OK)
					pDynObj->InvertParseConnection();

				// 刷新关联的子对象
				BSTR bstr;
				DISPID dispID;
				DISPPARAMS dispparams = {NULL, NULL, 1, 1};
				DISPID dispidPut = DISPID_PROPERTYPUT;
				dispparams.rgdispidNamedArgs = &dispidPut;
				dispparams.rgvarg = &prop.m_value;

				nCount = prop.m_arrObjNum.GetSize();
				for (i = 0; i < nCount; i++)
				{
					CSymbolProp::CRelatedProp& rePpt = prop.m_arrObjNum[i];
					if (rePpt.m_nSubIndex >= 0)
					{
						IDispatchPtr pDisp = m_arrSubObj[rePpt.m_nSubIndex]->GetObjectUnknown();

						// 得到所关联的子对象属性 dispID
						bstr = rePpt.m_strPropName.AllocSysString();
						hr = pDisp->GetIDsOfNames(IID_NULL, &bstr, 1, LOCALE_SYSTEM_DEFAULT, &dispID);
						::SysFreeString(bstr);
						if (FAILED(hr))
							return hr;

						pDynObj = rePpt.m_pDynamicObject;
						// 高级关联
						if (pDynObj != NULL)
						{	
							if (pDynObj->IsInitRun() != S_OK)
							{
								if (m_pScriptEngine == NULL)
									InitScriptEngine();
								pDynObj->InitRun(&m_xScriptEngineSite, pDisp, dispID);
							}

							hr = pDynObj->ParseConnection();
						
						}
						// 直接关联
						else
						{
							hr =  pDisp->Invoke(dispID, IID_NULL,
								LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT,
								&dispparams, NULL, NULL, NULL);
						}
					
						if (FAILED(hr))
							return hr;
					}
				}
			}

			return S_OK;
		}
	}

	return DISP_E_MEMBERNOTFOUND;
}

BOOL CSymbolObj::IsValidExPropValue(CSymbolProp* pProp, VARIANT* pvaValue)
{
	CString strScript = pProp->m_strValidScript;

	if (strScript == "")
		return TRUE;

	if (m_pScriptEngine == NULL)
	{
		if (!InitScriptEngine())
		{
			AfxMessageBox(_T("符号脚本引擎初始化错误！"));
			return TRUE;
		}
	}

	CComVariant var = *pvaValue;
	var.ChangeType(VT_BSTR);
	CString strValue = var.bstrVal;
	var.Clear();

	CString strOld = _T("symbol.");
	strOld += pProp->m_strName;
	strScript.Replace(strOld, strValue);

	BSTR bstrScript = strScript.AllocSysString();
	m_pScriptEngine->Evaluation(bstrScript, &var);
	SysFreeString(bstrScript);

	var.ChangeType(VT_BOOL);
		return var.boolVal;

	return TRUE;
}

HRESULT CSymbolObj::UpdateAllExPropValue()
{
	int nCount = m_arrExProp.GetSize();
	HRESULT hr;
	for (int i = 0; i < nCount; i++)
	{
		CSymbolProp& prop = m_arrExProp[i];
		hr = PutExtendProperty(prop.m_dispid, &prop.m_value);
		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}

BOOL CSymbolObj::InitScriptEngine()
{
	if (m_pScriptEngine == NULL)
		m_pScriptEngine = new CSymbolScrEng(this);

	return m_pScriptEngine->Initialize() == S_OK;
}

void CSymbolObj::Release()
{
	if (m_pScriptEngine)
		delete m_pScriptEngine;

	CGroupObj::Release();
}

CRect CSymbolObj::GetBoundingRect()
{
	CRect rcBound = CGroupObj::GetBoundingRect();
//	rcBound.UnionRect(&m_position.GetRECT(), &rcBound);

	return rcBound;
}

BOOL CSymbolObj::ProcessMouseInput(CDevObj* pObj, LPCSTR szPropName, BOOL bProcess)
{
	ICxDynamicObject* pDynObj;
		
	if (bProcess == FALSE)
	{
		int nPropCount = m_arrExProp.GetSize();
		for (int i = 0; i < nPropCount; i++)
		{
			CSymbolProp& prop = m_arrExProp[i];
			int nRePropCount = prop.m_arrObjNum.GetSize();
			for (int j = 0; j < nRePropCount; j++)
			{
				CSymbolProp::CRelatedProp& rePpt = prop.m_arrObjNum[j];
				if (pObj == m_arrSubObj[rePpt.m_nSubIndex] && rePpt.m_strPropName == szPropName)
				{
					pDynObj = rePpt.m_pDynamicObject;
					if (pDynObj != NULL && pDynObj->IsEnableMouseInput() == S_OK)
						return TRUE;
				}
			}
		}
	}
	else
	{
		HRESULT hr;
		IDispatchPtr pDisp = pObj->GetObjectUnknown();
		DISPID dispID;
		BSTR bstr;

		int nPropCount = m_arrExProp.GetSize();
		for (int i = 0; i < nPropCount; i++)
		{
			CSymbolProp& prop = m_arrExProp[i];
			int nRePropCount = prop.m_arrObjNum.GetSize();
			for (int j = 0; j < nRePropCount; j++)
			{
				CSymbolProp::CRelatedProp& rePpt = prop.m_arrObjNum[j];
				if (pObj == m_arrSubObj[rePpt.m_nSubIndex] && rePpt.m_strPropName == szPropName)
				{
					pDynObj = rePpt.m_pDynamicObject;
					if (pDynObj != NULL && pDynObj->IsEnableMouseInput() == S_OK)
					{	
						bstr = rePpt.m_strPropName.AllocSysString();
						hr = pDisp->GetIDsOfNames(IID_NULL, &bstr, 1, LOCALE_SYSTEM_DEFAULT, &dispID);
						::SysFreeString(bstr);
						if (FAILED(hr))
							return hr;

						//高级属性
						if (pDynObj->IsInitRun() != S_OK)
						{
							if (m_pScriptEngine == NULL)
								InitScriptEngine();
							pDynObj->InitRun(&m_xScriptEngineSite, pDisp, dispID);
						}

						prop.m_bNeedUpdateValue = TRUE;
						hr = pDynObj->InvertParseConnection();
						if (FAILED(hr))
							return hr;
						break;
					}
				}
			}
		}
	}

	return FALSE;
}

void CSymbolObj::ParseDynamic()
{
	m_bParsingDynamic = TRUE;
	
	CGroupObj::ParseDynamic();

	m_bParsingDynamic = FALSE;
}

BOOL CSymbolObj::GetDispatchIID(IID* piid)
{
	ASSERT( piid != NULL );
	
	*piid = __uuidof(IGroupObject);
	
	return TRUE;
}
