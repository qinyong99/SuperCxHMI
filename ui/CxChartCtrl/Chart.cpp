// Chart.cpp: implementation of the CChart class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxChartCtrl.h"
#include "Chart.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChart::CChart()
{
	m_bInitPrint = FALSE;
}

CChart::~CChart()
{
	int nCount = m_apSerials.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		m_apSerials[i]->Release();
	}

	if (m_pTimeAxis)
		m_pTimeAxis->Release();
	if (m_pValueAxis)
		m_pValueAxis->Release();
	if (m_pGrid)
		m_pGrid->Release();
}

STDMETHODIMP CChart::InitNew()
{
	m_dwRefreshInterval = 1000;
	m_bTransparency = VARIANT_FALSE;
	m_clrFore = 0x80000000 | COLOR_WINDOW;
	m_clrBack = 0x80000000 | COLOR_BTNFACE;
	m_enumFrameStyle = FrameStyleNone;
	m_clrFrame = 0x80000000 | COLOR_WINDOWFRAME;
	m_bShowMutiTimeAxis = VARIANT_FALSE;
	m_bShowMutiValueAxis = VARIANT_FALSE;
	m_bShowMutiGrid = VARIANT_FALSE;
	m_bShowLegend = VARIANT_TRUE;
	m_enumScrollDirection = ScrollDirectionLeft;
	m_enumZoomMode = ZoomModeBoth;
	m_fScrollPercentage = 0.1f;
		
	m_enumTrendMode = TrendModeRealTime;
		
	GetDefaultFont(&m_spCaptionFont);
	m_clrCaption = 0x80000000 | COLOR_WINDOWTEXT;

	m_pTimeAxis = new CComObject<CTimeAxis>;
	m_pTimeAxis->m_pChart = this;
	m_pTimeAxis->InitNew();
	m_pTimeAxis->AddRef();
	
	m_pValueAxis = new CComObject<CValueAxis>;
	m_pValueAxis->m_pChart = this;
	m_pValueAxis->InitNew();
	m_pValueAxis->AddRef();

	m_pGrid = new CComObject<CGrid>;
	m_pGrid->m_pChart = this;
	m_pGrid->InitNew();
	m_pGrid->AddRef();
	
//	AddSerial(_T("Tag"));

	return S_OK;
}

STDMETHODIMP CChart::Save(LPSTREAM pStm, BOOL fClearDirty)
{
	m_pTimeAxis->Save(pStm, fClearDirty);
	m_pValueAxis->Save(pStm, fClearDirty);
	m_pGrid->Save(pStm, fClearDirty);

	int nCount = m_apSerials.GetSize();
	pStm->Write(&nCount, sizeof(nCount), NULL);
	for (int i = 0; i < nCount; i++)
		m_apSerials[i]->Save(pStm, fClearDirty);
	
	return S_OK;
}

STDMETHODIMP CChart::Load(LPSTREAM pStm)
{
	m_pTimeAxis = new CComObject<CTimeAxis>;
	m_pTimeAxis->m_pChart = this;
	m_pTimeAxis->AddRef();
	m_pTimeAxis->Load(pStm);
	
	m_pValueAxis = new CComObject<CValueAxis>;
	m_pValueAxis->m_pChart = this;
	m_pValueAxis->AddRef();
	m_pValueAxis->Load(pStm);
	
	m_pGrid = new CComObject<CGrid>;
	m_pGrid->m_pChart = this;
	m_pGrid->AddRef();
	m_pGrid->Load(pStm);

	int nCount; 
	pStm->Read(&nCount, sizeof(nCount), NULL);
	for (int i = 0; i < nCount; i++)
	{
		CComObject<CSerial>* pSerial = new CComObject<CSerial>;
		pSerial->m_pChart = this;
		pSerial->Load(pStm);
		pSerial->AddRef();
		m_apSerials.Add(pSerial);
	}
	
	InitSerialDemoVaules();
	RecalcRects();
	
	return S_OK;
}

