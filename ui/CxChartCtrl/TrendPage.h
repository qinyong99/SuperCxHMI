// TrendPage.h : Declaration of the CTrendPage

#ifndef __CXDRAWTRENDPAGE_H_
#define __CXDRAWTRENDPAGE_H_

#include "resource.h"       // main symbols

#include "TagList.h"

EXTERN_C const CLSID CLSID_CxTrendCtrlPage;

#include "TabPage.h"
#include "PenPage.h"
#include "TimeAxisPage.h"
#include "ValueAxisPage.h"
#include "GridPage.h"
#include "LegendPage.h"

#define CPenTabPage CTabPageImpl<CPenPage>
#define CTimeAxisTabPage CTabPageImpl<CTimeAxisPage>
#define CValueAxisTabPage CTabPageImpl<CValueAxisPage>
#define CGridTabPage CTabPageImpl<CGridPage>
#define CLegendTabPage CTabPageImpl<CLegendPage>


/////////////////////////////////////////////////////////////////////////////
// CTrendPage
class ATL_NO_VTABLE CTrendPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTrendPage, &CLSID_CxTrendCtrlPage>,
	public IPropertyPageImpl<CTrendPage>,
	public CDialogImpl<CTrendPage>
{
public:
	CTrendPage() 
	{
		m_dwTitleID = IDS_TITLECxTrendCtrlPage;
		m_dwHelpFileID = IDS_HELPFILECxTrendCtrlPage;
		m_dwDocStringID = IDS_DOCSTRINGCxTrendCtrlPage;

		m_nCurPage = -1;
	}

	~CTrendPage()
	{
		for (int i = 0; i < m_apPages.GetSize(); i++)
		{
			delete m_apPages[i];
		}
	}

	enum {IDD = IDD_CXTRENDCTRLPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXTRENDCTRLPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTrendPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CTrendPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CTrendPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	NOTIFY_HANDLER(IDC_TAB, TCN_SELCHANGE, OnSelchangeTab)
	COMMAND_HANDLER(IDC_LIST_TAGS, LBN_SELCHANGE, OnSelchangeList_tags)
	MESSAGE_HANDLER(WM_VALUECHANGED, OnValueChanged)
	COMMAND_HANDLER(IDC_ADD, BN_CLICKED, OnClickedAdd)
	COMMAND_HANDLER(IDC_EDIT, BN_CLICKED, OnClickedEdit)
	COMMAND_HANDLER(IDC_REMOVE, BN_CLICKED, OnClickedRemove)
	MESSAGE_HANDLER(WM_TAGITEMCHANGED, OnTagItemChanged)
	REFLECT_NOTIFICATIONS()
END_MSG_MAP()

	LRESULT OnTagItemChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnValueChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
		return 0;
	}

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnClickedAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedRemove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	STDMETHOD(Apply)(void)
	{
		m_apPages[m_nCurPage]->ApplySerial();

		ATLTRACE(_T("CTrendPage::Apply\n"));
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

	LRESULT OnInitDialog(UINT, WPARAM wParam, LPARAM lParam, BOOL&);
	LRESULT OnSelchangeTab(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSelchangeList_tags(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	CTagList m_winTagList;
	CTabCtrl m_ctrlTab;

	CSimpleValArray<CTabPage *> m_apPages;
	int m_nCurPage;
};

#endif //__CXDRAWTRENDPAGE_H_
