// OleMessageFilter2.h: interface for the COleMessageFilter2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OLEMESSAGEFILTER2_H__2AB5F696_BA2A_43B9_BF3B_31EEC76042F1__INCLUDED_)
#define AFX_OLEMESSAGEFILTER2_H__2AB5F696_BA2A_43B9_BF3B_31EEC76042F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COleMessageFilter2 : public COleMessageFilter  
{
public:
	COleMessageFilter2();
	virtual ~COleMessageFilter2();

	BOOL Register2();

	virtual int OnNotRespondingDialog(HTASK htaskBusy);

	// Interface Maps
public:
	BEGIN_INTERFACE_PART(MessageFilter2, IMessageFilter)
		INIT_INTERFACE_PART(COleMessageFilter, MessageFilter)
		STDMETHOD_(DWORD, HandleInComingCall)(DWORD, HTASK, DWORD,
			LPINTERFACEINFO);
		STDMETHOD_(DWORD, RetryRejectedCall)(HTASK, DWORD, DWORD);
		STDMETHOD_(DWORD, MessagePending)(HTASK, DWORD, DWORD);
	END_INTERFACE_PART(MessageFilter2)
		
	DECLARE_INTERFACE_MAP()
};

#endif // !defined(AFX_OLEMESSAGEFILTER2_H__2AB5F696_BA2A_43B9_BF3B_31EEC76042F1__INCLUDED_)
