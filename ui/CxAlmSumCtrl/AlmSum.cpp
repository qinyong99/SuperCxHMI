// AlmSum.cpp: implementation of the CAlmSum class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxAlmSumCtrl.h"
#include "AlmSum.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlmSum::CAlmSum() :
	m_scrollbarHor(FALSE),
	m_pAlarmSinkWnd(NULL)
{
	m_pCtrlWithCapture = NULL;

	m_nSelectAlarm = -1;

	m_header.m_pAlmSum = this;
	m_grid.m_pAlmSum = this;
	m_scrollbarHor.m_pAlmSum = this;
	m_scrollbarVer.m_pAlmSum = this;

	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight = 12;
	logFont.lfWeight = FW_NORMAL;
	lstrcpy(logFont.lfFaceName, _T("Wingdings"));
	m_fontWinnding.CreateFontIndirect(&logFont);

	::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &m_nRowsPerWheelNotch, 0);
}

CAlmSum::~CAlmSum()
{
 	Release();
}

void CAlmSum::Release()
{
	if (m_pAlarmSinkWnd != NULL)
	{
		m_pAlarmSinkWnd->DestroyWindow();
		delete m_pAlarmSinkWnd;
		m_pAlarmSinkWnd = NULL;
	}

	int nCount = (int)m_apAlarms.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		delete m_apAlarms[i];
	}
	m_apAlarms.RemoveAll();
	
	nCount = m_apColumns.GetSize();
	for (i = 0; i < nCount; i++)
	{
		delete m_apColumns[i];
	}
	m_apColumns.RemoveAll();
	
	nCount = m_apForeFont.GetSize();
	for (i = 0; i < nCount; i++)
	{
		delete m_apForeFont[i];
	}
	m_apForeFont.RemoveAll();
	
	nCount = m_apBackColor.GetSize();
	for (i = 0; i < nCount; i++)
	{
		delete m_apBackColor[i];
	}
	m_apBackColor.RemoveAll();
}

static COLORREF clrInitTable[] = 
{
	RGB(  0, 255,   0),
	RGB(255,   0,   0),
	RGB(  0,   0, 255),
	RGB(255,   0, 255),
	RGB(255, 255,   0),
	RGB(  0, 255, 255),
	RGB(128, 128, 128),
	RGB(  0,   0, 128),
	RGB(  0, 128,	0),
	RGB(128,   0,   0),
	RGB(  0, 128, 128),
};

STDMETHODIMP CAlmSum::InitNew()
{
	m_bShowHeader = VARIANT_TRUE;
	m_bShowScrollBar = VARIANT_TRUE;
	
	m_bShowGrid = VARIANT_TRUE;
	m_bAutoScroll = VARIANT_TRUE;

	m_clrBack = 0x80000000 | COLOR_WINDOW;
	m_clrHeaderFore = 0x80000000 | COLOR_BTNTEXT;
	m_clrHeaderBack = 0x80000000 | COLOR_BTNFACE;

	m_bEnableAck = VARIANT_TRUE;
	m_bEnableContextMenu = VARIANT_TRUE;
	m_bEnableOnlineConfigure = VARIANT_TRUE;

	m_nSortField = AlarmField_TagName;
	m_nSecondSortField = (enumAlarmField)-1;
	m_bSortAscending = TRUE;

	m_bstrNodeFilter = L"*";
	m_bstrAreaFilter = L"*";
	m_bstrPriorityFilter = L"*";

	CColumn* pCol = new CColumn(AlarmField_Ack);
	m_apColumns.Add(pCol);
	pCol = new CColumn(AlarmField_TimeIn);
	m_apColumns.Add(pCol);
	pCol = new CColumn(AlarmField_Node);
	m_apColumns.Add(pCol);
	pCol = new CColumn(AlarmField_TagName);
	m_apColumns.Add(pCol);
	pCol = new CColumn(AlarmField_Value);
	m_apColumns.Add(pCol);
	pCol = new CColumn(AlarmField_Status);
	m_apColumns.Add(pCol);
	pCol = new CColumn(AlarmField_Priority);
	m_apColumns.Add(pCol);
	pCol = new CColumn(AlarmField_Area);
	m_apColumns.Add(pCol);
	pCol = new CColumn(AlarmField_Description);
	m_apColumns.Add(pCol);
	
	UpdateScrollbarVer(TRUE, FALSE);
	
	CComPtr<IFontDisp> spFont;
	GetDefaultFont(&spFont);
	
	for (int i = 0; i < NUM_STATUSS; i++)
	{
		CForeFont * pForeFont = new CForeFont(spFont, clrInitTable[i % 11]/*GetSysColor(COLOR_WINDOWTEXT)*/);
		m_apForeFont.Add(pForeFont);
	}
	
	for (i = 0; i < NUM_PRIORITYS; i++)
	{
		CBackColor * pBackColor = new CBackColor(GetSysColor(COLOR_WINDOW));
		m_apBackColor.Add(pBackColor);
	}

	GetDefaultFont(&m_spHeaderFont);
	m_lRowHeight = GetFontHeight(m_spHeaderFont) + 2;

//	RecalcRects();
	InitDemo();

	return S_OK;
}

