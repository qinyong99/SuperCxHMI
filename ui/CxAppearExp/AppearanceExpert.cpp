// AppearanceExpert.cpp : Implementation of CAppearanceExpert
#include "stdafx.h"
#include "CxAppearExp.h"
#include "AppearanceExpert.h"
#include "SampleObject.h"

/////////////////////////////////////////////////////////////////////////////
// CAppearanceExpert

#ifdef SubclassWindow
#undef SubclassWindow
#endif

LRESULT CAppearanceExpert::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CRect rect;
	::GetWindowRect(GetDlgItem(IDC_FILL_STYLE), &rect);
	ScreenToClient(&rect);
	m_wndFillStyle.m_pExpert = this;
	m_wndFillStyle.m_bitmapImage.LoadBitmap(IDB_IMAGE);
	m_wndFillStyle.Create(m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE);
	
	::GetWindowRect(GetDlgItem(IDC_FILL_PREVIEW), &rect);
	ScreenToClient(&rect);
	m_wndFillPreview.m_pExpert = this;
	m_wndFillPreview.Create(m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE);

	::GetWindowRect(GetDlgItem(IDC_COLORS), &rect);
	ScreenToClient(&rect);
	m_wndColorPalette.Create(m_hWnd, rect, WS_CHILD | WS_VISIBLE, IDC_COLORS, RGB(195,0,0), false);
	m_wndColorPalette.GetColorControl()->SetBorderType(BORDER_ETCHED);
	m_wndColorPalette.GetColorControl()->SetCustomBar(FALSE);
	m_wndColorPalette.GetColorControl()->InitColorPalette();

//	m_ImageList.Create(IDB_SELECT_COLOR, 20, 1, RGB(255, 255, 255));
//	m_btnSelectColor.SubclassWindow(GetDlgItem(IDC_SELECT_COLOR));
//	m_btnSelectColor.SetImageList(m_ImageList);
//	m_btnSelectColor.SetImages(0);

	::GetWindowRect(GetDlgItem(IDC_LINE_STYLE), &rect);
	ScreenToClient(&rect);
	m_wndLineStyle.m_pExpert = this;
	m_wndLineStyle.Create(m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE);

	::GetWindowRect(GetDlgItem(IDC_LINE_WIDTH), &rect);
	ScreenToClient(&rect);
	m_wndLineWidth.SetRange(1, 60);
//	m_wndSlider.m_pExpert = this;
	m_wndLineWidth.Create(m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE);
	m_wndLineWidth.SetCur(1);

	::GetWindowRect(GetDlgItem(IDC_LINE_PREVIEW), &rect);
	ScreenToClient(&rect);
	m_wndLinePreview.m_pExpert = this;
	m_wndLinePreview.Create(m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE);
	
	m_lbColorProp.Attach(GetDlgItem(IDC_COLORPROP));

	return 0;	
}

LRESULT CAppearanceExpert::OnSliderChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_pSampleObject != NULL)
		m_pSampleObject->m_fEdgeWidth = (float)m_wndLineWidth.GetCur();
	OnProertyChanged(CXPROPID_EdgeWidth, &m_wndLineWidth);
	return 0;
}

LRESULT CAppearanceExpert::OnSelectColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_dwColorPropDispID != DISPID_UNKNOWN)
	{
		CComVariant var = long(wParam);
		SetPropertyValue(m_dwColorPropDispID, &var);
	}

	return 0;
}

void CAppearanceExpert::OnColorPropSelecting(void* pSender)
{
	if (pSender != &m_lbColorProp && m_lbColorProp.m_hWnd != NULL)
	{
		SelectPropListItem(m_dwColorPropDispID);
	}
	if (pSender != &m_wndFillPreview && m_wndFillPreview.m_hWnd != NULL)
	{
		if (m_dwColorPropDispID == CXPROPID_FillColor || 
			m_dwColorPropDispID == CXPROPID_HighLightFillColor)
			m_wndFillPreview.Invalidate();
	}
	if (pSender != &m_wndLinePreview && m_wndLinePreview.m_hWnd != NULL)
	{
		if (m_dwColorPropDispID == CXPROPID_EdgeColor || 
			m_dwColorPropDispID == CXPROPID_HighLightEdgeColor)
			m_wndLinePreview.Invalidate();
	}
}

