// ColorPicker2.cpp : Implementation of CColorPicker2
#include "stdafx.h"
#include "CxComObj.h"
#include "ColorPicker2.h"

/////////////////////////////////////////////////////////////////////////////
// CColorPicker2

CColorPicker2::ColorTableEntry CColorPicker2::gm_sColors[] =
{
    { RGB(0x00, 0x00, 0x00),    _T("ºÚÉ«")		},
    { RGB(0xA5, 0x2A, 0x00),    _T("ºÖÉ«")		},
    { RGB(0x00, 0x40, 0x40),    _T("éÏé­ÂÌ")	},
    { RGB(0x00, 0x55, 0x00),    _T("ÉîÂÌ")		},
    { RGB(0x00, 0x00, 0x5E),    _T("Éî»ÒÀ¶")	},
    { RGB(0x00, 0x00, 0x8B),    _T("ÉîÀ¶")		},
    { RGB(0x4B, 0x00, 0x82),    _T("µåÀ¶")		},
    { RGB(0x28, 0x28, 0x28),    _T("Éî»Ò")		},
	
    { RGB(0x8B, 0x00, 0x00),    _T("Éîºì")		},
    { RGB(0xFF, 0x68, 0x20),    _T("½Û»Æ")		},
    { RGB(0x8B, 0x8B, 0x00),    _T("Éî»Æ")		},
    { RGB(0x00, 0x93, 0x00),    _T("ÂÌÉ«")		},
    { RGB(0x38, 0x8E, 0x8E),    _T("ÇàÉ«")      },
    { RGB(0x00, 0x00, 0xFF),    _T("À¶É«")      },
    { RGB(0x7B, 0x7B, 0xC0),    _T("À¶-»Ò")		},
    { RGB(0x66, 0x66, 0x66),    _T("»ÒÉ« - 40") },
	
    { RGB(0xFF, 0x00, 0x00),    _T("ºìÉ«")      },
    { RGB(0xFF, 0xAD, 0x5B),    _T("Ç³½Û»Æ")	},
    { RGB(0x32, 0xCD, 0x32),    _T("Ëá³ÈÉ«")    }, 
    { RGB(0x3C, 0xB3, 0x71),    _T("º£ÂÌ")		},
    { RGB(0x7F, 0xFF, 0xD4),    _T("±¦Ê¯À¶")    },
    { RGB(0x7D, 0x9E, 0xC0),    _T("Ç³À¶")		},
    { RGB(0x80, 0x00, 0x80),    _T("×ÏÉ«")		},
    { RGB(0x7F, 0x7F, 0x7F),    _T("»ÒÉ« - 50") },
	
    { RGB(0xFF, 0xC0, 0xCB),    _T("·Ûºì")      },
    { RGB(0xFF, 0xD7, 0x00),    _T("½ðÉ«")      },
    { RGB(0xFF, 0xFF, 0x00),    _T("»ÆÉ«")		},    
    { RGB(0x00, 0xFF, 0x00),    _T("ÏÊÂÌ")		},
    { RGB(0x40, 0xE0, 0xD0),    _T("ÇàÂÌ")		},
    { RGB(0xC0, 0xFF, 0xFF),    _T("ÌìÀ¶")		},
    { RGB(0x48, 0x00, 0x48),    _T("Ã·ºì")      },
    { RGB(0xC0, 0xC0, 0xC0),    _T("Ç³»Ò")		},
	
    { RGB(0xFF, 0xE4, 0xE1),    _T("Ãµ¹åºì")    },
    { RGB(0xD2, 0xB4, 0x8C),    _T("×Ø»Æ")      },
    { RGB(0xFF, 0xFF, 0xE0),    _T("Ç³»Æ")		},
    { RGB(0x98, 0xFB, 0x98),    _T("Ç³ÂÌ")		},
    { RGB(0xAF, 0xEE, 0xEE),    _T("Ç³ÇàÂÌ")	},
    { RGB(0x68, 0x83, 0x8B),    _T("µ­À¶")		},
	{ RGB(0xE6, 0xE6, 0xFA),    _T("µ­×Ï")		},
    { RGB(0xFF, 0xFF, 0xFF),    _T("°×É«")		}
};


