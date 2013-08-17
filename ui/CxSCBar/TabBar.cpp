// TabBar.cpp : implementation file
//功能:多页的可浮动工具栏
//制作:孙越洋
//时间:2002.1.5
//所属工程:CxScBar
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sizecbar.h"
#include "TabBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////
//箭头操作用数
#define LARROW		0
#define RARROW		1
#define STATE_NORMAL	1
#define STATE_DOWN		2
#define STATE_DISABLE	4


/////////////////////////////////////////////////////////////////////////////
// CTabBar
CTabBar::CTabBar()
{
	m_nTabHeight = 21;
	m_nTabWidth = 34;
	m_firstTabID = 0;
	m_nArrowSize = 64;
	m_nActivatePage = 0;
	m_nStateLArrow = m_nStateRArrow = STATE_NORMAL;
	m_bMouseTracking = FALSE;
	m_pIconList = NULL;
	m_nImageWidth = 0;
	m_nBarStyle = TABBAR_MODE_STATIC;
}

CTabBar::~CTabBar()
{
	if(m_pIconList != NULL)
		delete m_pIconList;
}

BEGIN_MESSAGE_MAP(CTabBar, CSizingControlBar)
	//{{AFX_MSG_MAP(CTabBar)
	ON_WM_CAPTURECHANGED()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTabBar message handlers

////////////////////////////////////////////////////////////////
//功能:击活指定的页面
////////////////////////////////////////////////////////////////
void CTabBar::ActivatePage(int index)
{
	if( index >= m_listChildWnd.GetSize() || index < 0)	//无效页面
		return;

	CRect rect;
	m_nActivatePage = index;
	int count = m_listChildWnd.GetSize();
	for(int i = 0; i < count; i++)
	{
		if(i != index)
			m_listChildWnd[i].pPageWnd->ShowWindow(SW_HIDE);
		else
			m_listChildWnd[i].pPageWnd->ShowWindow(SW_SHOW);
	}

	EnSurePageVisible(m_nActivatePage);

	Invalidate();
}

/////////////////////////////////////////////////////////////////
//功能:子类在此加入需要的属性页窗口
/////////////////////////////////////////////////////////////////
void CTabBar::OnAddPages()
{
}

/////////////////////////////////////////////////////////////////
//功能:如窗口对象不是用new加入的, 子类重载此函数按自己的方法删除
/////////////////////////////////////////////////////////////////
void CTabBar::OnDestroyAllChild()
{
	CTabPageInfo info;
	int count = m_listChildWnd.GetSize();
	for(int i = 0; i < count; i++)
	{
		m_listChildWnd[i].pPageWnd->DestroyWindow();
		delete m_listChildWnd[i].pPageWnd;
	}

	m_listChildWnd.RemoveAll();
}

////////////////////////////////////////////////////////////
//功能:改变箭头的状态为指定状态
//参数:nWhich[in]		右还是左箭头(LARROW, RARROW)
//		nState[in]		箭头状态(STATE_DOWN, STATE_UP)
////////////////////////////////////////////////////////////
void CTabBar::DrawArrowState(int nWhich, int nState)
{
	CRect rect;
	GetTabBarRect(&rect);
	rect.left = rect.right - m_nTabWidth;
	CRect rectLeft, rectRight;
	GetArrowRect(rect, rectLeft, rectRight);

	CDC* pdc = GetDC();
	if(nWhich == LARROW)
	{
		pdc->FillSolidRect(&rectLeft, GetSysColor(COLOR_3DFACE));
		DrawScrollArrows(pdc, &rectLeft, TRUE, nState);
	}
	else if(nWhich == RARROW)
	{
		pdc->FillSolidRect(&rectRight, GetSysColor(COLOR_3DFACE));
		DrawScrollArrows(pdc, &rectRight, FALSE, nState);
	}
	
	ReleaseDC(pdc);
}

