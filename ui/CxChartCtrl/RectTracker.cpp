// RectTracker1.cpp: implementation of the CRectTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "RectTracker.h"

/////////////////////////////////////////////////////////////////////////////
// CRectTracker global state

#define CX_BORDER   1
#define CY_BORDER   1

// various GDI objects we need to draw
extern __declspec(selectany) HCURSOR _afxCursors[10] = { 0, };
extern __declspec(selectany) HBRUSH _afxHatchBrush = 0;
extern __declspec(selectany) HPEN _afxBlackDottedPen = 0;
extern __declspec(selectany) int _afxHandleSize = 0;

void AfxDeleteObject(HGDIOBJ* pObject)
{
	ATLASSERT(pObject != NULL);
	if (*pObject != NULL)
	{
		DeleteObject(*pObject);
		*pObject = NULL;
	}
}

void __cdecl AfxTrackerTerm()
{
	AfxDeleteObject((HGDIOBJ*)&_afxHatchBrush);
	AfxDeleteObject((HGDIOBJ*)&_afxBlackDottedPen);
}
char _afxTrackerTerm = (char)atexit(&AfxTrackerTerm);

// the struct below is used to determine the qualities of a particular handle
struct AFX_HANDLEINFO
{
	size_t nOffsetX;    // offset within RECT for X coordinate
	size_t nOffsetY;    // offset within RECT for Y coordinate
	int nCenterX;       // adjust X by Width()/2 * this number
	int nCenterY;       // adjust Y by Height()/2 * this number
	int nHandleX;       // adjust X by handle size * this number
	int nHandleY;       // adjust Y by handle size * this number
	int nInvertX;       // handle converts to this when X inverted
	int nInvertY;       // handle converts to this when Y inverted
};

// this array describes all 8 handles (clock-wise)
extern __declspec(selectany) const AFX_HANDLEINFO _afxHandleInfo[] =
{
	// corner handles (top-left, top-right, bottom-right, bottom-left
	{ offsetof(RECT, left), offsetof(RECT, top),        0, 0,  0,  0, 1, 3 },
	{ offsetof(RECT, right), offsetof(RECT, top),       0, 0, -1,  0, 0, 2 },
	{ offsetof(RECT, right), offsetof(RECT, bottom),    0, 0, -1, -1, 3, 1 },
	{ offsetof(RECT, left), offsetof(RECT, bottom),     0, 0,  0, -1, 2, 0 },

	// side handles (top, right, bottom, left)
	{ offsetof(RECT, left), offsetof(RECT, top),        1, 0,  0,  0, 4, 6 },
	{ offsetof(RECT, right), offsetof(RECT, top),       0, 1, -1,  0, 7, 5 },
	{ offsetof(RECT, left), offsetof(RECT, bottom),     1, 0,  0, -1, 6, 4 },
	{ offsetof(RECT, left), offsetof(RECT, top),        0, 1,  0,  0, 5, 7 }
};

// the struct below gives us information on the layout of a RECT struct and
//  the relationship between its members
struct AFX_RECTINFO
{
	size_t nOffsetAcross;   // offset of opposite point (ie. left->right)
	int nSignAcross;        // sign relative to that point (ie. add/subtract)
};

// this array is indexed by the offset of the RECT member / sizeof(int)
extern __declspec(selectany) const AFX_RECTINFO _afxRectInfo[] =
{
	{ offsetof(RECT, right), +1 },
	{ offsetof(RECT, bottom), +1 },
	{ offsetof(RECT, left), -1 },
	{ offsetof(RECT, top), -1 },
};

/////////////////////////////////////////////////////////////////////////////
// CRectTracker intitialization

CRectTracker::CRectTracker(LPCRECT lpSrcRect, UINT nStyle)
{
	Construct();
	m_rect.CopyRect(lpSrcRect);
	m_nStyle = nStyle;
}

CRectTracker::CRectTracker()
{ 
	Construct(); 
}

