// RunView.cpp : implementation of the CRunView class
//
//2002.11.4
//改事件为无参数事件，实现类似与IE，但可能存在一些问题，测试过keydown事件
//	keydown事件使用方式
//		Sub Rectangle1_KeyDown()
//		MsgBox window.event.keycode		
//		window.event.cancelBubble = True
//		End Sub


#include "stdafx.h"
#include "RunInc.h"
#include "RunCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

short _GetShiftState()
{
	BOOL bShift = (GetKeyState(VK_SHIFT) < 0);
	BOOL bCtrl  = (GetKeyState(VK_CONTROL) < 0);
	BOOL bAlt   = (GetKeyState(VK_MENU) < 0);

	return (short)(bShift + (bCtrl << 1) + (bAlt << 2));
}

static void AFXAPI _PostTrailByte(CWnd* pWnd, BYTE bTrailByte)
{
	// Force new trail byte to the front of the queue.
	pWnd->PostMessage(WM_QUEUE_SENTINEL);
	pWnd->PostMessage(WM_CHAR, bTrailByte);
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOYIELD | PM_REMOVE) &&
		(msg.message != WM_QUEUE_SENTINEL))
	{
		::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	}

	ASSERT(msg.message == WM_QUEUE_SENTINEL);
	ASSERT(msg.hwnd == pWnd->m_hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// CRunView

static char szAllZeroInvalidMask[16] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

IMPLEMENT_DYNCREATE(CRunView, CScrollView)

BEGIN_MESSAGE_MAP(CRunView, CScrollView)
	//{{AFX_MSG_MAP(CRunView)
	ON_WM_TIMER()
	ON_WM_MOUSEACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_DEADCHAR()
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()
	ON_WM_SYSDEADCHAR()
	ON_WM_CANCELMODE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE_VOID(WM_PAINTEX, OnPaintEx)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
//	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipText)
	ON_NOTIFY (UDM_TOOLTIP_DISPLAY, NULL, NotifyDisplayTooltip)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunView construction/destruction

HCURSOR CRunView::m_hHandCursors[] = 
{
	NULL,
	NULL,
	NULL,
};

CRunView::CRunView()
{
	m_bLockDraw = FALSE;

	m_iButtonState = 0;
	m_iDblClkState = 0;

	m_iButton = 0;
	m_iKeyCode = 0;

	m_pEventObject = NULL;

	m_bZoomFitWindow = FALSE;
	m_fZoomRatio = 1.0f;
	m_fZoomX = -1.0f;
	m_fZoomY = -1.0f;

	m_bAllowMouseZoom = FALSE;
	m_bAllowScrollBars = FALSE;
	m_bDragMode = FALSE;

	// initialize zoom state (100% from size cache)
	m_zoomNum = CSize(100, 100);
	m_zoomDenom = CSize(100, 100);

	m_nScanTimer = -1;
	m_nStandTimer = -1;
	m_bTimerEnabled = TRUE;

	m_pRunCtrl = NULL;

	m_bDestory = FALSE;
	m_bNoClosePicture = FALSE;

	Reset();
}

CRunView::~CRunView()
{
}

void CRunView::Reset()
{
	m_bEnabled = TRUE;
	m_pObjectWithCapture = NULL;
	m_pObjectWithFocus = NULL;
	m_pItemToDeactivateOnLeave =  NULL;
	m_pObjectTrackMouseLeave = NULL;
	m_bMouseTracking = FALSE;
	m_pObjectHighlight = NULL;
	m_pEventObject = NULL;
}

BOOL CRunView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CScrollView::PreCreateWindow(cs))
	{
		return FALSE;
	}

//	cs.style &= ~WS_BORDER;
	cs.style |= WS_CLIPCHILDREN;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.dwExStyle &= ~WS_EX_WINDOWEDGE;
//	cs.dwExStyle |= WS_EX_TOPMOST;   

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRunView drawing

void CRunView::OnDraw(CDC* pDC)
{
	if (m_bLockDraw)
		return;

	CFormDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap;

	// only paint the rect that needs repainting
	CRect client;
	pDC->GetClipBox(client);
	client.InflateRect(1, 1);
	CRect rect = client;
	DocToClient(rect);

	if (!pDC->IsPrinting())
	{
		// draw to offscreen bitmap for fast looking repaints
		if (dc.CreateCompatibleDC(pDC))
		{
			if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			{
				OnPrepareDC(&dc, NULL);
				pDrawDC = &dc;

				// offset origin more because bitmap is just piece of the whole drawing
				dc.OffsetViewportOrg(-rect.left, -rect.top);
				pOldBitmap = dc.SelectObject(&bitmap);
				dc.SetBrushOrg(rect.left % 8, rect.top % 8);

				// might as well clip to the same rectangle
				dc.IntersectClipRect(client);
			}
		}
	}

	CRect rcBackGround(0, 0, (int)pDoc->GetWidth(), (int)pDoc->GetHeight());
	CRect rcFill;
	rcFill.IntersectRect(&rcBackGround, &client);

	if (pDoc->m_pImageBackGround != NULL)
	{	
		using namespace Gdiplus;

		TextureBrush tBrush(pDoc->m_pImageBackGround);
		Graphics graphics(pDrawDC->GetSafeHdc());
		graphics.FillRectangle(&tBrush, 
				Rect(rcFill.left, rcFill.top, rcFill.Width(), rcFill.Height()));
	}
	else
	{
		// paint background
		COLORREF clrBackColor;
		OleTranslateColor(pDoc->m_clrBackColor, 0, &clrBackColor);
		CBrush backBrush(clrBackColor);
		pDrawDC->FillRect(rcFill, &backBrush);
	}

	CBrush backBrush(GetSysColor(COLOR_APPWORKSPACE));
	if (client.left < -1)
	{
		rcFill.SetRect(client.left, rcBackGround.top, rcBackGround.left, rcBackGround.bottom);
		pDrawDC->FillRect(rcFill, &backBrush);
	}
	if (client.top < -1)
	{
		rcFill.SetRect(client.left, client.top, client.right, rcBackGround.top);
		pDrawDC->FillRect(rcFill, &backBrush);
	}
	if (client.right > rcBackGround.right)
	{
		rcFill.SetRect(rcBackGround.right, rcBackGround.top, client.right, rcBackGround.bottom);
		pDrawDC->FillRect(rcFill, &backBrush);
	}
	if (client.bottom > rcBackGround.bottom)
	{
		rcFill.SetRect(client.left, rcBackGround.bottom, client.right, client.bottom);
		pDrawDC->FillRect(rcFill, &backBrush);
	}

	pDoc->DrawDoc(pDrawDC);

	DrawHighlightObject(pDrawDC);

	if (pDrawDC != pDC)
	{
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0,0);
		pDC->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0,0);
		dc.SetMapMode(MM_TEXT);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
			&dc, 0, 0, SRCCOPY);
		dc.SelectObject(pOldBitmap);
	}
}

void CRunView::OnPaintEx()
{
//	MSG msg;
//	while (::PeekMessage(&msg, NULL, WM_PAINTEX, WM_PAINTEX, PM_REMOVE|PM_NOYIELD));
	
	CClientDC dc(this);

	if (GetDocument()->IsAllowDirectDraw())
	{
		OnDrawDirect(&dc);
		
		POSITION posObj;
		CRect* pUpdateBound;
		CObject* pObj;

		posObj = m_mapDirectDraw.GetStartPosition();
		while (posObj != NULL)
		{
			m_mapDirectDraw.GetNextAssoc(posObj, (void *&)pObj, (void *&)pUpdateBound);
			if (pUpdateBound != NULL)
				delete pUpdateBound;
		}

		m_mapDirectDraw.RemoveAll();
		m_rcDirectDraw.SetRectEmpty();
	}
	else
	{
		OnDrawEx(&dc);
		memset(m_wInvalidAreaMask, 0, 16);
	}
}

void CRunView::OnDrawDirect(CDC* pDC)
{
	//PROMPTT_TIME_ELAPSE;

	CFormDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap;
	
	// only paint the rect that needs repainting
	CRect client = m_rcDirectDraw;
	client.InflateRect(1, 1);
	CRect rect = client;
	DocToClient(rect);
	
	// draw to offscreen bitmap for fast looking repaints
	if (dc.CreateCompatibleDC(pDC))
	{
		if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
		{
			OnPrepareDC(&dc, NULL);
			pDrawDC = &dc;
			
			// offset origin more because bitmap is just piece of the whole drawing
			dc.OffsetViewportOrg(-rect.left, -rect.top);
			pOldBitmap = dc.SelectObject(&bitmap);
			dc.SetBrushOrg(rect.left % 8, rect.top % 8);
			
			// might as well clip to the same rectangle
			dc.IntersectClipRect(client);
		}
	}
	
	CRect rcBackGround(0, 0, (int)pDoc->GetWidth(), (int)pDoc->GetHeight());
	CRect rcFill;
	rcFill.IntersectRect(&rcBackGround, &client);
	
	if (pDoc->m_pImageBackGround != NULL)
	{	
		using namespace Gdiplus;
		
		TextureBrush tBrush(pDoc->m_pImageBackGround);
		Graphics graphics(pDrawDC->GetSafeHdc());
		graphics.FillRectangle(&tBrush, 
			Rect(rcFill.left, rcFill.top, rcFill.Width(), rcFill.Height()));
	}
	else
	{
		// paint background
		COLORREF clrBackColor;
		OleTranslateColor(pDoc->m_clrBackColor, 0, &clrBackColor);
		CBrush backBrush(clrBackColor);
		pDrawDC->FillRect(rcFill, &backBrush);
	}
	
	CDevObj* pObj;
	CRect* pUpdateBound;
	POSITION posObj;
	CRect rcObj;

	//for (int i = 0; i < 2000; i++)
	{
		posObj = m_mapDirectDraw.GetStartPosition();
		while (posObj != NULL)
		{
			m_mapDirectDraw.GetNextAssoc(posObj, (void *&)pObj, (void *&)pUpdateBound);
			
			if ((pDoc->m_ulVisibleLayer & (1 << pObj->m_nLayer)) != 0)
				pObj->Draw(pDrawDC);
		}
	}
	
	DrawHighlightObject(pDrawDC);
	
	if (pDrawDC != pDC)
	{
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0,0);
		pDC->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0,0);
		dc.SetMapMode(MM_TEXT);

		posObj = m_mapDirectDraw.GetStartPosition();
		while (posObj != NULL)
		{
			m_mapDirectDraw.GetNextAssoc(posObj, (void *&)pObj, (void *&)pUpdateBound);

			if (pUpdateBound != NULL)
			{
				rcObj = pUpdateBound;
				delete pUpdateBound;
			}
			else
			{
				rcObj = pObj->GetBoundingRect();
			}

			DocToClient(rcObj);
			
 			pDC->BitBlt(rcObj.left, rcObj.top, rcObj.Width(), rcObj.Height(),
 				&dc, rcObj.left - rect.left, rcObj.top - rect.top, SRCCOPY);

			//pDC->DrawFocusRect(&rcObj);
		}

		m_mapDirectDraw.RemoveAll();
		
		dc.SelectObject(pOldBitmap);
	}
}