#define DEFAULT_BOX_VALUE	-3
#define CUSTOM_BOX_VALUE	-2
#define INVALID_COLOR		-1
#define MAX_COLORS			100

static const CSize s_sizeTextHiBorder(3, 3);
static const CSize s_sizeTextMargin(2, 2);
static const CSize s_sizeBoxHiBorder(2, 2);
static const CSize s_sizeBoxMargin(0, 0);
static const CSize s_sizeBoxCore(14, 14);

STDMETHODIMP CColorPicker2::Pick(long hParent, RECT rect, OLE_COLOR clrDefault, OLE_COLOR clr, OLE_COLOR* pclr)
{
	BOOL bOked = FALSE;

	OleTranslateColor(clrDefault, NULL, &m_clrDefault);
	OleTranslateColor(clr, NULL, &m_clrCurrent);
	
	// See what version we are using
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	::GetVersionEx(&osvi);
	bool bIsXP = osvi.dwPlatformId == VER_PLATFORM_WIN32_NT &&
		(osvi.dwMajorVersion > 5 || (osvi.dwMajorVersion == 5 &&
		osvi.dwMinorVersion >= 1));

	// Get the flat flag
	m_bFlat = FALSE;
#if (_WIN32_WINNT >= 0x0501)
 	if (bIsXP)
		::SystemParametersInfo (SPI_GETFLATMENU, 0, &m_bFlat, FALSE);
#endif


	// Get all the colors I need
	int nAlpha = 48;
	m_clrBackground = ::GetSysColor (COLOR_MENU);
	m_clrHiLightBorder = ::GetSysColor (COLOR_HIGHLIGHT);
	m_clrHiLight = m_clrHiLightBorder;
#if (WINVER >= 0x0501)
	if (fIsXP)
		m_clrHiLight = ::GetSysColor (COLOR_MENUHILIGHT);
#endif
	m_clrHiLightText = ::GetSysColor (COLOR_HIGHLIGHTTEXT);
	m_clrText = ::GetSysColor (COLOR_MENUTEXT);
	m_clrLoLight = RGB (
		(GetRValue(m_clrBackground) * (255 - nAlpha) + 
			GetRValue(m_clrHiLightBorder) * nAlpha) >> 8,
		(GetGValue(m_clrBackground) * (255 - nAlpha) + 
			GetGValue(m_clrHiLightBorder) * nAlpha) >> 8,
		(GetBValue(m_clrBackground) * (255 - nAlpha) + 
			GetBValue(m_clrHiLightBorder) * nAlpha) >> 8);
   
	// Get the margins
	m_rectMargins.left = ::GetSystemMetrics(SM_CXEDGE);
	m_rectMargins.top = ::GetSystemMetrics(SM_CYEDGE);
	m_rectMargins.right = ::GetSystemMetrics(SM_CXEDGE);
	m_rectMargins.bottom = ::GetSystemMetrics(SM_CYEDGE);

	// Initialize some sizing parameters
	m_nNumColors = sizeof(gm_sColors) / sizeof(ColorTableEntry);
	_ASSERTE(m_nNumColors <= MAX_COLORS);
	if (m_nNumColors > MAX_COLORS)
		m_nNumColors = MAX_COLORS;

	// Initialize our state
	m_nCurrentSel = INVALID_COLOR;
	m_nChosenColorSel = INVALID_COLOR;
	m_clrPicker	= m_clrCurrent;

	// Create the font
	NONCLIENTMETRICS ncm;
	ncm .cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS), &ncm, 0);
	m_font.CreateFontIndirect(&ncm.lfMessageFont);

	// Create the palette
	struct 
	{
		LOGPALETTE LogPalette;
		PALETTEENTRY PalEntry[MAX_COLORS];
	} pal;

	LOGPALETTE *pLogPalette = (LOGPALETTE *)&pal;
	pLogPalette->palVersion = 0x300;
	pLogPalette->palNumEntries = (WORD)m_nNumColors; 

	for (int i = 0; i < m_nNumColors; i++)
	{
		pLogPalette ->palPalEntry [i] .peRed   = GetRValue (gm_sColors [i] .clrColor);
		pLogPalette ->palPalEntry [i] .peGreen = GetGValue (gm_sColors [i] .clrColor);
		pLogPalette ->palPalEntry [i] .peBlue  = GetBValue (gm_sColors [i] .clrColor);
		pLogPalette ->palPalEntry [i] .peFlags = 0;
	}
	m_palette.CreatePalette(pLogPalette);

