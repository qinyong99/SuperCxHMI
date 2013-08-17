// ScriptEidtor.cpp : Implementation of CScriptEditor

#include "stdafx.h"
#include "CxScripCrt.h"
#include "ScriptEditor.h"

/////////////////////////////////////////////////////////////////////////////
// CScriptEditor

CScriptEditor::CScriptEditor() :
	m_wndPaneScript(this, 1)
{
	m_bWindowOnly = TRUE;

	m_bSetSplitterPos = FALSE;
	m_objRF.AddRef();
	m_font = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);

	m_nLastLine = -1;
	m_bChanged = FALSE;
}

LRESULT CScriptEditor::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	m_winSplitter.Create(m_hWnd, rcClient);
	
	// 创建符号面板
	m_paneSymbol.Create(m_winSplitter);
	m_paneSymbol.SetTitle(_T("符号"));
	
	// 创建符号视图
	m_viewSymbol.Create(m_paneSymbol, rcDefault, NULL, 
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS/*| WS_VSCROLL | WS_HSCROLL |*/ 
		/*TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_DISABLEDRAGDROP*/,
		WS_EX_CLIENTEDGE, 101);
	m_paneSymbol.SetClient(m_viewSymbol);
 	m_paneSymbol.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
	
	// 创建脚本面板
	m_paneScript.Create(m_winSplitter);
	m_paneScript.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
	
	m_wndPaneScript.SubclassWindow(m_paneScript);

	m_btnViewSymbol.Create(m_paneScript, rcDefault, NULL, 
		WS_CHILD | WS_VISIBLE | BS_FLAT/* | BS_AUTOCHECKBOX | BS_PUSHLIKE*/ | BS_FLAT | BS_CENTER | 
                    BS_VCENTER, 0, ID_PANE_CLOSE);
	m_btnViewSymbol.SetFont(m_font);
	m_btnViewSymbol.SetWindowText(_T("<<"));
	m_btnViewSymbol.SetCheck(1);

	CRect rect;
	rect.SetRect(0, 0, 0, 240);
	m_cmbObjects.Create(m_paneScript, rect, NULL, 
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_SORT | CBS_HASSTRINGS | 
		WS_VSCROLL | WS_TABSTOP, 0, 103);
	
	m_cmbEvents.Create(m_paneScript, rect, NULL, 
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_SORT | CBS_HASSTRINGS | 
		WS_VSCROLL | WS_TABSTOP, WS_EX_CLIENTEDGE, 104);
	
	m_viewScript.Create(m_paneScript, rcDefault, NULL, 
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | 
		ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE |
		ES_NOHIDESEL | ES_SAVESEL | ES_SELECTIONBAR,
		WS_EX_CLIENTEDGE, 102);
	m_paneScript.SetClient(m_viewScript);
	
	m_winSplitter.SetSplitterPanes(m_paneScript, m_paneSymbol);

	m_tip.Create(m_hWnd);
	ATLASSERT(m_tip.IsWindow());
	m_tip.Activate(TRUE);
	m_tip.AddTool(m_btnViewSymbol, _T("隐藏符号窗口"));
	m_tip.AddTool(m_cmbObjects, _T("对象"));
	m_tip.AddTool(m_cmbEvents, _T("过程"));

	return 0;
}

LRESULT CScriptEditor::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CRect rect;
	GetClientRect(&rect);

	::SetWindowPos(m_winSplitter, NULL, rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
				SWP_NOZORDER | SWP_NOACTIVATE);

	if (!m_bSetSplitterPos && !rect.IsRectEmpty())
	{
		m_winSplitter.SetSplitterPos(rect.Width() - 172);
		m_bSetSplitterPos = TRUE;
	}

	return 0;
}

LRESULT CScriptEditor::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_viewScript.SetFocus();

	return 0;
}

