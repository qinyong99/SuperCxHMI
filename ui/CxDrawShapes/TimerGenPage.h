// TimerGenPage.h : Declaration of the CTimerGenPage

#ifndef __CXTIMERGENPAGE_H_
#define __CXTIMERGENPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_CxTimerGenPage;

/////////////////////////////////////////////////////////////////////////////
// CTimerGenPage
class ATL_NO_VTABLE CTimerGenPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTimerGenPage, &CLSID_CxTimerGenPage>,
	public IPropertyPageImpl<CTimerGenPage>,
	public CDialogImpl<CTimerGenPage>
{
public:
	CTimerGenPage() 
	{
		m_dwTitleID = IDS_TITLECxTimerGenPage;
		m_dwHelpFileID = IDS_HELPFILECxTimerGenPage;
		m_dwDocStringID = IDS_DOCSTRINGCxTimerGenPage;
	}

	enum {IDD = IDD_CXTIMERGENPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXTIMERGENPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTimerGenPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CTimerGenPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CTimerGenPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_INTERVAL, EN_CHANGE, OnChangeInterval)
	COMMAND_HANDLER(IDC_ENABLED, BN_CLICKED, OnClickedEnabled)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnChangeInterval(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	void SetDirty();

	STDMETHOD(Apply)(void);

	BOOL m_bLoadPropery;
	CButton m_btnEnabled;
};

#endif //__CXTIMERGENPAGE_H_
