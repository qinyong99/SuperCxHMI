// Service.cpp: implementation of the CService class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Service.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CService::CService(LPCTSTR lpszServiceName, CLSID clsidLocalServer) :
	m_hService(NULL)
{
	m_strServiceName = lpszServiceName;
	memset(&m_serviceStatus, 0, sizeof(SERVICE_STATUS));

	m_bService = TRUE;

	m_bCanLocalServer = TRUE;
	m_dwLocalServerStatus = 0;

	m_localserver.m_bstrName = m_strServiceName;
	m_localserver. m_clsidLocalServer = clsidLocalServer;
}

CService::~CService()
{
	if (m_hService != NULL)
		CloseServiceHandle(m_hService);
}

DWORD CService::Open()
{
	DWORD dwRet = 0;

	if (m_hService == NULL)
	{
		SC_HANDLE hSCM = OpenSCManager(0, 0, STANDARD_RIGHTS_REQUIRED);
		
		if (hSCM != NULL)
		{
			m_hService = OpenService(hSCM, m_strServiceName, 
				SERVICE_START | SERVICE_QUERY_STATUS | 
				SERVICE_PAUSE_CONTINUE | SERVICE_STOP | SERVICE_USER_DEFINED_CONTROL);

			UpdateStatus();
		}
	
		if (m_hService == NULL)
		{
			if (m_bCanLocalServer)
				m_bService = FALSE;
			else
				dwRet = ::GetLastError();
		}
	}

	return dwRet;
}

void CService::UpdateStatus()
{
	DWORD dwRet = 0;
	if (m_bService)
	{
		if (m_hService != NULL)
		{
			if (!QueryServiceStatus(m_hService, &m_serviceStatus))
				dwRet = ::GetLastError();
		}
	}
	else
	{
		m_localserver.QueryStatus(&m_dwLocalServerStatus);
	}
}

DWORD CService::Start()
{
	DWORD dwRet = 0;
	if (m_bService)
	{
		ATLASSERT(m_hService != NULL);
	
		if (!StartService(m_hService, 0, 0)) 
			dwRet = ::GetLastError();
	}
	else
	{
		m_localserver.Start(0, 0);
	}

	UpdateStatus();

	return dwRet;
}

DWORD CService::Stop()
{
	DWORD dwRet = 0;

	if (m_bService)
	{
		ATLASSERT(m_hService != NULL);
	
		if (!ControlService(m_hService, SERVICE_CONTROL_STOP, &m_serviceStatus)) 
			dwRet = ::GetLastError();
	}
	else
	{
		m_localserver.ControlServer(CX_SERVER_CONTROL_STOP, &m_dwLocalServerStatus);
	}
		
	return dwRet;
}

DWORD CService::Pause()
{
	DWORD dwRet = 0;

	if (m_bService)
	{
		ATLASSERT(m_hService != NULL);
	
		if (!ControlService(m_hService, SERVICE_CONTROL_PAUSE, &m_serviceStatus)) 
			dwRet = ::GetLastError();
	}
	else
	{
		m_localserver.ControlServer(CX_SERVER_CONTROL_PAUSE, &m_dwLocalServerStatus);
	}
	
	return dwRet;
}

#define SERVICE_CONTROL_UPDATE_ALL 128
#define SERVICE_CONTROL_SETUP_MODIFIED 129

DWORD CService::Refresh()
{
	DWORD dwRet = 0;

	if (m_bService)
	{
		ATLASSERT(m_hService != NULL);

		if (m_hService != NULL && ControlService(m_hService, SERVICE_CONTROL_UPDATE_ALL, &m_serviceStatus)) 
			dwRet = ::GetLastError();
	}
	else
	{
		m_localserver.ControlServer(CX_SERVER_CONTROL_UPDATE_ALL, &m_dwLocalServerStatus);
	}
	
	return dwRet;
}

DWORD CService::SetModified()
{
	DWORD dwRet = 0;

	if (m_bService)
	{
		ATLASSERT(m_hService != NULL);
		if (m_hService != NULL && GetCurrentState() == SERVICE_RUNNING)
		{
			if (!ControlService(m_hService, SERVICE_CONTROL_SETUP_MODIFIED, &m_serviceStatus)) 
				dwRet = ::GetLastError();
		}
	}
	else
	{
		m_localserver.ControlServer(CX_SERVER_CONTROL_SETUP_MODIFIED, &m_dwLocalServerStatus);
	}
	
	return dwRet;
}

DWORD CService::GetCurrentState()
{ 
	UpdateStatus();

	if (m_bService)
		return m_serviceStatus.dwCurrentState;

	return m_dwLocalServerStatus;
}