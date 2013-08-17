// FindReplaceDlg.cpp: implementation of the CFindReplaceDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FindReplaceDlg.h"
#include "ScriptEditorView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFindReplaceDlg::CFindReplaceDlg()
{
	m_nRange = 1;
}

CFindReplaceDlg* CFindReplaceDlg::CreateEditorDialog(BOOL bFind)
{
	ATLASSERT(CScriptEditorView::GetActiveView() != NULL);

	CFindReplaceDlg* pDlg = new CFindReplaceDlg;
	pDlg->m_bFind = bFind;
	pDlg->Create(CScriptEditorView::GetActiveView()->m_hWnd, bFind ? IDD_FIND: IDD_REPLACE);
	pDlg->InitDialog();
	
	pDlg->ShowWindow(SW_SHOW);
	
	return pDlg;
}

// modeless dialogs
HWND CFindReplaceDlg::Create(HWND hWndParent, UINT nIDTemplate, LPARAM dwInitParam)
{
	ATLASSERT(m_hWnd == NULL);
	_Module.AddCreateWndData(&m_thunk.cd, (CDialogImplBaseT< CWindow >*)this);
#ifdef _DEBUG
	m_bModal = false;
#endif //_DEBUG
	HWND hWnd = ::CreateDialogParam(_Module.GetResourceInstance(), MAKEINTRESOURCE(nIDTemplate),
		hWndParent, (DLGPROC)StartDialogProc, dwInitParam);
	ATLASSERT(m_hWnd == hWnd);
	return hWnd;
}

void CFindReplaceDlg::InitDialog()
{
	CScriptEditorView* pEditorView = CScriptEditorView::GetActiveView();
	ATLASSERT(pEditorView != NULL);

	m_cmbFindWhat.Attach(GetDlgItem(IDC_FINDWHAT));
	if (!m_bFind)
		m_cmbReplaceWith.Attach(GetDlgItem(IDC_REPLACEWITH));

	// ²éÕÒµÄ×Ö·û´®
	CStringArray arrStrings;
	pEditorView->GetSearchStrings(arrStrings);
	for (UINT i = 0; i < arrStrings.GetCount(); i++)
		m_cmbFindWhat.AddString(arrStrings[i]);
	
	CString strFind = pEditorView->GetSelText();
	// if selection is empty or spans multiple lines use old find text
	if (strFind.IsEmpty() || (strFind.FindOneOf(_T("\n\r")) != -1))
	{
		if (!m_bFind && !strFind.IsEmpty())
			m_nRange = 0;
		
		strFind.Empty();
		
		if (m_cmbFindWhat.GetCount() > 0)
			m_cmbFindWhat.GetLBText(0, strFind);
	}
	
	m_cmbFindWhat.SetWindowText(strFind);
	
	if (!m_bFind)
	{
		// Ìæ»»µÄ×Ö·û´®
		arrStrings.RemoveAll();
		pEditorView->GetReplaceStrings(arrStrings);
		for (i = 0; i < arrStrings.GetCount(); i++)
			m_cmbReplaceWith.AddString(arrStrings[i]);
		
		
		if (m_cmbReplaceWith.GetCount() > 0)
			m_cmbReplaceWith.SetCurSel(0);
	}
	
	BOOL bNext;
	pEditorView->GetSearchSetting(m_bMatchCase, m_bWholeWord, m_bRegExp, bNext);
	m_nDirection = bNext ? 1 : 0;
	
	OnEditchangeFindwhat();
	
	DoDataExchange(FALSE);
}

LRESULT CFindReplaceDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	_SCINTILLAEDIT_STATE* pEditState = GetScintillaEditState();
	pEditState->pFindReplaceDlg = NULL;

	return 0;
}

void CFindReplaceDlg::UpdateFindString(LPCTSTR lpszString)
{
	int nIndex = m_cmbFindWhat.FindStringExact(-1, lpszString);
	if (nIndex != -1)
		m_cmbFindWhat.DeleteString(nIndex);

	m_cmbFindWhat.InsertString(0, lpszString);
	m_cmbFindWhat.SetCurSel(0);
}

void CFindReplaceDlg::UpdateReplaceString(LPCTSTR lpszString)
{
	int nIndex = m_cmbReplaceWith.FindStringExact(-1, lpszString);
	if (nIndex != -1)
		m_cmbReplaceWith.DeleteString(nIndex);

	m_cmbReplaceWith.InsertString(0, lpszString);
	m_cmbReplaceWith.SetCurSel(0);
}

LRESULT CFindReplaceDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled) 
{
	USES_CONVERSION;
	CScriptEditorView* pEditorView = CScriptEditorView::GetActiveView();
	if (pEditorView == NULL)
		return 0;

	DoDataExchange(TRUE);

	int nSearchFlags = 0;
	if (m_bWholeWord)
		nSearchFlags |= SCFIND_WHOLEWORD;
	if (m_bMatchCase)
		nSearchFlags |= SCFIND_MATCHCASE;
	if (m_bRegExp)
		nSearchFlags |= SCFIND_REGEXP;

	CComBSTR bstrFind;
	m_cmbFindWhat.GetWindowText((BSTR&)bstrFind);
	UpdateFindString(OLE2T(bstrFind));

	pEditorView->StoreSearchString(OLE2T(bstrFind));
	pEditorView->StoreSearchSetting(m_bMatchCase, m_bWholeWord, m_bRegExp, m_nDirection != 0);

	if (pEditorView->FindNext(nSearchFlags, OLE2T(bstrFind), m_nDirection == 0) == -1)
		MessageBeep(MB_ICONHAND);
	else
		pEditorView->AdjustDialogPosition(this);

	return 0;
}

