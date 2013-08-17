/********************************************************************
	created:	2003/04/12
	created:	12:04:2003   10:50
	file base:	PPTooltip
	file ext:	cpp
	author:		Eugene Pustovoyt
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "PPToolTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPToolTip

CPPToolTip::CPPToolTip()
{
	m_pParentWnd = NULL;

	m_rgnShadow.CreateRectRgn(0, 0, 0, 0);
	m_rgnToolTip.CreateRectRgn(0, 0, 0, 0);
	// m_rgnCombo.CreateRectRgn(0, 0, 0, 0);

	m_ptOriginal.x = -1;
	m_ptOriginal.y = -1;

	m_nIndexCurrentWnd = PPTOOLTIP_TOOL_NOEXIST;
	m_nIndexDisplayWnd = PPTOOLTIP_TOOL_NOEXIST;

	SetDelayTime(TTDT_INITIAL, 500);
	SetDelayTime(TTDT_AUTOPOP, 5000);
	SetNotify(FALSE);
	SetDirection();
	SetBehaviour();
	SetDefaultStyles();
	SetDefaultColors();
	SetDefaultSizes();
	SetEffectBk(PPTOOLTIP_EFFECT_SOLID);
	RemoveAllTools();

	// Register the window class if it has not already been registered.
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, PPTOOLTIP_CLASSNAME, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style			= CS_SAVEBITS;
		wndcls.lpfnWndProc		= ::DefWindowProc;
		wndcls.cbClsExtra		= wndcls.cbWndExtra = 0;
		wndcls.hInstance		= hInst;
		wndcls.hIcon			= NULL;
		wndcls.hCursor			= LoadCursor(hInst, IDC_ARROW );
		wndcls.hbrBackground	= NULL;
		wndcls.lpszMenuName		= NULL;
		wndcls.lpszClassName	= PPTOOLTIP_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
			AfxThrowResourceException();
	}
}

CPPToolTip::~CPPToolTip()
{
	RemoveAllTools();
	RemoveAllNamesOfResource();

	m_nLengthLines.RemoveAll();
	m_nHeightLines.RemoveAll();

	// m_rgnCombo.Detach();
	// m_rgnCombo.DeleteObject();
	m_rgnToolTip.DeleteObject();
	m_rgnShadow.DeleteObject();

	if (IsWindow(m_hWnd))
        DestroyWindow();
}


BEGIN_MESSAGE_MAP(CPPToolTip, CWnd)
	//{{AFX_MSG_MAP(CPPToolTip)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPPToolTip message handlers

BOOL CPPToolTip::Create(CWnd* pParentWnd, BOOL bBalloonSize /* = TRUE */) 
{
	TRACE(_T("CPPToolTip::Create\n"));

	ASSERT_VALID(pParentWnd);

	DWORD dwStyle = WS_POPUP; 
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	m_pParentWnd = pParentWnd;

	if (!CreateEx(dwExStyle, PPTOOLTIP_CLASSNAME, NULL, dwStyle, 0, 0, 0, 0, pParentWnd->GetSafeHwnd(), NULL, NULL))
	{
		return FALSE;
	}

	SetDefaultFont();
	SetDefaultSizes(bBalloonSize);
	
	return TRUE;
}

void CPPToolTip::OnDestroy() 
{
	KillTimers();
	
	CWnd::OnDestroy();
}

void CPPToolTip::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	Pop();
}

BOOL CPPToolTip::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

void CPPToolTip::Pop()
{
	if (m_nIndexDisplayWnd == PPTOOLTIP_TOOL_HELPER)
		m_nIndexDisplayWnd = PPTOOLTIP_TOOL_NOEXIST;
	KillTimers();
	ShowWindow(SW_HIDE);
}

BOOL CPPToolTip::PreTranslateMessage(MSG* pMsg) 
{
	RelayEvent(pMsg);
	
	return CWnd::PreTranslateMessage(pMsg);
}

LRESULT CPPToolTip::SendNotify(CPoint * pt, PPTOOLTIP_INFO & ti) 
{ 
	TRACE(_T("CPPToolTip::SendNotify()\n")); 
 	// Make sure this is a valid window  
	if (!IsWindow(GetSafeHwnd())) 
		return 0L; 
  
	// See if the user wants to be notified  
	if (!GetNotify()) 
		return 0L; 
  	
	NM_PPTOOLTIP_DISPLAY lpnm; 
	PPTOOLTIP_INFO tiCopy = ti; 
	FromHandle(ti.hWnd)->ClientToScreen(&tiCopy.rectBounds); 
	m_pParentWnd->ScreenToClient(&tiCopy.rectBounds); 
	lpnm.pt = pt;  
	lpnm.ti = &tiCopy; 
	lpnm.hdr.hwndFrom = m_hWnd; 
	lpnm.hdr.idFrom   = GetDlgCtrlID(); 
	lpnm.hdr.code     = UDM_TOOLTIP_DISPLAY; 
	 
	::SendMessage(m_hNotifyWnd, WM_NOTIFY, lpnm.hdr.idFrom, (LPARAM)&lpnm);  
	CRect rcBound = ti.rectBounds;  
	ti = tiCopy; 
	ti.rectBounds = rcBound;  
	return 0L; 
}

void CPPToolTip::OnPaint() 
{
	if (!IsEnabledIndexTool(m_nIndexCurrentWnd))
		return;

	m_nIndexDisplayWnd = m_nIndexCurrentWnd;
	m_nIndexCurrentWnd = PPTOOLTIP_TOOL_NOEXIST;

	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(0, 0, 1, 1);

	// Create a memory device-context. This is done to help reduce
	// screen flicker, since we will paint the entire control to the
	// off screen device context first.CDC memDC;
	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap); 
	
	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0,0, SRCCOPY);

	OnDraw(&memDC, rect);

	//Copy the memory device context back into the original DC via BitBlt().
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0,0, SRCCOPY);
	
	//Cleanup resources.
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject(); 
}

void CPPToolTip::OnDraw(CDC * pDC, CRect rect)
{
	CBrush brBackground(m_crColor [PPTOOLTIP_COLOR_BK_BEGIN]);
	CBrush brBorder(m_crColor [PPTOOLTIP_COLOR_BORDER]);
	
	pDC->SetBkMode(TRANSPARENT); 

	//Sets clip region of the tooltip and draws the shadow if you need
	if (m_pToolInfo.nStyles & PPTOOLTIP_SHADOW)
	{
		//Draws the shadow for the tooltip
		OnDrawShadow(pDC);
		rect.DeflateRect(0, 0, m_nSizes[PPTTSZ_SHADOW_CX], m_nSizes[PPTTSZ_SHADOW_CY]);
	}
	pDC->SelectClipRgn(&m_rgnToolTip);

	OnDrawBackground(pDC, &rect);

	//Draws the main region's border of the tooltip
	pDC->FrameRgn(&m_rgnToolTip, &brBorder, m_nSizes[PPTTSZ_BORDER_CX], m_nSizes[PPTTSZ_BORDER_CY]);

	//Gets the rectangle to draw the tooltip text
	rect.DeflateRect(m_nSizes[PPTTSZ_MARGIN_CX], m_nSizes[PPTTSZ_MARGIN_CY]);
	if ((m_nLastDirection == PPTOOLTIP_RIGHT_BOTTOM) || (m_nLastDirection == PPTOOLTIP_LEFT_BOTTOM))
		rect.top += m_nSizes[PPTTSZ_HEIGHT_ANCHOR];
	else
		rect.bottom -= m_nSizes[PPTTSZ_HEIGHT_ANCHOR];

	// Draw the icon
	if (m_pToolInfo.hIcon != NULL)
	{
		CPoint ptIcon;
		ptIcon.x = m_nSizes[PPTTSZ_MARGIN_CX];
		ptIcon.y = rect.top;
		if (m_pToolInfo.nStyles & PPTOOLTIP_ICON_VCENTER_ALIGN)
			ptIcon.y = rect.top + (rect.Height() - m_szToolIcon.cy) / 2;
		else if (m_pToolInfo.nStyles & PPTOOLTIP_ICON_BOTTOM_ALIGN)
			ptIcon.y = rect.bottom - m_szToolIcon.cy;
		//First variant
//		pDC->DrawIcon(m_nSizes[PPTTSZ_MARGIN_CX], rect.top + (rect.Height() - m_szToolIcon.cy) / 2, m_pToolInfo.hIcon);

		//Second variant
		pDC->DrawState(ptIcon, m_szToolIcon, m_pToolInfo.hIcon, DSS_NORMAL, (CBrush*)NULL);

		//Third variant
//		DrawIconEx(pDC->m_hDC, ptIcon.x, ptIcon.y, m_pToolInfo.hIcon, m_szToolIcon.cx, 
//			m_szToolIcon.cy, 0, NULL, DI_NORMAL);

		rect.left += m_szToolIcon.cx + m_nSizes[PPTTSZ_MARGIN_CX]; 
	}

	//Aligns tooltip's text
	if (m_pToolInfo.nStyles & PPTOOLTIP_BOTTOM_ALIGN)
		rect.top = rect.bottom - m_szTextTooltip.cy;
	else if (m_pToolInfo.nStyles & PPTOOLTIP_VCENTER_ALIGN)
		rect.top += (rect.Height() - m_szTextTooltip.cy) / 2;

	//Prints the tooltip's text
	PrintTitleString(pDC, rect, m_pToolInfo.sTooltip, FALSE);

	brBackground.DeleteObject();
	brBorder.DeleteObject();
}

void CPPToolTip::OnDrawShadow(CDC * pDC)
{
	CBrush brShadow(m_crColor [PPTOOLTIP_COLOR_SHADOW]);
	//Draws the shadow for the tooltip
	int nRop2Mode = pDC->SetROP2(R2_MASKPEN);
	pDC->FillRgn(&m_rgnShadow, &brShadow);
	pDC->SetROP2(nRop2Mode);
	brShadow.DeleteObject();
}

void CPPToolTip::OnDrawBackground(CDC * pDC, CRect * pRect)
{
	switch (m_pToolInfo.nEffect)
	{
	default:
		pDC->FillSolidRect(pRect, m_crColor[PPTOOLTIP_COLOR_BK_BEGIN]);
		break;
	case PPTOOLTIP_EFFECT_HGRADIENT:
		FillGradient(pDC, pRect, m_crColor[PPTOOLTIP_COLOR_BK_BEGIN], m_crColor [PPTOOLTIP_COLOR_BK_END], TRUE);
		break;
	case PPTOOLTIP_EFFECT_VGRADIENT:
		FillGradient(pDC, pRect, m_crColor[PPTOOLTIP_COLOR_BK_BEGIN], m_crColor [PPTOOLTIP_COLOR_BK_END], FALSE);
		break;
	case PPTOOLTIP_EFFECT_HCGRADIENT:
		FillGradient(pDC, CRect(pRect->left, pRect->top, pRect->left + pRect->Width() / 2, pRect->bottom), 
			m_crColor[PPTOOLTIP_COLOR_BK_BEGIN], m_crColor [PPTOOLTIP_COLOR_BK_END], TRUE);
		FillGradient(pDC, CRect(pRect->left + pRect->Width() / 2, pRect->top, pRect->right, pRect->bottom), 
			m_crColor[PPTOOLTIP_COLOR_BK_END], m_crColor [PPTOOLTIP_COLOR_BK_BEGIN], TRUE);
		break;
	case PPTOOLTIP_EFFECT_VCGRADIENT:
		FillGradient(pDC, CRect (pRect->left, pRect->top, pRect->right, pRect->top + pRect->Height() / 2), 
			m_crColor[PPTOOLTIP_COLOR_BK_BEGIN], m_crColor [PPTOOLTIP_COLOR_BK_END], FALSE);
		FillGradient(pDC, CRect (pRect->left, pRect->top + pRect->Height() / 2, pRect->right, pRect->bottom), 
			m_crColor[PPTOOLTIP_COLOR_BK_END], m_crColor [PPTOOLTIP_COLOR_BK_BEGIN], FALSE);
		break;
	case PPTOOLTIP_EFFECT_3HGRADIENT:
		FillGradient(pDC, CRect(pRect->left, pRect->top, pRect->left + pRect->Width()/2, pRect->bottom), 
			m_crColor[PPTOOLTIP_COLOR_BK_BEGIN], m_crColor [PPTOOLTIP_COLOR_BK_MID], TRUE);
		FillGradient(pDC, CRect(pRect->left + pRect->Width() / 2, pRect->top, pRect->right, pRect->bottom), 
			m_crColor[PPTOOLTIP_COLOR_BK_MID], m_crColor [PPTOOLTIP_COLOR_BK_END], TRUE);
		break;
	case PPTOOLTIP_EFFECT_3VGRADIENT:
		FillGradient(pDC, CRect (pRect->left, pRect->top, pRect->right, pRect->top + pRect->Height() / 2), 
			m_crColor[PPTOOLTIP_COLOR_BK_BEGIN], m_crColor [PPTOOLTIP_COLOR_BK_MID], FALSE);
		FillGradient(pDC, CRect (pRect->left, pRect->top + pRect->Height() / 2, pRect->right, pRect->bottom), 
			m_crColor[PPTOOLTIP_COLOR_BK_MID], m_crColor [PPTOOLTIP_COLOR_BK_END], FALSE);
		break;
#ifdef PPTOOLTIP_USE_SHADE
	case PPTOOLTIP_EFFECT_NOISE:
	case PPTOOLTIP_EFFECT_DIAGSHADE:
	case PPTOOLTIP_EFFECT_HSHADE:
	case PPTOOLTIP_EFFECT_VSHADE:
	case PPTOOLTIP_EFFECT_HBUMP:
	case PPTOOLTIP_EFFECT_VBUMP:
	case PPTOOLTIP_EFFECT_SOFTBUMP:
	case PPTOOLTIP_EFFECT_HARDBUMP:
	case PPTOOLTIP_EFFECT_METAL:
		m_dNormal.Draw(pDC->GetSafeHdc(),0,0);
		break;
#endif
	}
}

