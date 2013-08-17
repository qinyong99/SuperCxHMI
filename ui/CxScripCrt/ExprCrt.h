// ExprCrt.h : Declaration of the CExprCrt

#ifndef __CXEXPRESSIONCREATOR_H_
#define __CXEXPRESSIONCREATOR_H_

#include "resource.h"       // main symbols
#include "SymbolView.h"
#include "KeyBoardDlg.h"
#include "TreePage.h"
#include "../CxTagPick/CxTagPick.h"

/////////////////////////////////////////////////////////////////////////////
// CExprCrt
class ATL_NO_VTABLE CExprCrt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CExprCrt, &CLSID_CxExpressionCreator>,
	public CAxDialogImpl<CExprCrt>,
	public IDispatchImpl<ICxExpressionCreator, &IID_ICxExpressionCreator, &LIBID_CXSCRIPCRTLib>,
	public IDispEventImpl<0, CExprCrt, &DIID__ICxTagPickCtrlEvents, &LIBID_CXTAGPICKLib, 1, 0>
{
public:
	CExprCrt() :
		m_winLeftPane(this, 1),
		m_winBottomPane(this, 2)
	{
		m_bUseLocalSetup = TRUE;
		m_bUseTagSymbol = TRUE;
		m_dwSymbolMask = -1;

		m_pTreePage = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CXEXPRESSIONCREATOR)

DECLARE_PROTECT_FINAL_CONSTRUCT()

enum { IDD = IDD_EXPR_CRT };

BEGIN_COM_MAP(CExprCrt)
	COM_INTERFACE_ENTRY(ICxExpressionCreator)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

BEGIN_MSG_MAP(CExprCrt)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_HANDLER(IDC_SHOW_SYMBOL, BN_CLICKED, OnClickedShowSymbol)
	COMMAND_HANDLER(IDC_CLEAR_EDIT, BN_CLICKED, OnClickedClear)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_SYMBOL_SELECT, OnSymbolSelect)
ALT_MSG_MAP(1)
	MESSAGE_HANDLER(WM_SIZE, OnLeftPaneSize)
ALT_MSG_MAP(2)
	MESSAGE_HANDLER(WM_SIZE, OnBottomPaneSize)
END_MSG_MAP()

	BEGIN_SINK_MAP(CExprCrt)
		SINK_ENTRY_INFO(0, DIID__ICxTagPickCtrlEvents, 1, OnChanged, NULL)
//		SINK_ENTRY(0, 1, OnChanged)
	END_SINK_MAP()

	HRESULT __stdcall OnChanged();

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnLeftPaneSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnBottomPaneSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSymbolSelect(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClickedShowSymbol(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedClear(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

// ICxExpressionCreator
public:
	STDMETHOD(ShowDialog)();
	STDMETHOD(get_UseLocalSetup)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_UseLocalSetup)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_Expression)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Expression)(/*[in]*/ BSTR newVal);
	STDMETHOD(AddGeneralSymbolPage)(DWORD dwSymbolMask);
	STDMETHOD(AddOPCServerSymbolPage)();
	STDMETHOD(AddSymbolTreePage)(BSTR bstrCaption, /*[out, retval]*/ ICxSymbolTreePage** ppPage);
	STDMETHOD(get_UseTagSymbol)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_UseTagSymbol)(/*[in]*/ BOOL newVal);
	STDMETHOD(GetMainSymbolTreePage)(/*[out, retval]*/ ICxSymbolTreePage** ppPage);

public:
	void DeflateWindow();
	void ReplaceLastSelect(LPCTSTR pszText);

	CAxWindow m_wndTagPickCtrl;
	CComPtr<IDispatch> m_spTagPickCtrlObject;

	CTreePage* m_pTreePage;

	CSymbolView m_viewSymbol;
	CEdit m_editExprssion;

	CPaneContainer m_paneExprssion;
	CPaneContainer m_paneLeft;
	CPaneContainer m_paneSymbol;
	CPaneContainer m_paneBottom;

	CContainedWindow m_winLeftPane;
	CContainedWindow m_winBottomPane;

	CKeyBoardDlg m_dlgKeyBoard;

	CSplitterWindow m_winSplitter;
	CHorSplitterWindow m_winHorSplitter;
	CSplitterWindow m_winBottomSplitter;
	
	BOOL m_bUseLocalSetup;
	CComBSTR m_bstrExpression;
	BOOL m_bUseTagSymbol;

	CString m_strLastSelectTag;
	DWORD m_dwSymbolMask;

	CImageList m_ImageList;
	
};

#endif //__CXEXPRESSIONCREATOR_H_
