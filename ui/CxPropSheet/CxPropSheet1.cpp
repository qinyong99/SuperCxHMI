// CxPropSheet1.cpp : implementation file
//

#include "stdafx.h"
#include "CxPropSheet.h"
#include "CxPropSheet1.h"

#include "OlePropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCxPropSheet

IMPLEMENT_DYNAMIC(CCxPropSheet, CPropertySheet)

CCxPropSheet::CCxPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_aryUnk = NULL;
	m_nObjCounts = 0;
	m_ClsIDs = NULL;
	m_iPageCnt = 0;
	m_psh.dwFlags |= PSH_HASHELP;
	EnableAutomation();
}

CCxPropSheet::~CCxPropSheet()
{
	for (UINT i = 0; i < m_iPageCnt; i++)
	{
		delete ((COlePropPage *)m_pageArray[i]);			
	}

	for(i = 0; i < (UINT)m_nObjCounts; i++)
	{
		m_aryUnk[i]->Release();
	}
	delete [] m_aryUnk;

	if(m_ClsIDs != NULL)
		delete [] m_ClsIDs;
}


BEGIN_MESSAGE_MAP(CCxPropSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CCxPropSheet)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CCxPropSheet, CPropertySheet)
	//{{AFX_DISPATCH_MAP(COlePropSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

static const IID IID_ICxPropSheet =
{ 0x828ce900, 0x15ee, 0x11d3, { 0xae, 0x81, 0x0, 0x10, 0x5a, 0xa2, 0x2f, 0x40 } };

BEGIN_INTERFACE_MAP(CCxPropSheet, CPropertySheet)
//	INTERFACE_PART(CCxPropSheet, IID_ICxPropSheet, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCxPropSheet message handlers

BOOL CCxPropSheet::Create(CWnd* pParentWnd, DWORD dwStyle, DWORD dwExStyle)
{
	// TODO: Add your specialized code here and/or call the base class
	ASSERT(m_aryUnk != NULL);

	//加入属性页
	for (UINT i = 0; i < m_iPageCnt; i++)
	{
		COlePropPage * pPage = new COlePropPage(m_ClsIDs[i], m_aryUnk, m_nObjCounts);
		if (pPage == NULL)
			return FALSE;
		m_pageArray.Add(pPage);
		AddPage((COlePropPage*)m_pageArray[i]);			
	}
	
	return CPropertySheet::Create(pParentWnd, dwStyle, dwExStyle);
}

////////////////////////////////////////////////////////////////
//功能:在对象中加入可能用到的OLE对象指针数组
////////////////////////////////////////////////////////////////
BOOL CCxPropSheet::AddObjects(ULONG count, LPUNKNOWN *pArray)
{
	m_aryUnk = new LPUNKNOWN[count];
	if(m_aryUnk == NULL)
		return FALSE;

	for(ULONG i = 0; i < count; i++)
	{
		m_aryUnk[i] = pArray[i];
		m_aryUnk[i]->AddRef();
	}

	m_nObjCounts = (int)count;
	return TRUE;
}

//////////////////////////////////////////////////////////////////
//功能:加入属性页的CLSID数组
//////////////////////////////////////////////////////////////////
BOOL CCxPropSheet::AddPageCLSID(ULONG count, LPCLSID pArray)
{
	m_ClsIDs = new CLSID[count];
	if(m_ClsIDs == NULL)
		return FALSE;

	for(ULONG i = 0; i < count; i++)
	{
		m_ClsIDs[i] = pArray[i];
	}

	m_iPageCnt = (int)count;
	return TRUE;
}

void CCxPropSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet::PostNcDestroy();

	delete this;
}

void CCxPropSheet::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CPropertySheet::OnLButtonDown(nFlags, point);
}

BOOL CCxPropSheet::OnInitDialog() 
{
	CPropertySheet::OnInitDialog();
	
	// TODO: Add extra initialization here
	int count = this->GetPageCount();
	SIZE size = {0, 0};
	for(int i = 0; i < count; i++)
	{
		COlePropPage* pPage = (COlePropPage *)GetPage(i);
		SIZE tmpSize = pPage->GetPageSize();
		if(tmpSize.cx > size.cx)
			size.cx = tmpSize.cx;
		if(tmpSize.cy > size.cy)
			size.cy = tmpSize.cy;
	}

	//调整主窗口大小
	CRect rect;
	GetTabControl()->GetItemRect(0, &rect);
	int tabHeight = rect.Height();
	int tabUnusedBottom = 50;
	int dxdy = 10;
	GetWindowRect(&rect);
	rect.right = rect.left + size.cx + dxdy * 4;
	rect.bottom = rect.top + size.cy + tabHeight + tabUnusedBottom + dxdy * 3;
	MoveWindow(&rect);

	//显示控件
	ShowButtons();

	//调整TAB的大小
	GetClientRect(&rect);
	rect.DeflateRect(dxdy, dxdy, dxdy, tabUnusedBottom);
	GetTabControl()->MoveWindow(&rect);	

	//调整页面的大小
	GetTabControl()->GetClientRect(rect);
	rect.left += dxdy;
	rect.right += dxdy;
	rect.top += dxdy;
	rect.bottom += dxdy;
	rect.DeflateRect(dxdy, tabHeight + dxdy, dxdy, dxdy);
	for(i = 0; i < count; i++)
	{
		COlePropPage* pPage = (COlePropPage *)GetPage(i);
		pPage->MoveWindow(&rect);
		pPage->ResetPropRect(&rect);

		TCITEM itemInfo;
		itemInfo.mask = TCIF_TEXT;
		CString str = pPage->GetPageTitle();
		LPTSTR pBuffer = str.LockBuffer();
		itemInfo.pszText = pBuffer;
		GetTabControl()->SetItem(i, &itemInfo);
		str.UnlockBuffer();
	};

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

////////////////////////////////////////////////////////////////
//功能:	移动几个按键到指定位置
//参数: nID[in]		按钮窗口的ID号
//		x[in]		按钮的x坐标
/////////////////////////////////////////////////////////////////
void CCxPropSheet::MoveButton(UINT nID, int x)
{
	CWnd* pWnd = GetDlgItem(nID);
	if(pWnd == NULL)
		return;

	CRect rect;
	GetClientRect(&rect);
	CRect tmpRect;
	pWnd->GetWindowRect(&tmpRect);
	ScreenToClient(&tmpRect);

	int h = tmpRect.bottom - tmpRect.top;
	int w = tmpRect.Width();
	tmpRect.left = x;
	tmpRect.right = x + w;
	tmpRect.top = rect.bottom - 40;
	tmpRect.bottom = tmpRect.top + h;

	pWnd->MoveWindow(&tmpRect);
	pWnd->ShowWindow(SW_SHOW);
}

/////////////////////////////////////////////////////////////
//功能:显示所有的ProperSheet上的Button
/////////////////////////////////////////////////////////////
void CCxPropSheet::ShowButtons()
{
	CRect rect;
	CRect tmpRect;
	CWnd* pWnd = GetDlgItem(IDOK);
	if(pWnd == NULL)
		return;
	pWnd->GetClientRect(&tmpRect);
	GetClientRect(&rect);


	int step = tmpRect.Width();
	int ntmp = (rect.Width() - 4 * tmpRect.Width()) / 5 ;
	int x = rect.right - ntmp;

	x -= step;
	MoveButton(0x9, x);
	x -= step + ntmp;
	MoveButton(0x3021, x);
	x -= step + ntmp;
	MoveButton(IDCANCEL, x);
	x -= step + ntmp;
	MoveButton(IDOK, x);
}
