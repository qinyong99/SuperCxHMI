// CustCtrl.h: interface for the CCustCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTCTRL_H__F0A2919C_C191_4E47_B258_1B95B384F6ED__INCLUDED_)
#define AFX_CUSTCTRL_H__F0A2919C_C191_4E47_B258_1B95B384F6ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "commctrl.h"

#include "CxDispid.h"
#include "CxHmiObj.h"
#include "CxHmiMisc.h"
#include "CxDrawObj.h"

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

////////////////////////////////////////////////////////////////
//功能描述: 对象预览按钮
////////////////////////////////////////////////////////////////
class CPreViewWindow : CWindowImpl<CPreViewWindow>
{
	HWND m_hWnd;
	IUnknown* m_pUnkObject;

	//消息映射
	BEGIN_MSG_MAP(CPreViewWindow)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	CPreViewWindow() : m_hWnd(NULL)
	{
	}

	/////////////////////////////////////////////////////
	//功能: 与窗口控件子类化并初始化参数
	//参数: hWnd[in] 控件父窗口的句柄
	//		nID[in] 控件ID
	//		IUnknown[in] 预览对象的接口
	/////////////////////////////////////////////////////
	BOOL LinkTo(HWND hWnd, WORD nID, IUnknown* pUnk)
	{
		m_hWnd = ::GetDlgItem(hWnd, nID);
		m_pUnkObject = pUnk;
		if (!m_hWnd)
			return FALSE;
		return SubclassWindow(m_hWnd);
	}
	

	//////////////////////////////////////////////////////////////////////
	//功能: 控件WM_PAINT消息响应函数
	//////////////////////////////////////////////////////////////////////
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		RECT rcObject, rcWindow;
		PAINTSTRUCT paint;
		SIZE OldViewSize, OldWindowSize;
		POINT OldOrg, OldViewOrg;
		CComQIPtr<ICxDrawObject, &IID_ICxDrawObject> pPreView(m_pUnkObject);
		pPreView->GetBoundingRect(&rcObject);
		::GetClientRect(m_hWnd, &rcWindow);
		HDC hdc = ::BeginPaint(m_hWnd, &paint);
		int OldMode = ::SetMapMode(hdc, MM_ANISOTROPIC);
		float nScale = min(float(rcWindow.right - rcWindow.left) / float(rcObject.right - rcObject.left), 
						   float(rcWindow.bottom - rcWindow.top) / float(rcObject.bottom - rcObject.top));
		::SetWindowExtEx(hdc, rcObject.right - rcObject.left, rcObject.bottom - rcObject.top, &OldWindowSize);
		::SetViewportExtEx(hdc, int((rcObject.right - rcObject.left) * nScale), int((rcObject.bottom - rcObject.top) * nScale), &OldViewSize);
		::SetViewportOrgEx(hdc, rcWindow.left + (rcWindow.right - rcWindow.left) / 2, rcWindow.top + (rcWindow.bottom - rcWindow.top) /2, &OldViewOrg);
		::SetWindowOrgEx(hdc, rcObject.left + (rcObject.right - rcObject.left) / 2, rcObject.top + (rcObject.bottom - rcObject.top) / 2, &OldOrg);
		pPreView->Draw(hdc);
		::SetWindowExtEx(hdc, OldWindowSize.cx, OldWindowSize.cy, NULL);
		::SetWindowOrgEx(hdc, OldOrg.x, OldOrg.y, NULL);
		::SetViewportExtEx(hdc, OldViewSize.cx, OldViewSize.cy, NULL);
		::SetViewportOrgEx(hdc, OldViewOrg.x, OldViewOrg.y, NULL);
		::SetMapMode(hdc, OldMode);
		::EndPaint(m_hWnd, &paint);
		return 0;
	}
};

