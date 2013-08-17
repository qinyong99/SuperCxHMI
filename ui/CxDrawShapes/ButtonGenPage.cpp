// ButtonGenPage.cpp : Implementation of CButtonGenPage
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "ButtonGenPage.h"

/////////////////////////////////////////////////////////////////////////////
// CButtonGenPage

LRESULT CButtonGenPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	m_editTextFont.Attach(GetDlgItem(IDC_TEXT_FONT));

	m_btnForeColor.SubclassWindow(GetDlgItem(IDC_FORECOLOR));
	m_btnBackColor.SubclassWindow(GetDlgItem(IDC_BACKCOLOR));

	m_btnForeColor.SetDefaultColor(GetSysColor(COLOR_BTNTEXT));
	m_btnBackColor.SetDefaultColor(GetSysColor(COLOR_BTNFACE));
	
	m_btnNormal.SubclassWindow(GetDlgItem(IDC_NORMAL));
	m_btnHover.SubclassWindow(GetDlgItem(IDC_HOVER));
	m_btnPushed.SubclassWindow(GetDlgItem(IDC_PUSHED));
	m_btnDisabled.SubclassWindow(GetDlgItem(IDC_DISABLED));

	m_editCaption.Attach(GetDlgItem(IDC_CAPTION));

	m_bLoadPropery = TRUE;

	if (m_nObjects > 0)
	{
		CComQIPtr<IDrawButton, &IID_IDrawButton> spButton(m_ppUnk[0]);
		
		OLE_COLOR clr;
		spButton->get_ForeColor(&clr);
		m_btnForeColor.SetColor(clr);

		spButton->get_BackColor(&clr);
		m_btnBackColor.SetColor(clr);

		CComBSTR bstr;
		spButton->get_Caption(&bstr);
		m_editCaption.SetWindowText(OLE2T(bstr));

		spButton->get_PictureNormal(&m_pPictureNormal);
		spButton->get_PictureHover(&m_pPictureHover);
		spButton->get_PicturePushed(&m_pPicturePushed);
		spButton->get_PictureDisabled(&m_pPictureDisabled);

		spButton->get_Font(&m_spTextFont);
		if (m_spTextFont != NULL)
		{	
			CComDispatchDriver dd(m_spTextFont);
			CComVariant var;
			dd.GetProperty(0, &var);
			if (var.vt == VT_BSTR)
				m_editTextFont.SetWindowText(OLE2T(var.bstrVal));
		}
		else
		{
			m_editTextFont.SetWindowText(_T(""));
		}
	}

	m_bLoadPropery = FALSE;

	UpdateClearButtons();

	return 0;
}

STDMETHODIMP CButtonGenPage::Apply()
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<IDrawButton, &IID_IDrawButton> spButton(m_ppUnk[i]);
		
		spButton->put_ForeColor(m_btnForeColor.GetColor());
		spButton->put_BackColor(m_btnBackColor.GetColor());
		
		spButton->put_Font(m_spTextFont);

		CComBSTR bstr;
		m_editCaption.GetWindowText((BSTR&)bstr);
		spButton->put_Caption(bstr);

		spButton->put_PictureNormal(m_pPictureNormal);
		spButton->put_PictureHover(m_pPictureHover);
		spButton->put_PicturePushed(m_pPicturePushed);
		spButton->put_PictureDisabled(m_pPictureDisabled);
	}
	m_bDirty = FALSE;

	return S_OK;
}

BOOL PickFont(HWND hWndParant, IFontDisp** ppFont)
{
	LOGFONT logfont;
	
	HFONT hFont;
	if (*ppFont != NULL)
	{
		CComPtr<IFont> spFont;
		(*ppFont)->QueryInterface(IID_IFont, (void**)&spFont);
		spFont->get_hFont(&hFont);
	}
	else
	{
		hFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	}
	
	GetObject(hFont, sizeof(LOGFONT), &logfont); 
	
	CHOOSEFONT cf; 
	memset(&cf, 0, sizeof CHOOSEFONT);
	cf.hInstance = _Module.m_hInst;
	cf.lStructSize = sizeof CHOOSEFONT;
	cf.hwndOwner = hWndParant;
	cf.lpLogFont = &logfont;
	cf.lpTemplateName = "IDD_FONTDIALOG";
	cf.Flags = CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_ENABLETEMPLATE;
	
	if (ChooseFont(&cf))
	{
		USES_CONVERSION;
		
		FONTDESC fd;
		fd.cbSizeofstruct = sizeof(FONTDESC);
		fd.lpstrName = T2OLE(logfont.lfFaceName);
		fd.sWeight = (short)logfont.lfWeight;
		fd.sCharset = logfont.lfCharSet;
		fd.fItalic = logfont.lfItalic;
		fd.fUnderline = logfont.lfUnderline;
		fd.fStrikethrough = logfont.lfStrikeOut;
		
		long lfHeight = logfont.lfHeight;
		if (lfHeight < 0)
			lfHeight = -lfHeight;
		
		int ppi;
		HDC hdc = ::GetDC(hWndParant);
		ppi = GetDeviceCaps(hdc, LOGPIXELSY);
		::ReleaseDC(hWndParant, hdc);
		
		fd.cySize.Lo = lfHeight * 720000 / ppi;
		fd.cySize.Hi = 0;
		
		if (*ppFont != NULL)
			(*ppFont)->Release();
		
		CComPtr<IDispatch> spFont;			
		HRESULT hr = OleCreateFontIndirect(&fd, IID_IDispatch, (void**)ppFont);
		return hr == S_OK;
	}
	
	return FALSE;
}

