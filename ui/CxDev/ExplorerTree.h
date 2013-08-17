#ifndef __EXPLORERTREE_H__
#define __EXPLORERTREE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExplorerTree.h : header file
//

//类用于获取OLE拖放消息

template <class T>
class CThisDropTarget : public COleDropTarget
{
public:
	virtual void OnDragLeave(CWnd* pWnd)
	{
		T* pT = static_cast<T*>(pWnd);
		pT->OnDragLeave();
	}
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
	{
		T* pT = static_cast<T*>(pWnd);
		return pT->OnDrop(pDataObject, dropEffect, point);
	}
	virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point)
	{
		T* pT = static_cast<T*>(pWnd);
		return pT->OnDropEx(pDataObject, dropDefault, dropList, point);
	}
	virtual DROPEFFECT OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
	{
		T* pT = static_cast<T*>(pWnd);
		return pT->OnDragScroll(dwKeyState, point);
	}
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
	{
		T* pT = static_cast<T*>(pWnd);
		return pT->OnDragOver(pDataObject, dwKeyState, point);
	}
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
	{
		T* pT = static_cast<T*>(pWnd);
		return pT->OnDragEnter(pDataObject, dwKeyState, point);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CExplorerTree window

class CDirWatcher;

class CExplorerTree : public CTreeCtrl
{
// Construction
public:
	CExplorerTree();

// Attributes
public:
	enum ImageIDS
	{
		IID_PROJECT = 0,
		IID_PICTURES,
		IID_PICTURE,
		IID_MAINFORM,
		IID_FORM,
		IID_SUBFORM,
		IID_REPORT,
		IID_SUBREPORT,
		IID_CODEMODULE,
		
	};

	CImageList m_ImageList;
	HTREEITEM m_htiSelect;
	BOOL m_bDownSelectItem;
	BOOL m_bNoSelectedChganded;

// Operations
public:
	void InitExplorerTree();
	void PopulateTree();

	void RefreshPictureItems();
	HTREEITEM FindPictureItem(LPCTSTR szName);
	HTREEITEM FindPictureSubItem(LPCTSTR szName, LPCTSTR szSubName);
	HTREEITEM FindPictureSubItem(HTREEITEM hItem, DWORD dwData);
	HTREEITEM FindPictureSubItem(LPCTSTR szName, DWORD dwData);
	void RefreshPictureItem(HTREEITEM hPicture, CDevDoc* pDoc = NULL);
	void InsertFormDocItem(CFormDoc* pDoc, HTREEITEM hItem);
	CDevDoc* GetPictureItemDocument(HTREEITEM hPicture);
	CDevDoc::CSubDoc* GetPictureSubItemDocument(HTREEITEM hPictureSubItem, CDevDoc*& pDoc);

	HTREEITEM FindChildItemByData(HTREEITEM hItem, DWORD dwData);
	void GetExpandItemsData(HTREEITEM hItem, CArray<DWORD, DWORD&>& arrData);

// Overrides

	// OLE drag/drop support
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual BOOL OnDrop(COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDropEx(COleDataObject* pDataObject,
		DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point);
	virtual DROPEFFECT OnDragScroll(DWORD dwKeyState, CPoint point);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExplorerTree)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExplorerTree();

	void OnEditDelete(BOOL bPrompt = TRUE);
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CExplorerTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddForm();
	afx_msg void OnAddBlankReport();
	afx_msg void OnAddReportWizard();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditClear();
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddCode();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LONG OnDocumentChangedMsg(DWORD dwNotify, DWORD);
	afx_msg LONG OnDocumentActivateMsg(DWORD dwNotify, DWORD);
	afx_msg LONG OnWatchNotifyMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	CString MainFileNameFromFullPath(CString strPath);
	HTREEITEM GetPictureRoot();
	CDocument* ExpandPictureItem(HTREEITEM hPicture);

	CThisDropTarget<CExplorerTree> m_dropTarget;
	HTREEITEM m_htiDrag; //开始拖放的结点

	CDocument* GetDocument(HTREEITEM hItem, DWORD& formID);

	BOOL IsPictureExist(LPCTSTR szName);

	CDirWatcher* m_pDirWatcher;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !__EXPLORERTREE_H__
