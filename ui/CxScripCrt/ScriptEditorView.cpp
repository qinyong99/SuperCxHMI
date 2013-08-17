// ScriptView.cpp: implementation of the CScriptEditorView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Resource.h"
#include "ScriptEditorView.h"
#include "Scripter.h"
#include "FindReplaceDlg.h"
#include "GoLineDlg.h"
#include "CxScripCrt.h"

/////////////////////////////////////////////////////////////////////////////
// _SCINTILLAEDIT_STATE

_SCINTILLAEDIT_STATE::_SCINTILLAEDIT_STATE()
{
	bNext = TRUE;
}

_SCINTILLAEDIT_STATE::~_SCINTILLAEDIT_STATE()
{
	ATLASSERT(pScripter == NULL);
	
	if (m_hInstScintillaEdit != NULL)
		::FreeLibrary(m_hInstScintillaEdit);
}

static _SCINTILLAEDIT_STATE _scintillaEditState;

_SCINTILLAEDIT_STATE* GetScintillaEditState()
{
	return &_scintillaEditState;
}

BOOL PASCAL InitScintillaEdit()
{
	_SCINTILLAEDIT_STATE* pState = GetScintillaEditState();
	if (pState->m_hInstScintillaEdit == NULL)
		pState->m_hInstScintillaEdit = LoadLibraryA(CScriptEditorView::GetLibraryName());
	return pState->m_hInstScintillaEdit != NULL;
}

//////////////////////////////////////////////////////////////////////
// CScriptEditorView

CScriptEditorView* CScriptEditorView::m_pActiveEditorView = NULL;

CScriptEditorView::CScriptEditorView()
{
	m_pScriptEditorSite = NULL;
	InitScintillaEdit();
}

CScriptEditorView::~CScriptEditorView()
{
}

const char vbsKeyWords[] = 
	"And As ByRef ByVal Case Call Const "
	"Continue Dim Do Each Else ElseIf End Error Exit False For Function Global "
	"Goto If In Loop Me New Next Not Nothing On Optional Or Private Public "
	"ReDim Rem Resume Select Set Sub Then To True Type While Wend With "
	"Boolean Byte Currency Date Double Integer Long Object Single String Type "
	"Variant";

void CScriptEditorView::InitScintillaEditCtrl() 
{
	SetCodePage(SC_CP_DBCS);
	
	ClearDocumentStyle();
	SetStyleBits(7);
	SetIndentationGuides(TRUE);
	SetTabWidth(4);
	SetIndent(4);
	SetEolMode(0);
	SetModEventMask(SC_MOD_INSERTTEXT|SC_MOD_DELETETEXT);
	
	// 字体
	LPTSTR pszFace = _T("System");
	WORD wDefSize = 10;
	HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	if (hFont == NULL)
		hFont = (HFONT)::GetStockObject(SYSTEM_FONT);
	if (hFont != NULL)
	{
		LOGFONT lf;
		if (::GetObject(hFont, sizeof(LOGFONT), &lf) != 0)
		{
			pszFace = lf.lfFaceName;
			HDC hDC = ::GetDC(NULL);
			if (lf.lfHeight < 0)
				lf.lfHeight = -lf.lfHeight;
			wDefSize = (WORD)MulDiv(lf.lfHeight, 72, GetDeviceCaps(hDC, LOGPIXELSY));
			::ReleaseDC(NULL, hDC);
		}
	}

	StyleSetFont(STYLE_DEFAULT, pszFace);
	StyleSetSize(STYLE_DEFAULT, wDefSize);

	//
	StyleSetBack(STYLE_DEFAULT, ::GetSysColor(COLOR_WINDOW));
	StyleClearAll();

//	StyleSetBack(STYLE_LINENUMBER, RGB(224, 224, 224));

//	StyleSetEolFilled(SCE_B_STRING, TRUE);
	
	StyleSetFore(SCE_B_DEFAULT, RGB(0,0,0));
	StyleSetFore(SCE_B_COMMENT, RGB(0,128,0));
	StyleSetFore(SCE_B_NUMBER, RGB(255,0,255));
	StyleSetFore(SCE_B_KEYWORD, RGB(0,0,255));
	//	StyleSetFore(SCI_STYLESETBOLD, SCE_HBA_WORD, 1);
	StyleSetFore(SCE_B_STRING, RGB(255,0,255));
	StyleSetFore(SCE_B_IDENTIFIER, RGB(0,0,0));
	
	// 选中的字体及背景颜色
	SetSelFore(TRUE, ::GetSysColor(COLOR_HIGHLIGHTTEXT));
	SetSelBack(TRUE, ::GetSysColor(COLOR_HIGHLIGHT));
	
	SetProperty(_T("fold"), _T("1"));
	//	SetProperty(_T("fold.html"), _T("1"));
	//	SetProperty(_T("fold.html.preprocessor"), _T("1"));
	SetProperty(_T("fold.comment"), _T("1"));
	SetProperty(_T("fold.at.else"), _T("1"));
	SetProperty(_T("fold.flags"), _T("1"));
	SetProperty(_T("fold.preprocessor"), _T("1"));
	SetProperty(_T("styling.within.preprocessor"), _T("1"));
	//	SetProperty(_T("asp.default.language"), _T("1"));
	
	SetFoldFlags(16);
	SetMarginMaskN(2, SC_MASK_FOLDERS);
	SetModEventMask(SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT);
	SetMarginWidthN(2, 14);
	SetMarginSensitiveN(2, TRUE);
	
//	SetLineNumberWidth();

	// 书签
	MarkerSetFore(MARKER_BOOKMARK, ::GetSysColor(COLOR_HIGHLIGHT));
	MarkerSetBack(MARKER_BOOKMARK, RGB(0, 255, 255));
	MarkerDefine(MARKER_BOOKMARK, SC_MARK_ROUNDRECT);
//	SetMarginWidthN(MARKER_BOOKMARK, 20);
	
	// define a set of markers to displa folding symbols
//	MarkerDefine(SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS);
//	MarkerDefine(SC_MARKNUM_FOLDER, SC_MARK_PLUS);
//	MarkerDefine(SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY);
//	MarkerDefine(SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY);
//	MarkerDefine(SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY);
//	MarkerDefine(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY);
//	MarkerDefine(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY);

//	SetFoldMarginColour(TRUE, ::GetSysColor(COLOR_WINDOW));

	MarkerDefine(SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS);
	MarkerSetFore(SC_MARKNUM_FOLDEROPEN, RGB(255, 255, 255));
	MarkerSetBack(SC_MARKNUM_FOLDEROPEN, RGB(128, 128, 128));

	MarkerDefine(SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS);
	MarkerSetFore(SC_MARKNUM_FOLDER, RGB(255, 255, 255));
	MarkerSetBack(SC_MARKNUM_FOLDER, RGB(128, 128, 128));

	MarkerDefine(SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
	MarkerSetFore(SC_MARKNUM_FOLDERSUB, RGB(255, 255, 255));
	MarkerSetBack(SC_MARKNUM_FOLDERSUB, RGB(128, 128, 128));

	MarkerDefine(SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER);
	MarkerSetFore(SC_MARKNUM_FOLDERTAIL, RGB(255, 255, 255));
	MarkerSetBack(SC_MARKNUM_FOLDERTAIL, RGB(128, 128, 128));

	MarkerDefine(SC_MARKNUM_FOLDEREND, SC_MARK_BOXPLUSCONNECTED);
	MarkerSetFore(SC_MARKNUM_FOLDEREND, RGB(255, 255, 255));
	MarkerSetBack(SC_MARKNUM_FOLDEREND, RGB(128, 128, 128));

	MarkerDefine(SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED);
	MarkerSetFore(SC_MARKNUM_FOLDEROPENMID, RGB(255, 255, 255));
	MarkerSetBack(SC_MARKNUM_FOLDEROPENMID, RGB(128, 128, 128));

	MarkerDefine(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER);
	MarkerSetFore(SC_MARKNUM_FOLDERMIDTAIL, RGB(255, 255, 255));
	MarkerSetBack(SC_MARKNUM_FOLDERMIDTAIL, RGB(128, 128, 128));
	

	SetLexer(SCLEX_VBSCRIPT);
	SetKeyWords(0, vbsKeyWords);

	// 自动完成列表中的图标
	CString strPixmap;

	HINSTANCE hInst = _Module.GetResourceInstance();
	HICON hIcon = (HICON)::LoadImage(hInst,	MAKEINTRESOURCE(IDI_METHOD), IMAGE_ICON, 16, 16, 0);
	ConvertToPixmap(strPixmap, hIcon);
	RegisterImage(0, strPixmap);
	if (hIcon)
		DestroyIcon(hIcon);

	hIcon = (HICON)::LoadImage(hInst, MAKEINTRESOURCE(IDI_PROP), IMAGE_ICON, 16, 16, 0);
	ConvertToPixmap(strPixmap, hIcon);
	RegisterImage(1, strPixmap);
	if (hIcon)
		DestroyIcon(hIcon);

//	AutoCSetTypeSeparator('?');

	AutoCSetFillups(_T("() "));
	AutoCSetIgnoreCase(TRUE);

	CallTipSetFore(GetSysColor(COLOR_INFOTEXT));
	CallTipSetBack(GetSysColor(COLOR_INFOBK));
	CallTipSetForeHlt(GetSysColor(COLOR_INFOTEXT));

	UsePopup(FALSE);
}

LRESULT CScriptEditorView::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);

	InitScintillaEditCtrl();

	_SCINTILLAEDIT_STATE* pEditState = GetScintillaEditState();
	if (pEditState->pScripter == NULL)
	{
		pEditState->pScripter = new CComObject<CScripter>;
		pEditState->pScripter->Initialize();
	}
	pEditState->m_dwScripterRef++;

	return lRet;
}

