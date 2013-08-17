// GoLineDlg.cpp: implementation of the CGoLineDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GoLineDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGoLineDlg::CGoLineDlg()
{

}

LRESULT CGoLineDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CString strText;
	strText.Format(_T("ÐÐºÅ(1 - %d):"), m_nLineCount);
	SetDlgItemText(IDC_LINE_TEXT, strText);
	m_nLine++;
	DoDataExchange(FALSE);
	
	return 0;
}

LRESULT CGoLineDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled)
{
	DoDataExchange(TRUE);
	m_nLine--;

	EndDialog(wID);
	return 0;
}

LRESULT CGoLineDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}