// ReportDoc.cpp: implementation of the CReportDoc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RunInc.h"
#include "RunCtrl.h"

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
	m_secReportHeader(this, _T("Section1"), _T("报眉"), 90),
	m_secPageHeader(this, _T("Section2"), _T("页眉"), 90),
	m_secDetail(this, _T("Section3"), _T("详细内容"), 360),
	m_secPageFooter(this, _T("Section4"), _T("页脚"), 90),
	m_secReportFooter(this, _T("Section5"), _T("报脚"), 90)
{
	//	m_pSelectSection = &m_secPageFooter;

	m_bPrint = FALSE;
	m_nTotalPageCount = 0;
	m_nRecordCount = 0;
}

CReportDoc::~CReportDoc()
{
	
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
	
	m_pChildDoc = pFormDoc;
	
	return pFormDoc;
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
	DISP_PROPERTY_EX_ID(CReportDoc, "DataConnection", CXPROPID_DataConnection, GetDataConnection, SetDataConnection, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CReportDoc, "DataRecordset", CXPROPID_DataRecordset, GetDataRecordset, SetDataRecordset, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CReportDoc, "PageCount", CXPROPID_PageCount, GetPageCount, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CReportDoc, "Page", CXPROPID_Page, GetPage, SetNotSupported, VT_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

//const extern IID LIBID_CXHMIOBJLib;
//IMPLEMENT_OLETYPELIB(CReportDoc, LIBID_CXHMIOBJLib, 1, 0)

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
		
//	return theApp.m_pExtTypeLib->GetTypeInfoOfGuid(__uuidof(DataReport), ppTypeInfo);
	return pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), __uuidof(DataReport), ppTypeInfo);
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

	CBrush* pBlackBrush = CBrush::FromHandle((HBRUSH)::GetStockObject(BLACK_BRUSH));
	

	CRectF rectF(0, 0, GetWidth(), GetHeight());
	
	rectF.DeflateRect(m_rcMargins.left, m_rcMargins.top, 
			m_rcMargins.right, m_rcMargins.bottom); // Margin

	CRect rect = rectF.GetRECT();

	if (nDrawState & SECTION_DRAW_LINE)
	{
		pDC->MoveTo(0, rect.top);
		pDC->LineTo(ROUND(GetWidth()), rect.top);
		
		pDC->MoveTo(0, rect.bottom);
		pDC->LineTo(ROUND(GetWidth()), rect.bottom);
		
		pDC->MoveTo(rect.left, 0);
		pDC->LineTo(rect.left, ROUND(GetHeight()));
		
		pDC->MoveTo(rect.right, 0);
		pDC->LineTo(rect.right, ROUND(GetHeight()));
	}
		
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
		
		if (nDrawState & SECTION_DRAW_LINE)
		{
			pDC->MoveTo(rect.left, rect.bottom);
			pDC->LineTo(rect.right, rect.bottom);
		}

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

void CReportDoc::DrawReportHeader(CDC* pDC, int& nHeightLeftOver)
{
	if (!m_secReportHeader.m_bVisible)
		return;

	int nSectionHeight = ROUND(m_secReportHeader.m_fHeight);
	if (nSectionHeight >= m_nPageHeight && m_secReportHeader.m_bKeepTogether)
	{
		DrawSectionBand(pDC, &m_secReportHeader, 0, 0, nHeightLeftOver);

		OnBreakPage(pDC, nHeightLeftOver);
		return;
	}
	
	int nSectionHeightLeftOver = nSectionHeight;

	// 跨页直接跳过
	while (nSectionHeightLeftOver > m_nPageHeight)
	{
		DrawSectionBand(pDC, &m_secReportHeader, 0, nSectionHeight - nSectionHeightLeftOver, nHeightLeftOver);
		nSectionHeightLeftOver -= m_nPageHeight;
		
		m_nPageCount++;
		nHeightLeftOver = m_nPageHeight;
	}

	DrawSectionBand(pDC, &m_secReportHeader, 0, nSectionHeight - nSectionHeightLeftOver);
	PageHeightDecrement(nHeightLeftOver, nSectionHeightLeftOver);
	
	// 是否需要新的页
	if (!m_bPageBreak &&
		(m_secReportHeader.m_enumForcePageBreak == PageBreakAfter || 
		m_secReportHeader.m_enumForcePageBreak == PageBreakBeforeAndAfter))
	{
		OnBreakPage(pDC, nHeightLeftOver);
	}		
}

