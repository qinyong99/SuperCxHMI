// ScriptEidtor.h : Declaration of the CScriptEditor

#ifndef __CXSCRIPTEIDTOR_H_
#define __CXSCRIPTEIDTOR_H_

#include "resource.h"       // main symbols
#include <atlctl.h>

#include "ScriptCreatorCP.h"
#include "ScriptEditorView.h"
#include "SymbolView.h"
#include "ComboBoxBold.h"
#include "RangeToFormat.h"

#define CHAIN_NOTIFYS_MEMBER(theChainMember) \
	{ \
		if(uMsg == WM_NOTIFY && theChainMember.ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult)) \
			return TRUE; \
	}

#define CHAIN_NOTIFYS_ALT_MEMBER(theChainMember, msgMapID) \
	{ \
		if(uMsg == WM_NOTIFY && theChainMember.ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, msgMapID)) \
			return TRUE; \
	}

/////////////////////////////////////////////////////////////////////////////
// ScriptEidtor
class ATL_NO_VTABLE CScriptEditor : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<ICxScriptEditor, &IID_ICxScriptEditor, &LIBID_CXSCRIPCRTLib>,
	public CComControl<CScriptEditor>,
	public IPersistStreamInitImpl<CScriptEditor>,
	public IOleControlImpl<CScriptEditor>,
	public IOleObjectImpl<CScriptEditor>,
	public IOleInPlaceActiveObjectImpl<CScriptEditor>,
	public IViewObjectExImpl<CScriptEditor>,
	public IOleInPlaceObjectWindowlessImpl<CScriptEditor>,
	public IConnectionPointContainerImpl<CScriptEditor>,
	public IPersistStorageImpl<CScriptEditor>,
	public ISpecifyPropertyPagesImpl<CScriptEditor>,
	public IQuickActivateImpl<CScriptEditor>,
	public IDataObjectImpl<CScriptEditor>,
	public IProvideClassInfo2Impl<&CLSID_CxScriptEditor, &DIID__ICxScriptCreatorEvents, &LIBID_CXSCRIPCRTLib>,
	public IPropertyNotifySinkCP<CScriptEditor>,
	public CComCoClass<CScriptEditor, &CLSID_CxScriptEditor>,
	public CProxy_ICxScriptCreatorEvents<CScriptEditor>
{
public:
	CScriptEditor();

DECLARE_REGISTRY_RESOURCEID(IDR_CXSCRIPTEDITOR)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CScriptEditor)
	COM_INTERFACE_ENTRY(ICxScriptEditor)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_PROP_MAP(CScriptEditor)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CScriptEditor)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
	CONNECTION_POINT_ENTRY(DIID__ICxScriptCreatorEvents)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CScriptEditor)
	CHAIN_MSG_MAP(CComControl<CScriptEditor>)
	DEFAULT_REFLECTION_HANDLER()
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
	COMMAND_ID_HANDLER(ID_PANE_CLOSE, OnPaneClose)
	MESSAGE_HANDLER(WM_SYMBOL_SELECT, OnSymbolSelect)
	NOTIFY_HANDLER(101, TCN_SELCHANGE, OnSymbolPageChanged)
	NOTIFY_HANDLER(102, SCN_UPDATEUI, OnUpdateUIScript)
	NOTIFY_HANDLER(102, SCN_SAVEPOINTLEFT, OnSavePointLeft)
	NOTIFY_HANDLER(102, SCN_MODIFIED, OnModified)
//	COMMAND_HANDLER(102, SCEN_CHANGE, OnChange)
	CHAIN_NOTIFYS_ALT_MEMBER(m_viewScript, 1)
