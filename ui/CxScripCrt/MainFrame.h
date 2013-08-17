// MainFrame.h: interface for the CMainFrame class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __MAINFRAME_H_
#define __MAINFRAME_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

#include "ScriptEditorView.h"
#include "SymbolView.h"
#include "ComboBoxBold.h"
#include "WaitMessageLoop.h"

class CCommandBarCtrlEx : public CCommandBarCtrl
{
public:
	CCommandBarCtrlEx()
	{
	}

	BEGIN_MSG_MAP(CCommandBarCtrlEx)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CCommandBarCtrl)
	ALT_MSG_MAP(1)
		CHAIN_MSG_MAP_ALT(CCommandBarCtrl, 1)
	ALT_MSG_MAP(2)
		CHAIN_MSG_MAP_ALT(CCommandBarCtrl, 2)
	ALT_MSG_MAP(3)
		CHAIN_MSG_MAP_ALT(CCommandBarCtrl, 3)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		// Let the toolbar initialize itself
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);
		// get and use system settings
		GetSystemSettings();
		// Parent init
		CWindow wndParent = GetParent();
		CWindow wndTopLevelParent = wndParent.GetTopLevelParent();
		m_wndParent.SubclassWindow(wndParent);
		// Toolbar Init
		SetButtonStructSize();
		SetImageList(NULL);
		
		// Create message hook if needed
		::EnterCriticalSection(&_Module.m_csWindowCreate);
		if(s_pmapMsgHook == NULL)
		{
			ATLTRY(s_pmapMsgHook = new CMsgHookMap);
			ATLASSERT(s_pmapMsgHook != NULL);
		}
		
		if(s_pmapMsgHook != NULL)
		{
			DWORD dwThreadID = ::GetCurrentThreadId();
			_MsgHookData* pData = s_pmapMsgHook->Lookup(dwThreadID);
			if(pData == NULL)
			{
				ATLTRY(pData = new _MsgHookData);
				ATLASSERT(pData != NULL);
				HHOOK hMsgHook = ::SetWindowsHookEx(WH_GETMESSAGE, MessageHookProc, _Module.GetModuleInstance(), dwThreadID);
				ATLASSERT(hMsgHook != NULL);
				if(pData != NULL && hMsgHook != NULL)
				{
					pData->hMsgHook = hMsgHook;
					pData->dwUsage = 1;
					BOOL bRet = s_pmapMsgHook->Add(dwThreadID, pData);
					bRet;
					ATLASSERT(bRet);
				}
			}
			else
			{
				(pData->dwUsage)++;
			}
		}
		::LeaveCriticalSection(&_Module.m_csWindowCreate);
		
		return lRet;
	}
};

class CMainFrame :
	public CFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, 
	public CIdleHandler,
	public IDispEventImpl<0, CMainFrame, &DIID__ICxScriptCreatorEvents, &LIBID_CXSCRIPCRTLib, 1, 0>
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME);

	CMainFrame();
	virtual ~CMainFrame();

	BEGIN_SINK_MAP(CMainFrame)
		SINK_ENTRY_INFO(0, DIID__ICxScriptCreatorEvents, 1, OnPositionChanged, NULL)
//		SINK_ENTRY(0, 1, OnPositionChanged)
	END_SINK_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		COMMAND_ID_HANDLER(ID_SAVE_EXIT, OnFileSave)
		COMMAND_ID_HANDLER(ID_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_EDIT_UNDO, OnEditUndo)
		COMMAND_ID_HANDLER(ID_EDIT_REDO, OnEditRedo)
		COMMAND_ID_HANDLER(ID_EDIT_CUT, OnEditCut)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnEditCopy)
		COMMAND_ID_HANDLER(ID_EDIT_PASTE, OnEditPaste)
		COMMAND_ID_HANDLER(ID_EDIT_CLEAR, OnEditClear)
		COMMAND_ID_HANDLER(ID_EDIT_FIND, OnEditFind)
		COMMAND_ID_HANDLER(ID_EDIT_REPEAT, OnEditRepeat)
		COMMAND_ID_HANDLER(ID_EDIT_REPLACE, OnEditReplace)
		COMMAND_ID_HANDLER(ID_EDIT_GOTO, OnEditGoLine)
		COMMAND_ID_HANDLER(ID_BOOKMARK_TOGGLE, OnBookmarkToggle)
		COMMAND_ID_HANDLER(ID_BOOKMART_NEXT, OnBookmarkNext)
		COMMAND_ID_HANDLER(ID_BOOKMART_PREVIOUS, OnBookmarkPrevious)
		COMMAND_ID_HANDLER(ID_BOOKMARK_CLEAR, OnBookmarkClearAll)
		COMMAND_ID_HANDLER(ID_EDIT_SELECT_ALL, OnEditSelectAll)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
	//	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_EDIT_UNDO,		UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_REDO,		UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_CUT, 		UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_COPY,		UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_PASTE,		UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_CLEAR,		UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_SELECT_ALL,	UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_FIND,		UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_REPEAT,		UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_REPLACE,		UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_VIEW_SYMBOL,		UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
	END_UPDATE_UI_MAP()

public:
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnEditUndo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditRedo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditCut(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditPaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditClear(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditFind(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditReplace(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditRepeat(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditGoLine(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBookmarkToggle(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBookmarkNext(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBookmarkPrevious(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBookmarkClearAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	LRESULT OnEditSelectAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	HRESULT __stdcall OnPositionChanged();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BOOL Show(HWND hwndParent, ICxScriptEditorSite* pSite, DWORD dwSymbolMask, CString& strText, BOOL& bModified, LPCTSTR lpszObject = NULL, LPCTSTR lpszEvent = NULL);


	HANDLE m_hEvent;
	BOOL m_bSave;
	BOOL m_bPrompt;

	CCommandBarCtrlEx m_CmdBar;
	CMultiPaneStatusBarCtrl m_sbar;

	CAxWindow m_wndScriptEidor;
	CComPtr<ICxScriptEditor> m_spScriptEditor;


};

#endif // __MAINFRAME_H_
