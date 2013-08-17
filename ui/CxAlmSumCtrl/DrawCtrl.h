// DrawCtrl.h: interface for the CDrawCtrl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DRAWCTRL_H_
#define __DRAWCTRL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMessageMap - abstract class that provides an interface for message maps

class ATL_NO_VTABLE CActionMap
{
public:
	virtual BOOL ProcessAction(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwActMapID = 0) = 0;
};

/////////////////////////////////////////////////////////////////////////////
// Message map

#define BEGIN_ACT_MAP(theClass) \
public: \
	BOOL ProcessAction(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwActMapID = 0) \
	{ \
		BOOL bHandled = TRUE; \
		uMsg; \
		wParam; \
		lParam; \
		lResult; \
		bHandled; \
		switch(dwActMapID) \
		{ \
		case 0:

#define ALT_ACT_MAP(actMapID) \
		break; \
		case actMapID:

#define ACTION_HANDLER(msg, func) \
	if(uMsg == msg) \
	{ \
		bHandled = TRUE; \
		lResult = func(uMsg, wParam, lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define ACTION_RANGE_HANDLER(msgFirst, msgLast, func) \
	if(uMsg >= msgFirst && uMsg <= msgLast) \
	{ \
		bHandled = TRUE; \
		lResult = func(uMsg, wParam, lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define CHAIN_ACT_MAP(theChainClass) \
	{ \
		if(theChainClass::ProcessAction(uMsg, wParam, lParam, lResult)) \
			return TRUE; \
	}

#define CHAIN_ACT_MAP_MEMBER(theChainMember) \
	{ \
		if(theChainMember.ProcessAction(uMsg, wParam, lParam, lResult)) \
			return TRUE; \
	}

#define CHAIN_ACT_MAP_ALT(theChainClass, actMapID) \
	{ \
		if(theChainClass::ProcessAction(uMsg, wParam, lParam, lResult, actMapID)) \
			return TRUE; \
	}

#define CHAIN_ACT_MAP_ALT_MEMBER(theChainMember, actMapID) \
	{ \
		if(theChainMember.ProcessAction(uMsg, wParam, lParam, lResult, actMapID)) \
			return TRUE; \
	}


#define END_ACT_MAP() \
			break; \
		default: \
			ATLTRACE2(atlTraceWindowing, 0, _T("Invalid message map ID (%i)\n"), dwActMapID); \
			ATLASSERT(FALSE); \
			break; \
		} \
		return FALSE; \
	}

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


// Empty message map macro

#define DECLARE_EMPTY_ACT_MAP() \
public: \
	BOOL ProcessAction(UINT, WPARAM, LPARAM, LRESULT&, DWORD) \
	{ \
		return FALSE; \
	}


class ATL_NO_VTABLE CDrawCtrl : public CActionMap
{
public:
	CDrawCtrl();
	virtual ~CDrawCtrl();

public:
	virtual void RecalcRect(RECT& rect)
	{
	}

	virtual void DoDraw(HDC hdc)
	{
		Rectangle(hdc, m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}

	virtual BOOL HitTest(POINT pt)
	{
		return ::PtInRect(&m_rect, pt);
	}

	RECT m_rect;
};

#endif // __DRAWCTRL_H_
