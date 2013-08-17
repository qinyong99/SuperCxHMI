/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Juraj Rojko jrojko@twist.cz
// All rights reserved
//
// VBScriptEditorView.h : interface of the CScriptView class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __SCRIPTVIEW_H__
#define __SCRIPTVIEW_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

_COM_SMARTPTR_TYPEDEF(ICxScriptEditor, _uuidof(ICxScriptEditor));
_COM_SMARTPTR_TYPEDEF(ICxRangeToFormat, _uuidof(ICxRangeToFormat));

class CFormDoc;

#define UPDATE_SYMBOL_FORM 1
#define UPDATE_SYMBOL_OBJECT 2
#define UPDATE_SYMBOL_RECORDSET 4

#define WM_UPDATE_SYMBOL WM_USER + 300

class CScriptView : public CFormView
{
protected: // create from serialization only
	CScriptView();
	DECLARE_DYNCREATE(CScriptView)
	DECLARE_EVENTSINK_MAP()

// Attributes
public:
	CFormDoc* GetDocument();

	ICxScriptEditorPtr m_pScriptEditor;
	ICxRangeToFormatPtr m_pRangeToFormat;

	CRect m_rectMargin;
	CSize m_sizePaper;
	CDWordArray m_aPageStart;    // array of starting pages

	static HWND c_hFindDialog;

// Operations
public:
	void GetText(CString& rString);
	void SetText(LPCTSTR lpszText);
	long GetTextLength() const;
	void EmptyUndoBuffer();
	void SetSavePoint();
	void SetSelectionStart(int nPos);
	void SetSelectionEnd(int nPos);
	int GetSelectionStart();
	int GetSelectionEnd();
	void ReplaceSel(LPCTSTR lpszText);
	void AppendText(int nLength, LPCTSTR lpszText);
	void GotoLine(int nLine);
	int GetLineCount();
	CString GetSelText() const;
	BOOL GetModify();
	BOOL CanPaste();
	int GetCurrentPos();
	int LineFromPosition(int nPos);
	int GetColumn(int nPos);
	BOOL HasSearched();
	void BeginUndoAction();
	void EndUndoAction();
	void EditObjectEventHandler(LPCTSTR lpszObject, LPCTSTR lpszEvent = NULL, BOOL bCreateAndEdit = TRUE);
	void DeleteEmptyEventHandler();
	BOOL IsSymbolViewVisible();
	void ShowSymbolView(BOOL b);
	void AddGeneralSymbolPage(DWORD dwSymbolMask);
	void AddOPCServerSymbolPage();
	BOOL AddSymbolTreePage(LPCTSTR lpszCaption, ICxSymbolTreePage** ppPage);
	BOOL GetCurrentSymbolTreePage(ICxSymbolTreePage** ppPage);

	long PrintPage(CDC* pDC, long nIndexStart, long nIndexStop);
	BOOL PaginateTo(CDC* pDC, CPrintInfo* pInfo);
	long FormatRange(BOOL bDraw, BOOL bUseStruct = FALSE);
	
	void SetPaperSize(CSize sizePaper);
	CSize GetPaperSize() const;
	void SetMargins(const CRect& rectMargin);
	CRect GetMargins() const;
	int GetPrintWidth() const;
	CRect GetPrintRect() const;
	CRect GetPageRect() const;

	static BOOL PreTranslateAppMessage(MSG* pMsg);
	
protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL
	BOOL OnPreparePrinting(CPrintInfo* pInfo);

	virtual void OnPrinterChanged(const CDC& dcPrinter);
	
// Implementation
public:
	CWnd m_wndScriptEidor;
	virtual ~CScriptView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void OnSetModified();
	void OnSymbolViewToggled();
	void OnSymbolPageChanged();

	void UpdateSymbolPage(DWORD dwUpdateMask);

// Generated message map functions
protected:

	//{{AFX_MSG(CScriptView)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);
	afx_msg void OnUpdateCursor(CCmdUI *pCmdUI);
	afx_msg void OnHelpIndex();
	afx_msg void OnFilePrint();
	afx_msg void OnPageSetup();
	afx_msg void OnUpdateNeedSel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedClip(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedText(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedFind(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditClear();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditFind();
	afx_msg void OnEditReplace();
	afx_msg void OnEditRepeat();
	afx_msg void OnEditGoLine();
	afx_msg void OnBookmarkToggle();
	afx_msg void OnBookmarkNext();
	afx_msg void OnBookmarkPrevious();
	afx_msg void OnBookmarkClearAll();
	afx_msg void OnDevModeChange(LPTSTR lpDeviceName);
	afx_msg void OnUpdateCurrentSymbolPage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BEGIN_INTERFACE_PART(ScriptEditorSite, ICxScriptEditorSite)
		STDMETHOD(GetObjectNames)(LPENUMSTRING* ppEnumObjects);
		STDMETHOD(GetObject)(LPCWSTR lpszObject, REFIID iid, void **ppObject);
		STDMETHOD(GetObjectTypeInfo)(LPCWSTR lpszObject, ITypeInfo** ppTinfo);
		STDMETHOD(GetObjectSourceTypeInfo)(LPCWSTR lpszObject, ITypeInfo** ppTinfo);
		STDMETHOD(GetObjectEvents)(LPCWSTR lpszObject, LPENUMSTRING* ppEnumEvents);
		STDMETHOD(GetObjectEventParams)(LPCWSTR lpszObject, LPCWSTR lpszEvent, LPENUMSTRING* ppEnumParams);
		STDMETHOD(GetObjectMembers)(LPCWSTR lpszObject, LPENUMSTRING* ppEnumProperties, LPENUMSTRING* ppEnumMethods);
		STDMETHOD(GetObjectMemberParams)(LPCWSTR lpszObject, LPENUMSTRING* ppEnumParams);
		STDMETHOD(GetGlobalTypeInfoCount)(UINT* pnInfoCount);
		STDMETHOD(GetGlobalTypeInfo)(UINT iTypeInfo, ITypeInfo** ppTinfo);
		STDMETHOD(ShowHelpString)(LPCWSTR lpszObject);
	END_INTERFACE_PART(ScriptEditorSite)

	DECLARE_INTERFACE_MAP()

	BOOL m_bSetModified;

friend class CChildFrame;
};

#ifndef _DEBUG  // debug version in VBScriptEditorView.cpp
inline CFormDoc* CScriptView::GetDocument()
   { return (CFormDoc*)m_pDocument; }
#endif

inline int CScriptView::GetPrintWidth() const
	{ return m_sizePaper.cx - m_rectMargin.left - m_rectMargin.right;}
inline CRect CScriptView::GetPrintRect() const
	{ return CRect(m_rectMargin.left, m_rectMargin.top, m_sizePaper.cx - m_rectMargin.right, m_sizePaper.cy - m_rectMargin.bottom); }
inline CRect CScriptView::GetPageRect() const
	{ return CRect(CPoint(0,0), m_sizePaper); }
inline void CScriptView::SetPaperSize(CSize sizePaper)
	{ m_sizePaper = sizePaper; }
inline CSize CScriptView::GetPaperSize() const
	{ return m_sizePaper; }
inline void CScriptView::SetMargins(const CRect& rectMargin)
	{ m_rectMargin = rectMargin; }
inline CRect CScriptView::GetMargins() const
	{ return m_rectMargin; }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VBSCRIPTEDITORVIEW_H__EDDE6D51_C472_11D1_8635_00403395B157__INCLUDED_)