BOOL CReportDoc::DrawPageHeader(CDC* pDC, int& nHeightLeftOver)
{
	if (!m_secPageHeader.m_bVisible)
		return TRUE;

	int nSectionHeight = ROUND(m_secPageHeader.m_fHeight);

	// 眉头不能大于页高
	if (nSectionHeight >= nHeightLeftOver)
		return FALSE;

	// 如果必须保持在一起，且剩余高度不够
	if (nSectionHeight >= nHeightLeftOver && m_secPageHeader.m_bKeepTogether)
		return FALSE;
	
	DrawSectionBand(pDC, &m_secPageHeader, m_nPageHeight - nHeightLeftOver, 0, nHeightLeftOver);
	nHeightLeftOver -= nSectionHeight;

	return TRUE;
}

BOOL CReportDoc::DrawDetail(CDC* pDC, int& nHeightLeftOver)
{
	if (!m_secDetail.m_bVisible)
		return TRUE;

	// 是否需要新的页
	if (!m_bPageBreak &&
		(m_secDetail.m_enumForcePageBreak == PageBreakBefore || 
		m_secDetail.m_enumForcePageBreak == PageBreakBeforeAndAfter))
	{
		OnBreakPage(pDC, nHeightLeftOver);
	}

	int nSectionHeightLeftOver = 0;

	while (1)
	{
		if (!DrawDetailPage(pDC, nHeightLeftOver, nSectionHeightLeftOver))
			return FALSE;

		if (m_nCurrentRecord >= m_nRecordCount)
			break;

		OnBreakPage(pDC, nHeightLeftOver);
	}

	// 是否需要新的页
	if (!m_bPageBreak &&
		(m_secDetail.m_enumForcePageBreak == PageBreakBefore || 
		m_secDetail.m_enumForcePageBreak == PageBreakBeforeAndAfter))
	{
		OnBreakPage(pDC, nHeightLeftOver);
	}

	return TRUE;
}

BOOL CReportDoc::OnBreakPage(CDC* pDC, int& nHeightLeftOver)
{
	m_bPageBreak = TRUE;
	m_nPageCount++;
	nHeightLeftOver = m_nPageHeight;

	// 绘制页眉
	if (!DrawPageHeader(pDC, nHeightLeftOver))
		return FALSE;
	
	if (!DrawPageFooter(pDC, nHeightLeftOver))
		return FALSE;
	
	return TRUE;
}

void CReportDoc::PageHeightDecrement(int& nHeightLeftOver, int nDecrement)
{
	nHeightLeftOver -= nDecrement;
	m_bPageBreak = FALSE;
}

