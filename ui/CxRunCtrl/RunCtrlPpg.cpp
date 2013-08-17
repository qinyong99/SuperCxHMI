// RunCtrlPpg.cpp : Implementation of the CRunCtrlPropPage property page class.

#include "stdafx.h"
#include "CxRunCtrl.h"
#include "RunCtrlPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CRunCtrlPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CRunCtrlPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CRunCtrlPropPage)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonPicture)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CRunCtrlPropPage, "CXRUNCTRL.RunCtrlPropPage.1",
	0x5ccf80c7, 0x87a0, 0x438d, 0x88, 0x86, 0xf4, 0x80, 0x49, 0x56, 0x96, 0xb8)


/////////////////////////////////////////////////////////////////////////////
// CRunCtrlPropPage::CRunCtrlPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CRunCtrlPropPage

BOOL CRunCtrlPropPage::CRunCtrlPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_RUNCTRL_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CRunCtrlPropPage::CRunCtrlPropPage - Constructor

CRunCtrlPropPage::CRunCtrlPropPage() :
	COlePropertyPage(IDD, IDS_RUNCTRL_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CRunCtrlPropPage)
	m_nBorderStyle = 0;
	m_bRefreshEnabled = FALSE;
	m_bZoomFitWindow = FALSE;
	m_strPicture = _T("");
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CRunCtrlPropPage::DoDataExchange - Moves data between page and properties

void CRunCtrlPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CRunCtrlPropPage)
	DDP_Radio(pDX, IDC_RADIO1, m_nBorderStyle, _T("BorderStyle"));
	DDX_Radio(pDX, IDC_RADIO1, m_nBorderStyle);
	DDP_Check(pDX, IDC_CHECK3, m_bRefreshEnabled, _T("RefreshEnabled") );
	DDX_Check(pDX, IDC_CHECK3, m_bRefreshEnabled);
	DDP_Check(pDX, IDC_CHECK4, m_bZoomFitWindow, _T("ZoomFitWindow") );
	DDX_Check(pDX, IDC_CHECK4, m_bZoomFitWindow);
	DDP_Text(pDX, IDC_EDIT1, m_strPicture, _T("Picture") );
	DDX_Text(pDX, IDC_EDIT1, m_strPicture);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CRunCtrlPropPage message handlers

void CRunCtrlPropPage::OnButtonPicture() 
{
	CFileDialog dlg(TRUE, _T("grf"), m_strPicture,
		OFN_FILEMUSTEXIST,
		_T("画面文件 (*.grf)|*.grf|所有文件 (*.*)|*.*||"));
	
	if (dlg.DoModal() != IDOK)
		return; 
	
	SetDlgItemText(IDC_EDIT1, dlg.GetPathName());
}