LRESULT CScriptEditorView::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	_SCINTILLAEDIT_STATE* pEditState = GetScintillaEditState();
	pEditState->m_dwScripterRef--;
	if (pEditState->m_dwScripterRef == 0)
	{
		if (pEditState->pFindReplaceDlg != NULL)
		{
			pEditState->pFindReplaceDlg->DestroyWindow();
			delete pEditState->pFindReplaceDlg;
			pEditState->pFindReplaceDlg = NULL;
		}
		pEditState->pScripter->UnloadScript();
		pEditState->pScripter->Release();
		pEditState->pScripter = NULL;
	}

	return 0;
}

void CScriptEditorView::EditFindReplace(BOOL bFind)
{
	_SCINTILLAEDIT_STATE* pEditState = GetScintillaEditState();
	
	if (pEditState->pFindReplaceDlg != NULL)
	{
		pEditState->pFindReplaceDlg->DestroyWindow();
		delete pEditState->pFindReplaceDlg;
		pEditState->pFindReplaceDlg = NULL;
	}
	
	m_pActiveEditorView = this;
	pEditState->pFindReplaceDlg = CFindReplaceDlg::CreateEditorDialog(bFind);
}

void CScriptEditorView::FindRepeat()
{
	CStringArray arrStrings;
	GetSearchStrings(arrStrings);
	ATLASSERT(arrStrings.GetCount() > 0);
	CString strFind = arrStrings[0];
	
	BOOL bCase;
	BOOL bWord;
	BOOL bRegExp;
	BOOL bNext;
	GetSearchSetting(bCase, bWord, bRegExp, bNext);
	
	int nSearchFlags = 0;
	if (bWord)
		nSearchFlags |= SCFIND_WHOLEWORD;
	if (bCase)
		nSearchFlags |= SCFIND_MATCHCASE;
	if (bRegExp)
		nSearchFlags |= SCFIND_REGEXP;
	
	if (FindNext(nSearchFlags, strFind, !bNext) == -1)
		MessageBeep(MB_ICONHAND);
}

void CScriptEditorView::GoLine()
{
	CGoLineDlg dlg;
	dlg.m_nLine = GetCurrentLineNumber();
	dlg.m_nLineCount = GetLineCount();
	if (dlg.DoModal() == IDOK)
		GotoLine(dlg.m_nLine);
}

void CScriptEditorView::BookmarkToggle()
{
	int nLine = GetCurrentLineNumber();
	
	DWORD dwMask = MarkerGet(nLine);
	if ((dwMask & (1 << MARKER_BOOKMARK)) != 0)
		MarkerDelete(nLine, MARKER_BOOKMARK);
	else
		MarkerAdd(nLine, MARKER_BOOKMARK);
}

void CScriptEditorView::BookmarkNext()
{
	int nLine = GetCurrentLineNumber();
	int nLineStart = nLine + 1;
	int nNextLine = MarkerNext(nLineStart, 1 << MARKER_BOOKMARK);
	if (nNextLine < 0)
		nNextLine = MarkerNext(0, 1 << MARKER_BOOKMARK);
	if (nNextLine < 0 || nNextLine == nLine)
	{
		MessageBeep(MB_ICONHAND);
	}
	else
	{
		EnsureVisibleEnforcePolicy(nNextLine);
		GotoLine(nNextLine);
	}
}

void CScriptEditorView::BookmarkPrevious()
{
	int nLine = GetCurrentLineNumber();
	int nLineStart = nLine - 1;
	int nNextLine = MarkerPrevious(nLineStart, 1 << MARKER_BOOKMARK);
	if (nNextLine < 0)
		nNextLine = MarkerPrevious(GetLineCount(), 1 << MARKER_BOOKMARK);
	if (nNextLine < 0 || nNextLine == nLine)
	{
		MessageBeep(MB_ICONHAND);
	}
	else
	{
		EnsureVisibleEnforcePolicy(nNextLine);
		GotoLine(nNextLine);
	}
}

void CScriptEditorView::BookmarkClearAll()
{
	MarkerDeleteAll(MARKER_BOOKMARK);
}

CString CScriptEditorView::GetSelText()
{
	int nLen = (GetSelectionEnd() - GetSelectionStart()) + 1;
	if (nLen > 0)
	{
		TCHAR *p = new TCHAR[nLen + 1];
		if (p != NULL)
		{
			*p = '\0';
			CScintillaEditCtrl::GetSelText(p);
			CString strReturn = p;
			delete [] p;
			return strReturn;
		}
	}
	return _T("");
}

LRESULT CScriptEditorView::OnCharAdded(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	SCNotification *pSCN = (SCNotification *)pnmh;
	CharAdded(static_cast<char>(pSCN->ch));

	return 0;
}

LRESULT CScriptEditorView::OnMarginClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	SCNotification *pSCN = (SCNotification *)pnmh;
	DoFolding(pSCN->margin, pSCN->position);

	return 0;
}

void CScriptEditorView::DoFolding(int nMargin, int nPosition)   
{
	// simply toggle fold
	if (nMargin == 2)
	{
		int nLine = LineFromPosition(nPosition);
		ToggleFold(nLine);
	}
}

int CScriptEditorView::GetLineNumberChars()
{
	// try to get number of lines in control
	int nLines = GetLineCount();
	
	int nChars = 1;
	while (nLines > 0)
	{
		nLines = nLines / 10;
		nChars++;
	}
	return nChars; // allow an extra char for safety
}

void CScriptEditorView::SetLineNumberWidth()
{
	int nChars = GetLineNumberChars();
	if (m_nLineNumerChars != nChars)
	{
		m_nLineNumerChars = nChars;

		int nWidth = TextWidth(STYLE_LINENUMBER, _T("9"));
	//	SetMarginWidthN(0, m_nLineNumerChars * nWidth);
	}
}

