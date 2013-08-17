// ReportLayoutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DevInc.h"
#include "ReportLayoutDlg.h"
#include "ReportFieldDlg.h"
#include "ReportWizardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportLayoutDlg dialog


IMPLEMENT_DYNCREATE(CReportLayoutDlg, CDialog)

CReportLayoutDlg::CReportLayoutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReportLayoutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportLayoutDlg)
	m_nLayout = 1;
	m_nOrientation = 0;
	//}}AFX_DATA_INIT

	m_pReportWizardDlg = NULL;
}


void CReportLayoutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportLayoutDlg)
	DDX_Control(pDX, IDC_ORIENTATION, m_statOrientation);
	DDX_Control(pDX, IDC_LAYOUT, m_statLayout);
	DDX_Radio(pDX, IDC_COLUMNAR, m_nLayout);
	DDX_Radio(pDX, IDC_PORTRAIT, m_nOrientation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportLayoutDlg, CDialog)
	//{{AFX_MSG_MAP(CReportLayoutDlg)
	ON_BN_CLICKED(IDC_COLUMNAR, OnColumnar)
	ON_BN_CLICKED(IDC_TABULAR, OnTabular)
	ON_BN_CLICKED(IDC_JUSTIFIED, OnJustified)
	ON_BN_CLICKED(IDC_PORTRAIT, OnPortrait)
	ON_BN_CLICKED(IDC_LANDSCAPE, OnLandscape)
	ON_BN_CLICKED(IDC_ORIENTATION, OnOrientation)
	//}}AFX_MSG_MAP
	ON_MESSAGE_VOID(CXMSG_DIALOG_APPLY, OnApply)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportLayoutDlg message handlers

void CReportLayoutDlg::OnApply()
{
	ASSERT(m_pReportWizardDlg != NULL);
	
	UpdateData(TRUE);

	m_pReportWizardDlg->m_nReportLayout = m_nLayout;
	m_pReportWizardDlg->m_nReportOrientation = m_nOrientation;
}

BOOL CReportLayoutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pReportWizardDlg = (CReportWizardDlg *)GetParent();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportLayoutDlg::OnColumnar() 
{
	if (m_bitmapColumnar.m_hObject == NULL)
		m_bitmapColumnar.LoadBitmap(IDB_COLUMNAR);

	m_statLayout.SetBitmap(m_bitmapColumnar);
}

void CReportLayoutDlg::OnTabular() 
{
	if (m_bitmapTabular.m_hObject == NULL)
		m_bitmapTabular.LoadBitmap(IDB_TABULAR);
	
	m_statLayout.SetBitmap(m_bitmapTabular);
}

void CReportLayoutDlg::OnJustified() 
{
	if (m_bitmapJustified.m_hObject == NULL)
		m_bitmapJustified.LoadBitmap(IDB_JUSTIFIED);
	
	m_statLayout.SetBitmap(m_bitmapJustified);
}

void CReportLayoutDlg::OnPortrait() 
{
	m_statOrientation.SetIcon(theApp.LoadIcon(IDI_PORTRAIT));
}

void CReportLayoutDlg::OnLandscape() 
{
	m_statOrientation.SetIcon(theApp.LoadIcon(IDI_LANDSCAPE));
}

void CReportLayoutDlg::OnOrientation() 
{
	m_nOrientation = m_nOrientation == 0 ? 1 : 0;
	UpdateData(FALSE);
}
