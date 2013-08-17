// CustCtrl.h: interface for the CCustCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTCTRL_H__FFCBDB6C_828F_464F_B8C2_57A32AD756C3__INCLUDED_)
#define AFX_CUSTCTRL_H__FFCBDB6C_828F_464F_B8C2_57A32AD756C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlapp.h>
#include <atlctrls.h>
#include <AtlMisc.h>
#include <math.h>

#undef SubclassWindow

#define WM_VALUECHANGED WM_USER + 1

#include "../CxComObj/CxComObj.h"

#define CPN_SELCHANGE        0x8000	/* Colour Picker Selection change */

struct NMCOLORBUTTON
{
	NMHDR hdr;
	BOOL fColorValid;
	COLORREF clr;
};

class CColorButton : public CWindowImpl<CColorButton>  
{
public:
	OLE_COLOR m_clr;
	OLE_COLOR m_clrDefault;
	
	CColorButton()
	{
		m_clr = CLR_DEFAULT;
		m_clrDefault = ::GetSysColor (COLOR_APPWORKSPACE);
	}
	
	void SetColor(OLE_COLOR clr)
	{
		m_clr = clr;
		InvalidateRect(NULL);
	}
	
	OLE_COLOR GetColor()
	{
		return m_clr;
	}

	void SetDefaultColor (OLE_COLOR clrDefault)
	{
		m_clrDefault = clrDefault;
	}

	OLE_COLOR GetDefaultColor()
	{
		return m_clrDefault;
	}
	
	BOOL SubclassWindow(HWND hWnd)
	{
		CWindowImpl<CColorButton>::SubclassWindow(hWnd);
		ModifyStyle(0, BS_OWNERDRAW);

		return TRUE;
	}

protected:
	
	BEGIN_MSG_MAP(CColorButton)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER (OCM_DRAWITEM, OnDrawItem)
	END_MSG_MAP()
		
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		SetFocus();

		CComPtr<ICxColorPicker2> p;		
		HRESULT hr = p.CoCreateInstance(CLSID_CxColorPicker2); 
		if (FAILED(hr)) 
			return 0;
	
		RECT rect;
		GetWindowRect(&rect);
		hr = p->Pick((long)m_hWnd, rect, m_clrDefault, m_clr, &m_clr);
		if (FAILED(hr)) 
			return 0;
		
		if (hr == S_OK)
		{
			SendNotification (CPN_SELCHANGE, m_clr, TRUE);
			InvalidateRect(NULL);
		}
	
		return 0;
	}

	void SendNotification (UINT nCode, COLORREF clr, BOOL bColorValid)
	{
		NMCOLORBUTTON nmclr;
		
		nmclr.hdr .code = nCode;
		nmclr.hdr .hwndFrom = m_hWnd;
		nmclr.hdr .idFrom = GetDlgCtrlID ();
		nmclr.fColorValid = bColorValid;
		nmclr.clr = clr;
		
		::SendMessage(GetParent(), WM_NOTIFY, 
				(WPARAM)GetDlgCtrlID(), (LPARAM)&nmclr);
	}
	
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LPDRAWITEMSTRUCT lpItem = (LPDRAWITEMSTRUCT)lParam;
		CDC dc(lpItem ->hDC);
		
		UINT uState = lpItem ->itemState;
		CRect rcDraw = lpItem ->rcItem;
		
		if ((uState & ODS_FOCUS) != 0/* || m_fPopupActive*/) 
		{
			CRect rcFocus (rcDraw.left, rcDraw.top, 
				rcDraw.right - 1, rcDraw.bottom);
			dc.DrawFocusRect(&rcFocus);
		}
		
		rcDraw.InflateRect(-4, -2);
//		dc.SetBkColor(GetSysColor(COLOR_WINDOW));
//		dc.ExtTextOut(0, 0, ETO_OPAQUE, &rcDraw, NULL, 0, NULL);
		
		dc.SetBkColor((m_clr == CLR_DEFAULT) ? m_clrDefault : m_clr);
		dc.ExtTextOut(0, 0, ETO_OPAQUE, &rcDraw, NULL, 0, NULL);
		dc.FrameRect(&rcDraw, (HBRUSH)::GetStockObject(WHITE_BRUSH));
		rcDraw.InflateRect(-1, -1);
		dc.DrawFocusRect(&rcDraw);
		
		return 1;
	}
	
};

class CLineCombo : public CWindowImpl<CLineCombo, CComboBox>  
{
public:
	CLineCombo()
	{
	}

