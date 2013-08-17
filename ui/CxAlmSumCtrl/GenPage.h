// GenPage.h : Declaration of the CGenPage

#ifndef __CXGENALMSUMPAGE_H_
#define __CXGENALMSUMPAGE_H_

#include "resource.h"       // main symbols
#include "CustCtrl.h"

EXTERN_C const CLSID CLSID_CxGenAlmSumPage;

/////////////////////////////////////////////////////////////////////////////
// CGenPage
class ATL_NO_VTABLE CGenPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGenPage, &CLSID_CxGenAlmSumPage>,
	public IPropertyPageImpl<CGenPage>,
	public CDialogImpl<CGenPage>
{
public:
	CGenPage() 
	{
		m_dwTitleID = IDS_TITLECxGenAlmSumPage;
		m_dwHelpFileID = IDS_HELPFILECxGenAlmSumPage;
		m_dwDocStringID = IDS_DOCSTRINGCxGenAlmSumPage;

		m_bLoadPropery = TRUE;	
	}

	enum {IDD = IDD_CXGENALMSUMPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXGENALMSUMPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CGenPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CGenPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CGenPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_ROW_HEIGHT, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_HEADER, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_SCROOLBAR, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_GRID, BN_CLICKED, OnSelchangeValue)
	NOTIFY_HANDLER(IDC_BACKCOLOR, CPN_SELCHANGE, OnValueChanged)
	NOTIFY_HANDLER(IDC_HEADER_FORECOLOR, CPN_SELCHANGE, OnValueChanged)
	NOTIFY_HANDLER(IDC_HEADER_BACKCOLOR, CPN_SELCHANGE, OnValueChanged)
	COMMAND_HANDLER(IDC_HEADERFONT_PICK, BN_CLICKED, OnClickedHeaderfontPick)
	REFLECT_NOTIFICATIONS()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnValueChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandle)
	{
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
		return 0;
	}
	LRESULT OnSelchangeValue(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if (!m_bLoadPropery)
			m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
		return 0;
	}

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	STDMETHOD(Apply)(void);

	LRESULT OnClickedHeaderfontPick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


private:
	CColorButton m_btnBackColor;
	CColorButton m_btnHeaderForeColor;
	CColorButton m_btnHeaderBackColor;
		
	CButton m_btnShowHeader;
	CButton m_btnShowScrollBar;
	CButton m_btnShowGrid;

	CEdit m_editHeaderFont;
	CComPtr<IFontDisp> m_spHeaderFont;

	BOOL m_bLoadPropery;
};

#endif //__CXGENALMSUMPAGE_H_
