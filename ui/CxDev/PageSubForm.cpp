// PageSubForm.cpp : implementation file
//

#include "stdafx.h"
#include "cxdev.h"
#include "PageSubForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSubForm property page

IMPLEMENT_DYNCREATE(CPageSubForm, CPropertyPage)

CPageSubForm::CPageSubForm() : CPropertyPage(CPageSubForm::IDD)
{
	//{{AFX_DATA_INIT(CPageSubForm)
	m_bGrid = FALSE;
	m_nHeight = 0;
	m_nWidth = 0;
	//}}AFX_DATA_INIT
}

CPageSubForm::~CPageSubForm()
{
}

void CPageSubForm::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSubForm)
	DDX_Control(pDX, IDC_GRIDCOLOR, m_btGridColor);
	DDX_Control(pDX, IDC_FOREGROUND, m_btForeColor);
	DDX_Control(pDX, IDC_BACKGROUND, m_btBackColor);
	DDX_Check(pDX, IDC_CHECK_GRID, m_bGrid);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
	DDV_MinMaxInt(pDX, m_nHeight, 0, 4000);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 0, 4000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSubForm, CPropertyPage)
	//{{AFX_MSG_MAP(CPageSubForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSubForm message handlers

BOOL CPageSubForm::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_nHeight = theApp.m_settingSubForm.m_nHeight;
	m_nWidth = theApp.m_settingSubForm.m_nWidth;

	m_btBackColor.SetColor(theApp.m_settingMainForm.m_clrBack);
	m_btForeColor.SetColor(theApp.m_settingMainForm.m_clrFore);
	
	m_btBackColor.SetDefaultColor(RGB(0, 0, 255));
	m_btForeColor.SetDefaultColor(RGB(0, 0, 0));

	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageSubForm::OnApply() 
{
	UpdateData(FALSE);

	theApp.m_settingSubForm.m_nHeight = m_nHeight;
	theApp.m_settingSubForm.m_nWidth = m_nWidth;

	theApp.m_settingMainForm.m_clrBack = m_btBackColor.GetColor();
	theApp.m_settingMainForm.m_clrFore = m_btForeColor.GetColor();

	return CPropertyPage::OnApply();
}