void CAppearanceExpert::OnColorPropSelected(void* pSender)
{
 	OnColorPropSelecting(pSender);

	if (m_wndColorPalette.m_hWnd != NULL)
	{
		COLORREF color = RGB(0,0,0);
		if (m_dwColorPropDispID != DISPID_UNKNOWN)
		{
			CComVariant var;
			if (GetPropertyVale(m_dwColorPropDispID, &var) == S_OK)
				color = var.lVal;
		}
	
		m_wndColorPalette.SetCurColor(color);
		m_wndColorPalette.Invalidate();
	}
}

void CAppearanceExpert::OnObjectChanged()
{
	if (m_nObjects == 0 || m_ppUnk == NULL)
	{
		if (m_pSampleObject != NULL)
		{
			delete m_pSampleObject;
			m_pSampleObject = NULL;
			OnProertyChanged(DISPID_UNKNOWN);
			return;
		}
	}
	else
	{
		if (m_pSampleObject == NULL)
		{
			m_pSampleObject = new CSampleObject;
			m_pSampleObject->ICxDrawObject_InitNew();
		}
	}

	ATLASSERT(m_pSampleObject != NULL);
	
	CComVariant var;
	
	if (GetPropertyVale(CXPROPID_FillStyle, &var) != S_OK)
		var = -1;
	if (m_pSampleObject->m_enumFillStyle != (enumFillStyle)var.lVal)
	{
		m_pSampleObject->m_enumFillStyle = (enumFillStyle)var.lVal;
		OnProertyChanged(CXPROPID_FillStyle);
	}
	
	if (GetPropertyVale(CXPROPID_FillColor, &var) != S_OK)
		var = long(RGB(0, 0, 0));
	if (m_pSampleObject->m_clrFillColor != (OLE_COLOR)var.lVal)
	{
		m_pSampleObject->m_clrFillColor = var.lVal;
		OnProertyChanged(CXPROPID_FillColor);
	}
	
	if (GetPropertyVale(CXPROPID_HighLightFillColor, &var) != S_OK)
		var = long(RGB(0, 0, 0));
	if (m_pSampleObject->m_clrHighLightFill != (OLE_COLOR)var.lVal)
	{
		m_pSampleObject->m_clrHighLightFill = var.lVal;
		OnProertyChanged(CXPROPID_HighLightFillColor);
	}
	
	if (GetPropertyVale(CXPROPID_EdgeStyle, &var) != S_OK)
		var = -1;
	if (m_pSampleObject->m_enumEdgeStyle != (enumEdgeStyle)var.lVal)
	{
		m_pSampleObject->m_enumEdgeStyle = (enumEdgeStyle)var.lVal;
		OnProertyChanged(CXPROPID_EdgeStyle);
	}
	
	if (GetPropertyVale(CXPROPID_EdgeWidth, &var) != S_OK)
		var = 1.0f;
	if (m_pSampleObject->m_fEdgeWidth != var.fltVal)
	{
		m_pSampleObject->m_fEdgeWidth = var.fltVal;
		OnProertyChanged(CXPROPID_EdgeWidth);
	}

	if (GetPropertyVale(CXPROPID_EdgeColor, &var) != S_OK)
		var = long(RGB(0, 0, 0));
	if (m_pSampleObject->m_clrEdgeColor != (OLE_COLOR)var.lVal)
	{
		m_pSampleObject->m_clrEdgeColor = var.lVal;
		OnProertyChanged(CXPROPID_EdgeColor);
	}

	if (GetPropertyVale(CXPROPID_HighLightEdgeColor, &var) != S_OK)
		var = long(RGB(0, 0, 0));
	if (m_pSampleObject->m_clrHighLightEdge != (OLE_COLOR)var.lVal)
	{
		m_pSampleObject->m_clrHighLightEdge = var.lVal;
		OnProertyChanged(CXPROPID_HighLightEdgeColor);
	}
		
	if (GetPropertyVale(CXPROPID_LineJoinStyle, &var) != S_OK)
		var = -1;
	if (m_pSampleObject->m_enumLineJoinStyle != (enumLineJoinStyle)var.lVal)
	{
		m_pSampleObject->m_enumLineJoinStyle = (enumLineJoinStyle)var.lVal;
		OnProertyChanged(CXPROPID_LineJoinStyle);
	}

	if (m_lbColorProp.m_hWnd != NULL)
		FillPropnameList(GUID_COLOR, 0, m_lbColorProp);
}

