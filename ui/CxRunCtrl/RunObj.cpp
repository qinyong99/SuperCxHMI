// DevItem.cpp : implementation of the CDevObj class
//

#include "stdafx.h"
#include "RunInc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//根据两个点求相对角度
int CalculateAngle(POINT ptOrigin, POINT ptCal)
{
	int dx = ptCal.x - ptOrigin.x;
	int dy = ptOrigin.y - ptCal.y;
	if (dx == 0)
	{
		if (dy < 0)
			return 270;
		return 90;
	}
	return int(atan2(dy, dx) * 57.29578) % 360;
}

float CalculateAngle(POINTF ptOrigin, POINTF ptCal)
{
	float dx = ptCal.x - ptOrigin.x;
	float dy = ptOrigin.y - ptCal.y;
	if (dx == 0)
	{
		if (dy < 0)
			return 270;
		return 90;
	}
	return float(fmod(atan2(dy, dx) * 57.29578, 360));
}

POINTF CalculateRotatePoint(POINTF ptOrgin, POINTF ptCal, float fAngle)
{
	fAngle = -CalculateAngle(ptOrgin, ptCal) + fAngle;
	float RR = (float)sqrt((ptOrgin.x - ptCal.x) * (ptOrgin.x - ptCal.x) +
			(ptOrgin.y - ptCal.y) * (ptOrgin.y - ptCal.y));

	POINTF ptRet;
	ptRet.x =  ptOrgin.x + float(RR * cos(fAngle * 0.0174533));
	ptRet.y =  ptOrgin.y + float(RR * sin(fAngle * 0.0174533));

	return ptRet;
}

/////////////////////////////////////////////////////////////////////////////
// CDevObj implementation

IMPLEMENT_SERIAL(CDevObj, CCmdTarget, 0)

CDevObj::CDevObj(CDevObjDoc* pContainer) 
{
	m_dwVer = 0;
	m_dwSubObjNum = -1;
	m_pDocument = NULL;
	m_lpStorage = NULL;
	m_lpNewStorage = NULL;
	m_bNeedCommit = FALSE;
	m_dwObjectNumber = 0;
	m_dwMiscStatus = 0;

	m_pDC = NULL;

	m_pGroupObj = NULL;

	m_bInitEventInfo = FALSE;

	m_dwEventCookie = 0;
	m_iidEventInterface = IID_NULL;
}

CDevObj::~CDevObj()
{
}

BEGIN_INTERFACE_MAP(CDevObj, CCmdTarget)
	INTERFACE_PART(CDevObj, IID_ICxScriptEngineSite, ScriptEngineSite)
END_INTERFACE_MAP()

IUnknown* CDevObj::GetInterfaceHook(const void* pv)
{
	const IID* piid;

#ifdef _DEBUG
	// The standard MFC implementation of QueryInterface doesn't switch module
	// states, since most objects just use static data to implement QI.  Since
	// we're calling GetDocument(), though, we wind up calling ASSERT_VALID on
	// a bunch of MFC objects, and that requires the correct module state.
	// Because ASSERT_VALID doesn't do anything in release builds, we only need
	// to switch module states in debug builds.
	AFX_MANAGE_STATE( m_pModuleState );
#endif  // _DEBUG

	piid = (const IID*)pv;

	if (*piid == m_iidEventInterface)
		return &m_xEventHandler;

	return NULL;
}

////////////////////////////////////////////////////////////////

STDMETHODIMP_( ULONG ) CDevObj::XScriptEngineSite::AddRef()
{
	METHOD_PROLOGUE(CDevObj, ScriptEngineSite)

	TRACE("XScriptEngineSite::AddRef().\n");

	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CDevObj::XScriptEngineSite::Release()
{
	METHOD_PROLOGUE(CDevObj, ScriptEngineSite)

	TRACE("XScriptEngineSite::Release().\n");

	return pThis->ExternalRelease();
}

STDMETHODIMP CDevObj::XScriptEngineSite::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CDevObj, ScriptEngineSite)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CDevObj::XScriptEngineSite::EvaluateExpression(BSTR bstrSource, VARIANT* pvaValue)
{
	USES_CONVERSION;

	METHOD_PROLOGUE(CDevObj, ScriptEngineSite)

	CFormDoc* pDoc = (CFormDoc *)pThis->m_pDocument;
	CFormScrEng* pEngine = pDoc->GetScriptEngine();
	if (pEngine == NULL)
		return E_FAIL;

	return pEngine->Evaluation(bstrSource, pvaValue, T2OLE(pDoc->GetDisplayName()));	
}

