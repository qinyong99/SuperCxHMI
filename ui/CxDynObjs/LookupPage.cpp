// LookupPage.cpp : Implementation of CLookupPage
#include "stdafx.h"
#include "CxDynObjs.h"
#include "LookupPage.h"

#include "stdio.h"

#include "LookupDynamic.h"


/////////////////////////////////////////////////////////////////////////////
// CLookupPage

LRESULT CLookupPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 字体
	m_font = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	
	LOGFONT lf;
	m_font.GetLogFont(lf);
	lf.lfWeight = FW_BOLD;
	m_fontBold.CreateFontIndirect(&lf);
	
	// 网格笔
	m_penGrid.CreatePen(PS_SOLID, 1, RGB(196, 196, 196));
	
	// 列表控件
	m_list.SubclassWindow(GetDlgItem(IDC_LIST));
	
	RECT rc = {0, 0, 0, 0};
	m_editInplace.Create(m_list, &rc, NULL, WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_NOHIDESEL);
	m_editInplace.SetFont((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
	
	return S_OK;
}

void CLookupPage::LoadLevelItems()
{
	m_list.DeleteAllItems();
	while (m_list.DeleteColumn(0));
	
	m_list.InsertColumn(0, _T(""), LVCFMT_LEFT, 21, -1);
	if (m_bExactmatch)
	{
		m_list.InsertColumn(1, _T("表达式值"), LVCFMT_LEFT, 150, -1);
		m_list.InsertColumn(2, _T("属性值"), LVCFMT_LEFT, 150, -1);
	}
	else
	{
		m_list.InsertColumn(1, _T("表达式最小值"), LVCFMT_LEFT, 90, -1);
		m_list.InsertColumn(2, _T("表达式最大值"), LVCFMT_LEFT, 90, -1);
		m_list.InsertColumn(3, _T("属性值"), LVCFMT_LEFT, 120, -1);
	}
	
	for (int i = 0; i < m_apItems.GetSize(); i++)
		m_list.InsertItem(i, _T(""));
	
	m_nSelectColumn = 1;
	if (m_list.GetItemCount() > 0)
		m_list.SelectItem(0);
	
	BOOL b = m_list.GetSelectedIndex() != -1;
	::EnableWindow(GetDlgItem(IDC_DELETE_ROW), b);
	::EnableWindow(GetDlgItem(IDC_EDIT_ROW), b);
}

void CLookupPage::OnFinalMessage(HWND hWnd)
{
	for (int i = 0; i < m_apItems.GetSize(); i++)
	{
		if (m_apItems[i] != NULL)
			delete m_apItems[i];
	}
}

LRESULT CLookupPage::OnMeasureItem(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MEASUREITEMSTRUCT* lpMeasureItemStruct = (MEASUREITEMSTRUCT *)lParam;
	lpMeasureItemStruct->itemHeight = 16;
	
	return 1;
}

LRESULT CLookupPage::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;
	
	DRAWITEMSTRUCT* lpDrawItemStruct = (DRAWITEMSTRUCT *)lParam;
	CDCHandle dc = lpDrawItemStruct->hDC;
	const RECT& rcItem = lpDrawItemStruct->rcItem;
	BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED) != 0;
	int itemID = lpDrawItemStruct->itemID;
	
	switch (lpDrawItemStruct->itemAction)
	{
	case ODA_SELECT:
	case ODA_DRAWENTIRE:
		{		
			HPEN hOldPen = (HPEN)dc.SelectPen(m_penGrid);

			dc.FillRect(&rcItem, COLOR_WINDOW);
			
			// 画序号列
			CRect rcT = rcItem;
			int nColumn = 0;
			rcT.right = rcT.left + m_list.GetColumnWidth(nColumn++);
			CRect r = rcT;

			dc.DrawEdge(&r, bSelected ? BDR_RAISEDINNER : EDGE_RAISED, BF_RECT|BF_ADJUST);
			dc.FillRect(&r, COLOR_3DFACE);

			HFONT hOldFont = dc.SelectFont(bSelected ? m_fontBold : m_font);
		
			TCHAR szText[16];
			sprintf(szText, "%d", itemID + 1);
			dc.DrawText(szText, -1, &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

			dc.SelectFont(m_font);

			// 表达式最小值
			rcT.left = rcT.right;
			rcT.right += m_list.GetColumnWidth(nColumn++);
			r = rcT;

			CLookupItem* pItem = m_apItems[itemID];
			CComVariant var = pItem->m_vaInput1;
			var.ChangeType(VT_BSTR);
			r.left += 4;
			dc.DrawText(OLE2T(var.bstrVal), -1, &r, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

			// 表达式最大值
			if (!m_bExactmatch)
			{
				rcT.left = rcT.right;
				rcT.right += m_list.GetColumnWidth(nColumn++);
				r = rcT;
				
				var = pItem->m_vaInput2;
				var.ChangeType(VT_BSTR);
				r.left += 4;
				dc.DrawText(OLE2T(var.bstrVal), -1, &r, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
				
			}
				
			// 颜色
			rcT.left = rcT.right;
			rcT.right += m_list.GetColumnWidth(nColumn);

			pItem->OnDrawOutput(dc.m_hDC, rcT, bSelected && m_nSelectColumn == nColumn);
/*
			r = rcT;

			COLORREF clrRect = 0;
			OleTranslateColor(level.m_vaOutput.lVal, 0, &clrRect);
			CBrush brush;
			brush.CreateSolidBrush(clrRect);
			HBRUSH hOldBrush = dc.SelectBrush(brush);
		// 	dc.FillRect(&r, brush);
			r.InflateRect(-2, -2);
			dc.Rectangle(&r);*/
				
			if (bSelected)
			{
				m_list.GetSubItemRect(itemID, m_nSelectColumn, LVIR_BOUNDS, &rcT);
				dc.FrameRect(&rcT, (HBRUSH)::GetStockObject(BLACK_BRUSH));
				if (m_nSelectColumn != m_nEditColumn)
				{
					rcT.InflateRect(-1, -1, -1, -1);
					dc.FrameRect(&rcT, (HBRUSH)::GetStockObject(BLACK_BRUSH));
				}
			}
			
			dc.SelectFont(hOldFont);
			dc.SelectPen(hOldPen);
		}
		
		break;
	}	
	
	return 0;
}

LRESULT CLookupPage::OnClickedInsertRow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CLookupItem* pItem = NULL;
	switch (m_pPropInfo->nType)
	{
	case PROPTYPE_COLOR:
		pItem = new CLookupItemColor(this);
		break;
		
	case PROPTYPE_ENUM:
		pItem = new CLookupItemEnum(this);
		break;
		
	case PROPTYPE_STRING:
	case PROPTYPE_NUMBER:
		pItem = new CLookupItemEdit(this);
		break;
		
	case PROPTYPE_BOOL:
		pItem = new CLookupItemBool(this);
		break;
		
	case PROPTYPE_FAKEFONT:
		pItem = new CLookupItemFakeFont(this);
		break;
		
	case PROPTYPE_FAKEFONTNAME:
		pItem = new CLookupItemFakeFontName(this);
		break;
		
	case PROPTYPE_FAKEFILE:
		pItem = new CLookupItemFile(this);
		break;

	case PROPTYPE_FAKEBRUSHFILE:
		pItem = new CLookupItemBrushFile(this);
		break;
		
	case PROPTYPE_FAKEFOLDER:
		pItem = new CLookupItemFolder(this);
		break;
		
	case PROPTYPE_FAKEPICTURE:
		pItem = new CLookupItemFakePicture(this);
		break;
		
	case PROPTYPE_TAGGROUP:
		pItem = new CLookupItemTagGroup(this);
		break;
		
	case PROPTYPE_SECURITYAREA:
		pItem = new CLookupItemSecurityArea(this);
		break;

	case PROPTYPE_ENUMSTRING:
		pItem = new CLookupItemEnumString(this);
		break;
		
	default:
		return 0;
		
	}

	pItem->m_vaOutput = m_pPropInfo->varCurrentValue;
	int nIndex = m_apItems.Insert(m_list.GetSelectedIndex(), pItem);

	m_list.InsertItem(nIndex, _T(""));
	
	if (m_list.GetSelectedIndex() == -1)
	{
		m_nSelectColumn = 1;
		m_list.SelectItem(nIndex);
	}
	
	BOOL b = m_list.GetSelectedIndex() != -1;
	::EnableWindow(GetDlgItem(IDC_DELETE_ROW), b);
	::EnableWindow(GetDlgItem(IDC_EDIT_ROW), b);

	return 0;
}

LRESULT CLookupPage::OnClickedDeleteRow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nIndex = m_list.GetSelectedIndex();
	if (nIndex != -1)
	{
		CLookupItem* pItem = m_apItems[nIndex];
		m_apItems.RemoveAt(nIndex);
		delete pItem;

		m_list.DeleteItem(nIndex);
	}
	
	if (m_list.GetItemCount() > 0)
	{
		if (m_list.GetItemCount() <= nIndex)
			nIndex--;
		m_list.SelectItem(nIndex);
	}
	
	BOOL b = m_list.GetSelectedIndex() != -1;
	::EnableWindow(GetDlgItem(IDC_DELETE_ROW), b);
	::EnableWindow(GetDlgItem(IDC_EDIT_ROW), b);

	if (nIndex < m_list.GetItemCount())
		m_list.RedrawItems(nIndex, m_list.GetItemCount() - 1);

	return 0;
}

