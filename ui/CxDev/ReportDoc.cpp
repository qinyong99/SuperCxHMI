// ReportDoc.cpp: implementation of the CReportDoc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CReportDoc
//////////////////////////////////////////////////////////////////////


const IID IID_ICxDataConnection = {0x424024A4,0x5846,0x4A64,{0x99,0xDD,0x69,0x58,0x57,0xF1,0xFC,0x5D}};
const IID IID_ICxDataRecordset = {0xBB7E961D,0x0A97,0x48A9,{0xA7,0x64,0xAE,0x4A,0x19,0x24,0xA0,0x4A}};
const CLSID CLSID_CxDataConnection = {0x65F875DF,0x7EC3,0x4DBD,{0x87,0x0E,0x08,0xD4,0x96,0xD4,0x77,0x6C}};
const CLSID CLSID_CxDataRecordset = {0x54A812E0,0x8C18,0x46AF,{0xB5,0x83,0x56,0x83,0xCD,0xB8,0x72,0x9A}};


IMPLEMENT_DYNCREATE(CReportDoc, CFormDoc)

CReportDoc::CReportDoc(CDevDoc* pDoc) :
	CFormDoc(pDoc),
	m_secReportHeader(this, _T("Section1"), _T("报表页眉"), 80),
	m_secPageHeader(this, _T("Section2"), _T("页面页眉"), 30),
	m_secDetail(this, _T("Section3"), _T("主体"), 360),
	m_secPageFooter(this, _T("Section4"), _T("页面页脚"), 30),
	m_secReportFooter(this, _T("Section5"), _T("报表页脚"), 80)
{
	//	m_pSelectSection = &m_secPageFooter;
}

CReportDoc::~CReportDoc()
{
	
}

