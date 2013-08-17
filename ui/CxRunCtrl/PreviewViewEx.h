// PreviewViewEx.h: interface for the CPreviewViewEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREVIEWVIEWEX_H__F8252410_8636_4B00_BFB1_DC27C47C3BAB__INCLUDED_)
#define AFX_PREVIEWVIEWEX_H__F8252410_8636_4B00_BFB1_DC27C47C3BAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPreviewViewEx : public CPreviewView
{
protected:
	CPreviewViewEx();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPreviewViewEx)
		
		// Attributes
public:
	
	// Operations
public:
	virtual void OnDisplayPageNumber(UINT nPage, UINT nPagesDisplayed);
		
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewViewEx)
	protected:
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	virtual ~CPreviewViewEx();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	virtual void OnActivateView(BOOL bActivate,
		CView* pActivateView, CView* pDeactiveView);
	
	// Generated message map functions
	//{{AFX_MSG(CPreviewViewEx)
	afx_msg void OnPreviewPrint();
	afx_msg void OnPreviewClose();
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	
	friend class CPrintView;
	friend class CPrintFrame;
	friend BOOL CALLBACK _AfxPreviewCloseProc2(CFrameWnd* pFrameWnd);
};

#endif // !defined(AFX_PREVIEWVIEWEX_H__F8252410_8636_4B00_BFB1_DC27C47C3BAB__INCLUDED_)