LRESULT CScriptEditor::OnScriptPaneSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
//	CRect rect;
//	m_paneScript.GetClientRect(rect);
//	
//	if (::IsWindow(m_btnViewSymbol.m_hWnd))
//		m_btnViewSymbol.SetWindowPos(NULL, rect.left + 1, rect.top + 1, 19, 19, SWP_NOZORDER | SWP_NOACTIVATE);
//	
//	rect.left += 23;
//	
//	if (::IsWindow(m_cmbObjects.m_hWnd))
//	{
//		m_cmbObjects.SetWindowPos(NULL, rect.left, rect.top,
//			rect.Width() / 2, 180, SWP_NOACTIVATE | SWP_NOZORDER);
//	}
//	if (::IsWindow(m_cmbEvents.m_hWnd))
//	{
//		m_cmbEvents.SetWindowPos(NULL, rect.left + rect.Width() / 2 + 1, rect.top,
//			rect.Width() / 2, 180, SWP_NOACTIVATE | SWP_NOZORDER);
//	}

	CRect rectClient;
	m_paneScript.GetClientRect(rectClient);

	CRect rect = rectClient;
	rect.right -= 23;
	
	if (::IsWindow(m_cmbObjects.m_hWnd))
	{
		m_cmbObjects.SetWindowPos(NULL, rect.left, rect.top,
			rect.Width() / 2, 180, SWP_NOACTIVATE | SWP_NOZORDER);
	}
	if (::IsWindow(m_cmbEvents.m_hWnd))
	{
		m_cmbEvents.SetWindowPos(NULL, rect.left + rect.Width() / 2 + 1, rect.top,
			rect.Width() / 2, 180, SWP_NOACTIVATE | SWP_NOZORDER);
	}

	rect.left = rect.right + 3;
	rect.right = rectClient.right;

	if (::IsWindow(m_btnViewSymbol.m_hWnd))
		m_btnViewSymbol.SetWindowPos(NULL, rect.left, rect.top + 1, rect.Width() - 1, 19, SWP_NOZORDER | SWP_NOACTIVATE);
		
	bHandled = FALSE;
	
	return 1;
}

LRESULT CScriptEditor::OnPaneClose(WORD, WORD, HWND hWndCtl, BOOL&)
{ 
	int nPane = m_winSplitter.GetSinglePaneMode();
	m_winSplitter.SetSinglePaneMode(nPane == 0 ? SPLIT_PANE_NONE : 0);

	BOOL b = m_winSplitter.GetSinglePaneMode() == 0;
	m_btnViewSymbol.SetWindowText(b ? _T(">>") : _T("<<"));
	m_tip.UpdateTipText(b ? _T("显示符号窗口") : _T("隐藏符号窗口"), m_btnViewSymbol);
	SetFocus();

	Fire_SymbolViewToggled();

	return 0; 
}

LRESULT CScriptEditor::OnSymbolSelect(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CTreePage* pTreePage = m_viewSymbol.GetCurrentTreePage();
	CString strText = pTreePage->GetSelectNameSpace();

	if (!strText.IsEmpty())
	{
		// 维持缩进
		int nCurLine = m_viewScript.GetCurrentLineNumber();
		int nIndentAmount = m_viewScript.GetLineIndentation(nCurLine);

		int nNewLineCount = 0;
		int nIndex = 0;
		while ((nIndex = strText.Find('\n', nIndex)) != -1)
		{
			nIndex += 1;
			nNewLineCount++;
		}

		m_viewScript.BeginUndoAction();

		m_viewScript.ReplaceSel(strText);

		for (int i = 1; i <= nNewLineCount; i++)
			m_viewScript.SetLineIndentation(nCurLine + i, nIndentAmount);

		m_viewScript.EndUndoAction();
	}

	m_viewScript.SetFocus();		

	return 0;
}

void CScriptEditor::LoadObjects()
{
	m_cmbObjects.ResetContent();

	int nIndex = m_cmbObjects.AddString(_T("(通用)"));
	if (nIndex != CB_ERR)
		m_cmbObjects.SetItemBold(nIndex, TRUE);
	
	CStringArray arrObjects;
	m_viewScript.GetObjectNames(arrObjects);
	
	for (UINT i = 0; i < arrObjects.GetCount(); i++)
	{
		nIndex = m_cmbObjects.AddString(arrObjects[i]);
		if (nIndex != CB_ERR)
			m_cmbObjects.SetItemBold(nIndex, TRUE);
	}
}

