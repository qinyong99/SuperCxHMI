#ifndef _INPLACEEDIT_H__
#define _INPLACEEDIT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InplaceEdit.h : header file
//

#define WM_INPLACEEDIT_KILLFOCUS  WM_USER + 300

/////////////////////////////////////////////////////////////////////////////
// CInplaceEdit window

class CInplaceEdit : public CEdit
{
// Construction
public:
	CInplaceEdit();

// Attributes
public:
	BOOL m_bCacel;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInplaceEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInplaceEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInplaceEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _INPLACEEDIT_H__
