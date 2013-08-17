// MainFrame.cpp: implementation of the CMainFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxScripCrt.h"
#include "MainFrame.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainFrame::CMainFrame()
{
	m_hEvent = NULL;
}

CMainFrame::~CMainFrame()
{

}

BOOL CMainFrame::Show(HWND hwndParent, ICxScriptEditorSite* pSite, DWORD dwSymbolMask, CString& strText, BOOL& bModified, LPCTSTR lpszObject, LPCTSTR lpszEvent)
{
	USES_CONVERSION;
		
	RECT rc = {0, 0, 640, 480};

	if (CreateEx(hwndParent, &rc, WS_POPUP|WS_CAPTION|WS_SYSMENU|
			WS_THICKFRAME|WS_MAXIMIZEBOX/*, WS_EX_TOOLWINDOW*/) == NULL)
	{
		ATLTRACE(_T("CMainFrame creation failed!\n"));
		return FALSE;
	}

	m_bSave = FALSE;
	m_bPrompt = FALSE;
	
	m_hEvent = CreateEvent(0, 0, 0, 0);
	::EnableWindow(hwndParent, false);
	ModifyStyleEx(WS_EX_APPWINDOW, 0); //使窗口标题不显示在任务条上
	CenterWindow();
	ShowWindow(SW_SHOW);

	m_spScriptEditor->SetScriptEditorSite(pSite);
	m_spScriptEditor->AddGeneralSymbolPage(dwSymbolMask);
	m_spScriptEditor->put_Text(T2BSTR(strText));
	m_spScriptEditor->EditObjectEventHandler(T2BSTR(lpszObject), T2BSTR(lpszEvent), TRUE);	
	m_spScriptEditor->EmptyUndoBuffer();
	m_spScriptEditor->SetSavePoint();

	CWaitMessageLoop loop;
	loop.AddMessageFilter(this);
	loop.AddIdleHandler(this);
	loop.Run(m_hEvent);

	BOOL bModify;
	m_spScriptEditor->GetModify(&bModify);
	if (m_bPrompt && bModify)
	{
		if (MessageBox(_T("脚本改动保存吗？"), _T("询问"), MB_YESNO | MB_ICONQUESTION) == IDYES)
			m_bSave = TRUE;
	}

	if (m_bSave)
	{
		m_spScriptEditor->DeleteEmptyEventHandler();

		CComBSTR bstr;
		m_spScriptEditor->get_Text(&bstr);
		strText = bstr;
		bModified = TRUE;
	}

	::EnableWindow(hwndParent, true);			
	DestroyWindow();
	CloseHandle(m_hEvent);
	m_hEvent = 0;

	return TRUE;
}

LRESULT CMainFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bPrompt = TRUE;
	SetEvent(m_hEvent);
	return 0;
}

LRESULT CMainFrame::OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_bSave = TRUE;
	SetEvent(m_hEvent);
	return 0;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SetEvent(m_hEvent);
	return 0;
}

int arrParts[] =
{
	ID_DEFAULT_PANE,
	ID_ROW_PANE,
	ID_COL_PANE
};

LRESULT CMainFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CRect rect;

	AtlAxWinInit();
	
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// atach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	CreateSimpleStatusBar();
	m_sbar.SubclassWindow(m_hWndStatusBar);
	
	m_sbar.SetPanes(arrParts, sizeof(arrParts) / sizeof(int), false);

	GetClientRect(&rect);
	m_hWndClient = m_wndScriptEidor.Create(m_hWnd, rect, _T(""), 
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	m_spScriptEditor.CoCreateInstance(CLSID_CxScriptEditor);

	CComPtr<IAxWinHostWindow> spContainer;
	m_wndScriptEidor.QueryHost(&spContainer);
	// Attach the browser control to the container
	spContainer->AttachControl(m_spScriptEditor, m_wndScriptEidor);

	DispEventAdvise(m_spScriptEditor);

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_SYMBOL, 1);

	return 0;
}

LRESULT CMainFrame::OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
//	if((HWND)wParam == m_viewScript.m_hWnd)
	{
		CMenu menuContext;
		menuContext.LoadMenu(IDR_CONTEXTMENU);
		CMenuHandle menuPopup(menuContext.GetSubMenu(0));
//		UIUpdateMenuBar();
//		menuPopup.EnableMenuItem(ID_EDIT_CUT, FALSE);
//		menuPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, LOWORD(lParam), HIWORD(lParam), m_hWnd);
//		menuPopup.EnableMenuItem(ID_EDIT_CUT, FALSE);
		m_CmdBar.TrackPopupMenu(menuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, LOWORD(lParam), HIWORD(lParam));
		
//		UIUpdateMenuBar();
	}