// 绘制普通页
BOOL CReportDoc::DrawDetailPage(CDC* pDC, int& nHeightLeftOver, int& nSectionHeightLeftOver)
{
	// 紧接报头，进行特殊处理
	if (!m_bPageBreak)
	{
		// 绘制页眉
		if (!DrawPageHeader(pDC, nHeightLeftOver))
			return TRUE;

		// 绘制页脚
		if (!DrawPageFooter(pDC, nHeightLeftOver))
			return TRUE;

		if (nHeightLeftOver <= 0)
			return TRUE;
	}

	if (m_nCurrentRecord >= m_nRecordCount)
		return TRUE;

	int nSectionHeight = ROUND(m_secDetail.m_fHeight);
	int nFooterHeight = GetPageFooterHeight();
	
	// 如果剩余高度不够
	if (nHeightLeftOver <= 0 || 
		(m_bPageBreak && nHeightLeftOver < nSectionHeight && m_secDetail.m_bKeepTogether))
		return FALSE;
	
	// 详细资料大于剩余高度并且必须保持在一起, 直接过度到下一记录
	if (nSectionHeight >= nHeightLeftOver && m_secDetail.m_bKeepTogether)
	{
		m_nCurrentRecord++;
	}
	else
	{
		// 处理剩余高度
		if (nSectionHeightLeftOver > 0)
		{
			ASSERT(!m_secDetail.m_bKeepTogether);

			DrawSectionBand(pDC, &m_secDetail, m_nPageHeight - nHeightLeftOver - nFooterHeight, nSectionHeight - nSectionHeightLeftOver, nHeightLeftOver);

			// 接着处理
			if (nSectionHeightLeftOver > nHeightLeftOver)
			{	
				nSectionHeightLeftOver -= nHeightLeftOver;
				return TRUE;
			}

			PageHeightDecrement(nHeightLeftOver, nSectionHeightLeftOver);
			nSectionHeightLeftOver = 0;
			
			m_nCurrentRecord++;
			if (m_nCurrentRecord >= m_nRecordCount)
				return TRUE;
		}
		
		// 处理完整记录
		while (nHeightLeftOver > nSectionHeight)
		{
			DrawSectionBand(pDC, &m_secDetail, m_nPageHeight - nHeightLeftOver - nFooterHeight, 0);
			PageHeightDecrement(nHeightLeftOver, nSectionHeight);
			
			m_nCurrentRecord++;
			if (m_nCurrentRecord >= m_nRecordCount)
				return TRUE;
		}

		if (!m_secDetail.m_bKeepTogether)
		{
			if (nHeightLeftOver > 0)
			{
				m_nCurrentRecord++;
				if (m_nCurrentRecord >= m_nRecordCount)
					return TRUE;

				DrawSectionBand(pDC, &m_secDetail, m_nPageHeight - nHeightLeftOver - nFooterHeight, 0, nHeightLeftOver);
				nSectionHeightLeftOver = nSectionHeight - nHeightLeftOver;
			}
		}
	}

	return TRUE;
}

int CReportDoc::GetPageFooterHeight()
{
	return ROUND(m_secPageFooter.m_fHeight);
}

BOOL CReportDoc::DrawPageFooter(CDC* pDC, int& nHeightLeftOver)
{
	if (!m_secPageFooter.m_bVisible)
		return TRUE;

	int nSectionHeight = ROUND(m_secPageFooter.m_fHeight);
	
	// 眉脚不能大于剩余高度
	if (nSectionHeight >= nHeightLeftOver)
		return FALSE;
	
	DrawSectionBand(pDC, &m_secPageFooter, m_nPageHeight - nSectionHeight);
	nHeightLeftOver -= nSectionHeight;
		
	return TRUE;	
}

void CReportDoc::DrawReportFooter(CDC* pDC, int& nHeightLeftOver)
{
	if (!m_secReportFooter.m_bVisible)
		return;

	// 是否需要新的页
	if (!m_bPageBreak &&
		(m_secReportFooter.m_enumForcePageBreak == PageBreakBefore || 
		m_secReportFooter.m_enumForcePageBreak == PageBreakBeforeAndAfter))
	{
		OnBreakPage(pDC, nHeightLeftOver);
	}

	int nSectionHeight = ROUND(m_secReportFooter.m_fHeight);
	int nFooterHeight = GetPageFooterHeight();
	if (m_secReportFooter.m_bKeepTogether)
	{
		if (nHeightLeftOver < nSectionHeight)
			OnBreakPage(pDC, nHeightLeftOver);

		DrawSectionBand(pDC, &m_secReportFooter, m_nPageHeight - nHeightLeftOver - nFooterHeight);
	}
	else
	{
		int nSectionHeightLeftOver = nSectionHeight;
		while (1)
		{
			DrawSectionBand(pDC, &m_secReportFooter, m_nPageHeight - nHeightLeftOver - nFooterHeight, nSectionHeight - nSectionHeightLeftOver, nHeightLeftOver);
			nSectionHeightLeftOver -= nHeightLeftOver;
			
			if (nSectionHeightLeftOver <= 0)
				break;

			OnBreakPage(pDC, nHeightLeftOver);
		}
	}
}

