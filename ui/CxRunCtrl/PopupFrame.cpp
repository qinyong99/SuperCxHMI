// PopupFrame.cpp : implementation file
//

#include "stdafx.h"
#include "RunInc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopupFrame

typedef struct tagKEYNAME
{
	TCHAR szName[32];
	int nVirtKey;
} KEYNAME;

LPCTSTR GetKeyName(int nVirtKey)
{
	static KEYNAME keys[]=
	{
		{"Left mouse button", VK_LBUTTON},
		{"Right mouse button", VK_RBUTTON},
		{"Control-break processing", VK_CANCEL},
		{"Middle mouse button", VK_MBUTTON},
		{"Back Space", VK_BACK},
		{"Tab", VK_TAB},
		{"Clear", VK_CLEAR},
		{"Enter", VK_RETURN},
		{"Shift", VK_SHIFT},
		{"Ctrl", VK_CONTROL},
		{"Alt", VK_MENU},
		{"Pause", VK_PAUSE},
		{"Caps Lock", VK_CAPITAL},
		{"Esc", VK_ESCAPE},
		{"Space", VK_SPACE},
		{"Page Up", VK_PRIOR},  
		{"Page Down", VK_NEXT},
		{"End", VK_END},
		{"Home", VK_HOME},
		{"Left", VK_LEFT},
		{"Up", VK_UP},
		{"Right", VK_RIGHT},
		{"Down", VK_DOWN},
		{"Select", VK_SELECT},
		{"Excute", VK_EXECUTE},
		{"Print Screen", VK_SNAPSHOT},
		{"Insert", VK_INSERT},
		{"Delete", VK_DELETE},
		{"Help", VK_HELP},
		{"0",'0'},
		{"1",'1'},
		{"2",'2'},
		{"3",'3'},
		{"4",'4'},
		{"5",'5'},
		{"6",'6'},
		{"7",'7'},
		{"8",'8'},
		{"9",'9'},
		{"A",'A'},
		{"B",'B'},
		{"C",'C'},
		{"D",'D'},
		{"E",'E'},
		{"F",'F'},
		{"G",'G'},
		{"H",'H'},
		{"I",'I'},
		{"J",'J'},
		{"K",'K'},
		{"L",'L'},
		{"M",'M'},
		{"N",'N'},
		{"O",'O'},
		{"P",'P'},
		{"Q",'Q'},
		{"R",'R'},
		{"S",'S'},
		{"T",'T'},
		{"U",'U'},
		{"V",'V'},
		{"W",'W'},
		{"X",'X'},
		{"Y",'Y'},
		{"Z",'Z'},
		{"Left windows", VK_LWIN},
		{"Right windows", VK_RWIN},
		{"Applications", VK_APPS},
		{"Numeric keypad 0", VK_NUMPAD0},
		{"Numeric keypad 1", VK_NUMPAD1},
		{"Numeric keypad 2", VK_NUMPAD2},
		{"Numeric keypad 3", VK_NUMPAD3},
		{"Numeric keypad 4", VK_NUMPAD4},
		{"Numeric keypad 5", VK_NUMPAD5},
		{"Numeric keypad 6", VK_NUMPAD6},
		{"Numeric keypad 7", VK_NUMPAD7},
		{"Numeric keypad 8", VK_NUMPAD8},
		{"Numeric keypad 9", VK_NUMPAD9}, 
		{"Multiply", VK_MULTIPLY},
		{"Add", VK_ADD},
		{"Separator", VK_SEPARATOR},
		{"Subtract", VK_SUBTRACT},
		{"Decimal Point", VK_DECIMAL},
		{"Divide", VK_DIVIDE},
		{"F1", VK_F1},
		{"F2", VK_F2},
		{"F3", VK_F3},
		{"F4", VK_F4},
		{"F5", VK_F5},
		{"F6", VK_F6},
		{"F7", VK_F7},
		{"F8", VK_F8},
		{"F9", VK_F9},
		{"F10", VK_F10},
		{"F11", VK_F11},
		{"F12", VK_F12},
		{"F13", VK_F13},
		{"F14", VK_F14},
		{"F15", VK_F15},
		{"F16", VK_F16},
		{"F17", VK_F17},
		{"F18", VK_F18},
		{"F19", VK_F19},
		{"F20", VK_F20},
		{"F21", VK_F21},
		{"F22", VK_F22},
		{"F23", VK_F23},
		{"F24", VK_F24},
		{"Attn", VK_ATTN},
		{"CrSel", VK_CRSEL},
		{"ExSel", VK_EXSEL},
		{"Erase", VK_EREOF},
		{"Play", VK_PLAY},
		{"Zoom", VK_ZOOM},
		{"Reserved for future use", VK_NONAME},
		{"PA1", VK_PA1},
		{"Clear(OEM)", VK_OEM_CLEAR},
		{"\\", '\\'},
		{"-", '-'},
		{"=", '='},
		{"[", '['},
		{"]", ']'},
		{";", ';'},
		{"\'", '\''},
		{",", ','},
		{".", '.'},
		{"/", '/'},
		{"`", '`'}
	};

	for (int i = 0; i < sizeof(keys) / sizeof(KEYNAME); i++)
	{
		if (keys[i].nVirtKey == nVirtKey)
			return keys[i].szName;
	}

	return NULL;
}

