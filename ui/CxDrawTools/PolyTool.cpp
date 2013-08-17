// PolyTool.cpp : Implementation of CPolyTool
#include "stdafx.h"
#include "CxDrawTools.h"
#include "PolyTool.h"

/////////////////////////////////////////////////////////////////////////////
// CPolyTool

STDMETHODIMP CPolyTool::DoLButtonDown(UINT nFlags, POINT point)
{
	HDC hdcDest;
	m_pDrawToolSite->GetDC(&hdcDest);
	::SetROP2(hdcDest, R2_NOTXORPEN);

	if (!m_bDrawing)
	{
		CaptureMouse();
		m_bDrawing = TRUE;
	}

	if (m_points.size() == 0 || memcmp(&m_points[m_points.size() - 1], &point, sizeof(POINT)) != 0)
	{
		if (m_points.size() == 0)
			m_point = point;
		if ((nFlags & MK_CONTROL) != 0 && m_points.size() > 0)
			Calc30Or45DegreeDuploidPoint(m_points[m_points.size() - 1], m_point);
		m_points.push_back(m_point);
		XorDoDraw(hdcDest);
	}
	m_pDrawToolSite->ReleaseDC(hdcDest);
	return S_OK;
}

STDMETHODIMP CPolyTool::DoMouseMove(UINT nFlags, POINT point)
{
	if (m_bDrawing)
	{
		HDC hdcDest;
		m_pDrawToolSite->GetDC(&hdcDest);
		::SetROP2(hdcDest, R2_NOTXORPEN);
       	XorDoDraw(hdcDest);
		m_point = point;
		if ((nFlags & MK_CONTROL) != 0 && m_points.size() > 0)
			Calc30Or45DegreeDuploidPoint(m_points[m_points.size() - 1], m_point);
		XorDoDraw(hdcDest);
		m_pDrawToolSite->ReleaseDC(hdcDest);
	}
	SetCursor(LoadCursor(NULL, IDC_CROSS));

    return S_OK;
}

STDMETHODIMP CPolyTool::DoLButtonDblClk(UINT nFlags, POINT point)
{
	DRAW_SHAPE drawShape;
	int i, size = (int)m_points.size();
	if (size < 2)
		return S_OK;
	m_pDrawToolSite->GetDrawShape(&drawShape);
	if (drawShape == DRAW_POLYGON && size < 3)
		return S_OK;

	CComPtr<IFloatPoint> spFloatPoint;
	spFloatPoint.CoCreateInstance(CLSID_FloatPoint);
				
	IUnknown *pUnk = NULL;
	switch (drawShape)
	{
	case DRAW_POLYGON:
		{
			m_pDrawToolSite->GetDrawObject(CLSID_DrawPolygon, &pUnk);
			CComQIPtr<IDrawPolygon> spDrawPolygon = pUnk;
			for (i = 0; i < size; i++)
			{
				spFloatPoint->put_X(float(m_points[i].x));
				spFloatPoint->put_Y(float(m_points[i].y));
				spDrawPolygon->AddPoint(spFloatPoint);
			}
		}
		break;
	case DRAW_POLYLINE:
		{
			m_pDrawToolSite->GetDrawObject(CLSID_DrawPolyline, &pUnk);
			CComQIPtr<IDrawPolyline> spDrawPolyline = pUnk;
			for (i = 0; i < size; i++)
			{
				spFloatPoint->put_X(float(m_points[i].x));
				spFloatPoint->put_Y(float(m_points[i].y));
				spDrawPolyline->AddPoint(spFloatPoint);
			}
		}
		break;
	case DRAW_PIPE:
		{
			m_pDrawToolSite->GetDrawObject(CLSID_DrawPipe, &pUnk);
			CComQIPtr<IDrawPipe> spDrawPipe = pUnk;
			for (i = 0, size = (int)m_points.size(); i < size; i++)
			{
				spFloatPoint->put_X(float(m_points[i].x));
				spFloatPoint->put_Y(float(m_points[i].y));
				spDrawPipe->AddPoint(spFloatPoint);
			}
		}
		break;
	default:
		return S_OK;
	}

	ReleaseMouse();
	Fire_OnCreate();
	m_bDrawing = FALSE;
	m_points.clear();

	return S_OK;
}

HRESULT CPolyTool::XorDoDraw(HDC hdcDest)
{
	if (m_points.size() == 0)
		return S_FALSE;

	DRAW_SHAPE drawShape;
	m_pDrawToolSite->GetDrawShape(&drawShape);

	
	::MoveToEx(hdcDest, m_points[m_points.size() - 1].x, m_points[m_points.size() - 1].y, NULL);
	::LineTo(hdcDest, m_point.x, m_point.y);
	if (drawShape == DRAW_POLYGON)
	{
		if (m_points.size() > 1)
			::LineTo(hdcDest, m_points[0].x, m_points[0].y);
		if (m_points.size() > 2)
		{
			::MoveToEx(hdcDest, m_points[m_points.size() - 1].x, m_points[m_points.size() - 1].y, NULL);
			::LineTo(hdcDest, m_points[0].x, m_points[0].y);
		}
	}
	return S_OK;
}

STDMETHODIMP CPolyTool::DoCancel()
{
	if (!m_bDrawing)
		return S_OK;

	ReleaseMouse();
	m_bDrawing = FALSE;
	m_points.clear();

	Fire_OnCancel();
	return S_OK;
}