#undef CopyRgn

void CRunView::OnDrawEx(CDC* pDC)
{
	CFormDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap;

	CRgn rgnClip;

	// only paint the rect that needs repainting
	int x1, x2;
	int y1, y2;
	int nXPos, nYPos;
	BOOL bFirstInvaidArea = TRUE;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			if (m_wInvalidAreaMask[y] & (1 << x))
			{
				nXPos = x * m_nAreaWidth;
				nYPos = y * m_nAreaHeight;

				if (bFirstInvaidArea)
				{
					bFirstInvaidArea = FALSE;
					x1 = x2 = x;
					y1 = y2 = y;
					
					CRect& rcArea = m_arrAreaRects[y][x];
					CRgn& rgn = m_arrAreaRgns[y][x];
					if (rcArea.IsRectEmpty())
					{
						rcArea.left = nXPos;
						rcArea.top = nYPos;
						rcArea.right = nXPos + m_nAreaWidth;
						rcArea.bottom = nYPos + m_nAreaHeight;
						DocToClient(rcArea);
						rgn.DeleteObject();
						rgn.CreateRectRgn(rcArea.left - 1, rcArea.top - 1, rcArea.right + 1, rcArea.bottom + 1);
					}
					rgnClip.CreateRectRgn(rcArea.left - 1, rcArea.top - 1, rcArea.right + 1, rcArea.bottom + 1);
				}
				else
				{
					if (x1 > x)
						x1 = x;
					if (x2 < x)
						x2 = x;
				//	if (y1 > y)
				//		y1 = y;
	 				if (y2 < y)
						y2 = y;

					CRect& rcArea = m_arrAreaRects[y][x];
					CRgn& rgn = m_arrAreaRgns[y][x];
					if (rcArea.IsRectEmpty())
					{
						rcArea.left = nXPos;
						rcArea.top = nYPos;
						rcArea.right = nXPos + m_nAreaWidth;
						rcArea.bottom = nYPos + m_nAreaHeight;
						DocToClient(rcArea);
						rgn.DeleteObject();
						rgn.CreateRectRgn(rcArea.left - 1, rcArea.top - 1, rcArea.right + 1, rcArea.bottom + 1);
					}
					rgnClip.CombineRgn(&rgnClip, &rgn, RGN_OR);
				}
			}
		}
	}
	if (bFirstInvaidArea)
		return;
	x2++;
	y2++;

	// only paint the rect that needs repainting
	CRect client;
	client.SetRect(x1 * m_nAreaWidth, y1 * m_nAreaHeight, x2 * m_nAreaWidth, y2 * m_nAreaHeight);
	client.InflateRect(1, 1);

	CRect rect = client;
	DocToClient(rect);

	CRect rcClient;
	GetClientRect(&rcClient);

	// 如果需要绘制的区块与可以显示的区块不重叠，则不绘制
	if (!rcClient.IntersectRect(&rcClient, &rect))
		return;
		
	// draw to offscreen bitmap for fast looking repaints
	if (dc.CreateCompatibleDC(pDC))
	{
		if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
		{
			OnPrepareDC(&dc, NULL);
			pDrawDC = &dc;

			// offset origin more because bitmap is just piece of the whole drawing
			dc.OffsetViewportOrg(-rect.left, -rect.top);
			pOldBitmap = dc.SelectObject(&bitmap);
			dc.SetBrushOrg(rect.left % 8, rect.top % 8);

			rgnClip.OffsetRgn(-rect.left, -rect.top);
			dc.SelectClipRgn(&rgnClip);

			// 取与可显示区块的交集
			ClientToDoc(rcClient);
			dc.IntersectClipRect(rcClient);
		}
	}

	if (pDoc->m_pImageBackGround != NULL)
	{	
		using namespace Gdiplus;

		TextureBrush tBrush(pDoc->m_pImageBackGround);
		Graphics graphics(pDrawDC->GetSafeHdc());
		graphics.FillRectangle(&tBrush, 
				Rect(client.left, client.top, client.Width(), client.Height()));
	}
	else
	{
		// paint background
		COLORREF clrBackColor;
		OleTranslateColor(pDoc->m_clrBackColor, 0, &clrBackColor);
		CBrush backBrush(clrBackColor);
		pDrawDC->FillRect(client, &backBrush);
	}
	
/////////////////////////////////////////////////////////////

	DWORD* pInvalidMask = (DWORD *)m_wInvalidAreaMask; 
	CDevObj* pObj;
	CDevObjList* pObjects = pDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	while (posObj != NULL)
	{
		pObj = (CDevObj*)pObjects->GetNext(posObj);
		DWORD* pAreaMask = (DWORD *)pObj->m_wAreaMask; 
		if ((pInvalidMask[0] & pAreaMask[0]) || 
			(pInvalidMask[1] & pAreaMask[1]) || 
			(pInvalidMask[2] & pAreaMask[2]) || 
			(pInvalidMask[3] & pAreaMask[3]))
		{
			if ((pDoc->m_ulVisibleLayer & (1 << pObj->m_nLayer)) != 0)
				pObj->Draw(pDrawDC);
			UpdateObjAreaMask(pObj);
		}
	}

	DrawHighlightObject(pDrawDC);

/////////////////////////////////////////////////////////////


	if (pDrawDC != pDC)
	{
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0,0);
		pDC->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0,0);
		dc.SetMapMode(MM_TEXT);

		for (int y = y1; y < y2; y++)
		{
			for (int x = x1; x < x2; x++)
			{
				if (m_wInvalidAreaMask[y] & (1 << x))
				{
					CRect& rcArea = m_arrAreaRects[y][x];
					pDC->BitBlt(rcArea.left, rcArea.top, rcArea.Width(), rcArea.Height(),
							&dc, rcArea.left - rect.left, rcArea.top - rect.top, SRCCOPY);
//#ifdef _DEBUG
//					CString str;
//					str.Format("%d, %d", x, y);
//					pDC->TextOut(rcArea.left, rcArea.top, str);
//#endif
				}
			}
		}

		dc.SelectObject(pOldBitmap);
	}

}

void CRunView::DrawHighlightObject(CDC* pDC)
{
	if (m_pObjectHighlight != NULL)
	{
		CPen pen(PS_SOLID, 0, RGB(0, 0, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);
		CBrush* pOldBrush = pDC->SelectObject(CBrush::FromHandle((HBRUSH)::GetStockObject(NULL_BRUSH)));
		int nDrawMode = pDC->SetROP2(R2_NOT);
		
		CRect rect = m_pObjectHighlight->GetBoundingRect();
		
		rect.InflateRect(2, 2);
		pDC->Rectangle(&rect);
		rect.InflateRect(2, 2);
		pDC->Rectangle(&rect);
		
		pDC->SetROP2(nDrawMode);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
}

//////////////////////////////////////////////////////////////////////////
//功 能：设置缩放因子
//////////////////////////////////////////////////////////////////////////
BOOL CRunView::SetZoomFactor(CSize zoomNum, CSize zoomDenom)
{
	if (zoomDenom != m_zoomDenom || zoomNum != m_zoomNum)
	{
		// sync to new zoom factor
		m_zoomNum = zoomNum;
		m_zoomDenom = zoomDenom;

		memset(m_arrAreaRects, 0, sizeof(CRect) * 128);
		GetDocument()->UpdateCtrlItemsPosition();

		// resync to new sizes
		Invalidate();
		return TRUE;
	}
	return FALSE;
}

void CRunView::SetScrollSizes2(int nMapMode, SIZE sizeTotal,
	const SIZE& sizePage, const SIZE& sizeLine)
{
	ASSERT(sizeTotal.cx >= 0 && sizeTotal.cy >= 0);
	ASSERT(nMapMode > 0);
	ASSERT(nMapMode != MM_ISOTROPIC && nMapMode != MM_ANISOTROPIC);

	int nOldMapMode = m_nMapMode;
	m_nMapMode = nMapMode;
	m_totalLog = sizeTotal;

	//BLOCK: convert logical coordinate space to device coordinates
	{
		CWindowDC dc(NULL);
		ASSERT(m_nMapMode > 0);
		dc.SetMapMode(m_nMapMode);

		// total size
		m_totalDev = m_totalLog;
		dc.LPtoDP((LPPOINT)&m_totalDev);
		m_pageDev = sizePage;
		dc.LPtoDP((LPPOINT)&m_pageDev);
		m_lineDev = sizeLine;
		dc.LPtoDP((LPPOINT)&m_lineDev);
		if (m_totalDev.cy < 0)
			m_totalDev.cy = -m_totalDev.cy;
		if (m_pageDev.cy < 0)
			m_pageDev.cy = -m_pageDev.cy;
		if (m_lineDev.cy < 0)
			m_lineDev.cy = -m_lineDev.cy;
	} // release DC here

	// now adjust device specific sizes
	ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);
	if (m_pageDev.cx == 0)
		m_pageDev.cx = m_totalDev.cx / 10;
	if (m_pageDev.cy == 0)
		m_pageDev.cy = m_totalDev.cy / 10;
	if (m_lineDev.cx == 0)
		m_lineDev.cx = m_pageDev.cx / 10;
	if (m_lineDev.cy == 0)
		m_lineDev.cy = m_pageDev.cy / 10;

	if (m_hWnd != NULL)
	{
		// window has been created, invalidate now
		if (m_bAllowScrollBars)
			UpdateBars();
		if (nOldMapMode != m_nMapMode)
			Invalidate(TRUE);
	}
}

//////////////////////////////////////////////////////////////////////////
//功 能：设置卷动信息
//////////////////////////////////////////////////////////////////////////
void CRunView::SetScrollInfo()
{
	CFormDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);
	
	CSize sizeTotal;
	sizeTotal.cx = (int)pDoc->GetWidth();
	sizeTotal.cy = (int)pDoc->GetHeight();

	sizeTotal.cx = MulDiv(sizeTotal.cx, m_zoomNum.cx, m_zoomDenom.cx);
	sizeTotal.cy = MulDiv(sizeTotal.cy, m_zoomNum.cy, m_zoomDenom.cy);

	if (!pDoc->IsMainForm())
	{
		CRect rect;
		GetClientRect(&rect);
		SetScrollSizes2(MM_TEXT, rect.Size());
	}
	else
	{
		SetScrollSizes2(MM_TEXT, sizeTotal);	
	}
}

