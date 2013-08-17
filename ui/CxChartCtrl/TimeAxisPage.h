// TimeAxisPage.h: interface for the CTimeAxisPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEAXISPAGE_H__8777C3D4_B4C0_4C78_AD36_02316A7CFF04__INCLUDED_)
#define AFX_TIMEAXISPAGE_H__8777C3D4_B4C0_4C78_AD36_02316A7CFF04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "CustCtrl.h"

class CTimeAxisPage : 
	public IPropertyPageImpl<CTimeAxisPage>,
	public CDialogImpl<CTimeAxisPage>,
	public CComObjectRoot
{
public:
	CTimeAxisPage() 
	{
		m_bLoadPropery = TRUE;	
	}

BEGIN_COM_MAP(CTimeAxisPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CTimeAxisPage)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	NOTIFY_HANDLER(IDC_AXIS_COLOR, CPN_SELCHANGE, OnValueChanged)
	NOTIFY_HANDLER(IDC_TITLE_COLOR, CPN_SELCHANGE, OnValueChanged)
	COMMAND_HANDLER(IDC_TICKS_NUM, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_TITLE, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_AXIS_SHOW, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_TITLE_SHOW, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_FORMAT, CBN_SELCHANGE, OnSelchangeValue)
	COMMAND_RANGE_HANDLER(IDC_DAY, IDC_SECOND, OnSelchangeValue);
	COMMAND_HANDLER(IDC_TICKFONT_PICK, BN_CLICKED, OnClickedTickfontPick)
	COMMAND_HANDLER(IDC_TITLEFONT_PICK, BN_CLICKED, OnClickedTitlefontPick)
	REFLECT_NOTIFICATIONS()
END_MSG_MAP()

	LRESULT OnSelchangeValue(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if (!m_bLoadPropery)
			SendMessage(GetParent(), WM_VALUECHANGED, 0, 0);
		return 0;
	}
	
	LRESULT OnValueChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandle)
	{
		SendMessage(GetParent(), WM_VALUECHANGED, 0, 0);
		return 0;
	}
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	STDMETHOD(SetObjects)(ULONG nObjects, IUnknown **ppUnk);
	STDMETHOD(Apply)(void);

	LRESULT OnClickedTickfontPick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedTitlefontPick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	enum { IDD = IDD_TIMEAXIS_PAGE };

private:
	CColorButton m_btnAxisColor;
	CColorButton m_btnTitleColor;
	CEdit m_editTicksNum;
	CEdit m_editTitle;
	CButton m_btnShowAxis;
	CButton m_btnShowTitle;
	CComboBox m_cbFormat;
	CEdit m_editTickFont;
	CComPtr<IFontDisp> m_spTickFont;
	CEdit m_editTitleFont;
	CComPtr<IFontDisp> m_spTitleFont;

	BOOL m_bLoadPropery;
};


#endif // !defined(AFX_PENPAGE_H__FD10C974_380A_4A09_9D12_3B658066DC30__INCLUDED_)
