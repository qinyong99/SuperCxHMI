// ReportView.h: interface for the CReportView class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __REPORTVIEW_H__
#define __REPORTVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CReportSection;

class CReportView : public CPrintView  
{
protected: // create from serialization only
	CReportView();
	DECLARE_DYNCREATE(CReportView)

// CReportView
public:
	CReportDoc* GetDocument();

// Operations
public:
		
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReportView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	
// Generated message map functions
protected:
	//{{AFX_MSG(CReportView)
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ReportView.cpp
inline CReportDoc* CReportView::GetDocument()
   { return (CReportDoc*)m_pDocument; }
#endif

#endif // __REPORTVIEW_H__
