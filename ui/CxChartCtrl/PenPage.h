// PenPage.h: interface for the CPenPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PENPAGE_H__FD10C974_380A_4A09_9D12_3B658066DC30__INCLUDED_)
#define AFX_PENPAGE_H__FD10C974_380A_4A09_9D12_3B658066DC30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "CustCtrl.h"

class CPenPage : 
	public IPropertyPageImpl<CPenPage>,
	public CDialogImpl<CPenPage>,
	public CWinDataExchange<CPenPage>,
	public CComObjectRoot
{
public:
	CPenPage() 
	{
		m_bLoadPropery = TRUE;
	}

BEGIN_COM_MAP(CPenPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_DDX_MAP(CPenPage)
	DDX_FLOAT(IDC_MIN_VALUE_LIMIT, m_dMinValLimit)
	DDX_FLOAT(IDC_MAX_VALUE_LIMIT, m_dMaxValLimit)
END_DDX_MAP()

BEGIN_MSG_MAP(CPenPage)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	NOTIFY_HANDLER(IDC_LINE_COLOR, CPN_SELCHANGE, OnValueChanged)
	COMMAND_HANDLER(IDC_LINE_STYLE, CBN_SELCHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_MARKER_STYLE, CBN_SELCHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_LINE_WIDTH, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_MAX_VALUE, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_MIN_VALUE, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_POINTS_NUM, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_SHOW, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_MIN_VALUE_LIMIT, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_MAX_VALUE_LIMIT, EN_CHANGE, OnSelchangeValue)
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
	enum { IDD = IDD_PEN_PAGE };

	
private:
	CLineCombo m_cmbLineStyle;
	CMarkerCombo m_cmbMarkerStyle;
	CColorButton m_btnLineColor;
	CEdit m_editLineWidth;
	CEdit m_editPointsNum;
	CButton m_btnShow;

	double m_dMinValLimit;
	double m_dMaxValLimit;
	
	BOOL m_bLoadPropery;
};


#endif // !defined(AFX_PENPAGE_H__FD10C974_380A_4A09_9D12_3B658066DC30__INCLUDED_)
