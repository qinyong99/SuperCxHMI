// ReportSection.cpp : implementation file
//

#include "stdafx.h"
#include "DevInc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportSection

IMPLEMENT_SERIAL(CReportSection, CCmdTarget, 0)

CReportSection::CReportSection()
{
	EnableAutomation();
	
	m_fHeight = 90;
	m_bKeepTogether = FALSE;
	m_enumForcePageBreak = PageBreakNone;
	m_bVisible = TRUE;
	
	AfxOleLockApp();
}

CReportSection::CReportSection(CReportDoc* pDoc, LPCTSTR lpszName, LPCTSTR lpszCaption, float fHeight)
{
	EnableAutomation();
	
	m_pDocument = pDoc;

	m_strName = lpszName;
	m_strCaption = lpszCaption;
	m_fHeight = fHeight;
	m_bKeepTogether = FALSE;
	m_enumForcePageBreak = PageBreakNone;
	m_bVisible = TRUE;

	AfxOleLockApp();
}

CReportSection::~CReportSection()
{
	AfxOleUnlockApp();
}

BEGIN_DISPATCH_MAP(CReportSection, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CReportSection)
	DISP_PROPERTY_EX_ID(CReportSection, "Name", CXPROPID_Name, GetName, SetName, VT_BSTR)
	DISP_FUNCTION_ID(CReportSection, "IsConnected", CXMETHID_IsConnected, IsConnected, VT_I2, VTS_BSTR)
	DISP_FUNCTION_ID(CReportSection, "ConnectObject", CXMETHID_ConnectObject, ConnectObject, VT_EMPTY, VTS_BSTR VTS_UNKNOWN)
	DISP_FUNCTION_ID(CReportSection, "ConnectDirect", CXMETHID_ConnectDirect, ConnectDirect, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CReportSection, "GetConnectObject", CXMETHID_GetConnectObject, GetConnectObject, VT_UNKNOWN, VTS_BSTR)
	DISP_FUNCTION_ID(CReportSection, "Disconnect", CXMETHID_Disconnect, Disconnect, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CReportSection, "GetConnectState", CXMETHID_GetConnectState, GetConnectState, VT_I4, VTS_I4)

	DISP_PROPERTY_EX_ID(CReportSection, "Height", CXPROPID_Height, GetHeight, SetHeight, VT_R4)
	DISP_PROPERTY_NOTIFY_ID(CReportSection, "Visible", CXPROPID_Visible, m_bVisible, OnVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CReportSection, "ForcePageBreak", CXPROPID_ForcePageBreak, m_enumForcePageBreak, OnForcePageBreakChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CReportSection, "KeepTogether", CXPROPID_KeepTogether, m_bKeepTogether, OnKeepTogetherChanged, VT_BOOL)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

const extern IID LIBID_CXHMIOBJLib;
IMPLEMENT_OLETYPELIB(CReportSection, LIBID_CXHMIOBJLib, 1, 0)

// {F0A5FD97-FE32-4091-A43B-488A8EB29FBC}
static const IID IID_IReportSection =
{ 0xF0A5FD97, 0xFE32, 0x4091, { 0xA4, 0x3B, 0x48, 0x8A, 0x8E, 0xB2, 0x9F, 0xBC } };

BEGIN_INTERFACE_MAP(CReportSection, CCmdTarget)
	INTERFACE_PART(CReportSection, IID_IReportSection, Dispatch)
	INTERFACE_PART(CReportSection, IID_ICategorizeProperties, CategorizeProperties)
	INTERFACE_PART(CReportSection, IID_IProvideClassInfo, ProvideClassInfo)
	INTERFACE_PART(CReportSection, IID_IProvideClassInfo2, ProvideClassInfo)
END_INTERFACE_MAP()

////////////////////////////////////////////////////////////////
//XProviderClassInfo

