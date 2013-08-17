// RunFrame.cpp : implementation file
//

#include "stdafx.h"
#include "CxRunCtrl.h"
#include "RunFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunFrame

IMPLEMENT_DYNCREATE(CRunFrame, CFrameWnd)

CRunFrame::CRunFrame()
{
}

CRunFrame::~CRunFrame()
{
}


BEGIN_MESSAGE_MAP(CRunFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CRunFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunFrame message handlers

BOOL CRunFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

//	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
		
	return TRUE;
}
