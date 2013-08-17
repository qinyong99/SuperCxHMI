// LinearPage.h : Declaration of the CLinearPage

#ifndef __LINEARPAGE_H_
#define __LINEARPAGE_H_

#include "resource.h"       // main symbols
#include "PropInfo.h"

EXTERN_C const CLSID CLSID_LinearPage;

/////////////////////////////////////////////////////////////////////////////
// CLinearPage
class ATL_NO_VTABLE CLinearPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLinearPage, &CLSID_LinearPage>,
	public IPropertyPageImpl<CLinearPage>,
	public CDialogImpl<CLinearPage>
{
public:
	CDynamicPropInfo* m_pPropInfo;

	CLinearPage() 
	{
		m_dwTitleID = IDS_TITLELinearPage;
		m_dwHelpFileID = IDS_HELPFILELinearPage;
		m_dwDocStringID = IDS_DOCSTRINGLinearPage;

		m_pPropInfo = NULL;
	}

	enum {IDD = IDD_LINEARPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_LINEARPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CLinearPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CLinearPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CLinearPage>)
	COMMAND_HANDLER(IDC_FETCHINPUT, BN_CLICKED, OnClickedFetchinput)
	COMMAND_RANGE_HANDLER(IDC_LOINVALUE_EXP, IDC_HIOUTVALUE_EXP, OnClickedExp)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	STDMETHOD(Apply)(void);
	STDMETHOD(SetObjects)(ULONG nObjects, IUnknown **ppUnk);
	STDMETHOD(SetPageSite)(IPropertyPageSite *pPageSite);

	HRESULT CrackPropertyType();
	HRESULT HelperSetText(int nIDDlgItem, CComVariant& var);
	LRESULT OnClickedFetchinput(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedExp(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
};

#endif //__LINEARPAGE_H_
