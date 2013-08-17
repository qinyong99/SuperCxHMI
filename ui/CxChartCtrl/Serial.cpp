// Serial.cpp : Implementation of CCxDrawChartApp and DLL registration.

#include "stdafx.h"
#include "CxChartCtrl.h"
#include "TrendCtrl.h"

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CSerial::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ISerial,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CSerial::InitNew()
{
	m_dMaxValLimit = +10000.00;
	m_dMinValLimit = -10000.00;

	m_lLineWidth = 1;
	m_enumLineStyle = LineStyleSolid;
	m_clrLineColor = m_pChart->GetNotUsedSerialColor();
	m_enumMarkerStyle = MarkerStyleNone;

	m_nMaxNumPoints = 600;
	m_nMaxNumDisplayPoints = 600;

	m_bShow = VARIANT_TRUE;
//	m_pPoints = new POINT[m_nMaxNumDisplayPoints];

	m_pTimeAxis = new CComObject<CTimeAxis>;
	m_pTimeAxis->m_pChart = m_pChart;
	m_pTimeAxis->InitNew();
	m_pTimeAxis->AddRef();
	m_pTimeAxis->m_clrTitle = m_clrLineColor;
	m_pTimeAxis->m_clrAxis = m_clrLineColor;

	m_pValueAxis = new CComObject<CValueAxis>;
	m_pValueAxis->m_pChart = m_pChart;
	m_pValueAxis->InitNew();
	m_pValueAxis->AddRef();
	m_pValueAxis->m_clrTitle = m_clrLineColor;
	m_pValueAxis->m_clrAxis = m_clrLineColor;

	m_pLegend = new CComObject<CLegend>;
	m_pLegend->m_pSerial = this;
	m_pLegend->InitNew();
	m_pLegend->AddRef();

	m_pGrid = new CComObject<CGrid>;
	m_pGrid->m_pChart = m_pChart;
	m_pGrid->InitNew();
	m_pGrid->AddRef();

	return S_OK;
}

STDMETHODIMP CSerial::Save(LPSTREAM pStm, BOOL fClearDirty)
{
	m_bstrSource.WriteToStream(pStm);
	pStm->Write(&m_clrLineColor, sizeof(m_clrLineColor), NULL);
	pStm->Write(&m_enumLineStyle, sizeof(m_enumLineStyle), NULL);
	pStm->Write(&m_lLineWidth, sizeof(m_lLineWidth), NULL);
	pStm->Write(&m_enumMarkerStyle, sizeof(m_enumMarkerStyle), NULL);
	pStm->Write(&m_nMaxNumPoints, sizeof(m_nMaxNumPoints), NULL);
	pStm->Write(&m_nMaxNumDisplayPoints, sizeof(m_nMaxNumDisplayPoints), NULL);
	pStm->Write(&m_bShow, sizeof(m_bShow), NULL);
	pStm->Write(&m_dMinValLimit, sizeof(double), NULL);
	pStm->Write(&m_dMaxValLimit, sizeof(double), NULL);
	
	m_pTimeAxis->Save(pStm, fClearDirty);
	m_pValueAxis->Save(pStm, fClearDirty);
	m_pLegend->Save(pStm, fClearDirty);
	m_pGrid->Save(pStm, fClearDirty);

	return S_OK;
}

