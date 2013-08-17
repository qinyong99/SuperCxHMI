// ColorPicker.h : Declaration of the CColorPicker

#ifndef __COLORPICKER_H_
#define __COLORPICKER_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CColorPicker
class ATL_NO_VTABLE CColorPicker : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CColorPicker, &CLSID_CxColorPicker>,
	public IDispatchImpl<ICxColorPicker, &IID_ICxColorPicker, &LIBID_CXCOMOBJLib>,
	public CDialogImpl<CColorPicker>
{
	struct _SystemColorEntry
	{
		OLE_COLOR clr;	
		TCHAR* text;
	};	
	static _SystemColorEntry m_SystemColors[];
	static OLE_COLOR m_clrPalette[];
	
	int m_nCellSize;
	int m_nTrackIndex;
	bool m_bTrackPaletteColor;
	bool m_bSelfClose;

	HANDLE m_hEvent;
	OLE_COLOR m_clr;	
	HFONT m_hFont;

	CContainedWindow m_winTab;
	CContainedWindow m_winList;	
	CContainedWindow m_wndPalette;

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	CColorPicker() :
		m_winTab(WC_TABCONTROL, this, 1),
		m_winList(_T("LISTBOX"), this, 2),
		m_wndPalette(_T("BUTTON"), this, 3),		

		m_nCellSize(0), m_nTrackIndex(-1), m_bTrackPaletteColor(false),
		m_clr(0), m_hEvent(0), m_hFont(0)
	{
		m_pUnkMarshaler = NULL;
		m_bSelfClose = FALSE;
		InitCommonControl();
	}

	HRESULT FinalConstruct();
	void FinalRelease();	

DECLARE_REGISTRY_RESOURCEID(IDR_CXCOLORPICKER)
	DECLARE_NOT_AGGREGATABLE(CColorPicker)
DECLARE_GET_CONTROLLING_UNKNOWN()

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CColorPicker)
	COM_INTERFACE_ENTRY(ICxColorPicker)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

enum { IDD = IDD_COLOR };

BEGIN_MSG_MAP(CColorPicker)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)	
	MESSAGE_HANDLER(WM_DRAWITEM,OnDrawItem)
	MESSAGE_HANDLER(WM_ACTIVATE,OnActivate)
	
	NOTIFY_HANDLER (IDC_TAB, TCN_SELCHANGE, OnTabSelChange)
	COMMAND_HANDLER(IDC_LIST, LBN_SELCHANGE, OnSelChangeList)

	ALT_MSG_MAP(1)
	ALT_MSG_MAP(2) //list
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDownList)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUpList)	
	ALT_MSG_MAP(3) //button
		MESSAGE_HANDLER(WM_ERASEBKGND,OnEraseBkGndPalette)
		MESSAGE_HANDLER(WM_LBUTTONDOWN,OnLButtonDownPalette)
		MESSAGE_HANDLER(WM_LBUTTONUP,OnLButtonUpPalette)
		MESSAGE_HANDLER(WM_MOUSEMOVE,OnMouseMovePalette)
		MESSAGE_HANDLER(WM_RBUTTONDOWN,OnRButtonDownPalette)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDownButton)

END_MSG_MAP()


// IColorPicker
public:
	STDMETHOD(Close)();
	STDMETHOD(Pick)(long hParent, long X, long Y, OLE_COLOR clr, OLE_COLOR* pclr);

// window message
private:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);		
	LRESULT OnTabSelChange(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);	
	LRESULT OnSelChangeList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);	
	LRESULT OnEraseBkGndPalette(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	
	LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	
	LRESULT OnLButtonUpPalette(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMovePalette(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDownPalette(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRButtonDownPalette(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	
	

	LRESULT OnKeyDownList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	
	LRESULT OnLButtonUpList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDownButton(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
private:
	void DrawListItem(LPDRAWITEMSTRUCT di);	
	void DrawPalette(LPDRAWITEMSTRUCT di);			
	int GetPaletteItemRect(int nIdx, RECT* rc);	
	int HitTestPalette(POINT p);		
	int SetStartColor(OLE_COLOR clr);	
//	void Close();

	void InitCommonControl ()
	{
		static bool b = true;
		if (b) {
			INITCOMMONCONTROLSEX icc = {sizeof INITCOMMONCONTROLSEX, ICC_TAB_CLASSES};
			InitCommonControlsEx(&icc);
			b = false;
		}
	}
};

#endif //__COLORPICKER_H_
