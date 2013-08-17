// PropCtrl.cpp : Implementation of CPropCtrl

#include "stdafx.h"
#include "CxPropCtrl.h"
#include "PropCtrl.h"
#include "..\CxDynObjs\CxDynObjs.h"
#include "..\CxDynObjs\CxDynObjs_i.c"

/////////////////////////////////////////////////////////////////////////////
// CPropCtrl

CPropCtrl::CPropCtrl() :
	m_list(this, 1),
	m_editInplace(this, 2),
	m_wndHeader(this, 3)
{
	m_bWindowOnly = TRUE;

	m_bShowCat = TRUE;

	m_nSignOffset = SIGN_OFFSET;
	m_nNameWidth = 80;
	m_nValueWidth = 80;
	m_nDynamicWidth = DYNAMIC_WIDTH;

	m_propEdit = NULL;

	m_bShowHelpString = TRUE;

	m_nObjects = 0;
	m_nObjectClasses = 0;
	m_ppUnk = NULL;
}

HRESULT CPropCtrl::OnDraw(ATL_DRAWINFO& di)
{
	CDCHandle dc = di.hdcDraw;
	CRect rect = *(CRect*)di.prcBounds;

// 	dc.FillRect(&rect, COLOR_3DFACE);	

	rect.top = 27;
	rect.bottom = 53;
	rect.left++;
	rect.right--;
	rect.right--;
	dc.DrawEdge(&rect, BDR_RAISEDINNER, BF_RECT);
	
	return S_OK;
}

LRESULT CPropCtrl::OnCreate(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 字体
	NONCLIENTMETRICS ncm;
    memset(&ncm, 0, sizeof(ncm));
    ncm.cbSize = sizeof(ncm);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
    m_font = CreateFontIndirect(&ncm.lfMenuFont); 
	
// 	m_font = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	
	LOGFONT lf;
	m_font.GetLogFont(lf);
	lf.lfWeight = FW_BOLD;
	m_fontBold.CreateFontIndirect(&lf);
	
	lstrcpy(lf.lfFaceName, _T("Wingdings"));
// 	lf.lfWeight = FW_NORMAL;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfHeight = 12;
	m_fontWinnding.CreateFontIndirect(&lf);
	
	// 网格笔
	m_penGrid.CreatePen(PS_SOLID, 1, RGB(196, 196, 196));

	// 组合框
	RECT rc = {0, 0, 0, 160};
	DWORD dwStyle = WS_VISIBLE | WS_CHILD |WS_VSCROLL
		| CBS_DROPDOWNLIST | CBS_SORT | CBS_AUTOHSCROLL | CBS_HASSTRINGS | CBS_OWNERDRAWFIXED;
	m_combo.Create(m_hWnd, rc, NULL, dwStyle, 0, 101);
	m_combo.SetFont(m_font);

	// 工具栏
	dwStyle = WS_CHILD | WS_VISIBLE | CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT;
	m_wndToolBar.Create(m_hWnd, rc, NULL, dwStyle, 0, 103);
	m_wndToolBar.AddBitmap(3, IDR_TOOL);
	TBBUTTON buttons[3];
	m_wndToolBar.SetButtonStructSize();
	for (int i = 0; i < 3; i++)
	{
		buttons[i].fsState   = TBSTATE_ENABLED;
		buttons[i].dwData    = 0;
		buttons[i].iString   = 0;
		buttons[i].iBitmap   = i;
		buttons[i].fsStyle = TBSTYLE_BUTTON;
		buttons[i].idCommand = ID_ALPHABETIC + i;
	}
	
	m_wndToolBar.AddButtons(3, buttons);
	UpdateToolBar();
	

	// 列表视图
	dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS
		| LVS_REPORT | LVS_SINGLESEL | LVS_OWNERDRAWFIXED/* | LVS_NOSORTHEADER*/;
	m_list.Create(m_hWnd, rc, NULL, dwStyle, WS_EX_CLIENTEDGE, 102);
// 	m_list.SetFont(m_font);

	m_wndHeader.SubclassWindow(m_list.GetHeader());

	m_list.InsertColumn(0, _T(""), LVCFMT_LEFT, 0, -1);

	m_editInplace.Create(m_list, &rc, NULL, WS_CHILD | ES_AUTOHSCROLL | ES_NOHIDESEL);
	m_editInplace.SetFont(m_list.GetFont());

//	m_font = m_list.GetFont();
//	m_font.GetLogFont(lf);


	return 0;
}

LRESULT CPropCtrl::OnSize(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_propEdit != NULL)
		m_list.SetFocus();
	
	CRect rc(0, 0, LOWORD(lParam), HIWORD(lParam));
  	rc.InflateRect(-GetSystemMetrics(SM_CXEDGE), -GetSystemMetrics(SM_CYEDGE));

	CRect rcT = rc;
	rcT.bottom = rcT.top + 17;
	m_combo.SetWindowPos(0, &rcT, SWP_NOZORDER);
	
	rcT.left += 1;
	rcT.top = rcT.bottom + 10;
	rcT.right = rcT.left + 72;
	rcT.bottom = rcT.top + 22;
 	m_wndToolBar.SetWindowPos(0, &rcT, SWP_NOZORDER);

	CRect rcWindow;
	m_list.GetWindowRect(&rcWindow);

	rcT = rc;
	rcT.top += 55;
	rcT.bottom = rc.bottom;
	m_list.SetWindowPos(0, &rcT, SWP_NOZORDER);

	ResizeColumns(/*rcT.Width() < rcWindow.Width()*/);

	return 0;
}

LRESULT CPropCtrl::OnMeasureItem(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MEASUREITEMSTRUCT* lpMeasureItemStruct = (MEASUREITEMSTRUCT *)lParam;
	UINT nCtrl = (UINT)wParam;

	if (nCtrl == 101)
		lpMeasureItemStruct->itemHeight = 15;
	else if (nCtrl == 102)
	{
		CClientDC dc(m_list);
		TEXTMETRIC tm;
		dc.GetTextMetrics(&tm);
		
// 		lpMeasureItemStruct->itemHeight = tm.tmHeight/* + 1*/;
 		lpMeasureItemStruct->itemHeight = 16;
	}
	else
		ATLASSERT(FALSE);

	return 1;
}

LRESULT CPropCtrl::OnDrawItem(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DRAWITEMSTRUCT* lpDrawItemStruct = (DRAWITEMSTRUCT *)lParam;
	UINT nCtrl = (UINT)wParam;
	
	if (nCtrl == 101)
		DrawComboItem(lpDrawItemStruct);
	else if (nCtrl == 102)
		DrawListItem(lpDrawItemStruct);
	else
		ATLASSERT(FALSE);
	
	return 0;
}

