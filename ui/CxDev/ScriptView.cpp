/////////////////////////////////////////////////////////////////////////
//所属项目: SuperCx
/////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifndef __ATLBASE_H__
#include <atlbase.h>
#endif	//#ifndef __ATLBASE_H__

#include "DevInc.h"
#include "../CxScripCrt/CxScripCrt_i.c"
#include "EnumString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptView

HWND CScriptView::c_hFindDialog = NULL;

IMPLEMENT_DYNCREATE(CScriptView, CFormView)

BEGIN_MESSAGE_MAP(CScriptView, CFormView)
	//{{AFX_MSG_MAP(CScriptView)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	ON_WM_DEVMODECHANGE()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CURSOR, OnUpdateCursor) 
	ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateNeedClip)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPEAT, OnUpdateNeedFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateNeedText)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_COMMAND(ID_EDIT_REPEAT, OnEditRepeat)
	//}}AFX_MSG_MAP
	ON_MESSAGE_VOID(WM_UPDATE_SYMBOL, OnUpdateCurrentSymbolPage)
	ON_COMMAND(ID_EDIT_GOTO, OnEditGoLine)
	ON_COMMAND(ID_BOOKMARK_TOGGLE, OnBookmarkToggle)
	ON_COMMAND(ID_BOOKMARK_NEXT, OnBookmarkNext)
	ON_COMMAND(ID_BOOKMARK_PREVIOUS, OnBookmarkPrevious)
	ON_COMMAND(ID_BOOKMARK_CLEAR, OnBookmarkClearAll)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectAll)
	ON_COMMAND(ID_PAGE_SETUP, OnPageSetup)
END_MESSAGE_MAP()

BEGIN_INTERFACE_MAP(CScriptView, CFormView)
	INTERFACE_PART(CScriptView, IID_ICxScriptEditorSite, ScriptEditorSite)
END_INTERFACE_MAP()

BEGIN_EVENTSINK_MAP(CScriptView, CFormView)
	ON_EVENT(CScriptView, AFX_IDW_PANE_FIRST, 2, OnSetModified, VTS_NONE)
	ON_EVENT(CScriptView, AFX_IDW_PANE_FIRST, 3, OnSymbolViewToggled, VTS_NONE)
	ON_EVENT(CScriptView, AFX_IDW_PANE_FIRST, 4, OnSymbolPageChanged, VTS_NONE)
END_EVENTSINK_MAP()

