// RunView.cpp : implementation of the CRunView class
//

#include "stdafx.h"
#include "CxRun.h"

#include "RunDoc.h"
#include "RunView.h"

#include "OEMSetup.h"

#include "../CxRunCtrl/CxRunCtrlIF.h"
#include "../CxRunCtrl/CxRunCtrl_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunView

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

IMPLEMENT_DYNCREATE(CRunView, CFormView)

BEGIN_MESSAGE_MAP(CRunView, CFormView)
	//{{AFX_MSG_MAP(CRunView)
	ON_WM_DESTROY()
	ON_WM_PAINT()
 	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CRunView, CFormView)
	ON_EVENT(CRunView, AFX_IDW_PANE_FIRST, 1 /* PictrueSize */, PictrueSize, VTS_I4 VTS_I4)
	ON_EVENT(CRunView, AFX_IDW_PANE_FIRST, 2 /* OnQuit */, OnQuit, VTS_NONE)
	ON_EVENT(CRunView, AFX_IDW_PANE_FIRST, 4 /* ShowPictureComplete */, ShowPictureComplete, VTS_BSTR)
	ON_EVENT(CRunView, AFX_IDW_PANE_FIRST, 7 /* PictureClose */, PictureClose, VTS_BSTR)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunView construction/destruction

CRunView::CRunView()
	: CFormView((LPCTSTR) NULL)
{
	//{{AFX_DATA_INIT(CRunView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

// 	m_pBrowserApp = NULL;

	m_pMenu = NULL;
	m_hAltMenu = NULL;
	m_hAccelTable = NULL;
	m_bLockMenu = TRUE;

	m_szPicture = CSize(0, 0);
}

CRunView::~CRunView()
{
	if (m_hAccelTable != NULL)
	{
		DestroyAcceleratorTable(m_hAccelTable);
		m_hAccelTable = NULL;
	}
	
//	if (m_pBrowserApp != NULL)
//		m_pBrowserApp->Release();
}

void CRunView::OnDestroy()
{
	if (m_pMenu != NULL)
	{
		::SetMenu(AfxGetMainWnd()->GetSafeHwnd(), m_hAltMenu);
		m_hAltMenu = NULL;
		m_pMenu = NULL;
	}

//	if (m_pBrowserApp != NULL)
//		m_pBrowserApp->Release();

}

/////////////////////////////////////////////////////////////////////////////
// CRunView diagnostics

#ifdef _DEBUG
void CRunView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRunView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CRunDoc* CRunView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRunDoc)));
	return (CRunDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRunView message handlers

BOOL CRunView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// create the view window itself
	m_pCreateContext = pContext;
	if (!CView::Create(lpszClassName, lpszWindowName,
				dwStyle, rect, pParentWnd,  nID, pContext))
	{
		return FALSE;
	}

	AfxEnableControlContainer();

	RECT rectClient;
	GetClientRect(&rectClient);

	// create the control window
	// AFX_IDW_PANE_FIRST is a safe but arbitrary ID
	if (!m_wndBrowser.CreateControl(CLSID_RunCtrl, lpszWindowName,
				WS_VISIBLE | WS_CHILD, rectClient, this, AFX_IDW_PANE_FIRST))
	{
		DestroyWindow();
		return FALSE;
	}

	SetCreateServerObjectAsLocal(COEMSetup::GetCreateServerObjectAsLocal());
//	SetLocalServerUseSACProxy(COEMSetup::GetLocalServerUseSACProxy());
	SetScrollBarsEnaled(TRUE);
	SetHighlightMouseObject(theApp.m_bHighlightMouseObject);

// 	if (theApp.m_bUseDefaultUserAndPassword)
// 	{
// 		SetDefaultUser(theApp.m_strDefaultUser);
// 		SetDefaultPassword(theApp.m_strDefaultPassword);
// 	}

	// 设置运行控件为当前焦点
	m_hWndFocus = m_wndBrowser.m_hWnd;

//	LPUNKNOWN lpUnk = m_wndBrowser.GetControlUnknown();
//	HRESULT hr = lpUnk->QueryInterface(DIID_IRunCtrl, (void**) &m_pBrowserApp);
//	if (!SUCCEEDED(hr))
//	{
//		m_pBrowserApp = NULL;
//		m_wndBrowser.DestroyWindow();
//		DestroyWindow();
//		return FALSE;
//	}

	//	static HMENU m_hMenu;
	//	HINSTANCE hInst = AfxFindResourceHandle(
	//		MAKEINTRESOURCE(IDR_MAINFRAME), RT_MENU);
	//	m_hMenu = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
	//	BOOL b  = ::SetMenu(m_hWnd, m_hMenu);
	//	DWORD dw = GetLastError();


	return TRUE;
}