void CChart::DoDraw(HDC hdcDest)
{
	USES_CONVERSION;

	int iOldMode = ::SetBkMode(hdcDest, TRANSPARENT);

	COLORREF clrCaption;
	OleTranslateColor(m_clrCaption, NULL, &clrCaption);
	COLORREF clrOldTextColor = ::SetTextColor(hdcDest, clrCaption);

	HFONT hFont;
	HFONT hOldFont;
	if (m_spCaptionFont != NULL)
	{
		CComPtr<IFont> pFont;
		m_spCaptionFont->QueryInterface(IID_IFont, (void**)&pFont);
		pFont->get_hFont(&hFont);
	}
	else
	{
		hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	}
	hOldFont = (HFONT)::SelectObject(hdcDest, hFont);

	RECT rcTemp;

	//填充背景
	if (!m_bTransparency)
	{
		COLORREF clrBack;
		OleTranslateColor(m_clrBack, NULL, &clrBack);
		HBRUSH brush = ::CreateSolidBrush(clrBack);

		CopyRect(&rcTemp, &m_rect);
		rcTemp.right = m_rcSerial.left;
		::FillRect(hdcDest, &rcTemp, brush);
		rcTemp.left = m_rcSerial.right;
		rcTemp.right = m_rect.right;
		::FillRect(hdcDest, &rcTemp, brush);

		CopyRect(&rcTemp, &m_rcSerial);
		rcTemp.top = m_rect.top;
		rcTemp.bottom = m_rcSerial.top;
		::FillRect(hdcDest, &rcTemp, brush);

		rcTemp.top = m_rcSerial.bottom;
		rcTemp.bottom = m_rect.bottom;
		::FillRect(hdcDest, &rcTemp, brush);

		::DeleteObject((HGDIOBJ)brush);

		//填充前景
		COLORREF clrFore;
		OleTranslateColor(m_clrFore, NULL, &clrFore);
		brush = ::CreateSolidBrush(clrFore);
		CopyRect(&rcTemp, &m_rcSerial);
		if (!::IsRectEmpty(&m_rcSerial))
			::FillRect(hdcDest, &m_rcSerial, brush);
		::DeleteObject((HGDIOBJ)brush);
	}

	// 绘制标题
	CopyRect(&rcTemp, &m_rcSerial);
	rcTemp.top = m_rect.top;
	rcTemp.bottom = m_rcSerial.top;
	::DrawText(hdcDest, W2T(m_bstrTitle), -1, &rcTemp, DT_SINGLELINE | DT_CENTER| DT_VCENTER);
	
	//画边框
	switch (m_enumFrameStyle)
	{
	case FrameStyleRaised:
		DrawEdge(hdcDest, &m_rect, BDR_RAISEDINNER, BF_RECT);
		break;
	case FrameStyleSunken:
		DrawEdge(hdcDest, &m_rect, BDR_SUNKENOUTER, BF_RECT);
		break;
	case FrameStyleEtched:
	case FrameStyleBump:
	case FrameStyleRectangle:
		{
			COLORREF clrFrame;
			if (m_enumFrameStyle == FrameStyleRectangle)
			{
				OleTranslateColor(m_clrFrame, NULL, &clrFrame);
			}
			else
			{
				clrFrame = m_enumFrameStyle == FrameStyleEtched ?
						GetSysColor(COLOR_BTNHILIGHT) : GetSysColor(COLOR_BTNSHADOW);
			}
					
			HPEN hFramePen = CreatePen(PS_SOLID, 1, clrFrame);
			HPEN hOldPen = (HPEN)SelectObject(hdcDest, hFramePen);
			
			MoveToEx(hdcDest, m_rect.left, m_rect.top, NULL);
			LineTo(hdcDest, m_rect.right, m_rect.top);
			LineTo(hdcDest, m_rect.right, m_rect.bottom);
			LineTo(hdcDest, m_rect.left, m_rect.bottom);
			LineTo(hdcDest, m_rect.left, m_rect.top);
			
			SelectObject(hdcDest, hOldPen);
			DeleteObject(hFramePen);
		}
		break;
	default:
		break;
	}

	int nCount = m_apSerials.GetSize();
	for (int i = 0; i < nCount; i++)
		m_apSerials[i]->DoDraw(hdcDest, m_rcSerial, m_bShowMutiTimeAxis, m_bShowMutiValueAxis, m_bShowMutiGrid, m_bShowLegend);
	
	if (!m_bShowMutiTimeAxis)
		m_pTimeAxis->DoDraw(hdcDest);
	if (!m_bShowMutiValueAxis)
		m_pValueAxis->DoDraw(hdcDest);
	if (!m_bShowMutiGrid)
		m_pGrid->DoDraw(hdcDest, m_rcSerial);


//	CRgn rgnSerial;
//	rgnSerial.CreateRectRgnIndirect(&m_rcSerial);
	
	// PRB: Clipping doesn't work in print preview, no idea why ...
//	SelectClipRgn(hdcDest, rgnSerial);

	//绘制画线
	for (i = 0; i < nCount; i++)
		m_apSerials[i]->DoDrawCurve(hdcDest, m_rcSerial);

	::SetBkMode(hdcDest, iOldMode);
	::SetTextColor(hdcDest, clrOldTextColor);
	::SelectObject(hdcDest, hOldFont);
}