void CPPToolTip::RelayEvent(MSG* pMsg)
{
	ASSERT(m_pParentWnd);
	CWnd * pWnd = NULL;
	CPoint pt;
	CRect rect;
	CString str;
	int nIndexTool = PPTOOLTIP_TOOL_NOEXIST;

//	PPTOOLTIP_INFO  Info;
		
	switch(pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
	case WM_NCLBUTTONDOWN:
	case WM_NCLBUTTONDBLCLK:
	case WM_NCRBUTTONDOWN:
	case WM_NCRBUTTONDBLCLK:
	case WM_NCMBUTTONDOWN:
	case WM_NCMBUTTONDBLCLK:
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		// The user has interupted the current tool - dismiss it
		Pop();
		break;
	case WM_MOUSEMOVE:
//		TRACE (_T("OnMouseMove()\n"));
		if ((m_ptOriginal == pMsg->pt) || 
			(m_nIndexCurrentWnd == PPTOOLTIP_TOOL_HELPER) ||
			(m_nIndexDisplayWnd == PPTOOLTIP_TOOL_HELPER))
			return; //Mouse pointer was not move

		//Check Active window
		if (!(m_nStyles & PPTOOLTIP_SHOW_INACTIVE))
		{
			pWnd = GetActiveWindow();
			if (!pWnd)
				return;
		}

		m_ptOriginal = pMsg->pt; //Stores the mouse's coordinates
		
		//Gets the real window under the mouse
		pt = pMsg->pt;
		m_pParentWnd->ScreenToClient(&pt);
		
		nIndexTool = FindTool(pt);

		if (!IsExistTool(nIndexTool))
		{
			//If the window under the mouse isn't exist
			if (IsCursorInToolTip() && (m_pToolInfo.nBehaviour & PPTOOLTIP_CLOSE_LEAVEWND))
				return;
			Pop();
			m_nIndexCurrentWnd = PPTOOLTIP_TOOL_NOEXIST;
			m_nIndexDisplayWnd = PPTOOLTIP_TOOL_NOEXIST;
			return;
		}
		else
		{
			//The window under the mouse is exist
			if (nIndexTool == m_nIndexDisplayWnd)
			{
				if (IsVisible())
				{
					//Now the tooltip is visible
					if ((m_pToolInfo.nBehaviour & PPTOOLTIP_CLOSE_LEAVEWND))
						return;
				}
				if (m_pToolInfo.nBehaviour & PPTOOLTIP_MULTIPLE_SHOW)
				{
					SetNewToolTip(nIndexTool);
				}
				else Pop();
			}
			else
			{
				SetNewToolTip(nIndexTool, !IsVisible());
			}
		}
		break;
	}
}

void CPPToolTip::SetNewToolTip(int nIndexTool, BOOL bWithDelay /* = TRUE */)
{
	TRACE (_T("CPPToolTip::SetNewToolTip(Index = 0x%X)\n"), nIndexTool);
	
	m_nIndexDisplayWnd = PPTOOLTIP_TOOL_NOEXIST; //reset the displayed window
	Pop();

	//Gets the info about current tool
	if (!GetTool(nIndexTool, m_pToolInfo))
		return;

	//Remembers the pointer to the current window
	m_nIndexCurrentWnd = nIndexTool;

	//Start the show timer
	if (bWithDelay)
		SetTimer(PPTOOLTIP_SHOW, m_nTimeInitial, NULL);
	else
		OnTimer(PPTOOLTIP_SHOW);
}

void CPPToolTip::OnTimer(UINT nIDEvent) 
{
	CPoint pt = m_ptOriginal, point;
	CString str;
	int nIndexTool = PPTOOLTIP_TOOL_HELPER;

	switch (nIDEvent)
	{
	case PPTOOLTIP_SHOW:
		TRACE(_T("OnTimerShow\n"));
		Pop();
		if (m_nIndexCurrentWnd != PPTOOLTIP_TOOL_HELPER)
		{
			GetCursorPos(&pt);
			point = pt;
			m_pParentWnd->ScreenToClient(&point);
			nIndexTool = FindTool(point);
		}
		if ((nIndexTool == m_nIndexCurrentWnd) && (pt == m_ptOriginal) && IsEnabledIndexTool(nIndexTool))
		{
			PrepareDisplayToolTip(&pt);
			if (m_nTimeAutoPop && !(m_pToolInfo.nBehaviour & PPTOOLTIP_DISABLE_AUTOPOP)) //Don't hide window if autopop is 0
				SetTimer(PPTOOLTIP_HIDE, m_nTimeAutoPop, NULL);
		}
		break;
	case PPTOOLTIP_HIDE:
		TRACE(_T("OnTimerHide\n"));
		if (!IsCursorInToolTip() || 
			!IsVisible() || 
			!(m_pToolInfo.nBehaviour & PPTOOLTIP_NOCLOSE_OVER))
			Pop();
		break;
	}
	
	CWnd::OnTimer(nIDEvent);
}

BOOL CPPToolTip::IsEnabledIndexTool(int nIndex)
{
	return (BOOL)(IsExistTool(nIndex) || (nIndex == PPTOOLTIP_TOOL_HELPER));
}

BOOL CPPToolTip::IsCursorInToolTip() const
{
    ASSERT(m_pParentWnd);
	
    // Is tooltip visible?
    if (!IsVisible() || !IsWindow(m_hWnd))
		return FALSE;
	
    CPoint pt;
    GetCursorPos(&pt);
	
	CPPToolTip * pWnd = (CPPToolTip*)WindowFromPoint(pt);
	
	return (pWnd == this);
}

void CPPToolTip::KillTimers(UINT nIDTimer /* = NULL */)
{
//	TRACE (_T("CPPToolTip::KillTimers\n"));
	if (nIDTimer == NULL)
	{
		KillTimer(PPTOOLTIP_SHOW);
		KillTimer(PPTOOLTIP_HIDE);
	}
	else if (nIDTimer == PPTOOLTIP_SHOW)
		KillTimer(PPTOOLTIP_SHOW);
	else if (nIDTimer == PPTOOLTIP_HIDE)
		KillTimer(PPTOOLTIP_HIDE);
}

void CPPToolTip::PrepareDisplayToolTip(CPoint * pt)
{
	TRACE (_T("CPPToolTip::DisplayToolTip()\n"));
	
	//Fills default members
	if (!(m_pToolInfo.nMask & PPTOOLTIP_MASK_STYLES))
		m_pToolInfo.nStyles = m_nStyles;
	if (!(m_pToolInfo.nMask & PPTOOLTIP_MASK_EFFECT))
	{
		m_pToolInfo.nEffect = m_nEffect;
		m_pToolInfo.nGranularity = m_nGranularity;
	}
	if (!(m_pToolInfo.nMask & PPTOOLTIP_MASK_COLORS))
	{
		m_pToolInfo.crBegin = m_crColor[PPTOOLTIP_COLOR_BK_BEGIN];
		m_pToolInfo.crMid = m_crColor[PPTOOLTIP_COLOR_BK_MID];
		m_pToolInfo.crEnd = m_crColor[PPTOOLTIP_COLOR_BK_END];
	}
	if (!(m_pToolInfo.nMask & PPTOOLTIP_MASK_DIRECTION))
		m_pToolInfo.nDirection = m_nDirection;
	if (!(m_pToolInfo.nMask & PPTOOLTIP_MASK_BEHAVIOUR))
		m_pToolInfo.nBehaviour = m_nBehaviour;
	
	//Send notify
	SendNotify(pt, m_pToolInfo);

	//If string and icon are not exist then exit
	if (/*(m_pToolInfo.hIcon == NULL) && */m_pToolInfo.sTooltip.IsEmpty())
		return;
	
	//calculate the width and height of the box dynamically
    CSize sz = GetTooltipSize(m_pToolInfo.sTooltip);
	m_szTextTooltip = sz; //Stores the real size of the tooltip's text
	
	//Gets size of the current icon
	m_szToolIcon = GetSizeIcon(m_pToolInfo.hIcon);
	if (m_szToolIcon.cx || m_szToolIcon.cy)
	{
		sz.cx += m_szToolIcon.cx;
		if (m_szTextTooltip.cx != 0)
			sz.cx += m_nSizes[PPTTSZ_MARGIN_CX]; //If text is exist then adds separator
		sz.cy = max(m_szToolIcon.cy, sz.cy);
	}

	//Gets size of the tooltip with margins
	sz.cx += m_nSizes[PPTTSZ_MARGIN_CX] * 2;
	sz.cy += m_nSizes[PPTTSZ_MARGIN_CY] * 2 + m_nSizes[PPTTSZ_HEIGHT_ANCHOR];
	if (m_pToolInfo.nStyles & PPTOOLTIP_SHADOW)
	{
		sz.cx += m_nSizes[PPTTSZ_SHADOW_CX];
		sz.cy += m_nSizes[PPTTSZ_SHADOW_CY];
	}
	
	CRect rect (0, 0, sz.cx, sz.cy);
	
#ifdef PPTOOLTIP_USE_SHADE
	//If needed to create the bitmap of the background effect
	switch (m_pToolInfo.nEffect)
	{
	case PPTOOLTIP_EFFECT_NOISE:
	case PPTOOLTIP_EFFECT_DIAGSHADE:
	case PPTOOLTIP_EFFECT_HSHADE:
	case PPTOOLTIP_EFFECT_VSHADE:
	case PPTOOLTIP_EFFECT_HBUMP:
	case PPTOOLTIP_EFFECT_VBUMP:
	case PPTOOLTIP_EFFECT_SOFTBUMP:
	case PPTOOLTIP_EFFECT_HARDBUMP:
	case PPTOOLTIP_EFFECT_METAL:
		SetShade(rect, m_pToolInfo.nEffect, m_nGranularity, 5, m_pToolInfo.crBegin);
	}
#endif

	DisplayToolTip(pt, &rect);
}

#undef CopyRgn

void CPPToolTip::DisplayToolTip(CPoint * pt, CRect * rect)
{
	//Calculate the placement on the screen
	CalculateInfoBoxRect(pt, rect);

	SetWindowPos(NULL, 
                 rect->left, rect->top,
                 rect->Width() + 2, rect->Height() + 2,
                 SWP_SHOWWINDOW|SWP_NOCOPYBITS|SWP_NOACTIVATE|SWP_NOZORDER);

	CRgn rgnCombo;
	rgnCombo.CreateRectRgn(0, 0, 0, 0);
	if (m_pToolInfo.nStyles & PPTOOLTIP_SHADOW)
	{
		rect->right -= m_nSizes[PPTTSZ_SHADOW_CX];
		rect->bottom -= m_nSizes[PPTTSZ_SHADOW_CY];
	}
	m_rgnToolTip.DeleteObject();
	GetWindowRegion(&m_rgnToolTip, CSize (rect->Width(), rect->Height()), *pt);
	rgnCombo.CopyRgn(&m_rgnToolTip);
	if (m_pToolInfo.nStyles & PPTOOLTIP_SHADOW)
	{
		m_rgnShadow.DeleteObject();
		m_rgnShadow.CreateRectRgn(0, 0, 0, 0);
		m_rgnShadow.CopyRgn(&m_rgnToolTip);
		m_rgnShadow.OffsetRgn(m_nSizes[PPTTSZ_SHADOW_CX], m_nSizes[PPTTSZ_SHADOW_CY]);
		rgnCombo.CombineRgn(&rgnCombo, &m_rgnShadow, RGN_OR);
	}
	SetWindowRgn((HRGN)rgnCombo.Detach(), FALSE);
}

CRect CPPToolTip::GetWindowRegion(CRgn * rgn, CSize sz, CPoint pt)
{
	CRect rect;
	rect.SetRect(0, 0, sz.cx, sz.cy);
	CRgn rgnRect;
	CRgn rgnAnchor;
	CPoint ptAnchor [3];
	ptAnchor [0] = pt;
	ScreenToClient(&ptAnchor [0]);
	
	switch (m_nLastDirection)
	{
	case PPTOOLTIP_LEFT_TOP:
	case PPTOOLTIP_RIGHT_TOP:
		rect.bottom -= m_nSizes[PPTTSZ_HEIGHT_ANCHOR];
		ptAnchor [1].y = ptAnchor [2].y = rect.bottom;
		break;
	case PPTOOLTIP_LEFT_BOTTOM:
	case PPTOOLTIP_RIGHT_BOTTOM:
		rect.top += m_nSizes[PPTTSZ_HEIGHT_ANCHOR];
		ptAnchor [1].y = ptAnchor [2].y = rect.top;
		break;
	}
	
	//Gets the region for rectangle with the text
	if (m_pToolInfo.nStyles & PPTOOLTIP_ROUNDED)
		rgnRect.CreateRoundRectRgn(rect.left, rect.top, rect.right + 1, rect.bottom + 1, 
		m_nSizes[PPTTSZ_ROUNDED_CX], m_nSizes[PPTTSZ_ROUNDED_CY]);
	else rgnRect.CreateRectRgn(rect.left, rect.top, rect.right + 1, rect.bottom + 1);
	
	//Gets the region for anchor
	if (m_pToolInfo.nStyles & PPTOOLTIP_ANCHOR)
	{
		switch (m_nLastDirection)
		{
		case PPTOOLTIP_LEFT_TOP:
		case PPTOOLTIP_LEFT_BOTTOM:
			ptAnchor [1].x = rect.right - m_nSizes[PPTTSZ_MARGIN_ANCHOR];
			ptAnchor [2].x = ptAnchor [1].x - m_nSizes[PPTTSZ_WIDTH_ANCHOR];
			break;
		case PPTOOLTIP_RIGHT_TOP:
		case PPTOOLTIP_RIGHT_BOTTOM:
			ptAnchor [1].x = rect.left + m_nSizes[PPTTSZ_MARGIN_ANCHOR];
			ptAnchor [2].x = ptAnchor [1].x + m_nSizes[PPTTSZ_WIDTH_ANCHOR];
			break;
		}
		rgnAnchor.CreatePolygonRgn(ptAnchor, 3, ALTERNATE);
	}
	else
		rgnAnchor.CreateRectRgn(0, 0, 0, 0);
	
	rgn->CreateRectRgn(0, 0, 0, 0);
	rgn->CombineRgn(&rgnRect, &rgnAnchor, RGN_OR);
	
	rgnAnchor.DeleteObject();
	rgnRect.DeleteObject();
	
	return rect;
}

///////////////////////////////////////////////////
//	Gets the size of the current tooltip text
//
//	Parameters:
//		none
//
//	Return value:
//		Size of current tooltip text
///////////////////////////////////////////////////
CSize CPPToolTip::GetTooltipSize(CString str)
{
	//Gets max windows rectangle
	CRect rect;
	GetWindowRect(&rect);

	//Creates compatibility context device in memory
//	CDC * pDC = GetDC();
	CWindowDC dc(NULL);

	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
//	memDC.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
//	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	//Prints the string on device context for gets minimal size of the rectangle 
	//of the tooltip
	CSize sz = PrintTitleString(&memDC, rect, str);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

//	ReleaseDC(pDC);

	//Returns minimal rectangle of the tooltip
	return sz;
}

CSize CPPToolTip::GetSizeIcon(HICON hIcon) const
{
	ICONINFO ii;
	CSize sz (0, 0);

	if (hIcon != NULL)
	{
		// Gets icon dimension
		::ZeroMemory(&ii, sizeof(ICONINFO));
		if (::GetIconInfo(hIcon, &ii))
		{
			sz.cx = (DWORD)(ii.xHotspot * 2);
			sz.cy = (DWORD)(ii.yHotspot * 2);
			//release icon mask bitmaps
			if(ii.hbmMask)
				::DeleteObject(ii.hbmMask);
			if(ii.hbmColor)
				::DeleteObject(ii.hbmColor);
		}
	}
	return sz;
}

