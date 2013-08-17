// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __MAINFRM_H__
#define __MAINFRM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDevObj;

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	BOOL m_bFullScreenMode;
	CRect m_FullScreenRect;
	WINDOWPLACEMENT m_OldWndPlacement;
	CToolBar* m_pwndFullScreenBar;

// Operations
public:
	void FullScreenModeOn();
	void FullScreenModeOff();
	void ViewCode(CDevObj* pObj, CView* pView, LPCTSTR lpszEvent = NULL);
	CMDIChildWnd* CreateOrActivateFrame(CDocTemplate* pTemplate,
		CRuntimeClass* pViewClass);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	BOOL CreateToolBar();
	BOOL CreateToolBox();
//	BOOL CreateFormatBar();
	BOOL CreateLayoutBar();
	BOOL CreateStatusBar();
	BOOL CreateExplorerBar();
	BOOL CreatePropertyBar();

	void FirstPosition();


public:  // control bar embedded members
	void OnViewMousePosChange(CPoint pt);
	CWnd* GetToolModule();
	CWnd* GetToolSymbol();
	BOOL CreateExpertBar();
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndLayoutBar;
	CToolboxBar m_wndToolBox;
	CPropertyBar m_wndPropertyBar;
	CExplorerBar m_wndExplorerBar;

	CExpertBar m_wndAppearExpert; 
	CExpertBar m_wndDynamicExpert; 
	CExpertBar m_wndActionExpert; 


// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnWindowClose();
	afx_msg void OnWindowCloseall();
	afx_msg void OnWindowNext();
	afx_msg void OnWindowPrevious();
	afx_msg void OnUpdateWindow(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnViewExplorerBar();
	afx_msg void OnViewPropertyBar();
	afx_msg void OnViewToolbox();
	afx_msg void OnCharColor();
	afx_msg void OnViewFullScreen();
	afx_msg void OnUpdateViewFullScreen(CCmdUI* pCmdUI);
	afx_msg void OnViewObject();
	afx_msg void OnViewCode();
	afx_msg void OnClose();
	afx_msg void OnEndSession(BOOL bEnding);
	afx_msg void OnViewDynamicExpert();
	afx_msg void OnUpdateViewDynamicExpert(CCmdUI* pCmdUI);
	afx_msg void OnViewAppearExpert();
	afx_msg void OnUpdateViewAppearExpert(CCmdUI* pCmdUI);
	afx_msg void OnViewActionExpert();
	afx_msg void OnUpdateViewActionExpert(CCmdUI* pCmdUI);
	afx_msg void OnViewStand();
	afx_msg void OnUpdateViewStand(CCmdUI* pCmdUI);
	afx_msg void OnViewLayout();
	afx_msg void OnUpdateViewLayout(CCmdUI* pCmdUI);
	afx_msg void OnHelpContent();
	afx_msg void OnHelpIndex();
	afx_msg void OnHelpSearch();
	afx_msg void OnAddControl();
	afx_msg void OnGridSetting();
	afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnInsertControl(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdatePictrue(CCmdUI* pCmdUI);
	afx_msg void OnTool(UINT nID);
	afx_msg void OnUpdateTool(CCmdUI* pCmdUI);
	afx_msg LONG OnProjectOpendMsg(DWORD, LONG);
   	DECLARE_MESSAGE_MAP()

	friend class CDevDoc;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __MAINFRM_H__
