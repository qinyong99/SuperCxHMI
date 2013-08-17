// ReportDoc.h: interface for the CReportDoc class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __REPORTDOC_H__
#define __REPORTDOC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

_COM_SMARTPTR_TYPEDEF(ICxDataConnection, __uuidof(ICxDataConnection));
_COM_SMARTPTR_TYPEDEF(ICxDataRecordset, __uuidof(ICxDataRecordset));
_COM_SMARTPTR_TYPEDEF(ICxDataField, __uuidof(ICxDataField));

class CReportDoc : public CFormDoc  
{
	enum 
	{
		SECTION_DRAW_CAPTION = 1,
		SECTION_DRAW_LINE = 2,
		SECTION_DRAW_ALL = 3
	};

public:
	DECLARE_DYNCREATE(CReportDoc)

	// create from serialization only
	CReportDoc(CDevDoc* pDoc = NULL);

// Attributes
public:
	class CReportSubDoc : public CFormDoc::CFormSubDoc
	{
	public:
		CReportSubDoc();
		
	public:
		virtual CDevDoc::SUBDOCTYPE GetType() { return CDevDoc::enumReportDoc; }
		virtual CFormDoc* CreateFormDoc();
	};

	virtual CString GetClassName() { return _T("DataReport"); }

	ICxDataConnectionPtr m_pDataConnection;
	ICxDataRecordsetPtr m_pDataRecordset;

protected:
	CRectF m_rcMargins; // MSDN: Article ID: Q105444 
	CRect m_rcPrintMargins;

	CReportSection m_secReportHeader;
	CReportSection m_secPageHeader;
	CReportSection m_secDetail;
	CReportSection m_secPageFooter;
	CReportSection m_secReportFooter;

	BOOL m_bPrint;

	int m_nCurrentRecord;
	int m_nRecordCount;
	int m_nCurrentPage; // 当前页
	int m_nPageCount; // 页数，只做传递变量使用
	int m_nTotalPageCount; // 总页数
	int m_nPageHeight; // 页高度

	BOOL m_bPageBreak;

// Operations
public:
	void ReCalcReportHeight(BOOL bUpdate = FALSE);

	void DrawDoc(CDC* pDC);
	void DrawOutline(CDC* pDC, int nDrawState = SECTION_DRAW_ALL);

	BOOL DrawReportPage(CDC* pDC, int nPage);
	BOOL DrawReport(CDC* pDC);
	void DrawReportHeader(CDC* pDC, int& nHeightLeftOver);
	BOOL DrawPageHeader(CDC* pDC, int& nHeightLeftOver);
	BOOL DrawDetail(CDC* pDC, int& nHeightLeftOver);
	BOOL DrawDetailPage(CDC* pDC, int& nHeightLeftOver, int& nSectionHeightLeftOver);
	BOOL DrawPageFooter(CDC* pDC, int& nHeightLeftOver);
	void DrawReportFooter(CDC* pDC, int& nHeightLeftOver);

	int GetPageFooterHeight();
	BOOL OnBreakPage(CDC* pDC, int& nHeightLeftOver);
	void PageHeightDecrement(int& nHeightLeftOver, int nDecrement);
	
	CRectF GetSectionRect(CReportSection* pSection);

	void DrawBand(CDC* pDC, LPRECT pRect, LPCTSTR lpszCaption);
	void DrawSectionBand(CDC* pDC, CReportSection* pSection, int nOffset, 
		int nSectionOffset = 0, int nSectionHeight = -1);

	void PreparePrint(CDC* pDC);

// Implementation
public:
	virtual ~CReportDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif	


	virtual void SerializeProperty(CArchive& ar);

	virtual void OnUpateTitle();

	virtual LPUNKNOWN GetObjectUnknown();

	void SetDocSize(float fWidth, float fHeight, BOOL bObject = FALSE);

// Generated message map functions
protected:
	//{{AFX_MSG(CReportDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
//	DECLARE_OLETYPELIB(CReportDoc)
		
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CReportDoc)
	afx_msg float GetLeftMargin();
	afx_msg void SetLeftMargin(float fNewValue);
	afx_msg float GetTopMargin();
	afx_msg void SetTopMargin(float fNewValue);
	afx_msg float GetRightMargin();
	afx_msg void SetRightMargin(float fNewValue);
	afx_msg float GetBottomMargin();
	afx_msg void SetBottomMargin(float fNewValue);
	ICxDataConnection* GetDataConnection();
	void SetDataConnection(ICxDataConnection* pDataConnection);
	ICxDataRecordset* GetDataRecordset();
	void SetDataRecordset(ICxDataRecordset* pDataRecordset);
	afx_msg int GetPageCount();
	afx_msg int GetPage();
	afx_msg void PrintOut();
	afx_msg void PrintPreview();
	//}}AFX_DISPATCH

	DECLARE_DISPATCH_MAP()
	
	BEGIN_INTERFACE_PART(ProvideClassInfo, IProvideClassInfo2)
		INIT_INTERFACE_PART(CFormObj, ProvideClassInfo)
		STDMETHOD(GetClassInfo)(LPTYPEINFO* ppTypeInfo);
		STDMETHOD(GetGUID)(DWORD dwGuidKind, GUID* pGUID);
	END_INTERFACE_PART_STATIC(ProvideClassInfo)
		
	DECLARE_INTERFACE_MAP()

	BOOL GetDispatchIID(IID* piid);

	friend class CReportObj;
	friend class CReportView;
};

#endif // __REPORTDOC_H__