BEGIN_MESSAGE_MAP(CReportDoc, CFormDoc)
	//{{AFX_MSG_MAP(CReportDoc)
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CReportDoc, CFormDoc)
	//{{AFX_DISPATCH_MAP(CReportDoc)
	DISP_FUNCTION_ID(CReportDoc, "PrintOut", 1, PrintOut, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CReportDoc, "PrintPreview", 2, PrintPreview, VT_EMPTY, VTS_NONE)

	DISP_PROPERTY_EX_ID(CReportDoc, "LeftMargin", CXPROPID_LeftMargin, GetLeftMargin, SetLeftMargin, VT_R4)
	DISP_PROPERTY_EX_ID(CReportDoc, "TopMargin", CXPROPID_TopMargin, GetTopMargin, SetTopMargin, VT_R4)
	DISP_PROPERTY_EX_ID(CReportDoc, "RightMargin", CXPROPID_RightMargin, GetRightMargin, SetRightMargin, VT_R4)
	DISP_PROPERTY_EX_ID(CReportDoc, "BottomMargin", CXPROPID_BottomMargin, GetBottomMargin, SetBottomMargin, VT_R4)
	DISP_PROPERTY_EX_ID(CFormDoc, "DataConnection", CXPROPID_DataConnection, GetDataConnection, SetDataConnection, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CFormDoc, "DataRecordset", CXPROPID_DataRecordset, GetDataRecordset, SetDataRecordset, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CFormDoc, "PageCount", CXPROPID_PageCount, GetPageCount, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CFormDoc, "Page", CXPROPID_Page, GetPage, SetNotSupported, VT_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

const extern IID LIBID_CXHMIOBJLib;
IMPLEMENT_OLETYPELIB(CReportDoc, LIBID_CXHMIOBJLib, 1, 0)

// {06A075DF-2EFC-4012-8DA2-AEFE3AC04967}
static const IID IID_IDataReport =
{ 0x06A075DF, 0x2EFC, 0x4012, { 0x8D, 0xA2, 0xAE, 0xFE, 0x3A, 0xC0, 0x49, 0x67 } };

BEGIN_INTERFACE_MAP(CReportDoc, CFormDoc)
	INTERFACE_PART(CReportDoc, IID_IDataReport, Dispatch)
	INTERFACE_PART(CReportDoc, IID_IProvideClassInfo, ProvideClassInfo)
	INTERFACE_PART(CReportDoc, IID_IProvideClassInfo2, ProvideClassInfo)
END_INTERFACE_MAP()

////////////////////////////////////////////////////////////////////////////////////
//XProviderClassInfo

STDMETHODIMP_(ULONG) CReportDoc::XProvideClassInfo::AddRef()
{
	METHOD_PROLOGUE_EX_(CReportDoc, ProvideClassInfo)
		return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CReportDoc::XProvideClassInfo::Release()
{
	METHOD_PROLOGUE_EX_(CReportDoc, ProvideClassInfo)
		return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CReportDoc::XProvideClassInfo::QueryInterface(
														   REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CReportDoc, ProvideClassInfo)
		return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CReportDoc::XProvideClassInfo::GetClassInfo(
														 LPTYPEINFO* ppTypeInfo)
{
	METHOD_PROLOGUE_EX(CReportDoc, ProvideClassInfo)
		
	return theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(DataReport), ppTypeInfo);
}

STDMETHODIMP CReportDoc::XProvideClassInfo::GetGUID(DWORD dwGuidKind,
													GUID* pGUID)
{
	METHOD_PROLOGUE_EX_(CReportDoc, ProvideClassInfo)
		
		if (dwGuidKind == GUIDKIND_DEFAULT_SOURCE_DISP_IID)
		{
			*pGUID = __uuidof(IDataReportEvents);
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

BOOL CReportDoc::GetDispatchIID(IID* piid)
{
	ASSERT( piid != NULL );
	
	*piid = __uuidof(IDataReport);
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CReportDoc diagnostics

#ifdef _DEBUG
void CReportDoc::AssertValid() const
{
	CFormDoc::AssertValid();
}

void CReportDoc::Dump(CDumpContext& dc) const
{
	CFormDoc::Dump(dc);
}
#endif //_DEBUG

void CReportDoc::DrawDoc(CDC* pDC)
{
	DrawOutline(pDC, SECTION_DRAW_CAPTION);

	CFormDoc::DrawDoc(pDC);

	int uMode = pDC->SetBkMode(TRANSPARENT/*OPAQUE*/);  

	DrawOutline(pDC, SECTION_DRAW_LINE);

	pDC->SetBkMode(uMode); 
}

void CReportDoc::DrawOutline(CDC* pDC, int nDrawState)
{
	CPen penGrid(PS_DOT, 0, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&penGrid);

	CRect rect(0, 0, ROUND(GetWidth()), ROUND(GetHeight()));
		
	if (m_secReportHeader.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + m_secReportHeader.m_fHeight);

		if (nDrawState & SECTION_DRAW_CAPTION)
			m_secReportHeader.Draw(pDC, &rect);

		if (nDrawState & SECTION_DRAW_LINE)
		{
			pDC->MoveTo(rect.left, rect.bottom);
			pDC->LineTo(rect.right, rect.bottom);
		}

		rect.top = rect.bottom;
	}
	
	if (m_secPageHeader.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + m_secPageHeader.m_fHeight);

		if (nDrawState & SECTION_DRAW_CAPTION)
			m_secPageHeader.Draw(pDC, &rect);
		
		if (nDrawState & SECTION_DRAW_LINE)
		{
			pDC->MoveTo(rect.left, rect.bottom);
			pDC->LineTo(rect.right, rect.bottom);
		}

		rect.top = rect.bottom;
	}
	
	if (m_secDetail.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + m_secDetail.m_fHeight);

		if (nDrawState & SECTION_DRAW_CAPTION)
			m_secDetail.Draw(pDC, &rect);
		
		if (nDrawState & SECTION_DRAW_LINE)
		{
			pDC->MoveTo(rect.left, rect.bottom);
			pDC->LineTo(rect.right, rect.bottom);
		}
		
		rect.top = rect.bottom;
	}
	
	if (m_secPageFooter.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + m_secPageFooter.m_fHeight);

		if (nDrawState & SECTION_DRAW_CAPTION)
			m_secPageFooter.Draw(pDC, &rect);
		
		if (nDrawState & SECTION_DRAW_LINE)
		{
			pDC->MoveTo(rect.left, rect.bottom);
			pDC->LineTo(rect.right, rect.bottom);
		}

		rect.top = rect.bottom;
	}
	
	if (m_secReportFooter.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + m_secReportFooter.m_fHeight);

		if (nDrawState & SECTION_DRAW_CAPTION)
			m_secReportFooter.Draw(pDC, &rect);
		
//		if (nDrawState & SECTION_DRAW_LINE)
//		{
//			pDC->MoveTo(rect.left, rect.bottom);
//			pDC->LineTo(rect.right, rect.bottom);
//		}
	}
	
	pDC->SelectObject(pOldPen);
}

