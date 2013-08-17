// BarListCtrl.h : header file
////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARLISTCTRL_H__79079EDA_2CC2_40F2_978D_343BB5CC61BE__INCLUDED_)
#define AFX_BARLISTCTRL_H__79079EDA_2CC2_40F2_978D_343BB5CC61BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define CBARLISTCTRL_LISTID		1

/////////////////////////////////////////////////////////////////////////////
// CBarListCtrl window
class CBarListCtrl : public CWnd
{
	//类用于获取OLE拖放消息
	class CThisDropTarget : public COleDropTarget
	{
	public:
		virtual void OnDragLeave( CWnd* pWnd );
		virtual BOOL OnDrop( CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point );
		virtual DROPEFFECT OnDropEx( CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point );
		virtual DROPEFFECT OnDragScroll( CWnd* pWnd, DWORD dwKeyState, CPoint point );
		virtual DROPEFFECT OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
		virtual DROPEFFECT OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
		CWnd* m_pParent;
		DWORD m_dwKeyStateSource;
		COleDataObject* m_pDropData;
		CThisDropTarget()
		{
			m_dwKeyStateSource = 0;
			m_pParent = NULL;
			m_pDropData = NULL;
		};
	} m_targetDrop;

// Construction
public:
	CBarListCtrl();

// Attributes
public:
protected:
	CImageList m_listImage;
	CImageList m_listImageLarge;
	CImageList m_listImageTool;
	int m_nHoverButton;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarListCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	CListCtrl* GetListCtrl();
	BOOL Create(CRect& rect, CWnd* pParent, UINT ID);
	virtual ~CBarListCtrl();

	// Generated message map functions
protected:
	virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;
	virtual BOOL PreCreateImage(CImageList* pImageLarge, CImageList* pImageSmall);
	CListCtrl m_wndListCtrl;
	//{{AFX_MSG(CBarListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()

	virtual void DoCommand(int nButton) {m_wndListCtrl.Arrange(LVA_DEFAULT);};
	virtual DROPEFFECT FindDragEffect(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) = 0; //由子类实现的判定拖放光标型状类
	virtual BOOL DoDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point) = 0; //执行放操作
	void GetButtonRect(int index, CRect* pRect) const;
	void HoverButton(int index);
	int HitestButton(CPoint point) const;
	CRect GetToolBarRect() const;
	
	BOOL m_bMouseTracking;
	int m_nPushButton;

	friend class CThisDropTarget;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARLISTCTRL_H__79079EDA_2CC2_40F2_978D_343BB5CC61BE__INCLUDED_)
