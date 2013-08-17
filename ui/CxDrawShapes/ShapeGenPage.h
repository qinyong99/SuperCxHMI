// ShapeGenPage.h : Declaration of the CShapeGenPage

#ifndef __CXSHAPEGENPAGE_H_
#define __CXSHAPEGENPAGE_H_

#include "resource.h"       // main symbols

#include "CustCtrl.h"

EXTERN_C const CLSID CLSID_CxShapeGenPage;

/////////////////////////////////////////////////////////////////////////////
// CShapeGenPage
class ATL_NO_VTABLE CShapeGenPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CShapeGenPage, &CLSID_CxShapeGenPage>,
	public IPropertyPageImpl<CShapeGenPage>,
	public CDialogImpl<CShapeGenPage>
{
public:
	CShapeGenPage() 
	{
		m_dwTitleID = IDS_TITLECxShapeGenPage;
		m_dwHelpFileID = IDS_HELPFILECxShapeGenPage;
		m_dwDocStringID = IDS_DOCSTRINGCxShapeGenPage;
	}

	enum {IDD = IDD_CXSHAPEGENPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXSHAPEGENPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CShapeGenPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CShapeGenPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CShapeGenPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	NOTIFY_HANDLER(IDC_FILLCOLOR, CPN_SELCHANGE, OnValueChanged)
	NOTIFY_HANDLER(IDC_EDGECOLOR, CPN_SELCHANGE, OnValueChanged)
	NOTIFY_HANDLER(IDC_HIGHLIGHT_FILLCOLOR, CPN_SELCHANGE, OnValueChanged)
	NOTIFY_HANDLER(IDC_HIGHLIGHT_EDGECOLOR, CPN_SELCHANGE, OnValueChanged)
	NOTIFY_HANDLER(IDC_HORFILLSPIN, UDN_DELTAPOS, OnHorFillSpin)
	NOTIFY_HANDLER(IDC_VERFILLSPIN, UDN_DELTAPOS, OnVerFillSpin)
	NOTIFY_HANDLER(IDC_EDGEWIDTHSPIN, UDN_DELTAPOS, OnEdgeWidthSpin)
	COMMAND_HANDLER(IDC_EDGETYPE, CBN_SELCHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_FILLTYPE, CBN_SELCHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_HORFILLDIR, CBN_SELCHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_HORFILLPER, EN_CHANGE, OnSelchangeValue)
	COMMAND_HANDLER(IDC_VERFILLDIR, CBN_CLOSEUP, OnSelchangeValue)
	COMMAND_HANDLER(IDC_VERFILLPER, EN_CHANGE, OnSelchangeValue)
	REFLECT_NOTIFICATIONS()
END_MSG_MAP()

	LRESULT OnHorFillSpin(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnVerFillSpin(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnEdgeWidthSpin(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	CColorButton m_btnEdgeColor;
	CColorButton m_btnHighLightEdgeColor;
	CColorButton m_btnFillColor;
	CColorButton m_btnHighLightFillColor;

	CIconCombo m_cmbHorDirection;
	CIconCombo m_cmbVerDirection;

	COwnerDrawCombo m_cmbFillStyle;
	COwnerDrawCombo m_cmbEdgeStyle;

	CRestrictEdit m_editHorizontalFillPercent;
	CRestrictEdit m_editVerticalFillPercent;
	CRestrictEdit m_editEdgeWidth;

	BOOL m_bLoadPropery;

	STDMETHOD(Apply)(void)
	{
		ATLTRACE(_T("CShapeGenPage::Apply\n"));
		for (UINT i = 0; i < m_nObjects; i++)
		{
			CComVariant value;
			CComDispatchDriver dd(m_ppUnk[i]);

			value = (long)m_btnEdgeColor.GetColor();
			dd.PutPropertyByName(L"EdgeColor", &value);
			value = (long)m_btnHighLightEdgeColor.GetColor();
			dd.PutPropertyByName(L"HighLightEdgeColor", &value);
			value = (long)m_btnFillColor.GetColor();
			dd.PutPropertyByName(L"FillColor", &value);
			value = (long)m_btnHighLightFillColor.GetColor();
			dd.PutPropertyByName(L"HighLightFillColor", &value);

			value = m_cmbHorDirection.GetCurSel();
			dd.PutPropertyByName(L"HorizontalFillDirection", &value);
			value = m_cmbVerDirection.GetCurSel();
			dd.PutPropertyByName(L"VerticalFillDirection", &value);

			value = m_cmbFillStyle.GetCurSel();
			dd.PutPropertyByName(L"FillStyle", &value);
			value = m_cmbEdgeStyle.GetCurSel();
			dd.PutPropertyByName(L"EdgeStyle", &value);

			value = m_editHorizontalFillPercent.GetText();
			dd.PutPropertyByName(L"HorizontalFillPercent", &value);
			value = m_editVerticalFillPercent.GetText();
			dd.PutPropertyByName(L"VerticalFillPercent", &value);
			value = m_editEdgeWidth.GetText();
			dd.PutPropertyByName(L"EdgeWidth", &value);
		}
		m_bDirty = FALSE;
		return S_OK;
	}
	LRESULT OnValueChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandle)
	{
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
		return 0;
	}
	LRESULT OnSelchangeValue(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if (!m_bLoadPropery)
			m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
		return 0;
	}
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_bLoadPropery = TRUE;

		m_btnEdgeColor.SubclassWindow(GetDlgItem(IDC_EDGECOLOR));
		m_btnHighLightEdgeColor.SubclassWindow(GetDlgItem(IDC_HIGHLIGHT_EDGECOLOR));
		m_btnFillColor.SubclassWindow(GetDlgItem(IDC_FILLCOLOR));
		m_btnHighLightFillColor.SubclassWindow(GetDlgItem(IDC_HIGHLIGHT_FILLCOLOR));

		m_btnEdgeColor.SetDefaultColor(RGB(0, 0, 0));
		m_btnHighLightEdgeColor.SetDefaultColor(RGB(255, 255, 255));
		m_btnFillColor.SetDefaultColor(RGB(0, 0, 255));
		m_btnHighLightFillColor.SetDefaultColor(RGB(255, 255, 255));
		

		// 边界缺省颜色
//		CCxDrawObjectBase objectbase;
//		OLE_COLOR clr;
//		objectbase.GetDefaultEdgeColor(clr);
//		COLORREF clrref;
//		OleTranslateColor(clr, 0, &clrref);
//		m_btnEdgeColor.SetDefaultColor(clrref);
//
//		objectbase.GetDefaultForegroundColor(clr);
//		OleTranslateColor(clr, 0, &clrref);
//		m_btnFillColor.SetDefaultColor(clrref);

		CComVariant value;
		CComDispatchDriver dd(m_ppUnk[0]);

		dd.GetPropertyByName(L"EdgeColor", &value);
		if (value.vt == VT_EMPTY)
			m_btnEdgeColor.EnableWindow(FALSE);
		else
			m_btnEdgeColor.SetColor(value.lVal);
		value.Clear();
		dd.GetPropertyByName(L"HighLightEdgeColor", &value);
		if (value.vt == VT_EMPTY)
			m_btnHighLightEdgeColor.EnableWindow(FALSE);
		else
			m_btnHighLightEdgeColor.SetColor(value.lVal);
		value.Clear();
		dd.GetPropertyByName(L"FillColor", &value);
		if (value.vt == VT_EMPTY)
			m_btnFillColor.EnableWindow(FALSE);
		else
			m_btnFillColor.SetColor(value.lVal);
		value.Clear();
		dd.GetPropertyByName(L"HighLightFillColor", &value);
		if (value.vt == VT_EMPTY)
			m_btnHighLightFillColor.EnableWindow(FALSE);
		else
			m_btnHighLightFillColor.SetColor(value.lVal);
		value.Clear();

		m_cmbHorDirection.LinkTo(m_hWnd, IDC_HORFILLDIR, IDB_ARROW);
		m_cmbVerDirection.LinkTo(m_hWnd, IDC_VERFILLDIR, IDB_ARROW);

		const int nComboNumber = 2;
		const int nComboItemNum = 3;
		TCHAR* DlgStaticText[nComboNumber][nComboItemNum] = 
					{_T("左边"), _T("中间"), _T("右边"),
					 _T("顶部"), _T("中部"), _T("底部")};

		for (int i = 0; i < nComboItemNum; i++)
		{
			m_cmbHorDirection.AddItem(DlgStaticText[0][i], i);
			m_cmbVerDirection.AddItem(DlgStaticText[1][i], i+3);
		}

		dd.GetPropertyByName(L"HorizontalFillDirection", &value);
		if (value.vt == VT_EMPTY)
			m_cmbHorDirection.DisEnable();
		else
			m_cmbHorDirection.SetCurSel(value.lVal);
		value.Clear();

		dd.GetPropertyByName(L"VerticalFillDirection", &value);
		if (value.vt == VT_EMPTY)
			m_cmbVerDirection.DisEnable();
		else
			m_cmbVerDirection.SetCurSel(value.lVal);
		value.Clear();

		m_cmbEdgeStyle.LinkTo(m_hWnd, IDC_EDGETYPE, COwnerDrawCombo::DRAWTYPE::LINETYPE);
		m_cmbFillStyle.LinkTo(m_hWnd, IDC_FILLTYPE, COwnerDrawCombo::DRAWTYPE::FILLTYPE);
		
		dd.GetPropertyByName(L"EdgeStyle", &value);
		if (value.vt == VT_EMPTY)
			m_cmbEdgeStyle.DisEnable();
		else
			m_cmbEdgeStyle.SetCurSel(value.lVal);
		value.Clear();

		dd.GetPropertyByName(L"FillStyle", &value);
		if (value.vt == VT_EMPTY)
			m_cmbFillStyle.DisEnable();
		else
			m_cmbFillStyle.SetCurSel(value.lVal);
		value.Clear();


		m_editHorizontalFillPercent.LinkTo(m_hWnd, IDC_HORFILLPER);
		m_editVerticalFillPercent.LinkTo(m_hWnd, IDC_VERFILLPER);
		m_editEdgeWidth.LinkTo(m_hWnd, IDC_EDGEWIDTH);
	

		m_editHorizontalFillPercent.SetValueType(CRestrictEdit::ValueType::TYPE_NUMBER);
		m_editVerticalFillPercent.SetValueType(CRestrictEdit::ValueType::TYPE_NUMBER);
		m_editEdgeWidth.SetValueType(CRestrictEdit::ValueType::TYPE_NUMBER);

		m_editHorizontalFillPercent.SetValueRange(0, 100);
		m_editVerticalFillPercent.SetValueRange(0, 100);
		m_editEdgeWidth.SetValueRange(1, 60);

		dd.GetPropertyByName(L"HorizontalFillPercent", &value);
		if (value.vt == VT_EMPTY)
			m_editHorizontalFillPercent.DisEnable();
		else
			m_editHorizontalFillPercent.SetText(value);

		dd.GetPropertyByName(L"VerticalFillPercent", &value);
		if (value.vt == VT_EMPTY)
			m_editVerticalFillPercent.DisEnable();
		else
			m_editVerticalFillPercent.SetText(value);

		dd.GetPropertyByName(L"EdgeWidth", &value);
		if (value.vt == VT_EMPTY)
			m_editEdgeWidth.DisEnable();
		else
			m_editEdgeWidth.SetText(value);

		m_bLoadPropery = FALSE;
	
		return 0;
	}
};

#endif //__CXSHAPEGENPAGE_H_
