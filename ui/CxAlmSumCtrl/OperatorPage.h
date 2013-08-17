// OperatorAPage.h : Declaration of the COperatorPage

#ifndef __CXOPERATORALMSUMPAGE_H_
#define __CXOPERATORALMSUMPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_CxOperatorAlmSumPage;

/////////////////////////////////////////////////////////////////////////////
// COperatorPage
class ATL_NO_VTABLE COperatorPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<COperatorPage, &CLSID_CxOperatorAlmSumPage>,
	public IPropertyPageImpl<COperatorPage>,
	public CDialogImpl<COperatorPage>
{
public:
	COperatorPage() 
	{
		m_dwTitleID = IDS_TITLECxOperatorAlmSumPage;
		m_dwHelpFileID = IDS_HELPFILECxOperatorAlmSumPage;
		m_dwDocStringID = IDS_DOCSTRINGCxOperatorAlmSumPage;

		m_bLoadPropery = TRUE;
	}

	enum {IDD = IDD_CXOPERATORALMSUMPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXOPERATORALMSUMPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(COperatorPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(COperatorPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<COperatorPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_ACK, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_CONTEXT_MENU, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_ONLINE_CONFIGURE, BN_CLICKED, OnSelchangeValue)
END_MSG_MAP()
	// Handler prototypes:
	//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	
	LRESULT OnSelchangeValue(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if (!m_bLoadPropery)
			m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
		return 0;
	}

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	STDMETHOD(Apply)(void);

private:
	CButton m_btnAck;
	CButton m_btnContextMenu;
	CButton m_btnOnlineConfigure;
	
	BOOL m_bLoadPropery;
};

#endif //__CXOPERATORALMSUMPAGE_H_
