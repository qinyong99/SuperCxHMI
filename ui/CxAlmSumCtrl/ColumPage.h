// ColumPage.h : Declaration of the CColumPage

#ifndef __CXCOLUMNALMSUMPAGE_H_
#define __CXCOLUMNALMSUMPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_CxColumnAlmSumPage;

/////////////////////////////////////////////////////////////////////////////
// CColumPage
class ATL_NO_VTABLE CColumPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CColumPage, &CLSID_CxColumnAlmSumPage>,
	public IPropertyPageImpl<CColumPage>,
	public CDialogImpl<CColumPage>
{
public:
	CColumPage() 
	{
		m_dwTitleID = IDS_TITLECxColumnAlmSumPage;
		m_dwHelpFileID = IDS_HELPFILECxColumnAlmSumPage;
		m_dwDocStringID = IDS_DOCSTRINGCxColumnAlmSumPage;

		m_bLoadPropery = TRUE;	
	}

	enum {IDD = IDD_CXCOLUMNALMSUMPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXCOLUMNALMSUMPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CColumPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CColumPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CColumPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_ADD, BN_CLICKED, OnClickedAdd)
	COMMAND_HANDLER(IDC_REMOVE, BN_CLICKED, OnClickedRemove)
	COMMAND_HANDLER(IDC_UP, BN_CLICKED, OnClickedUp)
	COMMAND_HANDLER(IDC_DOWN, BN_CLICKED, OnClickedDown)
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

	LRESULT OnValueChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
		return 0;
	}

	STDMETHOD(Apply)(void);
	
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedRemove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedUp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedDown(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
private:
	CListBox m_listAvailable;
	CListBox m_listSelected;

	BOOL m_bLoadPropery;
};

#endif //__CXCOLUMNALMSUMPAGE_H_
