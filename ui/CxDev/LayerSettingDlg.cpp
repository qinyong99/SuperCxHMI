////////////////////////////////////////////////////////////////////////////
// LayerSettingDlg.cpp : implementation file
//名称: 层设置对话框CLayerSettingDlg
//功能: 进行当前层,可见层等的设置
//编制: 孙越洋
//时间: 2002.2.5
//所属工程: CxDev
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cxdev.h"
#include "LayerSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STATE_BUTTON_WIDTH		12

BEGIN_MESSAGE_MAP(CLayerStateWnd, CWnd)
	//{{AFX_MSG_MAP(CLayerStateWnd)
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CLayerStateWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ASSERT(m_pCallBack);
	
	int nItem = HiTest(point);
	CRect rect = GetItemRect(nItem);
	rect.DeflateRect(m_nHDivider / 2, m_nVDivider / 2, m_nHDivider / 2, m_nVDivider / 2);
	CPoint pt;
	m_pCallBack->OnLBDownItem(nItem, rect, point);

	CWnd::OnLButtonDown(nFlags, point);
}

void CLayerStateWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	dc.SetBkMode(TRANSPARENT);
	for(int i = 0; i < m_nCount; i++)
	{
		CRect rect = GetItemRect(i);
		dc.FillSolidRect(&rect, ::GetSysColor(COLOR_3DFACE));
//		dc.Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0));
		if(m_pCallBack != NULL)
		{
			rect.DeflateRect(m_nHDivider / 2, m_nVDivider / 2, m_nHDivider / 2, m_nVDivider / 2);
			m_pCallBack->OnDrawItem(i, &dc, rect);
		}
	};
}

BOOL CLayerStateWnd::SubClass(CWnd* pWnd)
{
	ASSERT(pWnd);
	ASSERT(m_pCallBack);

	BOOL bRet = CWnd::SubclassWindow(pWnd->m_hWnd);
	Init();
	return bRet;
};

void CLayerStateWnd::Init()
{
	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(2, 2, 2, 2);

	m_nItemPerLine = rect.Width() / (m_sizeItem.cx + 2);
	m_nItemPerLine = m_nItemPerLine > 5 ? 5 : m_nItemPerLine;
	m_nHDivider = (rect.Width() - m_nItemPerLine * m_sizeItem.cx) / m_nItemPerLine;
	m_nHDivider = m_nHDivider > 2 ? m_nHDivider : 2;

	int nLine = m_nCount / m_nItemPerLine;
	if(m_nCount % m_nItemPerLine > 0)
		nLine++;
	m_nVDivider = (rect.Height() - nLine * m_sizeItem.cy) / nLine;
	m_nVDivider = m_nVDivider > 2 ? m_nVDivider : 2;

	m_rcWork = rect;
};

int CLayerStateWnd::HiTest(POINT pt)
{
	if(m_rcWork.PtInRect(pt))
	{
		int nCol = (pt.x - m_rcWork.left) / (m_sizeItem.cx + m_nHDivider);
		int nLine = (pt.y - m_rcWork.top) / (m_sizeItem.cy + m_nVDivider);
		int nItem = nLine * m_nItemPerLine + nCol;
		if(nItem >= m_nCount)
			return -1;
		else
			return nItem;
	}

	return -1;
}

CRect CLayerStateWnd::GetItemRect(int nIndex)
{
	CRect rcRet(0, 0, 0, 0);

	if(m_nItemPerLine == 0)
		return rcRet;

	int nLine = nIndex / m_nItemPerLine;
	int nCol = nIndex % m_nItemPerLine;

	rcRet.left = m_rcWork.left + (m_sizeItem.cx + m_nHDivider) * nCol;
	rcRet.right = rcRet.left + (m_sizeItem.cx + m_nHDivider);
	rcRet.top = m_rcWork.top + (m_sizeItem.cy + m_nVDivider) * nLine;
	rcRet.bottom = rcRet.top + (m_sizeItem.cy + m_nVDivider);

	return rcRet;
};

UINT CLayerStateWnd::OnNcHitTest( CPoint point )
{
	CRect rect;
	GetClientRect(&rect);
	ScreenToClient(&point);
	if(rect.PtInRect(point))
		return HTCLIENT;
	else
		return HTTRANSPARENT;
}

void CLayerStateWnd::InvalidateItem(int nIndex)
{
	CRect rect = GetItemRect(nIndex);
	InvalidateRect(&rect, false);
}

