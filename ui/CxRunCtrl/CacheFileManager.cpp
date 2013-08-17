// CacheFileManager.cpp: implementation of the CCacheFileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cxrunctrl.h"
#include "CacheFileManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MAX_BUFFER_SIZE 1024 * 1024 * 16

CCacheFileManager::CCacheFileManager()
{
	m_dwBufferSize = 0;
}

CCacheFileManager::~CCacheFileManager()
{
	Clear();
}

void CCacheFileManager::Clear()
{
	POSITION pos = m_listCacheFile.GetHeadPosition();
	while (pos != NULL)
	{
		CCacheFile* pCacheFile = (CCacheFile *)m_listCacheFile.GetNext(pos); 
		delete pCacheFile;
	}
	
	m_listCacheFile.RemoveAll();
}

CCacheFile* CCacheFileManager::OpenCacheFile(LPCTSTR lpszFileName, ICxDataSession* pIDataSession, BOOL* pbExist)
{
	m_critsec.Lock();

	CCacheFile* pCacheFile = NULL;

	CString strFileName = lpszFileName;
	strFileName.TrimLeft();
	strFileName.TrimRight();

	POSITION pos = m_listCacheFile.GetHeadPosition();
	while (pos != NULL)
	{
		POSITION posRemove = pos;
		CCacheFile* pSearchCacheFile = (CCacheFile *)m_listCacheFile.GetNext(pos); 
		if (pSearchCacheFile->GetFileName().CompareNoCase(strFileName) == 0)
		{
			m_listCacheFile.RemoveAt(posRemove);
			pCacheFile = pSearchCacheFile;
			m_dwBufferSize -= pCacheFile->GetFileSize();
			break;
		}
	}

	// 溢出缓冲区，则删除缓冲文件，
	pos = m_listCacheFile.GetTailPosition();
	while (pos != NULL)
	{
		if (m_dwBufferSize <= MAX_BUFFER_SIZE)
			break;
			
		POSITION posRemove = pos;
		CCacheFile* pSearchCacheFile = (CCacheFile *)m_listCacheFile.GetNext(pos);

		m_listCacheFile.RemoveAt(posRemove);
		m_dwBufferSize -= pSearchCacheFile->GetFileSize();
		delete pSearchCacheFile;
	}

	if (pCacheFile == NULL)
		pCacheFile = new CCacheFile;

	if (pCacheFile->Open(strFileName, pIDataSession, pbExist))
	{
		m_listCacheFile.AddHead(pCacheFile);
		m_dwBufferSize += pCacheFile->GetFileSize();
	}
	else
	{
		delete pCacheFile;
		pCacheFile = NULL;
	}

	m_critsec.Unlock();

	return pCacheFile;
}