void CAppearanceExpert::OnProertyChanged(DWORD dwDispID, void* pSender)
{
	if (pSender != &m_wndFillStyle && m_wndFillStyle.m_hWnd != NULL)
	{
		if (dwDispID == CXPROPID_FillStyle ||
			dwDispID == DISPID_UNKNOWN)
			m_wndFillStyle.Invalidate();
	}

	if (pSender != &m_wndFillPreview && m_wndFillPreview.m_hWnd != NULL)
	{
		if (dwDispID == CXPROPID_FillStyle || 
			dwDispID == CXPROPID_FillColor || 
			dwDispID == CXPROPID_HighLightFillColor ||
			dwDispID == DISPID_UNKNOWN)
			m_wndFillPreview.Invalidate();
	}

	if (pSender != &m_wndLineStyle && m_wndLineStyle.m_hWnd != NULL)
	{
		if (dwDispID == CXPROPID_EdgeStyle ||
			dwDispID == DISPID_UNKNOWN)
			m_wndLineStyle.Invalidate();
	}
	
	if (pSender != &m_wndLinePreview && m_wndLinePreview.m_hWnd != NULL)
	{
		if (dwDispID == CXPROPID_EdgeStyle ||
			dwDispID == CXPROPID_EdgeColor ||
			dwDispID == CXPROPID_HighLightEdgeColor ||
			dwDispID == CXPROPID_EdgeWidth ||
			dwDispID == DISPID_UNKNOWN)
			m_wndLinePreview.Invalidate();
	}

	if (pSender != &m_wndLineWidth && m_wndLineWidth.m_hWnd != NULL)
	{
		if (dwDispID == CXPROPID_EdgeWidth ||
			dwDispID == DISPID_UNKNOWN)
		{
			if (m_pSampleObject != NULL)
				m_wndLineWidth.SetCur(ROUND(m_pSampleObject->m_fEdgeWidth));
			m_wndLineWidth.Invalidate();
		}
	}

	if (pSender != &m_wndLinePreview && m_wndLinePreview.m_hWnd != NULL)
	{
		if (dwDispID == CXPROPID_LineJoinStyle ||
			dwDispID == DISPID_UNKNOWN)
			m_wndLinePreview.Invalidate();
	}

	if (pSender != NULL)
	{
		CComVariant var;
		switch (dwDispID) 
		{
		case CXPROPID_FillStyle:
			var = m_pSampleObject->m_enumFillStyle;
			break;
		case CXPROPID_EdgeStyle:
			var = m_pSampleObject->m_enumEdgeStyle;
			break;
		case CXPROPID_EdgeWidth:
			var = m_pSampleObject->m_fEdgeWidth;
			break;
		case CXPROPID_LineJoinStyle:
			var = m_pSampleObject->m_enumLineJoinStyle;
			break;
		default:
			return;
		}
		SetPropertyValue(dwDispID, &var);
	}
}

