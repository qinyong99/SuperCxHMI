// CacheFile.h: interface for the CCacheFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CACHEFILE_H__
#define __CACHEFILE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface ICxDataSession;

class CCacheFile  
{
public:
	CCacheFile();
	CCacheFile(BYTE* lpBuffer, DWORD dwFileSize);
//	CCacheFile(LPCTSTR lpszFileName);

	virtual ~CCacheFile();

	BOOL Open(LPCTSTR lpszFileName, ICxDataSession* pIDataSession, BOOL* pbExist);
	void Close();

	CString GetFileName() { return m_strFileName; }
	DWORD GetFileSize() { return m_dwFileSize; }
	BYTE* GetBuffer() { return m_lpBuffer; }

// Implementation
protected:
	virtual BOOL OpenLocalFile(LPCTSTR lpszFileName, BOOL* pbExist);
	virtual BOOL OpenRemoteServerFile(LPCTSTR lpszFileName, ICxDataSession* pIDataSession, BOOL* pbExist);

	CString m_strFileName;
	FILETIME m_ftFileTime; 
	DWORD m_dwFileSize;
	BYTE* m_lpBuffer;
};

#endif // __CACHEFILE_H__
