// PropPage.cpp : implementation file
// 所属项目: AutoCx的CxPropSheet工程
// 创建日期: 2001.10.8
// 创 建 人：孙越洋 
// 功能描述：属性页类,用于停放ole属性页对象。 
// 修改记录：
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxPropSheet.h"
#include "PropSheet.h"
#include "PropPage.h"
#include "comhelp.h"
#include <comdef.h>

#define UNICODE
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPage property page

IMPLEMENT_DYNCREATE(CPropPage, CPropertyPage)

CPropPage::CPropPage() : CPropertyPage(CPropPage::IDD)
{
	EnableAutomation();
	//{{AFX_DATA_INIT(CPropPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_lImageIndex = -1;
}

////////////////////////////////////////////////////////////////////////////
//功能:主要用的构造函数
//参数:clsidPage[in]	需要在窗口上显示的方该页面的GUID
//		lpAryUnk[in]	对应于各页面的对象的unknown接口指针数组
//		count[in]		数组中数据的个数
////////////////////////////////////////////////////////////////////////////
CPropPage::CPropPage(CLSID clsidPage, LPUNKNOWN* lpAryUnk, int& count) : CPropertyPage(CPropPage::IDD),
	m_clsidPage(clsidPage), m_pObject(lpAryUnk)
{
	// Create COM Property page and get IPropertyPage interface
	EnableAutomation();
	HRESULT hRes = E_FAIL;
	m_pPropPage=NULL;
	m_lImageIndex = -1;
	try {
		hRes = CoCreateInstance( m_clsidPage, NULL, CLSCTX_INPROC, IID_IPropertyPage, (void**)&m_pPropPage );
		if( FAILED( hRes ) )
			AfxThrowOleException( hRes );
		
		hRes = m_pPropPage->SetPageSite( (IPropertyPageSite*) GetInterface( &IID_IPropertyPageSite ) );
		if( FAILED( hRes ) )
			AfxThrowOleException( hRes );
		
		hRes = m_pPropPage->SetObjects( count, m_pObject );
		if( FAILED( hRes ) )
			AfxThrowOleException( hRes );
		
		IMalloc     *pIMalloc;
		if (FAILED(CoGetMalloc(MEMCTX_TASK, &pIMalloc)))
			AfxThrowOleException(E_FAIL);
		
		PROPPAGEINFO* pPPI = (PROPPAGEINFO*) pIMalloc->Alloc(sizeof(PROPPAGEINFO));
		pPPI->cb = sizeof(PROPPAGEINFO);
		hRes = m_pPropPage->GetPageInfo(pPPI);
#ifndef UNICODE
		m_strCaption.Format("%S", pPPI->pszTitle);
#else
		m_strCaption = pPPI->pszTitle;
#endif
		m_psp.pszTitle = m_strCaption;
		m_psp.dwFlags |= PSP_USETITLE;
		m_psp.hIcon = ::LoadIcon(NULL, IDI_QUESTION);
		
		pIMalloc->Free(pPPI);
		pIMalloc->Release();
		
		
	} catch (COleException * e)
	{
		throw (e);
	}
}

CPropPage::~CPropPage()
{
	if (m_pPropPage)
		m_pPropPage->Release();
}

/////////////////////////////////////////////////////////////////////////////
// CPropPage message handlers
IMPLEMENT_IUNKNOWN(CPropPage, PropertySite)

STDMETHODIMP CPropPage::XPropertySite::GetLocaleID(LCID* pLocaleID)
{
	METHOD_PROLOGUE(CPropPage, PropertySite);     // Creates a pThis pointer to 
	TRACE("CPropPage::PropertySite::GetLocaleID()\n");
	*pLocaleID = ::GetThreadLocale();
	
	return (HRESULT)NOERROR;   
}

STDMETHODIMP CPropPage::XPropertySite::OnStatusChange(DWORD dwFlags)
{
	METHOD_PROLOGUE(CPropPage, PropertySite);     // Creates a pThis pointer to 
	TRACE("CPropPage::PropertySite::OnStatusChange()\n");
	
	if (dwFlags & PROPPAGESTATUS_DIRTY)
	//	pThis->m_pPropPage->Apply();
		((CPropSheet *)pThis->GetParent())->EnableApplyButton();

	return (HRESULT)NOERROR;   
}

STDMETHODIMP CPropPage::XPropertySite::GetPageContainer(IUnknown **ppUnk)
{
	METHOD_PROLOGUE(CPropPage, PropertySite);     // Creates a pThis pointer to 
	TRACE("CPropPage::PropertySite::GetPageContainer()\n");
	
	return (HRESULT)E_NOTIMPL;   
}

STDMETHODIMP CPropPage::XPropertySite::TranslateAccelerator(MSG *pMsg)
{
	METHOD_PROLOGUE(CPropPage, PropertySite);     // Creates a pThis pointer to 
	TRACE("CPropPage::PropertySite::TranslateAccelerator()\n");
	
	return (HRESULT)E_NOTIMPL;   
}