//	// Register the window class used for the picker
//	WNDCLASSEX wc;
//	wc.cbSize = sizeof(WNDCLASSEX);
//	wc.style  = CS_CLASSDC | CS_SAVEBITS | CS_HREDRAW | CS_VREDRAW;
//	wc.lpfnWndProc = StartWindowProc;
//	wc.cbClsExtra  = 0;
//	wc.cbWndExtra = 0;
//	wc.hInstance = _Module.GetModuleInstance();
//	wc.hIcon = NULL;
//	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
//	wc.hbrBackground = (HBRUSH)(COLOR_MENU + 1);
//	wc.lpszMenuName = NULL;
//	wc.lpszClassName = _T ("ColorPicker");
//	wc.hIconSm = NULL;
//#if (_WIN32_WINNT >= 0x0501)
//	if (bIsXP)
//	{
//		BOOL bDropShadow;
//		::SystemParametersInfo(SPI_GETDROPSHADOW, 0, &bDropShadow, FALSE);
//		if (bDropShadow)
//			wc.style |= CS_DROPSHADOW;
//	}
//#endif
//	ATOM atom = ::RegisterClassEx(&wc);
//
//	// Create the window
//	_Module.AddCreateWndData(&m_thunk.cd, this);
//	HWND hWnd = ::CreateWindowEx(0, (LPCTSTR)MAKELONG(atom, 0), 
//		_T(""),  WS_POPUP, X, Y, 100, 100,
//		(HWND)hParent, NULL, _Module.GetModuleInstance(), NULL);
//	ATLASSERT(m_hWnd == hWnd);

	CRect rectWindow(rect.left, rect.bottom, rect.left + 100, rect.bottom + 100);
	Create((HWND)hParent, rectWindow, NULL, WS_POPUP, WS_EX_TOPMOST);

	// If we created the window
	if (m_hWnd != NULL)
	{
    	// Set the window size
	    SetWindowSize(rect);

		// Create the tooltips
		CToolTipCtrl sToolTip;
		CreateToolTips(sToolTip);

		// Find which cell (if any) corresponds to the initial color
		FindCellFromColor(m_clrCurrent);

		// Make visible
		ShowWindow(SW_SHOWNA);
// 		SetActiveWindow();

		// Purge the message queue of paints
		MSG msg;
		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
				return FALSE;
			DispatchMessage(&msg);
		}
		
		// Set capture to the window which received this message
		SetCapture();
		_ASSERTE(m_hWnd == ::GetCapture());

//		while (::PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE|PM_NOYIELD));

		BOOL bHasDown = FALSE;
		// Get messages until capture lost or cancelled/accepted
		while (m_hWnd == ::GetCapture())
		{
			MSG msg;
			if (!::GetMessage(&msg, NULL, 0, 0))
			{
				::PostQuitMessage(msg .wParam);
				break;
			}

			sToolTip.RelayEvent(&msg);

			switch (msg.message)
			{
				case WM_LBUTTONDOWN:
					{
						BOOL bHandled = TRUE;
						OnLButtonUp(msg.message, 
							msg.wParam, msg.lParam, bHandled);
					
					//	bHasDown = TRUE;	
					}
					break;
				case WM_LBUTTONUP:
					{
						CPoint pt(GET_X_LPARAM (msg.wParam), GET_Y_LPARAM (msg.lParam));
						CRect rect;
						GetClientRect(&rect);
						if (rect.PtInRect(pt))
						{
							BOOL bHandled = TRUE;
							OnLButtonUp(msg.message, 
								msg.wParam, msg.lParam, bHandled);
						}

					}
					break;

				case WM_MOUSEMOVE:
					{
						BOOL bHandled = TRUE;
						OnMouseMove(msg .message, 
							msg.wParam, msg.lParam, bHandled);
					}
					break;

				case WM_KEYUP:
					break;

				case WM_KEYDOWN:
					{
						BOOL bHandled = TRUE;
						OnKeyDown(msg.message, 
							msg.wParam, msg.lParam, bHandled);
					}
					break;

				case WM_RBUTTONDOWN:
					::ReleaseCapture();
					m_bOked = FALSE;
					break;

				// just dispatch rest of the messages
				default:
					DispatchMessage(&msg);
					break;
			}
		}
		::ReleaseCapture();
		bOked = m_bOked;

		// Destroy the window
		sToolTip.DestroyWindow();
		DestroyWindow();

		// If needed, show custom
		if (bOked)
		{
			if (bOked && m_nCurrentSel == CUSTOM_BOX_VALUE)
			{
				CColorDialog dlg(m_clrCurrent, 
					CC_FULLOPEN | CC_ANYCOLOR, m_hWnd);

				if (dlg.DoModal() == IDOK)
					m_clrCurrent = dlg.GetColor();
				else
					bOked = FALSE;
			}
			else
				m_clrCurrent = m_clrPicker;
		}
		
		// Clean up GDI objects
		m_font.DeleteObject();
		m_palette.DeleteObject();
	}

