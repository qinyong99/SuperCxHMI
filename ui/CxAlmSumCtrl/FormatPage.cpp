// FormatPage.cpp : Implementation of CFormatPage
#include "stdafx.h"
#include "CxAlmSumCtrl.h"
#include "FormatPage.h"

#include <atldlgs.h>

#include "../CxComObj/CxComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CFormatPage

LRESULT CFormatPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_listStatus.SubclassWindow(GetDlgItem(IDC_STATUS));
	CHeaderCtrl headerStatus = m_listStatus.GetHeader();
	long lStyle = headerStatus.GetWindowLong(GWL_STYLE);	
	lStyle &= ~(HDS_BUTTONS | HDS_DRAGDROP | HDS_HOTTRACK);
	headerStatus.SetWindowLong(GWL_STYLE, lStyle);

	m_listStatus.InsertColumn(0, _T("状态"), LVCFMT_CENTER, 160, 0);
	m_listStatus.InsertColumn(1, _T("颜色"), LVCFMT_CENTER, 40, 1);
	m_listStatus.InsertColumn(2, _T("字体"), LVCFMT_CENTER, 120, 2);

	m_listPriority.SubclassWindow(GetDlgItem(IDC_PRIORITY));
	CHeaderCtrl headerPriority = m_listPriority.GetHeader();
	lStyle = headerPriority.GetWindowLong(GWL_STYLE);	
	lStyle &= ~(HDS_BUTTONS | HDS_DRAGDROP | HDS_HOTTRACK);
	headerPriority.SetWindowLong(GWL_STYLE, lStyle);

	m_listPriority.InsertColumn(0, _T("优先级"), LVCFMT_CENTER, 160, 0);
	m_listPriority.InsertColumn(1, _T("颜色"), LVCFMT_CENTER, 40, 1);
	
//	m_listPriority.SubclassWindow(GetDlgItem(IDC_PRIORITY));
//	m_listPriority

	if (m_nObjects > 0)
	{
		CComQIPtr<ICxAlmSum, &IID_ICxAlmSum> spAlmSum(m_ppUnk[0]);

		long lCount;
		spAlmSum->GetStatusCount(&lCount);
		for (long l = 0; l < lCount; l++)
		{
			TCHAR szBuf[8];
			sprintf(szBuf, _T("%d"), l);
			int iIndex = m_listStatus.InsertItem(l, szBuf);
			
			OLE_COLOR clr;
			spAlmSum->GetStatusColor(l, &clr);
			CComPtr<IFontDisp> spFont;
			spAlmSum->GetStatusFont(l, &spFont);
			CForeFont * pForeFont = new CForeFont(spFont, clr);
			m_listStatus.SetItemData(iIndex, DWORD(pForeFont));
		}

		spAlmSum->GetPriorityCount(&lCount);
		for (l = 0; l < lCount; l++)
		{
			TCHAR szBuf[8];
			sprintf(szBuf, _T("%d"), l);
			int iIndex = m_listPriority.InsertItem(l, szBuf);
			
			OLE_COLOR clr;
			spAlmSum->GetPriorityColor(l, &clr);
			CBackColor * pBackColor = new CBackColor(clr);
			m_listPriority.SetItemData(iIndex, DWORD(pBackColor));
		}
	}
	
	return 0;
}

LRESULT CFormatPage::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int nCount = m_listStatus.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		CForeFont* pForeFont = (CForeFont *)m_listStatus.GetItemData(i);
		delete pForeFont;
	}

	nCount = m_listPriority.GetItemCount();
	for (i = 0; i < nCount; i++)
	{
		CBackColor* pBackColor = (CBackColor *)m_listPriority.GetItemData(i);
		delete pBackColor;
	}

	return 0;
}

