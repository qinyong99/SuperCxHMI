// DirWatcher.h: interface for the CDirWatcher class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DIRWATCHER_H__
#define _DIRWATCHER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDirWatcher  
{
public:
	CDirWatcher();
	virtual ~CDirWatcher();

	enum
	{
		FileChange_Add,
		FileChange_Remove,
		FileChange_Rename,
	};
	
	class CFileChangeInfo 
	{
	public:
		int nType;
		CString strName;
		CString strNewName;
	};

	static DWORD WINAPI _WatchProc(void* pv)
	{
		return ((CDirWatcher*)pv)->WatchProc();
	}
	
	DWORD WatchProc();
	
	BOOL StartWatch(HWND hNotifyWnd, LPCTSTR lpszPath);

	static const UINT m_nWatchNotifyMsg;

private:
	CString m_strPath;
	
	BYTE m_lpBuffer[4096]; 
	DWORD m_dwSize;
	OVERLAPPED m_overlapped;
	
	HANDLE m_hFileHandle;
	HANDLE m_hCompPort;
	
	DWORD m_dwThreadID;
	HANDLE m_hThread;

	HWND m_hNotifyWnd;
};

#endif // _DIRWATCHER_H__