CString GetAccelKeyName(DWORD dwShift, DWORD dwKeyCode)
{
	CString strText;

	if ((dwShift & 0x2) != 0)
	{
		strText += _T("Ctrl");
	}
	if ((dwShift & 0x4) != 0)
	{
		if (!strText.IsEmpty())
			strText += _T(" + ");
		strText += _T("Alt");
	}
	if ((dwShift & 0x1) != 0)
	{
		if (!strText.IsEmpty())
			strText += _T(" + ");
		strText += _T("Shift");
	}
	
	if (dwKeyCode != VK_CONTROL && dwKeyCode != VK_MENU && dwKeyCode != VK_SHIFT)
	{
		LPCTSTR pKeyName = GetKeyName(dwKeyCode);
		if (pKeyName != NULL)
		{
			if (!strText.IsEmpty())
				strText += _T(" + ");
			
			strText += GetKeyName(dwKeyCode);
		}
	}

	return strText;
}

IMPLEMENT_DYNCREATE(CPopupFrame, CFrameWnd)

CPopupFrame::CPopupFrame() :
	m_bMouseTracking(FALSE),
	m_bActive(FALSE),
	m_nBtnPressed(-1), m_nBtnWasPressed(-1), m_nBtnHover(-1),
	m_hThemeDLL(NULL), m_hTheme(NULL), m_pfnDrawThemeBackground(NULL),
	m_cxyOffset(2),
	m_cxBtnWidth(16), m_cyBtnHeight(14),
	m_cxFrameWidth(2), m_cyFrameHeight(2)
{
	m_x = m_y = m_cx = m_cy = 0;
	m_bCaption = FALSE;
	m_bThickframe = FALSE;
	m_bControlBox = TRUE;
	m_bTopMost = FALSE;

	m_bDisableModal = FALSE;

	m_pMenu = NULL;
	m_hAccelTable = NULL;
}

CPopupFrame::~CPopupFrame()
{
	if (m_hAccelTable != NULL)
	{
		DestroyAcceleratorTable(m_hAccelTable);
		m_hAccelTable = NULL;
	}
}

//const int WM_NCMOUSEHOVER = 0x2A0
//const int TME_NONCLIENT = 0x00000010
//const int WM_NCMOUSELEAVE = 0x02A2 

BEGIN_MESSAGE_MAP(CPopupFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CPopupFrame)
	ON_WM_CLOSE()
	ON_WM_NCACTIVATE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_NCMOUSEMOVE()
	ON_WM_MOUSEMOVE()
