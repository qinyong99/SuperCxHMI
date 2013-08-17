// ScriptEidtorView.h: interface for the CScriptEditorView class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SCRIPTVIEW_H_
#define __SCRIPTVIEW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScintillaEditCtrl.h"
#include "ScriptAnalyzer.h"

#define MARKER_BOOKMARK 1

class CFindReplaceDlg;
class CScripter;

interface ICxScriptEditorSite;

BOOL PASCAL InitScintillaEdit();

class _SCINTILLAEDIT_STATE
{
public:
	_SCINTILLAEDIT_STATE();
	virtual ~_SCINTILLAEDIT_STATE();

	CFindReplaceDlg* pFindReplaceDlg; // find or replace dialog
	CComObject<CScripter>* pScripter;
	DWORD m_dwScripterRef;
	BOOL bFindOnly; // Is pFindReplace the find or replace?
	CString strFind;    // last find string
	CString strReplace; // last replace string
	BOOL bCase; // TRUE==case sensitive, FALSE==not
	int bNext;  // TRUE==search down, FALSE== search up
	BOOL bWord; // TRUE==match whole word, FALSE==not
	BOOL bRegExp; // 是否使用正则表达式
	CStringArray arrSearchStrings;
	CStringArray arrReplaceStrings;
	
	HINSTANCE m_hInstScintillaEdit;      // handle to RICHED32.DLL
};

_SCINTILLAEDIT_STATE* GetScintillaEditState();

class CScriptEditorView : 
	public CWindowImpl<CScriptEditorView, CScintillaEditCtrl>,
	public CScriptAnalyzer
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CScintillaEditCtrl::GetWndClassName())

	CScriptEditorView();
	virtual ~CScriptEditorView();

	BEGIN_MSG_MAP(CScriptEditorView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	ALT_MSG_MAP(1)
		NOTIFY_CODE_HANDLER(SCN_CHARADDED, OnCharAdded)
		NOTIFY_CODE_HANDLER(SCN_MARGINCLICK, OnMarginClick)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnCharAdded(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnMarginClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	
	void InitScintillaEditCtrl();

	// Attributes
public:
	//	long GetTextLength() const;
	//	BOOL CanPaste() const;
	
	CString GetSelText();

	HWND GetFindDialogHandle();
	
	// Operations
public:
	void GoLine();
	void BookmarkToggle();
	void BookmarkNext();
	void BookmarkPrevious();
	void BookmarkClearAll();
	void FindRepeat();
	void EditFindReplace(BOOL bFind);

	CString GetCurrentLine(int& nPos);
	
	void SetLineIndentation(int nLine, int nIndent);
// 	int GetLineIndentation(int nLine);
	int GetCurrentLineNumber();
	int GetLineLength(int nLine);
	
	void AdjustDialogPosition(CWindow* pDlg);
	
	BOOL GetObjectMemberParams(ITypeInfo* pTypeInfo, CStringArray& arrWords, CStringArray& arrParams);
	BOOL GetObjectMembers(ITypeInfo* pTypeInfo, CStringArray& arrWords, CStringArray& arrProperties,CStringArray& arrMethods); 
	
	virtual void GetSearchStrings(CStringArray& arrStrings);
	virtual void StoreSearchString(LPCTSTR lpszString);
	virtual void GetReplaceStrings(CStringArray& arrStrings);
	virtual void StoreReplaceString(LPCTSTR lpszString);
	virtual void StoreSearchSetting(BOOL bCase, BOOL bWord, BOOL bRegExp, BOOL bNext);
	virtual void GetSearchSetting(BOOL& bCase, BOOL& bWord, BOOL& bRegExp, BOOL& bNext);
	
	void ReAnalyzeText();
	virtual BOOL IsEventHandler(LPCTSTR lpszSubName);
	void InsertObjectEventHandler(LPCTSTR lpszObject, LPCTSTR lpszEvent);
	void DeleteEmptyEventHandler();

	int FindNext(int nSearchFlags, LPCTSTR lpszString, BOOL bReverseDirection = FALSE);
	int MarkAll(int nSearchFlags, LPCTSTR lpszString);
	BOOL ReplaceOnce(int nSearchFlags, LPCTSTR lpszFindTarget, 
		LPCTSTR lpszReplaceTarget);
	int ReplaceAll(int nSearchFlags, LPCTSTR lpszFindString, 
		LPCTSTR lpsReplaceString, BOOL bInSelection = FALSE);
	
	void EnsureRangeVisible(int nPosStart, int nPosEnd, BOOL bEnforcePolicy = TRUE);

	virtual BOOL GetObjectMembers(CStringArray& arrWords, CStringArray& arrProperties,CStringArray& arrMethods);
	virtual BOOL GetObjectMemberParams(CStringArray& arrWords, CStringArray& arrParams);
	
	virtual void GetObjectNames(CStringArray& arrObjects);
	virtual void GetObjectEvents(LPCTSTR lpszObject, CStringArray& arrEvents);
	virtual void GetObjectEventParams(LPCTSTR lpszObject, LPCTSTR lpszEvent, CStringArray &params);
	virtual BOOL ShowHelpString(LPCTSTR lpszObject);

	void GetObjectEventOrParams(ITypeInfo* pTypeInfo, CStringArray& arrInfos, LPCTSTR lpszEvent = NULL);
	void GetObjectEventOrParams(LPUNKNOWN pUnkObject, CStringArray& arrInfos, LPCTSTR lpszEvent = NULL);

	void SetActive(BOOL bActive);
	static CScriptEditorView* GetActiveView() { return m_pActiveEditorView;	}

	ICxScriptEditorSite* m_pScriptEditorSite;

protected:
	CString ConvertToAutoCList(CStringArray& arrProperties, CStringArray& arrMethods);
	
	void DoFolding(int nMargin, int nPosition);
	
	void CharAdded(char ch);
	void MaintainIndentation(char ch);
	void StartAutoCompleteWord();
	void StartCallTip();
	
	//	virtual BOOL StartCallTip();
	//	void ContinueCallTip();
	
	//	virtual BOOL StartAutoComplete();
	//	virtual BOOL StartAutoCompleteWord(BOOL bOnlyOneWord);
	
	int GetLineNumberChars();
	void SetLineNumberWidth();
	
	BOOL ParsePrevWords(LPCTSTR lpszLine, int& pos, CStringArray& arrWord, int nBraceCount = 0);
	int GetParamIndex(LPCTSTR lpszLine, int& nPos);
	
protected:
	int m_nLineNumerChars;

	static CScriptEditorView* m_pActiveEditorView;
};

#endif // __SCRIPTVIEW_H_