//	// Unregister our class
//	::UnregisterClass((LPCTSTR)MAKELONG(atom, 0),
//		_Module.GetModuleInstance());

	*pclr = m_clrCurrent;

	return (bOked ? S_OK : S_FALSE);
}

void CColorPicker2::SetWindowSize(RECT rectParent)
{
	SIZE szText = {0, 0};

    // If we are showing a custom or default text area, get the font and text size.
    if (HasCustomText() || HasDefaultText())
    {
		CClientDC dc(m_hWnd);
		HFONT hfontOld = dc.SelectFont (m_font);

		// Get the size of the custom text (if there IS custom text)
		if (HasCustomText())
		{
			dc .GetTextExtent(m_pszCustomText, 
				_tcslen(m_pszCustomText), &szText);
		}

	    // Get the size of the default text (if there IS default text)
        if (HasDefaultText())
        {
			SIZE szDefault;
			dc .GetTextExtent (m_pszDefaultText, 
				_tcslen (m_pszDefaultText), &szDefault);
			if (szDefault .cx > szText .cx)
				szText .cx = szDefault .cx;
			if (szDefault .cy > szText .cy)
				szText .cy = szDefault .cy;
        }
		dc.SelectFont(hfontOld);

		// Commpute the final size
		szText .cx += 2 * (s_sizeTextMargin .cx + s_sizeTextHiBorder .cx);
		szText .cy += 2 * (s_sizeTextMargin .cy + s_sizeTextHiBorder .cy);
    }

	// Initiailize our box size
	_ASSERTE (s_sizeBoxHiBorder .cx == s_sizeBoxHiBorder .cy);
	_ASSERTE (s_sizeBoxMargin .cx == s_sizeBoxMargin .cy);
	m_sizeBox .cx = s_sizeBoxCore .cx + (s_sizeBoxHiBorder .cx + s_sizeBoxMargin .cx) * 2;
	m_sizeBox .cy = s_sizeBoxCore .cy + (s_sizeBoxHiBorder .cy + s_sizeBoxMargin .cy) * 2;

    // Get the number of columns and rows
    m_nNumColumns = 8;
    m_nNumRows = m_nNumColors / m_nNumColumns;
    if ((m_nNumColors % m_nNumColumns) != 0) 
		m_nNumRows++;

	// Compute the min width
	int nBoxTotalWidth = m_nNumColumns * m_sizeBox .cx;
	int nMinWidth = nBoxTotalWidth;
	if (nMinWidth < szText .cx)
		nMinWidth = szText .cx;

	// Create the rectangle for the default text
	m_rectDefaultText = CRect(
		CPoint (0, 0), 
		CSize (nMinWidth, HasDefaultText () ? szText .cy : 0)
		);
		
	// Initialize the color box rectangle
	m_rectBoxes = CRect(
		CPoint ((nMinWidth - nBoxTotalWidth) / 2, m_rectDefaultText .bottom), 
		CSize (nBoxTotalWidth, m_nNumRows * m_sizeBox .cy)
		);

	// Create the rectangle for the custom text
 	m_rectCustomText = CRect(
		CPoint (0, m_rectBoxes .bottom), 
		CSize (nMinWidth, HasCustomText () ? szText .cy : 0)
		);

    // Get the current window position, and set the new size
	CRect rectWindow(
		m_rectDefaultText.TopLeft(), 
		m_rectCustomText.BottomRight());
    CRect rect;
 	GetWindowRect(&rect);
 	rectWindow.OffsetRect(rect.TopLeft());
 

	// Adjust the rects for the border
	rectWindow.right += m_rectMargins .left + m_rectMargins .right;
	rectWindow.bottom += m_rectMargins .top + m_rectMargins .bottom;
	::OffsetRect(&m_rectDefaultText, m_rectMargins .left, m_rectMargins .top);
	::OffsetRect(&m_rectBoxes, m_rectMargins .left, m_rectMargins .top);
	::OffsetRect(&m_rectCustomText, m_rectMargins .left, m_rectMargins .top);

	// Get the screen rectangle
	CRect rectScreen(CPoint(0, 0), CSize(
		::GetSystemMetrics(SM_CXSCREEN),
        ::GetSystemMetrics(SM_CYSCREEN)));
#if (WINVER >= 0x0500)
	HMODULE hUser32 = ::GetModuleHandleA ("USER32.DLL");
	if (hUser32 != NULL)
	{
		typedef HMONITOR (WINAPI *FN_MonitorFromWindow) (HWND hWnd, DWORD dwFlags);
		typedef BOOL (WINAPI *FN_GetMonitorInfo) (HMONITOR hMonitor, LPMONITORINFO lpmi);
		FN_MonitorFromWindow pfnMonitorFromWindow = (FN_MonitorFromWindow)
			::GetProcAddress (hUser32, "MonitorFromWindow");
		FN_GetMonitorInfo pfnGetMonitorInfo = (FN_GetMonitorInfo)
			::GetProcAddress (hUser32, "GetMonitorInfoA");
		if (pfnMonitorFromWindow != NULL && pfnGetMonitorInfo != NULL)
		{
			MONITORINFO mi;
			HMONITOR hMonitor = pfnMonitorFromWindow(m_hWnd, 
				MONITOR_DEFAULTTONEAREST);
			mi .cbSize = sizeof (mi);
			pfnGetMonitorInfo(hMonitor, &mi);
			rectScreen = mi.rcWork;
		}
	}
#endif

    // Need to check it'll fit on screen: Too far right?
    if (rectWindow.right > rectScreen.right)
		::OffsetRect(&rectWindow, rectScreen .right - rectWindow .right, 0);

    // Too far left?
    if (rectWindow.left < rectScreen.left)
        ::OffsetRect(&rectWindow, rectScreen.left - rectWindow .left, 0);

    // Bottom falling out of screen?  If so, the move
	// the whole popup above the parents window
    if (rectWindow.bottom > rectScreen .bottom)
    {
   //     CRect rcParent;
	//	GetWindowRect(&rcParent);
        ::OffsetRect(&rectWindow, 0,
			- ((rectParent.bottom - rectParent.top) + 
			(rectWindow.bottom - rectWindow.top)));
    }

    // Set the window size and position
	MoveWindow(&rectWindow, TRUE);
}