STDMETHODIMP CAlmSum::Load(LPSTREAM pStm)
{
	ReadFontFromStream(&m_spHeaderFont, pStm);
	pStm->Read(&m_lRowHeight, sizeof(m_lRowHeight), NULL);

	pStm->Read(&m_clrBack, sizeof(OLE_COLOR), NULL);
	pStm->Read(&m_clrHeaderFore, sizeof(OLE_COLOR), NULL);
	pStm->Read(&m_clrHeaderBack, sizeof(OLE_COLOR), NULL);
	
	pStm->Read(&m_bShowHeader, sizeof(m_bShowHeader), NULL);
	pStm->Read(&m_bShowScrollBar, sizeof(m_bShowScrollBar), NULL);
	pStm->Read(&m_bShowGrid, sizeof(m_bShowGrid), NULL);
	pStm->Read(&m_bAutoScroll, sizeof(m_bAutoScroll), NULL);
	
	pStm->Read(&m_bEnableAck, sizeof(m_bEnableAck), NULL);
	pStm->Read(&m_bEnableContextMenu, sizeof(m_bEnableContextMenu), NULL);
	pStm->Read(&m_bEnableOnlineConfigure, sizeof(m_bEnableOnlineConfigure), NULL);

	pStm->Read(&m_nSortField, sizeof(m_nSortField), NULL);
	pStm->Read(&m_nSecondSortField, sizeof(m_nSecondSortField), NULL);
	pStm->Read(&m_bSortAscending, sizeof(BOOL), NULL);
	m_bstrNodeFilter.ReadFromStream(pStm);
	m_bstrAreaFilter.ReadFromStream(pStm);
	m_bstrPriorityFilter.ReadFromStream(pStm);

	int nCount; 
	pStm->Read(&nCount, sizeof(nCount), NULL);
	for (int i = 0; i < nCount; i++)
	{
		enumAlarmField af;
		pStm->Read(&af, sizeof(enumAlarmField), NULL);
		CColumn* pCol = new CColumn(af);
		m_apColumns.Add(pCol);
	}

	UpdateScrollbarVer(TRUE, FALSE);
	
	pStm->Read(&nCount, sizeof(nCount), NULL);
	for (i = 0; i < nCount; i++)
	{
		OLE_COLOR clr;
		CComPtr<IFontDisp> spFont;
		
		pStm->Read(&clr, sizeof(OLE_COLOR), NULL);
		HRESULT hr = OleLoadFromStream(pStm,
			IID_IFontDisp,
			(void**)&spFont);
		if (FAILED(hr))
			ATLASSERT(FALSE);
		
		CForeFont * pForeFont = new CForeFont(spFont, clr);
		m_apForeFont.Add(pForeFont);
	}
	
	pStm->Read(&nCount, sizeof(nCount), NULL);
	for (i = 0; i < nCount; i++)
	{
		OLE_COLOR clr;
		pStm->Read(&clr, sizeof(OLE_COLOR), NULL);
		CBackColor * pBackColor = new CBackColor(clr);
		m_apBackColor.Add(pBackColor);
	}
	
	RecalIdealColumnWidth();
// 	RecalcRects();
	InitDemo();

	return S_OK;
}

STDMETHODIMP CAlmSum::Save(LPSTREAM pStm, BOOL fClearDirty)
{
	SaveFontToStream(m_spHeaderFont, pStm);
	pStm->Write(&m_lRowHeight, sizeof(m_lRowHeight), NULL);

	pStm->Write(&m_clrBack, sizeof(OLE_COLOR), NULL);
	pStm->Write(&m_clrHeaderFore, sizeof(OLE_COLOR), NULL);
	pStm->Write(&m_clrHeaderBack, sizeof(OLE_COLOR), NULL);

	pStm->Write(&m_bShowHeader, sizeof(m_bShowHeader), NULL);
	pStm->Write(&m_bShowScrollBar, sizeof(m_bShowScrollBar), NULL);
	pStm->Write(&m_bShowGrid, sizeof(m_bShowGrid), NULL);
	pStm->Write(&m_bAutoScroll, sizeof(m_bAutoScroll), NULL);

	pStm->Write(&m_bEnableAck, sizeof(m_bEnableAck), NULL);
	pStm->Write(&m_bEnableContextMenu, sizeof(m_bEnableContextMenu), NULL);
	pStm->Write(&m_bEnableOnlineConfigure, sizeof(m_bEnableOnlineConfigure), NULL);

	pStm->Write(&m_nSortField, sizeof(m_nSortField), NULL);
	pStm->Write(&m_nSecondSortField, sizeof(m_nSecondSortField), NULL);
	pStm->Write(&m_bSortAscending, sizeof(BOOL), NULL);
	m_bstrNodeFilter.WriteToStream(pStm);
	m_bstrAreaFilter.WriteToStream(pStm);
	m_bstrPriorityFilter.WriteToStream(pStm);

	int nCount; 
	nCount = m_apColumns.GetSize();
	pStm->Write(&nCount, sizeof(nCount), NULL);
	for (int i = 0; i < nCount; i++)
	{
		pStm->Write(&m_apColumns[i]->m_enumAlarmField, sizeof(enumAlarmField), NULL);
	}
	
	nCount = m_apForeFont.GetSize();
	pStm->Write(&nCount, sizeof(nCount), NULL);
	for (i = 0; i < nCount; i++)
	{
		CForeFont * pForeFont = m_apForeFont[i];
		pStm->Write(&pForeFont->m_clr, sizeof(OLE_COLOR), NULL);
		CComQIPtr<IPersistStream> spStream = pForeFont->m_spFont;
		ATLASSERT(spStream != NULL);
		OleSaveToStream(spStream, pStm);
	}
	
	nCount = m_apBackColor.GetSize();
	pStm->Write(&nCount, sizeof(nCount), NULL);
	for (i = 0; i < nCount; i++)
	{
		CBackColor * pBackColor = m_apBackColor[i];
		pStm->Write(&pBackColor->m_clr, sizeof(OLE_COLOR), NULL);
	}
	
	return S_OK;
}

