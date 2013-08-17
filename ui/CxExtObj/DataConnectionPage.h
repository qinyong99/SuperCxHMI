// DataConnectionPage.h : Declaration of the CDataConnectionPage

#ifndef __CXDATACONNECTIONPAGE_H_
#define __CXDATACONNECTIONPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_CxDataConnectionPage;

/////////////////////////////////////////////////////////////////////////////
// CDataConnectionPage
class ATL_NO_VTABLE CDataConnectionPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDataConnectionPage, &CLSID_CxDataConnectionPage>,
	public IPropertyPageImpl2<CDataConnectionPage>,
	public CDialogImpl<CDataConnectionPage>
{
public:
	CDataConnectionPage() 
	{
		m_dwTitleID = IDS_TITLECxDataConnectionPage;
		m_dwHelpFileID = IDS_HELPFILECxDataConnectionPage;
		m_dwDocStringID = IDS_DOCSTRINGCxDataConnectionPage;
	}

	enum {IDD = IDD_CXDATACONNECTIONPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXDATACONNECTIONPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDataConnectionPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CDataConnectionPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CDataConnectionPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_BROWSE, BN_CLICKED, OnClickedBrowse)
	COMMAND_HANDLER(IDC_BUILDE, BN_CLICKED, OnClickedBuilder)
	COMMAND_HANDLER(IDC_RADIO1, BN_CLICKED, OnClickedRadio1)
	COMMAND_HANDLER(IDC_RADIO2, BN_CLICKED, OnClickedRadio2)
	COMMAND_HANDLER(IDC_RADIO3, BN_CLICKED, OnClickedRadio3)
	COMMAND_HANDLER(IDC_RADIO4, BN_CLICKED, OnClickedRadio4)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	STDMETHOD(Apply)();
	
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClickedBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedBuilder(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnClickedRadio1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedRadio2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedRadio3(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedRadio4(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	HRESULT PromptSource(LPOLESTR* ppwszInitString);
	HRESULT PromptSourceFileName(LPOLESTR* ppwszSelectedFile);

	void UpdateWindowsState();
};

#endif //__CXDATACONNECTIONPAGE_H_