void CColorPicker2::CreateToolTips(CToolTipCtrl &sToolTip)
{
    // Create the tool tip
    if (!sToolTip.Create(m_hWnd)) 
		return;

    // Add a tool for each cell
    for (int i = 0; i < m_nNumColors; i++)
    {
        CRect rect;
        if (!GetCellRect(i, &rect)) 
			continue;
		sToolTip.AddTool(m_hWnd, 
			gm_sColors[i].pszName, &rect, 1);
    }
}

BOOL CColorPicker2::GetCellRect(int nIndex, RECT *pRect) const
{
	// If the custom box
    if (nIndex == CUSTOM_BOX_VALUE)
    {
		*pRect = m_rectCustomText;
		return TRUE;
    }

	// If the default box
    else if (nIndex == DEFAULT_BOX_VALUE)
    {
		*pRect = m_rectDefaultText;
		return TRUE;
    }

	// Validate the range
    if (nIndex < 0 || nIndex >= m_nNumColors)
        return FALSE;

	// Compute the value of the boxes
    pRect->left = (nIndex % m_nNumColumns) * m_sizeBox .cx + m_rectBoxes .left;
    pRect->top  = (nIndex / m_nNumColumns) * m_sizeBox .cy + m_rectBoxes .top;
    pRect->right = pRect ->left + m_sizeBox .cx;
    pRect->bottom = pRect ->top + m_sizeBox .cy;

    return TRUE;
}

