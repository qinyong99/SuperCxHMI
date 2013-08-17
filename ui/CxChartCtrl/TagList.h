// AlarmAreaWindow.h: 报警区自绘窗口
//
//////////////////////////////////////////////////////////////////////

#ifndef __TAGLIST_H_
#define __TAGLIST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlapp.h>
#include <atlctrls.h>
#include <AtlMisc.h>

#include "../CxDynObjs/PropInfo.h"

#define BUTTON_WIDTH 18

typedef CContainedWindowT<CEdit> CContainedEdit;

#define WM_TAGITEMCHANGED WM_USER + 2

const CLSID CLSID_CxTagPicker = {0x6CF3993A,0x1D26,0x40BF,{0xB4,0xD8,0x94,0xC4,0xB5,0x4F,0x13,0x90}};

class CTagList : public CWindowImpl<CTagList, CListBox>  
{
public:
	CContainedEdit m_editInplace;
	int m_iEditItem;
	ExpressionCreatorProc m_pfnExpressionCreatorProc;

	BOOL m_bLocalSetup;
	
	CTagList() 
	{
		m_iEditItem = -1;
		m_pfnExpressionCreatorProc = NULL;
		m_bLocalSetup = TRUE;
	}

	////////////////////////////////////////////////////////////////////
	//功 能：初始化
	//参 数：1、窗口句柄
	//2、数据块对象
	//3、数据块报警区属性的起始DISPID
	////////////////////////////////////////////////////////////////////
	void Init(HWND hWnd)
	{
		ATLASSERT(::IsWindow(hWnd));
	//	m_hWnd = hWnd;
		SubclassWindow(hWnd);

		RECT rc = {0, 0, 0, 0};
		m_editInplace.Create(this, 1, m_hWnd, &rc, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS 
				| ES_AUTOHSCROLL | ES_NOHIDESEL);
		m_editInplace.SetFont(AtlGetStockFont(DEFAULT_GUI_FONT));
	}

	int AddTag(BSTR bstrSource)
	{
		USES_CONVERSION;

		return InsertString(-1, W2CT(bstrSource));
	}