///////////////////////////////////////////////////////////////////////////////
// ICxScriptEditorSite
///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_( ULONG ) CScriptView::XScriptEditorSite::AddRef()
{
	METHOD_PROLOGUE_(CScriptView, ScriptEditorSite)
		
	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CScriptView::XScriptEditorSite::Release()
{
	METHOD_PROLOGUE_( CScriptView, ScriptEditorSite )
		
	return pThis->ExternalRelease();
}

STDMETHODIMP CScriptView::XScriptEditorSite::QueryInterface(
	REFIID iid, void** ppInterface)
{
	METHOD_PROLOGUE_(CScriptView, ScriptEditorSite)
		
	return pThis->ExternalQueryInterface(&iid, ppInterface);
}

STDMETHODIMP CScriptView::XScriptEditorSite::GetObjectNames(
	LPENUMSTRING* ppEnumObjects)
{
	METHOD_PROLOGUE(CScriptView, ScriptEditorSite)

	CStringArray arrObjects;
	pThis->GetDocument()->GetObjectNames(arrObjects);

	return CEnumString::CreateEnumString(arrObjects, ppEnumObjects);
}

STDMETHODIMP CScriptView::XScriptEditorSite::GetObject(
	LPCWSTR lpszObject, REFIID iid, void **ppObject)
{
	METHOD_PROLOGUE(CScriptView, ScriptEditorSite)

	return E_NOTIMPL;
}

STDMETHODIMP CScriptView::XScriptEditorSite::GetObjectTypeInfo(
	LPCWSTR lpszObject, ITypeInfo** ppTinfo)
{
	METHOD_PROLOGUE(CScriptView, ScriptEditorSite)

	return E_NOTIMPL;
}

STDMETHODIMP CScriptView::XScriptEditorSite::GetObjectSourceTypeInfo(
	LPCWSTR lpszObject, ITypeInfo** ppTinfo)
{
	METHOD_PROLOGUE(CScriptView, ScriptEditorSite)

	return E_NOTIMPL;
}

STDMETHODIMP CScriptView::XScriptEditorSite::GetObjectEvents(
	LPCWSTR lpszObject, LPENUMSTRING* ppEnumEvents)
{
	USES_CONVERSION;
	METHOD_PROLOGUE(CScriptView, ScriptEditorSite)

	CStringArray arrEvents;
	pThis->GetDocument()->GetObjectEvents(OLE2T(lpszObject), arrEvents);
	
	return CEnumString::CreateEnumString(arrEvents, ppEnumEvents);
}

STDMETHODIMP CScriptView::XScriptEditorSite::GetObjectEventParams(
	LPCWSTR lpszObject, LPCWSTR lpszEvent, LPENUMSTRING* ppEnumParams)
{
	USES_CONVERSION;
	METHOD_PROLOGUE(CScriptView, ScriptEditorSite)

	CStringArray arrParams;
	pThis->GetDocument()->GetObjectEventParams(OLE2T(lpszObject), OLE2T(lpszEvent), arrParams);
	
	return CEnumString::CreateEnumString(arrParams, ppEnumParams);	
}

STDMETHODIMP CScriptView::XScriptEditorSite::GetObjectMembers(
	LPCWSTR lpszObject, LPENUMSTRING* ppEnumProperties, LPENUMSTRING* ppEnumMethods)
{
	METHOD_PROLOGUE(CScriptView, ScriptEditorSite)

	CStringArray arrWords;

	if (lpszObject != NULL)
	{
		LPCWSTR p1 = lpszObject;
		LPCWSTR p2 = lpszObject;
		if (*p2 != '\0')
		{
			while (1)
			{
				if (*p2 == OLECHAR('.') || *p2 == '\0')
				{
					CString strWord(p1, p2 - p1);
					arrWords.Add(strWord);

					if (*p2 == '\0')
						break;

					p1 = p2 + 1;
				}

				p2++;
			}
		}
	}
	
	ASSERT(arrWords.GetSize() > 0);

	CStringArray arrObjects;
	CStringArray arrProperties;
	CStringArray arrMethods;
	if (pThis->GetDocument()->GetObjectMembers(arrWords, arrObjects, arrProperties, arrMethods))
	{
		arrProperties.Append(arrObjects);
		CEnumString::CreateEnumString(arrProperties, ppEnumProperties);	
		CEnumString::CreateEnumString(arrMethods, ppEnumMethods);

		return S_OK;
	}
	
	return S_FALSE;
}

STDMETHODIMP CScriptView::XScriptEditorSite::GetObjectMemberParams(
	LPCWSTR lpszObject, LPENUMSTRING* ppEnumParams)
{
	METHOD_PROLOGUE(CScriptView, ScriptEditorSite)

	CStringArray arrWords;
	
	if (lpszObject != NULL)
	{
		LPCWSTR p1 = lpszObject;
		LPCWSTR p2 = lpszObject;
		if (*p2 != '\0')
		{
			while (1)
			{
				if (*p2 == OLECHAR('.') || *p2 == '\0')
				{
					CString strWord(p1, p2 - p1);
					arrWords.Add(strWord);
					
					if (*p2 == '\0')
						break;
					
					p1 = p2 + 1;
				}
				
				p2++;
			}
		}	
	}

	CStringArray arrParams;
	if (pThis->GetDocument()->GetObjectMemberParams(arrWords, arrParams))
		return CEnumString::CreateEnumString(arrParams, ppEnumParams);

	return S_FALSE;
}

STDMETHODIMP CScriptView::XScriptEditorSite::GetGlobalTypeInfoCount(
	UINT* pnInfoCount)
{
	USES_CONVERSION;
	METHOD_PROLOGUE(CScriptView, ScriptEditorSite)
		
	*pnInfoCount = 2;
	
	return S_OK;
}

const IID IID_ICxCommomFunctions = {0xE6EC9619,0x741A,0x4bab,{0x94,0xB4,0xB2,0xC9,0xAA,0xD9,0xD0,0x00}};

STDMETHODIMP CScriptView::XScriptEditorSite::GetGlobalTypeInfo(
	UINT iTypeInfo, ITypeInfo** ppTinfo)
{
	USES_CONVERSION;
	METHOD_PROLOGUE(CScriptView, ScriptEditorSite)

	if (iTypeInfo == 0)
		return theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(ICxGlobalFunctions), ppTinfo);
	if (iTypeInfo == 1)
		return theApp.m_pComTypeLib->GetTypeInfoOfGuid(IID_ICxCommomFunctions, ppTinfo);
	
	return E_INVALIDARG;	
}

