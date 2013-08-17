// ScintillaEditCtrl.h: interface for the CScintillaEditCtrlT class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SCINTILLAEDITCTRL_H_
#define __SCINTILLAEDITCTRL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Platform.h>
#include <Scintilla.h>
#include <SciLexer.h>

const char PIXMAP_FIRSTOPAQUEENTRY = '\"'+1;
const char PIXMAP_TRANSPARENTENTRY = 32;
const COLORREF CREF_NONE         = 0x01000000;
const COLORREF CREF_TOPLEFTPIXEL = 0x01000001;

template <class TBase>
class CScintillaEditCtrlT : public TBase
{
public:
	// Constructors
	CScintillaEditCtrlT(HWND hWnd = NULL) : TBase(hWnd) { }
	
	CScintillaEditCtrlT< TBase >& operator=(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}
	
	HWND Create(HWND hWndParent, _U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
		DWORD dwStyle = 0, DWORD dwExStyle = 0,
		_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		return CWindow::Create(GetWndClassName(), hWndParent, rect.m_lpRect, szWindowName, dwStyle, dwExStyle, MenuOrID.m_hMenu, lpCreateParam);
	}
	
	// Attributes
	static LPCTSTR GetWndClassName()
	{
		return _T("Scintilla");
	}
	
	static LPCTSTR GetLibraryName()
	{
		return _T("SciLexer.DLL");
	}
	