void CScriptEditor::LoadEvents()
{
	CString strObject;

	m_cmbEvents.ResetContent();
	
	int nSel = m_cmbObjects.GetCurSel();
	if (nSel == CB_ERR)
		return;

	m_cmbObjects.GetLBText(nSel, strObject);
	
	if (strObject == _T("(通用)"))
	{
		m_cmbEvents.AddString(_T("(声明)"));

		CStringArray arrBlocks;
		m_viewScript.GetGeneralBlock(arrBlocks);
		for (UINT i = 0; i < arrBlocks.GetCount(); i++)
			m_cmbEvents.AddString(arrBlocks[i]);
	}
	else
	{
		BOOL bShowHelpString = m_viewScript.ShowHelpString(strObject);

		CStringArray arrEvents;
		m_viewScript.GetObjectEvents(strObject, arrEvents);
		for (UINT i = 0; i < arrEvents.GetCount(); i++)
		{
			CString strEvent = arrEvents[i];
			if (!bShowHelpString)
			{
				int nIndex = strEvent.Find(_T(" "));
				if (nIndex != -1)
					strEvent = strEvent.Left(nIndex + 1);
			}

			m_cmbEvents.AddString(strEvent);
			if (i == 0)
				m_cmbEvents.SetItemData(0, -1);
		}
	}
}

LRESULT CScriptEditor::OnDropdownObjects(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nSel = m_cmbObjects.GetCurSel();
	CString strSelObject;
	if (nSel != CB_ERR)
		m_cmbObjects.GetLBText(nSel, strSelObject);
	
	LoadObjects();
	
	m_cmbObjects.SelectStringExact(0, strSelObject);

	return 0;
}

BOOL CScriptEditor::SelectDefaultEvent()
{
	CString strObject;
	int nSel = m_cmbObjects.GetCurSel();
	m_cmbObjects.GetLBText(nSel, strObject);

	nSel = -1;
	int nDefaultEvent = 0;
	for (int i = 0; i < m_cmbEvents.GetCount(); i++)
	{
		if (m_cmbEvents.GetItemData(i) == -1)
			nDefaultEvent = i;

		CString strEventT;
		m_cmbEvents.GetLBText(i, strEventT);
		
		int iIndex = strEventT.Find(_T(" "));
		if (iIndex != -1)
			strEventT = strEventT.Left(iIndex);

		if (m_viewScript.HasObjectEventHandler(strObject, strEventT))
		{
			nSel = i;
			break;
		}
	}

	if (nSel == -1 && m_cmbEvents.GetCount() > 0)
		nSel = nDefaultEvent;

	if (nSel != -1)
	{
		m_cmbEvents.SetCurSel(nSel);
		m_cmbEvents.SetItemBold(nSel, TRUE);
		return TRUE;
	}

	return FALSE;
}

LRESULT CScriptEditor::OnSelchangeObjects(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ReAnalyzeText();

	LoadEvents();
	SelectDefaultEvent();
	OnSelchangeEvents();
	
	return 0;
}

LRESULT CScriptEditor::OnDropdownEvents(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nSel = m_cmbObjects.GetCurSel();
	if (nSel == CB_ERR)
		return 0;
	
	CString strObject;
	m_cmbObjects.GetLBText(m_cmbObjects.GetCurSel(), strObject);

	if (strObject == _T("(通用)"))
	{
		for (int i = 0; i < m_cmbEvents.GetCount(); i++)
			m_cmbEvents.SetItemBold(i , TRUE);
		return 0;
	}

	CStringArray arrEventSubs;
	m_viewScript.GetObjectEventHandler(strObject, arrEventSubs);
	
	for (int i = 0; i < m_cmbEvents.GetCount(); i++)
	{
		CString strEvent;
		m_cmbEvents.GetLBText(i, strEvent);

		int nIndex = strEvent.Find(_T(" "));
		if (nIndex != -1)
			strEvent = strEvent.Left(nIndex);

		CString strEventSub = strObject;
		strEventSub += ('_');
		strEventSub += strEvent;

		BOOL b = FALSE;
		for (UINT j = 0; j < arrEventSubs.GetCount(); j++)
		{
			if (arrEventSubs[j].CompareNoCase(strEventSub) == 0)
				b = TRUE;
		}
		m_cmbEvents.SetItemBold(i, b);
	}

	return 0;
}