/////////////////////////////////////////////////////////////
//功能:画箭头按钮
//参数:pDC[in]		DC
//		bWhich[in]	向左还是向右
//		rect[in]	按钮画的位置
//		nState[in]	按钮的状态(STATE_UP, STATE_DOWN, STATE_DISABLE)
/////////////////////////////////////////////////////////////
void CTabBar::DrawScrollArrows(CDC *pDC, CRect rect, BOOL bWhich, int nState)
{
	CBrush brush;
	if((nState & STATE_DISABLE) != 0)
		brush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	else
		brush.CreateSolidBrush(RGB(0, 0, 0));
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	POINT pt[3];
	if(bWhich)
	{	//左箭
		pt[0].x = rect.left + rect.Width() / 4;
		pt[0].y = rect.top + rect.Height() / 2;
		pt[1].x = pt[0].x + rect.Width() / 2;
		pt[1].y = rect.top + rect.Height() / 4;
		pt[2].x = pt[1].x;
		pt[2].y = rect.bottom - rect.Height() / 4;
	}
	else
	{	//右箭
		pt[0].x = rect.left + rect.Width() / 4;
		pt[0].y = rect.top + rect.Height() / 4;
		pt[1].x = pt[0].x + rect.Width() / 2;
		pt[1].y = rect.top + rect.Height() / 2;
		pt[2].x = pt[0].x;
		pt[2].y = rect.bottom - rect.Height() / 4;
	};
	pDC->Polygon(pt, 3);
	
	if((nState & STATE_NORMAL) == 0)
	{	//有必要画箭头外框
		if((nState & STATE_DOWN) == 0)
		{
			pDC->Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0));
		}
		else
		{
			pDC->Draw3dRect(&rect, RGB(0, 0, 0), RGB(255, 255, 255));
		}
	}
	
	pDC->SelectObject(pOldBrush);
}