//	ON_MESSAGE(0x2A0, OnNcMouseLeave)
	ON_MESSAGE(0x02A2, OnNcMouseLeave)
	ON_WM_NCMOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	ON_WM_NCPAINT()
	ON_MESSAGE(WM_DISABLEMODAL, OnDisableModal)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(_GetThemeChangedMsg(), OnThemeChanged)
	ON_WM_SETTINGCHANGE()
	ON_MESSAGE(WM_CLOSE_FORM, OnCloseForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupFrame message handlers


BOOL CPopupFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	cs.x = m_x;
	cs.y = m_y;
	cs.cx = m_cx;// + 2 * GetSystemMetrics(SM_CXFRAME);
	cs.cy = m_cy;// + 2 * GetSystemMetrics(SM_CYFRAME);

	cs.hMenu = NULL;
	cs.lpszName = m_strCaption;

	cs.dwExStyle = WS_EX_WINDOWEDGE;
	
	cs.style = WS_VISIBLE | WS_POPUP /*| WS_CAPTION*//* | WS_SYSMENU*/;
	if (m_bCaption)
		cs.style |= WS_CAPTION;
	if (m_bThickframe)
		cs.style |= WS_THICKFRAME;

	cs.style &= ~FWS_ADDTOTITLE;
	
	return TRUE;
}

BOOL CPopupFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	CFormDoc* pDoc = (CFormDoc *)pContext->m_pCurrentDoc;
	ASSERT_VALID(pDoc);

	m_strCaption = pDoc->m_strCaption;
	
	m_cx = (int)pDoc->GetWidth();
	m_cy = (int)pDoc->GetHeight();

	if (pDoc->m_enumStartUpPosition == StartUpPositionWindowsDefault)
	{
		m_x = 0;
		m_y = 0;
	}
	else if (pDoc->m_enumStartUpPosition == StartUpPositionManual)
	{
		m_x = (int)pDoc->GetHorizontalPosition();
		m_y = (int)pDoc->GetVerticalPosition();
	}
	else 
	{
		CRect rect;
		
		if (pDoc->m_enumStartUpPosition == StartUpPositionOwner)
		{
			pParentWnd->GetWindowRect(&rect);
		}
		else
		{
			rect.left = 0;
			rect.top = 0;
			rect.right = GetSystemMetrics(SM_CXSCREEN);
			rect.bottom = GetSystemMetrics(SM_CYSCREEN);
		}
	
		if (rect.Width() > m_cx)
			m_x = (rect.right + rect.left - m_cx) / 2;
		
		if (rect.Height() > m_cy)
			m_y = (rect.bottom + rect.top - m_cy) / 2;
	}

	if (!pDoc->IsMainForm())
	{
		m_bTopMost = TRUE;
		m_bCaption = pDoc->m_enumBorderStyle != BorderStyleNone;
		m_bThickframe = pDoc->m_enumBorderStyle == BorderStyleSizable;
		m_bControlBox = pDoc->m_bControlBox;
	}
	
	BOOL b = CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);

	if (b)
	{
		CDevObj* pObj = pDoc->GetMenuObject();
		if (pObj != NULL)
		{
			CMenu menu;
			menu.CreateMenu();
					
			CComQIPtr<IDrawMenu, &IID_IDrawMenu> spDrawMenu(pObj->GetObjectUnknown());
			long lCount;
			spDrawMenu->get_ItemCount(&lCount);
			
			int nIndex = 0;
			AddMenuItems(menu.m_hMenu, spDrawMenu, nIndex, lCount, 0);

			m_pMenu = spDrawMenu;
			SetMenu(&menu);

			menu.Detach();

			if (m_arrAccels.GetSize() > 0)
				m_hAccelTable = CreateAcceleratorTable(m_arrAccels.GetData(), m_arrAccels.GetSize());

		}
	}

	return b;
}

void CPopupFrame::OnClose() 
{
	((CFormDoc *)GetActiveDocument())->Close();
	ShowOwnedWindows(TRUE);

	m_nBtnWasPressed = m_nBtnPressed = -1;
}