	BEGIN_MSG_MAP(CTagList)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnDblClick)
		MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem)
		MESSAGE_HANDLER(OCM_MEASUREITEM, OnMeasureItem)
		MESSAGE_HANDLER(WM_VSCROLL,	OnVScroll)
		DEFAULT_REFLECTION_HANDLER()
	ALT_MSG_MAP(1) 
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocusEdit)
		MESSAGE_HANDLER(WM_KEYDOWN,	OnKeyDownEdit)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCodeEdit)
	END_MSG_MAP()

	LRESULT OnGetDlgCodeEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)	
	{
		return DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTALLKEYS;
	}

	LRESULT OnKeyDownEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
			SetFocus();
		}
		else if (nVirtKey == VK_ESCAPE)
		{	
			char strText[256];
			GetText(m_iEditItem, strText);
			m_editInplace.SetWindowText(strText);
			SetFocus();
		}	
	
		return 0;
	}

	LRESULT OnKillFocusEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_iEditItem != -1)
		{
			char strText[256];
			m_editInplace.GetWindowText(strText, 256);
			DeleteString(m_iEditItem);
			InsertString(m_iEditItem, strText);
			SendMessage(GetParent(), WM_TAGITEMCHANGED, m_iEditItem, 0);
		}
		m_editInplace.SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE);
		m_iEditItem = -1;
		bHandled = FALSE;

		return 0;
	}

	LRESULT OnVScroll(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_editInplace.SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE);
		m_iEditItem = -1;

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		BOOL bOutside;
		int iIndex = ItemFromPoint(pt, bOutside);
		if (!bOutside && GetCurSel() == iIndex)
		{
			RECT rcItem;
			GetItemRect(iIndex, &rcItem);
			if (pt.x < rcItem.right - (BUTTON_WIDTH + 4))
			{
				::InflateRect(&rcItem, -1, -1);
				char strText[256];
				GetText(iIndex, strText);
				m_editInplace.SetWindowText(strText);
				m_editInplace.SetWindowPos(NULL, rcItem.left, rcItem.top, rcItem.right - rcItem.left - (BUTTON_WIDTH + 4), 
					rcItem.bottom - rcItem.top, SWP_SHOWWINDOW);
				m_editInplace.SetSelAll();
				m_editInplace.SetFocus();

				m_iEditItem = iIndex;
			}
			else
			{
				OnEditTag(iIndex);
			}
		}
		else
		{
			if (bOutside)
			{
				SetCurSel(-1);
				SendMessage(GetParent(), WM_COMMAND, MAKEWPARAM(IDC_LIST_TAGS, LBN_SELCHANGE), 0L);
			}
			else
			{
				bHandled = FALSE;
			}
		}
			
		return 0;
	}

	LRESULT OnDblClick(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		USES_CONVERSION;

		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		BOOL bOutside;
		int iIndex = ItemFromPoint(pt, bOutside);
		if (!bOutside)
		{
			OnEditTag(iIndex);
		}
		
		bHandled = TRUE;
		return 0;
	}

	void SetExpressionCreatorProc(ExpressionCreatorProc pfn)
	{
		m_pfnExpressionCreatorProc = pfn;
	}

	BOOL PickTag(BSTR& bstrTag)
	{
//		if (m_pfnExpressionCreatorProc)
//		{
//			CComBSTR bstr = bstrTag;
//			m_pfnExpressionCreatorProc(&bstr);
//			if (bstr != bstrTag)
//			{
//				bstrTag = bstr.m_str;
//				bstr.m_str = NULL;
//				return TRUE;
//			}
//		}
//		else
		{	
			CComPtr<IDispatch> spTagPick;
			HRESULT hr = spTagPick.CoCreateInstance(CLSID_CxTagPicker);
			if (FAILED(hr))
				return FALSE;
			
			VARIANTARG* pvars = new VARIANTARG[7];
			for (int i = 0; i < 7; i++)
				VariantInit(&pvars[i]);
			
			// 原标签
			pvars[0].vt = VT_BSTR;
			pvars[0].bstrVal = ::SysAllocString(bstrTag);
			// 关联标签组，此处没有
			pvars[1].vt = VT_BSTR;
			pvars[1].bstrVal = NULL;
			// 禁止使用非关联标签
			pvars[2].vt = VT_BOOL;
			pvars[2].boolVal = VARIANT_FALSE;
			// 使用本地配置
			pvars[3].vt = VT_BOOL;
			pvars[3].boolVal = m_bLocalSetup ? VARIANT_TRUE : VARIANT_FALSE;
			// Y座标
			pvars[4].vt = VT_I4;
			pvars[4].lVal = 0;
			// X座标
			pvars[5].vt = VT_I4;
			pvars[5].lVal = 0;
			// 父窗口句柄
			pvars[6].vt = VT_I4;
			pvars[6].lVal = (long)m_hWnd;
			
			CComVariant vaRet;
			
			DISPPARAMS dispparams = { pvars, NULL, 7, 0};
			hr = spTagPick->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &vaRet, NULL, NULL);
			for (i = 0; i < 7; i++)
				VariantClear(&pvars[i]);
			delete [] pvars;
			if (FAILED(hr))
				return FALSE;
			
			if (vaRet.vt == VT_BSTR)
			{
				if (bstrTag != NULL)
					::SysFreeString(bstrTag);

				bstrTag = ::SysAllocString(vaRet.bstrVal);

				return TRUE;
			}
		}

		return FALSE;
	}

	void OnEditTag(int iIndex)
	{
		USES_CONVERSION;

		CComBSTR bstr;
		GetTextBSTR(iIndex, (BSTR&)bstr);
		if (PickTag((BSTR&)bstr))
		{
			DeleteString(iIndex);
			InsertString(iIndex, W2CT(bstr));
			SendMessage(GetParent(), WM_TAGITEMCHANGED, iIndex, 0);
		}
	}

	void UpdateObject()
	{
		int iCount = GetCount();
		CComVariant varTemp;
		for (int i = 0; i < iCount; i++)
		{
			varTemp.vt = VT_BSTR;
			varTemp.bstrVal = NULL;
			GetTextBSTR(i, varTemp.bstrVal);
		}
		for (; i < 15; i++)
		{
			varTemp.vt = VT_BSTR;
			varTemp.bstrVal = NULL;
		}
	}
	
	void DrawEllipsisButton(CDCHandle dc, CRect rect)
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

	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
		CDCHandle dc = lpdis->hDC;

		BOOL bSelected = lpdis->itemState & ODS_SELECTED;
		RECT rcItem = lpdis->rcItem;

		switch (lpdis->itemAction)
		{
			case ODA_DRAWENTIRE:
			case ODA_SELECT:
				{	
					dc.FillRect(&rcItem, (HBRUSH)LongToPtr(bSelected ? (COLOR_HIGHLIGHT + 1) : (COLOR_WINDOW + 1)));
					dc.SetTextColor(bSelected ? ::GetSysColor(COLOR_HIGHLIGHTTEXT) : ::GetSysColor(COLOR_BTNTEXT));
					dc.SetBkMode(TRANSPARENT);
					HFONT hFontOld = dc.SelectFont(AtlGetStockFont(DEFAULT_GUI_FONT));
					
					char strText[256];
					RECT rcT = rcItem;
					rcT.left += 2;
					if (GetText(lpdis->itemID, strText) != LB_ERR)
						dc.DrawText(strText, -1, &rcT, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS);
					
					if (bSelected)
					{
						RECT rcButton = lpdis->rcItem;
						rcButton.left = rcButton.right - BUTTON_WIDTH - 4;
						DrawEllipsisButton(dc, rcButton);
					}
					
					dc.SelectFont(hFontOld);
				}
				break;
			case ODA_FOCUS:
				if (bSelected)
				{
					HPEN hPenOld = dc.SelectPen(AtlGetStockPen(BLACK_PEN));
					dc.DrawFocusRect(&lpdis->rcItem);
					dc.SelectPen(hPenOld);
				}
				break;
		}
		
		return 0;
	}

	LRESULT OnMeasureItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		MEASUREITEMSTRUCT *pmis = (MEASUREITEMSTRUCT *)lParam;
		pmis->itemHeight = BUTTON_WIDTH + 4;
		
		return TRUE;
	}
};

#endif //__TAGLIST_H_
