// ExprCrt.cpp : Implementation of CExprCrt
#include "stdafx.h"
#include "CxScripCrt.h"
#include "ExprCrt.h"
#include "../CxTagPick/CxTagPick_i.c"

/////////////////////////////////////////////////////////////////////////////
// CExprCrt

void CExprCrt::DeflateWindow()
{
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	SetWindowPos(NULL, rcWindow.left, rcWindow.top, rcWindow.Width() - 100, rcWindow.Height() - 50, SWP_NOMOVE | SWP_NOZORDER);

	HWND hWnd = GetDlgItem(IDC_EDITEXPR);
	::GetWindowRect(hWnd, &rcWindow);
	ScreenToClient(&rcWindow);
	::SetWindowPos(hWnd, NULL, rcWindow.left, rcWindow.top - 50, rcWindow.Width() - 100, rcWindow.Height(), SWP_NOZORDER);

	hWnd = GetDlgItem(IDC_CLEAR_EDIT);
	::GetWindowRect(hWnd, &rcWindow);
	ScreenToClient(&rcWindow);
	::SetWindowPos(hWnd, NULL, rcWindow.left, rcWindow.top - 50, rcWindow.Width(), rcWindow.Height(), SWP_NOSIZE | SWP_NOZORDER);

	hWnd = GetDlgItem(IDOK);
	::GetWindowRect(hWnd, &rcWindow);
	ScreenToClient(&rcWindow);
	::SetWindowPos(hWnd, NULL, rcWindow.left - 100, rcWindow.top - 50, rcWindow.Width(), rcWindow.Height(), SWP_NOSIZE | SWP_NOZORDER);

	hWnd = GetDlgItem(IDCANCEL);
	::GetWindowRect(hWnd, &rcWindow);
	ScreenToClient(&rcWindow);
	::SetWindowPos(hWnd, NULL, rcWindow.left - 100, rcWindow.top - 50, rcWindow.Width(), rcWindow.Height(), SWP_NOSIZE | SWP_NOZORDER);

	hWnd = GetDlgItem(IDC_SHOW_SYMBOL);
	::GetWindowRect(hWnd, &rcWindow);
	ScreenToClient(&rcWindow);
	::SetWindowPos(hWnd, NULL, rcWindow.left - 100, rcWindow.top - 50, rcWindow.Width(), rcWindow.Height(), SWP_NOSIZE | SWP_NOZORDER);
}

LRESULT CExprCrt::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	if (!m_bUseTagSymbol)
		DeflateWindow();

	m_editExprssion.Attach(GetDlgItem(IDC_EDITEXPR));

	CRect rcEdit;
	m_editExprssion.GetWindowRect(&rcEdit);
	ScreenToClient(&rcEdit);

	// 创建左右分割条
	CRect rcClient;
	GetClientRect(&rcClient);

	m_winSplitter.Create(m_hWnd, rcClient, "", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP, WS_EX_CONTROLPARENT);
	rcClient.InflateRect(-8, -8, -8, -40);
	m_winSplitter.SetSplitterRect(&rcClient);
	m_winSplitter.SetSplitterExtendedStyle(SPLIT_NONINTERACTIVE);
	
	// 创建左面板
	m_paneLeft.Create(m_winSplitter, "", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP, WS_EX_CONTROLPARENT);
	m_paneLeft.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
	m_winLeftPane.SubclassWindow(m_paneLeft);

	// 创建符号面板
	m_paneSymbol.Create(m_winSplitter, "", WS_CHILD | WS_VISIBLE /*| WS_CLIPCHILDREN | WS_CLIPSIBLINGS*/ | WS_TABSTOP, WS_EX_CONTROLPARENT);
	m_paneSymbol.SetTitle(_T("符号"));
	
	// 创建符号视图
	m_viewSymbol.Create(m_paneSymbol, rcDefault, NULL, 
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		WS_EX_CLIENTEDGE | WS_EX_CONTROLPARENT, 101);
	m_viewSymbol.SetNotifyWindow(m_hWnd);
	m_paneSymbol.SetClient(m_viewSymbol);
 	m_paneSymbol.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);

	m_winSplitter.SetSplitterPanes(m_paneLeft, m_paneSymbol);
	m_winSplitter.SetSplitterPos(rcEdit.Width());

	m_paneLeft.GetClientRect(&rcClient);
	m_winHorSplitter.Create(m_paneLeft, rcClient, "", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP, WS_EX_CONTROLPARENT);
	m_winHorSplitter.SetSplitterRect(&rcClient);
