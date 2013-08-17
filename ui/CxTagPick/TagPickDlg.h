// TagPickDlg.h: interface for the CTagPickDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __TAGPICKDLG_H_
#define __TAGPICKDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "RegPropertyBag.h"
#include "OEMSetup.h"

class CTagPickDlg : 
	public CAxDialogImpl<CTagPickDlg>,
	public IDispEventImpl<0, CTagPickDlg, &DIID__ICxTagPickCtrlEvents, &LIBID_CXTAGPICKLib, 1, 0>
{
public:
	CTagPickDlg()
	{
		m_pPickCtrlPropertyBag = new CComObject<CRegPropertyBag>(_T("PickCtrlPropertyBag"));
		m_pPickCtrlPropertyBag->AddRef();

		m_pPickCtrlPropertyBag->SetRegistryKey(COEMSetup::GetSystemName());
	}

	enum { IDD = IDD_TAG };
	
	BEGIN_MSG_MAP(CTagPickDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()
	// Handler prototypes:
	//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	BEGIN_SINK_MAP(CTagPickDlg)
		SINK_ENTRY_INFO(0, DIID__ICxTagPickCtrlEvents, 1, OnChanged, NULL)
//		SINK_ENTRY(0, 1, OnChanged)
	END_SINK_MAP()

	HRESULT __stdcall OnChanged();
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
public:
	CString m_strAssocTagGroup; //关联标签组
	BOOL m_bUseLocalSetup;
	BOOL m_bDisableAssoc;
	CString m_strPickTag;

	CComPtr<IUnknown> m_spDispatch; 
	CComObject<CRegPropertyBag>* m_pPickCtrlPropertyBag;
};

#endif // __TAGPICKDLG_H_
