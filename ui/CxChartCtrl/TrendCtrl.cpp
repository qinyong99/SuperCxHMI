// TrendCtrl.cpp : Implementation of CTrendCtrl

#include "stdafx.h"
#include "CxChartCtrl.h"
#include "TrendCtrl.h"
#include "RectTracker.h"

#include "../../svr/CxDatSes/CxDatSes_i.c"

/////////////////////////////////////////////////////////////////////////////
// CTrendCtrl


LRESULT CTrendCtrl::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(iccx);
	iccx.dwICC = ICC_DATE_CLASSES;
	::InitCommonControlsEx(&iccx);

	CreateToolPane();

	if (ClientIsRunMode())
	{
		ConnectToDataServer();

		OnRefreshData(TRUE);
		
		if (m_nTimer == -1 && IsWindow())
			m_nTimer = SetTimer(1, 100);
	}
		
	return 0;
}

void CTrendCtrl::CreateToolPane()
{
	ATLASSERT(m_wndToolPane.IsWindow() == FALSE);

	m_wndToolPane.m_pTrend = this;

	m_pTimeAxis->get_TimeSpan((double *)&m_wndToolPane.m_dtsScrollTimeSpan);
	
	m_wndToolPane.Create(m_hWnd);
}

#define MIN_TRENDANDTOOLBAR_HEIGHT 240

BOOL CTrendCtrl::RecalcRects()
{
	if (!IsWindow())
		return FALSE;
	
	m_wndToolPane.m_wndToolBar.ShowWindow(m_bShowToolPane == VARIANT_TRUE ? SW_SHOW : SW_HIDE);
	m_wndToolPane.m_wndList.ShowWindow(m_bShowList == VARIANT_TRUE ? SW_SHOW : SW_HIDE);
	m_wndToolPane.ShowWindow((m_bShowToolPane == VARIANT_TRUE || m_bShowList == VARIANT_TRUE) ? SW_SHOW : SW_HIDE);
	
	RECT rcClient;
	GetClientRect(&rcClient);
	
	m_rect = rcClient;
//	m_rect.right--;
 	m_rect.bottom++;
	if (m_bShowToolPane == VARIANT_TRUE || m_bShowList == VARIANT_TRUE)
	{
		if (m_bShowList == VARIANT_TRUE/* && m_wndToolPane.m_wndToolBar.IsWindowVisible()*/)
			m_rect.bottom = m_rect.top + 
			int(max(MIN_TRENDANDTOOLBAR_HEIGHT, (rcClient.bottom - rcClient.top) * 4 / 5)) - 24;
		else if (m_bShowToolPane == VARIANT_TRUE)
			m_rect.bottom -= 30;
	}
	
	RECT rect = m_rect;
	rect.top = rect.bottom;
	rect.bottom = rcClient.bottom;
	
	AdjustWindowRectEx(&rect, WS_BORDER, FALSE, 0);
	if (m_wndToolPane.IsWindow())
	{
		m_wndToolPane.SetWindowPos(0, &rect, SWP_NOZORDER);
		m_wndToolPane.UpdateLayout();
	}
	
	CChart::RecalcRects();
	
	if (m_lblCursorLegend.m_rect.left == m_lblCursorLegend.m_rect.right || 
		m_lblCursorLegend.m_rect.top == m_lblCursorLegend.m_rect.bottom)
	{
		rect.left = max(m_rcSerial.left + 10, (m_rcSerial.right - 100) / 3);
		rect.top = max(m_rcSerial.top + 10, (m_rcSerial.bottom - 120) / 3);
		rect.right = rect.left + 100;
		rect.bottom = rect.top + 120;
		m_lblCursorLegend.m_rect = rect;
	}
	if  (m_lblCursorLegend.m_rect.left < m_rcSerial.left)
		OffsetRect(&m_lblCursorLegend.m_rect, m_rcSerial.left - m_lblCursorLegend.m_rect.left + 2, 0);
	
	int nCount = m_apSerials.GetSize();
	if (m_wndToolPane.m_wndList.IsWindow())
	{
		m_wndToolPane.m_wndList.DeleteAllItems();
		for (int i = 0; i < nCount; i++)
			m_wndToolPane.m_wndList.InsertItem(i, _T(""));
	}

	return TRUE;
}

