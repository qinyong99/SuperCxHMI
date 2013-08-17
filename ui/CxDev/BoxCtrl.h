#ifndef __BOXCTRL_H__
#define __BOXCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BoxCtrl.h : header file
//

#define WM_INSERTCONTROL		WM_USER + 200

/////////////////////////////////////////////////////////////////////////////
// CBoxCtrl window

class CBoxCtrl : public CWnd
{
	DECLARE_DYNCREATE(CBoxCtrl)
// Construction
public:
	CBoxCtrl();

// Attributes
public:
	enum { HT_Nothing = -1, HT_Folder, HT_Item, HT_FolderClient};

private:
	CBrush m_brushDither;
	
	int	m_nFolderHeight;	//组标题高度
	int	m_nSelFolder;		//当时选中(打开状态)的组索引

	int	m_nSelItem;			//当时选中的项索引
	int m_nRSelItem;			//右键时击中的项索引

	int m_nLastFolderHighlighted;
	int m_nLastItemHighlighted;

	BOOL m_bMouseTracking;	//

	CPtrArray m_arFolder;	//pointer to CBarFolder

	class CBarItem
	{
	public:
		CBarItem(const char* szItem, const int image, DWORD dwData, BOOL bPoint = FALSE);
		virtual  ~CBarItem();

		int		m_nImageIndex;
		char*	m_szItem;				//项的提示
		BOOL	m_bControl;				//指出m_dwData是否为指针
		DWORD	m_dwData;				//COMMAND ID or CLSID
	};

	class CBarFolder
	{
	public:
		void RemoveUnusedCtrlItem(void* plist);
		int FindItemByCLSID(CLSID *pClsid);
		CBarFolder(const char * szName, DWORD dwData);
		virtual  ~CBarFolder();

		int GetItemCount();
		int InsertItem(int nIndex, const char* szText, const int image, const DWORD dwData, BOOL bPoint =FALSE);

		char*			m_szName;
		DWORD			m_dwData;

		CImageList *	m_pImageList;
		CPtrArray		m_arItems;	//pointer to CBarItem
	protected:
		void RemoveIcon(int nIndex);
	};

// Operations
public:
	BOOL GetDefalutControlDesc(CLSID clsid, CString& strText);
	void AddDefaultControlFolder();

	//组操作
	int		AddFolder(const char* szFolderName, const DWORD dwData = 0);
	void	RemoveFolder(const int nIndex);

	LPCTSTR GetFolderName(const int iFolder);
	void	SetFolderName(const int iFolder, const char* szFolderName);
	

	int		GetSelFolder();
	void	SetSelFolder(const int iFolder);
	int		GetFolderCount() const;
	void	GetFolderClientRect(CRect& rect) const;
	BOOL	IsValidItem(const int nIndex) const;
	BOOL	GetFolderRect(const int nIndex, CRect & rect) const;
	void	GetItemRect(const int iFolder, const int iItem, CRect& rect) const;
	CSize	GetItemSize(const int iFolder, const int iItem) const;
	void	SetFolderImageList(const int iFolder, CImageList* pImageList) const;
	CImageList* GetFolderImageList(const int iFolder) const;

	int		HitTestEx(const CPoint & point, int& nIndex) const;
	void	HighlightFolder(const int nIndex);
	void	HighlightItem(const int nIndex, const BOOL bPressed = FALSE);
	void	AnimateFolderScroll(const int iFrom, const int iTo);
	

	void	DrawFolder(CDC * pDC, const int nIndex, CRect rect, const BOOL bSelected);
	void	PaintItems(CDC * pDC, const int nIndex, CRect rc);
	void	DrawItem(CDC * pDC, const int iFolder, CRect rcItem, const int iItem);

	//项操作
	int		InsertItem(const int nFolder, const int nIndex, const char* szText, const int nImage = -1, const DWORD dwData = 0, BOOL bPoint =FALSE);
	void	RemoveItem(const int nIndex);
	void	OnChangeSelItem(int nIndex);
	void	SetSelItem(int iItem);
	int		GetSelItem() const;

	void	InitWindow();
	void	LoadWindow();
	void	SaveWindow();
	BOOL	InsertControl(int iFolder, CLSID clsid, LPCTSTR szText = NULL);


	void	CalcParentWndWidth(int, int&);	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBoxCtrl)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBoxCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBoxCtrl)
		afx_msg void OnPaint();
		afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnAddFolder();
		afx_msg void OnRemoveFolder();
		afx_msg void OnReNameFolder();
		afx_msg void OnAddItem();
		afx_msg void OnRemoveItem();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()
	virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;

	friend class CMainFrame;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__BOXCTRL_H__)
