// ColorDlg.cpp: implementation of the CColorDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ColorDlg.h"
#include "DynamicExpert.h"
#include "ItemEditDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColorDlg::CColorDlg() :
	m_list(this, 1),
	m_editInplace(this, 2)
{
	m_pExpert = NULL;

	m_nSelectColumn = -1;
	m_nEditItem = -1;
	m_nEditColumn = -1;

	m_bExactmatch = FALSE;
}

CColorDlg::~CColorDlg()
{
}

LRESULT CColorDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

void CColorDlg::LoadLevelItems()
{
	m_list.DeleteAllItems();
	while (m_list.DeleteColumn(0));

	m_list.InsertColumn(0, _T(""), LVCFMT_LEFT, 21, -1);
	if (m_bExactmatch)
	{
		m_list.InsertColumn(1, _T("表达式值"), LVCFMT_LEFT, 150, -1);
		m_list.InsertColumn(2, _T("颜色"), LVCFMT_LEFT, 140, -1);
	}
	else
	{
		m_list.InsertColumn(1, _T("表达式最小值"), LVCFMT_LEFT, 100, -1);
		m_list.InsertColumn(2, _T("表达式最大值"), LVCFMT_LEFT, 100, -1);
		m_list.InsertColumn(3, _T("颜色"), LVCFMT_LEFT, 90, -1);
	}

	for (int i = 0; i < m_arrLevels.GetSize(); i++)
		m_list.InsertItem(i, _T(""));

	m_nSelectColumn = 1;
	if (m_list.GetItemCount() > 0)
		m_list.SelectItem(0);

	BOOL b = m_list.GetSelectedIndex() != -1;
	::EnableWindow(GetDlgItem(IDC_DELETE_ROW), b);
	::EnableWindow(GetDlgItem(IDC_EDIT_ROW), b);
}

void CColorDlg::RemoveAllLevels()
{
	ATLASSERT(m_spUnkDynamic != NULL);
	
	CComDispatchDriver dd(m_spUnkDynamic);
	dd.Invoke0(4);
}

int CColorDlg::GetLevelsCount()
{
	ATLASSERT(m_spUnkDynamic != NULL);
	
	CComDispatchDriver dd(m_spUnkDynamic);
	CComVariant var;
	dd.GetProperty(6, &var);
	
	return var.lVal;
}

void CColorDlg::AddLevel(VARIANT* pInput1, VARIANT* pInput2, VARIANT* pOutput)
{
	ATLASSERT(m_spUnkDynamic != NULL);

	VARIANTARG* pVars = new VARIANTARG[3];
	for (int i = 0; i < 3; i++)
		VariantInit(&pVars[i]);
	
	VariantCopy(&pVars[2], pInput1);
	VariantCopy(&pVars[1], pInput2);
	VariantCopy(&pVars[0], pOutput);
	
	DISPPARAMS dispparams = {pVars, NULL, 3, 0};
	CComQIPtr<IDispatch> spDisp = m_spUnkDynamic;
	HRESULT hr = spDisp->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT,
		DISPATCH_METHOD, &dispparams, NULL, NULL, NULL);

	for (i = 0; i < 3; i++)
		VariantClear(&pVars[i]);

	delete[] pVars;
}

void CColorDlg::GetLevel(int iIndex, VARIANT *pInput1, VARIANT *pInput2, VARIANT *pOutput)
{
	ATLASSERT(m_spUnkDynamic != NULL);
	
	VARIANTARG* pVars = new VARIANTARG[4];
	for (int i = 0; i < 4; i++)
		VariantInit(&pVars[i]);
	
	pVars[3].vt = VT_I4;
	pVars[3].lVal = iIndex;
	pVars[2].vt = VT_BYREF|VT_VARIANT;
	pVars[2].pvarVal = pInput1;
	pVars[1].vt = VT_BYREF|VT_VARIANT;
	pVars[1].pvarVal = pInput2;
	pVars[0].vt = VT_BYREF|VT_VARIANT;
	pVars[0].pvarVal = pOutput;
	
	DISPPARAMS dispparams = {pVars, NULL, 4, 0};
	CComQIPtr<IDispatch> spDisp = m_spUnkDynamic;
	HRESULT hr = spDisp->Invoke(2, IID_NULL, LOCALE_USER_DEFAULT,
		DISPATCH_METHOD, &dispparams, NULL, NULL, NULL);
	
	for (i = 0; i < 4; i++)
		VariantClear(&pVars[i]);
	
	delete[] pVars;
}

