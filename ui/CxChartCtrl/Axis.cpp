// Axis.cpp : Implementation of CCxDrawChartApp and DLL registration.

#include "stdafx.h"
#include "CxChartCtrl.h"
#include "TrendCtrl.h"

#define DBL_EPSILON     2.2204460492503131e-016 /* smallest such that 1.0+DBL_EPSILON != 1.0 */

/************************************************************************************************/
/*                                                                                              */
/* Function: AutoScale()                                                                        */
/*                                                                                              */
/* Purpose : based on the value and direction passed in this function will return an easy to use*/
/*           value to be used in setting the chart scales automatically                         */
/*                                                                                              */
/* Inputs  : double dValue -> the value to scale                                                */
/*           BOOL bDirection -> the direction to scale toward.                                  */
/*                                                                                              */
/* Outputs : double <- the auto scaled value                                                    */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 15MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          15MAY02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
double AutoScale(double dValue, BOOL bDirection) {
	
    BOOL bNegative(dValue<0);
    
    if (bNegative) {
        dValue *= -1;
        bDirection = !bDirection;
	}
	
    if (dValue+DBL_EPSILON==0)
        return 0;
	
    double dLimits[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double dReturn(0);
	
    while (9) {
        if (dValue<*dLimits) {
            *(dLimits+0) = *(dLimits+0)/10;
            *(dLimits+1) = *(dLimits+1)/10;
            *(dLimits+2) = *(dLimits+2)/10;
            *(dLimits+3) = *(dLimits+3)/10;
            *(dLimits+4) = *(dLimits+4)/10;
            *(dLimits+5) = *(dLimits+5)/10;
            *(dLimits+6) = *(dLimits+6)/10;
            *(dLimits+7) = *(dLimits+7)/10;
            *(dLimits+8) = *(dLimits+8)/10;
            *(dLimits+9) = *(dLimits+9)/10;
		}
        else if (dValue>*(dLimits+sizeof(dLimits)/sizeof(double)-1)) {
            *(dLimits+0) = *(dLimits+0)*10;
            *(dLimits+1) = *(dLimits+1)*10;
            *(dLimits+2) = *(dLimits+2)*10;
            *(dLimits+3) = *(dLimits+3)*10;
            *(dLimits+4) = *(dLimits+4)*10;
            *(dLimits+5) = *(dLimits+5)*10;
            *(dLimits+6) = *(dLimits+6)*10;
            *(dLimits+7) = *(dLimits+7)*10;
            *(dLimits+8) = *(dLimits+8)*10;
            *(dLimits+9) = *(dLimits+9)*10;
		}
        else
            break;
	}
	
    for (int i(0);MIN==bDirection&&i<sizeof(dLimits)/sizeof(double)-1;i++) {
        if (dValue>=*(dLimits+i)&&dValue<=*(dLimits+i+1)) {
			if (dValue==*(dLimits+i+1))
				dReturn = *(dLimits+i+1);
			else
				dReturn = *(dLimits+i);
            break;
		}
	}
	
    for (i=0;MAX==bDirection&&i<sizeof(dLimits)/sizeof(double)-1;i++) {
        if (dValue>=*(dLimits+i)&&dValue<=*(dLimits+i+1)) {
			if (dValue==*(dLimits+i))
				dReturn = *(dLimits+i);
			else
				dReturn = *(dLimits+i+1);
            break;
		}
	}
	
    if (bNegative)
        dReturn *= -1;
	
    return dReturn;
}

/////////////////////////////////////////////////////////////////////////////
//
 
CAxis::CAxis()
{
	m_dMaxVal				= -DBL_MAX;
	m_dMinVal				= DBL_MAX;

	m_pChart = NULL;
}

void CAxis::ScrollPercentage(BOOL bUpOrRight, float fPercentage)
{
	double dRange = (m_dMaxVal - m_dMinVal) * fPercentage;
	Scroll(bUpOrRight ? dRange : -dRange);
}

void CAxis::Scroll(double dRange)
{
	m_dMaxVal += dRange;
	m_dMinVal += dRange;
}

void CAxis::Scale(double dMin, double dMax)
{
	m_dMinVal = dMin;
	m_dMaxVal = dMax;
}

STDMETHODIMP CAxis::Save(LPSTREAM pStm, BOOL fClearDirty)
{
	m_bstrDisplayFormat.WriteToStream(pStm);
	m_bstrTitle.WriteToStream(pStm);
	pStm->Write(&m_lNumTicks, sizeof(m_lNumTicks), NULL);
	pStm->Write(&m_bShowAxis, sizeof(m_bShowAxis), NULL);
	pStm->Write(&m_bShowTitle, sizeof(m_bShowTitle), NULL);
	pStm->Write(&m_clrAxis, sizeof(m_clrAxis), NULL);
	pStm->Write(&m_clrTitle, sizeof(m_clrTitle), NULL);

	SaveFontToStream(m_spTickFont, pStm);
	SaveFontToStream(m_spTitleFont, pStm);
			
	return S_OK;
}

STDMETHODIMP CAxis::Load(LPSTREAM pStm)
{
	m_bstrDisplayFormat.ReadFromStream(pStm);
	m_bstrTitle.ReadFromStream(pStm);
	pStm->Read(&m_lNumTicks, sizeof(m_lNumTicks), NULL);
	pStm->Read(&m_bShowAxis, sizeof(m_bShowAxis), NULL);
	pStm->Read(&m_bShowTitle, sizeof(m_bShowTitle), NULL);
	pStm->Read(&m_clrAxis, sizeof(m_clrAxis), NULL);
	pStm->Read(&m_clrTitle, sizeof(m_clrTitle), NULL);

	ReadFontFromStream(&m_spTickFont, pStm);
	ReadFontFromStream(&m_spTitleFont, pStm);

	return S_OK;
}

STDMETHODIMP CAxis::InitNew()
{
	m_lNumTicks = 5;
	m_bShowAxis = VARIANT_TRUE;
	m_bShowTitle = VARIANT_FALSE;
	COLORREF clr = RGB(0, 0, 0);
	m_clrAxis = RGB(0, 0, 0);
	m_clrTitle = RGB(0, 0, 0);
	GetDefaultFont(&m_spTickFont);
	GetDefaultFont(&m_spTitleFont);
	
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CValueAxis::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IValueAxis,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

void CValueAxis::UpdateDisplayFormat()
{
	double dViewMinValue = fabs(m_dViewMinVal - DBL_MAX) < 1E-6 ? m_dMinVal : m_dViewMinVal;
	double dViewMaxValue = fabs(m_dViewMaxVal + DBL_MAX) < 1E-6 ? m_dMaxVal : m_dViewMaxVal;
	
	WCHAR szBuf[8];

	// 在绝对值小于10^6时采用科学计数法
	if ((fabs(dViewMinValue) > 1E6 || fabs(dViewMaxValue) > 1E6))
		swprintf(szBuf, L"%%%d.%de", m_iWholeDigits, m_iDecimalDigits);
	else
		swprintf(szBuf, L"%%%d.%df", m_iWholeDigits, m_iDecimalDigits);

	m_bstrDisplayFormat = szBuf;
}

void CValueAxis::RecalcRect(RECT& rect)
{
	HDC hdc = ::GetDC(NULL);

	HFONT hFont;
	if (m_spTickFont != NULL)
	{
		CComPtr<IFont> pFont;
		m_spTickFont->QueryInterface(IID_IFont, (void**)&pFont);
		pFont->get_hFont(&hFont);
	}
	else
	{
		hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	}
	HFONT hOldFont = (HFONT)::SelectObject(hdc, hFont);

	int nCharWidth;
	GetCharWidth32(hdc, UINT('8'),  UINT('8'), &nCharWidth);

	::SelectObject(hdc, hOldFont);
	::ReleaseDC(NULL, hdc);

	CopyRect(&m_rect, &rect);
	
	m_rect.right = min(m_rect.left + (m_bShowAxis ? nCharWidth * m_iWholeDigits + 8 : 0), rect.right);
	rect.left = m_rect.right;
}

STDMETHODIMP CValueAxis::get_NumTicks(long *pVal)
{
	*pVal = m_lNumTicks;

	return S_OK;
}

STDMETHODIMP CValueAxis::put_NumTicks(long newVal)
{
	m_lNumTicks = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CValueAxis::get_Title(BSTR *pVal)
{
	*pVal = m_bstrTitle.Copy();

	return S_OK;
}

STDMETHODIMP CValueAxis::put_Title(BSTR newVal)
{
	m_bstrTitle = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CValueAxis::get_ShowAxis(VARIANT_BOOL *pVal)
{
	*pVal = m_bShowAxis;

	return S_OK;
}

STDMETHODIMP CValueAxis::put_ShowAxis(VARIANT_BOOL newVal)
{
	m_bShowAxis = newVal;
	m_pChart->RecalcRects();
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CValueAxis::get_ShowTitle(VARIANT_BOOL *pVal)
{
	*pVal = m_bShowTitle;

	return S_OK;
}

STDMETHODIMP CValueAxis::put_ShowTitle(VARIANT_BOOL newVal)
{
	m_bShowTitle = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CValueAxis::get_AxisColor(OLE_COLOR *pVal)
{
	*pVal = m_clrAxis;

	return S_OK;
}

STDMETHODIMP CValueAxis::put_AxisColor(OLE_COLOR newVal)
{
	m_clrAxis = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CValueAxis::get_TitleColor(OLE_COLOR *pVal)
{
	*pVal = m_clrTitle;

	return S_OK;
}

STDMETHODIMP CValueAxis::put_TitleColor(OLE_COLOR newVal)
{
	m_clrTitle = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CValueAxis::get_MaxValue(double *pVal)
{
	*pVal = m_dMaxVal;
	
	return S_OK;
}

STDMETHODIMP CValueAxis::put_MaxValue(double newVal)
{
	m_dMaxVal = newVal;// AutoScale(newVal, MAX);
	m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CValueAxis::get_MinValue(double *pVal)
{
	*pVal = m_dMinVal;
	
	return S_OK;
}

STDMETHODIMP CValueAxis::put_MinValue(double newVal)
{
	m_dMinVal = newVal;// AutoScale(newVal, MIN);
	m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CValueAxis::get_WholeDigits(long *pVal)
{
	*pVal = (long)m_iWholeDigits;
	
	return S_OK;
}

STDMETHODIMP CValueAxis::put_WholeDigits(long newVal)
{
	if (m_iWholeDigits < 1)
		return S_OK;
	
	m_iWholeDigits = (int)newVal;
	
	// 保证小数位数在总数字位数允许的范围内
	if (m_iDecimalDigits >= m_iWholeDigits - 1)
		m_iDecimalDigits = m_iWholeDigits - 2;
	
	if (m_iDecimalDigits < 0)
		m_iDecimalDigits = 0;
	
	UpdateDisplayFormat();
	m_pChart->RecalcRects();
	m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CValueAxis::get_DecimalDigits(long *pVal)
{
	*pVal = (long)m_iDecimalDigits;
	
	return S_OK;
}

STDMETHODIMP CValueAxis::put_DecimalDigits(long newVal)
{
	// 保证小数位数在总数字位数允许的范围内
	if ((int)newVal < m_iWholeDigits - 1)
		m_iDecimalDigits = (int)newVal;
	else
		m_iDecimalDigits = m_iWholeDigits - 2;
	
	if (m_iDecimalDigits < 0)
		m_iDecimalDigits = 0;
	
	UpdateDisplayFormat();
	m_pChart->RecalcRects();
	m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CValueAxis::get_TickFont(IFontDisp** ppFont)
{
	*ppFont = m_spTickFont;
	if (*ppFont != NULL)
		(*ppFont)->AddRef();
	
	return S_OK;
}

STDMETHODIMP CValueAxis::put_TickFont(IFontDisp* pFont)
{
	if (pFont)
	{
		CComQIPtr<IFont, &IID_IFont> p(pFont);
		if (p)
		{
			// 如果想等即返回
			CComQIPtr<IFont, &IID_IFont> pOld(m_spTickFont);
			if (p->IsEqual(pOld) == S_OK)
				return S_OK;
			
			m_spTickFont = 0;
			
			CComPtr<IFont> pFont;
			p->Clone(&pFont);
			if (pFont)
				pFont->QueryInterface(IID_IFontDisp, (void**) &m_spTickFont);
		}
	}
	else
	{
		if (m_spTickFont == NULL)
			return S_OK;
		
		m_spTickFont = 0;
	}
	
	m_pChart->RecalcRects();
	m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CValueAxis::putref_TickFont(IFontDisp* pFont)
{
	m_spTickFont = pFont;
	m_pChart->RecalcRects();
	m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CValueAxis::get_TitleFont(IFontDisp** ppFont)
{
	*ppFont = m_spTitleFont;
	if (*ppFont != NULL)
		(*ppFont)->AddRef();
	
	return S_OK;
}

STDMETHODIMP CValueAxis::put_TitleFont(IFontDisp* pFont)
{
	if (pFont)
	{
		CComQIPtr<IFont, &IID_IFont> p(pFont);
		if (p)
		{
			// 如果想等即返回
			CComQIPtr<IFont, &IID_IFont> pOld(m_spTitleFont);
			if (p->IsEqual(pOld) == S_OK)
				return S_OK;
			
			m_spTitleFont = 0;
			
			CComPtr<IFont> pFont;
			p->Clone(&pFont);
			if (pFont)
				pFont->QueryInterface(IID_IFontDisp, (void**) &m_spTitleFont);
		}
	}
	else
	{
		if (m_spTitleFont == NULL)
			return S_OK;
		
		m_spTitleFont = 0;
	}
	
	m_pChart->RecalcRects();
	m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CValueAxis::putref_TitleFont(IFontDisp* pFont)
{
	m_spTitleFont = pFont;
	m_pChart->RecalcRects();
	m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CValueAxis::Save(LPSTREAM pStm, BOOL fClearDirty)
{
	pStm->Write(&m_iWholeDigits, sizeof(int), NULL);
	pStm->Write(&m_iDecimalDigits, sizeof(int), NULL);

	pStm->Write(&m_dMinVal, sizeof(double), NULL);
	pStm->Write(&m_dMaxVal, sizeof(double), NULL);

	return CAxis::Save(pStm, fClearDirty);
}

STDMETHODIMP CValueAxis::Load(LPSTREAM pStm)
{
	pStm->Read(&m_iWholeDigits, sizeof(int), NULL);
	pStm->Read(&m_iDecimalDigits, sizeof(int), NULL);

	pStm->Read(&m_dMinVal, sizeof(double), NULL);
	pStm->Read(&m_dMaxVal, sizeof(double), NULL);

	return CAxis::Load(pStm);
}

STDMETHODIMP CValueAxis::InitNew()
{
	m_bstrTitle = L"值";
	m_bstrDisplayFormat = L"%7.2f";

	m_iWholeDigits = 8; 
	m_iDecimalDigits = 2; 

	m_dMaxVal = 100;
	m_dMinVal = 0;

	return CAxis::InitNew();
}

void CValueAxis::ScrollPercentage(BOOL bUpOrRight, float fPercentage)
{
	if (fabs(m_dViewMinVal - DBL_MAX) < 1E-6)
		m_dViewMinVal = m_dMinVal;
	if (fabs(m_dViewMaxVal + DBL_MAX) < 1E-6)
		m_dViewMaxVal = m_dMaxVal;

	double dRange = (m_dViewMaxVal - m_dViewMinVal) * fPercentage;
	if (!bUpOrRight)
		dRange = -dRange;
	
	Scroll(dRange);
}

void CValueAxis::Scroll(double dRange)
{
	if (fabs(m_dViewMinVal - DBL_MAX) < 1E-6)
		m_dViewMinVal = m_dMinVal;
	if (fabs(m_dViewMaxVal + DBL_MAX) < 1E-6)
		m_dViewMaxVal = m_dMaxVal;
	
	m_dViewMinVal += dRange;
	m_dViewMaxVal += dRange;

	UpdateDisplayFormat();
}

void CValueAxis::Scale(double dMin, double dMax)
{
	m_dViewMinVal = dMin;
	m_dViewMaxVal = dMax;

	UpdateDisplayFormat();
}

void CValueAxis::DoDraw(HDC hdcDest)
{
	USES_CONVERSION;

	if (!m_bShowAxis)
		return;

	double dViewMinValue = fabs(m_dViewMinVal - DBL_MAX) < 1E-6 ? m_dMinVal : m_dViewMinVal;
	double dViewMaxValue = fabs(m_dViewMaxVal + DBL_MAX) < 1E-6 ? m_dMaxVal : m_dViewMaxVal;

	if (::IsRectEmpty(&m_rect))
		return;

	COLORREF clrAxis;
	OleTranslateColor(m_clrAxis, NULL, &clrAxis);
	COLORREF clrOldText = ::SetTextColor(hdcDest, clrAxis);
	
	HFONT hFont;
	HFONT hOldFont;
	if (m_spTickFont != NULL)
	{
		CComPtr<IFont> pFont;
		m_spTickFont->QueryInterface(IID_IFont, (void**)&pFont);
		pFont->get_hFont(&hFont);
	}
	else
	{
		hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	}
	hOldFont = (HFONT)::SelectObject(hdcDest, hFont);

	TEXTMETRIC tm;
	::GetTextMetrics(hdcDest, &tm);
	int nHalfTickHeight = (tm.tmHeight + tm.tmExternalLeading) / 2;
		
	HPEN hPen = ::CreatePen(PS_SOLID, 1, clrAxis);
	HGDIOBJ hOldPen = ::SelectObject(hdcDest, hPen);

	TCHAR szBuf[16];

	::MoveToEx(hdcDest, m_rect.right, m_rect.top, NULL);
	::LineTo(hdcDest, m_rect.right, m_rect.bottom);

	int nX = m_rect.right;
	int nY;
	float fDY = float(m_rect.bottom - m_rect.top) / (m_lNumTicks - 1);
	
	double dDY = (dViewMaxValue - dViewMinValue) / (m_lNumTicks - 1); 
	
	RECT rect;
	rect.left = m_rect.left;
	rect.right = m_rect.right - 8;

	for (int i = 0; i < m_lNumTicks; i++)
	{
		nY = ROUND(m_rect.bottom - fDY * i);
		rect.top = nY - nHalfTickHeight;
		rect.bottom = nY + nHalfTickHeight;

		::MoveToEx(hdcDest, nX, nY, NULL);
		::LineTo(hdcDest, max(nX - (i % 2 == 0 ? 7 : 5), m_rect.left), nY);

		double dY = dViewMinValue + i * dDY;

		sprintf(szBuf, OLE2T(m_bstrDisplayFormat), dY);
		::DrawText(hdcDest, szBuf, strlen(szBuf), &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
	}

	if (m_bShowTitle)
	{
		COLORREF clrTitle;
		OleTranslateColor(m_clrTitle, NULL, &clrTitle);
		::SetTextColor(hdcDest, clrTitle);
		
		if (m_spTitleFont != NULL)
		{
			CComPtr<IFont> pFont;
			m_spTitleFont->QueryInterface(IID_IFont, (void**)&pFont);
			pFont->get_hFont(&hFont);
		}
		else
		{
			hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		}
		::SelectObject(hdcDest, hFont);

		rect = m_rect;
		rect.bottom = rect.top - nHalfTickHeight;
		rect.top = m_pChart->m_rect.top;
				
		::DrawText(hdcDest, W2T(m_bstrTitle), -1, &rect, DT_SINGLELINE | DT_RIGHT| DT_BOTTOM);
	}
	
	::SelectObject(hdcDest, hOldPen);
	::DeleteObject(hPen);

	::SetTextColor(hdcDest, clrOldText);
	::SelectObject(hdcDest, hOldFont);
}

void CValueAxis::GetPointForValue(DATAPOINT* value, POINT* point, int nCount)
{
	double dViewMinValue = fabs(m_dViewMinVal - DBL_MAX) < 1E-6 ? m_dMinVal : m_dViewMinVal;
	double dViewMaxValue = fabs(m_dViewMaxVal + DBL_MAX) < 1E-6 ? m_dMaxVal : m_dViewMaxVal;

	for (int i = 0; i < nCount; i++)
		point[i].y = m_rect.bottom - ROUND((value[i].dYVal - dViewMinValue) * (m_rect.bottom - m_rect.top) / (dViewMaxValue - dViewMinValue));  
}

double CValueAxis::GetValueForPos(float fPos)
{
	double dViewMinValue = fabs(m_dViewMinVal - DBL_MAX) < 1E-6 ? m_dMinVal : m_dViewMinVal;
	double dViewMaxValue = fabs(m_dViewMaxVal + DBL_MAX) < 1E-6 ? m_dMaxVal : m_dViewMaxVal;

	return dViewMinValue + (m_rect.bottom - fPos) * (dViewMaxValue - dViewMinValue) / (m_rect.bottom - m_rect.top);
}

////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CTimeAxis::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ITimeAxis,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CTimeAxis::get_NumTicks(long *pVal)
{
	*pVal = m_lNumTicks;

	return S_OK;
}

STDMETHODIMP CTimeAxis::put_NumTicks(long newVal)
{
	m_lNumTicks = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CTimeAxis::get_Title(BSTR *pVal)
{
	*pVal = m_bstrTitle.Copy();

	return S_OK;
}

STDMETHODIMP CTimeAxis::put_Title(BSTR newVal)
{
	m_bstrTitle = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CTimeAxis::get_ShowAxis(VARIANT_BOOL *pVal)
{
	*pVal = m_bShowAxis;

	return S_OK;
}

STDMETHODIMP CTimeAxis::put_ShowAxis(VARIANT_BOOL newVal)
{
	m_bShowAxis = newVal;
	m_pChart->RecalcRects();
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CTimeAxis::get_ShowTitle(VARIANT_BOOL *pVal)
{
	*pVal = m_bShowTitle;
	
	return S_OK;
}

STDMETHODIMP CTimeAxis::put_ShowTitle(VARIANT_BOOL newVal)
{
	m_bShowTitle = newVal;
	m_pChart->RecalcRects();
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CTimeAxis::get_AxisColor(OLE_COLOR *pVal)
{
	*pVal = m_clrAxis;

	return S_OK;
}

STDMETHODIMP CTimeAxis::put_AxisColor(OLE_COLOR newVal)
{
	m_clrAxis = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CTimeAxis::get_TitleColor(OLE_COLOR *pVal)
{
	*pVal = m_clrTitle;

	return S_OK;
}

STDMETHODIMP CTimeAxis::put_TitleColor(OLE_COLOR newVal)
{
	m_clrTitle = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CTimeAxis::get_MaxValue(double *pVal)
{
	*pVal = m_dMaxVal;
	
	return S_OK;
}

STDMETHODIMP CTimeAxis::put_MaxValue(double newVal)
{
// 	m_dMaxVal = AutoScale(newVal, MAX);
	m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CTimeAxis::get_MinValue(double *pVal)
{
	*pVal = m_dMinVal;
	
	return S_OK;
}

STDMETHODIMP CTimeAxis::put_MinValue(double newVal)
{
// 	m_dMinVal = AutoScale(newVal, MIN);
	m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CTimeAxis::Save(LPSTREAM pStm, BOOL fClearDirty)
{
	double dTimeSpan = m_dMaxVal - m_dMinVal;
	pStm->Write(&dTimeSpan, sizeof(double), NULL);

	return CAxis::Save(pStm, fClearDirty);
}

STDMETHODIMP CTimeAxis::Load(LPSTREAM pStm)
{
	double dTimeSpan;
	pStm->Read(&dTimeSpan, sizeof(double), NULL);
	m_dMinVal = m_dMaxVal - dTimeSpan;

	return CAxis::Load(pStm);
}

STDMETHODIMP CTimeAxis::InitNew()
{
	m_bstrTitle = L"时间";
	m_bstrDisplayFormat = L"%Y-%m-%d\\换行%H:%M:%S";
	return CAxis::InitNew();
}

void CTimeAxis::RecalcRect(RECT& rect)
{
	CopyRect(&m_rect, &rect);
	
	if (m_bShowAxis)
	{
		int nTickHeight = GetFontHeight(m_spTickFont);
		int nTitleHeight = GetFontHeight(m_spTitleFont);

		m_rect.top = m_rect.bottom - 
			(m_bShowTitle == VARIANT_TRUE ? nTickHeight + nTitleHeight + 12 : nTickHeight + 10);
		if (wcsstr(m_bstrDisplayFormat, L"\\换行"))
			m_rect.top -= nTickHeight;
	}
	else
	{
		m_rect.top = m_rect.bottom;
	}
		
	rect.bottom = m_rect.top;	
}

void CTimeAxis::DoDraw(HDC hdcDest)
{
	USES_CONVERSION;

	if (m_bShowAxis != VARIANT_TRUE || ::IsRectEmpty(&m_rect))
		return;

	COLORREF clrTitle;
	OleTranslateColor(m_clrTitle, NULL, &clrTitle);
	COLORREF clrAxis;
	OleTranslateColor(m_clrAxis, NULL, &clrAxis);

	COLORREF clrOldText = ::SetTextColor(hdcDest, clrTitle);

	HFONT hFont;
	HFONT hOldFont;
	if (m_spTitleFont != NULL)
	{
		CComPtr<IFont> pFont;
		m_spTitleFont->QueryInterface(IID_IFont, (void**)&pFont);
		pFont->get_hFont(&hFont);
	}
	else
	{
		hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	}
	hOldFont = (HFONT)::SelectObject(hdcDest, hFont);

	RECT rect = m_rect;
	rect.bottom -= 4;
	rect.top = rect.bottom - GetFontHeight(m_spTitleFont) - 2;

	if (m_bShowTitle == VARIANT_TRUE)
		::DrawText(hdcDest, W2T(m_bstrTitle), -1, &rect, DT_SINGLELINE | DT_CENTER| DT_VCENTER);

	::SetTextColor(hdcDest, clrAxis);

	if (m_spTickFont != NULL)
	{
		CComPtr<IFont> pFont;
		m_spTickFont->QueryInterface(IID_IFont, (void**)&pFont);
		pFont->get_hFont(&hFont);
	}
	else
	{
		hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	}
	::SelectObject(hdcDest, hFont);

	HPEN hPen = ::CreatePen(PS_SOLID, 1, clrAxis);
	HGDIOBJ hOldPen = ::SelectObject(hdcDest, hPen);

	::MoveToEx(hdcDest, m_rect.left, m_rect.top, NULL);
	::LineTo(hdcDest, m_rect.right, m_rect.top);

	BOOL bInvert = (m_pChart->m_enumScrollDirection == ScrollDirectionRight);
	int nX = !bInvert ? m_rect.right : m_rect.left;
	
	int nY = m_rect.top;
	float fDX = float(m_rect.right - m_rect.left ) / (m_lNumTicks - 1);

	rect.top = m_rect.top + 8;
	rect.bottom = m_rect.bottom;
	
	int nHalfTickWidth = 32;

	for (int i = 0; i < m_lNumTicks; i++)
	{
		int n = i;
		if (bInvert)
			n = -i;

		int xPos = ROUND(nX - fDX * n);
		if ((!bInvert && xPos < m_rect.left) || (bInvert && xPos > m_rect.right))
			break;

		COleDateTime dtWhen(m_dMaxVal - i * (m_dMaxVal - m_dMinVal) / (m_lNumTicks - 1));
		CString strText = dtWhen.Format(OLE2T(m_bstrDisplayFormat));

		int iIndex = strText.Find(_T("\\换行"));
		if (iIndex != -1)
		{
			CString str = strText.Mid(iIndex + 5);

			if (i == 0)
			{
				SIZE sz;
				::GetTextExtentPoint32(hdcDest, str, str.GetLength(), &sz);	
				nHalfTickWidth = sz.cx / 2;
			}

			str += '\n';

			CString strT = strText.Left(iIndex);
			str += strT;

			if (i == 0)
			{
				SIZE sz;
				::GetTextExtentPoint32(hdcDest, strT, strT.GetLength(), &sz);
				if (nHalfTickWidth < sz.cx / 2)
					nHalfTickWidth = sz.cx / 2;
			}

			strText = str;
		}
		else
		{
			if (i == 0)
			{
				SIZE sz;
				::GetTextExtentPoint32(hdcDest, strText, strText.GetLength(), &sz);	
				nHalfTickWidth = sz.cx / 2;
			}
		}

		nHalfTickWidth += 2;
		rect.left = xPos - nHalfTickWidth;
		rect.right = xPos + nHalfTickWidth;
		
		UINT nFormat = DT_CENTER;

		if (rect.left < m_rect.left)
		{
			OffsetRect(&rect, m_rect.left - 2 - rect.left, 0);
			nFormat = DT_LEFT;
		}
		else if (rect.right > m_rect.right)
		{
			OffsetRect(&rect, m_rect.right + 2 - rect.right , 0);
			nFormat = DT_RIGHT;
		}

		::IntersectRect(&rect, &rect, &m_rect);
		::DrawText(hdcDest, strText, strText.GetLength(), &rect, nFormat);

		::MoveToEx(hdcDest, xPos, nY, NULL);
		::LineTo(hdcDest, xPos, nY + 6);
	}
	
	::SelectObject(hdcDest, hOldPen);
	::DeleteObject(hPen);

	::SetTextColor(hdcDest, clrOldText);
	::SelectObject(hdcDest, hOldFont);
}

void CTimeAxis::ScrollForward(COleDateTimeSpan dtsSpan)
{
//	double dSpan = m_dMaxVal - m_dMinVal;
//
//	m_dMaxVal += (double)dtsSpan;
//	double dNow = (DATE)COleDateTime::GetCurrentTime();
//	if (m_dMaxVal > dNow)
//		m_dMaxVal = dNow;
//	
//	m_dMinVal = m_dMaxVal - dSpan;	
	m_dMaxVal += (double)dtsSpan;
	m_dMinVal += (double)dtsSpan;
}

void CTimeAxis::ScrollBack(COleDateTimeSpan dtsSpan)
{
	m_dMaxVal -= (double)dtsSpan;
	m_dMinVal -= (double)dtsSpan;
}

void CTimeAxis::ScrollTo(COleDateTime dtFrom, COleDateTime dtTo)
{
	if (dtFrom > dtTo)
	{
		COleDateTime dtTemp = dtFrom;
		dtFrom = dtTo;
		dtTo = dtTemp;
	}

	m_dMinVal = dtFrom.m_dt;
	m_dMaxVal = dtTo.m_dt;
}

// 卷动到最后
void CTimeAxis::ScrollForwardMost()
{
	double dSpan = m_dMaxVal - m_dMinVal;

//	DWORD dwCount = m_pSerial->m_values.GetSize();
//	if (dwCount > 0)
//		m_dMaxVal = m_pSerial->m_values[dwCount - 1].dXVal;
//	else
		m_dMaxVal = (DATE)COleDateTime::GetCurrentTime();

	m_dMinVal = m_dMaxVal - dSpan;	
}

void CTimeAxis::GetPointForValue(DATAPOINT* value, POINT* point, int nCount)
{
	if (m_pChart->m_enumScrollDirection == ScrollDirectionLeft)
		for (int i = 0; i < nCount; i++)
			point[i].x = m_rect.left + ROUND((value[i].dXVal - m_dMinVal) * (m_rect.right - m_rect.left) / (m_dMaxVal - m_dMinVal)); 
	else
		for (int i = 0; i < nCount; i++)
			point[i].x = m_rect.right - ROUND((value[i].dXVal - m_dMinVal) * (m_rect.right - m_rect.left) / (m_dMaxVal - m_dMinVal)); 
}

double CTimeAxis::GetValueForPos(float fPos)
{
	if (m_pChart->m_enumScrollDirection == ScrollDirectionLeft)
		return m_dMinVal + (fPos - m_rect.left) * (m_dMaxVal - m_dMinVal) / (m_rect.right - m_rect.left);
	else
		return m_dMinVal + (m_rect.right - fPos) * (m_dMaxVal - m_dMinVal) / (m_rect.right - m_rect.left);
}


STDMETHODIMP CTimeAxis::get_DisplayFormat(BSTR *pVal)
{
	*pVal = m_bstrDisplayFormat.Copy();

	return S_OK;
}

STDMETHODIMP CTimeAxis::put_DisplayFormat(BSTR newVal)
{
	m_bstrDisplayFormat = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CTimeAxis::get_TimeSpan(double *pVal)
{
	*pVal = m_dMaxVal - m_dMinVal;

	return S_OK;
}

STDMETHODIMP CTimeAxis::put_TimeSpan(double newVal)
{
	m_dMinVal = m_dMaxVal - newVal;
	if (m_pChart->m_enumTrendMode == TrendModeHistory)
		m_pChart->OnRefreshData(TRUE);
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CTimeAxis::get_TickFont(IFontDisp** ppFont)
{
	*ppFont = m_spTickFont;
	if (*ppFont != NULL)
		(*ppFont)->AddRef();
	
	return S_OK;
}

STDMETHODIMP CTimeAxis::put_TickFont(IFontDisp* pFont)
{
	if (pFont)
	{
		CComQIPtr<IFont, &IID_IFont> p(pFont);
		if (p)
		{
			// 如果想等即返回
			CComQIPtr<IFont, &IID_IFont> pOld(m_spTickFont);
			if (p->IsEqual(pOld) == S_OK)
				return S_OK;
			
			m_spTickFont = 0;
			
			CComPtr<IFont> pFont;
			p->Clone(&pFont);
			if (pFont)
				pFont->QueryInterface(IID_IFontDisp, (void**) &m_spTickFont);
		}
	}
	else
	{
		if (m_spTickFont == NULL)
			return S_OK;
		
		m_spTickFont = 0;
	}
	
	m_pChart->RecalcRects();
	m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CTimeAxis::putref_TickFont(IFontDisp* pFont)
{
	m_spTickFont = pFont;
	m_pChart->RecalcRects();
	m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CTimeAxis::get_TitleFont(IFontDisp** ppFont)
{
	*ppFont = m_spTitleFont;
	if (*ppFont != NULL)
		(*ppFont)->AddRef();
	
	return S_OK;
}

STDMETHODIMP CTimeAxis::put_TitleFont(IFontDisp* pFont)
{
	if (pFont)
	{
		CComQIPtr<IFont, &IID_IFont> p(pFont);
		if (p)
		{
			// 如果想等即返回
			CComQIPtr<IFont, &IID_IFont> pOld(m_spTitleFont);
			if (p->IsEqual(pOld) == S_OK)
				return S_OK;
			
			m_spTitleFont = 0;
			
			CComPtr<IFont> pFont;
			p->Clone(&pFont);
			if (pFont)
				pFont->QueryInterface(IID_IFontDisp, (void**) &m_spTitleFont);
		}
	}
	else
	{
		if (m_spTitleFont == NULL)
			return S_OK;
		
		m_spTitleFont = 0;
	}
	
	m_pChart->RecalcRects();
	m_pChart->FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CTimeAxis::putref_TitleFont(IFontDisp* pFont)
{
	m_spTitleFont = pFont;
	m_pChart->RecalcRects();
	m_pChart->FireViewChange();
	
	return S_OK;
}