LRESULT CFindReplaceDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled) 
{
	DestroyWindow();

	return 0;
}

LRESULT CFindReplaceDlg::OnMarkAll(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled) 
{
	USES_CONVERSION;
	CScriptEditorView* pEditorView = CScriptEditorView::GetActiveView();
	if (pEditorView == NULL)
		return 0;
	
	DoDataExchange(TRUE);
	
	int nSearchFlags = 0;
	if (m_bWholeWord)
		nSearchFlags |= SCFIND_WHOLEWORD;
	if (m_bMatchCase)
		nSearchFlags |= SCFIND_MATCHCASE;
	if (m_bRegExp)
		nSearchFlags |= SCFIND_REGEXP;
	
	CComBSTR bstrFind;
	m_cmbFindWhat.GetWindowText((BSTR&)bstrFind);
	UpdateFindString(OLE2T(bstrFind));

	pEditorView->StoreSearchString(OLE2T(bstrFind));
	pEditorView->StoreSearchSetting(m_bMatchCase, m_bWholeWord, m_bRegExp, m_nDirection != 0);
	if (pEditorView->MarkAll(nSearchFlags, OLE2T(bstrFind)) > 0)
		pEditorView->AdjustDialogPosition(this);

	return 0;
}

void CFindReplaceDlg::OnEditchangeFindwhat()
{
	CComBSTR bstrFind;
	m_cmbFindWhat.GetWindowText((BSTR&)bstrFind);
	CString strFind = bstrFind;
	
	::EnableWindow(GetDlgItem(IDOK), !strFind.IsEmpty());
	if (m_bFind)
	{
		::EnableWindow(GetDlgItem(IDC_MARKALL), !strFind.IsEmpty());
	}
	else
	{
		::EnableWindow(GetDlgItem(IDC_REPLACE), !strFind.IsEmpty() && m_nRange != 0);
		::EnableWindow(GetDlgItem(IDC_REPLACEALL), !strFind.IsEmpty());
	}
}

LRESULT CFindReplaceDlg::OnEditchangeFindwhat(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled) 
{
	OnEditchangeFindwhat();

	return 0;
}

LRESULT CFindReplaceDlg::OnDirectionup(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled) 
{
	if (m_bFind)
		return 0;

	::EnableWindow(GetDlgItem(IDC_REPLACE), FALSE);
	return 0;
}

LRESULT CFindReplaceDlg::OnDirectiondown(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled) 
{
	if (m_bFind)
		return 0;

	CComBSTR bstrFind;
	m_cmbFindWhat.GetWindowText((BSTR&)bstrFind);
	CString strFind = bstrFind;
	::EnableWindow(GetDlgItem(IDC_REPLACE), !strFind.IsEmpty());

	return 0;
}

LRESULT CFindReplaceDlg::OnReplace(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled)
{
	USES_CONVERSION;
	CScriptEditorView* pEditorView = CScriptEditorView::GetActiveView();
	if (pEditorView == NULL)
		return 0;
	
	DoDataExchange(TRUE);
	
	int nSearchFlags = 0;
	if (m_bWholeWord)
		nSearchFlags |= SCFIND_WHOLEWORD;
	if (m_bMatchCase)
		nSearchFlags |= SCFIND_MATCHCASE;
	if (m_bRegExp)
		nSearchFlags |= SCFIND_REGEXP;
	
	CComBSTR bstrFind;
	m_cmbFindWhat.GetWindowText((BSTR&)bstrFind);
	UpdateFindString(OLE2T(bstrFind));

	pEditorView->StoreSearchString(OLE2T(bstrFind));

	CComBSTR bstrReplace;
	m_cmbReplaceWith.GetWindowText((BSTR&)bstrReplace);
	UpdateReplaceString(OLE2T(bstrReplace));

	pEditorView->StoreReplaceString(OLE2T(bstrReplace));
	
	pEditorView->StoreSearchSetting(m_bMatchCase, m_bWholeWord, m_bRegExp, m_nDirection != 0);
	if (pEditorView->ReplaceOnce(nSearchFlags, OLE2T(bstrFind), OLE2T(bstrReplace)))
		pEditorView->AdjustDialogPosition(this);
	else
		MessageBeep(MB_ICONHAND);

	return 0;
}

LRESULT CFindReplaceDlg::OnReplaceAll(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled)
{
	USES_CONVERSION;
	CScriptEditorView* pEditorView = CScriptEditorView::GetActiveView();
	if (pEditorView == NULL)
		return 0;
	
	DoDataExchange(TRUE);
	
	int nSearchFlags = 0;
	if (m_bWholeWord)
		nSearchFlags |= SCFIND_WHOLEWORD;
	if (m_bMatchCase)
		nSearchFlags |= SCFIND_MATCHCASE;
	if (m_bRegExp)
		nSearchFlags |= SCFIND_REGEXP;
	
	CComBSTR bstrFind;
	m_cmbFindWhat.GetWindowText((BSTR&)bstrFind);
	UpdateFindString(OLE2T(bstrFind));

	pEditorView->StoreSearchString(OLE2T(bstrFind));

	CComBSTR bstrReplace;
	m_cmbReplaceWith.GetWindowText((BSTR&)bstrReplace);
	UpdateReplaceString(OLE2T(bstrReplace));

	pEditorView->StoreReplaceString(OLE2T(bstrReplace));

	pEditorView->StoreSearchSetting(m_bMatchCase, m_bWholeWord, m_bRegExp, m_nDirection != 0);
	if (pEditorView->ReplaceAll(nSearchFlags, OLE2T(bstrFind), OLE2T(bstrReplace), m_nRange == 0) > 0)
		pEditorView->AdjustDialogPosition(this);
	else
		MessageBeep(MB_ICONHAND);

	return 0;
}