void CColorDlg::GetCurrentColor(DISPID dispid, OLE_COLOR* pColor)
{
	CComDispatchDriver dd(m_pExpert->m_ppUnk[0]);

	CComVariant var;
	dd.GetProperty(dispid, &var);
	*pColor = var.lVal;
}

void CColorDlg::OnShow()
{
	USES_CONVERSION;
	
	CComVariant var;

	m_spUnkDynamic.Release();

	m_list.DeleteAllItems();
	m_arrLevels.RemoveAll();
		
	CPropInfo prop;
	m_pExpert->GetSelectProp(prop);
	
	IUnknown* pUnkDyn = NULL;
	m_pExpert->GetDynamic(prop.m_bstrName, &pUnkDyn);
	m_spUnkDynamic = pUnkDyn;
	
	if (m_spUnkDynamic == NULL)
	{
		HRESULT hr = m_spUnkDynamic.CoCreateInstance(CLSID_LookupDynamic);
		if (SUCCEEDED(hr))
		{
			CComQIPtr<ICxDynamicObject> spDynamicObject(m_spUnkDynamic);
			if (spDynamicObject != NULL)
			{
				m_pExpert->InitDynamicPropInfo();
				spDynamicObject->InitNew(long(m_pExpert->m_pPropInfo));
			}
			
			// 初始化项目
			OLE_COLOR clrCurrent;
			GetCurrentColor(prop.m_dispid, &clrCurrent);
			for (int i = 0; i < 5; i++)
			{
				CComVariant vaInput1 = i * 20;
				CComVariant vaInput2 = (i + 1) * 20;
				CComVariant vaOuput = (long)clrCurrent;
				AddLevel(&vaInput1, &vaInput2, &vaOuput);
			}
		}
	}
	
	if (m_spUnkDynamic != NULL)
	{
		CLSID clsid;
		CDynamicExpert::GetDynamicClassID(m_spUnkDynamic, &clsid);
		if (!InlineIsEqualGUID(clsid, CLSID_LookupDynamic))
		{
			m_pExpert->SelectAdvacedProp(prop.m_dispid);
			return;
		}
		
		CComDispatchDriver dd(m_spUnkDynamic);

		var.Clear();
		dd.GetPropertyByName(L"ExactMatch", &var);
		m_bExactmatch = var.boolVal != VARIANT_FALSE;
		CheckRadioButton(IDC_EXACTMATCH, IDC_RANGECOMPARE, m_bExactmatch ? IDC_EXACTMATCH : IDC_RANGECOMPARE);
		
		var.Clear();
		dd.GetPropertyByName(L"Source", &var);
		if (var.vt != VT_BSTR)
			var.ChangeType(VT_BSTR);
		if (var.vt == VT_BSTR)
			m_pExpert->SetSource(var.bstrVal);

		int nLevelCount = GetLevelsCount();
		for (int i = 0; i < nLevelCount; i++)
		{
			CLookupLevel level;
			GetLevel(i, &level.m_vaInput1, &level.m_vaInput2, &level.m_vaOutput);
			m_arrLevels.Add(level);
		}

		LoadLevelItems();
	}
}

void CColorDlg::OnOK()
{
	USES_CONVERSION;
	CPropInfo prop;
	m_pExpert->GetSelectProp(prop);
	
	CComBSTR bstr;
	BOOL bValid = m_pExpert->GetSource((BSTR&)bstr);
	
	if (!bValid)
	{
		m_pExpert->DeleteDynamic(prop.m_bstrName);
		return;
	}
	
	ATLASSERT(m_spUnkDynamic != NULL);
	
	CComDispatchDriver dd(m_spUnkDynamic);
	
	CComVariant var(bstr);
	dd.PutPropertyByName(L"Source", &var);

	var = m_bExactmatch;
	dd.PutPropertyByName(L"ExactMatch", &var);

	RemoveAllLevels();
	for (int i = 0; i < m_arrLevels.GetSize(); i++)
	{
		CLookupLevel& level = m_arrLevels[i];
		AddLevel(&level.m_vaInput1, &level.m_vaInput2, &level.m_vaOutput);
	}
		
	m_pExpert->PutDynamic(prop.m_bstrName, m_spUnkDynamic);
}

