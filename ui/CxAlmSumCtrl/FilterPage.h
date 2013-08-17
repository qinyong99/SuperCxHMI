// FilterPage.h : Declaration of the CFilterPage

#ifndef __CXFILTERALMSUMPAGE_H_
#define __CXFILTERALMSUMPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_CxFilterAlmSumPage;

class CNetNode;

/////////////////////////////////////////////////////////////////////////////
// CFilterPage
class ATL_NO_VTABLE CFilterPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFilterPage, &CLSID_CxFilterAlmSumPage>,
	public IPropertyPageImpl<CFilterPage>,
	public CDialogImpl<CFilterPage>
{
public:
	CFilterPage() 
	{
		m_dwTitleID = IDS_TITLECxFilterAlmSumPage;
		m_dwHelpFileID = IDS_HELPFILECxFilterAlmSumPage;
		m_dwDocStringID = IDS_DOCSTRINGCxFilterAlmSumPage;

		m_bLoadPropery = TRUE;
		m_bRunMode = FALSE;
	}

	enum {IDD = IDD_CXFILTERALMSUMPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXFILTERALMSUMPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFilterPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CFilterPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CFilterPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	NOTIFY_HANDLER(IDC_AREA, LVN_ITEMCHANGED, OnItemchangedArea)
	COMMAND_HANDLER(IDC_HIGH, BN_CLICKED, OnClickedHigh)
	COMMAND_HANDLER(IDC_LOW, BN_CLICKED, OnClickedLow)
	COMMAND_HANDLER(IDC_MIDDLE, BN_CLICKED, OnClickedMiddle)
	NOTIFY_HANDLER(IDC_NODE, LVN_ITEMCHANGED, OnItemchangedNode)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	
	LRESULT OnItemchangedArea(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnClickedHigh(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedLow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedMiddle(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnItemchangedNode(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	STDMETHOD(Apply)(void);
	
	HRESULT EnumNetNodesFormSetup();
	HRESULT EnumLocalAlarmAreasFormSetup();

	HRESULT EnumNetNodes();
	HRESULT EnumAlarmAreas();

	HRESULT LoadNetNodeList();
	HRESULT LoadAreaList();

	CCheckListViewCtrl m_listNode;
	CCheckListViewCtrl m_listArea;

	CSimpleArray<CNetNode *> m_apNetNodes;
	CSimpleArray<CComBSTR> m_arrAlarmAreas;

	BOOL m_bLoadPropery;
	BOOL m_bRunMode;
	CComPtr<ICxDataSession> m_spDataSession;
};

#endif //__CXFILTERALMSUMPAGE_H_