LRESULT CFormatPage::OnListStatusLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	LVHITTESTINFO lvh = { {LOWORD(lParam), HIWORD(lParam)}, LVHT_ONITEM, 0, 0 };
	
	int iItem = m_listStatus.SubItemHitTest(&lvh);
	if (-1 == iItem) 
		return 0;

	RECT rcFocusCell;
	if (m_cellStatusListFocus.IsValidCell())
	{
		m_listStatus.GetSubItemRect(m_cellStatusListFocus.m_nRow, m_cellStatusListFocus.m_nCol, LVIR_BOUNDS, &rcFocusCell);
		m_listStatus.InvalidateRect(&rcFocusCell, TRUE);
	}
		
	m_cellStatusListFocus.SetValue(iItem, lvh.iSubItem);

	if (m_cellStatusListFocus.IsValidCell())
	{
		m_listStatus.GetSubItemRect(m_cellStatusListFocus.m_nRow, m_cellStatusListFocus.m_nCol, LVIR_BOUNDS, &rcFocusCell);
		m_listStatus.InvalidateRect(&rcFocusCell, TRUE);
	}
				
	return 0;
}

LRESULT CFormatPage::OnListPriorityLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	LVHITTESTINFO lvh = { {LOWORD(lParam), HIWORD(lParam)}, LVHT_ONITEM, 0, 0 };
	
	int iItem = m_listPriority.SubItemHitTest(&lvh);
	if (-1 == iItem) 
		return 0;

	RECT rcFocusCell;
	if (m_cellPriorityListFocus.IsValidCell())
	{
		m_listPriority.GetSubItemRect(m_cellPriorityListFocus.m_nRow, m_cellPriorityListFocus.m_nCol, LVIR_BOUNDS, &rcFocusCell);
		m_listPriority.InvalidateRect(&rcFocusCell, TRUE);
	}
		
	m_cellPriorityListFocus.SetValue(iItem, lvh.iSubItem);

	if (m_cellPriorityListFocus.IsValidCell())
	{
		m_listPriority.GetSubItemRect(m_cellPriorityListFocus.m_nRow, m_cellPriorityListFocus.m_nCol, LVIR_BOUNDS, &rcFocusCell);
		m_listPriority.InvalidateRect(&rcFocusCell, TRUE);
	}
				
	return 0;
}

STDMETHODIMP CFormatPage::Apply(void)
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<ICxAlmSum, &IID_ICxAlmSum> spAlmSum(m_ppUnk[i]);

		int nCount = m_listStatus.GetItemCount();
		for (int iItem = 0; iItem < nCount; iItem++)
		{
			CForeFont* pForeFont = (CForeFont *)m_listStatus.GetItemData(iItem);
			spAlmSum->SetStatusColor(iItem, pForeFont->m_clr);
			spAlmSum->SetStatusFont(iItem, pForeFont->m_spFont);
		}

		nCount = m_listPriority.GetItemCount();
		for (iItem = 0; iItem < nCount; iItem++)
		{
			CBackColor* pBackColor = (CBackColor *)m_listPriority.GetItemData(iItem);
			spAlmSum->SetPriorityColor(iItem, pBackColor->m_clr);
		}

	}
	m_bDirty = FALSE;
	return S_OK;
}

