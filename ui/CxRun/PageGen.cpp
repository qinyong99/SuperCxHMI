// PageApp.cpp : implementation file
//

#include "stdafx.h"
#include "CxRun.h"
#include "PageGen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageGen property page

IMPLEMENT_DYNCREATE(CPageGen, CPropertyPage)

CPageGen::CPageGen() : CPropertyPage(CPageGen::IDD)
{
	//{{AFX_DATA_INIT(CPageGen)
	m_strServer = _T("");
	m_strStartPicture = _T("");
	m_bFullScreen = FALSE;
	m_nUseSetting = 0;
	m_bHighlightMouseObject = TRUE;
	m_bPromptLocalServerModified = TRUE;
	m_strDefaultUser = _T("");
	m_strDefaultPassword = _T("");
	//}}AFX_DATA_INIT
}

CPageGen::~CPageGen()
{
}

void CPageGen::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageGen)
	DDX_Text(pDX, IDC_SERVER, m_strServer);
	DDX_Control(pDX, IDC_SERVER, m_edtServer);
	DDX_Text(pDX, IDC_SERVER2, m_strServer2);
	DDX_Control(pDX, IDC_SERVER2, m_edtServer2);
	DDX_Text(pDX, IDC_PICTURE, m_strStartPicture);
	DDX_Control(pDX, IDC_PICTURE, m_edtStartPicture);
	DDX_Check(pDX, IDC_CHECK1, m_bFullScreen);
	DDX_Check(pDX, IDC_CHECK2, m_bHighlightMouseObject);
	DDX_Radio(pDX, IDC_RADIO1, m_nUseSetting);
	DDX_Check(pDX, IDC_CHECK3, m_bPromptLocalServerModified);
	DDX_Check(pDX, IDC_CHECK4, m_bUseDefaultUserAndPassword);
	DDX_Text(pDX, IDC_DEFAULT_USER, m_strDefaultUser);
	DDX_Text(pDX, IDC_DEFAULT_PASSWORD, m_strDefaultPassword);
	DDX_Control(pDX, IDC_CHECK4, m_checkUseDefaultUserAndPassword);
	DDX_Control(pDX, IDC_DEFAULT_USER, m_edtDefaultUser);
	DDX_Control(pDX, IDC_DEFAULT_PASSWORD, m_edtDefaultPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageGen, CPropertyPage)
	//{{AFX_MSG_MAP(CPageGen)
	ON_BN_CLICKED(IDC_BUTTON_SERVER, OnButtonServer)
	ON_BN_CLICKED(IDC_BUTTON_SERVER2, OnButtonServer2)
	ON_BN_CLICKED(IDC_BUTTON_PICTURE, OnButtonPicture)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageGen message handlers

BOOL CPageGen::OnInitDialog() 
{
	TCHAR szBuf[256];

	CPropertyPage::OnInitDialog();
	
	CString strIniFile = theApp.GetSystemSetupFile();

	m_nUseSetting = ::GetPrivateProfileInt(szIniRunSection, szIniUseSettingEntry, 0, strIniFile);
	
	szBuf[0] = _T('\0');
	::GetPrivateProfileString(szIniRunSection, szIniServerEntry, _T(""), szBuf, 256, strIniFile);
	m_strServer = szBuf;

	szBuf[0] = _T('\0');
	::GetPrivateProfileString(szIniRunSection, szIniServer2Entry, _T(""), szBuf, 256, strIniFile);
	m_strServer2 = szBuf;

	szBuf[0] = _T('\0');
	::GetPrivateProfileString(szIniRunSection, szIniStartPictureEntry, _T(""), szBuf, 256, strIniFile);
	m_strStartPicture = szBuf;

	m_bUseDefaultUserAndPassword = ::GetPrivateProfileInt(szIniRunSection, szIniUseDefaultUserAndPassword, 0, strIniFile) != 0;

	szBuf[0] = _T('\0');
	::GetPrivateProfileString(szIniRunSection, szIniDefaultUser, _T(""), szBuf, 256, strIniFile);
	m_strDefaultUser = szBuf;

	szBuf[0] = _T('\0');
	::GetPrivateProfileString(szIniRunSection, szIniDefaultPassword, _T(""), szBuf, 256, strIniFile);
	m_strDefaultPassword = szBuf;

	m_bFullScreen = ::GetPrivateProfileInt(szIniRunSection, szIniFullScreenEntry, 0, strIniFile) != 0;
	m_bHighlightMouseObject = ::GetPrivateProfileInt(szIniRunSection, szIniHighlightMouseObject, 1, strIniFile) != 0;
	m_bPromptLocalServerModified = ::GetPrivateProfileInt(szIniRunSection, szIniPromptLocalServerModified, 1, strIniFile) != 0;

	UpdateData(FALSE);

	UpdateEnableState(m_nUseSetting);

	m_edtDefaultUser.EnableWindow(m_bUseDefaultUserAndPassword);
	m_edtDefaultPassword.EnableWindow(m_bUseDefaultUserAndPassword);

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageGen::OnButtonServer() 
{
	LPITEMIDLIST pidlRoot = NULL;
	SHGetSpecialFolderLocation(m_hWnd, CSIDL_NETWORK, &pidlRoot);
	
	CString strDisplayName;
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(BROWSEINFO));
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = pidlRoot;
	bi.pszDisplayName = strDisplayName.GetBuffer(MAX_PATH + 1);
	bi.lpszTitle = _T("查找计算机");
	bi.ulFlags = BIF_BROWSEFORCOMPUTER;
	
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	strDisplayName.ReleaseBuffer();
	
	if (pidl)
		m_edtServer.SetWindowText(strDisplayName);
	
	IMalloc *pMalloc = NULL;
	SHGetMalloc(&pMalloc);
	pMalloc->Free(pidlRoot);
}