void CRunView::SetAllowMouseZoom(BOOL bAllow)
{
	m_bAllowMouseZoom = bAllow;
}

void CRunView::SetAllowScrollBars(BOOL bAllow)
{
	m_bAllowScrollBars = bAllow;

	if (m_hWnd != NULL)
	{
		if (m_bAllowScrollBars)
		{
			UpdateBars();
		}
		else
		{
			// reset and turn any scroll bars off
			if (GetStyle() & (WS_HSCROLL|WS_VSCROLL))
			{
				SetScrollPos(SB_HORZ, 0);
				SetScrollPos(SB_VERT, 0);
				EnableScrollBarCtrl(SB_BOTH, FALSE);
				ASSERT((GetStyle() & (WS_HSCROLL|WS_VSCROLL)) == 0);
			}
		}
	}
}

void CRunView::SetDragMode(BOOL b)
{
	m_bDragMode = b;
}

void CRunView::SetZoomFitWindow(BOOL bFitWindow, float fZoomRatio, float fZoomX, float fZoomY)
{
	m_bZoomFitWindow = bFitWindow;
	m_fZoomRatio = fZoomRatio;

	BOOL bUpdate = FALSE;
	if (m_fZoomX != fZoomX)
	{
		bUpdate = TRUE;
		m_fZoomX = fZoomX;
	}
	if (m_fZoomY != fZoomY)
	{
		bUpdate = TRUE;
		m_fZoomY = fZoomY;
	}
	
	if (bUpdate)
		Invalidate();

	Zoom();
}

void CRunView::Zoom()
{
	if (m_bZoomFitWindow)
	{
		CFormDoc* pDoc = GetDocument();
		if (pDoc != NULL)
		{
			CRect rcClient;
			GetClientRect(rcClient);
			CSize zoomNum(rcClient.Width(), rcClient.Height());
			CSize zoomDenom((int)pDoc->GetWidth(), (int)pDoc->GetHeight());
			SetZoomFactor(zoomNum, zoomDenom);
		}
	}
	else
	{
		CSize zoomNum(int(m_fZoomRatio * 10000), int(m_fZoomRatio * 10000));
		CSize zoomDenom(10000, 10000);
		SetZoomFactor(zoomNum, zoomDenom);
	}

	SetScrollInfo();
}

void CRunView::ClientToDoc(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(&point);
}

void CRunView::ClientToDoc(CSize& size)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(&size);
}

void CRunView::ClientToDoc(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(rect);
}

void CRunView::DocToClient(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&point);
}

void CRunView::DocToClient(CSize& size)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&size);
}

void CRunView::DocToClient(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	rect.NormalizeRect();
}

void CRunView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CScrollView::OnPrepareDC(pDC, pInfo);

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportExt(m_zoomNum);
	pDC->SetWindowExt(m_zoomDenom);

	if (m_fZoomX != -1.0f && m_fZoomY != -1.0f)
	{
		pDC->SetWindowOrg((int)m_fZoomX, (int)m_fZoomY);
	}

	CFormObj* pFormObj = GetDocument()->m_pFormObj;
	if (pFormObj != NULL)
	{
		CRect rcPos = pFormObj->m_position.GetRECT();
		pDC->SetWindowOrg(rcPos.TopLeft());
	}
}

int CRunView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//	EnableToolTips(TRUE);   // enable tool tips for view
	m_tooltip.Create(this, TRUE);
	m_tooltip.SetBehaviour(PPTOOLTIP_MULTIPLE_SHOW);
	m_tooltip.AddTool(this, NULL/*, IDI_TOOLTIP*/);
	m_tooltip.SetNotify();
	
	UINT nStyle = m_tooltip.GetStyles();
	//nStyle &= ~PPTOOLTIP_ROUNDED;
	m_tooltip.SetStyles(nStyle);
	m_tooltip.SetSize(CPPToolTip::PPTTSZ_ROUNDED_CX, 4);
	m_tooltip.SetSize(CPPToolTip::PPTTSZ_ROUNDED_CY, 4);
	m_tooltip.SetSize(CPPToolTip::PPTTSZ_MARGIN_CX, 3);
	m_tooltip.SetSize(CPPToolTip::PPTTSZ_MARGIN_CY, 2);

	return 0;
}

void CRunView::OnInitialUpdate()
{
	CFormDoc* pDoc = GetDocument();

	m_pRunCtrl = pDoc->m_pDocument->m_pRunCtrl;

	if (pDoc->IsMainForm())
		m_pRunCtrl->FirePictrueSize((int)pDoc->GetWidth(), (int)pDoc->GetHeight());

	SetScrollInfo();

	UpdateOptimizeDrawPara();

	CScrollView::OnInitialUpdate();

//	FireInitialize();
}

void CRunView::UpdateOptimizeDrawPara()
{
	CFormDoc* pDoc = GetDocument();

	m_nAreaWidth = ((int)pDoc->GetWidth() + 15) / 16;
	m_nAreaHeight =  ((int)pDoc->GetHeight() + 7) / 8;

	memset(m_arrAreaRects, 0, sizeof(CRect) * 128); // 128 = 8 * 16

	CDevObj* pObj;
	CDevObjList* pObjects = GetDocument()->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	while (posObj != NULL)
	{
		pObj = (CDevObj*)pObjects->GetNext(posObj);
		UpdateObjAreaMask(pObj);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRunView printing

BOOL CRunView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRunView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRunView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CRunView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	
}

void CRunView::OnSize(UINT nType, int cx, int cy)
{
	CFormDoc* pDoc = GetDocument();

	if (m_bAllowScrollBars)
		CScrollView::OnSize(nType, cx, cy);
	else
		CView::OnSize(nType, cx, cy);

	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
		pActiveItem->SetItemRects();

	if (m_bZoomFitWindow)
		Zoom();
	else if (!pDoc->IsMainForm())
		SetScrollInfo();

	UpdateOptimizeDrawPara();
	GetDocument()->FireEvent(CXEVENID_RESIZE, EVENT_PARAM(VTS_NONE));
}

BOOL CRunView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	memset(m_arrAreaRects, 0, sizeof(CRect) * 128); // 128 = 8 * 16
	return CScrollView::OnScrollBy(sizeScroll, bDoScroll);
}

////////////////////////////////////////////////////////////////////////////
// CRunView diagnostics

#ifdef _DEBUG
void CRunView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CRunView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFormDoc* CRunView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFormDoc)));
	return (CFormDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRunView message handlers

BOOL CRunView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CRunView::Refresh(BOOL bAll)
{
	FireEvent(CXEVENID_SCAN);

	if (bAll)
		memset(m_wInvalidAreaMask, -1, 16);

	CFormDoc* pDoc = GetDocument();
	pDoc->ParseDynamic();
	CDevObjList* pObjList = pDoc->GetDynamicObjects();
	POSITION posObj = pObjList->GetHeadPosition();
	CDevObj* pObj;

	while (posObj != NULL)
	{
		pObj = (CDevObj*)pObjList->GetNext(posObj);
		pObj->ParseDynamic();
	}

	// 2005.11.19 注释下面语句，因某些情况画面无法完全刷新
 	if (bAll)
 		memset(m_wInvalidAreaMask, 0, 16);
}

void CRunView::OnTimer(UINT nIDEvent) 
{
	if (!m_bTimerEnabled)
		return;

	if (nIDEvent == m_nScanTimer)
	{
		Refresh();
		return;
	}
	
	// 2006.12.22 日注释掉
//	memset(m_wInvalidAreaMask, 0, 16);

	CDevObjList* pObjList = GetDocument()->GetTimerObjects();
	POSITION posObj = pObjList->GetHeadPosition();
	CDevObj* pObj;

	while (posObj != NULL)
	{
		pObj = (CDevObj*)pObjList->GetNext(posObj);
		pObj->OnTimer();
	}
}

void CRunView::ReSetTimer(BOOL bScan, BOOL bStand, BOOL bSet)
{
	if (bSet)
	{
		if (bScan)
		{
			if (m_nScanTimer != -1)
				KillTimer(m_nScanTimer);
			m_nScanTimer = SetTimer(ID_SCANTIMER, GetDocument()->m_nScanInterval, NULL);
		}
		if (bStand)
		{
			if (m_nStandTimer != -1)
				KillTimer(m_nStandTimer);
			m_nStandTimer = SetTimer(ID_STANDTIMER, STANDTIMER_INTERVAL, NULL);
		}
	}
	else
	{
		BOOL bHasSet = FALSE;

		if (bScan && m_nScanTimer != -1)
		{
			KillTimer(m_nScanTimer);
			m_nScanTimer = -1;
			bHasSet = TRUE;
		}
		
		if (bStand && m_nStandTimer != -1)
		{
			KillTimer(m_nStandTimer);
			m_nStandTimer = -1;
			bHasSet = TRUE;
		}

		if (bHasSet && m_nScanTimer == -1 && m_nStandTimer == -1)
		{
			MSG msg;
			while (::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE|PM_NOYIELD));
		}
	}
}

int CRunView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	if (m_pRunCtrl->m_bInPlaceActive && !m_pRunCtrl->m_bUIActive)
		m_pRunCtrl->m_bPendingUIActivation = TRUE;
	
	return MA_ACTIVATE;
//	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

///////////////////////////////////////////////////////////////////////
//功 能：击中测试
///////////////////////////////////////////////////////////////////////
CDevObj* CRunView::HitTestObjects(CPoint point) const
{
	CFormDoc* pDoc = GetDocument();
	if (point.x >= 0 && point.x < (int)pDoc->GetWidth() &&
		point.y >= 0 && point.y < (int)pDoc->GetHeight())
	{
		int nRow = int(point.y / m_nAreaHeight);
		ASSERT(nRow <= 7);
		WORD wHitAreaMask = 1 << int(point.x / m_nAreaWidth);
	
		CDevObj* pObj;
		CDevObjList* pObjects = pDoc->GetObjects();
		POSITION posObj = pObjects->GetTailPosition();
		while (posObj != NULL)
		{
			pObj = (CDevObj*)pObjects->GetPrev(posObj);
			if (pObj->m_wAreaMask[nRow] & wHitAreaMask)
			{
				if (pObj->HitTest(point, this))
				{
					if ((pDoc->m_ulVisibleLayer & (1 << pObj->m_nLayer)) == 0)
						continue;
					if (pObj->m_nPrivilege > (int)theApp.m_dwUserPrivilege)
						return NULL;
									
					return pObj;
				}
			}
		}
	}

	return NULL;
}