void CColorPicker2::FindCellFromColor(COLORREF clr)
{
    if (clr == CLR_DEFAULT && HasDefaultText ())
    {
        m_nChosenColorSel = DEFAULT_BOX_VALUE;
        return;
    }

    for (int i = 0; i < m_nNumColors; i++)
    {
        if (gm_sColors [i] .clrColor == clr)
        {
            m_nChosenColorSel = i;
            return;
        }
    }

    if (HasCustomText ())
        m_nChosenColorSel = CUSTOM_BOX_VALUE;
    else
        m_nChosenColorSel = INVALID_COLOR;
}

void CColorPicker2::ChangeSelection(int nIndex)
{
	CClientDC dc(m_hWnd);

	// Clamp the index
    if (nIndex > m_nNumColors)
        nIndex = CUSTOM_BOX_VALUE; 

	// If the current selection is valid, redraw old selection with out
	// it being selected
    if ((m_nCurrentSel >= 0 && m_nCurrentSel < m_nNumColors) ||
        m_nCurrentSel == CUSTOM_BOX_VALUE || m_nCurrentSel == DEFAULT_BOX_VALUE)
    {
        int nOldSel = m_nCurrentSel;
        m_nCurrentSel = INVALID_COLOR;
        DrawCell (dc, nOldSel);
    }

    // Set the current selection as row/col and draw (it will be drawn selected)
    m_nCurrentSel = nIndex;
    DrawCell(dc, m_nCurrentSel);

    // Store the current colour
	BOOL fValid = TRUE;
	COLORREF clr;
    if (m_nCurrentSel == CUSTOM_BOX_VALUE)
		clr = m_clrDefault;
    else if (m_nCurrentSel == DEFAULT_BOX_VALUE)
        clr = m_clrPicker = m_clrDefault;
    else if (m_nCurrentSel == INVALID_COLOR)
	{
		clr = RGB (0, 0, 0);
		fValid = FALSE;
	}
	else
        clr = m_clrPicker = gm_sColors [m_nCurrentSel] .clrColor;

	// Send the message
	if (m_bTrackSelection)
	{
		if (fValid)
			m_clrCurrent = clr;
		InvalidateRect (NULL);
//		SendNotification(CPN_SELCHANGE, m_clrCurrent, fValid); 
	}
}

void CColorPicker2::EndSelection(BOOL fOked)
{
    ::ReleaseCapture ();
	m_bOked = fOked;
}

