// SliderEntryDlg.cpp: implementation of the CSliderEntryDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SliderEntryDlg.h"
#include "CxExtObj.h"
#include "DataEntry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSliderEntryDlg::CSliderEntryDlg(CDataEntry* pEntry)
{
	m_pDataEntry = pEntry;
}

CSliderEntryDlg::~CSliderEntryDlg()
{
	
}

LRESULT CSliderEntryDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	m_slider.Attach(GetDlgItem(IDC_SLIDER));
	m_slider.SetRange(m_pDataEntry->m_varMin.intVal, m_pDataEntry->m_varMax.intVal);

	CComVariant var;
	m_pDataEntry->GetValue(&var);
	var.ChangeType(VT_I4);
	if (var.vt == VT_I4)
	{
		m_slider.SetPos(var.lVal);
		SetDlgItemInt(IDC_NUM, var.lVal);
	}

	var = m_pDataEntry->m_varMin;
	var.ChangeType(VT_BSTR);
	if (var.vt == VT_BSTR)
		SetDlgItemText(IDC_MIN, OLE2T(var.bstrVal));

	var = m_pDataEntry->m_varMax;
	var.ChangeType(VT_BSTR);
	if (var.vt == VT_BSTR)
		SetDlgItemText(IDC_MAX, OLE2T(var.bstrVal));
	
	CenterWindow();
	
	return 1;  // Let the system set the focus
}

LRESULT CSliderEntryDlg::OnReleasedcaptureSlider(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int n = m_slider.GetPos();
	SetDlgItemInt(IDC_NUM, n);

	CComVariant var = m_slider.GetPos();
	m_pDataEntry->PutValue(&var);

	return 0;
}

LRESULT CSliderEntryDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
//	CComVariant var = m_slider.GetPos();
//	m_pDataEntry->PutValue(&var);

	EndDialog(IDOK);
	
	return 0;
}

LRESULT CSliderEntryDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
	
	return 0;
}
