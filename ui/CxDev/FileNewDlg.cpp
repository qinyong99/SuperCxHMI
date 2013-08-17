// FileNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cxdev.h"
#include "FileNewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileNewDlg dialog


CFileNewDlg::CFileNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileNewDlg)
	m_strName = _T("");
	m_strDesc = _T("");
	m_nWidth = 0;
	m_nHeight = 0;
	m_strBrushFile = _T("");
	m_bNextShow = FALSE;
	//}}AFX_DATA_INIT
}


void CFileNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileNewDlg)
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Text(pDX, IDC_EDIT2, m_strDesc);
	DDX_Text(pDX, IDC_EDIT3, m_nWidth);
	DDX_Text(pDX, IDC_EDIT4, m_nHeight);
	DDX_Text(pDX, IDC_EDIT6, m_strBrushFile);
	DDX_Check(pDX, IDC_CHECK1, m_bNextShow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileNewDlg, CDialog)
	//{{AFX_MSG_MAP(CFileNewDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileNewDlg message handlers


