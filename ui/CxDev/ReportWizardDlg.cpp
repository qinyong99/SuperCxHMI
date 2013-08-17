// ReportWizardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DevInc.h"
#include "ReportFieldDlg.h"
#include "ReportLayoutDlg.h"
#include "ReportWizardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportWizardDlg dialog


CReportWizardDlg::CReportWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReportWizardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportWizardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nCurrentPage = -1;
	m_pDialog = NULL;

	m_nReportLayout = ReportLayout_Tabular;
	m_nReportOrientation = ReportOrientation_Portrait;
}


void CReportWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportWizardDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportWizardDlg, CDialog)
	//{{AFX_MSG_MAP(CReportWizardDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDC_FINISH, OnFinish)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_INTERFACE_MAP(CReportWizardDlg, CDialog)
	INTERFACE_PART(CReportWizardDlg, IID_IPropertyPageSite, PageSite)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////
//pageSite接口函数
STDMETHODIMP_(ULONG) CReportWizardDlg::XPageSite::AddRef()
{
	METHOD_PROLOGUE(CReportWizardDlg, PageSite)
		
	return pThis->ExternalAddRef();
}

STDMETHODIMP CReportWizardDlg::XPageSite::QueryInterface(REFIID iid, void** ppvObj)
{
	METHOD_PROLOGUE(CReportWizardDlg, PageSite)
		
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP_(ULONG) CReportWizardDlg::XPageSite::Release()
{
	METHOD_PROLOGUE(CReportWizardDlg, PageSite)
		
	return pThis->ExternalRelease();
}

STDMETHODIMP CReportWizardDlg::XPageSite::OnStatusChange(DWORD dwFlags)
{
	METHOD_PROLOGUE(CReportWizardDlg, PageSite)

	switch(dwFlags)
	{
	case PROPPAGESTATUS_DIRTY:
		pThis->m_arrFields.RemoveAll();	

		break;
	case PROPPAGESTATUS_VALIDATE:
		break;
	default:
		break;
	}


	return S_OK;
}

STDMETHODIMP CReportWizardDlg::XPageSite::GetLocaleID(LCID* pLocaleID)
{
	if (pLocaleID == NULL)
		return E_POINTER;

	*pLocaleID = LOCALE_SYSTEM_DEFAULT;
	return S_OK;
}

STDMETHODIMP CReportWizardDlg::XPageSite::GetPageContainer(IUnknown** ppUnk)
{
	METHOD_PROLOGUE(CReportWizardDlg, PageSite)

	return E_NOTIMPL;
}

STDMETHODIMP CReportWizardDlg::XPageSite::TranslateAccelerator(LPMSG pMsg)
{
	METHOD_PROLOGUE(CReportWizardDlg, PageSite)
		
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CReportWizardDlg message handlers

BOOL CReportWizardDlg::ShowDialogPage(CRuntimeClass* pDialogClass, UINT nIDTemplate)
{
	ASSERT(m_pDialog == NULL);

	ASSERT(pDialogClass != NULL);
	m_pDialog = (CDialog *)pDialogClass->CreateObject();
	ASSERT_KINDOF(CDialog, m_pDialog);

	if (!m_pDialog->Create(nIDTemplate, this))
		return FALSE;

	CRect rcWindow;
	GetDlgItem(IDC_RECT)->GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);
	CRect rect;
	m_pDialog->GetWindowRect(&rect);
	rect.OffsetRect(rcWindow.CenterPoint() - rect.CenterPoint());
	m_pDialog->MoveWindow(rect);
	m_pDialog->ShowWindow(SW_SHOW);

	return TRUE;
}

HRESULT CReportWizardDlg::ShowOleProertyPage(CLSID clsidPage, IUnknown* pUnk)
{
	ASSERT(m_pPropertyPage == NULL);

	HRESULT hr = m_pPropertyPage.CreateInstance(clsidPage);
	if (FAILED(hr))
		return hr;
	
	CComPtr<IPropertyPageSite> spPageSite;
	m_pPropertyPage->SetPageSite(&m_xPageSite);
	
	m_pPropertyPage->SetObjects(1, &pUnk);
	
	PROPPAGEINFO proppageinfo;
	m_pPropertyPage->GetPageInfo(&proppageinfo);
	SIZE size = proppageinfo.size;
	
	// 重新定位对话框
//	::SetWindowPos(m_hWnd, NULL, 0, 0, size.cx, size.cy + 50, SWP_NOZORDER|SWP_NOMOVE);
	
	CRect rcWindow;
	GetDlgItem(IDC_RECT)->GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);
	CRect rect(0, 0, size.cx, size.cy - 10);
	rect.OffsetRect(rcWindow.CenterPoint() - rect.CenterPoint());
		
	m_pPropertyPage->Activate(m_hWnd, &rect, TRUE);
	
	// 重新定位对话框对象
//	::SetWindowPos(GetDlgItem(IDOK), NULL, rect.right - 180, rect.bottom + 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
//	::SetWindowPos(GetDlgItem(IDCANCEL), NULL, rect.right - 100, rect.bottom + 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	
	m_pPropertyPage->Show(SW_SHOW);
	
	return S_OK;
}