BOOL CPopupFrame::OnNcActivate(BOOL bActive) 
{
	BOOL b = CFrameWnd::OnNcActivate(bActive);

    if (m_nFlags & WF_STAYACTIVE)
        bActive = TRUE;
    if (!IsWindowEnabled())
        bActive = FALSE;

	m_bActive = bActive; 

	if ((GetStyle() & WS_CAPTION) == WS_CAPTION)
	{	
		// get DC and window rectangle
		CWindowDC dc(this);
		RECT rect;
		GetWindowRect(&rect);
		int cxWidth = rect.right - rect.left;
		int cyHeight = rect.bottom - rect.top;

		// draw buttons
		RECT arrRect[3] = { 0 };
		CalcBtnRects(cxWidth, cyHeight, arrRect);
		DrawButtons(&dc, arrRect, -1);   // draw all buttons
	}

	return b;

//    // Mimic MFC kludge to stay active if WF_STAYACTIVE bit is on. Yuk!
//    if (m_nFlags & WF_STAYACTIVE)
//        bActive = TRUE;
//    if (!IsWindowEnabled())
//        bActive = FALSE;
//    if (bActive == m_bActive)
//        return TRUE;
//
//    // If this is a MDI app, manually activate/paint active MDI child
//    // window, because Windows won't do it if parent frame is invisible.
//    // Must do this BEFORE calling Default, or it will not work.
//    //
//    CFrameWnd* pActiveFrame = GetActiveFrame();
//    if (pActiveFrame != this) {
//        pActiveFrame->SendMessage(WM_NCACTIVATE, bActive);
//        pActiveFrame->SendMessage(WM_NCPAINT);
//    }
//
//    // Turn WS_VISIBLE off before calling DefWindowProc,
//    // so DefWindowProc won't paint and thereby cause flicker.
//    // 
//    DWORD dwStyle = GetStyle();
//    if (dwStyle & WS_VISIBLE)
//        ::SetWindowLong(m_hWnd, GWL_STYLE, (dwStyle & ~ WS_VISIBLE));
//
//    MSG& msg = AfxGetThreadState()->m_lastSentMsg;
//    msg.wParam = bActive;
//    Default();
//    if (dwStyle & WS_VISIBLE)
//        ::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
//
//    // At this point, nothing has happened (since WS_VISIBLE was off).
//    // Now it's time to paint.
//    //
//    m_bActive = bActive;                  // update state
//    SendMessage(WM_NCPAINT);    // paint non-client area (frame too)
//
//    return TRUE; 
}

void CPopupFrame::OnLButtonUp(UINT nHitTest, CPoint point) 
{
	ClientToScreen(&point);
	RECT rect = { 0 };
	GetWindowRect(&rect);
	point.x -= rect.left;
	point.y -= rect.top;

	int nBtn = m_nBtnWasPressed;
	ReleaseCapture();
	
	RECT arrRect[3] = { 0 };
	CalcBtnRects(rect.right - rect.left, rect.bottom - rect.top, arrRect, false/*m_bLayoutRTL*/);
	if(::PtInRect(&arrRect[nBtn], point))
	{
		switch(nBtn)
		{
		case 0:		// close
			::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0L);
			break;
		case 1:		// restore
			::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0L);
			break;
		case 2:		// minimize
			::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0L);
			break;
		default:
			break;
		}
	}
}

void CPopupFrame::OnCaptureChanged(CWnd* pWnd)
{
	if (m_nBtnPressed != -1)
	{
		ATLASSERT(m_nBtnPressed == m_nBtnWasPressed);   // must be
		m_nBtnPressed = -1;
		RECT rect = { 0 };
		GetWindowRect(&rect);
		RECT arrRect[3] = { 0 };
		CalcBtnRects(rect.right - rect.left, rect.bottom - rect.top, arrRect);
		CWindowDC dc(this);
		DrawButtons(&dc, arrRect, m_nBtnWasPressed);
		m_nBtnWasPressed = -1;
	}
	else
	{
		Default();
	}
}

void CPopupFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	RECT rect = { 0 };
	GetWindowRect(&rect);
	point.x -= rect.left;
	point.y -= rect.top;

	RECT arrRect[3] = { 0 };
	CalcBtnRects(rect.right - rect.left, rect.bottom - rect.top, arrRect, /*m_bLayoutRTL*/false);

	if (::PtInRect(&arrRect[0], point) && m_bControlBox)
	{
		m_nBtnWasPressed = m_nBtnPressed = 0;
	}