STDMETHODIMP CScriptView::XScriptEditorSite::ShowHelpString(
	LPCWSTR lpszObject)
{
	USES_CONVERSION;
	METHOD_PROLOGUE(CScriptView, ScriptEditorSite)

	return pThis->GetDocument()->ShowHelpString(OLE2T(lpszObject)) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CScriptView construction/destruction

CScriptView::CScriptView() :
	CFormView((LPCTSTR) NULL)
{
	m_bSetModified = TRUE;

	m_rectMargin = theApp.m_settingGen.m_rectPageMargin;
}

CScriptView::~CScriptView()
{
}

BOOL CScriptView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// create the view window itself
	m_pCreateContext = pContext;
	if (!CView::Create(lpszClassName, lpszWindowName,
		dwStyle, rect, pParentWnd,  nID, pContext))
	{
		return FALSE;
	}
	
	AfxEnableControlContainer();
	
	RECT rectClient;
	GetClientRect(&rectClient);
	
	// create the control window
	// AFX_IDW_PANE_FIRST is a safe but arbitrary ID
	if (!m_wndScriptEidor.CreateControl(CLSID_CxScriptEditor, lpszWindowName,
		WS_VISIBLE | WS_CHILD, rectClient, this, AFX_IDW_PANE_FIRST))
	{
		DestroyWindow();
		return FALSE;
	}
	
	LPUNKNOWN lpUnk = m_wndScriptEidor.GetControlUnknown();
	HRESULT hr = lpUnk->QueryInterface(IID_ICxScriptEditor, (void**)&m_pScriptEditor);
	if (FAILED(hr))
	{
		m_pScriptEditor = NULL;
		m_wndScriptEidor.DestroyWindow();
		DestroyWindow();
		return FALSE;
	}

	m_pScriptEditor->SetScriptEditorSite(&m_xScriptEditorSite);
	m_pScriptEditor->GetRangeToFormatStruct(&m_pRangeToFormat);
	
	return TRUE;
}

void CScriptView::OnPaint()
{
	// this is done to avoid CView::OnPaint
	Default();
}

void CScriptView::OnInitialUpdate()
{
	USES_CONVERSION;

	CFormView::OnInitialUpdate();

	CFormDoc* pDoc = (CFormDoc *)GetDocument();
	if (pDoc != NULL)
	{
		m_bSetModified = FALSE;
		SetText(pDoc->m_strScript);
		m_bSetModified = TRUE;
		EmptyUndoBuffer();
		SetSavePoint();

		AddGeneralSymbolPage(CX_ALL_SYMBOL);
		AddSymbolTreePage(_T("窗体"), NULL);
		if (pDoc->IsKindOf(RUNTIME_CLASS(CReportDoc)))
			AddSymbolTreePage(_T("记录集"), NULL);
		AddOPCServerSymbolPage();

		ShowSymbolView(theApp.m_settingGen.m_bShowScriptSymbol);
	}
}

BOOL CScriptView::PreCreateWindow(CREATESTRUCT& cs) 
{
	BOOL b = CFormView::PreCreateWindow(cs);
	
	cs.style |= WS_CLIPCHILDREN;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

// 	cs.style &= ~WS_BORDER;
// 	cs.dwExStyle &= ~WS_BORDER;
	
	return b;
}

void CScriptView::OnSetModified()
{
	if (m_bSetModified && GetDocument() != NULL)
		GetDocument()->SetModifiedFlag();
}

void CScriptView::OnSymbolViewToggled()
{
	theApp.m_settingGen.m_bShowScriptSymbol = IsSymbolViewVisible();
}

void CScriptView::OnSymbolPageChanged()
{
	UpdateSymbolPage(-1);
}

void CScriptView::OnDestroy() 
{
	m_pRangeToFormat.Release();

	CFormDoc* pDoc = GetDocument();
	if (pDoc != NULL)
	{
		GetText(pDoc->m_strScript);

		if (GetModify())
			pDoc->SetModifiedFlag();
	}
	
	CFormView::OnDestroy();
}

///////////////////////////////////////////////////////
//功  能:	上下文菜单
///////////////////////////////////////////////////////
void CScriptView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.CreatePopupMenu();
	CString strSel;
	BOOL bPaste;

	CWnd* pMainWnd = AfxGetMainWnd();
	ASSERT(pMainWnd != NULL);
	CMenu* pMainMenu = pMainWnd->GetMenu();
	ASSERT(pMainMenu != NULL);

	strSel = GetSelText();
	bPaste = CanPaste();

	if (strSel.IsEmpty())
	{
		if (bPaste)
		{
			menu.AppendMenu(MF_BYPOSITION, ID_EDIT_PASTE,		"粘贴(&P)");
			menu.AppendMenu(MF_SEPARATOR, 0, LPCTSTR(NULL));
		}
		menu.AppendMenu(MF_BYPOSITION, ID_EDIT_SELECTALL,	"全选(&A)");
	}
	else
	{
		menu.AppendMenu(MF_BYPOSITION, ID_EDIT_CUT,			"剪切(&T)");
		menu.AppendMenu(MF_BYPOSITION, ID_EDIT_COPY,			"拷贝(&C)");
		if (bPaste)
			menu.AppendMenu(MF_BYPOSITION, ID_EDIT_PASTE,		"粘贴(&P)");
		menu.AppendMenu(MF_BYPOSITION, ID_EDIT_CLEAR,		"删除(&D)");
		menu.AppendMenu(MF_SEPARATOR, 0, LPCTSTR(NULL));
		menu.AppendMenu(MF_BYPOSITION, ID_EDIT_SELECTALL,	"全选(&A)");
	}

	menu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);
	menu.DestroyMenu();
}