void CRunView::TrackObjectMouseLeave(CDevObj* pObj)
{
	if (pObj != m_pObjectTrackMouseLeave)
	{
		if (m_pObjectTrackMouseLeave != NULL)
			m_pObjectTrackMouseLeave->OnMouseLeave();
		m_pObjectTrackMouseLeave = pObj;
	}
}

/////////////////////////////////////////////////////////////////////////
//功 能：处理无窗口对象鼠标消息
/////////////////////////////////////////////////////////////////////////
BOOL CRunView::HandleWindowlessMouseMessage(CPoint point)
{
	const MSG* pMessage;
	CDevObj* pObjHit;
	BOOL bHandled;

	pMessage = GetCurrentMessage();

	//首先看是否有项目捕获鼠标消息
	if (m_pObjectWithCapture != NULL)
	{
		m_pObjectWithCapture->OnWindowMessage(pMessage);
		return TRUE;
	}

	pObjHit = HitTestObjects(point);
	if (m_pObjectHighlight != pObjHit)
	{
		if (m_pObjectHighlight != NULL)
		{
			m_pObjectHighlight->Invalidate(this);
			m_pObjectHighlight = NULL;
		}
		if (theApp.m_bHighlightMouseObject && pObjHit != NULL && !pObjHit->IsKindOf(RUNTIME_CLASS(CCtrlObj)) && pObjHit->GetEnventSinkStatus())
		{
			m_pObjectHighlight = pObjHit;
			m_pObjectHighlight->Invalidate(this);
		}
	}

	if (pObjHit == NULL)
		TrackObjectMouseLeave(pObjHit);

	if (pObjHit != NULL && pObjHit->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
		if (!((CCtrlObj *)pObjHit)->m_pClientItem->IsWindowless())
			pObjHit = NULL;

	if (pObjHit != NULL)
	{
		bHandled = pObjHit->OnWindowMessage(pMessage);
		if (!bHandled)
		{
			// The windowless control didn't do anything, but we don't want the
			// container to do anything either.
			{
				Default();
			}
		}
		return TRUE;
	}

	return FALSE;
}

LRESULT CRunView::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bMouseTracking = FALSE;
	TrackObjectMouseLeave(NULL);

	return 0;
}

void CRunView::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_pRunCtrl->MouseMove(nFlags, point);

	if (m_bDragMode)
	{
		SetCursor(GetHandCursor(1));
		return;
	}

	if (!m_bEnabled)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return;
	}
	
	ClientToDoc(point);

	if (!m_bMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
 		if (_TrackMouseEvent(&tme))
			m_bMouseTracking = TRUE;
	}

	m_ptLastMousePosition = point;

	BOOL bHandled = FALSE; 
	if (IsCancelBubble() || ((bHandled = HandleWindowlessMouseMessage(point)) && IsCancelBubble()))
		return;

// 	if (IsCancelBubble() || HandleWindowlessMouseMessage(point) )
// 		return;
// 
 	if (IsCancelBubble() || HandlePointerInactiveMove(point, nFlags) || IsCancelBubble())
 		return;

	
// 	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	
	HWND hWndSave = m_hWnd;
	//FireMouseMove((short)m_iButtonState, _GetShiftState(), point.x, point.y);
	FireEvent(CXEVENID_MOUSEMOVE);
	if (m_hWnd != hWndSave)
		return;

	if (IsCancelBubble())
 		return;

	if (!bHandled)
		SetStandardCursor(GetDocument()->m_enumMousePointer);

//	Default();
}

BOOL CRunView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_bDragMode)
	{
		SetCursor(GetHandCursor(1));
		return TRUE;
	}

	if (HandlePointerInactiveSetCursor(message))
		return TRUE;

	if (HandleWindowlessMouseMessage(m_ptLastMousePosition))
		return TRUE;
	
	if (pWnd != this || m_bDragMode)
		return CScrollView::OnSetCursor(pWnd, nHitTest, message);

	return TRUE;
}

void CRunView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (CWnd::GetFocus() != this)
		SetFocus();

	m_pRunCtrl->ButtonDown(LEFT_BUTTON, nFlags, point);

	if (!m_bEnabled)
		return;

	ClientToDoc(point);
	
	if (IsCancelBubble() || (HandleWindowlessMouseMessage(point) && IsCancelBubble()))
		return;

	CDevObj* pObjHit;
	CCtrlItem* pItemHit = NULL;
	MSG msg;

	pObjHit = HitTestObjects(point);

	if (pObjHit != NULL && pObjHit->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
		pItemHit = ((CCtrlObj *)pObjHit)->m_pClientItem;

	if (pItemHit != NULL)
	{
		if (!pItemHit->IsInPlaceActive())
		{
			msg = *GetCurrentMessage();
			pItemHit->Activate(OLEIVERB_INPLACEACTIVATE, this, &msg);
			return;
		}
	}

	OnButtonDown(LEFT_BUTTON, nFlags, point);
}

void CRunView::OnRButtonDown(UINT nFlags, CPoint point) 
{
 	m_pRunCtrl->ButtonDown(RIGHT_BUTTON, nFlags, point);

	if (!m_bEnabled || m_bDragMode)
		return;

	ClientToDoc(point);

	if (IsCancelBubble() || (HandleWindowlessMouseMessage(point) && IsCancelBubble()))
		return;
	
	OnButtonDown(RIGHT_BUTTON, nFlags, point);
}

void CRunView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	m_pRunCtrl->OnButtonDown(MIDDLE_BUTTON, nFlags, point);

	if (!m_bEnabled || m_bDragMode)
		return;

	ClientToDoc(point);

	if (IsCancelBubble() || (HandleWindowlessMouseMessage(point) && IsCancelBubble()))
		return;
	
	OnButtonDown(MIDDLE_BUTTON, nFlags, point);
}

void CRunView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_pRunCtrl->OnButtonUp(LEFT_BUTTON, nFlags, point);

	if (!m_bEnabled || m_bDragMode)
		return;
	
	ClientToDoc(point);

	if (IsCancelBubble() || (HandleWindowlessMouseMessage(point) && IsCancelBubble()))
		return;

	OnButtonUp(LEFT_BUTTON, nFlags, point);
}

void CRunView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_pRunCtrl->OnButtonUp(RIGHT_BUTTON, nFlags, point);

	if (!m_bEnabled || m_bDragMode)
		return;

	ClientToDoc(point);
	
	if (IsCancelBubble() || (HandleWindowlessMouseMessage(point) && IsCancelBubble()))
		return;
	
	OnButtonUp(RIGHT_BUTTON, nFlags, point);
}

void CRunView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	m_pRunCtrl->OnButtonUp(MIDDLE_BUTTON, nFlags, point);

	if (!m_bEnabled || m_bDragMode)
		return;

	ClientToDoc(point);

	if (IsCancelBubble() || (HandleWindowlessMouseMessage(point) && IsCancelBubble()))
		return;
	
	OnButtonUp(MIDDLE_BUTTON, nFlags, point);
}

void CRunView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	m_pRunCtrl->OnButtonDblClk(LEFT_BUTTON, nFlags, point);

	if (!m_bEnabled || m_bDragMode)
		return;

	ClientToDoc(point);

	if (IsCancelBubble() || (HandleWindowlessMouseMessage(point) && IsCancelBubble()))
		return;
	
	OnButtonDblClk(LEFT_BUTTON, nFlags, point);

// 	if (m_bAllowMouseZoom && (nFlags & MK_CONTROL) != 0)
// 	{
// 		CRect rcClient;
// 		GetClientRect(rcClient);
// 
// 		OnScrollBy(rcClient.CenterPoint() - point, TRUE);
// 	}
}

void CRunView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	m_pRunCtrl->OnButtonDblClk(RIGHT_BUTTON, nFlags, point);

	if (!m_bEnabled || m_bDragMode)
		return;
	
	ClientToDoc(point);

	if (IsCancelBubble() || (HandleWindowlessMouseMessage(point) && IsCancelBubble()))
		return;
	
	OnButtonDblClk(RIGHT_BUTTON, nFlags, point);
}

void CRunView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
	m_pRunCtrl->OnButtonDblClk(MIDDLE_BUTTON, nFlags, point);

	if (!m_bEnabled || m_bDragMode)
		return;

	ClientToDoc(point);

	if (IsCancelBubble() || (HandleWindowlessMouseMessage(point) && IsCancelBubble()))
		return;

	OnButtonDblClk(MIDDLE_BUTTON, nFlags, point);
}

BOOL CRunView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (!m_bEnabled)
		return FALSE;
	
	if (m_pObjectWithFocus != NULL)
	{
		if (m_pObjectWithFocus->OnWindowMessage(GetCurrentMessage()))
			return TRUE;
	}

//	ClientToDoc(pt);
//	
//	if (!VerifySecurityArea(GetDocument()->m_nSecurityArea))
//		return FALSE;
//	
//	if (IsCancelBubble() || (HandleWindowlessMouseMessage(pt) && IsCancelBubble()))
//		return FALSE;


//	return FALSE;

	if (m_bAllowMouseZoom && (nFlags & MK_CONTROL) != 0)
	{
		int nCount = abs(zDelta) / WHEEL_DELTA;
		while (nCount--)
		{
			if (zDelta < 0)
				m_pRunCtrl->ZoomIn();
			else
				m_pRunCtrl->ZoomOut();
		}

		return TRUE;
	}
	
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CRunView::OnButtonDown(USHORT nButton, UINT nFlags, CPoint point)
{
	HWND hWndSave = m_hWnd;
	ButtonDown(nButton, nFlags, point);
	if (m_hWnd != hWndSave)
		return;

	Default();
}

void CRunView::OnButtonUp(USHORT nButton, UINT nFlags, CPoint point)
{
	HWND hWndSave = m_hWnd;
	Default();
	ButtonUp(nButton, nFlags, point);
	if (m_hWnd != hWndSave)
		return;
}

void CRunView::OnButtonDblClk(USHORT nButton, UINT nFlags, CPoint point)
{
	HWND hWndSave = m_hWnd;
	ButtonDblClk(nButton, nFlags, point);
	if (m_hWnd != hWndSave)
		return;

	Default();
}

/////////////////////////////////////////////////////////////////////////
//功 能：处理无窗口对象键盘消息
//描 述：只对当前获取焦点的对象
/////////////////////////////////////////////////////////////////////////
BOOL CRunView::HandleWindowlessKeyboardMessage()
{
   if (m_pObjectWithFocus != NULL)
   {
		if (m_pObjectWithFocus->OnWindowMessage(GetCurrentMessage()))
			return TRUE;
   }

   return FALSE;
}

