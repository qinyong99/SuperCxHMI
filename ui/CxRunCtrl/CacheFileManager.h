// CacheFileManager.h: interface for the CCacheFileManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CACHEFILEMANAGER_H__
#define __CACHEFILEMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CacheFile.h"

class CCacheFileManager  
{
public:
	CCacheFileManager();
	virtual ~CCacheFileManager();

	CCacheFile* OpenCacheFile(LPCTSTR lpszFileName, ICxDataSession* pIDataSession = NULL, BOOL* pbExist = NULL);
	void Clear();

protected:
	CPtrList m_listCacheFile;
	DWORD m_dwBufferSize;

	CCriticalSection m_critsec;
};

#endif // __CACHEFILEMANAGER_H__