// LocalServer.cpp: implementation of the CLocalServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LocalServer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLocalServer::CLocalServer()
{
	m_clsidLocalServer = CLSID_NULL;

	m_pfnServerProc = NULL;
	m_hServerThread = NULL;
	m_pfnHandlerProc = NULL;

	m_dwStatus = CX_SERVER_STOPPED; // 初始为停止状态

	m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CLocalServer::~CLocalServer()
{
	if (m_hStopEvent != NULL)
		SetEvent(m_hStopEvent);

	if (m_hServerThread != NULL)
	{
		WaitForSingleObject(m_hServerThread, 10000);
		CloseHandle(m_hServerThread);
		m_hServerThread = NULL;
	}

	if (m_hStopEvent != NULL)
		CloseHandle(m_hStopEvent);
}

BOOL CLocalServer::StartCtrlDispatcher(LPCTSTR lpszName, ServerMainProc pfnServerProc, BOOL bStartServerPorc)
{
	m_bstrName = lpszName;
	m_pfnServerProc = pfnServerProc;

	if (bStartServerPorc)
		m_hServerThread = ::CreateThread(NULL, 0, _ServerThreadProc, this, 0, NULL);

	if (ControlDispatcher() != 0)
		return FALSE;

	return TRUE;
}

void CLocalServer::GetPipeName(CComBSTR& bstrName)
{
	bstrName = L"\\\\.\\pipe\\CxBackgroundServer_";
	bstrName += m_bstrName;
}

DWORD CLocalServer::ControlDispatcher()
{
	USES_CONVERSION;

	HANDLE hPipe = INVALID_HANDLE_VALUE;
    HANDLE hEvents[2] = {NULL, NULL};
	OVERLAPPED os;
	DWORD dwWait;
	BOOL bRet;

	CComBSTR bstrPipeName;
	GetPipeName(bstrPipeName);

	hPipe = CreateNamedPipe(W2T(bstrPipeName), FILE_FLAG_OVERLAPPED | PIPE_ACCESS_DUPLEX,
                  PIPE_TYPE_BYTE|PIPE_WAIT, 1, 256, 256, 10000, NULL);
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		ATLTRACE(_T("failed to create pipe!"));
		return 1;
	}

	// 通知客户进程已经准备好
	HANDLE hEevent = GetEventHandle();
    SetEvent(hEevent);
    CloseHandle(hEevent);

	hEvents[0] = m_hStopEvent;
	hEvents[1] = CreateEvent(NULL, TRUE, FALSE, NULL); 
    if (hEvents[1] == NULL)
        goto cleanup;

	while (1)
	{
		memset(&os, 0, sizeof(OVERLAPPED));
        os.hEvent = hEvents[1];
        ResetEvent(hEvents[1]);

	    ConnectNamedPipe(hPipe, &os);

        if (GetLastError() == ERROR_IO_PENDING)
        {
            dwWait = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
            if (dwWait != WAIT_OBJECT_0 + 1)     // not overlapped i/o event - error occurred,
                break;                           // or server stop signaled
        }

		DWORD cbRead;
		DWORD buff[2] = {0, 0};

		memset(&os, 0, sizeof(OVERLAPPED));
        os.hEvent = hEvents[1];
        ResetEvent(hEvents[1]);

		bRet = ReadFile(hPipe, &buff, sizeof(buff), &cbRead, &os);

        if (!bRet && (GetLastError() == ERROR_IO_PENDING))
        {
            dwWait = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
            if (dwWait != WAIT_OBJECT_0 + 1)     // not overlapped i/o event - error occurred,
                break;                           // or server stop signaled
        }

		switch (buff[0])
        {
		case 1:
			ServerStart(hPipe, buff[1]);
			break;
		case 2:
			ServerGetStatus(hPipe);
			break;
		case 3:
			ServerControl(hPipe, buff[1]);
			break;
			
        default:
            ATLASSERT(false);
        }

		FlushFileBuffers(hPipe);
        DisconnectNamedPipe(hPipe);
	}

cleanup:
	if (hPipe != INVALID_HANDLE_VALUE )
        CloseHandle(hPipe);

	if (hEvents[1]) // overlapped i/o event
        CloseHandle(hEvents[1]);	
	
	return 0;
}

