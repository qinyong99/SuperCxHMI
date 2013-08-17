// BarListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "cxdev.h"
#include "BarListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#ifndef WM_NCMOUSELEAVE
//#define WM_NCMOUSELEAVE                 0x02A2
//#endif
//#ifndef TME_NONCLIENT
//#define TME_NONCLIENT   0x00000010 
//#endif

#define GLW_ICON_CLOSE	0
#define GLW_ICON_OPEN	1
#define GLW_HEADERSIZE	20
#define GLW_TOOLWIDTH	16
#define GLW_TOOLSTEP	4

//////////////////////////////////////////////////////////////////////////
//CSymBolLibWnd::CThisDropTarget成员, 用于拖放操作的函数
DROPEFFECT CBarListCtrl::CThisDropTarget::OnDragEnter(CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
{
	m_dwKeyStateSource = dwKeyState;
	return OnDragOver(pWnd, pDataObject, dwKeyState, point);
}

DROPEFFECT CBarListCtrl::CThisDropTarget::OnDragOver(CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
{
	if(pWnd != m_pParent)
		return DROPEFFECT_NONE;
	
	return ((CBarListCtrl*)m_pParent)->FindDragEffect(pWnd, pDataObject, dwKeyState, point);
}

DROPEFFECT CBarListCtrl::CThisDropTarget::OnDragScroll(CWnd *pWnd, DWORD dwKeyState, CPoint point)
{
	return DROPEFFECT_NONE;
}

DROPEFFECT CBarListCtrl::CThisDropTarget::OnDropEx(CWnd *pWnd, COleDataObject *pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point)
{
	DROPEFFECT efRet = ((CBarListCtrl*)m_pParent)->DoDrop(pWnd, pDataObject, dropDefault, dropList, point);
	pDataObject->Release();
	return efRet;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//功能:进行Drop操作
/////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CBarListCtrl::CThisDropTarget::OnDrop(CWnd *pWnd, COleDataObject *pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	pDataObject->Release();
	return TRUE;
}

void CBarListCtrl::CThisDropTarget::OnDragLeave(CWnd *pWnd)
{
	m_dwKeyStateSource = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CBarListCtrl

CBarListCtrl::CBarListCtrl()
{
	m_bMouseTracking = FALSE;
	m_nHoverButton = -1;
	m_nPushButton = -1;
}

CBarListCtrl::~CBarListCtrl()
{
}


BEGIN_MESSAGE_MAP(CBarListCtrl, CWnd)
	//{{AFX_MSG_MAP(CBarListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarListCtrl message handlers

BOOL CBarListCtrl::Create(CRect &rect, CWnd *pParent, UINT ID)
{
	return CWnd::Create(AfxRegisterWndClass(CS_DBLCLKS), "", WS_CHILD, rect, pParent, ID);
}

//////////////////////////////////////////////////////////
//功能:获取第index个button的位置
//参数:index[in]		button索引号
//		pRect[out]		返回的位置
//////////////////////////////////////////////////////////
void CBarListCtrl::GetButtonRect(int index, CRect *pRect) const
{
	CRect rect = GetToolBarRect();
//	CRect rectWnd;
//	GetWindowRect(&rectWnd);
//	rect.OffsetRect(-rectWnd.left, -rectWnd.top);
//	rect.DeflateRect(2, 2, 2, 2);

	rect.left = rect.left + index * (GLW_TOOLWIDTH + GLW_TOOLSTEP) + 2;
	rect.right = rect.left + GLW_TOOLWIDTH + GLW_TOOLSTEP;
	rect.bottom -= 0;
	rect.top += 1;

	*pRect = rect;
}

//////////////////////////////////////////////////////////
//功能: 当光标移动到按钮上时, 更新按钮显示
//参数:index[in]		按钮序号从0开始
//////////////////////////////////////////////////////////
void CBarListCtrl::HoverButton(int index)
{
	CRect rect;

	CDC* pdc = GetDC();
	if(index != m_nHoverButton && m_nHoverButton != -1)
	{
		GetButtonRect(m_nHoverButton, &rect);
		pdc->Draw3dRect(&rect, GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_3DFACE));
	}

	if(index >= 0 && index < 6)
	{
		GetButtonRect(index, &rect);
		if(m_nPushButton >= 0 && m_nPushButton == index)
			pdc->Draw3dRect(&rect, RGB(0, 0, 0), RGB(255, 255, 255));
		else
			pdc->Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0));
		m_nHoverButton = index;
	}
	else
		m_nHoverButton = -1;

	ReleaseDC(pdc);
}

////////////////////////////////////////////////////////////
//功能:判定point是否在某个工具按钮上
//返回:-1不在按钮上, > -1在第几个按钮上(从零开始)
////////////////////////////////////////////////////////////
int CBarListCtrl::HitestButton(CPoint point) const
{
	CRect rect = GetToolBarRect();
	rect.right = rect.left + GLW_TOOLWIDTH;
	int index = 0;

	do
	{
		if(rect.PtInRect(point))
			return index;
		rect.left = rect.right + GLW_TOOLSTEP;
		rect.right = rect.left + GLW_TOOLWIDTH;
	}while(index++ < 4);

	return -1;
}