CRectF CReportDoc::GetSectionRect(CReportSection* pSection)
{
	CRectF rect(0, 0, GetWidth(), GetHeight());
	
	if (m_secReportHeader.m_bVisible)
	{
		rect.bottom = rect.top + m_secReportHeader.m_fHeight;

		if (pSection == &m_secReportHeader)
			return rect;

		rect.top = rect.bottom;
	}
	
	if (m_secPageHeader.m_bVisible)
	{
		rect.bottom = rect.top + m_secPageHeader.m_fHeight;

		if (pSection == &m_secPageHeader)
			return rect;
		
		rect.top = rect.bottom;
	}
	
	if (m_secDetail.m_bVisible)
	{
		rect.bottom = rect.top + m_secDetail.m_fHeight;

		if (pSection == &m_secDetail)
			return rect;

		rect.top = rect.bottom;
	}
	
	if (m_secPageFooter.m_bVisible)
	{
		rect.bottom = rect.top + m_secPageFooter.m_fHeight;
		
		if (pSection == &m_secPageFooter)
			return rect;
		
		rect.top = rect.bottom;
	}
	
	if (m_secReportFooter.m_bVisible)
	{
		rect.bottom = rect.top + m_secReportFooter.m_fHeight;
		
		if (pSection == &m_secReportFooter)
			return rect;
	}

	rect.SetRectEmpty();
	return rect;
}

void CReportDoc::DrawReportHeader(CDC* pDC, LPRECT pRect)
{
			
}

void CReportDoc::DrawPageHeader(CDC* pDC, LPRECT pRect)
{
	
}

void CReportDoc::DrawDetail(CDC* pDC, LPRECT pRect)
{
	
}

void CReportDoc::DrawPageFooter(CDC* pDC, LPRECT pRect)
{
	
}

void CReportDoc::DrawReportFooter(CDC* pDC, LPRECT pRect)
{
	
}

void CReportDoc::DrawBand(CDC* pDC, LPRECT pRect, LPCTSTR lpszCaption)
{
//	CBrush brush(GetSysColor(COLOR_BTNFACE));
//	pDC->FillRect(rectItem, &brush);

	pDC->Draw3dRect(pRect, GetSysColor(COLOR_BTNHILIGHT), GetSysColor(COLOR_BTNSHADOW));
	
	CFont font;
	font.CreateStockObject(DEFAULT_GUI_FONT);
	CFont* pOldFont = pDC->SelectObject(&font);
	pDC->DrawText(lpszCaption, pRect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SelectObject(pOldFont);
}

void CReportDoc::ReCalcReportHeight(BOOL bUpdate)
{
	m_position.bottom = 0;

	if (m_secReportHeader.m_bVisible)
		m_position.bottom += m_secReportHeader.m_fHeight;
	
	if (m_secPageHeader.m_bVisible)
		m_position.bottom += m_secPageHeader.m_fHeight;
	
	if (m_secDetail.m_bVisible)
		m_position.bottom += m_secDetail.m_fHeight;
	
	if (m_secPageFooter.m_bVisible)
		m_position.bottom += m_secPageFooter.m_fHeight;
	
	if (m_secReportFooter.m_bVisible)
		m_position.bottom += m_secReportFooter.m_fHeight;

	if (bUpdate)
		UpdateAllViews(NULL, HINT_UPDATE_DOCSIZE);
}

HRESULT CReportDoc::InitAmbientProperties()
{
	CString strName;

	//缺省填充风格
	strName.LoadString(IDS_DEFAULT_FILLSTYLE);
	CAmbientProperty* pProp = new CAmbientProperty(CXPROPID_DEFAULT_FILLSTYLE, strName,
		COleVariant((long)FillStyleTransparence, VT_I4), VTI_I4,
		TRUE );
	m_apAmbientProperties.Add(pProp);

	return CFormDoc::InitAmbientProperties();
}

void CReportDoc::SetLandscapeWidth()
{
	HDC hDCScreen = ::GetDC(NULL);
	ASSERT(hDCScreen != NULL);
	
	int cxPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSX);

	::ReleaseDC(NULL, hDCScreen);

	m_position.left = m_position.top = 0;
	m_position.right = 930; // float(cxPixelsPerInch * 29.7 / 2.54 - 192); //793.701
}