STDMETHODIMP CSerial::Load(LPSTREAM pStm)
{
	m_bstrSource.ReadFromStream(pStm);
	m_bstrTag = m_bstrSource;

	pStm->Read(&m_clrLineColor, sizeof(m_clrLineColor), NULL);
	pStm->Read(&m_enumLineStyle, sizeof(m_enumLineStyle), NULL);
	pStm->Read(&m_lLineWidth, sizeof(m_lLineWidth), NULL);
	pStm->Read(&m_enumMarkerStyle, sizeof(m_enumMarkerStyle), NULL);
	pStm->Read(&m_nMaxNumPoints, sizeof(m_nMaxNumPoints), NULL);
	pStm->Read(&m_nMaxNumDisplayPoints, sizeof(m_nMaxNumDisplayPoints), NULL);
	pStm->Read(&m_bShow, sizeof(m_bShow), NULL);
	pStm->Read(&m_dMinValLimit, sizeof(double), NULL);
	pStm->Read(&m_dMaxValLimit, sizeof(double), NULL);

//	m_pPoints = new POINT[m_nMaxNumDisplayPoints];
	
	m_pTimeAxis = new CComObject<CTimeAxis>;
	m_pTimeAxis->m_pChart = m_pChart;
	m_pTimeAxis->AddRef();
	m_pTimeAxis->Load(pStm);

	m_pValueAxis = new CComObject<CValueAxis>;
	m_pValueAxis->m_pChart = m_pChart;
	m_pValueAxis->AddRef();
	m_pValueAxis->Load(pStm);

	m_pLegend = new CComObject<CLegend>;
	m_pLegend->m_pSerial = this;
	m_pLegend->AddRef();
	m_pLegend->Load(pStm);

	m_pGrid = new CComObject<CGrid>;
	m_pGrid->m_pChart = m_pChart;
	m_pGrid->AddRef();
	m_pGrid->Load(pStm);

	return S_OK;
}

STDMETHODIMP CSerial::get_Source(BSTR *pVal)
{
	*pVal = m_bstrSource.Copy();

	return S_OK;
}

STDMETHODIMP CSerial::put_Source(BSTR newVal)
{
	if (m_bstrSource == newVal)
		return S_OK;

	if (m_dwTagCookie != 0)
		m_pChart->ConnectToDataServer(NULL, this);
	RemoveAllValues();
	m_bstrSource = newVal;
	m_bstrTag = m_bstrSource;

	if (m_pChart->ClientIsRunMode())
		m_pChart->ConnectToDataServer(this);
		
	return S_OK;
}

STDMETHODIMP CSerial::get_TimeAxis(ITimeAxis **pVal)
{
	ATLASSERT(m_pTimeAxis != NULL);
	m_pTimeAxis->QueryInterface(IID_ITimeAxis, (void **)pVal);

	return S_OK;
}

STDMETHODIMP CSerial::get_ValueAxis(IValueAxis **pVal)
{
	ATLASSERT(m_pValueAxis != NULL);
	m_pValueAxis->QueryInterface(IID_IValueAxis, (void **)pVal);

	return S_OK;
}

STDMETHODIMP CSerial::get_Legend(ILegend **pVal)
{
	ATLASSERT(m_pLegend != NULL);
	m_pLegend->QueryInterface(IID_ILegend, (void **)pVal);

	return S_OK;
}

STDMETHODIMP CSerial::get_Grid(IGrid **pVal)
{
	ATLASSERT(m_pGrid != NULL);
	m_pGrid->QueryInterface(IID_IGrid, (void **)pVal);

	return S_OK;
}

STDMETHODIMP CSerial::get_MaxNumPoints(long *pVal)
{
	*pVal = m_nMaxNumPoints;

	return S_OK;
}

STDMETHODIMP CSerial::put_MaxNumPoints(long newVal)
{
	m_nMaxNumPoints = newVal;

	return S_OK;
}

STDMETHODIMP CSerial::get_MaxNumDisplayPoints(long *pVal)
{
	*pVal = m_nMaxNumDisplayPoints;

	return S_OK;
}

STDMETHODIMP CSerial::put_MaxNumDisplayPoints(long newVal)
{
	// 最小显示两个点
	if (newVal < 2)
		return E_INVALIDARG;

	m_nMaxNumDisplayPoints = newVal;
	InitDemoValues();
	m_pChart->FireViewChange();

	return S_OK;
}