class CIconCombo : CWindowImpl<CIconCombo>
{
	//组合框行元素
	class ComboItem
	{
	public:
		TCHAR* m_pchText;
		int m_nImage;
		ComboItem(const TCHAR*	pchText, int nImage)
		{
			int nTextLength = ::lstrlen(pchText) + 1;
			m_pchText = new TCHAR[nTextLength];
			::lstrcpyn(m_pchText, pchText, nTextLength);
			m_nImage = nImage;
		}
		
		~ComboItem()
		{
			delete[] m_pchText;
		}
	};
	
	HWND m_hWndCombo;
	//组合框元素集
	CSimpleValArray<ComboItem*> m_Items;
	//图标集
	HIMAGELIST m_hImageList;


	BEGIN_MSG_MAP(CIconCombo)
		MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem)
	END_MSG_MAP()
		
	
	CIconCombo() : m_hImageList(NULL)
	{
		for (int i = 0; i < m_Items.m_nSize; i++)
			delete m_Items[i];
	}
	
	BOOL LinkTo(HWND hWnd, WORD nID, WORD nIDofImageList)
	{
		m_hWndCombo = ::GetDlgItem(hWnd, nID);
		m_hImageList = ::ImageList_LoadImage(_Module.m_hInstResource, MAKEINTRESOURCE(nIDofImageList), 16, 5, RGB(255, 255, 255), IMAGE_BITMAP, 0);
		if (m_hWndCombo == NULL)
			return FALSE;
		return SubclassWindow(m_hWndCombo);
	}
	
	void AddItem(const TCHAR* pchText, int nImage)
	{
		m_Items.Add(new ComboItem(pchText, nImage));
		int nIndex = SendMessage(m_hWndCombo, CB_ADDSTRING, 0, (LPARAM)_T(""));
		SendMessage(m_hWndCombo, CB_SETITEMDATA, nIndex, m_Items.GetSize() - 1);
		SendMessage(m_hWndCombo, CB_SETCURSEL, 0, 0);
	}
	
	/////////////////////////////////////////////////////////////////////////////
	//功能: 重设行元素值
	//参数: nIndex [in] 当前行0基值序号
	//		pchText [in] 行元素字串
	//		nImage [in] 图标0基值序号
	////////////////////////////////////////////////////////////////////////////
	void ReSetItem(int nIndex, TCHAR* pchText, int nImage)
	{
		ATLASSERT(nIndex >= 0 && nIndex < m_Items.m_nSize);
		m_Items[nIndex]->m_pchText = pchText;
		m_Items[nIndex]->m_nImage = nImage;
	}
	
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		COLORREF clrBack,clrFore,clrHlBack,clrHlFore; 
		OLE_COLOR clrBackColor = COLOR_WINDOW | 0x80000000;
		OLE_COLOR clrForeColor = COLOR_WINDOWTEXT | 0x80000000;
		OLE_COLOR clrHighliteBack = COLOR_HIGHLIGHT | 0x80000000;
		OLE_COLOR clrHighliteFore = COLOR_HIGHLIGHTTEXT | 0x80000000;
		OleTranslateColor(clrBackColor, 0, &clrBack);
		OleTranslateColor(clrForeColor, 0, &clrFore);
		OleTranslateColor(clrHighliteBack, 0, &clrHlBack);
		OleTranslateColor(clrHighliteFore, 0, &clrHlFore);
		
		DRAWITEMSTRUCT* pItemStruct = (DRAWITEMSTRUCT*)lParam;
		HDC hdc = pItemStruct->hDC;
		RECT rcItem = pItemStruct->rcItem;
		RECT rcTemp = rcItem;
		
		TCHAR* pItemText = m_Items[pItemStruct->itemData]->m_pchText;
		int nImage = m_Items[pItemStruct->itemData]->m_nImage;
		
		if (m_hImageList)
		{
			//设置背景颜色
			rcTemp.right = rcTemp.left + rcTemp.bottom - rcTemp.top;
			::SetBkColor(hdc, clrBack);
			::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcTemp, 0, 0, 0);

			//::InflateRect(&rcTemp, -1, -1);
			
			int ix = ::GetSystemMetrics(SM_CXSMICON);
			int iy = ::GetSystemMetrics(SM_CYSMICON);
			int cx = rcTemp.right - rcTemp.left;
			int cy = rcTemp.bottom - rcTemp.top;
			int dx = ix < cx ? ix : cx;
			int dy = iy < cy ? iy : cy;
			int x = max(rcTemp.left, (cx - dx) / 2 + rcTemp.left + 1);
			int y = max(rcTemp.top, (cy - dy) / 2 + rcTemp.top + 1);

			if (nImage != -1)
				::ImageList_DrawEx(m_hImageList, nImage, hdc, x, y, 0/*dx*/, 0/*dy*/, CLR_NONE, CLR_DEFAULT, 
				(pItemStruct->itemState & ODS_SELECTED ? ILD_SELECTED : ILD_NORMAL));
			rcTemp = rcItem;
			rcTemp.left += rcTemp.bottom - rcTemp.top;
		}
		
		::SetTextColor(hdc, pItemStruct->itemState & ODS_SELECTED ? clrHlFore : clrFore);
		::SetBkColor(hdc, pItemStruct->itemState &	ODS_SELECTED ? clrHlBack : clrBack);
		::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcTemp, 0, 0, 0);
		::SetBkMode(hdc, TRANSPARENT);
		::DrawText(hdc, pItemText, ::lstrlen(pItemText), &rcTemp, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
		return 0;
	}

	void DisEnable()
	{
//		::SendMessage(m_hWndCombo, WM_ENABLE, false, 0);
		::EnableWindow(m_hWndCombo, false);
	}

	void Enable()
	{
//		::SendMessage(m_hWndCombo, WM_ENABLE, true, 0);
		::EnableWindow(m_hWndCombo, true);
	}

	int GetCurSel()
	{
		return ::SendMessage(m_hWndCombo, CB_GETCURSEL, 0, 0);
	}

	void SetCurSel(int nIndex)
	{
		::SendMessage(m_hWndCombo, CB_SETCURSEL, nIndex, 0);
	}

};