DWORD _stdcall CLocalServer::_ServerThreadProc(LPVOID pParam)
{
	return ((CLocalServer *)pParam)->ServerThreadProc();
}

DWORD CLocalServer::ServerThreadProc()
{
	USES_CONVERSION;

	LPWSTR psz = m_bstrServerThreadArgs;
    DWORD argc = 0;
	int nLen = 0;
	LPTSTR pszT;
	LPTSTR* argv;
	LPTSTR p;

	if (psz != NULL)
	{
		while (psz[nLen])
		{
			nLen += wcslen(&psz[nLen]) + 1;
			argc++;
		}
	}

	if (!argc)
    {
	    m_pfnServerProc(0, NULL);
        goto onexit;
    }
    
	pszT = W2T(psz);
	argv = (LPTSTR*)alloca((argc + 1) * sizeof(LPTSTR*));
	p;

	for (argc = 0, p = pszT; *p; p += strlen(p) + 1)
        argv[argc++] = p;
    argv[argc] = NULL;

	m_pfnServerProc(argc, argv);

onexit:
	if (m_hStopEvent != NULL)
		SetEvent(m_hStopEvent);
	
	return 0;
}

void CLocalServer::RegisterCtrlHandler(ServerHandlerProc pfnHandlerProc)
{
	m_pfnHandlerProc = pfnHandlerProc;
}

BOOL CLocalServer::SetStatus(DWORD dwStatus)
{
	m_dwStatus = dwStatus;
	return TRUE;
}

BOOL CLocalServer::ServerStart(HANDLE hPipe, DWORD dwCount)
{
	BOOL bRet;
	DWORD dwResult = 0;
	
	LPWSTR pszArgs = (LPWSTR)alloca(dwCount * sizeof(WCHAR));
	DWORD cbRead = 0;
	bRet = ReadFile(hPipe, pszArgs, dwCount * sizeof(WCHAR), &cbRead, NULL);
	if (!bRet || dwCount != cbRead / sizeof(WCHAR) || pszArgs[dwCount - 1])
		goto onexit;

	if (m_hServerThread != NULL)
		goto onexit;

	m_bstrServerThreadArgs = pszArgs;
	m_hServerThread = ::CreateThread(NULL, 0, _ServerThreadProc, this, 0, NULL);

onexit:
	WriteFile(hPipe, &dwResult, sizeof(DWORD), &cbRead, NULL);

	return TRUE;
}

BOOL CLocalServer::ServerGetStatus(HANDLE hPipe)
{
	DWORD cbWrite = 0;
	return WriteFile(hPipe, &m_dwStatus, sizeof(DWORD), &cbWrite, NULL);
}

BOOL CLocalServer::ServerControl(HANDLE hPipe, DWORD dwControl)
{
	DWORD dwRet = ERROR_INVALID_SERVICE_CONTROL;
	if (m_pfnHandlerProc != NULL)
	{
		m_pfnHandlerProc(dwControl);
		dwRet = ERROR_SUCCESS;
	}

	DWORD cbWrite = 0;
	return WriteFile(hPipe, &dwRet, sizeof(DWORD), &cbWrite, NULL);
}

BOOL CLocalServer::RegisterServiceCtrlHandler(ServerHandlerProc lpHandlerProc)
{
	m_pfnHandlerProc = lpHandlerProc;
	return TRUE;
}

HANDLE CLocalServer::GetEventHandle()
{
    CComBSTR bstrName = L"CxBackgroundServer_";
	bstrName += m_bstrName;

    return CreateEventW(NULL, TRUE, FALSE, bstrName);
}

