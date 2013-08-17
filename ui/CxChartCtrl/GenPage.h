// GenPage.h : Declaration of the CGenPage

#ifndef __CXGENTRENDPAGE_H_
#define __CXGENTRENDPAGE_H_

#include "resource.h"       // main symbols
#include "CustCtrl.h"

EXTERN_C const CLSID CLSID_CxGenChartPage;

/////////////////////////////////////////////////////////////////////////////
// CGenPage
class ATL_NO_VTABLE CGenPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGenPage, &CLSID_CxGenChartPage>,
	public IPropertyPageImpl<CGenPage>,
	public CDialogImpl<CGenPage>
{
public:
	CGenPage() 
	{
		m_dwTitleID = IDS_TITLECxGenTrendPage;
		m_dwHelpFileID = IDS_HELPFILECxGenTrendPage;
		m_dwDocStringID = IDS_DOCSTRINGCxGenTrendPage;

		m_bLoadPropery = TRUE;	
	}

	enum {IDD = IDD_CXGENTRENDPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXGENTRENDPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CGenPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CGenPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CGenPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	NOTIFY_HANDLER(IDC_FRAME_COLOR, CPN_SELCHANGE, OnValueChanged)
	COMMAND_HANDLER(IDC_FRAME_TYPE, CBN_SELCHANGE, OnSelchangeValue)
	NOTIFY_HANDLER(IDC_FORE_COLOR, CPN_SELCHANGE, OnValueChanged)
	NOTIFY_HANDLER(IDC_BACK_COLOR, CPN_SELCHANGE, OnValueChanged)
	COMMAND_RANGE_HANDLER(IDC_RADIO1, IDC_RADIO5, OnSelchangeValue)
	COMMAND_RANGE_HANDLER(IDC_RADIO6, IDC_RADIO7, OnSelchangeValue)
	COMMAND_HANDLER(IDC_TRANSPARENCY, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_SHOW_LEGEND, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_SHOW_MUTITIMEAXIS, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_SHOW_MUTIVALEAXIS, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_SHOW_MUTIGRID, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_SHOW_TOOLPANE, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_SHOW_LIST, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_CAPTION, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_REFRESH_INTERVAL, EN_CHANGE, OnSelchangeValue)
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

	LRESULT OnInitDialog(UINT, WPARAM wParam, LPARAM lParam, BOOL&);

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	
	STDMETHOD(Apply)(void);

private:
	CEdit m_editCaption;
	CEdit m_editRefreshInterval;
	CButton m_btnTransparency;
	CColorButton m_btnForeColor;
	CColorButton m_btnBackColor;
	CColorButton m_btnFrameColor;
	CButton m_btnShowLegend;
	CButton m_btnShowMutiTimeAxis;
	CButton m_btnShowMutiValueAxis;
	CButton m_btnShowMutiGrid;
	CButton m_btnShowToolPane;
	CButton m_btnShowList;
	CComboBox m_cmbFrameType;

	BOOL m_bLoadPropery;
};

#endif //__CXGENTRENDPAGE_H_