	void Init(HWND hWnd)
	{
		ATLASSERT(::IsWindow(hWnd));
		SubclassWindow(hWnd);
		
		TCHAR szBuf[8];
		for (int i = 0; i < 6; i++)
		{
			sprintf(szBuf, "%d", i);
			AddString(szBuf);
		}
	}

	BEGIN_MSG_MAP(CLineCombo)
		MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem)
	END_MSG_MAP()

	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
		HDC hdc = lpDrawItemStruct->hDC;
		RECT rcItem = lpDrawItemStruct->rcItem;

		BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED) != 0;
		::SetBkColor(hdc, bSelected ? GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_WINDOW));
		::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcItem, 0, 0, 0);

		HPEN hPen = ::CreatePen(lpDrawItemStruct->itemID, 1, bSelected ? GetSysColor(COLOR_HIGHLIGHTTEXT) : GetSysColor(COLOR_WINDOWTEXT));
		HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

		::MoveToEx(hdc, rcItem.left, (rcItem.top + rcItem.bottom) / 2, NULL);
		::LineTo(hdc, rcItem.right, (rcItem.top + rcItem.bottom) / 2);
	
		::SelectObject(hdc, hOldPen);
		::DeleteObject(hPen);

		return 0;
	}

};

class CMarkerCombo : public CWindowImpl<CMarkerCombo, CComboBox>  
{
public:
	enum   CMarkerType	{ NONE, CROSS, CIRCLE, TRIANGLE, DIAMOND, SQUARE };

	CMarkerCombo()
	{
	}

	void Init(HWND hWnd)
	{
		ATLASSERT(::IsWindow(hWnd));
		SubclassWindow(hWnd);
		
		TCHAR szBuf[8];
		for (int i = 0; i < 6; i++)
		{
			sprintf(szBuf, "%d", i);
			AddString(szBuf);
		}
	}

	BEGIN_MSG_MAP(CMarkerCombo)
		MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem)
	END_MSG_MAP()

	void DrawLine(HDC hdc, int x1, int y1, int x2, int y2)			{ ::MoveToEx(hdc, x1, y1, NULL); ::LineTo(hdc, x2, y2); }

	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
		HDC hdc = lpDrawItemStruct->hDC;
		RECT rcItem = lpDrawItemStruct->rcItem;

		BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED) != 0;
		::SetBkColor(hdc, bSelected ? GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_WINDOW));
		::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcItem, 0, 0, 0);

		HPEN hPen = ::CreatePen(PS_SOLID, 1, bSelected ? GetSysColor(COLOR_HIGHLIGHTTEXT) : GetSysColor(COLOR_WINDOWTEXT));
		HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

		HBRUSH hBrush = ::CreateSolidBrush(bSelected ? GetSysColor(COLOR_HIGHLIGHTTEXT) : GetSysColor(COLOR_WINDOWTEXT));
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

		int x, y, mode, n;
		mode = lpDrawItemStruct->itemID;
		x = (rcItem.left + rcItem.right) / 2;
		y = (rcItem.top + rcItem.bottom) / 2;
		n = 4;

		switch(lpDrawItemStruct->itemID)
		{
			case CROSS:
				DrawLine(hdc, x - n + 1, y, x + n, y );
				DrawLine(hdc, x, y - n + 1, x, y + n );
				break;

			case CIRCLE:
				::Ellipse(hdc, x - n, y - n, x + n, y + n);
				break;

			case TRIANGLE:
			{
				POINT  p[3];
				p[0].x = x - n;
				p[0].y = (int)(y + n * sqrt(3.) / 3);
				p[1].x = x + n;
				p[1].y = (int)(y + n * sqrt(3.) / 3);
				p[2].x = x;
				p[2].y = (int)(y - 2 * n * sqrt(3.) / 3);
				::Polygon(hdc, p, 3);
				break;
			}

			case SQUARE:
				::Rectangle(hdc, x - n, y - n, x + n, y + n);
				break;

			case DIAMOND:
			{
				POINT  p[4];
				p[0].x = x - n;
				p[0].y = y;
				p[1].x = x;
				p[1].y = y + n;
				p[2].x = x + n;
				p[2].y = y;
				p[3].x = x;
				p[3].y = y - n;
				::Polygon(hdc, p, 4);
				break;
			}
		}

		::SelectObject(hdc, hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hdc, hOldBrush);
		::DeleteObject(hBrush);
	
		return 0;
	}

};

#endif // !defined(AFX_CUSTCTRL_H__FFCBDB6C_828F_464F_B8C2_57A32AD756C3__INCLUDED_)