LRESULT CLookupPage::OnClickedEditRow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nIndex = m_list.GetSelectedIndex();
	if (nIndex != -1)
	{
		CLookupItem* pItem = m_apItems[nIndex];
		CItemEditDialog dlg(pItem, m_pPropInfo->nType, &m_pPropInfo->m_arrEnumPropItems);
		if (dlg.DoModal() == IDOK)
			m_list.RedrawItems(nIndex, nIndex);
	}
	
	return 0;
}

HRESULT CLookupPage::SetPageSite(IPropertyPageSite* pPageSite)
{
	HRESULT hr = IPropertyPageImpl<CLookupPage>::SetPageSite(pPageSite);
	if (FAILED(hr))
		return hr;

	hr = CrackPropertyType();

	return hr;		
}

HRESULT CLookupPage::Apply(void)
{
	int nCount = m_apItems.GetSize();
	if (nCount < 1)
		return S_FALSE;

	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<ILookupDynamic> spDynamic = m_ppUnk[i];
		spDynamic->put_ExactMatch(m_bExactmatch);
		spDynamic->RemoveAllLevels();
		
		for (int j = 0; j < nCount; j++)
		{
			CLookupItem* pItem = m_apItems[j];
			spDynamic->AddLevel(pItem->m_vaInput1, pItem->m_vaInput2, pItem->m_vaOutput);
		}
	}
	m_bDirty = FALSE;

	return S_OK;
}

