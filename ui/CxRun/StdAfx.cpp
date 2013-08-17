// stdafx.cpp : source file that includes just the standard includes
//	CxRun.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

BOOL IsLocalComputer(LPCTSTR szComputer)
{
	USES_CONVERSION;
	if (lstrcmpi(_T("localhost"), szComputer) == 0)
		return TRUE;
	
	// 根据计算机名判断
	TCHAR szBuf[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwSize = MAX_COMPUTERNAME_LENGTH;
	GetComputerName(szBuf, &dwSize);
	
	if (lstrcmpi(szBuf, szComputer) == 0)
		return TRUE;
	
	// 根据IP地址名判断
	WORD wVersionRequested;
	WSADATA wsaData;
	TCHAR szName[255];
	PHOSTENT pHost;
	wVersionRequested = MAKEWORD(2, 0);
	
	BOOL bRet = FALSE;
	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if (gethostname(szName, sizeof(szName)) == 0)
		{
			pHost = gethostbyname(szName);
			for (int nAdapter = 0; pHost->h_addr_list[nAdapter] != NULL; nAdapter++)
			{
				sockaddr_in sa;
				memcpy(&sa.sin_addr.s_addr, pHost->h_addr_list[nAdapter], pHost->h_length);
				CString strIP = inet_ntoa(sa.sin_addr);
				
				if (lstrcmpi(strIP, szComputer) == 0)
				{
					bRet = TRUE;
					break;
				}
			}
		}
		
		WSACleanup();
	}
	
	return bRet;
}
