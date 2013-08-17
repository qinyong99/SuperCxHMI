// DrawTxt.cpp : Implementation of CDrawTxt
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "DrawTxt.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawTxt

HRESULT CDrawTxt::DoDraw(HDC hdcDest)
{
	USES_CONVERSION;

	// 如果字体宽度小于1，无法绘制
	if (m_dFontWidth > 0 && ROUND(m_dFontWidth) == 0)
		return S_OK;

	//设置背景模式
	int iBkMode = m_bsBackStyle == BackStyleTransparent ? TRANSPARENT : OPAQUE;
	int iOldBkMode = SetBkMode(hdcDest, iBkMode);
//	UINT uOldTextAlignMode = SetTextAlign(hdcDest, m_nTextAlign);
	COLORREF colFore;
	OleTranslateColor(m_clrForeColor, NULL, &colFore);
	COLORREF colOldTextColor = SetTextColor(hdcDest, colFore);

	COLORREF colBack;
	OleTranslateColor(m_clrBackColor, NULL, &colBack);
	COLORREF clrOldBackColor = SetBkColor(hdcDest, colBack);

	HFONT hFont = CreateTextFont(ROUND(m_dFontHeight), ROUND(m_dFontWidth));
	HFONT hOldFont = (HFONT)SelectObject(hdcDest, hFont);

	LPTSTR szCaption = W2T(m_bstrCaption);
	int cbCaption = lstrlen(szCaption);
	RECT rcCaption;
	RECTFToRECT(&m_rcPosition, &rcCaption);
	UINT uFormat = DT_NOPREFIX/* | DT_EXTERNALLEADING*/;
	// 考虑到斜体时可能文本超过外围矩形
	if (m_bAutoSize)
		uFormat |= DT_NOCLIP;
	else if (m_bWordWrap)
		uFormat |= DT_WORDBREAK;
	if (m_nTextAlign == TextAlignCenter)
		uFormat |= DT_CENTER;
	else if (m_nTextAlign == TextAlignRight)
		uFormat |= DT_RIGHT;
	else
		uFormat |= DT_LEFT;
	DrawText(hdcDest, szCaption, cbCaption, &rcCaption, uFormat);

	SelectObject(hdcDest, hOldFont);
	DeleteObject(hFont);

	SetBkMode(hdcDest, iOldBkMode);
//	SetTextAlign(hdcDest, uOldTextAlignMode);
	SetTextColor(hdcDest, colOldTextColor);
	SetBkColor(hdcDest, clrOldBackColor);

	return S_OK;

}

STDMETHODIMP CDrawTxt::InitNew()
{
	GetDefaultFontColor(m_clrForeColor);
//	m_clrForeColor = 0x80000000 | COLOR_WINDOWTEXT;
	m_clrBackColor = 0x80000000 | COLOR_WINDOW;
	
	m_bstrFontName = "宋体";
	m_dFontHeight = 15;
	m_dFontWidth = 0;
	m_bStrikethrough = FALSE;
	m_bUnderLine = FALSE;
	m_lWeight = FW_NORMAL;
	m_bItalic = FALSE;

	m_bsBackStyle = BackStyleTransparent;
	m_nTextAlign = TextAlignLeft;
	m_rcPosition.left = m_rcPosition.top = m_rcPosition.right = m_rcPosition.bottom = 0;

	m_bAutoSize = TRUE;
	m_bWordWrap = TRUE;

	return ICxDrawObjectImpl<CDrawTxt>::InitNew();
}

