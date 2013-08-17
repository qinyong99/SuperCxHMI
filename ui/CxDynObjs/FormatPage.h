// FormatPage.h : Declaration of the CFormatPage

#ifndef __FORMATPAGE_H_
#define __FORMATPAGE_H_

#include "resource.h"       // main symbols
#include "PropInfo.h"

EXTERN_C const CLSID CLSID_FormatPage;

/////////////////////////////////////////////////////////////////////////////
// CFormatPage
class ATL_NO_VTABLE CFormatPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFormatPage, &CLSID_FormatPage>,
	public IPropertyPageImpl<CFormatPage>,
	public CDialogImpl<CFormatPage>
{
public:
	CDynamicPropInfo* m_pPropInfo;
	
	CFormatPage() 
	{
		m_dwTitleID = IDS_TITLEFormatPage;
		m_dwHelpFileID = IDS_HELPFILEFormatPage;
		m_dwDocStringID = IDS_DOCSTRINGFormatPage;
		
		m_pPropInfo = NULL;
	}

	enum {IDD = IDD_FORMATPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_FORMATPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFormatPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CFormatPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CFormatPage>)
	COMMAND_HANDLER(IDC_ENABLEFORMAT, BN_CLICKED, OnClickedFormat)
	COMMAND_HANDLER(IDC_DATATYPE, CBN_SELCHANGE, OnSelchangeDataType)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_PARA_EXP1, BN_CLICKED, OnClickedExp1)
	COMMAND_HANDLER(IDC_PARA_EXP2, BN_CLICKED, OnClickedExp2)
END_MSG_MAP()

	STDMETHOD(SetObjects)(ULONG nObjects, IUnknown **ppUnk);
	STDMETHOD(Apply)(void);
	STDMETHOD(SetPageSite)(IPropertyPageSite *pPageSite);

	HRESULT CrackPropertyType();

	HRESULT HelperSetText(int nIDDlgItem, CComVariant &var);
	BOOL OnCreateExpression(BSTR* pbstr, int iFlag = 0);

	LRESULT OnClickedFormat(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelchangeDataType(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnClickedExp1(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnClickedExp2(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

#endif //__FORMATPAGE_H_
