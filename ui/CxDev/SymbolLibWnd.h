#if !defined(AFX_SymbolLibWND_H__A2CC7841_5A02_4C2A_81CB_99FA0A7DD0D6__INCLUDED_)
#define AFX_SymbolLibWND_H__A2CC7841_5A02_4C2A_81CB_99FA0A7DD0D6__INCLUDED_

////////////////////////////////////////////////////////////////////
// SymbolLibWnd.h : header file

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDevObj;
class CSymbolLibDoc;

/////////////////////////////////////////////////////////////////////////////
// CSymbolLibWnd window

class CSymbolLibWnd : public CBarListCtrl
{
// Construction
public:
	CSymbolLibWnd();

// Attributes
public:

protected:
// Operations
//	void AlignToGrid(CPoint& point);
	
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolLibWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL DoPaste();
	BOOL CanAddSymbol();
	void OnBackToRoot();
	virtual ~CSymbolLibWnd();

	void LibDelete(BOOL bPrompt = TRUE);

	// Generated message map functions
protected:
	void OnUpdate();
	afx_msg void OnGetDispInfo( NMHDR * pNotifyStruct, LRESULT * result );
	//{{AFX_MSG(CSymbolLibWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnItemdblclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListKeyDown(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLibNew();
	afx_msg void OnLibRename();
	afx_msg void OnLibDelete();
	afx_msg void OnLibOpen();
	afx_msg void OnLibProperty();
	afx_msg void OnLibCopy();
	afx_msg void OnLibCut();
	afx_msg void OnLibPaste();
	//}}AFX_MSG
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
protected:
	void EnsureObjectVisible(CDevObj* pObj);
	BOOL CreateSymbolObject(CSymbolLibDoc* pDoc, CArchive& ar, IStoragePtr& pStorage);
	void UpdateImage(CDevObj* pObj);
	virtual BOOL PreCreateImage(CImageList *pImageLarge, CImageList *pImageSmall);
	BOOL GetSelectedObjectData(LPSTGMEDIUM lpStgMedium);
	void GetDragInfoData(LPSTGMEDIUM lpStgMedium);
	COleDataSource* PrepareDataSource();
	virtual DROPEFFECT FindDragEffect(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL DoDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point);
	void DoCommand(int nButton);
	CSymbolLibDoc* m_pCurrentDocument;	//ø‚Œƒµµ÷∏’Î
	CImageList m_ImageContentLarge;
	CImageList m_ImageContentSmall;
	BOOL m_isDragBeginer;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SymbolLibWND_H__A2CC7841_5A02_4C2A_81CB_99FA0A7DD0D6__INCLUDED_)
