// ReportView.h: interface for the CReportView class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __REPORTVIEW_H__
#define __REPORTVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CReportSection;

class CReportView : public CLayoutView  
{
protected: // create from serialization only
	CReportView();
	DECLARE_DYNCREATE(CReportView)

// CReportView
public:
	CReportDoc* GetDocument();

// Operations
public:
	CReportSection* HitTestSection(CPoint point);

	void SelectSection(CReportSection* pSection);
	virtual void Select(CDevObj* pObj, BOOL bAdd = FALSE, BOOL bUpdate = TRUE);

	BOOL HitTestLine(CPoint point1, CPoint point2, CPoint point, CPoint* pOffset = NULL);
	int HitTestSectionLine(CPoint point, CPoint* pOffset = NULL);
	void DrawTrackerLine(CDC* pDC, int nLine, CPoint point);
	CPoint MoveSectionLineTo(int nLine, CPoint point);

	virtual BOOL DoSpecialLButtonDown(UINT nFlags, const CPoint& point);
	virtual BOOL DoSpecialMouseMove(UINT nFlags, const CPoint& point);
		
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

protected:
	CReportSection* m_pSelectSection;
	
	
// Generated message map functions
protected:
	//{{AFX_MSG(CReportView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAddSubReport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CPropertyBar;	
	friend class CExpertBar;
};

#ifndef _DEBUG  // debug version in ReportView.cpp
inline CReportDoc* CReportView::GetDocument()
   { return (CReportDoc*)m_pDocument; }
#endif

#endif // __REPORTVIEW_H__
