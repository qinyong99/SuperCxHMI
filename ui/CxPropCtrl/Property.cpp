// Property.cpp: implementation of the CProperty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxPropCtrl.h"
#include "Property.h"
#include "PropCtrl.h"
#include "PopupList.h"
#include "PropertyPageDlg.h"
#include "../CxComObj/CxComObj.h"
#include "../CxComObj/CxComObj_i.c"

const CLSID CLSID_CxDataConnectionPage = {0x6E390124,0x837E,0x447D,{0xAD,0x60,0x29,0xA8,0x4E,0x6E,0xDB,0x19}};
const CLSID CLSID_CxDataRecordsetPage = {0xD1E7A6A7,0x609E,0x4C65,{0x8B,0x81,0x8C,0x82,0x85,0x90,0x48,0x93}};


//////////////////////////////////////////////////////////////////////
// CProperty
//////////////////////////////////////////////////////////////////////

CProperty::CProperty(CPropCtrl* pPropCtrl)
{
	m_pPropCtrl = pPropCtrl;
	m_dispid = DISPID_UNKNOWN;
	m_lRef = 1;
	m_bHasPutRef = FALSE;
	m_bHasPut = FALSE;
	m_bExtend = FALSE;
}

void CProperty::DrawItemText(CDCHandle dc, LPCWSTR lpszText, unsigned int cbSize, CRect rect, 
	COLORREF clrBack, COLORREF clrText, UINT uFormat)
{
	USES_CONVERSION;
	
	dc.SetBkColor(clrBack);
	dc.SetTextColor(clrText);	
	dc.ExtTextOut(0, 0, ETO_OPAQUE, &rect, 0, 0, 0);
	dc.SetBkMode(TRANSPARENT);
	rect.left += 2;
	dc.DrawText(W2CT(lpszText), -1/*cbSize*/, &rect, uFormat);
}

HRESULT CProperty::HelperGetValue(CComVariant& var, BOOL& bSame)
{
	bSame = TRUE;

	CComDispatchDriver dd(m_pPropCtrl->m_ppUnk[0]);
	dd.GetProperty(m_dispid, &var);
		
	for (UINT i = 1; i < m_pPropCtrl->m_nObjects; i++)
	{
		CComQIPtr<IDispatch, &IID_IDispatch> spDisp(m_pPropCtrl->m_ppUnk[i]);
		if (spDisp == NULL)
		{
			ATLTRACE(_T("Failed to obtain a IDispatch pointer!\n"));
			continue;
		}

		CComDispatchDriver ddTemp(spDisp);
		CComVariant varTemp;
		HRESULT hr = ddTemp.GetProperty(m_dispid, &varTemp);
		if (FAILED(hr))
		{
			ATLASSERT(false);
//			MessageBox(m_pPropCtrl->m_hWnd, _T("读取属性值失败！"),_T("错误"), MB_OK | MB_ICONSTOP);
		}
				
		//ATL对VT_UI4类型处理出错
		if (varTemp.vt == VT_UI4 && var.vt == VT_UI4)
			bSame = (varTemp.lVal == var.lVal);
		else if (varTemp != var)
			bSame = FALSE;
	
		if (!bSame)
			break;
	}
	
	return S_OK;
}

void CProperty::DrawEllipsisButton(CDCHandle dc, CRect rect)
{
	dc.DrawFrameControl(&rect, DFC_BUTTON, DFCS_BUTTONPUSH);
	
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNTEXT));
	HPEN hOldPen = dc.SelectPen(pen);
	
	RECT rcT;
	rcT.left = (rect.left + rect.right) / 2 - 1;
	rcT.right = rcT.left + 2;
	rcT.top = (rect.top + rect.bottom) / 2 + 2;
	rcT.bottom = rcT.top + 2;
	
	dc.Rectangle(rcT.left, rcT.top, rcT.right, rcT.bottom);
	OffsetRect(&rcT, -4, 0);
	dc.Rectangle(rcT.left, rcT.top, rcT.right, rcT.bottom);
	OffsetRect(&rcT, 8, 0);
	dc.Rectangle(rcT.left, rcT.top, rcT.right, rcT.bottom);
	
	dc.SelectPen(hOldPen);
}

///////////////////////////////////////////////////////////////////////////////////
//功能：设置属性值
//描述：如果是选择了多个对象，则全部改变
///////////////////////////////////////////////////////////////////////////////////
HRESULT CProperty::HelperPutValue(CComVariant& var)
{
	VARIANT_BOOL vbCancel = VARIANT_FALSE;
	m_pPropCtrl->Fire_PropertyChanging(m_dispid, &vbCancel);
	if (vbCancel == VARIANT_TRUE)
		return S_FALSE;
	
	for (UINT i = 0; i < m_pPropCtrl->m_nObjects; i++)
	{
		CComQIPtr<IDispatch, &IID_IDispatch> spDisp(m_pPropCtrl->m_ppUnk[i]);
		if (spDisp == NULL)
		{
			ATLTRACE(_T("Failed to obtain a IDispatch pointer!\n"));
			continue;
		}
		CComDispatchDriver dd(spDisp);
		HRESULT hr = dd.PutProperty(m_dispid, &var);
		if (FAILED(hr))
			MessageBox(m_pPropCtrl->m_hWnd, _T("改变属性失败！"),_T("错误"), MB_OK | MB_ICONSTOP);
	}
	m_pPropCtrl->Fire_PropertyChanged(m_dispid);
	
	return S_OK;
}

int CProperty::GetConnectState()
{
	CComVariant* pVars = new CComVariant[1];

	pVars[0].vt = VT_I4;
	pVars[0].lVal = m_dispid;
	
	DISPPARAMS dispparams = {pVars, NULL, 1, 0};
	CComVariant varResult;
	
	CComQIPtr<IDispatch, &IID_IDispatch> spDisp(m_pPropCtrl->m_ppUnk[0]);
	HRESULT hr = spDisp->Invoke(CXMETHID_GetConnectState, IID_NULL, LOCALE_USER_DEFAULT,
		DISPATCH_METHOD, &dispparams, &varResult, NULL, NULL);
	
	delete [] pVars;
	
	if (FAILED(hr))
		return -1;
	
	return varResult.lVal;
}

//////////////////////////////////////////////////////////////////////
// CPropertyBlank
//////////////////////////////////////////////////////////////////////

HRESULT CPropertyBlank::GetText(CComBSTR& bstr, BOOL& bSame)
{
	CComVariant var;
	BOOL bSameValue;
	HelperGetValue(var, bSameValue);
	bSame = bSameValue;
	if (var.vt != VT_BSTR)
		var.ChangeType(VT_BSTR);
	
	if (var.vt != VT_BSTR)
	{
		bstr = L"**error**";		
		return S_FALSE;
	}
	
	bstr = var.bstrVal;		
	return S_OK;
}

HRESULT CPropertyBlank::SetText(CComBSTR& bstr)
{
	return S_FALSE;
}

HRESULT CPropertyBlank::OnPaint(CDCHandle dc, CRect rect, BOOL bSelected)
{
	//绘制属性名称
	CRect rcT = rect;
	rcT.left = m_pPropCtrl->m_nSignOffset;
	rcT.right = rcT.left + m_pPropCtrl->m_nNameWidth;
	rcT.left++;
	OnDrawName(dc, rcT, bSelected);

	//绘制属性值
	rcT.left = rcT.right;
	rcT.right = rcT.left + m_pPropCtrl->m_nValueWidth;
	rcT.left++;
	OnDrawValue(dc, rcT, bSelected);
	
	//绘制连接状态
	if (m_dispid != CXPROPID_Name)
	{
		rcT.left = rcT.right;
		rcT.right = rcT.left + m_pPropCtrl->m_nDynamicWidth;
		rcT.left++;
		OnDrawConnectState(dc, rcT, bSelected);
	}
	
	return S_OK;
}

