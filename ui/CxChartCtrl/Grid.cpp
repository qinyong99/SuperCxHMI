// Grid.cpp : Implementation of CCxDrawChartApp and DLL registration.

#include "stdafx.h"
#include "CxChartCtrl.h"
#include "TrendCtrl.h"

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CGrid::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IGrid,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CGrid::get_HorizontalColor(OLE_COLOR *pVal)
{
	*pVal = m_clrHorizontalColor;

	return S_OK;
}

STDMETHODIMP CGrid::put_HorizontalColor(OLE_COLOR newVal)
{
	m_clrHorizontalColor = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CGrid::get_VerticalColor(OLE_COLOR *pVal)
{
	*pVal = m_clrVerticalColor;

	return S_OK;
}

STDMETHODIMP CGrid::put_VerticalColor(OLE_COLOR newVal)
{
	m_clrVerticalColor = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CGrid::get_Width(long *pVal)
{
	*pVal = m_nWidth;

	return S_OK;
}

STDMETHODIMP CGrid::put_Width(long newVal)
{
	m_nWidth = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CGrid::get_HorizontalStyle(enumLineStyle *pVal)
{
	*pVal = m_enumHorizontalStyle;
	
	return S_OK;
}

STDMETHODIMP CGrid::put_HorizontalStyle(enumLineStyle newVal)
{
	m_enumHorizontalStyle = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CGrid::get_VerticalStyle(enumLineStyle *pVal)
{
	*pVal = m_enumVerticalStyle;

	return S_OK;
}

STDMETHODIMP CGrid::put_VerticalStyle(enumLineStyle newVal)
{
	m_enumVerticalStyle = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CGrid::get_NumberOfHorizontalLines(long *pVal)
{
	*pVal = m_lNumberOfVerticalLines;

	return S_OK;
}

STDMETHODIMP CGrid::put_NumberOfHorizontalLines(long newVal)
{
	m_lNumberOfVerticalLines = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CGrid::get_NumberOfVerticalLines(long *pVal)
{
	*pVal = m_lNumberOfHorizontalLines;

	return S_OK;
}

STDMETHODIMP CGrid::put_NumberOfVerticalLines(long newVal)
{
	m_lNumberOfHorizontalLines = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CGrid::get_ShowHorizontal(VARIANT_BOOL *pVal)
{
	*pVal = m_bShowHorizontal;

	return S_OK;
}

STDMETHODIMP CGrid::put_ShowHorizontal(VARIANT_BOOL newVal)
{
	m_bShowHorizontal = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CGrid::get_ShowVertical(VARIANT_BOOL *pVal)
{
	*pVal = m_bShowVertical;	

	return S_OK;
}

STDMETHODIMP CGrid::put_ShowVertical(VARIANT_BOOL newVal)
{
	m_bShowVertical = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CGrid::Save(LPSTREAM pStm, BOOL fClearDirty)
{
	pStm->Write(&m_bShowVertical, sizeof(m_bShowVertical), NULL);
	pStm->Write(&m_bShowHorizontal, sizeof(m_bShowHorizontal), NULL);
	pStm->Write(&m_lNumberOfVerticalLines, sizeof(m_lNumberOfVerticalLines), NULL);
	pStm->Write(&m_lNumberOfHorizontalLines, sizeof(m_lNumberOfHorizontalLines), NULL);
	pStm->Write(&m_enumVerticalStyle, sizeof(m_enumVerticalStyle), NULL);
	pStm->Write(&m_enumHorizontalStyle, sizeof(m_enumHorizontalStyle), NULL);
	pStm->Write(&m_nWidth, sizeof(m_nWidth), NULL);
	pStm->Write(&m_clrVerticalColor, sizeof(m_clrVerticalColor), NULL);
	pStm->Write(&m_clrHorizontalColor, sizeof(m_clrHorizontalColor), NULL);

	return S_OK;
}

STDMETHODIMP CGrid::Load(LPSTREAM pStm)
{
	pStm->Read(&m_bShowVertical, sizeof(m_bShowVertical), NULL);
	pStm->Read(&m_bShowHorizontal, sizeof(m_bShowHorizontal), NULL);
	pStm->Read(&m_lNumberOfVerticalLines, sizeof(m_lNumberOfVerticalLines), NULL);
	pStm->Read(&m_lNumberOfHorizontalLines, sizeof(m_lNumberOfHorizontalLines), NULL);
	pStm->Read(&m_enumVerticalStyle, sizeof(m_enumVerticalStyle), NULL);
	pStm->Read(&m_enumHorizontalStyle, sizeof(m_enumHorizontalStyle), NULL);
	pStm->Read(&m_nWidth, sizeof(m_nWidth), NULL);
	pStm->Read(&m_clrVerticalColor, sizeof(m_clrVerticalColor), NULL);
	pStm->Read(&m_clrHorizontalColor, sizeof(m_clrHorizontalColor), NULL);

	return S_OK;
}

STDMETHODIMP CGrid::InitNew()
{
	m_bShowVertical = VARIANT_TRUE;
	m_bShowHorizontal = VARIANT_TRUE;
	m_lNumberOfVerticalLines = 4;
	m_lNumberOfHorizontalLines = 4;
	m_enumVerticalStyle = LineStyleSolid;
	m_enumHorizontalStyle = LineStyleSolid;
	m_nWidth = 1;
	m_clrVerticalColor = RGB(224 ,224, 224);
	m_clrHorizontalColor = RGB(224 ,224, 224);

	return S_OK;
}

void CGrid::DoDraw(HDC hdcDest, RECT& rect)
{
	if (::IsRectEmpty(&rect))
		return;

	if (m_bShowHorizontal)
	{
		COLORREF clrHorizontalColor;
		OleTranslateColor(m_clrHorizontalColor, NULL, &clrHorizontalColor);

		HPEN hHorPen = ::CreatePen(m_enumHorizontalStyle, m_nWidth, clrHorizontalColor);
		HGDIOBJ hOldPen = ::SelectObject(hdcDest, hHorPen);
		
		float fDY = float(rect.top - rect.bottom) / (m_lNumberOfVerticalLines);
		for (int i = 1; i < m_lNumberOfVerticalLines + 1; i++)
		{
			::MoveToEx(hdcDest, rect.left, ROUND(rect.bottom + i * fDY), NULL);
			::LineTo(hdcDest, rect.right, ROUND(rect.bottom + i * fDY));
		}

		::SelectObject(hdcDest, hOldPen);
		::DeleteObject(hHorPen);
	}

	if (m_bShowVertical)
	{
		COLORREF clrVerticalColor;
		OleTranslateColor(m_clrVerticalColor, NULL, &clrVerticalColor);

		HPEN hVerPen = ::CreatePen(m_enumVerticalStyle, m_nWidth, clrVerticalColor);
		HGDIOBJ hOldPen = ::SelectObject(hdcDest, hVerPen);

		float fDX = float(rect.right - rect.left) / m_lNumberOfHorizontalLines;
		for (int i = 1; i < m_lNumberOfHorizontalLines + 1; i++)
		{
			::MoveToEx(hdcDest, ROUND(rect.left + i * fDX), rect.top, NULL);
			::LineTo(hdcDest, ROUND(rect.left + i * fDX), rect.bottom);
		}

		::SelectObject(hdcDest, hOldPen);
		::DeleteObject(hVerPen);
	}
}