void CReportDoc::InitializeProperty()
{
	m_strCaption = _T("报表");

	m_clrBackColor = RGB(255, 255, 255);
	m_clrForeColor = RGB(0, 0, 0);

	HDC hDCScreen = ::GetDC(NULL);
	ASSERT(hDCScreen != NULL);

	int cxPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSX);
	int cyPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSY);

	::ReleaseDC(NULL, hDCScreen);

	// 600 + 96 + 96
	m_position.left = m_position.top = 0;
	m_position.right = float(cxPixelsPerInch * 15.875 / 2.54); // 613.7008 16.2375 15.996
	ReCalcReportHeight();

//	float a = float(cxPixelsPerInch * 21 / 2.54); //793.701
	if (m_pFormObj != NULL)
	{
		m_secPageHeader.m_bVisible = FALSE;
		m_secPageFooter.m_bVisible = FALSE;

		m_secDetail.m_strName = _T("Section2");
		m_secReportFooter.m_strName = _T("Section3");

		m_rcMargins.left = m_rcMargins.right = 0;
		m_rcMargins.top = m_rcMargins.bottom = 0;
	}
	else
	{
		m_rcMargins.left = m_rcMargins.right = float(cxPixelsPerInch * 2.54 / 2.54);
		m_rcMargins.top = m_rcMargins.bottom = float(cxPixelsPerInch * 2.54 / 2.54); //2.38125 2.499
	}
	
	// 创建数据环境
	VERIFY(m_pDataConnection.CreateInstance(CLSID_CxDataConnection) == S_OK);

	IPersistStreamInitPtr pPersistStreamInit = m_pDataConnection;			
	ASSERT(pPersistStreamInit != NULL);
	pPersistStreamInit->InitNew();

	VERIFY(m_pDataRecordset.CreateInstance(CLSID_CxDataRecordset) == S_OK);

	CComVariant var = (IUnknown *)m_pDataConnection;
	m_pDataRecordset->put_ActiveConnection(var);
	
	pPersistStreamInit = m_pDataRecordset;
	ASSERT(pPersistStreamInit != NULL);
	pPersistStreamInit->InitNew();
}

void CReportDoc::SerializeProperty(CArchive& ar)
{
	HRESULT hr;
	CLSID clsid;
	CArchiveStream stm(&ar);
	LPSTREAM pStream = &stm;
	
	if (ar.IsStoring())
	{
		ar << m_rcMargins;
		
		// 保存数据连接
		IPersistStreamInitPtr pPersistStreamInit = m_pDataConnection;			
		ASSERT(pPersistStreamInit != NULL);

		pPersistStreamInit->GetClassID(&clsid);
		hr = WriteClassStm(pStream, clsid);
		if (FAILED(hr))
			AfxThrowOleException(hr);
			
		hr = pPersistStreamInit->Save(pStream, TRUE);
		if (FAILED(hr))
			AfxThrowOleException(hr);
		
		// 保存数据记录集
		pPersistStreamInit = m_pDataRecordset;	
		ASSERT(pPersistStreamInit != NULL);
		
		pPersistStreamInit->GetClassID(&clsid);
		hr = WriteClassStm(pStream, clsid);
		if (FAILED(hr))
			AfxThrowOleException(hr);
		
		hr = pPersistStreamInit->Save(pStream, TRUE);
		if (FAILED(hr))
			AfxThrowOleException(hr);
			
	}
	else
	{
		ar >> m_rcMargins;

		// 导入数据连接
		hr = ReadClassStm(pStream, &clsid);
		if (FAILED(hr))
			AfxThrowOleException(hr);

		m_pDataConnection.CreateInstance(clsid);

		IPersistStreamInitPtr pPersistStreamInit = m_pDataConnection;
		hr = pPersistStreamInit->Load(pStream);
		if (FAILED(hr))
			AfxThrowOleException(hr);

		// 导入数据记录集
		hr = ReadClassStm(pStream, &clsid);
		if (FAILED(hr))
			AfxThrowOleException(hr);
		
		m_pDataRecordset.CreateInstance(clsid);

		CComVariant var = (IUnknown *)m_pDataConnection;
		m_pDataRecordset->put_ActiveConnection(var);
		
		pPersistStreamInit = m_pDataRecordset;
		hr = pPersistStreamInit->Load(pStream);
		if (FAILED(hr))
			AfxThrowOleException(hr);
	}

	m_secReportHeader.Serialize(ar);
	m_secPageHeader.Serialize(ar);
	m_secDetail.Serialize(ar);
	m_secPageFooter.Serialize(ar);
	m_secReportFooter.Serialize(ar);

	CFormDoc::SerializeProperty(ar);
}