LRESULT CTrendCtrl::OnSize (UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)		
{
	RecalcRects();
	
	return 0;
}

// 不进行缺省绘制
LRESULT CTrendCtrl::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1;
}

// 不进行缺省绘制
LRESULT CTrendCtrl::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	UINT nChar = (UINT)wParam;    // virtual-key code 
	switch (nChar)
    {
	case VK_DOWN:   
		Scroll(FALSE, FALSE);
		break;            
		
	case VK_UP:     
		Scroll(FALSE, TRUE);
		break;
		
	case VK_RIGHT:  
		Scroll(TRUE, TRUE);
		break;
		
	case VK_LEFT:   
		Scroll(TRUE, FALSE); 
		break;

	case VK_DELETE:
		if (m_pCurrentObject != NULL && m_pCurrentObject->CanDelete())
		{
			m_apChartObject.Remove(m_pCurrentObject);
			m_pCurrentObject = NULL;
			Invalidate();
		}
		break;

	default:
		DefWindowProc(uMsg,wParam,lParam);
	}

	return 0;
}

LRESULT CTrendCtrl::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetFocus();

	POINT point;
	point.x = GET_X_LPARAM(lParam);
	point.y = GET_Y_LPARAM(lParam);

	int nCount = m_apChartObject.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CChartObject* pObject = m_apChartObject[i];
		if (pObject->m_bVisible && PtInRect(&pObject->m_rect, point))
		{
			m_pCurrentObject = pObject;
			Invalidate();
			break;
		}
	}

	if (m_pCurrentObject != NULL)
	{
		CRectTracker tracker(&m_pCurrentObject->m_rect, CRectTracker::resizeOutside);
		if (tracker.HitTest(point) != CRectTracker::hitNothing)
		{
			ReleaseCapture();
				
			if (tracker.Track(m_hWnd, point))
			{
				tracker.m_rect.NormalizeRect();
				m_pCurrentObject->m_rect = tracker.m_rect;
				Invalidate(FALSE);
			}
		}
		else
		{
			m_pCurrentObject = NULL;
			Invalidate();
		}

	}
	else
	{
		CRectTracker tracker;
			
		if (tracker.TrackRubberBand(m_hWnd, point, TRUE))
		{
			tracker.m_rect.NormalizeRect();

			if (tracker.m_rect.Width() > 8 && tracker.m_rect.Height() > 8)
			{
				RECTF rectDest;
				rectDest.left = (float)tracker.m_rect.left;
				rectDest.top = (float)tracker.m_rect.top;
				rectDest.right = (float)tracker.m_rect.right;
				rectDest.bottom = (float)tracker.m_rect.bottom;

				if (m_bShowCursor)
					Zoom(rectDest.top, rectDest.bottom, rectDest.right, rectDest.left);
			}
		}
	}
	
	return 0;
}

LRESULT CTrendCtrl::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_nSnappedCurve != -1)
	{
		long nIndex;
		m_apSerials[m_nSnappedCurve]->GetIndexByXVal(nIndex, m_dSnappedXVal);
		if (nIndex != -1)
			InsertDataNotation(m_nSnappedCurve, nIndex);
	}

	return 0;
}

LRESULT CTrendCtrl::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT point;
	point.x = GET_X_LPARAM(lParam);
	point.y = GET_Y_LPARAM(lParam);

	m_ptCurrentPoint = point;

	if (m_bShowCursor == VARIANT_TRUE)
	{
		HDC hDC = GetDC();

		CRgn rgnSerial;
		rgnSerial.CreateRectRgnIndirect(&m_rcSerial);

	//	if (rgnSerial.PtInRegion(point))
	//		SetCursor(LoadCursor(NULL,IDC_CROSS));
		
		// PRB: Clipping doesn't work in print preview, no idea why ...
		SelectClipRgn(hDC, rgnSerial);

		DrawCursor(hDC);
		DrawCursorLegend(hDC);
		ReleaseDC(hDC);
	}
	
	return 0;
}