BOOL OleTypeMatchGuid(
	LPTYPEINFO pTypeInfo,
	TYPEDESC* pTypeDesc,
	REFGUID guidType,
	ULONG cIndirectionLevels)
{
	ATLASSERT(pTypeInfo != NULL);
	ATLASSERT(pTypeDesc != NULL);
	ATLASSERT(cIndirectionLevels >= 0);
	
	LPTYPEINFO pTypeInfoRef = NULL;
	
	BOOL bMatch = FALSE;
	
	switch (pTypeDesc->vt)
	{
	case VT_USERDEFINED:
		// It's an alias: Expand the alias and try to match.
		if (SUCCEEDED(pTypeInfo->GetRefTypeInfo(
			pTypeDesc->hreftype,
			&pTypeInfoRef)))
		{
			ATLASSERT(pTypeInfoRef != NULL);
			LPTYPEATTR pTypeAttr = NULL;
			if (SUCCEEDED(pTypeInfoRef->GetTypeAttr(&pTypeAttr)))
			{
				ATLASSERT(pTypeAttr != NULL);
				
				// If we've dereferenced the correct number of times,
				// test the GUIDs for equality.
				if (cIndirectionLevels == 0)
					bMatch = IsEqualGUID(pTypeAttr->guid, guidType);
				
				if (!bMatch && pTypeAttr->typekind == TKIND_ALIAS)
				{
					// GUIDs didn't match, but type expanded to another alias!
					bMatch = OleTypeMatchGuid(pTypeInfoRef,
						&pTypeAttr->tdescAlias, guidType,
						cIndirectionLevels);
				}
				
				pTypeInfoRef->ReleaseTypeAttr(pTypeAttr);
			}
			
			pTypeInfoRef->Release();
		}
		break;
		
	case VT_PTR:
		// It's a pointer: Dereference and try to match with one less level
		// of indirection.
		ATLASSERT(pTypeDesc->lptdesc != NULL);
		bMatch = OleTypeMatchGuid(pTypeInfo, pTypeDesc->lptdesc, guidType,
			cIndirectionLevels - 1);
		break;
	}
	
	return bMatch;
}

#include "atlcoll.h"

const CLSID CLSID_GroupObject = {0xA5A495FF,0xBDA6,0x467e,{0x95,0x18,0x48,0xBD,0xB7,0x3C,0xE9,0xC0}};
const CLSID CLSID_FormObject = {0x932362AB,0x1A3C,0x4e2a,{0xA4,0x59,0xF2,0x04,0xB1,0xF7,0x0E,0x10}};