BOOL CRunView::HandlePointerInactiveMove(CPoint point,
	DWORD dwKeyState)
{
	CDevObj* pObjHit = NULL;
	CCtrlItem* pItemHit = NULL;
	CRect rect;
	BOOL bHandled;
	DWORD dwPolicy;
	MSG msg;

    bHandled = FALSE;

	pObjHit = HitTestObjects(point);
	if (pObjHit != NULL && pObjHit->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
		pItemHit = ((CCtrlObj *)pObjHit)->m_pClientItem;
	if (pItemHit != NULL && !pItemHit->SupportsIPointerInactive())
		pItemHit = NULL;

    if (m_pItemToDeactivateOnLeave != NULL)
	{
		// We have an active object that wants to be deactivated as soon as the
		// mouse leaves its extents.
		if (m_pItemToDeactivateOnLeave != pItemHit)
		{
			TRACE("Deactivating item because of POINTERINACTIVE_DEACTIVATEONLEAVE.\n" );
			m_pItemToDeactivateOnLeave->Deactivate();
			m_pItemToDeactivateOnLeave = NULL;
		}
	}

	if (pItemHit != NULL)
	{
		if (!pItemHit->IsInPlaceActive())
		{
			// The mouse was over an inactive control that wants to receive mouse
			// pointer messages, so send the message to that control first.
			dwPolicy = pItemHit->OnPointerInactiveMove( point, dwKeyState );
			if (dwPolicy&POINTERINACTIVE_ACTIVATEONENTRY)
			{
				msg = *GetCurrentMessage();
				TRACE("Activating item because of POINTERINACTIVE_ACTIVATEONENTRY.\n");
				pItemHit->Activate(OLEIVERB_SHOW, this, &msg);
				if (dwPolicy&POINTERINACTIVE_DEACTIVATEONLEAVE)
				{
				   // The control wants to be deactivated as soon as the mouse
				   // leaves its extents.
				   m_pItemToDeactivateOnLeave = pItemHit;
				}
			}
			bHandled = TRUE;
		}
	}
	else
	{
		bHandled = FALSE;
	}

   return bHandled;
}

BOOL CRunView::HandlePointerInactiveSetCursor(
	DWORD dwMouseMessage )
{
   	CDevObj* pObjHit = NULL;
	CCtrlItem* pItemHit = NULL;
	CRect rect;
	BOOL bHandled;
	DWORD dwPolicy;
	MSG msg;
	CPoint point;

	point = m_ptLastMousePosition;
	pObjHit = HitTestObjects(point);
	if (pObjHit != NULL && pObjHit->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
		pItemHit = ((CCtrlObj *)pObjHit)->m_pClientItem;
	if (pItemHit != NULL && !pItemHit->SupportsIPointerInactive())
		pItemHit = NULL;
   
	if (m_pItemToDeactivateOnLeave != NULL)
	{
		// If the pointer is still over the same control, then we really haven't
		// left it.
		if (m_pItemToDeactivateOnLeave != pItemHit)
		{
			TRACE("Deactivating item because of POINTERINACTIVE_DEACTIVATEONLEAVE.\n");
			m_pItemToDeactivateOnLeave->Deactivate();
			m_pItemToDeactivateOnLeave = NULL;
		}
	}

	bHandled = FALSE;
	if (pItemHit != NULL)
	{
		if (!pItemHit->IsInPlaceActive())
		{
			// The mouse was over an inactive control that wants to receive mouse
			// pointer messages, so send the message to that control first.
			dwPolicy = pItemHit->OnPointerInactiveSetCursor(point, dwMouseMessage, &bHandled);
			if (dwPolicy&POINTERINACTIVE_ACTIVATEONENTRY)
			{
				msg = *GetCurrentMessage();
				TRACE("Activating item because of POINTERINACTIVE_ACTIVATEONENTRY.\n");
				pItemHit->Activate( OLEIVERB_SHOW, this, &msg );
				if (dwPolicy&POINTERINACTIVE_DEACTIVATEONLEAVE)
				{
					m_pItemToDeactivateOnLeave = pItemHit;
				}
				bHandled = TRUE;
			}
		}
	}

   return bHandled;
}

void CRunView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!m_bEnabled || m_bDragMode)
		return;

	if (IsCancelBubble() || (HandleWindowlessKeyboardMessage() && IsCancelBubble()))
		return;

	HWND hWndSave = m_hWnd;
	USHORT nCharShort = (USHORT)nChar;
	KeyDown(&nCharShort);
	if ((m_hWnd == hWndSave) && (nCharShort != 0))
		CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRunView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!m_bEnabled || m_bDragMode)
		return;

	if (IsCancelBubble() || (HandleWindowlessKeyboardMessage() && IsCancelBubble()))
		return;

	HWND hWndSave = m_hWnd;
	USHORT nCharShort = (USHORT)nChar;
	KeyUp(&nCharShort);
	if ((m_hWnd == hWndSave) && (nCharShort != 0))
		CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CRunView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!m_bEnabled || m_bDragMode)
		return;

	if (IsCancelBubble() || (HandleWindowlessKeyboardMessage() && IsCancelBubble()))
		return;

//	USHORT nCharShort = (USHORT)nChar;
//	USHORT nCharSave = nCharShort;
//	BOOL bLeadByte = IsDBCSLeadByte((BYTE)nCharShort);
//	MSG msg;
//	if (bLeadByte)
//	{
//		// We have the lead-byte of a DBCS character.  Peek for the
//		// next WM_CHAR message, which will contain the other byte.

//		BOOL bMessage;
//		VERIFY(bMessage = ::PeekMessage(&msg, m_hWnd, WM_CHAR, WM_CHAR,
//			PM_NOYIELD | PM_NOREMOVE));

		// Combine the bytes to form the DBCS character.

//		if (bMessage)
//			nCharShort = (USHORT)((nCharShort << 8) | msg.wParam);
//	}

	USHORT nCharShort = (USHORT)nChar;
	USHORT nCharSave = nCharShort;
	HWND hWndSave = m_hWnd;
	nCharSave = nCharShort;
	if (m_pEventObject != NULL)
		m_pEventObject->put_keyCode(nCharShort);
	FireEvent(CXEVENID_KEYPRESS);
	if (m_pEventObject != NULL)
		m_pEventObject->get_keyCode((long *)&nCharShort);

	if ((m_hWnd == hWndSave) && (nCharShort != 0))
		CScrollView::OnChar(nChar, nRepCnt, nFlags);

//	FireKeyPress(&nCharShort);

//	if (m_hWnd != hWndSave)
//		return;
	
//	if (nCharShort != 0)
//	{
//		if (nCharSave != nCharShort)
//		{
//			nChar = nCharShort;

			// Event handler has changed the character.
//
//			BOOL bNewLeadByte = IsDBCSLeadByte(HIBYTE(nCharShort));

//			if (bLeadByte)
//			{
//				if (bNewLeadByte)
//				{
					// Event handler changed character from DBCS to DBCS:
					// Remove the old trail byte and post the new one.
//
//					VERIFY(::PeekMessage(&msg, m_hWnd, WM_CHAR, WM_CHAR,
//						PM_NOYIELD | PM_REMOVE));
//					_PostTrailByte(this, LOBYTE(nCharShort));
//					nChar = HIBYTE(nCharShort);
//				}
//				else
//				{
					// Event handler changed character from DBCS to SBCS:
					// Remove the second byte from the queue, and forward
					// along the new single-byte character.
//
//					VERIFY(::PeekMessage(&msg, m_hWnd, WM_CHAR, WM_CHAR,
//						PM_NOYIELD | PM_REMOVE));
//				}
//			}
//			else
//			{
//				if (bNewLeadByte)
//				{
					// Event handler changed character from SBCS to DBCS:
					// Post the new trail byte.

//					_PostTrailByte(this, LOBYTE(nCharShort));
//					nChar = HIBYTE(nCharShort);
//				}
//			}
//		}

//		DefWindowProc(WM_CHAR, nChar, MAKELONG(nRepCnt, nFlags));
//	}

//	if (bLeadByte)
//	{
//		VERIFY(::PeekMessage(&msg, m_hWnd, WM_CHAR, WM_CHAR,
//				PM_NOYIELD | PM_REMOVE));
///	}
}

void CRunView::OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (HandleWindowlessKeyboardMessage())
		return;
	
	CScrollView::OnDeadChar(nChar, nRepCnt, nFlags);
}

void CRunView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (IsCancelBubble() || (HandleWindowlessKeyboardMessage() && IsCancelBubble()))
		return;
	
	HWND hWndSave = m_hWnd;
	USHORT nCharShort = (USHORT)nChar;
	KeyDown(&nCharShort);
	if ((m_hWnd == hWndSave) && (nCharShort != 0))
		CScrollView::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CRunView::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (IsCancelBubble() || (HandleWindowlessKeyboardMessage() && IsCancelBubble()))
		return;

	HWND hWndSave = m_hWnd;
	USHORT nCharShort = (USHORT)nChar;
	KeyUp(&nCharShort);
	if ((m_hWnd == hWndSave) && (nCharShort != 0))
		CScrollView::OnSysKeyUp(nChar, nRepCnt, nFlags);
}

void CRunView::OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	BOOL bHandled;

	bHandled = HandleWindowlessKeyboardMessage();
	if (bHandled)
	{
		return;
	}
	
	CScrollView::OnSysDeadChar(nChar, nRepCnt, nFlags);
}

void CRunView::OnCancelMode() 
{
	BOOL bHandled;

	CScrollView::OnCancelMode();

	bHandled = HandleWindowlessKeyboardMessage();
	if (bHandled)
	{
		return;
	}
}

//////////////////////////////////////////////////////////////////
//功 能：处理无窗口控件焦点
//参 数：1、是否是焦点
//////////////////////////////////////////////////////////////////
BOOL CRunView::HandleWindowlessFocusMessage(BOOL bFoucs)
{
	if (m_pObjectWithFocus != NULL)
	{
		MSG msg;
		msg.message = bFoucs ? WM_SETFOCUS : WM_KILLFOCUS;
		m_pObjectWithFocus->OnWindowMessage(&msg);
		return TRUE;
	}

	return FALSE;
}

// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place.
void CRunView::OnSetFocus(CWnd* pOldWnd) 
{
	CCtrlItem* pItem = NULL;
	if (m_pObjectWithFocus != NULL && m_pObjectWithFocus->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
	{
		pItem = ((CCtrlObj *)m_pObjectWithFocus)->m_pClientItem;
		pItem->SetFocus();
	}
	else
	{
		pItem = GetDocument()->GetDefaultItem();
		if (pItem != NULL)
			pItem->DisplayAsDefault(TRUE);
	}

//	if (m_pObjectWithFocus != NULL && m_pObjectWithFocus->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
//	{
//		COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
//		if (pActiveItem != NULL &&
//			pActiveItem->GetItemState() == COleClientItem::activeUIState)
//		{
//			// need to set focus to this item if it is in the same view
//			CWnd* pWnd = pActiveItem->GetInPlaceWindow();
//			if (pWnd != NULL)
//			{
//				pWnd->SetFocus();   // don't call the base class
//				return;
//			}
//		}
//	}

	//无窗口控件设定焦点
	HandleWindowlessFocusMessage(TRUE);

	CScrollView::OnSetFocus(pOldWnd);
}

/////////////////////////////////////////////////////////////////
void CRunView::OnKillFocus(CWnd* pNewWnd) 
{
	HandleWindowlessFocusMessage(FALSE);

 	m_pObjectWithCapture = NULL;

	CScrollView::OnKillFocus(pNewWnd);
}

void CRunView::OnFocus(CDevObj* pObj, BOOL bGotFocus)
{
	ASSERT(pObj != NULL);

	if (bGotFocus)
	{
		m_pObjectWithFocus = pObj;
	}
	else
	{
		if (m_pObjectWithFocus == pObj)
			m_pObjectWithFocus = NULL;

	}

	if (pObj->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
	{
		CCtrlItem* pItem = ((CCtrlObj *)pObj)->m_pClientItem;
		CCtrlItem* pDefaultItem = GetDocument()->GetDefaultItem();

		if (pItem->ActsLikeButton())
		{
			pItem->DisplayAsDefault(bGotFocus);
			if (pDefaultItem != NULL && pDefaultItem != pItem)
				pDefaultItem->DisplayAsDefault(FALSE);
		}
		else if (bGotFocus)
		{
			if (pDefaultItem != NULL)
				pDefaultItem->DisplayAsDefault(TRUE);
		}
	}
}

BOOL CRunView::TranslateAccelerator(CCtrlItem* pItem, LPMSG pMsg, DWORD dwModifiers)
{
	CFormDoc* pDoc = GetDocument();
	CCtrlItem* pNextItem = NULL;

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (LOWORD(pMsg->wParam))
		{
		case VK_TAB:
			{
				pNextItem = pDoc->GetNextTabItem(pItem, ::GetKeyState(VK_SHIFT) < 0);
				if (pNextItem != NULL && pNextItem != pItem)
					pNextItem->SetFocus();
				
				return TRUE;
			}
		case VK_RETURN:
		case VK_EXECUTE:
			if (pItem != NULL && pItem->ActsLikeButton())
				pNextItem = pItem;
			else
				pNextItem = pDoc->GetDefaultItem();
			if (pNextItem == NULL)
				return FALSE;
		case VK_ESCAPE:
		case VK_CANCEL:
			if (pNextItem == NULL)
				pNextItem = pDoc->GetCancelItem();
	
			if (pNextItem != NULL)
			{
				if (pNextItem->IsEnabled())
				{
					CComDispatchDriver dd(pNextItem->m_pDispatch);
					dd.Invoke0(DISPID_DOCLICK);
				}
				else
				{
					MessageBeep(0);
				}	
			}
			return TRUE;
		}
	}

	return FALSE;
}

HRESULT CRunView::OnWindowlessReleaseFocus()
{
	ASSERT(m_pObjectWithFocus != NULL);

	HandleWindowlessFocusMessage(FALSE);
	m_pObjectWithFocus = NULL;

	return S_OK;
}

////////////////////////////////////////////////////////////////////
//功 能：设置无窗口控件的焦点
//描 述：代码参照www.codeguru.com/activex/windowlessactive
////////////////////////////////////////////////////////////////////
HRESULT CRunView::OnWindowlessSetFocus(CDevObj* pObj)
{
	ASSERT(pObj != NULL);

	if (m_pObjectWithFocus != NULL && m_pObjectWithFocus != pObj)
	{
		HandleWindowlessFocusMessage(FALSE);
	}

	m_pObjectWithFocus = pObj;

	HandleWindowlessFocusMessage(TRUE);

	return S_OK;	
}

HRESULT CRunView::OnWindowlessReleaseCapture(CDevObj* pObj)
{
	// 2011.4.25: 鼠标按下时弹出菜单时，释放鼠标捕获
//	if (m_pObjectWithCapture == pObj)
	{
		m_pObjectWithCapture = NULL;
		ReleaseCapture();
	}

	return S_OK;
}

HRESULT CRunView::OnWindowlessSetCapture(CDevObj* pObj)
{
	ASSERT(pObj != NULL);

	m_pObjectWithCapture = pObj;
	SetCapture();
	
	return S_OK;
}

CDevObj* CRunView::GetObjectWithCapture() const
{
	return m_pObjectWithCapture;
}

CDevObj* CRunView::GetObjectWithFocus() const
{
	return m_pObjectWithFocus;
}

HRESULT CRunView::OnWindowlessDefWindowMessage(UINT nMessage,
	WPARAM wParam, LPARAM lParam, LRESULT* plResult)
{
	const MSG* pMessage;

	(void)nMessage;
	(void)wParam;
	(void)lParam;

	pMessage = GetCurrentMessage();
	ASSERT(pMessage->message == nMessage);
	ASSERT(pMessage->wParam == wParam);
	ASSERT(pMessage->lParam == lParam);

	*plResult = Default();

	return S_OK;
}

void CRunView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	switch (lHint)
	{
	case HINT_UPDATE_DRAWOBJ:   // a single object has changed
		InvalidObject((CDevObj*)pHint);
		break;
	case HINT_UPDATE_WINDOW:    // redraw entire window
		Invalidate(FALSE);
		break;
	
	default:
		ASSERT(FALSE);
		break;
	}		
}

inline void CRunView::UpdateObjAreaMask(CDevObj *pObj, BOOL bOr)
{
	while (pObj->m_pGroupObj != NULL)
		pObj = pObj->m_pGroupObj;

	if (!bOr)
		memset(pObj->m_wAreaMask, 0, 16);

	CRect rect = pObj->GetBoundingRect();

	int y0 = int(rect.top / m_nAreaHeight);
	if (y0 < 0)
		y0 = 0;
	if (y0 > 7)
		y0 = 7;

	int y1 = int(rect.bottom / m_nAreaHeight);
	if (y1 < 0)
		y1 = 0;
	if (y1 > 7)
		y1 = 7;

	int x0 = int(rect.left / m_nAreaWidth);
	if (x0 < 0)
		x0 = 0;
	if (x0 > 15)
		x0 = 15;

	int x1 = int(rect.right / m_nAreaWidth);
	if (x1 < 0)
		x1 = 0;
	if (x1 > 15)
		x1 = 15;

	for (int x = x0; x <= x1; x++)	
	{
		ASSERT(y0 >= 0 && y0 <= 7);
		pObj->m_wAreaMask[y0] |= WORD(1 << x);
	}

	for (int y = y0 + 1; y <= y1; y++)
	{
		ASSERT(y >= 0 && y <= 7);
		pObj->m_wAreaMask[y] = pObj->m_wAreaMask[y0];
	}
}

inline void CRunView::InvalidObject(CDevObj *pObj)
{
	CRect rect = pObj->GetBoundingRect();
	if (pObj == m_pObjectHighlight)
		rect.InflateRect(4, 4);

	if (GetDocument()->IsAllowDirectDraw())
	{
		if (m_mapDirectDraw.GetCount() == 0)
			PostMessage(WM_PAINTEX);

		CRect* pUpdateBound = NULL;
		m_mapDirectDraw.Lookup(pObj, (void *&)pUpdateBound);

		if (pUpdateBound == NULL)
			pUpdateBound = new CRect(pObj->GetBoundingRect());
		else
			pUpdateBound->UnionRect(pUpdateBound, &rect);
		m_mapDirectDraw.SetAt(pObj, pUpdateBound);

		m_rcDirectDraw.UnionRect(&m_rcDirectDraw, pUpdateBound);
	}
	else
	{
		UpdateObjAreaMask(pObj, TRUE);
		InvalidateRectEx(&rect);
	}

/*
	int y0 = int(rect.top / m_nAreaHeight);
//	if (y0 < 0)
//		y0 = 0;
	int y1 = int(rect.bottom / m_nAreaHeight);
//	if (y1 > 7)
//		y1 = 7;
	int x0 = int(rect.left / m_nAreaWidth);
//	if (x0 < 0)
//		x0 = 0;
	int x1 = int(rect.right / m_nAreaWidth);
//	if (x1 > 15)
//		x1 = 15;

	//不在刷新区
	if (x1 < 0 || x0 > 15)
		return;
	if (y1 < 0 || y0 > 7)
		return;

	if (x0 < 0)
		x0 = 0;
	if (x1 > 15)
		x1 = 15;

	WORD wMask = 0;
	for (int x = x0; x <= x1; x++)	
		wMask |= WORD(1 << x);

	//必须调整，否则数组出界
	if (y0 < 0)
		y0 = 0;
	if (y1 > 7)
		y1 = 7;

	for (int y = y0; y <= y1; y++)
		m_wInvalidAreaMask[y] |= wMask;

	PostMessage(WM_PAINTEX);*/

}

void CRunView::InvalidateRectEx(LPCRECT lpRect)
{
	ASSERT(lpRect != NULL);
		
	int y0 = int(lpRect->top / m_nAreaHeight);
	//	if (y0 < 0)
	//		y0 = 0;
	int y1 = int(lpRect->bottom / m_nAreaHeight);
	//	if (y1 > 7)
	//		y1 = 7;
	int x0 = int(lpRect->left / m_nAreaWidth);
	//	if (x0 < 0)
	//		x0 = 0;
	int x1 = int(lpRect->right / m_nAreaWidth);
	//	if (x1 > 15)
	//		x1 = 15;
	
	//不在刷新区
	if (x1 < 0 || x0 > 15)
		return;
	if (y1 < 0 || y0 > 7)
		return;
	
	if (x0 < 0)
		x0 = 0;
	if (x1 > 15)
		x1 = 15;

	if (memcmp(m_wInvalidAreaMask, szAllZeroInvalidMask, 16) == 0)
		PostMessage(WM_PAINTEX);
	
	WORD wMask = 0;
	for (int x = x0; x <= x1; x++)	
		wMask |= WORD(1 << x);
	
	//必须调整，否则数组出界
	if (y0 < 0)
		y0 = 0;
	if (y0 > 7)
		y0 = 7;
	if (y1 > 7)
		y1 = 7;
	
	for (int y = y0; y <= y1; y++)
	{
		ASSERT(y >= 0 && y <= 7);
		m_wInvalidAreaMask[y] |= wMask;
	}
}