//	m_paneLeft.SetClient(m_winHorSplitter);

	if (m_bUseTagSymbol)
	{
		m_wndTagPickCtrl.Create(m_winHorSplitter, rcClient, _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP, WS_EX_CONTROLPARENT);

		m_spTagPickCtrlObject.CoCreateInstance(CLSID_CxTagPickCtrl);
		CComPtr<IAxWinHostWindow> spContainer;
		m_wndTagPickCtrl.QueryHost(&spContainer);
		spContainer->AttachControl(m_spTagPickCtrlObject, m_wndTagPickCtrl);
		m_wndTagPickCtrl.SetParent(m_winLeftPane);

		CComQIPtr<IAxWinAmbientDispatch> spDispatch = spContainer;
		spDispatch->put_BackColor((OLE_COLOR)GetSysColor(COLOR_3DFACE));
	}
	else
	{
		if (!m_pTreePage)
		{
			m_pTreePage = new CTreePage;
			m_pTreePage->AddRef();
		}
			
		m_ImageList.Create(16, 16, ILC_MASK, 2, 2);

		ImageList_AddIcon(m_ImageList, AtlLoadIcon(IDI_OBJECT));
		ImageList_AddIcon(m_ImageList, AtlLoadIcon(IDI_PROP));
		ImageList_AddIcon(m_ImageList, AtlLoadIcon(IDI_METHOD));

		m_pTreePage->m_bDblClickSelect = FALSE;
		m_pTreePage->m_hNotifyWindow = m_hWnd;
		m_pTreePage->m_bWithList = true;
		m_pTreePage->m_bDisplayIcon = true;
		m_pTreePage->m_bObjectHiberarchy = FALSE;

		m_pTreePage->Create(m_winHorSplitter);
		m_pTreePage->OnInit();
		m_pTreePage->ShowWindow(SW_SHOW);

		if (m_pTreePage->m_bDisplayIcon)
		{
			m_pTreePage->m_tree.SetImageList((HIMAGELIST)m_ImageList, TVSIL_NORMAL);
			m_pTreePage->m_list.SetImageList((HIMAGELIST)m_ImageList, LVSIL_SMALL);
		}

	}

	// 创建下面板
	m_paneBottom.Create(m_winHorSplitter, "", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP, WS_EX_CONTROLPARENT);
	m_paneBottom.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
	m_winBottomPane.SubclassWindow(m_paneBottom);

	m_winHorSplitter.SetSplitterPanes(m_spTagPickCtrlObject != NULL ? m_wndTagPickCtrl : m_pTreePage->m_hWnd, m_paneBottom);
	m_winHorSplitter.SetSplitterPos(rcClient.Height() - rcEdit.Height() - 1);

	m_paneBottom.GetClientRect(&rcClient);
	m_winBottomSplitter.Create(m_paneBottom, rcClient, "", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP, WS_EX_CONTROLPARENT);
	m_winBottomSplitter.SetSplitterRect(&rcClient);

	// 创建表达式面板
	m_paneExprssion.Create(m_winBottomSplitter, "", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP, WS_EX_CONTROLPARENT);
	m_paneExprssion.SetTitle(_T("表达式"));
	m_paneExprssion.SetClient(m_editExprssion);
	m_paneExprssion.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
	m_editExprssion.SetParent(m_paneExprssion);

	m_dlgKeyBoard.Create(m_winBottomSplitter);
	m_dlgKeyBoard.SetTextOutWindow(m_editExprssion);
	m_dlgKeyBoard.ShowWindow(SW_SHOW);

	m_winBottomSplitter.SetSplitterPanes(m_paneExprssion, m_dlgKeyBoard);
	m_winBottomSplitter.SetSplitterPos(m_bUseTagSymbol ? 410 : 310);

	if (m_spTagPickCtrlObject)
	{	
		DispEventAdvise(m_spTagPickCtrlObject);

		CComVariant var;
		CComDispatchDriver dd(m_spTagPickCtrlObject);

		var = m_bstrExpression;
		dd.PutPropertyByName(L"Value", &var);

		var = m_bUseLocalSetup ? true : false;
		dd.PutPropertyByName(L"UseLocalSetup", &var);

	//	var = m_strAssocTagGroup;
	//	dd.PutPropertyByName(L"AssocGroup", &var);

	//	var = m_bDisableAssoc ? true : false;
	//	dd.PutPropertyByName(L"DisableAssoc", &var);

		var = true;
		dd.PutPropertyByName(L"Browsed", &var);
	}

	m_editExprssion.SetWindowText(W2T(m_bstrExpression));
	m_editExprssion.SetSel(0, -1);

	return 1;
}
	
