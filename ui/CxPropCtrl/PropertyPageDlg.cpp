// PropertyPageDlg.cpp: implementation of the CPropertyPageDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PropertyPageDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPropertyPageDlg::CPropertyPageDlg()
{
	m_clsidPage = CLSID_NULL;
	m_spObject = NULL;
}

CPropertyPageDlg::CPropertyPageDlg(CLSID clsidPage, IUnknown* pUnk)
{
	m_clsidPage = clsidPage;
	m_spObject = pUnk;
}

CPropertyPageDlg::~CPropertyPageDlg()
{

}

HRESULT CPropertyPageDlg::ShowPage()
{
	HRESULT hr = m_spPropertyPage.CoCreateInstance(m_clsidPage);
	if (FAILED(hr))
		return hr;

	CComPtr<IPropertyPageSite> spPageSite;
	_InternalQueryInterface(IID_IPropertyPageSite, (void**)&spPageSite);
	m_spPropertyPage->SetPageSite(spPageSite);

	m_spPropertyPage->SetObjects(1, &m_spObject.p);
	
	PROPPAGEINFO proppageinfo;
	m_spPropertyPage->GetPageInfo(&proppageinfo);
	SIZE size = proppageinfo.size;

	// 重新定位对话框
	::SetWindowPos(m_hWnd, NULL, 0, 0, size.cx, size.cy + 70, SWP_NOZORDER|SWP_NOMOVE);
	
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = rect.left + size.cx;
	rect.bottom = rect.top + size.cy;
	m_spPropertyPage->Activate(m_hWnd, &rect, TRUE);

	// 重新定位对话框对象
	::SetWindowPos(GetDlgItem(IDOK), NULL, rect.right - 180, rect.bottom + 10, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	::SetWindowPos(GetDlgItem(IDCANCEL), NULL, rect.right - 100, rect.bottom + 10, 0, 0, SWP_NOZORDER|SWP_NOSIZE);

	m_spPropertyPage->Show(SW_SHOW);

	return S_OK;
}

LRESULT CPropertyPageDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	ShowPage();

	SetWindowText(OLE2T(m_bstrCaption));
	CenterWindow();
	
	return 1;  // Let the system set the focus
}

LRESULT CPropertyPageDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_spPropertyPage->Apply() == S_OK)
		EndDialog(IDOK);
	
	return 0;
}

LRESULT CPropertyPageDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
	
	return 0;
}

void CPropertyPageDlg::OnFinalMessage(HWND hWnd)
{
	m_spPropertyPage->Deactivate();
	m_spPropertyPage.Release();
}