BOOL CAlmSum::RecalcRects()
{
	RECT rect = m_rect;
	if (m_bShowScrollBar == VARIANT_TRUE)
	{
		m_scrollbarVer.RecalcRect(rect);
		m_scrollbarHor.RecalcRect(rect);
	}
	if (m_bShowHeader == VARIANT_TRUE)
		m_header.RecalcRect(rect);

	m_grid.m_rect = rect;

	// 设定卷动条页尺寸
	if (m_bShowScrollBar == VARIANT_TRUE)
	{
		m_scrollbarVer.SetPage(rect.bottom - rect.top);
		m_scrollbarHor.SetPage(rect.right - rect.left);
	}

	return TRUE;
}

void CAlmSum::ReConnectNetNodes()
{
	if (m_pAlarmSinkWnd == NULL)
	{
		CComBSTR bstrServer;
		GetDefaultServer(&bstrServer);
		m_pAlarmSinkWnd = new CAlarmSinkWnd(this, bstrServer);	
		RECT rcPos = { 0, 0, 0, 0 };
		if (!m_pAlarmSinkWnd->Create(NULL, rcPos, WS_OVERLAPPED))
			ATLASSERT(FALSE);
	}
	
	m_pAlarmSinkWnd->SubscribeAlarm(m_bstrNodeFilter);
}

void CAlmSum::InitDemo()
{
	if (!ClientIsRunMode())
	{
		FILETIME filetime;
		GetSystemTimeAsFileTime(&filetime);
		FileTimeToLocalFileTime(&filetime, &filetime);
		
		{
			for (int i = 0; i < 9; i++)
			{
				CAlarm* pAlarm = new CAlarm(this);
				TCHAR szBuf[32];
				sprintf(szBuf, _T("Node%d"), i + 1);
				pAlarm->m_strNode = szBuf;
				sprintf(szBuf, _T("Tag%d"), i + 1);
				pAlarm->m_strTag = szBuf;
				sprintf(szBuf, _T("描述%d"), i + 1);
				pAlarm->m_strDesc = szBuf;
				sprintf(szBuf, _T("报警区%d"), i + 1);
				pAlarm->m_strArea = szBuf;
				pAlarm->m_timeIn = filetime;
				pAlarm->m_timeOut = filetime;
				//	GetSystemTimeAsFileTime(&pAlarm->m_timeIn);
				//	FileTimeToLocalFileTime(&pAlarm->m_timeIn, &pAlarm->m_timeIn);
				//GetLocalTime(&pAlarm->m_timeIn);
				pAlarm->m_vaValue = i * 100.0f;
				pAlarm->m_strStatus = _gszStatusLabel[i % NUM_STATUSS];
				if (pAlarm->m_strStatus != _T("正常"))
				{
					pAlarm->m_bAckRequired = TRUE;
					pAlarm->m_bAck = i % 2;
				}
			
				pAlarm->m_dwSeverity = i % NUM_PRIORITYS * 333 + 100;
				
				AddAlarm(pAlarm);
			}
		}
	}
}

void CAlmSum::AddAlarm(CAlarm* pAlarm)
{
	if (!FilterAlarm(pAlarm))
		return;

	pAlarm->UpdateDrawStyle();

	if (m_nSecondSortField == -1)
	{
		AddAlarm(m_apAlarms, pAlarm, m_nSortField);
	}
	else
	{
		m_apAlarms.Add(pAlarm);
		ReSortAlarms();
	}
	
	UpdateScrollbarVer(FALSE, TRUE);
	if (m_bAutoScroll == VARIANT_TRUE)
		EnsureAlarmVisible(pAlarm);
}

