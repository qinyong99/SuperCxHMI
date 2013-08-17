// WaitMessageLoop.h: interface for the CWaitMessageLoop class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __WAITMESSAGELOOP_H_
#define __WAITMESSAGELOOP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWaitMessageLoop : public CMessageLoop 
{
public:
	CWaitMessageLoop()
	{
	}
	virtual ~CWaitMessageLoop()
	{
	}

	BOOL Run(HANDLE hEvent)
	{
		BOOL bDoIdle = TRUE;
		int nIdleCount = 0;
		DWORD dwRet;
			
		for(;;)
		{
			while (::PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
			{
				if (!PreTranslateMessage(&m_msg))
				{
					TranslateMessage(&m_msg);
					DispatchMessage(&m_msg);
				}

				if (WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0)
					return TRUE; // Event is now signaled.
			}
			
			if (IsIdleMessage(&m_msg))
			{
				bDoIdle = TRUE;
				nIdleCount = 0;
			}
		
			if (bDoIdle)
			{
				if (!OnIdle(nIdleCount++))
					bDoIdle = FALSE;
			}
			
			dwRet = MsgWaitForMultipleObjects(1, &hEvent, FALSE, INFINITE, QS_ALLINPUT);
			
			if (dwRet == WAIT_OBJECT_0)
				return TRUE;    // The event was signaled
			
			if (dwRet != WAIT_OBJECT_0 + 1)
				break;          // Something else happened
		} 
			
		return FALSE;	
	}
};

#endif // __WAITMESSAGELOOP_H_
