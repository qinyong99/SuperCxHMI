// EventExpert.h : Declaration of the CEventExpert

#ifndef __CXEVENTEXPERT_H_
#define __CXEVENTEXPERT_H_

#include "resource.h"       // main symbols
#include "ExpertPropertyPage.h"
#include "TabPage.h"
#include "ShowPictureDlg.h"
#include "DownloadRecipeDlg.h"
#include "DataEntryDlg.h"
#include "SimpleArray2.h"

EXTERN_C const CLSID CLSID_CxEventExpert;

#define CShowPicturePage CTabPageImpl<CShowPictureDlg>
#define CDownloadRecipePage CTabPageImpl<CDownloadRecipeDlg>
#define CDataEntryPage CTabPageImpl<CDataEntryDlg>

#define NUM_PAGES 5

typedef CContainedWindowT<CListViewCtrl> CContainedList;

class CEventInfo
{
public:
	CString m_strName;
	CString m_strDescription;

public:
	CEventInfo()
	{

	}
	
	CEventInfo(const CEventInfo& eventinfo)
	{
		InternalCopy(eventinfo);
	}
	
	CEventInfo& operator = (CEventInfo& eventinfo)
	{
		InternalCopy(eventinfo);
		return *this;
	}
	
	void InternalCopy(const CEventInfo& eventinfo)
	{
		m_strName = eventinfo.m_strName;
		m_strDescription = eventinfo.m_strDescription;
	}
};

typedef CSimpleArray2<CEventInfo> CEventInfoArray;

/////////////////////////////////////////////////////////////////////////////
// CEventExpert
class ATL_NO_VTABLE CEventExpert :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEventExpert, &CLSID_CxEventExpert>,
	public CExpertPropertyPage<CEventExpert>,
	public CDialogImpl<CEventExpert>
{
public:
	CEventExpert() :
		m_listEvents(this, 1)
	{
		m_dwTitleID = IDS_TITLECxEventExpert;
		m_dwHelpFileID = IDS_HELPFILECxEventExpert;
		m_dwDocStringID = IDS_DOCSTRINGCxEventExpert;
	}

	enum {IDD = IDD_CXEVENTEXPERT};

DECLARE_REGISTRY_RESOURCEID(IDR_CXEVENTEXPERT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CEventExpert) 
	COM_INTERFACE_ENTRY(IPropertyPage)
	COM_INTERFACE_ENTRY2(ICxExpertPage, IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CEventExpert)
	CHAIN_MSG_MAP(IPropertyPageImpl<CEventExpert>)
	COMMAND_HANDLER(IDC_NEXT_OBJECT, BN_CLICKED, OnClickedNextObject)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_APPLY, BN_CLICKED, OnClickedApply)
	COMMAND_HANDLER(IDC_ACTIONS, CBN_SELCHANGE, OnSelchangeActions)
	COMMAND_HANDLER(IDC_SHOW_HAND, BN_CLICKED, OnClickedShowHand)
ALT_MSG_MAP(1)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDownEvents)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDownEvents)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDownEvents)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedNextObject(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelchangeActions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnLButtonDownEvents(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDownEvents(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
	
	LRESULT OnClickedShowHand(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	virtual void OnFinalMessage(HWND hWnd);

	STDMETHOD(Apply)(void);

	STDMETHOD(IsSupportMultiObject)();

	void OnObjectChanged();

	void DisplayShowHandState();
	void LoadEventsList();

	void GetObjectEventParams(LPUNKNOWN pUnkObject, CStringArray& arrParams, LPCTSTR lpszEvent);
	void GetObjectEvents(LPUNKNOWN pUnkObject, CEventInfoArray& arrInfos, int& nDefaultIndex);

	void SyncEventChange();
	void SyncActionPage();

	void OnActionScriptChanged();
	void UpdateActionScript(LPCTSTR lpszAction, LPCTSTR lpszScript);

	static CEventInfoArray* m_parrSortEvents;
	static int CALLBACK CompareItems(int nItem1, int nItem2, LPARAM nCol);

	CComboBox m_cmbActions;
	CContainedList m_listEvents;
	CButton m_btnShowHand;

	CTabPage* m_apPages[NUM_PAGES];

	CString m_strActionScript; // ¶¯×÷½Å±¾

	CEventInfoArray m_arrEvents;

	CString m_strLastSelectEvent;
};

#endif //__CXEVENTEXPERT_H_