//	else if(::PtInRect(&arrRect[1], point))
//	{
//		m_nBtnWasPressed = m_nBtnPressed = 1;
//	}
//	else if(::PtInRect(&arrRect[2], point))
//	{
//		m_nBtnWasPressed = m_nBtnPressed = 2;
//	}
	else
	{
		CFrameWnd::OnNcLButtonDown(nHitTest, point);
	}

	// draw the button state if it was pressed
	if (m_nBtnPressed != -1)
	{
		SetActiveWindow();

		SetCapture();
		CWindowDC dc(this);
		CalcBtnRects(rect.right - rect.left, rect.bottom - rect.top, arrRect);
		DrawButtons(&dc, arrRect, m_nBtnPressed);
	}
}

BOOL CPopupFrame::StartTrackMouseHover()
{
	TRACKMOUSEEVENT tme = { 0 };
	tme.cbSize = sizeof(tme);
	tme.dwFlags = TME_LEAVE | /*TME_HOVER | */0x00000010;
	tme.hwndTrack = m_hWnd;
	return _TrackMouseEvent(&tme);
}

void CPopupFrame::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	if (m_hThemeDLL == NULL)
		return;

	RECT rect = { 0 };
	GetWindowRect(&rect);
	point.x -= rect.left;
	point.y -= rect.top;

	RECT arrRect[3] = { 0 };
	CalcBtnRects(rect.right - rect.left, rect.bottom - rect.top, arrRect, /*m_bLayoutRTL*/false);

	int nOldBtnHover = m_nBtnHover;

	if (::PtInRect(&arrRect[0], point))
	{
		m_nBtnHover = 0;
	}
	else
	{
		m_nBtnHover = -1;
	}

	if (nOldBtnHover != m_nBtnHover)
	{
		CWindowDC dc(this);
		CalcBtnRects(rect.right - rect.left, rect.bottom - rect.top, arrRect);
		DrawButtons(&dc, arrRect, 0);
	}

	if (m_nBtnHover != -1 && !m_bMouseTracking)
	{
		if (StartTrackMouseHover())
			m_bMouseTracking = TRUE;
	}
}

LRESULT CPopupFrame::OnNcMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_nBtnHover = -1;
	
	RECT rect = { 0 };
	GetWindowRect(&rect);
	RECT arrRect[3] = { 0 };
	CalcBtnRects(rect.right - rect.left, rect.bottom - rect.top, arrRect, /*m_bLayoutRTL*/false);
	
	CWindowDC dc(this);
	CalcBtnRects(rect.right - rect.left, rect.bottom - rect.top, arrRect);
	DrawButtons(&dc, arrRect, 0);

	m_bMouseTracking = FALSE;

	return 0;
}

void CPopupFrame::OnMouseMove(UINT nHitTest, CPoint point) 
{
	ClientToScreen(&point);
	RECT rect = { 0 };
	GetWindowRect(&rect);
	point.x -= rect.left;
	point.y -= rect.top;
	RECT arrRect[3] = { 0 };
	CalcBtnRects(rect.right - rect.left, rect.bottom - rect.top, arrRect, /*m_bLayoutRTL*/false);
	int nOldBtnPressed = m_nBtnPressed;
	m_nBtnPressed = ::PtInRect(&arrRect[m_nBtnWasPressed], point) ? m_nBtnWasPressed : -1;
	if (m_nBtnHover != -1 || nOldBtnPressed != m_nBtnPressed)
	{
		m_nBtnHover = 0;

		CWindowDC dc(this);
		CalcBtnRects(rect.right - rect.left, rect.bottom - rect.top, arrRect);
		DrawButtons(&dc, arrRect, (m_nBtnPressed != -1) ? m_nBtnPressed : nOldBtnPressed);
	}
}

void CPopupFrame::OnNcPaint() 
{
	CFrameWnd::OnNcPaint();

	if ((GetStyle() & WS_CAPTION) == WS_CAPTION)
	{	
		// get DC and window rectangle
		CWindowDC dc(this);
		RECT rect;
		GetWindowRect(&rect);
		int cxWidth = rect.right - rect.left;
		int cyHeight = rect.bottom - rect.top;

		// draw buttons
		RECT arrRect[3] = { 0 };
		CalcBtnRects(cxWidth, cyHeight, arrRect);
		DrawButtons(&dc, arrRect, -1);   // draw all buttons
	}
}