STDMETHODIMP CDevObj::XScriptEngineSite::Execute(BSTR bstrStatement)
{
	USES_CONVERSION;

	METHOD_PROLOGUE(CDevObj, ScriptEngineSite)

	CFormDoc* pDoc = (CFormDoc *)pThis->m_pDocument;
	CFormScrEng* pEngine = pDoc->GetScriptEngine();
	if (pEngine == NULL)
		return E_FAIL;
	
	pEngine->Execute(bstrStatement, T2OLE(pDoc->GetDisplayName()));
	return S_OK;
}

STDMETHODIMP CDevObj::XScriptEngineSite::PreTreatExpression(BSTR bstrSource, BSTR* pbstrSource)
{
	METHOD_PROLOGUE(CDevObj, ScriptEngineSite)

	if (pbstrSource != NULL)
	{
		CString strSouce = bstrSource;
		CString strAssocTagGroup = ((CFormDoc *)pThis->m_pDocument)->GetAssocTagGroupEx(TRUE);
					
		CString str1 = strSouce;
		str1.MakeUpper();
		CString str2;
		int nStart = 0;
		int nFind;
			
		while ((nFind = str1.Find(_T("TAGGROUP."), nStart)) != -1)
		{
			str2 += strSouce.Mid(nStart, nFind - nStart);
			nStart += 9;
			if (nFind > 0 && (!__iscsym(strSouce[nFind - 1]) || strSouce[nFind - 1] == '.'))
			{
				str2 += strSouce.Mid(nFind, 9);
				continue;
			}

			str2 += strAssocTagGroup;
			str2 += '.';
		}
		
		str2 += strSouce.Right(strSouce.GetLength() - nStart);

		if (*pbstrSource != NULL)
			SysFreeString(*pbstrSource);

		*pbstrSource = str2.AllocSysString();
	}
		
	return S_OK;
}

STDMETHODIMP CDevObj::XScriptEngineSite::PreTreatExpressionEx(BSTR bstrSource, BSTR* pbstrSource, VARIANT* pvaMin, VARIANT* pvaMax)
{
	METHOD_PROLOGUE(CDevObj, ScriptEngineSite)

	CFormScrEng* pEngine = pThis->m_pDocument->GetScriptEngine();
	LPCWSTR p = bstrSource;
	if ((p[0] == OLECHAR('T') || p[0] == OLECHAR('t')) &&
		(p[1] == OLECHAR('A') || p[1] == OLECHAR('a')) &&
		(p[2] == OLECHAR('G') || p[2] == OLECHAR('g')) &&
		(p[3] == OLECHAR('G') || p[3] == OLECHAR('g')) &&
		(p[4] == OLECHAR('R') || p[4] == OLECHAR('r')) &&
		(p[5] == OLECHAR('O') || p[5] == OLECHAR('o')) &&
		(p[6] == OLECHAR('U') || p[6] == OLECHAR('u')) &&
		(p[7] == OLECHAR('P') || p[7] == OLECHAR('p')) &&
		p[8] == OLECHAR('.'))
	{
		CString strSource = ((CFormDoc *)pThis->m_pDocument)->GetAssocTagGroupEx(TRUE);
		if (strSource.IsEmpty() == FALSE)
			strSource += _T('.');
		strSource += (p + 9); 
//		if (bstrSource)
//			SysFreeString(bstrSource);
		BSTR bstr = strSource.AllocSysString();
		pEngine->PreTreatExpression(bstr, pbstrSource, pvaMin, pvaMax);
		SysFreeString(bstr);
	}
	else
	{
		pEngine->PreTreatExpression(bstrSource, pbstrSource, pvaMin, pvaMax);
	}
		
	return S_OK;
}

