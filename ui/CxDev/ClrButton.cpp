// ClrButton.cpp : implementation file
//

#include "stdafx.h"
#include "cxdev.h"
#include "ClrButton.h"

#include "../CxComObj/CxComObj_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClrButton

CClrButton::CClrButton()
{
	m_clr = CLR_DEFAULT;
	m_clrDefault = ::GetSysColor (COLOR_APPWORKSPACE);
}

CClrButton::~CClrButton()
{
}


BEGIN_MESSAGE_MAP(CClrButton, CComboBox)
	//{{AFX_MSG_MAP(ClrButton)
		ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ClrButton message handlers

void CClrButton::SetColor(OLE_COLOR clr)
{
	m_clr = clr;
	InvalidateRect(NULL);
}

OLE_COLOR CClrButton::GetColor()
{
	return m_clr;
}

void CClrButton::SetDefaultColor (OLE_COLOR clrDefault)
{
	m_clrDefault = clrDefault;
}

OLE_COLOR CClrButton::GetDefaultColor()
{
	return m_clrDefault;
}

void CClrButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// Get the device context from the context
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	
	UINT uState = lpDrawItemStruct->itemState;
	CRect rcDraw = lpDrawItemStruct->rcItem;
	
	if ((uState & ODS_FOCUS) != 0/* || m_fPopupActive*/) 
	{
		CRect rcFocus (rcDraw.left, rcDraw.top, 
			rcDraw.right - 1, rcDraw.bottom);
		pDC->DrawFocusRect(&rcFocus);
	}
	
	rcDraw.InflateRect(-4, -2);
	
	pDC->SetBkColor((m_clr == CLR_DEFAULT) ? m_clrDefault : m_clr);
	pDC->ExtTextOut(0, 0, ETO_OPAQUE, &rcDraw, NULL, 0, NULL);

	CBrush* pWhiteBrush = CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH));

	pDC->FrameRect(&rcDraw, pWhiteBrush);
	rcDraw.InflateRect(-1, -1);
	pDC->DrawFocusRect(&rcDraw);
}

void CClrButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	
	CComPtr<ICxColorPicker2> p;		
	HRESULT hr = p.CoCreateInstance(CLSID_CxColorPicker2); 
	if (FAILED(hr)) 
		return;
	
	RECT rect;
	GetWindowRect(&rect);
	hr = p->Pick((long)m_hWnd, rect, m_clrDefault, m_clr, &m_clr);
	if (FAILED(hr)) 
		return;
	
	if (hr == S_OK)
	{
		SendNotification (CPN_SELCHANGE, m_clr, TRUE);
		InvalidateRect(NULL);
	}
}

void CClrButton::SendNotification (UINT nCode, COLORREF clr, BOOL bColorValid)
{
	NMCOLORBUTTON nmclr;
	
	nmclr.hdr .code = nCode;
	nmclr.hdr .hwndFrom = m_hWnd;
	nmclr.hdr .idFrom = GetDlgCtrlID ();
	nmclr.fColorValid = bColorValid;
	nmclr.clr = clr;
	
	GetParent()->SendMessage(WM_NOTIFY, 
		(WPARAM)GetDlgCtrlID(), (LPARAM)&nmclr);
}