const CLSID CLSID_CxDataConnectionPage = {0x6E390124,0x837E,0x447D,{0xAD,0x60,0x29,0xA8,0x4E,0x6E,0xDB,0x19}};
const CLSID CLSID_CxDataRecordsetPage = {0xD1E7A6A7,0x609E,0x4C65,{0x8B,0x81,0x8C,0x82,0x85,0x90,0x48,0x93}};

BOOL CReportWizardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 创建数据环境
	VERIFY(m_pDataConnection.CreateInstance(CLSID_CxDataConnection) == S_OK);
	
	IPersistStreamInitPtr pPersistStreamInit = m_pDataConnection;			
	ASSERT(pPersistStreamInit != NULL);
	pPersistStreamInit->InitNew();
	
	VERIFY(m_pDataRecordset.CreateInstance(CLSID_CxDataRecordset) == S_OK);
	
	m_pDataRecordset->put_UseLocalSetup(VARIANT_TRUE);
	CComVariant var = (IUnknown *)m_pDataConnection;
	m_pDataRecordset->put_ActiveConnection(var);
	
	pPersistStreamInit = m_pDataRecordset;
	ASSERT(pPersistStreamInit != NULL);
	pPersistStreamInit->InitNew();
	
	m_nCurrentPage = 0;
	OnShowCurrentPage();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportWizardDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pPropertyPage != NULL)
	{
		m_pPropertyPage->Deactivate();
		m_pPropertyPage.Release();
	}

	if (m_pDialog != NULL)
	{
		m_pDialog->SendMessage(CXMSG_DIALOG_APPLY);
		m_pDialog->DestroyWindow();
		delete m_pDialog;
		m_pDialog = NULL;
	}
}

void CReportWizardDlg::OnNext() 
{
	if (m_pPropertyPage != NULL)
	{
		m_pPropertyPage->Apply();
		m_pPropertyPage->Deactivate();
		m_pPropertyPage.Release();
	}

	if (m_pDialog != NULL)
	{
		m_pDialog->SendMessage(CXMSG_DIALOG_APPLY);
		m_pDialog->DestroyWindow();
		delete m_pDialog;
		m_pDialog = NULL;
	}
	
	m_nCurrentPage++;
	OnShowCurrentPage();
}

void CReportWizardDlg::OnBack() 
{
	if (m_pPropertyPage != NULL)
	{
		m_pPropertyPage->Apply();
		m_pPropertyPage->Deactivate();
		m_pPropertyPage.Release();
	}

	if (m_pDialog != NULL)
	{
		m_pDialog->SendMessage(CXMSG_DIALOG_APPLY);
		m_pDialog->DestroyWindow();
		delete m_pDialog;
		m_pDialog = NULL;
	}

	m_nCurrentPage--;
	OnShowCurrentPage();
}

void CReportWizardDlg::OnFinish() 
{
	if (m_pPropertyPage != NULL)
	{
		m_pPropertyPage->Apply();
		m_pPropertyPage->Deactivate();
		m_pPropertyPage.Release();
	}

	if (m_pDialog != NULL)
	{
		m_pDialog->SendMessage(CXMSG_DIALOG_APPLY);
		m_pDialog->DestroyWindow();
		delete m_pDialog;
		m_pDialog = NULL;
	}

	EndDialog(IDOK);
}

void CReportWizardDlg::OnShowCurrentPage()
{
	switch (m_nCurrentPage) 
	{
	case 0:
		ShowOleProertyPage(CLSID_CxDataConnectionPage, m_pDataConnection);
		SetWindowText(_T("报表向导——连接源"));
		break;
	case 1:
		ShowOleProertyPage(CLSID_CxDataRecordsetPage, m_pDataRecordset);
		SetWindowText(_T("报表向导——记录集"));
		break;
	case 2:
 		ShowDialogPage(RUNTIME_CLASS(CReportFieldDlg), IDD_REPORT_FIELD);
		SetWindowText(_T("报表向导——字段"));
		break;
	case 3:
		ShowDialogPage(RUNTIME_CLASS(CReportLayoutDlg), IDD_REPORT_LAYOUT);
		SetWindowText(_T("报表向导——格式"));
		break;
	default:
		break;
	}

	GetDlgItem(IDC_BACK)->EnableWindow(m_nCurrentPage > 0);
	BOOL bLastPage = m_nCurrentPage >= 3;
	GetDlgItem(IDC_NEXT)->ShowWindow(!bLastPage ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_FINISH)->ShowWindow(bLastPage ? SW_SHOW : SW_HIDE);
}