BOOL CReportDoc::DrawReport(CDC* pDC)
{
	m_bPageBreak = TRUE;
	m_nPageCount = 1;
	int nHeightLeftOver = m_nPageHeight;

	DrawReportHeader(pDC, nHeightLeftOver);
	
	if (!DrawDetail(pDC, nHeightLeftOver))
		return FALSE;

	DrawReportFooter(pDC, nHeightLeftOver);
	
	return TRUE;
}

void CReportDoc::DrawBand(CDC* pDC, LPRECT pRect, LPCTSTR lpszCaption)
{
	POSITION posObj = m_objects.GetHeadPosition();
	while (posObj != NULL)
	{
		CDevObj* pObj = (CDevObj *)m_objects.GetNext(posObj);
		
		CRect rcBound = pObj->GetBoundingRect();
		if (rcBound.IntersectRect(&rcBound, pRect))
		{
			pObj->ParseDynamic();
			if (m_bPrint)
				pObj->Print(pDC);
			else
				pObj->Draw(pDC);
		}
	}
}

void CReportDoc::DrawSectionBand(CDC* pDC, CReportSection* pSection, int nOffset, int nSectionOffset, int nSectionHeight)
{
	if (m_nCurrentPage == m_nPageCount)
	{
		pDC->SaveDC();

		CRect rect = GetSectionRect(pSection).GetRECT();

		pDC->SetWindowOrg(-m_rcPrintMargins.left, -m_rcPrintMargins.top + rect.top + nSectionOffset - nOffset);
		
		CRect rectClip = rect;
		rectClip.top = rect.top + nSectionOffset;
		if (nSectionHeight != -1 && nSectionHeight < rectClip.Height())
			rectClip.bottom = rectClip.top + nSectionHeight;
				
		pDC->IntersectClipRect(rectClip);

		if (pSection == &m_secDetail)
			m_pDataRecordset->put_AbsolutePosition(enumPosition(m_nCurrentRecord + 1)); 
		DrawBand(pDC, &rect, NULL);
		
		pDC->RestoreDC(-1);
	}
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
		UpdateAllViews(NULL);
}

void CReportDoc::SerializeProperty(CArchive& ar)
{
	//运行模式不支持存储
	ASSERT(ar.IsLoading());
	
	HRESULT hr;
	CLSID clsid;
	CArchiveStream stm(&ar);
	LPSTREAM pStream = &stm;
	
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

	m_secReportHeader.Serialize(ar);
	m_secPageHeader.Serialize(ar);
	m_secDetail.Serialize(ar);
	m_secPageFooter.Serialize(ar);
	m_secReportFooter.Serialize(ar);

	CFormDoc::SerializeProperty(ar);
}

void CReportDoc::OnUpateTitle()
{
	SetTitle(_T("数据报表"));	
}

void CReportDoc::SetDocSize(float fWidth, float fHeight, BOOL bObject)
{
//	ASSERT(m_pFormObj != NULL);
//
//	if (!bObject)
//	{
//		if (fWidth < 0 || fHeight < 0)
//			return;
//		
//		m_position.right = m_position.left + fWidth;
//		m_position.bottom = m_position.top + fHeight;
//
//		UpdateAllViews(NULL, HINT_UPDATE_DOCSIZE);
//	}

}

float CReportDoc::GetLeftMargin()
{
	return m_rcMargins.left;
}

void CReportDoc::SetLeftMargin(float fNewValue)
{
	m_rcMargins.left = fNewValue;
	UpdateAllViews(NULL);
}

float CReportDoc::GetTopMargin()
{
	return m_rcMargins.top;
}

void CReportDoc::SetTopMargin(float fNewValue)
{
	m_rcMargins.top = fNewValue;
	UpdateAllViews(NULL);
}

float CReportDoc::GetRightMargin()
{
	return m_rcMargins.right;
}