void CAppearanceExpert::FillPropnameList(REFGUID guid, int nIndirect, CListBox& list)
{
	USES_CONVERSION;

	list.ResetContent();
	UINT cProps = 0;

	if (m_nObjects > 0)
	{
		LPTYPEINFO pTypeInfo;
		LPTYPEATTR pTypeAttr;
		LPFUNCDESC pFuncDesc;
		LPVARDESC pVarDesc;
		ULONG iObj;
		WORD iProp;

		CAtlMap<CLSID, int> mapObjectIndexs;

		for (iObj = 0; iObj < m_nObjects; iObj++)
		{
			if (m_ppEnumProp[iObj] != NULL)
			{
				m_ppEnumProp[iObj]->Reset();
				LONG nIndex;
				while (S_FALSE != m_ppEnumProp[iObj]->Next(&nIndex))
				{
					long nType;
					m_ppEnumProp[iObj]->GetType(&nType);
					if (nType == PROPTYPE_COLOR)
					{
						CComBSTR bstrName;
						if (!m_bShowHelpString)
							m_ppEnumProp[iObj]->GetName(&bstrName);
						else
							m_ppEnumProp[iObj]->GetDescription(&bstrName);
						DISPID dispid;
						m_ppEnumProp[iObj]->GetDispid(&dispid);
						
						// Don't insert duplicates.
						LPCTSTR lpstr = OLE2CT(bstrName);
						if (list.FindString(-1, lpstr)	== LB_ERR)
						{
							int iItem = list.AddString(lpstr);
							if (iItem >= 0)
							{
								list.SetItemData(iItem, (DWORD)dispid);
								++cProps;
							}
						}
					}
				}
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

			pTypeInfo = NULL;
			if ((spDisp != NULL) &&
				SUCCEEDED(spDisp->GetTypeInfo(0, LOCALE_USER_DEFAULT, &pTypeInfo)))
			{
				ATLASSERT(pTypeInfo != NULL);
				pTypeAttr = NULL;
				if (SUCCEEDED(pTypeInfo->GetTypeAttr(&pTypeAttr)))
				{
					ATLASSERT(pTypeAttr != NULL);

					// Iterate through all properties of object.

					for (iProp = 0; iProp < pTypeAttr->cFuncs; iProp++)
					{
						pFuncDesc = NULL;
						if (SUCCEEDED(pTypeInfo->GetFuncDesc(iProp, &pFuncDesc)))
						{
							if ((pFuncDesc->invkind == INVOKE_PROPERTYPUT || pFuncDesc->invkind == INVOKE_PROPERTYPUTREF) && 
								((pFuncDesc->wFuncFlags & (FUNCFLAG_FRESTRICTED | FUNCFLAG_FHIDDEN | FUNCFLAG_FNONBROWSABLE)) == 0))
							{
								if (pFuncDesc->cParams != 1 || !OleTypeMatchGuid(pTypeInfo,	&pFuncDesc->lprgelemdescParam[0].tdesc, guid, nIndirect))
									continue;

								CComBSTR bstrName; 
								
								if (SUCCEEDED(pTypeInfo->GetDocumentation(
									pFuncDesc->memid, m_bShowHelpString ? NULL : &bstrName, m_bShowHelpString ? &bstrName : NULL, 0, 0)))
								{
									// Don't insert duplicates.
									LPCTSTR lpstr = OLE2CT(bstrName);
									if (list.FindString(-1, lpstr)	== LB_ERR)
									{
										int iItem = list.AddString(lpstr);
										if (iItem >= 0)
										{
											list.SetItemData(iItem, (DWORD)pFuncDesc->memid);
											++cProps;
										}
									}
								}
							}
							
							pTypeInfo->ReleaseFuncDesc(pFuncDesc);
						}
					}

					for (iProp = 0; iProp < pTypeAttr->cVars; iProp++)
					{
						pVarDesc = NULL;
						if (SUCCEEDED(pTypeInfo->GetVarDesc(iProp, &pVarDesc)))
						{
							// Check whether property has desired type

							if (!(pVarDesc->wVarFlags & VARFLAG_FHIDDEN) &&
								OleTypeMatchGuid(pTypeInfo,
									&pVarDesc->elemdescVar.tdesc, guid,
									nIndirect))
							{
								// Get property name and insert into list.

								CComBSTR bstrName; 
								
								if (SUCCEEDED(pTypeInfo->GetDocumentation(
									pVarDesc->memid, m_bShowHelpString ? NULL : &bstrName, m_bShowHelpString ? &bstrName : NULL, 0, 0)))
								{
									// Don't insert duplicates.

									LPCTSTR lpstr = OLE2CT(bstrName);
									if (list.FindString(-1, lpstr) == LB_ERR)
									{
										int iItem = list.AddString(lpstr);
										if (iItem >= 0)
										{
											list.SetItemData(iItem, (DWORD)pVarDesc->memid);
											++cProps;
										}
									}
								}
							}
							pTypeInfo->ReleaseVarDesc(pVarDesc);
						}
					}

					pTypeInfo->ReleaseTypeAttr(pTypeAttr);
				}
				pTypeInfo->Release();
			}
		}

	}

	OnColorPropSelecting(&m_lbColorProp);
	SelectPropListItem(m_dwColorPropDispID);
	OnColorPropSelected(&m_lbColorProp);

	list.EnableWindow(cProps > 0);
}

LRESULT CAppearanceExpert::OnSelchangeColorProp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OnColorPropSelecting(&m_lbColorProp);
	m_dwColorPropDispID = m_lbColorProp.GetItemData(m_lbColorProp.GetCurSel());
	OnColorPropSelected(&m_lbColorProp);
	
	return 0;
}