void CChart::InitSerialDemoVaules()
{
	int nCount = m_apSerials.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		m_apSerials[i]->InitDemoValues();
	}
}

static COLORREF clrInitTable[] = 
{
	RGB(255,   0,   0),
	RGB(  0, 255,   0),
	RGB(  0,   0, 255),
	RGB(255,   0, 255),
	RGB(255, 255,   0),
	RGB(  0, 255, 255),
	RGB(128, 128, 128),
	RGB(  0,   0, 128)
};

COLORREF CChart::GetNotUsedSerialColor()
{
	int nCount = m_apSerials.GetSize();
	for (int i = 0; i < 8; i++)
	{
		BOOL bUsed = FALSE;
		for (int j = 0; j < nCount; j++)
		{
			if (m_apSerials[j]->m_clrLineColor == clrInitTable[i])
			{
				bUsed = TRUE;
			}
		}
		
		if (!bUsed)
			return clrInitTable[i];
	}
	
	return rand() % RGB(255, 255, 255);
}

HRESULT CChart::FireViewChange()
{
	return S_OK;
}

BOOL CChart::ClientIsRunMode()
{
	return TRUE;
}

// 计算图例总高度
int CChart::RecalcLegendsRect(RECT rect)
{
	int nLegendsHeight = 0;
	int nCount = m_apSerials.GetSize();	

	// 计算出最大的图例宽度
	int nLargestWidth = 0;
	for (int i = 0; i < nCount; i++)
	{
		CSerial* pSerial = m_apSerials[i];
		pSerial->CalcLegendLargestWidth(nLargestWidth);
	}
	if (nLargestWidth == 0)
		return 0;

	int nShowLegendCount = 0;
	for (i = 0; i < nCount; i++)
	{
		CSerial* pSerial = m_apSerials[i];
		if (pSerial->m_pLegend != NULL && pSerial->m_pLegend->m_bShow)
			nShowLegendCount++;
	}

	// 每行的图例数目
	int nLegendsPerLine = (rect.right - rect.left + 40) / nLargestWidth;
	if (nLegendsPerLine < 1)
		nLegendsPerLine = 1;
	// 每行显示使用最佳对齐方式
	int nLineCount = (nShowLegendCount + nLegendsPerLine - 1) / nLegendsPerLine;
	nLegendsPerLine = (nShowLegendCount + nLineCount - 1) / nLineCount;

	nShowLegendCount = 0;
	int nLegendsLineHeight = 0;
	BOOL bEnlargeLineHeight = FALSE;
	CSimpleArray<CSerial *> apNeedAdjustSerials;

	for (i = 0; i < nCount; i++)
	{
		CSerial* pSerial = m_apSerials[i];

		RECT rcT = rect;
		rcT.left += (nShowLegendCount % nLegendsPerLine) * nLargestWidth;
		rcT.right = min(rcT.left + nLargestWidth, rect.right);
		pSerial->RecalcLegend(rcT);

		if (rcT.top != rect.top)
		{
			apNeedAdjustSerials.Add(pSerial);

			if (nLegendsLineHeight < rcT.top - rect.top)
			{
				if (nLegendsLineHeight != 0)
					bEnlargeLineHeight = TRUE;
				nLegendsLineHeight = rcT.top - rect.top;
			}

			nShowLegendCount++;
			if (nShowLegendCount % nLegendsPerLine == 0)
			{
				if (bEnlargeLineHeight)
				{
					for (int j = 0; j < apNeedAdjustSerials.GetSize(); j++)
						apNeedAdjustSerials[j]->AdjustLegendHeight(nLegendsLineHeight);
				}

				nLegendsHeight += nLegendsLineHeight;
				rect.top += nLegendsLineHeight;
				nLegendsLineHeight = 0;
				apNeedAdjustSerials.RemoveAll();
			}
		}
	}

	if (bEnlargeLineHeight)
	{
		for (int j = 0; j < apNeedAdjustSerials.GetSize(); j++)
			apNeedAdjustSerials[j]->AdjustLegendHeight(nLegendsLineHeight);
	}

	nLegendsHeight += nLegendsLineHeight;

	return nLegendsHeight;
}

