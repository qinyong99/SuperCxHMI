// SliderEntryDlg.h: interface for the CSliderEntryDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SLIDERENTRYDLG_H_
#define __SLIDERENTRYDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CDataEntry;

class CSliderEntryDlg :
	public CDialogImpl<CSliderEntryDlg>
{
public:
	CSliderEntryDlg(CDataEntry* pEntry);
	virtual ~CSliderEntryDlg();
	
	enum { IDD = IDD_SLIDER };
	
	BEGIN_MSG_MAP(CSliderEntryDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		NOTIFY_HANDLER(IDC_SLIDER, NM_RELEASEDCAPTURE, OnReleasedcaptureSlider)
		END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnReleasedcaptureSlider(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	
public:
	CDataEntry* m_pDataEntry;

	CTrackBarCtrl m_slider;
	
};

#endif // __SLIDERENTRYDLG_H_