LRESULT CButtonGenPage::OnClickedTextfontPick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;
	
	if (PickFont(m_hWnd, &m_spTextFont.p))
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
	
	if (m_spTextFont != NULL)
	{	
		CComDispatchDriver dd(m_spTextFont);
		CComVariant var;
		dd.GetProperty(0, &var);
		if (var.vt == VT_BSTR)
			m_editTextFont.SetWindowText(OLE2T(var.bstrVal));
	}
	else
	{
		m_editTextFont.SetWindowText(_T(""));
	}
	
	return 0;
}

BOOL GetPicture(HWND hwnd, IPictureDisp** ppPicture)
{
	USES_CONVERSION;

	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof OPENFILENAME);			
	ofn.lStructSize = sizeof OPENFILENAME;
	ofn.lpstrFile =  (TCHAR*) _alloca(_MAX_PATH * sizeof TCHAR);
	ofn.nMaxFile = _MAX_PATH;	
	ofn.hwndOwner = hwnd;				
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	
	memset(ofn.lpstrFile, 0, _MAX_PATH * sizeof TCHAR); 

	ofn.lpstrFilter = _T("所有图片文件\0*.bmp;*.dib;*.gif;*.jpg;*.wmf;*.emf;*.ico;*.cur\0")
		_T("位图 (*.bmp;*.dib)\0*.bmp;*.dib\0")
		_T("GIF 图像 (*.gif)\0*.gif\0")
		_T("JPEG 图像 (*.jpg)\0*.jpg\0")
		_T("元文件 (*.wmf;*.emf)\0*.wmf;*.emf\0")
		_T("Icons (*.ico;*.cur)\0*.ico;*.cur\0")
		_T("所有文件(*.*)\0*.*\0");
	ofn.lpstrTitle = _T("加载图片");
	
	if (GetOpenFileName(&ofn))
	{
		CComBSTR bstr(ofn.lpstrFile);

		if (*ppPicture)
			(*ppPicture)->Release();

		HRESULT hr = OleLoadPicturePath(bstr.m_str, 0, 0, 0, IID_IDispatch, (void**)ppPicture);

		return TRUE;
	}

	return FALSE;
}

LRESULT CButtonGenPage::OnClickedNormal(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (GetPicture(m_hWnd, &m_pPictureNormal))
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);

	UpdateClearButtons();

	return 0;
}

LRESULT CButtonGenPage::OnClickedHover(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (GetPicture(m_hWnd, &m_pPictureHover))
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);

	UpdateClearButtons();

	return 0;
}

LRESULT CButtonGenPage::OnClickedPushed(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (GetPicture(m_hWnd, &m_pPicturePushed))
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);

	UpdateClearButtons();

	return 0;
}

LRESULT CButtonGenPage::OnClickedDisabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (GetPicture(m_hWnd, &m_pPictureDisabled))
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
	
	UpdateClearButtons();
	
	return 0;
}

LRESULT CButtonGenPage::OnClickedClearNormal(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pPictureNormal = NULL;
	m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);

	UpdateClearButtons();

	return 0;
}

LRESULT CButtonGenPage::OnClickedClearHover(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pPictureHover = NULL;
	m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);

	UpdateClearButtons();

	return 0;
}

LRESULT CButtonGenPage::OnClickedClearPushed(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pPicturePushed = NULL;
	m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);

	UpdateClearButtons();

	return 0;
}

LRESULT CButtonGenPage::OnClickedClearDisabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pPictureDisabled = NULL;
	m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
	
	UpdateClearButtons();
	
	return 0;
}