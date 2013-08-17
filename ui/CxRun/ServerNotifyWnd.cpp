// ServerNotifyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CxRun.h"
#include "ServerNotifyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerNotifyWnd

CServerNotifyWnd::CServerNotifyWnd()
{
}

BEGIN_MESSAGE_MAP(CServerNotifyWnd, CWnd)
	//{{AFX_MSG_MAP(CServerNotifyWnd)
	ON_MESSAGE(WM_SERVER_DISCONNECTED, OnServerDisconnected)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CServerNotifyWnd message handlers

LRESULT CServerNotifyWnd::OnServerDisconnected(WPARAM wParam, LPARAM lParam)
{
	theApp.SwitchServer();

	return 0;
}