/////////////////////////////////////////////////////////////////////////
//功能:ICxPropertySite接口函数, 用于指定本页面对应的图标
//参数:index[in]	图标索引号(-1为无图标, 0无动态属性, 1有动态属, ...)
/////////////////////////////////////////////////////////////////////////
STDMETHODIMP CPropPage::XPropertySite::SetDynamicIcon(long index)
{
	METHOD_PROLOGUE(CPropPage, PropertySite);     // Creates a pThis pointer to 
	TRACE("CPropPage::PropertySite::SetDynamicIcon()\n");

	pThis->m_lImageIndex = index;
	if(pThis->GetSafeHwnd() == NULL)
		return E_FAIL;

	CPropSheet* pParent = (CPropSheet*)pThis->GetParent();
	if(pParent == NULL)
		return E_FAIL;

	pParent->OnChangeIcon();

	return 0;
}

void CPropPage::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CPropertyPage::OnFinalRelease();
}

void CPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPage)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPropPage, CPropertyPage)
	//{{AFX_DISPATCH_MAP(CPropPage)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IPropPage to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

BEGIN_INTERFACE_MAP(CPropPage, CPropertyPage)
	INTERFACE_PART(CPropPage, IID_IPropertyPageSite, PropertySite)
	INTERFACE_PART(CPropPage, IID_ICxPropertyPageSite, PropertySite)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPage message handlers

///////////////////////////////////////////////////////////////////////////
//功能:取页面的需要的大小
//////////////////////////////////////////////////////////////////////////
SIZE CPropPage::GetPageSize()
{
	PROPPAGEINFO Info;
	Info.cb = sizeof(Info);
	m_pPropPage->GetPageInfo(&Info);

	return Info.size;
}

///////////////////////////////////////////////////////////////////
//功能:取页面应显示的名称
///////////////////////////////////////////////////////////////////
CString CPropPage::GetPageTitle()
{
	PROPPAGEINFO info;
	m_pPropPage->GetPageInfo(&info);
	CString str(info.pszTitle);
	return str;
}

///////////////////////////////////////////////////////////////////////
//功能:重设对应ole属面窗口的大小
///////////////////////////////////////////////////////////////////////
void CPropPage::ResetPropRect(RECT *pRect)
{
	try
	{
		CRect pgrect;
		GetClientRect(&pgrect);
		m_pPropPage->Move(&pgrect);
		//CRect pgrect;
		//GetWindowRect(&pgrect);
		//ScreenToClient(pgrect);
		HRESULT hRes = m_pPropPage->Show( SW_SHOW );
		if( FAILED( hRes ) )
			AfxThrowOleException( hRes );
		
	} catch (COleException * e)
	{
		throw (e);
	}
	
	// Add WS_EX_CONTROLPARENT style to property page
	// necessary to allow tabbing from page to sheet.
	// Get COM Prop Page
	CWnd * pWnd = GetWindow(GW_CHILD);
	CString str;
	::GetClassName(pWnd->GetSafeHwnd(), str.GetBuffer(128), 128);
    str.ReleaseBuffer();
	if (str == (CString)_T("#32770"))
		pWnd->ModifyStyleEx(0,WS_EX_CONTROLPARENT,0);
}

BOOL CPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	try
	{
		RECT pgrect;
		GetClientRect(&pgrect);
		HRESULT hRes = m_pPropPage->Activate( GetSafeHwnd(), &pgrect, TRUE );
		if( FAILED( hRes ) )
			AfxThrowOleException( hRes );
		hRes = m_pPropPage->Show(SW_SHOW);
		if( FAILED( hRes ) )
			AfxThrowOleException( hRes );
	}
	catch (COleException * e)
	{
		throw(e);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CPropPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CPropPage::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	CRect pgrect;
	GetClientRect(&pgrect);
	m_pPropPage->Move(&pgrect);
}

BOOL CPropPage::OnApply() 
{
	HRESULT hres = m_pPropPage->Apply();
	if(FAILED(hres))
		return FALSE;

	((CPropSheet *)GetParent())->EnableApplyButton(FALSE);

	return CPropertyPage::OnApply();
}

void CPropPage::OnOK() 
{
	if (m_pPropPage->IsPageDirty() == S_OK)
		if(!OnApply())
			MessageBox("页面保存失败!");
	
	CPropertyPage::OnOK();
}

void CPropPage::OnHelp()
{
// 	m_pPropPage->Help();
}

BOOL CPropPage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
//	HRESULT hres = m_pPropPage->TranslateAccelerator(pMsg);
//	if(hres = S_OK)
//		return 1;
//	return CPropertyPage::PreTranslateMessage(pMsg);
	return 0;
}


BOOL CPropPage::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}
