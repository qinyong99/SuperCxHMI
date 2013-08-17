// ExplorerBar.cpp: implementation of the CExplorerBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\CxSCBar\SCBEXT.h"
#include "BarListCtrl.h"
#include "SymbolLibWnd.h"
#include "ModuleWnd.h"
#include "DevInc.h"
#include "Explorertree.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////
//箭头操作用数
#define LARROW		0
#define RARROW		1
#define STATE_NORMAL	1
#define STATE_DOWN		2
#define STATE_DISABLE	4

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExplorerBar::CExplorerBar()
{
}

CExplorerBar::~CExplorerBar()
{

}

BEGIN_MESSAGE_MAP(CExplorerBar, CTabBar)
    //{{AFX_MSG_MAP(CExplorerBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyBar message handlers
////////////////////////////////////////////////////////
//功能:在本工具中加入各种工具页面
//返回:成功TRUE,失败FALSE
////////////////////////////////////////////////////////
void CExplorerBar::OnAddPages()
{
	CImageList *pImage = new CImageList();
	pImage->Create(IDB_EXPLORER_ICONS, 16, 4, RGB(255, 255, 255));
	SetImageList(pImage);

	int id = 0;
	CRect rect;
	rect.SetRectEmpty();

	CWnd* pWnd = new CExplorerTree();
	if(pWnd == NULL)
		AfxThrowMemoryException();
	if (!((CExplorerTree *)pWnd)->Create(WS_VISIBLE | WS_CHILD | TVS_SHOWSELALWAYS ,
			rect, this, ID_EXPLORER_WND))
		return;
	AddPage(pWnd, "工程", id++);

	pWnd = new CSymbolLibWnd();
	if(pWnd == NULL)
		AfxThrowMemoryException();
	if(!((CSymbolLibWnd *)pWnd)->Create(rect, this, ID_SYMBOL_WND))
		return;
	AddPage(pWnd, _T("图符"), id++);

	pWnd = new CModuleWnd();
	if(pWnd == NULL)
		AfxThrowMemoryException();
	if(!((CModuleWnd *)pWnd)->Create(rect, this, ID_MODULE_WND))
		return;
	AddPage(pWnd, _T("模块"), id++);
}

////////////////////////////////////////////////////////
//功能:取nWhich指定的工具窗口对象
//参数:nWhich[in]		工具代号(见头文件)
//返回:成功窗口对象指针,失败NULL
////////////////////////////////////////////////////////
CWnd* CExplorerBar::GetToolWnd(int nWhich)
{
	if(nWhich < 0 || nWhich >= EXPBAR_NULL)
		return NULL;

	return m_listChildWnd[nWhich].pPageWnd;
}

///////////////////////////////////////////////////////
//功能:关闭所有的工具子窗体
///////////////////////////////////////////////////////
void CExplorerBar::DestroyAllChild()
{
	OnDestroyAllChild();
}
