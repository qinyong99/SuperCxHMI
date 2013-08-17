// BitmapView.h: interface for the CBitmapView class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __BITMAPVIEW_H__
#define __BITMAPVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBitmapDoc;

class CBitmapView : public CPrintView  
{
protected: // create from serialization only
	CBitmapView();
	DECLARE_DYNCREATE(CBitmapView)

// CBitmapView
public:
	CBitmapDoc* GetDocument();

// Operations
public:
		
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBitmapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	
// Generated message map functions
protected:
	//{{AFX_MSG(CBitmapView)
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ReportView.cpp
inline CBitmapDoc* CBitmapView::GetDocument()
   { return (CBitmapDoc*)m_pDocument; }
#endif

#endif // __BITMAPVIEW_H__
