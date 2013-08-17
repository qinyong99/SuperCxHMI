// GridPage.h: interface for the CGridPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDPAGE_H__0B3AFE0B_1025_4A37_874E_CF636A513D58__INCLUDED_)
#define AFX_GRIDPAGE_H__0B3AFE0B_1025_4A37_874E_CF636A513D58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "CustCtrl.h"

class CGridPage :
	public IPropertyPageImpl<CGridPage>,
	public CDialogImpl<CGridPage>,
	public CComObjectRoot
{
public:
	CGridPage() 
	{
	}

BEGIN_COM_MAP(CGridPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CGridPage)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	NOTIFY_HANDLER(IDC_HOR_COLOR, CPN_SELCHANGE, OnValueChanged)
	NOTIFY_HANDLER(IDC_VER_COLOR, CPN_SELCHANGE, OnValueChanged)
	COMMAND_HANDLER(IDC_HOR_SHOW, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_VER_SHOW, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_HOR_STYLE, CBN_SELCHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_VER_STYLE, CBN_SELCHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_HOR_NUM, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_VER_NUM, EN_CHANGE, OnSelchangeValue)
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


public:
	enum { IDD = IDD_GRID_PAGE };

	
private:
	CColorButton m_btnHorColor;
	CColorButton m_btnVerColor;
	CEdit m_editHorNum;
	CEdit m_editVerNum;
	CButton m_btnHorShow;
	CButton m_btnVerShow;
	CLineCombo m_cmbHorStyle;
	CLineCombo m_cmbVerStyle;

	BOOL m_bLoadPropery;
};

#endif // !defined(AFX_GRIDPAGE_H__0B3AFE0B_1025_4A37_874E_CF636A513D58__INCLUDED_)