////////////////////////////////////////////////////////////////

STDMETHODIMP_( ULONG ) CDevObj::XEventHandler::AddRef()
{
	METHOD_PROLOGUE(CDevObj, EventHandler)

	TRACE("XEventHandler::AddRef().\n");

	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CDevObj::XEventHandler::Release()
{
	METHOD_PROLOGUE(CDevObj, EventHandler)

	TRACE("XEventHandler::Release().\n");

	return pThis->ExternalRelease();
}

STDMETHODIMP CDevObj::XEventHandler::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CDevObj, EventHandler)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CDevObj::XEventHandler::GetIDsOfNames(REFIID iid, LPOLESTR* ppszNames, UINT nNames, LCID lcid, DISPID* pDispIDs)
{
	METHOD_PROLOGUE(CDevObj, EventHandler)

	return E_NOTIMPL;
}

STDMETHODIMP CDevObj::XEventHandler::GetTypeInfo(UINT iTypeInfo, LCID lcid,	ITypeInfo** ppTypeInfo)
{
	METHOD_PROLOGUE(CDevObj, ScriptEngineSite)

	return E_NOTIMPL;
}

STDMETHODIMP CDevObj::XEventHandler::GetTypeInfoCount(UINT* pnInfoCount)
{
	METHOD_PROLOGUE(CDevObj, EventHandler)

	*pnInfoCount = 0;
	return S_OK;
}

STDMETHODIMP CDevObj::XEventHandler::Invoke(DISPID dispidMember, REFIID iid, LCID lcid,
	WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult, EXCEPINFO* pExceptionInfo, UINT* piArgError)
{
	METHOD_PROLOGUE(CDevObj, EventHandler)

	CFormDoc* pDoc = (CFormDoc *)pThis->m_pDocument;
	CFormScrEng* pEngine = pDoc->GetScriptEngine();
	if (pEngine != NULL)
	{
		CString strEvent;
		if (pThis->GetEventByDispid(dispidMember, strEvent))
		{
			USES_CONVERSION;
			USES_CHINESEVAR_CONVERSION;

			CString strEventSub = VARC2A(pThis->GetDisplayName());
			strEventSub += _T("_");
			strEventSub += strEvent;

			return pEngine->InvokeScriptDispatch(T2COLE(pDoc->GetDisplayName()), T2COLE(strEventSub), iid, lcid,
					wFlags, pdpParams, pvarResult, pExceptionInfo, piArgError);
		}
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////

CString CDevObj::GetClassName()
{
	return m_strClassName;
}

inline void CDevObj::Invalidate(CRunView* pView)
{
	ASSERT_VALID(this);
	
	if (pView == NULL)
		m_pDocument->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, this);
	else 
		pView->OnUpdate(NULL, HINT_UPDATE_DRAWOBJ, this);

}

//对象移动到指定位置（与DrawCli例子不同）
void CDevObj::MoveTo(const CRectF &position, BOOL bUniformScale, CRunView* pView)
{
	ASSERT_VALID(this);

	if (position == m_position)
		return;

	Invalidate(pView);
	m_position = position;
	Invalidate(pView);
}

void CDevObj::RotateAt(CPointF point, float fAngle, CRunView* pView)
{
	CPointF ptCenter = m_position.CenterPoint();
	CPointF ptNew = CalculateRotatePoint(point, ptCenter, fAngle);
	m_position.OffsetRect(ptNew - ptCenter);
}

void CDevObj::ScaleAt(CPointF point, float fHorScale, float fVerScale, CRunView* pView)
{
	m_position.OffsetRect(-point);
	m_position.left *= fHorScale;
	m_position.right *= fHorScale;
	m_position.top *= fVerScale;
	m_position.bottom *= fVerScale;
	m_position.OffsetRect(point);
}

BOOL CDevObj::HitTest(CPoint point, const CRunView* pView)
{
	if (m_bVisible)
		return GetBoundingRect().PtInRect(point);
	//	return  (point.x >= m_position.left && point.x < m_position.right &&
	//			point.y >= m_position.top && point.y < m_position.bottom);

	return FALSE;
}

//得到唯一的对象编号，目的是用于序列化
DWORD CDevObj::GetNewObjectNumber()
{
	ASSERT_VALID(this);

	CDevObjDoc* pDoc = m_pDocument;
	ASSERT_VALID(pDoc);
	DWORD dwNextObjectNumber = pDoc->m_dwNextObjectNumber;

	for (;;)
	{
		//保证对象编号没有被其它对象使用
		CDevObjList* pObjList = pDoc->GetObjects();
		POSITION posObj = pObjList->GetHeadPosition();
		CDevObj* pObj = NULL;
		BOOL bCollision = FALSE;
		while (posObj != NULL)
		{
			pObj = pObjList->GetNext(posObj);
			if (pObj->m_dwObjectNumber == dwNextObjectNumber)
			{
				bCollision = TRUE;
				break;
			}
		}
		if (!bCollision)
			break;  // no item using m_dwNextObjectNumber

		// m_dwNextObjectNumber is in use, bump to next one!
		++dwNextObjectNumber;
	}

	pDoc->m_dwNextObjectNumber = dwNextObjectNumber + 1;
	return dwNextObjectNumber;
}

//依据对象编号产生对象名称
void CDevObj::GetObjectName(LPTSTR lpszObjectName) const
{
	ASSERT_VALID(this);
	ASSERT(lpszObjectName != NULL);

	wsprintf(lpszObjectName, _T("CDevObj %lu"), m_dwObjectNumber);
}

//////////////////////////////////////////////////////////////////////////////////////
//功能：从存贮装载动态属性
//////////////////////////////////////////////////////////////////////////////////////
HRESULT CDevObj::LoadDynamicProperty(IStream *pStm)
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
HRESULT CDevObj::SaveDynamicProperty(IStream *pStm)
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

void CDevObj::OnDynamicChange()
{
	ASSERT_VALID(this);

	((CFormDoc *)m_pDocument)->OnDynamicChange(this);
}

void CDevObj::FakeParseDynamic()
{
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		m_arLinkInfo[i].pDynamicObject->FakeParseConnection();
	}
}

