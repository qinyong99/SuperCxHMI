#if !defined(AFX_NOTIFYWND_H__D4E25698_9970_41B8_98FC_BDB5671CA5C8__INCLUDED_)
#define AFX_NOTIFYWND_H__D4E25698_9970_41B8_98FC_BDB5671CA5C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotifyWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNotifyWnd window

class CNotifyWnd : public CWnd
{
	// Construction
public:
	CNotifyWnd();
	
protected:
	//{{AFX_MSG(CSocketWnd)
	LRESULT OnServerDiscnet(WPARAM wParam, LPARAM lParam);
	LRESULT OnSocketDead(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTIFYWND_H__D4E25698_9970_41B8_98FC_BDB5671CA5C8__INCLUDED_)
