// ButtonGenPage.h : Declaration of the CButtonGenPage

#ifndef __CXBUTTONGENPAGE_H_
#define __CXBUTTONGENPAGE_H_

#include "resource.h"       // main symbols

#include "CustCtrl.h"

EXTERN_C const CLSID CLSID_CxButtonGenPage;

class CBitmapButtonEx : public CBitmapButtonImpl<CBitmapButtonEx>
{
public:
	DECLARE_WND_SUPERCLASS(_T("WTL_BitmapButtonEx"), GetWndClassName())
		
	CBitmapButtonEx()
	{
	}
	
	void SetPicture(IPictureDisp* pPicture)
	{
		m_pPicture = 0;
		m_pPicture = pPicture;
		
		InvalidateRect(NULL);
	}
	
	void DoPaint(CDCHandle dc)
	{
		RECT rect;
		GetClientRect(&rect);
		
		if (DrawPicture(dc, rect, m_pPicture))
			return;
		
		::DrawEdge(dc, &rect, BDR_SUNKENOUTER, BF_RECT | BF_ADJUST);
		::FillRect(dc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	}
	
	CComPtr<IPictureDisp> m_pPicture;
};

/////////////////////////////////////////////////////////////////////////////
// CButtonGenPage
class ATL_NO_VTABLE CButtonGenPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CButtonGenPage, &CLSID_CxButtonGenPage>,
	public IPropertyPageImpl<CButtonGenPage>,
	public CDialogImpl<CButtonGenPage>
{
public:
	CButtonGenPage() 
	{
		m_dwTitleID = IDS_TITLECxButtonGenPage;
		m_dwHelpFileID = IDS_HELPFILECxButtonGenPage;
		m_dwDocStringID = IDS_DOCSTRINGCxButtonGenPage;
	}

	enum {IDD = IDD_CXBUTTONGENPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXBUTTONGENPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CButtonGenPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CButtonGenPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CButtonGenPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	NOTIFY_HANDLER(IDC_FORECOLOR, CPN_SELCHANGE, OnValueChanged)
	NOTIFY_HANDLER(IDC_BACKCOLOR, CPN_SELCHANGE, OnValueChanged)
	COMMAND_HANDLER(IDC_TEXTFONT_PICK, BN_CLICKED, OnClickedTextfontPick)
	COMMAND_HANDLER(IDC_CAPTION, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_NORMAL, BN_CLICKED, OnClickedNormal)
	COMMAND_HANDLER(IDC_HOVER, BN_CLICKED, OnClickedHover)
	COMMAND_HANDLER(IDC_PUSHED, BN_CLICKED, OnClickedPushed)
	COMMAND_HANDLER(IDC_DISABLED, BN_CLICKED, OnClickedDisabled)
	COMMAND_HANDLER(IDC_CLEARNORMAL, BN_CLICKED, OnClickedClearNormal)
	COMMAND_HANDLER(IDC_CLEARHOVER, BN_CLICKED, OnClickedClearHover)
	COMMAND_HANDLER(IDC_CLEARPUSHED, BN_CLICKED, OnClickedClearPushed)
	COMMAND_HANDLER(IDC_CLEARDISABLED, BN_CLICKED, OnClickedClearDisabled)
	REFLECT_NOTIFICATIONS()
END_MSG_MAP()
	// Handler prototypes:
	//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//	LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnSelchangeValue(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if (!m_bLoadPropery)
			m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
		
		return 0;
	}
	LRESULT OnValueChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
		return 0;
	}

	STDMETHOD(Apply)(void);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClickedNormal(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedHover(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedTextfontPick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedPushed(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedDisabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedClearHover(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedClearNormal(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedClearPushed(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedClearDisabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	void UpdateClearButtons()
	{
		m_btnNormal.SetPicture(m_pPictureNormal);
		m_btnHover.SetPicture(m_pPictureHover);
		m_btnPushed.SetPicture(m_pPicturePushed);
		m_btnDisabled.SetPicture(m_pPictureDisabled);
		
		::EnableWindow(GetDlgItem(IDC_CLEARNORMAL), m_pPictureNormal != NULL); 
		::EnableWindow(GetDlgItem(IDC_CLEARHOVER), m_pPictureHover != NULL); 
		::EnableWindow(GetDlgItem(IDC_CLEARPUSHED), m_pPicturePushed != NULL);
		::EnableWindow(GetDlgItem(IDC_CLEARDISABLED), m_pPictureDisabled != NULL); 
	}

	CColorButton m_btnForeColor;
	CColorButton m_btnBackColor;
	CEdit m_editCaption;

	CBitmapButtonEx m_btnNormal;
	CBitmapButtonEx m_btnHover;
	CBitmapButtonEx m_btnPushed;
	CBitmapButtonEx m_btnDisabled;

	CEdit m_editTextFont;
	CComPtr<IFontDisp> m_spTextFont;

	CComPtr<IPictureDisp> m_pPictureNormal;
	CComPtr<IPictureDisp> m_pPictureHover;
	CComPtr<IPictureDisp> m_pPicturePushed;
	CComPtr<IPictureDisp> m_pPictureDisabled;

	BOOL m_bLoadPropery;
};

#endif //__CXBUTTONGENPAGE_H_