///////////////////////////////////////////////////
//	Calculates the real rect for the tooltip with margins
//
//	Parameters:
//		pt		[in] - the mouse coordinates (screen coordinates)
//		sz		[in] - the size of the tooltip text
//
//	Return value:
//		The rectangle when the tooltip will draw (screen coordinates)
///////////////////////////////////////////////////
void CPPToolTip::CalculateInfoBoxRect(CPoint * pt, CRect * rect)
{
	// Use the screen's right edge as the right hand border, not the right edge of the client.
	CWindowDC wdc(NULL);
	CRect rWindow(0, 0, 0, 0);
	rWindow.right = GetDeviceCaps(wdc, HORZRES);// - 8;
	rWindow.bottom = GetDeviceCaps(wdc, VERTRES);// - 8;
/*
	m_szToolIcon = GetSizeIcon(m_pToolInfo.hIcon);
	if (m_szToolIcon.cx || m_szToolIcon.cy)
	{
		sz.cx += m_szToolIcon.cx + m_nSizes[PPTTSZ_MARGIN_CX];
		sz.cy = max(m_szToolIcon.cy, sz.cy);
	}

	//Gets size of the tooltip with margins
	sz.cx += m_nSizes[PPTTSZ_MARGIN_CX] * 2;
	sz.cy += m_nSizes[PPTTSZ_MARGIN_CY] * 2 + m_nSizes[PPTTSZ_HEIGHT_ANCHOR];
	if (m_pToolInfo.nStyles & PPTOOLTIP_SHADOW)
	{
		rWindow.right -= m_nSizes[PPTTSZ_SHADOW_CX];
		rWindow.bottom -= m_nSizes[PPTTSZ_SHADOW_CY];
		sz.cx += m_nSizes[PPTTSZ_SHADOW_CX];
		sz.cy += m_nSizes[PPTTSZ_SHADOW_CY];
	}
*/	
//	CRect rect;
//	rect.SetRect(0, 0, sz.cx, sz.cy);
//	CRect rectCopy = *rect;
	
	//Offset the rect from the mouse pointer
	CPoint ptEnd;
	m_nLastDirection = m_pToolInfo.nDirection;
	
	if (!TestHorizDirection(pt->x, rect->Width(), rWindow.right, m_nLastDirection, rect))
	{
		m_nLastDirection = GetNextHorizDirection(m_nLastDirection);
		TestHorizDirection(pt->x, rect->Width(), rWindow.right, m_nLastDirection, rect);
	}
	if (!TestVertDirection(pt->y, rect->Height(), rWindow.bottom, m_nLastDirection, rect))
	{
		m_nLastDirection = GetNextVertDirection(m_nLastDirection);
		TestVertDirection(pt->y, rect->Height(), rWindow.bottom, m_nLastDirection, rect);
	}

	//Returns the rect of the tooltip
	if ((m_pToolInfo.nStyles & PPTOOLTIP_SHADOW) && 
		((m_nLastDirection == PPTOOLTIP_LEFT_TOP) || (m_nLastDirection == PPTOOLTIP_LEFT_BOTTOM)))
		rect->OffsetRect(m_nSizes[PPTTSZ_SHADOW_CX], m_nSizes[PPTTSZ_SHADOW_CY]);
}

///////////////////////////////////////////////////
//	Gets the next horizontal direction
//
//	Parameters:
//		nDirection		[in] - the current direction
//
//	Return value:
//		The next horizontal direction
///////////////////////////////////////////////////
int CPPToolTip::GetNextHorizDirection(int nDirection) const
{
	switch (nDirection)
	{
	case PPTOOLTIP_LEFT_TOP:
		nDirection = PPTOOLTIP_RIGHT_TOP;
		break;
	case PPTOOLTIP_RIGHT_TOP:
		nDirection = PPTOOLTIP_LEFT_TOP;
		break;
	case PPTOOLTIP_LEFT_BOTTOM:
		nDirection = PPTOOLTIP_RIGHT_BOTTOM;
		break;
	case PPTOOLTIP_RIGHT_BOTTOM:
		nDirection = PPTOOLTIP_LEFT_BOTTOM;
		break;
	}
	return nDirection;
}

///////////////////////////////////////////////////
//	Gets the next vertical direction
//
//	Parameters:
//		nDirection		[in] - the current direction
//
//	Return value:
//		The next vertical direction
///////////////////////////////////////////////////
int CPPToolTip::GetNextVertDirection(int nDirection) const
{
	switch (nDirection)
	{
	case PPTOOLTIP_LEFT_TOP:
		nDirection = PPTOOLTIP_LEFT_BOTTOM;
		break;
	case PPTOOLTIP_LEFT_BOTTOM:
		nDirection = PPTOOLTIP_LEFT_TOP;
		break;
	case PPTOOLTIP_RIGHT_TOP:
		nDirection = PPTOOLTIP_RIGHT_BOTTOM;
		break;
	case PPTOOLTIP_RIGHT_BOTTOM:
		nDirection = PPTOOLTIP_RIGHT_TOP;
		break;
	}
	return nDirection;
}

BOOL CPPToolTip::TestHorizDirection(int x, int cx, int w_cx, int nDirection, LPRECT rect) const
{
	int left,right;
	
	switch (nDirection)
	{
	case PPTOOLTIP_LEFT_TOP:
	case PPTOOLTIP_LEFT_BOTTOM:
		right = ((x + (int)m_nSizes[PPTTSZ_MARGIN_ANCHOR]) > w_cx) ? w_cx : (x + m_nSizes[PPTTSZ_MARGIN_ANCHOR]);
		left = right - cx;
		break;
	case PPTOOLTIP_RIGHT_TOP:
	case PPTOOLTIP_RIGHT_BOTTOM:
		left = (x < (int)m_nSizes[PPTTSZ_MARGIN_ANCHOR]) ? 0 : (x - m_nSizes[PPTTSZ_MARGIN_ANCHOR]);
		right = left + cx;
		break;
	}

	BOOL bTestOk = ((left >= 0) && (right <= w_cx)) ? TRUE : FALSE;
	if (bTestOk)
	{
		rect->left = left;
		rect->right = right;
	}

	return bTestOk;
}

BOOL CPPToolTip::TestVertDirection(int y, int cy, int w_cy, int nDirection, LPRECT rect) const
{
	int top, bottom;

	switch (nDirection)
	{
	case PPTOOLTIP_LEFT_TOP:
	case PPTOOLTIP_RIGHT_TOP:
		bottom = y;
		top = bottom - cy;
		break;
	case PPTOOLTIP_LEFT_BOTTOM:
	case PPTOOLTIP_RIGHT_BOTTOM:
		top = y;
		bottom = top + cy;
		break;
	}

	BOOL bTestOk = ((top >= 0) && (bottom <= w_cy)) ? TRUE : FALSE;
	if (bTestOk)
	{
		rect->top = top;
		rect->bottom = bottom;
	}

	return bTestOk;
}

/////////////////////////////////////////////////////////////////
// Gets the system tooltip's logfont
/////////////////////////////////////////////////////////////////
LPLOGFONT CPPToolTip::GetSystemToolTipFont() const
{
    static LOGFONT LogFont;

    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    if (!SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0))
        return FALSE;

    memcpy(&LogFont, &(ncm.lfStatusFont), sizeof(LOGFONT));

    return &LogFont; 
}