BOOL CChart::RecalcRects()
{
	int nCount = m_apSerials.GetSize();	

	RECT rect = m_rect;
	rect.right -= 8;
	rect.top += 12 + GetFontHeight(m_spCaptionFont);

	RECT rcTemp = rect;

	//计算chart位置

	for (int i = nCount - 1; i >= 0; i--)
	{
		CSerial* pSerial = m_apSerials[i];
		if (m_bShowMutiTimeAxis)
			pSerial->RecalcTimeAxis(rect);
		if (m_bShowMutiValueAxis)
			pSerial->RecalcValueAxis(rect);
	}

	if (!m_bShowMutiTimeAxis)
		m_pTimeAxis->RecalcRect(rect);
	if (!m_bShowMutiValueAxis)
		m_pValueAxis->RecalcRect(rect);

	// 计算图例高度
	int nLegendsHeight = 0;
	if (m_bShowLegend)
		nLegendsHeight = RecalcLegendsRect(rect);
	// 减去图例高度
	rect.bottom -= nLegendsHeight;
	
	// 计算图例位置
	if (m_bShowLegend)
	{
		RECT rcT = rect;
		rcT.bottom = rcTemp.bottom;
		rcT.top = rcTemp.bottom - nLegendsHeight;
		RecalcLegendsRect(rcT);
	}
	
	// 计算时间轴位置
	rect.bottom = rcTemp.bottom - nLegendsHeight;
	if (m_bShowMutiTimeAxis)
	{
		for (i = nCount - 1; i >= 0; i--)
		{
			CSerial* pSerial = m_apSerials[i];
			pSerial->RecalcTimeAxis(rect);
		}
	}
	else
	{
		m_pTimeAxis->RecalcRect(rect);
	}

	// 计算值轴位置
	rect.left = rcTemp.left;
	if (m_bShowMutiValueAxis)
	{
		for (i = nCount - 1; i >= 0; i--)
		{
			CSerial* pSerial = m_apSerials[i];
			pSerial->RecalcValueAxis(rect);
		}
	}
	else
	{
		m_pValueAxis->RecalcRect(rect);
	}
	
	m_rcSerial = rect;

	return TRUE;
}

// 更新数据源的属性
void CChart::UpdateSerialSourceProp(CSerial* pSerial)
{
	ATLASSERT(m_spDataSession != NULL);
	
	DWORD dwCount = 2;
	
	DWORD dwPropertyIDs[2];
	dwPropertyIDs[0] = 0x80010002;//CXDBPROPID_Desc; // 描述
	dwPropertyIDs[1] = 0x80010011;//CXDBPROPID_EngineerUnit; // 单位
	VARIANT* pvData = NULL;
	HRESULT* pErrors = NULL;
	HRESULT hr = m_spDataSession->GetTagProperties(pSerial->m_bstrSource, dwCount, dwPropertyIDs, &pvData, &pErrors);
	if (FAILED(hr))
		goto onexit;
	
	if (pErrors[0] == S_OK && pvData[0].vt == VT_BSTR)
		pSerial->m_bstrSourceDesc = pvData[0].bstrVal;
	if (pErrors[1] == S_OK && pvData[1].vt == VT_BSTR)
		pSerial->m_bstrSourceUnit = pvData[1].bstrVal;
	
onexit:
	if (pvData != NULL)
	{
		for (DWORD dw = 0; dw < dwCount; dw++)
			VariantClear(pvData + dw);
		CoTaskMemFree(pvData);
	}
	if (pErrors != NULL)
		CoTaskMemFree(pErrors);
}

