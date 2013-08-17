// WindowPrintView.h: interface for the CWindowPrintView class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __WINDOWPRINTVIEW_H__
#define __WINDOWPRINTVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWindowPrintView : public CPrintView  
{
protected: // create from serialization only
	CWindowPrintView();
	DECLARE_DYNCREATE(CWindowPrintView)

// CWindowPrintView
public:

// Operations
public:
		
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindowPrintView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWindowPrintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	
// Generated message map functions
protected:
	//{{AFX_MSG(CWindowPrintView)
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __WINDOWPRINTVIEW_H__