void CPropertyBlank::OnDrawName(CDCHandle dc, CRect rect, BOOL bSelected)
{
	CComBSTR bstr;
	if (m_dispid == CXPROPID_Name)
	{
		bstr += "(";
		bstr += m_pPropCtrl->m_bShowHelpString ? m_desc : m_name;
		bstr += ")";
	}
	else
	{
		bstr += m_pPropCtrl->m_bShowHelpString ? m_desc : m_name;
	}
	
	COLORREF clrBack = bSelected ? GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_WINDOW);
	COLORREF clrText = bSelected ? GetSysColor(COLOR_HIGHLIGHTTEXT) : GetSysColor(COLOR_WINDOWTEXT);
	
	DrawItemText(dc.m_hDC, bstr, bstr.Length(), rect, clrBack, clrText);
}

void CPropertyBlank::OnDrawValue(CDCHandle dc, CRect rect, BOOL bSelected)
{
	CComBSTR bstr;
	BOOL bSameValue;
	GetText(bstr, bSameValue);

	if (bSameValue)
		DrawItemText(dc.m_hDC, bstr, bstr.Length(), rect);
	else
		dc.FillRect(&rect, COLOR_WINDOW);
}

void CPropertyBlank::OnDrawConnectState(CDCHandle dc, CRect rect, BOOL bSelected)
{
	rect.left = (rect.left + rect.right - 16) / 2;
	rect.right = rect.left + 16;
	
	int nConectState = GetConnectState();

	rect.InflateRect(-1, -1);
	dc.FillRect(&rect, COLOR_3DFACE);
	rect.InflateRect(-1, -1);
	if (bSelected)
		dc.DrawFocusRect(&rect);
	rect.InflateRect(-1, -1);
	if (nConectState != CSTATE_DISABLE)
		dc.DrawEdge(&rect, BDR_RAISEDINNER, BF_RECT|BF_ADJUST);
	else
		dc.DrawEdge(&rect, BDR_SUNKENOUTER, BF_RECT|BF_ADJUST);

	// 如果已经连接动画
	if (nConectState > CSTATE_DISCONNECT_SUPPORTMOUSEINPUT)
	{
		HFONT hOldFont = dc.SelectFont(m_pPropCtrl->m_fontWinnding);
		COLORREF clrOld = dc.SetTextColor(RGB(128, 0, 0));
		int nOldBackMode = dc.SetBkMode(TRANSPARENT);
		TCHAR szBuf[8];
		wsprintf(szBuf, _T("%c"), 252); 
		dc.DrawText(szBuf, 1, &rect, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
		dc.SetBkMode(nOldBackMode);
		dc.SetTextColor(clrOld);
		dc.SelectFont(hOldFont);
	}
}

//////////////////////////////////////////////////////////////////////
// CPropertyEdit
//////////////////////////////////////////////////////////////////////

HRESULT CPropertyEdit::OnClick(CRect rect, CPoint pt, int nIndex)
{
	USES_CONVERSION;	
	
	CComBSTR bstr;
	BOOL bSameValue;
	if (GetText(bstr, bSameValue) != S_OK)
		return E_FAIL;
	
	CEdit& edit = m_pPropCtrl->m_editInplace;

	edit.SetWindowText(OLE2T(bstr));
	rect.InflateRect(-3, -1, 0, -1);
	edit.SetWindowPos(NULL, rect.left, rect.top, 
		rect.Width(), rect.Height(), SWP_SHOWWINDOW);
// 	edit.SetFocus();

	m_pPropCtrl->m_list.ClientToScreen(&pt);
	edit.ScreenToClient(&pt);
	edit.SendMessage(WM_LBUTTONDOWN, 0, MAKELONG(pt.x, pt.y));

	m_pPropCtrl->m_propEdit = this;
	
	return S_OK;
}

HRESULT CPropertyEdit::OnDblClick(CRect rect, int nIndex)
{
	USES_CONVERSION;	
	
	CComBSTR bstr;
	BOOL bSameValue;
	if (GetText(bstr, bSameValue) != S_OK)
		return E_FAIL;
	
	CEdit& edit = m_pPropCtrl->m_editInplace;
	
	edit.SetWindowText(OLE2T(bstr));
	rect.InflateRect(-3, -1, 0, -1);
	edit.SetWindowPos(NULL, rect.left, rect.top, 
		rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	edit.SetSelAll();
	edit.SetFocus();
			
	m_pPropCtrl->m_propEdit = this;

	return S_OK;
}

HRESULT CPropertyEdit::SetText(CComBSTR& bstr)
{
	return HelperPutValue(CComVariant(bstr));
}

HRESULT CPropertyEdit::GetText(CComBSTR& bstr, BOOL& bSame)
{
	BOOL bSameValue;
	if (CPropertyBlank::GetText(bstr, bSameValue) == S_FALSE)
		bstr = L"";
	bSame = bSameValue;

	return S_OK;	
}

//////////////////////////////////////////////////////////////////////
// CPropertyEnum
//////////////////////////////////////////////////////////////////////

HRESULT CPropertyEnum::Fill(IUnknown *pUnk) 
{
	USES_CONVERSION;

	ITypeInfo *pti = (ITypeInfo *)pUnk;
	if (!pti) 
		return E_FAIL;
	
	TYPEATTR* pta = 0;
	pti->GetTypeAttr(&pta);
	if (!pta) 
		return E_FAIL;
	
	TCHAR* pText = 0;
	
	if (pta->typekind == TKIND_ENUM)
	{		
		VARDESC* pvd = 0;
		WCHAR szBuf[128];
		
		for (int i = 0; i < pta->cVars; i++)
		{
			pti->GetVarDesc(i, &pvd);	
			
			CComBSTR bstrName; 
			CComBSTR bstrDocString;
			pti->GetDocumentation(pvd->memid, &bstrName, &bstrDocString, 0, 0);

			CEnumPropItem item;
			swprintf(szBuf, L"%d - %s", pvd->lpvarValue->lVal, bstrName);
			item.m_bstrName = szBuf;
			swprintf(szBuf, L"%d - %s", pvd->lpvarValue->lVal, bstrDocString);
			item.m_bstrDesc = szBuf;

			item.m_varValue = *pvd->lpvarValue;
			m_arrEnumPropItems.Add(item);
			
			pti->ReleaseVarDesc(pvd);
		}
	}
	
	pti->ReleaseTypeAttr(pta);
	
	return S_OK;
}

HRESULT CPropertyEnum::OnClick(CRect rect, CPoint pt, int nIndex)
{
	USES_CONVERSION;
	
	if (m_bCanEdit && !m_pPropCtrl->m_editInplace.IsWindowVisible())
	{
		USES_CONVERSION;
		
		CComBSTR bstr;
		BOOL bSameValue;
		if (GetText(bstr, bSameValue) != S_OK)
			return E_FAIL;

		CEdit& edit = m_pPropCtrl->m_editInplace;
		
		edit.SetWindowText(OLE2T(bstr));
		CRect rcT = rect;
		rcT.right -= rcT.bottom - rcT.top;
		rcT.InflateRect(-3, -1, 0, -1);
		edit.SetWindowPos(NULL, rcT.left, rcT.top, 
			rcT.Width(), rcT.Height(), SWP_SHOWWINDOW);
			
		CPoint ptT = pt;
		m_pPropCtrl->m_list.ClientToScreen(&ptT);
		edit.ScreenToClient(&ptT);
		edit.SendMessage(WM_LBUTTONDOWN, 0, MAKELONG(ptT.x, ptT.y));
		
		m_pPropCtrl->m_propEdit = this;
	}

	m_pPropCtrl->m_editInplace.SetSelAll();

	if (pt.x < rect.right - rect.Height() - 1)
		return S_OK;
	
	CPopupList combo;
	HFONT hFont = m_pPropCtrl->m_list.GetFont();
	if (!combo.Create(m_pPropCtrl->m_list, hFont))
		return E_FAIL;
	
	CComVariant var;
	BOOL bSameValue;
	HelperGetValue(var, bSameValue);
	
	int nCurValue = -1;
	for (int i = 0; i < m_arrEnumPropItems.GetSize(); i++)
	{		
		CEnumPropItem& item = m_arrEnumPropItems[i];

		if (m_pPropCtrl->m_bShowHelpString)
			combo.AddString(OLE2T(item.m_bstrDesc));
		else
			combo.AddString(OLE2T(item.m_bstrName));
		if (item.m_varValue == var) 
			nCurValue = i;		
	}
	
	pt.x = rect.left;
	pt.y = rect.bottom;
	ClientToScreen(m_pPropCtrl->m_list, &pt);


	nCurValue = combo.Show(pt.x, pt.y, rect.Width(), 
		nCurValue, &m_pPropCtrl->m_editInplace);

	
	
//	if (m_pPropCtrl->m_propEdit == NULL)
//		SetFocus(m_pPropCtrl->m_list);

	

	if (nCurValue != -1)
	{
		m_pPropCtrl->m_propEdit = NULL;
		SetFocus(m_pPropCtrl->m_list);

		HelperPutValue(CComVariant(m_arrEnumPropItems[nCurValue].m_varValue));
		return S_OK;	
	}

	SetFocus(m_pPropCtrl->m_list);
	
	return S_FALSE;	
}

HRESULT CPropertyEnum::OnDblClick(CRect rect, int nIndex)
{
	if (m_bCanEdit)
	{
		USES_CONVERSION;

		CComBSTR bstr;
		BOOL bSameValue;
		if (GetText(bstr, bSameValue) != S_OK)
			return E_FAIL;
		
		CEdit& edit = m_pPropCtrl->m_editInplace;
		
		edit.SetWindowText(OLE2T(bstr));
		rect.right -= rect.bottom - rect.top;
		rect.InflateRect(-3, -1, 0, -1);
		edit.SetWindowPos(NULL, rect.left, rect.top, 
			rect.Width(), rect.Height(), SWP_SHOWWINDOW);
		edit.SetSelAll();
		edit.SetFocus();
		
		m_pPropCtrl->m_propEdit = this;
	}
	else
	{
		CComVariant var;
		BOOL bSameValue;
		HelperGetValue(var, bSameValue);

		int nCurValue = -1;
		for (int i = 0; i < m_arrEnumPropItems.GetSize(); i++)
		{
			CComVariant varT = m_arrEnumPropItems[i].m_varValue;
			varT.ChangeType(var.vt);

			if (varT == var)
			{
				nCurValue = i;
				break;
			}
		}
		
		if (nCurValue == -1) 
			return S_FALSE; 
		
		if (++nCurValue == m_arrEnumPropItems.GetSize())
			nCurValue = 0;
		
		var = m_arrEnumPropItems[nCurValue].m_varValue;
		HelperPutValue(var);
	}

	return S_OK;
}	

void CPropertyEnum::OnDrawValue(CDCHandle dc, CRect rect, BOOL bSelected)
{
	CRect rcT;
	CComVariant var;
	BOOL bSameValue;
	
	HelperGetValue(var, bSameValue);
	if (bSameValue == TRUE)
	{	
		rcT = rect;
		if (bSelected)
			rcT.right -= rcT.bottom - rcT.top;

		int nCurValue = -1;

		for (int i = 0; i < m_arrEnumPropItems.GetSize(); i++)
		{
			CComVariant varT = m_arrEnumPropItems[i].m_varValue;
			varT.ChangeType(var.vt);

			if (varT == var)
			{
				nCurValue = i;
				break;
			}
		}

		LPCWSTR lpszText = NULL;	
		if (nCurValue == -1)
		{
			if (var.vt == VT_BSTR)
				lpszText = var.bstrVal;
			else
				lpszText = L"**error**";
		}
		else
		{			
			if (m_pPropCtrl->m_bShowHelpString)
				lpszText = m_arrEnumPropItems[nCurValue].m_bstrDesc;
			else
				lpszText = m_arrEnumPropItems[nCurValue].m_bstrName;
		}
		
		int cbSize = lstrlenW(lpszText); 
		DrawItemText(dc.m_hDC, lpszText, cbSize, rcT);
	}
	else
	{
		dc.FillRect(&rect, COLOR_WINDOW);
	}
	
	if (bSelected)
	{
		rcT = rect;
		rcT.left = rcT.right - rcT.Height();
		rcT.InflateRect(1, 0, 0, 1);
		dc.DrawFrameControl(&rcT, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
	}
}

HRESULT CPropertyEnum::SetText(CComBSTR& bstr)
{
	if (m_bCanEdit)
		return HelperPutValue(CComVariant(bstr));

	return S_FALSE;
}

//////////////////////////////////////////////////////////////////////
// CPropertyBool
//////////////////////////////////////////////////////////////////////

HRESULT CPropertyBool::OnClick(CRect rect, CPoint pt, int nIndex)
{
	if (pt.x < rect.right - rect.Height() - 1)
		return S_FALSE;
	
	CPopupList combo;
	HFONT hFont = m_pPropCtrl->m_list.GetFont();
	if (!combo.Create(m_pPropCtrl->m_list, hFont))
		return E_FAIL;
	
	CComVariant var;
	BOOL bSameValue;
	HelperGetValue(var, bSameValue);
	
	combo.AddString(_T("True"));
	combo.AddString(_T("False"));
	
	pt.x = rect.left;
	pt.y = rect.bottom;
	ClientToScreen(m_pPropCtrl->m_list, &pt);
	
	int nCmd = combo.Show(pt.x, pt.y, rect.Width(), 
		var.boolVal ? 0 : 1);
	if (nCmd != -1)
	{
		HelperPutValue(CComVariant(nCmd == 0));
		return S_OK;
	}
	
	return S_FALSE;	
}

HRESULT CPropertyBool::OnDblClick(CRect rect, int nIndex)
{
	CComVariant var;
	BOOL bSameValue;
	HelperGetValue(var, bSameValue);	 
	
	var = CComVariant(!var.boolVal);
	HelperPutValue(var);
		
	return S_OK;
}

void CPropertyBool::OnDrawValue(CDCHandle dc, CRect rect, BOOL bSelected)
{
	CRect rcT;
	CComVariant var;
	BOOL bSameValue;
	
	HelperGetValue(var, bSameValue);
	if (bSameValue == TRUE)
	{	
		LPCWSTR lpszText = var.boolVal ? L"True" : L"False";	
		unsigned int cbSize = lstrlenW(lpszText);
		
		rcT = rect;
		
		if (bSelected)
			rcT.right -= rcT.bottom - rcT.top;
		DrawItemText(dc.m_hDC, lpszText, cbSize, rcT);
	}
	else
	{
		dc.FillRect(&rect, COLOR_WINDOW);
	}
	
	if (bSelected)
	{
		rcT = rect;
		rcT.left = rcT.right - rcT.Height();
		rcT.InflateRect(1, 0, 0, 1);
		dc.DrawFrameControl(&rcT, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
	}
}

//////////////////////////////////////////////////////////////////////
// CPropertyColor
//////////////////////////////////////////////////////////////////////

HRESULT CPropertyColor::OnDblClick(CRect rect, int nIndex)
{
	CPoint pt;
	pt.x = rect.right;
	pt.y = rect.bottom;

	return OnClick(rect, pt, nIndex);
}

HRESULT CPropertyColor::OnClick(CRect rect, CPoint pt, int nIndex)
{
	if (pt.x >= rect.right - rect.Height() - 1)
	{
		CComPtr<ICxColorPicker> p;		
		HRESULT hr = p.CoCreateInstance(CLSID_CxColorPicker); 
		if (FAILED(hr)) 
			return hr;
		
		CComVariant var;
		BOOL bSameValue;
		hr = HelperGetValue(var, bSameValue);
		if (FAILED(hr)) 
			return hr;
		
		pt.x = rect.right;
		pt.y = rect.bottom;
		ClientToScreen(m_pPropCtrl->m_list, &pt);
		
		OLE_COLOR clr = var.lVal;
		HWND hWnd = m_pPropCtrl->m_list;
		hr = p->Pick((long)hWnd, pt.x, pt.y, clr, &clr); 				
		if (FAILED(hr)) 
			return hr;
		
		if (clr == (OLE_COLOR)var.lVal)
			return S_FALSE;
		
		var = (long)clr;	
		HelperPutValue(var);
		
		return S_OK;
	}
	else
	{
		rect.left += rect.Height() + 1;
		rect.right -= rect.Height() + 1;
		return CPropertyEdit::OnClick(rect, pt, nIndex);
	}

	return S_FALSE;
}

void CPropertyColor::OnDrawValue(CDCHandle dc, CRect rect, BOOL bSelected)
{
	USES_CONVERSION;

	CRect rcT;
	CComVariant var;
	BOOL bSameValue;

	dc.FillRect(&rect, COLOR_WINDOW);

	HRESULT hr = HelperGetValue(var, bSameValue);
	if (FAILED(hr)) 
		return;

	if (bSameValue == TRUE)
	{	
		rcT = rect;
		if (bSelected)
			rcT.right -= rcT.Height();
		
		CRect rcColor = rcT;
		rcColor.left += 1;
		rcColor.right = rcColor.left + rcColor.Height();

		rcT.left = rcColor.right;
		rcColor.InflateRect(-1, -1);

		HPEN hOldPen = dc.SelectStockPen(BLACK_PEN);
		
		COLORREF color;
		OleTranslateColor(OLE_COLOR(var.lVal), 0, &color);
		CBrush brush;
		brush.CreateSolidBrush(color);
		HBRUSH hOldBrush = dc.SelectBrush(brush);
	
		dc.Rectangle(&rcColor);
		
		dc.SelectPen(hOldPen);
		dc.SelectBrush(hOldBrush);
	
		TCHAR szBuf[16];
		wsprintf(szBuf,_T("&H%08x&"), var.lVal);
		_tcsupr(szBuf);
		DrawItemText(dc.m_hDC, T2W(szBuf), lstrlen(szBuf), rcT);
	}
	
	if (bSelected)
	{
		rcT = rect;
		rcT.left = rcT.right - rcT.Height();
		rcT.InflateRect(1, 0, 0, 1);
		dc.DrawFrameControl(&rcT, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
	}
}

HRESULT CPropertyColor::SetText(CComBSTR& bstr)
{
	LPOLESTR p = bstr;
	while (iswspace(*p)) 
		p++;

	if (*p == '&' && *(p + 1) == 'H')
	{
		long lValue = wcstoul(p + 2, NULL, 16);

// 		BYTE* pComponents = (BYTE*)&var.lVal;
// 		BYTE bValue = pComponents[2];
// 		pComponents[2] = pComponents[0];
// 		pComponents[0] = bValue;

		COLORREF color;
		if (OleTranslateColor(lValue, 0, &color) == S_OK)
			return HelperPutValue(CComVariant(lValue));	
	}
	
	return E_FAIL;
}

HRESULT CPropertyColor::GetText(CComBSTR& bstr, BOOL& bSame)
{
	CComVariant var;
	if (FAILED(HelperGetValue(var, bSame)))
	{
		bstr = L"";
		return S_FALSE;
	}
	
	TCHAR szText[16];
	wsprintf(szText, _T("&H%08x&"), var.lVal);
	_tcsupr(szText);
	bstr = szText;
	
	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// CPropertyPropPage
//////////////////////////////////////////////////////////////////////

HRESULT CPropertyPropPage::Init()
{
	USES_CONVERSION;
	
	wchar_t sGuid[40] = {0};
	StringFromGUID2(m_clsidPage, sGuid, 40);
	TCHAR sKey[MAX_PATH] = {0};
	wsprintf(sKey, _T("SOFTWARE\\Classes\\CLSID\\%s\\InprocServer32"), W2T(sGuid));
				
	CRegKey key;
	HRESULT hr = key.Open(HKEY_LOCAL_MACHINE, sKey, KEY_READ);
	if (ERROR_SUCCESS != hr)
		return E_FAIL;

	DWORD dw = MAX_PATH;
	TCHAR sPath[MAX_PATH] = {0};
	hr = key.QueryValue(sPath, 0, &dw);
	if (ERROR_SUCCESS != hr)
		return E_FAIL;

	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(sPath, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
		return E_FAIL;	
   
	FindClose(hFind);
	
	return S_OK;
}

bool CPropertyPropPage::OnClickImpl()
{
//	DWORD dw;
//	HANDLE hThread = CreateThread(0, 0, ThreadProc, this,  0, &dw);
//	AtlWaitWithMessageLoop(hThread);
//	CloseHandle(hThread);

	OCPFIPARAMS ocpf;
	memset(&ocpf, 0, sizeof OCPFIPARAMS);
	
	ocpf.cbStructSize = sizeof OCPFIPARAMS;
	ocpf.hWndOwner = m_pPropCtrl->m_hWnd;
	ocpf.x = 0;
	ocpf.y = 0;
	ocpf.lpszCaption = m_name.m_str;
	ocpf.cObjects = m_pPropCtrl->m_nObjects;
	ocpf.lplpUnk = (IUnknown**)m_pPropCtrl->m_ppUnk;
	ocpf.cPages = 1;
	ocpf.lpPages = &m_clsidPage;
	ocpf.lcid = LOCALE_SYSTEM_DEFAULT;
	ocpf.dispidInitialProperty = m_dispid;
	
	OleCreatePropertyFrameIndirect(&ocpf);

	return TRUE;
}

DWORD WINAPI CPropertyPropPage::ThreadProc(void* pv)
{		
	CoInitialize(0);
	
	CPropertyPropPage* _this = static_cast<CPropertyPropPage*>(pv);
	
	OCPFIPARAMS ocpf;
	memset(&ocpf, 0, sizeof OCPFIPARAMS);
	
	ocpf.cbStructSize = sizeof OCPFIPARAMS;
	ocpf.hWndOwner = _this->m_pPropCtrl->m_hWnd;
	ocpf.x = 0;
	ocpf.y = 0;
	ocpf.lpszCaption = _this->m_name.m_str;
	ocpf.cObjects = 1;
	ocpf.lplpUnk = (IUnknown**)&_this->m_pPropCtrl->m_ppUnk[0];
	ocpf.cPages = 1;
	ocpf.lpPages = &_this->m_clsidPage;
	ocpf.lcid = LOCALE_SYSTEM_DEFAULT;
	ocpf.dispidInitialProperty = _this->m_dispid;
	
	OleCreatePropertyFrameIndirect(&ocpf);
	
	::CoUninitialize();
	return 0;
}

HRESULT CPropertyPropPage::OnClick(CRect rect, CPoint pt, int nIndex)
{
	return OnDblClick(rect, nIndex);
}

HRESULT CPropertyPropPage::OnDblClick(CRect rect, int nIndex)
{
	return OnClickImpl() ? S_OK : S_FALSE;	
}

void CPropertyPropPage::OnDrawValue(CDCHandle dc, CRect rect, BOOL bSelected)
{
	CRect rcT;
	CComBSTR bstr;
	BOOL bSameValue;
	
	GetText(bstr, bSameValue);
	
	if (bSameValue == TRUE)
	{	
		rcT = rect;
		
		if (bSelected)
			rcT.right -= rcT.bottom - rcT.top;
		DrawItemText(dc.m_hDC, bstr, bstr.Length(), rcT);
	}
	else
	{
		dc.FillRect(&rect, COLOR_WINDOW);
	}
	
	if (bSelected)
	{
		rcT = rect;
		rcT.left = rcT.right - (rcT.bottom - rcT.top);
		rcT.InflateRect(1, 0, 0, 1);
		DrawEllipsisButton(dc, rcT);
	}
}

//////////////////////////////////////////////////////////////////////
// CPropertyPicture
//////////////////////////////////////////////////////////////////////

HRESULT CPropertyPicture::GetText (CComBSTR& bstr, BOOL& bSame)
{
	CComVariant var;
	BOOL bSameValue;
	HRESULT hr = HelperGetValue(var, bSameValue);
	bSame = bSameValue;
	if (SUCCEEDED(hr) && 
		((var.vt == VT_DISPATCH) || (var.vt == VT_UNKNOWN)))
	{	
		bstr = L"(空)";
		CComQIPtr<IPicture> pPicture(var.pdispVal);
		if(pPicture.p)
		{
			short type = 0;
			hr = pPicture->get_Type(&type);
			if (SUCCEEDED(hr))
			{
				switch (type)
				{
					case PICTYPE_UNINITIALIZED: 
						bstr = L"(未初始化)"; 
						break;
					case PICTYPE_NONE:			 
						break;
					case PICTYPE_BITMAP:			 
						bstr = L"(位图)"; 
						break;
					case PICTYPE_METAFILE:		 
						bstr = L"(元文件)"; 
						break;
					case PICTYPE_ICON:			 
						bstr = L"(图标)"; 
						break;
					case PICTYPE_ENHMETAFILE:	 
						bstr = L"(增强元文件)"; 
						break;
					default:							 
						bstr = L"(未知格式)"; 
						break;			
				}
			}
		}
	}
	
	if (FAILED(hr)) 
	{
		bstr = L"**错误**";		
		return S_FALSE;
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// CPropertyDataConnection
//////////////////////////////////////////////////////////////////////

bool CPropertyDataConnection::OnClickImpl()
{	
	CComObject<CPropertyPageDlg>* pDlg = new CComObject<CPropertyPageDlg>;
	pDlg->AddRef();
	
	pDlg->m_bstrCaption = _T("数据连接属性");
	pDlg->m_clsidPage = m_clsidPage;
	
	CComVariant var;
	BOOL bSameValue;
	HRESULT hr = HelperGetValue(var, bSameValue);
	pDlg->m_spObject = var.pdispVal;
	
	VARIANT_BOOL vbCancel = VARIANT_FALSE;
	m_pPropCtrl->Fire_PropertyChanging(m_dispid, &vbCancel);
	ATLASSERT(vbCancel == VARIANT_FALSE);
	
	if (pDlg->DoModal() == IDOK)
	{
		m_pPropCtrl->Fire_PropertyChanged(m_dispid);
	}
	
	pDlg->Release();
	
	return TRUE;	
}

HRESULT CPropertyDataConnection::GetText(CComBSTR& bstr, BOOL& bSame)
{
	CComVariant var;
	BOOL bSameValue;
	HRESULT hr = HelperGetValue(var, bSameValue);
	bSame = bSameValue;
	
	if (SUCCEEDED(hr) && 
		((var.vt == VT_DISPATCH) || (var.vt == VT_UNKNOWN)))
	{	
		if (var.pdispVal != NULL)
		{
			CComDispatchDriver dd(var.pdispVal);
			CComVariant varT;
			dd.GetPropertyByName(L"ConnectionString", &varT);
			if (varT.vt == VT_BSTR)
				bstr = varT.bstrVal;
		}
		else
		{
			bstr = L"(空)";
		}
	}
	
	if (FAILED(hr)) 
	{
		bstr = L"**error**";		
		return S_FALSE;
	}
	
	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// CPropertyDataRecordset
//////////////////////////////////////////////////////////////////////

bool CPropertyDataRecordset::OnClickImpl()
{	
	CComObject<CPropertyPageDlg>* pDlg = new CComObject<CPropertyPageDlg>;
	pDlg->AddRef();
	
	pDlg->m_bstrCaption = _T("数据记录集属性");
	pDlg->m_clsidPage = m_clsidPage;
	
	CComVariant var;
	BOOL bSameValue;
	HRESULT hr = HelperGetValue(var, bSameValue);
	pDlg->m_spObject = var.pdispVal;
	
	VARIANT_BOOL vbCancel = VARIANT_FALSE;
	m_pPropCtrl->Fire_PropertyChanging(m_dispid, &vbCancel);
	ATLASSERT(vbCancel == VARIANT_FALSE);
	
	if (pDlg->DoModal() == IDOK)
	{
		m_pPropCtrl->Fire_PropertyChanged(m_dispid);
	}
	
	pDlg->Release();
	
	return TRUE;
}

HRESULT CPropertyDataRecordset::GetText(CComBSTR& bstr, BOOL& bSame)
{
	CComVariant var;
	BOOL bSameValue;
	HRESULT hr = HelperGetValue(var, bSameValue);
	bSame = bSameValue;
	
	if (SUCCEEDED(hr) && 
		((var.vt == VT_DISPATCH) || (var.vt == VT_UNKNOWN)))
	{	
		if (var.pdispVal != NULL)
		{
			CComDispatchDriver dd(var.pdispVal);
			CComVariant varT;
			dd.GetPropertyByName(L"Source", &varT);
			if (varT.vt == VT_BSTR)
				bstr = varT.bstrVal;
		}
		else
		{
			bstr = L"(空)";
		}
	}
	
	if (FAILED(hr)) 
	{
		bstr = L"**error**";		
		return S_FALSE;
	}
	
	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// CPropertyPredefinedValue
//////////////////////////////////////////////////////////////////////

HRESULT CPropertyPredefinedValue::Fill(IUnknown *pUnk)
{
	IPerPropertyBrowsing* ppb = (IPerPropertyBrowsing *)pUnk;
	
	CALPOLESTR castr;
	CADWORD  cadw; 
	
	::ZeroMemory(&castr, sizeof(castr));
	::ZeroMemory(&cadw, sizeof(cadw));
	
	HRESULT hr = ppb->GetPredefinedStrings(m_dispid, &castr, &cadw);
	if (FAILED(hr)) 
		return hr;
	
	for (ULONG i = 0; i < cadw.cElems; i++)
	{
		CComVariant var;			
		hr = ppb->GetPredefinedValue(m_dispid, cadw.pElems[i], &var);			
		if (FAILED(hr)) 
			continue;

		CEnumPropItem item;
		item.m_bstrName = castr.pElems[i];
		item.m_bstrDesc = castr.pElems[i];
		item.m_varValue = var;

		m_arrEnumPropItems.Add(item);
	}
	
	if (castr.pElems != NULL)
	{
		for (ULONG i = 0; i < castr.cElems; i++)
			CoTaskMemFree((void *)castr.pElems[i]);  
		CoTaskMemFree((void *)castr.pElems); 			
	}
	if (cadw.pElems != NULL)
	{			
		CoTaskMemFree((void *)cadw.pElems);			
	}
	
	return m_arrEnumPropItems.GetSize() > 0 ? S_OK : E_FAIL;
}	

//////////////////////////////////////////////////////////////////////
// CPropertyCustom
//////////////////////////////////////////////////////////////////////

HRESULT CPropertyCustom::OnClick(CRect rect, CPoint pt, int nIndex)
{
	if (pt.x >= rect.right - rect.Height() - 1)
		return OnDblClick(rect, nIndex);

	if (IsEditable())
	{
		rect.right -= rect.Height() + 1;
		return CPropertyEdit::OnClick(rect, pt, nIndex);
	}
	else
	{
		return OnDblClick(rect, nIndex);
	}
	
	return S_FALSE;
}

HRESULT CPropertyCustom::OnDblClick(CRect rect, int nIndex)
{
	return OnClickImpl() ? S_OK : S_FALSE;	
}

void CPropertyCustom::OnDrawValue(CDCHandle dc, CRect rect, BOOL bSelected)
{
	CRect rcT;
	CComBSTR bstr;
	BOOL bSameValue;
	
	GetText(bstr, bSameValue);
	
	if (bSameValue == TRUE)
	{	
		rcT = rect;
		
		if (bSelected)
			rcT.right -= rcT.bottom - rcT.top;
		DrawItemText(dc.m_hDC, bstr, bstr.Length(), rcT);
	}
	else
	{
		dc.FillRect(&rect, COLOR_WINDOW);
	}
	
	if (bSelected)
	{
		rcT = rect;
		rcT.left = rcT.right - (rcT.bottom - rcT.top);
		rcT.InflateRect(1, 0, 0, 1);
		DrawEllipsisButton(dc, rcT);
	}
}

//////////////////////////////////////////////////////////////////////
// CPropertyFile
//////////////////////////////////////////////////////////////////////

bool CPropertyFile::OnClickImpl()
{
	USES_CONVERSION;
	
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof OPENFILENAME);			
	ofn.lStructSize = sizeof OPENFILENAME;
	ofn.lpstrFile =  (TCHAR*) _alloca(_MAX_PATH * sizeof TCHAR);
	ofn.nMaxFile = _MAX_PATH;	
	ofn.hwndOwner = m_pPropCtrl->m_hWnd;				
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	
	CComBSTR bstr;
	BOOL bSameValue;
	GetText(bstr, bSameValue);
	if (bstr.m_str != NULL)
		lstrcpy(ofn.lpstrFile, W2T(bstr.m_str)); 
	
	ofn.lpstrFilter = 
		_T("所有文件\0*.*\0");
	ofn.lpstrTitle = _T("选择文件");
	
	if (GetOpenFileName(&ofn))
	{
		bstr = ofn.lpstrFile;
		return SetText(bstr) == S_OK;		
	}	
	return false;
}

//////////////////////////////////////////////////////////////////////
// CPropertyBrushFile
//////////////////////////////////////////////////////////////////////

bool CPropertyBrushFile::OnClickImpl()
{
	USES_CONVERSION;
	
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof OPENFILENAME);			
	ofn.lStructSize = sizeof OPENFILENAME;
	ofn.lpstrFile =  (TCHAR*) _alloca(_MAX_PATH * sizeof TCHAR);
	ofn.nMaxFile = _MAX_PATH;	
	ofn.hwndOwner = m_pPropCtrl->m_hWnd;				
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	
	CComBSTR bstr;
	BOOL bSameValue;
	GetText(bstr, bSameValue);
	if (bstr.m_str != NULL)
		lstrcpy(ofn.lpstrFile, W2T(bstr.m_str)); 
	
	ofn.lpstrFilter = 
		_T("所有图片文件\0*.bmp;*.dib;*.gif;*.jpg;*.png;*.wmf;*.emf;*.ico;*.cur\0")
		_T("位图 (*.bmp;*.dib)\0*.bmp;*.dib\0")
		_T("GIF 图像 (*.gif)\0*.gif\0")
		_T("JPEG 图像 (*.jpg)\0*.jpg\0")
		_T("元文件 (*.wmf;*.emf)\0*.wmf;*.emf\0")
		_T("Icons (*.ico;*.cur)\0*.ico;*.cur\0")
		_T("所有文件(*.*)\0*.*\0");
	ofn.lpstrTitle = _T("选择文件");
	
	if (GetOpenFileName(&ofn))
	{
		bstr = ofn.lpstrFile;
		return SetText(bstr) == S_OK;		
	}	
	return false;
}

//////////////////////////////////////////////////////////////////////
// CPropertyFolder
//////////////////////////////////////////////////////////////////////

bool CPropertyFolder::OnClickImpl()
{	
	USES_CONVERSION;
	BROWSEINFO bi;
	memset(&bi, 0, sizeof BROWSEINFO);
	bi.ulFlags = 0; //BIF_NEWDIALOGSTYLE | BIF_USENEWUI | BIF_EDITBOX
	bi.hwndOwner = m_pPropCtrl->m_hWnd;
	bi.pszDisplayName = (TCHAR*)_alloca(MAX_PATH * sizeof TCHAR);
	memset(bi.pszDisplayName, 0, MAX_PATH * sizeof TCHAR);
	
	CComBSTR bstr;
	BOOL bSameValue;
	GetText(bstr, bSameValue);
	lstrcpy(bi.pszDisplayName, W2T(bstr.m_str)); 
	
	LPITEMIDLIST pidl = 0;
	pidl = SHBrowseForFolder(&bi);
	if (pidl)
	{
		SHGetPathFromIDList(pidl, bi.pszDisplayName);
		
		CComPtr<IMalloc> pMalloc;
		SHGetMalloc(&pMalloc);
		if (pMalloc) pMalloc->Free(pidl);
		
		bstr = bi.pszDisplayName;
		return SetText(bstr) == S_OK;		
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// CPropertyFakePicture
//////////////////////////////////////////////////////////////////////

HRESULT CPropertyFakePicture::SetText(CComBSTR& bstr)
{		
	CComPtr<IDispatch> spPicture;
	OleLoadPicturePath(bstr, 0, 0, 0, IID_IDispatch, (void**)&spPicture);
	HRESULT hr = HelperPutValue(CComVariant(spPicture));

	return hr;
}

HRESULT CPropertyFakePicture::GetText(CComBSTR& bstr, BOOL& bSame)
{
	CComVariant var;
	BOOL bSameValue;
	HRESULT hr = HelperGetValue(var, bSameValue);
	bSame = bSameValue;
	
	if (SUCCEEDED(hr) && 
		((var.vt == VT_DISPATCH) || (var.vt == VT_UNKNOWN)))
	{	
		bstr = L"(空)";
		CComQIPtr<IPicture> spPicture(var.pdispVal);
		if (spPicture != NULL)
		{
			short type = 0;
			hr = spPicture->get_Type(&type);
			if (SUCCEEDED(hr))
				switch (type)
			{
					case PICTYPE_UNINITIALIZED: bstr = L"(未初始化)"; break;
					case PICTYPE_NONE:			 break;
					case PICTYPE_BITMAP:			 bstr = L"(位图)"; break;
					case PICTYPE_METAFILE:		 bstr = L"(元文件)"; break;
					case PICTYPE_ICON:			 bstr = L"(图标)"; break;
					case PICTYPE_ENHMETAFILE:	 bstr = L"(增强元文件)"; break;
					default:							 bstr = L"(未知格式)"; break;			
			}
		}
	}
	
	if (FAILED(hr)) 
	{
		bstr = L"**error**";		
		return S_FALSE;
	}
	return S_OK;
}

bool CPropertyFakePicture::OnClickImpl()
{
	USES_CONVERSION;
	
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof OPENFILENAME);			
	ofn.lStructSize = sizeof OPENFILENAME;
	ofn.lpstrFile =  (TCHAR*) _alloca(_MAX_PATH * sizeof TCHAR);
	ofn.nMaxFile = _MAX_PATH;	
	ofn.hwndOwner = m_pPropCtrl->m_hWnd;				
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	
	memset(ofn.lpstrFile, 0, _MAX_PATH * sizeof TCHAR); 
	
	ofn.lpstrFilter = 
		_T("所有图片文件\0*.bmp;*.dib;*.gif;*.jpg;*.png;*.wmf;*.emf;*.ico;*.cur\0")
		_T("位图 (*.bmp;*.dib)\0*.bmp;*.dib\0")
		_T("GIF 图像 (*.gif)\0*.gif\0")
		_T("JPEG 图像 (*.jpg)\0*.jpg\0")
		_T("元文件 (*.wmf;*.emf)\0*.wmf;*.emf\0")
		_T("Icons (*.ico;*.cur)\0*.ico;*.cur\0")
		_T("所有文件(*.*)\0*.*\0");
	ofn.lpstrTitle = _T("加载图片");
	
	if (GetOpenFileName(&ofn))
	{
		return SetText(CComBSTR(ofn.lpstrFile)) == S_OK;		
	}	
	
	return false;
}

//////////////////////////////////////////////////////////////////////
// CPropertyFakeFont
//////////////////////////////////////////////////////////////////////

bool CPropertyFakeFont::OnClickImpl()
{
	LOGFONT logfont;
	HFONT hFont;
	
	CHOOSEFONT cf; 
	memset(&cf, 0, sizeof CHOOSEFONT);
	cf.lStructSize = sizeof CHOOSEFONT;
	cf.hInstance = _Module.m_hInst;
	cf.hwndOwner = m_pPropCtrl->m_hWnd;
	cf.lpLogFont = &logfont;
	cf.lpTemplateName = "IDD_FONTDIALOG";
	cf.Flags = CF_SCREENFONTS | CF_EFFECTS | CF_ENABLETEMPLATE;

	CComVariant var;
	BOOL bSameValue;
	HRESULT hr = HelperGetValue(var, bSameValue);
	if (SUCCEEDED(hr) && 
		((var.vt == VT_DISPATCH) || (var.vt == VT_UNKNOWN)))
	{	
		CComQIPtr<IFont> spFont(var.pdispVal);

		if (spFont != NULL)
		{
			spFont->get_hFont(&hFont);
			GetObject(hFont, sizeof(LOGFONT), &logfont); 
			cf.Flags |= CF_INITTOLOGFONTSTRUCT;
		}
	}
	
	if (ChooseFont(&cf))
	{
		USES_CONVERSION;
		
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
		HDC hdc = m_pPropCtrl->GetDC();
		ppi = GetDeviceCaps(hdc, LOGPIXELSY);
		m_pPropCtrl->ReleaseDC(hdc);
		
		fd.cySize.Lo = lfHeight * 720000 / ppi;
		fd.cySize.Hi = 0;
		
		CComPtr<IDispatch> spFont;			
		hr = OleCreateFontIndirect(&fd, IID_IDispatch, (void**)&spFont);
		if (spFont != NULL)
			hr = HelperPutValue(CComVariant(spFont));
		return hr == S_OK;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// CPropertyFakeFontName
//////////////////////////////////////////////////////////////////////

HRESULT CPropertyFakeFontName::OnDblClick(CRect rect, int nIndex)
{
	CPoint pt;
	pt.x = rect.right;
	pt.y = rect.bottom;
	
	return OnClick(rect, pt, nIndex);
}

HRESULT CPropertyFakeFontName::OnClick(CRect rect, CPoint pt, int nIndex)
{
	if (pt.x >= rect.right - rect.Height() - 1)
	{
		CComPtr<ICxFontCombo> p;		
		HRESULT hr = p.CoCreateInstance(CLSID_CxFontCombo); 
		if (FAILED(hr)) 
			return hr;
		
		CComVariant var;
		BOOL bSameValue;
		hr = HelperGetValue(var, bSameValue);
		if (FAILED(hr)) return hr;
		
		CComBSTR bstr = var.bstrVal;
		HWND hWnd = m_pPropCtrl->m_list;
		POINT pt = {rect.left, rect.bottom};	
		ClientToScreen(m_pPropCtrl->m_list, &pt);
		hr = p->PickFont((long)hWnd, pt.x, pt.y, rect.Width(), bstr, &bstr); 				
		if (FAILED(hr)) return hr;
		
		if (bstr == var.bstrVal)
			return S_FALSE;
		
		var = bstr;	
		hr = HelperPutValue(var);
		
		return hr;		
		
	}
	else
	{
		rect.right -= rect.Height() + 1;
		return CPropertyEdit::OnClick(rect, pt, nIndex);
	}

	return S_FALSE;
}

void CPropertyFakeFontName::OnDrawValue(CDCHandle dc, CRect rect, BOOL bSelected)
{
	CRect rcT;
	CComBSTR bstr;
	BOOL bSameValue;
	
	GetText(bstr, bSameValue);
	
	if (bSameValue == TRUE)
	{	
		rcT = rect;
		
		if (bSelected)
			rcT.right -= rcT.Height();
		DrawItemText(dc, bstr, bstr.Length(), rcT);
	}
	else
	{
		dc.FillRect(&rect, COLOR_WINDOW);
	}
	
	if (bSelected)
	{
		rcT = rect;
		rcT.left = rcT.right - rcT.Height();
		rcT.InflateRect(1, 0, 0, 1);
		dc.DrawFrameControl(&rcT, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
	}
}

const CLSID CLSID_CxTagPicker = {0x6CF3993A,0x1D26,0x40BF,{0xB4,0xD8,0x94,0xC4,0xB5,0x4F,0x13,0x90}};

bool CPropertyTagGroup::OnClickImpl()
{	
	CComPtr<IDispatch> spTagPick;
	HRESULT hr = spTagPick.CoCreateInstance(CLSID_CxTagPicker);
	if (FAILED(hr))
		return FALSE;

	VARIANTARG* pvars = new VARIANTARG[7];
	for (int i = 0; i < 7; i++)
		VariantInit(&pvars[i]);

	//关联标签组
	CComBSTR bstr;

	CComDispatchDriver dd(m_pPropCtrl->m_ppUnk[0]);
	CComVariant vaAssocTagGroup;
	hr = dd.GetPropertyByName(L"AssocTagGroup", &vaAssocTagGroup);
	if (SUCCEEDED(hr))
	{
		bstr = vaAssocTagGroup.bstrVal;
	}

	// 原标签组
	CComBSTR bstrGroup;
	BOOL bSameValue;
	GetText(bstrGroup, bSameValue);
	pvars[0].vt = VT_BSTR;
	pvars[0].bstrVal = bstrGroup;
	bstrGroup.m_str = NULL;
	// 关联标签组
	pvars[1].vt = VT_BSTR;
	pvars[1].bstrVal = bstr;
	bstr.m_str = NULL;
	// 禁止使用非关联标签
	pvars[2].vt = VT_BOOL;
	pvars[2].boolVal = VARIANT_FALSE;
	// 使用本地配置
	pvars[3].vt = VT_BOOL;
	pvars[3].boolVal = VARIANT_TRUE;
	// Y座标
	pvars[4].vt = VT_I4;
	pvars[4].lVal = 0;
	// X座标
	pvars[5].vt = VT_I4;
	pvars[5].lVal = 0;
	// 父窗口句柄
	pvars[6].vt = VT_I4;
	pvars[6].lVal = (long)m_pPropCtrl->m_hWnd;

	CComVariant vaRet = L"";

	DISPPARAMS dispparams = { pvars, NULL, 7, 0};
	hr = spTagPick->Invoke(2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &vaRet, NULL, NULL);
	for (i = 0; i < 7; i++)
		VariantClear(&pvars[i]);
	delete [] pvars;
	if (FAILED(hr))
		return FALSE;

	if (vaRet.vt == VT_BSTR)
		return SetText(CComBSTR(vaRet.bstrVal)) == S_OK;

	return FALSE;
}

//////////////////////////////////////////////////////////////////

bool CPropertySecurityArea::OnClickImpl()
{	
	CComPtr<IDispatch> spTagPick;
	HRESULT hr = spTagPick.CoCreateInstance(CLSID_CxTagPicker);
	if (FAILED(hr))
		return FALSE;
	
	VARIANTARG* pvars = new VARIANTARG[5];
	for (int i = 0; i < 5; i++)
		VariantInit(&pvars[i]);
	
	// 原安全区
	CComBSTR bstrArea;
	BOOL bSameValue;
	GetText(bstrArea, bSameValue);
	pvars[0].vt = VT_BSTR;
	pvars[0].bstrVal = bstrArea;
	bstrArea.m_str = NULL;
	// 使用本地配置
	pvars[1].vt = VT_BOOL;
	pvars[1].boolVal = VARIANT_TRUE;
	// Y座标
	pvars[2].vt = VT_I4;
	pvars[2].lVal = 0;
	// X座标
	pvars[3].vt = VT_I4;
	pvars[3].lVal = 0;
	// 父窗口句柄
	pvars[4].vt = VT_I4;
	pvars[4].lVal = (long)m_pPropCtrl->m_hWnd;
	
	CComVariant vaRet = L"";
	
	DISPPARAMS dispparams = { pvars, NULL, 5, 0};
	hr = spTagPick->Invoke(3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &vaRet, NULL, NULL);
	for (i = 0; i < 5; i++)
		VariantClear(&pvars[i]);
	delete [] pvars;
	if (FAILED(hr))
		return FALSE;
	
	if (vaRet.vt == VT_BSTR)
		return SetText(CComBSTR(vaRet.bstrVal)) == S_OK;
	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// CPropertyArray
//////////////////////////////////////////////////////////////////////

// 为支持多选增加功能
// 显示共有属性
void CPropertyArray::AddProperty(CProperty* pprop, BOOL bPutRef)
{
	for (int i = 0; i < m_Array.GetSize(); i++)
	{
		CProperty* pTemp = m_Array[i];
		if (pTemp->m_dispid == pprop->m_dispid && 
			pTemp->m_name == pprop->m_name &&
			pprop->m_dispid != CXPROPID_Name &&
			pTemp->VerifyClass(-1) == pprop->VerifyClass(-1))
		{
			if (bPutRef)
				pTemp->m_bHasPutRef = TRUE;
			else
				pTemp->m_bHasPut = TRUE;
			pTemp->m_lRef++;
			delete pprop;
			return;
		}
	}
	
	if (bPutRef)
		pprop->m_bHasPutRef = TRUE;
	else
		pprop->m_bHasPut = TRUE;
	m_Array.Add(pprop);
}

void CPropertyArray::ClearArray()
{	
	for (int i = 0; i < m_Array.GetSize(); i++)
		delete m_Array[i];	
	m_Array.RemoveAll();
}

int CPropertyArray::Fill_ListView(HWND hList, int iIndex, BOOL bCalNum)
{
	long r = 0;
	int iRet = 0;
	
	LVITEM lvi;
	memset(&lvi, 0, sizeof LVITEM);
	
	lvi.mask = LVIF_PARAM;
	lvi.iItem = iIndex;
	
	for (int i = m_Array.GetSize() - 1; i >= 0; i--)
	{	
		CProperty* pProp = (CProperty *)m_Array[i];
		long nPropObjectCount = pProp->m_bExtend ? 
			pProp->m_pPropCtrl->m_nObjects : pProp->m_pPropCtrl->m_nObjectClasses;
		
		long nPropRef = 0;
		if (pProp->m_bHasPut)
			nPropRef += nPropObjectCount;
		if (pProp->m_bHasPutRef)
			nPropRef += nPropObjectCount;
		
		//注意此处的两倍，以为有些属性如画面的特殊性
		if (pProp->m_lRef == nPropRef)
		{
			if (!bCalNum)
			{
				lvi.lParam = (long)pProp;
				r = ListView_InsertItem(hList, &lvi);
				ATLASSERT(-1 != r);
			}
			
			iRet++;
		}
	}
	return iRet;
}

//////////////////////////////////////////////////////////////////////
// CPropertyCat
//////////////////////////////////////////////////////////////////////

CPropertyCat::CPropertyCat(CPropCtrl* pPropCtrl, ICategorizeProperties* pCP, PROPCAT propcat) : 
	CProperty(pPropCtrl), 
	m_propcat(propcat), 
	m_bExpand(true) 
{
	if (pCP && SUCCEEDED(pCP->GetCategoryName(m_propcat, LOCALE_SYSTEM_DEFAULT, &m_name))) 
		return;
	m_name = GetCategoryName(m_propcat);
}

CPropertyCat::~CPropertyCat() 
{ 
	ClearArray(); 
}

LPCWSTR CPropertyCat::GetCategoryName(PROPCAT pcat)
{		
	struct _PropCatEntry 
	{ 
		PROPCAT pcat; 
		LPCWSTR text; 
	};
	
	static _PropCatEntry _PropCatEntryArray [] = 
	{			
		PROPCAT_Misc,       L"杂项",
			PROPCAT_Font,       L"字体",
			PROPCAT_Position,   L"位置",
			PROPCAT_Appearance, L"外观",
			PROPCAT_Behavior,   L"行为",
			PROPCAT_Data,       L"数据",
			PROPCAT_List,       L"List",
			PROPCAT_Text,       L"文本",
			PROPCAT_Scale,      L"Scale",
			PROPCAT_DDE,        L"DDE" ,
			PROPCAT_Nil,        L"Unknown",
			0,                  L"*错误*"
	};
	
	_PropCatEntry* pentry = &_PropCatEntryArray[0];
	while (pentry->pcat && (pentry->pcat != pcat))
		pentry++;
	
	return pentry->text;
}

HRESULT CPropertyCat::OnPaint(CDCHandle dc, CRect rect, BOOL bSelected)
{
	CRect rcT = rect;
	rcT.right = rcT.left + m_pPropCtrl->m_nSignOffset;

	CPoint ptCenter = rcT.CenterPoint();
	CRect rcSign;
	rcSign.left = ptCenter.x - 4;			
	rcSign.top = ptCenter.y - 4;
	rcSign.right = ptCenter.x + 5;
	rcSign.bottom = ptCenter.y + 5;	

	CPen pen;
	pen.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_WINDOWTEXT));
	HPEN hOlePen = dc.SelectPen(pen);

	dc.MoveTo(rcSign.left + 2, ptCenter.y);
	dc.LineTo(rcSign.right - 2, ptCenter.y);
	
	if (!m_bExpand)
	{					
		dc.MoveTo(ptCenter.x, rcSign.top + 2);
		dc.LineTo(ptCenter.x, rcSign.bottom - 2);
	}

	dc.FrameRect(&rcSign, (HBRUSH)::GetStockObject(BLACK_BRUSH));

	dc.SelectPen(hOlePen);
	
	//绘制属性名称
	rcT.left = rcT.right;
	rcT.right = rcT.left + m_pPropCtrl->m_nNameWidth;
	rcT.left++;
	
	COLORREF clrBack = bSelected ? GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_WINDOW);
	COLORREF clrText = bSelected ? GetSysColor(COLOR_HIGHLIGHTTEXT) : GetSysColor(COLOR_WINDOWTEXT);
	
	HFONT hOldFont = dc.SelectFont(m_pPropCtrl->m_fontBold);
	DrawItemText(dc.m_hDC, m_name.m_str, m_name.Length(), rcT, clrBack, clrText);
	dc.SelectFont(hOldFont);
	
	return S_OK;
}

HRESULT CPropertyCat::OnClick(CRect rect, CPoint pt, int nIndex)
{
	return OnDblClick(rect, nIndex);
}

HRESULT CPropertyCat::OnDblClick(CRect rect, int nIndex)
{
//	m_pPropCtrl->m_bNotPanitHeader = TRUE;
//	m_pPropCtrl->m_list.SetRedraw(FALSE);

	if (m_bExpand)
		Collapse(nIndex + 1);
	else
		Expand(nIndex + 1);
// 	m_pPropCtrl->m_list.SetRedraw(TRUE);
	m_bExpand = !m_bExpand;
	
	return S_OK;
}

void CPropertyCat::Collapse(int iIndex)
{		
	for (int i = 0; i < m_Array.GetSize(); i++)
		ListView_DeleteItem(m_pPropCtrl->m_list, iIndex); 
}

void CPropertyCat::Expand(int iIndex)
{
	Fill_ListView(m_pPropCtrl->m_list, iIndex);
}
