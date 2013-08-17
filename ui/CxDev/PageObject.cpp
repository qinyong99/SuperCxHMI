// PageObject.cpp : implementation file
//

#include "stdafx.h"
#include "cxdev.h"
#include "PageObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageObject property page

IMPLEMENT_DYNCREATE(CPageObject, CPropertyPage)

CPageObject::CPageObject() : CPropertyPage(CPageObject::IDD)
{
	//{{AFX_DATA_INIT(CPageObject)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPageObject::~CPageObject()
{
}

void CPageObject::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageObject)
	DDX_Control(pDX, IDC_HIGHLIGHTFILLCOLOR, m_btForeColor);
	DDX_Control(pDX, IDC_FILLCOLOR, m_btBackColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageObject, CPropertyPage)
	//{{AFX_MSG_MAP(CPageObject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageObject message handlers

BOOL CPageObject::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_btBackColor.SetColor(theApp.m_settingMainForm.m_clrBack);
	m_btForeColor.SetColor(theApp.m_settingMainForm.m_clrFore);
	
	m_btBackColor.SetDefaultColor(RGB(0, 0, 255));
	m_btForeColor.SetDefaultColor(RGB(0, 0, 0));
	
	UpdateData(FALSE);		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageObject::OnApply() 
{
	UpdateData();
	
	theApp.m_settingMainForm.m_clrBack = m_btBackColor.GetColor();
	theApp.m_settingMainForm.m_clrFore = m_btForeColor.GetColor();

	return CPropertyPage::OnApply();
}