void CRunView::OnPaint()
{
	Default();
}

/////////////////////////////////////////////////////////////////////////////
//【函  数】void OnPictrueSize( long, long lHeight ) 
//【参  数】lWidth  : long,
//          lHeight : long,
//【功  能】改变显示画面窗口的尺寸
//【返回值】void
//【说  明】不影响全局变量?
/////////////////////////////////////////////////////////////////////////////
void CRunView::PictrueSize(long lWidth, long lHeight)
{
	m_szPicture.cx = lWidth;
	m_szPicture.cy = lHeight;

	CRect rcOldPos;
	m_wndBrowser.GetWindowRect(&rcOldPos);

	ReLayout();

	CRect rcNewPos;
	m_wndBrowser.GetWindowRect(&rcNewPos);

	// 新位置小于旧位置时需要刷新
	if (rcNewPos.Width() < rcOldPos.Width() || rcNewPos.Height() < rcOldPos.Height())
		Invalidate();
}

void CRunView::OnQuit()
{
	CWnd* pFrameWnd = AfxGetMainWnd();
	pFrameWnd->PostMessage(WM_CLOSE, 0, 0);
}

#include "../CxOle/CxHmiObj.h"
#include "../CxOle/CxHmiObj_i.c"

void CRunView::AddMenuItems(HMENU hMenu, IDrawMenu* pMenu, int& nIndex, int nCount, int nDepth)
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

			if (strItemText == _T("-"))
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


void CRunView::ShowPictureComplete(LPCTSTR lpszPicture)
{
	IDispatch* pMenuObject = GetMainMenu();

	if (pMenuObject != NULL)
	{
		m_arrAccels.RemoveAll();

		if (m_menu.m_hMenu != NULL)
			m_menu.DestroyMenu();

		m_menu.CreateMenu();
		
		CComQIPtr<IDrawMenu, &IID_IDrawMenu> spDrawMenu(pMenuObject);
		long lCount;
		spDrawMenu->get_ItemCount(&lCount);
		
		int nIndex = 0;
		AddMenuItems(m_menu.m_hMenu, spDrawMenu, nIndex, lCount, 0);
		
		if (m_pMenu == NULL)
		{
			CMenu* pMenu = AfxGetMainWnd()->GetMenu();
			if (pMenu != NULL)
				m_hAltMenu = pMenu->GetSafeHmenu();
		}

		AfxGetMainWnd()->SetMenu(&m_menu);
		
		m_pMenu = spDrawMenu;

		m_bLockMenu = FALSE;
	
		pMenuObject->Release();

		if (m_arrAccels.GetSize() > 0)
			m_hAccelTable = CreateAcceleratorTable(m_arrAccels.GetData(), m_arrAccels.GetSize());
	}
	else
	{
		if (m_pMenu != NULL)
		{
			::SetMenu(AfxGetMainWnd()->GetSafeHwnd(), m_hAltMenu);
			
			m_hAltMenu = NULL;
			m_pMenu = NULL;
		}
	}
}

void CRunView::PictureClose(LPCTSTR lpszPicture)
{
	m_bLockMenu = TRUE;

	if (m_hAccelTable != NULL)
	{
		DestroyAcceleratorTable(m_hAccelTable);
		m_hAccelTable = NULL;
	}
}

