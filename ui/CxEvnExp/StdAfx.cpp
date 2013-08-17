// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif

#include <atlimpl.cpp>

BOOL GetCurrentProject(LPTSTR lpszProject)
{
	HKEY hSectionKey = NULL;
	HKEY hAppKey = NULL;
	HKEY hSoftKey = NULL;
	HKEY hCompanyKey = NULL;
	BOOL bRet = FALSE;
	
	if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("software"), 0, KEY_WRITE|KEY_READ,
		&hSoftKey) == ERROR_SUCCESS)
	{
		DWORD dw;
		if (RegCreateKeyEx(hSoftKey, _T("SuperCx"), 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCompanyKey, &dw) == ERROR_SUCCESS)
		{
			if (RegCreateKeyEx(hCompanyKey, _T("ProjectSetup"), 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hAppKey, &dw) == ERROR_SUCCESS)
			{
				if (RegCreateKeyEx(hAppKey, _T("General"), 0, REG_NONE,
					REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
					&hSectionKey, &dw) == ERROR_SUCCESS)
				{
					DWORD dwType, dwCount;
					LONG lResult = RegQueryValueEx(hSectionKey, _T("CurrentProject"), NULL, &dwType,
						NULL, &dwCount);
					if (lResult == ERROR_SUCCESS)
					{
						ATLASSERT(dwType == REG_SZ);
						ATLASSERT(dwCount < 128);
						lResult = RegQueryValueEx(hSectionKey, _T("CurrentProject"), NULL, &dwType,
							(LPBYTE)lpszProject, &dwCount);
						if (lResult == ERROR_SUCCESS)
							bRet = TRUE;
					}
				}
			}
		}
	}
	
	if (hSectionKey)
		RegCloseKey(hSectionKey);
	if (hAppKey)
		RegCloseKey(hAppKey);
	if (hSoftKey != NULL)
		RegCloseKey(hSoftKey);
	if (hCompanyKey != NULL)
		RegCloseKey(hCompanyKey);
	
	return bRet;
}

BOOL GetPictureFolder(LPTSTR lpszFolder)
{
	TCHAR szProject[4096];
	if (!GetCurrentProject(szProject))
		return FALSE;
	
	TCHAR szT[4096];
	DWORD dw = ::GetPrivateProfileString(_T("Path"), _T("Picture"),
		NULL, szT, 4096, szProject);
	ATLASSERT(dw < 4096);
	
	LPTSTR lpszFind = strstr(szT, _T("<工程路径>"));
	if (lpszFind != NULL)
	{
		// always capture the complete file name including extension (if present)
		LPTSTR lpszTemp = (LPTSTR)szProject;
		for (LPCTSTR lpsz = szProject; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
		{
			// remember last directory/drive separator
			if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
				lpszTemp = (LPTSTR)_tcsinc(lpsz);
		}
		
		lstrcpyn(lpszFolder, szProject, lpszTemp - szProject);
		lstrcat(lpszFolder, szT + 10);
	}
	else
	{
		lstrcat(lpszFolder, szT);
	}
	
	return TRUE;
}

BOOL GetRecipeFolder(LPTSTR lpszFolder)
{
	TCHAR szProject[4096];
	if (!GetCurrentProject(szProject))
		return FALSE;
	
	TCHAR szT[4096];
	DWORD dw = ::GetPrivateProfileString(_T("Path"), _T("Recipe"),
		NULL, szT, 4096, szProject);
	ATLASSERT(dw < 4096);
	
	LPTSTR lpszFind = strstr(szT, _T("<工程路径>"));
	if (lpszFind != NULL)
	{
		// always capture the complete file name including extension (if present)
		LPTSTR lpszTemp = (LPTSTR)szProject;
		for (LPCTSTR lpsz = szProject; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
		{
			// remember last directory/drive separator
			if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
				lpszTemp = (LPTSTR)_tcsinc(lpsz);
		}
		
		lstrcpyn(lpszFolder, szProject, lpszTemp - szProject);
		lstrcat(lpszFolder, szT + 10);
	}
	else
	{
		lstrcat(lpszFolder, szT);
	}
	
	return TRUE;
}