void CPropCtrl::DrawComboItem(DRAWITEMSTRUCT* lpDrawItemStruct)
{
	CDCHandle dc = lpDrawItemStruct->hDC;
	const RECT& rcItem = lpDrawItemStruct->rcItem;
	BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED) != 0;
	int nItemID = lpDrawItemStruct->itemID;

	TCHAR szText[255]; 
	m_combo.GetLBText(nItemID, szText);
	LPTSTR p = szText;
	while (*p != ' ')
	{
		if (*p == '\0')
			return;
		p++;
	}
	*p = '\0';
	p++;
	
	dc.SetBkColor(bSelected ? GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_WINDOW));
	dc.SetTextColor(bSelected ? GetSysColor(COLOR_HIGHLIGHTTEXT) : GetSysColor(COLOR_WINDOWTEXT));
	dc.ExtTextOut(0, 0, ETO_OPAQUE, &rcItem, 0, 0, 0);
	dc.SetBkMode(TRANSPARENT);	
	
	HFONT hOldFont = dc.SelectFont(m_fontBold);
	
	RECT rcT = rcItem;
	rcT.left += 1;
	rcT.top += 1;
	dc.DrawText(szText, -1, &rcT, DT_CALCRECT | DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	dc.DrawText(szText, -1, &rcT, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	
	if (hOldFont) 
		dc.SelectFont(hOldFont);
	
	rcT.left = rcT.right + 4;
	rcT.right = rcItem.right;
	dc.DrawText(p, -1, &rcT, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	if (lpDrawItemStruct->itemState & ODS_FOCUS)
		dc.DrawFocusRect(&rcItem);
}

void CPropCtrl::DrawListItem(DRAWITEMSTRUCT* lpDrawItemStruct)
{
	CDCHandle dc = lpDrawItemStruct->hDC;
	CRect rcItem = lpDrawItemStruct->rcItem;
	BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED) != 0;
	
	switch (lpDrawItemStruct->itemAction)
	{
	case ODA_SELECT:
	case ODA_DRAWENTIRE:
		{	
//			rcItem.right = rcItem.left + m_nSignOffset + m_nNameWidth + m_nValueWidth + m_nDynamicWidth;
// 			rcItem.OffsetRect(-2, 0);
			
			HPEN hOldPen = dc.SelectPen(m_penGrid); 
			
			int nTop = rcItem.top;
			int nBottom = rcItem.bottom - 1;
			int nXPos = rcItem.left + m_nSignOffset;

			dc.MoveTo(nXPos, nTop);
			dc.LineTo(nXPos, nBottom);
			nXPos += m_nNameWidth;
			dc.MoveTo(nXPos, nTop);
			dc.LineTo(nXPos, nBottom);
			nXPos += m_nValueWidth;
			dc.MoveTo(nXPos, nTop);
			dc.LineTo(nXPos, nBottom);
			
			dc.MoveTo(rcItem.left + m_nSignOffset, nBottom);
			dc.LineTo(rcItem.right + m_nSignOffset, nBottom);
							
			dc.SelectPen(hOldPen); 

			CProperty* pprop = (CProperty*)lpDrawItemStruct->itemData;
			rcItem.bottom -= 1;
			if (pprop != NULL)
				pprop->OnPaint(dc.m_hDC, rcItem, bSelected);

		}
		break;
	}	
}

LRESULT CPropCtrl::OnEraseBkgnd(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CDCHandle dc = (HDC)wParam;

	CRect rect;
	dc.GetClipBox(&rect);     // Erase the area needed
	dc.FillRect(&rect, COLOR_3DFACE);	

	return 1;
}

LRESULT CPropCtrl::OnAlphabetick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_bShowCat)
	{
		m_bShowCat = FALSE;
		UpdateToolBar();

		MakeListView();
	}
	return 1;
}

LRESULT CPropCtrl::OnCategorized(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (!m_bShowCat)
	{
		m_bShowCat = TRUE;
		UpdateToolBar();
		
		MakeListView();
	}

	return 1;
}

LRESULT CPropCtrl::OnProperty(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	Fire_ShowPropPages();
	return 1;
}

void CPropCtrl::UpdateToolBar()
{
	m_wndToolBar.CheckButton(ID_ALPHABETIC, !m_bShowCat);
	m_wndToolBar.CheckButton(ID_CATEGORIZED, m_bShowCat);
}

void CPropCtrl::ResizeColumns()
{
	m_list.Invalidate();

	CRect rect;
	m_list.GetClientRect(&rect);

  	if (m_list.GetColumnWidth(0) > rect.Width())
 	{
 		m_list.SetColumnWidth(0, 0);
  		m_list.GetClientRect(&rect);
 	}
 
	int nWidth = rect.Width() - m_nSignOffset - DYNAMIC_WIDTH;
	int nNameWidth = min(max(CalcNameColumnWidth(), 76), nWidth / 2);

	m_nNameWidth = nNameWidth;
	m_nValueWidth = nWidth - nNameWidth;

	m_list.SetColumnWidth(0, rect.Width());
}

LRESULT CPropCtrl::OnPaintHeader(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPaintDC dc(m_wndHeader);

	CPen pen1;
	pen1.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_3DHILIGHT));
	CPen pen2;
	pen2.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_3DSHADOW));

	int nOldBackMode = dc.SetBkMode(TRANSPARENT);
	HFONT hOldFont = dc.SelectFont(m_font);
	
	CRect rect;
	m_wndHeader.GetClientRect(&rect);
 	dc.FillRect(&rect, COLOR_3DFACE);	

	rect.bottom--;
	HPEN hOldPen = dc.SelectPen(pen2);
	dc.MoveTo(rect.left, rect.bottom);
	dc.LineTo(rect.right, rect.bottom);

	CRect rcT = rect;
	rcT.bottom += 2;
	rcT.right = m_nSignOffset;
	if (m_bShowCat)
	{
		dc.SelectPen(pen2);
		dc.MoveTo(rcT.right, rcT.top);
		dc.LineTo(rcT.right, rcT.bottom);
	}
	
	rcT.left = rcT.right;
	rcT.right = rcT.left + m_nNameWidth;
	if (m_bShowCat)
	{
		dc.SelectPen(pen1);
		dc.MoveTo(rcT.left + 1, rcT.top);
		dc.LineTo(rcT.left + 1, rcT.bottom);
	}
	dc.SelectPen(pen2);
	dc.MoveTo(rcT.right, rcT.top);
	dc.LineTo(rcT.right, rcT.bottom);
	rcT.left += 6;
	dc.DrawText(_T("属性"), -1, &rcT, DT_SINGLELINE | DT_VCENTER);

	rcT.left = rcT.right;
	rcT.right = rcT.left + m_nValueWidth;
	dc.SelectPen(pen1);
	dc.MoveTo(rcT.left + 1, rcT.top);
	dc.LineTo(rcT.left + 1, rcT.bottom);
	dc.SelectPen(pen2);
	dc.MoveTo(rcT.right, rcT.top);
	dc.LineTo(rcT.right, rcT.bottom);
	rcT.left += 6;
	dc.DrawText(_T("静态"), -1, &rcT, DT_SINGLELINE | DT_VCENTER);

	rcT.left = rcT.right;
	rcT.right = rcT.left + m_nDynamicWidth;
	dc.SelectPen(pen1);
	dc.MoveTo(rcT.left + 1, rcT.top);
	dc.LineTo(rcT.left + 1, rcT.bottom);
	dc.SelectPen(pen2);
	dc.MoveTo(rcT.right, rcT.top);
	dc.LineTo(rcT.right, rcT.bottom);
	rcT.left += 6;
	dc.DrawText(_T("动态"), -1, &rcT, DT_SINGLELINE | DT_VCENTER);
	
	dc.SelectPen(hOldPen);
	dc.SelectFont(hOldFont);
	dc.SetBkMode(nOldBackMode);
	
	return 0;
}