CString CRunView::GetPicture() const
{
	CString result;
	m_wndBrowser.GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void CRunView::SetPicture(LPCSTR lpsz)
{
//	ASSERT(false);
	m_wndBrowser.SetProperty(0x1, VT_BSTR, lpsz);
}

CString CRunView::GetServer() const
{
	CString result;
	m_wndBrowser.GetProperty(10, VT_BSTR, (void*)&result);
	return result;
}

void CRunView::SetServer(LPCSTR lpsz)
{
	m_wndBrowser.SetProperty(10, VT_BSTR, lpsz);
}

BOOL CRunView::GetZoomFitWindow()
{
	BOOL result;
	m_wndBrowser.GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void CRunView::SetZoomFitWindow(BOOL propVal)
{
	m_wndBrowser.SetProperty(3, VT_BOOL, propVal);
}

BOOL CRunView::GetCreateServerObjectAsLocal()
{
	BOOL result;
	m_wndBrowser.GetProperty(15, VT_BOOL, (void*)&result);
	return result;
}

void CRunView::SetCreateServerObjectAsLocal(BOOL propVal)
{
	m_wndBrowser.SetProperty(15, VT_BOOL, propVal);
}

BOOL CRunView::GetLocalServerUseSACProxy()
{
	BOOL result;
	m_wndBrowser.GetProperty(16, VT_BOOL, (void*)&result);
	return result;
}

void CRunView::SetLocalServerUseSACProxy(BOOL propVal)
{
	m_wndBrowser.SetProperty(16, VT_BOOL, propVal);
}

BOOL CRunView::GetScrollBarsEnaled()
{
	BOOL result;
	m_wndBrowser.GetProperty(6, VT_BOOL, (void*)&result);
	return result;
}

void CRunView::SetScrollBarsEnaled(BOOL propVal)
{
	m_wndBrowser.SetProperty(6, VT_BOOL, propVal);
}

void CRunView::ClearPicturesCache()
{
	m_wndBrowser.InvokeHelper(20, DISPATCH_METHOD,
		VT_EMPTY, NULL, VTS_NONE);
}

void CRunView::SetHighlightMouseObject(boolean bHighlight)
{
	static BYTE parms[] =
		VTS_BOOL;
	m_wndBrowser.InvokeHelper(25, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		bHighlight);
}

void CRunView::SetDefaultUser(LPCSTR lpsz)
{
	m_wndBrowser.SetProperty(13, VT_BSTR, lpsz);
}

void CRunView::SetDefaultPassword(LPCSTR lpsz)
{
	m_wndBrowser.SetProperty(14, VT_BSTR, lpsz);
}

LPDISPATCH CRunView::GetMainMenu()
{
	LPDISPATCH result;
	m_wndBrowser.InvokeHelper(26, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void CRunView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	CString strPicture = GetDocument()->GetPathName();
	SetPicture(strPicture);
}

BOOL CRunView::OnEraseBkgnd(CDC* pDC) 
{
	ASSERT_VALID(pDC);
	
	// Fill background with APPWORKSPACE
	CBrush backBrush(GetSysColor(COLOR_APPWORKSPACE));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);     // Erase the area needed
	
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);

	return TRUE;
}

BOOL CRunView::PreCreateWindow(CREATESTRUCT& cs) 
{
	BOOL b = CFormView::PreCreateWindow(cs);

	cs.style |= WS_CLIPCHILDREN;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return b;
}

BOOL CRunView::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if (m_hAccelTable != NULL &&
		::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
		{
			return TRUE;
		}
	}

	// CFormView 会过来对话框消息，如光标键
	return FALSE;
}

BOOL CRunView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (m_pMenu != NULL && !m_bLockMenu && pHandlerInfo == NULL)
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
	
	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CRunView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	ReLayout();
}

void CRunView::ReLayout()
{
	if (::IsWindow(m_wndBrowser.m_hWnd))
	{
		// need to push non-client borders out of the client area
		CRect rect;
		GetClientRect(rect);
		::AdjustWindowRectEx(rect,
			m_wndBrowser.GetStyle(), FALSE, GetExStyle());

		int nWidth = m_szPicture.cx;
		int nHeight = m_szPicture.cy;

		if (nWidth > rect.Width() && nHeight <= rect.Height())
			nHeight += GetSystemMetrics(SM_CXVSCROLL) + 1;
		else if (nHeight > rect.Height() && nWidth <= rect.Width())
			nWidth += GetSystemMetrics(SM_CXHSCROLL) + 1;

		m_wndBrowser.SetWindowPos(NULL, rect.left, rect.top,
			min(nWidth, rect.Width()), min(nHeight, rect.Height()), SWP_NOACTIVATE | SWP_NOZORDER);
	}
}
