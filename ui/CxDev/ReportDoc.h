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
		virtual CMultiDocTemplate* GetLayoutTemplate() { return theApp.m_pReportTemplate; }
	};

	virtual CString GetClassName() { return _T("Report"); }

	ICxDataConnectionPtr m_pDataConnection;
	ICxDataRecordsetPtr m_pDataRecordset;

protected:
	CRectF m_rcMargins; // MSDN: Article ID: Q105444 

	CReportSection m_secReportHeader;
	CReportSection m_secPageHeader;
	CReportSection m_secDetail;
	CReportSection m_secPageFooter;
	CReportSection m_secReportFooter;
	
// Operations
public:
	void SetLandscapeWidth();

	void ReCalcReportHeight(BOOL bUpdate = FALSE);

	void DrawDoc(CDC* pDC);
	void DrawOutline(CDC* pDC, int nDrawState = SECTION_DRAW_ALL);

	void DrawReportHeader(CDC* pDC, LPRECT pRect);
	void DrawPageHeader(CDC* pDC, LPRECT pRect);
	void DrawDetail(CDC* pDC, LPRECT pRect);
	void DrawPageFooter(CDC* pDC, LPRECT pRect);
	void DrawReportFooter(CDC* pDC, LPRECT pRect);

	CDevObj* AddDataLink(LPCTSTR lpszSource, CPointF point, LPCTSTR lpszCaption = NULL, enumTextAlign nAlign = TextAlignLeft, int nFontSize = 11, enumFontStyle nFontStyle = FontStyleRegular);
	CDevObj* AddDigitalDataLink(LPCTSTR lpszSource, CPointF point, int nDigital, enumTextAlign nAlign = TextAlignLeft, int nFontSize = 11, enumFontStyle nFontStyle = FontStyleRegular);
	CDevObj* AddFieldDataLink(LPCTSTR lpszField, CPointF point, float fWidth, enumTextAlign nAlign = TextAlignLeft, int nFontSize = 11, enumFontStyle nFontStyle = FontStyleRegular);
	CDevObj* AddLabel(LPCTSTR lpszText, CPointF point, float fWidth, enumTextAlign nAlign = TextAlignLeft, int nFontSize = 11, enumFontStyle nFontStyle = FontStyleRegular);
	CDevObj* AddLine(float fX1, float fY1, float fX2, float fY2, float fWidth, COLORREF crColor = RGB(0, 0, 0));
	CDevObj* AddRectangle(float fX1, float fY1, float fX2, float fY2, float fWidth, COLORREF crColor = RGB(0, 0, 0));
	
	CRectF GetSectionRect(CReportSection* pSection);

	void DrawBand(CDC* pDC, LPRECT pRect, LPCTSTR lpszCaption);

	virtual BOOL IsValidObjectName(LPCTSTR lpszName);
	BOOL IsValidSectionName(LPCTSTR lpszName);


// Implementation
public:
	virtual ~CReportDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif	

	virtual HRESULT InitAmbientProperties();

	virtual void InitializeProperty();
	virtual void SerializeProperty(CArchive& ar);

	virtual void OnUpateTitle();

	virtual LPUNKNOWN GetObjectUnknown();


// Generated message map functions
protected:
	//{{AFX_MSG(CReportDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	virtual HRESULT MapPropertyToCategory(DISPID dispid, PROPCAT* ppropcat);
	virtual HRESULT GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName);
	
	DECLARE_OLETYPELIB(CReportDoc)
		
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
	friend class CPropertyBar;
	friend class CDevDoc;
	friend class CMiddleWnd;
};

#endif // __REPORTDOC_H__
