// GridSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cxdev.h"
#include "GridSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridSettingDlg dialog


CGridSettingDlg::CGridSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGridSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGridSettingDlg)
	m_bGridEnable = FALSE;
	m_nGridSpaceWidth = 10;
	m_nGridSpaceHeight = 10;
	m_bAlignToGrid = FALSE;
	//}}AFX_DATA_INIT
}


void CGridSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridSettingDlg)
	DDX_Control(pDX, IDC_GRID_SPACE_HEIGHT, m_editGridSpaceHeight);
	DDX_Control(pDX, IDC_GRID_SPACE_WIDTH, m_editGridSpaceWidth);
	DDX_Control(pDX, IDC_GRID_COLOR, m_clrGridColor);
	DDX_Check(pDX, IDC_GRID_ENABLE, m_bGridEnable);
	DDX_Text(pDX, IDC_GRID_SPACE_WIDTH, m_nGridSpaceWidth);
	DDX_Text(pDX, IDC_GRID_SPACE_HEIGHT, m_nGridSpaceHeight);
	DDX_Check(pDX, IDC_ALIGN_GRID, m_bAlignToGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGridSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CGridSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridSettingDlg message handlers

BOOL CGridSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_clrGridColor.SetColor(theApp.m_settingGen.m_clrGrid);
	m_clrGridColor.SetDefaultColor(RGB(0, 0, 0));

	m_bGridEnable = theApp.m_settingGen.m_bGrid;

	m_nGridSpaceWidth = theApp.m_settingGen.m_nGridSpaceWidth;
	m_nGridSpaceHeight = theApp.m_settingGen.m_nGridSpaceHeight;

	m_bAlignToGrid = theApp.m_settingGen.m_bAlignToGrid;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CGridSettingDlg::OnOK() 
{
	UpdateData();

	theApp.m_settingGen.m_clrGrid = m_clrGridColor.GetColor();

	theApp.m_settingGen.m_bGrid = m_bGridEnable;

	if (m_nGridSpaceWidth < 2)
	{
		MessageBox(_T("水平间隔不应小于2。"), NULL, MB_OK | MB_ICONERROR);
		m_editGridSpaceWidth.SetFocus();
		m_editGridSpaceWidth.SetSel(0, -1);
		return;
	}
	if (m_nGridSpaceHeight < 2)
	{
		MessageBox(_T("垂直间隔不应小于2。"), NULL, MB_OK | MB_ICONERROR);
		m_editGridSpaceHeight.SetFocus();
		m_editGridSpaceHeight.SetSel(0, -1);
		return;
	}

	theApp.m_settingGen.m_nGridSpaceWidth = m_nGridSpaceWidth;
	theApp.m_settingGen.m_nGridSpaceHeight = m_nGridSpaceHeight;

	theApp.m_settingGen.m_bAlignToGrid = m_bAlignToGrid;
	
	CDialog::OnOK();
}