/////////////////////////////////////////////////////////////
//功能:画一个标签
//参数:bSelected[in]	选中状态
//		pDC[in]			DC
//		indexImage[in]	图标索引号
//		rect[in]		画的位置
//		strTitle[in]	显示名
//		nLeftMost[in]	可画到的最右位置, 如rect超出则只画一半
/////////////////////////////////////////////////////////////
void CTabBar::DrawTab(CDC *pDC, CRect rect, CString &strTitle, int indexImage, BOOL bSelected, int nLeftMost)
{
	CRgn rgn;
	POINT ptRgn[8];
	CRgn oldRgn;
	if(rect.right > nLeftMost)
	{	//为画半个
		ptRgn[0].x = nLeftMost;
		ptRgn[0].y = rect.bottom;
		ptRgn[1].x = nLeftMost - 3;
		ptRgn[1].y = rect.bottom - rect.Height() / 4;
		ptRgn[2].x = nLeftMost;
		ptRgn[2].y = ptRgn[1].y - rect.Height() / 4;
		ptRgn[3].x = nLeftMost - 3;
		ptRgn[3].y = ptRgn[2].y - rect.Height() / 4;
		ptRgn[4].x = nLeftMost;
		ptRgn[4].y = rect.top;

		ptRgn[5].x = rect.left;
		ptRgn[5].y = rect.top;
		ptRgn[6].x = rect.left;
		ptRgn[6].y = rect.bottom;

		rgn.CreatePolygonRgn(ptRgn, 7, WINDING);
		oldRgn.CreateRectRgn(0, 0, 0, 0);
		::GetClipRgn(pDC->m_hDC, (HRGN)(oldRgn.m_hObject));
		pDC->SelectClipRgn(&rgn, RGN_COPY);
	}
	if(bSelected)
		pDC->FillSolidRect(&rect, GetSysColor(COLOR_3DFACE));
	

	//画图标
	POINT pt[6];
	if(rect.Width() > 18)
	{
		pt[0].x = rect.left + 4;
		pt[0].y = rect.top + (rect.Height() - 16) / 2;
		if(m_pIconList != NULL && m_pIconList->GetImageCount() > indexImage)
			m_pIconList->Draw(pDC, indexImage, pt[0], ILD_TRANSPARENT);
	};

	//写字
	rect.DeflateRect(2, 2, 2, 2);
	int oldMode = pDC->SetBkMode(TRANSPARENT);
	CRect rectTmp = rect;
	rectTmp.left += m_nImageWidth + 4;
	pDC->DrawText(strTitle, &rectTmp, DT_LEFT | DT_SINGLELINE |DT_VCENTER);
	pDC->SetBkMode(oldMode);
	rect.InflateRect(2, 1, 2, 1);


	//画边框
	CPen pen, *pOldPen;
	pen.CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	if(bSelected)
	{	//选中项变化一下
		pt[0].x = rect.left;
		pt[0].y = rect.bottom - 4;
		pt[1].x = pt[0].x;
		pt[1].y = rect.top + 1;
		pt[2].x = rect.left + 1;
		pt[2].y = rect.top + 1;
		pt[3].x = rect.left + 1;
		pt[3].y = rect.bottom - 4;
		pt[4].x = rect.left + 4;
		pt[4].y = rect.bottom - 1;
	}
	else
	{
		pt[0].x = rect.left;
		pt[0].y = rect.top + 1;
		pt[1].x = rect.left;
		pt[1].y = rect.bottom - 4;
	}
	pOldPen = pDC->SelectObject(&pen);
	if(bSelected)
		pDC->Polyline(pt, 5);
	else
		pDC->Polyline(pt, 2);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	pt[0].x = rect.left + 4;
	pt[0].y = rect.bottom - 1;
	pt[1].x = rect.right - 4;
	pt[1].y = rect.bottom -1;
	pt[2].x = rect.right - 1;
	pt[2].y = rect.bottom - 4;
	pt[3].x = rect.right - 1;
	pt[3].y = rect.top;
	pen.CreatePen(PS_SOLID, 0, RGB(128, 128, 128));
	pOldPen = pDC->SelectObject(&pen);
	pDC->Polyline(pt, 4);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	pt[0].x = rect.left;
	pt[0].y = rect.bottom - 4;
	pt[1].x = rect.left + 4;
	pt[1].y = rect.bottom;
	pt[2].x = rect.right - 4;
	pt[2].y = rect.bottom;
	pt[3].x = rect.right;
	pt[3].y = rect.bottom - 4;
	pt[4].x = rect.right;
	pt[4].y = rect.top;
	pen.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	pOldPen = pDC->SelectObject(&pen);
	pDC->Polyline(pt, 5);
	if(rect.right > nLeftMost)
	{	//恢复画半个时的变动
		CRect tmpRect;
		oldRgn.GetRgnBox(&tmpRect);
		if( tmpRect.IsRectNull())
			::SelectClipRgn(pDC->m_hDC, NULL);
		else
			pDC->SelectClipRgn(&oldRgn, RGN_COPY);
		pDC->Polyline(ptRgn, 5);
	}
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

////////////////////////////////////////////////////////////
//功能:画Tab控制的图像
////////////////////////////////////////////////////////////
void CTabBar::DrawTabCtrl(CDC *pDC, CRect rect)
{
	int count = m_listChildWnd.GetSize();
	CRect rectLeft, rectRight, rectTmp;
	CString strText;
	GetArrowRect(rect, rectLeft, rectRight);

	if(count > 0)
	{	//作各标签
		if(m_nBarStyle == TABBAR_MODE_DYNAMIC)
		{
			CRect rectSel(0, 0, 0, 0);
			rectTmp = rect;
			strText = "高明";

			rectTmp.right = rect.left + m_nTabWidth;
			for(int i = 0; i < count; i++)
			{
				strText = m_listChildWnd[i].str;

				if(rectTmp.left > rectLeft.left - 5)	//已超出边界的页
					break;

				if(i < m_firstTabID)
				continue;
				else if( i == m_nActivatePage)
				{	//选中项
					rectSel = rectTmp;
					//DrawTab(&dc, rectTmp, strText, TRUE);
				}
				else
				{	//未选中项
					DrawTab(pDC, rectTmp, strText, m_listChildWnd[i].indexIcon, FALSE, rectLeft.left- 5);
				}

				rectTmp.right += m_nTabWidth;
				rectTmp.left += m_nTabWidth;
			};

			if(!rectSel.IsRectEmpty())
			{	//画选中项
				strText = m_listChildWnd[m_nActivatePage].str;
				DrawTab(pDC, rectSel, strText, m_listChildWnd[m_nActivatePage].indexIcon, TRUE, rectLeft.left - 5);
			}
		}
		else if(m_nBarStyle == TABBAR_MODE_STATIC)
		{
			if(rect.Width() > 3 * m_nTabWidth)
				rect.right = rect.left + 3 * m_nTabWidth;
			int width = rect.Width() / count;
			rectTmp = rect;
			for(int i = 0; i < count; i++)
			{
				rectTmp.right = rectTmp.left + width - 1;
				strText = m_listChildWnd[i].str;
				if( i == m_nActivatePage)
				{	//选中项
					DrawTab(pDC, rectTmp, strText, m_listChildWnd[m_nActivatePage].indexIcon, TRUE, rect.right);
				}
				else
				{	//未选中项
					DrawTab(pDC, rectTmp, strText, m_listChildWnd[i].indexIcon, FALSE, rect.right);
				}

				rectTmp.left += width;
			};

			return;
		};
	}

	//作滚动按钮
	rect.left = rect.right - m_nTabWidth;
	if(GetArrowRect(rect, rectLeft, rectRight))
	{
		DrawScrollArrows(pDC, rectLeft, TRUE, m_nStateLArrow);
		DrawScrollArrows(pDC, rectRight, FALSE, m_nStateRArrow);
	}
}

////////////////////////////////////////////////////////////
//功能:确保指指定的页的Tab标题可以看见, 必要作移动
//参数:nPage[in]	指定的页号(从0开始)
////////////////////////////////////////////////////////////
void CTabBar::EnSurePageVisible(int nPage)
{
	if(nPage < 0 || nPage >= m_listChildWnd.GetSize())	//无效参数
		return;

	CRect rect;
	GetTabBarRect(&rect);
	CRect leftRect, rightRect;
	GetArrowRect(rect, leftRect, rightRect);
	
	//求当前可以显示的页数
	int count = m_listChildWnd.GetSize();
	int pageCount = 0;
	int tmpRight = rect.left + m_nTabWidth;
	while(tmpRight < leftRect.left - 5)
	{
		pageCount++;
		tmpRight += m_nTabWidth;
	}

	//当前页TAB是否已显示
	if(pageCount < 1)	//无足够空间
		return;

	if(nPage >= m_firstTabID && nPage < m_firstTabID + pageCount)
	{	//该页已显示出来了
		return;
	}

	//移动TabBar
	int moveCount = 0;
	if(nPage < m_firstTabID)
	{	//在左边
		moveCount = m_firstTabID - nPage;
		while(moveCount > 0)
		{
			TabScroll(FALSE);
			moveCount--;
		}
	}
	else
	{	//在右边
		moveCount = nPage - m_firstTabID - pageCount + 1;
		while(moveCount > 0)
		{
			TabScroll(TRUE);
			moveCount--;
		}
	}
}

/////////////////////////////////////////////////////////////////
//功能:给定位置广计算移动工具两箭头的位置
//参数:rectBount[in]		总位置
//		rectLeft[out]		左箭头的位置
//		rectRight[out]		右箭头的位置
//返回:成功TRUE, 如总位置大小FALSE
/////////////////////////////////////////////////////////////////
BOOL CTabBar::GetArrowRect(CRect &rectBound, CRect &rectLeft, CRect &rectRight)
{
	CRect rect(rectBound.left, rectBound.top, rectBound.right, rectBound.bottom);
	rectLeft = rect;
	rectRight = rect;
	int len = rectBound.Width() > rectBound.Height() ? rectBound.Height() / 2 - 2 : rectBound.Width() / 4 - 2;
	if(len < 4)	//太小
		return FALSE;

	rectRight.right = rect.right - len;
	rectRight.left = rectRight.right - len;
	rectRight.top = rect.top + rect.Height() / 4;
	rectRight.bottom = rect.bottom - rect.Height() / 4;
	rectLeft.right = rectRight.left;
	rectLeft.top = rectRight.top;
	rectLeft.left = rectLeft.right - len;
	rectLeft.bottom = rectRight.bottom;

	return TRUE;
}

//////////////////////////////////////////////////////////////////
//功能:返回用于Tab条的位置
//参数:rect[out]		返回的位置
//////////////////////////////////////////////////////////////////
void CTabBar::GetTabBarRect(CRect *rect)
{
	CRect theRect;

	GetClientRect(&theRect);
	theRect.DeflateRect(4, 2, 4, 0);
	theRect.top = theRect.bottom - m_nTabHeight - 2;
	
	*rect = theRect;
}

/////////////////////////////////////////////////////////////
//功能:判定point所指点在什么窗口Tab中的什么部份
//参数:point[in]	所求点
//返回:-1:在空白, >= 0在第几个Tab上, -2:在左移按钮上, -3:在右移按钮上
/////////////////////////////////////////////////////////////
int CTabBar::HiTest(CPoint point)
{
	int nRet = -1;
	CRect rect;
	GetTabBarRect(&rect);
	if(m_nBarStyle == TABBAR_MODE_STATIC)
	{	//静态方式
		if(rect.Width() > 3 * m_nTabWidth)
			rect.right = rect.left + 3 * m_nTabWidth;
		if(rect.PtInRect(point))
		{
			return (point.x - rect.left) * 3 / rect.Width();
		}
		else
			return -1;
	};


	if( !rect.PtInRect(point))	//不在Tab区
		return nRet;
	else
	{
		CRect rectTmp = rect;
		CRect rectLeft, rectRight;
		GetArrowRect(rect, rectLeft, rectRight);
		rectTmp.left = rectLeft.left - 5;
		rectTmp.right = rectRight.right + 5;

		if(rectTmp.PtInRect(point))
		{	//在按钮区
			CRect rectLeft, rectRight;
			rect.left = rect.right - m_nTabWidth;
			if(GetArrowRect(rect, rectLeft, rectRight) == FALSE)
				return -1;

			if(rectLeft.PtInRect(point))
				return -2;
			else if(rectRight.PtInRect(point))
				return -3;
			else
				return -1;
		}

		rectTmp = rect;
		rectTmp.right = rect.left + m_nTabWidth;
		int max = m_listChildWnd.GetSize() - 1;
		if(max < 0)	//无一个子窗口存在
			return nRet;

		nRet = m_firstTabID;
		do
		{
			if(rectTmp.PtInRect(point))
			{
				if(nRet <= max)	//点中存在的Tab标签
					return nRet;
				else	//点中一个不存在的Tab标签
					return -1;
			}
			
			nRet++;
			rectTmp.left += m_nTabWidth;
			rectTmp.right += m_nTabWidth;

			if(rectTmp.right > rectLeft.left - 5)
				rectTmp.right = rectLeft.left - 5;
		}while(rectTmp.left < rectLeft.left - 5);
	}

	return -1;	//无结果
}

/////////////////////////////////////////////////////////////////
//功能:根据系统字体设置Tab的大小, 子类可在此设置不同于默认字体的字体
/////////////////////////////////////////////////////////////////
void CTabBar::InitSize()
{
	HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	CFont* pFont = CFont::FromHandle(hFont);
	LOGFONT info;
	pFont->GetLogFont(&info);
	pFont->Detach();

	m_nTabHeight = info.lfHeight > 0 ? info.lfHeight : -info.lfHeight; 
	m_nTabWidth = 4 * m_nTabHeight;
	m_nTabHeight += m_nTabHeight / 3;
	m_nArrowSize = 104;
}

void CTabBar::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	if(pWnd != this)
	{
		CRect rect;
		GetTabBarRect(&rect);
		m_nStateLArrow &= ~STATE_DOWN;
		m_nStateRArrow &= ~STATE_DOWN;
		InvalidateRect(&rect);
	}
	CSizingControlBar::OnCaptureChanged(pWnd);
}

int CTabBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CSizingControlBar::OnCreate(lpCreateStruct) == -1)
        return -1;

	//m_listIcon.Create(IDB_TABBAR_ICONS, 16, 1, RGB(255, 255, 255));
	InitSize();
	OnAddPages();

 	UpdateScrollArrows();
	return 0;
}

void CTabBar::OnDestroy() 
{
	CSizingControlBar::OnDestroy();
	
	// TODO: Add your message handler code here
	OnDestroyAllChild();
}

void CTabBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int nHiCode = HiTest(point);
	if( nHiCode != -1)	//击中非空白区
	{
		if(nHiCode == -2 || nHiCode == -3)
			OnLButtonDown(nFlags, point);
		return;
	}
	
	CSizingControlBar::OnLButtonDblClk(nFlags, point);
}

void CTabBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int htCode = HiTest(point);
	if(htCode >= 0)
	{	//页面上
		ActivatePage(htCode);
	}
	else if(htCode == -1)
	{	//空白
		CSizingControlBar::OnLButtonDown(nFlags, point);
	}
	else if(htCode == -2)
	{	//左移
		m_nStateLArrow |= STATE_DOWN;
		if((m_nStateLArrow & STATE_DISABLE) == 0)
			DrawArrowState(LARROW, STATE_DOWN);
		SetCapture();
	}
	else if(htCode == -3)
	{	//右移
		m_nStateRArrow |= STATE_DOWN;
		if((m_nStateRArrow & STATE_DISABLE) == 0)
			DrawArrowState(RARROW, STATE_DOWN);
		SetCapture();
	};
}

void CTabBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nHiCode = HiTest(point);
	if(nHiCode == -2)
	{	//左箭头
		if(m_nStateLArrow == STATE_DOWN)
		{
			TabScroll(FALSE);
		};
		m_nStateLArrow &= ~STATE_DOWN;
		DrawArrowState(LARROW, m_nStateLArrow);
	}
	else if(nHiCode == -3)
	{	//右箭头
		if(m_nStateRArrow == STATE_DOWN)
		{
			TabScroll(TRUE);
		};
		m_nStateRArrow &= ~STATE_DOWN;
		DrawArrowState(RARROW, m_nStateRArrow);
	}

	ReleaseCapture();

	CSizingControlBar::OnLButtonUp(nFlags, point);
}

LRESULT CTabBar::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_nStateLArrow |= STATE_NORMAL;
	m_nStateRArrow |= STATE_NORMAL;

	DrawArrowState(LARROW, m_nStateLArrow);
	DrawArrowState(RARROW, m_nStateRArrow);
	m_bMouseTracking = FALSE;

	return 0;
}

void CTabBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nBarStyle == TABBAR_MODE_STATIC)
	{
		CSizingControlBar::OnMouseMove(nFlags, point);
		return;
	}

	if (!m_bMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		memset(&tme, 0, sizeof(TRACKMOUSEEVENT));
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		//监控鼠标离开
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
  
		if (_TrackMouseEvent(&tme))
		{
			m_bMouseTracking = TRUE;
		}
	}

	int nHiCode = HiTest(point);
	if(nHiCode == -2)
	{	//左移按钮,且是平的状态
		m_nStateLArrow &= ~STATE_DOWN;
		m_nStateLArrow &= ~STATE_NORMAL;
		m_nStateRArrow |= STATE_NORMAL;
	}
	else if(nHiCode == -3)
	{	//右移按钮,且是平的状态
		m_nStateRArrow &= ~STATE_DOWN;
		m_nStateRArrow &= ~STATE_NORMAL;
		m_nStateLArrow |= STATE_NORMAL;
	}
	else 
	{	//不在按钮区,并且状态不是平
		m_nStateLArrow |= STATE_NORMAL;
		m_nStateRArrow |= STATE_NORMAL;
	}

	DrawArrowState(LARROW, m_nStateLArrow);
	DrawArrowState(RARROW, m_nStateRArrow);

	CSizingControlBar::OnMouseMove(nFlags, point);
}

void CTabBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	CFont* pFont = CFont::FromHandle(hFont);
	CFont* pOldFont = dc.SelectObject(pFont);

	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(&rect, GetSysColor(COLOR_3DFACE));
	rect.DeflateRect(2, 2, 2, m_nTabHeight);
	dc.Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0));
	rect.bottom += 1;
	dc.Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0));
	
	GetTabBarRect(&rect);
	DrawTabCtrl(&dc, rect);

	dc.SelectObject(pOldFont);
	pFont->Detach();
}

void CTabBar::OnSize(UINT nType, int cx, int cy) 
{
    CSizingControlBar::OnSize(nType, cx, cy);
	CRect rect;
	GetClientRect(rect);
	rect.bottom = rect.bottom - m_nTabHeight;
	rect.DeflateRect(4, 4, 4, 2);	

	int count = m_listChildWnd.GetSize();
	for(int i = 0; i < count; i++)
	{
		::MoveWindow(m_listChildWnd[i].pPageWnd->m_hWnd, rect.left, rect.top,
			rect.Width(), rect.Height(), TRUE);
	}

	Invalidate();
}

//////////////////////////////////////////////////////////
//功能:移动Tab向左或右一页
//参数:bLeftOrRight[in]		移动方向
//////////////////////////////////////////////////////////
void CTabBar::TabScroll(BOOL bLeftOrRight)
{
	int count = m_listChildWnd.GetSize();

	if(bLeftOrRight)
	{	//左移
		if(m_firstTabID < count - 1)
			m_firstTabID++;
		else
			return;
	}
	else
	{	//右移
		if(m_firstTabID > 0)
			m_firstTabID--;
		else
			return;
	}

	UpdateScrollArrows();
	Invalidate();
}