void CRectTracker::Construct()
{
	static BOOL bInitialized;
	if (!bInitialized)
	{
		// sanity checks for assumptions we make in the code
		ATLASSERT(sizeof(((RECT*)NULL)->left) == sizeof(int));
		ATLASSERT(offsetof(RECT, top) > offsetof(RECT, left));
		ATLASSERT(offsetof(RECT, right) > offsetof(RECT, top));
		ATLASSERT(offsetof(RECT, bottom) > offsetof(RECT, right));

		if (_afxHatchBrush == NULL)
		{
			// create the hatch pattern + bitmap
			WORD hatchPattern[8];
			WORD wPattern = 0x1111;
			for (int i = 0; i < 4; i++)
			{
				hatchPattern[i] = wPattern;
				hatchPattern[i+4] = wPattern;
				wPattern <<= 1;
			}
			HBITMAP hatchBitmap = CreateBitmap(8, 8, 1, 1, &hatchPattern);
			ATLASSERT(hatchBitmap != NULL);
			
			// create black hatched brush
			_afxHatchBrush = CreatePatternBrush(hatchBitmap);
			DeleteObject(hatchBitmap);
			ATLASSERT(_afxHatchBrush != NULL);
		}

		if (_afxBlackDottedPen == NULL)
		{
			// create black dotted pen
			_afxBlackDottedPen = CreatePen(PS_DOT, 0, RGB(0, 0, 0));
			ATLASSERT(_afxBlackDottedPen != NULL);
		}

		HINSTANCE hInst = _Module.GetResourceInstance();

		// initialize the cursor array
		_afxCursors[0] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACKNWSE));
		_afxCursors[1] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACKNESW));
		_afxCursors[2] = _afxCursors[0];
		_afxCursors[3] = _afxCursors[1];
		_afxCursors[4] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACKNS));
		_afxCursors[5] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACKWE));
		_afxCursors[6] = _afxCursors[4];
		_afxCursors[7] = _afxCursors[5];
		_afxCursors[8] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACK4WAY));
		_afxCursors[9] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_MOVE4WAY));

		// get default handle size from Windows profile setting
		static const TCHAR szWindows[] = _T("windows");
		static const TCHAR szInplaceBorderWidth[] =
			_T("oleinplaceborderwidth");
		_afxHandleSize = GetProfileInt(szWindows, szInplaceBorderWidth, 4);
		bInitialized = TRUE;
	}
	
	m_nStyle = 0;
	m_nHandleSize = _afxHandleSize;
	m_sizeMin.cy = m_sizeMin.cx = m_nHandleSize*2;

	m_rectLast.SetRectEmpty();
	m_sizeLast.cx = m_sizeLast.cy = 0;
	m_bErase = FALSE;
	m_bFinalErase =  FALSE;
}

CRectTracker::~CRectTracker()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRectTracker operations