/////////////////////////////////////////////////////////////////////////////
// CScriptView diagnostics

#ifdef _DEBUG
void CScriptView::AssertValid() const
{
	CFormView::AssertValid();
}

void CScriptView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFormDoc* CScriptView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFormDoc)));
	return (CFormDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScriptView message handlers

////////////////////////////////////////////////////////////
//功 能：当视图转换为不激活时，自动生成的事件函数如果没有改动，则删除
///////////////////////////////////////////////////////////////
void CScriptView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	m_pScriptEditor->SetActive(bActivate);

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CScriptView::OnUpdateCursor(CCmdUI *pCmdUI)
{
	CString strPos;
	
	int nPos = GetCurrentPos();
	int nRow = LineFromPosition(nPos);
	int nCol = GetColumn(nPos);
	strPos.Format("%d,%d", nRow + 1, nCol + 1);
	pCmdUI->SetText(strPos);
}

BOOL CScriptView::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST) ||
		pMsg->message >= WM_MOUSEFIRST && (pMsg->message <= WM_MOUSELAST))
	{
		IOleInPlaceActiveObjectPtr pInPlaceActiveObject = m_pScriptEditor;
		if (pInPlaceActiveObject->TranslateAccelerator(pMsg) == S_OK)
			return TRUE;
	}

	return FALSE;
}

void CScriptView::OnHelpIndex() 
{
	CString strKeyword = GetSelText();

	HH_AKLINK link;
	
	link.cbStruct = sizeof(HH_AKLINK);
	link.fReserved = FALSE;
	link.pszKeywords = strKeyword; 
	link.pszUrl = NULL; 
	link.pszMsgText = NULL; 
	link.pszMsgTitle = NULL; 
	link.pszWindow = NULL;
	link.fIndexOnFail = TRUE ;
	
	CString strHelpFile = theApp.GetCurrentAppDirectory();
	strHelpFile += _T("CxHlp.chm");
	HtmlHelp(
		NULL,
		strHelpFile,
		HH_KEYWORD_LOOKUP,
		(DWORD)&link);
}

void CScriptView::OnPageSetup() 
{
	CPageSetupDialog dlg;
	PAGESETUPDLG& psd = dlg.m_psd;
	psd.Flags |= PSD_MARGINS | PSD_INHUNDREDTHSOFMILLIMETERS;
	MulDivRect(&psd.rtMargin, m_rectMargin, 2540, 1440);
	RoundRect(&psd.rtMargin);
	// get the current device from the app
	PRINTDLG pd;
	pd.hDevNames = NULL;
	pd.hDevMode = NULL;
	theApp.GetPrinterDeviceDefaults(&pd);
	psd.hDevNames = pd.hDevNames;
	psd.hDevMode = pd.hDevMode;
	if (dlg.DoModal() == IDOK)
	{
		RoundRect(&psd.rtMargin);
		MulDivRect(m_rectMargin, &psd.rtMargin, 1440, 2540);
		theApp.m_settingGen.m_rectPageMargin = m_rectMargin;
		theApp.SelectPrinter(psd.hDevNames, psd.hDevMode);
	}
}

void CScriptView::OnFilePrint()
{
	CFormView::OnFilePrint();
}

void CScriptView::OnUpdateNeedSel(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	int nStartChar = GetSelectionStart();
	int nEndChar = GetSelectionEnd();
	pCmdUI->Enable(nStartChar != nEndChar);
	ASSERT_VALID(this);
}

void CScriptView::OnUpdateNeedClip(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(CanPaste());
}

void CScriptView::OnUpdateNeedText(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(GetTextLength() != 0);
}

void CScriptView::OnUpdateNeedFind(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);

	pCmdUI->Enable(GetTextLength() > 0 && HasSearched());
}

void CScriptView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	BOOL b;
	m_pScriptEditor->CanUndo(&b);
	pCmdUI->Enable(b);
}

void CScriptView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	BOOL b;
	m_pScriptEditor->CanRedo(&b);
	pCmdUI->Enable(b);
}

void CScriptView::OnEditCut()
{
	ASSERT_VALID(this);
	m_pScriptEditor->Cut();
}

void CScriptView::OnEditCopy()
{
	ASSERT_VALID(this);
	m_pScriptEditor->Copy();
}

