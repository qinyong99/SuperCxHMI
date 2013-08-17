// LegendPage.h: interface for the CLegendPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEGENDPAGE_H__A767A27C_E232_4509_B6E4_834D6E10E6D6__INCLUDED_)
#define AFX_LEGENDPAGE_H__A767A27C_E232_4509_B6E4_834D6E10E6D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "CustCtrl.h"

class CLegendPage :
	public IPropertyPageImpl<CLegendPage>,
	public CDialogImpl<CLegendPage>,
	public CComObjectRoot
{
public:
	CLegendPage() 
	{
		m_bLoadPropery = TRUE;		
	}

BEGIN_COM_MAP(CLegendPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CLegendPage)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_SHOW, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_DESC, EN_CHANGE, OnSelchangeValue)
	NOTIFY_HANDLER(IDC_TEXT_COLOR, CPN_SELCHANGE, OnValueChanged)
	COMMAND_HANDLER(IDC_TEXTFONT_PICK, BN_CLICKED, OnClickedTextfontPick)
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

	LRESULT OnClickedTextfontPick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


public:
	enum { IDD = IDD_LEGEND_PAGE };

	
private:
	CButton m_btnShow;
	CEdit m_editDesc;
	CColorButton m_btnTextColor;
	CEdit m_editTextFont;
	CComPtr<IFontDisp> m_spTextFont;

	BOOL m_bLoadPropery;
};

#endif // !defined(AFX_LEGENDPAGE_H__A767A27C_E232_4509_B6E4_834D6E10E6D6__INCLUDED_)