HRESULT CDrawTxt::DoLoad(DWORD dwVer, LPSTREAM pStm)
{
	pStm->Read(&m_enumMousePointer, sizeof(m_enumMousePointer), NULL);
	m_bstrDescription.ReadFromStream(pStm);
	pStm->Read(&m_dFontHeight, sizeof(double), NULL);
	pStm->Read(&m_dFontWidth, sizeof(double), NULL);
	pStm->Read(&m_lWeight, sizeof(long), NULL);
	pStm->Read(&m_bItalic, sizeof(BOOL), NULL);
	pStm->Read(&m_bUnderLine, sizeof(BOOL), NULL);
	pStm->Read(&m_bStrikethrough, sizeof(BOOL), NULL);
	m_bstrFontName.ReadFromStream(pStm);
	m_bstrCaption.ReadFromStream(pStm);
	pStm->Read(&m_clrForeColor, sizeof(OLE_COLOR), NULL);
	pStm->Read(&m_clrBackColor, sizeof(OLE_COLOR), NULL);
	pStm->Read(&m_nTextAlign, sizeof(UINT), NULL);
	pStm->Read(&m_bsBackStyle, sizeof(enumBackStyle), NULL);
	pStm->Read(&m_rcPosition, sizeof(RECTF), NULL);

	if (dwVer > _SUPERCX_VER_0207)
	{
		pStm->Read(&m_bAutoSize, sizeof(BOOL), NULL);
		pStm->Read(&m_bWordWrap, sizeof(BOOL), NULL);
	}
		
	return CCxDrawObjectBase::DoLoad(dwVer, pStm);
}

HRESULT CDrawTxt::DoSave(DWORD dwVer, LPSTREAM pStm)
{
	pStm->Write(&m_enumMousePointer, sizeof(m_enumMousePointer), NULL);
	m_bstrDescription.WriteToStream(pStm);
	pStm->Write(&m_dFontHeight, sizeof(double), NULL);
	pStm->Write(&m_dFontWidth, sizeof(double), NULL);
	pStm->Write(&m_lWeight, sizeof(long), NULL);
	pStm->Write(&m_bItalic, sizeof(BOOL), NULL);
	pStm->Write(&m_bUnderLine, sizeof(BOOL), NULL);
	pStm->Write(&m_bStrikethrough, sizeof(BOOL), NULL);
	m_bstrFontName.WriteToStream(pStm);
	m_bstrCaption.WriteToStream(pStm);
	pStm->Write(&m_clrForeColor, sizeof(OLE_COLOR), NULL);
	pStm->Write(&m_clrBackColor, sizeof(OLE_COLOR), NULL);
	pStm->Write(&m_nTextAlign, sizeof(UINT), NULL);
	pStm->Write(&m_bsBackStyle, sizeof(enumBackStyle), NULL);
	pStm->Write(&m_rcPosition, sizeof(RECTF), NULL);

	if (dwVer > _SUPERCX_VER_0207)
	{
		pStm->Write(&m_bAutoSize, sizeof(BOOL), NULL);
		pStm->Write(&m_bWordWrap, sizeof(BOOL), NULL);
	}

	return CCxDrawObjectBase::DoSave(dwVer, pStm);
}

STDMETHODIMP CDrawTxt::SaveOldVer(LPSTREAM pStm, BOOL fClearDirty)
{
//	pStm->Write(&m_bAutoSize, sizeof(BOOL), NULL);
	pStm->Write(&m_dFontHeight, sizeof(double), NULL);
	pStm->Write(&m_dFontWidth, sizeof(double), NULL);
	pStm->Write(&m_lWeight, sizeof(long), NULL);
	pStm->Write(&m_bItalic, sizeof(BOOL), NULL);
	pStm->Write(&m_bUnderLine, sizeof(BOOL), NULL);
	pStm->Write(&m_bStrikethrough, sizeof(BOOL), NULL);
	m_bstrFontName.WriteToStream(pStm);
	m_bstrCaption.WriteToStream(pStm);
	pStm->Write(&m_clrForeColor, sizeof(OLE_COLOR), NULL);
	pStm->Write(&m_clrBackColor, sizeof(OLE_COLOR), NULL);
	pStm->Write(&m_nTextAlign, sizeof(UINT), NULL);
	pStm->Write(&m_bsBackStyle, sizeof(enumBackStyle), NULL);
	pStm->Write(&m_rcPosition, sizeof(RECTF), NULL);
	
	return ICxDrawObjectImpl<CDrawTxt>::SaveOldVer(pStm, fClearDirty);
}

