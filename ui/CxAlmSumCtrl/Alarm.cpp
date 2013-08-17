// Alarm.cpp: implementation of the CAlarm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxAlmSumCtrl.h"
#include "AlmSumCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CAlarm::UpdateDrawStyle()
{
	for (int i = 0; i < NUM_STATUSS; i++)
	{
		if (m_strStatus == _gszStatusLabel[i])
		{
			CForeFont* fontFore = m_pAlmSum->m_apForeFont[i];
			CComPtr<IFont> spFont;
			fontFore->m_spFont->QueryInterface(IID_IFont, (void**)&spFont);
			spFont->get_hFont(&m_font.m_hFont);
			m_clrFore = fontFore->m_clr;
		}
	}

	for (i = 0; i < NUM_PRIORITYS; i++)
	{
		int n = 0;
		if (m_dwSeverity >= 667)
			n = 2;
		else if (m_dwSeverity >= 334)
			n = 1;
		m_clrBack = m_pAlmSum->m_apBackColor[n]->m_clr;
	}
}

void CAlarm::DrawBackground(HDC hdc, RECT rcBound, BOOL bSelected)
{
	COLORREF clr;
	if (bSelected)
		clr = GetSysColor(COLOR_HIGHLIGHT);
	else
		clr = m_clrBack;
	HBRUSH brush = ::CreateSolidBrush(clr);
	::FillRect(hdc, &rcBound, brush);
	::DeleteObject((HGDIOBJ)brush);
}

void CAlarm::DrawField(enumAlarmField nField, HDC hDC, RECT rcBound, BOOL bSelected)
{
	HFONT hOldFond = (HFONT)::SelectObject(hDC, m_font);
	COLORREF clr;
	if (bSelected)
		clr = GetSysColor(COLOR_HIGHLIGHTTEXT);
	else
		clr = m_clrFore;
	COLORREF clrOld = SetTextColor(hDC, clr);

	rcBound.left += 4;
	rcBound.right -= 4;

	switch (nField)
	{
	case AlarmField_Ack:
		{
			if (m_bAckRequired || m_bAck)
			{
				int nXCenter = (rcBound.left + rcBound.right) / 2;
				int nYCenter = (rcBound.top + rcBound.bottom) / 2;
				RECT rect;
				rect.left = nXCenter - 6;
				rect.right = nXCenter + 5;
				rect.top = nYCenter - 6;
				rect.bottom = nYCenter + 5;
				::IntersectRect(&rect, &rect, &rcBound);
				HBRUSH hBrush = CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
				::FillRect(hDC, &rect, hBrush);
				DrawEdge(hDC, &rect, BDR_RAISEDINNER, BF_RECT);
				DeleteObject(hBrush);
				
				if (m_bAck)
				{
					COLORREF clrOld = SetTextColor(hDC, RGB(255, 0, 0));
					int nOldBackMode = SetBkMode(hDC, TRANSPARENT);
					HFONT hOldFont = (HFONT)SelectObject(hDC, m_pAlmSum->m_fontWinnding);
					TCHAR szBuf[8];
					wsprintf(szBuf, "%c", 252); 
					DrawText(hDC, szBuf, 1, &rect, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
					SetBkMode(hDC, nOldBackMode);
					SetTextColor(hDC, clrOld);
					SelectObject(hDC, hOldFont);
				}
			}
		}
		break;
	case AlarmField_DateIn:
		{
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_timeIn, &st);
			
			TCHAR szBuf[16];
			wsprintf(szBuf, _T("%02d-%02d-%02d"),\
				st.wYear,	
				st.wMonth,
                st.wDay);
			DrawText(hDC, szBuf, -1, &rcBound, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_TOP);
		}
		break;
	case AlarmField_TimeIn:
		{
			SYSTEMTIME st;
			FileTimeToSystemTime(&m_timeIn, &st);

			TCHAR szBuf[16];
			wsprintf(szBuf, _T("%02d:%02d:%02d"),
                 st.wHour,
                 st.wMinute,
                 st.wSecond);
			DrawText(hDC, szBuf, -1, &rcBound, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_TOP);
		}
		break;
	case AlarmField_DateOut:
		{
			if (m_bAck)
			{
				SYSTEMTIME st;
				FileTimeToSystemTime(&m_timeOut, &st);
				
				TCHAR szBuf[16];
				wsprintf(szBuf, _T("%02d-%02d-%02d"),
					st.wYear,
					st.wMonth,
					st.wDay);
				DrawText(hDC, szBuf, -1, &rcBound, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_TOP);
			}
		}
		break;
	case AlarmField_TimeOut:
		{
			if (m_bAck)
			{
				SYSTEMTIME st;
				FileTimeToSystemTime(&m_timeOut, &st);
				
				TCHAR szBuf[16];
				wsprintf(szBuf, _T("%02d:%02d:%02d"),
					st.wHour,
					st.wMinute,
					st.wSecond);
				DrawText(hDC, szBuf, -1, &rcBound, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_TOP);
			}
		}
		break;
	case AlarmField_Node:
		DrawText(hDC, m_strNode, -1, &rcBound, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_TOP);
		break;
	case AlarmField_TagName:
		DrawText(hDC, m_strTag, -1, &rcBound, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_TOP);
		break;
	case AlarmField_Status:
		DrawText(hDC, m_strStatus, -1, &rcBound, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_TOP);
		break;
	case AlarmField_Description:
		DrawText(hDC, m_strDesc, -1, &rcBound, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_TOP);
		break;
	case AlarmField_Value:
		{
			USES_CONVERSION;

		// 	if (m_vaValue.vt != VT_EMPTY)
			{
				CComVariant var = m_vaValue;
				var.ChangeType(VT_BSTR);
				DrawText(hDC, OLE2T(var.bstrVal), -1, &rcBound, DT_SINGLELINE | DT_RIGHT | DT_VCENTER | DT_TOP);
			}
		}
		break;
	case AlarmField_Priority:
		{
			CString str;
			str.Format(_T("%d"), m_dwSeverity);
			DrawText(hDC, str, -1, &rcBound, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_TOP);
		}
		break;
	case AlarmField_Area:
		DrawText(hDC, m_strArea, -1, &rcBound, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_TOP);
		break;
	case AlarmField_Extension1:
		DrawText(hDC, m_strExtension1, -1, &rcBound, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_TOP);
		break;
	case AlarmField_Extension2:
		DrawText(hDC, m_strExtension2, -1, &rcBound, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_TOP);
		break;

//	default:
//		DrawText(hDC, "未定义列", -1, &rcBound, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_TOP);
	}
	
	::SelectObject(hDC, hOldFond);
	SetTextColor(hDC, clrOld);
}