void CRectTracker::Draw(HDC hDC) const
{
	CDCHandle dc = hDC;

	// set initial DC state
	dc.SaveDC();
	dc.SetMapMode(MM_TEXT);
	dc.SetViewportOrg(0, 0);
	dc.SetWindowOrg(0, 0);

	// get normalized rectangle
	CRect rect = m_rect;
	rect.NormalizeRect();

	CPenHandle penOld;
	CBrushHandle brushOld;
	HANDLE hTemp;
	int nOldROP;

	// draw lines
	if ((m_nStyle & (dottedLine|solidLine)) != 0)
	{
		if (m_nStyle & dottedLine)
			penOld = dc.SelectPen(_afxBlackDottedPen);
		else
			penOld = dc.SelectStockPen(BLACK_PEN);
		brushOld = dc.SelectStockBrush(NULL_BRUSH);
		nOldROP = dc.SetROP2(R2_COPYPEN);
		rect.InflateRect(+1, +1);   // borders are one pixel outside
		dc.Rectangle(rect.left, rect.top, rect.right, rect.bottom);
		dc.SetROP2(nOldROP);
	}

	// if hatchBrush is going to be used, need to unrealize it
	if ((m_nStyle & (hatchInside|hatchedBorder)) != 0)
		UnrealizeObject(_afxHatchBrush);

	// hatch inside
	if ((m_nStyle & hatchInside) != 0)
	{
		hTemp = dc.SelectStockPen(NULL_PEN);
		if (penOld == NULL)
			penOld = (HPEN)hTemp;
		hTemp = dc.SelectBrush(_afxHatchBrush);
		if (brushOld == NULL)
			brushOld = (HBRUSH)hTemp;
		dc.SetBkMode(TRANSPARENT);
		nOldROP = dc.SetROP2(R2_MASKNOTPEN);
		dc.Rectangle(rect.left+1, rect.top+1, rect.right, rect.bottom);
		dc.SetROP2(nOldROP);
	}

	// draw hatched border
	if ((m_nStyle & hatchedBorder) != 0)
	{
		hTemp = dc.SelectBrush(_afxHatchBrush);
		if (brushOld == NULL)
			brushOld = (HBRUSH)hTemp;
		dc.SetBkMode(OPAQUE);
		CRect rectTrue;
		GetTrueRect(&rectTrue);
		dc.PatBlt(rectTrue.left, rectTrue.top, rectTrue.Width(),
			rect.top-rectTrue.top, 0x000F0001 /* Pn */);
		dc.PatBlt(rectTrue.left, rect.bottom,
			rectTrue.Width(), rectTrue.bottom-rect.bottom, 0x000F0001 /* Pn */);
		dc.PatBlt(rectTrue.left, rect.top, rect.left-rectTrue.left,
			rect.Height(), 0x000F0001 /* Pn */);
		dc.PatBlt(rect.right, rect.top, rectTrue.right-rect.right,
			rect.Height(), 0x000F0001 /* Pn */);
	}

	// draw resize handles
	if ((m_nStyle & (resizeInside|resizeOutside)) != 0)
	{
		UINT mask = GetHandleMask();
		for (int i = 0; i < 8; ++i)
		{
			if (mask & (1<<i))
			{
				GetHandleRect((TrackerHit)i, &rect);
				dc.FillSolidRect(rect, RGB(0, 0, 0));
			}
		}
	}

	// cleanup pDC state
	if (penOld != NULL)
		dc.SelectPen(penOld);
	if (brushOld != NULL)
		dc.SelectBrush(brushOld);
	dc.RestoreDC(-1);
}

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

BOOL CRectTracker::SetCursor(HWND hWnd, UINT nHitTest) const
{
	// trackers should only be in client area
	if (nHitTest != HTCLIENT)
		return FALSE;

	// convert cursor position to client co-ordinates
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(hWnd, &point);

	// do hittest and normalize hit
	int nHandle = HitTestHandles(point);
	if (nHandle < 0)
		return FALSE;

	// need to normalize the hittest such that we get proper cursors
	nHandle = NormalizeHit(nHandle);

	// handle special case of hitting area between handles
	//  (logically the same -- handled as a move -- but different cursor)
	if (nHandle == hitMiddle && !m_rect.PtInRect(point))
	{
		// only for trackers with hatchedBorder (ie. in-place resizing)
		if (m_nStyle & hatchedBorder)
			nHandle = (TrackerHit)9;
	}

	ATLASSERT(nHandle < _countof(_afxCursors));
	::SetCursor(_afxCursors[nHandle]);
	return TRUE;
}

int CRectTracker::HitTest(CPoint point) const
{
	TrackerHit hitResult = hitNothing;

	CRect rectTrue;
	GetTrueRect(&rectTrue);
	ATLASSERT(rectTrue.left <= rectTrue.right);
	ATLASSERT(rectTrue.top <= rectTrue.bottom);
	if (rectTrue.PtInRect(point))
	{
		if ((m_nStyle & (resizeInside|resizeOutside)) != 0)
			hitResult = (TrackerHit)HitTestHandles(point);
		else
			hitResult = hitMiddle;
	}
	return hitResult;
}