void CScriptView::OnEditPaste()
{
	ASSERT_VALID(this);
	m_pScriptEditor->Paste();
}

void CScriptView::OnEditClear()
{
	ASSERT_VALID(this);
	m_pScriptEditor->Clear();
}

void CScriptView::OnEditUndo()
{
	ASSERT_VALID(this);
	m_pScriptEditor->Undo();
}

void CScriptView::OnEditRedo()
{
	ASSERT_VALID(this);
	m_pScriptEditor->Redo();
}

void CScriptView::OnEditSelectAll()
{
	ASSERT_VALID(this);
	m_pScriptEditor->SelectAll();
}
void CScriptView::OnEditFind()
{
	ASSERT_VALID(this);
	m_pScriptEditor->Find();

	m_pScriptEditor->get_FindDialogHandle((long *)&c_hFindDialog);
}

void CScriptView::OnEditReplace()
{
	ASSERT_VALID(this);
	m_pScriptEditor->Replace();

	m_pScriptEditor->get_FindDialogHandle((long *)&c_hFindDialog);
}

void CScriptView::OnEditRepeat()
{
	ASSERT_VALID(this);
	m_pScriptEditor->FindRepeat();
}

void CScriptView::OnEditGoLine()
{
	ASSERT_VALID(this);
	m_pScriptEditor->GoLine();
}

void CScriptView::OnBookmarkToggle()
{
	ASSERT_VALID(this);
	m_pScriptEditor->BookmarkToggle();
}

void CScriptView::OnBookmarkNext()
{
	ASSERT_VALID(this);
	m_pScriptEditor->BookmarkNext();
}

void CScriptView::OnBookmarkPrevious()
{
	ASSERT_VALID(this);
	m_pScriptEditor->BookmarkPrevious();
}

void CScriptView::OnBookmarkClearAll()
{
	ASSERT_VALID(this);
	m_pScriptEditor->BookmarkClearAll();
}

BOOL CScriptView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CScriptView::GetText(CString& rString)
{
	CComBSTR bstr;
	m_pScriptEditor->get_Text(&bstr);
	rString = bstr;
}

void CScriptView::SetText(LPCTSTR lpszText)
{
	USES_CONVERSION;
	m_pScriptEditor->put_Text(T2BSTR(lpszText));
}

long CScriptView::GetTextLength() const
{
	long lLength;
	m_pScriptEditor->GetTextLength(&lLength);
	return lLength;
}

void CScriptView::EmptyUndoBuffer()
{
	m_pScriptEditor->EmptyUndoBuffer();
}

void CScriptView::SetSavePoint()
{
	m_pScriptEditor->SetSavePoint();
}

void CScriptView::SetSelectionStart(int nPos)
{
	m_pScriptEditor->SetSelectionStart(nPos);
}

void CScriptView::SetSelectionEnd(int nPos)
{
	m_pScriptEditor->SetSelectionEnd(nPos);
}

int CScriptView::GetSelectionStart()
{
	long lPos;
	m_pScriptEditor->GetSelectionStart(&lPos);
	return lPos;
}

int CScriptView::GetSelectionEnd()
{
	long lPos;
	m_pScriptEditor->GetSelectionEnd(&lPos);
	return lPos;
}

void CScriptView::ReplaceSel(LPCTSTR lpszText)
{
	USES_CONVERSION;
	m_pScriptEditor->ReplaceSel(T2BSTR(lpszText));
}

void CScriptView::AppendText(int nLength, LPCTSTR lpszText)
{
	USES_CONVERSION;
	m_pScriptEditor->AppendText(nLength, T2BSTR(lpszText));
}

void CScriptView::GotoLine(int nLine)
{
	m_pScriptEditor->SetSelectionEnd(nLine);
}

int CScriptView::GetLineCount()
{
	long lCount;
	m_pScriptEditor->GetLineCount(&lCount);
	return lCount;
}

CString CScriptView::GetSelText() const
{
	USES_CONVERSION;

	CComBSTR bstr;
	m_pScriptEditor->GetSelText(&bstr);
	return bstr;
}

BOOL CScriptView::GetModify()
{
	BOOL b;
	m_pScriptEditor->GetModify(&b);
	return b;
}

BOOL CScriptView::CanPaste()
{
	BOOL b;
	m_pScriptEditor->CanPaste(&b);
	return b;
}

int CScriptView::GetCurrentPos()
{
	long lPos;
	m_pScriptEditor->GetCurrentPos(&lPos);
	return lPos;
}

int CScriptView::LineFromPosition(int nPos)
{
	long lLine;
	m_pScriptEditor->LineFromPosition(nPos, &lLine);
	return lLine;
}