void CPageGen::OnButtonServer2() 
{
	LPITEMIDLIST pidlRoot = NULL;
	SHGetSpecialFolderLocation(m_hWnd, CSIDL_NETWORK, &pidlRoot);
	
	CString strDisplayName;
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(BROWSEINFO));
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = pidlRoot;
	bi.pszDisplayName = strDisplayName.GetBuffer(MAX_PATH + 1);
	bi.lpszTitle = _T("查找计算机");
	bi.ulFlags = BIF_BROWSEFORCOMPUTER;
	
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	strDisplayName.ReleaseBuffer();
	
	if (pidl)
		m_edtServer2.SetWindowText(strDisplayName);
	
	IMalloc *pMalloc = NULL;
	SHGetMalloc(&pMalloc);
	pMalloc->Free(pidlRoot);
}

void CPageGen::OnButtonPicture() 
{
	CFileDialog dlg(TRUE, _T("grf"), m_strStartPicture,
		OFN_FILEMUSTEXIST,
		_T("画面文件 (*.grf)|*.grf|所有文件 (*.*)|*.*||"));

	if (dlg.DoModal() != IDOK)
		return; 
	
	m_edtStartPicture.SetWindowText(dlg.GetPathName());
}

void CPageGen::OnOK() 
{
	UpdateData();

	CString strIniFile = theApp.GetSystemSetupFile();

	CString str;
	str.Format(_T("%d"), m_nUseSetting);
	::WritePrivateProfileString(szIniRunSection, szIniUseSettingEntry, str,	strIniFile);
	::WritePrivateProfileString(szIniRunSection, szIniServerEntry, m_strServer, strIniFile);
	::WritePrivateProfileString(szIniRunSection, szIniServer2Entry, m_strServer2, strIniFile);
	::WritePrivateProfileString(szIniRunSection, szIniStartPictureEntry, m_strStartPicture, strIniFile);
	::WritePrivateProfileString(szIniRunSection, szIniDefaultUser, m_strDefaultUser, strIniFile);
	::WritePrivateProfileString(szIniRunSection, szIniDefaultPassword, m_strDefaultPassword, strIniFile);
	str.Format(_T("%d"), m_bFullScreen);
	::WritePrivateProfileString(szIniRunSection, szIniFullScreenEntry, str,	strIniFile);
	str.Format(_T("%d"), m_bHighlightMouseObject);
	::WritePrivateProfileString(szIniRunSection, szIniHighlightMouseObject, str, strIniFile);
	str.Format(_T("%d"), m_bPromptLocalServerModified);
	::WritePrivateProfileString(szIniRunSection, szIniPromptLocalServerModified, str, strIniFile);
	str.Format(_T("%d"), m_bUseDefaultUserAndPassword);
	::WritePrivateProfileString(szIniRunSection, szIniUseDefaultUserAndPassword, str, strIniFile);

	CPropertyPage::OnOK();
}

void CPageGen::OnRadio1() 
{
	UpdateEnableState(0);
}

void CPageGen::OnRadio2() 
{
	UpdateEnableState(1);
}

void CPageGen::OnRadio3() 
{
	UpdateEnableState(2);
}

void CPageGen::OnCheck4()
{
	BOOL bEnable = m_checkUseDefaultUserAndPassword.GetCheck();
	m_edtDefaultUser.EnableWindow(bEnable);
	m_edtDefaultPassword.EnableWindow(bEnable);
}

void CPageGen::UpdateEnableState(int nUseSetting)
{
	GetDlgItem(IDC_CHECK3)->EnableWindow(nUseSetting == 0);
	GetDlgItem(IDC_SERVER)->EnableWindow(nUseSetting == 1);
	GetDlgItem(IDC_BUTTON_SERVER)->EnableWindow(nUseSetting == 1);
	GetDlgItem(IDC_SERVER2)->EnableWindow(nUseSetting == 1);
	GetDlgItem(IDC_BUTTON_SERVER2)->EnableWindow(nUseSetting == 1);
	GetDlgItem(IDC_PICTURE)->EnableWindow(nUseSetting == 2);
	GetDlgItem(IDC_BUTTON_PICTURE)->EnableWindow(nUseSetting == 2);
	GetDlgItem(IDC_CHECK1)->EnableWindow(nUseSetting == 2);
	GetDlgItem(IDC_CHECK2)->EnableWindow(nUseSetting == 2);
}