void CReportDoc::OnUpateTitle()
{
	if (m_pFormObj != NULL)
		SetTitle(_T("报表"));
	else
		CFormDoc::OnUpateTitle();
}

float CReportDoc::GetLeftMargin()
{
	return m_rcMargins.left;
}

void CReportDoc::SetLeftMargin(float fNewValue)
{
	m_rcMargins.left = fNewValue;
}

float CReportDoc::GetTopMargin()
{
	return m_rcMargins.top;
}

void CReportDoc::SetTopMargin(float fNewValue)
{
	m_rcMargins.top = fNewValue;
}

float CReportDoc::GetRightMargin()
{
	return m_rcMargins.right;
}

void CReportDoc::SetRightMargin(float fNewValue)
{
	m_rcMargins.right = fNewValue;
}

float CReportDoc::GetBottomMargin()
{
	return m_rcMargins.bottom;	
}

void CReportDoc::SetBottomMargin(float fNewValue)
{
	m_rcMargins.bottom = fNewValue;
}

LPUNKNOWN CReportDoc::GetObjectUnknown()
{
	return GetInterface(&IID_IUnknown);
}

BOOL CReportDoc::IsValidObjectName(LPCTSTR lpszName)
{
	// 首先判断是否合法对象名
	if (!CFormDoc::IsValidObjectName(lpszName))
		return FALSE;
	
	CString strName = lpszName;
		
	if (strName.CompareNoCase(m_secReportHeader.m_strName) == 0)
		return FALSE;
	
	if (m_pFormObj == NULL)
	{
		if (strName.CompareNoCase(m_secPageHeader.m_strName) == 0)
			return FALSE;
	}
	
	if (strName.CompareNoCase(m_secDetail.m_strName) == 0)
		return FALSE;
	
	if (m_pFormObj == NULL)
	{
		if (strName.CompareNoCase(m_secPageFooter.m_strName) == 0)
			return FALSE;
	}
	
	if (strName.CompareNoCase(m_secReportFooter.m_strName) == 0)
		return FALSE;
	
	return TRUE;
}

BOOL CReportDoc::IsValidSectionName(LPCTSTR lpszName)
{
	if (!IsValidObjectName(lpszName))
		return FALSE;

	// 已经存在该对象
	CDevObj* pSearchObj = NULL;
	if (GetNamedObjectsMap()->LookupNoCase(lpszName, pSearchObj))
		return FALSE;

	return TRUE;
}

HRESULT CReportDoc::MapPropertyToCategory(DISPID dispid, PROPCAT* ppropcat)
{
	switch (dispid)
	{
	case CXPROPID_LeftMargin:
	case CXPROPID_TopMargin:
	case CXPROPID_RightMargin:
	case CXPROPID_BottomMargin:
		*ppropcat = -6;
		break;

	case CXPROPID_DataConnection:
	case CXPROPID_DataRecordset:
		*ppropcat = -7;
		break;	
	default:
		return CFormDoc::MapPropertyToCategory(dispid, ppropcat);
	}
	
	return S_OK;
}

HRESULT CReportDoc::GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName)
{
	if (propcat == -6)
	{
		*pbstrName = ::SysAllocString(OLESTR("边距"));
		return S_OK;
	}
	else if (propcat == -7)
	{
		*pbstrName = ::SysAllocString(OLESTR("数据"));
		return S_OK;
	}
		
	return CFormDoc::GetCategoryName(propcat, lcid, pbstrName);
}

ICxDataConnection* CReportDoc::GetDataConnection()
{
	ICxDataConnection* pDataConnection = NULL;
	
	if (m_pDataConnection != NULL)
	{
		m_pDataConnection->AddRef();
		return m_pDataConnection;
	}
	
	return NULL;
}

void CReportDoc::SetDataConnection(ICxDataConnection* pDataConnection)
{
	ASSERT(m_pDataConnection != NULL);
	
	if (pDataConnection != NULL)
	{
		if (m_pDataConnection != NULL)
			m_pDataConnection->Release();
		
		pDataConnection->QueryInterface(IID_ICxDataConnection, (void**)&m_pDataConnection);
	}
}