void CDevObj::ParseDynamic()
{
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		m_arLinkInfo[i].pDynamicObject->ParseConnection();
	}
}

void CDevObj::SerializeDynamicProperty(CArchive& ar)
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

////////////////////////////////////////////////////////////////////////
//原IDynamicSet接口函数
HRESULT CDevObj::AddDynamic(DISPID dispid, IUnknown *pUnknown)
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

HRESULT CDevObj::HasDynamic(DISPID dispid, VARIANT_BOOL *pConnected)
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

HRESULT CDevObj::GetDynamic(DISPID dispid, IUnknown **ppUnknown)
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

HRESULT CDevObj::RemoveDynamic(DISPID dispid)
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

void CDevObj::AdviseEnventSink()
{
	if (m_dwEventCookie == 0)
	{
		if (!m_bInitEventInfo)
		{
			if (GetEnventSinkStatus())	
				InitEventInfo();
			m_bInitEventInfo = TRUE;
		}

		if (m_pEventCP == NULL)
		{
			IConnectionPointContainerPtr pConnectionPointContainer;
			GetObjectUnknown()->QueryInterface(IID_IConnectionPointContainer, (void**)&pConnectionPointContainer);
		
			pConnectionPointContainer->FindConnectionPoint(m_iidEventInterface, &m_pEventCP);
		}

		if (m_pEventCP != NULL)
		{
			HRESULT hr = m_pEventCP->Advise(&m_xEventHandler, &m_dwEventCookie);
			ASSERT(hr == S_OK);
		}
	}
}

