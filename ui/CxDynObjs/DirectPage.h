// DirectPage.h : Declaration of the CDirectPage

#ifndef __DIRECTPAGE_H_
#define __DIRECTPAGE_H_

#include "resource.h"       // main symbols
#include "PropInfo.h"

EXTERN_C const CLSID CLSID_DirectPage;

/////////////////////////////////////////////////////////////////////////////
// CDirectPage
class ATL_NO_VTABLE CDirectPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDirectPage, &CLSID_DirectPage>,
	public IPropertyPageImpl<CDirectPage>,
	public CDialogImpl<CDirectPage>
{
public:
	CDynamicPropInfo* m_pPropInfo;

	CDirectPage() 
	{
		m_dwTitleID = IDS_TITLEDirectPage;
		m_dwHelpFileID = IDS_HELPFILEDirectPage;
		m_dwDocStringID = IDS_DOCSTRINGDirectPage;

		m_pPropInfo = NULL;
	}

	enum {IDD = IDD_DIRECTPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_DIRECTPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDirectPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CDirectPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CDirectPage>)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	STDMETHOD(Apply)(void);
	STDMETHOD(SetObjects)(ULONG nObjects, IUnknown **ppUnk);
	STDMETHOD(SetPageSite)(IPropertyPageSite *pPageSite);

	HRESULT CrackPropertyType();
};

#endif //__DIRECTPAGE_H_