void CPopupFrame::CalcBtnRects(int cxWidth, int cyHeight, RECT arrRect[3], bool bInvertX)
{
	int yStart = m_cxyOffset;
	
	RECT rcBtn = { cxWidth - m_cxBtnWidth, yStart, cxWidth, yStart + m_cyBtnHeight };
	int nDirection = -1;
	if (bInvertX)
	{
		::SetRect(&rcBtn, 0, yStart, m_cxBtnWidth, yStart + m_cyBtnHeight);
		nDirection = 1;
	}

	::OffsetRect(&rcBtn, nDirection * (m_cxyOffset + m_cxFrameWidth), m_cyFrameHeight);
	arrRect[0] = rcBtn;
	if (m_hTheme != NULL)
		::OffsetRect(&rcBtn, nDirection * (m_cxBtnWidth + m_cxyOffset), 0);
	else
		::OffsetRect(&rcBtn, nDirection * (m_cxBtnWidth + m_cxyOffset), 0);
	arrRect[1] = rcBtn;
	if (m_hTheme != NULL)
		::OffsetRect(&rcBtn, nDirection * (m_cxBtnWidth + m_cxyOffset), 0);
	else
		::OffsetRect(&rcBtn, nDirection * m_cxBtnWidth, 0);
	arrRect[2] = rcBtn;
}

#ifndef TMSCHEMA_H

const int WP_MINBUTTON = 15;
const int WP_MAXBUTTON = 17;
const int WP_CLOSEBUTTON = 18;
const int CBS_NORMAL = 1;
const int CBS_HOT = 2;
const int CBS_PUSHED = 3;
const int CBS_DISABLED = 4;
const int CBS_INACTIVE = 5;
const int RBS_NORMAL = 1;
const int RBS_PUSHED = 3;
const int MINBS_NORMAL = 1;
const int MINBS_PUSHED = 3;

#endif //TMSCHEMA_H

int CPopupFrame::GetButtonState(int nBtn)
{
	if (m_nBtnPressed == nBtn)
		return CBS_PUSHED;
	else if (m_nBtnHover == nBtn)
		return CBS_HOT;
	else if (!m_bActive)
		return CBS_INACTIVE;

	return CBS_NORMAL;
}

void CPopupFrame::DrawButtons(CDC* pDC, LPRECT pRects, int nBtn)
{
	if (!m_bControlBox)
		return;

	if (m_hTheme != NULL)
	{
		if (nBtn == -1 || nBtn == 0)
			m_pfnDrawThemeBackground(m_hTheme, pDC->GetSafeHdc(), WP_CLOSEBUTTON, GetButtonState(0), &pRects[0], NULL);
//		if (nBtn == -1 || nBtn == 1)
//			m_pfnDrawThemeBackground(m_hTheme, pDC->GetSafeHdc(), WP_MAXBUTTON, (m_nBtnPressed == 1) ? RBS_PUSHED : RBS_NORMAL, &pRects[1], NULL);
//		if (nBtn == -1 || nBtn == 2)
//			m_pfnDrawThemeBackground(m_hTheme, pDC->GetSafeHdc(), WP_MINBUTTON, (m_nBtnPressed == 2) ? MINBS_PUSHED : MINBS_NORMAL, &pRects[2], NULL);
	}
	else
	{
		if (nBtn == -1 || nBtn == 0)
			pDC->DrawFrameControl(&pRects[0], DFC_CAPTION, DFCS_CAPTIONCLOSE | ((m_nBtnPressed == 0) ? DFCS_PUSHED : 0));
//		if (nBtn == -1 || nBtn == 1)
//			pDC->DrawFrameControl(&pRects[1], DFC_CAPTION, DFCS_CAPTIONRESTORE | ((m_nBtnPressed == 1) ? DFCS_PUSHED : 0));
//		if (nBtn == -1 || nBtn == 2)
//			pDC->DrawFrameControl(&pRects[2], DFC_CAPTION, DFCS_CAPTIONMIN | ((m_nBtnPressed == 2) ? DFCS_PUSHED : 0));
	}
}

