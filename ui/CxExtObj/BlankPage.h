// BlankPage.h : Declaration of the CBlankPage

#ifndef __CXBLANKPAGE_H_
#define __CXBLANKPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_CxBlankPage;

/////////////////////////////////////////////////////////////////////////////
// CBlankPage
class ATL_NO_VTABLE CBlankPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBlankPage, &CLSID_CxBlankPage>,
	public IPropertyPageImpl<CBlankPage>,
	public CDialogImpl<CBlankPage>
{
public:
	CBlankPage() 
	{
		m_dwTitleID = IDS_TITLECxBlankPage;
		m_dwHelpFileID = IDS_HELPFILECxBlankPage;
		m_dwDocStringID = IDS_DOCSTRINGCxBlankPage;
	}

	enum {IDD = IDD_CXBLANKPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXBLANKPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBlankPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CBlankPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CBlankPage>)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	STDMETHOD(Apply)(void)
	{
		ATLTRACE(_T("CBlankPage::Apply\n"));
		for (UINT i = 0; i < m_nObjects; i++)
		{
			// Do something interesting here
			// ICircCtl* pCirc;
			// m_ppUnk[i]->QueryInterface(IID_ICircCtl, (void**)&pCirc);
			// pCirc->put_Caption(CComBSTR("something special"));
			// pCirc->Release();
		}
		m_bDirty = FALSE;
		return S_OK;
	}
};

#endif //__CXBLANKPAGE_H_