ICxDataRecordset* CReportDoc::GetDataRecordset()
{
	ICxDataRecordset* pDataRecordset = NULL;
	
	if (m_pDataRecordset != NULL)
	{
		m_pDataRecordset->AddRef();
		return m_pDataRecordset;
	}
	
	return NULL;
}

void CReportDoc::SetDataRecordset(ICxDataRecordset* pDataRecordset)
{
	ASSERT(m_pDataRecordset != NULL);

	if (pDataRecordset != NULL)
	{
		if (m_pDataRecordset != NULL)
			m_pDataRecordset->Release();

		pDataRecordset->QueryInterface(IID_ICxDataRecordset, (void**)&m_pDataRecordset);
	}
}

CReportDoc::CReportSubDoc::CReportSubDoc()
{
}

CFormDoc* CReportDoc::CReportSubDoc::CreateFormDoc()
{
	CFormDoc* pFormDoc = new CReportDoc(m_pParentDoc);
	
	pFormDoc->m_pFormSubDoc = this;
	if (!pFormDoc->OnNewDocument())
	{
		delete pFormDoc;
		return NULL;
	}
	
	// 创建名称并更新标题
	if (m_strName.IsEmpty())
	{
		ASSERT(!IsFirstSubDoc());
		m_pParentDoc->CreateUniqueSubDocumentName(_T("Report"), m_strName);
	}
	
	pFormDoc->OnUpateTitle();
	
	m_pChildDoc = pFormDoc;
	
	return pFormDoc;
}

const CLSID CLSID_FormatDynamic = {0x6DC9B1A9,0x0153,0x46C2,{0x90,0x0F,0xD6,0xC1,0x4F,0x05,0x7B,0x12}};
const CLSID CLSID_DrawTxt = {0xE9973BB1,0x6C00,0x11D5,{0xA4,0x6A,0xB9,0xDB,0x27,0x52,0xB6,0x66}};
const CLSID CLSID_DrawDataLink = {0x653F53C1,0x8EF4,0x42FC,{0x91,0x19,0x22,0x5C,0x51,0xDE,0x65,0x23}};
const CLSID CLSID_DrawLine = {0x26EA8099,0x912C,0x4EEE,{0xAD,0x13,0xFD,0x9E,0xA6,0xB0,0xD9,0x5C}};
const CLSID CLSID_DrawRectangle = {0xEF1554D7,0x9747,0x4787,{0xAA,0x0A,0x60,0x05,0x11,0x8D,0x23,0x8C}};

const IID IID_IDrawTxt = {0xE9973BB0,0x6C00,0x11D5,{0xA4,0x6A,0xB9,0xDB,0x27,0x52,0xB6,0x66}};
const IID IID_IDrawDataLink = {0xF098E098,0xDAA3,0x4B29,{0x90,0x0D,0x9D,0x77,0xA1,0xE7,0xB4,0xFB}};
const IID IID_IDrawLine = {0x8C82DC47,0x4DD8,0x440D,{0x94,0xDC,0x91,0x4F,0xDF,0x5B,0xEB,0xB6}};
const IID IID_IDrawRectangle = {0x8F563EFF,0xCC51,0x4956,{0xB8,0x81,0xE6,0x70,0x07,0xA3,0x37,0xB1}};

CDevObj* CReportDoc::AddFieldDataLink(LPCTSTR lpszField, CPointF point, float fWidth, enumTextAlign nAlign, int nFontSize, enumFontStyle nFontStyle)
{
	USES_CONVERSION;

	CString strText;
	strText.Format(_T("[%s]"), lpszField);
	CDevObj* pObj = AddLabel(strText, point, fWidth, nAlign, nFontSize, nFontStyle);

	// 数据连接对象
	IUnknownPtr pFormatDynamic;
	pFormatDynamic.CreateInstance(CLSID_FormatDynamic);

	ICxDynamicObjectPtr pDynamicObject = pFormatDynamic;
	if (pDynamicObject != NULL)
	{
		CDynamicPropInfo propinfo;
		propinfo.pfnExpression = ExpressionCreatorProc;
		pDynamicObject->InitNew(long(&propinfo));
	}

	IDispatchPtr pDisp = pFormatDynamic;
	COleDispatchDriver dd(pDisp, FALSE);
	CString strSource;
	strSource.Format(_T("DataRecordset(\"%s\").Value"), lpszField);
	dd.SetProperty(1, VT_BSTR, strSource);
	dd.SetProperty(3, VT_I4, 0);

	CComQIPtr<IDrawTxt, &IID_IDrawTxt> spDrawTxt(pObj->GetObjectUnknown());
	spDrawTxt->ConnectObject(L"Caption", pFormatDynamic);

	return pObj;
}