LRESULT CPopupFrame::OnDisableModal(WPARAM, LPARAM)
{
	return m_bDisableModal;
}

BOOL CPopupFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (m_pMenu != NULL && pHandlerInfo == NULL)
	{
		if (nCode == CN_COMMAND)
		{
			if (m_pMenu->SelectItem(nID) == S_OK)
				return TRUE;			
		}
		else if (nCode == CN_UPDATE_COMMAND_UI)
		{
			CCmdUI* pCmdUI = (CCmdUI*)pExtra;

			long lState;
			if (m_pMenu->GetItemState(pCmdUI->m_nID, &lState) == S_OK)
			{
				pCmdUI->Enable((lState & MenuStateDisable) == 0);
				pCmdUI->SetCheck((lState & MenuStateCheck) != 0);
				
				return TRUE;
			}
		}	
	}
	
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CPopupFrame::AddMenuItems(HMENU hMenu, IDrawMenu* pMenu, int& nIndex, int nCount, int nDepth)
{
	USES_CONVERSION;
	HTREEITEM hItem = NULL;
	
	while (nIndex < nCount)
	{
		CComBSTR bstrItemText;
		pMenu->GetItemText(nIndex + 1, &bstrItemText);
		
		int nItemDepth = 0;
		LPCWSTR p = bstrItemText;
		while (*p++ == '.')
			nItemDepth++;
		
		if (nItemDepth < nDepth)
		{
			nIndex--;
			break;
		}
		
		int nNextItemDepth = nItemDepth;
		if (nIndex + 1 < nCount)
		{
			nNextItemDepth = 0;
			
			CComBSTR bstrNextItemText;
			pMenu->GetItemText(nIndex + 2, &bstrNextItemText);
			
			LPCWSTR p = bstrNextItemText;
			while (*p++ == '.')
				nNextItemDepth++;
		}
		
		if (nNextItemDepth > nItemDepth)
		{
			HMENU hNewMenu = CreatePopupMenu();
			ATLASSERT(hNewMenu != NULL);
			
			AddMenuItems(hNewMenu, pMenu, ++nIndex, nCount, nDepth + 1);
			AppendMenu(hMenu, MF_POPUP, (UINT)hNewMenu, OLE2T(bstrItemText + nItemDepth));
		}
		else
		{
			CString strItemText = bstrItemText + nItemDepth;

			if (bstrItemText == L"-")
			{
				AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
			}
			else
			{
				long lShift = 0;
				pMenu->GetItemAccelShift(nIndex + 1, &lShift);
				long lKeyCode = 0;
				pMenu->GetItemAccelKeyCode(nIndex + 1, &lKeyCode);
				
				ACCEL accel;
				accel.fVirt = FVIRTKEY;
				if ((lShift & 1) != 0)
					accel.fVirt |= FSHIFT;
				if ((lShift & 2) != 0)
					accel.fVirt |= FCONTROL;
				if ((lShift & 4) != 0)
					accel.fVirt |= FALT;
				accel.key = (USHORT)lKeyCode;
				accel.cmd = nIndex + 1;
				m_arrAccels.Add(accel);
				
				CString strAccel = GetAccelKeyName(lShift, lKeyCode);
				if (!strAccel.IsEmpty())
				{
					strItemText += _T("\t");
					strItemText += strAccel;
				}
				
				AppendMenu(hMenu, MF_STRING | MF_BYCOMMAND, nIndex + 1, strItemText);
			}
		}
		
		nIndex++;
	}
}


