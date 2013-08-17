// AppearanceExpert.h : Declaration of the CAppearanceExpert

#ifndef __CXAPPEARANCEEXPERT_H_
#define __CXAPPEARANCEEXPERT_H_

#include "resource.h"       // main symbols
#include "ExpertPropertyPage.h"
#include "FillStyleWnd.h"
#include "FillPreviewWnd.h"
#include "DropPaletteWnd.h"
#include "LineStyleWnd.h"
#include "LinePreviewWnd.h"
#include "Slider.h"

class CSampleObject;

EXTERN_C const CLSID CLSID_CxAppearanceExpert;

/////////////////////////////////////////////////////////////////////////////
// CAppearanceExpert
class ATL_NO_VTABLE CAppearanceExpert :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAppearanceExpert, &CLSID_CxAppearanceExpert>,
	public CExpertPropertyPage<CAppearanceExpert>,
	public CDialogImpl<CAppearanceExpert>
{
public:
	CAppearanceExpert() 
	{
		m_dwTitleID = IDS_TITLECxAppearanceExpert;
		m_dwHelpFileID = IDS_HELPFILECxAppearanceExpert;
		m_dwDocStringID = IDS_DOCSTRINGCxAppearanceExpert;

		m_pSampleObject = NULL;
		m_dwColorPropDispID = DISPID_UNKNOWN;
	}

	enum {IDD = IDD_CXAPPEARANCEEXPERT};

DECLARE_REGISTRY_RESOURCEID(IDR_CXAPPEARANCEEXPERT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CAppearanceExpert) 
	COM_INTERFACE_ENTRY(IPropertyPage)
	COM_INTERFACE_ENTRY2(ICxExpertPage, IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CAppearanceExpert)
	CHAIN_MSG_MAP(IPropertyPageImpl<CAppearanceExpert>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
	MESSAGE_HANDLER(WM_SLIDERCHANGE, OnSliderChange)
	MESSAGE_HANDLER(WM_SELECTCOLOROK, OnSelectColor)
	COMMAND_HANDLER(IDC_COLORPROP, LBN_SELCHANGE, OnSelchangeColorProp)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSliderChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSelectColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSelchangeColorProp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void FillPropnameList(REFGUID guid, int nIndirect, CListBox& list);
	void SelectPropListItem(DISPID dispid);
	void GetColorPropValue(DISPID dispid, OLE_COLOR* pColor);

	void OnObjectChanged();
	void OnProertyChanged(DWORD dwDispID, void* pSender = NULL);
	void OnColorPropSelecting(void* pSender = NULL);
	void OnColorPropSelected(void* pSender = NULL);

	UINT m_nFillStyle;
	CFillStyleWnd m_wndFillStyle;
	CFillPreviewWnd m_wndFillPreview;
	CDropPaletteWnd m_wndColorPalette;
	CLineStyleWnd m_wndLineStyle;
	CSlider m_wndLineWidth;
	CLinePreviewWnd m_wndLinePreview;

	CSampleObject* m_pSampleObject;

	CListBox m_lbColorProp;
	DISPID m_dwColorPropDispID;
};

#endif //__CXAPPEARANCEEXPERT_H_