void CScriptEditor::OnSelchangeEvents()
{
	CString strObject;
	int nSel = m_cmbObjects.GetCurSel();
	if (nSel == LB_ERR)
		return;
	m_cmbObjects.GetLBText(nSel, strObject);

	CString strEvent;
	nSel = m_cmbEvents.GetCurSel();
	if (nSel == LB_ERR)
		return;
	m_cmbEvents.GetLBText(nSel, strEvent);

	if (strObject == _T("(通用)"))
	{
		if (strEvent == _T("(声明)"))
		{
			int nPos = m_viewScript.GetFirstBlockBegin();
			if (nPos == 0)
			{
				m_viewScript.InsertText(0, _T("\r\n"));
				GotoLine(0);
			}
			else if (m_viewScript.GetCurrentPos() >= nPos)
			{
				GotoLine(0);
			}
		}
		else
		{
			int nPos = m_viewScript.GetGeneralBlockBodyBegin(strEvent);
			if (nPos != -1)
			{
				int nLine = m_viewScript.LineFromPosition(nPos);
				GotoLine(nLine);
			}
		}
		
		SetFocus();
		return;
	}
	
	int nIndex = strEvent.Find(_T(" "));
	if (nIndex != -1)
		strEvent = strEvent.Left(nIndex);
	
	m_viewScript.InsertObjectEventHandler(strObject, strEvent);
	m_cmbEvents.SetItemBold(nSel, TRUE);

	m_nLastLine = -1;

	SetFocus();
}

LRESULT CScriptEditor::OnSelchangeEvents(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnSelchangeEvents();

	return 0;
}

STDMETHODIMP CScriptEditor::Undo()
{
	m_viewScript.Undo();
	
	return S_OK;
}

STDMETHODIMP CScriptEditor::Redo()
{
	m_viewScript.Redo();

	return S_OK;
}

STDMETHODIMP CScriptEditor::Cut()
{
	m_viewScript.Cut();
	
	return S_OK;
}

STDMETHODIMP CScriptEditor::Copy()
{
	m_viewScript.Copy();

	return S_OK;
}

STDMETHODIMP CScriptEditor::Paste()
{
	m_viewScript.Paste();

	return S_OK;
}

STDMETHODIMP CScriptEditor::Clear()
{
	m_viewScript.Clear();

	return S_OK;
}

STDMETHODIMP CScriptEditor::Find()
{
	m_viewScript.EditFindReplace(TRUE);

	return S_OK;
}

STDMETHODIMP CScriptEditor::FindRepeat()
{
	m_viewScript.FindRepeat();

	return S_OK;
}

STDMETHODIMP CScriptEditor::Replace()
{
	m_viewScript.EditFindReplace(FALSE);

	return S_OK;
}

STDMETHODIMP CScriptEditor::GoLine()
{
	m_viewScript.GoLine();

	return S_OK;
}

STDMETHODIMP CScriptEditor::BookmarkToggle()
{
	m_viewScript.BookmarkToggle();

	return S_OK;
}

STDMETHODIMP CScriptEditor::BookmarkNext()
{
	m_viewScript.BookmarkNext();

	return S_OK;
}

STDMETHODIMP CScriptEditor::BookmarkPrevious()
{
	m_viewScript.BookmarkPrevious();

	return S_OK;
}

STDMETHODIMP CScriptEditor::BookmarkClearAll()
{
	m_viewScript.BookmarkClearAll();

	return S_OK;
}

STDMETHODIMP CScriptEditor::SelectAll()
{
	m_viewScript.SelectAll();

	return S_OK;
}

STDMETHODIMP CScriptEditor::EmptyUndoBuffer()
{
	m_viewScript.EmptyUndoBuffer();

	return S_OK;
}

STDMETHODIMP CScriptEditor::SetSavePoint()
{
	m_viewScript.SetSavePoint();

	return S_OK;
}