LRESULT CColorDlg::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

			if (bSelected)
				DrawEdge(dc, &r, BDR_RAISEDINNER, BF_RECT|BF_ADJUST);
			else
				DrawEdge(dc, &r, EDGE_RAISED, BF_RECT|BF_ADJUST);
				
			dc.FillRect(&r, COLOR_3DFACE);

			HFONT hOldFont = NULL;
			if (bSelected)
				hOldFont = dc.SelectFont(m_fontBold);
			else
				hOldFont = dc.SelectFont(m_font);

			TCHAR szText[16];
			sprintf(szText, "%d", itemID + 1);
			dc.DrawText(szText, -1, &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

			dc.SelectFont(m_font);

			// 表达式最小值
			rcT.left = rcT.right;
			rcT.right += m_list.GetColumnWidth(nColumn++);
			r = rcT;

//			dc.MoveTo(r.right, r.top);
//			dc.LineTo(r.right, r.bottom);
//			dc.LineTo(r.left, r.bottom);
		
			CLookupLevel& level = m_arrLevels[itemID];
			CComVariant var = level.m_vaInput1;
			var.ChangeType(VT_BSTR);
			r.left += 4;
			dc.DrawText(OLE2T(var.bstrVal), -1, &r, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

			// 表达式最大值
			if (!m_bExactmatch)
			{
				rcT.left = rcT.right;
				rcT.right += m_list.GetColumnWidth(nColumn++);
				r = rcT;
				
				//			dc.MoveTo(r.right, r.top);
				//			dc.LineTo(r.right, r.bottom);
				//			dc.LineTo(r.left, r.bottom);
				
				
				var = level.m_vaInput2;
				var.ChangeType(VT_BSTR);
				r.left += 4;
				dc.DrawText(OLE2T(var.bstrVal), -1, &r, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
				
			}
				
			// 颜色
			rcT.left = rcT.right;
			rcT.right += m_list.GetColumnWidth(nColumn++);
			r = rcT;

	//		dc.MoveTo(r.right, r.top);
	//			dc.LineTo(r.right, r.bottom);
	//			dc.LineTo(r.left, r.bottom);
	

			COLORREF clrRect = 0;
			OleTranslateColor(level.m_vaOutput.lVal, 0, &clrRect);
			CBrush brush;
			brush.CreateSolidBrush(clrRect);
			HBRUSH hOldBrush = dc.SelectBrush(brush);
		// 	dc.FillRect(&r, brush);
			r.InflateRect(-2, -2);
			dc.Rectangle(&r);
				
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
			dc.SelectBrush(hOldBrush);
		
/*
			//画Grid Line
			HPEN oldPen = (HPEN)::SelectObject(hDC, m_hGridPen); 
			
			int nTempX = di->rcItem.left + m_nLabelOff + ListView_GetColumnWidth(m_winList, 1) - 2;
			::MoveToEx(hDC, nTempX, di->rcItem.top, 0);
			::LineTo(hDC, nTempX, di->rcItem.bottom);
			
			nTempX += ListView_GetColumnWidth(m_winList, 2);
			::MoveToEx(hDC, nTempX , di->rcItem.top, 0);
			::LineTo(hDC, nTempX, di->rcItem.bottom);
			
			nTempX += ListView_GetColumnWidth(m_winList, 3);
			::MoveToEx(hDC, nTempX , di->rcItem.top, 0);
			::LineTo(hDC, nTempX, di->rcItem.bottom);
			
			::MoveToEx(hDC, di->rcItem.left + m_nLabelOff, di->rcItem.bottom - 1, 0);
			::LineTo(hDC,di->rcItem.right, di->rcItem.bottom - 1);
			
			if (m_nSelectItem == (int)di->itemID/ *(di->itemState & ODS_SELECTED)* / && m_nFocusColumn != -1 && m_nFocusColumn != 4)
			{	
				RECT rcFocusColumn;
				ListView_GetSubItemRect(m_winList, di->itemID, m_nFocusColumn, LVIR_BOUNDS, &rcFocusColumn);
				HBRUSH hBrush=(HBRUSH)GetStockObject(BLACK_BRUSH);
				FrameRect(hDC, &rcFocusColumn, hBrush);
				InflateRect(&rcFocusColumn, -1, -1);
				FrameRect(hDC, &rcFocusColumn, hBrush);
							
				::SelectObject(hDC, oldPen);	
			}*/
		}
		
		break;
	}	
	
	return 0;
}