// 插入事件处理函数，如果已经存在，则光标卷动到函数位置
void CScriptEditorView::InsertObjectEventHandler(LPCTSTR lpszObject, LPCTSTR lpszEvent)
{
	int nSubBegin;
	int nSubBodyBegin;
	int nSubBodyEnd;
	int nSubEnd;

	CString strScript;
	GetText(strScript);

	if (!GetObjectEventHandler(lpszObject, lpszEvent, 
		nSubBegin, nSubBodyBegin, nSubBodyEnd, nSubEnd))
	{
		CString strEventSubName = lpszObject;
		strEventSubName += _T('_');
		strEventSubName += lpszEvent;

		int nStart = -1;
		int nEnd = -1;
		GetInsertPosition(strEventSubName, nStart, nEnd);
		ATLASSERT(nStart != -1 && nEnd != -1);

		CStringArray arrParams;
		GetObjectEventParams(lpszObject, lpszEvent, arrParams);

		CString strReplaceText;
		if (nStart > 0)
			strReplaceText += _T("\r\n");
		strReplaceText += _T("Sub ");
		strReplaceText += lpszObject;
		strReplaceText += _T("_");
		strReplaceText += lpszEvent;
		strReplaceText += _T("(");
		for (int i = 0; i < (int)arrParams.GetCount(); i++)
		{
			strReplaceText += arrParams[i];
			if (i < (int)arrParams.GetCount() - 1)
				strReplaceText += ", ";
		}
		strReplaceText += _T(")");
		strReplaceText += _T("\r\n\r\nEnd Sub\r\n\r\n");

		SetSelectionStart(nStart);
		SetSelectionEnd(nEnd);
		ReplaceSel(strReplaceText);

		int nLine = LineFromPosition(nStart);
		if (nStart > 0)
			nLine += 1;
		GotoLine(nLine + 1);
	}
	else
	{		
		int nLine = LineFromPosition(nSubBodyBegin);
		GotoLine(nLine);
	}
}

BOOL CScriptEditorView::IsEventHandler(LPCTSTR lpszSubName)
{
	CString strSubName = lpszSubName;
	int nUnderLine = strSubName.ReverseFind(_T('_'));
	if (nUnderLine != -1)
	{
		CString strObject = strSubName.Left(nUnderLine);
		CString strEvent = strSubName.Right(strSubName.GetLength() - nUnderLine - 1);
		
		CStringArray arrEvents;
		GetObjectEvents(strObject, arrEvents);
		for (UINT i = 0; i < arrEvents.GetCount(); i++)
		{
			CString strEventT = arrEvents[i];
			int iIndex = strEventT.Find(_T(" "));
			if (iIndex != -1)
				strEventT = strEventT.Left(iIndex);

			if (strEventT.CompareNoCase(strEvent) == 0)
				return TRUE;
		}
	}
 
	return FALSE;
}

void CScriptEditorView::DeleteEmptyEventHandler()
{
	int nPos = GetCurrentPos();

	for (int i = m_arrBlocks.GetSize() - 1; i >= 0; i--)
	{
		CScriptBlock& sb = m_arrBlocks[i];
		if (sb.m_nType != SBT_Sub || !IsEventHandler(sb.m_strName))
			continue;

		// 事件过程不空继续
		CString strSubBody(m_pText + sb.m_nBodyBegin, sb.m_nBodyEnd - sb.m_nBodyBegin);
		strSubBody.TrimLeft();
		strSubBody.TrimRight();
		if (!strSubBody.IsEmpty())
			continue;

		// 注释不为空继续
		CString strComment(m_pText + sb.m_nCommentBegin, sb.m_nBegin - sb.m_nCommentBegin);
		strSubBody.TrimLeft();
		strSubBody.TrimRight();
		if (!strSubBody.IsEmpty())
			continue;

		SetSelectionStart(sb.m_nCommentBegin);
		SetSelectionEnd(sb.m_nEnd);
		ReplaceSel(_T(""));

		if (nPos > sb.m_nCommentBegin)
		{
			if (nPos > sb.m_nEnd)
				nPos -= (sb.m_nEnd - sb.m_nCommentBegin);
			else
				nPos = sb.m_nCommentBegin;
		}
	}

	CString strScript;
	GetText(strScript);

	// 截掉前面的空白字符
	int nLen = strScript.GetLength();
	strScript.TrimLeft();
	nLen = nLen - strScript.GetLength();
	if (nLen != 0)
	{
		SetSelectionStart(0);
		SetSelectionEnd(nLen);
		ReplaceSel(_T(""));
		if (nPos > nLen)
			nPos -= nLen;
		else
			nPos = 0;
	}

	SetSel(nPos, nPos);
}

void CScriptEditorView::GetObjectNames(CStringArray& arrObjects)
{
	USES_CONVERSION;

	if (m_pScriptEditorSite != NULL)
	{
		CComPtr<IEnumString> spEnumString;
		
		HRESULT hr = m_pScriptEditorSite->GetObjectNames(&spEnumString);
		if (hr == S_OK)
		{
			LPOLESTR pszItem;
			while (spEnumString->Next(1, &pszItem, NULL) == S_OK)
			{
				arrObjects.Add(OLE2T(pszItem));
				CoTaskMemFree(pszItem);
			}
		}
	}
}

void CScriptEditorView::GetObjectEventOrParams(ITypeInfo* pTypeInfo, CStringArray& arrInfos, LPCTSTR lpszEvent)
{
	USES_CONVERSION;

	TYPEATTR* pTypeAttr = NULL;
	pTypeInfo->GetTypeAttr(&pTypeAttr);

	if (lpszEvent != NULL)
	{
		BSTR bstrNames = T2OLE(lpszEvent); 
		MEMBERID memberid;
		pTypeInfo->GetIDsOfNames(&bstrNames, 1, &memberid);

		BSTR bstrParams[32];
		UINT numParams = 0;

		pTypeInfo->GetNames(memberid, bstrParams, 32, &numParams);
		for (UINT nParam = 1; nParam < numParams; nParam++)
		{
			arrInfos.Add(bstrParams[nParam]);
			SysFreeString(bstrParams[nParam]);
		}
	}
	else
	{
		//遍历Dispatch接口中的所有函数
		for (int nFunction = 0; nFunction < pTypeAttr->cFuncs; nFunction++)
		{
			FUNCDESC* pfd;
			pTypeInfo->GetFuncDesc(nFunction, &pfd);
			
			CComBSTR bstrName;
			CComBSTR bstrDocString;
			pTypeInfo->GetDocumentation(pfd->memid, &bstrName, &bstrDocString, NULL, NULL);
			CString strInfo;
			strInfo = bstrName;
			strInfo += _T(" ");
			strInfo += W2T(bstrDocString);
			arrInfos.Add(strInfo);
			
			pTypeInfo->ReleaseFuncDesc(pfd);
		}
	}

	pTypeInfo->ReleaseTypeAttr(pTypeAttr);
	pTypeAttr = NULL;
}

void CScriptEditorView::GetObjectEventOrParams(LPUNKNOWN pUnkObject, CStringArray& arrInfos, LPCTSTR lpszEvent)
{
	USES_CONVERSION;

	CComPtr<IProvideClassInfo> spProvideClassInfo;
	CComPtr<ITypeInfo> spClassInfo;
	CComPtr<ITypeInfo> spTypeInfo;
	TYPEATTR* pClassTypeAttr = NULL;
	TYPEATTR* pTypeAttr = NULL;

	// 获取类信息
	HRESULT hr = pUnkObject->QueryInterface(&spProvideClassInfo);
	if (FAILED(hr))
		goto ONERROR;
	
	hr = spProvideClassInfo->GetClassInfo(&spClassInfo);
	if (FAILED(hr))
		goto ONERROR;
	
	spClassInfo->GetTypeAttr(&pClassTypeAttr);
	ATLASSERT(pClassTypeAttr->typekind == TKIND_COCLASS);

	//遍历类所实现的接口
	int i;
	for (i = 0; i < pClassTypeAttr->cImplTypes; i++)
	{
		int nImplTypeFlags; 
		spClassInfo->GetImplTypeFlags(i, &nImplTypeFlags);
		if ((nImplTypeFlags & IMPLTYPEFLAG_FSOURCE) == 0)	// 不是事件接口
			continue;
		
		// 获取应用类型信息句柄
		HREFTYPE hRef;
		hr = spClassInfo->GetRefTypeOfImplType(i, &hRef);
		if (FAILED(hr))
			goto ONERROR;

		// 获取类型信息
		hr = spClassInfo->GetRefTypeInfo(hRef, &spTypeInfo);
		if (FAILED(hr))
			goto ONERROR;	

		spTypeInfo->GetTypeAttr(&pTypeAttr);

		// 查找Dispatch接口
		if (pTypeAttr->typekind == TKIND_INTERFACE)
		{
			//Get Dispatch Interface's Info;
			hr = spTypeInfo->GetRefTypeOfImplType(-1, &hRef);
			if (FAILED(hr))
				goto ONERROR;	

			CComPtr<ITypeInfo> spTypeInfoTmp;
			hr = spTypeInfo->GetRefTypeInfo(hRef, &spTypeInfoTmp);
			if (FAILED(hr))
				goto ONERROR;

			spTypeInfo->ReleaseTypeAttr(pTypeAttr);
			spTypeInfo = spTypeInfoTmp;
			spTypeInfo->GetTypeAttr(&pTypeAttr);
		}

		ATLASSERT(pTypeAttr->typekind == TKIND_DISPATCH);

		GetObjectEventOrParams(spTypeInfo, arrInfos, lpszEvent);
	
		spTypeInfo->ReleaseTypeAttr(pTypeAttr);
		pTypeAttr = NULL;
		spTypeInfo.Release();
	}

ONERROR:
	if (spClassInfo != NULL && pClassTypeAttr != NULL)
		spClassInfo->ReleaseTypeAttr(pClassTypeAttr);
	if (spTypeInfo != NULL && pClassTypeAttr != NULL)
		spTypeInfo->ReleaseTypeAttr(pTypeAttr);
}

