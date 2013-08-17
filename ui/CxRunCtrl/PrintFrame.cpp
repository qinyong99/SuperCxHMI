// PrintFrame.cpp : implementation file
//

#include "stdafx.h"
#include "RunInc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintFrame

IMPLEMENT_DYNCREATE(CPrintFrame, CFrameWnd)

CPrintFrame::CPrintFrame()
{
}

CPrintFrame::~CPrintFrame()
{
}


BEGIN_MESSAGE_MAP(CPrintFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CPrintFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintFrame message handlers

BOOL CPrintFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	
	cs.hMenu = NULL;
	cs.style = WS_POPUPWINDOW | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	
	return TRUE;
}

BOOL CPrintFrame::ContinueModal()
{
	SendMessageToDescendants(WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0, TRUE, TRUE);
	
	return CWnd::ContinueModal();
}