/////////////////////////////////////////////////////////////////
// Prints the formating string of the tooltip
//  
// Parameters:
//		pDC			 - [in] The device context to print the string
//      str			 - [in] The formating string for drawing
//      rect         - [in] The rectangle to draws the tooltip
//		bEnableAlign - [in] If TRUE align's operators are enables.
//
// Return values:
//      None.
//---------------------------------------------------------------
// Format of string:
//  <b> text </b> - The bold string
//  <i> text </i> - The italic string 
//  <u> text </u> - The underline string
//  <s> text </s> - The strike out string
//
//  <cb=0x123456> text </cb> - The background color (RGB (12,34,56))
//  <ct=0x123456> text </ct> - The text color (RGB (12,34,56))
//
//  <cbi=1> text </cbi> - The index background color(0 - F)
//  <cti=1> text </cti> - The index text color(0 - F)
//
//	<al> or <al_l> - sets align to left edge
//  <al_c> - sets align to the center 
//  <al_r> - sets align to the right edge
//
//	<hr=100%> - the horizontal line with length 100%
//  <hr=32> - the horizontal line with length 32 pixels.
//
//	<a="link"> text </a> - The hyperlink 
//
//  <h=1> text </h> - hot zone (number of zone)
//
//	<br[=1]> - new line (numbers of the lines)
//  <t[=1]> - tabulation (number of tabulations)
//
//  <img[=0x0000] [cx=0] [cy=0]>				 - draws the image by his name
//  <ilst[=0x0000]>								 - draws the image from image list
//  <bmp[=0x0000] mask[=0xFF00FF] [cx=0] [cy=0]> - draws bitmap
//  <icon[=0x0000] [cx=0] [cy=0]>				 - draws the icon
////////////////////////////////////////////////////////////////
CSize CPPToolTip::PrintTitleString(CDC * pDC, CRect rect, CString str, BOOL bCalculate /* = TRUE */)
{
	enum{	CMD_NONE = 0,
			CMD_BOLD,
			CMD_ITALIC,
			CMD_STRIKE,
			CMD_UNDERLINE,
			CMD_COLOR_TEXT,
			CMD_COLOR_TEXT_INDEX,
			CMD_COLOR_BK,
			CMD_COLOR_BK_INDEX,
			CMD_NEW_LINE,
			CMD_TABULATION,
			CMD_HORZ_LINE,
			CMD_HORZ_LINE_PERCENT,
			CMD_DRAW_IMAGE,
			CMD_DRAW_IMAGE_LIST,
			CMD_DRAW_BITMAP,
			CMD_DRAW_BITMAP_MASK,
			CMD_DRAW_ICON
	};

	enum{	ALIGN_LEFT = 0,
			ALIGN_CENTER,
			ALIGN_RIGHT
		};

	//Clears the length of the lines
	if (bCalculate)
	{
		m_nLengthLines.RemoveAll();
		m_nHeightLines.RemoveAll();
	}
	
	int nLine = 0;
	int nCmd = CMD_NONE;
	int nAlign = ALIGN_LEFT;
	BOOL bCloseTag = FALSE;

	CSize sz(0, 0);
	CSize szLine (0, 0);
	CSize szIcon (0, 0); //The size of icon

	if (str.IsEmpty())
		return sz;

	CPoint	pt = rect.TopLeft();
	CPoint  ptCur = pt;
	
	// Copies default logfont's structure
	LOGFONT lf;
    memcpy(&lf, &m_LogFont, sizeof(LOGFONT));

	CFont font;
	font.CreateFontIndirect(&lf);

	CFont * pOldFont = pDC->SelectObject(&font);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int nHeight = tm.tmHeight; //The height of the font
	int nWidth = tm.tmAveCharWidth; //The width of the font

	CString strTag = _T("");  // Tag's name 
	CString strText = _T(""); // The current text to output
	CString sParam = _T(""); // The text parameter

	UINT nParam = 0, nParam1 = 0;
	int nLineHeight = bCalculate ? nHeight : m_nHeightLines.GetAt(0); //The height of the current line
	
	CUIntArray percent;
	percent.Add(0);

	int nTemp = 0; //the temporary variable
	BOOL bFirstOutput = TRUE;
	
	for (int i = 0; i <= str.GetLength(); i++)
	{
		if (i < str.GetLength())
		{
			nCmd = CMD_NONE;
			strText = SearchBeginOfTag(str, i);
			if (strText.IsEmpty())
			{
				//Tag was found
				strTag = GetNameOfTag(str, i);
				bCloseTag = (strTag.GetAt(0) == _T('/')) ? TRUE : FALSE;
				if (bCloseTag)
					strTag = strTag.Right(strTag.GetLength() - 1);
				
				if (!strTag.CompareNoCase(_T("b")))
				{
					nCmd = CMD_BOLD;
				}
				else if (!strTag.CompareNoCase(_T("i")))
				{
					nCmd = CMD_ITALIC;
				}
				else if (!strTag.CompareNoCase(_T("u")))
				{
					nCmd = CMD_UNDERLINE;
				}
				else if (!strTag.CompareNoCase(_T("s")))
				{
					nCmd = CMD_STRIKE;
				}
				else if (!strTag.CompareNoCase(_T("br")))
				{
					nCmd = CMD_NEW_LINE;
					nParam = GetUIntValue(str, i, 1);
				}
				else if (!strTag.CompareNoCase(_T("t")))
				{
					nCmd = CMD_TABULATION;
					nParam = GetUIntValue(str, i, 1);
				}
				else if (strTag.GetAt(0) == _T('\n'))
				{
					nCmd = CMD_NEW_LINE;
					nParam = 1;
				}
				else if (strTag.GetAt(0) == _T('\t'))
				{
					nCmd = CMD_TABULATION;
					nParam = 1;
				}
				else if (!strTag.CompareNoCase(_T("ct")))
				{
					nCmd = CMD_COLOR_TEXT;
					nParam = GetUIntValue(str, i, (UINT)m_crColor[PPTOOLTIP_COLOR_FG]);
				}
				else if (!strTag.CompareNoCase(_T("cti")))
				{
					nCmd = CMD_COLOR_TEXT;
					nParam = GetUIntValue(str, i, PPTOOLTIP_COLOR_FG);
				}
				else if (!strTag.CompareNoCase(_T("cb")))
				{
					nCmd = CMD_COLOR_BK;
					nParam = GetUIntValue(str, i, (UINT)m_crColor[PPTOOLTIP_COLOR_BK_BEGIN]);
				}
				else if (!strTag.CompareNoCase(_T("cbi")))
				{
					nCmd = CMD_COLOR_BK_INDEX;
					nParam = GetUIntValue(str, i, PPTOOLTIP_COLOR_BK_BEGIN);
				}
				else if (!strTag.CompareNoCase(_T("al")) || !strTag.CompareNoCase(_T("al_l")))
				{
					nAlign = ALIGN_LEFT;
				}
				else if (!strTag.CompareNoCase(_T("al_c")))
				{
					if (!bCalculate)
						nAlign = ALIGN_CENTER;
				}
				else if (!strTag.CompareNoCase(_T("al_r")))
				{
					if (!bCalculate)
						nAlign = ALIGN_RIGHT;
				}
				else if (!strTag.CompareNoCase(_T("hr")))
				{
					sParam = GetStringValue(str, i);
					if (!sParam.IsEmpty())
						nParam = _tcstoul(sParam, 0, 0);
					else
						nParam = 100;
					nCmd = (sParam.Right(1) == _T("%"))? CMD_HORZ_LINE_PERCENT : CMD_HORZ_LINE;
				}
				else if (!strTag.CompareNoCase(_T("img")))
				{
					nCmd = CMD_DRAW_IMAGE;
					sParam = GetStringValue(str, i);
					szIcon = CSize(0, 0);
					//Gets two param
					for (nTemp = 0; nTemp < 2; nTemp++)
					{
						strTag = GetPropertiesOfTag(str, i);
						if (!strTag.CompareNoCase(_T("cx")))
							szIcon.cx = GetUIntValue(str, i, 0);
						else if (!strTag.CompareNoCase(_T("cy")))
							szIcon.cy = GetUIntValue(str, i, 0);
					}
				}
				else if (!strTag.CompareNoCase(_T("ilst")))
				{
					nCmd = CMD_DRAW_IMAGE_LIST;
					nParam = GetUIntValue(str, i, 0);
				}
				else if (!strTag.CompareNoCase(_T("icon")))
				{
					nCmd = CMD_DRAW_ICON;
					nParam = GetUIntValue(str, i, 0);
					szIcon = CSize(0, 0);
					//Gets two param
					for (nTemp = 0; nTemp < 2; nTemp++)
					{
						strTag = GetPropertiesOfTag(str, i);
						if (!strTag.CompareNoCase(_T("cx")))
							szIcon.cx = GetUIntValue(str, i, 0);
						else if (!strTag.CompareNoCase(_T("cy")))
							szIcon.cy = GetUIntValue(str, i, 0);
					}
				}
				else if (!strTag.CompareNoCase(_T("bmp")))
				{
					nCmd = CMD_DRAW_BITMAP;
					nParam = GetUIntValue(str, i, 0);
					sParam.Empty();
					//Gets three param
					for (nTemp = 0; nTemp < 3; nTemp++)
					{
						strTag = GetPropertiesOfTag(str, i);
						if (!strTag.CompareNoCase(_T("mask")))
						{
							sParam = strTag;
							nParam1 = GetUIntValue(str, i, 0xFF00FF);
						}
						else if (!strTag.CompareNoCase(_T("cx")))
							szIcon.cx = GetUIntValue(str, i, 0);
						else if (!strTag.CompareNoCase(_T("cy")))
							szIcon.cy = GetUIntValue(str, i, 0);
					}
				}
				else nCmd = CMD_NONE;
				SearchEndOfTag(str, i);
			}
			else
			{
				//If text to output is exist
				if (bFirstOutput)
				{
					switch (nAlign)
					{
					case ALIGN_CENTER:
						ptCur.x = pt.x + (rect.Width() - m_nLengthLines.GetAt(nLine)) / 2;
						break;
					case ALIGN_RIGHT:
						ptCur.x = pt.x + rect.Width() - m_nLengthLines.GetAt(nLine);
						break;
					}
				}
				szLine = pDC->GetTextExtent(strText);
				if (bCalculate)
					nLineHeight = max(nLineHeight, szLine.cy);
				else
					pDC->TextOut(ptCur.x, ptCur.y + m_nHeightLines.GetAt(nLine) - nHeight, strText);
				ptCur.x += szLine.cx;
				strText = _T("");
				bFirstOutput = FALSE;
				i--;
			}
		}
		else
		{
			nCmd = CMD_NEW_LINE;
			nParam = 1; 
		}
				
		//Prepares to first draw in line
		switch (nCmd)
		{
		case CMD_DRAW_IMAGE:
		case CMD_DRAW_IMAGE_LIST:
		case CMD_DRAW_BITMAP:
		case CMD_DRAW_ICON:
			if (bFirstOutput)
			{
				switch (nAlign)
				{
				case ALIGN_CENTER:
					ptCur.x = pt.x + (rect.Width() - m_nLengthLines.GetAt(nLine)) / 2;
					break;
				case ALIGN_RIGHT:
					ptCur.x = pt.x + rect.Width() - m_nLengthLines.GetAt(nLine);
					break;
				}
				bFirstOutput = FALSE;
			}
			break;
		}
		
		//Executes command
		switch (nCmd)
		{
		case CMD_BOLD:
			//Bold text
			pDC->SelectObject(pOldFont);
			font.DeleteObject();
			lf.lfWeight = m_LogFont.lfWeight;
			if (!bCloseTag)
			{
				lf.lfWeight *= 2;
				if (lf.lfWeight > FW_BLACK)
					lf.lfWeight = FW_BLACK;
			}
			font.CreateFontIndirect(&lf);
			pDC->SelectObject(&font);
			break;
		case CMD_ITALIC:
			//Italic text
			pDC->SelectObject(pOldFont);
			font.DeleteObject();
			lf.lfItalic = bCloseTag ? FALSE : TRUE;
			font.CreateFontIndirect(&lf);
			pDC->SelectObject(&font);
			break;
		case CMD_STRIKE:
			//Strikeout text
			pDC->SelectObject(pOldFont);
			font.DeleteObject();
			lf.lfStrikeOut = bCloseTag ? FALSE : TRUE;
			font.CreateFontIndirect(&lf);
			pDC->SelectObject(&font);
			break;
		case CMD_UNDERLINE:
			//Underline text
			pDC->SelectObject(pOldFont);
			font.DeleteObject();
			lf.lfUnderline = bCloseTag ? FALSE : TRUE;
			font.CreateFontIndirect(&lf);
			pDC->SelectObject(&font);
			break;
		case CMD_COLOR_TEXT:
			//Color of the text
			pDC->SetTextColor((COLORREF)nParam);
			break;
		case CMD_COLOR_TEXT_INDEX:
			//Indexed color of the text
			if (nParam < PPTOOLTIP_MAX_COLORS)
				pDC->SetTextColor(m_crColor[nParam]);
			break;
		case CMD_COLOR_BK:
			//Color of the background
			pDC->SetBkColor((COLORREF)nParam);
			pDC->SetBkMode(bCloseTag ? TRANSPARENT : OPAQUE);
			break;
		case CMD_COLOR_BK_INDEX:
			//Indexed color of the background
			if (nParam < PPTOOLTIP_MAX_COLORS)
			{
				pDC->SetBkColor(m_crColor[nParam]);
				pDC->SetBkMode(bCloseTag ? TRANSPARENT : OPAQUE);
			}
			break;
		case CMD_HORZ_LINE_PERCENT:
			//Horizontal line with percent length
			if (bCalculate)
			{
				percent.SetAt(nLine, percent.GetAt(nLine) + nParam);
				nParam = 0;
			}
			else nParam = ::MulDiv(rect.Width(), nParam, 100);
		case CMD_HORZ_LINE:
			//Horizontal line with absolute length
			//If text to output is exist
			if (!bCalculate)
				DrawHorzLine(pDC, ptCur.x, ptCur.x + nParam, ptCur.y + m_nHeightLines.GetAt(nLine) / 2);
			ptCur.x += nParam;
			break;
		case CMD_DRAW_IMAGE:
			if (!sParam.IsEmpty())
			{
				if (bCalculate)
				{
					szLine = DrawResource(sParam, pDC, ptCur, 0, szIcon, bCalculate);
					nLineHeight = max (nLineHeight, szLine.cy);
				}
				else
					szLine = DrawResource(sParam, pDC, ptCur, m_nHeightLines.GetAt(nLine), szIcon, bCalculate);

				ptCur.x += szLine.cx;
			}
			break;
		case CMD_DRAW_IMAGE_LIST:
			if (m_imgTooltip.m_hImageList != NULL)
			{
				if (bCalculate)
				{
					szLine = DrawIconFromImageList(pDC, ptCur, m_szImage, m_imgTooltip, nParam, bCalculate);
					nLineHeight = max (nLineHeight, szLine.cy);
				}
				else
				{
					szLine = DrawIconFromImageList(pDC, ptCur, m_szImage, m_imgTooltip, nParam, bCalculate);
				}
				// If in one line a few bitmap with different height, then store max height
				ptCur.x += szLine.cx; //m_szImage.cx;
			}
			break;
		case CMD_DRAW_BITMAP:
			if (nParam != 0)
			{
				if (bCalculate)
				{
					szLine = DrawBitmap(pDC, ptCur, 0, nParam, !sParam.IsEmpty(), nParam1, szIcon, bCalculate);
					nLineHeight = max (nLineHeight, szLine.cy);
				}
				else
				{
					szLine = DrawBitmap(pDC, ptCur, m_nHeightLines.GetAt(nLine), nParam, !sParam.IsEmpty(), nParam1, szIcon, bCalculate);
				}
				// If in one line a few bitmap with different height, then store max height
				ptCur.x += szLine.cx;
			}
		case CMD_DRAW_ICON:
			if (nParam != 0)
			{
				if (bCalculate)
				{
					szLine = DrawIcon(pDC, ptCur, 0, nParam, szIcon, bCalculate);
					nLineHeight = max (nLineHeight, szLine.cy);
				}
				else
				{
					szLine = DrawIcon(pDC, ptCur, m_nHeightLines.GetAt(nLine), nParam, szIcon, bCalculate);
				}
				// If in one line a few bitmap with different height, then store max height
				ptCur.x += szLine.cx;
			}
			break;
		case CMD_NEW_LINE:
			//New line
			if (!nParam)
				nParam = 1;
			if (bCalculate)
			{
				sz.cx = max(sz.cx, ptCur.x - pt.x);
				m_nLengthLines.Add(ptCur.x - pt.x); //Adds the real length of the lines
				m_nHeightLines.Add(nLineHeight); //Adds the real height of the lines
			}
			ptCur.y += m_nHeightLines.GetAt(nLine) * nParam;
			nLine ++;
			percent.Add(0);
			bFirstOutput = TRUE;
			ptCur.x = pt.x;
			nLineHeight = nHeight;
	//		szLine.cy = nHeight;
			break;
		case CMD_TABULATION:
			//Tabulation
			if (!nParam)
				nParam = 1;
			nParam1 = (ptCur.x - pt.x) % (nWidth * 4);
			if (nParam1)
			{
				//aligns with tab
				ptCur.x += (nWidth * 4) - nParam1;
				nParam --;
			}
			ptCur.x += (nParam * nWidth * 4);
			break;
		}
	}
	
	//Gets real height of the tooltip
	sz.cy = ptCur.y - pt.y;

	pDC->SelectObject(pOldFont);
	font.DeleteObject();

	//Adds the percent's length to the line's length
	for (i = 0; i < percent.GetSize(); i++)
	{
		if (percent.GetAt(i))
			m_nLengthLines.SetAt(i, m_nLengthLines.GetAt(i) + ::MulDiv(percent.GetAt(i), sz.cx, 100));
	}

	return sz;
}

CString CPPToolTip::SearchBeginOfTag(CString & str, int & nIndex)
{
	CString sText = _T("");
	BOOL bTagFound = FALSE;
	
	for (nIndex; nIndex < str.GetLength(); nIndex ++)
	{
		switch (str.GetAt(nIndex))
		{
		case _T('\r'):
			break;
		case _T('<'):
			nIndex ++;
			if ((nIndex < str.GetLength()) && (str.GetAt(nIndex) != _T('<')))
			{
				if (!sText.IsEmpty())
					nIndex --;
				return sText;
			}
			sText += _T('<');
			break;
		case _T('\t'):
		case _T('\n'):
			if (!sText.IsEmpty())
				nIndex--;
			return sText;
		default:
			sText += str.GetAt(nIndex);
			break;
		}
	}
	return sText;
}

void CPPToolTip::SearchEndOfTag(CString & str, int & nIndex)
{
	for (nIndex; nIndex < str.GetLength(); nIndex ++)
	{
		switch (str.GetAt(nIndex))
		{
		case _T('>'):
		case _T('\n'):
		case _T('\t'):
			return;
		}
	}
}

CString CPPToolTip::GetNameOfTag(CString & str, int & nIndex)
{
	CString sText = _T("");
	
	for (nIndex; nIndex < str.GetLength(); nIndex ++)
	{
		switch (str.GetAt(nIndex))
		{
		case _T('\r'): //Pass character
			break;
		case _T('\t'): //It is a tab tag
		case _T('\n'): //It is a new line tag
			if (sText.IsEmpty())
			{
				sText += str.GetAt(nIndex);
//				nIndex ++;
				return sText;
			}
			break;
		case _T(' '):
			if (!sText.IsEmpty())
			{
				nIndex ++;
				return sText;
			}
			break;
		case _T('>'):
		case _T('='):
			return sText;
		default:
			sText += str.GetAt(nIndex);
			break;
		}
	}
	return sText;
}

CString CPPToolTip::GetPropertiesOfTag(CString & str, int & nIndex)
{
	CString sText = _T("");
	
	for (nIndex; nIndex < str.GetLength(); nIndex ++)
	{
		switch (str.GetAt(nIndex))
		{
		case _T('\r'): //Pass characters
		case _T('\t'):
		case _T('\n'):
			break;
		case _T(' '):
			if (!sText.IsEmpty())
			{
				nIndex ++;
				return sText;
			}
			break;
		case _T('>'):
		case _T('='):
			return sText;
		default:
			sText += str.GetAt(nIndex);
			break;
		}
	}
	return sText;
}

CString CPPToolTip::GetStringValue(CString & str, int & nIndex)
{
	CString sText = _T("");
	BOOL bValueFound = FALSE;
	
	for (nIndex; nIndex < str.GetLength(); nIndex ++)
	{
		switch (str.GetAt(nIndex))
		{
		case _T('\r'): //Pass character
		case _T('\t'): //It is a tab tag
		case _T('\n'): //It is a new line tag
			break;
		case _T(' '):
			if (!sText.IsEmpty())
			{
				nIndex ++;
				return sText;
			}
			break;
		case _T('>'):
			return sText;
		case _T('='):
			bValueFound = TRUE;
			break;
		default:
			if (!bValueFound)
				return sText;
			sText += str.GetAt(nIndex);
			break;
		}
	}
	return sText;

}

UINT CPPToolTip::GetUIntValue(CString & str, int & nIndex, UINT nDefValue)
{
	CString sText = GetStringValue(str, nIndex);
	if (!sText.IsEmpty())
		nDefValue = _tcstoul(sText, 0, 0);

	return nDefValue;
}

CSize CPPToolTip::DrawResource(CString sName, CDC * pDC, CPoint pt, int nMaxHeight, CSize szResource, BOOL bCalculate)
{
	CSize sz(0, 0);
	
	int nIndex = FindIdOfResource(sName);
	if (nIndex < 0)
		return sz;

	PPTOOLTIP_NAME_RES nr = m_arrNameRes.GetAt(nIndex);

	if (nr.nID == 0)
		return sz;

	switch (nr.nTypeRes)
	{
	case TYPE_RES_ICON:
		sz = DrawIcon(pDC, pt, nMaxHeight, nr.nID, szResource, bCalculate);
		break;
	case TYPE_RES_BITMAP:
		sz = DrawBitmap(pDC, pt, nMaxHeight, nr.nID, FALSE, nr.crMask, szResource, bCalculate);
		break;
	case TYPE_RES_MASK_BITMAP:
		sz = DrawBitmap(pDC, pt, nMaxHeight, nr.nID, TRUE, nr.crMask, szResource, bCalculate);
		break;
	}

	return sz;
}

