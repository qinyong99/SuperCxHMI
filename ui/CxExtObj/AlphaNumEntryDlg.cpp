// AlphaNumEntryDlg.cpp: implementation of the CAlphaNumEntryDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AlphaNumEntryDlg.h"
#include "CxExtObj.h"
#include "DataEntry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlphaNumEntryDlg::CAlphaNumEntryDlg(CDataEntry* pEntry, BOOL bNumberOnly) : 
	m_edit(this, 1)
{
	m_pDataEntry = pEntry;
	m_bNumberOnly = bNumberOnly;
}

CAlphaNumEntryDlg::~CAlphaNumEntryDlg()
{

}

LRESULT CAlphaNumEntryDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_edit.SubclassWindow(GetDlgItem(IDC_ALPHANUM));
	
	if (!m_bNumberOnly)
		SetWindowText(_T("文本输入"));
		
	CenterWindow();
	
	return 1;  // Let the system set the focus
}

LRESULT CAlphaNumEntryDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CComBSTR bstr;
	m_edit.GetWindowText((BSTR&)bstr);

	CComVariant var;
	if (m_bNumberOnly)
	{
		var = bstr;
		HRESULT hr = var.ChangeType(VT_R8);
		
		if (hr != S_OK || var > m_pDataEntry->m_varMax || var < m_pDataEntry->m_varMin)
		{
			CString strMessage = _T("请输入 ");
			var = m_pDataEntry->m_varMin;
			var.ChangeType(VT_BSTR);
			strMessage += var.bstrVal;
			strMessage += _T(" 到 ");
			var = m_pDataEntry->m_varMax;
			var.ChangeType(VT_BSTR);
			strMessage += var.bstrVal;
			strMessage += _T(" 之间的数字！");
			MessageBox(strMessage);

			m_edit.SetFocus();
			m_edit.SetSel(0, -1);
			return 0;
		}
	}
	else
	{
		CComBSTR bstrVal = L"\"";
		bstrVal += bstr;
		bstrVal += L"\"";
		var = bstrVal;
	}

	m_pDataEntry->PutValue(&var);
	EndDialog(IDOK);
	
	return 0;
}

LRESULT CAlphaNumEntryDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
	
	return 0;
}

LRESULT CAlphaNumEntryDlg::OnCharEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	TCHAR ch = (TCHAR)wParam;

	if (m_bNumberOnly)
	{
		if (wParam >= 0x20)
		{
			CComBSTR bstr;
			m_edit.GetWindowText((BSTR&)bstr);
			
			if ((IsCharAlphaNumeric(ch) && !IsCharAlpha(ch)) || 
//				(ch == _T('-') && bstr.Length() == 0) || 
				(ch == _T('.') && bstr.Length() > 0 && wcsrchr(bstr, WCHAR('.')) == NULL))
				bHandled = FALSE;
			else
				MessageBeep(MB_OK);
		}
		else
		{
			bHandled = FALSE;
		}
	}
	else
	{
		bHandled = FALSE;
	}

	return 0;
}

