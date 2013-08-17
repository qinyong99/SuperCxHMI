// CacheFile.cpp: implementation of the CCacheFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RunInc.h"
#include "CacheFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCacheFile::CCacheFile()
{
	m_dwFileSize = 0;
	m_lpBuffer = NULL;
	memset(&m_ftFileTime, 0, sizeof(FILETIME));
}

CCacheFile::CCacheFile(BYTE* lpBuffer, DWORD dwFileSize)
{
	m_dwFileSize = dwFileSize;
	m_lpBuffer = lpBuffer;
}

CCacheFile::~CCacheFile()
{
	Close();
}

BOOL CCacheFile::OpenRemoteServerFile(LPCTSTR lpszFileName, ICxDataSession* pIDataSession, BOOL* pbExist)
{
	if (pbExist != NULL)
		*pbExist = FALSE;

	USES_CONVERSION;
	ASSERT(pIDataSession != NULL);

	DWORD dwCount = 0;
	BYTE* pBuffer = NULL;
	HRESULT hr = pIDataSession->FetchPictureFile(T2W(lpszFileName), &m_ftFileTime, &dwCount, &pBuffer);
	ASSERT(SUCCEEDED(hr));
	if (SUCCEEDED(hr))
	{
		if (hr == S_OK)
		{
			Close();
			
			m_strFileName = lpszFileName;
			m_lpBuffer = (BYTE *)malloc(dwCount);
			if (m_lpBuffer == NULL)
			{
				if (pBuffer != NULL)
					CoTaskMemFree(pBuffer);
				return FALSE;
			}
			
			memcpy(m_lpBuffer, pBuffer, dwCount);
			m_dwFileSize = dwCount;
		}
		else
		{
			if (pbExist != NULL)
				*pbExist = TRUE;
		}
		
		if (pBuffer != NULL)
			CoTaskMemFree(pBuffer);

		return TRUE;
	}
	
	return FALSE;
}

BOOL CCacheFile::OpenLocalFile(LPCTSTR lpszFileName, BOOL* pbExist)
{
	if (pbExist != NULL)
		*pbExist = FALSE;

	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD dwBytesRead, dwFileSize;
	BYTE* lpBuffer = NULL;
	BOOL bRet = FALSE;
	
	hFile = ::CreateFile(
		lpszFileName,
		GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	
	if (hFile == INVALID_HANDLE_VALUE)
		goto Exit;
	
	// 如果时间是否发生变化
	FILETIME ftFileTime;
	GetFileTime(hFile, NULL, NULL, &ftFileTime);
	if (memcmp(&ftFileTime, &m_ftFileTime, sizeof(FILETIME)) == 0)
	{
		if (pbExist != NULL)
			*pbExist = TRUE;
		bRet = TRUE;
		goto Exit;
	}
	
	m_ftFileTime = ftFileTime;
	
	dwFileSize = ::GetFileSize(hFile, NULL);
	if (dwFileSize == 0xFFFFFFFF || dwFileSize == 0) 
		goto Exit;
	
	lpBuffer = (BYTE *)malloc(dwFileSize);
	if (lpBuffer == NULL)
		goto Exit;
	
	if (!::ReadFile(hFile, lpBuffer, dwFileSize, &dwBytesRead, NULL))
		goto Exit;
	
	if (dwBytesRead == 0)
		goto Exit;
	
	Close();
	
	m_strFileName = lpszFileName;
	m_lpBuffer = lpBuffer;
	m_dwFileSize = dwFileSize;
	lpBuffer = NULL;
	dwFileSize = 0;
	
	bRet = TRUE;
	
Exit:
	if (lpBuffer != NULL)
		free(lpBuffer);
	
	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle(hFile);
	
	return bRet;
}

// Server://Node/Picture/abc.grf
BOOL CCacheFile::Open(LPCTSTR lpszFileName, ICxDataSession* pIDataSession, BOOL* pbExist)
{
	BOOL b;

	CString strFileName = lpszFileName;
	if (strFileName.Left(7).CompareNoCase(_T("Server:")) == 0)
	{
		strFileName = strFileName.Mid(7);
		strFileName.TrimLeft();

		b = OpenRemoteServerFile(strFileName, pIDataSession, pbExist);
	}
	else
	{
		b = OpenLocalFile(strFileName, pbExist);
	}

	if (b)
		m_strFileName = lpszFileName;

	return b;
}

void CCacheFile::Close()
{
	if (m_lpBuffer != NULL)
	{
		free(m_lpBuffer);
		m_lpBuffer = NULL;
	}
	
	m_strFileName.Empty();
	m_dwFileSize = 0;
}

