// ColorPicker2.h : Declaration of the CColorPicker2

#ifndef __CXCOLORPICKER2_H_
#define __CXCOLORPICKER2_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CColorPicker2
class ATL_NO_VTABLE CColorPicker2 : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CColorPicker2, &CLSID_CxColorPicker2>,
	public IDispatchImpl<ICxColorPicker2, &IID_ICxColorPicker2, &LIBID_CXCOMOBJLib>,
	public CWindowImpl<CColorPicker2>
{
public:
	struct ColorTableEntry
	{
		COLORREF clrColor;
		LPCTSTR	pszName;
	};

public:
	CColorPicker2()
	{
		m_clrCurrent = CLR_DEFAULT;
		m_clrDefault = ::GetSysColor(COLOR_APPWORKSPACE);
		m_pszDefaultText = _tcsdup(_T("自动"));
		m_pszCustomText = _tcsdup(_T("其他颜色..."));
		m_bPopupActive = FALSE;
		m_bTrackSelection = FALSE;
		m_bMouseOver = FALSE;
	}

	COLORREF GetColor (void) const
	{
		return m_clrCurrent;
	}
	
	void SetColor (COLORREF clrCurrent)
	{
		m_clrCurrent = clrCurrent;
		if (IsWindow ())
			InvalidateRect (NULL);
	}
	
	COLORREF GetDefaultColor (void) const
	{
		return m_clrDefault;
	}
	
	void SetDefaultColor (COLORREF clrDefault)
	{
		m_clrDefault = clrDefault;
	}
	
	void SetCustomText (LPCTSTR pszText)
	{
		if (m_pszCustomText)
			free (m_pszCustomText);
		if (pszText)
			m_pszCustomText = _tcsdup (pszText);
		else
			m_pszCustomText = NULL;
	}
	
	void SetCustomText (UINT nID)
	{
		if (nID == 0)
			SetCustomText ((LPCTSTR) NULL);
		else
		{
			TCHAR szText [256];
			if (LoadString (_Module .GetResourceInstance (),
				nID, szText, sizeof (szText) / sizeof (szText [0])) == 0)
				return;
			SetCustomText (szText);
		}
	}
	
	void SetDefaultText (LPCTSTR pszText)
	{
		if (m_pszDefaultText)
			free (m_pszDefaultText);
		if (pszText)
			m_pszDefaultText = _tcsdup (pszText);
		else
			m_pszDefaultText = NULL;
	}
	
	void SetDefaultText (UINT nID)
	{
		if (nID == 0)
			SetDefaultText ((LPCTSTR) NULL);
		else
		{
			TCHAR szText [256];
			if (LoadString (_Module .GetResourceInstance (),
				nID, szText, sizeof (szText) / sizeof (szText [0])) == 0)
				return;
			SetDefaultText (szText);
		}
	}
	
	BOOL GetTrackSelection (void) const
	{
		return m_bTrackSelection;
	}
	
	void SetTrackSelection (BOOL fTrack)
	{
		m_bTrackSelection = fTrack;
	}
	
	void SetText (UINT nDefault, UINT nCustom)
	{
		SetDefaultText (nDefault);
		SetCustomText (nCustom);
	}
	
	BOOL HasCustomText () const
	{
		return m_pszCustomText && m_pszCustomText [0] != 0;
	}
	
	BOOL HasDefaultText () const
	{
		return m_pszDefaultText && m_pszDefaultText [0] != 0;
	}

DECLARE_WND_CLASS_EX(_T("ColorPicker"), CS_CLASSDC | CS_SAVEBITS | CS_HREDRAW | CS_VREDRAW, COLOR_MENU)

DECLARE_REGISTRY_RESOURCEID(IDR_CXCOLORPICKER2)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CColorPicker2)
	COM_INTERFACE_ENTRY(ICxColorPicker2)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

BEGIN_MSG_MAP (CColorButton)
	MESSAGE_HANDLER(WM_PAINT, OnPaint);
	MESSAGE_HANDLER(WM_QUERYNEWPALETTE, OnQueryNewPalette);
	MESSAGE_HANDLER(WM_PALETTECHANGED, OnPaletteChanged);
END_MSG_MAP ()

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnQueryNewPalette(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnPaletteChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);


// ICxColorPicker2
public:
	STDMETHOD(Pick)(long hParent, RECT rect, OLE_COLOR clrDefault, OLE_COLOR clr, OLE_COLOR* pclr);
	
protected:
	void SetWindowSize(RECT rect);
	void CreateToolTips(CToolTipCtrl &sToolTip);
	BOOL GetCellRect(int nIndex, RECT *pRect) const;
	void FindCellFromColor(COLORREF clr);
	void ChangeSelection(int nIndex);
	void EndSelection(BOOL fOked);
	void DrawCell(CDC &dc, int nIndex);
	int HitTest(const POINT &pt);

protected:

	static ColorTableEntry	gm_sColors[];

	COLORREF				m_clrCurrent;
	COLORREF				m_clrDefault;
	LPTSTR					m_pszDefaultText;
	LPTSTR					m_pszCustomText;
	BOOL					m_bPopupActive;
	BOOL					m_bTrackSelection;
	BOOL					m_bMouseOver;
	
	int						m_nNumColumns;
    int						m_nNumRows;
	int						m_nNumColors;
    CFont					m_font;
	CPalette				m_palette;
	COLORREF				m_clrPicker;
	CRect					m_rectMargins;
	CRect					m_rectCustomText;
	CRect					m_rectDefaultText;
	CRect					m_rectBoxes;
	BOOL					m_bFlat;
	CSize					m_sizeBox;
    int						m_nCurrentSel;
    int						m_nChosenColorSel;
	BOOL					m_bOked;
	COLORREF				m_clrBackground;
	COLORREF				m_clrHiLightBorder;
	COLORREF				m_clrHiLight;
	COLORREF				m_clrLoLight;
	COLORREF				m_clrHiLightText;
	COLORREF				m_clrText;
};

#endif //__CXCOLORPICKER2_H_