int CScriptView::GetColumn(int nPos)
{
	long lCol;
	m_pScriptEditor->GetColumn(nPos, &lCol);
	return lCol;
}	

BOOL CScriptView::HasSearched()
{
	BOOL b;
	m_pScriptEditor->HasSearched(&b);
	return b;
}

void CScriptView::BeginUndoAction()
{
	m_pScriptEditor->BeginUndoAction();
}

void CScriptView::EndUndoAction()
{
	m_pScriptEditor->EndUndoAction();
}

void CScriptView::EditObjectEventHandler(LPCTSTR lpszObject, LPCTSTR lpszEvent, BOOL bCreateAndEdit)
{
	USES_CONVERSION;
	m_pScriptEditor->EditObjectEventHandler(T2BSTR(lpszObject), T2BSTR(lpszEvent), bCreateAndEdit);
}

void CScriptView::DeleteEmptyEventHandler()
{
	m_pScriptEditor->DeleteEmptyEventHandler();
}

BOOL CScriptView::IsSymbolViewVisible()
{
	BOOL b;
	m_pScriptEditor->get_SymbolViewVisible(&b);
	return b;
}

void CScriptView::ShowSymbolView(BOOL b)
{
	m_pScriptEditor->put_SymbolViewVisible(b);
}

void CScriptView::AddGeneralSymbolPage(DWORD dwSymbolMask)
{
	m_pScriptEditor->AddGeneralSymbolPage(dwSymbolMask);
}

void CScriptView::AddOPCServerSymbolPage()
{
	m_pScriptEditor->AddOPCServerSymbolPage();
}

BOOL CScriptView::AddSymbolTreePage(LPCTSTR lpszCaption, ICxSymbolTreePage** ppPage)
{
	USES_CONVERSION;
	return m_pScriptEditor->AddSymbolTreePage(T2BSTR(lpszCaption), ppPage) == S_OK;
}

BOOL CScriptView::GetCurrentSymbolTreePage(ICxSymbolTreePage** ppPage)
{
	USES_CONVERSION;
	return m_pScriptEditor->GetCurrentSymbolTreePage(ppPage) == S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CScriptView Printing support

long CScriptView::FormatRange(BOOL bDraw, BOOL bUseStruct)
{
	long lPos;
	m_pScriptEditor->FormatRange(TRUE, bUseStruct, &lPos);
	return lPos;
}

long CScriptView::PrintPage(CDC* pDC, long nIndexStart, long nIndexStop)
// worker function for laying out text in a rectangle.
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	POINT pt;
	CRect rectMargin;
	CRect rectPage;
	CRect rectPrint;
	
	pDC->Escape(GETPRINTINGOFFSET, 0, NULL, &pt);
	rectMargin.left = pt.x;
	rectMargin.top  = pt.y;
	
	pDC->Escape(GETPHYSPAGESIZE, 0, NULL, &pt);
	rectPage.SetRect(0, 0, pt.x, pt.y);
	rectPrint.SetRect(0, 0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	
	rectMargin.right  = pt.x                     // total paper width
		- pDC->GetDeviceCaps(HORZRES) // printable width
		- rectMargin.left;   // left unprtable margin
	
	rectMargin.bottom = pt.y                     // total paper height
		- pDC->GetDeviceCaps(VERTRES) // printable ht
		- rectMargin.top;    // rt unprtable margin
	
	pt.x = pDC->GetDeviceCaps(LOGPIXELSX);    // dpi in X direction
	pt.y = pDC->GetDeviceCaps(LOGPIXELSY);    // dpi in Y direction
	
	rectMargin.left = MulDiv(m_rectMargin.left, pt.x, 1440)
		- rectMargin.left;
	rectMargin.top  = MulDiv(m_rectMargin.top, pt.y, 1440)
		- rectMargin.top;
	rectMargin.right  = MulDiv(m_rectMargin.right, pt.x, 1440)
		- rectMargin.right;
	rectMargin.bottom = MulDiv(m_rectMargin.bottom, pt.y, 1440)
		- rectMargin.bottom;
	
	rectPrint.InflateRect(-rectMargin.left, -rectMargin.top, -rectMargin.right, -rectMargin.bottom);
	
	
	// offset by printing offset
//	pDC->SetViewportOrg(-pDC->GetDeviceCaps(PHYSICALOFFSETX),
//		-pDC->GetDeviceCaps(PHYSICALOFFSETY));
	// adjust DC because richedit doesn't do things like MFC
	if (::GetDeviceCaps(pDC->m_hDC, TECHNOLOGY) != DT_METAFILE && pDC->m_hAttribDC != NULL)
	{
//		::ScaleWindowExtEx(pDC->m_hDC,
//			::GetDeviceCaps(pDC->m_hDC, LOGPIXELSX),
//			::GetDeviceCaps(pDC->m_hAttribDC, LOGPIXELSX),
//			::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY),
//			::GetDeviceCaps(pDC->m_hAttribDC, LOGPIXELSY), NULL);

	}

	m_pRangeToFormat->put_TargetDC((long)pDC->m_hAttribDC);
	m_pRangeToFormat->put_DC((long)pDC->m_hDC);
	m_pRangeToFormat->put_PageLeft(rectPage.left);
	m_pRangeToFormat->put_PageTop(rectPage.top);
	m_pRangeToFormat->put_PageRight(rectPage.right);
	m_pRangeToFormat->put_PageBottom(rectPage.bottom);
	m_pRangeToFormat->put_Left(rectPrint.left);
	m_pRangeToFormat->put_Top(rectPrint.top);
	m_pRangeToFormat->put_Right(rectPrint.right);
	m_pRangeToFormat->put_Bottom(rectPrint.bottom);
	m_pRangeToFormat->put_Min(nIndexStart);
	m_pRangeToFormat->put_Max(nIndexStop);
	
	return FormatRange(TRUE, TRUE);
}

void CScriptView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_bContinuePrinting);
	
	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage <= (UINT)m_aPageStart.GetSize());
	long nIndex = (long) m_aPageStart[nPage-1];
	
	// print as much as possible in the current page.
	nIndex = PrintPage(pDC, nIndex, GetTextLength());
	
	if (nIndex >= GetTextLength())
	{
		TRACE0("End of Document\n");
		pInfo->SetMaxPage(nPage);
	}
	
	// update pagination information for page just printed
	if (nPage == (UINT)m_aPageStart.GetSize())
	{
		if (nIndex < GetTextLength())
			m_aPageStart.Add(nIndex);
	}
	else
	{
		ASSERT(nPage+1 <= (UINT)m_aPageStart.GetSize());
		ASSERT(nIndex == (long)m_aPageStart[nPage+1-1]);
	}
}