//int CRunView::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
//{
//	CPoint ptTemp = point;
//	CRunView* pRunView = (CRunView *)this;
//	pRunView->ClientToDoc(ptTemp);
//	CDevObj* pObjHit = HitTestObjects(ptTemp);
//	if (pObjHit == NULL)
//		return CScrollView::OnToolHitTest(point, pTI);
//	if (pObjHit->m_strToolTip.IsEmpty())
//		return -1;
//
//	CRect rect = pObjHit->GetBoundingRect();
//	pRunView->DocToClient(rect);
//	pTI->hwnd = m_hWnd;
//	pTI->uId = (UINT)pObjHit;
//	pTI->rect.left = rect.left;
//	pTI->rect.top = rect.top;
//	pTI->rect.right = rect.right;
//	pTI->rect.bottom = rect.bottom;
//	pTI->lpszText = LPSTR_TEXTCALLBACK; 
//
//	return 1;
//}
//
//BOOL CRunView::OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
//{
//	// need to handle both ANSI and UNICODE versions of the message
//	TOOLTIPTEXT* pTTT = (TOOLTIPTEXT *)pNMHDR;
//
//	const MSG* pMessage;
//	CPoint point;
//	pMessage = GetCurrentMessage();
//	ASSERT (pMessage);
//	point = pMessage->pt;
//	ScreenToClient(&point);
//	ClientToDoc(point);
//
//	CDevObj* pObjHit = HitTestObjects(point);
//	if (pObjHit == NULL)
//		return FALSE;
//
//#ifndef _UNICODE
//	lstrcpyn(pTTT->szText, pObjHit->m_strToolTip, pObjHit->m_strToolTip.GetLength() + 1);
//#else
//	_mbstowcsz(pTTT->szText, pObjHit->m_strToolTip, pObjHit->m_strToolTip.GetLength() + 1);
//#endif
//
//	*pResult = 0;
//	return TRUE;    // message was handled
//}

void CRunView::NotifyDisplayTooltip(NMHDR * pNMHDR, LRESULT * result)
{
	*result = 0;
	NM_PPTOOLTIP_DISPLAY * pNotify = (NM_PPTOOLTIP_DISPLAY*)pNMHDR;

	CPoint point = *pNotify->pt;
	ScreenToClient(&point);
	ClientToDoc(point);
	CDevObj* pObjHit = HitTestObjects(point);
	if (pObjHit != NULL)
	{
		pNotify->ti->sTooltip = pObjHit->m_strToolTip;
	//	point.Offset(2000, 10);
	//	*pNotify->pt = point;
	}
	
	//UINT nID = CWnd::FromHandle(pNotify->ti->hWnd)->GetDlgCtrlID();
	//	
	//	if (nID == IDC_LIST1)
	//	{
	//		//This is notification for titletip
	//		CRect rect = pNotify->ti->rectBounds;
	//		ClientToScreen(&rect);
	//		BOOL bOutside = FALSE;
	//		CPoint pt = *pNotify->pt;
	//		pt -= rect.TopLeft();
	//		UINT nItem = m_list1.ItemFromPoint(pt, bOutside);
	//		pNotify->ti->sTooltip = m_str [nItem];
	//		pt = rect.TopLeft();
	//		//		pt.x ++;
	//		//		pt.y ++;
	//		m_list1.GetItemRect(nItem, rect);
	//		rect.OffsetRect(pt);
	//		*pNotify->pt = rect.TopLeft();
	//	}
	//	else if (nID == IDC_LIST2)
	//	{
	//		//This is notification for titletip
	//		CRect rect = pNotify->ti->rectBounds;
	//		ClientToScreen(&rect);
	//		BOOL bOutside = FALSE;
	//		CPoint pt = *pNotify->pt;
	//		pt -= rect.TopLeft();
	//		UINT nItem = m_list2.ItemFromPoint(pt, bOutside);
	//		pNotify->ti->sTooltip = m_str [nItem];
	//		if (m_show_under)
	//		{
	//			pt.x = rect.left;
	//			pt.y = rect.bottom + 1;
	//		}
	//		else 
	//		{
	//			pt = *pNotify->pt;
	//			pt.y += 20;
	//		}
	//		*pNotify->pt = pt;
	//	}
	
}


inline void CRunView::FireEvent(DISPID dispid)
	{ GetDocument()->FireEvent(dispid, EVENT_PARAM(VTS_NONE)); }
inline void CRunView::FireKeyDown(USHORT* pnChar, short nShiftState)
	{ GetDocument()->FireEvent(CXEVENID_KEYDOWN, EVENT_PARAM(VTS_PI2 VTS_I2), pnChar,
		nShiftState); }
inline void CRunView::FireKeyUp(USHORT* pnChar, short nShiftState)
	{ GetDocument()->FireEvent(CXEVENID_KEYUP, EVENT_PARAM(VTS_PI2 VTS_I2), pnChar,
		nShiftState); }
inline void CRunView::FireKeyPress(USHORT* pnChar)
	{ GetDocument()->FireEvent(CXEVENID_KEYPRESS, EVENT_PARAM(VTS_PI2), pnChar); }
inline void CRunView::FireMouseDown(short nButton,
		short nShiftState, OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y)
	{ GetDocument()->FireEvent(CXEVENID_MOUSEDOWN,
		EVENT_PARAM(VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS),
		nButton, nShiftState, x, y); }
inline void CRunView::FireMouseUp(short nButton,
		short nShiftState, OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y)
	{ GetDocument()->FireEvent(CXEVENID_MOUSEUP,
		EVENT_PARAM(VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS),
		nButton, nShiftState, x, y); }
inline void CRunView::FireMouseMove(short nButton,
		short nShiftState, OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y)
	{ GetDocument()->FireEvent(CXEVENID_MOUSEMOVE,
		EVENT_PARAM(VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS),
		nButton, nShiftState, x, y); }
inline void CRunView::FireClick()
	{ GetDocument()->FireEvent(CXEVENID_CLICK, EVENT_PARAM(VTS_NONE)); }
inline void CRunView::FireDblClick()
	{ GetDocument()->FireEvent(CXEVENID_DBLCLICK, EVENT_PARAM(VTS_NONE)); }
inline void CRunView::FireInitialize()
	{ GetDocument()->FireEvent(CXEVENID_INITIALIZE, EVENT_PARAM(VTS_NONE)); }
inline void CRunView::FireActivate()
	{ GetDocument()->FireEvent(CXEVENID_ACTIVATE, EVENT_PARAM(VTS_NONE)); }
inline void CRunView::FireDeactivate()
	{ GetDocument()->FireEvent(CXEVENID_DEACTIVATE, EVENT_PARAM(VTS_NONE)); }


void CRunView::ButtonDown(USHORT iButton, UINT, CPoint point)
{
// 	if (m_iButtonState == 0)
// 		SetCapture();

	m_iButtonState |= iButton;

	FireEvent(CXEVENID_MOUSEDOWN);
	//FireMouseDown(iButton, _GetShiftState(), point.x, point.y);

	m_iDblClkState &= ~iButton;
}

void CRunView::ButtonUp(USHORT iButton, UINT, CPoint point)
{
	if (m_iButtonState != 0)
	{
		m_iButtonState &= ~iButton;

//		if (m_iButtonState == 0)
//			ReleaseCapture();

		FireEvent(CXEVENID_MOUSEUP);
	//	FireMouseUp(iButton, _GetShiftState(), point.x, point.y);

		if (!(m_iDblClkState & iButton))
		{
			CRect rect;
			GetClientRect(&rect);
			if (rect.PtInRect(point))
				FireEvent(CXEVENID_CLICK);
		//		FireClick();
		}

		m_iDblClkState &= ~iButton;
	}
}

void CRunView::ButtonDblClk(USHORT iButton, UINT, CPoint)
{
	SetCapture();
	m_iButtonState |= iButton;

	FireEvent(CXEVENID_DBLCLICK);
//	FireDblClick();
	m_iDblClkState |= iButton;
}

void CRunView::KeyUp(USHORT* pnChar)
{
	USHORT nShiftState = _GetShiftState();
	FireEvent(CXEVENID_KEYUP);
//	FireKeyUp(pnChar, nShiftState);

	// If handler set *pnChar to zero, cancel further processing.
//	if (*pnChar != 0)
//		OnKeyUpEvent(*pnChar, nShiftState);
}

void CRunView::KeyDown(USHORT* pnChar)
{
	USHORT nShiftState = _GetShiftState();
	FireEvent(CXEVENID_KEYDOWN);
//	FireKeyDown(pnChar, nShiftState);

	// If handler set *pnChar to zero, cancel further processing.
//	if (*pnChar != 0)
//		OnKeyDownEvent(*pnChar, nShiftState);
}

void CRunView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate)
		FireActivate();
	else
		FireDeactivate();
	
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CRunView::IsCancelBubble()
{
	VARIANT_BOOL b = VARIANT_FALSE;
	if (m_pEventObject != NULL)
		m_pEventObject->get_cancelBubble(&b);
	return b == VARIANT_TRUE;
}

void CRunView::InitEventObject()
{
	if (m_pEventObject == NULL)
		return;

	const MSG* pMsg = GetCurrentMessage();

	CPoint point(pMsg->pt);
	m_pEventObject->put_screenX(point.x);
	m_pEventObject->put_screenY(point.y);
	ScreenToClient(&point);
	m_pEventObject->put_clientX(point.x);
	m_pEventObject->put_clientY(point.y);
	ClientToDoc(point);
	m_pEventObject->put_x(point.x);
	m_pEventObject->put_y(point.y);
	m_pEventObject->put_ctrlKey(GetKeyState(VK_CONTROL) < 0 ? VARIANT_TRUE : VARIANT_FALSE);
	m_pEventObject->put_shiftKey(GetKeyState(VK_SHIFT) < 0 ? VARIANT_TRUE : VARIANT_FALSE);
	m_pEventObject->put_altKey(GetKeyState(VK_MENU) < 0 ? VARIANT_TRUE : VARIANT_FALSE);
	m_pEventObject->put_cancelBubble(VARIANT_FALSE);
	m_pEventObject->put_button(m_iButton);
	m_pEventObject->put_keyCode(m_iKeyCode);
	VARIANT var;
	VariantInit(&var);
	m_pEventObject->put_returnValue(var);
}