BOOL CPopupFrame::PreTranslateMessage(MSG* pMsg) 
{
//	if (CFrameWnd::PreTranslateMessage(pMsg))
//		return TRUE;

	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if (m_hAccelTable != NULL &&
			::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

UINT CPopupFrame::_GetThemeChangedMsg()
{
#ifndef WM_THEMECHANGED
	static const UINT WM_THEMECHANGED = 0x031A;
#endif //!WM_THEMECHANGED
	return WM_THEMECHANGED;
}

void CPopupFrame::OpenThemeData()
{
	ATLASSERT(m_hThemeDLL != NULL);

	PFN_OpenThemeData pfnOpenThemeData = (PFN_OpenThemeData)::GetProcAddress(m_hThemeDLL, "OpenThemeData");
	ATLASSERT(pfnOpenThemeData != NULL);
	if(pfnOpenThemeData != NULL)
		m_hTheme = pfnOpenThemeData(m_hWnd, L"Window");
}

void CPopupFrame::CloseThemeData()
{
	ATLASSERT(m_hThemeDLL != NULL);

	if (m_hTheme == NULL)
		return;   // nothing to do

	PFN_CloseThemeData pfnCloseThemeData = (PFN_CloseThemeData)::GetProcAddress(m_hThemeDLL, "CloseThemeData");
	ATLASSERT(pfnCloseThemeData != NULL);
	if(pfnCloseThemeData != NULL)
	{
		pfnCloseThemeData(m_hTheme);
		m_hTheme = NULL;
	}
}

int CPopupFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_hThemeDLL = ::LoadLibrary(_T("uxtheme.dll"));
	if (m_hThemeDLL != NULL)
	{
		m_pfnDrawThemeBackground = (PFN_DrawThemeBackground)::GetProcAddress(m_hThemeDLL, "DrawThemeBackground");
		ASSERT(m_pfnDrawThemeBackground != NULL);
		if(m_pfnDrawThemeBackground != NULL)
		{
			OpenThemeData();
		}
		else
		{
			::FreeLibrary(m_hThemeDLL);
			m_hThemeDLL = NULL;
		}
	}

	GetSystemSettings();

	DWORD dwStyle = GetStyle();
	m_cxFrameWidth = (dwStyle & WS_THICKFRAME) ? GetSystemMetrics(SM_CXSIZEFRAME) : GetSystemMetrics(SM_CXFIXEDFRAME);
	m_cyFrameHeight = (dwStyle & WS_THICKFRAME) ? GetSystemMetrics(SM_CYSIZEFRAME) : GetSystemMetrics(SM_CYFIXEDFRAME);
	
	return 0;
}

void CPopupFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
	if (m_hThemeDLL != NULL)
	{
		CloseThemeData();
		::FreeLibrary(m_hThemeDLL);
		m_hThemeDLL = NULL;
	}
}

LRESULT CPopupFrame::OnThemeChanged(WPARAM, LPARAM)
{
	if (m_hThemeDLL != NULL)
	{
		CloseThemeData();
		OpenThemeData();
	}

	return 0;
}

void CPopupFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	GetSystemSettings();
}

void CPopupFrame::GetSystemSettings()
{
	NONCLIENTMETRICS info = { sizeof(NONCLIENTMETRICS) };
	BOOL bRet = ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
	ASSERT(bRet);
	if (bRet)
	{
		if (m_hTheme != NULL)
		{
			m_cxBtnWidth = info.iCaptionWidth - 2 * m_cxyOffset;
			m_cyBtnHeight = info.iCaptionHeight - 2 * m_cxyOffset;
		}
		else
		{
			m_cxBtnWidth = info.iCaptionWidth - m_cxyOffset;
			m_cyBtnHeight = info.iCaptionHeight - 2 * m_cxyOffset;
		}
	}

	RECT rect = { 0 };
	GetClientRect(&rect);
	AdjustBtnSize(rect.bottom);
}

void CPopupFrame::AdjustBtnSize(int cyHeight)
{
	if (cyHeight > 1 && m_cyBtnHeight > cyHeight)
	{
		if (m_hTheme != NULL)
		{
			m_cyBtnHeight = cyHeight;
			m_cxBtnWidth = cyHeight;
		}
		else
		{
			m_cyBtnHeight = cyHeight;
			m_cxBtnWidth = cyHeight + m_cxyOffset;
		}
	}
}

LRESULT CPopupFrame::OnCloseForm(WPARAM wParam, LPARAM lParam)
{
	((CFormDoc *)GetActiveDocument())->OnCloseForm();

	return 0;
}