CDevObj* CReportDoc::AddDataLink(LPCTSTR lpszSource, CPointF point, LPCTSTR lpszCaption, enumTextAlign nAlign, int nFontSize, enumFontStyle nFontStyle)
{
	USES_CONVERSION;
	
	CDevObj* pObj = AddLabel(lpszCaption, point, 0, nAlign, nFontSize, nFontStyle);
	
	// 数据连接对象
	IUnknownPtr pFormatDynamic;
	pFormatDynamic.CreateInstance(CLSID_FormatDynamic);
	
	ICxDynamicObjectPtr pDynamicObject = pFormatDynamic;
	if (pDynamicObject != NULL)
	{
		CDynamicPropInfo propinfo;
		propinfo.pfnExpression = ExpressionCreatorProc;
		pDynamicObject->InitNew(long(&propinfo));
	}
	
	IDispatchPtr pDisp = pFormatDynamic;
	COleDispatchDriver dd(pDisp, FALSE);
	dd.SetProperty(1, VT_BSTR, lpszSource);
	dd.SetProperty(3, VT_I4, 0);
	
	CComQIPtr<IDrawTxt, &IID_IDrawTxt> spDrawTxt(pObj->GetObjectUnknown());
	spDrawTxt->ConnectObject(L"Caption", pFormatDynamic);
	
	return pObj;
}

//CDevObj* CReportDoc::AddDataLink(LPCTSTR lpszSource, CPointF point, LPCTSTR lpszCaption, enumTextAlign nAlign, int nFontSize, enumFontStyle nFontStyle)
//{
//	USES_CONVERSION;
//	
//	// 数据连接对象
//	IUnknownPtr pFormatDynamic;
//	pFormatDynamic.CreateInstance(CLSID_FormatDynamic);
//	
//	ICxDynamicObjectPtr pDynamicObject = pFormatDynamic;
//	if (pDynamicObject != NULL)
//	{
//		PROPERTYINFO propinfo;
//		memset(&propinfo, 0, sizeof(propinfo));
//		propinfo.pfnExpression = ExpressionCreatorProc;
//		pDynamicObject->InitNew(long(&propinfo));
//	}
//	
//	IDispatchPtr pDisp = pFormatDynamic;
//	COleDispatchDriver dd(pDisp, FALSE);
//	dd.SetProperty(1, VT_BSTR, lpszSource);
//	dd.SetProperty(3, VT_I4, 0);
//	
//	// 数据戳对象
//	CDrawObj* pObj = AddDrawObj(CLSID_DrawDataLink, NULL);
//	
//	CComQIPtr<IDrawDataLink, &IID_IDrawDataLink> spDataLink(pObj->GetObjectUnknown());
//	spDataLink->ConnectObject(L"Caption", pFormatDynamic);
//	spDataLink->put_Alignment(nAlign);
//	if (lpszCaption != NULL)
//		spDataLink->put_Caption(T2OLE(lpszCaption));
//	spDataLink->put_FontStyle(nFontStyle);
//	spDataLink->put_FontSize(nFontSize);
//	
//	CRectF rcPos = pObj->GetPositionRect();
//	switch (nAlign) 
//	{
//	case TextAlignCenter:
//		rcPos.OffsetRect(point.x - (rcPos.left + rcPos.right) / 2, 
//			point.y - (rcPos.top + rcPos.bottom) / 2);
//		break;
//	case TextAlignRight:
//		rcPos.OffsetRect(point.x - rcPos.right, 
//			point.y - (rcPos.top + rcPos.bottom) / 2);
//		break;
//	default:
//		rcPos.OffsetRect(point.x - rcPos.left, 
//			point.y - (rcPos.top + rcPos.bottom) / 2);
//		break;
//	}
//	
//	pObj->MoveTo(rcPos, FALSE);
//	
//	return pObj;