LRESULT CFormatPage::OnClickListStatus(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	USES_CONVERSION;

	NMITEMACTIVATE *pnmitem = (NMITEMACTIVATE *)pnmh;
	if (!m_cellStatusListFocus.m_bSelected)
		return 0;
	if (pnmitem->iItem == -1)
		return 0;

	RECT rcFocusCell;
	m_listStatus.GetSubItemRect(pnmitem->iItem, pnmitem->iSubItem, LVIR_BOUNDS, &rcFocusCell);
	
	if (pnmitem->iSubItem == 1)
	{
	//	POINT point = {rcFocusCell.left, rcFocusCell.bottom};
	//	m_listStatus.ClientToScreen(&point);

		CComPtr<ICxColorPicker2> p;		
		HRESULT hr = p.CoCreateInstance(CLSID_CxColorPicker2); 
		if (FAILED(hr)) 
			return 0;

		CForeFont* pForeFont = (CForeFont *)m_listStatus.GetItemData(pnmitem->iItem);
		OLE_COLOR clr = pForeFont->m_clr;

		//显示颜色对话框获取颜色
		RECT rect = rcFocusCell;
		m_listStatus.ClientToScreen(&rect);
		hr = p->Pick((long)m_hWnd, rect, GetSysColor(COLOR_WINDOWTEXT), clr, &clr);
		if (FAILED(hr)) 
			return 0;

		if (hr == S_OK)
			pForeFont->m_clr = clr;
	}
	else if (pnmitem->iSubItem == 2)
	{
		CFontDialog dlg(NULL, CF_SCREENFONTS);
		if (dlg.DoModal() == IDOK)
		{
			LOGFONT& logfont = dlg.m_lf;

			FONTDESC fd;
			fd.cbSizeofstruct = sizeof(FONTDESC);
			fd.lpstrName = T2OLE(logfont.lfFaceName);
			fd.sWeight = (short)logfont.lfWeight;
			fd.sCharset = logfont.lfCharSet;
			fd.fItalic = logfont.lfItalic;
			fd.fUnderline = logfont.lfUnderline;
			fd.fStrikethrough = logfont.lfStrikeOut;

			long lfHeight = logfont.lfHeight;
			if (lfHeight < 0)
				lfHeight = -lfHeight;

			int ppi;
			HDC hdc;
			if (m_hWnd)
			{
				hdc = ::GetDC(m_hWnd);
				ppi = GetDeviceCaps(hdc, LOGPIXELSY);
				::ReleaseDC(m_hWnd, hdc);
			}
			else
			{
				hdc = ::GetDC(GetDesktopWindow());
				ppi = GetDeviceCaps(hdc, LOGPIXELSY);
				::ReleaseDC(GetDesktopWindow(), hdc);
			}
			fd.cySize.Lo = lfHeight * 720000 / ppi;
			fd.cySize.Hi = 0;

			CComPtr<IFontDisp> spFont;
			OleCreateFontIndirect(&fd, IID_IFontDisp, (void**) &spFont);

			CForeFont* pForeFont = (CForeFont *)m_listStatus.GetItemData(pnmitem->iItem);
			pForeFont->m_spFont = spFont;
		}
	}
	else
	{
		return 0;
	}

	m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
	m_listStatus.RedrawItems(pnmitem->iItem, pnmitem->iItem);

	return 0;
}

LRESULT CFormatPage::OnClickListPriority(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	USES_CONVERSION;

	NMITEMACTIVATE *pnmitem = (NMITEMACTIVATE *)pnmh;

	if (!m_cellPriorityListFocus.m_bSelected)
		return 0;
	if (pnmitem->iItem == -1)
		return 0;

	RECT rcFocusCell;
	m_listPriority.GetSubItemRect(pnmitem->iItem, pnmitem->iSubItem, LVIR_BOUNDS, &rcFocusCell);
	
	if (pnmitem->iSubItem == 1)
	{
	//	POINT point = {rcFocusCell.left, rcFocusCell.bottom};
	//	m_listPriority.ClientToScreen(&point);

		CComPtr<ICxColorPicker2> p;		
		HRESULT hr = p.CoCreateInstance(CLSID_CxColorPicker2); 
		if (FAILED(hr)) 
			return 0;
		
		CBackColor* pBackColor = (CBackColor *)m_listPriority.GetItemData(pnmitem->iItem);
		OLE_COLOR clr = pBackColor->m_clr;
		
		//显示颜色对话框获取颜色
		RECT rect = rcFocusCell;
		m_listPriority.ClientToScreen(&rect);
		hr = p->Pick((long)m_hWnd, rect/*point.x, point.y*/, GetSysColor(COLOR_WINDOW), clr, &clr);
		if (FAILED(hr)) 
			return 0;
		
		if (hr == S_OK)
			pBackColor->m_clr = clr;
	}
	else
	{
		return 0;
	}

	m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
	m_listPriority.RedrawItems(pnmitem->iItem, pnmitem->iItem);

	return 0;
}

LRESULT CFormatPage::OnDblClickListStatus(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	m_cellStatusListFocus.m_bSelected = TRUE;
		
	return OnClickListStatus(idCtrl, pnmh, bHandled);
}

