// RectTool.cpp : Implementation of CRectTool
#include "stdafx.h"
#include "CxDrawTools.h"
#include "RectTool.h"

#include "DTGUID.h"

const CLSID CLSID_DrawChart = {0xEF34F70F,0x78B8,0x4A70,{0x93,0x7C,0x38,0x02,0x44,0x52,0x4C,0x67}};
const CLSID CLSID_DrawAlmSum = {0x967B0FC2,0x0761,0x4D0F,{0x86,0x5F,0x0C,0xCB,0x7B,0xA3,0x4F,0x3F}};

/////////////////////////////////////////////////////////////////////////////
// CRectTool

STDMETHODIMP CRectTool::DoLButtonDown(UINT nFlags, POINT point)
{ 
	if (m_pDrawObject)
		return S_OK;
	
//	HWND hWnd;
//	m_pDrawToolSite->GetWindow(&hWnd);
//	UpdateWindow(hWnd);
	CaptureMouse();

	DRAW_SHAPE drawShape;
	m_pDrawToolSite->GetDrawShape(&drawShape);
	
	HRESULT hr;
	
	CLSID clsid = CLSID_NULL;
	switch (drawShape)
	{
	case DRAW_LINE:
		clsid = CLSID_DrawLine;
		break;

	case DRAW_RECTANGLE:
		clsid = CLSID_DrawRectangle;
		break;

	case DRAW_ROUNDRECT:
		clsid = CLSID_DrawRoundRectangle;
		break;

	case DRAW_ELLIPSE:
		clsid = CLSID_DrawEllipse;
		break;

	case DRAW_VARIABLE:
		clsid = CLSID_DrawVariable;
		break;

	case DRAW_BUTTON:
		clsid = CLSID_DrawButton;
		break;

	case DRAW_MENU:
		clsid = CLSID_DrawMenu;
		break;

	case DRAW_TIMER:
		clsid = CLSID_DrawTimer;
		break;

	case DRAW_CHART:
		clsid = CLSID_DrawChart;
		break;

	case DRAW_ALMSUM:
		clsid = CLSID_DrawAlmSum;
		break;
	
	default:
		return S_FALSE;
	}
	
	if (clsid == CLSID_NULL)
		return S_FALSE;

	IUnknown* pUnknown = NULL;
	m_pDrawToolSite->GetDrawObject(clsid, &pUnknown);
	hr = pUnknown->QueryInterface(IID_ICxDrawObject, (void**)&m_pDrawObject);
	if (FAILED(hr))
	{
		return hr;
	}
	m_ptPreMove = point;
	m_rect.left = m_rect.right = (float)point.x;
	m_rect.top = m_rect.bottom = (float)point.y;

	m_pDrawObject->MoveTo(m_rect, FALSE);

	m_bIsMouseMoved = FALSE;

//	HDC hdcDest;
//	m_pDrawToolSite->GetDC(&hdcDest);
//	::SetROP2(hdcDest, R2_NOTXORPEN);
//	m_pDrawObject->DrawTrackerShape(hdcDest, NULL, 0);
//	m_pDrawToolSite->ReleaseDC(hdcDest);

	return S_OK;
}