CDevObj* CReportDoc::AddDigitalDataLink(LPCTSTR lpszSource, CPointF point, int nDigital, enumTextAlign nAlign, int nFontSize, enumFontStyle nFontStyle)
{
	USES_CONVERSION;

	CDevObj* pObj = AddDataLink(lpszSource, point, NULL, nAlign, nFontSize, nFontStyle);
	
	CComQIPtr<IDrawDataLink, &IID_IDrawDataLink> spDataLink(pObj->GetObjectUnknown());
	
	IUnknown* pUnk;
	spDataLink->GetConnectObject(L"Caption", &pUnk);
	
	IDispatchPtr pDisp = pUnk;
	COleDispatchDriver dd(pDisp, FALSE);
	
	dd.SetProperty(3, VT_I4, 1);
	static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4;
	dd.InvokeHelper(4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, 4, 0, 0);
	
	CString strSample;
	dd.GetProperty(7, VT_BSTR, (void*)&strSample);
	spDataLink->put_Value(T2OLE(strSample));

	return pObj;
}

CDevObj* CReportDoc::AddLabel(LPCTSTR lpszText, CPointF point, float fWidth, enumTextAlign nAlign, int nFontSize, enumFontStyle nFontStyle)
{
	USES_CONVERSION;
	
	// 文本对象
	CDrawObj* pObj = AddDrawObj(CLSID_DrawTxt, NULL);
	
	CComQIPtr<IDrawTxt, &IID_IDrawTxt> spDrawTxt(pObj->GetObjectUnknown());
	spDrawTxt->put_Alignment(nAlign);
	spDrawTxt->put_Caption(T2OLE(lpszText));
	spDrawTxt->put_FontStyle(nFontStyle);
	spDrawTxt->put_FontSize(nFontSize);
	
	CRectF rcPos = pObj->GetPositionRect();
	switch (nAlign) 
	{
	case TextAlignCenter:
		rcPos.OffsetRect(point.x + fWidth / 2 - (rcPos.left + rcPos.right) / 2, 
			point.y - (rcPos.top + rcPos.bottom) / 2);
		break;
	case TextAlignRight:
		rcPos.OffsetRect(point.x + fWidth - rcPos.right, 
					point.y - (rcPos.top + rcPos.bottom) / 2);
		break;
	default:
		rcPos.OffsetRect(point.x - rcPos.left, 
			point.y - (rcPos.top + rcPos.bottom) / 2);
		break;
	}

	pObj->MoveTo(rcPos, FALSE);

	return pObj;
}

CDevObj* CReportDoc::AddLine(float fX1, float fY1, float fX2, float fY2, float fWidth, COLORREF crColor)
{
	// 文本对象
	CDrawObj* pObj = AddDrawObj(CLSID_DrawLine, NULL);
	
	CComQIPtr<IDrawLine, &IID_IDrawLine> spDrawLine(pObj->GetObjectUnknown());
	spDrawLine->put_EdgeWidth(fWidth);
	spDrawLine->put_EdgeColor(crColor);
	
	CRectF rcPos(fX1, fY1, fX2, fY2);
	pObj->MoveTo(rcPos, FALSE);
	CPoint point(ROUND(fX2), ROUND(fY2));
	pObj->MoveHandleTo(2, CDevObj::reshape, point);

	return pObj;
}

CDevObj* CReportDoc::AddRectangle(float fX1, float fY1, float fX2, float fY2, float fWidth, COLORREF crColor)
{
	// 文本对象
	CDrawObj* pObj = AddDrawObj(CLSID_DrawRectangle, NULL);
	
	CComQIPtr<IDrawRectangle, &IID_IDrawRectangle> spDrawRectangle(pObj->GetObjectUnknown());
	spDrawRectangle->put_EdgeWidth(fWidth);
	spDrawRectangle->put_EdgeColor(crColor);
	
	CRectF rcPos(fX1, fY1, fX2, fY2);
	pObj->MoveTo(rcPos, FALSE);
// 	CPoint point(ROUND(fX2), ROUND(fY2));
// 	pObj->MoveHandleTo(2, CDevObj::reshape, point);
	
	return pObj;
}

int CReportDoc::GetPageCount()
{
	return 0;
}

int CReportDoc::GetPage()
{
	return 0;
}

void CReportDoc::PrintOut()
{
}

void CReportDoc::PrintPreview()
{
}