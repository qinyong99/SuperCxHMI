// NumericEntryDlg.cpp: implementation of the CNumericEntryDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NumericEntryDlg.h"
#include "EventExpert.h"
#include "ActionParser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNumericEntryDlg::CNumericEntryDlg() :
	m_editMax(this, 1),
	m_editMin(this, 2)
{
	m_pExpert = NULL;
}

LRESULT CNumericEntryDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_editMax.SubclassWindow(GetDlgItem(IDC_MAX));
	m_editMax.SetWindowText(_T("65535"));
	m_editMin.SubclassWindow(GetDlgItem(IDC_MIN));
	m_editMin.SetWindowText(_T("0"));
	m_btnFetchLimit.Attach(GetDlgItem(IDC_FETCH_LIMIT));
	
	return S_OK;
}

LRESULT CNumericEntryDlg::OnChangeLimit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pExpert->OnActionScriptChanged();

	return 0;
}

LRESULT CNumericEntryDlg::OnClickedFetchLimit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pExpert->OnActionScriptChanged();

	BOOL bEnable = (m_btnFetchLimit.GetCheck() != 1);
	m_editMax.EnableWindow(bEnable);
	m_editMin.EnableWindow(bEnable);

	return 0;
}

void CNumericEntryDlg::OnShow()
{
	CActionParser parser(m_strScript);
	
	CString strRight;
	if (parser.HasAssignment(_T("DataEntry.max"), strRight))
	{
		m_editMax.SetWindowText(strRight);
	}

	if (parser.HasAssignment(_T("DataEntry.min"), strRight))
	{
		m_editMin.SetWindowText(strRight);
		m_editMin.SetSel(0, -1);
	}

	if (parser.HasAssignment(_T("DataEntry.fetch"), strRight))
	{
		BOOL bCheck = (strRight.CompareNoCase(_T("true")) == 0);
		m_btnFetchLimit.SetCheck(bCheck ? 1 : 0);
		m_editMax.EnableWindow(!bCheck);
		m_editMin.EnableWindow(!bCheck);
	}
}

void CNumericEntryDlg::OnOK()
{
	USES_CONVERSION;

	m_strScript += _T("\r\n");

	CComBSTR bstr;
	m_editMin.GetWindowText((BSTR&)bstr);

	CComVariant var = bstr;
	if (var.ChangeType(VT_R4) != S_OK)
	{
		MessageBox(_T("输入合法的数据！"));
		m_editMin.SetFocus();
		m_editMin.SetSel(0, -1);
		return;
	}

	m_strScript += _T("	DataEntry.min = ");
	m_strScript += OLE2T(bstr);
	m_strScript += _T("\r\n");

	bstr.Empty();
	m_editMax.GetWindowText((BSTR&)bstr);

	var = bstr;
	if (var.ChangeType(VT_R4) != S_OK)
	{
		MessageBox(_T("请输入合法的数据！"));
		m_editMax.SetFocus();
		m_editMax.SetSel(0, -1);
		return;
	}

	m_strScript += _T("	DataEntry.max = ");
	m_strScript += OLE2T(bstr);
	m_strScript += _T("\r\n");

	if (m_btnFetchLimit.GetCheck() == 1)
		m_strScript += _T("	DataEntry.fetch = true");
	else
		m_strScript += _T("	DataEntry.fetch = false");
}

LRESULT CNumericEntryDlg::OnCharEditMax(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;
	
	TCHAR ch = (TCHAR)wParam;
	
	if (wParam >= 0x20)
	{
		CComBSTR bstr;
		m_editMax.GetWindowText((BSTR&)bstr);
		
		if ((IsCharAlphaNumeric(ch) && !IsCharAlpha(ch)) || 
			(ch == _T('.') && bstr.Length() > 0 && wcsrchr(bstr, WCHAR('.')) == NULL))
			bHandled = FALSE;
		else
			MessageBeep(MB_OK);
	}
	else
	{
		bHandled = FALSE;
	}
	
	return 0;
}

LRESULT CNumericEntryDlg::OnCharEditMin(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;
	
	TCHAR ch = (TCHAR)wParam;
	
	if (wParam >= 0x20)
	{
		CComBSTR bstr;
		m_editMin.GetWindowText((BSTR&)bstr);
		
		if ((IsCharAlphaNumeric(ch) && !IsCharAlpha(ch)) || 
			(ch == _T('.') && bstr.Length() > 0 && wcsrchr(bstr, WCHAR('.')) == NULL))
			bHandled = FALSE;
		else
			MessageBeep(MB_OK);
	}
	else
	{
		bHandled = FALSE;
	}
	
	return 0;
}