int CRectTracker::NormalizeHit(int nHandle) const
{
	ATLASSERT(nHandle <= 8 && nHandle >= -1);
	if (nHandle == hitMiddle || nHandle == hitNothing)
		return nHandle;
	const AFX_HANDLEINFO* pHandleInfo = &_afxHandleInfo[nHandle];
	if (m_rect.Width() < 0)
	{
		nHandle = (TrackerHit)pHandleInfo->nInvertX;
		pHandleInfo = &_afxHandleInfo[nHandle];
	}
	if (m_rect.Height() < 0)
		nHandle = (TrackerHit)pHandleInfo->nInvertY;
	return nHandle;
}

BOOL CRectTracker::Track(HWND hWnd, CPoint point, BOOL bAllowInvert,
	HWND hWndClipTo)
{
	// perform hit testing on the handles
	int nHandle = HitTestHandles(point);
	if (nHandle < 0)
	{
		// didn't hit a handle, so just return FALSE
		return FALSE;
	}

	// otherwise, call helper function to do the tracking
	m_bAllowInvert = bAllowInvert;
	return TrackHandle(nHandle, hWnd, point, hWndClipTo);
}

BOOL CRectTracker::TrackRubberBand(HWND hWnd, CPoint point, BOOL bAllowInvert)
{
	// simply call helper function to track from bottom right handle
	m_bAllowInvert = bAllowInvert;
	m_rect.SetRect(point.x, point.y, point.x, point.y);
	return TrackHandle(hitBottomRight, hWnd, point, NULL);
}

// WTL的同名函数有内存泄漏
void DrawDragRect(HDC hDC, LPCRECT lpRect, SIZE size, LPCRECT lpRectLast, SIZE sizeLast)
{
	CDCHandle dc = hDC;

	// first, determine the update region and select it
	CRgn rgnNew;
	CRgn rgnOutside, rgnInside;
	rgnOutside.CreateRectRgnIndirect(lpRect);
	RECT rect = *lpRect;
	::InflateRect(&rect, -size.cx, -size.cy);
	::IntersectRect(&rect, &rect, lpRect);
	rgnInside.CreateRectRgnIndirect(&rect);
	rgnNew.CreateRectRgn(0, 0, 0, 0);
	rgnNew.CombineRgn(rgnOutside, rgnInside, RGN_XOR);
	
	HBRUSH hBrush = CDCHandle::GetHalftoneBrush();
	HBRUSH hBrushLast = hBrush;
	
	HBRUSH hBrushOld = NULL;
	
	CRgn rgnLast = NULL, rgnUpdate = NULL;
	if(lpRectLast != NULL)
	{
		// find difference between new region and old region
		rgnLast.CreateRectRgn(0, 0, 0, 0);
		rgnOutside.SetRectRgn(lpRectLast->left, lpRectLast->top, lpRectLast->right, lpRectLast->bottom);
		rect = *lpRectLast;
		::InflateRect(&rect, -sizeLast.cx, -sizeLast.cy);
		::IntersectRect(&rect, &rect, lpRectLast);
		rgnInside.SetRectRgn(rect.left, rect.top, rect.right, rect.bottom);
		rgnLast.CombineRgn(rgnOutside, rgnInside, RGN_XOR);
		
		// only diff them if brushes are the same
		if(hBrush == hBrushLast)
		{
			rgnUpdate.CreateRectRgn(0, 0, 0, 0);
			rgnUpdate.CombineRgn(rgnLast, rgnNew, RGN_XOR);
		}
	}
	if(hBrush != hBrushLast && lpRectLast != NULL)
	{
		// brushes are different -- erase old region first
		dc.SelectClipRgn(rgnLast);
		dc.GetClipBox(&rect);
		hBrushOld = dc.SelectBrush(hBrushLast);
		dc.PatBlt(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATINVERT);
		dc.SelectBrush(hBrushOld);
		hBrushOld = NULL;
	}
	
	// draw into the update/new region
	dc.SelectClipRgn(rgnUpdate != NULL ? rgnUpdate : rgnNew);
	dc.GetClipBox(&rect);
	hBrushOld = dc.SelectBrush(hBrush);
	dc.PatBlt(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATINVERT);
	
	// cleanup DC
	if(hBrushOld != NULL)
		dc.SelectBrush(hBrushOld);
	dc.SelectClipRgn(NULL);
	DeleteObject(hBrush);
}