STDMETHODIMP_(ULONG) CReportSection::XProvideClassInfo::AddRef()
{
	METHOD_PROLOGUE_EX_(CReportSection, ProvideClassInfo)
		
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CReportSection::XProvideClassInfo::Release()
{
	METHOD_PROLOGUE_EX_(CReportSection, ProvideClassInfo)

	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CReportSection::XProvideClassInfo::QueryInterface(
		REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CReportSection, ProvideClassInfo)

	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CReportSection::XProvideClassInfo::GetClassInfo(
		LPTYPEINFO* ppTypeInfo)
{
	METHOD_PROLOGUE_EX(CReportSection, ProvideClassInfo)
		
	return theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(ReportSection), ppTypeInfo);
}

STDMETHODIMP CReportSection::XProvideClassInfo::GetGUID(DWORD dwGuidKind,
		GUID* pGUID)
{
	METHOD_PROLOGUE_EX_(CReportSection, ProvideClassInfo)
		
	*pGUID = GUID_NULL;
	return E_INVALIDARG;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_( ULONG ) CReportSection::XCategorizeProperties::AddRef()
{
	METHOD_PROLOGUE(CReportSection, CategorizeProperties)
		
	TRACE("XCategorizeProperties::AddRef().\n");
	
	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CReportSection::XCategorizeProperties::Release()
{
	METHOD_PROLOGUE(CReportSection, CategorizeProperties)
		
	TRACE("XCategorizeProperties::Release().\n");
	
	return pThis->ExternalRelease();
}

STDMETHODIMP CReportSection::XCategorizeProperties::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CReportSection, CategorizeProperties)
		
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CReportSection::XCategorizeProperties::MapPropertyToCategory(DISPID dispid, PROPCAT* ppropcat)
{
	METHOD_PROLOGUE(CReportSection, CategorizeProperties)

	switch (dispid)
	{
		case CXPROPID_Height:
			*ppropcat = -4;
			break;
		case CXPROPID_Visible:
			*ppropcat = -5;
			break;
		default:
			*ppropcat = -2;
			break;
	}
		
	return S_OK;
}

STDMETHODIMP CReportSection::XCategorizeProperties::GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName)
{
	METHOD_PROLOGUE(CReportSection, CategorizeProperties)

	switch (propcat)
	{
	case -2:
		*pbstrName = ::SysAllocString(OLESTR("杂项"));
		break;
	case -5:
		*pbstrName = ::SysAllocString(OLESTR("外观"));
		break;
	case -4:
		*pbstrName = ::SysAllocString(OLESTR("位置"));
		break;
	default:
		return S_FALSE;
	}	
		
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CReportSection message handlers

void CReportSection::Draw(CDC* pDC, LPRECT pRect)
{
	EmbossedTextOut(pDC, pRect->left + 20, pRect->top + 4, m_strCaption, -1, 
		GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNHILIGHT), 1, 1, pRect);
}

VOID CReportSection::EmbossedTextOut(CDC* pDC, int x, int y, LPCTSTR lpsz, UINT cb, 
		COLORREF crText, COLORREF crShadow, int cx, int cy, LPCRECT pClipRect) 
{     
	COLORREF crOld;     
	UINT uMode; 
	SIZE sizeText;     
	RECT rcText;      
	/* If text length is -1, use lstrlen to get the length     
	** of the text.     
	*/     
	if (cb == -1)         
		cb = lstrlen(lpsz);   
	
	/* If the shadow or text color is -1, use the     
	** system color for that one.     
	*/     
	if (crShadow == (COLORREF)-1)         
		crShadow = GetSysColor(COLOR_BTNSHADOW);     
	if (crText == (COLORREF)-1)         
		crText = GetSysColor(COLOR_BTNTEXT);
	
	/* setup the DC, saving off the old values     
	*/     
	uMode = pDC->SetBkMode(TRANSPARENT/*OPAQUE*/);     
	crOld = pDC->SetTextColor(crShadow); 
	
	/* Draw the text at the desired offset using the     
	** shadow color, then again at the normal position     
	** using the text color.  This will the text an 'Embossed'     
	** or 'drop shadowed' look depending on what shadow color    
	** and offset are used.     */     
	sizeText = pDC->GetTextExtent(lpsz, cb);     
	rcText.left   = x;    
	rcText.right  = x + cx + sizeText.cx;      
	rcText.top    = y;    
	rcText.bottom = y + cy + sizeText.cy; 
	CRect rect;
	rect.IntersectRect(&rcText, pClipRect);
	pDC->ExtTextOut(x + cx, y + cy, ETO_CLIPPED, &rect, lpsz, cb, NULL);     
//	pDC->SetBkMode(TRANSPARENT);     
	pDC->SetTextColor(crText);     
	pDC->ExtTextOut(x, y, ETO_CLIPPED, &rect, lpsz, cb, NULL); 
	
	/* restore the DC     
	*/     
	pDC->SetTextColor(crOld);     
	pDC->SetBkMode(uMode); 
}  

void CReportSection::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);
	
	DWORD dwVer;

	if (ar.IsStoring())
	{
		ASSERT_VALID(m_pDocument);  //文档不能为空
		
		dwVer = _SUPERCX_VER;
		ar << dwVer;
		
		ar << m_strName;
		ar << m_strCaption;
		ar << m_fHeight;
		int nTemp = m_enumForcePageBreak;
		ar << nTemp;
		ar << m_bKeepTogether;
		ar << m_bVisible;
	}
	else
	{
		ASSERT(m_pDocument != NULL);  //文档不能为空
		
		ar >> dwVer;
		
		ar >> m_strName;
		ar >> m_strCaption;
		ar >> m_fHeight;
		int nTemp;
		ar >> nTemp;
		m_enumForcePageBreak = (enumForcePageBreak)nTemp;
		ar >> m_bKeepTogether;
		ar >> m_bVisible;
	}

	//序列化动态属性
	SerializeDynamicProperty(ar);
}
//////////////////////////////////////////////////////////////////////////////////////
//功能：从存贮装载动态属性
//////////////////////////////////////////////////////////////////////////////////////
HRESULT CReportSection::LoadDynamicProperty(IStream *pStm)
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
HRESULT CReportSection::SaveDynamicProperty(IStream *pStm)
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

