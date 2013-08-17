// PushbuttonEntryDlg.cpp: implementation of the CPushbuttonEntryDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PushbuttonEntryDlg.h"
#include "EventExpert.h"
#include "ActionParser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPushbuttonEntryDlg::CPushbuttonEntryDlg()
{
	m_pExpert = NULL;
}

LRESULT CPushbuttonEntryDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_editOpen.Attach(GetDlgItem(IDC_OPEN));
	m_editOpen.SetWindowText(_T("´ò¿ª"));
	m_editClose.Attach(GetDlgItem(IDC_CLOSE));
	m_editClose.SetWindowText(_T("¹Ø±Õ"));
	
	return S_OK;
}

LRESULT CPushbuttonEntryDlg::OnChangeCapion(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pExpert->OnActionScriptChanged();

	return 0;
}

void CPushbuttonEntryDlg::OnShow()
{
	CActionParser parser(m_strScript);
	
	CString strRight;
	if (parser.HasAssignment(_T("DataEntry.openCaption"), strRight))
	{
		strRight = strRight.Mid(1, strRight.GetLength() - 2);
		m_editOpen.SetWindowText(strRight);
		m_editOpen.SetSel(0, -1);
	}
	
	if (parser.HasAssignment(_T("DataEntry.closeCaption"), strRight))
	{
		strRight = strRight.Mid(1, strRight.GetLength() - 2);
		m_editClose.SetWindowText(strRight);
	}

}

void CPushbuttonEntryDlg::OnOK()
{
	USES_CONVERSION;
	
	m_strScript += _T("\r\n");
	
	CComBSTR bstr;
	m_editOpen.GetWindowText((BSTR&)bstr);
	
	m_strScript += _T("	DataEntry.openCaption = \"");
	m_strScript += OLE2T(bstr);
	m_strScript += _T("\"\r\n");
	
	bstr.Empty();
	m_editClose.GetWindowText((BSTR&)bstr);
	
	m_strScript += _T("	DataEntry.closeCaption = \"");
	m_strScript += OLE2T(bstr);
	m_strScript += _T("\"");
}
