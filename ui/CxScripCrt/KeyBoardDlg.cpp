// KeyBoardDlg.cpp: implementation of the CKeyBoardDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KeyBoardDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyBoardDlg::CKeyBoardDlg()
{
}

CKeyBoardDlg::~CKeyBoardDlg()
{
}

void CKeyBoardDlg::SetTextOutWindow(HWND hWndTextOut)
{
	m_edit.Attach(hWndTextOut);
}

LRESULT CKeyBoardDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1;
}

LRESULT CKeyBoardDlg::OnClickedSymbols(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	USES_CONVERSION;
	CComBSTR bstr;
	
//	Invalidate();
//	::InvalidateRect(GetDlgItem(wID), NULL, TRUE);

	GetDlgItemText(wID, (BSTR&)bstr);
	CString strInsertTxt(bstr);

	if (strInsertTxt == _T("&&"))
		strInsertTxt = _T("&");

	if (wID <= IDC_BUTTON19)
	{
		if (strInsertTxt != _T("(") && strInsertTxt != _T(")"))
		{
			if (strInsertTxt != _T("Not"))
				strInsertTxt.Insert(0, _T(" "));
			strInsertTxt += _T(" ");
		}
	}

	m_edit.ReplaceSel(strInsertTxt);
	m_edit.SetFocus();
	m_edit.SetFocus();

	return 0;		
}

LRESULT CKeyBoardDlg::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	

	return 0;
}