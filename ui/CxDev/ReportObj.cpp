// ReportObj.cpp: implementation of the CReportObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CReportObj, CFormObj, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReportObj::CReportObj(CDevObjDoc* pContainerDoc) :
	CFormObj(pContainerDoc)
{
	EnableAutomation();
	
	m_strClassName = "Report";
	
	AfxOleLockApp();
}

CReportObj::~CReportObj()
{
	AfxOleUnlockApp();
}

BEGIN_DISPATCH_MAP(CReportObj, CFormObj)
	//{{AFX_DISPATCH_MAP(CReportObj)
	DISP_PROPERTY_EX_ID(CReportObj, "LeftMargin", CXPROPID_LeftMargin, GetLeftMargin, SetLeftMargin, VT_R4)
	DISP_PROPERTY_EX_ID(CReportObj, "TopMargin", CXPROPID_TopMargin, GetTopMargin, SetTopMargin, VT_R4)
	DISP_PROPERTY_EX_ID(CReportObj, "RightMargin", CXPROPID_RightMargin, GetRightMargin, SetRightMargin, VT_R4)
	DISP_PROPERTY_EX_ID(CReportObj, "BottomMargin", CXPROPID_BottomMargin, GetBottomMargin, SetBottomMargin, VT_R4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

const extern IID LIBID_CXHMIOBJLib;
IMPLEMENT_OLETYPELIB(CReportObj, LIBID_CXHMIOBJLib, 1, 0)

// {6C92D770-8D58-4817-B2AB-96554D98D4BF}
static const IID IID_IReportObject =
{ 0x6C92D770, 0x8D58, 0x4817, { 0xB2, 0xAB, 0x96, 0x55, 0x4D, 0x98, 0xD4, 0xBF } };

BEGIN_INTERFACE_MAP(CReportObj, CFormObj)
	INTERFACE_PART(CReportObj, IID_IReportObject, Dispatch)
	INTERFACE_PART(CReportObj, IID_IProvideClassInfo, ProvideClassInfo)
	INTERFACE_PART(CReportObj, IID_IProvideClassInfo2, ProvideClassInfo)
END_INTERFACE_MAP()

////////////////////////////////////////////////////////////////////////////////////
//XProviderClassInfo

STDMETHODIMP_(ULONG) CReportObj::XProvideClassInfo::AddRef()
{
	METHOD_PROLOGUE_EX_(CReportObj, ProvideClassInfo)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CReportObj::XProvideClassInfo::Release()
{
	METHOD_PROLOGUE_EX_(CReportObj, ProvideClassInfo)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CReportObj::XProvideClassInfo::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CReportObj, ProvideClassInfo)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CReportObj::XProvideClassInfo::GetClassInfo(
	LPTYPEINFO* ppTypeInfo)
{
	METHOD_PROLOGUE_EX(CReportObj, ProvideClassInfo)

	return theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(ReportObject), ppTypeInfo);
}

STDMETHODIMP CReportObj::XProvideClassInfo::GetGUID(DWORD dwGuidKind,
	GUID* pGUID)
{
	METHOD_PROLOGUE_EX_(CReportObj, ProvideClassInfo)

	if (dwGuidKind == GUIDKIND_DEFAULT_SOURCE_DISP_IID)
	{
		*pGUID = __uuidof(IReportObjectEvents);
		return NOERROR;
	}
	else
	{
		*pGUID = GUID_NULL;
		return E_INVALIDARG;
	}
}

//////////////////////////////////////////////////////////////////////
//

BOOL CReportObj::GetDispatchIID(IID* piid)
{
	ASSERT( piid != NULL );
	
	*piid = __uuidof(IReportObject);
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
//功 能：当最后引用被释放，删除对象
//描 述：与CCmdTarget::OnFinalRelease()实现不一样，如调用CCmdTarget::OnFinalRelease()会出错
/////////////////////////////////////////////////////////////////////////
void CReportObj::OnFinalRelease()
{
	// When the last reference for an automation object is released
	//  OnFinalRelease is called.  This implementation deletes the
	//  object.  Add additional cleanup required for your object before
	//  deleting it from memory.
	
	delete this;
}

BOOL CReportObj::CreateFormDoc()
{
	m_pFormDoc = new CReportDoc();
	m_pFormDoc->m_pFormObj = this;
	
	if (!m_pFormDoc->OnNewDocument())
	{
		delete m_pFormDoc;
		m_pFormDoc = NULL;
		return FALSE;
	}

	m_pFormDoc->OnUpateTitle();
	
	return TRUE;
}

void CReportObj::Draw(CDC* pDC)
{
	CReportDoc* pReportDoc = GetReportDocument();
	ASSERT(pReportDoc != NULL);

	CRect client;
	pDC->GetClipBox(client);

	int nSavedDC = pDC->SaveDC();

	CRect rcPos = m_position.GetRECT();

	CBrush backBrush(RGB(255, 255, 255));
	pDC->FillRect(rcPos, &backBrush);

	CRect rcPosDP = rcPos;
	pDC->LPtoDP(&rcPosDP);

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportExt(rcPosDP.Size());

	pDC->SetWindowExt((int)pReportDoc->GetWidth(), (int)pReportDoc->GetHeight());
	
	pDC->DPtoLP(&rcPosDP);
	pDC->SetWindowOrg(-rcPosDP.left, -rcPosDP.top);
//	pDC->SetViewportOrg(0, 0);
//	pDC->IntersectClipRect(0, 0, int(pReportDoc->m_fReportWidth), int(pReportDoc->m_fReportHeight));
//	pDC->OffsetViewportOrg()

	pReportDoc->DrawOutline(pDC);

//	pDC->SetViewportOrg(ptOrg);

	pDC->RestoreDC(nSavedDC);
}

float CReportObj::GetLeftMargin()
{
	return GetReportDocument()->GetLeftMargin();
}

void CReportObj::SetLeftMargin(float fNewValue)
{
	CReportDoc* pReportDoc = GetReportDocument();
	ASSERT(pReportDoc != NULL);

	pReportDoc->SetLeftMargin(fNewValue);
}

float CReportObj::GetTopMargin()
{
	return GetReportDocument()->GetTopMargin();
}

void CReportObj::SetTopMargin(float fNewValue)
{
	CReportDoc* pReportDoc = GetReportDocument();
	ASSERT(pReportDoc != NULL);
	
	pReportDoc->SetTopMargin(fNewValue);
}

float CReportObj::GetRightMargin()
{
	return GetReportDocument()->GetRightMargin();	
}

void CReportObj::SetRightMargin(float fNewValue)
{
	CReportDoc* pReportDoc = GetReportDocument();
	ASSERT(pReportDoc != NULL);
	
	pReportDoc->SetRightMargin(fNewValue);
}

float CReportObj::GetBottomMargin()
{
	return GetReportDocument()->GetBottomMargin();	
}

void CReportObj::SetBottomMargin(float fNewValue)
{
	CReportDoc* pReportDoc = GetReportDocument();
	ASSERT(pReportDoc != NULL);
	
	pReportDoc->SetBottomMargin(fNewValue);
}

HRESULT CReportObj::MapPropertyToCategory(DISPID dispid, PROPCAT* ppropcat)
{
	switch (dispid)
	{
	case CXPROPID_LeftMargin:
	case CXPROPID_TopMargin:
	case CXPROPID_RightMargin:
	case CXPROPID_BottomMargin:
		*ppropcat = -6;
		break;

	default:
		return CFormObj::MapPropertyToCategory(dispid, ppropcat);
	}

	return S_OK;
}

HRESULT CReportObj::GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName)
{
	if (propcat == -6)
	{
		*pbstrName = ::SysAllocString(OLESTR("边距"));
		return S_OK;
	}
	
	return CFormObj::GetCategoryName(propcat, lcid, pbstrName);
}