ALT_MSG_MAP(1)
	MESSAGE_HANDLER(WM_SIZE, OnScriptPaneSize)
	MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
	COMMAND_HANDLER(103, CBN_DROPDOWN, OnDropdownObjects)
	COMMAND_HANDLER(103, CBN_SELCHANGE, OnSelchangeObjects)
	COMMAND_HANDLER(104, CBN_DROPDOWN, OnDropdownEvents)
	COMMAND_HANDLER(104, CBN_SELCHANGE, OnSelchangeEvents)
	CHAIN_MSG_MAP_ALT_MEMBER(m_cmbObjects, 0)
	CHAIN_MSG_MAP_ALT_MEMBER(m_cmbEvents, 0)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSymbolPageChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnSymbolSelect(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnPaneClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnScriptPaneSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnUpdateUIScript(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSavePointLeft(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnModified(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnDropdownObjects(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelchangeObjects(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDropdownEvents(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelchangeEvents(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	void LoadObjects();
	void LoadEvents();

	BOOL SelectDefaultEvent();

	void OnSelchangeEvents();

	void SyncObjectAndEvents();
	void ReAnalyzeText();

	BOOL PreTranslateAccelerator(LPMSG /*pMsg*/, HRESULT& /*hRet*/);

	void InsertFormSymbol(CTreeViewCtrl& ctrlTree);

// ICxScriptEidtor
public:
	STDMETHOD(GetCurrentSymbolTreePage)(/*[out, retval]*/ ICxSymbolTreePage** ppPage);
	STDMETHOD(AddSymbolTreePage)(BSTR bstrCaption, /*[out, retval]*/ ICxSymbolTreePage** ppPage);
	STDMETHOD(AddOPCServerSymbolPage)();
	STDMETHOD(AddGeneralSymbolPage)(DWORD dwSymbolMask);
	STDMETHOD(SetActive)(BOOL bActive);
	STDMETHOD(get_FindDialogHandle)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_SymbolViewVisible)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_SymbolViewVisible)(/*[in]*/ BOOL newVal);
	STDMETHOD(DeleteEmptyEventHandler)();
	STDMETHOD(EndUndoAction)();
	STDMETHOD(BeginUndoAction)();
	STDMETHOD(FormatRange)(BOOL bDraw, BOOL bUseStruct, long* plPos);
	STDMETHOD(GetRangeToFormatStruct)(ICxRangeToFormat** ppRangeToFormat);
	STDMETHOD(GetColumn)(long lPos, long* plCol);
	STDMETHOD(LineFromPosition)(long lPos, long* plLine);
	STDMETHOD(GetCurrentPos)(long* plPos);
	STDMETHOD(GetSelectionEnd)(long* plPos);
	STDMETHOD(GetSelectionStart)(long* plPos);
	STDMETHOD(GetSelText)(BSTR* pbstrText);
	STDMETHOD(GetLineCount)(long* plCount);
	STDMETHOD(GotoLine)(long lLine);
	STDMETHOD(AppendText)(long lLength, BSTR bstrText);
	STDMETHOD(ReplaceSel)(BSTR bstrText);
	STDMETHOD(SetSelectionEnd)(long lPos);
	STDMETHOD(SetSelectionStart)(long lPos);
	STDMETHOD(CanUndo)(BOOL* pbUndo);
	STDMETHOD(CanRedo)(BOOL* pbRedo);
	STDMETHOD(CanPaste)(BOOL* pbPaste);
	STDMETHOD(HasSearched)(BOOL* pbSearch);
	STDMETHOD(GetTextLength)(long* plCount);
	STDMETHOD(IsSelected)(BOOL* pbSel);
	STDMETHOD(EditObjectEventHandler)(BSTR bstrObject, BSTR bstrEvent, BOOL bCreateAndEdit);
	STDMETHOD(get_Text)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Text)(/*[in]*/ BSTR newVal);
	STDMETHOD(SetScriptEditorSite)(IUnknown* pSite);
	STDMETHOD(GetModify)(BOOL* pbModify);
	STDMETHOD(SetSavePoint)();
	STDMETHOD(EmptyUndoBuffer)();
	STDMETHOD(SelectAll)();
	STDMETHOD(BookmarkClearAll)();
	STDMETHOD(BookmarkPrevious)();
	STDMETHOD(BookmarkNext)();
	STDMETHOD(BookmarkToggle)();
	STDMETHOD(GoLine)();
	STDMETHOD(Replace)();
	STDMETHOD(FindRepeat)();
	STDMETHOD(Find)();
	STDMETHOD(Clear)();
	STDMETHOD(Paste)();
	STDMETHOD(Copy)();
	STDMETHOD(Cut)();
	STDMETHOD(Redo)();
	STDMETHOD(Undo)();

	BOOL m_bSetSplitterPos;
	CSplitterWindow m_winSplitter;
	CSymbolView m_viewSymbol;
	CScriptEditorView m_viewScript;
	
	CPaneContainer m_paneSymbol;
	CPaneContainer m_paneScript;
	CContainedWindow m_wndPaneScript;
	
	CComboBoxBold m_cmbObjects;
	CComboBoxBold m_cmbEvents;
	
	CButton m_btnViewSymbol;
	CImageList m_ImageList;

	CToolTipCtrl m_tip;

	CFont m_font;

	CComObjectCached<CRangeToFormat> m_objRF;

	int m_nLastLine;
	BOOL m_bChanged;
};

#endif //__CXSCRIPTEIDTOR_H_