void CRectTracker::DrawTrackerRect(
	LPCRECT lpRect, HWND hWndClipTo, HDC hDC, HWND hWnd)
{
	// first, normalize the rectangle for drawing
	CRect rect = *lpRect;
	rect.NormalizeRect();

	CWindow wnd = hWnd;
	CWindow wndClipTo = hWndClipTo;

	// convert to client coordinates
	if (wndClipTo != NULL)
	{
		wnd.ClientToScreen(&rect);
		wndClipTo.ScreenToClient(&rect);
	}

	CSize size(0, 0);
	if (!m_bFinalErase)
	{
		// otherwise, size depends on the style
		if (m_nStyle & hatchedBorder)
		{
			size.cx = size.cy = max(1, GetHandleSize(rect)-1);
			rect.InflateRect(size);
		}
		else
		{
			size.cx = CX_BORDER;
			size.cy = CY_BORDER;
		}
	}

	// and draw it
	if (m_bFinalErase || !m_bErase)
		DrawDragRect(hDC, rect, size, m_rectLast, m_sizeLast);

	// remember last rectangles
	m_rectLast = rect;
	m_sizeLast = size;
}

void CRectTracker::AdjustRect(int nHandle, LPRECT)
{
	if (nHandle == hitMiddle)
		return;

	// convert the handle into locations within m_rect
	int *px, *py;
	GetModifyPointers(nHandle, &px, &py, NULL, NULL);

	// enforce minimum width
	int nNewWidth = m_rect.Width();
	int nAbsWidth = m_bAllowInvert ? abs(nNewWidth) : nNewWidth;
	if (px != NULL && nAbsWidth < m_sizeMin.cx)
	{
		nNewWidth = nAbsWidth != 0 ? nNewWidth / nAbsWidth : 1;
		ATLASSERT((int*)px - (int*)&m_rect < _countof(_afxRectInfo));
		const AFX_RECTINFO* pRectInfo = &_afxRectInfo[(int*)px - (int*)&m_rect];
		*px = *(int*)((BYTE*)&m_rect + pRectInfo->nOffsetAcross) +
			nNewWidth * m_sizeMin.cx * -pRectInfo->nSignAcross;
	}

	// enforce minimum height
	int nNewHeight = m_rect.Height();
	int nAbsHeight = m_bAllowInvert ? abs(nNewHeight) : nNewHeight;
	if (py != NULL && nAbsHeight < m_sizeMin.cy)
	{
		nNewHeight = nAbsHeight != 0 ? nNewHeight / nAbsHeight : 1;
		ATLASSERT((int*)py - (int*)&m_rect < _countof(_afxRectInfo));
		const AFX_RECTINFO* pRectInfo = &_afxRectInfo[(int*)py - (int*)&m_rect];
		*py = *(int*)((BYTE*)&m_rect + pRectInfo->nOffsetAcross) +
			nNewHeight * m_sizeMin.cy * -pRectInfo->nSignAcross;
	}
}

void CRectTracker::GetTrueRect(LPRECT lpTrueRect) const
{
	CRect rect = m_rect;
	rect.NormalizeRect();
	int nInflateBy = 0;
	if ((m_nStyle & (resizeOutside|hatchedBorder)) != 0)
		nInflateBy += GetHandleSize() - 1;
	if ((m_nStyle & (solidLine|dottedLine)) != 0)
		++nInflateBy;
	rect.InflateRect(nInflateBy, nInflateBy);
	*lpTrueRect = rect;
}

void CRectTracker::OnChangedRect(const CRect& /*rectOld*/)
{
	// no default implementation, useful for derived classes
}

/////////////////////////////////////////////////////////////////////////////
// CRectTracker implementation helpers