void CColorPicker2::DrawCell(CDC &dc, int nIndex)
{
	// Get the drawing rect
	CRect rect;
	if (!GetCellRect (nIndex, &rect)) 
		return;

	// Get the text pointer and colors
	LPCTSTR pszText;
	COLORREF clrBox;
	SIZE sizeMargin;
	SIZE sizeHiBorder;
	if (nIndex == CUSTOM_BOX_VALUE)
	{
		pszText = m_pszCustomText;
		sizeMargin = s_sizeTextMargin;
		sizeHiBorder = s_sizeTextHiBorder;
	}
	else if (nIndex == DEFAULT_BOX_VALUE)
	{
		pszText = m_pszDefaultText;
		sizeMargin = s_sizeTextMargin;
		sizeHiBorder = s_sizeTextHiBorder;
	}
	else
	{
		pszText = NULL;
		clrBox = gm_sColors [nIndex] .clrColor;
		sizeMargin = s_sizeBoxMargin;
		sizeHiBorder = s_sizeBoxHiBorder;
	}

	// Based on the selectons, get our colors
	COLORREF clrHiLight;
	COLORREF clrText;
	bool fSelected;
	if (m_nCurrentSel == nIndex)
	{
		fSelected = true;
		clrHiLight = m_clrHiLight;
		clrText = m_clrHiLightText;
	}
	else if (m_nChosenColorSel == nIndex)
	{
		fSelected = true;
		clrHiLight = m_clrLoLight;
		clrText = m_clrText;
	}
	else
	{
		fSelected = false;
		clrHiLight = m_clrLoLight;
		clrText = m_clrText;
	}

	// Select and realize the palette
	HPALETTE hpalOld = NULL;
	if (pszText == NULL)
	{
		if (m_palette .m_hPalette != NULL && 
			(dc .GetDeviceCaps (RASTERCAPS) & RC_PALETTE) != 0)
		{
			hpalOld = dc .SelectPalette (m_palette, FALSE);
			dc .RealizePalette ();
		}
	}

	// If we are currently selected
	if (fSelected)
	{
		// If we have a background margin, then draw that
		if (sizeMargin .cx > 0 || sizeMargin .cy > 0)
		{
			dc.SetBkColor(m_clrBackground);
			dc.ExtTextOut(0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
			rect.InflateRect(- sizeMargin .cx, - sizeMargin .cy);
		}

		// Draw the selection rectagle
		dc.SetBkColor(m_clrHiLightBorder);
		dc.ExtTextOut(0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
		rect.InflateRect(-1, -1);

		// Draw the inner coloring
		dc.SetBkColor(clrHiLight);
		dc.ExtTextOut(0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
		rect.InflateRect(-(sizeHiBorder .cx - 1), - (sizeHiBorder .cy - 1));
	}

	//
	// Otherwise, we are not selected
	//

	else
	{
		
		//
		// Draw the background
		//

		dc.SetBkColor(m_clrBackground);
		dc.ExtTextOut(0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
		rect .InflateRect( 
			- (sizeMargin .cx + sizeHiBorder .cx), 
			- (sizeMargin .cy + sizeHiBorder .cy));
	}

	//
	// Draw custom text
	//

	if (pszText)
	{
		HFONT hfontOld = dc.SelectFont(m_font);
		dc.SetTextColor(clrText);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(pszText, _tcslen (pszText), 
			&rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		dc.SelectFont(hfontOld);

		
		if (nIndex == DEFAULT_BOX_VALUE)
		{
			rect.right = 24;
			rect.top += 1;
			rect.InflateRect(-3, 0);
			rect.InflateRect(1, 1);
			dc.SetBkColor(::GetSysColor (COLOR_3DSHADOW));
			dc.ExtTextOut(0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
			rect.InflateRect(-1, -1);
			dc.SetBkColor(m_clrDefault);
			dc.ExtTextOut(0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
		}
    }        

	//
	// Otherwise, draw color
	//

	else
	{

		//
		// Draw color (ok, this code is bit sleeeeeezy.  But the
		// area's that are being drawn are SO small, that nobody
		// will notice.)
		//

		dc .SetBkColor (::GetSysColor (COLOR_3DSHADOW));
		dc .ExtTextOut (0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
		rect .InflateRect (-1, -1);
		dc .SetBkColor (gm_sColors [nIndex] .clrColor);
		dc .ExtTextOut (0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
	}

	//
	// Restore the pallete
	//

	if (hpalOld && (dc.GetDeviceCaps (RASTERCAPS) & RC_PALETTE) != 0)
		dc.SelectPalette (hpalOld, FALSE);
}

LRESULT CColorPicker2::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) 
{
	UINT nChar = wParam;

	int nOffset = 0;
	switch (nChar)
	{
		case VK_DOWN:
			nOffset = m_nNumColumns;
			break;

		case VK_UP:
			nOffset = -m_nNumColumns;
			break;

		case VK_RIGHT:
			nOffset = 1;
			break;

		case VK_LEFT:
			nOffset = -1;
			break;

		case VK_ESCAPE:
			m_clrPicker = m_clrCurrent;
			EndSelection(FALSE);
			break;

		case VK_RETURN:
		case VK_SPACE:
			if (m_nCurrentSel == INVALID_COLOR)
				m_clrPicker = m_clrCurrent;
	        EndSelection (m_nCurrentSel != INVALID_COLOR);
			break;
	}

	if (nOffset != 0)
	{

		int nNewSel;
		if (m_nCurrentSel == INVALID_COLOR)
			nNewSel = nOffset > 0 ? DEFAULT_BOX_VALUE : CUSTOM_BOX_VALUE;
		else if (m_nCurrentSel == DEFAULT_BOX_VALUE)
			nNewSel = nOffset > 0 ? 0 : CUSTOM_BOX_VALUE;
		else if (m_nCurrentSel == CUSTOM_BOX_VALUE)
			nNewSel = nOffset > 0 ? DEFAULT_BOX_VALUE : m_nNumColors - 1;
		else
		{
			nNewSel = m_nCurrentSel + nOffset;
			if (nNewSel < 0)
				nNewSel = DEFAULT_BOX_VALUE;
			else if (nNewSel >= m_nNumColors)
				nNewSel = CUSTOM_BOX_VALUE;
		}

		while (true)
		{
			if (nNewSel == DEFAULT_BOX_VALUE && !HasDefaultText())
				nNewSel = nOffset > 0 ? 0 : CUSTOM_BOX_VALUE;
			else if (nNewSel == CUSTOM_BOX_VALUE && !HasCustomText())
				nNewSel = nOffset > 0 ? DEFAULT_BOX_VALUE : m_nNumColors - 1;
			else
				break;
		}

        ChangeSelection(nNewSel);
	}
	bHandled = FALSE;
	
	return FALSE;
}

LRESULT CColorPicker2::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) 
{
	// Where did the button come up at?
	CPoint pt(GET_X_LPARAM (lParam), GET_Y_LPARAM (lParam));
    int nNewSelection = HitTest(pt);

	// If valid, then change selection and end
	if (nNewSelection != m_nCurrentSel)
		ChangeSelection(nNewSelection);
	EndSelection(nNewSelection != INVALID_COLOR);
	
	return 0;
}

LRESULT CColorPicker2::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) 
{
	// Do a hit test
	CPoint pt(GET_X_LPARAM (lParam), GET_Y_LPARAM (lParam));
    int nNewSelection = HitTest (pt);
	
	// OK - we have the row and column of the current selection 
	// (may be CUSTOM_BOX_VALUE) Has the row/col selection changed? 
	// If yes, then redraw old and new cells.
    if (nNewSelection != m_nCurrentSel)
        ChangeSelection(nNewSelection);
	
	return 0;
}

LRESULT CColorPicker2::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) 
{
	CPaintDC dc(m_hWnd);
	
	// Draw raised window edge (ex-window style WS_EX_WINDOWEDGE is sposed to do this,
    // but for some reason isn't
	CRect rect;
	GetClientRect(&rect);
	if (m_bFlat)
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 0, ::GetSysColor (COLOR_GRAYTEXT));
		HPEN hpenOld = dc .SelectPen (pen);
		dc.Rectangle(rect .left, rect .top, 
			rect.Width(), rect .Height ());
		dc.SelectPen(hpenOld);
	}
	else
	{
		dc.DrawEdge (&rect, EDGE_RAISED, BF_RECT);
	}
	
    // Draw the Default Area text
    if (HasDefaultText ())
        DrawCell(dc, DEFAULT_BOX_VALUE);
	
    // Draw colour cells
    for (int i = 0; i < m_nNumColors; i++)
        DrawCell(dc, i);
    
    // Draw custom text
    if (HasCustomText())
        DrawCell(dc, CUSTOM_BOX_VALUE);

	return 0;
}

LRESULT CColorPicker2::OnQueryNewPalette(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) 
{
    Invalidate();
    return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CColorPicker2::OnPaletteChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) 
{
    LRESULT lResult = DefWindowProc(uMsg, wParam, lParam);
	Invalidate();

	return lResult;
}

int CColorPicker2::HitTest (const POINT &pt)
{
	// If we are in the custom text
    if (m_rectCustomText.PtInRect(pt))
        return CUSTOM_BOX_VALUE;
	
	// If we are in the default text
    if (m_rectDefaultText.PtInRect(pt))
        return DEFAULT_BOX_VALUE;
	
	// If the point isn't in the boxes, return invalid color
	if (!m_rectBoxes.PtInRect(pt))
		return INVALID_COLOR;
	
	// Convert the point to an index
	int nRow = (pt.y - m_rectBoxes.top) / m_sizeBox.cy;
	int nCol = (pt.x - m_rectBoxes.left) / m_sizeBox.cx;
	if (nRow < 0 || nRow >= m_nNumRows || nCol < 0 || nCol >= m_nNumColumns)
		return INVALID_COLOR;
	int nIndex = nRow * m_nNumColumns + nCol;
	if (nIndex >= m_nNumColors)
		return INVALID_COLOR;
	
	return nIndex;
}