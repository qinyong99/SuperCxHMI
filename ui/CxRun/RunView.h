// RunView.h : interface of the CRunView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNVIEW_H__ADDD10CD_3F20_44C2_B8B4_019EE7ADFAC3__INCLUDED_)
#define AFX_RUNVIEW_H__ADDD10CD_3F20_44C2_B8B4_019EE7ADFAC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IRunCtrl;

interface IDrawMenu;

class CRunView : public CFormView
{
protected: // create from serialization only
	CRunView();
	DECLARE_DYNCREATE(CRunView)
	DECLARE_EVENTSINK_MAP()

public:
	void ReLayout();
		
// Attributes
public:
	CRunDoc* GetDocument();
	// m_pSelection holds the selection to the current CCxRunCntrItem.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not CCxRunCntrItem objects.  This selection
	//  mechanism is provided just to help you get started.

// Operations
public:
	CString GetPicture() const;
	void SetPicture(LPCSTR lpsz);
	CString GetServer() const;
	void SetServer(LPCSTR lpsz);
	BOOL GetCreateServerObjectAsLocal();
	void SetCreateServerObjectAsLocal(BOOL);
	BOOL GetLocalServerUseSACProxy();
	void SetLocalServerUseSACProxy(BOOL);
	BOOL GetZoomFitWindow();
	void SetZoomFitWindow(BOOL);
	BOOL GetScrollBarsEnaled();
	void SetScrollBarsEnaled(BOOL);
	void ClearPicturesCache();
	void SetHighlightMouseObject(boolean bHighlight);
	void SetDefaultUser(LPCSTR lpsz);
	void SetDefaultPassword(LPCSTR lpsz);
	LPDISPATCH GetMainMenu();


	void AddMenuItems(HMENU hMenu, IDrawMenu* pMenu, int& nIndex, int nCount, int nDepth);

	CArray<ACCEL, ACCEL&> m_arrAccels;
	HACCEL m_hAccelTable;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	// Events
	virtual void PictrueSize(long lWidth, long lHeight);
	virtual void OnQuit();
	virtual void ShowPictureComplete(LPCTSTR lpszPicture);
	virtual void PictureClose(LPCTSTR lpszPicture);

// Implementation
public:
	CWnd m_wndBrowser;
	virtual ~CRunView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
 	IRunCtrl* m_pBrowserApp;

	IDrawMenu* m_pMenu;
	CMenu m_menu;
	BOOL m_bLockMenu;
	HMENU m_hAltMenu;

	CSize m_szPicture;

// Generated message map functions
protected:
	//{{AFX_MSG(CRunView)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RunView.cpp
inline CRunDoc* CRunView::GetDocument()
   { return (CRunDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNVIEW_H__ADDD10CD_3F20_44C2_B8B4_019EE7ADFAC3__INCLUDED_)