// 当第一指大于第二个值，返回大于0
int CAlarm::CompareAlarm(CAlarm* pAlarm1, CAlarm* pAlarm2, enumAlarmField nField)
{
	switch (nField)
	{
	case AlarmField_Ack:
		return (pAlarm1->m_bAck - pAlarm2->m_bAck);
	case AlarmField_DateIn:
	case AlarmField_TimeIn:
		return CompareFileTime(&pAlarm1->m_timeIn, &pAlarm2->m_timeIn);
	case AlarmField_DateOut:
	case AlarmField_TimeOut:
		return CompareFileTime(&pAlarm1->m_timeOut, &pAlarm2->m_timeOut);
	case AlarmField_Node:
		return pAlarm1->m_strNode.Compare(pAlarm2->m_strNode);
	case AlarmField_TagName:
		return pAlarm1->m_strTag.Compare(pAlarm2->m_strTag);
	case AlarmField_Status:
		return pAlarm1->m_strStatus.Compare(pAlarm2->m_strStatus);
	case AlarmField_Description:
		return pAlarm1->m_strDesc.Compare(pAlarm2->m_strDesc);
	case AlarmField_Value:
		{
			DWORD dw = VarCmp(&pAlarm1->m_vaValue, &pAlarm2->m_vaValue, LOCALE_USER_DEFAULT, 0);
			if (dw == VARCMP_GT)
				return 1;
			else if (dw == VARCMP_LT)
				return -1;
			else
				return 0;
		}
	case AlarmField_Priority:
		return (pAlarm1->m_dwSeverity - pAlarm2->m_dwSeverity);
	case AlarmField_Area:
		return pAlarm1->m_strArea.Compare(pAlarm2->m_strArea);
	case AlarmField_Extension1:
		return pAlarm1->m_strExtension1.Compare(pAlarm2->m_strExtension1);
	case AlarmField_Extension2:
		return pAlarm1->m_strExtension2.Compare(pAlarm2->m_strExtension2);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
//CColumn

void CColumn::SetIdealWidth(HDC hdc)
{
	SIZE sz;
	::GetTextExtentPoint32(hdc, m_strCaption, m_strCaption.GetLength(), &sz);
	m_nWidth = max(_gnColumnWidth[m_enumAlarmField], sz.cx + 8);
}

void CColumn::Draw(HDC hDC, RECT rcBound, BOOL bPush)
{
	if (bPush)
	{
 		::DrawEdge(hDC, &rcBound, BDR_SUNKENOUTER, BF_RECT);
//  	::DrawFrameControl(hDC, &rcBound, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_PUSHED | DFCS_ADJUSTRECT);
		OffsetRect(&rcBound, 1, 1);
	}
	else
	{
		::DrawEdge(hDC, &rcBound, EDGE_RAISED, BF_RECT);
// 		::DrawFrameControl(hDC, &rcBound, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_ADJUSTRECT);
	}
	
//	DrawEdge(hDC, &rcBound, EDGE_RAISED, BF_RECT);
//	DrawEdge(hDC, &rcBound, BDR_RAISEDINNER, BF_RECT | BF_ADJUST);
	::InflateRect(&rcBound, -4, 0);
	UINT nFormat = DT_SINGLELINE | DT_VCENTER;
	switch (m_nFormat)
	{
	case LVCFMT_CENTER:
		nFormat |= DT_CENTER;
		break;
	case LVCFMT_RIGHT:
		nFormat |= DT_RIGHT;
		break;
	default:
		nFormat |= DT_LEFT;
	}
	
	DrawText(hDC, m_strCaption, _tcslen(m_strCaption), &rcBound, nFormat);
}
