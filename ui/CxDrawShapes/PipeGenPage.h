// PipeGenPage.h : Declaration of the CPipeGenPage

#ifndef __CXPIPEGENPAGE_H_
#define __CXPIPEGENPAGE_H_

#include "resource.h"       // main symbols

#include "CustCtrl.h"

EXTERN_C const CLSID CLSID_CxPipeGenPage;

/////////////////////////////////////////////////////////////////////////////
// CPipeGenPage
class ATL_NO_VTABLE CPipeGenPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPipeGenPage, &CLSID_CxPipeGenPage>,
	public IPropertyPageImpl<CPipeGenPage>,
	public CDialogImpl<CPipeGenPage>
{
public:
	CPipeGenPage() 
	{
		m_dwTitleID = IDS_TITLECxPipeGenPage;
		m_dwHelpFileID = IDS_HELPFILECxPipeGenPage;
		m_dwDocStringID = IDS_DOCSTRINGCxPipeGenPage;
	}

	enum {IDD = IDD_CXPIPEGENPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXPIPEGENPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPipeGenPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CPipeGenPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CPipeGenPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	NOTIFY_HANDLER(IDC_LIGHT_COLOR, CPN_SELCHANGE, OnValueChanged)
	NOTIFY_HANDLER(IDC_DARK_COLOR, CPN_SELCHANGE, OnValueChanged)
	NOTIFY_HANDLER(IDC_LIQUID_COLOR, CPN_SELCHANGE, OnValueChanged)
	COMMAND_HANDLER(IDC_ROUND, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_FLOW, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_FLOW_DIRECTION, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_FLOW_SPEED, CBN_SELCHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_RADIUS, EN_CHANGE, OnSelchangeValue)
	REFLECT_NOTIFICATIONS()
END_MSG_MAP()
	
	LRESULT OnSelchangeValue(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if (!m_bLoadPropery)
			m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
		return 0;
	}
	LRESULT OnValueChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandle)
	{
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
		return 0;
	}

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	STDMETHOD(Apply)(void);

private:
	CEdit m_editRadius;
	CButton m_btnRound;
	CColorButton m_btnLightColor;
	CColorButton m_btnDarkColor;
	CButton m_btnFlow;
	CButton m_btnFlowDirection;
	CColorButton m_btnLiquidColor;
	CComboBox m_cmbFlowSpeed;
	
	BOOL m_bLoadPropery;
};

#endif //__CXPIPEGENPAGE_H_