LRESULT CPropCtrl::OnMouseMessageHeader(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1;
}

LRESULT CPropCtrl::OnSetCursorHeader(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	return 1;
}

STDMETHODIMP CPropCtrl::AddComboString(BSTR bstrItemName)
{
	USES_CONVERSION;
	m_combo.AddString(OLE2T(bstrItemName));

	return S_OK;
}

STDMETHODIMP CPropCtrl::get_NameShowHelpString(BOOL *pVal)
{
	*pVal = m_bShowHelpString;
	
	return S_OK;
}

STDMETHODIMP CPropCtrl::put_NameShowHelpString(BOOL newVal)
{
	m_bShowHelpString = newVal;
	
	return S_OK;
}

STDMETHODIMP CPropCtrl::get_ShowDynamic(BOOL *pVal)
{
	return E_NOTIMPL;
}

STDMETHODIMP CPropCtrl::put_ShowDynamic(BOOL newVal)
{
	return E_NOTIMPL;
}

STDMETHODIMP CPropCtrl::GetSelectedItemName(BSTR* pbstrItemName)
{
	USES_CONVERSION;

	int nSel = m_combo.GetCurSel();
	char szText[256];
	m_combo.GetLBText(nSel, szText);

	LPTSTR p = szText;
	while (*p != ' ')
	{
		if (*p == '\0')
			return S_FALSE;
		p++;
	}
	*p = '\0';	
	*pbstrItemName = SysAllocString(T2OLE(szText));
	
	return S_OK;
}

STDMETHODIMP CPropCtrl::Refresh()
{
	m_list.Invalidate();
	
	return S_OK;
}

#include "atlcoll.h"

STDMETHODIMP CPropCtrl::SetSelectedObject(ULONG nObjects, IUnknown **ppUnk, BSTR bstrItemName, IUnknown** ppEnumUnk)
{
	USES_CONVERSION;
	HRESULT hr;
	UINT iObj;

	if (!IsWindow()) 
		return E_FAIL; 

	m_combo.ResetContent();
 	m_list.SetRedraw(FALSE);
	m_list.DeleteAllItems();

 	ClearArray();

	if (m_ppUnk != NULL && m_nObjects > 0)
	{
		for (iObj = 0; iObj < m_nObjects; iObj++)
			m_ppUnk[iObj]->Release();

		delete [] m_ppUnk;

		m_nObjects = 0;
	}
	
	if (ppUnk == NULL)
		return E_POINTER;

	m_ppUnk = NULL;
	ATLTRY(m_ppUnk = new IUnknown*[nObjects]);

	if (m_ppUnk == NULL)
		return E_OUTOFMEMORY;

	for (iObj = 0; iObj < nObjects; iObj++)
	{
		ppUnk[iObj]->AddRef();
		m_ppUnk[iObj] = ppUnk[iObj];
	}

	m_nObjects = nObjects;
	m_nObjectClasses = 0;

	if (m_nObjects > 0)
	{
		CAtlMap<CLSID, int> mapObjectIndexs;
		
		for (iObj = 0; iObj < m_nObjects; iObj++)
		{
			if (ppEnumUnk != NULL && ppEnumUnk[iObj] != NULL)
			{
				CComQIPtr<ICategorizeProperties> pCP(m_ppUnk[iObj]);
				
 				ExploreExtendProperty((ICxEnumExtendProperty *)ppEnumUnk[iObj], pCP);
				ppEnumUnk[iObj]->Release();
			}
			
			CLSID clsid;
			CComQIPtr<ICxDrawObject> spDrawObject = m_ppUnk[iObj];
			
			if (spDrawObject != NULL)
			{
				spDrawObject->GetClassID(&clsid);
			}
			else
			{
				CComQIPtr<IGroupObject> spGroupObject = m_ppUnk[iObj];
				if (spGroupObject != NULL)
				{
					clsid = CLSID_GroupObject;
				}
				else
				{
					CComQIPtr<IFormObject> spFormObject = m_ppUnk[iObj];
					if (spFormObject != NULL)
					{
						clsid = CLSID_FormObject;
					}
					else
					{
						clsid = CLSID_NULL;
						clsid.Data1 = iObj;
					}
				}
			}
			
			if (!mapObjectIndexs.Lookup(clsid))
				mapObjectIndexs.SetAt(clsid, iObj + 1);
		}

		POSITION pos = mapObjectIndexs.GetStartPosition();
		while (pos != NULL) 
		{
			iObj = mapObjectIndexs.GetNextValue(pos) - 1;
			CComQIPtr<IDispatch> spDisp = m_ppUnk[iObj];
			if (spDisp == NULL)
				return E_FAIL;

			//浏览缺省类型接口
			UINT nInfoCount = 0;
			hr = spDisp->GetTypeInfoCount(&nInfoCount);

			CComQIPtr<IPerPropertyBrowsing> ppb(spDisp);
			CComQIPtr<ICategorizeProperties> pCP(spDisp);

			for (UINT iInfo = 0; iInfo < nInfoCount; iInfo++)
			{
				CComPtr<ITypeInfo> pti;	
				hr = spDisp->GetTypeInfo(iInfo, LOCALE_SYSTEM_DEFAULT, &pti);	
				if (FAILED(hr)) 
					return hr;
				if (!pti) 
					return E_UNEXPECTED;
 				ExploreTypeInfo(pti, ppb, pCP);
			}

			m_nObjectClasses++;
		}

		SortArrayCat();
		MakeListView();
	}

	m_combo.AddString(OLE2CT(bstrItemName));
	m_combo.SetCurSel(0);

	return S_OK;
}

void CPropCtrl::SortArrayCat()
{
	int m = 0;
	int r = 0;	
	int c = m_Array.GetSize();
	
	CProperty* p1 = 0;
	CProperty* p2 = 0;

	for (int i1 = 0; i1 < c; i1++)
	{		
		m = i1;
		for (int i2 = i1 + 1; i2 < c; i2++)
		{
			//杂项始终放在后面
			if (m_Array[m]->m_name == L"杂项")
				r = -1;
			else
				r = wcscmp(m_Array[i2]->m_name, m_Array[m]->m_name); 			
			if (r > 0) m = i2;
		}

		p1 = m_Array[i1];
		p2 = m_Array[m];
		m_Array.SetAtIndex(i1, p2);
		m_Array.SetAtIndex(m, p1);
	}
}

