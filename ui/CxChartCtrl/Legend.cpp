// Legend.cpp : Implementation of CCxDrawChartApp and DLL registration.

#include "stdafx.h"
#include "CxChartCtrl.h"
#include "TrendCtrl.h"

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CLegend::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ILegend,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

void CLegend::RecalcRect(RECT& rect)
{ 
	if (!m_bShow)
		return;

	int nHeight = GetFontHeight(m_spTextFont);

	CopyRect(&m_rect, &rect);
	m_rect.bottom = m_rect.top + nHeight + 2;
	rect.top = m_rect.bottom;
}

void CLegend::AdjustHeight(int nHeight)
{
	m_rect.bottom = m_rect.top + nHeight;
}

STDMETHODIMP CLegend::Save(LPSTREAM pStm, BOOL fClearDirty)
{
	pStm->Write(&m_bShow, sizeof(m_bShow), NULL);
	m_bstrDesc.WriteToStream(pStm);

	SaveFontToStream(m_spTextFont, pStm);
	pStm->Write(&m_clrText, sizeof(m_clrText), NULL);
	
	return S_OK;
}

STDMETHODIMP CLegend::Load(LPSTREAM pStm)
{
	pStm->Read(&m_bShow, sizeof(m_bShow), NULL);
	m_bstrDesc.ReadFromStream(pStm);

	ReadFontFromStream(&m_spTextFont, pStm);
	pStm->Read(&m_clrText, sizeof(m_clrText), NULL);
	
	return S_OK;
}

STDMETHODIMP CLegend::InitNew()
{
	m_bShow = VARIANT_TRUE;
	GetDefaultFont(&m_spTextFont);
	m_clrText = 0x80000000 | COLOR_WINDOWTEXT;
//	m_bstrDesc = m_pSerial->m_bstrSource;

//	int iIndex = m_pSerial->m_pChart->m_apSerials.Find(m_pSerial);
//	TCHAR szBuf[32];
//	sprintf(szBuf, _T("Serial)
//	m_bstrDesc = m_pSerial->m_pChart->m_apSerials.Find(m_pSerial);

	
	return S_OK;
}

void CLegend::CalcLargestWidth(int& nWidth)
{
	if (!m_bShow)
		return;

	HDC hdc = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
		
	HFONT hFont;
	HFONT hOldFont;
	if (m_spTextFont != NULL)
	{
		CComPtr<IFont> pFont;
		m_spTextFont->QueryInterface(IID_IFont, (void**)&pFont);
		pFont->get_hFont(&hFont);
	}
	else
	{
		hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	}
	hOldFont = (HFONT)::SelectObject(hdc, hFont);

	CComBSTR bstrDesc = m_bstrDesc;
	if (bstrDesc == L"")
		bstrDesc = m_pSerial->m_bstrSource;

	SIZE szSize;
	::GetTextExtentPoint32W(hdc, bstrDesc, bstrDesc.Length(), &szSize);

	int nCalcWidth = szSize.cx + szSize.cy + 40;
	if (nWidth < nCalcWidth)
		nWidth = nCalcWidth;

	::SelectObject(hdc, hOldFont);
	DeleteDC(hdc);
}

void CLegend::DoDraw(HDC hdcDest)
{
	USES_CONVERSION;

	if (!m_bShow)
		return;

	COLORREF clrText;
	OleTranslateColor(m_clrText, NULL, &clrText);
	COLORREF clrOldTextColor = ::SetTextColor(hdcDest, clrText);

	HFONT hFont;
	HFONT hOldFont;
	if (m_spTextFont != NULL)
	{
		CComPtr<IFont> pFont;
		m_spTextFont->QueryInterface(IID_IFont, (void**)&pFont);
		pFont->get_hFont(&hFont);
	}
	else
	{
		hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	}
	hOldFont = (HFONT)::SelectObject(hdcDest, hFont);


	COLORREF clrLineColor;
	OleTranslateColor(m_pSerial->m_clrLineColor, NULL, &clrLineColor);

	HBRUSH hBrush = ::CreateSolidBrush(clrLineColor);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdcDest, hBrush);

	HPEN hOldPen = (HPEN)SelectObject(hdcDest, GetStockObject(BLACK_PEN));

	RECT rect = m_rect;
	rect.right = rect.left + rect.bottom - rect.top;
	InflateRect(&rect, -4, -4);
	OffsetRect(&rect, -4, 0);
	::IntersectRect(&rect, &rect, &m_rect);
	Rectangle(hdcDest, rect.left, rect.top, rect.right, rect.bottom);

	SelectObject(hdcDest, hOldBrush);
	::DeleteObject((HGDIOBJ)hBrush);

	SelectObject(hdcDest, hOldPen);

	rect.left = rect.right + 4; 
	rect.right = m_rect.right;
	rect.top = m_rect.top;
	rect.bottom = m_rect.bottom;

	CComBSTR bstrDesc = m_bstrDesc;
	if (bstrDesc == L"")
		bstrDesc = m_pSerial->m_bstrSource;
//	::ExtTextOut(hdcDest, rect.left, rect.top, ETO_CLIPPED, &rect, W2T(bstrDesc), 3, NULL);
	::DrawText(hdcDest, W2T(bstrDesc), -1, &rect, DT_SINGLELINE | DT_VCENTER);

	::SetTextColor(hdcDest, clrOldTextColor);
	::SelectObject(hdcDest, hOldFont);
}

STDMETHODIMP CLegend::get_Show(VARIANT_BOOL *pVal)
{
	*pVal = m_bShow;

	return S_OK;
}

STDMETHODIMP CLegend::put_Show(VARIANT_BOOL newVal)
{
	m_bShow = newVal;
	m_pSerial->m_pChart->RecalcRects();
	m_pSerial->m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CLegend::get_Description(BSTR *pVal)
{
	*pVal = m_bstrDesc.Copy();

	return S_OK;
}

STDMETHODIMP CLegend::put_Description(BSTR newVal)
{
	m_bstrDesc = newVal;
	m_pSerial->m_pChart->RecalcRects();
	m_pSerial->m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CLegend::get_TextFont(IFontDisp** ppFont)
{
	*ppFont = m_spTextFont;
	if (*ppFont != NULL)
		(*ppFont)->AddRef();
	
	return S_OK;
}

STDMETHODIMP CLegend::put_TextFont(IFontDisp* pFont)
{
	if (pFont)
	{
		CComQIPtr<IFont, &IID_IFont> p(pFont);
		if (p)
		{
			// 如果想等即返回
			CComQIPtr<IFont, &IID_IFont> pOld(m_spTextFont);
			if (p->IsEqual(pOld) == S_OK)
				return S_OK;
			
			m_spTextFont = 0;
			
			CComPtr<IFont> pFont;
			p->Clone(&pFont);
			if (pFont)
				pFont->QueryInterface(IID_IFontDisp, (void**) &m_spTextFont);
		}
	}
	else
	{
		if (m_spTextFont == NULL)
			return S_OK;
		
		m_spTextFont = 0;
	}
	
	m_pSerial->m_pChart->RecalcRects();
	m_pSerial->m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CLegend::putref_TextFont(IFontDisp* pFont)
{
	m_spTextFont = pFont;
	m_pSerial->m_pChart->RecalcRects();
	m_pSerial->m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CLegend::get_TextColor(OLE_COLOR *pVal)
{
	*pVal = m_clrText;
	
	return S_OK;
}

STDMETHODIMP CLegend::put_TextColor(OLE_COLOR newVal)
{
	m_clrText = newVal;
	m_pSerial->m_pChart->FireViewChange();
	
	return S_OK;
}