class COwnerDrawCombo : CWindowImpl<COwnerDrawCombo>
{

public:
	typedef enum
	{
		LINETYPE = 1,	//线型组合框
		FILLTYPE = 2	//填充组合框
	} DRAWTYPE;

	HWND m_hWndCombo;
	DRAWTYPE m_enumDrawType;
	
	BEGIN_MSG_MAP(COwnerDrawCombo)
		MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem)
	END_MSG_MAP()

	COwnerDrawCombo()
	{
		m_hWndCombo = NULL;
	}

	/////////////////////////////////////////////////////////////////////////////////
	//功能: 与对话框上的控件连接并初始化参数
	//参数: hWnd[in] 控件所在对话框的窗口句柄
	//		nID[in] 控件ID
	//		enumDrawType[in] 组合框自画类型
	/////////////////////////////////////////////////////////////////////////////////
	BOOL LinkTo(HWND hWnd, WORD nID, DRAWTYPE enumDrawType) 
	{
		m_enumDrawType = enumDrawType;
		m_hWndCombo = ::GetDlgItem(hWnd, nID);
		if (m_hWndCombo == NULL)
			return FALSE;
		if (SubclassWindow(m_hWndCombo) == FALSE)
			return FALSE;

		int nIndex = 0;
		if (m_enumDrawType == LINETYPE)
		{
			//共5种线型,见CxOle.idl中的定义
			for (int i = 0; i < 7; i++)
			{
				nIndex = ::SendMessage(m_hWndCombo, CB_ADDSTRING, 0, (LPARAM)NULL);
				::SendMessage(m_hWndCombo, CB_SETITEMDATA, nIndex, (enumEdgeStyle)i);
				
			}
			::SendMessage(m_hWndCombo, CB_SETCURSEL, 0, 0);
		}
		else if (m_enumDrawType == FILLTYPE)
		{
			//共7种填充类型,见CxOle.idl中的定义
			for (int i = 0; i < 23; i++)
			{
				nIndex = SendMessage(m_hWndCombo, CB_ADDSTRING, 0, (LPARAM)NULL);
				SendMessage(m_hWndCombo, CB_SETITEMDATA, nIndex, (enumFillStyle)i);
			}
			SendMessage(m_hWndCombo, CB_SETCURSEL, 0, 0);
		}

		return TRUE;
	}

	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		DRAWITEMSTRUCT* pItemStruct = (DRAWITEMSTRUCT*)lParam;
		HDC hdc = pItemStruct->hDC;
		RECT rcItem = pItemStruct->rcItem;

		BOOL bSelected = ((pItemStruct->itemState & ODS_SELECTED) != 0);
		::SetBkColor(hdc, bSelected ? 
				GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_WINDOW));
		::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcItem, 0, 0, 0);
	
		CCxDrawObjectBase cBaseTool;
		cBaseTool.m_enumLineJoinStyle = LineJoinStyleSquare;
		cBaseTool.m_nHorFillPercent = 100;									//水平填充度为100%
		cBaseTool.m_nVerFillPercent = 100;	
		cBaseTool.m_pObjectPath = NULL;
		cBaseTool.m_clrFillColor = RGB(0, 0, 0);
		cBaseTool.m_clrEdgeColor = RGB(0, 0, 0);
		cBaseTool.m_clrHighLightEdge = RGB(255, 255, 255);
		cBaseTool.m_clrHighLightFill = RGB(255, 255, 255);

		if (m_enumDrawType == LINETYPE)
		{	
			using namespace Gdiplus;

			cBaseTool.m_enumEdgeStyle = (enumEdgeStyle)pItemStruct->itemID;
			if (cBaseTool.m_enumEdgeStyle == EdgeStylePipe)
			{
				cBaseTool.m_fEdgeWidth = 12;
			}
			else
			{
				cBaseTool.m_clrEdgeColor = bSelected ? 
					GetSysColor(COLOR_HIGHLIGHTTEXT) : GetSysColor(COLOR_WINDOWTEXT);
				cBaseTool.m_fEdgeWidth = 2;
			}

			GraphicsPath path;
			path.AddLine(rcItem.left + 4, (rcItem.top + rcItem.bottom) / 2, 
					rcItem.right - 4, (rcItem.top + rcItem.bottom) / 2);	

			Graphics graphics(hdc);
			cBaseTool.DrawObjectEdge(&graphics, &path);
		}
		else if (m_enumDrawType == FILLTYPE)
		{
			using namespace Gdiplus;

			cBaseTool.m_enumFillStyle = (enumFillStyle)pItemStruct->itemID;

			if (cBaseTool.m_enumFillStyle < 8 || cBaseTool.m_enumFillStyle > 21)
				cBaseTool.m_clrFillColor = bSelected ? 
						GetSysColor(COLOR_HIGHLIGHTTEXT) : GetSysColor(COLOR_WINDOWTEXT);

			GraphicsPath path;
			InflateRect(&rcItem, -12, -1);
			Rect rect((rcItem.right + rcItem.left) /  2 - 8, rcItem.top, 
				16, rcItem.bottom - rcItem.top);
				
			path.AddRectangle(rect);	

			Graphics graphics(hdc);
			cBaseTool.FillObjectRgn(&graphics, &path);
		}

		return 0;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//功能: 设置当前填充类型
	/////////////////////////////////////////////////////////////////////////////////////////
	void SetCurrType(enumFillStyle CurrFillStyle)
	{
		enumFillStyle FillStyle;
		int nCount = ::SendMessage(m_hWndCombo, CB_GETCOUNT, 0, 0);
		for (int i = 0; i < nCount; i++)
		{
			 FillStyle = (enumFillStyle)::SendMessage(m_hWndCombo, CB_GETITEMDATA, i, 0);
			 if (FillStyle == CurrFillStyle)
			 {
				 ::SendMessage(m_hWndCombo, CB_SETCURSEL, i, 0);
				 break;
			 }
		}
	}

	void SetCurSel(int nIndex)
	{
		::SendMessage(m_hWndCombo, CB_SETCURSEL, nIndex, 0);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////
	//功能: 设置当前边界类型
	//////////////////////////////////////////////////////////////////////////////////////////
	void SetCurrType(enumEdgeStyle CurrEdgeStyle)
	{
		enumEdgeStyle EdgeStyle;
		int nCount = ::SendMessage(m_hWndCombo, CB_GETCOUNT, 0, 0);
		for (int i = 0; i < nCount; i++)
		{
			 EdgeStyle = (enumEdgeStyle)::SendMessage(m_hWndCombo, CB_GETITEMDATA, i, 0);
			 if (EdgeStyle == CurrEdgeStyle)
			 {
				 ::SendMessage(m_hWndCombo, CB_SETCURSEL, i, 0);
				 break;
			 }
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	//功能: 取当前类型值
	/////////////////////////////////////////////////////////////////////////////////////////
	int GetCurSel()
	{
		int nIndex = ::SendMessage(m_hWndCombo, CB_GETCURSEL, 0, 0);
		return ::SendMessage(m_hWndCombo, CB_GETITEMDATA, nIndex, 0);
	}

	void DisEnable()
	{
		::EnableWindow(m_hWndCombo, false);
	}

	void Enable()
	{
		::EnableWindow(m_hWndCombo, true);
	}

};

class CRestrictEdit : public CWindowImpl<CRestrictEdit>
{
	//重载WM_CHAR消息
	BEGIN_MSG_MAP(RestrictEdit)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
	END_MSG_MAP()
		
public:
	//接受的文本类型
	typedef enum {
		TYPE_NUMBER,
			TYPE_CHAR
	} ValueType;
	
	CRestrictEdit()
	{
		m_bIsSetRange = false;
	}
	
private:
	ValueType m_nValueType;
	bool m_bIsSetRange;
	long m_nMinValue;
	long m_nMaxValue;
	HWND m_hWndEdit;
	
public:
	//////////////////////////////////////////////////////////////
	//功能: 与EDIT控件资源连接
	//参数: [in] HWND hWndDlg :EDIT控件所在的对话框窗口积句柄
	//		[in] int nID :EDIT控件的资源ID
	//////////////////////////////////////////////////////////////
	BOOL CRestrictEdit::LinkTo(HWND hWndDlg, int nID)
	{
		m_hWndEdit = ::GetDlgItem(hWndDlg, nID);
		if (m_hWndEdit == NULL)
			return FALSE;
		return SubclassWindow(m_hWndEdit);
	}
	
	LRESULT CRestrictEdit::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		
		switch(m_nValueType)
		{
		case TYPE_NUMBER:
			if (wParam == VK_BACK || (wParam >= 0x30 && wParam <= 0x39))
			{
				CComVariant temp = GetText();
				DefWindowProc(uMsg, wParam, lParam); 
				if (InValidValue())
					SetText(temp);
				
			}
			else
				MessageBeep(MB_OK);
			break;
			
		case TYPE_CHAR:
			DefWindowProc(uMsg, wParam, lParam);
			break;
			
		default:
			break;
		}
		
		return S_OK;
	}
	
	/////////////////////////////////////////////////////////////
	//功能: 设置EDIT控件接受输入值类型
	//参数: [in] int nType :输入类型
	////////////////////////////////////////////////////////////
	HRESULT CRestrictEdit::SetValueType(ValueType nType)
	{
		ATLASSERT(nType == TYPE_NUMBER || nType == TYPE_CHAR);
		m_nValueType = nType;
		return S_OK;
	}

	/////////////////////////////////////////////////////////////
	//功能: 设置数值输入范围
	//参数: [in] long nMinValue :范围最小值
	//		[in] long nMaxValue :范围最大值
	///////////////////////////////////////////////////////////////
	HRESULT CRestrictEdit::SetValueRange(long nMinValue, long nMaxValue)
	{
		m_nMinValue = nMinValue;
		m_nMaxValue = nMaxValue;
		m_bIsSetRange = true;
		for (int len = 1; nMaxValue >= 10; nMaxValue /= 10, len++);
		SetLimitText(len);
		return S_OK;
	}
	
	////////////////////////////////////////////////////////////////
	//功能: 设置输字符最大长度
	//参数: [in] long nLimitLength :最大长度 
	///////////////////////////////////////////////////////////////
	HRESULT CRestrictEdit::SetLimitText(long nLimitLength)
	{
		
		ATLASSERT(::IsWindow(m_hWndEdit));
		SendMessage(m_hWndEdit, EM_LIMITTEXT, (WPARAM)nLimitLength, 0);
		return S_OK;
	}
	
	/////////////////////////////////////////////////////////////////////
	//功能: 获取EDIT控件中的文本
	//参数: [in] 无
	//		[retval] CComVariant :返回含文本的VARIANTARG变量
	//////////////////////////////////////////////////////////////////////
	CComVariant CRestrictEdit::GetText()
	{
		ATLASSERT(::IsWindow(m_hWndEdit));
		CComVariant varText;
		if (varText.vt != VT_BSTR)
			varText.ChangeType(VT_BSTR);
		long nTextLength = ::SendMessage(m_hWndEdit, WM_GETTEXTLENGTH, 0, 0) + 1;
		TCHAR* tchar = new TCHAR[nTextLength];
		::SendMessage(m_hWndEdit, WM_GETTEXT, (WPARAM)nTextLength, (LPARAM)tchar);
		varText = tchar;
		delete[] tchar;
		return varText;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//功能: 设置EDIT控件中的文本
	//参数: [in] CComVariant var :含文字串
	/////////////////////////////////////////////////////////////////////////
	HRESULT CRestrictEdit::SetText(CComVariant var)
	{
		ATLASSERT(::IsWindow(m_hWndEdit));
		if (var.vt == VT_NULL)
			return S_FALSE;
		USES_CONVERSION;
		if (var.vt != VT_BSTR)
			var.ChangeType(VT_BSTR);
		::SendMessage(m_hWndEdit, WM_SETTEXT, 0, (LPARAM)(W2T(var.bstrVal)));
		return S_OK;
	}
	
	///////////////////////////////////////////////////////////////////////
	//功能: 判定EDIT中的文本是否为合符设定规则的有效文本
	//参数: [in] 无
	//      [reval] bool :无效为true,有效为false
	///////////////////////////////////////////////////////////////////////
	bool CRestrictEdit::InValidValue()
	{
		if (m_nValueType == TYPE_NUMBER)
		{
			if (m_bIsSetRange == false)
				return false;
			CComVariant var;
			var = GetText();
			if (var.vt != VT_NULL)
				var.ChangeType(VT_I4);
			if (var.vt == VT_I4 && (var.lVal < m_nMinValue || var.lVal > m_nMaxValue))
				return true;
		}
		return false;
	}
	
	void CRestrictEdit::DisEnable()
	{
		::SendMessage(m_hWndEdit, WM_ENABLE, false, 0);
	}
	
	void CRestrictEdit::Enable()
	{
		::SendMessage(m_hWndEdit, WM_ENABLE, true, 0);
	}
};

#endif // !defined(AFX_CUSTCTRL_H__F0A2919C_C191_4E47_B258_1B95B384F6ED__INCLUDED_)