void CRectTracker::GetHandleRect(int nHandle, CRect* pHandleRect) const
{
	ATLASSERT(nHandle < 8);

	// get normalized rectangle of the tracker
	CRect rectT = m_rect;
	rectT.NormalizeRect();
	if ((m_nStyle & (solidLine|dottedLine)) != 0)
		rectT.InflateRect(+1, +1);

	// since the rectangle itself was normalized, we also have to invert the
	//  resize handles.
	nHandle = NormalizeHit(nHandle);

	// handle case of resize handles outside the tracker
	int size = GetHandleSize();
	if (m_nStyle & resizeOutside)
		rectT.InflateRect(size-1, size-1);

	// calculate position of the resize handle
	int nWidth = rectT.Width();
	int nHeight = rectT.Height();
	CRect rect;
	const AFX_HANDLEINFO* pHandleInfo = &_afxHandleInfo[nHandle];
	rect.left = *(int*)((BYTE*)&rectT + pHandleInfo->nOffsetX);
	rect.top = *(int*)((BYTE*)&rectT + pHandleInfo->nOffsetY);
	rect.left += size * pHandleInfo->nHandleX;
	rect.top += size * pHandleInfo->nHandleY;
	rect.left += pHandleInfo->nCenterX * (nWidth - size) / 2;
	rect.top += pHandleInfo->nCenterY * (nHeight - size) / 2;
	rect.right = rect.left + size;
	rect.bottom = rect.top + size;

	*pHandleRect = rect;
}

int CRectTracker::GetHandleSize(LPCRECT lpRect) const
{
	if (lpRect == NULL)
		lpRect = &m_rect;

	int size = m_nHandleSize;
	if (!(m_nStyle & resizeOutside))
	{
		// make sure size is small enough for the size of the rect
		int sizeMax = min(abs(lpRect->right - lpRect->left),
			abs(lpRect->bottom - lpRect->top));
		if (size * 2 > sizeMax)
			size = sizeMax / 2;
	}
	return size;
}

int CRectTracker::HitTestHandles(CPoint point) const
{
	CRect rect;
	UINT mask = GetHandleMask();

	// see if hit anywhere inside the tracker
	GetTrueRect(&rect);
	if (!rect.PtInRect(point))
		return hitNothing;  // totally missed

	// see if we hit a handle
	for (int i = 0; i < 8; ++i)
	{
		if (mask & (1<<i))
		{
			GetHandleRect((TrackerHit)i, &rect);
			if (rect.PtInRect(point))
				return (TrackerHit)i;
		}
	}

	// last of all, check for non-hit outside of object, between resize handles
	if ((m_nStyle & hatchedBorder) == 0)
	{
		CRect rect = m_rect;
		rect.NormalizeRect();
		if ((m_nStyle & dottedLine|solidLine) != 0)
			rect.InflateRect(+1, +1);
		if (!rect.PtInRect(point))
			return hitNothing;  // must have been between resize handles
	}
	return hitMiddle;   // no handle hit, but hit object (or object border)
}

