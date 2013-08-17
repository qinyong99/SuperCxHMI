#if !defined(AFX_PRINTVIEW_H__F81E9415_5F8A_4BB8_BDE8_866F2022F8ED__INCLUDED_)
#define AFX_PRINTVIEW_H__F81E9415_5F8A_4BB8_BDE8_866F2022F8ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintView view

class CRunCtrl;

class CPrintView : public CView
{
protected:
	CPrintView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPrintView)

// Attributes
public:
	CRunView* m_pView;
	CRunCtrl* m_pRunCtrl;

	BOOL m_bPrinting;
// Operations
public:

	void OnFilePrintPreview(CRunCtrl* pRunCtrl, CRunView* pView = NULL);
	
	
	void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo,
		POINT, CPreviewView* pView);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPrintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPrintView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTVIEW_H__F81E9415_5F8A_4BB8_BDE8_866F2022F8ED__INCLUDED_)