CSize CPPToolTip::DrawBitmap(CDC * pDC, CPoint pt, int nMaxHeight, UINT nID, BOOL bUseMask, COLORREF crMask, CSize szBitmap, BOOL bCalculate)
{
	CSize sz(0, 0);
	HBITMAP	hBitmap = GetBitmapFromResources(nID);
	
	int		nRetValue;
	BITMAP	csBitmapSize;
	
	if (hBitmap == NULL)
		return sz;
	
	// Get bitmap size
	nRetValue = ::GetObject(hBitmap, sizeof(csBitmapSize), &csBitmapSize);
	if (nRetValue == 0)
		return sz;
	
	sz.cx = (DWORD)csBitmapSize.bmWidth;
	sz.cy = (DWORD)csBitmapSize.bmHeight;

	if (!szBitmap.cy)
		szBitmap.cy = sz.cy;

	if (!szBitmap.cx)
		szBitmap.cx = sz.cx;

	if (bCalculate)
		return szBitmap;
	
	HDC hSrcDC = ::CreateCompatibleDC(pDC->m_hDC);
	HDC hResDC = ::CreateCompatibleDC(pDC->m_hDC);
	
	HBITMAP hSrcBitmap = ::CreateCompatibleBitmap(pDC->m_hDC, szBitmap.cx, szBitmap.cy);
	HBITMAP hOldSrcBitmap = (HBITMAP)::SelectObject(hSrcDC, hSrcBitmap);
	HBITMAP hOldResBitmap = (HBITMAP)::SelectObject(hResDC, hBitmap);

	//Scales a bitmap if need
	if ((sz.cx != szBitmap.cx) || (sz.cy != szBitmap.cy))
		::StretchBlt(hSrcDC, 0, 0, szBitmap.cx, szBitmap.cy, hResDC, 0, 0, sz.cx, sz.cy, SRCCOPY);
	else
		::BitBlt(hSrcDC, 0, 0, szBitmap.cx, szBitmap.cy, hResDC, 0, 0, SRCCOPY);

	::SelectObject(hResDC, hOldResBitmap);
	::DeleteDC(hResDC);
	::DeleteObject(hOldResBitmap);
	::DeleteObject(hBitmap);

	pt.y += (nMaxHeight - szBitmap.cy);

	if (bUseMask)
	{
		//Draws a bitmap with mask
		::SelectObject(hSrcDC, hOldSrcBitmap);
		CImageList img;
		img.Create(szBitmap.cx, szBitmap.cy, ILC_COLOR32 | ILC_MASK, 1, 1);
		img.Add(CBitmap::FromHandle(hSrcBitmap), crMask);
		DrawIconFromImageList(pDC, pt, szBitmap, img, 0, FALSE);
	}
	else
	{
		//Draws a bitmap without mask
		pDC->BitBlt(pt.x, pt.y, szBitmap.cx, szBitmap.cy, CDC::FromHandle(hSrcDC), 0, 0, SRCCOPY);
		::SelectObject(hSrcDC, hOldSrcBitmap);
	}

	::DeleteDC(hSrcDC);
	::DeleteObject(hOldSrcBitmap);
	::DeleteObject(hSrcBitmap);

	return szBitmap;
}

CSize CPPToolTip::DrawIcon(CDC * pDC, CPoint pt, int nMaxHeight, UINT nID, CSize szIcon, BOOL bCalculate)
{
	CSize sz (0, 0);
	HICON hIcon = GetIconFromResources(nID, szIcon);
	if (hIcon != NULL)
	{
		sz = GetSizeIcon(hIcon);
		if (!bCalculate)
		{
			pt.y += (nMaxHeight - sz.cy);
			pDC->DrawState(pt, sz, hIcon, DSS_NORMAL, (CBrush*)NULL);
		}
	}
	
	if (hIcon)
		::DestroyIcon(hIcon);
	
	return sz;
}

CSize CPPToolTip::DrawIconFromImageList(CDC * pDC, CPoint pt, CSize sz, CImageList & img, int nIndex /* = 0 */, BOOL bCalculate /* = TRUE */)
{
	if (img.GetSafeHandle() == NULL)
		return CSize (0, 0);

	int nCount = img.GetImageCount();
	if (nIndex >= nCount)
		return CSize (0, 0);

	if (bCalculate)
		return sz;
	
	HICON hIcon = img.ExtractIcon(nIndex);
	pDC->DrawState(pt, sz, hIcon, DSS_NORMAL, (CBrush*)NULL);

	if (hIcon)
		DestroyIcon(hIcon);

	return sz;
}

void CPPToolTip::DrawHorzLine(CDC * pDC, int xStart, int xEnd, int y) const
{
	CPen pen(PS_SOLID, 1, pDC->GetTextColor());
	CPen * penOld = pDC->SelectObject(&pen);
	pDC->MoveTo(xStart, y);
	pDC->LineTo(xEnd, y);
	pDC->SelectObject(penOld);
	pen.DeleteObject();
}

void CPPToolTip::FillGradient (	CDC * pDC, CRect rect, 
								COLORREF colorStart, COLORREF colorFinish, 
								BOOL bHorz/* = TRUE*/)
{
    // this will make 2^6 = 64 fountain steps
    int nShift = 6;
    int nSteps = 1 << nShift;

    for (int i = 0; i < nSteps; i++)
    {
        // do a little alpha blending
        BYTE bR = (BYTE) ((GetRValue(colorStart) * (nSteps - i) +
                   GetRValue(colorFinish) * i) >> nShift);
        BYTE bG = (BYTE) ((GetGValue(colorStart) * (nSteps - i) +
                   GetGValue(colorFinish) * i) >> nShift);
        BYTE bB = (BYTE) ((GetBValue(colorStart) * (nSteps - i) +
                   GetBValue(colorFinish) * i) >> nShift);

		CBrush br (RGB(bR, bG, bB));

        // then paint with the resulting color
        CRect r2 = rect;
        if (!bHorz)
        {
            r2.top = rect.top + 
                ((i * rect.Height()) >> nShift);
            r2.bottom = rect.top + 
                (((i + 1) * rect.Height()) >> nShift);
            if (r2.Height() > 0)
                pDC->FillRect(r2, &br);
        }
        else
        {
            r2.left = rect.left + 
                ((i * rect.Width()) >> nShift);
            r2.right = rect.left + 
                (((i + 1) * rect.Width()) >> nShift);
            if (r2.Width() > 0)
                pDC->FillRect(r2, &br);
        }
    }
}

