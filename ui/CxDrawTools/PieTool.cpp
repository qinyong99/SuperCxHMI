// PieTool.cpp : Implementation of CPieTool
#include "stdafx.h"
#include "CxDrawTools.h"
#include "PieTool.h"

#include "math.h"

#define MIN_LONG 0x80000000
const double PI = 3.1415926;


/////////////////////////////////////////////////////////////////////////////
// CPieTool
CPieTool::CPieTool()
{
	m_bIsFirstMove3thPt = true;
	m_nPoint = 0;
}

STDMETHODIMP CPieTool::DoMouseMove(UINT nFlags, POINT point)
{
	SetCursor(LoadCursor(NULL, IDC_CROSS));

	if (m_pDrawObject == NULL)
		return S_OK;
	
	HDC hdcDest;
	m_pDrawToolSite->GetDC(&hdcDest);
	::SetROP2(hdcDest, R2_NOTXORPEN);

	DRAW_SHAPE drawShape;
	m_pDrawToolSite->GetDrawShape(&drawShape);
	
	if (m_nPoint == 1)
	{
		m_pDrawObject->DrawTrackerShape(hdcDest, m_points, 2);
		m_points[1] = point;
		if ((nFlags & MK_CONTROL) != 0)
			Calc30Or45DegreeDuploidPoint(m_points[0], m_points[1]);
		m_pDrawObject->DrawTrackerShape(hdcDest, m_points, 2);
	}
	else if (m_nPoint == 2)
	{
		if (!m_bIsFirstMove3thPt)
			m_pDrawObject->DrawTrackerShape(hdcDest, m_points, 3);
		else
			m_pDrawObject->DrawTrackerShape(hdcDest, m_points, 2);
		m_points[2] = point;
		if ((nFlags & MK_CONTROL) != 0 && drawShape == DRAW_PIE)
			Calc30Or45DegreeDuploidPoint(m_points[0], m_points[2]);
		m_pDrawObject->DrawTrackerShape(hdcDest, m_points, 3);
		m_bIsFirstMove3thPt = false;
	}

	m_pDrawToolSite->ReleaseDC(hdcDest);
	return 0;
}

STDMETHODIMP CPieTool::DoLButtonDown(UINT nFlags, POINT point)
{
	HDC hdcDest;
	m_pDrawToolSite->GetDC(&hdcDest);
	::SetROP2(hdcDest, R2_NOTXORPEN);

	DRAW_SHAPE drawShape;
	m_pDrawToolSite->GetDrawShape(&drawShape);
	
	if (m_nPoint == 0)
	{
		CaptureMouse();

		IUnknown* pUnknown = NULL;
		HRESULT hr;
		switch (drawShape)
		{
		case DRAW_ARC:
			hr = m_pDrawToolSite->GetDrawObject(CLSID_DrawArc, &pUnknown);
			break;
		case DRAW_PIE:
			hr = m_pDrawToolSite->GetDrawObject(CLSID_DrawPie, &pUnknown);
			break;
		case DRAW_CHORD:
			hr = m_pDrawToolSite->GetDrawObject(CLSID_DrawChord, &pUnknown);
			break;
		default:
			ATLASSERT(FALSE);
		}
		if (FAILED(hr))
			ATLASSERT(FALSE);

		hr = pUnknown->QueryInterface(IID_ICxDrawObject, (void**)&m_pDrawObject);
		if (FAILED(hr))
			ATLASSERT(FALSE);

		m_points[1] = m_points[0] = point;
		m_pDrawObject->DrawTrackerShape(hdcDest, m_points, 2);
		//m_nPoint++;
	}
	else if (m_nPoint == 1)
	{
	//	m_pDrawObject->DrawTrackerShape(hdcDest, m_points, 2);

		m_points[1] = point;
		if ((nFlags & MK_CONTROL) != 0)
			Calc30Or45DegreeDuploidPoint(m_points[0], m_points[1]);
	//	m_nPoint++;
		m_bIsFirstMove3thPt = true;
	}
	else if (m_nPoint == 2)
	{
//		m_pDrawObject->DrawTrackerShape(hdcDest, m_points, 3);

		m_points[2] = point;
		if ((nFlags & MK_CONTROL) != 0 && drawShape == DRAW_PIE)
			Calc30Or45DegreeDuploidPoint(m_points[0], m_points[2]);
	//	m_nPoint++;

		m_pDrawObject->MoveHandlesTo(m_points, 3);
		
		Fire_OnCreate();

		m_pDrawObject->Release();
		m_pDrawObject = NULL;
		m_bIsFirstMove3thPt = true;
		m_nPoint = -1;
		ReleaseMouse();
	}

	m_nPoint++;

	m_pDrawToolSite->ReleaseDC(hdcDest);
	return 0;
}

STDMETHODIMP CPieTool::DoCancel()
{
	if (m_pDrawObject == NULL)
		return S_OK;

	ReleaseMouse();

	Fire_OnCancel();
	m_pDrawObject->Release();
	m_pDrawObject = NULL;
	m_bIsFirstMove3thPt = true;
	m_nPoint = 0;

	return S_OK;
}



