// TextGenPage.cpp : Implementation of CTextGenPage
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "TextGenPage.h"

/////////////////////////////////////////////////////////////////////////////
// CTextGenPage

static int _TTDefaults[] = { 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48 };
#define _countof(array) (sizeof(array)/sizeof(array[0]))

LRESULT CTextGenPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	m_cmbFontName.Init(GetDlgItem(IDC_FONT_NAME));
	m_cmbFontSize.Attach(GetDlgItem(IDC_FONT_SIZE));
	m_cmbFontStyle.Attach(GetDlgItem(IDC_FONT_STYLE));
	m_btnFontColor.SubclassWindow(GetDlgItem(IDC_FONT_COLOR));
	m_btnStrikeThrough.Attach(GetDlgItem(IDC_STRIKE_THROUGH));
	m_btnUnderLine.Attach(GetDlgItem(IDC_UNDER_LINE));
	m_SampleBox.Attach(GetDlgItem(IDC_SAMPLE_BOX));

	m_btnFontColor.SetDefaultColor(RGB(0, 0, 0));

	m_cmbFontStyle.InsertString(0, _T("规则"));
	m_cmbFontStyle.InsertString(1, _T("粗体"));
	m_cmbFontStyle.InsertString(2, _T("斜体"));
	m_cmbFontStyle.InsertString(3, _T("粗斜体"));

	TCHAR szBuf[8];
	for (int i = 0; i < _countof(_TTDefaults); i++)
	{
		sprintf(szBuf, "%d", _TTDefaults[i]);
		m_cmbFontSize.AddString(szBuf);
	}
	
	m_bLoadPropery = TRUE;

	if (m_nObjects > 0)
	{
		CComQIPtr<IDrawTxt, &IID_IDrawTxt> spText(m_ppUnk[0]);

		CComBSTR bstr;
		spText->get_FontName(&bstr);
		m_cmbFontName.SelectString(-1, OLE2T(bstr));

		long l;
		spText->get_FontStyle((enumFontStyle *)&l);
		m_cmbFontStyle.SetCurSel(l);

		OLE_COLOR clr;
		spText->get_FontColor(&clr);
		m_btnFontColor.SetColor(clr);

		VARIANT_BOOL b;
		spText->get_StrikeThrough(&b);
		m_btnStrikeThrough.SetCheck(b == VARIANT_TRUE);

		spText->get_UnderLine(&b);
		m_btnUnderLine.SetCheck(b == VARIANT_TRUE);
	}

	m_bLoadPropery = FALSE;

	return 0;
}

STDMETHODIMP CTextGenPage::Apply()
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<IDrawTxt, &IID_IDrawTxt> spText(m_ppUnk[i]);

		CComBSTR bstr = m_cmbFontName.GetCurrentName();
		spText->put_FontName(bstr);
	
		int iIndex = m_cmbFontStyle.GetCurSel();
		spText->put_FontStyle((enumFontStyle)iIndex);

		m_cmbFontSize.GetWindowText((BSTR&)bstr);
		CComVariant var = bstr;
		var.ChangeType(VT_I4);
		if (var.vt == VT_I4)
			spText->put_FontSize(var.lVal);

		spText->put_FontColor(m_btnFontColor.GetColor());
		spText->put_StrikeThrough(m_btnStrikeThrough.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
		spText->put_UnderLine(m_btnUnderLine.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE);
	}
	m_bDirty = FALSE;

	return S_OK;
}

//static int _TTDefaults[] = { 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48 };
//#define _countof(array) (sizeof(array)/sizeof(array[0]))

/*LRESULT CTextGenPage::OnSelchangeFont_name(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_cmbFontSize.ResetContent();
	if (m_cmbFontName.GetFontType() != RASTER_FONTTYPE)
	{
		// Fill with "common" sizes
		TCHAR szBuf[8];
		for (int i = 0; i < _countof(_TTDefaults); i++)
		{
			sprintf(szBuf, "%d", _TTDefaults[i]);
			m_cmbFontSize.AddString(szBuf);
		}
	}

	OnSelchangeValue(wNotifyCode, wID, hWndCtl, bHandled);

	return 0;
}
*/
#ifdef SelectFont
#undef SelectFont

LRESULT CTextGenPage::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;
	CPaintDC dc(m_hWnd);
	
	RECT rcText;
	m_SampleBox.GetWindowRect(&rcText);
	ScreenToClient(&rcText);

	OLE_COLOR clr = m_btnFontColor.GetColor();
	COLORREF clrRef;
	OleTranslateColor(clr, 0, &clrRef);
	COLORREF crText = dc.SetTextColor(clrRef);
	int bkMode = dc.SetBkMode(TRANSPARENT);

	//创建字体
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));
	logFont.lfCharSet = DEFAULT_CHARSET;

	logFont.lfHeight = 20;

	CComBSTR bstr;
	m_cmbFontSize.GetWindowText((BSTR&)bstr);
	CComVariant var = bstr;
	var.ChangeType(VT_I4);
	if (var.vt == VT_I4)
		logFont.lfHeight = var.lVal;
		
//	logFont.lfWidth = ROUND(m_dFontWidth);

	int iIndex = m_cmbFontStyle.GetCurSel();
	logFont.lfWeight = (iIndex == FontStyleBold || iIndex == FontStyleBoldItalic) ? FW_BOLD : FW_NORMAL;
	logFont.lfItalic = (iIndex == FontStyleItalic || iIndex == FontStyleBoldItalic);

	logFont.lfUnderline = m_btnUnderLine.GetCheck();
	logFont.lfStrikeOut = m_btnStrikeThrough.GetCheck();

	bstr = m_cmbFontName.GetCurrentName();
	LPTSTR szFontName = W2T(bstr);
	int cbFontName = lstrlen(szFontName);
	lstrcpyn(logFont.lfFaceName, szFontName, cbFontName + 1);

	HFONT hFont = CreateFontIndirect(&logFont);	
	HFONT hOldFont = dc.SelectFont(hFont);

	dc.DrawText(_T("AaBbCc"), 6, &rcText, DT_SINGLELINE|DT_CENTER|DT_VCENTER);

	dc.SelectFont(hOldFont);
	DeleteObject(hFont);

	// Put the DC back the way it was
	dc.SetBkMode(bkMode);
	dc.SetTextColor(crText);

	return 0;
}

#endif SelectFont