/////////////////////////////////////////////////////////////
//功能：在线的数据队例中加一个新的数据
//////////////////////////////////////////////////////////
void CSerial::AddValue(DATAPOINT &newVal)
{
	// 删除已经不在显示范围的点
	double dXMinVal = GetTimeAxis()->m_dMinVal;
	double dXMaxVal = GetTimeAxis()->m_dMaxVal;
	int nCount = m_values.GetSize();

	if (nCount >= 2)
	{
		double dXStep = (dXMaxVal - dXMinVal) / (m_nMaxNumDisplayPoints - 1);
		double dXLastVal = m_values[nCount - 1].dXVal;
		double dXNextVal = m_values[nCount - 2].dXVal + dXStep;

		if (fabs(newVal.dXVal - dXNextVal) <= fabs(dXLastVal - dXNextVal))
		{
			m_dValTotal -= m_values[nCount - 1].dYVal;
			m_values.RemoveAt(--nCount);
		}
	}
	
	for (int i = 0; i < nCount; i++)
	{
		if (m_values[i].dXVal >= dXMinVal)
			break;
	}

	while (--i > 0)
	{
		m_dValTotal -= m_values[0].dYVal;
		m_values.RemoveAt(0);
	}

	if (newVal.dYVal > m_dMaxValLimit)
		newVal.dYVal = m_dMaxValLimit;
	if (newVal.dYVal < m_dMinValLimit)
		newVal.dYVal = m_dMinValLimit;
	m_values.Add(newVal);

	CValueAxis* pValueAxis = GetValueAxis();
	double value = newVal.dYVal;

	m_dValTotal += value;
	if (value > m_dMaxVal)
		m_dMaxVal = value;
	if (value < m_dMinVal)
		m_dMinVal = value;

	BOOL bScale = FALSE;
	if (value > pValueAxis->m_dMaxVal)
	{
		pValueAxis->m_dMaxVal = AutoScale(value, MAX);
		bScale = TRUE;
	}
	if (value < pValueAxis->m_dMinVal)
	{
		pValueAxis->m_dMinVal = AutoScale(value, MIN);
		bScale = TRUE;
	}
	if (bScale)
		pValueAxis->UpdateDisplayFormat();
}

void CSerial::RemoveAllValues()
{
	m_values.RemoveAll();

	m_dValTotal = 0;
	m_dMaxVal = -DBL_MAX;
	m_dMinVal = +DBL_MAX;
}

STDMETHODIMP CSerial::get_LineColor(OLE_COLOR *pVal)
{
	*pVal = m_clrLineColor;
	
	return S_OK;
}