/////////////////////////////////////////////////////////////////////////////
// CLayerSettingDlg dialog
CLayerSettingDlg::CLayerSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLayerSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLayerSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCurLayer = -1;
	m_ulLockLayer = 0;
	m_ulVisibleLayer = -1;
}


void CLayerSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLayerSettingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLayerSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CLayerSettingDlg)
	ON_BN_CLICKED(IDC_BT_VISIBLE, OnBtVisible)
	ON_BN_CLICKED(IDC_BT_HIDDEN, OnBtHidden)
	ON_BN_CLICKED(IDC_BT_LOCK, OnBtLock)
	ON_BN_CLICKED(IDC_BT_UNLOCK, OnBtUnlock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayerSettingDlg message handlers
void CLayerSettingDlg::OnDrawItem(int nItem, CDC* pdc, CRect& rect)
{
	ASSERT(nItem >= 0);

	unsigned long ulMask = 1;
	ulMask = ulMask << nItem;
	BOOL bVisible, bLock;
	
	if( (m_ulLockLayer & ulMask) !=  0)
	{	//锁定层
		bLock = TRUE;
	}
	else
	{	//未锁定层
		bLock = FALSE;
	}

	if( (m_ulVisibleLayer & ulMask) != 0)
	{	//可见层
		bVisible = TRUE;
	}
	else
	{	//不可见层
		bVisible = FALSE;
	}

	DrawItem(pdc, rect, bVisible, bLock, nItem);
};

void CLayerSettingDlg::OnLBDownItem(int nItem, CRect rect, CPoint pt)
{
	if(nItem >= 0 && nItem < 30)
	{
		unsigned long ulMask = 1;
		ulMask = ulMask << nItem;

		CRect rc = GetOutTextRect(rect);
		if(rc.PtInRect(pt))
		{	//设定当前层
			if(m_nCurLayer == nItem)
				return;
			else
				m_wndState.InvalidateItem(m_nCurLayer);
			m_nCurLayer = nItem;
	//		m_ulVisibleLayer = m_ulVisibleLayer | ulMask;
		}
		else
		{
			rc = GetLockBtRect(rect);
			if(rc.PtInRect(pt))
			{	//设定锁定层
				if( (m_ulLockLayer & ulMask) == 0)
					m_ulLockLayer = m_ulLockLayer | ulMask;
				else
					m_ulLockLayer = m_ulLockLayer ^ ulMask;
			}
			else 
			{	
				rc = GetVisibleBtRect(rect);
				if(rc.PtInRect(pt))
				{	//设定可见层
					if( (m_ulVisibleLayer & ulMask) == 0)
						m_ulVisibleLayer = m_ulVisibleLayer | ulMask;
					else //if (nItem != m_nCurLayer)
						m_ulVisibleLayer = m_ulVisibleLayer ^ ulMask;
				};
			}

		}
	}

	m_wndState.InvalidateItem(nItem);
};


BOOL CLayerSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listImage.Create(IDB_LSTATE, 12,  1, RGB(255, 255, 255));

	m_wndState.m_nCount = 30;
	m_wndState.m_sizeItem.cx = 38;
	m_wndState.m_sizeItem.cy = 30;
	m_wndState.m_pCallBack = (CLayerStateWndCallBack*)this;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_LAYERGROUP);
	m_wndState.SubClass(GetDlgItem(IDC_STATIC_LAYERGROUP));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DrawBack(CDC* pdc, CRect& rc, COLORREF clr, COLORREF clrWite = RGB(255, 255, 255))
{
	CRect rect = rc;
	rect.DeflateRect(2, 2, 2, 2);

	int x,y;

	for(y = rect.top ; y < rect.bottom; y++)
	{
		for(x = rect.left; x < rect.right; x++)
		{
			if( (y % 2) == 0)
			{
				if( (x % 2) == 0)
					pdc->SetPixel(x, y, clr);
				else
					pdc->SetPixel(x, y, clrWite);
			}
			else
			{
				if( (x % 2) != 0)
					pdc->SetPixel(x, y, clr);
				else
					pdc->SetPixel(x, y, clrWite);
			}
		};
	};
};