HRESULT CLookupPage::SetObjects(ULONG nObjects, IUnknown **ppUnk)
{
	HRESULT hr = IPropertyPageImpl<CLookupPage>::SetObjects(nObjects, ppUnk);
	if (SUCCEEDED(hr))
	{
		CComQIPtr<ILookupDynamic> spDynamic = m_ppUnk[0];
		if (spDynamic != NULL)
		{
			spDynamic->get_ExactMatch(&m_bExactmatch);
			CheckRadioButton(IDC_EXACTMATCH, IDC_RANGECOMPARE, m_bExactmatch ? IDC_EXACTMATCH : IDC_RANGECOMPARE);
						
			CLookupItem* pItem = NULL;
			
			long nLevelCount = 0;
			spDynamic->get_LevelsCount(&nLevelCount);

			BOOL bHasLevels = nLevelCount > 0;
			if (nLevelCount == 0)
				nLevelCount = 5;
			
			for (int i = 0; i < nLevelCount; i++)
			{
				switch (m_pPropInfo->nType)
				{
				case PROPTYPE_COLOR:
					pItem = new CLookupItemColor(this);
					break;
	
				case PROPTYPE_ENUM:
					pItem = new CLookupItemEnum(this);
					break;
			
				case PROPTYPE_STRING:
				case PROPTYPE_NUMBER:
					pItem = new CLookupItemEdit(this);
					break;
			
				case PROPTYPE_BOOL:
					pItem = new CLookupItemBool(this);
					break;

				case PROPTYPE_FAKEFONT:
					pItem = new CLookupItemFakeFont(this);
					break;

				case PROPTYPE_FAKEFONTNAME:
					pItem = new CLookupItemFakeFontName(this);
					break;

				case PROPTYPE_FAKEFILE:
					pItem = new CLookupItemFile(this);
					break;

				case PROPTYPE_FAKEBRUSHFILE:
					pItem = new CLookupItemBrushFile(this);
					break;

				case PROPTYPE_FAKEFOLDER:
					pItem = new CLookupItemFolder(this);
					break;

				case PROPTYPE_FAKEPICTURE:
					pItem = new CLookupItemFakePicture(this);
					break;

				case PROPTYPE_TAGGROUP:
					pItem = new CLookupItemTagGroup(this);
					break;

				case PROPTYPE_SECURITYAREA:
					pItem = new CLookupItemSecurityArea(this);
					break;

				case PROPTYPE_ENUMSTRING:
					pItem = new CLookupItemEnumString(this);
					break;

				default:
					ATLASSERT(FALSE);
					return 0;
	
				}
				if (bHasLevels)
				{
					spDynamic->GetLevel(i, &pItem->m_vaInput1, &pItem->m_vaInput2, &pItem->m_vaOutput);
				}
				else
				{
					pItem->m_vaInput1 = i * 20;
					pItem->m_vaInput2 = (i + 1) * 20;
					pItem->m_vaOutput = m_pPropInfo->varCurrentValue;
				}
				m_apItems.Add(pItem);
			}

			LoadLevelItems();
		}
	}

	return hr;
}