void CReportSection::SerializeDynamicProperty(CArchive& ar)
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

HRESULT CReportSection::AddDynamic(DISPID dispid, IUnknown *pUnknown)
{
	VARIANT_BOOL bHasConnected;
	HasDynamic(dispid, &bHasConnected);
	if (bHasConnected == VARIANT_TRUE)
	{
		RemoveDynamic(dispid);
	}

		if (pUnknown == NULL)
		return S_OK;

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

HRESULT CReportSection::HasDynamic(DISPID dispid, VARIANT_BOOL *pConnected)
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

HRESULT CReportSection::GetDynamic(DISPID dispid, IUnknown **ppUnknown)
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

HRESULT CReportSection::RemoveDynamic(DISPID dispid)
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

VARIANT_BOOL CReportSection::IsConnected(LPCTSTR pszPropName)
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

void CReportSection::ConnectObject(LPCTSTR pszPropName, LPUNKNOWN punkObject)
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

void CReportSection::ConnectDirect(LPCTSTR pszPropName, LPCTSTR pszDataSource)
{
}

LPUNKNOWN CReportSection::GetConnectObject(LPCTSTR pszPropName)
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

void CReportSection::Disconnect(LPCTSTR pszPropName)
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

int CReportSection::GetConnectState(DISPID dispid)
{
	int nState = 0;
	VARIANT_BOOL bHasConnection;
	HasDynamic(dispid, &bHasConnection);
	if (bHasConnection == VARIANT_TRUE)
		nState |= 0x2;

	return nState;
}

void CReportSection::OnVisibleChanged()
{
	m_pDocument->ReCalcReportHeight(TRUE);
	m_pDocument->UpdateAllViews(NULL);
}

void CReportSection::OnForcePageBreakChanged()
{
}

void CReportSection::OnKeepTogetherChanged()
{
}

float CReportSection::GetHeight()
{
	return m_fHeight;
}

void CReportSection::SetHeight(float fNewValue)
{
	ASSERT(m_pDocument != NULL);
	
	m_fHeight = fNewValue;
	m_pDocument->ReCalcReportHeight(TRUE);

	m_pDocument->UpdateAllViews(NULL);
}

BSTR CReportSection::GetName()
{
	return m_strName.AllocSysString();
}

void CReportSection::SetName(LPCTSTR lpszNewName)
{
	ASSERT(m_pDocument != NULL);

	if (m_strName.CompareNoCase(lpszNewName) != 0)
	{
		if (m_pDocument->IsValidSectionName(lpszNewName))
		{
			m_strName = lpszNewName;
			theApp.NotifySelectChanged();
		}
	}
}

LPUNKNOWN CReportSection::GetObjectUnknown()
{
	return GetInterface(&IID_IUnknown);
}

BOOL CReportSection::GetDispatchIID(IID* piid)
{
	ASSERT( piid != NULL );
	
	*piid = __uuidof(IReportSection);
	
	return TRUE;
}

