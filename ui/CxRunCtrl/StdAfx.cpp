// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

BOOL StrInStrlist(LPCTSTR lpszList, LPCTSTR lpszSub)
{
	if (lpszList == NULL)
		return FALSE;
	
	LPCTSTR p1;
	LPCTSTR p2;
	p1 = lpszList;
	
	for (;;)
	{
		while (*p1 == ' ')
		{
			p1++;
		}
		p2 = p1;
		
		while (__iscsym(*p2))
			p2++;
		
		p2++;
		
		TCHAR szT[128];
		lstrcpyn(szT, p1, p2 - p1);
		if (lstrcmpi(szT, lpszSub) == 0)
			return TRUE;
		
		if (*p2 == '\0')
			return FALSE;
		
		p1 = p2;
	}
	
	return FALSE;
}
