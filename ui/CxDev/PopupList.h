#ifndef _POPUPLIST_H__
#define _POPUPLIST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopupList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPopupList window

class CPopupList : public CListBox
{
// Construction
public:
	CPopupList();

// Attributes
public:
	BOOL m_bClose;
	BOOL m_bOked;

// Operations
public:
	void Close(BOOL bOked);
	void AdjustPosition(RECT rect);

	BOOL Create(CWnd* pParentWnd, CFont* pFont);
	int Show(int x, int y, int cx, int nSel);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPopupList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPopupList)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _POPUPLIST_H__
