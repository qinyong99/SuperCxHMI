// OlePropPage.cpp : implementation file
//

#include "stdafx.h"
#include "OlePropPage.h"
#include "comhelp.h"
#include <comdef.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COlePropPage property page

IMPLEMENT_DYNCREATE(COlePropPage, CPropertyPage)

COlePropPage::COlePropPage() : CPropertyPage(COlePropPage::IDD)
{
	EnableAutomation();
	//{{AFX_DATA_INIT(COlePropPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COlePropPage::COlePropPage(CLSID clsidPage, LPUNKNOWN* lpAryUnk, int& count) : CPropertyPage(COlePropPage::IDD),
	m_clsidPage(clsidPage), m_pObject(lpAryUnk)
{
	// Create COM Property page and get IPropertyPage interface
	EnableAutomation();
	HRESULT hRes = E_FAIL;
	m_pPropPage=NULL;
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


COlePropPage::~COlePropPage()
{
	// Release COM Property Page
	if (m_pPropPage)
		m_pPropPage->Release();
}

void COlePropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COlePropPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COlePropPage, CPropertyPage)
	//{{AFX_MSG_MAP(COlePropPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(COlePropPage, CPropertyPage)
	//{{AFX_DISPATCH_MAP(COlePropPage)
	DISP_FUNCTION(COlePropPage, "SetDynamicIcon", SetDynamicIcon, VT_I4, VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// {7BDB75D9-323E-4aac-B183-0B7495E144C6}
static const GUID <<IID_IOlePropPage>> = 
{ 0x7bdb75d9, 0x323e, 0x4aac, { 0xb1, 0x83, 0xb, 0x74, 0x95, 0xe1, 0x44, 0xc6 } };

BEGIN_INTERFACE_MAP(COlePropPage, CPropertyPage)
	INTERFACE_PART(COlePropPage, IID_IOlePropPage, Dispatch)
	INTERFACE_PART(COlePropPage, IID_IPropertyPageSite, PropertySite)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COlePropPage message handlers
IMPLEMENT_IUNKNOWN(COlePropPage, PropertySite)
IMPLEMENT_IUNKNOWN(COlePropPage, OlePropSiteCtrl)


STDMETHODIMP COlePropPage::XPropertySite::GetLocaleID(LCID* pLocaleID)
{
	METHOD_PROLOGUE(COlePropPage, PropertySite);     // Creates a pThis pointer to 
	TRACE("COlePropPage::PropertySite::GetLocaleID()\n");
	*pLocaleID = ::GetThreadLocale();
	
	return (HRESULT)NOERROR;   
}

STDMETHODIMP COlePropPage::XPropertySite::OnStatusChange(DWORD dwFlags)
{
	METHOD_PROLOGUE(COlePropPage, PropertySite);     // Creates a pThis pointer to 
	TRACE("COlePropPage::PropertySite::OnStatusChange()\n");
	
	if (dwFlags & PROPPAGESTATUS_DIRTY)
		pThis->m_pPropPage->Apply();

	return (HRESULT)NOERROR;   
}

STDMETHODIMP COlePropPage::XPropertySite::GetPageContainer(IUnknown **ppUnk)
{
	METHOD_PROLOGUE(COlePropPage, PropertySite);     // Creates a pThis pointer to 
	TRACE("COlePropPage::PropertySite::GetPageContainer()\n");
	
	return (HRESULT)E_NOTIMPL;   
}

STDMETHODIMP COlePropPage::XPropertySite::TranslateAccelerator(MSG *pMsg)
{
	METHOD_PROLOGUE(COlePropPage, PropertySite);     // Creates a pThis pointer to 
	TRACE("COlePropPage::PropertySite::TranslateAccelerator()\n");
	
	return (HRESULT)E_NOTIMPL;   
}

/////////////////////////////////////////////////////////////////////////////
// COlePropPage message handlers
BOOL COlePropPage::OnKillActive() 
{
	m_pPropPage->Apply();
	
	return CPropertyPage::OnKillActive();
}

///////////////////////////////////////////////////////////////////////////
//功能:取页面的需要的大小
//////////////////////////////////////////////////////////////////////////
SIZE COlePropPage::GetPageSize()
{
	PROPPAGEINFO Info;
	Info.cb = sizeof(Info);
	m_pPropPage->GetPageInfo(&Info);

	return Info.size;
}

///////////////////////////////////////////////////////////////////////
//功能:重设对应ole属面窗口的大小
///////////////////////////////////////////////////////////////////////
void COlePropPage::ResetPropRect(RECT *pRect)
{
	HRESULT hRes = E_FAIL;
	
	try {
		CRect pgrect;
		GetWindowRect(&pgrect);
		ScreenToClient(pgrect);
		hRes = m_pPropPage->Activate( GetSafeHwnd(), pgrect, TRUE );
		if( FAILED( hRes ) )
			AfxThrowOleException( hRes );
		
		hRes = m_pPropPage->Show( SW_SHOW );
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

///////////////////////////////////////////////////////////////////
//功能:取页面应显示的名称
///////////////////////////////////////////////////////////////////
CString COlePropPage::GetPageTitle()
{
	PROPPAGEINFO info;
	m_pPropPage->GetPageInfo(&info);
	CString str(info.pszTitle);
	return str;
}



long COlePropPage::SetDynamicIcon(long index) 
{
	// TODO: Add your dispatch handler code here

	return 0;
}