void CScriptEditorView::GetObjectEvents(LPCTSTR lpszObject, CStringArray& arrEvents)
{
	USES_CONVERSION;

	if (m_pScriptEditorSite != NULL)
	{
		CComPtr<IEnumString> spEnumString;
		
		HRESULT hr = m_pScriptEditorSite->GetObjectEvents(T2CW(lpszObject), &spEnumString);
		if (hr == S_OK)
		{
			LPOLESTR pszItem;
			while (spEnumString->Next(1, &pszItem, NULL) == S_OK)
			{
				arrEvents.Add(OLE2T(pszItem));
				CoTaskMemFree(pszItem);
			}
			return;
		}

		CComPtr<ITypeInfo> spTypeInfo;
		hr = m_pScriptEditorSite->GetObjectSourceTypeInfo(T2CW(lpszObject), &spTypeInfo);
		if (SUCCEEDED(hr))
		{
			GetObjectEventOrParams(spTypeInfo, arrEvents);
			return;
		}
			
		CComPtr<IUnknown> spUnkObject;
		hr = m_pScriptEditorSite->GetObject(T2CW(lpszObject), 
				IID_IUnknown, (void **)&spUnkObject);
		if (SUCCEEDED(hr))
		{
			GetObjectEventOrParams(spUnkObject, arrEvents);
			return;
		}
	}
}

BOOL CScriptEditorView::ShowHelpString(LPCTSTR lpszObject)
{
	USES_CONVERSION;

	if (m_pScriptEditorSite != NULL)
		return m_pScriptEditorSite->ShowHelpString(T2CW(lpszObject)) == S_OK;
	
	return FALSE;
}

void CScriptEditorView::GetObjectEventParams(LPCTSTR lpszObject, LPCTSTR lpszEvent, CStringArray& arrParams)
{
	USES_CONVERSION;
	
	if (m_pScriptEditorSite != NULL)
	{
		CComPtr<IEnumString> spEnumString;
		
		HRESULT hr = m_pScriptEditorSite->GetObjectEventParams(T2CW(lpszObject), T2CW(lpszEvent), &spEnumString);
		if (hr == S_OK)
		{
			LPOLESTR pszItem;
			while (spEnumString->Next(1, &pszItem, NULL) == S_OK)
			{
				arrParams.Add(OLE2T(pszItem));
				CoTaskMemFree(pszItem);
			}
			return;
		}

		CComPtr<ITypeInfo> spTypeInfo;
		hr = m_pScriptEditorSite->GetObjectSourceTypeInfo(T2CW(lpszObject), &spTypeInfo);
		if (SUCCEEDED(hr))
		{
			GetObjectEventOrParams(spTypeInfo, arrParams, lpszEvent);
			return;
		}
		
		CComPtr<IUnknown> spUnkObject;
		hr = m_pScriptEditorSite->GetObject(T2CW(lpszObject), 
			IID_IUnknown, (void **)&spUnkObject);
		if (SUCCEEDED(hr))
		{
			GetObjectEventOrParams(spUnkObject, arrParams, lpszEvent);
			return;
		}
	}
}

//Helper for finding the function index for a DISPID
HRESULT GetFuncIndexOfMemId(ITypeInfo* pTypeInfo, DISPID dispidMember, UINT& nIndex)
{
	HRESULT hr;
//	CComQIPtr<ITypeInfo2, &IID_ITypeInfo2> spTypeInfo2 = pTypeInfo;
	if (0/*spTypeInfo2 != NULL*/)
	{
//		hr = spTypeInfo2->GetFuncIndexOfMemId(dispidMember, INVOKE_FUNC, &nIndex);
//		if (FAILED(hr))
//			return hr;

	}
	else // search for funcdesc
	{
		TYPEATTR* pAttr;
		hr = pTypeInfo->GetTypeAttr(&pAttr);
		if (FAILED(hr))
			return hr;
		FUNCDESC* pFuncDesc = NULL;
		for (int i = 0; i < pAttr->cFuncs; i++)
		{
			hr = pTypeInfo->GetFuncDesc(i, &pFuncDesc);
			if (FAILED(hr))
				return hr;
			if (pFuncDesc->memid == dispidMember)
				break;
			pTypeInfo->ReleaseFuncDesc(pFuncDesc);
			pFuncDesc = NULL;
		}
		pTypeInfo->ReleaseTypeAttr(pAttr);
		if (pFuncDesc == NULL)
		{
			return E_FAIL;
		}
		else
		{
			nIndex = i;
			pTypeInfo->ReleaseFuncDesc(pFuncDesc);
		}
	}

	return S_OK;
}