/*
	else
	{
		bHandled = FALSE;
	}*/

		
	return 0;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST) ||
	  pMsg->message >= WM_MOUSEFIRST && (pMsg->message <= WM_MOUSELAST))
	{
		HWND hFindDialog;
		if (m_spScriptEditor->get_FindDialogHandle((long *)&hFindDialog) == S_OK)
			if (::IsWindow(hFindDialog) && ::IsDialogMessage(hFindDialog, pMsg))
				return TRUE;

		CComQIPtr<IOleInPlaceActiveObject> spInPlaceActiveObject = m_spScriptEditor;
		if (spInPlaceActiveObject->TranslateAccelerator(pMsg) == S_OK)
			return TRUE;
	}

	return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	BOOL b;
	long lStartChar;
	m_spScriptEditor->GetSelectionStart(&lStartChar);
	long lEndChar;
	m_spScriptEditor->GetSelectionEnd(&lEndChar);
	b = lStartChar != lEndChar;

	UIEnable(ID_EDIT_CUT, b);
	UIEnable(ID_EDIT_COPY, b);
	UIEnable(ID_EDIT_CLEAR, b);
	
	long lLength;
	m_spScriptEditor->GetTextLength(&lLength);
	b = lLength > 0;
	BOOL bNotEmpty = b;
	
	UIEnable(ID_EDIT_SELECT_ALL, b);
	UIEnable(ID_EDIT_FIND, b);
	UIEnable(ID_EDIT_REPLACE, b);
	
	m_spScriptEditor->CanPaste(&b);
	UIEnable(ID_EDIT_PASTE, b);

	m_spScriptEditor->CanRedo(&b);
	UIEnable(ID_EDIT_REDO, b);

	m_spScriptEditor->CanUndo(&b);
	UIEnable(ID_EDIT_UNDO, b);
	
	m_spScriptEditor->HasSearched(&b);
	UIEnable(ID_EDIT_REPEAT, bNotEmpty && b);

	UIUpdateToolBar();

// 	OnPositionChanged();
		
	return FALSE;
}

LRESULT CMainFrame::OnEditUndo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->Undo();

	return 0;
}

LRESULT CMainFrame::OnEditRedo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->Redo();

	return 0;
}

LRESULT CMainFrame::OnEditCut(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->Cut();

	return 0;
}

LRESULT CMainFrame::OnEditCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->Copy();
	
	return 0;
}

LRESULT CMainFrame::OnEditPaste(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->Paste();

	return 0;
}

LRESULT CMainFrame::OnEditClear(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->Clear();

	return 0;
}

LRESULT CMainFrame::OnEditSelectAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->SelectAll();

	return 0;
}

LRESULT CMainFrame::OnEditFind(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->Find();

	return 0;
}

LRESULT CMainFrame::OnEditReplace(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->Replace();

	return 0;
}

LRESULT CMainFrame::OnEditRepeat(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->FindRepeat();

	return 0;
}

LRESULT CMainFrame::OnEditGoLine(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->GoLine();
	
	return 0;
}

LRESULT CMainFrame::OnBookmarkToggle(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->BookmarkToggle();

	return 0;
}

LRESULT CMainFrame::OnBookmarkNext(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->BookmarkNext();

	return 0;
}

LRESULT CMainFrame::OnBookmarkPrevious(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->BookmarkPrevious();

	return 0;
}

LRESULT CMainFrame::OnBookmarkClearAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spScriptEditor->BookmarkClearAll();

	return 0;
}

HRESULT CMainFrame::OnPositionChanged()
{
	long lPos;
	long lRow;
	long lCol;

	m_spScriptEditor->GetCurrentPos(&lPos);
	m_spScriptEditor->LineFromPosition(lPos, &lRow);
	m_spScriptEditor->GetColumn(lPos, &lCol);

	TCHAR szBuff[100];
	wsprintf(szBuff, _T("行: %i"), lRow + 1);
	m_sbar.SetPaneText(ID_ROW_PANE, szBuff);
	wsprintf(szBuff, _T("列: %i"), lCol + 1);
	m_sbar.SetPaneText(ID_COL_PANE, szBuff);

	return S_OK;
}
