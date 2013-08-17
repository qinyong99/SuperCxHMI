// DirWatcher.cpp: implementation of the CDirWatcher class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cxdev.h"
#include "DirWatcher.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const UINT CDirWatcher::m_nWatchNotifyMsg = ::RegisterWindowMessage(_T("Watch_Notify_Msg"));

CDirWatcher::CDirWatcher()
{
	m_hFileHandle = INVALID_HANDLE_VALUE;
	m_hCompPort = NULL;

	m_hThread = NULL;
	m_hNotifyWnd = NULL;
}

CDirWatcher::~CDirWatcher()
{
	PostQueuedCompletionStatus(m_hCompPort, 0, 0, NULL); 

	if (m_hThread != NULL)
	{
		WaitForSingleObject(m_hThread, 10000);
		::TerminateThread(m_hThread, 98765321);
	}

	if (m_hCompPort != INVALID_HANDLE_VALUE)
		CloseHandle(m_hCompPort);
	
	if (m_hFileHandle != NULL)
		CloseHandle(m_hFileHandle);
}

BOOL CDirWatcher::StartWatch(HWND hNotifyWnd, LPCTSTR lpszPath)
{
	m_hNotifyWnd = hNotifyWnd;
	m_strPath = lpszPath;

	m_hFileHandle = CreateFile(m_strPath, 
		FILE_LIST_DIRECTORY,
		FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED,
		NULL); 
	if (m_hFileHandle == INVALID_HANDLE_VALUE)
		return FALSE;

	m_hCompPort = CreateIoCompletionPort(m_hFileHandle, m_hCompPort, DWORD(this), 0);

	memset(&m_overlapped, 0, sizeof(OVERLAPPED));
	ReadDirectoryChangesW(m_hFileHandle, m_lpBuffer, 4096,
			FALSE, FILE_NOTIFY_CHANGE_FILE_NAME, &m_dwSize, &m_overlapped, NULL);
	
	m_hThread = CreateThread(NULL, 0, _WatchProc, (void*)this, 0, &m_dwThreadID);

	return TRUE;
}

DWORD CDirWatcher::WatchProc()
{
	DWORD dwNumBytes;
	CDirWatcher* p;
	LPOVERLAPPED lpOverlapped;
	DWORD cbOffset;
	PFILE_NOTIFY_INFORMATION pfni;
	WCHAR szBuf[MAX_PATH];

	do 
	{
		GetQueuedCompletionStatus(m_hCompPort,
			&dwNumBytes, (LPDWORD)&p, &lpOverlapped, INFINITE);

		if (p != NULL)
		{
			pfni = (PFILE_NOTIFY_INFORMATION)m_lpBuffer;
			do 
			{
				cbOffset = pfni->NextEntryOffset;
				
				switch (pfni->Action) 
				{
				case FILE_ACTION_ADDED:
					{
						CFileChangeInfo* pFileChangeInfo = new CFileChangeInfo;
						
						memcpy(szBuf, pfni->FileName, pfni->FileNameLength);
						*((WCHAR *)((BYTE *)szBuf + pfni->FileNameLength)) = 0;
						pFileChangeInfo->strName = szBuf;
						pFileChangeInfo->nType = FileChange_Add;

						PostMessage(m_hNotifyWnd, m_nWatchNotifyMsg, 0, LPARAM(pFileChangeInfo));
					}
					break;
				case FILE_ACTION_REMOVED:
					{
						CFileChangeInfo* pFileChangeInfo = new CFileChangeInfo;

						memcpy(szBuf, pfni->FileName, pfni->FileNameLength);
						*((WCHAR *)((BYTE *)szBuf + pfni->FileNameLength)) = 0;
						pFileChangeInfo->strName = szBuf;
						pFileChangeInfo->nType = FileChange_Remove;
						
						PostMessage(m_hNotifyWnd, m_nWatchNotifyMsg, 0, LPARAM(pFileChangeInfo));
					}
					break;
				case FILE_ACTION_RENAMED_OLD_NAME:
					{
						CFileChangeInfo* pFileChangeInfo = new CFileChangeInfo;

						memcpy(szBuf, pfni->FileName, pfni->FileNameLength);
						*((WCHAR *)((BYTE *)szBuf + pfni->FileNameLength)) = 0;
						pFileChangeInfo->strName = szBuf;

						pfni = (PFILE_NOTIFY_INFORMATION)((LPBYTE)pfni + cbOffset);
						ATLASSERT(pfni->Action == FILE_ACTION_RENAMED_NEW_NAME);

						memcpy(szBuf, pfni->FileName, pfni->FileNameLength);
						*((WCHAR *)((BYTE *)szBuf + pfni->FileNameLength)) = 0;
						pFileChangeInfo->strNewName = szBuf;

						pFileChangeInfo->nType = FileChange_Rename;

						PostMessage(m_hNotifyWnd, m_nWatchNotifyMsg, 0, LPARAM(pFileChangeInfo));

						cbOffset = pfni->NextEntryOffset;
					}
					break;
				default:
					break;
				}
						
				pfni = (PFILE_NOTIFY_INFORMATION)((LPBYTE)pfni + cbOffset); 
			} while (cbOffset); 

			ReadDirectoryChangesW(m_hFileHandle, m_lpBuffer, 4096,
				FALSE, FILE_NOTIFY_CHANGE_FILE_NAME, &m_dwSize, &m_overlapped, NULL);
		}
		
	} while(p != NULL);

	return 0;
}