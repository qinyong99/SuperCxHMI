// FontCombo.h : Declaration of the CFontCombo

#ifndef __CXFONTCOMBO_H_
#define __CXFONTCOMBO_H_

#include "resource.h"       // main symbols

// reserve lobyte for charset
#define PRINTER_FONT 0x0100
#define TT_FONT 0x0200
#define DEVICE_FONT 0x0400

typedef CContainedWindowT<CListBox> CContainedListBox;

/////////////////////////////////////////////////////////////////////////////
// CFontCombo
class ATL_NO_VTABLE CFontCombo : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFontCombo, &CLSID_CxFontCombo>,
	public IDispatchImpl<ICxFontCombo, &IID_ICxFontCombo, &LIBID_CXCOMOBJLib>,
	public CWindowImpl<CFontCombo>
{
public:
	CFontCombo() : 
		m_list(this, 1)
	{
		m_hImages = NULL;

		m_bClose = FALSE;
		m_bOked = FALSE;
	}
	
	class CFontDesc
	{
	public:
		CFontDesc()
		{
		}
		CFontDesc(const CFontDesc& fd)
		{
			InternalCopy(fd);
		}
		
		CFontDesc& operator = (CFontDesc& fd)
		{
			InternalCopy(fd);
			return *this;
		}
		
		void InternalCopy(const CFontDesc& fd)
		{
			m_strName = fd.m_strName;
			m_strScript = fd.m_strScript;
			m_nCharSet = fd.m_nCharSet;
			m_nPitchAndFamily = fd.m_nPitchAndFamily;
			m_dwFlags = fd.m_dwFlags;
		}
		
		int Compare(LPCTSTR lpszName)
		{
			return m_strName.CompareNoCase(lpszName);
		}
		
		int Compare(CFontDesc& fd)
		{
			return m_strName.CompareNoCase(fd.m_strName);
		}

		CString m_strName;
		CString m_strScript;
		BYTE m_nCharSet;
		BYTE m_nPitchAndFamily;
		DWORD m_dwFlags;
	};

DECLARE_REGISTRY_RESOURCEID(IDR_CXFONTCOMBO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFontCombo)
	COM_INTERFACE_ENTRY(ICxFontCombo)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

BEGIN_MSG_MAP(CFontCombo)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
ALT_MSG_MAP(1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUP)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
END_MSG_MAP()

// ICxFontCombo
public:
	STDMETHOD(PickFont)(long hParent, long  x, long y, long lWidth, BSTR bstrIn, BSTR* pbstrOut);
	
protected:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
protected:
	BOOL Create(HWND hWnd, HIMAGELIST hImages);
	void AdjustPosition(RECT rect);
	int Show(int x, int y, int cx, int cyItem);
	void Close(BOOL bOked);

	void EnumFontFamiliesEx(BYTE nCharSet = DEFAULT_CHARSET);
	void AddFont(ENUMLOGFONT* pelf, DWORD dwType, LPCTSTR lpszScript = NULL);

	static BOOL CALLBACK EnumFamScreenCallBackEx(
		ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType,
		LPVOID pThis);

	CSimpleArray<CFontDesc> m_arrFontDesc;
	
	BOOL m_bClose;
	BOOL m_bOked;
	HIMAGELIST m_hImages;
	CString m_strSel;

	CContainedListBox m_list;
};

#endif //__CXFONTCOMBO_H_