	// Text retrieval and modification
	void GetText(CString& rString)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		
		int nLen = GetLength();
		GetText(nLen + 1, rString.GetBufferSetLength(nLen + 1));
		rString.ReleaseBuffer();
	}
	void GetText(int nLength, LPTSTR lpszText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_GETTEXT, nLength, (LPARAM)lpszText);
	}
	void SetText(LPCTSTR lpszText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETTEXT, 0, (LPARAM)lpszText);
	}
	void SetSavePoint()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETSAVEPOINT, 0, 0L);
	}
	int GetLine(int nIndex, LPTSTR lpszBuffer)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETLINE, nIndex,
			(LPARAM)lpszBuffer);
	}
	void ReplaceSel(LPCTSTR lpszText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_REPLACESEL, 0,
			(LPARAM)lpszText);
	}
	void AppendText(int nLength, LPCTSTR lpszText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_APPENDTEXT, nLength,
			(LPARAM)lpszText);
	}
	void InsertText(int nPos, LPCTSTR lpszText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_INSERTTEXT, nPos,
			(LPARAM)lpszText);
	}
	void ClearAll()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_CLEARALL, 0, 0L);
	}
	void ClearDocumentStyle()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_CLEARDOCUMENTSTYLE, 0, 0L);
	}
	int GetCharAt(int nPos)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETCHARAT, nPos, 0L);
	}
	void SetStyleBits(int nBits = 5)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETSTYLEBITS, nBits, 0L);
	}
	int GetStyleAt(int nPos)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETSTYLEAT, nPos, 0L);
	}

	// Searching
	long FindText(int nSearchFlags, TextToFind* pttf)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd, SCI_FINDTEXT, nSearchFlags, (LPARAM)pttf);
	}

	// Cut, copy and paste
	void Cut()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_CUT, 0, 0L);
	}
	void Copy()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_COPY, 0, 0L);
	}
	void Paste()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_PASTE, 0, 0L);
	}
	void Clear()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_CLEAR, 0, 0L);
	}
	BOOL CanPaste()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, SCI_CANPASTE, 0, 0L);
	}

	// Undo and Redo
	void Undo()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_UNDO, 0, 0L);
	}
	BOOL CanUndo()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, SCI_CANUNDO, 0, 0L);
	}
	void Redo()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_REDO, 0, 0L);
	}
	BOOL CanRedo()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, SCI_CANREDO, 0, 0L);
	}
	void BeginUndoAction()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_BEGINUNDOACTION, 0, 0L);
	}
	void EndUndoAction()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_ENDUNDOACTION, 0, 0L);
	}
	void EmptyUndoBuffer()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_EMPTYUNDOBUFFER, 0, 0L);
	}

	// Line endings
	void SetEolMode(int nEOLMode)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETEOLMODE, nEOLMode, 0L);
	}

	int GetEolMode()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETEOLMODE, 0, 0L);
	}

	void ConvertEols(int nEolMode)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_CONVERTEOLS, nEolMode, 0L);
	}

	// Scrolling and automatic scrolling
	void LineScroll(int nColumn, int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_LINESCROLL, nColumn, (LPARAM)nLine);
	}

	// Autocompletion
	void AutoCShow(int nLenEntered, LPCTSTR lpszList)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_AUTOCSHOW, nLenEntered, (LPARAM)lpszList);
	}
	BOOL AutoCActive()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, SCI_AUTOCACTIVE, 0, 0L);
	}
	void AutoCCancel()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_AUTOCCANCEL, 0, 0L);
	}
	void RegisterImage(int nType, LPCTSTR lpszXpmData)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_REGISTERIMAGE, nType, (LPARAM)lpszXpmData);
	}
	void AutoCSetTypeSeparator(char chSeparatorCharacter)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_AUTOCSETTYPESEPARATOR, chSeparatorCharacter, 0L);
	}
	void AutoCComplete()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_AUTOCCOMPLETE, 0, 0L);
	}
	void AutoCSetFillups(LPCTSTR lpszChars)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_AUTOCSETFILLUPS, 0, (LPARAM)lpszChars);
	}
	void AutoCSetIgnoreCase(BOOL bIgnoreCase)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_AUTOCSETIGNORECASE, bIgnoreCase, 0L);
	}

	// Call tips
	void CallTipShow(int nPosStart, LPCTSTR lpszDefinition)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_CALLTIPSHOW, nPosStart, (LPARAM)lpszDefinition);
	}
	BOOL CallTipActive()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, SCI_CALLTIPACTIVE, 0, 0L);
	}
	void CallTipCancel()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_CALLTIPCANCEL, 0, 0L);
	}
	void CallTipSetHlt(int nHlStart, int nHlEnd)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_CALLTIPSETHLT, nHlStart, (LPARAM)nHlEnd);
	}
	void CallTipSetFore(COLORREF clrColour)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_CALLTIPSETFORE, clrColour, 0L);
	}
	void CallTipSetBack(COLORREF clrColour)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_CALLTIPSETBACK, clrColour, 0L);
	}
	void CallTipSetForeHlt(COLORREF clrColour)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_CALLTIPSETFOREHLT, clrColour, 0L);	
	}

	// Margins
	void SetMarginWidthN(int nMargin, int nPixelWidth)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETMARGINWIDTHN, nMargin, (LPARAM)nPixelWidth);
	}
	void SetMarginMaskN(int nMargin, int nMask)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETMARGINMASKN, nMargin, (LPARAM)nMask);
	}
	void SetMarginSensitiveN(int nMargin, BOOL bSensitive)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETMARGINSENSITIVEN, nMargin, (LPARAM)bSensitive);
	}
	void SetFoldMarginColour(BOOL bUseSetting, COLORREF clrColour)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETFOLDMARGINCOLOUR, bUseSetting, (LPARAM)clrColour);
	}
	void SetFoldMarginHiColour(BOOL bUseSetting, COLORREF clrColour)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETFOLDMARGINHICOLOUR, bUseSetting, (LPARAM)clrColour);
	}

	// Folding
	void SetFoldFlags(int nFlags)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETFOLDFLAGS, nFlags, 0L);
	}
	void ToggleFold(int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_TOGGLEFOLD, nLine, 0L);
	}
	void EnsureVisible(int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_ENSUREVISIBLE, nLine, 0L);
	}
	void EnsureVisibleEnforcePolicy(int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_ENSUREVISIBLEENFORCEPOLICY, nLine, 0L);
	}

	// Notifications
	void SetModEventMask(int nEventMask)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETMODEVENTMASK, nEventMask, 0L);
	}

	int GetModEventMask()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETMODEVENTMASK, 0, 0L);
	}

	// Markers
	void MarkerDefine(int nMarkerNumber, int nMarkerSymbols)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_MARKERDEFINE, nMarkerNumber, (LPARAM)nMarkerSymbols);
	}
	void MarkerSetFore(int nMarkerNumber, COLORREF clrFore)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_MARKERSETFORE, nMarkerNumber, (LPARAM)clrFore);
	}
	void MarkerSetBack(int nMarkerNumber, COLORREF clrBack)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_MARKERSETBACK, nMarkerNumber, (LPARAM)clrBack);
	}
	int MarkerAdd(int nLine, int nMarkerNumber)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_MARKERADD, nLine, (LPARAM)nMarkerNumber);
	}
	void MarkerDelete(int nLine, int nMarkerNumber)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_MARKERDELETE, nLine, (LPARAM)nMarkerNumber);
	}
	void MarkerDeleteAll(int nMarkerNumber)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_MARKERDELETEALL, nMarkerNumber, 0L);
	}
	DWORD MarkerGet(int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, SCI_MARKERGET, nLine, 0L);
	}
	int MarkerNext(int nLineStart, int nMarkerMask)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_MARKERNEXT, nLineStart, (LPARAM)nMarkerMask);
	}
	int MarkerPrevious(int nLineStart, int nMarkerMask)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_MARKERPREVIOUS, nLineStart, (LPARAM)nMarkerMask);
	}

	// Style definition
	void StyleClearAll()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_STYLECLEARALL, 0, 0L);
	}
	void StyleSetFore(int nStyleNumber, COLORREF clrFore)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_STYLESETFORE, nStyleNumber, (LPARAM)clrFore);
	}
	void StyleSetBack(int nStyleNumber, COLORREF clrBack)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_STYLESETBACK, nStyleNumber, (LPARAM)clrBack);
	}
	void StyleSetFont(int nStyle, LPTSTR lpszFontName)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_STYLESETFONT, nStyle, (LPARAM)lpszFontName);
	}
	void StyleSetSize(int nStyle, int nSizeInPoints)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_STYLESETSIZE, nStyle, (LPARAM)nSizeInPoints);
	}
	void StyleSetEolFilled(int nStyleNumber, BOOL bEolFilled)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_STYLESETEOLFILLED, nStyleNumber, (LPARAM)bEolFilled);
	}

	// Caret, selection, and hotspot styles
	void SetSelFore(BOOL bUseSelectionForeColour, COLORREF clrFore)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETSELFORE, bUseSelectionForeColour, (LPARAM)clrFore);
	}
	void SetSelBack(BOOL bUseSelectionBackColour, COLORREF clrBack)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETSELBACK, bUseSelectionBackColour, (LPARAM)clrBack);
	}

	// Printing
	long FormatRange(BOOL bDraw, RangeToFormat *pfr)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd, SCI_FORMATRANGE, bDraw, (LPARAM)pfr);
	}

	// Selection and information
	int GetLength()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETLENGTH, 0, 0L);	
	}
	int GetLineCount()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETLINECOUNT, 0, 0L);	
	}
	int LineFromPosition(int nPosition)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_LINEFROMPOSITION, nPosition, 0L);	
	}
	int GetColumn(int nPos)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETCOLUMN, nPos, 0L);	
	}
	int TextWidth(int nStyleNumber, LPTSTR lpszText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_TEXTWIDTH, nStyleNumber, (LPARAM)lpszText);	
	}
	int GetSelectionStart()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETSELECTIONSTART, 0, 0L);	
	}
	int GetSelectionEnd()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETSELECTIONEND, 0, 0L);	
	}
	void SetSelectionStart(int nPos)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETSELECTIONSTART, nPos, 0L);	
	}
	void SetSelectionEnd(int nPos)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETSELECTIONEND, nPos, 0L);	
	}
	void SetSel(int nAnchorPos, int nCurrentPos)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETSEL, nAnchorPos, (LPARAM)nCurrentPos);
	}
	int LineLength(int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_LINELENGTH, nLine, 0L);
	}
	void GotoPos(int nPos)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_GOTOPOS, nPos, 0L);
	}
	void GotoLine(int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_GOTOLINE, nLine, 0L);
	}
	void SelectAll()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SELECTALL, 0, 0L);
	}
	void HideSelection(BOOL bHide)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_HIDESELECTION, bHide, 0L);
	}
	void GetSelText(LPSTR lpszText)
	{ 
		ATLASSERT(::IsWindow(m_hWnd)); 
		::SendMessage(m_hWnd, SCI_GETSELTEXT, 0, (LPARAM)lpszText);
	}
	int PointXFromPosition(int nPos)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_POINTXFROMPOSITION, 0, (LPARAM)nPos);
	}
	int PointYFromPosition(int nPos)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_POINTYFROMPOSITION, 0, (LPARAM)nPos);
	}
	void SetCurrentPos(int nPos)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETCURRENTPOS, nPos, 0L);
	}
	int GetCurrentPos()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETCURRENTPOS, 0, 0L);
	}
	int PositionFromLine(int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_POSITIONFROMLINE, nLine, 0L);
	}
	int GetLineEndPosition(int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETLINEENDPOSITION, nLine, 0L);
	}
	BOOL GetModify()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, SCI_GETMODIFY, 0, 0L);
	}

	// Tabs and Indentation Guides
	void SetTabWidth(int nWidthInChars)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETTABWIDTH, nWidthInChars, 0L);
	}
	void SetIndent(int nWidthInChars)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETINDENT, nWidthInChars, 0L);
	}
	void SetIndentationGuides(BOOL bView)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETINDENTATIONGUIDES, bView, 0L);
	}
	int GetLineIndentation(int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETLINEINDENTATION, nLine, 0L);
	}
	void SetLineIndentation(int nLine, int nIndentation)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETLINEINDENTATION, nLine, (LPARAM)nIndentation);
	}
	int GetLineIndentPosition(int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETLINEINDENTPOSITION, nLine, 0L);
	}

	// Lexer
	void SetLexer(int nLexer)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETLEXER, nLexer, 0L);
	}
	void SetKeyWords(int nKeyWordSet, LPCTSTR lpszKeyWordList)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETKEYWORDS, nKeyWordSet, (LPARAM)lpszKeyWordList);
	}
	void SetProperty(LPCTSTR lpszkey, LPCTSTR lpszValue)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETPROPERTY, (WPARAM)lpszkey, (LPARAM)lpszValue);
	}

	// Other settings
	void SetCodePage(int nCodePage)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETCODEPAGE, nCodePage, 0L);
	}

	// Search and replace using the target
	void SetTargetStart(int nPos)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETTARGETSTART, nPos, 0L);
	}
	int GetTargetStart()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETTARGETSTART, 0, 0L);
	}
	void SetTargetEnd(int nPos)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETTARGETEND, nPos, 0L);
	}
	int GetTargetEnd()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_GETTARGETEND, 0, 0L);
	}

	int SearchInTarget(int nLength, LPCTSTR lpszText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_SEARCHINTARGET, nLength, (LPARAM)lpszText);
	}

	void SetSearchFlags(int nSearchFlags)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_SETSEARCHFLAGS, nSearchFlags, 0L);
	}

	int ReplaceTarget(int nLength, LPCTSTR lpszText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_REPLACETARGET, nLength, (LPARAM)lpszText);
	}

	int ReplaceTargetRE(int nLength, LPCTSTR lpszText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, SCI_REPLACETARGETRE, nLength, (LPARAM)lpszText);
	}

	// Popup edit menu
	void UsePopup(BOOL bEnablePopup)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, SCI_USEPOPUP, bEnablePopup, 0L);
	}

