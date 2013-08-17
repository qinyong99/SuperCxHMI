// KeyBoardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cxrunctrl.h"
#include "KeyBoardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardDlg dialog

// Virtual key code
typedef struct _KEYCODE
{
	UINT ID;
	BYTE VK;
} KEYCODE;

KEYCODE keycode[] =
{
	{IDC_KEY_BACK,		0x08},
	{IDC_KEY_TAB,		0x09},

	{IDC_KEY_ENTER,		0x0D},

	{IDC_KEY_SHIFT,		0x10},
	{IDC_KEY_CONTROL,	0x11},
	{IDC_KEY_MENU,		0x12},
	{IDC_KEY_PAUSE,		0x13},
	{IDC_KEY_CAPS,		0x14},

	{IDC_KEY_ESC,		0x1B},

	{IDC_KEY_SPACE,		0x20},
	{IDC_KEY_PAGEUP,	0x21},
	{IDC_KEY_PAGEDOWN,	0x22},
	{IDC_KEY_END,		0x23},
	{IDC_KEY_HOME,		0x24},
	{IDC_KEY_LEFT,		0x25},
	{IDC_KEY_UP,		0x26},
	{IDC_KEY_RIGHT,		0x27},
	{IDC_KEY_DOWN,		0x28},

	{IDC_KEY_INS,		0x2D},
	{IDC_KEY_DEL,		0x2E},

	{IDC_KEY_0,			0x30},
	{IDC_KEY_1,			0x31},
	{IDC_KEY_2,			0x32},
	{IDC_KEY_3,			0x33},
	{IDC_KEY_4,			0x34},
	{IDC_KEY_5,			0x35},
	{IDC_KEY_6,			0x36},
	{IDC_KEY_7,			0x37},
	{IDC_KEY_8,			0x38},
	{IDC_KEY_9,			0x39},
	
	{IDC_KEY_A,			0x41},
	{IDC_KEY_B,			0x42},
	{IDC_KEY_C,			0x43},
	{IDC_KEY_D,			0x44},
	{IDC_KEY_E,			0x45},
	{IDC_KEY_F,			0x46},
	{IDC_KEY_G,			0x47},
	{IDC_KEY_H,			0x48},
	{IDC_KEY_I,			0x49},
	{IDC_KEY_J,			0x4A},
	{IDC_KEY_K,			0x4B},
	{IDC_KEY_L,			0x4C},
	{IDC_KEY_M,			0x4D},
	{IDC_KEY_N,			0x4E},
	{IDC_KEY_O,			0x4F},
	{IDC_KEY_P,			0x50},
	{IDC_KEY_Q,			0x51},
	{IDC_KEY_R,			0x52},
	{IDC_KEY_S,			0x53},
	{IDC_KEY_T,			0x54},
	{IDC_KEY_U,			0x55},
	{IDC_KEY_V,			0x56},
	{IDC_KEY_W,			0x57},
	{IDC_KEY_X,			0x58},
	{IDC_KEY_Y,			0x59},
	{IDC_KEY_Z,			0x5A},
	{IDC_KEY_WIN,		0x5B},
	{IDC_KEY_APP,		0x5D},

	{IDC_KEY_F1,		0x70},
	{IDC_KEY_F2,		0x71},
	{IDC_KEY_F3,		0x72},
	{IDC_KEY_F4,		0x73},
	{IDC_KEY_F5,		0x74},
	{IDC_KEY_F6,		0x75},
	{IDC_KEY_F7,		0x76},
	{IDC_KEY_F8,		0x77},
	{IDC_KEY_F9,		0x78},
	{IDC_KEY_F10,		0x79},
	{IDC_KEY_F11,		0x7A},
	{IDC_KEY_F12,		0x7B},

	{IDC_KEY_COLON,		0xBA},
	{IDC_KEY_PLUS,		0xBB},
	{IDC_KEY_COMMA,		0xBC},
	{IDC_KEY_PERIOD,	0xBE},
	{IDC_KEY_MINUS,		0xBD},
	{IDC_KEY_SLASH,		0xBF},
	{IDC_KEY_WAVE,		0xC0},

	{IDC_KEY_BRA,		0xDB},
	{IDC_KEY_SEPARATOR,	0xDC},
	{IDC_KEY_KET,		0xDD},
	{IDC_KEY_QUOTE,		0xDE},
};

CKeyBoardDlg* CKeyBoardDlg::c_pKeyBoardDlg = NULL;
HWND CKeyBoardDlg::m_hwndFocusWindow = NULL;
HANDLE CKeyBoardDlg::m_hEvent = NULL;