/////////////////////////////////////////////////////////
//功能:更新当前箭头状态
/////////////////////////////////////////////////////////
void CTabBar::UpdateScrollArrows()
{
	if(m_firstTabID <= 0)
		m_nStateLArrow |= STATE_DISABLE;
	else
		m_nStateLArrow &= ~STATE_DISABLE;

	if(m_firstTabID >= m_listChildWnd.GetSize() - 1)
		m_nStateRArrow |= STATE_DISABLE;
	else
		m_nStateRArrow &= ~STATE_DISABLE;

	CRect rect;
	GetTabBarRect(&rect);
	InvalidateRect(&rect);
}

////////////////////////////////////////////////////////////////////////
//功能:加入一个页面
//参数:pPage[in]		子窗口对象
//	strName[in]			对应的名称
//	nIconIndex[in]		对应的图标索引号
//返回:成功子窗口对引的索引号,失败-1
//说明:bmp在函数调用完后,不为对象保留,可以删除
///////////////////////////////////////////////////////////////////////
int CTabBar::AddPage(CWnd *pPage, const CString &strName, int nIconIndex)
{
	ASSERT(pPage);

	CTabBar::CTabPageInfo info;
	info.str = strName;
	info.pPageWnd = pPage;
	info.indexIcon = nIconIndex;

	return m_listChildWnd.Add(info);
}

/////////////////////////////////////////////////////////////
//功能:设置Tab上所用的图标
//参数:pNewImage[in]		Tab上用的图标,应用new创建, 由本对象管理
/////////////////////////////////////////////////////////////
void CTabBar::SetImageList(CImageList* pNewImage)
{
	ASSERT(pNewImage);

	if (m_pIconList != NULL)
		delete m_pIconList;
	m_pIconList = pNewImage;

	int nCount = pNewImage->GetImageCount();
	IMAGEINFO info;
	if (nCount > 0 && pNewImage->GetImageInfo( 0, &info ))
	{
		m_nImageWidth = info.rcImage.right - info.rcImage.left;
		m_nTabWidth = m_nImageWidth * 7 / 2;
		m_nTabHeight = (info.rcImage.bottom - info.rcImage.top) * 4 / 3;
	}

	Invalidate();
}

/////////////////////////////////////////////////////////////////////////
//功能:设置bar的风格
/////////////////////////////////////////////////////////////////////////
void CTabBar::SetTabMode(int nMode)
{
	if(nMode != TABBAR_MODE_DYNAMIC && nMode != TABBAR_MODE_STATIC)
	{
		TRACE("UnSoportted mode!");
		ASSERT(FALSE);
		return;
	}

	m_nBarStyle = nMode;
	Invalidate();
}