void CAlmSum::ReSortAlarms()
{
	CAlarm* pSelectAlarm = NULL;
	if (m_nSelectAlarm >= 0 && m_nSelectAlarm < (int)m_apAlarms.GetCount())
	{
		pSelectAlarm = m_apAlarms[m_nSelectAlarm];
		m_nSelectAlarm = -1;
	}

	if (m_nSortField == -1 && m_nSecondSortField == -1)
		return;

	
	CAtlArray<CAlarm *> apAlarms;
//	apAlarms.SetCount(m_apAlarms.GetCount());

	UINT i;
	if (m_nSortField != -1)
	{
		for (i = 0; i < m_apAlarms.GetCount(); i++)
		{
			AddAlarm(apAlarms, m_apAlarms[i], m_nSortField);
		}

		m_apAlarms.RemoveAll();
		if (m_nSecondSortField == -1)
		{
			m_apAlarms.Append(apAlarms);
			FireViewChange();
			return;
		}

		for (i = 0; i < apAlarms.GetCount(); i++)
		{
			AddAlarm(m_apAlarms, apAlarms[i], m_nSecondSortField);
		}
		FireViewChange();
		return;
	}
	
	if (m_nSecondSortField == -1)
	{
		for (i = 0; i < m_apAlarms.GetCount(); i++)
		{
			AddAlarm(apAlarms, m_apAlarms[i], m_nSecondSortField);
		}

		m_apAlarms.RemoveAll();
		m_apAlarms.Append(apAlarms);
	}

	if (pSelectAlarm != NULL)
	{
		for (i = 0; i < m_apAlarms.GetCount(); i++)
		{
			if (pSelectAlarm = m_apAlarms[i])
			{
				m_nSelectAlarm = (int)i;
				break;
			}
		}
	}
	FireViewChange();
}

// 使用二分法找到位置插入
inline void CAlmSum::AddAlarm(CAtlArray<CAlarm *>& apAlarms, CAlarm* pAlarm, enumAlarmField nSortField)
{
	if (nSortField == -1)
	{
		apAlarms.Add(pAlarm);
		return;
	}

	int l, r, m;

	l = 0;

	r = (int)apAlarms.GetCount() - 1;
	// 没有值不用排序，直接添加返回
	if (r < 0)
	{
		apAlarms.Add(pAlarm);
		return;
	}

	if (m_bSortAscending)
	{
		while (l <= r)
		{
			m = (l + r) / 2;
			int nRet = CAlarm::CompareAlarm(pAlarm, apAlarms[m], nSortField);
			if (nRet < 0)
			{
				r = m - 1;
			}
			else
			{
				l = m + 1;
			}
		}
	}
	else
	{
		while (l <= r)
		{
			m = (l + r) / 2;
			int nRet = CAlarm::CompareAlarm(pAlarm, apAlarms[m], nSortField);
			if (nRet >= 0)
			{
				r = m - 1;
			}
			else
			{
				l = m + 1;
			}
		}
	}
	
	apAlarms.InsertAt(l, pAlarm);
}

CAlarm* CAlmSum::GetSelectAlarm()
{
	if (m_nSelectAlarm >= 0 && m_nSelectAlarm < (int)m_apAlarms.GetCount())
		return m_apAlarms[m_nSelectAlarm];

	return NULL;
}

void CAlmSum::SelectAlarm(CAlarm* pAlarm)
{
	int nIndex = -1;
	if (pAlarm != NULL)
	{
		for (int i = 0; i < (int)m_apAlarms.GetCount(); i++)
		{
			if (m_apAlarms[i] == pAlarm)
			{
				nIndex = i;
				break;
			}
		}
	}
	SelectAlarm(nIndex);
}

void CAlmSum::SelectAlarm(int nIndex)
{
	if (nIndex >= 0 && nIndex < (int)m_apAlarms.GetCount())
		m_nSelectAlarm = nIndex;
	else
		m_nSelectAlarm = -1;

	FireViewChange();
}

// 确认报警
void CAlmSum::AckAlarm(CAlarm* pAlarm)
{
	USES_CONVERSION;

	if (m_bEnableAck == VARIANT_FALSE || pAlarm->m_bAck == TRUE)
		return;

	CString strQualityTag;
	strQualityTag.Format(_T("Tags.%s.%s"), pAlarm->m_strNode, pAlarm->m_strTag);
	m_pAlarmSinkWnd->AckAlarm(T2OLE(strQualityTag));
	OnAlarmAcked(strQualityTag);
}

void CAlmSum::AckAllAlarms()
{
	for (UINT i = 0; i < m_apAlarms.GetCount(); i++)
	{
		AckAlarm(m_apAlarms[i]);
	}
}

void CAlmSum::AckPageAlarms()
{
	UINT nFirstVisibleRow = (UINT)GetFirstVisibleRow();
	UINT nLastVisibleRow = nFirstVisibleRow + (m_grid.m_rect.bottom - m_grid.m_rect.top) / m_lRowHeight;

	for (UINT i = nFirstVisibleRow; i < m_apAlarms.GetCount(); i++)
	{
		if (i > nLastVisibleRow)
			break;

		AckAlarm(m_apAlarms[i]);
	}
}

int CAlmSum::GetFirstVisibleRow()
{
	int nPos = m_scrollbarVer.GetPos();
	return int((nPos + m_lRowHeight / 2) / m_lRowHeight);
}

