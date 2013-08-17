// SortPage.h : Declaration of the CSortPage

#ifndef __CXSORTALMSUMPAGE_H_
#define __CXSORTALMSUMPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_CxSortAlmSumPage;

/////////////////////////////////////////////////////////////////////////////
// CSortPage
class ATL_NO_VTABLE CSortPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSortPage, &CLSID_CxSortAlmSumPage>,
	public IPropertyPageImpl<CSortPage>,
	public CDialogImpl<CSortPage>
{
public:
	CSortPage() 
	{
		m_dwTitleID = IDS_TITLECxSortAlmSumPage;
		m_dwHelpFileID = IDS_HELPFILECxSortAlmSumPage;
		m_dwDocStringID = IDS_DOCSTRINGCxSortAlmSumPage;

		m_bLoadPropery = FALSE;
	}

	enum {IDD = IDD_CXSORTALMSUMPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXSORTALMSUMPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSortPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CSortPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CSortPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_RANGE_HANDLER(IDC_SORTASCENDING, IDC_SORTDESCENDING, OnSelchangeValue)
	COMMAND_HANDLER(IDC_SORTFIELD, CBN_SELCHANGE, OnSelchangeValue)
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

	LRESULT OnInitDialog(UINT, WPARAM wParam, LPARAM lParam, BOOL&);

	STDMETHOD(Apply)(void);
	
	CComboBox m_cmbSortField;
	BOOL m_bLoadPropery;
};

#endif //__CXSORTALMSUMPAGE_H_