BOOL CLocalServer::GetClassServerPath(REFCLSID clsid, CComBSTR& bstrServerPath)
{
	USES_CONVERSION;

	HKEY hKey;
	HKEY hServerKey;
	WCHAR szCLSID[64];
	LONG nResult;
	ULONG nBytes;
	DWORD dwType;
	LPTSTR pszServerPath;
	BOOL bRet;

	StringFromGUID2(clsid, szCLSID, 64);

	hKey = NULL;
	hServerKey = NULL;
	bRet = FALSE;
   
	CComBSTR bstrKey = L"CLSID\\";
	bstrKey += szCLSID;

	nResult = RegOpenKeyEx(HKEY_CLASSES_ROOT, W2T(bstrKey), 0, KEY_READ, &hKey);
	if (nResult != ERROR_SUCCESS)
		goto onexit;

	nResult = RegOpenKeyEx(hKey, _T("InprocServer32"), 0, KEY_READ, &hServerKey);
	if (nResult != ERROR_SUCCESS)
	{
		nResult = RegOpenKeyEx( hKey, _T("InprocHandler32"), 0, KEY_READ, &hServerKey);
		if (nResult != ERROR_SUCCESS)
		{
			nResult = RegOpenKeyEx(hKey, _T("LocalServer32"), 0, KEY_READ, &hServerKey);
			if (nResult != ERROR_SUCCESS)
				goto onexit;
		}
	}

	nBytes = 0;
	nResult = RegQueryValueEx(hServerKey, NULL, NULL, &dwType, NULL, &nBytes);
	if (nResult != ERROR_SUCCESS || dwType != REG_SZ)
		goto onexit;

	pszServerPath = LPTSTR(_alloca(nBytes));
	nResult = RegQueryValueEx(hServerKey, NULL, NULL, &dwType, LPBYTE(pszServerPath), &nBytes);
	if (nResult != ERROR_SUCCESS || dwType != REG_SZ)
		goto onexit;

	bstrServerPath = pszServerPath;

	bRet = TRUE;

onexit:
	if (hKey != NULL)
		RegCloseKey(hKey );
	if (hServerKey != NULL )
		RegCloseKey(hServerKey);
	
	return bRet;
}

