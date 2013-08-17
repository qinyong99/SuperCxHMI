// ValueAxisPage.h: interface for the CValueAxisPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUEAXISPAGE_H__A8E910C0_08B4_4C04_956D_BC9CDE22CCF4__INCLUDED_)
#define AFX_VALUEAXISPAGE_H__A8E910C0_08B4_4C04_956D_BC9CDE22CCF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "CustCtrl.h"

class CValueAxisPage : 
	public IPropertyPageImpl<CValueAxisPage>,
	public CDialogImpl<CValueAxisPage>,
	public CWinDataExchange<CValueAxisPage>,
	public CComObjectRoot
{
public:
	CValueAxisPage() 
	{
		m_bLoadPropery = TRUE;	
	}

BEGIN_COM_MAP(CValueAxisPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_DDX_MAP(CValueAxisPage)
	DDX_FLOAT(IDC_MIN_VALUE, m_dMinVal)
	DDX_FLOAT(IDC_MAX_VALUE, m_dMaxVal)
END_DDX_MAP()

BEGIN_MSG_MAP(CValueAxisPage)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	NOTIFY_HANDLER(IDC_AXIS_COLOR, CPN_SELCHANGE, OnValueChanged)
	NOTIFY_HANDLER(IDC_TITLE_COLOR, CPN_SELCHANGE, OnValueChanged)
	COMMAND_HANDLER(IDC_TICKS_NUM, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_TITLE, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_WHOLEDIGITS, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_DECIMALDIGITS, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_AXIS_SHOW, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_TITLE_SHOW, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_TICKFONT_PICK, BN_CLICKED, OnClickedTickfontPick)
	COMMAND_HANDLER(IDC_TITLEFONT_PICK, BN_CLICKED, OnClickedTitlefontPick)
	COMMAND_HANDLER(IDC_MIN_VALUE, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_MAX_VALUE, EN_CHANGE, OnSelchangeValue)
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
	enum { IDD = IDD_VALUEAXIS_PAGE };

private:
	CColorButton m_btnAxisColor;
	CColorButton m_btnTitleColor;
	CEdit m_editTicksNum;
	CEdit m_editTitle;
	CButton m_btnShowAxis;
	CButton m_btnShowTitle;
	CEdit m_editTickFont;
	CComPtr<IFontDisp> m_spTickFont;
	CEdit m_editTitleFont;
	CComPtr<IFontDisp> m_spTitleFont;
	
	double m_dMinVal;
	double m_dMaxVal;

	BOOL m_bLoadPropery;
};

#endif // !defined(AFX_VALUEAXISPAGE_H__A8E910C0_08B4_4C04_956D_BC9CDE22CCF4__INCLUDED_)
