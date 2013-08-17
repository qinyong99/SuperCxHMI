// PageApp.cpp : implementation file
//

#include "stdafx.h"
#include "CxDev.h"
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
	m_strPictureFolder = _T("");
	m_strStartPicture = _T("");
	m_bFullScreen = FALSE;
	m_bUseSetting = FALSE;
	m_bHighlightMouseObject = TRUE;
	//}}AFX_DATA_INIT
}

CPageGen::~CPageGen()
{
}

void CPageGen::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageGen)
	DDX_Text(pDX, IDC_FOLDER, m_strPictureFolder);
	DDX_Control(pDX, IDC_FOLDER, m_edtPictureFolder);
	DDX_Check(pDX, IDC_USESETTING, m_bUseSetting);
	DDX_Control(pDX, IDC_USESETTING, m_btnUseSetting);
	DDX_Text(pDX, IDC_PICTURE, m_strStartPicture);
	DDX_Control(pDX, IDC_PICTURE, m_edtStartPicture);
	DDX_Check(pDX, IDC_CHECK2, m_bFullScreen);
	DDX_Check(pDX, IDC_CHECK3, m_bHighlightMouseObject);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageGen, CPropertyPage)
	//{{AFX_MSG_MAP(CPageGen)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, OnButtonFolder)
	ON_BN_CLICKED(IDC_BUTTON_PICTURE, OnButtonPicture)
	ON_BN_CLICKED(IDC_USESETTING, OnUseSetting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageGen message handlers

const TCHAR szIniRunSection[] = _T("Run");
const TCHAR szIniUseSettingEntry[] = _T("UseSetting");
const TCHAR szIniStartPictureEntry[] = _T("StartPicture");
const TCHAR szIniFullScreenEntry[] = _T("FullScreen");
const TCHAR szIniHighlightMouseObject[] = _T("HighlightMouseObject");

BOOL CPageGen::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_strPictureFolder = theApp.GetIniStartPictureFolder();

	CString strIniFile = theApp.GetSystemSetupFile();

	m_nUseSetting = ::GetPrivateProfileInt(szIniRunSection, szIniUseSettingEntry, 0, strIniFile);
	m_bUseSetting = (m_nUseSetting == 2);
	
	TCHAR szBuf[256];
	::GetPrivateProfileString(szIniRunSection, szIniStartPictureEntry, _T(""), szBuf, 256, strIniFile);
	m_strStartPicture = szBuf;
	m_bFullScreen = ::GetPrivateProfileInt(szIniRunSection, szIniFullScreenEntry, 0, strIniFile) != 0;

	UpdateData(FALSE);

	OnUseSetting();

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageGen::OnButtonFolder() 
{
	BROWSEINFO bi;
	memset(&bi, 0, sizeof BROWSEINFO);
	bi.hwndOwner = m_hWnd;
	
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl)
	{
		TCHAR szFolderPath[MAX_PATH];
		
		if (::SHGetPathFromIDList(pidl, szFolderPath))
		{
			IMalloc* pMalloc = NULL;
			if(SUCCEEDED(::SHGetMalloc(&pMalloc)))
			{
				pMalloc->Free(pidl);
				pMalloc->Release();
			}

			m_edtPictureFolder.SetWindowText(szFolderPath);
		}
	}	
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

	theApp.UpdateIniStartPictureFolder(m_strPictureFolder);

	CString strIniFile = theApp.GetSystemSetupFile();

	if (m_bUseSetting)
	{
		m_nUseSetting = 2;
	}
	else 
	{
		if (m_nUseSetting == 2)
			m_nUseSetting = 0;
	}

	CString str;
	str.Format(_T("%d"), m_nUseSetting);
	::WritePrivateProfileString(szIniRunSection, szIniUseSettingEntry, str,	strIniFile);
	::WritePrivateProfileString(szIniRunSection, szIniStartPictureEntry, m_strStartPicture, strIniFile);
	str.Format(_T("%d"), m_bFullScreen);
	::WritePrivateProfileString(szIniRunSection, szIniFullScreenEntry, str,	strIniFile);
	str.Format(_T("%d"), m_bHighlightMouseObject);
	::WritePrivateProfileString(szIniRunSection, szIniHighlightMouseObject, str, strIniFile);
	
	CPropertyPage::OnOK();
}

void CPageGen::OnUseSetting() 
{
	BOOL b = m_btnUseSetting.GetCheck();
	GetDlgItem(IDC_PICTURE)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_PICTURE)->EnableWindow(b);
	GetDlgItem(IDC_CHECK2)->EnableWindow(b);
	GetDlgItem(IDC_CHECK3)->EnableWindow(b);
}