void CScriptView::OnPrinterChanged(const CDC& dcPrinter)
{
	// this is typically called by the view when it gets a WM_DEVMODECHANGE
	// also called during page setup
	CSize size;
	if (dcPrinter.m_hDC != NULL)
	{
		// this will fill in the page size
		size.cx = MulDiv(dcPrinter.GetDeviceCaps(PHYSICALWIDTH), 1440,
			dcPrinter.GetDeviceCaps(LOGPIXELSX));
		size.cy = MulDiv(dcPrinter.GetDeviceCaps(PHYSICALHEIGHT), 1440,
			dcPrinter.GetDeviceCaps(LOGPIXELSY));
	}
	else
		size = CSize(8*1440+720, 11*1440); // 8.5" by 11"
	if (GetPaperSize() != size)
	{
		SetPaperSize(size);
//		if (m_nWordWrap == WrapToTargetDevice) //wrap to ruler
//			WrapChanged();
	}
}

void CScriptView::OnDevModeChange(LPTSTR lpDeviceName) 
{
	// WM_DEVMODECHANGE forwarded by the main window of the app
	CDC dc;
	AfxGetApp()->CreatePrinterDC(dc);
	OnPrinterChanged(dc);
}

void CScriptView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	ASSERT_VALID(this);
//  ASSERT_VALID(pDC);
	// initialize page start vector
	ASSERT(m_aPageStart.GetSize() == 0);
	m_aPageStart.Add(0);
	ASSERT(m_aPageStart.GetSize() > 0);
	FormatRange(FALSE, FALSE); // required by RichEdit to clear out cache
	
	ASSERT_VALID(this);
}

void CScriptView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	ASSERT_VALID(this);
	FormatRange(FALSE, FALSE); // required by RichEdit to clear out cache
	m_aPageStart.RemoveAll();
}

BOOL CScriptView::PaginateTo(CDC* pDC, CPrintInfo* pInfo)
	// attempts pagination to pInfo->m_nCurPage, TRUE == success
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	
	CRect rectSave = pInfo->m_rectDraw;
	UINT nPageSave = pInfo->m_nCurPage;
	ASSERT(nPageSave > 1);
	ASSERT(nPageSave >= (UINT)m_aPageStart.GetSize());
	VERIFY(pDC->SaveDC() != 0);
	pDC->IntersectClipRect(0, 0, 0, 0);
	pInfo->m_nCurPage = m_aPageStart.GetSize();
	while (pInfo->m_nCurPage < nPageSave)
	{
		ASSERT(pInfo->m_nCurPage == (UINT)m_aPageStart.GetSize());
		OnPrepareDC(pDC, pInfo);
		ASSERT(pInfo->m_bContinuePrinting);
		pInfo->m_rectDraw.SetRect(0, 0,
			pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
		pDC->DPtoLP(&pInfo->m_rectDraw);
		OnPrint(pDC, pInfo);
		if (pInfo->m_nCurPage == (UINT)m_aPageStart.GetSize())
			break;
		++pInfo->m_nCurPage;
	}
	BOOL bResult = pInfo->m_nCurPage == nPageSave;
	pDC->RestoreDC(-1);
	pInfo->m_nCurPage = nPageSave;
	pInfo->m_rectDraw = rectSave;
	ASSERT_VALID(this);
	return bResult;
}

