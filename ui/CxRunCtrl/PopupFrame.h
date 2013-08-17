#if !defined(AFX_POPUPFRAME_H__DA755618_7C19_4EF4_8E98_3D04DCA3F2DE__INCLUDED_)
#define AFX_POPUPFRAME_H__DA755618_7C19_4EF4_8E98_3D04DCA3F2DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopupFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPopupFrame frame

class CFormDoc;

#define WM_CLOSE_FORM WM_USER + 101

class CPopupFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CPopupFrame)
public:
	CPopupFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	int m_x;
	int m_y;
	int m_cx;
	int m_cy;
	BOOL m_bCaption;
	BOOL m_bThickframe;
	BOOL m_bControlBox;
	BOOL m_bTopMost;
	CString m_strCaption;

	BOOL m_bDisableModal;

	IDrawMenu* m_pMenu;
	CArray<ACCEL, ACCEL&> m_arrAccels;
	HACCEL m_hAccelTable;

// Operations
public:
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
		
	void AdjustBtnSize(int cyHeight);
	void CalcBtnRects(int cxWidth, int cyHeight, RECT arrRect[3], bool bInvertX = false);
	void DrawButtons(CDC* pDC, LPRECT pRects, int nBtn);
	int GetButtonState(int nBtn);

	void AddMenuItems(HMENU hMenu, IDrawMenu* pMenu, int& nIndex, int nCount, int nDepth);

	void GetSystemSettings();

	BOOL StartTrackMouseHover();

	BOOL m_bMouseTracking;

	int m_nBtnPressed;
	int m_nBtnWasPressed;
	int m_nBtnHover;

	BOOL m_bActive;

	int m_cxyOffset;      // offset between nonclient elements
	int m_cxBtnWidth;     // nonclient button width
	int m_cyBtnHeight;    // nonclient button height
	int m_cxFrameWidth;
	int m_cyFrameHeight;

//	BOOL m_bLBtnDown;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupFrame)
	public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPopupFrame();

	// Generated message map functions
	//{{AFX_MSG(CPopupFrame)
	afx_msg void OnClose();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnNcMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnNcPaint();
	afx_msg LRESULT OnDisableModal(WPARAM, LPARAM);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnThemeChanged(WPARAM, LPARAM);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnCloseForm(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPFRAME_H__DA755618_7C19_4EF4_8E98_3D04DCA3F2DE__INCLUDED_)
