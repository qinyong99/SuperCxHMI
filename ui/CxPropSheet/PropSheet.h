#if !defined(AFX_PROPSHEET_H__E7FFE618_E196_4F89_95EE_C57DE56C6A99__INCLUDED_)
#define AFX_PROPSHEET_H__E7FFE618_E196_4F89_95EE_C57DE56C6A99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropSheet.h : header file
// 所属项目: AutoCx的CxPropSheet工程
// 创建日期: 2001.10.8
// 创 建 人：孙越洋 
// 功能描述：属性页安放主窗口类,用于创建和管理属性页。 
// 修改记录：
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CPropSheet

class CPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropSheet)

// Construction
public:
	CPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropSheet)
	public:
	virtual int DoModal();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void EnableApplyButton(BOOL bEnable = TRUE);
	void OnChangeIcon();
	virtual BOOL Create(CWnd* pParentWnd=NULL, DWORD dwStyle=(DWORD)(-1), DWORD dwExStyle=0);
	BOOL AddPageCLSID(ULONG  count, LPCLSID pArray);
	BOOL AddObjects(ULONG count, LPUNKNOWN* pArray);
	virtual ~CPropSheet();

	// Generated message map functions
protected:
	void InitTabImage();
	void ShowButtons();
	void MoveButton(UINT nID, int x);
	//{{AFX_MSG(CPropSheet)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

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

#endif // !defined(AFX_PROPSHEET_H__E7FFE618_E196_4F89_95EE_C57DE56C6A99__INCLUDED_)