void CScriptView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT(pInfo != NULL);  // overriding OnPaint -- never get this.
	
	pDC->SetMapMode(MM_TEXT);
	
	if (pInfo->m_nCurPage > (UINT)m_aPageStart.GetSize() &&
		!PaginateTo(pDC, pInfo))
	{
		// can't paginate to that page, thus cannot print it.
		pInfo->m_bContinuePrinting = FALSE;
	}
	ASSERT_VALID(this);
}


void CScriptView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	if (::IsWindow(m_wndScriptEidor.m_hWnd))
	{
		// need to push non-client borders out of the client area
		CRect rect;
		GetClientRect(rect);
		::AdjustWindowRectEx(rect,
			m_wndScriptEidor.GetStyle(), FALSE, WS_EX_CLIENTEDGE);
		m_wndScriptEidor.SetWindowPos(NULL, rect.left, rect.top,
			rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

BOOL CScriptView::PreTranslateAppMessage(MSG* pMsg)
{
	if (::IsWindow(c_hFindDialog) && ::IsDialogMessage(c_hFindDialog, pMsg))
		return TRUE;

	return FALSE;	// message not handled
}

void CScriptView::UpdateSymbolPage(DWORD dwUpdateMask)
{
	USES_CONVERSION;
	CFormDoc* pDoc = GetDocument();

	CComPtr<ICxSymbolTreePage> spSymbolTreePage;
	GetCurrentSymbolTreePage(&spSymbolTreePage);

	if (spSymbolTreePage != NULL)
	{
		CComBSTR bstrCaption;
		spSymbolTreePage->get_Caption(&bstrCaption);

		// 窗体
		if (bstrCaption == L"窗体")
		{
			if ((dwUpdateMask & UPDATE_SYMBOL_FORM) != 0 ||
				((dwUpdateMask & UPDATE_SYMBOL_OBJECT) != 0 && spSymbolTreePage->HasNodeExpanded() == S_OK))
			{
				spSymbolTreePage->SetDefaultNameSpace(T2BSTR(pDoc->GetDisplayName()));
				spSymbolTreePage->SetSymbolTreeSite(&pDoc->m_xSymbolTreeSite);
				spSymbolTreePage->Populate();
			}
		}
		else if (bstrCaption == L"记录集")
		{
			CReportDoc* pReportDoc = (CReportDoc *)pDoc;

			COleDispatchDriver ddTemp(spSymbolTreePage, FALSE);
				
			OLE_HANDLE hItem = NULL;
			static BYTE BASED_CODE parms[] =
				VTS_HANDLE VTS_BSTR VTS_DISPATCH VTS_UNKNOWN;

			// 数据记录集
			IDispatch* pIObj = (IDispatch *)pReportDoc->m_pDataRecordset;
			ddTemp.InvokeHelper(2, DISPATCH_METHOD, VT_HANDLE, &hItem, parms, 
					NULL, _T("DataRecordset"), pIObj, NULL);
			
			if (pReportDoc->m_pDataRecordset->Open() == S_OK)
			{
				long lFieldCount;
				pReportDoc->m_pDataRecordset->get_FieldCount(&lFieldCount);
				for (long l = 0; l < lFieldCount; l++)
				{
					ICxDataFieldPtr pDataField;
					CComVariant vaIndex = l + 1;
					pReportDoc->m_pDataRecordset->get_Field(vaIndex, &pDataField);
					CComBSTR bstrFieldName;
					pDataField->get_Name(&bstrFieldName);
					CString strName = _T("DataRecordset(\"");
					strName += OLE2T(bstrFieldName);
					strName += _T("\")");
					pIObj = (IDispatch *)pDataField;
					ddTemp.InvokeHelper(2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NULL, strName, pIObj, NULL);
				}
			}

			spSymbolTreePage->Populate();
		}
//		else if (bstrCaption == L"OPC")
//		{
//			spSymbolTreePage->Populate();
//		}
	}
}

void CScriptView::OnUpdateCurrentSymbolPage()
{
	CFormDoc* pDoc = GetDocument();
	UpdateSymbolPage(pDoc->m_dwSymbolUpdateMask);
	pDoc->m_dwSymbolUpdateMask = 0;
}


