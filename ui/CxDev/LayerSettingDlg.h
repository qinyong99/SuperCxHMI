#if !defined(AFX_LAYERSETTINGDLG_H__4DF81036_8834_4833_BA83_80890BB4B765__INCLUDED_)
#define AFX_LAYERSETTINGDLG_H__4DF81036_8834_4833_BA83_80890BB4B765__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
//LayerSettingDlg.h : header file
//名称: 层设置对话框CLayerSettingDlg
//功能: 进行当前层,可见层等的设置
//编制: 孙越洋
//时间: 2002.2.5
//所属工程: CxDev
/////////////////////////////////////////////////////////////////////////////
class CLayerStateWndCallBack
{
public:
	virtual void OnDrawItem(int nItem, CDC* pdc, CRect& rect) = 0;
	virtual void OnLBDownItem(int nItem, CRect rect, CPoint pt) = 0;
};

class CLayerStateWnd : public CWnd
{
public:
	CLayerStateWnd()
	{
		m_pCallBack = NULL;
		m_sizeItem.cx = m_sizeItem.cy = 0;
		m_nCount = 0;
		m_nItemPerLine = 0;
		m_nVDivider = 8;
		m_nHDivider = 8;
	};

public:
	CLayerStateWndCallBack* m_pCallBack;
	SIZE m_sizeItem;
	int m_nCount;
	int m_nVDivider;
	int m_nHDivider;

protected:
	int m_nItemPerLine;
	CRect m_rcWork;

public:
	BOOL SubClass(CWnd* pWnd);
	CRect GetItemRect(int nIndex);
	void InvalidateItem(int nIndex);
	void Init();

protected:
	int HiTest(POINT pt);

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg UINT OnNcHitTest( CPoint point );

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CLayerSettingDlg dialog
class CLayerSettingDlg : public CDialog , public CLayerStateWndCallBack
{
public:
	virtual void OnDrawItem(int nItem, CDC* pdc, CRect& rect);
	virtual void OnLBDownItem(int nItem, CRect rect, CPoint pt);

// Construction
public:
	unsigned long m_ulLockLayer;
	int m_nCurLayer;
	unsigned long m_ulVisibleLayer;
	CLayerSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLayerSettingDlg)
	enum { IDD = IDD_LAYER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayerSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CRect GetVisibleBtRect(CRect rcItem);
	CRect GetLockBtRect(CRect rcItem);
	CRect GetOutTextRect(CRect rcItem);
	void DrawItem(CDC* pdc, CRect rect, BOOL bVisible, BOOL bLock, int nItem);
	CImageList m_listImage;
	CLayerStateWnd m_wndState;
	// Generated message map functions
	//{{AFX_MSG(CLayerSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtVisible();
	afx_msg void OnBtHidden();
	afx_msg void OnBtLock();
	afx_msg void OnBtUnlock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYERSETTINGDLG_H__4DF81036_8834_4833_BA83_80890BB4B765__INCLUDED_)
