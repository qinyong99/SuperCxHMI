// PushbuttonEntryDlg.cpp: implementation of the CPushbuttonEntryDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PushbuttonEntryDlg.h"
#include "CxExtObj.h"
#include "DataEntry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPushbuttonEntryDlg::CPushbuttonEntryDlg(CDataEntry* pEntry)
{
	m_pDataEntry = pEntry;
}

CPushbuttonEntryDlg::~CPushbuttonEntryDlg()
{
	
}

LRESULT CPushbuttonEntryDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	m_btnOpen.Attach(GetDlgItem(IDC_OPEN));
	m_btnClose.Attach(GetDlgItem(IDC_CLOSE));

	m_btnOpen.SetWindowText(OLE2T(m_pDataEntry->m_bstrOpenCaption));
	m_btnClose.SetWindowText(OLE2T(m_pDataEntry->m_bstrCloseCaption));

//	CComVariant var;
//	m_pDataEntry->GetValue(&var);
//	VariantChangeType(&var, &var, 0, VT_BOOL);
//	m_switch.SetCheck(var.boolVal == VARIANT_TRUE ? 1 : 0);
//
//	m_switch.SetWindowText(m_switch.GetCheck() == 1 ? 
//		OLE2T(m_pDataEntry->m_bstrCloseCaption) : OLE2T(m_pDataEntry->m_bstrOpenCaption));
		
	CenterWindow();
	
	return 1;  // Let the system set the focus
}

//LRESULT CPushbuttonEntryDlg::OnClickedSwitch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
//{
//	USES_CONVERSION;
//	CComVariant var(m_switch.GetCheck() == 1 ? VARIANT_TRUE : VARIANT_FALSE);
//	m_pDataEntry->PutValue(&var);
//
//	MessageBox("aa");
//
//	m_pDataEntry->GetValue(&var);
//	VariantChangeType(&var, &var, 0, VT_BOOL);
//	m_switch.SetCheck(var.boolVal != VARIANT_FALSE ? 1 : 0);
//
//	m_switch.SetWindowText(m_switch.GetCheck() == 1 ? 
//		OLE2T(m_pDataEntry->m_bstrCloseCaption) : OLE2T(m_pDataEntry->m_bstrOpenCaption));
//	
//	return 0;


LRESULT CPushbuttonEntryDlg::OnClickedOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CComVariant var(VARIANT_TRUE);
	m_pDataEntry->PutValue(&var);

	return 0;
}

LRESULT CPushbuttonEntryDlg::OnClickedClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CComVariant var(VARIANT_FALSE);
	m_pDataEntry->PutValue(&var);
	
	return 0;
}

LRESULT CPushbuttonEntryDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(IDOK);
	
	return 0;
}

LRESULT CPushbuttonEntryDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
	
	return 0;
}
