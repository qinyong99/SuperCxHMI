#if !defined(AFX_SERVERNOTIFYWND_H__2582AE0E_83D7_4BB7_B2E6_8A60B46CA5E7__INCLUDED_)
#define AFX_SERVERNOTIFYWND_H__2582AE0E_83D7_4BB7_B2E6_8A60B46CA5E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerNotifyWnd.h : header file
//

#define WM_SERVER_DISCONNECTED WM_USER + 100

/////////////////////////////////////////////////////////////////////////////
// CServerNotifyWnd window

class CServerNotifyWnd : public CWnd
{
	// Construction
public:
	CServerNotifyWnd();
	
protected:
	//{{AFX_MSG(CServerNotifyWnd)
	LRESULT OnServerDisconnected(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERNOTIFYWND_H__2582AE0E_83D7_4BB7_B2E6_8A60B46CA5E7__INCLUDED_)
