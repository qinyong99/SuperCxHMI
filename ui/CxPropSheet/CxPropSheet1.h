#if !defined(AFX_CXPROPSHEET1_H__517A1265_F1B7_4A83_AE16_67B8DFA77459__INCLUDED_)
#define AFX_CXPROPSHEET1_H__517A1265_F1B7_4A83_AE16_67B8DFA77459__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CxPropSheet1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCxPropSheet

class CCxPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CCxPropSheet)

// Construction
public:
	virtual ~CCxPropSheet();
	CCxPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCxPropSheet)
	public:
	virtual BOOL Create(CWnd* pParentWnd=NULL, DWORD dwStyle=(DWORD)(-1), DWORD dwExStyle=0);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL AddPageCLSID(ULONG  count, LPCLSID pArray);
	BOOL AddObjects(ULONG count, LPUNKNOWN* pArray);

	// Generated message map functions
protected:
	void ShowButtons();
	void MoveButton(UINT nID, int x);
	//{{AFX_MSG(CCxPropSheet)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	LPUNKNOWN* m_aryUnk;	//有属性页的OLE对象的Unknown指针
	int m_nObjCounts;	//对象的个数
	CPtrArray m_pageArray;//属性页集
	UINT m_iPageCnt;	//属性页个数
	CLSID* m_ClsIDs;
	CImageList m_imageTab;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CXPROPSHEET1_H__517A1265_F1B7_4A83_AE16_67B8DFA77459__INCLUDED_)