STDMETHODIMP CScriptEditor::GetModify(BOOL *pbModify)
{
	*pbModify = m_viewScript.GetModify();

	return S_OK;
}

STDMETHODIMP CScriptEditor::SetScriptEditorSite(IUnknown *pSite)
{
	m_viewScript.m_pScriptEditorSite = (ICxScriptEditorSite*)pSite;

	return S_OK;
}

STDMETHODIMP CScriptEditor::get_Text(BSTR *pVal)
{
	CString strText;
	m_viewScript.GetText(strText);
	*pVal = strText.AllocSysString();

	return S_OK;
}

STDMETHODIMP CScriptEditor::put_Text(BSTR newVal)
{
	USES_CONVERSION;
	m_viewScript.SetText(OLE2T(newVal));
//	m_viewScript.ConvertEOLs(0);

	return S_OK;
}

STDMETHODIMP CScriptEditor::EditObjectEventHandler(BSTR bstrObject, BSTR bstrEvent, BOOL bCreateAndEdit)
{
	USES_CONVERSION;

	ReAnalyzeText();
	LoadObjects();
	
	if (bstrObject == NULL)
	{
		m_nLastLine = -1;
		SyncObjectAndEvents();

		SetFocus();
		return S_OK;
	}
	
	m_cmbObjects.SelectStringExact(0, OLE2T(bstrObject));
	LoadEvents();

	if (bstrEvent == NULL)
	{
		SelectDefaultEvent();
	}
	else
	{
		CString strEvent = bstrEvent;
		strEvent += _T(" ");
		m_cmbEvents.SelectString(0, strEvent);
	}

	if (bCreateAndEdit)
		OnSelchangeEvents();
		
	return S_OK;
}

STDMETHODIMP CScriptEditor::IsSelected(BOOL *pbSel)
{
	int nStartChar = m_viewScript.GetSelectionStart();
	int nEndChar = m_viewScript.GetSelectionEnd();
	*pbSel = nStartChar != nEndChar;

	return S_OK;
}

STDMETHODIMP CScriptEditor::GetTextLength(long *plCount)
{
	*plCount = m_viewScript.GetLength();

	return S_OK;
}

STDMETHODIMP CScriptEditor::HasSearched(BOOL *pbSearch)
{
	CStringArray arrStrings;
	m_viewScript.GetSearchStrings(arrStrings);
	*pbSearch = arrStrings.GetCount() > 0;

	return S_OK;
}

STDMETHODIMP CScriptEditor::CanPaste(BOOL *pbPaste)
{
	*pbPaste = m_viewScript.CanPaste();
	
	return S_OK;
}

STDMETHODIMP CScriptEditor::CanRedo(BOOL *pbRedo)
{
	*pbRedo = m_viewScript.CanRedo();
	
	return S_OK;
}

STDMETHODIMP CScriptEditor::CanUndo(BOOL *pbUndo)
{
	*pbUndo = m_viewScript.CanUndo();
	
	return S_OK;
}

void CScriptEditor::ReAnalyzeText()
{
	if (m_bChanged)
	{
		m_viewScript.ReAnalyzeText();
		m_bChanged = FALSE;
	}
}