LRESULT CExprCrt::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_editExprssion.GetWindowText((BSTR&)m_bstrExpression);

	EndDialog(wID);
	return 0;
}

LRESULT CExprCrt::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

STDMETHODIMP CExprCrt::ShowDialog()
{
	DoModal();

	return S_OK;
}

STDMETHODIMP CExprCrt::get_UseLocalSetup(BOOL *pVal)
{
	*pVal = m_bUseLocalSetup;

	return S_OK;
}

STDMETHODIMP CExprCrt::put_UseLocalSetup(BOOL newVal)
{
	m_bUseLocalSetup = newVal;

	return S_OK;
}

STDMETHODIMP CExprCrt::get_Expression(BSTR *pVal)
{
	*pVal = m_bstrExpression.Copy();

	return S_OK;
}

STDMETHODIMP CExprCrt::put_Expression(BSTR newVal)
{
	m_bstrExpression = newVal;

	return S_OK;
}

LRESULT CExprCrt::OnLeftPaneSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CRect rcClient;
	m_paneLeft.GetClientRect(rcClient);
	
	if (::IsWindow(m_winHorSplitter.m_hWnd))
	{
		m_winHorSplitter.SetWindowPos(NULL, rcClient.left, rcClient.top,
			rcClient.Width(), rcClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		m_winHorSplitter.SetSplitterRect(&rcClient);
	}
		
	bHandled = FALSE;
	
	return 1;
}

LRESULT CExprCrt::OnBottomPaneSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CRect rcClient;
	m_paneBottom.GetClientRect(rcClient);
	
	if (::IsWindow(m_winBottomSplitter.m_hWnd))
	{
		m_winBottomSplitter.SetWindowPos(NULL, rcClient.left, rcClient.top,
			rcClient.Width(), rcClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		m_winBottomSplitter.SetSplitterRect(&rcClient);

		m_dlgKeyBoard.Invalidate();
	}
		
	bHandled = FALSE;
	
	return 1;
}

LRESULT CExprCrt::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsWindow(m_winSplitter.m_hWnd))
	{
		CRect rcClient;
		GetClientRect(rcClient);
		m_winSplitter.SetWindowPos(NULL, rcClient.left, rcClient.top,
			rcClient.Width(), rcClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		rcClient.InflateRect(-8, -8, -8, -40);
		m_winSplitter.SetSplitterRect(&rcClient);
	}

	return 1;
}

LRESULT CExprCrt::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_pTreePage)
		m_pTreePage->Release();

	m_spTagPickCtrlObject.Release();
	return 0;
}

void CExprCrt::ReplaceLastSelect(LPCTSTR pszText)
{
	CString strText;
	int nTextLen = m_editExprssion.GetWindowTextLength();
	m_editExprssion.GetWindowText(strText.GetBuffer(nTextLen + 1), nTextLen + 1);
	strText.ReleaseBuffer();

	int nStartChar = nTextLen;
	int nEndChar = nTextLen;
	m_editExprssion.GetSel(nStartChar, nEndChar);
	if (nStartChar == nEndChar && nStartChar == nTextLen)
	{
		int nLen = m_strLastSelectTag.GetLength();
		if (m_strLastSelectTag.GetLength() > 0 && m_strLastSelectTag == strText.Right(nLen))
			m_editExprssion.SetSel(nTextLen - nLen, nTextLen);
	}

	m_strLastSelectTag = pszText;

	m_editExprssion.ReplaceSel(m_strLastSelectTag);
}

