// OleMessageFilter2.cpp: implementation of the COleMessageFilter2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxRun.h"
#include "OleMessageFilter2.h"
#include "DcomErrorDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COleMessageFilter2::COleMessageFilter2()
{

}

COleMessageFilter2::~COleMessageFilter2()
{

}

BOOL COleMessageFilter2::Register2()
{
	ASSERT_VALID(this);
	ASSERT(!m_bRegistered); // calling Register twice?
	
	if (::CoRegisterMessageFilter(&m_xMessageFilter2, NULL) == S_OK)
	{
		m_bRegistered = TRUE;
		return TRUE;
	}
	return FALSE;
}

int COleMessageFilter2::OnNotRespondingDialog(HTASK htaskBusy)
{
	CDcomErrorDlg dlg;
	
	int nResult = dlg.DoModal();
	
	return nResult;
}

/////////////////////////////////////////////////////////////////////////////
// COleMessageFilter2 OLE interface implementation

BEGIN_INTERFACE_MAP(COleMessageFilter2, CCmdTarget)
	INTERFACE_PART(COleMessageFilter2, IID_IMessageFilter, MessageFilter2)
END_INTERFACE_MAP()

STDMETHODIMP_(ULONG) COleMessageFilter2::XMessageFilter2::AddRef()
{
	METHOD_PROLOGUE_EX_(COleMessageFilter2, MessageFilter2)
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleMessageFilter2::XMessageFilter2::Release()
{
	METHOD_PROLOGUE_EX_(COleMessageFilter2, MessageFilter2)
	return pThis->ExternalRelease();
}

STDMETHODIMP COleMessageFilter2::XMessageFilter2::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(COleMessageFilter2, MessageFilter2)
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP_(DWORD) COleMessageFilter2::XMessageFilter2::HandleInComingCall(
	DWORD dwCallType, HTASK /*htaskCaller*/,
	DWORD /*dwTickCount*/, LPINTERFACEINFO /*lpInterfaceInfo*/)
{
	METHOD_PROLOGUE_EX_(COleMessageFilter2, MessageFilter2)

	// check for application busy first...
	if (pThis->m_nBusyCount == 0)
	{
		if (dwCallType == CALLTYPE_TOPLEVEL ||
			dwCallType == CALLTYPE_TOPLEVEL_CALLPENDING)
		{
			// make sure CWinThread::OnIdle has a chance to run later
			MSG msg;
			if (!::PeekMessage(&msg, NULL, WM_KICKIDLE, WM_KICKIDLE, PM_NOREMOVE))
				::PostThreadMessage(GetCurrentThreadId(), WM_KICKIDLE, 0, 0);
		}
		return SERVERCALL_ISHANDLED;
	}

	if (dwCallType == CALLTYPE_TOPLEVEL ||
		dwCallType == CALLTYPE_TOPLEVEL_CALLPENDING)
	{
		// application is busy and we have rejectable CALLTYPE
		return pThis->m_nBusyReply;
	}

	// application is busy, but CALLTYPE indicates that it *must* be handled
	return SERVERCALL_ISHANDLED;
}

STDMETHODIMP_(DWORD) COleMessageFilter2::XMessageFilter2::RetryRejectedCall(
	HTASK htaskCallee, DWORD dwTickCount, DWORD dwRejectType)
{
	METHOD_PROLOGUE_EX(COleMessageFilter2, MessageFilter2)
	ASSERT_VALID(pThis);

	// rejected calls get cancelled regardless of timeout
	if (dwRejectType == SERVERCALL_REJECTED)
		return (DWORD)-1;

	// if insignificant time has passed, don't panic -- just retry
	if (dwTickCount <= pThis->m_nRetryReply)
		return 0;   // retry right away (0-100 are retry immediate)

	// too much time has passed, do something more drastic
	if (pThis->m_bEnableBusy)
	{
		// show busy dialog
		int selType = pThis->OnBusyDialog(htaskCallee);

		// take action depending on selection
		switch (selType)
		{
		case -1:
			return (DWORD)-1;   // cancel outgoing call

		case COleBusyDialog::retry:
			return 0;           // retry immediately
		}
	}
	return pThis->m_nRetryReply;    // use standard retry timeout
}

STDMETHODIMP_(DWORD) COleMessageFilter2::XMessageFilter2::MessagePending(
	HTASK htaskCallee, DWORD dwTickCount, DWORD /*dwPendingType*/)
{
	METHOD_PROLOGUE_EX(COleMessageFilter2, MessageFilter2)
	ASSERT_VALID(pThis);

	MSG msg;
	if (dwTickCount > pThis->m_nTimeout && !pThis->m_bUnblocking &&
		pThis->IsSignificantMessage(&msg))
	{
		if (pThis->m_bEnableNotResponding)
		{
			pThis->m_bUnblocking = TRUE;    // avoid reentrant calls

			// eat all mouse messages in our queue
			while (PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST,
				PM_REMOVE|PM_NOYIELD))
				;
			// eat all keyboard messages in our queue
			while (PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST,
				PM_REMOVE|PM_NOYIELD))
				;

			// show not responding dialog
			int nResult = pThis->OnNotRespondingDialog(htaskCallee);
			pThis->m_bUnblocking = FALSE;

			if (nResult == IDABORT)
				 ExitProcess(0);
			else if (nResult == IDIGNORE)
				return PENDINGMSG_CANCELCALL;

			return PENDINGMSG_WAITNOPROCESS;
		}
	}

	// don't process re-entrant messages
	if (pThis->m_bUnblocking)
		return PENDINGMSG_WAITDEFPROCESS;

	// allow application to process pending message
	if (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE|PM_NOYIELD))
		pThis->OnMessagePending(&msg);

	// by default we return pending MSG wait
	return PENDINGMSG_WAITNOPROCESS;
}