BOOL CScriptEditorView::GetObjectMembers(ITypeInfo* pTypeInfo, CStringArray& arrWords, 
		CStringArray& arrProperties, CStringArray& arrMethods)
{
	USES_CONVERSION;

	BOOL bResult = FALSE;
	int nWords = (int)arrWords.GetCount();

	if (nWords == 0)
	{
		TYPEATTR* pTypeAttr = NULL;
		HRESULT hr = pTypeInfo->GetTypeAttr(&pTypeAttr);
		if (FAILED(hr))
			return FALSE;

		for (int i = 0; i < pTypeAttr->cFuncs; i++)
		{
			FUNCDESC* pfd = 0;
			pTypeInfo->GetFuncDesc(i, &pfd);

			if ((pfd->wFuncFlags & (FUNCFLAG_FRESTRICTED | 
				FUNCFLAG_FHIDDEN | FUNCFLAG_FNONBROWSABLE)) == 0)
			{
				CComBSTR bstrName;
				pTypeInfo->GetDocumentation(pfd->memid, &bstrName, NULL, NULL, NULL);
				CString str = OLE2T(bstrName);
				if (pfd->invkind == INVOKE_FUNC)
				{
					arrMethods.Add(OLE2T(bstrName));
					bResult = TRUE;
				}
				else if (pfd->invkind == INVOKE_PROPERTYGET)
				{
					arrProperties.Add(str);
					bResult = TRUE;
				}
			}

			pTypeInfo->ReleaseFuncDesc(pfd);
		}
	

		for (i = 0; i < pTypeAttr->cVars; i++)
		{
			VARDESC* pvd;
			pTypeInfo->GetVarDesc(i, &pvd);
			if (pvd->varkind == VAR_DISPATCH && (pvd->wVarFlags & (VARFLAG_FHIDDEN | 
				VARFLAG_FRESTRICTED | VARFLAG_FNONBROWSABLE)) == 0   )
			{
				CComBSTR bstrName;
				pTypeInfo->GetDocumentation(pvd->memid, &bstrName, NULL, NULL, NULL);
				CString str = OLE2T(bstrName);
				arrProperties.Add(str);
				bResult = TRUE;
			}
			pTypeInfo->ReleaseVarDesc(pvd);
		}

		pTypeInfo->ReleaseTypeAttr(pTypeAttr);
	}
	else
	{
		BSTR bstrNames = T2OLE(arrWords[0]); 
		DISPID dispidMember;
		if (FAILED(pTypeInfo->GetIDsOfNames(&bstrNames, 1, &dispidMember)))
			return FALSE;

		arrWords.RemoveAt(0);

		UINT nIndex;
		HRESULT hr = GetFuncIndexOfMemId(pTypeInfo, dispidMember, nIndex);
		if (FAILED(hr))
			return FALSE;

		FUNCDESC* pfd = NULL;
		hr = pTypeInfo->GetFuncDesc(nIndex, &pfd);
		if (FAILED(hr))
			return FALSE;

//		CComBSTR bstrName;
//		pTypeInfo->GetDocumentation(pfd->memid, &bstrName, NULL, NULL, NULL);

		TYPEDESC* ptdesc = NULL;
		if (pfd->invkind != INVOKE_FUNC && pfd->cParams > 0)
			ptdesc = &pfd->lprgelemdescParam[0].tdesc;
		else 
			ptdesc = &pfd->elemdescFunc.tdesc;
		if (ptdesc->vt == VT_PTR)
		{
			ptdesc = ptdesc->lptdesc;
		}
		if (ptdesc->vt == VT_USERDEFINED)
		{
			CComPtr<ITypeInfo> spUserDefinedTypeInfo;
			hr = pTypeInfo->GetRefTypeInfo(ptdesc->hreftype, &spUserDefinedTypeInfo);
			if (SUCCEEDED(hr))
			{
				TYPEATTR *pta = NULL;
				spUserDefinedTypeInfo->GetTypeAttr(&pta);
				if (pta && pta->typekind == TKIND_ALIAS)
				{
					if (pta->tdescAlias.vt == VT_USERDEFINED)
					{
						CComPtr<ITypeInfo> spAliasTypeInfo;
						hr = spUserDefinedTypeInfo->GetRefTypeInfo(pta->tdescAlias.hreftype, &spAliasTypeInfo);
						if (SUCCEEDED(hr))
							bResult = GetObjectMembers(spAliasTypeInfo, arrWords, arrProperties, arrMethods);
					}
				}
				else
				{
					
					bResult = GetObjectMembers(spUserDefinedTypeInfo, arrWords, arrProperties, arrMethods);
				}
				
				if (pta)
					spUserDefinedTypeInfo->ReleaseTypeAttr(pta);
			}
		}
	
		if (pfd)
			pTypeInfo->ReleaseFuncDesc(pfd);
	}

	return bResult;
}