HRESULT CExprCrt::OnChanged()
{
	USES_CONVERSION;

	CComDispatchDriver dd(m_spTagPickCtrlObject);
	CComVariant var;
	dd.GetPropertyByName(L"Value", &var);

	ReplaceLastSelect(OLE2T(var.bstrVal));

	return 0;
}

LRESULT CExprCrt::OnSymbolSelect(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CString strText;
	HWND hWndFrom = *(HWND*)lParam;

	if (m_pTreePage && hWndFrom == m_pTreePage->m_hWnd)
	{
		strText = m_pTreePage->GetSelectNameSpace();
		ReplaceLastSelect(strText);
	}
	else 
	{
		CTreePage* pTreePage = m_viewSymbol.GetCurrentTreePage();

		strText = pTreePage->GetSelectNameSpace();
		m_editExprssion.ReplaceSel(strText);
		m_editExprssion.SetFocus();
	}

	return 0;
}

STDMETHODIMP CExprCrt::AddGeneralSymbolPage(DWORD dwSymbolMask)
{
	m_viewSymbol.AddGeneralPage(dwSymbolMask, m_hWnd);

	return S_OK;
}

STDMETHODIMP CExprCrt::AddOPCServerSymbolPage()
{
	m_viewSymbol.AddOPCServerPage(m_hWnd);

	return S_OK;
}

STDMETHODIMP CExprCrt::AddSymbolTreePage(BSTR bstrCaption, /*[out, retval]*/ ICxSymbolTreePage** ppPage)
{
	USES_CONVERSION;
	CTreePage* pTreePage = m_viewSymbol.AddTreePage(OLE2T(bstrCaption), TRUE, m_hWnd);
	if (ppPage != NULL)
		return pTreePage->QueryInterface(IID_ICxSymbolTreePage, (void**)ppPage);

	return S_OK;
}

STDMETHODIMP CExprCrt::get_UseTagSymbol(/*[out, retval]*/ BOOL *pVal)
{
	*pVal = m_bUseTagSymbol;
	return S_OK;
}

STDMETHODIMP CExprCrt::put_UseTagSymbol(/*[in]*/ BOOL newVal)
{
	m_bUseTagSymbol = newVal;
	return S_OK;
}

STDMETHODIMP CExprCrt::GetMainSymbolTreePage(/*[out, retval]*/ ICxSymbolTreePage** ppPage)
{
	if (!m_pTreePage)
	{
		m_pTreePage = new CTreePage;
		m_pTreePage->AddRef();
	}

	if (ppPage != NULL)
		return m_pTreePage->QueryInterface(IID_ICxSymbolTreePage, (void**)ppPage);

	return S_FALSE;
}

LRESULT CExprCrt::OnClickedShowSymbol(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	LockWindowUpdate(TRUE);

	int nPane = m_winSplitter.GetSinglePaneMode();
	BOOL b = m_winSplitter.GetSinglePaneMode() == 0;

	m_winSplitter.SetSinglePaneMode(b ? SPLIT_PANE_NONE : 0);

	CRect rcWindow;
	GetWindowRect(&rcWindow);
	int nWidth = rcWindow.Width() + (b ? 182 : -182);
	SetWindowPos(NULL, 0, 0, nWidth, rcWindow.Height(), SWP_NOMOVE | SWP_NOZORDER);

	LockWindowUpdate(FALSE);

	SetDlgItemText(IDC_SHOW_SYMBOL, b ? _T("符号窗口 >>") : _T("符号窗口 <<"));

//	CenterWindow();

	return 0;
}

LRESULT CExprCrt::OnClickedClear(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_editExprssion.SetWindowText(_T(""));
	m_editExprssion.SetFocus();

	return 0;
}