STDMETHODIMP CRectTool::DoLButtonUp(UINT nFlags, POINT point)
{
	if (m_pDrawObject == NULL)
		return S_OK;
	
	if (m_bIsMouseMoved == false)
		return S_OK;

	HDC hdcDest;
	m_pDrawToolSite->GetDC(&hdcDest);
	::SetROP2(hdcDest, R2_NOTXORPEN);

	DRAW_SHAPE drawShape;
	m_pDrawToolSite->GetDrawShape(&drawShape);
	if (drawShape == DRAW_LINE)
	{
		m_pDrawObject->DrawTrackerHandleShape(hdcDest, 2, m_ptPreMove);
		m_ptPreMove = point;
		POINT ptOrigin;
		ptOrigin.x = (int)m_rect.left;
		ptOrigin.y = (int)m_rect.top;
		if ((nFlags & MK_CONTROL) != 0)
			Calc30Or45DegreeDuploidPoint(ptOrigin, m_ptPreMove);
		m_pDrawObject->MoveHandleTo(2, m_ptPreMove);
	}
	else
	{
		m_pDrawObject->DrawTrackerShape(hdcDest, NULL, 0);
	}
	m_pDrawToolSite->ReleaseDC(hdcDest);


//	HDC hdcDest;
//	m_pDrawToolSite->GetDC(&hdcDest);
//	::SetROP2(hdcDest, R2_NOP);
//	::SetROP2(hdcDest, R2_NOTXORPEN);
//	m_pDrawObject->DrawTrackerShape(hdcDest, NULL, 0);
//	m_pDrawToolSite->ReleaseDC(hdcDest);

	ReleaseMouse();
	Fire_OnCreate();

	m_pDrawObject->Release();
	m_pDrawObject = NULL;
	m_bIsMouseMoved = false;

	return S_OK;
}

STDMETHODIMP CRectTool::DoMouseMove(UINT nFlags, POINT point)
{
	SetCursor(LoadCursor(NULL, IDC_CROSS));
	
	if (m_pDrawObject == NULL)
		return S_OK;
	
	HDC hdcDest;
	m_pDrawToolSite->GetDC(&hdcDest);
	::SetROP2(hdcDest, R2_NOTXORPEN);
	
	DRAW_SHAPE drawShape;
	m_pDrawToolSite->GetDrawShape(&drawShape);
	
	if (drawShape == DRAW_LINE)
	{
		if (m_bIsMouseMoved)
			m_pDrawObject->DrawTrackerHandleShape(hdcDest, 2, m_ptPreMove);
		m_ptPreMove = point;
		POINT ptOrigin;
		ptOrigin.x = (int)m_rect.left;
		ptOrigin.y = (int)m_rect.top;
		if ((nFlags & MK_CONTROL) != 0)
			Calc30Or45DegreeDuploidPoint(ptOrigin, m_ptPreMove);
		m_pDrawObject->DrawTrackerHandleShape(hdcDest, 2, m_ptPreMove);
	}
	else
	{
		m_rect.right = (float)point.x;
		m_rect.bottom = (float)point.y;

		if ((nFlags & MK_CONTROL) != 0)\
		{
			float fDX = (float)fabs(m_rect.right - m_rect.left);
			float fDY = (float)fabs(m_rect.bottom - m_rect.top);
		
			if (fDX > fDY)
				m_rect.bottom = m_rect.top + (m_rect.bottom > m_rect.top ? fDX : -fDX);
			else
				m_rect.right = m_rect.left + (m_rect.right > m_rect.left ? fDY : -fDY);
		}

		RECTF rcTemp = m_rect;
		if (m_rect.right < m_rect.left)
		{
			rcTemp.left = m_rect.right;
			rcTemp.right = m_rect.left;
		}
		if (m_rect.bottom < m_rect.top)
		{
			rcTemp.top = m_rect.bottom;
			rcTemp.bottom = m_rect.top;
		}

		if (m_bIsMouseMoved)
			m_pDrawObject->DrawTrackerShape(hdcDest, NULL, 0);
		m_pDrawObject->MoveTo(rcTemp, FALSE);
		m_pDrawObject->DrawTrackerShape(hdcDest, NULL, 0);
	}
	
	m_pDrawToolSite->ReleaseDC(hdcDest);
	m_bIsMouseMoved = true;

	return S_OK;	
}

STDMETHODIMP CRectTool::DoCancel()
{
	if (m_pDrawObject == NULL)
		return S_OK;

//	HDC hdcDest;
//	m_pDrawToolSite->GetDC(&hdcDest);
//	::SetROP2(hdcDest, R2_NOTXORPEN);
//	m_pDrawObject->DrawTrackerShape(hdcDest, NULL, 0);
//	m_pDrawToolSite->ReleaseDC(hdcDest);

	ReleaseMouse();

	Fire_OnCancel();
	m_pDrawObject->Release();
	m_pDrawObject = NULL;

	return S_OK;
}