//获取页面类型
HRESULT CLookupPage::CrackPropertyType()
{
	IDynamicFrm* pDynamicFrm = NULL;
	HRESULT hr = m_pPageSite->QueryInterface(IID_IDynamicFrm, (void **)&pDynamicFrm);
	if (FAILED(hr))
		return hr;

	CDynamicPropInfo* pPropInfo = NULL;
	pDynamicFrm->get_PropertyInfo((long *)&pPropInfo);
	
	//获取接口属性的页面类型
	m_pPropInfo = pPropInfo;

	pDynamicFrm->Release();

	return S_OK;
}

LRESULT CLookupPage::OnLButtonDownList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT ptMsg = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	LVHITTESTINFO lvh;
	lvh.pt = ptMsg;
	
	if (m_list.SubItemHitTest(&lvh) != -1)
	{
		if (m_nSelectColumn == lvh.iSubItem)
		{
			if (m_list.GetSelectedIndex() == lvh.iItem)
			{
				if (OnEditCell(lvh.iItem, lvh.iSubItem, &ptMsg, uMsg == WM_LBUTTONDBLCLK))
					return 1;
			}
		}
		else
		{
			m_nSelectColumn = lvh.iSubItem;
			if (m_list.GetSelectedIndex() == lvh.iItem)
				m_list.RedrawItems(lvh.iItem, lvh.iItem);
		}
	}
	
	LRESULT lRet = m_list.DefWindowProc(uMsg, wParam, lParam);
	
	BOOL b = m_list.GetSelectedIndex() != -1;
	::EnableWindow(GetDlgItem(IDC_DELETE_ROW), b);
	::EnableWindow(GetDlgItem(IDC_EDIT_ROW), b);
	
	return lRet;
}

//LRESULT CLookupPage::OnLButtonDblClkList(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
//{
//	
//}

BOOL CLookupPage::OnEditCell(int nItem, int nColumn, POINT* pPoint, BOOL bDblClick)
{
	USES_CONVERSION;
	
	int nEditMode = -1;
	
	if (m_bExactmatch)
	{
		if (nColumn == 1)
			nEditMode = 0;
		else if (nColumn == 2)
			nEditMode = 1;
	}
	else
	{
		if (nColumn == 1 || nColumn == 2)
			nEditMode = 0;
		else if (nColumn == 3)
			nEditMode = 1;
	}
	
	switch (nEditMode) 
	{
	case 0:
		{
			CLookupItem* pItem = m_apItems[nItem];
			
			CComVariant var = nColumn == 1 ? pItem->m_vaInput1 : pItem->m_vaInput2;
			var.ChangeType(VT_BSTR);
			m_editInplace.SetWindowText(OLE2T(var.bstrVal));
			
			CRect rect;
			m_list.GetSubItemRect(nItem, nColumn, LVIR_BOUNDS, &rect);
			
			m_editInplace.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, 
				rect.bottom - rect.top, SWP_SHOWWINDOW);
			m_editInplace.SetSelAll();
			m_editInplace.SetFocus();
		}
		break;
	case 1:
		{
			m_nEditItem = nItem;
			m_nEditColumn = nColumn;
			m_list.RedrawItems(nItem, nItem);
			
			CLookupItem* pItem = m_apItems[nItem];
			
			CRect rect;
			m_list.GetSubItemRect(nItem, nColumn, LVIR_BOUNDS, &rect);
			if (!bDblClick)
				pItem->OnClickOutput(rect, pPoint != NULL ? *pPoint : rect.BottomRight());
			else
				pItem->OnDblClickOutput(rect, *pPoint);
			if (!m_editInplace.IsWindowVisible())
			{
				m_list.RedrawItems(nItem, nItem);
				
				m_nEditItem = -1;
				m_nEditColumn = -1;
			}
		}
		return TRUE;
		
	default:
		return FALSE;
	}
	
	m_nEditItem = nItem;
	m_nEditColumn = nColumn;

	return TRUE;
}