void CDevObj::Release()
{
	if (m_lpStorage != NULL)
		m_lpStorage->Release();

	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		if (m_arLinkInfo[i].pDynamicObject != NULL)
			m_arLinkInfo[i].pDynamicObject->Release();
	}
		
	if (m_dwEventCookie != 0)
	{
		if (m_pEventCP == NULL)
		{
			IConnectionPointContainerPtr pConnectionPointContainer;
			GetObjectUnknown()->QueryInterface(IID_IConnectionPointContainer, (void**)&pConnectionPointContainer);
		
			pConnectionPointContainer->FindConnectionPoint(m_iidEventInterface, &m_pEventCP);
		}

		if (m_pEventCP != NULL)
		{
			m_pEventCP->Unadvise(m_dwEventCookie);
			m_dwEventCookie = 0;
			m_pEventCP.Release();
		}
	}
}

void CDevObj::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	if (ar.IsStoring())
	{
		ASSERT_VALID(m_pDocument);  //文档不能为空
		ar << m_dwObjectNumber;   // save the object number
		ar << m_dwSubObjNum;
		ar << m_nLayer;
		ar << m_bLocked;
		ar << m_bVisible;
		ar << m_strDisplayName;
		ar << m_strToolTip;
		ar << m_nPrivilege;
		ar << m_dwMiscStatus;
	}
	else
	{
		if (m_pDocument == NULL)
		{
			CDevObjDoc* pContainerDoc = (CDevObjDoc*)ar.m_pDocument;
			ASSERT_VALID(pContainerDoc);
			ASSERT_KINDOF(CDevObjDoc, pContainerDoc);
			m_pDocument = pContainerDoc;
			ASSERT(pContainerDoc == m_pDocument);
		}
		ASSERT(m_pDocument != NULL);  //文档不能为空

		if (m_pDocument->m_dwVer == _SUPERCX_OLDVER)
			m_dwVer = _SUPERCX_OLDVER;
		else
			ar >> m_dwVer;

		ar >> m_dwObjectNumber;
		ar >> m_dwSubObjNum;
		ar >> m_nLayer;
		if (m_dwVer >= _SUPERCX_VER_0203)
			ar >> m_bLocked;
		ar >> m_bVisible;
		ar >> m_strDisplayName;
		ar >> m_strToolTip;
		ar >> m_nPrivilege;
		ar >> m_dwMiscStatus;

		if (m_dwVer != _SUPERCX_OLDVER)
			ar >> m_strDescription;
	}
}

void CDevObj::SaveProperty(CArchive& ar)
{
}

void CDevObj::RestoreProperty(CArchive& ar) 
{
}

void CDevObj::InitRun()
{
	int nCount = m_arLinkInfo.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		m_arLinkInfo[i].pDynamicObject->InitRun(&m_xScriptEngineSite, GetObjectDispatch(), m_arLinkInfo[i].dispid);
	}

	if (nCount > 0)
		((CFormDoc *)m_pDocument)->m_dynamicobjects.AddTail(this);
}

BOOL CDevObj::OnMouseInput(LPCSTR szPropName, BOOL bProcess)
{
	if (m_pGroupObj != NULL && m_pGroupObj->IsKindOf(RUNTIME_CLASS(CSymbolObj)))
		return ((CSymbolObj *)m_pGroupObj)->ProcessMouseInput(this, szPropName, bProcess);

	return FALSE;
}

HRESULT CDevObj::PutProperty(DISPID dwDispID, VARIANT* pVar)
{
	IDispatchPtr pDisp = GetObjectUnknown();
	
	DISPPARAMS dispparams = {NULL, NULL, 1, 1};
	dispparams.rgvarg = pVar;
	DISPID dispidPut = DISPID_PROPERTYPUT;
	dispparams.rgdispidNamedArgs = &dispidPut;
	
	if (pVar->vt == VT_UNKNOWN || pVar->vt == VT_DISPATCH || 
		(pVar->vt & VT_ARRAY) || (pVar->vt & VT_BYREF))
	{
		HRESULT hr = pDisp->Invoke(dwDispID, IID_NULL,
			LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUTREF,
			&dispparams, NULL, NULL, NULL);
		if (SUCCEEDED(hr))
			return hr;
	}
	
	return pDisp->Invoke(dwDispID, IID_NULL,
		LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT,
		&dispparams, NULL, NULL, NULL);
}