HHOOK CKeyBoardDlg::m_hHookMouse = NULL;
WNDPROC CKeyBoardDlg::m_pfnOldWndProc = NULL;

BOOL CKeyBoardDlg::m_bShift = FALSE;
BOOL CKeyBoardDlg::m_bControl = FALSE;
BOOL CKeyBoardDlg::m_bMenu = FALSE;

BOOL CKeyBoardDlg::m_bOnlyNumber = FALSE;
BOOL CKeyBoardDlg::m_bDefaultPos = TRUE;
int CKeyBoardDlg::m_nXPos = -1;
int CKeyBoardDlg::m_nYPos = -1;

CKeyBoardDlg::CKeyBoardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyBoardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyBoardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	
}

CKeyBoardDlg::~CKeyBoardDlg()
{
	// Clear the static window pointer.
	ASSERT(c_pKeyBoardDlg == this);
	c_pKeyBoardDlg = NULL;
}

void CKeyBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyBoardDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyBoardDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyBoardDlg)
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(1, 65535, OnKeyCommand)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardDlg message handlers

void CKeyBoardDlg::ShowSoftKeyBoard(CWnd* pParentWnd, BOOL bOnlyNumber, BOOL bDefaultPos, int nXPos, int nYPos)
{
	if (c_pKeyBoardDlg != NULL)
	{
		if (m_bOnlyNumber != bOnlyNumber)
		{
			HideSoftKeyBoard();
		}
		else 
		{
			if (!bDefaultPos)
			{
				::SetWindowPos(c_pKeyBoardDlg->m_hWnd, NULL,
					nXPos, nYPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			}

			return;
		}
	}

	m_bOnlyNumber = bOnlyNumber;
	m_bDefaultPos = bDefaultPos;
	m_nXPos = nXPos;
	m_nYPos = nYPos;

	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	c_pKeyBoardDlg = new CKeyBoardDlg;
	c_pKeyBoardDlg->m_hThread = CreateThread(NULL, 0, _MessageLoopProc, (void*)c_pKeyBoardDlg, 0, &c_pKeyBoardDlg->m_dwThreadID);

	WaitForSingleObject(m_hEvent, INFINITE);
	CloseHandle(m_hEvent);
	m_hEvent = NULL;
}

void CKeyBoardDlg::HideSoftKeyBoard()
{
	if (c_pKeyBoardDlg != NULL)
	{
		HANDLE hThread = c_pKeyBoardDlg->m_hThread;

		::PostThreadMessage(c_pKeyBoardDlg->m_dwThreadID, WM_QUIT, 0, 0L);
		::WaitForSingleObject(hThread, INFINITE);
	}
}

void CKeyBoardDlg::PostNcDestroy()
{
	// Free the C++ class.
	delete this;
}

void CKeyBoardDlg::OnKeyCommand(UINT nID)
{
	for (int i = 0; i < sizeof(keycode)/sizeof(KEYCODE); i++)
	{
		UINT id = keycode[i].ID;
		BYTE vk = keycode[i].VK;
		if (nID == id)
		{
			switch (keycode[i].ID)
			{
			case IDC_KEY_SHIFT:
				m_bShift = !m_bShift;
				break;
			case IDC_KEY_CONTROL:
				m_bControl = !m_bControl;
				break;
			case IDC_KEY_MENU:
				m_bMenu = !m_bMenu;
				break;
			default:
				if (m_bShift)
					keybd_event(VK_SHIFT, 0, 0, 0);
				if (m_bControl)
					keybd_event(VK_CONTROL, 0, 0, 0);
				if (m_bMenu)
					keybd_event(VK_MENU, 0, 0, 0);
				// Key stroke
				keybd_event(vk, 0, 0, 0);
				keybd_event(vk, 0, KEYEVENTF_KEYUP, 0);

				if (m_bShift)
				{
					keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
					m_bShift = false;
					::InvalidateRect(::GetDlgItem(m_hWnd, IDC_KEY_SHIFT), NULL, true);
				}
				if (m_bControl)
				{
					keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
					m_bControl = false;
					::InvalidateRect(::GetDlgItem(m_hWnd, IDC_KEY_CONTROL), NULL, true);
				}
				if (m_bMenu)
				{
					keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
					m_bMenu = false;
					::InvalidateRect(::GetDlgItem(m_hWnd, IDC_KEY_MENU), NULL, true);
				}
				break;
			}
		}
	}
}

DWORD WINAPI CKeyBoardDlg::MessageLoopProc()
{
	// 线程函数由系统调用，此时还是系统的模块状态。必须切换模块状态，因为后面使用到对话框资源，
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

//	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
//	ASSERT(pModuleState->m_hCurrentInstanceHandle != NULL);

	MSG msg;
    // force msg-q to be created just in case, NOP otherwise
    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	HWND hwndForeFirst = ::GetForegroundWindow();

//	m_hWnd = CreateDialog(hInstance, (LPCTSTR)IDD_KEYBOARD, NULL, (DLGPROC)WndProc);

	if (!Create(m_bOnlyNumber ? IDD_KEYBOARD_NUMBER : IDD_KEYBOARD))
	{
		delete this;
		return 1;
	}

	if (hwndForeFirst != NULL)
		::SetForegroundWindow(hwndForeFirst);

	SetEvent(m_hEvent);

	// Acquire and dispatch messages until a WM_QUIT message is received.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		HWND hwndFore = ::GetForegroundWindow();
	
		if (IsWindow(hwndFore))
		{
			if (hwndFore != m_hWnd)
			{
				if (m_hwndFocusWindow != hwndFore)
				{
					if(IsWindow(m_hwndFocusWindow))
					{
						AttachThreadInput(
							GetWindowThreadProcessId(m_hWnd, NULL),
							GetWindowThreadProcessId(m_hwndFocusWindow, NULL),
							FALSE);
					}

					m_hwndFocusWindow = hwndFore;

					BOOL b = AttachThreadInput(
						GetWindowThreadProcessId(m_hWnd, NULL),
						GetWindowThreadProcessId(m_hwndFocusWindow, NULL), 
						TRUE);
				}
			}
		}

		if (!m_hWnd || !::IsDialogMessage(m_hWnd, &msg))
		{
			TranslateMessage(&msg);	// Translates virtual key codes
			DispatchMessage(&msg);	// Dispatches message to window
		}
	}
 
	DestroyWindow();

	return 0;
}