void CReportDoc::SetRightMargin(float fNewValue)
{
	m_rcMargins.right = fNewValue;
	UpdateAllViews(NULL);
}

float CReportDoc::GetBottomMargin()
{
	return m_rcMargins.bottom;	
}

void CReportDoc::SetBottomMargin(float fNewValue)
{
	m_rcMargins.bottom = fNewValue;
	UpdateAllViews(NULL);
}

LPUNKNOWN CReportDoc::GetObjectUnknown()
{
	return GetInterface(&IID_IUnknown);
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
		ICxDataConnectionPtr pDataConnectionT;

		HRESULT hr = pDataConnection->QueryInterface(IID_ICxDataConnection, (void**)&pDataConnectionT);
		if (SUCCEEDED(hr))
		{
			if (m_pDataConnection != NULL)
				m_pDataConnection->Release();

			m_pDataConnection = pDataConnectionT;
		}
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

void CReportDoc::PreparePrint(CDC* pDC)
{
	POINT point;
	
	pDC->Escape(GETPRINTINGOFFSET, 0, NULL, &point);
	m_rcPrintMargins.left = point.x;
	m_rcPrintMargins.top  = point.y;
	
	pDC->Escape(GETPHYSPAGESIZE, 0, NULL, &point);
	
	m_rcPrintMargins.right  = point.x                     // total paper width
		- pDC->GetDeviceCaps(HORZRES) // printable width
		- m_rcPrintMargins.left;   // left unprtable margin
	
	m_rcPrintMargins.bottom = point.y                     // total paper height
		- pDC->GetDeviceCaps(VERTRES) // printable ht
		- m_rcPrintMargins.top;    // rt unprtable margin
	
	CSize szPage = point;
	pDC->DPtoLP(&szPage);
	pDC->DPtoLP(m_rcPrintMargins);
	
	m_rcPrintMargins.left = ROUND(m_rcMargins.left) - m_rcPrintMargins.left;
	m_rcPrintMargins.top = ROUND(m_rcMargins.top) - m_rcPrintMargins.top;
	m_rcPrintMargins.right = ROUND(m_rcMargins.right) - m_rcPrintMargins.right;
	m_rcPrintMargins.bottom = ROUND(m_rcMargins.bottom) - m_rcPrintMargins.bottom;
	
	m_nPageHeight = int(szPage.cy - m_rcMargins.top - m_rcMargins.bottom);

	// 计算页数
	DrawReportPage(pDC, 0);
	m_nTotalPageCount = m_nPageCount;
}

BOOL CReportDoc::DrawReportPage(CDC* pDC, int nPage)
{
	m_nCurrentRecord = 0;
	m_nPageCount = 0;
	m_nCurrentPage = nPage; // 不进行绘制

	return DrawReport(pDC);
}

int CReportDoc::GetPageCount()
{
	return m_nTotalPageCount;
}

int CReportDoc::GetPage()
{
	return m_nCurrentPage;
}

void CReportDoc::PrintOut()
{
	m_pDataRecordset->Open();
	m_pDataRecordset->get_RecordCount((long *)&m_nRecordCount);

	FireEvent(CXEVENID_INITIALIZE, EVENT_PARAM(VTS_NONE));
	
	BOOL bAutoDelete = m_bAutoDelete;
	m_bAutoDelete = FALSE;
	m_pDocument->m_pRunCtrl->PrintDirect(this, RUNTIME_CLASS(CReportView));
	m_bAutoDelete = bAutoDelete;
}

void CReportDoc::PrintPreview()
{
	m_pDataRecordset->Open();
	m_pDataRecordset->get_RecordCount((long *)&m_nRecordCount);

	FireEvent(CXEVENID_INITIALIZE, EVENT_PARAM(VTS_NONE));

	BOOL bAutoDelete = m_bAutoDelete;
	m_bAutoDelete = FALSE;
	m_pDocument->m_pRunCtrl->PrintPreview(NULL, this, RUNTIME_CLASS(CReportView));
	m_bAutoDelete = bAutoDelete;
}

