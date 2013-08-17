// stdafx.cpp : source file that includes just the standard includes
//	CxDev.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

OLECHAR* CoTaskMemAllocString(const OLECHAR* psz)
{
	int cb = (wcslen(psz) + 1) * sizeof(OLECHAR);
	OLECHAR* pszT = (OLECHAR *)CoTaskMemAlloc(cb);
	if (pszT == 0) 
		return NULL;
	wcscpy(pszT, psz);
	return pszT;
}