// 
BOOL CScriptEditorView::GetObjectMembers(CStringArray& arrWords, CStringArray& arrProperties,CStringArray& arrMethods)
{
	USES_CONVERSION;

	CString strWords;
	for (UINT i = 0; i < arrWords.GetCount(); i++)
	{
		if (i != 0)
			strWords += _T('.');
		strWords += arrWords[i];
	}

	CComPtr<IEnumString> spEnumProperties;
	CComPtr<IEnumString> spEnumMethods;
	
	HRESULT hr = m_pScriptEditorSite->GetObjectMembers(T2CW(strWords), &spEnumProperties, &spEnumMethods);
	if (hr == S_OK)
	{
		LPOLESTR pszItem;
		while (spEnumProperties->Next(1, &pszItem, NULL) == S_OK)
		{
			arrProperties.Add(OLE2T(pszItem));
			CoTaskMemFree(pszItem);
		}
		while (spEnumMethods->Next(1, &pszItem, NULL) == S_OK)
		{
			arrMethods.Add(OLE2T(pszItem));
			CoTaskMemFree(pszItem);
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CScriptEditorView::GetObjectMemberParams(ITypeInfo* pTypeInfo, CStringArray& arrWords, CStringArray& arrParams)
{
	USES_CONVERSION;
	
	BOOL bResult = FALSE;
	int nWords = (int)arrWords.GetCount();
	
	if (nWords > 0)
	{
		BSTR bstrNames = T2OLE(arrWords[0]); 
		DISPID dispidMember;
		if (FAILED(pTypeInfo->GetIDsOfNames(&bstrNames, 1, &dispidMember)))
			return FALSE;

		UINT nIndex;
		HRESULT hr = GetFuncIndexOfMemId(pTypeInfo, dispidMember, nIndex);
		if (FAILED(hr))
			return FALSE;
		
		FUNCDESC* pfd = NULL;
		hr = pTypeInfo->GetFuncDesc(nIndex, &pfd);
		if (FAILED(hr))
			return FALSE;
		
		if (nWords == 1)
		{
			if (pfd->invkind == INVOKE_FUNC)
			{
				BSTR bstrParams[32];
				UINT numParams = 0;
				
				pTypeInfo->GetNames(dispidMember, bstrParams, 32, &numParams);
				for (UINT nParam = 1; nParam < numParams; nParam++)
				{
					CString str = bstrParams[nParam];
					if ((int)nParam > pfd->cParams - pfd->cParamsOpt)
						str = "[" + str + "]";
					arrParams.Add(str);
					SysFreeString(bstrParams[nParam]);

//					arrParams.Add(bstrParams[nParam]);
//					SysFreeString(bstrParams[nParam]);
				}

				bResult = TRUE;
			}
		}
		else
		{
			arrWords.RemoveAt(0);

			TYPEDESC* ptdesc = &pfd->elemdescFunc.tdesc;
			if (ptdesc->vt == VT_PTR)
			{
				ptdesc = ptdesc->lptdesc;
			}
			if (ptdesc->vt == VT_USERDEFINED)
			{
				CComPtr<ITypeInfo> spUserDefinedTypeInfo;
				hr = pTypeInfo->GetRefTypeInfo(ptdesc->hreftype, &spUserDefinedTypeInfo);
				if (SUCCEEDED(hr))
				{
					TYPEATTR *pta = NULL;
					spUserDefinedTypeInfo->GetTypeAttr(&pta);
					if (pta && pta->typekind == TKIND_ALIAS)
					{
						if (pta->tdescAlias.vt == VT_USERDEFINED)
						{
							CComPtr<ITypeInfo> spAliasTypeInfo;
							hr = spUserDefinedTypeInfo->GetRefTypeInfo(pta->tdescAlias.hreftype, &spAliasTypeInfo);
							if (SUCCEEDED(hr))
								bResult = GetObjectMemberParams(spAliasTypeInfo, arrWords, arrParams);
						}
					}
					else
					{
						
						bResult = GetObjectMemberParams(spUserDefinedTypeInfo, arrWords, arrParams);
					}
					
					if (pta)
						spUserDefinedTypeInfo->ReleaseTypeAttr(pta);
				}	
			}
		}

		if (pfd)
			pTypeInfo->ReleaseFuncDesc(pfd);
	}

	return bResult;
}

BOOL CScriptEditorView::GetObjectMemberParams(CStringArray& arrWords, CStringArray& arrParams)
{
	USES_CONVERSION;

	CString strWords;
	for (UINT i = 0; i < arrWords.GetCount(); i++)
	{
		if (i != 0)
			strWords += _T('.');
		strWords += arrWords[i];
	}
	
	CComPtr<IEnumString> spEnumString;
	
	HRESULT hr = m_pScriptEditorSite->GetObjectMemberParams(T2CW(strWords), &spEnumString);
	if (hr == S_OK)
	{
		LPOLESTR pszItem;
		while (spEnumString->Next(1, &pszItem, NULL) == S_OK)
		{
			arrParams.Add(OLE2T(pszItem));
			CoTaskMemFree(pszItem);
		}
		
		return TRUE;
	}
	
	if (arrWords.GetCount() == 1)
	{
		UINT nInfoCount;
		hr = m_pScriptEditorSite->GetGlobalTypeInfoCount(&nInfoCount);
		if (hr == S_OK)
		{
			for (i = 0; i < nInfoCount; i++)
			{
				CComPtr<ITypeInfo> spTypeInfo;
				hr = m_pScriptEditorSite->GetGlobalTypeInfo(i, &spTypeInfo);
				if (hr == S_OK)
				{
					if (GetObjectMemberParams(spTypeInfo, arrWords, arrParams))
						return TRUE;
				}
			}
		}

		_SCINTILLAEDIT_STATE* pEditState = GetScintillaEditState();
		if (pEditState->pScripter != NULL)
			return pEditState->pScripter->GetLangFuncParams(arrWords[0], arrParams);
	}

	return FALSE;
}

void CScriptEditorView::GetSearchStrings(CStringArray& arrStrings)
{
	_SCINTILLAEDIT_STATE* pEditState = GetScintillaEditState();
	arrStrings.Append(pEditState->arrSearchStrings);
}

void CScriptEditorView::StoreSearchString(LPCTSTR lpszString)
{
	_SCINTILLAEDIT_STATE* pEditState = GetScintillaEditState();

	for (int i = pEditState->arrSearchStrings.GetCount() - 1; i >= 0; i--)
	{
		if (pEditState->arrSearchStrings[i] == lpszString)
			pEditState->arrSearchStrings.RemoveAt(i);
	}
	
	pEditState->arrSearchStrings.InsertAt(0, lpszString);
	
	if (pEditState->arrSearchStrings.GetCount() > 8)
		pEditState->arrSearchStrings.RemoveAt(pEditState->arrSearchStrings.GetCount() - 1);
}

void CScriptEditorView::GetReplaceStrings(CStringArray& arrStrings)
{
	_SCINTILLAEDIT_STATE* pEditState = GetScintillaEditState();
	arrStrings.Append(pEditState->arrReplaceStrings);
}

void CScriptEditorView::StoreReplaceString(LPCTSTR lpszString)
{
	_SCINTILLAEDIT_STATE* pEditState = GetScintillaEditState();

	for (int i = pEditState->arrReplaceStrings.GetCount() - 1; i >= 0; i--)
	{
		if (pEditState->arrReplaceStrings[i] == lpszString)
			pEditState->arrReplaceStrings.RemoveAt(i);
	}
	
	pEditState->arrReplaceStrings.InsertAt(0, lpszString);
	
	if (pEditState->arrReplaceStrings.GetCount() > 8)
		pEditState->arrReplaceStrings.RemoveAt(pEditState->arrReplaceStrings.GetCount() - 1);
}

void CScriptEditorView::StoreSearchSetting(BOOL bCase, BOOL bWord, BOOL bRegExp, BOOL bNext)
{
	_SCINTILLAEDIT_STATE* pEditState = GetScintillaEditState();
	pEditState->bCase = bCase;
	pEditState->bWord = bWord;
	pEditState->bRegExp = bRegExp;
	pEditState->bNext = bNext;
}

void CScriptEditorView::GetSearchSetting(BOOL& bCase, BOOL& bWord, BOOL& bRegExp, BOOL& bNext)
{
	_SCINTILLAEDIT_STATE* pEditState = GetScintillaEditState();
	bCase = pEditState->bCase;
	bWord = pEditState->bWord;
	bRegExp = pEditState->bRegExp;
	bNext = pEditState->bNext;
}

void CScriptEditorView::EnsureRangeVisible(int nPosStart, int nPosEnd, BOOL bEnforcePolicy)
{
	int nLineStart = LineFromPosition(min(nPosStart, nPosEnd));
	int nLineEnd = LineFromPosition(max(nPosStart, nPosEnd));
	for (int i = nLineStart; i <= nLineEnd; i++) 
	{
		if (bEnforcePolicy)
			EnsureVisibleEnforcePolicy(i);
		else
			EnsureVisible(i);
	}
}

void CScriptEditorView::AdjustDialogPosition(CWindow* pDlg)
{
	ATLASSERT(pDlg != NULL);
	int nStart = GetSelectionStart();
	CPoint point;
	point.x = PointXFromPosition(nStart);
	point.y = PointYFromPosition(nStart);
	ClientToScreen(&point);
	CRect rectDlg;
	pDlg->GetWindowRect(&rectDlg);
	if (rectDlg.PtInRect(point))
	{
		if (point.y > rectDlg.Height())
			rectDlg.OffsetRect(0, point.y - rectDlg.bottom - 20);
		else
		{
			int nVertExt = GetSystemMetrics(SM_CYSCREEN);
			if (point.y + rectDlg.Height() < nVertExt)
				rectDlg.OffsetRect(0, 40 + point.y - rectDlg.top);
		}
		pDlg->MoveWindow(&rectDlg);
	}
}

int CScriptEditorView::FindNext(int nSearchFlags, LPCTSTR lpszString, BOOL bReverseDirection)
{
	int nLength = lstrlen(lpszString);
	if (nLength < 1)
		return -1;
	
	int nStartPosition;
	int nEndPosition;
	if (!bReverseDirection)
	{
		nStartPosition = GetSelectionEnd();
		nEndPosition = GetLength();
	}
	else
	{
		nStartPosition = GetSelectionStart() - 1;
		nEndPosition = 0;
	}
	
	SetTargetStart(nStartPosition);
	SetTargetEnd(nEndPosition);
	
	SetSearchFlags(nSearchFlags);
	
	int nFindPosition = SearchInTarget(nLength, lpszString);
	// 循环查找
	if (nFindPosition == -1)
	{
		if (!bReverseDirection)
		{
			nStartPosition = 0;
			nEndPosition = GetLength();
		}
		else
		{
			nStartPosition = GetLength();
			nEndPosition = 0;
		}
		
		SetTargetStart(nStartPosition);
		SetTargetEnd(nEndPosition);
		nFindPosition = SearchInTarget(nLength, lpszString);
	}
	
	if (nFindPosition != -1)
	{
		int nStart = GetTargetStart();
		int nEnd = GetTargetEnd();
		EnsureRangeVisible(nStart, nEnd);
		SetSel(nStart, nEnd);
	}
	
	return nFindPosition;
}

int CScriptEditorView::MarkAll(int nSearchFlags, LPCTSTR lpszString)
{
	int nPosCurrent = GetCurrentPos();
	int nMarked = 0;
	int nPosFirstFound = FindNext(nSearchFlags, lpszString);
	
	if (nPosFirstFound != -1) 
	{
		int nPosFound = nPosFirstFound;
		do 
		{
			nMarked++;
			int nLine = LineFromPosition(nPosFound);
			MarkerAdd(nLine, MARKER_BOOKMARK);
			nPosFound = FindNext(nSearchFlags, lpszString);
		} while ((nPosFound != -1) && (nPosFound != nPosFirstFound));
	}
	SetCurrentPos(nPosCurrent);
	SetSel(nPosCurrent, nPosCurrent);
	
	return nMarked;
}

BOOL CScriptEditorView::ReplaceOnce(int nSearchFlags, LPCTSTR lpszFindTarget, 
		LPCTSTR lpszReplaceTarget) 
{
	CString strSelText = GetSelText();
	if (strSelText != lpszFindTarget)
	{
		if (FindNext(nSearchFlags, lpszFindTarget) == -1)
			return FALSE;

		return TRUE;
	}

	BOOL bRegExp = (SCFIND_REGEXP & nSearchFlags) != 0;

	int nReplaceLen = lstrlen(lpszReplaceTarget);
	int nStartPosition = GetSelectionStart();
	int nEndPosition = GetSelectionEnd();

	SetTargetStart(nStartPosition);
	SetTargetEnd(nEndPosition);

	int nLenReplaced = nReplaceLen;

	if (bRegExp) 
		nLenReplaced = ReplaceTargetRE(nReplaceLen, lpszReplaceTarget);
	else 
		ReplaceTarget(nReplaceLen, lpszReplaceTarget);

	SetSel(nStartPosition + nReplaceLen, nStartPosition);
	
	FindNext(nSearchFlags, lpszFindTarget);

	return TRUE;
}

// 替换所有
int CScriptEditorView::ReplaceAll(int nSearchFlags, LPCTSTR lpszFindTarget, 
		LPCTSTR lpszReplaceTarget, BOOL bInSelection) 
{
	// 如果查找字符串长度小于1返回
	int nFindLen = lstrlen(lpszFindTarget);
	if (nFindLen < 1)
		return -1;

	// 要替换的字符串长度
	int nReplaceLen = lstrlen(lpszReplaceTarget);

	BOOL bRegExp = (SCFIND_REGEXP & nSearchFlags) != 0;

	int nStartPosition = GetSelectionStart();
	int nEndPosition = GetSelectionEnd();

	if (bInSelection) 
	{
		if (nStartPosition == nEndPosition) 
			return -2;
	} 
	else 
	{
		// 整个范围
		nStartPosition = 0;
		nEndPosition = GetLength();
	}

	SetSearchFlags(nSearchFlags);

	SetTargetStart(nStartPosition);
	SetTargetEnd(nEndPosition);

	int nPosFind = SearchInTarget(nFindLen, lpszFindTarget);
	if (nFindLen == 1 && bRegExp && lpszFindTarget[0] == '^')
	{
		// Special case for replace all start of line so it hits the first line
		nPosFind = nStartPosition;
		SetTargetStart(nStartPosition);
		SetTargetEnd(nStartPosition);
	}
	
	if ((nPosFind != -1) && (nPosFind <= nEndPosition)) 
	{
		int nLastMatch = nPosFind;
		int nReplacements = 0;
		BeginUndoAction();
		// Replacement loop
		while (nPosFind != -1) 
		{
			int nLenTarget = GetTargetEnd() - GetTargetStart();
			int nMovepastEOL = 0;
			if (nLenTarget <= 0) 
			{
				char chNext = static_cast<char>(GetCharAt(GetTargetEnd()));
				if (chNext == '\r' || chNext == '\n') 
				{
					nMovepastEOL = 1;
				}
			}
			int nLenReplaced = nReplaceLen;
			if (bRegExp) 
				nLenReplaced = ReplaceTargetRE(nReplaceLen, lpszReplaceTarget);
			else 
				ReplaceTarget(nReplaceLen, lpszReplaceTarget);
	
			// Modify for change caused by replacement
			nEndPosition += nLenReplaced - nLenTarget;
			// For the special cases of start of line and end of line
			// something better could be done but there are too many special cases
			nLastMatch = nPosFind + nLenReplaced + nMovepastEOL;
			if (nLenReplaced == 0 && nLenTarget == 0) 
			{
				nLastMatch++;
			}
			if (nLastMatch >= nEndPosition) 
			{
				// Run off the end of the document/selection with an empty match
				nPosFind = -1;
			} 
			else 
			{
				SetTargetStart(nLastMatch);
				SetTargetEnd(nEndPosition);

				nPosFind = SearchInTarget(nFindLen, lpszFindTarget);
			}
			nReplacements++;
		}

		if (bInSelection) 
			SetSel(nStartPosition, nEndPosition);
		else 
			SetSel(nLastMatch, nLastMatch);
		
		EndUndoAction();

		return nReplacements;
	}

	return 0;
}

void CScriptEditorView::SetLineIndentation(int nLine, int nIndent) 
{
	if (nIndent < 0)
		return;

	int nSelStart = GetSelectionStart();
	int nSelEnd = GetSelectionEnd();
	int nPosBefore = GetLineIndentPosition(nLine);
	CScintillaEditCtrl::SetLineIndentation(nLine, nIndent);
	int nPosAfter = GetLineIndentPosition(nLine);
	int nPosDifference = nPosAfter - nPosBefore;
	if (nPosAfter > nPosBefore) 
	{
		// Move selection on
		if (nSelStart >= nPosBefore) 
		{
			nSelStart += nPosDifference;
		}
		if (nSelEnd >= nPosBefore) 
		{
			nSelEnd += nPosDifference;
		}
	} 
	else if (nPosAfter < nPosBefore)
	{
		// Move selection back
		if (nSelStart >= nPosAfter) 
		{
			if (nSelStart >= nPosBefore)
				nSelStart += nPosDifference;
			else
				nSelStart = nPosAfter;
		}
		if (nSelEnd >= nPosAfter) 
		{
			if (nSelEnd >= nPosBefore)
				nSelEnd += nPosDifference;
			else
				nSelEnd = nPosAfter;
		}
	}
	SetSelectionStart(nSelStart);
	SetSelectionStart(nSelEnd);
}

//int CScriptEditorView::GetLineIndentation(int nLine) 
//{
//	return GetLineIndentation(nLine);
//}

int CScriptEditorView::GetCurrentLineNumber() 
{
	return LineFromPosition(GetCurrentPos());
}

int CScriptEditorView::GetLineLength(int nLine) 
{
	return GetLineEndPosition(nLine) - PositionFromLine(nLine);
}

void CScriptEditorView::MaintainIndentation(char ch) 
{
	int nEolMode = GetEolMode();
	int nCurLine = GetCurrentLineNumber();
	int nLastLine = nCurLine - 1;
	int nIndentAmount = 0;
	
	if (((nEolMode == SC_EOL_CRLF || nEolMode == SC_EOL_LF) && ch == '\n') ||
		(nEolMode == SC_EOL_CR && ch == '\r')) 
	{
		while (nLastLine >= 0 && GetLineLength(nLastLine) == 0)
			nLastLine--;
		
		if (nLastLine >= 0) 
		{
			nIndentAmount = GetLineIndentation(nLastLine);
		}
		if (nIndentAmount > 0) 
		{
			SetLineIndentation(nCurLine, nIndentAmount);
		}
	}
}

CString CScriptEditorView::GetCurrentLine(int& nPos)
{
	nPos = GetCurrentPos();
	int nLine = LineFromPosition(nPos);
	nPos -= PositionFromLine(nLine);

	CString strLine;
	GetLine(nLine, strLine.GetBuffer(LineLength(nLine)));
	strLine.ReleaseBuffer();
	return strLine;
}

void CScriptEditorView::CharAdded(char ch)
{
	int nSelStart = GetSelectionStart();
	int nSelEnd = GetSelectionEnd();
	
	if ((nSelStart == nSelEnd) && (nSelStart > 0)) 
	{
		int nStyle = GetStyleAt(nSelStart - 1);

		if (nStyle != 1) 
		{
			// 如果提示活动
			if (CallTipActive()) 
			{
				if (ch == ')' || ch == '(') 
				{
					CallTipCancel();
					StartCallTip();
				} 
				else if (ch == ',')
				{
					StartCallTip();
				}
			} 
			// 如果列表活动
			else if (AutoCActive()) 
			{
				if (ch == ')' || ch == '(') 
				{
			//		AutoCComplete();
					StartCallTip();
				}
			}
			else 
			{
				MaintainIndentation(ch);

				if (ch == '(' || ch == ' ' || ch == ',') 
					StartCallTip();
				else  if (ch == '.')
					StartAutoCompleteWord();
			}
		}
	}
}

void CScriptEditorView::StartAutoCompleteWord()
{
	int nPos;
	CString strLine = GetCurrentLine(nPos);

	nPos--;
	CStringArray arrWords;
	if (!ParsePrevWords(strLine, nPos, arrWords))
		return;

	CStringArray arrProperties, arrMethods;
	if (!GetObjectMembers(arrWords, arrProperties, arrMethods))
		return;
	CString strList = ConvertToAutoCList(arrProperties, arrMethods);
	AutoCShow(0, strList);
}

void CScriptEditorView::StartCallTip()
{
	int nPos;
	CString strLine = GetCurrentLine(nPos);

	CStringArray arrWords;
	int nWordsEndPos = nPos - 1;

	// 如果解析到行头合法却没找到关键词
	if (ParsePrevWords(strLine, nWordsEndPos, arrWords, -1) && arrWords.GetCount() < 1)
	{
		LPTSTR p = (LPTSTR)(LPCTSTR)strLine;
		// 过滤掉前面空格
		while (*p != '\0')
		{
			if (!isspace(*p))
				break;
			p++;
		}

		while (*p != '\0')
		{
			if (isspace(*p))
				break;
			p++;
		}
		
		if (*p != '\0')
		{
			*p = '(';
			nWordsEndPos = p - (LPTSTR)(LPCTSTR)strLine;
			ParsePrevWords(strLine, nWordsEndPos, arrWords, -1);
		}
	}

	if (arrWords.GetCount() < 1)
	{
		if (CallTipActive())
			CallTipCancel();
		return;
	}

	CStringArray arrParams;
	if (!GetObjectMemberParams(arrWords, arrParams))
		return;

	CString strMethod = arrWords[arrWords.GetCount() - 1];

	int nParamStartPos = nPos - 1;
	int nParam = GetParamIndex(strLine, nParamStartPos);
	if (nParamStartPos != nWordsEndPos)
	{
		if (CallTipActive())
			CallTipCancel();
		return;
	}
	nParamStartPos = GetCurrentPos() - (nPos - nParamStartPos) - strMethod.GetLength() + 1;
	int nSelStart = 0;
	int nSelEnd = 0;

	CString strParam = strMethod;
	strParam += _T("(");
	for (int i = 0; i < (int)arrParams.GetCount(); i++)
	{
		if (i == nParam)
			nSelStart = strParam.GetLength();
	
		strParam += arrParams[i];

		if (i == nParam)
			nSelEnd = strParam.GetLength();
	
		if (i < (int)arrParams.GetCount() - 1)
			strParam += _T(", ");
	}
	strParam += _T(")");

	CallTipSetHlt(nSelStart, nSelEnd);
	CallTipShow(nParamStartPos, strParam);
	
}

// 对字符串中有可能使分析产生歧义的字符进行转换
void RegularString(CString& strText)
{
	LPTSTR p = (LPTSTR)(LPCTSTR)strText;
	BOOL bString = FALSE;
	while (*p != '\0')
	{
		if (*p == '"')
		{
			bString = !bString;
		}
		else if (bString)
		{
			if (*p == ',' || *p == ')' || *p == '(') 
				*p = 's';
		}
		p++;
	}
}

///////////////////////////////////////////////////////////////////
//功能：指定位置在参数中的序号
//nPos: 指定位置，返回为首参数位置
///////////////////////////////////////////////////////////////////
int CScriptEditorView::GetParamIndex(LPCTSTR lpszLine, int& pos)
{
	CString line = lpszLine;
	RegularString(line);
	int s = pos;
	int nBraceCount = 0;
	int n = 0;
	while (s >= 0)
	{
		if (line[s] == ')')
		{
			// 找到与之对应的括号
			nBraceCount -= 1;
		}
		else if (line[s] == '(')
		{
			nBraceCount += 1;
			if (nBraceCount > 0)
				break;
		}
		// 在括号对等的情况下处理词
		else if (nBraceCount == 0)
		{
			if (line[s] == ',')
				n++;
		}
		s--;
	}
	
	pos = s;
	return n;
}

CString CScriptEditorView::ConvertToAutoCList(CStringArray& arrProperties,CStringArray& arrMethods)
{
	CStringArray arrTemp;
	int i, j;

	//  属性
	for (i = 0; i < (int)arrMethods.GetCount(); i++)
	{
		CString strInsert = arrMethods[i];
		strInsert += _T("?0");
		BOOL bAdd = TRUE;
		for (j = 0; j < (int)arrTemp.GetCount(); j++)
		{
			if (strInsert.CompareNoCase(arrTemp[j]) <= 0)
			{
				if (strInsert != arrTemp[j])
					arrTemp.InsertAt(j, strInsert);
							
				bAdd = FALSE;
				break;
			}
		}

		if (bAdd)
			arrTemp.Add(strInsert);
	}

	// 方法
	for (i = 0; i < (int)arrProperties.GetCount(); i++)
	{
		CString strInsert = arrProperties[i];
		strInsert += _T("?1");//fclose?2 fopen
		BOOL bAdd = TRUE;
		for (j = 0; j < (int)arrTemp.GetCount(); j++)
		{
			if (strInsert.CompareNoCase(arrTemp[j]) <= 0)
			{
				if (strInsert != arrTemp[j])
					arrTemp.InsertAt(j, strInsert);
				
				bAdd = FALSE;
				break;
			}
		}
		
		if (bAdd)
			arrTemp.Add(strInsert);
	}

	CString strRet;
	for (i = 0; i < (int)arrTemp.GetCount(); i++)
	{
		if (!strRet.IsEmpty())
			strRet += _T(" ");
		CString str = arrTemp[i];
		strRet += arrTemp[i];
	}

	return strRet;
}

///////////////////////////////////////////////////////////////////
//功能：处理.前面字符，分解出有效词存放到字符串数组中
// 如 aa.bb. 得到 aa，bb; aa.bb(cc.gg(), . 得到 aa，bb; aa.bb(cc.gg( 得到 cc，dd
// pos为最后面有效词字符位 
// 目前没有处理WITH语句的情形
///////////////////////////////////////////////////////////////////
BOOL CScriptEditorView::ParsePrevWords(LPCTSTR lpszLine, int& pos, CStringArray& arrWords, int nBraceCount)
{
	USES_CHINESEVAR_CONVERSION;
	
	CString line(lpszLine, pos + 1);
	line = VARC2A(line);
	RegularString(line);
	int s = line.GetLength() - 1;
	int e = s;
	int nNextSeperator = -1;
	
	BOOL bValid = TRUE;
	while (s >= 0)
	{
		if (line[s] == ')')
		{
			// 找到与之对应的括号
			nBraceCount -= 1;
		}
		else if (line[s] == '(')
		{
			nBraceCount += 1;
			if (nBraceCount > 0)
			{
				// 加入合法词
				if (isalpha(line[s + 1]))
				{
					if (arrWords.GetCount() == 0)
						e = nNextSeperator;
					CString word = line.Mid(s + 1, nNextSeperator - s - 1);
					arrWords.InsertAt(0, VARA2C(word));
				}
				break;
			}

			nNextSeperator = s;
		}
		// 在括号对等的情况下处理词
		else if (nBraceCount == 0)
		{
			// 非字母数据下划线即认为是分隔符号
			if (!__iscsym(line[s]) || s == 0)
			{
				if (line[s] == '.' && nNextSeperator == -1)
				{
					nNextSeperator = s;
					s--;
					continue;
				}
				
				if (s == 0 && __iscsym(line[s]))
					s--;
				// 第一个字符必须为字母
				if (isalpha(line[s + 1]))
				{
					if (arrWords.GetCount() == 0)
						e = nNextSeperator;
					CString word = line.Mid(s + 1, nNextSeperator - s - 1);
					arrWords.InsertAt(0, VARA2C(word));
					if (s > 0 && line[s] != '.')
						break;
				}
				// 没有处理with语句的情况
				else
				{
					bValid = FALSE;
					break;
				}
				nNextSeperator = s;
			}
		}
		
		s--;
	}
	
	if (!bValid)
	{
		arrWords.RemoveAll();
	}
	else
	{
		CString strTemp(line, e);
		strTemp = VARA2C(strTemp);
		pos = strTemp.GetLength();  //返回最后一个字母位置
	}

	return bValid;
}

HWND CScriptEditorView::GetFindDialogHandle()
{
	_SCINTILLAEDIT_STATE* pEditState = GetScintillaEditState();
	
	if (pEditState->pFindReplaceDlg != NULL)
		return pEditState->pFindReplaceDlg->m_hWnd;

	return NULL;
}

void CScriptEditorView::SetActive(BOOL bActive)
{
	if (bActive)
		m_pActiveEditorView = this;
	else
		m_pActiveEditorView = NULL;
}

void CScriptEditorView::ReAnalyzeText()
{
#ifdef _DEBUG
	DWORD dwTick = GetTickCount();
#endif
	
	CString strScript;
	GetText(strScript);

	AnalyzeText(strScript);

#ifdef _DEBUG
	dwTick = GetTickCount() - dwTick;
	CString str;
	str.Format("%d", dwTick);
//	MessageBox(str);
#endif
}