LRESULT CFormatPage::OnDblClickListPriority(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	m_cellPriorityListFocus.m_bSelected = TRUE;
	
	return OnClickListPriority(idCtrl, pnmh, bHandled);
}

LRESULT CFormatPage::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	UINT nCtrl = (UINT)wParam;
	LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
	if (nCtrl == IDC_STATUS)
		DrawStatusListItem(lpdis);
	else if (nCtrl == IDC_PRIORITY)
		DrawPriorityListItem(lpdis);

	return 0;
}

void CFormatPage::DrawStatusListItem(LPDRAWITEMSTRUCT lpdis)
{
	if (m_nObjects < 1)
		return;

	CDCHandle dc = lpdis->hDC;
	
	switch (lpdis->itemAction)
	{
		case ODA_DRAWENTIRE:
		case ODA_SELECT:
			{
				USES_CONVERSION;

				dc.FillRect(&lpdis->rcItem, (HBRUSH)LongToPtr(COLOR_WINDOW + 1));
				dc.SetTextColor(::GetSysColor(COLOR_BTNTEXT));
				dc.SetBkMode(TRANSPARENT);
				HFONT hFontOld = dc.SelectFont(AtlGetStockFont(DEFAULT_GUI_FONT));

				CComQIPtr<ICxAlmSum, &IID_ICxAlmSum> spAlmSum(m_ppUnk[0]);
				int iIndex = lpdis->itemID;
				CRect rect = lpdis->rcItem;

				CComBSTR bstr;
				spAlmSum->GetStatusLabel(iIndex, &bstr);
				int nWidth = m_listStatus.GetColumnWidth(0);
				rect.right = rect.left + nWidth;
				rect.InflateRect(-4, 0);
				dc.DrawText(OLE2T(bstr), -1, &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS);

				//画网格
				HPEN hGridPen = CreatePen(PS_SOLID, 1, RGB(196, 196, 196)); 
				HPEN hOldPen = dc.SelectPen(hGridPen);
				dc.MoveTo(lpdis->rcItem.left + nWidth - 2, lpdis->rcItem.top, 0);
				dc.LineTo(lpdis->rcItem.left + nWidth - 2, lpdis->rcItem.bottom - 1);

				CForeFont* pForeFont = (CForeFont *)m_listStatus.GetItemData(iIndex);

				dc.SelectStockPen(BLACK_PEN);
				HBRUSH hColorBrush = ::CreateSolidBrush(pForeFont->m_clr);	
				HBRUSH hOldBrush = dc.SelectBrush(hColorBrush);
				m_listStatus.GetSubItemRect(iIndex, 1, LVIR_BOUNDS, &rect);
				rect.InflateRect(0, 0, -1, -1);
				dc.Rectangle(rect.left, rect.top, rect.right, rect.bottom);

				//画字体名称
				CComVariant var;
				CComDispatchDriver dd(pForeFont->m_spFont);
				dd.GetProperty(DISPID_FONT_NAME, &var);
				m_listStatus.GetSubItemRect(iIndex, 2, LVIR_BOUNDS, &rect);
				dc.FillRect(&rect, (HBRUSH)LongToPtr(COLOR_BTNFACE + 1));
				dc.DrawEdge(&rect, BDR_RAISEDINNER, BF_RECT);
				rect.InflateRect(-4, 0);
				dc.DrawText(OLE2T(var.bstrVal), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS);
								
				//画网格
				dc.SelectPen(hGridPen);
				dc.MoveTo(lpdis->rcItem.left, lpdis->rcItem.bottom - 1, 0);
				dc.LineTo(lpdis->rcItem.right - 1, lpdis->rcItem.bottom - 1);
				dc.LineTo(lpdis->rcItem.right - 1, lpdis->rcItem.top);

				//焦点格子
				if (m_cellStatusListFocus.m_nRow == iIndex)
				{
					CRect rcFocusCell;
					
					m_listStatus.GetSubItemRect(m_cellStatusListFocus.m_nRow, m_cellStatusListFocus.m_nCol, LVIR_BOUNDS, &rcFocusCell);
					HBRUSH hBrush=(HBRUSH)GetStockObject(BLACK_BRUSH);
					dc.FrameRect(&rcFocusCell, hBrush);
					rcFocusCell.InflateRect(-1, -1);
					dc.FrameRect(&rcFocusCell, hBrush);
				}
				
				dc.SelectBrush(hOldBrush);
				dc.SelectPen(hOldPen);
				::DeleteObject(hColorBrush);
				::DeleteObject(hGridPen);
				dc.SelectFont(hFontOld);
			}

	}	
}

