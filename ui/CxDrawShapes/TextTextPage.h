// TextTextPage.h : Declaration of the CTextTextPage

#ifndef __CXTEXTTEXTPAGE_H_
#define __CXTEXTTEXTPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_CxTextTextPage;

/////////////////////////////////////////////////////////////////////////////
// CTextTextPage
class ATL_NO_VTABLE CTextTextPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTextTextPage, &CLSID_CxTextTextPage>,
	public IPropertyPageImpl<CTextTextPage>,
	public CDialogImpl<CTextTextPage>
{
public:
	CTextTextPage() 
	{
		m_dwTitleID = IDS_TITLECxTextTextPage;
		m_dwHelpFileID = IDS_HELPFILECxTextTextPage;
		m_dwDocStringID = IDS_DOCSTRINGCxTextTextPage;
	}

	enum {IDD = IDD_CXTEXTTEXTPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXTEXTTEXTPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTextTextPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CTextTextPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CTextTextPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_TEXT, EN_CHANGE, OnSelchangeValue)
END_MSG_MAP()
	
	LRESULT OnSelchangeValue(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if (!m_bLoadPropery)
			m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
		
		return 0;
	}

	LRESULT OnInitDialog(UINT, WPARAM wParam, LPARAM lParam, BOOL&);
	STDMETHOD(Apply)(void);

	CEdit m_editText;
BOOL m_bLoadPropery;
};

#endif //__CXTEXTTEXTPAGE_H_
