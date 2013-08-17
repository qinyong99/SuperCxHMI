// VariableGenPage.h : Declaration of the CVariableGenPage

#ifndef __CXVARIABLEGENPAGE_H_
#define __CXVARIABLEGENPAGE_H_

#include "resource.h"       // main symbols

#include "../CxDynObjs/CxDynObjs.h"

EXTERN_C const CLSID CLSID_CxVariableGenPage;

/////////////////////////////////////////////////////////////////////////////
// CVariableGenPage
class ATL_NO_VTABLE CVariableGenPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVariableGenPage, &CLSID_CxVariableGenPage>,
	public IPropertyPageImpl<CVariableGenPage>,
	public CAxDialogImpl<CVariableGenPage>
{
public:
	CVariableGenPage() 
	{
		m_dwTitleID = IDS_TITLECxVariableGenPage;
		m_dwHelpFileID = IDS_HELPFILECxVariableGenPage;
		m_dwDocStringID = IDS_DOCSTRINGCxVariableGenPage;
	}

	enum {IDD = IDD_CXVARIABLEGENPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXVARIABLEGENPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CVariableGenPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CVariableGenPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CVariableGenPage>)
	COMMAND_HANDLER(IDC_CREATE_EXPRESSION, BN_CLICKED, OnClickedCreateExpression)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_VARTYPE, CBN_SELCHANGE, OnSelchangeVartype)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnSelchangeVartype(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCreateExpression(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	STDMETHOD(Apply)(void);

	void SyncValueType();

	CComPtr<IDirectDynamic> m_spDirectDynamic;

	CComboBox m_cmbVarType;
	CButton m_btnEvaluateSource;
	CEdit m_editName;
	CEdit m_editDesc;
};

#endif //__CXVARIABLEGENPAGE_H_
