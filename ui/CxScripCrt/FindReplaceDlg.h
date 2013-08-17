// FindReplaceDlg.h: interface for the CFindReplaceDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FINDREPLACEDLG_H__
#define _FINDREPLACEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"

class CScriptEditorView;

class CFindReplaceDlg :
	public CDialogImpl<CFindReplaceDlg>,
	public CWinDataExchange<CFindReplaceDlg> 
{
public:
	CFindReplaceDlg();

	enum { IDD = IDD_FIND };
	
	BEGIN_MSG_MAP(CFindReplaceDlg)
//		if ((m_pCurrentMsg->message >= WM_KEYFIRST && m_pCurrentMsg->message <= WM_KEYLAST)/* ||
//		   (m_pCurrentMsg->message >= WM_MOUSEFIRST && m_pCurrentMsg->message <= WM_MOUSELAST)*/)
//		{
//			if (IsDialogMessage((MSG*)m_pCurrentMsg))
//				return TRUE;
//		}
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_MARKALL, OnMarkAll)
		COMMAND_ID_HANDLER(IDC_REPLACE, OnReplace)
		COMMAND_ID_HANDLER(IDC_REPLACEALL, OnReplaceAll)
		COMMAND_HANDLER(IDC_FINDWHAT, CBN_EDITCHANGE, OnEditchangeFindwhat)
		COMMAND_ID_HANDLER(ID_DIRECTIONUP, OnDirectionup)
		COMMAND_ID_HANDLER(ID_DIRECTIONDOWN, OnDirectiondown)
	END_MSG_MAP()
		
	BEGIN_DDX_MAP(CFindReplaceDlg)
//		DDX_CONTROL(IDC_FINDWHAT, m_cmbFindWhat);
//		if (!m_bFind)
//		{
//			DDX_CONTROL(IDC_REPLACEWITH, m_cmbReplaceWith);
//		}
		DDX_CHECK(IDC_REGEXP, m_bRegExp);
		if (m_bFind)
		{	DDX_RADIO(ID_DIRECTIONUP, m_nDirection);
		}
		else
		{
			DDX_RADIO(ID_DIRECTIONUP, m_nRange);
		}
		DDX_CHECK(IDC_MATCHWORD, m_bWholeWord);
		DDX_CHECK(IDC_MATCHCASE, m_bMatchCase);
	END_DDX_MAP()
		
protected:
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
	LRESULT OnMarkAll(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
	LRESULT OnReplace(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
	LRESULT OnReplaceAll(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
	LRESULT OnEditchangeFindwhat(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
	LRESULT OnDirectionup(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
	LRESULT OnDirectiondown(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);

public:
	static CFindReplaceDlg* CreateEditorDialog(BOOL bFind = TRUE);
	// modeless dialogs
	HWND Create(HWND hWndParent, UINT nIDTemplate, LPARAM dwInitParam = NULL);

	void InitDialog();
	void OnEditchangeFindwhat();

	void UpdateFindString(LPCTSTR lpszString);
	void UpdateReplaceString(LPCTSTR lpszString);

	CComboBox m_cmbFindWhat;
	CComboBox m_cmbReplaceWith;
	BOOL m_bWholeWord;
	BOOL m_bRegExp;
	int	m_nDirection;
	int	m_nRange;
	BOOL m_bMatchCase;

	BOOL m_bFind;
};

#endif // _FINDREPLACEDLG_H__