void CKeyBoardDlg::ReleaseFocus()
{
	if (::IsWindow(m_hwndFocusWindow))
	{
		HWND hwnd = ::GetForegroundWindow();
		if (::IsWindow(hwnd))
		{
			if (hwnd == m_hwndFocusWindow)
				return;
		}
		::SetForegroundWindow(m_hwndFocusWindow);
	}
}

LRESULT CALLBACK CKeyBoardDlg::_ButtonSubclassProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	ASSERT(c_pKeyBoardDlg != NULL);
	return c_pKeyBoardDlg->ButtonSubclassProc(hWnd, nMsg, wParam, lParam);
}

LRESULT CALLBACK CKeyBoardDlg::ButtonSubclassProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    RECT rc;
	PAINTSTRUCT ps;
	::GetClientRect(hWnd, &rc);
	HDC hdc;
	char buffer[100];
	DWORD cmd;

	switch (nMsg)
	{
	case WM_PAINT:
		hdc = ::BeginPaint(hWnd, &ps);
		FillRect(hdc, &rc, (HBRUSH) GetStockObject(LTGRAY_BRUSH));

		::GetWindowText(hWnd, buffer, sizeof(buffer));
		SelectObject(hdc, (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0L));
		SetBkMode(hdc,TRANSPARENT);
		DrawText(hdc, buffer, strlen(buffer), &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		FrameRect(hdc, &rc, (HBRUSH) GetStockObject(BLACK_BRUSH));
		InflateRect(&rc, -1, -1);
		FrameRect(hdc, &rc, (HBRUSH) GetStockObject(WHITE_BRUSH));
		InflateRect(&rc, 1, 1);
		cmd = ::GetWindowLong(hWnd, GWL_ID);
		if ((cmd == IDC_KEY_SHIFT && m_bShift) ||
			(cmd == IDC_KEY_CONTROL && m_bControl) ||
			(cmd == IDC_KEY_MENU && m_bMenu))
		{
			rc.left++;
			rc.top++;
		}
		else
		{
			rc.bottom--;
			rc.right--;
		}
		FrameRect(hdc, &rc, (HBRUSH) GetStockObject(GRAY_BRUSH));
		// End paint.
		::EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		hdc = ::GetDC(hWnd);
		::ValidateRect(hWnd, &rc);
		FrameRect(hdc, &rc, (HBRUSH) GetStockObject(BLACK_BRUSH));
		InflateRect(&rc, -1, -1);
		FrameRect(hdc, &rc, (HBRUSH) GetStockObject(WHITE_BRUSH));
		InflateRect(&rc, 1, 1);
		rc.left++;
		rc.top++;
		FrameRect(hdc, &rc, (HBRUSH) GetStockObject(GRAY_BRUSH));
		::ReleaseDC(hWnd, hdc);
		::SetCapture(hWnd);
		break;
	case WM_LBUTTONUP:
		ReleaseFocus();
		::SendMessage(::GetParent(hWnd), WM_COMMAND, GetWindowLong(hWnd, GWL_ID), 0);
		::InvalidateRect(hWnd, NULL, TRUE);
		ReleaseCapture();
		break;
	default:
		return CallWindowProc(m_pfnOldWndProc, hWnd, nMsg, wParam, lParam); 
	}
	return 0;
}