int CPropCtrl::Fill_ListView(HWND hList, int iIndex)
{
	int idx = 0;	
	long r = 0;
	int iRet = 0;
	
	LVITEM lvi;
	memset(&lvi, 0, sizeof LVITEM);
	
	lvi.mask = LVIF_PARAM;
	lvi.iItem = iIndex;
	
	CPropertyCat* pa;
	
	int iInsertCount = 0;
	for (int i = 0; i < m_Array.GetSize(); i++)
	{	
		iInsertCount = 0;
		pa = (CPropertyCat*)m_Array[i];
		//如果属性目录展开才填充， 或者字母顺序
		if (pa->isExpand() || !m_bShowCat)
			iInsertCount += pa->Fill_ListView(hList, iIndex, TRUE);
		if (m_bShowCat && iInsertCount > 0)
			//		if (pa->Fill_ListView(hList, iIndex) > 0 && m_bShowCat)
		{
			iInsertCount++;
		}
	}
	ListView_SetItemCount(hList, iInsertCount);
	for (i = 0; i < m_Array.GetSize(); i++)
	{	
		iInsertCount = 0;
		pa = (CPropertyCat*)m_Array[i];
		//如果属性目录展开才填充， 或者字母顺序
		if (pa->isExpand() || !m_bShowCat)
			iInsertCount = pa->Fill_ListView(hList, iIndex);
		if (m_bShowCat && iInsertCount > 0)
			//		if (pa->Fill_ListView(hList, iIndex) > 0 && m_bShowCat)
		{
			lvi.lParam = (long)m_Array[i];
			r = ListView_InsertItem(hList, &lvi);
			ATLASSERT(-1 != r);
		}
	}
	
	return iRet;
}

int CPropCtrl::CalcNameColumnWidth()
{
	CPropertyCat* pcat;
	CProperty* pprop;
	int xMax = 16;

	CClientDC dc(m_list);

	HFONT hOldFont = dc.SelectFont(m_font);	
			
	for (int i = 0; i < m_Array.GetSize(); i++)
	{
		pcat = (CPropertyCat*)m_Array[i];		
		for (int x = 0; x < pcat->m_Array.GetSize(); x++)
		{
			pprop = pcat->m_Array[x];
			SIZE sz;
			CComBSTR& bstr = m_bShowHelpString ? pprop->m_desc : pprop->m_name;
			::GetTextExtentPoint32W(dc.m_hDC, bstr, bstr.Length(), &sz);
			if (sz.cx > xMax) 
				xMax = sz.cx;
		}
	}

	if (m_bShowCat)
	{
 		dc.SelectFont(m_fontBold);
	
		for (int i = 0; i < m_Array.GetSize(); i++)
		{
			pcat = (CPropertyCat*)m_Array[i];
			SIZE sz;
			::GetTextExtentPoint32W(dc.m_hDC, pcat->m_name, -1, &sz);
			if (sz.cx > xMax) 
				xMax = sz.cx;
		}
	}

	if (hOldFont) 
		dc.SelectFont(hOldFont);

	return xMax + 4;
}

void CPropCtrl::MakeListView()
{
// 	m_list.SetRedraw(FALSE);

	m_list.DeleteAllItems();
	Fill_ListView(m_list, 0);
		
	m_nSignOffset = m_bShowCat ? SIGN_OFFSET : 0;
	m_nDynamicWidth = DYNAMIC_WIDTH;

	if (!m_bShowCat)
		m_list.SortItems(CompareFunc, m_bShowHelpString);

	m_list.SetRedraw(TRUE);

	ResizeColumns();
}

int CALLBACK CPropCtrl::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CProperty* pprop1 = (CProperty*)lParam1;
	CProperty* pprop2 = (CProperty*)lParam2;
	BOOL bShowHelpString = BOOL(lParamSort);
	
	if (!pprop1 || !pprop1->m_name) 
		return -1;
	if (!pprop2 || !pprop2->m_name) 
		return 1;
	
	//是名称属性排在最前面
	if (pprop1->m_dispid == CXPROPID_Name)
		return -1;
	if (pprop2->m_dispid == CXPROPID_Name)
		return 1;
	
	int r;
	if (bShowHelpString)
		r = wcscmp(pprop1->m_desc, pprop2->m_desc); 
	else
		r = wcscmp(pprop1->m_name, pprop2->m_name); 
	
	return r;
}

HRESULT CPropCtrl::ExploreExtendProperty(ICxEnumExtendProperty* pEnum, ICategorizeProperties* pCP)
{
	USES_CONVERSION;
	LONG nIndex;
	LONG nType;
	HRESULT hr;
	
	pEnum->Reset();
	while (S_FALSE != pEnum->Next(&nIndex))
	{
		hr = pEnum->GetType(&nType);
		if (FAILED(hr))
		{
			return hr;
		}
		CComBSTR bstrName; 
		hr = pEnum->GetName(&bstrName);
		if (FAILED(hr))
		{
			return hr;
		}
		CComBSTR bstrDesc; 
		hr = pEnum->GetDescription(&bstrDesc);
		if (FAILED(hr))
		{
			return hr;
		}
		DISPID dispid;
		hr = pEnum->GetDispid(&dispid);
		if (FAILED(hr))
		{
			return hr;
		}
		
		CProperty* pprop = NULL;
		switch (nType)
		{
		case PROPTYPE_COLOR:
			pprop = new CPropertyColor(this);
			break;
		case PROPTYPE_BOOL:
			pprop = new CPropertyBool(this);
			break;
		case PROPTYPE_STRING:
			pprop = new CPropertyEdit(this, PROPTYPE_STRING);
			break;
		case PROPTYPE_NUMBER:
			pprop = new CPropertyEdit(this, PROPTYPE_NUMBER);
			break;

		default:
			continue;
		}
		
		pprop->m_name = bstrName;
		pprop->m_desc = bstrDesc;
		pprop->m_dispid = dispid;
		pprop->m_bExtend = TRUE;
		
		if (FAILED(pprop->Init()))
		{					
			ATLTRACE(_T("[WARNING] Can't init property: \'%s\', dispid: \'%u\'\n"), 
				W2CT(pprop->GetName()), pprop->GetDISPID());
			delete pprop;									
		}	
		else
		{
			AddProperty(pprop, pCP, FALSE);
		}
	}
	
	return S_OK;
}