int CAlmSum::GetFirstVisibleColumn()
{
	int nPos = m_scrollbarHor.GetPos();
	int nCount = m_apColumns.GetSize();
	
	int nX = 0;
	for (int i = 0; i < nCount; i++)
	{
		if (nX >= nPos)
			break;
		nX += m_apColumns[i]->m_nWidth;
	}
	
	return i;
}

void CAlmSum::UpdateScrollbarVer(BOOL bHor, BOOL bVer)
{
	if (bHor)
	{
		int nCount = m_apColumns.GetSize();
		
		int nWidth = 0;
		for (int i = 0; i < nCount; i++)
		{
			nWidth += m_apColumns[i]->m_nWidth;
		}

		m_scrollbarHor.SetRange(0, nWidth);
	}

	if (bVer)
		m_scrollbarVer.SetRange(0, (int)m_apAlarms.GetCount() * m_lRowHeight);
}

void CAlmSum::DoHScroll(UINT nSBCode, UINT nPos, CDrawScrollBar* pScrollBar)
{
	int nCurPos = m_scrollbarHor.GetPos();
	int nFirstColumn = GetFirstVisibleColumn();

	switch (nSBCode)
	{
	case SB_LINELEFT:      // Scroll left.
		if (nCurPos > 0)
		{
			int xScroll = m_apColumns[nFirstColumn - 1]->m_nWidth;
			m_scrollbarHor.SetPos(max(nCurPos - xScroll, 0));
		}
		break;
	case SB_LINERIGHT:   // Scroll right.
		if (nCurPos < m_scrollbarHor.GetMax())
		{
			if (nFirstColumn < m_apColumns.GetSize())
			{
				int xScroll = m_apColumns[nFirstColumn]->m_nWidth;
				m_scrollbarHor.SetPos(nCurPos + xScroll);
			}
		}
		break;
	case SB_PAGEUP:      // Scroll left.
		if (nCurPos > 0)
		{
			int nScroll = 0;
			for (int i = nFirstColumn - 1; i >= 0; i--)
			{
				if (nScroll + m_apColumns[i]->m_nWidth >= m_scrollbarHor.GetPage())
					break;
				nScroll += m_apColumns[i]->m_nWidth;
			}
			nScroll = max(nScroll, m_apColumns[nFirstColumn - 1]->m_nWidth);
			m_scrollbarHor.SetPos(nCurPos - nScroll);
		}	
		break;
	case SB_PAGEDOWN:   // Scroll right.
		if (nCurPos < m_scrollbarHor.GetMax())
		{
			int nScroll = 0;
			for (int i = nFirstColumn; i < m_apColumns.GetSize(); i++)
			{
				if (nScroll + m_apColumns[i]->m_nWidth >= m_scrollbarHor.GetPage())
					break;
				nScroll += m_apColumns[i]->m_nWidth;
			}
			nScroll = max(nScroll, m_apColumns[nFirstColumn]->m_nWidth);
			m_scrollbarHor.SetPos(nCurPos + nScroll);
		}
		break;
	default:
		return;
	}
}

void CAlmSum::DoVScroll(UINT nSBCode, UINT nPos, CDrawScrollBar* pScrollBar)
{
	int nCurPos = m_scrollbarVer.GetPos();
	int nFirstColumn = GetFirstVisibleRow();
	
	switch (nSBCode)
	{
	case SB_LINEUP:      // Scroll left.
		if (nCurPos > 0)
			m_scrollbarVer.SetPos(max(nCurPos - m_lRowHeight, 0));
		break;
	case SB_LINEDOWN:   // Scroll right.
		if (nCurPos < m_scrollbarVer.GetMax())
			m_scrollbarVer.SetPos(nCurPos + m_lRowHeight);
		break;
	case SB_PAGEUP:      // Scroll left.
		if (nCurPos > 0)
		{
			int nScroll = max(m_lRowHeight, m_scrollbarVer.GetPage() / m_lRowHeight * m_lRowHeight);
			m_scrollbarVer.SetPos(nCurPos - nScroll);
		}
		break;
	case SB_PAGEDOWN:   // Scroll right.
		if (nCurPos < m_scrollbarVer.GetMax())
		{
			int nScroll = max(m_lRowHeight, m_scrollbarVer.GetPage() / m_lRowHeight * m_lRowHeight);
			m_scrollbarVer.SetPos(nCurPos + nScroll);
		}
		break;
	case SB_TOP:
		if (nCurPos > 0)
			m_scrollbarVer.SetPos(0);
		break;
	case SB_BOTTOM:
		if (nCurPos < m_scrollbarVer.GetMax())
			m_scrollbarVer.SetPos(m_scrollbarVer.GetMax());
		break;
	default:
		return;
	}
}