void CDevObj::InitEventInfo()
{
	USES_CONVERSION;
	CComPtr<IProvideClassInfo> spProvideClassInfo;
	CComPtr<ITypeInfo> spClassInfo;
	CComPtr<ITypeInfo> spTypeInfo;
	TYPEATTR* pClassTypeAttr = NULL;
	TYPEATTR* pTypeAttr = NULL;

	m_arrEventInfo.RemoveAll();

	// 获取类信息
	HRESULT hr = GetObjectUnknown()->QueryInterface(&spProvideClassInfo);
	if (FAILED(hr))
		goto ONERROR;
	
	hr = spProvideClassInfo->GetClassInfo(&spClassInfo);
	if (FAILED(hr))
		goto ONERROR;
	
	spClassInfo->GetTypeAttr(&pClassTypeAttr);
	ASSERT(pClassTypeAttr->typekind == TKIND_COCLASS);

	//遍历类所实现的接口
	int i;
	for (i = 0; i < pClassTypeAttr->cImplTypes; i++)
	{
		int nImplTypeFlags; 
		spClassInfo->GetImplTypeFlags(i, &nImplTypeFlags);
		if ((nImplTypeFlags & IMPLTYPEFLAG_FSOURCE) == 0)	// 不是事件接口
			continue;
		
		// 获取应用类型信息句柄
		HREFTYPE hRef;
		hr = spClassInfo->GetRefTypeOfImplType(i, &hRef);
		if (FAILED(hr))
			goto ONERROR;

		// 获取类型信息
		hr = spClassInfo->GetRefTypeInfo(hRef, &spTypeInfo);
		if (FAILED(hr))
			goto ONERROR;	

		spTypeInfo->GetTypeAttr(&pTypeAttr);
		m_iidEventInterface = pTypeAttr->guid;

		// 查找Dispatch接口
		if (pTypeAttr->typekind == TKIND_INTERFACE)
		{
			//Get Dispatch Interface's Info;
			hr = spTypeInfo->GetRefTypeOfImplType(-1, &hRef);
			if (FAILED(hr))
				goto ONERROR;	

			CComPtr<ITypeInfo> spTypeInfoTmp;
			hr = spTypeInfo->GetRefTypeInfo(hRef, &spTypeInfoTmp);
			if (FAILED(hr))
				goto ONERROR;

			spTypeInfo->ReleaseTypeAttr(pTypeAttr);
			spTypeInfo = spTypeInfoTmp;
			spTypeInfo->GetTypeAttr(&pTypeAttr);
		}

		ASSERT(pTypeAttr->typekind == TKIND_DISPATCH);

		//遍历Dispatch接口中的所有函数
		for (int nFunction = 0; nFunction < pTypeAttr->cFuncs; nFunction++)
		{
			FUNCDESC* pfd;
			spTypeInfo->GetFuncDesc(nFunction, &pfd);
			
			CComBSTR bstrName;
			spTypeInfo->GetDocumentation(pfd->memid, &bstrName, NULL, NULL, NULL);
			
			EVENTINFO eventinfo;
			eventinfo.dispid = pfd->memid;
			eventinfo.strMethod = bstrName;
			m_arrEventInfo.Add(eventinfo);

			spTypeInfo->ReleaseFuncDesc(pfd);
		}
		
		spTypeInfo->ReleaseTypeAttr(pTypeAttr);
		pTypeAttr = NULL;
		spTypeInfo.Release();
	}

ONERROR:
	if (spClassInfo != NULL && pClassTypeAttr != NULL)
		spClassInfo->ReleaseTypeAttr(pClassTypeAttr);
	if (spTypeInfo != NULL && pClassTypeAttr != NULL)
		spTypeInfo->ReleaseTypeAttr(pTypeAttr);
}

BOOL CDevObj::GetEventByDispid(DISPID dispid, CString& strEvent)
{
	for (int i = 0; i < m_arrEventInfo.GetSize(); i++)
	{
		EVENTINFO& eventinfo = m_arrEventInfo[i];
		if (eventinfo.dispid == dispid)
		{
			strEvent = eventinfo.strMethod;
			return TRUE;
		}
	}

	return FALSE;
}