HRESULT CPropCtrl::ExploreTypeInfo(ITypeInfo* pTypeInfo, 
	IPerPropertyBrowsing* ppb, ICategorizeProperties* pCP)
{
	USES_CONVERSION;

	if (pTypeInfo/*pDispatch*/ == 0) 
		return E_POINTER;

	TYPEATTR* pta = 0;
	HRESULT hr = pTypeInfo->GetTypeAttr(&pta);
	if (FAILED(hr)) 
		return hr;
	if (!pta) 
		return E_UNEXPECTED;
	
	for (int i = 0; i < pta->cFuncs; i++)
	{
		FUNCDESC* pfd = 0;
		pTypeInfo->GetFuncDesc(i, &pfd);

		if (pfd && (pfd->invkind == INVOKE_PROPERTYPUT || pfd->invkind == INVOKE_PROPERTYPUTREF)
			&& ((pfd->wFuncFlags & (FUNCFLAG_FRESTRICTED | FUNCFLAG_FHIDDEN | FUNCFLAG_FNONBROWSABLE)) == 0))
		{	
			if(pfd->cParams != 1)
				continue;

			CProperty* pprop = 0;

			CComBSTR bstrName; CComBSTR bstrDocString;
			pTypeInfo->GetDocumentation(pfd->memid, &bstrName, &bstrDocString, 0, 0);
			
			if (ppb)
				CrackPerPropertyBrowsing(ppb, pfd->memid, pprop);
						
			if (!pprop)
				CrackProperty(pTypeInfo, pfd->lprgelemdescParam[0].tdesc, pprop);
			
			if (pprop)
			{
				pprop->m_name = bstrName;
				pprop->m_desc = bstrDocString;
				pprop->m_dispid = pfd->memid;
		
				if (FAILED(pprop->Init()))
				{					
					ATLTRACE(_T("[WARNING] Can't init property: \'%s\', dispid: \'%u\'\n"), 
						W2CT(pprop->GetName()), pprop->GetDISPID());
					delete pprop;									
				}	
				else
				{
					AddProperty(pprop, pCP, pfd->invkind == INVOKE_PROPERTYPUTREF);
				}

				pprop = 0;
			}
		}
		pTypeInfo->ReleaseFuncDesc(pfd);			
	} 
	
	for (i = 0; i < pta->cVars; i++)
	{
		VARDESC* pvd = NULL;
		pTypeInfo->GetVarDesc(i, &pvd);

		if(((pvd->wVarFlags & VARFLAG_FREADONLY) == 0) && 
			((pvd->wVarFlags & (VARFLAG_FHIDDEN | VARFLAG_FNONBROWSABLE)) == 0))
		{
			CProperty* pprop = NULL;

			CComBSTR bstrName; 
			CComBSTR bstrDocString;
			pTypeInfo->GetDocumentation(pvd->memid, &bstrName, &bstrDocString, 0, 0);
			
			if (ppb)
				CrackPerPropertyBrowsing(ppb, pvd->memid, pprop);
						
			if (!pprop)
				CrackProperty(pTypeInfo, pvd->elemdescVar.tdesc, pprop);
			
			if (pprop)
			{
				pprop->m_name = bstrName;
				pprop->m_desc = bstrDocString;
				pprop->m_dispid = pvd->memid;
		
				if (FAILED(pprop->Init()))
				{					
					ATLTRACE(_T("[WARNING] Can't init property: \'%s\', dispid: \'%u\'\n"), 
						W2CT(pprop->GetName()), pprop->GetDISPID());
					delete pprop;									
				}	
				else
				{
					AddProperty(pprop, pCP);
				}

				pprop = 0;
			}
		}
		pTypeInfo->ReleaseVarDesc(pvd);	
	}
	
	pTypeInfo->ReleaseTypeAttr(pta);
	return S_OK;
}

HRESULT CPropCtrl::CrackPerPropertyBrowsing(IPerPropertyBrowsing* ppb, MEMBERID memid, CProperty*& pprop)
{
//	CLSID clsid;
//	HRESULT hr = ppb->MapPropertyToPage(memid, &clsid);
//	if (SUCCEEDED(hr) && !InlineIsEqualGUID(clsid, CLSID_NULL))
//	{
//		pprop = new CPropertyPropPage(this, clsid);	
//		return S_OK;
//	}


//	HRESULT hr = E_FAIL;				
//	
//	CLSID clsid;
//	
//	hr = ppb->MapPropertyToPage(memid, &clsid);
//	if (SUCCEEDED(hr) && !InlineIsEqualGUID(clsid, CLSID_NULL))
//	{
//	/*		if (InlineIsEqualGUID(clsid, CLSID_StockPicturePage))
//	{
//	pprop = new CPropertyPicture(this);
//	if (FAILED(pprop->Init()))
//	{
//				delete pprop;
//				pprop = new CPropertyFakePicture(this);	
//				}
//				
//				  return S_OK;
//				  }
//		*/
//		//		else if (InlineIsEqualGUID(clsid, CLSID_StockColorPage))
//		//		{
//		//			pprop = new kPropertyColor(this);
//		//		}	
//		//		else if (InlineIsEqualGUID(clsid, CLSID_StockFontPage))
//		//		{
//		//			pprop = new CPropertyPropPage(this, CLSID_StockFontPage);	
//		//			if (FAILED(pprop->Init()))
//		//			{
//		//				delete pprop;
//		//				pprop = new CPropertyFakeFont(this);	
//		//			}			
//		//		}
//		//		else 
//		//		{
//		//			pprop = new kPropertyPropPage(this, clsid);									
//		//		}
//		
//	}
	
	pprop = new CPropertyPredefinedValue(this);	
	pprop->m_dispid = memid;
	if (((CPropertyPredefinedValue *)pprop)->Fill(ppb) != S_OK)
	{
		delete pprop;
		pprop = 0;
		return S_FALSE;
	}
	
	return S_OK;
}

HRESULT CPropCtrl::CrackProperty (ITypeInfo* pti, TYPEDESC& tdesc, CProperty*& pprop)
{
	switch (tdesc.vt)
	{	 
		case VT_EMPTY: 
		case VT_NULL:
		case VT_ERROR:			
			pprop = new CPropertyBlank(this);
			break;			

		case VT_PTR:	
			return CrackProperty(pti, *tdesc.lptdesc, pprop);
			break;

		case VT_USERDEFINED:	
			return CrackUserDefinedProperty(pti, tdesc.hreftype, pprop);
			break;
		
		case VT_BSTR:
			pprop = new CPropertyEdit(this, PROPTYPE_STRING);
			break;
		case VT_I2:
		case VT_I4:
		case VT_R4:
		case VT_R8:
		case VT_CY:			
		case VT_VARIANT:
		case VT_DECIMAL:
		case VT_I1:
		case VT_UI1:
		case VT_UI2:
		case VT_UI4:
		case VT_INT:
		case VT_UINT:  								
			pprop = new CPropertyEdit(this, PROPTYPE_NUMBER);
			break;

/*		case VT_DATE:				
			pprop = new kPropertyDate(this);
			break;
*/
		case VT_BOOL:  
			pprop = new CPropertyBool(this);
			break;
				
		default: 
			return E_FAIL; 
	}
	
	return S_OK;
}

