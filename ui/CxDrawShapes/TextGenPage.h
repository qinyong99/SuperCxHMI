// TextGenPage.h : Declaration of the CTextGenPage

#ifndef __CXTEXTGENPAGE_H_
#define __CXTEXTGENPAGE_H_

#include "resource.h"       // main symbols
#include "FontComboBox.h"
#include "CustCtrl.h"

EXTERN_C const CLSID CLSID_CxTextGenPage;

/////////////////////////////////////////////////////////////////////////////
// CTextGenPage
class ATL_NO_VTABLE CTextGenPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTextGenPage, &CLSID_CxTextGenPage>,
	public IPropertyPageImpl<CTextGenPage>,
	public CDialogImpl<CTextGenPage>
{
public:
	CTextGenPage() 
	{
		m_dwTitleID = IDS_TITLECxTextGenPage;
		m_dwHelpFileID = IDS_HELPFILECxTextGenPage;
		m_dwDocStringID = IDS_DOCSTRINGCxTextGenPage;
	}

	enum {IDD = IDD_CXTEXTGENPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXTEXTGENPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTextGenPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()
	
BEGIN_MSG_MAP(CTextGenPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CTextGenPage>)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	NOTIFY_HANDLER(IDC_FONT_COLOR, CPN_SELCHANGE, OnValueChanged)
	COMMAND_HANDLER(IDC_FONT_NAME, CBN_SELCHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_FONT_STYLE, CBN_SELCHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_FONT_SIZE, CBN_SELCHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_STRIKE_THROUGH, BN_CLICKED, OnSelchangeValue)
	COMMAND_HANDLER(IDC_UNDER_LINE, BN_CLICKED, OnSelchangeValue)
	REFLECT_NOTIFICATIONS()
END_MSG_MAP()
	
	LRESULT OnSelchangeValue(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if (!m_bLoadPropery)
		{
			m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
			
			RECT rcText;
			m_SampleBox.GetWindowRect(&rcText);
			ScreenToClient(&rcText);
			InvalidateRect(&rcText);
		}
		return 0;
	}
	LRESULT OnValueChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandle)
	{
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
		
		RECT rcText;
		m_SampleBox.GetWindowRect(&rcText);
		ScreenToClient(&rcText);
		InvalidateRect(&rcText);
		return 0;
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnInitDialog(UINT, WPARAM wParam, LPARAM lParam, BOOL&);
	//	LRESULT OnSelchangeFont_name(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// Handler prototypes:
	//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	STDMETHOD(Apply)(void);

private:
	CFontComboBox m_cmbFontName;
	CComboBox m_cmbFontSize;
	CComboBox m_cmbFontStyle;
	CButton m_btnStrikeThrough;
	CButton m_btnUnderLine;
	
	CStatic m_SampleBox;
	
	CColorButton m_btnFontColor;
	
	BOOL m_bLoadPropery;
};

#endif //__CXTEXTGENPAGE_H_