LRESULT CLookupPage::OnKillFocusEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_editInplace.DefWindowProc(nMessage, wParam, lParam);

	if (m_nEditItem != -1)
	{
		CComBSTR bstr;
		m_editInplace.GetWindowText((BSTR&)bstr);
		
		CLookupItem* pItem = m_apItems[m_nEditItem];
		switch (m_nEditColumn) 
		{
		case 1:
			pItem->m_vaInput1 = bstr;
			break;
		case 2:
			if (!m_bExactmatch)
				pItem->m_vaInput2 = bstr;
			else
				pItem->m_vaOutput = bstr;
			break;
		case 3:
			pItem->m_vaOutput = bstr;
			break;
		default:
			ATLASSERT(FALSE);
			break;
		}
	}

	m_nEditItem = -1;
	m_nEditColumn = -1;
	
	m_editInplace.SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	
	return 0;
}

LRESULT CLookupPage::OnGetDlgCodeList(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return DLGC_WANTTAB |  DLGC_WANTARROWS | DLGC_WANTALLKEYS;
}

LRESULT CLookupPage::OnGetDlgCodeEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)	
{
	return DLGC_WANTTAB |  DLGC_WANTARROWS | DLGC_WANTALLKEYS;
}

LRESULT CLookupPage::OnKeyDownList(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	int nVirtKey = (int)wParam;

	switch (nVirtKey) 
	{
	case VK_TAB:
		PostMessage(WM_KEYDOWN, VK_TAB, 0);
		PostMessage(WM_KEYUP, VK_TAB, 0);
		break;
	case VK_LEFT:
	case VK_RIGHT:
		{
			int nItem = m_list.GetSelectedIndex();
			if (nItem != -1)
			{
				if (nVirtKey == VK_LEFT)
				{
					if (m_nSelectColumn > 1)
						m_nSelectColumn--;
				}
				else if (nVirtKey == VK_RIGHT)
				{
					if (m_nSelectColumn < m_list.GetHeader().GetItemCount() - 1)
						m_nSelectColumn++;
				}
			}
			
			m_list.RedrawItems(nItem, nItem);
		}
		break;
	case VK_RETURN:
		OnEditCell(m_list.GetSelectedIndex(), m_nSelectColumn);
		break;
	default:
		bHandled = FALSE;
		break;
	}
	
	return 0;
}

LRESULT CLookupPage::OnKeyDownEdit (UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	int nVirtKey = (int)wParam;
	if (nVirtKey == VK_TAB)
	{
		bHandled = TRUE;
		PostMessage(WM_KEYDOWN, VK_TAB, 0);
		PostMessage(WM_KEYUP, VK_TAB, 0);		
	}
	if ((nVirtKey == VK_RETURN) || (nVirtKey == VK_TAB))
	{
		m_list.SetFocus();
	}
	else if (nVirtKey == VK_ESCAPE)
	{	
		m_nEditItem = -1;
		m_list.SetFocus();
	}	
	
	return 0;
}

LRESULT CLookupPage::OnClickedExactmatch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (!m_bExactmatch)
	{
		m_bExactmatch = TRUE;
		LoadLevelItems();
	}

	return 0;
}

LRESULT CLookupPage::OnClickedRangecompare(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_bExactmatch)
	{
		m_bExactmatch = FALSE;
		LoadLevelItems();
	}

	return 0;
}