STDMETHODIMP CDrawTxt::LoadOldVer(LPSTREAM pStm)
{
//	pStm->Read(&m_bAutoSize, sizeof(BOOL), NULL);
	pStm->Read(&m_dFontHeight, sizeof(double), NULL);
	pStm->Read(&m_dFontWidth, sizeof(double), NULL);
	pStm->Read(&m_lWeight, sizeof(long), NULL);
	pStm->Read(&m_bItalic, sizeof(BOOL), NULL);
	pStm->Read(&m_bUnderLine, sizeof(BOOL), NULL);
	pStm->Read(&m_bStrikethrough, sizeof(BOOL), NULL);
	m_bstrFontName.ReadFromStream(pStm);
	m_bstrCaption.ReadFromStream(pStm);
	pStm->Read(&m_clrForeColor, sizeof(OLE_COLOR), NULL);
	pStm->Read(&m_clrBackColor, sizeof(OLE_COLOR), NULL);
	pStm->Read(&m_nTextAlign, sizeof(UINT), NULL);
	pStm->Read(&m_bsBackStyle, sizeof(enumBackStyle), NULL);
	pStm->Read(&m_rcPosition, sizeof(RECTF), NULL);

	return ICxDrawObjectImpl<CDrawTxt>::LoadOldVer(pStm);
}

STDMETHODIMP CDrawTxt::get_Caption(BSTR *pVal)
{
	*pVal = m_bstrCaption.Copy();

	return S_OK;
}