////////////////////////////////////////////////////////////

	void ConvertPaletteToPixmap(CString& strPalette, CPaletteHandle paletteHandle) 
	{
		strPalette.Format("\"%c c None\",\n", PIXMAP_TRANSPARENTENTRY);
		for (int i = 0; i < paletteHandle.GetEntryCount(); i++) 
		{
			CString strTemp;
			PALETTEENTRY palentry;
			paletteHandle.GetPaletteEntries(i, 1, &palentry);
			// Some pixmaps I've found, have 16 bits per channel, so the format
			// mask for each channel should be %02x00 instead of just %02x,
			// but Scintilla doesn't support those (Irfanview does, though)
			strTemp.Format("\"%c c #%02x%02x%02x\",\n", PIXMAP_FIRSTOPAQUEENTRY + i, 
				palentry.peRed, 
				palentry.peGreen,
				palentry.peBlue);
			strPalette += strTemp;
		}
	}

	void ConvertPixelsToPixmap(CString& strPixels, CSize& sizPixels, CDC& dcColor, CPaletteHandle paletteHandle, 
			const CDC& dcMask, COLORREF crefTransparent) 
	{
		int nWidth = sizPixels.cx;
		int nHeight = sizPixels.cy;

		// Check if the transparent color has to come from the topleft pixel
		if (crefTransparent == CREF_TOPLEFTPIXEL) 
		{
			ATLASSERT(dcMask.m_hDC == NULL);
			crefTransparent = dcColor.GetPixel(0,0);
		}

		// Image dump
		strPixels = "";
		for (int y = 0; y<nHeight; y++) 
		{
			strPixels += "\"";
			for (int x = 0; x<nWidth; x++) 
			{
				CString str;
				BOOL bTransparent;
				COLORREF crefColor;
				crefColor = dcColor.GetPixel(x,y);
				if (crefTransparent != CREF_NONE) 
				{
					if (dcMask.m_hDC != NULL) 
					{
						COLORREF crefMask;
						crefMask = dcMask.GetPixel(x,y);
						bTransparent = (crefMask == crefTransparent);
					} else 
					{
						bTransparent = (crefColor == crefTransparent);
					}
				} 
				else 
				{
					bTransparent = FALSE;
				}
				BYTE cColor;
				if (bTransparent) 
				{
					cColor = PIXMAP_TRANSPARENTENTRY;
				} 
				else 
				{
					cColor = paletteHandle.GetNearestPaletteIndex(crefColor) + 
						PIXMAP_FIRSTOPAQUEENTRY;
				}
				str.Format("%c",cColor);
				strPixels += str;
			}
			strPixels += "\",\n";
		}
		// There's an extra "," at the end, but it doesn't really matter.
	}

	void ConvertToPixmap(CString& strPixmap, HBITMAP hBmpColor, HBITMAP hBmpMask, 
			COLORREF clrTransparent) 
	{
 		ATLASSERT(hBmpColor != NULL);
    
		CClientDC dc(m_hWnd);
    
		// Ideally we would like to use DrawIcon on a memory HDC and 
		// use GetPixel to obtain the values, but for some reason DrawIcon on HICONs 
		// extracted from IMAGELISTs only renders the top left quarter of the icon
		// at x2 size. So instead, we attach the mask and the color HBITMAPs to memory HDCs
		// and calculate the masking ourselves.
    
		CDC dcMask;
		HBITMAP hBmpOldMask = NULL;
		if (hBmpMask != NULL) 
		{
			// If the mask bitmap is valid, attach the bitmap to a memory DC
			dcMask.CreateCompatibleDC(dc);       
			hBmpOldMask = dcMask.SelectBitmap(hBmpMask);
		}
    
		CDC dcColor;
		HBITMAP hBmpOldColor = NULL;

		// Attach the color bitmap to a memory DC
		dcColor.CreateCompatibleDC(dc);
		hBmpOldColor = dcColor.SelectBitmap(hBmpColor);

		// Get the color bitmap palette
		CPaletteHandle paletteHandle = dcColor.GetCurrentPalette();
		// Get the color bitmap extents
		BITMAP bitbmp;
		::GetObject(hBmpColor, sizeof(BITMAP), &bitbmp);
	    
		// Header dump
		CString strHeader;
		strHeader.Format(
			"/* XPM */\n"
			"static char * icon [] = {\n"
			"\"%d %d %d %d\",\n", 
			bitbmp.bmWidth, bitbmp.bmHeight, paletteHandle.GetEntryCount()+1, 1
			);
		// Get the palette text
		CString strPalette;
		ConvertPaletteToPixmap(strPalette, paletteHandle);
		// Get the pixel data text
		CString strPixels;
		ConvertPixelsToPixmap(strPixels, CSize(bitbmp.bmWidth, bitbmp.bmHeight), dcColor, paletteHandle, dcMask, clrTransparent);
		// Finish the pixmap
		strPixels += "}";
    
		// Release resources
		if (hBmpMask != NULL) 
			dcMask.SelectBitmap(hBmpOldMask);
		dcColor.SelectBitmap(hBmpOldColor);
	    
		strPixmap = strHeader + strPalette + strPixels;
	}

	void ConvertToPixmap(CString& strPixmap, const HICON hicon, 
			COLORREF clrBackground = RGB(0xFF, 0xFF, 0xFF))
	{
		ICONINFO iconinfo;
		
		// Get the icon information (mask & color bitmaps)
		GetIconInfo(hicon, &iconinfo);
		ConvertToPixmap(strPixmap, iconinfo.hbmColor, iconinfo.hbmMask, clrBackground);
	}
};
	
typedef CScintillaEditCtrlT<CWindow> CScintillaEditCtrl;

#endif __SCINTILLAEDITCTRL_H_