STDMETHODIMP CSerial::put_LineColor(OLE_COLOR newVal)
{
	m_clrLineColor = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CSerial::get_LineStyle(enumLineStyle *pVal)
{
	*pVal = m_enumLineStyle;

	return S_OK;
}

STDMETHODIMP CSerial::put_LineStyle(enumLineStyle newVal)
{
	m_enumLineStyle = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CSerial::get_LineWidth(long *pVal)
{
	*pVal = m_lLineWidth;

	return S_OK;
}

STDMETHODIMP CSerial::put_LineWidth(long newVal)
{
	m_lLineWidth = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CSerial::get_MarkerStyle(enumMarkerStyle *pVal)
{
	*pVal = m_enumMarkerStyle;

	return S_OK;
}

STDMETHODIMP CSerial::put_MarkerStyle(enumMarkerStyle newVal)
{
	m_enumMarkerStyle = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

void CSerial::AdjustLegendHeight(int nHeight)
{
	m_pLegend->AdjustHeight(nHeight);
}

void CSerial::CalcLegendLargestWidth(int& nWidth)
{
	m_pLegend->CalcLargestWidth(nWidth);
}

void CSerial::RecalcLegend(RECT& rect)
{
	m_pLegend->RecalcRect(rect);
}

void CSerial::RecalcTimeAxis(RECT& rect)
{
	m_pTimeAxis->RecalcRect(rect);
}

void CSerial::RecalcValueAxis(RECT& rect)
{
	m_pValueAxis->RecalcRect(rect);
}

CValueAxis* CSerial::GetValueAxis()
{
	if (m_pChart->m_bShowMutiValueAxis)
		return m_pValueAxis;

	return m_pChart->m_pValueAxis;
}

CTimeAxis* CSerial::GetTimeAxis()
{
	if (m_pChart->m_bShowMutiTimeAxis)
		return m_pTimeAxis;
	
	return m_pChart->m_pTimeAxis;
}

void DrawLine(HDC hdc, int x1, int y1, int x2, int y2)			{ ::MoveToEx(hdc, x1, y1, NULL); ::LineTo(hdc, x2, y2); }

void DrawMarker(HDC hdc, int x, int y, int mode, int n)
{
	switch(mode)
	{
		case MarkerStyleCross:
			DrawLine(hdc, x - n, y, x + n + 1, y );
			DrawLine(hdc, x, y - n, x, y + n + 1);
			break;

		case MarkerStyleCircle:
			::Ellipse(hdc, x - n, y - n, x + n, y + n);
			break;

		case MarkerStyleTriangle:
		{
			POINT  p[3];
			p[0].x = x - n;
			p[0].y = (int)(y + n * sqrt(3.) / 3);
			p[1].x = x + n;
			p[1].y = (int)(y + n * sqrt(3.) / 3);
			p[2].x = x;
			p[2].y = (int)(y - 2 * n * sqrt(3.) / 3);
			::Polygon(hdc, p, 3);
			break;
		}

		case MarkerStyleSquare:
			::Rectangle(hdc, x - n, y - n, x + n, y + n);
			break;

		case MarkerStyleDiamond:
		{
			POINT  p[4];
			p[0].x = x - n;
			p[0].y = y;
			p[1].x = x;
			p[1].y = y + n;
			p[2].x = x + n;
			p[2].y = y;
			p[3].x = x;
			p[3].y = y - n;
			::Polygon(hdc, p, 4);
			break;
		}
	}
}

void CSerial::DoDraw(HDC hdcDest, RECT& rect, BOOL bTimeAxis, BOOL bValueAxis, BOOL bGrid, BOOL bLegend)
{
	if (bGrid)
		m_pGrid->DoDraw(hdcDest, rect);

	if (bTimeAxis)
		m_pTimeAxis->DoDraw(hdcDest);
	if (bValueAxis)
		m_pValueAxis->DoDraw(hdcDest);
	if (bLegend)
		m_pLegend->DoDraw(hdcDest);
}

#include "PolyLineInRect.h"

void CSerial::DoDrawCurve(HDC hdcDest, RECT& rect)
{
	if (m_bShow != VARIANT_TRUE)
		return;

	int nCount = m_values.GetSize();
	if (nCount < 1)
		return;

	POINT* pPoints = new POINT[nCount];
	GetTimeAxis()->GetPointForValue(m_values.GetData(), pPoints, nCount);
	GetValueAxis()->GetPointForValue(m_values.GetData(), pPoints, nCount);
	
	COLORREF clrLineColor;
	OleTranslateColor(m_clrLineColor, NULL, &clrLineColor);
	
	HPEN hPen = ::CreatePen(int(m_enumLineStyle), m_lLineWidth, clrLineColor);
	HPEN hOldPen = (HPEN)::SelectObject(hdcDest, hPen);
	HBRUSH hBrush = ::CreateSolidBrush(clrLineColor);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdcDest, hBrush);

//	POINT point[2];
//	point[0].x = 400;
//	point[0].y = 400;
//	point[1].x = 0;
//	point[1].y = 200;
//	
//	PolyLineInRect(hdcDest, point, 2, rect);


//	::Polyline(hdcDest, pPoints, nCount);
	PolyLineInRect(hdcDest, pPoints, nCount, rect);

	for (int i = 0; i < nCount; i++)
	{
		RECT rcTemp = rect;
		::InflateRect(&rcTemp, 1, 1);
		if (::PtInRect(&rcTemp, pPoints[i]))
			DrawMarker(hdcDest, pPoints[i].x, pPoints[i].y, m_enumMarkerStyle, 4);
	}	

	delete [] pPoints;
	
	::SelectObject(hdcDest, hOldPen);
	::DeleteObject(hPen);
	::SelectObject(hdcDest, hOldBrush);
	::DeleteObject(hBrush);
}

void CSerial::InitDemoValues()
{
	return;

	if (!m_pChart->ClientIsRunMode())
	{
		CComObject<CSerial>* pThis = (CComObject<CSerial> *)this;
		int iIndex = m_pChart->m_apSerials.Find(pThis);
		int nCount = m_pChart->m_apSerials.GetSize();
		double dValue = GetValueAxis()->m_dMinVal + 
				(GetValueAxis()->m_dMaxVal - GetValueAxis()->m_dMinVal) * (iIndex + 1) / (nCount + 1);

		RemoveAllValues();
		for (int i = 0; i < m_nMaxNumDisplayPoints; i++)
		{
			DATAPOINT value;
			value.dXVal = GetTimeAxis()->m_dMinVal + i * (GetTimeAxis()->m_dMaxVal - GetTimeAxis()->m_dMinVal) / (m_nMaxNumDisplayPoints - 1);
			value.dYVal = dValue;
			AddValue(value);
		}
	}
}

void CSerial::Scroll(BOOL bHorizon, BOOL bUpOrRight, float fPercentage)
{
	if (bHorizon)
		m_pTimeAxis->ScrollPercentage(bUpOrRight, fPercentage);
	else
		m_pValueAxis->ScrollPercentage(bUpOrRight, fPercentage);

	if (m_pChart->m_enumTrendMode = TrendModeHistory)
		RefreshHistData();
}

void CSerial::ScrollForward(COleDateTimeSpan dtsSpan)
{
	m_pTimeAxis->ScrollForward(dtsSpan);

	if (m_pChart->m_enumTrendMode == TrendModeHistory)
		RefreshHistData();
}

void CSerial::ScrollBack(COleDateTimeSpan dtsSpan)
{
	m_pTimeAxis->ScrollBack(dtsSpan);

	if (m_pChart->m_enumTrendMode == TrendModeHistory)
		RefreshHistData();
}

void CSerial::ScrollTo(COleDateTime dtFrom, COleDateTime dtTo)
{
	m_pTimeAxis->ScrollTo(dtFrom, dtTo);
	
	if (m_pChart->m_enumTrendMode == TrendModeHistory)
		RefreshHistData();
}

// 卷动到最后
void CSerial::ScrollForwardMost()
{
	m_pTimeAxis->ScrollForwardMost();

	if (m_pChart->m_enumTrendMode == TrendModeHistory)
		RefreshHistData();
}

// 缩放
void CSerial::Zoom(float fYHi, float fYLo, float fXHi, float fXLo)
{
	double dYMax = m_pValueAxis->GetValueForPos(fYHi);
	double dYMin = m_pValueAxis->GetValueForPos(fYLo);
	m_pValueAxis->Scale(dYMin, dYMax);

	double dXMax = m_pTimeAxis->GetValueForPos(fXHi);
	double dXMin = m_pTimeAxis->GetValueForPos(fXLo);
	m_pTimeAxis->Scale(dXMin, dXMax);

	if (m_pChart->m_enumTrendMode == TrendModeHistory)
		RefreshHistData();
}

void CSerial::Zoom(BOOL bHorizon, float fHi, float fLo)
{
	if (bHorizon)
	{
		double dXMax = m_pTimeAxis->GetValueForPos(fHi);
		double dXMin = m_pTimeAxis->GetValueForPos(fLo);
		m_pTimeAxis->Scale(dXMin, dXMax);
	}
	else
	{
		double dYMax = m_pValueAxis->GetValueForPos(fHi);
		double dYMin = m_pValueAxis->GetValueForPos(fLo);
		m_pValueAxis->Scale(dYMin, dYMax);
	}

	if (m_pChart->m_enumTrendMode == TrendModeHistory)
		RefreshHistData();
}

BOOL CSerial::IsShowCurrentData()
{
	if (m_pChart->m_enumTrendMode == TrendModeHistory)
	{
		COleDateTime dtEndTime(GetTimeAxis()->m_dMaxVal);
		if (dtEndTime > COleDateTime::GetCurrentTime())
			return FALSE;
	}

	return TRUE;
}

void CSerial::RefreshHistData()
{
	if (m_dwTagCookie == 0)
		return;

	RemoveAllValues();

	COleDateTime dtStartTime(GetTimeAxis()->m_dMinVal);
	SYSTEMTIME stStartTime;
	dtStartTime.GetAsSystemTime(stStartTime);
	FILETIME ftStartTime;
	SystemTimeToFileTime(&stStartTime, &ftStartTime);
	LocalFileTimeToFileTime(&ftStartTime, &ftStartTime);

	COleDateTime dtEndTime(GetTimeAxis()->m_dMaxVal);
	SYSTEMTIME stEndTime;
	dtEndTime.GetAsSystemTime(stEndTime);
	FILETIME ftEndTime;
	SystemTimeToFileTime(&stEndTime, &ftEndTime);
	LocalFileTimeToFileTime(&ftEndTime, &ftEndTime);
	
	FILETIME* pftTimeStamps;
	VARIANT* pvDataValues;
	
	COleDateTimeSpan dtSpan = dtEndTime - dtStartTime;
	DWORD dwNumValues = DWORD(dtSpan.GetTotalSeconds() * 1000 / m_pChart->m_dwRefreshInterval);
	dwNumValues = min(dwNumValues, (DWORD)m_nMaxNumDisplayPoints) + 2;

	HRESULT hr = m_pChart->m_spDataSession->ReadTagHistoryData(m_dwTagCookie,
			TRUE, &ftStartTime, &ftEndTime, &dwNumValues, &pftTimeStamps, &pvDataValues);
	if (!FAILED(hr))
	{
		DATAPOINT datapoint;

		for (DWORD dw = 0; dw < dwNumValues; dw++)
		{
			FILETIME ftTimeStamp = pftTimeStamps[dw];
			FileTimeToLocalFileTime(&ftTimeStamp, &ftTimeStamp); //转化为本地时间
			COleDateTime dtTimeStamp(ftTimeStamp);   // 毫秒位给去掉了
			datapoint.dXVal = dtTimeStamp.m_dt;

			CComVariant& var = (CComVariant&)pvDataValues[dw];
			var.ChangeType(VT_R8);
			datapoint.dYVal = var.dblVal;

			AddValue(datapoint);
		}

		if (pftTimeStamps)
			CoTaskMemFree(pftTimeStamps);
		if (pvDataValues)
			CoTaskMemFree(pvDataValues);
	}
}

void CSerial::GetIndexByXVal(long& nIndex, double x)
{
	// 使用二分法查找最接近的值
	int l, r, m;

	l = 0;
	r = m_values.GetSize() - 1;

	if (r < 0)
	{
		nIndex = -1;
		return;
	}

	while (l <= r)
	{
		m = (l + r) / 2;
		if (x < m_values[m].dXVal)
		{
			r = m - 1;
		}
		else
		{
			l = m + 1;
		}
	}

	nIndex = l;

	if (nIndex >= m_values.GetSize())
		nIndex = m_values.GetSize() - 1;
	if (nIndex > 0)
	{
		if (fabs(m_values[nIndex - 1].dXVal - x) < fabs(m_values[nIndex].dXVal - x))
			nIndex = nIndex - 1;
	}


//	double dStep;
//
//	int nCount = m_values.GetSize();
//	if (nCount == 0)
//	{
//		nIndex = -1;
//		return;
//	}
//	
//	if (nCount == 1)
//	{
//		nIndex = 0;
//		return;
//	}
//
//	dStep = fabs(m_values[1].dXVal - m_values[0].dXVal);
//	
//	nIndex = (long)((x - m_values[0].dXVal) / dStep + 0.5);
//	
//	if (nIndex < 0)
//		nIndex = 0;
//	
//	if (nIndex >= nCount)
//		nIndex = nCount - 1;
}

STDMETHODIMP CSerial::get_Show(VARIANT_BOOL *pVal)
{
	*pVal = m_bShow;

	return S_OK;
}

STDMETHODIMP CSerial::put_Show(VARIANT_BOOL newVal)
{
	m_bShow = newVal;
	m_pChart->FireViewChange();

	return S_OK;
}

STDMETHODIMP CSerial::get_MinValueLimit(/*[out, retval]*/ double *pVal)
{
	*pVal = m_dMinValLimit;
	return S_OK;
}

STDMETHODIMP CSerial::put_MinValueLimit(/*[in]*/ double newVal)
{
	m_dMinValLimit = newVal;
	return S_OK;
}

STDMETHODIMP CSerial::get_MaxValueLimit(/*[out, retval]*/ double *pVal)
{
	*pVal = m_dMaxValLimit;
	return S_OK;
}

STDMETHODIMP CSerial::put_MaxValueLimit(/*[in]*/ double newVal)
{
	m_dMaxValLimit = newVal;
	return S_OK;
}