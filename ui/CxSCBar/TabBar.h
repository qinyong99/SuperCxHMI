#if !defined(AFX_TABBAR_H__A38BBD66_A6E7_452A_B1A3_FA4CAEA18BDD__INCLUDED_)
#define AFX_TABBAR_H__A38BBD66_A6E7_452A_B1A3_FA4CAEA18BDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// TabBar.h : header file
//功能:多页的可浮动工具栏
//制作:孙越洋
//时间:2002.1.5
//所属工程:CxScBar
//说明:使用者从类继承一个类, 然后在OnAddPages()中用AddPage()加入各页面窗口,
//	如不使用new创建CWnd对象, 则需要载OnDestroyAllChild()函数(在其后调用其类的对应函数),
//	并按自己方式删除子页面窗口对象, 并清空m_listChildWnd. 窗口的图标将显示在页Tab上.
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CTabBar window

class AFX_EXT_CLASS CTabBar : public CSizingControlBar
{
	class CTabPageInfo
	{
	public:
		CWnd* pPageWnd;		//一页对应的窗口
		int	 indexIcon;		//图标对应的索号
		CString str;		//显示名称
	};

// Construction
public:
	CTabBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTabMode(int nMode);
	void SetImageList(CImageList* pNewImage);
	virtual int AddPage(CWnd* pPage,  const CString& strName, int nIconIndex);
	void ActivatePage(int index);
	virtual ~CTabBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabBar)
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
protected:
	void UpdateScrollArrows();
	void TabScroll(BOOL bLeftOrRight);
	virtual void InitSize();
	int HiTest(CPoint point);
	void GetTabBarRect(CRect* rect);
	BOOL GetArrowRect(CRect& rectBound, CRect &rectLeft, CRect& rectRight);
	void EnSurePageVisible(int nPage);
	void DrawTabCtrl(CDC* pDC, CRect rect);
	void DrawTab(CDC* pDC, CRect rect, CString& strTitle, int indexImage, BOOL bSelected, int nLeftMost);
	void DrawScrollArrows(CDC *pDC, CRect rect, BOOL bWhich, int nState);
	void DrawArrowState(int nWhich, int nState);
	virtual void OnDestroyAllChild();
	virtual void OnAddPages();

protected:
	int m_nTabHeight;
	int m_nTabWidth;
	int m_nImageWidth;
	int m_firstTabID;	//第一个显示出来的Tab的ID号(从0开始, 如为-1则表示没有)
	int m_nArrowSize;	//箭头区宽度
	CArray<CTabPageInfo, CTabPageInfo> m_listChildWnd;
	int m_nActivatePage;
	CImageList* m_pIconList;

private:
	int m_nStateLArrow;
	int m_nStateRArrow;
	RECT m_clientRect;	//客户区丰主窗口的位置
	BOOL m_bMouseTracking;
	int m_nBarStyle;	//风格
};

#define TABBAR_MODE_DYNAMIC		0	//可滚动的TAB
#define TABBAR_MODE_STATIC		1	//可缩放大小但不可滚动
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBAR_H__A38BBD66_A6E7_452A_B1A3_FA4CAEA18BDD__INCLUDED_)
