// FormatPage.h : Declaration of the CFormatPage

#ifndef __CXFORMATALMSUMPAGE_H_
#define __CXFORMATALMSUMPAGE_H_

#include "resource.h"       // main symbols
#include "Alarm.h"


EXTERN_C const CLSID CLSID_CxFormatAlmSumPage;

typedef CContainedWindowT<CListViewCtrl> CContainedListViewCtrl;

/////////////////////////////////////////////////////////////////////////////
// CFormatPage
class ATL_NO_VTABLE CFormatPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFormatPage, &CLSID_CxFormatAlmSumPage>,
	public IPropertyPageImpl<CFormatPage>,
	public CDialogImpl<CFormatPage>
{
public:
	CFormatPage() :
		m_listStatus(this, 1),
		m_listPriority(this, 2)
	{
		m_dwTitleID = IDS_TITLECxFormatAlmSumPage;
		m_dwHelpFileID = IDS_HELPFILECxFormatAlmSumPage;
		m_dwDocStringID = IDS_DOCSTRINGCxFormatAlmSumPage;
	}

	class CCell
	{
	public:
		CCell()
		{
			m_nRow = m_nCol = -1;
			m_bSelected = FALSE;
		}

		BOOL IsValidCell()
		{
			return (m_nRow >= 0 && m_nCol >= 0);
		}
		void SetValue(int nRow, int nCol)
		{
			m_bSelected = (m_nRow == nRow && m_nCol == nCol);
			m_nRow = nRow;
			m_nCol = nCol;
		}

		int m_nRow;
		int m_nCol;
		BOOL m_bSelected;
	};

	enum {IDD = IDD_CXFORMATALMSUMPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXFORMATALMSUMPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFormatPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CFormatPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CFormatPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
	NOTIFY_HANDLER(IDC_STATUS, NM_CLICK, OnClickListStatus)
	NOTIFY_HANDLER(IDC_STATUS, NM_DBLCLK, OnDblClickListStatus)
	NOTIFY_HANDLER(IDC_PRIORITY, NM_CLICK, OnClickListPriority)
	NOTIFY_HANDLER(IDC_PRIORITY, NM_DBLCLK, OnDblClickListPriority)
ALT_MSG_MAP(1) //listStatus
	MESSAGE_HANDLER(WM_LBUTTONDOWN,	OnListStatusLButtonDown)
ALT_MSG_MAP(2) //listPriority
	MESSAGE_HANDLER(WM_LBUTTONDOWN,	OnListPriorityLButtonDown)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickListStatus(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDblClickListStatus(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnClickListPriority(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDblClickListPriority(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnListStatusLButtonDown(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void DrawStatusListItem(LPDRAWITEMSTRUCT di);
	LRESULT OnListPriorityLButtonDown(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void DrawPriorityListItem(LPDRAWITEMSTRUCT di);


	STDMETHOD(Apply)(void);
	
private:
	CContainedListViewCtrl m_listStatus;
	CContainedListViewCtrl m_listPriority;

	CCell m_cellStatusListFocus;
	CCell m_cellPriorityListFocus;
	
	BOOL m_bLoadPropery;
};

#endif //__CXFORMATALMSUMPAGE_H_
