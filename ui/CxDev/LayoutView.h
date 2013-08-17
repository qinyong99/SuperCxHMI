// LayoutView.h : interface of the CLayoutView class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __LAYOUTVIEW_H__
#define __LAYOUTVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDevObj;

class CLayoutView : public CScrollView
{
protected: // create from serialization only
	CLayoutView();
	DECLARE_DYNCREATE(CLayoutView)

// Attributes
public:
	CFormDoc* GetDocument();
	BOOL m_bIsMainFormView;
	BOOL m_bIsFormView;
	
	CDevObjList m_selection;
	CDevObj* m_pFocusObj; //焦点对象，与m_pObjWithFocus不一样

	BOOL m_bActive; // is the view active?
	BOOL m_bDestoy;

	// zoom factor
	CSize m_zoomNum;
	CSize m_zoomDenom;

	// 拖放
	static CLIPFORMAT m_cfItemData;
	static CLIPFORMAT m_cfDragInfo;

	// 拖放信息
	CRect m_rcDragRect;
	CPoint m_ptDragStart;
	int m_nDragShapePointSize;
	CPoint* m_pDragShapePoints;
	BYTE* m_pDragShapePointTypes;

	static BOOL m_bCanDropEffectCopy; 
	BOOL m_bInDrag;
	COleDropTarget m_dropTarget;
	CPoint m_ptDragPoint;
	DROPEFFECT m_prevDropEffect;
	
	CRect m_rectMargin; // 打印边界

	// Operations
public:
	CDevObj* m_pObjWithFocus;
	
	void OnFocus(CDevObj* pItem, BOOL tGotFocus);
	HRESULT OnWindowlessReleaseFocus();
	HRESULT OnWindowlessSetFocus( CDevObj* pItem);
	CDevObj* GetObjectWithFocus() const;
	
	void EnsureObjectVisible(CDevObj* pObj);

	void DrawDragShape(CDC* pDC = NULL);

	//得到所选择项目的数据，用于拷贝和拖放
	COleDataSource* GetClipboardData(LPPOINT lpStart, BOOL bDrag = FALSE);
	void GetSelectedObjectData(LPSTGMEDIUM lpStgMedium);
	
	// 从数据源读取拖放信息
	BOOL GetDragInfo(COleDataObject* pDataObject, BOOL bShape = TRUE);

	BOOL DoPasteObject(COleDataObject* pDataObject, CPoint* pPoint);
	BOOL DoPasteSubmodule(COleDataObject* pDataObject, CPoint* pPoint);

	CRect GetSelectionRect(BOOL bBound = FALSE);
	void SetScrollInfo();
	BOOL SetZoomFactor(CSize zoomNum, CSize zoomDenom);
	void DocToClient(CRect& rect);
	void DocToClient(CPoint& point);
	void DocToClient(CSize& size);
	void ClientToDoc(CPoint& point);
	void ClientToDoc(CRect& rect);
	void ClientToDoc(CSize& size);

	BOOL CanSelect(CDevObj* pObj);
	virtual void Select(CDevObj* pObj, BOOL bAdd = FALSE, BOOL bUpdate = TRUE);
	virtual void MutiSelect(CDevObjList* pObjList, BOOL bAdd = FALSE);
	void SelectWithinRect(CRect rect, BOOL bAdd = FALSE);
	virtual void UpdateFocusObject();

	void DrawGrid(CDC* pDC, CRect* pRect);
	void AlignToGrid(CPoint& point);
	void GetGridSpace(CSize& size);

	void DrawTracker(CDC* pDC);
	void UpdateWindow();

	void PrepareRopDC(CDC* pDC, BOOL bPrepare = TRUE);

