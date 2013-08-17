// ComboBoxBold.cpp : implementation file
//

#include "stdafx.h"
#include "ComboBoxBold.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboBoxBold

CComboBoxBold::CComboBoxBold()
{
//	NONCLIENTMETRICS ncm;
//    memset(&ncm, 0, sizeof(ncm));
//    ncm.cbSize = sizeof(ncm);
//    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
//    m_font = CreateFontIndirect(&ncm.lfMenuFont); 
	
	m_font = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	
	LOGFONT lf;
	m_font.GetLogFont(lf);
	lf.lfWeight = FW_BOLD;
	m_fontBold.CreateFontIndirect(&lf);
}

CComboBoxBold::~CComboBoxBold()
{
}

/////////////////////////////////////////////////////////////////////////////
// CComboBoxBold message handlers

void CComboBoxBold::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (GetDlgCtrlID() != (int)lpDrawItemStruct->CtlID)
	{
		SetMsgHandled(FALSE);
		return;
	}
	

	CDCHandle dc = lpDrawItemStruct->hDC;

	CRect rect = lpDrawItemStruct->rcItem;
	
	// 保存字体的前景颜色和背景颜色
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();
	
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState  & ODS_SELECTED))
	{
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&rect, ::GetSysColor(COLOR_HIGHLIGHT));
	}
	else
		dc.FillSolidRect(&rect, crOldBkColor);
	
	if (lpDrawItemStruct->itemID != (UINT) -1)
	{
		BOOL bBold = (BOOL)GetItemData(lpDrawItemStruct->itemID);
		HFONT hOldFont = dc.SelectFont(bBold ? m_fontBold : m_font);
		
		CString strItem;
		GetLBText(lpDrawItemStruct->itemID, (CString&)strItem);
		ATLASSERT(strItem.GetLength() > 0);
		
		rect.left += 1;
		rect.top += 1;
		dc.DrawText(
			strItem,
			strItem.GetLength(),
			&rect,
			DT_LEFT | DT_SINGLELINE | DT_VCENTER);
		
		dc.SelectFont(hOldFont);
	}

	if (lpDrawItemStruct->itemState & ODS_FOCUS)
		dc.DrawFocusRect(&lpDrawItemStruct->rcItem);
	
	// 恢复文字前景和背景颜色
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);
}

void CComboBoxBold::SetItemBold(int iItem, BOOL bBold)
{
	SetItemData(iItem, bBold);
	Invalidate();
}

void CComboBoxBold::SetIcon(int iconId)
{
	m_hIcon = (HICON)::LoadImage(_Module.m_hInst,
		MAKEINTRESOURCE(iconId),IMAGE_ICON,16,16,0);
}

void CComboBoxBold::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if (GetDlgCtrlID() != (int)lpMeasureItemStruct->CtlID)
	{
		SetMsgHandled(FALSE);
		return;
	}

	lpMeasureItemStruct->itemHeight = 16;
}

int CComboBoxBold::SelectStringExact(int nStartAfter, LPCTSTR lpszString)
{
	int nSel = FindStringExact(nStartAfter, lpszString);
	if (nSel != LB_ERR)
		SetCurSel(nSel);

	return nSel;
}