void CAlmSum::DoDraw(HDC hdcDest)
{
	int iOldMode = ::SetBkMode(hdcDest, TRANSPARENT);
	
	HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	HFONT hOldFont = (HFONT)::SelectObject(hdcDest, hFont);

	if (m_bShowScrollBar == VARIANT_TRUE)
	{
		RECT rect = m_rect;
		rect.left = rect.right - GetSystemMetrics(SM_CXVSCROLL);
		rect.top = rect.bottom - GetSystemMetrics(SM_CYHSCROLL);

		HBRUSH hBrush = ::CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
		::FillRect(hdcDest, &rect, hBrush);
		::DeleteObject((HGDIOBJ)hBrush);
//		::DrawFrameControl(hdcDest, &rect, DFC_BUTTON/*DFC_SCROLL*/, DFCS_FLAT/*DFCS_SCROLLSIZEGRIP*/);

		m_scrollbarHor.DoDraw(hdcDest);
		m_scrollbarVer.DoDraw(hdcDest);
	}
	if (m_bShowHeader == VARIANT_TRUE)
		m_header.DoDraw(hdcDest);

	m_grid.DoDraw(hdcDest);

	::SetBkMode(hdcDest, iOldMode);
	::SelectObject(hdcDest, hOldFont);
}

HRESULT CAlmSum::FireViewChange()
{
	return S_OK;
}

BOOL CAlmSum::ClientIsRunMode()
{
	return S_OK;
}

void CAlmSum::SetCapture(BOOL bCapture, CDrawCtrl* pCtrl)
{
	if (bCapture)
		m_pCtrlWithCapture = pCtrl;
	else 
		m_pCtrlWithCapture = NULL;	
}

LRESULT CAlmSum::DoWindowMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lResult = 0;

	if (m_pCtrlWithCapture != NULL)
	{
		bHandled = m_pCtrlWithCapture->ProcessAction(uMsg, wParam, lParam, lResult);
	}
	else
	{
		POINT point;
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);

		CDrawCtrl* pCtrlHit = NULL;
		if (m_bShowScrollBar == VARIANT_TRUE)
		{
			if (m_scrollbarHor.HitTest(point))
				pCtrlHit = &m_scrollbarHor;
			if (m_scrollbarVer.HitTest(point))
				pCtrlHit = &m_scrollbarVer;
		}
		if (m_bShowHeader == VARIANT_TRUE)
		{
			if (m_header.HitTest(point))
				pCtrlHit = &m_header;
		}
		
		if (m_grid.HitTest(point))
			pCtrlHit = &m_grid;

		if (pCtrlHit != NULL)
			bHandled = pCtrlHit->ProcessAction(uMsg, wParam, lParam, lResult);
	}

	if (uMsg == WM_MOUSEWHEEL)
		DoMouseWheel(uMsg, wParam, lParam, bHandled);

//	if (uMsg == WM_LBUTTONDBLCLK && !bHandled)
//	{
//		if (m_bEnableOnlineConfigure != VARIANT_FALSE)
//		{
//			OnEditProperties();
//			bHandled = TRUE;
//		}
//	}

	if (uMsg == WM_RBUTTONUP && !bHandled)
	{
		if (m_bEnableContextMenu != VARIANT_FALSE)
		{
			OnContextMenu();
//			bHandled = TRUE; // 会使鼠标焦点不能释放
		}
	}

	return lResult;
}

LRESULT CAlmSum::DoMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
	uMsg;
	int zDelta = (int)(short)HIWORD(wParam);
#else
	int zDelta = (uMsg == WM_MOUSEWHEEL) ? (int)(short)HIWORD(wParam) : (int)wParam;
#endif //!((_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400))

	// A m_nRowsPerWheelNotch value less than 0 indicates that the mouse
	// wheel scrolls whole pages, not just lines.
	if (m_nRowsPerWheelNotch == -1)
	{
		int nPagesScrolled = zDelta / 120;
		
		if (nPagesScrolled > 0)
			for (int i = 0; i < nPagesScrolled; i++)
				DoVScroll(SB_PAGEUP, 0, NULL);
		else
			for (int i = 0; i > nPagesScrolled; i--)
				DoVScroll(SB_PAGEDOWN, 0, NULL);
	}
	else
	{
		int nRowsScrolled = m_nRowsPerWheelNotch * zDelta / 120;
		
		if (nRowsScrolled > 0)
			for (int i = 0; i < nRowsScrolled; i++)
				DoVScroll(SB_LINEUP, 0, NULL);
		else
			for (int i = 0; i > nRowsScrolled; i--)
				DoVScroll(SB_LINEDOWN, 0, NULL);
	}

	return 0;
}

LRESULT CAlmSum::DoKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lResult = 0;

	int nNextSelectAlarm = m_nSelectAlarm;
	switch (wParam)
	{ 
		case VK_UP: 
		case VK_LEFT:
			if (m_nSelectAlarm > 0)
				nNextSelectAlarm = m_nSelectAlarm - 1;
			break;
		case VK_DOWN: 
		case VK_RIGHT:
			if (m_nSelectAlarm < (int)m_apAlarms.GetCount() - 1)
				nNextSelectAlarm = m_nSelectAlarm + 1;
			break;
		case VK_PRIOR:
			DoVScroll(SB_PAGEUP, 0, NULL);
			break;
		case VK_NEXT:
			DoVScroll(SB_PAGEDOWN, 0, NULL);
			break;
		case VK_HOME: 
			DoVScroll(SB_TOP, 0, NULL);
			break;
		case VK_END: 
			DoVScroll(SB_BOTTOM, 0, NULL);
			break;
	}

	if (nNextSelectAlarm != m_nSelectAlarm)
	{
		SelectAlarm(nNextSelectAlarm);
		EnsureAlarmVisible(m_nSelectAlarm);
	}

	return lResult;
}

