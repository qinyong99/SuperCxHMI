// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif

#include <atlimpl.cpp>

HRESULT GetDefaultFont(IFontDisp** ppFont)
{
	USES_CONVERSION;
	HFONT hSystemFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	if (hSystemFont == NULL)
		hSystemFont = (HFONT) GetStockObject(SYSTEM_FONT);

	LOGFONT logfont;
	GetObject(hSystemFont, sizeof(logfont), &logfont);
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
	HDC hdc;

	hdc = ::GetDC(GetDesktopWindow());
	ppi = GetDeviceCaps(hdc, LOGPIXELSY);
	::ReleaseDC(GetDesktopWindow(), hdc);

	fd.cySize.Lo = lfHeight * 720000 / ppi;
	fd.cySize.Hi = 0;
	
	return OleCreateFontIndirect(&fd, IID_IFontDisp, (void**)ppFont);
}

int GetFontHeight(IFontDisp* pFontDisp)
{
	TEXTMETRIC tm;
	HFONT hOldFont = NULL;
	HDC hDC = ::GetDC(NULL);
	
	if (pFontDisp != NULL)
	{
		CComPtr<IFont> pFont;
		pFontDisp->QueryInterface(IID_IFont, (void**)&pFont);
		HFONT hFont;
		pFont->get_hFont(&hFont);
		hOldFont = (HFONT)::SelectObject(hDC, hFont);
	}
	
	::GetTextMetrics(hDC, &tm);

	if (hOldFont != NULL)
		::SelectObject(hDC, hOldFont);
	::ReleaseDC(NULL, hDC);

	return tm.tmHeight + tm.tmExternalLeading;
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

HRESULT SaveFontToStream(IFontDisp* pFont, IStream* pStm)
{
	BOOL b = (pFont != NULL);
	pStm->Write(&b, sizeof(b), NULL);
	if (b)
	{
		CComPtr<IPersistStream> spStream;
		pFont->QueryInterface(IID_IPersistStream, (void**)&spStream);
		if (spStream != NULL)
			OleSaveToStream(spStream, pStm);
		else
			WriteClassStm(pStm, CLSID_NULL);
	}

	return S_OK;
}

HRESULT ReadFontFromStream(IFontDisp** ppFont, IStream* pStm)
{
	BOOL b;
	pStm->Read(&b, sizeof(b), NULL);
	if (b)
	{
		CComPtr<IUnknown> spUnk;
		OleLoadFromStream(pStm, IID_IUnknown, (void **)&spUnk);
		if (spUnk != NULL)
			spUnk->QueryInterface(IID_IFontDisp, (void **)ppFont);
	}
	
	return S_OK;
}
