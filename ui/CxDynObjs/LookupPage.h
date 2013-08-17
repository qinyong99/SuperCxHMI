// LookupPage.h : Declaration of the CLookupPage

#ifndef __LOOKUPPAGE_H_
#define __LOOKUPPAGE_H_

#include "resource.h"       // main symbols
#include "CxLookupItem.h"
#include "PropInfo.h"
#include "ItemEditDialog.h"
#include "SimpleArray2.h"

EXTERN_C const CLSID CLSID_LookupPage;


typedef CContainedWindowT<CListViewCtrl> CContainedList;
typedef CContainedWindowT<CEdit> CContainedEdit;

/////////////////////////////////////////////////////////////////////////////
// CLookupPage
class ATL_NO_VTABLE CLookupPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLookupPage, &CLSID_LookupPage>,
	public IPropertyPageImpl<CLookupPage>,
	public CDialogImpl<CLookupPage>
{
public:
	CLookupPage() :
		m_list(this, 1),
		 m_editInplace(this, 2)
	{
		m_dwTitleID = IDS_TITLELookupPage;
		m_dwHelpFileID = IDS_HELPFILELookupPage;
		m_dwDocStringID = IDS_DOCSTRINGLookupPage;

		m_nSelectColumn = -1;
		m_nEditItem = -1;
		m_nEditColumn = -1;
		
		m_bExactmatch = FALSE;

		m_pPropInfo = NULL;
	}

	enum {IDD = IDD_LOOKUPPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_LOOKUPPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CLookupPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CLookupPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CLookupPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
	MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
	COMMAND_HANDLER(IDC_EXACTMATCH, BN_CLICKED, OnClickedExactmatch)
	COMMAND_HANDLER(IDC_RANGECOMPARE, BN_CLICKED, OnClickedRangecompare)
	COMMAND_HANDLER(IDC_INSERT_ROW, BN_CLICKED, OnClickedInsertRow)
	COMMAND_HANDLER(IDC_DELETE_ROW, BN_CLICKED, OnClickedDeleteRow)
	COMMAND_HANDLER(IDC_EDIT_ROW, BN_CLICKED, OnClickedEditRow)
ALT_MSG_MAP(1) 
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDownList)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDownList)
	MESSAGE_HANDLER(WM_KEYDOWN,	OnKeyDownList)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCodeList)
ALT_MSG_MAP(2)		
	MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocusEdit)
	MESSAGE_HANDLER(WM_KEYDOWN,	OnKeyDownEdit)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCodeEdit)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void OnFinalMessage(HWND hWnd);

	LRESULT OnClickedExactmatch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedRangecompare(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedInsertRow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedDeleteRow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedEditRow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMeasureItem(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnLButtonDownList(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
//	LRESULT OnLButtonDblClkList(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetDlgCodeList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDownList(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnGetDlgCodeEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	
	LRESULT OnKeyDownEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocusEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

// IProertyPage
public:
	STDMETHOD(Apply)(void);
	STDMETHOD(SetObjects)(ULONG nObjects, IUnknown **ppUnk);
	STDMETHOD(SetPageSite)(IPropertyPageSite *pPageSite);

public:
	HRESULT CrackPropertyType();

	BOOL OnEditCell(int nItem, int nColumn, POINT* pPoint = NULL, BOOL bDblClick = FALSE);
	void LoadLevelItems();

	BOOL m_bExactmatch;
	
	CContainedList m_list;
	int m_nSelectColumn;
	int m_nEditItem;
	int m_nEditColumn;
	
	CContainedEdit m_editInplace;
	
	CFont m_font;
	CFont m_fontBold;
	CPen m_penGrid;
	
	CDynamicPropInfo* m_pPropInfo;
	
	CSimpleArray2<CLookupItem *> m_apItems;
};

#endif //__LOOKUPPAGE_H_