////////////////////////////////////////////////////////////////////////////
//功能:画层项的图形表示
//参数:pdc[in]			设备
//		rect[in]		位置
//		bVisible[in]	是否是可见状态
//		bLock(in]		是否是锁定状态
//		nItem[in]		第几个项
////////////////////////////////////////////////////////////////////////////
void CLayerSettingDlg::DrawItem(CDC *pdc, CRect rect, BOOL bVisible, BOOL bLock, int nItem)
{
	COLORREF clrLight = RGB(255, 255, 255);
	COLORREF clrDark = RGB(0, 0, 0);

	ASSERT(pdc);
//	pdc->Draw3dRect(&rect, clrDark, clrDark);

	CRect rcTmp = GetOutTextRect(rect);
	if(nItem == m_nCurLayer)
	{
		DrawBack(pdc, rect, RGB(200, 200, 180));
		pdc->Draw3dRect(&rcTmp, clrDark, clrLight);
	}
	else
		pdc->Draw3dRect(&rcTmp, clrLight, clrDark);


	CFont font;
	font.CreateFont(rcTmp.Height() / 2, rcTmp.Width() / 2, 0, 0, 0, false, false,
		0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
	rcTmp.DeflateRect(1, 1, 1, 1);
	CString str;
	str.Format("%d", ++nItem);
	CFont* pOldFont = pdc->SelectObject(&font);
	pdc->DrawText(str, &rcTmp, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pdc->SelectObject(pOldFont);
	font.DeleteObject();

	rcTmp = GetLockBtRect(rect);
	POINT pt;
	pt.x = rcTmp.left + 1;
	pt.y = rcTmp.top + 1;
	if(bLock)
	{
		m_listImage.Draw(pdc, 2, pt, ILD_NORMAL);
		pdc->Draw3dRect(&rcTmp, clrDark, clrLight);
	}
	else
	{
		m_listImage.Draw(pdc, 0, pt, ILD_NORMAL);
		pdc->Draw3dRect(&rcTmp, clrLight, clrDark);
	};
	
	rcTmp = GetVisibleBtRect(rect);
	pt.x = rcTmp.left + 1;
	pt.y = rcTmp.top + 1;
	if(bVisible)
	{
		m_listImage.Draw(pdc, 1, pt, ILD_NORMAL);
		pdc->Draw3dRect(&rcTmp, clrLight, clrDark);
	}
	else
	{
		m_listImage.Draw(pdc, 3, pt, ILD_NORMAL);
		pdc->Draw3dRect(&rcTmp, clrDark, clrLight);
	};
}


CRect CLayerSettingDlg::GetOutTextRect(CRect rcItem)
{
	rcItem.DeflateRect(1, 1, 1, 1);

	CRect rcRet;
	rcRet.left = rcItem.left;
	rcRet.top = rcItem.top;
	rcRet.bottom = rcItem.bottom;
	rcRet.right = rcItem.right - STATE_BUTTON_WIDTH - 3;

	return rcRet;
}

CRect CLayerSettingDlg::GetLockBtRect(CRect rcItem)
{
	CRect rcRet;

	rcItem.DeflateRect(1, 1, 1, 1);

	rcRet.left = rcItem.right - STATE_BUTTON_WIDTH - 3;
	rcRet.top = rcItem.top;
	rcRet.right = rcItem.right;
	rcRet.bottom = rcItem.bottom - rcItem.Height() / 2;

	return rcRet;
}

CRect CLayerSettingDlg::GetVisibleBtRect(CRect rcItem)
{
	CRect rcRet;

	rcItem.DeflateRect(1, 1, 1, 1);

	rcRet.left = rcItem.right - STATE_BUTTON_WIDTH - 3;
	rcRet.top = rcItem.bottom - rcItem.Height() / 2;
	rcRet.right = rcItem.right;
	rcRet.bottom = rcItem.bottom;

	return rcRet;
}

void CLayerSettingDlg::OnBtVisible() 
{
	// TODO: Add your control notification handler code here
	m_ulVisibleLayer = 0xFFFFFFFF;
	m_wndState.Invalidate(false);
}

void CLayerSettingDlg::OnBtHidden() 
{
	// TODO: Add your control notification handler code here
	m_ulVisibleLayer = 0;
	m_wndState.Invalidate();
}

void CLayerSettingDlg::OnBtLock() 
{
	// TODO: Add your control notification handler code here
	m_ulLockLayer = 0xFFFFFFFF;
	m_wndState.Invalidate();
}

void CLayerSettingDlg::OnBtUnlock() 
{
	// TODO: Add your control notification handler code here
	m_ulLockLayer = 0;
	m_wndState.Invalidate();
}