LRESULT CColorDlg::OnMeasureItem(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MEASUREITEMSTRUCT* lpMeasureItemStruct = (MEASUREITEMSTRUCT *)lParam;
	lpMeasureItemStruct->itemHeight = 16;

	return 1;
}

LRESULT CColorDlg::OnLButtonDownList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
				if (OnEditCell(lvh.iItem, lvh.iSubItem))
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

#include "../CxComObj/CxComObj.h"
#include "../CxComObj/CxComObj_i.c"

BOOL CColorDlg::OnEditCell(int nItem, int nColumn)
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
			CLookupLevel& level = m_arrLevels[nItem];

			CComVariant var = nColumn == 1 ? level.m_vaInput1 : level.m_vaInput2;
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
			m_list.RedrawItems(nItem, nItem);
			
			m_nEditItem = nItem;
			m_nEditColumn = nColumn;
			
			CLookupLevel& level = m_arrLevels[nItem];

			OLE_COLOR clr;
					
			CComPtr<ICxColorPicker2> p;		
			HRESULT hr = p.CoCreateInstance(CLSID_CxColorPicker2); 
			if (FAILED(hr)) 
				return FALSE;
			
			//显示颜色对话框获取颜色
			CRect rect;
			m_list.GetSubItemRect(nItem, nColumn, LVIR_BOUNDS, &rect);
			m_list.ClientToScreen(&rect);

			OLE_COLOR clrDefault = level.m_vaOutput.lVal;
			hr = p->Pick((long)m_list.m_hWnd, rect, clrDefault, clr, &clr);
			if (hr == S_OK)
				level.m_vaOutput = long(clr);
 		
			m_list.RedrawItems(nItem, nItem);
		
			m_nEditItem = -1;
			m_nEditColumn = -1;
		}
		return TRUE;
	
	default:
		return FALSE;
	}

	m_nEditItem = nItem;
	m_nEditColumn = nColumn;
	return TRUE;
}

LRESULT CColorDlg::OnGetDlgCodeList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return DLGC_WANTTAB |  DLGC_WANTARROWS | DLGC_WANTALLKEYS;
}

LRESULT CColorDlg::OnGetDlgCodeEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
// 	return m_editInplace.DefWindowProc(uMsg, wParam, lParam);
 	return DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTALLKEYS;
}

LRESULT CColorDlg::OnKeyDownList(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CColorDlg::OnKeyDownEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CColorDlg::OnKillFocusEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_nEditItem != -1)
	{
		CComBSTR bstr;
		m_editInplace.GetWindowText((BSTR&)bstr);

		CLookupLevel& level = m_arrLevels[m_nEditItem];
		switch (m_nEditColumn) 
		{
		case 1:
			level.m_vaInput1 = bstr;
			break;
		case 2:
			level.m_vaInput2 = bstr;
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

LRESULT CColorDlg::OnClickedExactmatch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (!m_bExactmatch)
	{
		m_bExactmatch = TRUE;
		LoadLevelItems();
	}
	
	return 0;
}

LRESULT CColorDlg::OnClickedRangecompare(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_bExactmatch)
	{
		m_bExactmatch = FALSE;
		LoadLevelItems();
	}
	
	return 0;
}

LRESULT CColorDlg::OnClickedInsertRow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CPropInfo prop;
	m_pExpert->GetSelectProp(prop);

	OLE_COLOR clrCurrent;
	GetCurrentColor(prop.m_dispid, &clrCurrent);

	CLookupLevel level;
	level.m_vaOutput = (long)clrCurrent;
	int nIndex = m_arrLevels.Insert(m_list.GetSelectedIndex(), level);
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

LRESULT CColorDlg::OnClickedDeleteRow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nIndex = m_list.GetSelectedIndex();
	if (nIndex != -1)
	{
		m_arrLevels.RemoveAt(nIndex);
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

LRESULT CColorDlg::OnClickedEditRow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nIndex = m_list.GetSelectedIndex();
	if (nIndex != -1)
	{
		CLookupLevel& level = m_arrLevels[nIndex];
		CItemEditDialog dlg(&level, PROPTYPE_COLOR, NULL);
		if (dlg.DoModal() == IDOK)
			m_list.RedrawItems(nIndex, nIndex);
	}
	
	return 0;
}