LRESULT CTrendCtrl::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	UINT nHitTest = (short)LOWORD(lParam);
		
	if (m_pCurrentObject != NULL)
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		if (PtInRect(&m_rcSerial, point))
		{
			CRectTracker tracker(&m_pCurrentObject->m_rect, CRectTracker::resizeOutside);
			if (tracker.SetCursor(m_hWnd, nHitTest))
				return 0;
		}
	}
//	if (m_pTracker && m_pTracker->SetCursor(this, nHitTest))
//        return TRUE;


	if (m_bShowCursor == VARIANT_TRUE)
		SetCursor(LoadCursor(NULL,IDC_CROSS));
	else
		SetCursor(LoadCursor(NULL,IDC_ARROW));

	return 0;
}

BOOL CTrendCtrl::PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet)
{
	BOOL bHandled;
	if (pMsg->message == WM_KEYDOWN && pMsg->hwnd == m_hWnd &&
		(pMsg->wParam == VK_LEFT || 
		pMsg->wParam == VK_RIGHT ||
		pMsg->wParam == VK_UP ||
		pMsg->wParam == VK_DOWN ||
		pMsg->wParam == VK_DELETE))
	{
		hRet = S_OK; 
		OnKeyDown(pMsg->message,pMsg->wParam,pMsg->lParam,bHandled);
		return TRUE;
	}
	
	return FALSE;
}

STDMETHODIMP CTrendCtrl::InitNew()
{
	m_bShowCursor = VARIANT_FALSE;
	m_bShowToolPane = VARIANT_TRUE;
	m_bShowList = VARIANT_TRUE;

	CChart::InitNew();
	m_bShowLegend = VARIANT_FALSE;
	
	return IPersistStreamInitImpl<CTrendCtrl>::InitNew();
}