void CAppearanceExpert::SelectPropListItem(DISPID dispid)
{
	m_dwColorPropDispID = DISPID_UNKNOWN;

	for (int i = 0; i < m_lbColorProp.GetCount(); i++)
	{
		if (m_lbColorProp.GetItemData(i) == (DWORD)dispid)
		{
			m_dwColorPropDispID = dispid;
			m_lbColorProp.SetCurSel(i);
			return;
		}
	}

	if (m_dwColorPropDispID == DISPID_UNKNOWN)
	{
		if (m_lbColorProp.GetCount() > 0)
		{
			m_dwColorPropDispID = m_lbColorProp.GetItemData(0);
			m_lbColorProp.SetCurSel(0);
		}
	}
}

void CAppearanceExpert::GetColorPropValue(DISPID dispid, OLE_COLOR* pColor)
{
	switch (dispid) 
	{
	case CXPROPID_FillColor:
		*pColor = m_pSampleObject->m_clrFillColor;
		break;
	case CXPROPID_HighLightFillColor:
		*pColor = m_pSampleObject->m_clrHighLightFill;
		break;
	case CXPROPID_EdgeColor:
		*pColor = m_pSampleObject->m_clrEdgeColor;
		break;
	case CXPROPID_HighLightEdgeColor:
		*pColor = m_pSampleObject->m_clrHighLightEdge;
		break;
	default:
		{
			CComVariant var;
			GetPropertyVale(dispid, &var);
			*pColor = var.lVal;
		}
		break;
	}
}

LRESULT CAppearanceExpert::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
	CDCHandle dc = lpdis->hDC;
	CRect rcItem = lpdis->rcItem;
		
	switch (lpdis->itemAction)
	{
	case ODA_DRAWENTIRE:
	case ODA_SELECT:
		{	
			CRect rcT = rcItem;
			rcT.right = rcT.left + rcT.bottom - rcT.top;
			OLE_COLOR color;
			GetColorPropValue((DISPID)m_lbColorProp.GetItemData(lpdis->itemID), &color);
			COLORREF clrFill;
			OleTranslateColor(color, 0, &clrFill);

			dc.DrawEdge(&rcT, EDGE_RAISED, BF_RECT|BF_ADJUST);
			dc.FillRect(&rcT, COLOR_3DFACE);

			rcT.InflateRect(-2, -2);
			dc.DrawEdge(&rcT, BDR_SUNKENOUTER, BF_RECT|BF_ADJUST);
			
			CBrush brush;
			brush.CreateSolidBrush(clrFill);
			dc.FillRect(&rcT, brush);
			
			rcT = rcItem;
			rcT.left += rcT.bottom - rcT.top;
			
			BOOL bSelected = lpdis->itemState & ODS_SELECTED;
			dc.FillRect(&rcT, (HBRUSH)LongToPtr(bSelected ? (COLOR_HIGHLIGHT + 1) : (COLOR_WINDOW + 1)));
			dc.SetTextColor(bSelected ? ::GetSysColor(COLOR_HIGHLIGHTTEXT) : ::GetSysColor(COLOR_BTNTEXT));
			dc.SetBkMode(TRANSPARENT);
			HFONT hFontOld = dc.SelectFont(AtlGetStockFont(DEFAULT_GUI_FONT));
						
			CString strText;
			m_lbColorProp.GetText(lpdis->itemID, strText);
			rcT.left += 4;
			dc.DrawText(strText, -1, &rcT, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS);
					
			dc.SelectFont(hFontOld);
		}
		break;
	case ODA_FOCUS:
		{
			HPEN hPenOld = dc.SelectPen(AtlGetStockPen(BLACK_PEN));
			rcItem.left += rcItem.bottom - rcItem.top;
			rcItem.InflateRect(-1, -1);
			dc.DrawFocusRect(&rcItem);
			dc.SelectPen(hPenOld);
		}
		break;
	}
	
	return 0;
}