//解析用户自定义属性，参数二为对应引用类型句柄
HRESULT CPropCtrl::CrackUserDefinedProperty(ITypeInfo* pti, HREFTYPE hrt, CProperty*& pprop)
{
	CComPtr<ITypeInfo> pti2;
	HRESULT hr;

	hr = pti->GetRefTypeInfo(hrt, &pti2);
	if(FAILED(hr))	return hr;

	TYPEATTR* pta = 0;
	hr = pti2->GetTypeAttr(&pta);
	if(FAILED(hr))	return hr;

	hr = E_FAIL; 		
	switch(pta->typekind)
	{
		case TKIND_ALIAS:
			if (pta->tdescAlias.vt == VT_USERDEFINED)
			{	
				hr = CrackUserDefinedProperty(pti2, pta->tdescAlias.hreftype, pprop);							
			}			
			else if(InlineIsEqualGUID(pta->guid, GUID_COLOR))
			{
				pprop = new CPropertyColor(this);	
				hr = S_OK;
			}
			else if(InlineIsEqualGUID(pta->guid, GUID_FILE))
			{
				pprop = new CPropertyFile(this);	
				hr = S_OK;
			}
			else if(InlineIsEqualGUID(pta->guid, GUID_BRUSHFILE))
			{
				pprop = new CPropertyBrushFile(this);	
				hr = S_OK;
			}
			else if(InlineIsEqualGUID(pta->guid, GUID_TAGGROUPTYPE))
			{
				pprop = new CPropertyTagGroup(this);	
				hr = S_OK;
			}
			else if(InlineIsEqualGUID(pta->guid, GUID_SECURITYAREATYPE))
			{
				pprop = new CPropertySecurityArea(this);	
				hr = S_OK;
			}
			else if(InlineIsEqualGUID(pta->guid, GUID_FONTNAME))
			{
				pprop = new CPropertyFakeFontName(this);	
				hr = S_OK;
			}
			else if(InlineIsEqualGUID(pta->guid, GUID_FOLDER))
			{
				pprop = new CPropertyFolder(this);	
				hr = S_OK;
			}
			else if(InlineIsEqualGUID(pta->guid, GUID_CXVARIABLETYPE))
			{
				CComDispatchDriver dd(m_ppUnk[0]);
				CComVariant var;
				dd.GetProperty(CXPROPID_VariableType, &var);

				int nType = var.lVal;
				switch (nType)
				{
				case PropertyTypeColor:
					pprop = new CPropertyColor(this);
					break;
				case PropertyTypeBool:
					pprop = new CPropertyBool(this);
					break;
				default:
				case PropertyTypeString:
					pprop = new CPropertyEdit(this, PROPTYPE_STRING);
					break;
				case PropertyTypeNumeric:
					pprop = new CPropertyEdit(this, PROPTYPE_NUMBER);
					break;
				}	

				hr = S_OK;
			}
			else
			{				
				hr = CrackProperty(pti, pta->tdescAlias, pprop);
			}								
			break;

	 case TKIND_ENUM:
			pprop = new CPropertyEnum(this);
			((CPropertyEnum*)pprop)->Fill(pti2);
			hr = S_OK;		
			break;

	case TKIND_INTERFACE:
	case TKIND_DISPATCH:
		
		if (InlineIsEqualGUID(pta->guid, IID_IFontDisp) ||			
			InlineIsEqualGUID(pta->guid, IID_IFont) )
		{
//			pprop = new CPropertyPropPage(this, CLSID_StockFontPage);	
			pprop = new CPropertyFakeFont(this);
			hr = S_OK;	
		}
		else if(InlineIsEqualGUID(pta->guid, IID_IPictureDisp)
			|| InlineIsEqualGUID(pta->guid, IID_IPicture) )
		{
	//		pprop = new CPropertyPicture(this);	
			pprop = new CPropertyFakePicture(this);	
			hr = S_OK;
		}	
		else if(InlineIsEqualGUID(pta->guid, IID_ICxDataConnection))
		{
			pprop = new CPropertyDataConnection(this);	
			hr = S_OK;
		}	
		else if(InlineIsEqualGUID(pta->guid, IID_ICxDataRecordset))
		{
			pprop = new CPropertyDataRecordset(this);	
			hr = S_OK;
		}
		
		break;
		
	default:	//TKIND_COCLASS, TKIND_RECORD, TKIND_UNION
		break;
	}

	pti2->ReleaseTypeAttr(pta);
	return hr;
}

void CPropCtrl::AddProperty(CProperty* pprop, ICategorizeProperties* pCP, BOOL bPutRef)
{
	ATLASSERT(pprop != 0);
	
	PROPCAT propcat = 0;
	HRESULT hr = E_FAIL;
		
	DISPID dipid = pprop->GetDISPID();
	if (dipid == CXPROPID_HorPosition ||
		dipid == CXPROPID_VerPosition ||
		dipid == CXPROPID_Width ||
		dipid == CXPROPID_Height)
	{
		propcat = PROPCAT_Position;
	}
	else if (pCP != NULL)
		hr = pCP->MapPropertyToCategory(pprop->GetDISPID(), &propcat);

	if (propcat == 0)
	{
		switch (pprop->GetDISPID())
		{
//		case CXPROPID_HorPosition:
//		case CXPROPID_VerPosition:
//		case CXPROPID_Width:
//		case CXPROPID_Height:
//			propcat = PROPCAT_Position;
//			break;
			
		case DISPID_TEXT: 
			propcat = PROPCAT_Text; 
			break;
			
		case DISPID_FONT: 
			propcat = PROPCAT_Font; 
			break;
			
		case DISPID_APPEARANCE:
		case DISPID_CAPTION: 
		case DISPID_BACKCOLOR:
		case DISPID_BACKSTYLE:
		case DISPID_BORDERCOLOR:
		case DISPID_BORDERSTYLE:
		case DISPID_BORDERVISIBLE:
		case DISPID_BORDERWIDTH:
		case DISPID_FILLCOLOR:
		case DISPID_FILLSTYLE:
		case DISPID_FORECOLOR:
		case DISPID_PICTURE:
			propcat = PROPCAT_Appearance; 
			break;
			
		case DISPID_DRAWMODE:
		case DISPID_DRAWSTYLE:
		case DISPID_DRAWWIDTH:
		case DISPID_ENABLED:
		case DISPID_AUTOSIZE:
		case DISPID_TABSTOP:
			propcat = PROPCAT_Behavior;
			break;
			
		default: 
			propcat = PROPCAT_Misc;
			break;
		}
	}
	
	int idx = -1;
	for (int i = 0; i < m_Array.GetSize(); i++)
	{
		if (((CPropertyCat*)m_Array[i])->GetPropCat() == propcat)
			idx = i;
	}
	
	CPropertyCat* pCat = 0;
	if (idx == -1)
	{		
		pCat = new CPropertyCat(this, pCP, propcat);
		m_Array.Add(pCat);
		idx = m_Array.GetSize() - 1;
	}
	else
	{
		pCat = (CPropertyCat*)m_Array[idx];
	}
	
	//	pCat->m_Array.Add(pprop);
	pCat->AddProperty(pprop, bPutRef);
}