	virtual BOOL DoSpecialLButtonDown(UINT nFlags, const CPoint& point) { return FALSE; }
	virtual BOOL DoSpecialMouseMove(UINT nFlags, const CPoint& point) { return FALSE; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayoutView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	CDevObj* GetObjectAt(POINT pt);
	void InvalidObject(CDevObj* pObj);
	
	virtual ~CLayoutView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 绘制 XP 风格标题栏
#ifndef _UXTHEME_H_
	typedef HANDLE HTHEME;
#endif //!_UXTHEME_H_

	typedef HTHEME (STDAPICALLTYPE *PFN_OpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
	typedef HRESULT (STDAPICALLTYPE *PFN_CloseThemeData)(HTHEME hTheme);
	typedef HRESULT (STDAPICALLTYPE *PFN_DrawThemeBackground)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, OPTIONAL const RECT *pClipRect);

	HMODULE m_hThemeDLL;
	HTHEME m_hTheme;
	PFN_DrawThemeBackground m_pfnDrawThemeBackground;

	static UINT _GetThemeChangedMsg();
	void OpenThemeData();
	void CloseThemeData();
	
// Generated message map functions
protected:
	CDevObj* FindObjBackward(CDevObj* pObj);
	CDevObj* FindObjForward(CDevObj* pObj);
	//{{AFX_MSG(CLayoutView)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCancelEdit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnViewZoomCustom();
	afx_msg void OnObjectMoveback();
	afx_msg void OnObjectMoveforward();
	afx_msg void OnSpaceEvenhoriz();
	afx_msg void OnUpdateSpaceEvenhoriz(CCmdUI* pCmdUI);
	afx_msg void OnSpaceEvenvert();
	afx_msg void OnUpdateSpaceEvenvert(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEditProperties();
	afx_msg void OnGroupObject();
	afx_msg void OnUngroupObject();
	afx_msg void OnInsertBitmap();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateGroupObject(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUngroupObject(CCmdUI* pCmdUI);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcPaint();
	afx_msg void OnEditConvertToBmp();
	afx_msg void OnUpdateEditConverttobmp(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditScript();
	afx_msg void OnAddToSymbolLib();
	afx_msg void OnAddToModuleLib();
	afx_msg void OnLayerSetup();
	afx_msg void OnObjectForwardMost();
	afx_msg void OnUpdateObjectForwardMost(CCmdUI* pCmdUI);
	afx_msg void OnObjectBackwordMost();
	afx_msg void OnUpdateObjectBackwordMost(CCmdUI* pCmdUI);
	afx_msg void OnArrageSamehight();
	afx_msg void OnUpdateAllSelUpTwo(CCmdUI* pCmdUI);
	afx_msg void OnArrageSamewidth();
	afx_msg void OnArrageBoth();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditClone();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDesignSymbol();
	afx_msg void OnCreateSymbol();
	afx_msg void OnUpdateDesignSymbol(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCreateSymbol(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddtoSymbol(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddtoModule(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditSubform();
	afx_msg void OnUpdateEditSubform(CCmdUI* pCmdUI);
	afx_msg void OnAddSubmodule();
	afx_msg void OnCreateSubform();
	afx_msg void OnUpdateCreateSubform(CCmdUI* pCmdUI);
	afx_msg void OnBreakSubform();
	afx_msg void OnUpdateBreakSubform(CCmdUI* pCmdUI);
	afx_msg void OnBreakSymbol();
	afx_msg void OnUpdateBreakSymbol(CCmdUI* pCmdUI);
	afx_msg void OnHelpIndex();
	afx_msg void OnPageSetup();
	afx_msg void OnTabOrder();
	//}}AFX_MSG
	afx_msg void OnTracker(UINT nID);
	afx_msg void OnUpdateTracker(CCmdUI* pCmdUI);
	afx_msg void OnAlign(UINT nID);
	afx_msg void OnCenter(UINT nID);
	afx_msg void OnRotate(UINT nID);
	afx_msg void OnMirror(UINT nID);
	afx_msg void OnUpdateAlign(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCenter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRotateOrMirror(CCmdUI* pCmdUI);
	afx_msg void OnZoom(UINT nID);
	afx_msg void OnUpdateZoom(CCmdUI* pCmdUI);
	afx_msg void OnLayer(UINT nID);
	afx_msg void OnUpdateLayer(CCmdUI* pCmdUI);
	afx_msg void OnGridSettingChanged();
	afx_msg LRESULT OnThemeChanged(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	friend class CDevObj;
	friend class CDevTool;
	friend class CSelectTool;
	friend class CPropertyBar;
	friend class CMiddleWnd;
};

#ifndef _DEBUG  // debug version in LayoutView.cpp
inline CFormDoc* CLayoutView::GetDocument()
   { return (CFormDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __LAYOUTVIEW_H__