void CScriptEditor::SyncObjectAndEvents()
{
	int nIndex;

	int nLine = m_viewScript.GetCurrentLineNumber();
	if (nLine == m_nLastLine)
		return;

	m_cmbEvents.SetRedraw(false);

	m_nLastLine = nLine;
	ReAnalyzeText();

	int nPos = m_viewScript.GetCurrentPos();
	CString strBlockName = m_viewScript.GetContext(nPos);

	if (!strBlockName.IsEmpty())
	{
		CString strBlockNameT = strBlockName;
		strBlockNameT.MakeUpper();

		CStringArray arrObjects;
		m_viewScript.GetObjectNames(arrObjects);
		
		for (UINT i = 0; i < arrObjects.GetCount(); i++)
		{
			CString strEventSubName = arrObjects[i];
			strEventSubName += _T('_');
			strEventSubName.MakeUpper();

			if (strBlockNameT.Find(strEventSubName, 0) == 0)
			{
				m_cmbObjects.SelectStringExact(0, arrObjects[i]);
				LoadEvents();

				CString strEvent = strBlockName;
				strEvent.Delete(0, strEventSubName.GetLength());
				strEvent += _T(" ");
				int nIndex = m_cmbEvents.SelectString(0, strEvent);
				if (nIndex != CB_ERR)
				{
					m_cmbEvents.SetItemBold(nIndex, TRUE);
					goto onexit;
				}
			}
		}
	}

	m_cmbObjects.SelectString(0, _T("(通用)"));
	LoadEvents();

	if (strBlockName.IsEmpty())
		strBlockName = _T("(声明)");
	nIndex = m_cmbEvents.SelectStringExact(0, strBlockName);
	if (nIndex != CB_ERR)
		m_cmbEvents.SetItemBold(nIndex, TRUE);

onexit:
	m_cmbEvents.SetRedraw(true);
}

LRESULT CScriptEditor::OnUpdateUIScript(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	Fire_PositionChanged();

	SyncObjectAndEvents();
	
	return 0;
}

LRESULT CScriptEditor::OnSavePointLeft(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	Fire_Modified();
	
	return 0;
}

LRESULT CScriptEditor::OnModified(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	SCNotification* pscn = (SCNotification *)pnmh;
	if (pscn->modificationType & SC_MOD_DELETETEXT)
	{
		if (pscn->linesAdded != 0)
			m_nLastLine = -1;
	}

	m_bChanged = TRUE;
	return 0;
}

BOOL CScriptEditor::PreTranslateAccelerator(LPMSG pMsg, HRESULT& /*hRet*/)
{
	if (m_tip.IsWindow())
		m_tip.RelayEvent(pMsg);

	return FALSE;
}

STDMETHODIMP CScriptEditor::SetSelectionStart(long lPos)
{
	m_viewScript.SetSelectionStart(lPos);

	return S_OK;
}

STDMETHODIMP CScriptEditor::SetSelectionEnd(long lPos)
{
	m_viewScript.SetSelectionEnd(lPos);

	return S_OK;
}

STDMETHODIMP CScriptEditor::ReplaceSel(BSTR bstrText)
{
	USES_CONVERSION;
	m_viewScript.ReplaceSel(OLE2T(bstrText));

	return S_OK;
}

STDMETHODIMP CScriptEditor::AppendText(long lLength, BSTR bstrText)
{
	USES_CONVERSION;
	m_viewScript.AppendText(lLength, OLE2T(bstrText));

	return S_OK;
}

STDMETHODIMP CScriptEditor::GotoLine(long lLine)
{
	m_viewScript.GotoLine(lLine);

	return S_OK;
}

STDMETHODIMP CScriptEditor::GetLineCount(long *plCount)
{
	*plCount = m_viewScript.GetLineCount();

	return S_OK;
}

STDMETHODIMP CScriptEditor::GetSelText(BSTR *pbstrText)
{
	*pbstrText = m_viewScript.GetSelText().AllocSysString();

	return S_OK;
}

STDMETHODIMP CScriptEditor::GetSelectionStart(long *plPos)
{
	*plPos = m_viewScript.GetSelectionStart();

	return S_OK;
}

STDMETHODIMP CScriptEditor::GetSelectionEnd(long *plPos)
{
	*plPos = m_viewScript.GetSelectionEnd();

	return S_OK;
}

STDMETHODIMP CScriptEditor::GetCurrentPos(long *plPos)
{
	*plPos = m_viewScript.GetCurrentPos();

	return S_OK;
}

STDMETHODIMP CScriptEditor::LineFromPosition(long lPos, long *plLine)
{
	*plLine = m_viewScript.LineFromPosition(lPos);

	return S_OK;
}

STDMETHODIMP CScriptEditor::GetColumn(long lPos, long *plCol)
{
	*plCol = m_viewScript.GetColumn(lPos);

	return S_OK;
}