LRESULT CPropCtrl::OnDropdownCombo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nIndex = m_combo.GetCurSel();
	CString strText;
	m_combo.GetLBText(nIndex, strText);

	m_combo.ResetContent();
	Fire_ComboDropDown();
	if (strText.IsEmpty())
		m_combo.AddString(strText);

	nIndex = m_combo.FindStringExact(-1, strText);
	m_combo.SetCurSel(nIndex);
	
	return 0;
}

LRESULT CPropCtrl::OnSelchangeCombo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	Fire_ComboSelchange();
	
	return 0;
}

void CPropCtrl::OnFinalMessage(HWND hWnd)
{
	ClearArray();
}

LRESULT CPropCtrl::OnLButtonDownList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT ptMsg = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	LVHITTESTINFO lvh;
	lvh.pt = ptMsg;
	
	if (m_list.SubItemHitTest(&lvh) != -1)
	{
		int nItem = lvh.iItem;
		CRect rect;
		m_list.GetSubItemRect(nItem, 0, LVIR_BOUNDS, &rect);
		rect.bottom--;
		CProperty* pprop = (CProperty *)m_list.GetItemData(nItem);

		BOOL bCat = pprop->VerifyClass(0x10);
		if (bCat) 
		{
			bool bExpand = ((CPropertyCat*)pprop)->isExpand();
			rect.right = rect.left + m_nSignOffset;
			if (ptMsg.x < rect.right)
				pprop->OnClick(rect, ptMsg, nItem);
		
			if ((bExpand != ((CPropertyCat*)pprop)->isExpand()))
				ResizeColumns();
		}
		else if (m_list.GetSelectedIndex() == nItem)
		{
			rect.left += m_nSignOffset + m_nNameWidth;
			if (ptMsg.x > rect.left)
			{
				rect.right = rect.left + m_nValueWidth;
				if (ptMsg.x < rect.right)
				{
					pprop->OnClick(rect, ptMsg, nItem);
					m_list.RedrawItems(nItem, nItem);
					return 1;
				}
				else
				{
					DisplayDynamicDialog(pprop, nItem);
				}
			}
		}
	}
	
	bHandled = FALSE;
	return 1;
}

LRESULT CPropCtrl::OnLButtonDblClkList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT ptMsg = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	LVHITTESTINFO lvh;
	lvh.pt = ptMsg;
	
	if (m_list.SubItemHitTest(&lvh) != -1)
	{
		int nItem = lvh.iItem;
		CRect rect;
		m_list.GetSubItemRect(nItem, 0, LVIR_BOUNDS, &rect);
		rect.bottom--;
		CProperty* pprop = (CProperty *)m_list.GetItemData(nItem);
		
		BOOL bCat = pprop->VerifyClass(0x10);
		if (bCat) 
		{
			bool bExpand = ((CPropertyCat*)pprop)->isExpand();
			pprop->OnDblClick(rect, nItem);
						
			if ((bExpand != ((CPropertyCat*)pprop)->isExpand()))
				ResizeColumns();
		}
		else
		{
			rect.left += m_nSignOffset + m_nNameWidth;
			rect.right = rect.left + m_nValueWidth;
			if (ptMsg.x < rect.right)
			{
				pprop->OnDblClick(rect, nItem);	
				m_list.RedrawItems(nItem, nItem);
			}
			else
			{
				DisplayDynamicDialog(pprop, nItem);
			}			
		}
	}
	
	bHandled = FALSE;
	return 1;
}

LRESULT CPropCtrl::OnGetDlgCodeList(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return DLGC_WANTTAB |  DLGC_WANTARROWS | DLGC_WANTALLKEYS;
}

LRESULT CPropCtrl::OnGetDlgCodeEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)	
{
	return DLGC_WANTTAB |  DLGC_WANTARROWS | DLGC_WANTALLKEYS;
}

LRESULT CPropCtrl::OnKeyDownEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
 		m_propEdit = NULL;
		m_list.SetFocus();
	}	
	
	return 0;
}

LRESULT CPropCtrl::OnKillFocusEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_propEdit != NULL)
	{
		if (m_propEdit->VerifyClass(2))
		{
			CComBSTR bstr;
			m_editInplace.GetWindowText(bstr.m_str);
			m_propEdit->SetText(bstr); 
		}
		
		m_propEdit = NULL;
	}

	m_editInplace.SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	bHandled = FALSE;
	return 0;
}

HRESULT CPropCtrl::GetDynamic(IUnknown* punkObj, BSTR bstrPropName, IUnknown** ppunkDyn)
{
	CComVariant* pVars = new CComVariant[1];

	pVars[0] = bstrPropName;
	DISPPARAMS dispparams = {pVars, NULL, 1, 0};
	CComVariant varResult;
	CComQIPtr<IDispatch> spDisp = punkObj;
	HRESULT hr = spDisp->Invoke(CXMETHID_GetConnectObject, IID_NULL, LOCALE_USER_DEFAULT,
		DISPATCH_METHOD, &dispparams, &varResult, NULL, NULL);
	delete [] pVars;

	*ppunkDyn = varResult.punkVal;
	return hr;
}

HRESULT CPropCtrl::PutDynamic(IUnknown* punkObj, BSTR bstrPropName, IUnknown* punkDyn)
{
	CComVariant* pVars = new CComVariant[2];

	pVars[1] = bstrPropName;
	pVars[0] = punkDyn;

	DISPPARAMS dispparams = {pVars, NULL, 2, 0};
	CComVariant varResult;
	CComPtr<IDispatch> pDisp;
	punkObj->QueryInterface(IID_IDispatch, (void**)&pDisp);
	HRESULT hr = pDisp->Invoke(CXMETHID_ConnectObject, IID_NULL, LOCALE_USER_DEFAULT,
		DISPATCH_METHOD, &dispparams, &varResult, NULL, NULL);

	delete [] pVars;

	return hr;
}

HRESULT CPropCtrl::DeleteDynamic(IUnknown* punkObj, BSTR bstrPropName)
{
	CComVariant* pVars = new CComVariant[1];
	
	pVars[0] = bstrPropName;
	DISPPARAMS dispparams = {pVars, NULL, 1, 0};

	CComVariant varResult;
	CComPtr<IDispatch> pDisp;
	punkObj->QueryInterface(IID_IDispatch, (void**)&pDisp);
	HRESULT hr = pDisp->Invoke(CXMETHID_Disconnect, IID_NULL, LOCALE_USER_DEFAULT,
		DISPATCH_METHOD, &dispparams, &varResult, NULL, NULL);

	delete[] pVars;

	return hr;
}

