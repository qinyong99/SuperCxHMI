// FontComboBox.h: interface for the CFontComboBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONTCOMBOBOX_H__7D79A33B_CCD5_4E2A_A8E0_D2B30C27FA65__INCLUDED_)
#define AFX_FONTCOMBOBOX_H__7D79A33B_CCD5_4E2A_A8E0_D2B30C27FA65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

#include <atlapp.h>
#include <atlctrls.h>
#include <atlFrame.h>
#include <AtlMisc.h>

struct FONTITEM_PPG
{
	DWORD dwFontType;
	LOGFONT lf;
};

#define DX_BITMAP        20
#define DY_BITMAP        12

void _InitMaskFromBitmap(CBitmap* pbmp, CBitmap* pbmpMask);
void _DrawMaskedBitmap(CDCHandle& dc, CBitmap* pbmp, CBitmap* pbmpMask,
	int x, int y, int cx, int cy);

class CFontComboBox : public CWindowImpl<CFontComboBox, CComboBox>  
{
public:
	CFontComboBox();
	virtual ~CFontComboBox();

	CBitmap m_bmpTrueType;
	CBitmap m_bmpMask;

	BEGIN_MSG_MAP(CFontComboBox)
		MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()
		
	void Init(HWND hWnd)
	{
		ATLASSERT(::IsWindow(hWnd));
		SubclassWindow(hWnd);

		m_bmpTrueType.LoadBitmap(IDB_TRUETYPE);
		_InitMaskFromBitmap(&m_bmpTrueType, &m_bmpMask);

		CClientDC dc(m_hWnd);
		EnumFontFamilies(dc.m_hDC, (LPCTSTR) NULL,
			(FONTENUMPROC)CFontComboBox::EnumFontFamiliesCallBack, (LPARAM)this);
	}

	static int CALLBACK EnumFontFamiliesCallBack(ENUMLOGFONT *lpelf, NEWTEXTMETRIC *, int FontType, LPARAM lParam)
	{
		CFontComboBox *pBox = (CFontComboBox *)lParam;
		ATLASSERT(pBox);
		pBox->AddFont(&lpelf->elfLogFont, FontType);
		return 1;
	}

	int AddFont(LOGFONT *pLF, DWORD FontType)
	{
		int nEntry;
		FONTITEM_PPG* pFontItem = NULL;

		// Font already in the combobox
		if (FindStringExact(-1, (LPCTSTR) pLF->lfFaceName) != CB_ERR)
			return CB_ERR;

		// allocate some memory for the FONTITEM_PPG structure
		pFontItem = new FONTITEM_PPG;

		ATLASSERT(pFontItem);
		pFontItem->lf = *pLF;
		pFontItem->dwFontType = FontType;

		nEntry = AddString( (LPCTSTR) pFontItem->lf.lfFaceName );

		if (nEntry == CB_ERR)
			delete pFontItem;
		else
			SetItemData( nEntry, (DWORD) pFontItem );

		return nEntry;
	}

	FONTITEM_PPG* CFontComboBox::GetFontItem(int sel)
	{
		USES_CONVERSION;

		if (sel == -1)
			sel = GetCurSel();

		if (sel == -1)
		{
			CComBSTR bstr;

			GetWindowText((BSTR&)bstr);
			sel = FindStringExact(-1, OLE2T(bstr));
			if (sel == CB_ERR)
				sel = 0;
		}

		ATLASSERT(GetItemData(sel));
		return (FONTITEM_PPG*)GetItemData(sel);
	}

	LPLOGFONT GetLogFont(int sel)
	{
		return &GetFontItem(sel)->lf;
	}

	DWORD GetFontType(int sel = -1)
	{
		return GetFontItem(sel)->dwFontType;
	}

	BSTR GetCurrentName()
	{
		CComBSTR bstr;

		GetWindowText((BSTR&)bstr);
		return bstr;
	}

	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;
	
		// make sure this is a *real* item
		if (lpDIS->itemID == -1)
			return 0;

		CDCHandle dc = lpDIS->hDC;
		FONTITEM_PPG* pFI = (FONTITEM_PPG*)lpDIS->itemData;    // pointer to a FONTITEM storied in item data
		LOGFONT* pLF = &pFI->lf;
		COLORREF crBk, crText;
		TEXTMETRIC tm;
		int x, y;

		// Calculate the colors to use
		crBk = dc.SetBkColor(
			GetSysColor(lpDIS->itemState & ODS_SELECTED ? COLOR_HIGHLIGHT : COLOR_WINDOW) );
		crText = dc.SetTextColor(
			GetSysColor(lpDIS->itemState & ODS_SELECTED ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT) );

		// Calculate the position of the text
		dc.GetTextMetrics( &tm );
		x = LOWORD(GetDialogBaseUnits()) / 4;
		y = (lpDIS->rcItem.bottom + lpDIS->rcItem.top - tm.tmHeight) / 2;

		// Draw the text
		dc.ExtTextOut(lpDIS->rcItem.left + DX_BITMAP + 2 * x, y, ETO_CLIPPED | ETO_OPAQUE,
			&lpDIS->rcItem,(LPCTSTR) pLF->lfFaceName,
			lstrlen((LPCTSTR) pLF->lfFaceName), NULL );

		// Put the colors back as they were
		dc.SetTextColor( crText );
		dc.SetBkColor( crBk );

		// Draw the TrueType bitmap
		if (pFI->dwFontType & TRUETYPE_FONTTYPE)
		{
			int dy;
			dy = ((lpDIS->rcItem.bottom - lpDIS->rcItem.top) - DY_BITMAP) / 2;
			_DrawMaskedBitmap(dc, &m_bmpTrueType, &m_bmpMask,
				x, lpDIS->rcItem.top + dy, DX_BITMAP, DY_BITMAP);
		}

		// Draw the focus rect if needed
		if (lpDIS->itemState & ODS_FOCUS)
			dc.DrawFocusRect(&lpDIS->rcItem);

		return 0;
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int nCount = GetCount();
		for (int i = 0; i < nCount; i++)
		{
			delete (FONTITEM_PPG *)GetItemData(i);
		}

		return DefWindowProc(uMsg, wParam, lParam);
	}
};

#endif // !defined(AFX_FONTCOMBOBOX_H__7D79A33B_CCD5_4E2A_A8E0_D2B30C27FA65__INCLUDED_)