#ifdef PPTOOLTIP_USE_SHADE
void CPPToolTip::SetShade(CRect rect, UINT shadeID /* = 0 */, BYTE granularity /* = 8 */, 
						  BYTE coloring /* = 0 */, COLORREF color /* = 0 */)
{
	long	sXSize,sYSize,bytes,j,i,k,h;
	BYTE	*iDst ,*posDst;
	
	sYSize= rect.Height(); //rect.bottom-rect.top;
	sXSize= rect.Width(); //rect.right-rect.left ;

	m_dh.Create(max(1,sXSize /*-2*m_FocusRectMargin-1*/ ),1,8);	//create the horizontal focus bitmap
	m_dv.Create(1,max(1,sYSize /*-2*m_FocusRectMargin*/),8);	//create the vertical focus bitmap

	m_dNormal.Create(sXSize,sYSize,8);					//create the default bitmap

	COLORREF hicr = m_pToolInfo.crBegin; //GetSysColor(COLOR_BTNHIGHLIGHT);		//get the button base colors
	COLORREF midcr = m_pToolInfo.crMid;  //GetSysColor(COLOR_BTNFACE);
	COLORREF locr = m_pToolInfo.crEnd;   //GetSysColor(COLOR_BTNSHADOW);
	long r,g,b;											//build the shaded palette
	for(i=0;i<129;i++){
		r=((128-i)*GetRValue(locr)+i*GetRValue(midcr))/128;
		g=((128-i)*GetGValue(locr)+i*GetGValue(midcr))/128;
		b=((128-i)*GetBValue(locr)+i*GetBValue(midcr))/128;
		m_dNormal.SetPaletteIndex((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
		m_dh.SetPaletteIndex((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
		m_dv.SetPaletteIndex((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
	}
	for(i=1;i<129;i++){
		r=((128-i)*GetRValue(midcr)+i*GetRValue(hicr))/128;
		g=((128-i)*GetGValue(midcr)+i*GetGValue(hicr))/128;
		b=((128-i)*GetBValue(midcr)+i*GetBValue(hicr))/128;
		m_dNormal.SetPaletteIndex((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
		m_dh.SetPaletteIndex((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
		m_dv.SetPaletteIndex((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
	}

	m_dNormal.BlendPalette(color,coloring);	//color the palette

	iDst=m_dh.GetBits();		//build the horiz. dotted focus bitmap
	j=(long)m_dh.GetWidth();
	for(i=0;i<j;i++){
//		iDst[i]=64+127*(i%2);	//soft
		iDst[i]=255*(i%2);		//hard
	}

	iDst=m_dv.GetBits();		//build the vert. dotted focus bitmap
	j=(long)m_dv.GetHeight();
	for(i=0;i<j;i++){
//		*iDst=64+127*(i%2);		//soft
		*iDst=255*(i%2);		//hard
		iDst+=4;
	}

	bytes = m_dNormal.GetLineWidth();
	iDst = m_dNormal.GetBits();
	posDst =iDst;
	long a,x,y,d,xs,idxmax,idxmin;

	int grainx2=RAND_MAX/(max(1,2*granularity));
	idxmax=255-granularity;
	idxmin=granularity;

	switch (shadeID)
	{
//----------------------------------------------------
	case PPTOOLTIP_EFFECT_METAL:
		m_dNormal.Clear();
		// create the strokes
		k=40;	//stroke granularity
		for(a=0;a<200;a++){
			x=rand()/(RAND_MAX/sXSize); //stroke postion
			y=rand()/(RAND_MAX/sYSize);	//stroke position
			xs=rand()/(RAND_MAX/min(sXSize,sYSize))/2; //stroke lenght
			d=rand()/(RAND_MAX/k);	//stroke color
			for(i=0;i<xs;i++){
				if (((x-i)>0)&&((y+i)<sYSize))
					m_dNormal.SetPixelIndex(x-i,y+i,(BYTE)d);
				if (((x+i)<sXSize)&&((y-i)>0))
					m_dNormal.SetPixelIndex(sXSize-x+i,y-i,(BYTE)d);
			}
		}
		//blend strokes with SHS_DIAGONAL
		posDst =iDst;
		a=(idxmax-idxmin-k)/2;
		for(i = 0; i < sYSize; i++) {
			for(j = 0; j < sXSize; j++) {
				d=posDst[j]+((a*i)/sYSize+(a*(sXSize-j))/sXSize);
				posDst[j]=(BYTE)d;
				posDst[j]+=rand()/grainx2;
			}
			posDst+=bytes;
		}

		break;
//----------------------------------------------------
	case PPTOOLTIP_EFFECT_HARDBUMP:	// 
		//set horizontal bump
		for(i = 0; i < sYSize; i++) {
			k=(255*i/sYSize)-127;
			k=(k*(k*k)/128)/128;
			k=(k*(128-granularity*2))/128+128;
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		//set vertical bump
		d=min(16,sXSize/6);	//max edge=16
		a=sYSize*sYSize/4;
		posDst =iDst;
		for(i = 0; i < sYSize; i++) {
			y=i-sYSize/2;
			for(j = 0; j < sXSize; j++) {
				x=j-sXSize/2;
				xs=sXSize/2-d+(y*y*d)/a;
				if (x>xs) posDst[j]=idxmin+(BYTE)(((sXSize-j)*128)/d);
				if ((x+xs)<0) posDst[j]=idxmax-(BYTE)((j*128)/d);
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case PPTOOLTIP_EFFECT_SOFTBUMP: //
		for(i = 0; i < sYSize; i++) {
			h=(255*i/sYSize)-127;
			for(j = 0; j < sXSize; j++) {
				k=(255*(sXSize-j)/sXSize)-127;
				k=(h*(h*h)/128)/128+(k*(k*k)/128)/128;
				k=k*(128-granularity)/128+128;
				if (k<idxmin) k=idxmin;
				if (k>idxmax) k=idxmax;
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case PPTOOLTIP_EFFECT_VBUMP: // 
		for(j = 0; j < sXSize; j++) {
			k=(255*(sXSize-j)/sXSize)-127;
			k=(k*(k*k)/128)/128;
			k=(k*(128-granularity))/128+128;
			for(i = 0; i < sYSize; i++) {
				posDst[j+i*bytes]=(BYTE)k;
				posDst[j+i*bytes]+=rand()/grainx2-granularity;
			}
		}
		break;
//----------------------------------------------------
	case PPTOOLTIP_EFFECT_HBUMP: //
		for(i = 0; i < sYSize; i++) {
			k=(255*i/sYSize)-127;
			k=(k*(k*k)/128)/128;
			k=(k*(128-granularity))/128+128;
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case PPTOOLTIP_EFFECT_DIAGSHADE:	//
		a=(idxmax-idxmin)/2;
		for(i = 0; i < sYSize; i++) {
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)(idxmin+a*i/sYSize+a*(sXSize-j)/sXSize);
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case PPTOOLTIP_EFFECT_HSHADE:	//
		a=idxmax-idxmin;
		for(i = 0; i < sYSize; i++) {
			k=a*i/sYSize+idxmin;
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case PPTOOLTIP_EFFECT_VSHADE:	//:
		a=idxmax-idxmin;
		for(j = 0; j < sXSize; j++) {
			k=a*(sXSize-j)/sXSize+idxmin;
			for(i = 0; i < sYSize; i++) {
				posDst[j+i*bytes]=(BYTE)k;
				posDst[j+i*bytes]+=rand()/grainx2-granularity;
			}
		}
		break;
//----------------------------------------------------
	case PPTOOLTIP_EFFECT_NOISE:
		for(i = 0; i < sYSize; i++) {
			for(j = 0; j < sXSize; j++) {
				posDst[j]=128+rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
	}
//----------------------------------------------------
}
#endif

HICON CPPToolTip::GetIconFromResources(UINT nID, CSize szIcon /* = CSize(0, 0) */) const
{
	// Find correct resource handle
	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nID), RT_GROUP_ICON);
	// Set icon when the mouse is IN the button
	HICON hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(nID), IMAGE_ICON, szIcon.cx, szIcon.cy, 0);

	return hIcon;
}

HBITMAP CPPToolTip::GetBitmapFromResources(UINT nID) const
{
	// Find correct resource handle
	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nID), RT_BITMAP);
	// Load bitmap
	HBITMAP hBitmap = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nID), IMAGE_BITMAP, 0, 0, 0);

	return hBitmap;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetStyles  (public member function)
//    Sets the new styles of the control
//
//  Parameters :
//		nStyle		[in] - new style
//
//  Returns :
//		Old styles
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::SetStyles(DWORD nStyles, int nIndexTool /* = PPTOOLTIP_TOOL_NOEXIST */)
{
	TRACE(_T("CPPToolTip::SetStyles()\n"));

	ModifyStyles(nStyles, -1, nIndexTool);
}  // End of SetStyles

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::ModifyStyles  (public member function)
//    Modify the styles of the control
//
//  Parameters :
//		nAddStyle	 [in] - The styles to add
//		nRemoveStyle [in] - The styles to remove
//
//  Returns :
//		Old styles
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::ModifyStyles(DWORD nAddStyles, DWORD nRemoveStyles, int nIndexTool /* = PPTOOLTIP_TOOL_NOEXIST */)
{
	if (!IsExistTool(nIndexTool))
	{
		m_nStyles &= ~nRemoveStyles;
		m_nStyles |= nAddStyles;
	}
	else
	{
		PPTOOLTIP_INFO ti;
		GetTool(nIndexTool, ti);
		if (!(ti.nMask & PPTOOLTIP_MASK_STYLES))
			ti.nStyles = m_nStyles;
		ti.nStyles &= ~nRemoveStyles;
		ti.nStyles |= nAddStyles;
		ti.nMask |= PPTOOLTIP_MASK_STYLES;
		SetAtTool(nIndexTool, ti);
	}
}  // End of ModifyStyles

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::GetStyles (public member function)
//    Gets the current styles of the control
//
//  Parameters :
//
//  Returns :
//		Current styles
//
/////////////////////////////////////////////////////////////////////////////
DWORD CPPToolTip::GetStyles(int nIndexTool /* = PPTOOLTIP_TOOL_NOEXIST */)
{
	TRACE(_T("CPPToolTip::GetStyles()\n"));

	if (IsExistTool(nIndexTool))
	{
		PPTOOLTIP_INFO ti;
		GetTool(nIndexTool, ti);
		if (ti.nMask & PPTOOLTIP_MASK_STYLES)
			return ti.nStyles;
	}
	return m_nStyles;
}  // End of GetStyles

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetDefaultStyles  (public member function)
//    Sets the new styles of the control
//
//  Parameters :
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::SetDefaultStyles(int nIndexTool /* = PPTOOLTIP_TOOL_NOEXIST */)
{
	TRACE(_T("CPPToolTip::SetDefaultStyles()\n"));

	SetStyles(PPTOOLTIP_BALLOON | PPTOOLTIP_ICON_VCENTER_ALIGN, nIndexTool);
}  // End of SetDefaultStyles


/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetColor (public member function)
//    Set the color
//
//  Parameters :
//		nIndex  [in] - index of the color
//		crColor [in] - new color
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::SetColor(int nIndex, COLORREF crColor)
{
	TRACE (_T("CPPToolTip::SetColor(nIndex = %d)\n"), nIndex);
	
	if (nIndex >= PPTOOLTIP_MAX_COLORS)
		return;

	m_crColor [nIndex] = crColor;
}  // End of SetColor

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::GetColor (public member function)
//    Set the color
//
//  Parameters :
//		nIndex  [in] - index of the color
//
//  Returns :
//		Current color
//
/////////////////////////////////////////////////////////////////////////////
COLORREF CPPToolTip::GetColor(int nIndex)
{
	TRACE (_T("CPPToolTip::GetColor(nIndex = %d)\n"), nIndex);

	if (nIndex >= PPTOOLTIP_MAX_COLORS)
		nIndex = PPTOOLTIP_COLOR_FG;

	return m_crColor [nIndex];
}  // End of GetColor

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetDefaultColors (public member function)
//    Set the color as default
//
//  Parameters :
//		None
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::SetDefaultColors()
{
	TRACE (_T("CPPToolTip::SetDefaultColors\n"));
	
	SetColor(PPTOOLTIP_COLOR_0, RGB (0, 0, 0));
	SetColor(PPTOOLTIP_COLOR_1, RGB (0, 0, 128));
	SetColor(PPTOOLTIP_COLOR_2, RGB (0, 128, 0));
	SetColor(PPTOOLTIP_COLOR_3, RGB (0, 128, 128));
	SetColor(PPTOOLTIP_COLOR_4, RGB (128, 0, 0));
	SetColor(PPTOOLTIP_COLOR_5, RGB (128, 0, 128));
	SetColor(PPTOOLTIP_COLOR_6, RGB (128, 128, 0));
	SetColor(PPTOOLTIP_COLOR_7, RGB (128, 128, 128));
	SetColor(PPTOOLTIP_COLOR_8, RGB (0, 0, 255));
	SetColor(PPTOOLTIP_COLOR_9, RGB (0, 255, 0));
	SetColor(PPTOOLTIP_COLOR_10, RGB (0, 255, 255));
	SetColor(PPTOOLTIP_COLOR_11, RGB (255, 0, 0));
	SetColor(PPTOOLTIP_COLOR_12, RGB (255, 0, 255));
	SetColor(PPTOOLTIP_COLOR_13, RGB (255, 255, 0));
	SetColor(PPTOOLTIP_COLOR_14, RGB (192, 192, 192));
	SetColor(PPTOOLTIP_COLOR_15, RGB (255, 255, 255));
	SetColor(PPTOOLTIP_COLOR_FG, ::GetSysColor(COLOR_INFOTEXT));
	SetColor(PPTOOLTIP_COLOR_BK_BEGIN, ::GetSysColor(COLOR_INFOBK));
	SetColor(PPTOOLTIP_COLOR_BK_MID, ::GetSysColor(COLOR_INFOBK));
	SetColor(PPTOOLTIP_COLOR_BK_END, ::GetSysColor(COLOR_INFOBK));
	SetColor(PPTOOLTIP_COLOR_LINK, RGB(0, 0, 238));
	SetColor(PPTOOLTIP_COLOR_VISITED, RGB(85, 26, 139));
	SetColor(PPTOOLTIP_COLOR_HOVER, RGB(255, 0, 0));
	SetColor(PPTOOLTIP_COLOR_SHADOW, ::GetSysColor(COLOR_3DSHADOW));
	SetColor(PPTOOLTIP_COLOR_BORDER, ::GetSysColor(COLOR_INFOTEXT));
}  // End of SetDefaultColors

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetGradientColors (public member function)
//    Set the gradient colors
//
//  Parameters :
//		crStart [in] - start color
//		crEnd [in] - end color
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::SetGradientColors(COLORREF crBegin, COLORREF crMid, COLORREF crEnd, int nIndexTool /* = PPTOOLTIP_TOOL_NOEXIST */)
{
	TRACE (_T("CPPToolTip::SetGradientColors\n"));
	if (!IsExistTool(nIndexTool))
	{
		SetColor(PPTOOLTIP_COLOR_BK_BEGIN, crBegin);
		SetColor(PPTOOLTIP_COLOR_BK_MID, crMid);
		SetColor(PPTOOLTIP_COLOR_BK_END, crEnd);
	}
	else
	{
		PPTOOLTIP_INFO ti;
		GetTool(nIndexTool, ti);
		ti.crBegin = crBegin;
		ti.crMid = crMid;
		ti.crEnd = crEnd;
		ti.nMask |= PPTOOLTIP_MASK_COLORS;
		SetAtTool(nIndexTool, ti);
	}
} // End of SetGradientColors

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::GetGradientColors (public member function)
//    Set the gradient colors
//
//  Parameters :
//		None
//
//  Returns :
//		crStart [out] - start color
//		crEnd [out] - end color
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::GetGradientColors(COLORREF & crBegin, COLORREF & crMid, COLORREF & crEnd, int nIndexTool /* = -1 */)
{
	TRACE (_T("CPPToolTip::GetGradientColors\n"));
	if (IsExistTool(nIndexTool))
	{
		PPTOOLTIP_INFO ti;
		GetTool(nIndexTool, ti);
		if (ti.nMask & PPTOOLTIP_MASK_COLORS)
		{
			crBegin = ti.crBegin;
			crMid = ti.crMid;
			crEnd = ti.crEnd;
			return;
		}
	}
	crBegin = GetColor(PPTOOLTIP_COLOR_BK_BEGIN);
	crMid = GetColor(PPTOOLTIP_COLOR_BK_MID);
	crEnd = GetColor(PPTOOLTIP_COLOR_BK_END);
} // End of GetGradientColors

void CPPToolTip::SetMaskTool(int nIndexTool, UINT nMask /* = 0 */)
{
	ModifyMaskTool(nIndexTool, nMask, -1);
}

void CPPToolTip::ModifyMaskTool(int nIndexTool, UINT nAddMask, UINT nRemoveMask)
{
	if (!IsExistTool(nIndexTool))
		return;
	PPTOOLTIP_INFO ti;
	GetTool(nIndexTool, ti);
	ti.nMask &= ~nRemoveMask;
	ti.nMask |= nAddMask;
	SetAtTool(nIndexTool, ti);
}

UINT CPPToolTip::GetMaskTool(int nIndexTool)
{
	if (!IsExistTool(nIndexTool))
		return 0;
	PPTOOLTIP_INFO ti;
	GetTool(nIndexTool, ti);
	return ti.nMask;
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetEffectBk (public member function)
//    sets the background's effect
//
//  Parameters :
//		nEffect	[in]  - the background's effect 
//
//  Returns :
//		None 
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::SetEffectBk(UINT nEffect, BYTE nGranularity /* = 2 */, int nIndexTool /* = PPTOOLTIP_TOOL_NOEXIST */)
{
	TRACE (_T("CPPToolTip::SetEffectBk\n"));
	
	if (!IsExistTool(nIndexTool))
	{
		m_nEffect = nEffect;
		m_nGranularity = nGranularity;
	}
	else
	{
		PPTOOLTIP_INFO ti;
		GetTool(nIndexTool, ti);
		ti.nEffect = nEffect;
		ti.nGranularity = nGranularity;
		ti.nMask |= PPTOOLTIP_MASK_EFFECT;
		SetAtTool(nIndexTool, ti);
	}
} // End SetEffectBk

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::GetEffectBk (public member function)
//    gets the background's effect
//
//  Parameters :
//		None 
//
//  Returns :
//		the background's effect 
//
/////////////////////////////////////////////////////////////////////////////
UINT CPPToolTip::GetEffectBk(int nIndexTool /* = PPTOOLTIP_TOOL_NOEXIST */)
{
	BYTE nGranularity = 0;
	
	return GetEffectBk(nGranularity, nIndexTool);
} // End SetEffectBk

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::GetEffectBk (public member function)
//    gets the background's effect
//
//  Parameters :
//		nGranularity	[out] - effect's granularity
//
//  Returns :
//		the background's effect 
//
/////////////////////////////////////////////////////////////////////////////
UINT CPPToolTip::GetEffectBk(BYTE & nGranularity, int nIndexTool /* = PPTOOLTIP_TOOL_NOEXIST */)
{
	TRACE (_T("CPPToolTip::GetEffectBk\n"));
	
	if (IsExistTool(nIndexTool))
	{
		PPTOOLTIP_INFO ti;
		GetTool(nIndexTool, ti);
		if (ti.nMask & PPTOOLTIP_MASK_EFFECT)
		{
			nGranularity = ti.nGranularity;
			return ti.nEffect;
		}
	}
	nGranularity = m_nGranularity;
	return m_nEffect;
} // End SetEffectBk

/////////////////////////////////////////////////////////////////////
// CPPToolTip::SetNotify
// This function sets or removes the notification messages from the control before display.
//
// Parameters:
//	bParentNotify [in] - If TRUE the control will be send the notification 
//				   to parent window
//				   Else the notification will not send
///////////////////////////////////////////////////////////////////////
void CPPToolTip::SetNotify(BOOL bParentNotify /* = TRUE */)
{
	HWND hWnd = NULL;

	if (bParentNotify)
		hWnd = m_pParentWnd->GetSafeHwnd();

	SetNotify(hWnd);
} //End SetNotify

/////////////////////////////////////////////////////////////////////
// CPPToolTip::SetNotify
// This function sets or removes the notification messages from the control before display.
//
// Parameters:
//	hWnd [in] -    If non-NULL the control will be send the notification 
//				   to specified window
//				   Else the notification will not send
///////////////////////////////////////////////////////////////////////
void CPPToolTip::SetNotify(HWND hWnd)
{
	TRACE(_T("CPPToolTip::SetNotify\n"));

	m_hNotifyWnd = hWnd;
} //End SetNotify

/////////////////////////////////////////////////////////////////////
// CPPToolTip::GetNotify
// This function determines will be send the notification messages from 
// the control or not before display.
//
// Return value:
//	TRUE if the control will be notified the specified window
///////////////////////////////////////////////////////////////////////
BOOL CPPToolTip::GetNotify()
{
	TRACE(_T("CPPToolTip::GetNotify\n"));

	return (m_hNotifyWnd != NULL);
}  //End GetNotify

/////////////////////////////////////////////////////////////////////
// CPPToolTip::SetDelayTime
// Call this function to set the delay time for a tool tip control. 
// The delay time is the length of time the cursor must remain on a tool 
// before the tool tip window appears. The default delay time is 500 milliseconds.
//
// Parameters:
//   dwDuration	[in]  - Flag that specifies which duration value will be retrieved. 
//						This parameter can be one of the following values:
//			
//			TTDT_AUTOPOP  - Retrieve the length of time the tool tip 
//							window remains visible if the pointer is 
//							stationary within a tool's bounding rectangle. 
//			TTDT_INITIAL  - Retrieve the length of time the pointer 
//							must remain stationary within a tool's bounding 
//							rectangle before the tool tip window appears. 
//			TTDT_RESHOW   - Retrieve the length of time it takes for 
//							subsequent tool tip windows to appear as the 
//							pointer moves from one tool to another.
//	 nTime [in] - The specified delay time, in milliseconds.
//
/////////////////////////////////////////////////////////////////////
void CPPToolTip::SetDelayTime(DWORD dwDuration, UINT nTime)
{
	switch (dwDuration)
	{
	case TTDT_AUTOPOP:
		m_nTimeAutoPop = nTime;
		break;
	case TTDT_INITIAL :
		m_nTimeInitial = nTime;
		break;
//	case TTDT_RESHOW:
//		m_nTimeReShow = nTime;
//		break;
	}
} // End SetDelayTime

/////////////////////////////////////////////////////////////////////
// CPPToolTip::GetDelayTime
// Retrieves the initial, pop-up, and reshow durations currently set 
// for a CPPToolTip control
//
// Parameters:
//   dwDuration	[in] - Flag that specifies which duration value will 
//					   be retrieved. This parameter can be one of the 
//					   following values:
//			
//			TTDT_AUTOPOP  - Retrieve the length of time the tool tip 
//							window remains visible if the pointer is 
//							stationary within a tool's bounding rectangle. 
//			TTDT_INITIAL  - Retrieve the length of time the pointer 
//							must remain stationary within a tool's bounding 
//							rectangle before the tool tip window appears. 
//			TTDT_RESHOW   - Retrieve the length of time it takes for 
//							subsequent tool tip windows to appear as the 
//							pointer moves from one tool to another. 
//
// Return value:
//	The specified delay time, in milliseconds
///////////////////////////////////////////////////////////////////////
UINT CPPToolTip::GetDelayTime(DWORD dwDuration) const
{
	UINT nTime = 0;
	switch (dwDuration)
	{
	case TTDT_AUTOPOP:
		nTime = m_nTimeAutoPop;
		break;
	case TTDT_INITIAL:
		nTime = m_nTimeInitial;
		break;
//	case TTDT_RESHOW:
//		nTime = m_nTimeReShow;
//		break;
	}

	return nTime;
} // End GetDelayTime

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetSize (public member function)
//    Sets the specified size
//
//  Parameters :
//		nSizeIndex  [in] - index of the size
//		nValue [in] - size's value
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::SetSize(int nSizeIndex, UINT nValue)
{
	TRACE(_T("CPPToolTip::SetSize(nSizeIndex = %d, nValue = %d)\n"), nSizeIndex, nValue);
	if (nSizeIndex >= PPTTSZ_MAX_SIZES)
		return;

	m_nSizes [nSizeIndex] = nValue;
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::GetSize (public member function)
//    Gets the specified size
//
//  Parameters :
//		nSizeIndex  [in] - index of the size
//
//  Returns :
//		size's value
//
/////////////////////////////////////////////////////////////////////////////
UINT CPPToolTip::GetSize(int nSizeIndex)
{
	TRACE(_T("CPPToolTip::GetSize(nSizeIndex = %d)\n"), nSizeIndex);
	if (nSizeIndex >= PPTTSZ_MAX_SIZES)
		return 0;

	return m_nSizes [nSizeIndex];
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetDefaultSizes (public member function)
//    Sets all sizes to default values
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::SetDefaultSizes(BOOL bBalloonSize /* = TRUE */)
{
	TRACE(_T("CPPToolTip::SetDefaultSizes()\n"));

	if (bBalloonSize)
	{
		SetSize(PPTTSZ_ROUNDED_CX, 16);
		SetSize(PPTTSZ_ROUNDED_CY, 16);
		SetSize(PPTTSZ_MARGIN_CX, 12);
		SetSize(PPTTSZ_MARGIN_CY, 12);
		SetSize(PPTTSZ_SHADOW_CX, 4);
		SetSize(PPTTSZ_SHADOW_CY, 4);
		SetSize(PPTTSZ_WIDTH_ANCHOR, 12);
		SetSize(PPTTSZ_HEIGHT_ANCHOR, 16);
		SetSize(PPTTSZ_MARGIN_ANCHOR, 16);
		SetSize(PPTTSZ_BORDER_CX, 1);
		SetSize(PPTTSZ_BORDER_CY, 1);
	}
	else
	{
		SetSize(PPTTSZ_ROUNDED_CX, 0);
		SetSize(PPTTSZ_ROUNDED_CY, 0);
		SetSize(PPTTSZ_MARGIN_CX, 3);
		SetSize(PPTTSZ_MARGIN_CY, 1);
		SetSize(PPTTSZ_SHADOW_CX, 0);
		SetSize(PPTTSZ_SHADOW_CY, 0);
		SetSize(PPTTSZ_WIDTH_ANCHOR, 0);
		SetSize(PPTTSZ_HEIGHT_ANCHOR, 0);
		SetSize(PPTTSZ_MARGIN_ANCHOR, 0);
		SetSize(PPTTSZ_BORDER_CX, 1);
		SetSize(PPTTSZ_BORDER_CY, 1);
	}
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetDirection (public member function)
//    Sets the tooltip's direction
//
//  Parameters :
//		nDirection  [in] - direction of the tooltip
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::SetDirection(UINT nDirection /* = PPTOOLTIP_RIGHT_BOTTOM */, int nIndexTool /* = PPTOOLTIP_TOOL_NOEXIST */)
{
	TRACE(_T("CPPToolTip::SetDirection(nDirection = %d)\n"), nDirection);

	if (nDirection >= PPTOOLTIP_MAX_DIRECTIONS)
		return;

	if (!IsExistTool(nIndexTool))
	{
		m_nDirection = nDirection;
	}
	else
	{
		PPTOOLTIP_INFO ti;
		GetTool(nIndexTool, ti);
		ti.nDirection = nDirection;
		ti.nMask |= PPTOOLTIP_MASK_DIRECTION;
		SetAtTool(nIndexTool, ti);
	}
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::GetDirection (public member function)
//    Gets the tooltip's direction
//
//  Returns :
//		tooltip's direction
//
/////////////////////////////////////////////////////////////////////////////
UINT CPPToolTip::GetDirection(int nIndexTool /* = PPTOOLTIP_TOOL_NOEXIST */)
{
	TRACE(_T("CPPToolTip::GetDirection()\n"));

	if (IsExistTool(nIndexTool))
	{
		PPTOOLTIP_INFO ti;
		GetTool(nIndexTool, ti);
		if (ti.nMask & PPTOOLTIP_MASK_DIRECTION)
			return ti.nDirection;
	}
	return m_nDirection;
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetBehaviour (public member function)
//    Sets the tooltip's direction
//
//  Parameters :
//		nBehaviour  [in] - direction of the tooltip
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////

void CPPToolTip::SetBehaviour(UINT nBehaviour /* = 0 */, int nIndexTool /* = PPTOOLTIP_TOOL_NOEXIST */)
{
	TRACE(_T("CPPToolTip::SetBehaviour(nBehaviour = 0x%X)\n"), nBehaviour);

	if (IsExistTool(nIndexTool))
	{
		PPTOOLTIP_INFO ti;
		GetTool(nIndexTool, ti);
		ti.nBehaviour = nBehaviour;
		ti.nMask |= PPTOOLTIP_MASK_BEHAVIOUR;
		SetAtTool(nIndexTool, ti);
	}
	else m_nBehaviour = nBehaviour;
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::GetBehaviour (public member function)
//    Gets the tooltip's direction
//
//  Returns :
//		tooltip's direction
//
/////////////////////////////////////////////////////////////////////////////
UINT CPPToolTip::GetBehaviour(int nIndexTool /* = PPTOOLTIP_TOOL_NOEXIST */)
{
	TRACE(_T("CPPToolTip::GetBehaviour()\n"));

	if (IsExistTool(nIndexTool))
	{
		PPTOOLTIP_INFO ti;
		GetTool(nIndexTool, ti);
		if (ti.nMask & PPTOOLTIP_MASK_BEHAVIOUR)
			return ti.nBehaviour;
	}
	return m_nBehaviour;
}

/*
/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetMultipleShow (public member function)
//    Sets the multiple show for the tooltip
//
//  Parameters :
//		bMultiple	[in] - 
//		pWnd		[in] - the pointer to the window
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::SetMultipleShow(BOOL bMultiple  = TRUE , CWnd * pWnd  = NULL )
{
	if (bMultiple)
		ModifyStyles(PPTOOLTIP_MULTIPLE_SHOW, 0, pWnd);
	else
		ModifyStyles(0, PPTOOLTIP_MULTIPLE_SHOW, pWnd);
} // End of SetMultipleShow

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::IsMultipleShow (public member function)
//    
//
//  Parameters :
//		pWnd		[in] - the pointer to the window
//
//  Returns :
//		TRUE if for window sets the multiple show
//
/////////////////////////////////////////////////////////////////////////////
BOOL CPPToolTip::IsMultipleShow(CWnd * pWnd  = NULL )
{
	return (BOOL)(PPTOOLTIP_MULTIPLE_SHOW & GetStyles(pWnd));
} // End of IsMultipleShow
*/

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetFont  (public member function)
//    Sets the new font to the control
//
//  Parameters :
//		font		[in] - new font
//
//  Returns :
//		Nonzero if successful; otherwise 0.
//
/////////////////////////////////////////////////////////////////////////////
BOOL CPPToolTip::SetFont(CFont & font)
{
	TRACE(_T("CPPToolTip::SetFont()\n"));

	LOGFONT lf;
	font.GetLogFont (&lf);

	return SetFont(&lf);
}  // End of SetFont

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetFont  (public member function)
//    Sets the new font to the control
//
//  Parameters :
//		lf			[in] - structure LOGFONT for the new font
//
//  Returns :
//		Nonzero if successful; otherwise 0.
//
/////////////////////////////////////////////////////////////////////////////
BOOL CPPToolTip::SetFont(LPLOGFONT lf)
{
	TRACE(_T("CPPToolTip::SetFont()\n"));

//	m_font.DeleteObject();

	// Store font as the global default
    memcpy(&m_LogFont, lf, sizeof(LOGFONT));

	return TRUE; //m_font.CreateFontIndirect(lf);
}  // End of SetFont

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetFont  (public member function)
//    Sets the new font to the control
//
//  Parameters :
//		font		[in] - new font
//
//  Returns :
//		Nonzero if successful; otherwise 0.
//
/////////////////////////////////////////////////////////////////////////////
BOOL CPPToolTip::SetFont(LPCTSTR lpszFaceName, int nSizePoints /* = 8 */,
									BOOL bUnderline /* = FALSE */, BOOL bBold /* = FALSE */,
									BOOL bStrikeOut /* = FALSE */, BOOL bItalic /* = FALSE */)
{
	TRACE(_T("CPPToolTip::SetFont()\n"));

	CDC* pDC = GetDC();
	LOGFONT lf;
	memset (&lf, 0, sizeof(LOGFONT));

	_tcscpy (lf.lfFaceName, lpszFaceName);
	lf.lfHeight = -MulDiv (nSizePoints, GetDeviceCaps (pDC->m_hDC, LOGPIXELSY), 72);
	lf.lfUnderline = bUnderline;
	lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	lf.lfStrikeOut = bStrikeOut;
	lf.lfItalic = bItalic;

	if (pDC)
		ReleaseDC(pDC);

	return SetFont(&lf);
}  // End of SetFont

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetDefaultFonts  (public member function)
//    Sets default fonts of the control
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::SetDefaultFont()
{
	TRACE(_T("CPPToolTip::SetDefaultFont()\n"));

	LPLOGFONT lpSysFont = GetSystemToolTipFont();

	SetFont(lpSysFont);
} // End of SetDefaultFonts

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::GetFont  (public member function)
//    Sets the new font to the control
//
//  Parameters :
//		font		[out] - the current font
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::GetFont(CFont & font)
{
	font.CreateFontIndirect(&m_LogFont);
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::GetFont  (public member function)
//    Sets the new font to the control
//
//  Parameters :
//		lf		[out] - the current font's structure
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::GetFont(LPLOGFONT lf)
{
	memcpy(lf, &m_LogFont, sizeof(LOGFONT));
}

///////////////////////////////////////////////////////////////////
//
//
// Parameters:
//		pt		[in] - the mouse's coordinates in client coordinates
//
// Return values:
//      CWnd*	[out] - the pointer to the window under the mouse. Returns
//                      NULL if under the mouse no control.
///////////////////////////////////////////////////////////////////
HWND CPPToolTip::GetWndFromPoint(CPoint & pt, BOOL bGetDisabled /* = TRUE */) const
{
	ASSERT(m_pParentWnd);
	
    CPoint point = pt;
	// Find the window under the cursor
    m_pParentWnd->ClientToScreen(&point);
    HWND hWnd = ::WindowFromPoint(point);
	
	// WindowFromPoint misses disabled windows and such - go for a more
    // comprehensive search in this case.
	UINT nFlags = CWP_ALL;
	if (!bGetDisabled)
		nFlags |= CWP_SKIPDISABLED;
    if (hWnd == m_pParentWnd->GetSafeHwnd())
        hWnd = m_pParentWnd->ChildWindowFromPoint(pt, nFlags)->GetSafeHwnd();

    return hWnd;
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::AddTool (public member function)
//    adds or updates the tool
//
//  Parameters :
//		pWnd	[in] - the pointer to the window
//		nIdText  [in] - the tooltip's text id 
//		nIdIcon  [in] - the icon's identificator 
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::AddTool(CWnd * pWnd, UINT nIdText, HICON hIcon /* = NULL */,
						 LPCRECT lpRectTool /* = NULL */, UINT nIDTool /* = 0 */)
{
	CString str;
    str.LoadString(nIdText);
	AddTool(pWnd, str, hIcon, lpRectTool, nIDTool);
} // End AddTool

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::AddTool (public member function)
//    adds or updates the tool
//
//  Parameters :
//		pWnd	[in] - the pointer to the window
//		nIdText  [in] - the tooltip's text id 
//		nIdIcon  [in] - the icon's identificator 
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::AddTool(CWnd * pWnd, UINT nIdText, UINT nIdIcon, CSize szIcon /* = CSize(0, 0) */,
						 LPCRECT lpRectTool /* = NULL */, UINT nIDTool /* = 0 */)
{
	CString str;
    str.LoadString(nIdText);
	AddTool(pWnd, str, nIdIcon, szIcon, lpRectTool, nIDTool);
} // End AddTool

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::AddTool (public member function)
//    adds or updates the tool
//
//  Parameters :
//		pWnd	[in] - the pointer to the window
//		sTooltipText [in] - the tooltip's text 
//		nIdIcon  [in] - the icon's identificator 
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::AddTool(CWnd * pWnd, CString sTooltipText, UINT nIdIcon, CSize szIcon /* = CSize(0, 0) */,
						 LPCRECT lpRectTool /* = NULL */, UINT nIDTool /* = 0 */)
{
	HICON hIcon	= GetIconFromResources (nIdIcon, szIcon);

	AddTool(pWnd, sTooltipText, hIcon, lpRectTool, nIDTool);
} // End AddTool

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::AddTool (public member function)
//    adds or updates the tool
//
//  Parameters :
//		pWnd	[in] - the pointer to the window
//		sTooltipText [in] - the tooltip's text 
//		hIcon   [in] - the icon's handle 
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::AddTool(CWnd * pWnd, CString sTooltipText, HICON hIcon /* = NULL */,
						 LPCRECT lpRectTool /* = NULL */, UINT nIDTool /* = 0 */)
{
	// Store the tool information
	PPTOOLTIP_INFO ti;
	ti.hWnd = pWnd->GetSafeHwnd();
	ti.nIDTool = nIDTool;
	ti.hIcon = hIcon;
	ti.sTooltip = sTooltipText;
	ti.nMask = 0; //All values as default
	
    if (lpRectTool)
		ti.rectBounds = lpRectTool;
	else
		ti.rectBounds.SetRectEmpty();

	AddTool(ti);
} // End AddTool

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::AddTool (public member function)
//    adds or updates the tool
//
//  Parameters :
//		ti		[in] - the tooltip's structure 
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::AddTool(PPTOOLTIP_INFO ti) 
{ 
	ASSERT (ti.hWnd);  
	TRACE (_T("CPPToolTip::AddTool = 0x%X\n"), ti.hWnd); 
	// Get bounding region for tooltip info 
/*
    if (ti.rectBounds.IsRectEmpty()) 
    {  
		CRect rect; 
		CWnd::FromHandle(ti.hWnd)->GetClientRect(&rect); 
		//m_pParentWnd->ScreenToClient(rect); 
		ti.rectBounds = rect; 
    } 
*/
	int nIndexTool = FindTool(CWnd::FromHandle(ti.hWnd), ti.rectBounds); 
	if (!IsExistTool(nIndexTool)) 
		m_arrTools.Add(ti); 
	else 
		m_arrTools.SetAt(nIndexTool, ti); 
} // End AddTool

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::FindTool (public member function)
//    searches the tool under cursor point
//
//  Parameters :
//		pt	[in] - cursor point in client coordinates
//
//  Returns :
//		< 0  = Not available tool
//      >= 0 = Index of the found tool
//
/////////////////////////////////////////////////////////////////////////////
int CPPToolTip::FindTool(CPoint & pt) 
{ 
	HWND hWnd = GetWndFromPoint(pt, m_nStyles & PPTOOLTIP_SHOW_DISABLED); 
	
	if (m_arrTools.GetSize() == 1 && ::IsChild(m_pParentWnd->GetSafeHwnd(), hWnd))
		hWnd = m_pParentWnd->GetSafeHwnd();
	
	PPTOOLTIP_INFO pToolInfo;  
	int nSize = m_arrTools.GetSize(); 
	CPoint ptClient;  
	// Find the window under the cursor 
    m_pParentWnd->ClientToScreen(&pt); 
    for (int i = 0; i < nSize; i++) 
    { 
        pToolInfo = m_arrTools.GetAt(i); 
        if (hWnd == pToolInfo.hWnd)  
		{ 
			ptClient = pt; 
			::ScreenToClient(hWnd, &ptClient); 
  			if (pToolInfo.rectBounds.PtInRect(ptClient) || pToolInfo.rectBounds.IsRectEmpty()) 
			{
				return i;  
			}
		}
	}  

	return PPTOOLTIP_TOOL_NOEXIST;
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::FindTool (public member function)
//    searches the tool with window's pointer and rectangle
//
//  Parameters :
//		pWnd	[in] - the pointer to the window
//		lpRect	[in] - rectangle of the hot area. Or NULL for any rectangle
//
//  Returns :
//		< 0  = Not available tool
//      >= 0 = Index of the found tool
//
/////////////////////////////////////////////////////////////////////////////
int CPPToolTip::FindTool(CWnd * pWnd, LPCRECT lpRect /* = NULL */)
{
	HWND hWnd = pWnd->GetSafeHwnd();
	PPTOOLTIP_INFO pToolInfo;
	int nSize = m_arrTools.GetSize();
    for (int i = 0; i < nSize; i++)
    {
        pToolInfo = m_arrTools.GetAt(i);
		
        if (hWnd == pToolInfo.hWnd) 
			if ((NULL == lpRect) || (lpRect == pToolInfo.rectBounds))
			return i;
    }
	return PPTOOLTIP_TOOL_NOEXIST;
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::FindTool (public member function)
//    searches the tool with id
//
//  Parameters :
//		nIDTool	[in] - id of the window
//
//  Returns :
//		< 0  = Not available tool
//      >= 0 = Index of the found tool
//
/////////////////////////////////////////////////////////////////////////////
int CPPToolTip::FindTool(UINT nIDTool)
{
	PPTOOLTIP_INFO pToolInfo;
	int nSize = m_arrTools.GetSize();
    for (int i = 0; i < nSize; i++)
    {
        pToolInfo = m_arrTools.GetAt(i);
		
        if (nIDTool == pToolInfo.nIDTool)
			return i;
    }
	return PPTOOLTIP_TOOL_NOEXIST;
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::GetTool (public member function)
//    gets the tooltip's info
//
//  Parameters :
//		nIndexTool	[in]  - the index of the tool
//		ti		[out] - the tooltip's structure 
//
//  Returns :
//		FALSE - tool not found
//
/////////////////////////////////////////////////////////////////////////////
BOOL CPPToolTip::GetTool(int nIndexTool, PPTOOLTIP_INFO & ti)
{
	if (!IsExistTool(nIndexTool))
		return FALSE;
	ti = m_arrTools.GetAt(nIndexTool);
	return TRUE;
} // End GetTool

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::RemoveTool (public member function)
//    removes the tool
//
//  Parameters :
//		pWnd	[in]  - the pointer to the window
//
//  Returns :
//		None 
//
/////////////////////////////////////////////////////////////////////////////
BOOL CPPToolTip::RemoveTool(int nIndexTool)
{
	TRACE (_T("CPPToolTip::RemoveTool\n"));

	if (!IsExistTool(nIndexTool))
		return FALSE;
	
	m_arrTools.RemoveAt(nIndexTool);

	return TRUE;
} // End RemoveTool

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::RemoveAllTools (public member function)
//    removes the tool
//
//  Parameters :
//		None
//
//  Returns :
//		None 
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::RemoveAllTools()
{
	TRACE (_T("CPPToolTip::RemoveAllTools\n"));
	
	m_arrTools.RemoveAll();
} // End RemoveAllTools

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::IsExistTool (public member function)
//    removes the tool
//
//  Parameters :
//		nIndexTool	[in]  - the index of the tool
//
//  Returns :
//		TRUE - the tool is exist
//
/////////////////////////////////////////////////////////////////////////////
BOOL CPPToolTip::IsExistTool(int nIndexTool)
{
	return (BOOL)((nIndexTool < m_arrTools.GetSize()) && (nIndexTool >= 0));
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetAtTool (public member function)
//    adds or updates the tool
//
//  Parameters :
//		nIndexTool	[in] - the index of the tool
//		ti		[in] - the tooltip's structure 
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::SetAtTool(int nIndexTool, PPTOOLTIP_INFO & ti)
{
	if (!IsExistTool(nIndexTool))
		return;
	m_arrTools.SetAt(nIndexTool, ti);
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::ShowHelpTooltip (public member function)
//    shows the tooltip as help window
//
//  Parameters :
//		pt	[in] - the point of the tooltip's anchor in client coordinates
//		nIdText  [in] - the tooltip's text id 
//		nIdIcon  [in] - the icon's identificator 
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::ShowHelpTooltip(CPoint & pt, UINT nIdText, HICON hIcon /* = NULL */)
{
	CString str;
    str.LoadString(nIdText);
	ShowHelpTooltip(pt, str, hIcon);
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::ShowHelpTooltip (public member function)
//    shows the tooltip as help window
//
//  Parameters :
//		pt	[in] - the point of the tooltip's anchor in client coordinates
//		nIdText  [in] - the tooltip's text id 
//		nIdIcon  [in] - the icon's identificator 
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::ShowHelpTooltip(CPoint & pt, UINT nIdText, UINT nIdIcon)
{
	CString str;
    str.LoadString(nIdText);
	ShowHelpTooltip(pt, str, nIdIcon);
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::ShowHelpTooltip (public member function)
//    shows the tooltip as help window
//
//  Parameters :
//		pt	[in] - the point of the tooltip's anchor in client coordinates
//		sTooltipText [in] - the tooltip's text 
//		nIdIcon  [in] - the icon's identificator 
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::ShowHelpTooltip(CPoint & pt, CString sTooltipText, UINT nIdIcon)
{
	HICON hIcon	= GetIconFromResources(nIdIcon);
	ShowHelpTooltip(pt, sTooltipText, hIcon);
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::ShowHelpTooltip (public member function)
//    shows the tooltip as help window
//
//  Parameters :
//		pt	[in] - the point of the tooltip's anchor in client coordinates
//		sTooltipText [in] - the tooltip's text 
//		hIcon   [in] - the icon's handle 
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::ShowHelpTooltip(CPoint & pt, CString sTooltipText, HICON hIcon /* = NULL */)
{
	PPTOOLTIP_INFO ti;
	ti.hWnd = m_pParentWnd->GetSafeHwnd();
	ti.hIcon = hIcon;
	ti.sTooltip = sTooltipText;
	ti.nMask = 0; //All values as default
	ShowHelpTooltip(pt, ti);
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::ShowHelpTooltip (public member function)
//    shows the tooltip as help window
//
//  Parameters :
//		pt		[in] - the point of the tooltip's anchor in client coordinates
//		ti		[in] - the tooltip's structure 
//
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::ShowHelpTooltip(CPoint & pt, PPTOOLTIP_INFO & ti)
{
	TRACE(_T("ShowHelpTooltip()\n"));
	m_nIndexDisplayWnd = PPTOOLTIP_TOOL_NOEXIST;
	m_nIndexCurrentWnd = PPTOOLTIP_TOOL_HELPER;
	m_pToolInfo = ti;

	m_ptOriginal = pt;
	m_pParentWnd->ClientToScreen(&m_ptOriginal);
	
	//Start the show timer
	OnTimer(PPTOOLTIP_SHOW);
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetImageList (public member function)
//    sets the image list to tooltip
//
//  Parameters :
//		nIdBitmap	[in] - Resource IDs of the bitmap to be associated with the image list
//		cx			[in] - Dimensions of each image, in pixels.
//		cy			[in] - Dimensions of each image, in pixels.
//		nCount		[in] - Number of images that the image list initially contains.
//		crMask		[in] - Color used to generate a mask. Each pixel of this color in the 
//						   specified bitmap is changed to black, and the corresponding 
//						   bit in the mask is set to one.
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::SetImageList(UINT nIdBitmap, int cx, int cy, int nCount, COLORREF crMask /* = RGB(255, 0, 255) */)
{
	// Load bitmap
	HBITMAP hBitmap = GetBitmapFromResources(nIdBitmap);
	SetImageList(hBitmap, cx, cy, nCount, crMask);
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::SetImageList (public member function)
//    sets the image list to tooltip
//
//  Parameters :
//		hBitmap		[in] - Handle of the bitmap to be associated with the image list
//		cx			[in] - Dimensions of each image, in pixels.
//		cy			[in] - Dimensions of each image, in pixels.
//		nCount		[in] - Number of images that the image list initially contains.
//		crMask		[in] - Color used to generate a mask. Each pixel of this color in the 
//						   specified bitmap is changed to black, and the corresponding 
//						   bit in the mask is set to one.
//  Returns :
//		None
//
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::SetImageList(HBITMAP hBitmap, int cx, int cy, int nCount, COLORREF crMask /* = RGB(255, 0, 255) */)
{
	if (m_imgTooltip.m_hImageList != NULL)
		m_imgTooltip.DeleteImageList();

	m_imgTooltip.Create(cx, cy, ILC_COLOR32 | ILC_MASK, nCount, 1);
	m_imgTooltip.Add(CBitmap::FromHandle(hBitmap), crMask);

	m_szImage = CSize(cx, cy);
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::GetImageList (public member function)
//    gets the image list from tooltip
//
//  Parameters :
//		sz		   [out] - Dimensions of each image, in pixels.
//  Returns :
//		A pointer to a CImageList object
//
/////////////////////////////////////////////////////////////////////////////
CImageList * CPPToolTip::GetImageList(CSize & sz)
{
	sz = m_szImage;
	return &m_imgTooltip;
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::AddNameOfResource (public member function)
//    Associates the resource name with him ID
//
//  Parameters :
//		sName		[in] - A resource name in format string of the tooltip
//		nID			[in] - A resource ID associated with name.
//		nTypeRes	[in] - A resource type:
//							TYPE_RES_ICON - a resource is a icon
//							TYPE_RES_BITMAP - a resource is a bitmap
//							TYPE_RES_MASK_BITMAP - a resource is a transparent bitmap
//		crMask		[in] - Color used to generate a mask. Each pixel of this color in the 
//						   specified bitmap is changed to black, and the corresponding 
//						   bit in the mask is set to one.
//  Returns :
//		None
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::AddNameOfResource(CString sName, UINT nID, BYTE nTypeRes /* = TYPE_RES_TRAN_BITMAP */, COLORREF crMask /* = RGB(255, 0, 255) */)
{
	if (sName.IsEmpty() || (nID == 0) || (nTypeRes >= MAX_TYPES_RES))
		return;
	
	PPTOOLTIP_NAME_RES nr;
	nr.sName = sName;
	nr.nID = nID;
	nr.nTypeRes = nTypeRes;
	nr.crMask = crMask;

	m_arrNameRes.Add(nr);
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::FindIdOfResource (public member function)
//    Search the ID resource by his name
//
//  Parameters :
//		sName		[in] - A resource name in format string of the tooltip
//  Returns :
//		The array's index with ID resource associated with name or -1 if ID not found
/////////////////////////////////////////////////////////////////////////////
int CPPToolTip::FindIdOfResource(CString sName)
{
	PPTOOLTIP_NAME_RES nr;
	for (int i = 0; i < m_arrNameRes.GetSize(); i++)
	{
		nr = m_arrNameRes.GetAt(i);
		if (!sName.CompareNoCase(nr.sName))
			return i;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::FindIdOfResource (public member function)
//    Search the resource name by his ID
//
//  Parameters :
//		nID		[in] - A ID resource
//  Returns :
//		The array's index with resource name associated with ID or -1 if name not found
/////////////////////////////////////////////////////////////////////////////
int CPPToolTip::FindNameOfResource(UINT nID)
{
	PPTOOLTIP_NAME_RES nr;
	for (int i = 0; i < m_arrNameRes.GetSize(); i++)
	{
		nr = m_arrNameRes.GetAt(i);
		if (nr.nID == nID)
			return i;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::FindIdOfResource (public member function)
//    Removes resource name by index of the array
//
//  Parameters :
//		nIndex		[in] - A The index of the resource name in the array
//  Returns :
//		None
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::RemoveNameOfResource(int nIndex)
{
	if (nIndex < m_arrNameRes.GetSize())
		m_arrNameRes.RemoveAt(nIndex);
}

/////////////////////////////////////////////////////////////////////////////
//  CPPToolTip::FindIdOfResource (public member function)
//    Removes all resource names from the array
/////////////////////////////////////////////////////////////////////////////
void CPPToolTip::RemoveAllNamesOfResource()
{
	m_arrNameRes.RemoveAll();
}