STDMETHODIMP CTrendCtrl::Load(LPSTREAM pStm)
{
	HRESULT hr = CChart::Load(pStm);
	if (FAILED(hr))
		return hr;

	hr = IPersistStreamInitImpl<CTrendCtrl>::Load(pStm);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

STDMETHODIMP CTrendCtrl::Save(LPSTREAM pStm, BOOL fClearDirty)
{
	HRESULT hr = CChart::Save(pStm, fClearDirty);
	if (FAILED(hr))
		return hr;

	hr = IPersistStreamInitImpl<CTrendCtrl>::Save(pStm,  fClearDirty);
	if (FAILED(hr))
		return hr;
	
	return S_OK;
}

// 是否运行模式
BOOL CTrendCtrl::ClientIsRunMode()
{
	BOOL bUserMode = TRUE;
	HRESULT hRet = GetAmbientUserMode(bUserMode);
	
	return bUserMode;
}

HRESULT CTrendCtrl::OnDraw(ATL_DRAWINFO& di)
{
	USES_CONVERSION;

	CDCHandle dc = di.hdcDraw;

 	CDC dcMem;
	CBitmap bitmap;
	dcMem.CreateCompatibleDC(dc);
	bitmap.CreateCompatibleBitmap(dc, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top);
	HBITMAP hBmpOld = dcMem.SelectBitmap(bitmap);

	HDC hdcDest = dcMem.m_hDC;

	DoDraw(hdcDest);

	m_ptLastPoint = CPoint(-1, -1);

	int nCount = m_apChartObject.GetSize();
	for (int i = 1; i < nCount; i++)
	{
		CChartObject* pObject = m_apChartObject[i];
		pObject->DoDraw(hdcDest);
		if (m_pCurrentObject == pObject)
		{
			CRectTracker tracker(&m_pCurrentObject->m_rect, CRectTracker::resizeOutside);
			tracker.Draw(hdcDest);
		}
	}

	if (m_bShowCursor == VARIANT_TRUE)
	{
		DrawCursor(hdcDest);
		DrawCursorLegend(hdcDest);
		if (m_pCurrentObject == &m_lblCursorLegend)
		{
			CRectTracker tracker(&m_pCurrentObject->m_rect, CRectTracker::resizeOutside);
			tracker.Draw(hdcDest);
		}
	}

	dc.BitBlt(0, 0, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top,
		dcMem, 0, 0, SRCCOPY);
	dcMem.SelectBitmap(hBmpOld);

	return S_OK;
}

void CTrendCtrl::DrawCursor(HDC hdcDest)
{
	m_nSnappedCurve = -1;
	
	if (::PtInRect(&m_rcSerial, m_ptCurrentPoint))
	{
		long nIndex;
		// Snap cursor to nearest curve
		for (int i = 0; i < m_apSerials.GetSize(); i++)
		{
			// Get xval for current position
			m_dSnappedXVal = m_apSerials[i]->GetTimeAxis()->GetValueForPos((float)m_ptCurrentPoint.x);
			// Find index for this value
			m_apSerials[i]->GetIndexByXVal(nIndex, m_dSnappedXVal);
		
			// get yval for index
			if (nIndex == -1)
				continue;

			m_dSnappedXVal = m_apSerials[i]->m_values[nIndex].dXVal;
			m_dSnappedYVal = m_apSerials[i]->m_values[nIndex].dYVal;

			DATAPOINT datapoint;
			datapoint.dYVal = m_dSnappedYVal;
			// Check if cursor is in snap-range
			POINT point;
			m_apSerials[i]->GetValueAxis()->GetPointForValue(&datapoint, &point);
			if (abs(m_ptCurrentPoint.y - point.y) < 5)
			{
				m_nSnappedCurve = i;
				m_ptCurrentPoint.y = point.y;
				break;
			}
		}
	}

	int	nOldROP2 = ::SetROP2(hdcDest, R2_NOTXORPEN);

//	HPEN hPen = ::CreatePen(PS_DASHDOT, 1, RGB(0, 0, 0));
//	HPEN hOldPen = (HPEN)SelectObject(hdcDest, hPen);
	
	if (m_ptLastPoint != CPoint(-1,-1))
	{
		::MoveToEx(hdcDest, m_ptLastPoint.x, m_rcSerial.top, NULL);
		::LineTo(hdcDest, m_ptLastPoint.x, m_rcSerial.bottom);
		::MoveToEx(hdcDest, m_rcSerial.left, m_ptLastPoint.y, NULL);
		::LineTo(hdcDest, m_rcSerial.right, m_ptLastPoint.y);
	}
	
	::MoveToEx(hdcDest, m_ptCurrentPoint.x, m_rcSerial.top, NULL);
	::LineTo(hdcDest, m_ptCurrentPoint.x, m_rcSerial.bottom);
	::MoveToEx(hdcDest, m_rcSerial.left, m_ptCurrentPoint.y, NULL);
	::LineTo(hdcDest, m_rcSerial.right, m_ptCurrentPoint.y);
	
	m_ptLastPoint = m_ptCurrentPoint;

//	SelectObject(hdcDest, hOldPen);
//	DeleteObject(hPen);
	
	::SetROP2(hdcDest, nOldROP2);
	
//	DrawCursorLegend(pDC);
}

void CTrendCtrl::DrawCursorLegend(HDC hdcDest)
{
	USES_CONVERSION;

	double dValue;

	m_lblCursorLegend.m_strText.Empty();

	if (!m_bShowMutiTimeAxis)
	{
		CTimeAxis& axis = *m_pTimeAxis;
		if (axis.m_bShowAxis == VARIANT_TRUE)
		{
			dValue = axis.GetValueForPos((float)m_ptCurrentPoint.x);
			COleDateTime dt(dValue);
			CString strText;
			strText.Format(_T("X: "));
			strText += dt.Format(OLE2T(axis.m_bstrDisplayFormat));
			strText += _T("\r\n");
			m_lblCursorLegend.m_strText += strText;
		}
	}
	else
	{
		int nCount = m_apSerials.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			CTimeAxis& axis = *m_apSerials[i]->m_pTimeAxis;
			if (axis.m_bShowAxis == VARIANT_TRUE)
			{
				dValue = axis.GetValueForPos((float)m_ptCurrentPoint.x);
				COleDateTime dt(dValue);
				CString strText;
				strText.Format(_T("X[%02d]: "), i);
				strText += dt.Format(OLE2T(axis.m_bstrDisplayFormat));
				strText += _T("\r\n");
				m_lblCursorLegend.m_strText += strText;
			}
		}
	}

	if (!m_bShowMutiValueAxis)
	{
		TCHAR szBuf[32];

		CValueAxis& axis = *m_pValueAxis;
		if (axis.m_bShowAxis == VARIANT_TRUE)
		{
			dValue = axis.GetValueForPos((float)m_ptCurrentPoint.y);
			sprintf(szBuf, _T("Y: %.2f"), dValue);
			CString strText = szBuf;
			strText += _T("\r\n");
			m_lblCursorLegend.m_strText += strText;
		}
	}
	else
	{
		int nCount = m_apSerials.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			TCHAR szBuf[32];

			CValueAxis& axis = *m_apSerials[i]->m_pValueAxis;
			if (axis.m_bShowAxis == VARIANT_TRUE)
			{
				dValue = axis.GetValueForPos((float)m_ptCurrentPoint.y);
				sprintf(szBuf, _T("Y[%02d]: %.2f"), i, dValue);
				CString strText = szBuf;
				strText += _T("\r\n");
				m_lblCursorLegend.m_strText += strText;
			}
		}
	}

	if (m_nSnappedCurve != -1)
	{
		CString strText = m_apSerials[m_nSnappedCurve]->m_bstrSource;
		strText += _T(": ");

		COleDateTime dt(m_dSnappedXVal);
		strText += dt.Format(OLE2T(m_apSerials[m_nSnappedCurve]->GetTimeAxis()->m_bstrDisplayFormat));

		TCHAR szBuf[32];
		sprintf(szBuf, _T(", %.2f"), m_dSnappedYVal);

		strText += szBuf;
		strText += _T("\r\n");
		m_lblCursorLegend.m_strText += strText;
	}

	m_lblCursorLegend.DoDraw(hdcDest);
}