BOOL CKeyBoardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

//	HMENU hMenu = ::GetSystemMenu(m_hWnd, FALSE);
//	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL) ;
//	AppendMenu(hMenu, 0, SC_CLOSE, "关闭键盘(&K)");
	
	for (int i = 0; i < sizeof(keycode)/sizeof(KEYCODE); i++)
	{
		HWND hWnd = ::GetDlgItem(m_hWnd, keycode[i].ID);
		if (hWnd != NULL)
			m_pfnOldWndProc = (WNDPROC)::SetWindowLong(hWnd, GWL_WNDPROC, (long)_ButtonSubclassProc);
	}

	m_hHookMouse = ::SetWindowsHookEx(
			WH_MOUSE,
			_MouseProc,
			AfxGetInstanceHandle(),
			GetCurrentThreadId());
	ASSERT(m_hHookMouse != NULL);

	CRect rc;
	::GetClientRect(m_hWnd, &rc);
	::SetWindowPos(m_hWnd, NULL,
		m_bDefaultPos ? GetSystemMetrics(SM_CXFULLSCREEN) - (rc.right - rc.left) : m_nXPos,
		m_bDefaultPos ? GetSystemMetrics(SM_CYFULLSCREEN) - (rc.bottom - rc.top) : m_nYPos,
		0, 0, SWP_NOSIZE | SWP_NOZORDER);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKeyBoardDlg::OnDestroy()
{
	if (m_hHookMouse != NULL)
		UnhookWindowsHookEx(m_hHookMouse);

	CDialog::OnDestroy();
}

LRESULT CALLBACK CKeyBoardDlg::_MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (c_pKeyBoardDlg != NULL)
		return c_pKeyBoardDlg->MouseProc(nCode, wParam, lParam);

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


LRESULT CALLBACK CKeyBoardDlg::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    int i;
	LPMOUSEHOOKSTRUCT pMHS = (LPMOUSEHOOKSTRUCT)lParam;

    if (nCode < 0)  // do not process the message 
		return CallNextHookEx(NULL, nCode, wParam, lParam); 
	
	switch (wParam)
	{
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
		for (i = 0; i < sizeof(keycode)/sizeof(KEYCODE); i++)
		{	
			if (::GetDlgItem(m_hWnd, keycode[i].ID) == pMHS->hwnd)
				return ::PostMessage(::GetDlgItem(m_hWnd, keycode[i].ID), wParam, 0, 0);
		}
		break;
	case WM_LBUTTONUP:
		for (i = 0; i < sizeof(keycode)/sizeof(KEYCODE); i++)
		{
			if (::GetDlgItem(m_hWnd, keycode[i].ID) == pMHS->hwnd)
				return ::PostMessage(::GetDlgItem(m_hWnd, keycode[i].ID), wParam, 0, 0);
		}
		break;
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


void CKeyBoardDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (nFlags & MK_LBUTTON)
		::PostMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0L);	
	
	CDialog::OnMouseMove(nFlags, point);
}

void CKeyBoardDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	UINT nID = TrackPopupMenu(::GetSystemMenu(m_hWnd, FALSE), TPM_LEFTALIGN | TPM_RETURNCMD, point.x, point.y, 0, m_hWnd, NULL);
	::SendMessage(m_hWnd, WM_SYSCOMMAND, nID, 0L);
}

void CKeyBoardDlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	switch (nID & 0xFFF0)
	{
	case SC_CLOSE:
		::PostThreadMessage(m_dwThreadID, WM_QUIT, 0, 0L);
		return;
	}
	
	CDialog::OnSysCommand(nID, lParam);
}
