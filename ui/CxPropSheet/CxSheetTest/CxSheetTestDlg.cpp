// CxSheetTestDlg.cpp : implementation file
// 所属项目: AutoCx的CxSheetTest工程
// 创建日期: 2001.10.8
// 创 建 人：孙越洋 
// 功能描述：测试工程主对话框窗口类。 
// 修改记录：
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxSheetTest.h"
#include "CxSheetTestDlg.h"

#include "..\CxPropSheetExport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCxSheetTestDlg dialog

CCxSheetTestDlg::CCxSheetTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCxSheetTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCxSheetTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCxSheetTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCxSheetTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCxSheetTestDlg, CDialog)
	//{{AFX_MSG_MAP(CCxSheetTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TEST, OnTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCxSheetTestDlg message handlers

BOOL CCxSheetTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCxSheetTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCxSheetTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCxSheetTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

const IID IID_IDrawBitmap = {0xFC36C0EF,0x827C,0x11D5,{0x8D,0x95,0x00,0xE0,0x4C,0x69,0xB3,0xAF}};
const CLSID CLSID_DrawBitmap = {0xFC36C0F0,0x827C,0x11D5,{0x8D,0x95,0x00,0xE0,0x4C,0x69,0xB3,0xAF}};
const CLSID CLSID_MainPage = {0x06DC5CEA,0xD2BB,0x49F4,{0xAF,0xC7,0x04,0xFF,0xAC,0xE2,0x53,0x3D}};
const CLSID CLSID_TestPage1 = {0x4081C3E5,0xDC50,0x497D,{0x97,0x11,0x28,0xC0,0xA3,0xDA,0xA0,0xD7}};

///////////////////////////////////////////////////////////////////////////////
//功能:开始属性页测试(用已注册的CxBitmap和TestPage1测试属性页对象
///////////////////////////////////////////////////////////////////////////////
void CCxSheetTestDlg::OnTest() 
{
	// TODO: Add your control notification handler code here
	CLSID ids[2];
	ids[0] = CLSID_MainPage;
	ids[1] = CLSID_TestPage1;
	IUnknown* pUnk[2] = {NULL, NULL};
	HRESULT hr = CoCreateInstance(CLSID_DrawBitmap, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void **)(&(pUnk[0])));
	pUnk[1] = pUnk[0];
	if(hr != S_OK)
		AfxThrowOleException(hr);
	hr = CxCreatePropertyFrame(m_hWnd, 0, 0, L"测试", 2, pUnk, 2, ids, 1020, 0, NULL);
	if(hr != S_OK)
		AfxThrowOleException(hr);
}