LRESULT CTrendCtrl::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_nTimer != -1)
		KillTimer(m_nTimer);
	
	return 0;
}

HRESULT CTrendCtrl::FireViewChange()
{
	if (m_wndToolPane.m_wndList.IsWindow())
		m_wndToolPane.m_wndList.Invalidate(FALSE);

	return CComControlBase::FireViewChange();
}

STDMETHODIMP CTrendCtrl::get_ShowToolPane(VARIANT_BOOL *pVal)
{
	*pVal = m_bShowToolPane;

	return S_OK;
}

STDMETHODIMP CTrendCtrl::put_ShowToolPane(VARIANT_BOOL newVal)
{
	m_bShowToolPane = newVal;
//	m_wndToolPane.ShowWindow(m_bShowToolPane != VARIANT_FALSE ? SW_SHOW : SW_HIDE);
	RecalcRects();
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CTrendCtrl::get_ShowList(VARIANT_BOOL *pVal)
{
	*pVal = m_bShowList;
	
	return S_OK;
}

STDMETHODIMP CTrendCtrl::put_ShowList(VARIANT_BOOL newVal)
{
	m_bShowList = newVal;
//	m_wndList.ShowWindow(m_bShowToolPane != VARIANT_FALSE ? SW_SHOW : SW_HIDE);
	RecalcRects();
	FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CTrendCtrl::get_ShowCursor(VARIANT_BOOL *pVal)
{
	*pVal = m_bShowCursor;
	
	return S_OK;
}

STDMETHODIMP CTrendCtrl::put_ShowCursor(VARIANT_BOOL newVal)
{
	m_bShowCursor = newVal;
	if (m_pCurrentObject == &m_lblCursorLegend)
		m_pCurrentObject = NULL;
	
	m_lblCursorLegend.m_bVisible = m_bShowCursor != VARIANT_FALSE;
	
	if (m_wndToolPane.IsWindow())
		m_wndToolPane.UpdateToolBar();
	
	FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CTrendCtrl::put_TrendMode(enumTrendMode newVal)
{
	if (m_enumTrendMode == newVal)
		return S_OK;
	
	m_enumTrendMode = newVal;
	m_wndToolPane.UpdateToolBar();
	
	for (int i = 0; i < m_apSerials.GetSize(); i++)
	{
		CSerial* pSerial = m_apSerials[i];
		pSerial->RemoveAllValues();
	}

	OnRefreshData(TRUE);

	FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CTrendCtrl::put_ZoomMode(enumZoomMode newVal)
{
	if (m_enumZoomMode == newVal)
		return S_OK;
	
	m_wndToolPane.Zoom(1);
	m_enumZoomMode = newVal;
	
	FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CTrendCtrl::Zoom(float fYHi, float fYLo, float fXHi, float fXLo)
{
	RECTF rectDest;
	rectDest.left = fXLo;
	rectDest.top = fYHi;
	rectDest.right = fXHi;
	rectDest.bottom = fYLo;
	
	RECTF rectSource;
	rectSource.left = (float)m_rcSerial.left;
	rectSource.top = (float)m_rcSerial.top;
	rectSource.right = (float)m_rcSerial.right;
	rectSource.bottom = (float)m_rcSerial.bottom;
	float fXZoom = m_enumZoomMode == ZoomModeVertical ? 1 : (rectDest.right - rectDest.left) / (rectSource.right - rectSource.left);
	float fYZoom = m_enumZoomMode == ZoomModeHorizontal ? 1 : (rectDest.bottom - rectDest.top) / (rectSource.bottom - rectSource.top);
	
	if (m_enumZoomMode == ZoomModeHorizontal)
	{
		m_wndToolPane.m_fScaleFactor *= fXZoom;	
		rectDest.top = rectSource.top;
		rectDest.bottom = rectSource.bottom;
	}
	else if (m_enumZoomMode == ZoomModeVertical)
	{
		m_wndToolPane.m_fScaleFactor *= fYZoom;	
		rectDest.left = rectSource.left;
		rectDest.right = rectSource.right;
	}
	else
	{
		//选用两者的大值放大
		if (fXZoom >= fYZoom)
		{
			m_wndToolPane.m_fScaleFactor *= fXZoom;
			
			float fYCenter = (rectDest.top + rectDest.bottom) / 2;
			float fHeightHalf = fXZoom * (rectSource.bottom - rectSource.top) / 2;
			rectDest.top = fYCenter - fHeightHalf;
			rectDest.bottom = fYCenter + fHeightHalf;
		}
		else
		{
			m_wndToolPane.m_fScaleFactor *= fYZoom;
			
			float fXCenter = (rectDest.left + rectDest.right) / 2;
			float fWidthHalf = fYZoom * (rectSource.right - rectSource.left) / 2;
			rectDest.left = fXCenter - fWidthHalf;
			rectDest.right = fXCenter + fWidthHalf;
		}
	}
	
	if (!m_bShowMutiTimeAxis && !m_bShowMutiValueAxis)
	{
		double dXMax = m_pTimeAxis->GetValueForPos(rectDest.right);
		double dXMin = m_pTimeAxis->GetValueForPos(rectDest.left);
		m_pTimeAxis->Scale(dXMin, dXMax);

		double dYMax = m_pValueAxis->GetValueForPos(rectDest.top);
		double dYMin = m_pValueAxis->GetValueForPos(rectDest.bottom);
		m_pValueAxis->Scale(dYMin, dYMax);

		if (m_enumTrendMode == TrendModeHistory)
			OnRefreshData(TRUE);
	}
	else if (!m_bShowMutiTimeAxis && m_bShowMutiValueAxis)
	{
		double dXMax = m_pTimeAxis->GetValueForPos(rectDest.right);
		double dXMin = m_pTimeAxis->GetValueForPos(rectDest.left);
		m_pTimeAxis->Scale(dXMin, dXMax);

		int nCount = m_apSerials.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			m_apSerials[i]->Zoom(FALSE, rectDest.top, rectDest.bottom);
		}
	}
	else if (m_bShowMutiTimeAxis && !m_bShowMutiValueAxis)
	{
		double dYMax = m_pValueAxis->GetValueForPos(rectDest.top);
		double dYMin = m_pValueAxis->GetValueForPos(rectDest.bottom);
		m_pValueAxis->Scale(dYMin, dYMax);

		int nCount = m_apSerials.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			m_apSerials[i]->Zoom(TRUE, rectDest.right, rectDest.left);
		}
	}
	else
	{
		// 每条画笔缩放
		int nCount = m_apSerials.GetSize();
		for (int i = 0; i < nCount; i++)
		{
	//		m_apSerials[i]->Zoom(fYHi, fYLo, fXHi, fXLo);
			m_apSerials[i]->Zoom(rectDest.top, rectDest.bottom, rectDest.right, rectDest.left);
		}
	}
	
	m_wndToolPane.UpdateZoomRadioEdit();
	
	FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CTrendCtrl::EditProperties()
{
	IUnknown* pUnk = GetUnknown();

	CComQIPtr<ISpecifyPropertyPages> spSpec(pUnk);
	if (!spSpec)
		return E_NOINTERFACE;

	// Get list of property pages
	CAUUID cauuid = {0};
	HRESULT hr = spSpec->GetPages(&cauuid);
	
	// Use the system-supplied property frame
	if (SUCCEEDED(hr) && (cauuid.cElems != 0))
	{
		hr = OleCreatePropertyFrame(
			m_hWnd,       // Parent window of the property frame
			0,          // Horizontal position of the property frame
			0,          // Vertical position of the property frame
			L"趋势",// Property frame caption
			1,          // Number of objects
			&pUnk,      // Array of IUnknown pointers for objects
			cauuid.cElems,    // Number of property pages
			cauuid.pElems,    // Array of CLSIDs for property pages
			NULL,       // Locale identifier
			0,          // Reserved - 0
			NULL        // Reserved - 0
			);
		
		// Free array allocated by GetPages
		CoTaskMemFree(cauuid.pElems);
	}
		
	return S_OK;
}

STDMETHODIMP CTrendCtrl::EditSerials()
{
	IUnknown* pUnk = GetUnknown();
	if (pUnk == NULL)
		return E_FAIL;

	CLSID guidPage = CLSID_CxTrendCtrlPage;
	
	OleCreatePropertyFrame(
		m_hWnd,       // Parent window of the property frame
		0,          // Horizontal position of the property frame
		0,          // Vertical position of the property frame
		L"趋势",// Property frame caption
		1,          // Number of objects
		&pUnk,      // Array of IUnknown pointers for objects
		1,    // Number of property pages
		&guidPage,    // Array of CLSIDs for property pages
		NULL,       // Locale identifier
		0,          // Reserved - 0
		NULL        // Reserved - 0
		);

	return S_OK;
}

STDMETHODIMP CTrendCtrl::InsertDataNotation(int nCurve, int nIndex)
{
	USES_CONVERSION;

	CDataNotation* pNotation = new CDataNotation;
	pNotation->m_nCurve = nCurve;
	pNotation->m_nIndex = nIndex;
	pNotation->m_dXVal = m_apSerials[nCurve]->m_values[nIndex].dXVal;
	pNotation->m_dYVal = m_apSerials[nCurve]->m_values[nIndex].dYVal;
	pNotation->m_pTrend = this;

	COleDateTime dt(pNotation->m_dXVal);
	CString strText = dt.Format(OLE2T(m_apSerials[nCurve]->GetTimeAxis()->m_bstrDisplayFormat));
	
	TCHAR szBuf[32];
	sprintf(szBuf, _T(", %.2f"), pNotation->m_dYVal);
	
	strText += szBuf;
	pNotation->m_strText = strText;

	m_apChartObject.Add(pNotation);

	Invalidate();

	return S_OK;
}

LRESULT CTrendCtrl::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int n = m_dwRefreshInterval / 100;
	if (n < 1)
		n = 1;
	if ((m_nTimeFactor %= n) == 0)
	{
		if (OnRefreshData())
			FireViewChange();
	}
	
	m_nTimeFactor += 1;

	return 0;
}