void CRunView::Drag()
{
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return;

	// set capture to the window which received this message
	SetCapture();
	ASSERT(this == CWnd::GetCapture());

	CPoint ptLast;
	GetCursorPos(&ptLast);

	SetCursor(GetHandCursor(2));

	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == this)
	{
		MSG msg;
		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage(msg.wParam);
			break;
		}

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			goto onexit;
		case WM_MOUSEMOVE:
			if (OnScrollBy(ptLast - msg.pt, TRUE))
				ptLast = msg.pt;
			break;
		
		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

onexit:
	ReleaseCapture();
}

/////////////////////////////////////////////////////////////////////////////
//【函  数】LRESULT WindowProc( UINT, WPARAM, LPARAM lParam ) 
//【参  数】message : UINT,  
//          wParam  : WPARAM,
//          lParam  : LPARAM,
//【功  能】预处理消息
//【返回值】LRESULT
//【说  明】代替原来的BOOL PreTranslateMessage( MSG* ) 
/////////////////////////////////////////////////////////////////////////////
LRESULT CRunView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL bActiveEventObject = TRUE;

	switch (message)
	{
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONDOWN:
		m_iButton |= LEFT_BUTTON;
		break;
	case WM_MBUTTONDOWN:
		m_iButton |= MIDDLE_BUTTON;
		break;
	case WM_RBUTTONDOWN:
		m_iButton |= RIGHT_BUTTON;
		break;
	case WM_LBUTTONUP:
		m_iButton &= ~LEFT_BUTTON;
		break;
	case WM_MBUTTONUP:
		m_iButton &= ~MIDDLE_BUTTON;
		break;
	case WM_RBUTTONUP:
		m_iButton &= ~RIGHT_BUTTON;
		break;
	case WM_LBUTTONDBLCLK:
		m_iButton |= LEFT_BUTTON;
		break;
	case WM_MBUTTONDBLCLK:
		m_iButton |= MIDDLE_BUTTON;
		break;
	case WM_RBUTTONDBLCLK:
		m_iButton |= RIGHT_BUTTON;
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		m_iKeyCode = wParam;
		break;
	case WM_CHAR:
		m_iKeyCode = wParam;
		break;
	default:
		bActiveEventObject = FALSE;
	}

	if (bActiveEventObject && m_pDocument != NULL)
	{
		CFormScrEng* pEng = GetDocument()->GetScriptEngine();
		if (pEng != NULL)
			pEng->ActiveEventObject(this);
	}

//	if (::IsWindow(m_tooltip.m_hWnd))
//		m_tooltip.RelayEvent((MSG *)GetCurrentMessage());

	return CScrollView::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
//【函  数】BOOL PreTranslateMessage( MSG* ) 
//【参  数】pMsg : MSG*,
//【功  能】预处理消息
//【返回值】BOOL
//【说  明】实际上在作为ActiveX控件时，此函数并没有被调用，由
//LRESULT WindowProc( UINT, WPARAM, LPARAM lParam )代替
/////////////////////////////////////////////////////////////////////////////
BOOL CRunView::PreTranslateMessage(MSG* pMsg) 
{
	if (::IsWindow(m_tooltip.m_hWnd))
		m_tooltip.RelayEvent(pMsg);

	CFormDoc* pDoc = GetDocument();
	CDevObj* pCurrentObj = NULL;
	CCtrlItem* pItem = NULL;

	if (m_pObjectWithFocus != NULL)
	{
		pCurrentObj = m_pObjectWithFocus;
	}

	if (((pMsg->message >= WM_KEYFIRST) && (pMsg->message <= WM_KEYLAST)) ||
		((pMsg->message >= WM_MOUSEFIRST) && (pMsg->message <= WM_MOUSELAST)) || 
		pMsg->message == WM_SETCURSOR)
	{
		if (m_bDragMode)
		{
			if (pMsg->message == WM_LBUTTONDOWN)
			{
				Drag();
				return TRUE;
			}

			if (::IsChild(m_hWnd, pMsg->hwnd))
				return TRUE;
		}

		// Give the item with focus first crack at keyboard and mouse messages
		if (pCurrentObj != NULL && pCurrentObj->TranslateAccelerator(pMsg))
			return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		CCtrlItem* pCurrentItem = NULL;
		switch (LOWORD(pMsg->wParam))
		{
		case VK_TAB:
			if (pCurrentObj != NULL && pCurrentObj->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
				pCurrentItem = ((CCtrlObj *)pCurrentObj)->m_pClientItem;
		case VK_EXECUTE:
		case VK_RETURN:
		case VK_ESCAPE:
		case VK_CANCEL:
			return TranslateAccelerator(pCurrentItem, pMsg, 0);
		}
	}

	return FALSE;
}

#if (WINVER < 0x0500)
__declspec(selectany) struct
{
	enum { cxWidth = 32, cyHeight = 32 };
	int xHotSpot;
	int yHotSpot;
	unsigned char arrANDPlane[cxWidth * cyHeight / 8];
	unsigned char arrXORPlane[cxWidth * cyHeight / 8];
} _Hand_CursorData = 
{
	5, 0, 
	{
		0xF9, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 
		0xF0, 0xFF, 0xFF, 0xFF, 0xF0, 0x3F, 0xFF, 0xFF, 0xF0, 0x07, 0xFF, 0xFF, 0xF0, 0x01, 0xFF, 0xFF, 
		0xF0, 0x00, 0xFF, 0xFF, 0x10, 0x00, 0x7F, 0xFF, 0x00, 0x00, 0x7F, 0xFF, 0x00, 0x00, 0x7F, 0xFF, 
		0x80, 0x00, 0x7F, 0xFF, 0xC0, 0x00, 0x7F, 0xFF, 0xC0, 0x00, 0x7F, 0xFF, 0xE0, 0x00, 0x7F, 0xFF, 
		0xE0, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xF8, 0x01, 0xFF, 0xFF, 
		0xF8, 0x01, 0xFF, 0xFF, 0xF8, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	},
	{
		0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 
		0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0xC0, 0x00, 0x00, 0x06, 0xD8, 0x00, 0x00, 
		0x06, 0xDA, 0x00, 0x00, 0x06, 0xDB, 0x00, 0x00, 0x67, 0xFB, 0x00, 0x00, 0x77, 0xFF, 0x00, 0x00, 
		0x37, 0xFF, 0x00, 0x00, 0x17, 0xFF, 0x00, 0x00, 0x1F, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0x00, 0x00, 
		0x0F, 0xFE, 0x00, 0x00, 0x07, 0xFE, 0x00, 0x00, 0x07, 0xFE, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 
		0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	}
};
#endif //(WINVER < 0x0500)

HCURSOR CRunView::GetHandCursor(int nIndex)
{
	if (nIndex < 0 || nIndex > 2)
		return NULL;

	if (m_hHandCursors[nIndex] == NULL)
	{
		switch (nIndex)
		{
		case 0:
			// create or load a cursor
#if (WINVER >= 0x0500)
			m_hHandCursors[nIndex] = ::LoadCursor(NULL, IDC_HAND);
#else
			m_hHandCursors[nIndex] = ::CreateCursor(theApp.m_hInstance, _Hand_CursorData.xHotSpot, _Hand_CursorData.yHotSpot, _Hand_CursorData.cxWidth, _Hand_CursorData.cyHeight, _Hand_CursorData.arrANDPlane, _Hand_CursorData.arrXORPlane);
#endif //!(WINVER >= 0x0500)
			ATLASSERT(m_hHandCursors[nIndex] != NULL);
			break;
		default:
			m_hHandCursors[nIndex] = ::LoadCursor(theApp.m_hInstance, MAKEINTRESOURCE(IDC_HAND1 + nIndex - 1));
			break;
			
		}
	}

	return m_hHandCursors[nIndex];
}

void CRunView::SetStandardCursor(enumMousePointer enumMousePointer)
{
	switch (enumMousePointer)
	{
//	case fmMousePointerDefault:
	case MousePointerArrow:
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		break;
	case MousePointerHand:
		SetCursor(GetHandCursor());
		break;
	case MousePointerCross:
		SetCursor(LoadCursor(NULL, IDC_CROSS));
		break;
	case MousePointerIBeam:
		SetCursor(LoadCursor(NULL, IDC_IBEAM));
		break;
	case MousePointerSizeNESW:
		SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		break;
	case MousePointerSizeNS:
		SetCursor(LoadCursor(NULL, IDC_SIZENS));
		break;
	case MousePointerSizeNWSE:
		SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
		break;
	case MousePointerSizeWE:
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		break;
	case MousePointerUpArrow:
		SetCursor(LoadCursor(NULL, IDC_UPARROW));
		break;
	case MousePointerHourglass:
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		break;
	case MousePointerSizeAll:
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		break;
	}
}


void CRunView::OnDestroy() 
{
	if (m_bDestory)
		return;

	m_bDestory = TRUE;

	CScrollView::OnDestroy();

	// 如果不是主窗体，则由控件来删除。
	// 弹出式窗体可能被客户程序先于控件窗口删除
	if (!GetDocument()->IsMainForm())
	{
		if (!m_bNoClosePicture)
			m_pRunCtrl->ClosePicture();
	}
	else
	{
		// 2006.10.23 日加入，修正问题：
		// 画面中有主菜单的情况下，打开子窗体后无法正常退出
		m_pRunCtrl->m_pFrameWnd = NULL;
	}
}

void CRunView::OnNcPaint()
{
	DWORD dwStyle = GetStyle();
	DWORD dwExStyle = GetExStyle();
	DWORD dwScrollStyle = dwStyle & (WS_HSCROLL | WS_VSCROLL);

	// Paint scrollbars, if any.
	if (dwScrollStyle != 0)
		Default();

	if (dwScrollStyle == (WS_HSCROLL | WS_VSCROLL))
	{
		CWindowDC dc(this);

		CRect rc;
		GetWindowRect(&rc);
		rc.OffsetRect(-rc.TopLeft());

		// Workaround for Windows bug:
		// Draw the corner between the scrollbars

		int cxVScroll = GetSystemMetrics(SM_CXVSCROLL);

		if (dwExStyle & WS_EX_LEFTSCROLLBAR)    // Middle East Windows only
			rc.right = rc.left + cxVScroll;
		else
			rc.left = rc.right - cxVScroll;

		rc.top = rc.bottom - GetSystemMetrics(SM_CYVSCROLL);

		CBrush brushGUI(GetSysColor(COLOR_3DFACE));
		dc.FillRect(rc, &brushGUI);
	}
}