DWORD CLocalServer::StartProcess()
{
	HANDLE hEvents[2] = {NULL, NULL};
	PROCESS_INFORMATION pi;
    STARTUPINFOW si;
	BOOL bRet;
	DWORD dwRet;

	hEvents[0] = GetEventHandle();
    ZeroMemory(&si, sizeof(STARTUPINFOW));
    si.cb = sizeof(STARTUPINFOW);

	CComBSTR bstrPath;
	GetClassServerPath(m_clsidLocalServer, bstrPath);

    bRet = CreateProcessW(NULL, bstrPath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    if (bRet)
    {
        hEvents[1] = pi.hProcess;
        dwRet = WaitForMultipleObjects(2, hEvents, FALSE, 30000);
        if (dwRet != WAIT_OBJECT_0)
            bRet = FALSE;
    
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    CloseHandle(hEvents[0]);
  
    return bRet;
}

BOOL CLocalServer::SendStartMessage(HANDLE hPipe, LPCWSTR *argv, DWORD argc)
{
	DWORD dwLen = 1;
	for (DWORD dw = 0; dw < argc; dw++)
		dwLen += wcslen(argv[dw]) + 1;

	BYTE* pCmdBuf = (BYTE*)alloca(8 + dwLen * sizeof(WCHAR));
	*(DWORD *)pCmdBuf = 1;
	*(DWORD *)(pCmdBuf + 4) = dwLen;

	LPWSTR p = (LPWSTR)(pCmdBuf + 8);
	for (dw = 0; dw < argc; dw++)
	{
		wcscpy(p, argv[dw]);
        p += wcslen(p) + 1;
    }
    *p = 0;

	DWORD cbBytes = 0;
    BOOL bRet = WriteFile(hPipe, pCmdBuf, 8 + dwLen * sizeof(WCHAR), &cbBytes, NULL);
    if (bRet)
	{
		DWORD dwResult;
		bRet = ReadFile(hPipe, &dwResult, sizeof(DWORD), &cbBytes, NULL);
	}

     return bRet;
}

BOOL CLocalServer::GetStatus(HANDLE hPipe, DWORD* pdwStatus)
{
    DWORD cmd[2];
	DWORD cbBytes = 0;
    BOOL bRet;
    
    cmd[0] = 2;
    cmd[1] = 0;
    bRet = WriteFile(hPipe, cmd, sizeof(cmd), &cbBytes, NULL );
    if (!bRet || cbBytes != sizeof(cmd))
        return bRet;
 
    bRet = ReadFile( hPipe, pdwStatus, sizeof(DWORD), &cbBytes, NULL );
  
    return bRet;
}

BOOL CLocalServer::QueryStatus(DWORD* pdwStatus)
{
    HANDLE hPipe = OpenPipe();
    if (hPipe != INVALID_HANDLE_VALUE)
    {
        BOOL bRet = GetStatus(hPipe, pdwStatus);
        CloseHandle(hPipe);
        if (bRet)
            return TRUE;
    }

	*pdwStatus = CX_SERVER_STOPPED;

    return TRUE;
}

BOOL CLocalServer::WaitForStartup()
{
    DWORD dwStatus;
    BOOL bRet = FALSE;

    for (int i = 0; i < 30; i++)
    {
		dwStatus = 0;
        bRet = QueryStatus(&dwStatus);
        if (!bRet)
            break;
        if (dwStatus == CX_SERVER_RUNNING)
            break;

        bRet = FALSE;
        Sleep(300);
    }
    return bRet;
}

BOOL CLocalServer::Start(DWORD dwNumArgs, LPCWSTR* lpArgVectors)
{
	BOOL bRet = FALSE;
	
	HANDLE hPipe = OpenPipe();
    if (hPipe == INVALID_HANDLE_VALUE)
    {
        /* start the service process */
        if (StartProcess())
            hPipe = OpenPipe();
    }

	if (hPipe != INVALID_HANDLE_VALUE)
    {
        SendStartMessage(hPipe, lpArgVectors, dwNumArgs);
        CloseHandle(hPipe);
        bRet = TRUE;
    }

	if (bRet)
        WaitForStartup();

	return bRet;
}

HANDLE CLocalServer::OpenPipe()
{
	CComBSTR bstrPipeName;
	GetPipeName(bstrPipeName);

	HANDLE hPipe = INVALID_HANDLE_VALUE;

    do {
        hPipe = CreateFileW(bstrPipeName, GENERIC_READ|GENERIC_WRITE,
                         0, NULL, OPEN_ALWAYS, 0, NULL);
        if (hPipe != INVALID_HANDLE_VALUE)
            break;
        if (GetLastError() != ERROR_PIPE_BUSY)
            break;
    } while (WaitNamedPipeW(bstrPipeName, NMPWAIT_WAIT_FOREVER));
 
    return hPipe;
}

BOOL CLocalServer::SendControl(HANDLE hPipe, DWORD dwControl, DWORD* pdwResult)
{
    DWORD cmd[2];
	DWORD cbBytes = 0;
    BOOL rRet;
    
    cmd[0] = 3;
    cmd[1] = dwControl;
    rRet = WriteFile(hPipe, cmd, sizeof(cmd), &cbBytes, NULL);
    if (!rRet || cbBytes != sizeof(cmd))
         return rRet;

    rRet = ReadFile(hPipe, pdwResult, sizeof(DWORD), &cbBytes, NULL);
   
	return rRet;
}

BOOL CLocalServer::ControlServer(DWORD dwControl, DWORD* pdwStatus)
{
    BOOL bRet = FALSE;
    HANDLE hPipe;

    bRet = QueryStatus(pdwStatus);
    if (!bRet)
        return FALSE;
  
    switch (*pdwStatus)
    {
    case SERVICE_STOPPED:
        return FALSE;
    case SERVICE_START_PENDING:
        if (dwControl == SERVICE_CONTROL_STOP)
            break;
        /* fall thru */
    case SERVICE_STOP_PENDING:
         return FALSE;
    }

    hPipe = OpenPipe();
    if (hPipe != INVALID_HANDLE_VALUE)
    {
        DWORD dwResult = ERROR_SUCCESS;
        bRet = SendControl(hPipe, dwControl, &dwResult);
        CloseHandle(hPipe);
        if (dwResult != ERROR_SUCCESS)
             bRet = FALSE;
    }

    return bRet;
}