void CFormatPage::DrawPriorityListItem(LPDRAWITEMSTRUCT lpdis)
{
	if (m_nObjects < 1)
		return;

	CDCHandle dc = lpdis->hDC;
	
	switch (lpdis->itemAction)
	{
		case ODA_DRAWENTIRE:
		case ODA_SELECT:
			{
				USES_CONVERSION;

				dc.FillRect(&lpdis->rcItem, (HBRUSH)LongToPtr(COLOR_WINDOW + 1));
				dc.SetTextColor(::GetSysColor(COLOR_BTNTEXT));
				dc.SetBkMode(TRANSPARENT);
				HFONT hFontOld = dc.SelectFont(AtlGetStockFont(DEFAULT_GUI_FONT));

				CComQIPtr<ICxAlmSum, &IID_ICxAlmSum> spAlmSum(m_ppUnk[0]);
				int iIndex = lpdis->itemID;
				CRect rect = lpdis->rcItem;

				CComBSTR bstr;
				spAlmSum->GetPriorityLabel(iIndex, &bstr);
				int nWidth = m_listPriority.GetColumnWidth(0);
				rect.right = rect.left + nWidth;
				rect.InflateRect(-4, 0);
				dc.DrawText(OLE2T(bstr), -1, &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS);

				//画网格
				HPEN hGridPen = CreatePen(PS_SOLID, 1, RGB(196, 196, 196)); 
				HPEN hOldPen = dc.SelectPen(hGridPen);
				
				dc.MoveTo(lpdis->rcItem.left + nWidth - 2, lpdis->rcItem.top, 0);
				dc.LineTo(lpdis->rcItem.left + nWidth - 2, lpdis->rcItem.bottom - 1);

				//画颜色
				CBackColor* pBackColor = (CBackColor *)m_listPriority.GetItemData(iIndex);
				dc.SelectStockPen(BLACK_PEN);
				HBRUSH hColorBrush = ::CreateSolidBrush(pBackColor->m_clr);	
				HBRUSH hOldBrush = dc.SelectBrush(hColorBrush);
				m_listPriority.GetSubItemRect(iIndex, 1, LVIR_BOUNDS, &rect);
				rect.InflateRect(0, 0, -1, -1);
				dc.Rectangle(rect.left, rect.top, rect.right, rect.bottom);

				dc.SelectPen(hGridPen);
				dc.MoveTo(lpdis->rcItem.left, lpdis->rcItem.bottom - 1, 0);
				dc.LineTo(lpdis->rcItem.right - 1, lpdis->rcItem.bottom - 1);
				dc.LineTo(lpdis->rcItem.right - 1, lpdis->rcItem.top);

				//焦点格子
				if (m_cellPriorityListFocus.m_nRow == iIndex)
				{
					CRect rcFocusCell;
					
					m_listPriority.GetSubItemRect(m_cellPriorityListFocus.m_nRow, m_cellPriorityListFocus.m_nCol, LVIR_BOUNDS, &rcFocusCell);
					HBRUSH hBrush=(HBRUSH)GetStockObject(BLACK_BRUSH);
					dc.FrameRect(&rcFocusCell, hBrush);
					rcFocusCell.InflateRect(-1, -1);
					dc.FrameRect(&rcFocusCell, hBrush);
				}
				
				dc.SelectBrush(hOldBrush);
				dc.SelectPen(hOldPen);
				::DeleteObject(hColorBrush);
				::DeleteObject(hGridPen);
				dc.SelectFont(hFontOld);
			}

	}	
}