STDMETHODIMP CScriptEditor::GetRangeToFormatStruct(ICxRangeToFormat **ppRangeToFormat)
{
	return m_objRF.QueryInterface(IID_ICxRangeToFormat, (void**)ppRangeToFormat);
}

STDMETHODIMP CScriptEditor::FormatRange(BOOL bDraw, BOOL bUseStruct, long *plPos)
{
	*plPos = m_viewScript.FormatRange(bDraw, bUseStruct ? &m_objRF.m_rf : NULL);

	return S_OK;
}

STDMETHODIMP CScriptEditor::BeginUndoAction()
{
	m_viewScript.BeginUndoAction();

	return S_OK;
}

STDMETHODIMP CScriptEditor::EndUndoAction()
{
	m_viewScript.EndUndoAction();

	return S_OK;
}

STDMETHODIMP CScriptEditor::DeleteEmptyEventHandler()
{
	ReAnalyzeText();
	m_viewScript.DeleteEmptyEventHandler();

	return S_OK;
}

STDMETHODIMP CScriptEditor::get_SymbolViewVisible(BOOL *pVal)
{
	*pVal = m_winSplitter.GetSinglePaneMode() != 0;

	return S_OK;
}

STDMETHODIMP CScriptEditor::put_SymbolViewVisible(BOOL newVal)
{
	if (newVal != (m_winSplitter.GetSinglePaneMode() != 0))
	{
		m_winSplitter.SetSinglePaneMode(newVal ? SPLIT_PANE_NONE : 0);
		
		BOOL b = m_winSplitter.GetSinglePaneMode() == 0;
		m_btnViewSymbol.SetWindowText(b ? _T(">>") : _T("<<"));
		m_tip.UpdateTipText(b ? _T("显示符号窗口") : _T("隐藏符号窗口"), m_btnViewSymbol);
	}

	return S_OK;
}

STDMETHODIMP CScriptEditor::get_FindDialogHandle(long *pVal)
{
	*pVal = (long)m_viewScript.GetFindDialogHandle();

	return *pVal != NULL ? S_OK : S_FALSE;
}

STDMETHODIMP CScriptEditor::SetActive(BOOL bActive)
{
	m_viewScript.SetActive(bActive);

	if (bActive)
	{
		LoadObjects();

		m_nLastLine = -1;
		SyncObjectAndEvents();
	}

	return S_OK;
}

STDMETHODIMP CScriptEditor::AddGeneralSymbolPage(DWORD dwSymbolMask)
{
	m_viewSymbol.AddGeneralPage(dwSymbolMask, m_hWnd);

	return S_OK;
}

STDMETHODIMP CScriptEditor::AddOPCServerSymbolPage()
{
	m_viewSymbol.AddOPCServerPage(m_hWnd);

	return S_OK;
}

STDMETHODIMP CScriptEditor::AddSymbolTreePage(BSTR bstrCaption, /*[out, retval]*/ ICxSymbolTreePage** ppPage)
{
	USES_CONVERSION;
	CTreePage* pTreePage = m_viewSymbol.AddTreePage(OLE2T(bstrCaption), TRUE, m_hWnd);
	if (ppPage != NULL)
		return pTreePage->QueryInterface(IID_ICxSymbolTreePage, (void**)ppPage);

	return S_OK;
}

STDMETHODIMP CScriptEditor::GetCurrentSymbolTreePage(/*[out, retval]*/ ICxSymbolTreePage** ppPage)
{
	ATLASSERT(ppPage != NULL);
	if (ppPage == NULL)
		return E_INVALIDARG;

	CTreePage* pTreePage = m_viewSymbol.GetCurrentTreePage();
	if (pTreePage != NULL)
		return pTreePage->QueryInterface(IID_ICxSymbolTreePage, (void**)ppPage);

	return S_OK;
}

LRESULT CScriptEditor::OnSymbolPageChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	Fire_SymbolPageChanged();
	return 1;
}

LRESULT CScriptEditor::OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
//	m_viewScript.SetLineNumberWidth();
	m_bChanged = TRUE;
	return 0;
}

LRESULT CScriptEditor::OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ((HWND)wParam == m_viewScript.m_hWnd)
		bHandled = FALSE;

	return 0;
}