BOOL CRectTracker::TrackHandle(int nHandle, HWND hWnd, CPoint point,
	HWND hWndClipTo)
{
	ATLASSERT(nHandle >= 0);
	ATLASSERT(nHandle <= 8);   // handle 8 is inside the rect

	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	ATLASSERT(!m_bFinalErase);

	CWindow wnd = hWnd;
	CWindow wndClipTo = hWndClipTo;

	// save original width & height in pixels
	int nWidth = m_rect.Width();
	int nHeight = m_rect.Height();

	// set capture to the window which received this message
	wnd.SetCapture();
	ATLASSERT(wnd == GetCapture());
	wnd.UpdateWindow();
	if (wndClipTo != NULL)
		wndClipTo.UpdateWindow();
	CRect rectSave = m_rect;

	// find out what x/y coords we are supposed to modify
	int *px, *py;
	int xDiff, yDiff;
	GetModifyPointers(nHandle, &px, &py, &xDiff, &yDiff);
	xDiff = point.x - xDiff;
	yDiff = point.y - yDiff;

	// get DC for drawing
	CDCHandle dcDraw;
	if (wndClipTo != NULL)
	{
		// clip to arbitrary window by using adjusted Window DC
		dcDraw = wndClipTo.GetDCEx(NULL, DCX_CACHE);
	}
	else
	{
		// otherwise, just use normal DC
		dcDraw = wnd.GetDC();
	}

	CRect rectOld;
	BOOL bMoved = FALSE;

	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		::GetMessage(&msg, NULL, 0, 0);

		if (GetCapture() != wnd)
			break;

		switch (msg.message)
		{
		// handle movement/accept messages
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
			rectOld = m_rect;
			// handle resize cases (and part of move)
			if (px != NULL)
				*px = (int)(short)LOWORD(msg.lParam) - xDiff;
			if (py != NULL)
				*py = (int)(short)HIWORD(msg.lParam) - yDiff;

			// handle move case
			if (nHandle == hitMiddle)
			{
				m_rect.right = m_rect.left + nWidth;
				m_rect.bottom = m_rect.top + nHeight;
			}
			// allow caller to adjust the rectangle if necessary
			AdjustRect(nHandle, &m_rect);

			// only redraw and callback if the rect actually changed!
			m_bFinalErase = (msg.message == WM_LBUTTONUP);
			if (!rectOld.EqualRect(&m_rect) || m_bFinalErase)
			{
				if (bMoved)
				{
					m_bErase = TRUE;
					DrawTrackerRect(&rectOld, wndClipTo, dcDraw, wnd);
				}
				OnChangedRect(rectOld);
				if (msg.message != WM_LBUTTONUP)
					bMoved = TRUE;
			}
			if (m_bFinalErase)
				goto ExitLoop;

			if (!rectOld.EqualRect(&m_rect))
			{
				m_bErase = FALSE;
				DrawTrackerRect(&m_rect, wndClipTo, dcDraw, wnd);
			}
			break;

		// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_RBUTTONDOWN:
			if (bMoved)
			{
				m_bErase = m_bFinalErase = TRUE;
				DrawTrackerRect(&m_rect, wndClipTo, dcDraw, wnd);
			}
			m_rect = rectSave;
			goto ExitLoop;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	if (wndClipTo != NULL)
		wndClipTo.ReleaseDC(dcDraw);
	else
		wnd.ReleaseDC(dcDraw);
	ReleaseCapture();

	// restore rect in case bMoved is still FALSE
	if (!bMoved)
		m_rect = rectSave;
	m_bFinalErase = FALSE;
	m_bErase = FALSE;

	// return TRUE only if rect has changed
	return !rectSave.EqualRect(&m_rect);
}

void CRectTracker::GetModifyPointers(
	int nHandle, int** ppx, int** ppy, int* px, int* py)
{
	ATLASSERT(nHandle >= 0);
	ATLASSERT(nHandle <= 8);

	if (nHandle == hitMiddle)
		nHandle = hitTopLeft;   // same as hitting top-left

	*ppx = NULL;
	*ppy = NULL;

	// fill in the part of the rect that this handle modifies
	//  (Note: handles that map to themselves along a given axis when that
	//   axis is inverted don't modify the value on that axis)

	const AFX_HANDLEINFO* pHandleInfo = &_afxHandleInfo[nHandle];
	if (pHandleInfo->nInvertX != nHandle)
	{
		*ppx = (int*)((BYTE*)&m_rect + pHandleInfo->nOffsetX);
		if (px != NULL)
			*px = **ppx;
	}
	else
	{
		// middle handle on X axis
		if (px != NULL)
			*px = m_rect.left + abs(m_rect.Width()) / 2;
	}
	if (pHandleInfo->nInvertY != nHandle)
	{
		*ppy = (int*)((BYTE*)&m_rect + pHandleInfo->nOffsetY);
		if (py != NULL)
			*py = **ppy;
	}
	else
	{
		// middle handle on Y axis
		if (py != NULL)
			*py = m_rect.top + abs(m_rect.Height()) / 2;
	}
}

UINT CRectTracker::GetHandleMask() const
{
	UINT mask = 0x0F;   // always have 4 corner handles
	int size = m_nHandleSize*3;
	if (abs(m_rect.Width()) - size > 4)
		mask |= 0x50;
	if (abs(m_rect.Height()) - size > 4)
		mask |= 0xA0;
	return mask;
}