// 连接到数据服务器
HRESULT CChart::ConnectToDataServer(CSerial* pAddSerial, CSerial* pRemoveSerial)
{
	HRESULT hr;
	
	if (m_spDataSession == NULL)
	{
		hr = m_spDataSession.CoCreateInstance(CLSID_CxDataSession);
		if (FAILED(hr))
			return hr;

		CComBSTR bstrServer;
		GetDefaultServer(&bstrServer);
		m_spDataSession->SetThisNode(bstrServer, NULL, FALSE);
	}
	ATLASSERT(m_spDataSession != NULL);
	

	if (pRemoveSerial != NULL)
	{
		ATLASSERT(pRemoveSerial->m_dwTagCookie != 0);

		// 如果有序列使用同样的标签，则不移除
		BOOL bRemoved = TRUE;
		int nCount = m_apSerials.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			CSerial* pSerial = m_apSerials[i];
			if (pSerial->m_dwTagCookie == pRemoveSerial->m_dwTagCookie)
			{
				if (pSerial != pRemoveSerial)
				{
					bRemoved = FALSE;
					break;
				}
			}
		}
		
		if (bRemoved)
		{
			hr = m_spDataSession->CancelRequestTag(1, &pRemoveSerial->m_dwTagCookie);
			if (!FAILED(hr))
			{
				pRemoveSerial->m_dwTagCookie = 0;
			}
		}
	}

	if (pAddSerial != NULL)
	{
		m_spDataSession->RequestTag(pAddSerial->m_bstrTag, &pAddSerial->m_dwTagCookie);
		
		UpdateSerialSourceProp(pAddSerial);
	}
	
	// 如果增加序列和删除序列参数都为空，则认为是增加所有序列
	if (pAddSerial == NULL && pRemoveSerial == NULL)
	{
		int nCount = m_apSerials.GetSize();

		CSimpleArray<DWORD> arrCookies;
		for (int i = 0; i < nCount; i++)
		{
			CSerial* pSerial = m_apSerials[i];
			if (pSerial->m_dwTagCookie != 0)
			{
				arrCookies.Add(pSerial->m_dwTagCookie);
				pSerial->m_dwTagCookie = 0;
			}
		}

		if (arrCookies.GetSize() > 0)
			m_spDataSession->CancelRequestTag(arrCookies.GetSize(), arrCookies.GetData());

		for (i = 0; i < nCount; i++)
		{
			CSerial* pSerial = m_apSerials[i];
	
			m_spDataSession->RequestTag(pSerial->m_bstrTag, &pSerial->m_dwTagCookie);
			UpdateSerialSourceProp(pSerial);
		}
	}

	m_spDataSession->CommitTags(FALSE);
		
	return S_OK;
}

BOOL CChart::OnRefreshData(BOOL bForce)
{
	if (m_spDataSession == NULL)
		return TRUE;

	if (m_enumTrendMode == TrendModeHistory && !bForce)
	{
//		if (bForce)
//		{
//			int nCount = m_apSerials.GetSize();
//			for (int i = 0; i < nCount; i++)
//			{
//				m_apSerials[i]->RefreshHistData();
//			}
//
//		}
		
		return bForce;
	}
	
	m_spDataSession->RefreshTags();
	
	int nCount = m_apSerials.GetSize();
	DATAPOINT datapoint;

	if (!m_bShowMutiTimeAxis && m_enumTrendMode == TrendModeRealTime)
		m_pTimeAxis->ScrollForwardMost();
	
	for (int i = 0; i < nCount; i++)
	{
		CSerial* pSerial = m_apSerials[i];

		if (bForce)
			pSerial->RefreshHistData();

		if (pSerial->IsShowCurrentData())
		{
			if (pSerial->m_dwTagCookie != 0)
			{
				CComVariant var;
				FILETIME ftTimeStamp;
				HRESULT hr = m_spDataSession->ReadTagByCookie(pSerial->m_dwTagCookie, &var, &ftTimeStamp);
				if (!FAILED(hr))
				{
					FileTimeToLocalFileTime(&ftTimeStamp, &ftTimeStamp); //转化为本地时间
					COleDateTime dtTimeStamp(ftTimeStamp);
					datapoint.dXVal = dtTimeStamp.m_dt;
					var.ChangeType(VT_R8);
					datapoint.dYVal = var.dblVal;
					pSerial->AddValue(datapoint);
				}
			}
			else
			{
				OnRefreshSerialData(pSerial);
			}
		}
		
		if (m_enumTrendMode == TrendModeRealTime)
			pSerial->ScrollForwardMost();
	}

	return TRUE;
}