HRESULT CPropCtrl::DisplayDynamicDialog(CProperty* pProp, int nIndex)
{
	USES_CONVERSION;

	HRESULT hResult;
	
	if (pProp->GetPropType() == PROPTYPE_UNKNOWN)
		return S_FALSE;

	// 在打开动态对话框的时候如果切换了应用程序再回来，下面的值就无效而出错，所以使用临时变量存贮
	CComPtr<IUnknown> spUnkObject = m_ppUnk[0];
	CComBSTR bstrPropName = pProp->m_name;
	
	//设置属性信息
	CDynamicPropInfo propinfo;
	propinfo.spUnkObject = spUnkObject;
	propinfo.nType = pProp->GetPropType();
	propinfo.pfnExpression = NULL;
	propinfo.bstrPropertyName  = m_bShowHelpString ? pProp->m_desc : pProp->m_name;
	BOOL bSame;
	pProp->HelperGetValue(propinfo.varCurrentValue, bSame);
	propinfo.bSupportMouseInput = (pProp->GetConnectState() & 0x1) != 0;

	Fire_GetExpressionCreatorProc(long(&propinfo.pfnExpression));

	IUnknown* punkDyn = NULL;
	GetDynamic(spUnkObject, pProp->m_name, &punkDyn);
	propinfo.spUnkDynamic = punkDyn;

	if (propinfo.nType == PROPTYPE_ENUM || propinfo.nType == PROPTYPE_ENUMSTRING)
	{
		CPropertyEnum* pPropEnum = (CPropertyEnum *)pProp;
		CEnumPropItemArray& arrEnumPropItems = pPropEnum->m_arrEnumPropItems;
		for (int i = 0; i < arrEnumPropItems.GetSize(); i++)
		{
			CEnumPropItem item;
			
			item.m_bstrName = arrEnumPropItems[i].m_bstrName;
			item.m_bstrDesc = m_bShowHelpString ? 
				arrEnumPropItems[i].m_bstrDesc : arrEnumPropItems[i].m_bstrName;
			item.m_varValue = arrEnumPropItems[i].m_varValue;
			propinfo.m_arrEnumPropItems.Add(item);
		}
	}

	CComPtr<IDynamicFrm> spDynamicFrm;
	hResult = spDynamicFrm.CoCreateInstance(CLSID_DynamicFrm);
	if (FAILED(hResult))
		return hResult;

	spDynamicFrm->put_PropertyInfo(long(&propinfo));

	VARIANT_BOOL vbCancel = VARIANT_FALSE;
	Fire_PropertyChanging(-1, &vbCancel);
		
	if (spDynamicFrm->EditDynamic(long(m_hWnd), 0, 0) == S_OK)
	{
		// 实际动态对象已经改变，无法取消
		ATLASSERT(vbCancel == VARIANT_FALSE);

		Fire_PropertyChanged(-1);
		
		//设置动态属性
		if (propinfo.spUnkDynamic != NULL)
			PutDynamic(spUnkObject, bstrPropName, propinfo.spUnkDynamic);
		else
			DeleteDynamic(spUnkObject, bstrPropName);
		
		m_list.RedrawItems(nIndex, nIndex);
	}
	
	return S_OK;
}

LRESULT CPropCtrl::OnContextMenuList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT ptMsg = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

	CPoint pt = ptMsg;
	m_list.ScreenToClient(&pt);

	LVHITTESTINFO lvh;
	lvh.pt = pt;
	
	if (m_list.SubItemHitTest(&lvh) != -1)
	{
		int nItem = lvh.iItem;
		CRect rect;
		m_list.GetSubItemRect(nItem, 0, LVIR_BOUNDS, &rect);
		CProperty* pprop = (CProperty *)m_list.GetItemData(nItem);

		CMenu menu;
		menu.CreatePopupMenu();

		bool b = m_nObjects > 0;
		
		menu.AppendMenu(MF_BYPOSITION | MF_STRING | (b ? 0 : MF_GRAYED), 1, m_bShowCat ? _T("按字母顺序") : _T("按目录结构"));
		menu.AppendMenu(MF_BYPOSITION | MF_STRING, 2, _T("刷新"));
		
		BOOL bCat = pprop->VerifyClass(0x10);
		if (!bCat) 
		{
			rect.left += m_nSignOffset + m_nNameWidth;
			if (pt.x >= m_nSignOffset + m_nNameWidth + m_nValueWidth)
			{
				int nConectState = pprop->GetConnectState();
				bool b1 = nConectState > 1;
				bool b2 = nConectState != CSTATE_DISABLE;
			
				menu.AppendMenu(MF_BYPOSITION | MF_SEPARATOR);
				menu.AppendMenu(MF_BYPOSITION | MF_STRING | (b2 ? 0 : MF_GRAYED), 3, _T("连接动画"));
				menu.AppendMenu(MF_BYPOSITION | MF_STRING | (b1 && b2 ? 0 : MF_GRAYED), 4, _T("删除动画连接"));
			}
		}

		int r = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD, 
			ptMsg.x, ptMsg.y, m_list);

		switch (r)
		{
		case 1:	
			m_bShowCat = !m_bShowCat;
			UpdateToolBar();
			MakeListView();

			break;
		case 2:	
			m_list.Invalidate();
			break;
		case 3:
			DisplayDynamicDialog(pprop, nItem);
			break;
		case 4:
			{
				VARIANT_BOOL vbCancel = VARIANT_FALSE;
				Fire_PropertyChanging(-1, &vbCancel);
				if (vbCancel == VARIANT_FALSE)
				{
					DeleteDynamic(m_ppUnk[0], pprop->m_name);
					m_list.InvalidateRect(NULL);
					
					Fire_PropertyChanged(-1);
				}
			}
			break;
		default: 
			break;
		}

		return 1;
	}
					
	bHandled = FALSE;
	return 0;
}

LRESULT CPropCtrl::OnVScrollList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_propEdit != NULL)
		m_list.SetFocus();

	bHandled = FALSE;
	return 0;
}

BOOL CPropCtrl::PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (m_editInplace.IsWindowVisible())
		{
			if (GetKeyState(VK_CONTROL) < 0)
			{
				switch(pMsg->wParam)
				{
				case 'C':
					m_editInplace.Copy();
					return TRUE;
				case 'X':
					m_editInplace.Cut();
					return TRUE;
				case 'V':
					m_editInplace.Paste();
					return TRUE;
				case 'Z':
					m_editInplace.Undo();
					return TRUE;
				default:
					break;
				}
			}
		}

		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			if (m_propEdit != NULL)
			{
				m_propEdit = NULL;
				m_list.SetFocus();
			}
			hRet = S_FALSE;
			return TRUE;
		}
	}

	return FALSE;
}