STDMETHODIMP CDrawTxt::put_Caption(BSTR newVal)
{
	if (m_bstrCaption == newVal)
		return S_OK;
	
	FireViewChange();
	m_bstrCaption = newVal;
	ReCalcBound();
	FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CDrawTxt::get_Alignment(enumTextAlign *pVal)
{
	*pVal = m_nTextAlign;

	return S_OK;
}

STDMETHODIMP CDrawTxt::put_Alignment(enumTextAlign newVal)
{
	if (m_nTextAlign == newVal)
		return S_OK;

	m_nTextAlign = newVal;

	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawTxt::get_FontName(CXFONTNAME *pVal)
{
	*pVal = m_bstrFontName.Copy();

	return S_OK;
}

STDMETHODIMP CDrawTxt::put_FontName(CXFONTNAME newVal)
{
	if (m_bstrFontName == newVal)
		return S_OK;

	FireViewChange();
	m_bstrFontName = newVal;
	ReCalcBound();
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawTxt::get_FontSize(long *pVal)
{
	HDC hDC = ::GetDC(NULL);
	*pVal = MulDiv(ROUND(m_dFontHeight), 72, GetDeviceCaps(hDC, LOGPIXELSY));
	::ReleaseDC(NULL, hDC);

	return S_OK; 
}

//暂时以象素为单位
STDMETHODIMP CDrawTxt::put_FontSize(long newVal)
{
	FireViewChange();

	HDC hDC = ::GetDC(NULL);
	m_dFontHeight = MulDiv(newVal, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	m_dFontWidth = 0;
	::ReleaseDC(NULL, hDC);
	
	ReCalcBound();
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawTxt::get_AutoSize(VARIANT_BOOL *pVal)
{
	*pVal = m_bAutoSize ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CDrawTxt::put_AutoSize(VARIANT_BOOL newVal)
{
	m_bAutoSize = newVal != VARIANT_FALSE;
	FireViewChange();
	ReCalcBound();
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawTxt::get_WordWrap(VARIANT_BOOL *pVal)
{
	*pVal = m_bWordWrap ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CDrawTxt::put_WordWrap(VARIANT_BOOL newVal)
{
	m_bWordWrap = newVal != VARIANT_FALSE;
	FireViewChange();
	ReCalcBound();
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawTxt::get_FontStyle(enumFontStyle *pVal)
{
	if ((m_lWeight == FW_BOLD && m_bItalic == TRUE))
		*pVal = FontStyleBoldItalic;
	else if (m_lWeight == FW_BOLD)
		*pVal = FontStyleBold;
	else if (m_bItalic == TRUE)
		*pVal = FontStyleItalic;
	else 
		*pVal = FontStyleRegular;

	return S_OK;
}

STDMETHODIMP CDrawTxt::put_FontStyle(enumFontStyle newVal)
{
	FireViewChange();
	m_lWeight = (newVal == FontStyleBold || newVal == FontStyleBoldItalic) ? FW_BOLD : FW_NORMAL;
	m_bItalic = (newVal == FontStyleItalic || newVal == FontStyleBoldItalic);
	ReCalcBound();
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawTxt::get_StrikeThrough(VARIANT_BOOL *pVal)
{
	*pVal = m_bStrikethrough ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CDrawTxt::put_StrikeThrough(VARIANT_BOOL newVal)
{
	BOOL bStrikethrough = (newVal != VARIANT_FALSE);
	if (m_bStrikethrough == bStrikethrough)
		return S_OK;

	FireViewChange();
	m_bStrikethrough = bStrikethrough;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawTxt::get_UnderLine(VARIANT_BOOL *pVal)
{
	*pVal = m_bUnderLine ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CDrawTxt::put_UnderLine(VARIANT_BOOL newVal)
{
	BOOL bUnderLine = (newVal != VARIANT_FALSE);
	if (m_bUnderLine == bUnderLine)
		return S_OK;

	FireViewChange();
	m_bUnderLine = bUnderLine;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawTxt::get_BackStyle(enumBackStyle *pVal)
{
	*pVal = m_bsBackStyle;
	return S_OK;
}

STDMETHODIMP CDrawTxt::put_BackStyle(enumBackStyle newVal)
{
	if (m_bsBackStyle == newVal)
		return S_OK;

	m_bsBackStyle = newVal;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawTxt::get_BackColor(OLE_COLOR *pVal)
{
	*pVal = m_clrBackColor;
	return S_OK;
}

STDMETHODIMP CDrawTxt::put_BackColor(OLE_COLOR newVal)
{
	if (m_clrBackColor == newVal)
		return S_OK;

	m_clrBackColor = newVal;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawTxt::GetHandleCount(long* pHandleCount)
{
	*pHandleCount = 0;
	return S_OK;
}

STDMETHODIMP CDrawTxt::get_FontColor(OLE_COLOR *pVal)
{
	*pVal = m_clrForeColor;
	return S_OK;
}

STDMETHODIMP CDrawTxt::put_FontColor(OLE_COLOR newVal)
{
	if (m_clrForeColor == newVal)
		return S_OK;

	m_clrForeColor = newVal;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawTxt::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("Text");
	return S_OK;
}

//根据当前的文本和字体计算边界
void CDrawTxt::ReCalcBound()
{
	if (!m_bAutoSize)
		return;

	RECTF rcCaption;
	rcCaption.left = rcCaption.top = rcCaption.right = rcCaption.bottom = 0;
	CalculateBound(rcCaption);
	m_rcPosition.bottom = m_rcPosition.top + rcCaption.bottom;
	
	switch (m_nTextAlign) 
	{
	case DT_CENTER:
		m_rcPosition.left = (m_rcPosition.left + m_rcPosition.right - rcCaption.right) / 2;
		m_rcPosition.right = m_rcPosition.left + rcCaption.right;
		break;
	case DT_RIGHT:
		m_rcPosition.left = m_rcPosition.right - rcCaption.right;
		break;
	default:
		m_rcPosition.right = m_rcPosition.left + rcCaption.right;
		break;
	}	
}

//根据当前的文本和字体计算边界
void CDrawTxt::CalculateBound(RECTF& rcCaption)
{
	USES_CONVERSION;

	//创建设备上下文
	HDC hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);

	HFONT hFont = CreateTextFont(ROUND(m_dFontHeight), ROUND(m_dFontWidth));	
	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	LPTSTR szCaption = W2T(m_bstrCaption);
	int cbCaption = lstrlen(szCaption);
	RECT rcTemp;
	RECTFToRECT(&rcCaption, &rcTemp);
	UINT uFormat = DT_CALCRECT|DT_NOPREFIX;
	if (m_bAutoSize)
		uFormat |= DT_NOCLIP;
	else if (m_bWordWrap)
		uFormat |= DT_WORDBREAK;
	if (m_nTextAlign == TextAlignCenter)
		uFormat |= DT_CENTER;
	else if (m_nTextAlign == TextAlignRight)
		uFormat |= DT_RIGHT;
	else
		uFormat |= DT_LEFT;
	DrawText(hDC, szCaption, cbCaption, &rcTemp, uFormat);
	rcCaption.left = (float)rcTemp.left;
	rcCaption.top = (float)rcTemp.top;
	rcCaption.right = (float)rcTemp.right;
	rcCaption.bottom = (float)rcTemp.bottom;

	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);

	DeleteDC(hDC);
}

//根据边界计算字体尺寸
void CDrawTxt::ReCalcFontSize(double& dFontHeight, double& dFontWidth)
{
	RECTF rcCaption;
	rcCaption.left = rcCaption.top = rcCaption.right = rcCaption.bottom = 0;
	CalculateBound(rcCaption);

	if (ROUND(dFontWidth) == 0)
		dFontWidth = GetAveCharWidth(ROUND(dFontHeight));

	dFontHeight = (m_rcPosition.bottom - m_rcPosition.top) * dFontHeight / (rcCaption.bottom - rcCaption.top);
	dFontWidth = (m_rcPosition.right - m_rcPosition.left) * dFontWidth / (rcCaption.right - rcCaption.left);
	// 不要使字体宽度为0
	if (dFontWidth < 0.01)
		dFontWidth = 0.01;
}

HFONT CDrawTxt::CreateTextFont(int nHeight, int nWidth)
{
	USES_CONVERSION;

	//创建字体
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight = nHeight;
	logFont.lfWidth = nWidth;
	logFont.lfWeight = m_lWeight;
	logFont.lfItalic = m_bItalic;
	logFont.lfUnderline = m_bUnderLine;
	logFont.lfStrikeOut = m_bStrikethrough;
	LPTSTR szFontName = W2T(m_bstrFontName);
	int cbFontName = lstrlen(szFontName);
	lstrcpyn(logFont.lfFaceName, szFontName, cbFontName + 1);
	return CreateFontIndirect(&logFont);	
}

int CDrawTxt::GetAveCharWidth(int nFontHeight)
{
	//创建设备上下文
	HDC hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	HFONT hFont = CreateTextFont(ROUND(m_dFontHeight), 0);	
	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	TEXTMETRIC tm;
	GetTextMetrics(hDC, &tm);

	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
	
	DeleteDC(hDC);

	return tm.tmAveCharWidth;
}

STDMETHODIMP CDrawTxt::GetCharPosition(long lIndex, long *plX, long *plY)
{
	USES_CONVERSION;

	RECT rcCaption;
	RECTFToRECT(&m_rcPosition, &rcCaption);

	//创建设备上下文
	HDC hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);

	HFONT hFont = CreateTextFont(ROUND(m_dFontHeight), ROUND(m_dFontWidth));
	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	LPTSTR szCaption = W2T(m_bstrCaption);
	int cbCaption = lstrlen(szCaption);
	DrawText(hDC, szCaption, cbCaption, &rcCaption, DT_LEFT|DT_NOCLIP|DT_CALCRECT|DT_NOPREFIX);

	int nTotalLine = 1;
	int nCharLine = 1;
	TCHAR* pChar = szCaption;
	TCHAR* pLineShartChar = pChar;
	TCHAR* pLineEndChar = pChar;
	if (pChar == NULL || *pChar == 0)
	{
		*plX = rcCaption.left;
		*plY = rcCaption.top;
		return S_OK;
	}
	for (int i = 0; i < cbCaption; i++)
	{
		if (*pChar == 10)
		{
		//	if (*(pChar + 1) != 0)
			nTotalLine++;
			pChar++;
			if (lIndex == -1 || i < lIndex)
			{
				nCharLine++;
				pLineShartChar = pLineEndChar = pChar;
			}
			continue;
		}
		pChar++;
		if (lIndex == -1 || i <= lIndex)
			pLineEndChar = pChar;
	}

	*plY = long(rcCaption.top + (rcCaption.bottom - rcCaption.top) * (nCharLine - 1) / nTotalLine);
	if (pLineEndChar == pLineShartChar)
	{
		*plX = rcCaption.left;
	}
	else
	{
		DrawText(hDC, pLineShartChar, pLineEndChar - pLineShartChar, &rcCaption, DT_LEFT|DT_NOCLIP|DT_CALCRECT|DT_NOPREFIX);
		*plX = rcCaption.right;
	}

	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);

	DeleteDC(hDC);

	return S_OK;
}

STDMETHODIMP CDrawTxt::get_HorizontalPosition(float *pVal)
{
	*pVal = m_rcPosition.left;

	return S_OK;
}

STDMETHODIMP CDrawTxt::put_HorizontalPosition(float newVal)
{
	if (m_rcPosition.left == newVal)
		return S_OK;

	FireViewChange();
	m_rcPosition.right = m_rcPosition.right - m_rcPosition.left + newVal;
	m_rcPosition.left = newVal;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawTxt::get_VerticalPosition(float *pVal)
{
	*pVal = m_rcPosition.top;

	return S_OK;
}

STDMETHODIMP CDrawTxt::put_VerticalPosition(float newVal)
{
	if (m_rcPosition.top == newVal)
		return S_OK;

	FireViewChange();
	m_rcPosition.bottom = m_rcPosition.bottom - m_rcPosition.top + newVal;
	m_rcPosition.top = newVal;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawTxt::get_Width(float *pVal)
{
	*pVal = m_rcPosition.right - m_rcPosition.left;

	return S_OK;
}

STDMETHODIMP CDrawTxt::put_Width(float newVal)
{
	if (newVal < 0)
		return S_FALSE;
	
	if (m_rcPosition.right == m_rcPosition.left + newVal)
		return S_OK;
	
	FireViewChange();
	m_rcPosition.right = m_rcPosition.left + newVal;
	if (m_bAutoSize)
		ReCalcFontSize(m_dFontHeight, m_dFontWidth);
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawTxt::get_Height(float *pVal)
{
	*pVal = m_rcPosition.bottom - m_rcPosition.top;

	return S_OK;
}

STDMETHODIMP CDrawTxt::put_Height(float newVal)
{
	if (newVal < 0)
		return S_FALSE;
	
	if (m_rcPosition.bottom == m_rcPosition.top + newVal)
		return S_OK;
	
	FireViewChange();
	m_rcPosition.bottom = m_rcPosition.top + newVal;
	if (m_bAutoSize)
		ReCalcFontSize(m_dFontHeight, m_dFontWidth);
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawTxt::get_IdealHeight(/*[out, retval]*/ float *pVal)
{
	RECTF rcCaption = m_rcPosition;
	CalculateBound(rcCaption);	
	
	*pVal = rcCaption.bottom - rcCaption.top;

	return S_OK;
}

HRESULT CDrawTxt::ICxDrawObject_GetPositionRect(RECTF* pPositionRect)
{
	*pPositionRect = m_rcPosition;
	return S_OK;
}

STDMETHODIMP CDrawTxt::GetBoundingRect(RECT* pBoundingRect)
{
	USES_CONVERSION;

	RECT rcCaption;
	RECTFToRECT(&m_rcPosition, &rcCaption);

	// 矫正斜体出界的情况, 计算10次大概耗时1毫秒

/*
	if (m_bItalic)
	{
		//创建设备上下文
		HDC hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
		
		HFONT hFont = CreateTextFont(ROUND(m_dFontHeight), ROUND(m_dFontWidth));	
		HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
		LPTSTR szCaption = W2T(m_bstrCaption);
		int cbCaption = lstrlen(szCaption);
		UINT uFormat = DT_NOCLIP|DT_CALCRECT|DT_NOPREFIX;
		if (m_nTextAlign == TextAlignCenter)
			uFormat |= DT_CENTER;
		else if (m_nTextAlign == TextAlignRight)
			uFormat |= DT_RIGHT;
		else
			uFormat |= DT_LEFT;
		
		int nLeftMargin = 0;
		int nRightMargine = 0;
		
		OLECHAR* pChar = m_bstrCaption;
		OLECHAR* pLineShartChar = pChar;
		if (pChar != NULL && *pChar != 0)
		{
			int nMaxLineWidth = 0;
			while (1)
			{
				// 如果是换行符
				if (*pChar == 13 || *pChar == 10 || *pChar == 0)
				{
					if (pChar > pLineShartChar)
					{
						RECT rcT = rcCaption;
						CComBSTR bstrT(pChar - pLineShartChar, pLineShartChar);
						DrawText(hDC, OLE2T(bstrT), -1, &rcT, uFormat);
						ABCFLOAT abcfloat;
						GetCharABCWidthsFloatW(hDC, *pLineShartChar, *pLineShartChar, &abcfloat);
						int A = ROUND(abcfloat.abcfA);
						GetCharABCWidthsFloatW(hDC, *(pChar - 1), *(pChar - 1), &abcfloat);
						int C = ROUND(abcfloat.abcfC);
						if (A < 0)
							rcT.left += A;
						
						if (C < 0)
							rcT.right -= C;    // two minus' make a plus
						
						if (nLeftMargin < rcCaption.left - rcT.left)
							nLeftMargin = rcCaption.left - rcT.left;
						if (nRightMargine < rcT.right - rcCaption.right)
							nRightMargine = rcT.right - rcCaption.right;
					}
					
					if (*pChar == 0)
						break;
					
					if (*(pChar + 1) == 10 || *(pChar + 1) == 13)
						pChar++;
					
					pChar++;
					pLineShartChar = pChar;
				}
				else
				{
					pChar++;
				}
			}
		}
		
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);
		
		DeleteDC(hDC);
		
		rcCaption.left -= nLeftMargin;
		rcCaption.right += nRightMargine;
	}*/

	int nDX = ROUND(m_dFontWidth);
	if (nDX == 0)
		nDX = ROUND(m_dFontHeight);
	InflateRect(&rcCaption, nDX, 1);
	
	*pBoundingRect = rcCaption;

	return S_OK;
}

STDMETHODIMP CDrawTxt::MoveHandleTo(long nHandle, POINT point)
{
	DefaultMoveHandleTo(&m_rcPosition, -nHandle, point);
	NormalizeRect(&m_rcPosition);
	if (m_bAutoSize)
		ReCalcFontSize(m_dFontHeight, m_dFontWidth);

	return S_OK;
}

HRESULT CDrawTxt::ICxDrawObject_MoveTo(RECTF rect, BOOL bUniformScale)
{
	BOOL bNeedCalcFontSize = !(m_rcPosition.right - m_rcPosition.left == rect.right - rect.left &&
		m_rcPosition.bottom - m_rcPosition.top == rect.bottom - rect.top);
		
	m_rcPosition = rect;
	NormalizeRect(&m_rcPosition);
	
	if (bNeedCalcFontSize && m_bAutoSize)
		ReCalcFontSize(m_dFontHeight, m_dFontWidth);
		
	return 0;
}

STDMETHODIMP CDrawTxt::HitTest(POINT point, BOOL* pbHitTest)
{
	RECT fixed;
	RECTFToRECT(&m_rcPosition, &fixed);
	*pbHitTest = PtInRect(&fixed, point);

	return 0;
}

STDMETHODIMP CDrawTxt::Intersects(RECT rect, BOOL* pbIntersects)
{
	RECT fixed;
	
	fixed.left = ROUND(m_rcPosition.left);
	fixed.top = ROUND(m_rcPosition.top);
	fixed.right = ROUND(m_rcPosition.right);
	fixed.bottom = ROUND(m_rcPosition.bottom);
	
	fixed.right++;
	fixed.bottom++;
	RECT temp;
	IntersectRect(&temp, &fixed, &rect);
	if (IsRectEmpty(&temp))
		*pbIntersects = FALSE;
	else
		*pbIntersects = TRUE;
	
	return S_OK;
}

STDMETHODIMP CDrawTxt::RotateAt(POINTF point, float fAngle)
{
	RECTF rcPosition = m_rcPosition;
	POINTF ptCenter;
	ptCenter.x = (rcPosition.left + rcPosition.right) / 2;
	ptCenter.y = (rcPosition.top + rcPosition.bottom) / 2;
	POINTF ptNew = CalculateRotatePoint(point, ptCenter, fAngle);
	rcPosition.left += ptNew.x - ptCenter.x;
	rcPosition.right += ptNew.x - ptCenter.x;
	rcPosition.top += ptNew.y - ptCenter.y;
	rcPosition.bottom += ptNew.y - ptCenter.y;
	MoveTo(rcPosition, FALSE);
		
	return S_OK;
}

STDMETHODIMP CDrawTxt::ScaleAt(POINTF point, float fHorScale, float fVerScale)
{
	RECTF rcPosition = m_rcPosition;
	rcPosition.left += -point.x;
	rcPosition.right += -point.x;
	rcPosition.top += -point.x;
	rcPosition.bottom += -point.y;
	rcPosition.left = rcPosition.left * fHorScale;
	rcPosition.right = rcPosition.right * fHorScale;
	rcPosition.top = rcPosition.top * fVerScale;
	rcPosition.bottom  = rcPosition.bottom * fVerScale;
	rcPosition.left += point.x;
	rcPosition.right += point.x;
	rcPosition.top += point.x;
	rcPosition.bottom += point.y;
	MoveTo(rcPosition, FALSE);

	return S_OK;
}

STDMETHODIMP CDrawTxt::DrawRotateAtShape(HDC hdcDest, POINTF point, long lAngle)
{
	POINTF ptCenter;
	RECTF rcPosition = m_rcPosition;
	ptCenter.x = (rcPosition.left + rcPosition.right) / 2;
	ptCenter.y = (rcPosition.top + rcPosition.bottom) / 2;
	POINTF ptNew = CalculateRotatePoint(point, ptCenter, float(lAngle));
	rcPosition.left += ptNew.x - ptCenter.x;
	rcPosition.right += ptNew.x - ptCenter.x;
	rcPosition.top += ptNew.y - ptCenter.y;
	rcPosition.bottom += ptNew.y - ptCenter.y;
	::Rectangle(hdcDest, ROUND(rcPosition.left), ROUND(rcPosition.top), ROUND(rcPosition.right), ROUND(rcPosition.bottom));

	return S_OK;
}

STDMETHODIMP CDrawTxt::DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
{
	RECTF rect = m_rcPosition;
	DefaultMoveHandleTo(&rect, -nHandle, point);
	::Rectangle(hdcDest, ROUND(rect.left), ROUND(rect.top), ROUND(rect.right), ROUND(rect.bottom));
	return S_OK;
}

STDMETHODIMP CDrawTxt::DrawTrackerShape(HDC hdcDest, POINT* pPoints, int nCount)
{
	RECT rect;
	rect.left = ROUND(m_rcPosition.left);
	rect.top = ROUND(m_rcPosition.top);
	rect.right = ROUND(m_rcPosition.right);
	rect.bottom = ROUND(m_rcPosition.bottom);
	
	MoveToEx(hdcDest, rect.left, rect.top, NULL);
	LineTo(hdcDest, rect.right, rect.top);
	LineTo(hdcDest, rect.right, rect.bottom);
	LineTo(hdcDest, rect.left, rect.bottom);
	LineTo(hdcDest, rect.left, rect.top);

	return S_OK;
}