void CAlmSum::EnsureAlarmVisible(CAlarm* pAlarm)
{
	for (UINT i = 0; i < m_apAlarms.GetCount(); i++)
	{
		if (pAlarm == m_apAlarms[i])
		{
			EnsureAlarmVisible(i);
			return;
		}
	}
}

void CAlmSum::EnsureAlarmVisible(int nIndex)
{
	int nFirstVisibleRow = GetFirstVisibleRow();
	int nVisibleRowCount = (m_grid.m_rect.bottom - m_grid.m_rect.top) / m_lRowHeight;
	if (nVisibleRowCount > 0)
	{
		if (nIndex < nFirstVisibleRow)
		{
			int nUpRow = nFirstVisibleRow - nIndex;
			while (nUpRow-- > 0)
				DoVScroll(SB_LINEUP, 0, NULL);
		}
		else if (nIndex > nFirstVisibleRow + nVisibleRowCount - 1)
		{
			int nUpDow = nIndex - (nFirstVisibleRow + nVisibleRowCount - 1);
			while (nUpDow-- > 0)
				DoVScroll(SB_LINEDOWN, 0, NULL);
		}
	}
}

int CAlmSum::GetColumnWidth(int nCol)
{
	return m_apColumns[nCol]->m_nWidth;
}

void CAlmSum::SetColumnWidth(int nCol, int nWidth)
{
	m_apColumns[nCol]->m_nWidth = nWidth;
	UpdateScrollbarVer(TRUE, FALSE);
}

void CAlmSum::RecalIdealColumnWidth(CColumn* pColumn)
{
	HDC hdc = ::GetDC(NULL);
	
	HFONT hFont;
	if (m_spHeaderFont != NULL)
	{
		CComPtr<IFont> pFont;
		m_spHeaderFont->QueryInterface(IID_IFont, (void**)&pFont);
		pFont->get_hFont(&hFont);
	}
	else
	{
		hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	}
	HFONT hOldFont = (HFONT)::SelectObject(hdc, hFont);

	if (pColumn != NULL)
	{
		pColumn->SetIdealWidth(hdc);
	}
	else
	{
		int nCount = m_apColumns.GetSize();
		for (int i = 0; i < nCount; i++)
			m_apColumns[i]->SetIdealWidth(hdc);
	}
	
	::SelectObject(hdc, hOldFont);
	::ReleaseDC(NULL, hdc);

	UpdateScrollbarVer(TRUE, FALSE);
	RecalcRects();
}

BOOL CAlmSum::FilterAlarm(CAlarm* pAlarm)
{
	USES_CONVERSION;

	if (!StrInStrlist(m_bstrNodeFilter, T2OLE(pAlarm->m_strNode)))
		return FALSE;
	if (pAlarm->m_strArea != _T("ALL") && !StrInStrlist(m_bstrAreaFilter, T2OLE(pAlarm->m_strArea)))
		return FALSE;

	CComBSTR bstr;
	if (pAlarm->m_dwSeverity >= 667)
		bstr = L"高";
	else if (pAlarm->m_dwSeverity >= 334)
		bstr = L"中";
	else
		bstr = L"低";

	if (!StrInStrlist(m_bstrPriorityFilter, bstr))
		return FALSE;

	return TRUE;
}
	
void CAlmSum::ReFilterAlarms()
{
	if (ClientIsRunMode())
		ReConnectNetNodes();

	int nCount = m_apAlarms.GetCount();
	for (int i = nCount - 1; i >= 0; i--)
	{
		if (!FilterAlarm(m_apAlarms[i]))
		{
			delete m_apAlarms[i];
			m_apAlarms.RemoveAt(i);
		}
	}

	FireViewChange();
}

HRESULT CAlmSum::OnEditProperties()
{
	return S_OK;
}

void CAlmSum::PopupPropertyPage(GUID guidPage)
{
	IUnknown* pUnk = GetObjectUnknown();
	if (pUnk == NULL)
		return;

	OleCreatePropertyFrame(
		GetClientWindow(),       // Parent window of the property frame
		0,          // Horizontal position of the property frame
		0,          // Vertical position of the property frame
		L"报警表",// Property frame caption
		1,          // Number of objects
		&pUnk,      // Array of IUnknown pointers for objects
		1,    // Number of property pages
		&guidPage,    // Array of CLSIDs for property pages
		NULL,       // Locale identifier
		0,          // Reserved - 0
		NULL        // Reserved - 0
		);
}

