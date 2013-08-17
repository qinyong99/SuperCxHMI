#if !defined(AFX_CHECKLISTCTRL_H__1C8ADBC7_E4FB_44F2_97FD_F2CACB512AAF__INCLUDED_)
#define AFX_CHECKLISTCTRL_H__1C8ADBC7_E4FB_44F2_97FD_F2CACB512AAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckListCtrl window

class CCheckListCtrl : public CListCtrl
{
// Construction
public:
	CCheckListCtrl();

// Attributes
public:

// Operations
public:
	BOOL GetCheckState(int nIndex);
	BOOL SetCheckState(int nItem, BOOL bCheck);
	void CheckSelectedItems(int nCurrItem);

	void SetCheckStyle();
	// dwExMask = 0 means all styles
	DWORD SetExtendedListViewStyle(DWORD dwExStyle, DWORD dwExMask = 0);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCheckListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCheckListCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKLISTCTRL_H__1C8ADBC7_E4FB_44F2_97FD_F2CACB512AAF__INCLUDED_)
