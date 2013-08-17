// PageMainForm.cpp : implementation file
//

#include "stdafx.h"
#include "cxdev.h"
#include "DevInc.h"
#include "PageMainForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageMainForm property page

IMPLEMENT_DYNCREATE(CPageMainForm, CPropertyPage)

CPageMainForm::CPageMainForm() : CPropertyPage(CPageMainForm::IDD)
{
	//{{AFX_DATA_INIT(CPageMainForm)
	m_bGrid = FALSE;
	m_nHeight = 0;
	m_nWidth = 0;
	//}}AFX_DATA_INIT
}

CPageMainForm::~CPageMainForm()
{
}

void CPageMainForm::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageMainForm)
	DDX_Control(pDX, IDC_GRIDCOLOR, m_btGridColor);
	DDX_Control(pDX, IDC_FOREGROUND, m_btForeColor);
	DDX_Control(pDX, IDC_BACKGROUND, m_btBackGround);
	DDX_Check(pDX, IDC_CHECK_GRID, m_bGrid);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
	DDV_MinMaxInt(pDX, m_nHeight, 64, 5000);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 0, 5000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageMainForm, CPropertyPage)
	//{{AFX_MSG_MAP(CPageMainForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageMainForm message handlers

BOOL CPageMainForm::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_nHeight = theApp.m_settingMainForm.m_nHeight;
	m_nWidth = theApp.m_settingMainForm.m_nWidth;

	m_btBackGround.SetColor(theApp.m_settingMainForm.m_clrBack);
	m_btForeColor.SetColor(theApp.m_settingMainForm.m_clrFore);

	m_btBackGround.SetDefaultColor(RGB(0, 0, 255));
	m_btForeColor.SetDefaultColor(RGB(0, 0, 0));
		
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageMainForm::OnApply() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();	

	theApp.m_settingMainForm.m_nHeight = m_nHeight;
	theApp.m_settingMainForm.m_nWidth = m_nWidth;

	theApp.m_settingMainForm.m_clrBack = m_btBackGround.GetColor();
	theApp.m_settingMainForm.m_clrFore = m_btForeColor.GetColor();

	return CPropertyPage::OnApply();
}
