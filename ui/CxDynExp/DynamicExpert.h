// DynamicExpert.h : Declaration of the CDynamicExpert

#ifndef __CXDYNAMICEXPERT_H_
#define __CXDYNAMICEXPERT_H_

#include "resource.h"       // main symbols
#include "ExpertPropertyPage.h"
#include "TypeInfoBrowse.h"
#include "TabPage.h"

EXTERN_C const CLSID CLSID_CxDynamicExpert;

EXTERN_C const CLSID CLSID_LookupDynamic;
EXTERN_C const CLSID CLSID_LinearDynamic;
EXTERN_C const CLSID CLSID_FormatDynamic;
EXTERN_C const CLSID CLSID_DirectDynamic;

typedef CContainedWindowT<CTreeViewCtrl> CContainedTree;

#define NUM_PAGES 7

HRESULT HelperSetText(HWND hDlg, int nIDDlgItem, CComVariant &var);

/////////////////////////////////////////////////////////////////////////////
// CDynamicExpert
class ATL_NO_VTABLE CDynamicExpert :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDynamicExpert, &CLSID_CxDynamicExpert>,
	public CExpertPropertyPage<CDynamicExpert>,
	public CDialogImpl<CDynamicExpert>
{
public:
	CDynamicExpert() :
		m_treeModes(this, 1) 
	{
		m_dwTitleID = IDS_TITLECxDynamicExpert;
		m_dwHelpFileID = IDS_HELPFILECxDynamicExpert;
		m_dwDocStringID = IDS_DOCSTRINGCxDynamicExpert;

		m_pPropInfo = NULL;
		m_nCurPage = -1;

		if (m_pExpCrtExp == NULL)
			m_pExpCrtExp = this;

		m_pLastSelectProp = NULL;

		m_bSupportMultiObject = FALSE;
		m_nExtendPropFirst = -1;
	}

	enum {IDD = IDD_CXDYNAMICEXPERT};

DECLARE_REGISTRY_RESOURCEID(IDR_CXDYNAMICEXPERT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDynamicExpert) 
	COM_INTERFACE_ENTRY(IPropertyPage)
	COM_INTERFACE_ENTRY2(ICxExpertPage, IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CDynamicExpert)
	CHAIN_MSG_MAP(IPropertyPageImpl<CDynamicExpert>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_NEXT_OBJECT, BN_CLICKED, OnClickedNextObject)
	COMMAND_HANDLER(IDC_CREATE_EXPRESSION, BN_CLICKED, OnClickedCreateExpression)
	COMMAND_HANDLER(IDC_APPLY, BN_CLICKED, OnClickedApply)
	NOTIFY_HANDLER(IDC_MODES, TVN_ITEMEXPANDING, OnItemexpandingModes)
ALT_MSG_MAP(1)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDownModes)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDownModes)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDownModes)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedNextObject(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnLButtonDownModes(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDownModes(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);

	LRESULT OnItemexpandingModes(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnClickedCreateExpression(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	virtual void OnFinalMessage(HWND hWnd);

	static CDynamicExpert* m_pExpCrtExp;
	static void ExpressionCreatorProc(BSTR* pbstrExpression, int iFlag = 0);

	void OnObjectChanged();

	void SyncModePage();
	void SyncSouce();

	void PopulateTree();
	void EnsureExpandAdvanceNode(HTREEITEM hRootItem);
	void InsertAllPropNodes(HTREEITEM hRootItem);
	int FindProperty(DISPID dispid);
	int GetConnectState(DISPID dispid);

	HRESULT GetDynamic(BSTR bstrPropName, IUnknown** ppunkDyn);
	HRESULT PutDynamic(BSTR bstrPropName, IUnknown* punkDyn);
	HRESULT DeleteDynamic(BSTR bstrPropName);

	void OnPropertyChanging();
	void OnPropertyChanged();

	static void GetDynamicClassID(IUnknown* pDynUnk, CLSID* pClassID);

	BOOL GetSource(BSTR& bstrText);
	void SetSource(BSTR bstrText);
	void GetSelectProp(CPropInfo& prop);
	void InitDynamicPropInfo();

	CContainedTree m_treeModes;

	CPropInfoArray m_arrProps;
	int m_nExtendPropFirst;
	CString m_strExtendCat;

	CDynamicPropInfo* m_pPropInfo;

	CTabPage* m_apPages[NUM_PAGES];
	int m_nCurPage;

	void SelectAdvacedProp(DISPID dispid);
	void SelectLastSelectProp();

	CPropInfo* m_pLastSelectProp;
	CString m_strLastSelectCat;
	
};

#endif //__CXDYNAMICEXPERT_H_