BOOL CChart::OnRefreshSerialData(CSerial* pSerial)
{
	return FALSE;
}

void CChart::OnPrint()
{
	if (!m_bInitPrint)
	{
		m_printer.OpenDefaultPrinter();
		m_devmode.CopyFromPrinter(m_printer);
		m_rectMargin.SetRect(900, 720, 900, 720);

		m_bInitPrint = TRUE;
	}

	CPrintDialog dlg(FALSE);
	dlg.m_pd.hDevMode = m_devmode.CopyToHDEVMODE();
	dlg.m_pd.hDevNames = m_printer.CopyToHDEVNAMES();
	dlg.m_pd.nMinPage = 1;
	dlg.m_pd.nMaxPage = 1;
	
	if(dlg.DoModal() == IDOK)
	{
		m_devmode.CopyFromHDEVMODE(dlg.m_pd.hDevMode);
		m_printer.ClosePrinter();
		m_printer.OpenPrinter(dlg.m_pd.hDevNames, m_devmode.m_pDevMode);
		
		CPrintJob job;
		job.StartPrintJob(false, m_printer, m_devmode.m_pDevMode, this, _T("趋势打印"), 0, 0);
	}
	
	::GlobalFree(dlg.m_pd.hDevMode);
	::GlobalFree(dlg.m_pd.hDevNames);
}

bool CChart::PrintPage(UINT nPage, HDC hDC)
{
	if (nPage >= 1)		// we have only one page
		return false;

	CDCHandle dc = hDC;

	POINT pt;
	CRect rectMargin;
	CRect rectPage;
	
	CRect rect = m_rect;
	
	dc.Escape(GETPRINTINGOFFSET, 0, NULL, &pt);
	rectMargin.left = pt.x;
	rectMargin.top  = pt.y;
	
	dc.Escape(GETPHYSPAGESIZE, 0, NULL, &pt);
	rectPage.SetRect(0, 0, dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));
	
	rectMargin.right  = pt.x                     // total paper width
		- dc.GetDeviceCaps(HORZRES) // printable width
		- rectMargin.left;   // left unprtable margin
	
	rectMargin.bottom = pt.y                     // total paper height
		- dc.GetDeviceCaps(VERTRES) // printable ht
		- rectMargin.top;    // rt unprtable margin
	
	pt.x = dc.GetDeviceCaps(LOGPIXELSX);    // dpi in X direction
	pt.y = dc.GetDeviceCaps(LOGPIXELSY);    // dpi in Y direction
	
	rectMargin.left = MulDiv(m_rectMargin.left, pt.x, 1440)
		- rectMargin.left;
	rectMargin.top  = MulDiv(m_rectMargin.top, pt.y, 1440)
		- rectMargin.top;
	rectMargin.right  = MulDiv(m_rectMargin.right, pt.x, 1440)
		- rectMargin.right;
	rectMargin.bottom = MulDiv(m_rectMargin.bottom, pt.y, 1440)
		- rectMargin.bottom;
	
	rectPage.InflateRect(-rectMargin.left, -rectMargin.top, -rectMargin.right, -rectMargin.bottom);
	
	float fXScale = (float)rectPage.Width() / rect.Width();
	float fYScale = (float)rectPage.Height() / rect.Height();
	float fScale = min(fXScale, fYScale); // max(min(nXScale, nYScale), 1);
	
	int cxBlt = int(rect.Width() * fScale);
	int cyBlt = int(rect.Height() * fScale);
	int xOff = (rectPage.left + rectPage.right) / 2 - cxBlt / 2;
	int yOff = (rectPage.top + rectPage.bottom) / 2 - cyBlt / 2;
	
	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetViewportOrg(xOff, yOff);
	dc.SetViewportExt(cxBlt, cyBlt);
	dc.SetWindowExt(rect.Width(), rect.Height());
	rect.OffsetRect(-rect.left, 0);
	dc.IntersectClipRect(&rect);
	
	DoDraw(dc);
	
	return true;
}