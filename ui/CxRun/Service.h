// Service.h: interface for the CService class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVICE_H__12CAF672_297F_4F24_A4CF_091ACB4DC50B__INCLUDED_)
#define AFX_SERVICE_H__12CAF672_297F_4F24_A4CF_091ACB4DC50B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LocalServer.h"

class CService  
{
public:
	CService(LPCTSTR lpszServiceName = NULL, CLSID clsidLocalServer = CLSID_NULL);
	virtual ~CService();

	DWORD Open();

	DWORD Start();
	DWORD Stop();
	DWORD Pause();
	DWORD Refresh();
	DWORD SetModified();

	void UpdateStatus();

	DWORD GetCurrentState(); 

private:
	CString m_strServiceName;
	SC_HANDLE m_hService;
	SERVICE_STATUS m_serviceStatus;

	BOOL m_bService;

	BOOL m_bCanLocalServer;
	CLocalServer m_localserver;
	DWORD m_dwLocalServerStatus;
};

#endif // !defined(AFX_SERVICE_H__12CAF672_297F_4F24_A4CF_091ACB4DC50B__INCLUDED_)
