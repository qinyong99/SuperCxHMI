// MenuGenPage.h : Declaration of the CMenuGenPage

#ifndef __CXMENUGENPAGE_H_
#define __CXMENUGENPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_CxMenuGenPage;

typedef CContainedWindowT<CTreeViewCtrl> CContainedTree;
typedef CContainedWindowT<CEdit> CContainedEdit;

/////////////////////////////////////////////////////////////////////////////
// CMenuGenPage
class ATL_NO_VTABLE CMenuGenPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMenuGenPage, &CLSID_CxMenuGenPage>,
	public IPropertyPageImpl<CMenuGenPage>,
	public CDialogImpl<CMenuGenPage>
{
public:
	CMenuGenPage() :
		m_tree(this, 1),
		m_editAccelKey(this, 2) 
	{
		m_dwTitleID = IDS_TITLECxMenuGenPage;
		m_dwHelpFileID = IDS_HELPFILECxMenuGenPage;
		m_dwDocStringID = IDS_DOCSTRINGCxMenuGenPage;

		m_bSetingCaption = FALSE;
	}

	class CItemInfo
	{
	public:
		CItemInfo()
		{
			m_dwState = 0;
			m_dwShiftState = 0;
			m_dwKeyCode = 0;
		}
		
		DWORD m_dwState;
		CString m_strID;
		DWORD m_dwShiftState;
		DWORD m_dwKeyCode;
	};

	enum {IDD = IDD_CXMENUGENPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXMENUGENPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMenuGenPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CMenuGenPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CMenuGenPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_UP, BN_CLICKED, OnClickedUp)
	COMMAND_HANDLER(IDC_DOWN, BN_CLICKED, OnClickedDown)
	COMMAND_HANDLER(IDC_LEFT, BN_CLICKED, OnClickedLeft)
	COMMAND_HANDLER(IDC_RIGHT, BN_CLICKED, OnClickedRight)
	COMMAND_HANDLER(IDC_INSERT, BN_CLICKED, OnClickedInsert)
	COMMAND_HANDLER(IDC_REMOVE, BN_CLICKED, OnClickedRemove)
	COMMAND_HANDLER(IDC_CAPTION, EN_CHANGE, OnChangeCaption)
	COMMAND_HANDLER(IDC_SEPARATOR, BN_CLICKED, OnClickedSeparator)
	COMMAND_HANDLER(IDC_CHECK, BN_CLICKED, OnClickedCheck)
	COMMAND_HANDLER(IDC_DISABLE, BN_CLICKED, OnClickedDisable)
	COMMAND_HANDLER(IDC_ACCEL_KEY_ENABLE, BN_CLICKED, OnClickedAccelKeyEnable)
	NOTIFY_HANDLER(IDC_TREE, TVN_SELCHANGED, OnSelchangedTree)
	NOTIFY_HANDLER(IDC_TREE, TVN_ENDLABELEDIT, OnEndlabeleditTree)
	COMMAND_HANDLER(IDC_NAME, EN_KILLFOCUS, OnKillfocusName)
	NOTIFY_HANDLER(IDC_TREE, TVN_DELETEITEM, OnDeleteitemTree)
ALT_MSG_MAP(1)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDownTree)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDownTree)
//	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDownTree)
ALT_MSG_MAP(2)
	MESSAGE_HANDLER(WM_KEYDOWN,	OnKeyDownAccelKey)
	MESSAGE_HANDLER(WM_CHAR, OnCharAccelKey)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedUp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedDown(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedLeft(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedRight(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedInsert(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedRemove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeCaption(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedSeparator(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedDisable(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedAccelKeyEnable(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSelchangedTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnEndlabeleditTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnLButtonDownTree(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
//	RESULT OnKeyDownModes(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnDeleteitemTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnKillfocusName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnKeyDownAccelKey(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCharAccelKey(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetDirty()
	{
		if (!m_bLoadPropery)
			m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
	}

	STDMETHOD(Apply)(void);

	void AddMenuItems(HTREEITEM hParentItem, IDrawMenu* pMenu, int& nIndex, int nCount, int nDepth);
	void AddMenuItems(HTREEITEM hItem, IDrawMenu* pMenu, int& nIndex, int nDepth);

	HTREEITEM CopyItemTo(HTREEITEM hItem, HTREEITEM hParent, HTREEITEM hInsertAfter);
	void MoveItemTo(HTREEITEM hItem, HTREEITEM hParent, HTREEITEM hInsertAfter);

	CButton m_btnButtonDisplayed;
	CContainedEdit m_editAccelKey;
	CEdit m_editButtonCaption;
	CContainedTree m_tree; 
	CEdit m_editID;
	
	CButton m_btnSeparator;
	CButton m_btnCheck;
	CButton m_btnDisable;
	CButton m_btnAccelKeyEnable;

	BOOL m_bSetingCaption;
	BOOL m_bLoadPropery;

	CImageList m_ImageList;
};

#endif //__CXMENUGENPAGE_H_