///////////////////////////////////////////////////////////
//功能:返回工具条位置区的位置, 按屏幕坐标
///////////////////////////////////////////////////////////
CRect CBarListCtrl::GetToolBarRect() const
{
	CRect rect;

	GetClientRect(&rect);	
	rect.bottom = GLW_HEADERSIZE;
	return rect;
}

LRESULT CBarListCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bMouseTracking = FALSE;
	m_nPushButton = -1;
	HoverButton(-1);

	return 0;
}

int CBarListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_targetDrop.m_pParent = this;
	m_targetDrop.Register(this);

	CRect rect;
	GetClientRect(&rect);
	if(!m_wndListCtrl.Create(WS_CHILD | WS_VISIBLE, rect, this, CBARLISTCTRL_LISTID))
		return -1;

	m_wndListCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	if (!PreCreateImage(&m_listImageLarge, &m_listImage))
	{
		m_listImage.Create(IDB_GLIB_SMALL, 16, 2, RGB(255, 255, 255));
		m_listImageLarge.Create(IDB_GLIB_LARGE, 32, 2, RGB(255, 255, 255));
	}
	m_listImageTool.Create(IDB_GLIB_TOOL, GLW_TOOLWIDTH, 2, RGB(255, 255, 255));
	m_wndListCtrl.SetImageList(&m_listImage, LVSIL_SMALL);
	m_wndListCtrl.SetImageList(&m_listImageLarge, LVSIL_NORMAL);

	EnableToolTips(TRUE);

	return 0;
}

CListCtrl* CBarListCtrl::GetListCtrl()
{
	return &m_wndListCtrl;
}

void CBarListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect rect;
	GetClientRect(&rect);	
	rect.top = rect.Height() > GLW_HEADERSIZE ? rect.top + GLW_HEADERSIZE : rect.bottom;
	m_wndListCtrl.MoveWindow(&rect);
	m_wndListCtrl.Arrange(LVA_DEFAULT);
}

/////////////////////////////////////////////////////////////;
//功能:由子类继承,创建非默认的大小图标
//返回:TRUE已创建图标,不使用默认图标, FALSE未创建,使用默认图标
//////////////////////////////////////////////////////////////
BOOL CBarListCtrl::PreCreateImage(CImageList *pImageLarge, CImageList *pImageSmall)
{
	return FALSE;
}

int CBarListCtrl::OnToolHitTest(CPoint point, TOOLINFO *pTI) const
{
	CRect rect;
	int nIndex = HitestButton(point);
	if(nIndex < 0)
		return CWnd::OnToolHitTest(point, pTI);
	GetButtonRect(nIndex, &rect);

	pTI->hwnd = m_hWnd;
	pTI->uId = nIndex;
	pTI->rect.left = rect.left;
	pTI->rect.top = rect.top;
	pTI->rect.right = rect.right;
	pTI->rect.bottom = rect.bottom;
	pTI->lpszText = LPSTR_TEXTCALLBACK;

	return nIndex + 1;
}

BOOL CBarListCtrl::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;

	int nIndex = (int)(pTTT->hdr.idFrom);
	LPTSTR strTip;
	switch(nIndex)
	{
	case 0:
		strTip = _T("返回");
		break;
	case 1:
		strTip = _T("新建");
		break;
	case 2:
		strTip = _T("大图标");
		break;
	case 3:
		strTip = _T("小图标");
		break;
	case 4:
		strTip = _T("列表");
		break;
	default:
		strTip = _T("");
		break;
	};

#ifndef _UNICODE
		lstrcpyn(pTTT->szText, strTip, 80);
#else
		_mbstowcsz(pTTT->szText, strTip, 80);
#endif

	*pResult = 0;
	return TRUE;  
}

void CBarListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		//监控鼠标离开
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		  
		if (_TrackMouseEvent(&tme))
		{
			m_bMouseTracking = TRUE;
		}
	}

	CRect rect = GetToolBarRect();
	if (rect.PtInRect(point))
	{
		int index = HitestButton(point);
		HoverButton(index);
	}
}

void CBarListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int index = HitestButton(point);
	if (index >= 0)
	{
		m_nPushButton = index;
		HoverButton(index);
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CBarListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int index = HitestButton(point);
	if(index >= 0)
	{
		m_nPushButton = -1;
		HoverButton(index);
		DoCommand(index);
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CBarListCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(&rect);

	dc.FillSolidRect(&rect, GetSysColor(COLOR_3DFACE));
	for(int i = 0; i < GetSystemMetrics(SM_CYEDGE); i++)
	{
		dc.MoveTo(rect.left + 1, rect.bottom + i);
		dc.LineTo(rect.right - GetSystemMetrics(SM_CXEDGE), rect.bottom + i);
	}

	POINT pt;
	pt.x = pt.y = 3;
	for(i = 0; i < 6; i++)
	{
		m_listImageTool.Draw(&dc, i, pt, ILD_TRANSPARENT);
		pt.x += GLW_TOOLWIDTH + GLW_TOOLSTEP;
	}
	
	// Do not call CWnd::OnPaint() for painting messages
}