void CAlmSum::OnContextMenu()
{
	HMENU hMenu = CreatePopupMenu();
	if (hMenu == NULL) 
		return;
	
	InsertMenu(hMenu, 0, MF_BYPOSITION | MF_STRING | (m_bEnableOnlineConfigure != VARIANT_FALSE ? 0 : MF_GRAYED), 5, _T("属性"));

	
	InsertMenu(hMenu, 0, MF_BYPOSITION | MF_SEPARATOR, 0, 0);
	InsertMenu(hMenu, 0, MF_BYPOSITION | MF_STRING, 4, _T("过滤"));
	InsertMenu(hMenu, 0, MF_BYPOSITION | MF_STRING, 3, _T("排序"));
	
	InsertMenu(hMenu, 0, MF_BYPOSITION | MF_SEPARATOR, 0, 0);
	InsertMenu(hMenu, 0, MF_BYPOSITION | MF_STRING | (m_bEnableAck != VARIANT_FALSE ? 0 : MF_GRAYED), 2, _T("确认页"));
	InsertMenu(hMenu, 0, MF_BYPOSITION | MF_STRING | (m_bEnableAck != VARIANT_FALSE ? 0 : MF_GRAYED), 1, _T("确认全部"));
	
	POINT pt;
	GetCursorPos(&pt);

	int nRet = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD, 
		pt.x, pt.y, 0, GetClientWindow(), 0);

	switch (nRet)
	{
	case 1:
		AckAllAlarms();
		break;
	case 2:
		AckPageAlarms();
		break;
	case 3:
		PopupPropertyPage(CLSID_CxSortAlmSumPage);
		break;
	case 4:
		PopupPropertyPage(CLSID_CxFilterAlmSumPage);
		break;
	case 5:
		OnEditProperties();
		break;
	default:
		break;
	}

	DestroyMenu(hMenu);
}

void CAlmSum::OnAlarm(DWORD dwCount, CXEVENTSTRUCT* pEvents)
{
	USES_CONVERSION;

	CAlarm* pSelectAlarm = GetSelectAlarm();

	for (DWORD dw = 0; dw < dwCount; dw++)
	{
		CXEVENTSTRUCT es = pEvents[dw];
		CAlarm* pAlarm = NULL;
	
		LPCTSTR lpszSource = W2T(es.szSource);
		LPCTSTR lpszTemp = lpszSource;
		while (*lpszTemp != '\0')
		{
			if (*lpszTemp == _T('.'))
				break;
			lpszTemp++;
		}
		CString strNode(lpszSource, lpszTemp - lpszSource);
		CString strTag(lpszTemp + 1);
		
		for (UINT i = 0; i < m_apAlarms.GetCount(); i++)
		{
			if (m_apAlarms[i]->m_strNode == strNode &&
				m_apAlarms[i]->m_strTag == strTag)
			{
				pAlarm = m_apAlarms[i];
				m_apAlarms.RemoveAt(i);
				break;
			}
		}
		
		if (pAlarm == NULL)
		{
			pAlarm = new CAlarm(this);
			pAlarm->m_strNode = strNode;
			pAlarm->m_strTag = strTag;
		}

		if ((es.wChangeMask & CX_CHANGE_ACTIVE_STATE) != 0)
		{
			FileTimeToLocalFileTime(&es.ftTime, &pAlarm->m_timeIn);
			pAlarm->m_bAckRequired = es.bAckRequired;
			if ((es.wNewState & CX_CONDITION_ACTIVE) == 0)
				pAlarm->m_strStatus = _T("正常");
		}
				
		if ((es.wChangeMask & CX_CHANGE_ACK_STATE) != 0)
		{
			pAlarm->m_bAck = ((es.wNewState & CX_CONDITION_ACKED) != 0);
			if (pAlarm->m_bAck)
				FileTimeToLocalFileTime(&es.ftActiveTime, &pAlarm->m_timeOut);
			else
				memset(&pAlarm->m_timeOut, 0, sizeof(FILETIME));
		}

		if ((es.wChangeMask & CX_CHANGE_ATTRIBUTE) != 0)
		{
			if (es.dwNumEventAttrs == 4)
			{
				pAlarm->m_vaValue = es.pEventAttributes[0];
				pAlarm->m_strArea = es.pEventAttributes[1].bstrVal;
				pAlarm->m_strExtension1 = es.pEventAttributes[2].bstrVal;
				pAlarm->m_strExtension2 = es.pEventAttributes[3].bstrVal;
			}
		}
		
		if ((es.wChangeMask & CX_CHANGE_SEVERITY) != 0)
			pAlarm->m_dwSeverity = es.dwSeverity;
		//	pAlarm->m_time = es.ftTime;
		if ((es.wChangeMask & CX_CHANGE_MESSAGE) != 0)
			pAlarm->m_strDesc = es.szMessage;
		//	pAlarm->m_strConditionName = es.szConditionName;
		if ((es.wChangeMask & CX_CHANGE_SUBCONDITION) != 0)
			pAlarm->m_strStatus = es.szSubconditionName;
				
		AddAlarm(pAlarm);
	}

	SelectAlarm(pSelectAlarm);
}