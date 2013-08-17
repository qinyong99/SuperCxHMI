// SQLBuildDlg.h: interface for the CSQLBuildDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SQLBUILDDLG_H__
#define _SQLBUILDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

#include "../../svr/CxDBProv/CxDBProv.h"

typedef CContainedWindowT<CListViewCtrl> CContainedList;
typedef CContainedWindowT<CEdit> CContainedEdit;

class CSQLBuildDlg :
	public CDialogImpl<CSQLBuildDlg>
{
	class CFieldItem
	{
	public:
		CFieldItem()
		{
			m_bTime = FALSE;
		}
		CFieldItem(const CFieldItem& field)
		{
			InternalCopy(field);
		}
		void InternalCopy(const CFieldItem& field)
		{
			m_strName = field.m_strName;
			m_strAlias = field.m_strAlias;
			m_bTime = field.m_bTime;
			m_strTimeFormat = field.m_strTimeFormat;
		}

		CString m_strName;
		CString m_strAlias;
		BOOL m_bTime;
		CString m_strTimeFormat;
	};

public:
	CSQLBuildDlg();
	
	enum { IDD = IDD_SQLBUILD };
	
	BEGIN_MSG_MAP(CSQLBuildDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_ALL_FIELD, BN_CLICKED, OnClickedAllField)
		COMMAND_HANDLER(IDC_TAB, CBN_SELCHANGE, OnSelchangeTab)
		MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
		MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
		NOTIFY_HANDLER(IDC_FIELD, LVN_DELETEITEM, OnDeleteitemField)
		COMMAND_HANDLER(IDC_FIELD_DELETE, BN_CLICKED, OnClickedFieldDelete)
		COMMAND_HANDLER(IDC_FIELD_DOWN, BN_CLICKED, OnClickedFieldDown)
		COMMAND_HANDLER(IDC_FIELD_INSERT, BN_CLICKED, OnClickedFieldInsert)
		COMMAND_HANDLER(IDC_FIELD_UP, BN_CLICKED, OnClickedFieldUp)
		COMMAND_HANDLER(IDC_TIME, BN_CLICKED, OnClickedTime)
		COMMAND_HANDLER(IDC_ACTOR, BN_CLICKED, OnClickedActor)
		COMMAND_HANDLER(IDC_AREA, BN_CLICKED, OnClickedArea)
		COMMAND_HANDLER(IDC_SEVERITY, BN_CLICKED, OnClickedSeverity)
		COMMAND_HANDLER(IDC_SOURCE, BN_CLICKED, OnClickedSource)
	ALT_MSG_MAP(1) 
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDownList)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDownList)
		MESSAGE_HANDLER(WM_KEYDOWN,	OnKeyDownList)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCodeList)
		MESSAGE_HANDLER(WM_HSCROLL, OnVScrollList)
		MESSAGE_HANDLER(WM_VSCROLL, OnVScrollList)
	ALT_MSG_MAP(2)		
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocusEdit)
		MESSAGE_HANDLER(WM_KEYDOWN,	OnKeyDownEdit)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCodeEdit) 
	END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedAllField(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	LRESULT OnSelchangeTab(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMeasureItem(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDeleteitemField(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnLButtonDownList(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetDlgCodeList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDownList(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnVScrollList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnGetDlgCodeEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	
	LRESULT OnKeyDownEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocusEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClickedFieldDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedFieldDown(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedFieldInsert(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedFieldUp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnClickedTime(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedActor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedArea(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedSeverity(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedSource(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
 	void LoadTabs();
	void LoadFields();

	BOOL OnEditCell(int nItem, int nColumn);

	void UpdateFieldButtons();

	CComPtr<ICxDataConnection> m_spDataConnection;
	CComPtr<ICxDataRecordset> m_spDataRecordset;

	CComBSTR m_bstrConnectionString;
	BOOL m_bHistoryData;

	CComBSTR m_bstrSQL;

	CComboBox m_cmbStatement;
	CComboBox m_cmbTab;
	CButton m_btnAllField;
	CContainedList m_listField;
	CButton m_btnTime;
	CDateTimePickerCtrl m_dtpcDateFrom;
	CDateTimePickerCtrl m_dtpcTimeFrom;
	CDateTimePickerCtrl m_dtpcDateTo;
	CDateTimePickerCtrl m_dtpcTimeTo;
	CButton m_btnName;
	CEdit m_editName;
	CButton m_btnType;
	CButton m_btnAnalog;
	CButton m_btnDigital;
	CButton m_btnString;
	CButton m_btnSource;
	CEdit m_editSource;
	CButton m_btnArea;
	CEdit m_editArea;
	CButton m_btnActor;
	CEdit m_editActor;
	CButton m_btnSeverity;
	CEdit m_editSeverityForm;
	CEdit m_editSeverityTo;
	CButton m_btnInterval;
	CEdit m_editInterval;
	CButton m_btnOrder;

	CFont m_font;
	CFont m_fontBold;
	CPen m_penGrid;
	
	int m_nSelectColumn;
	int m_nEditItem;
	int m_nEditColumn;
	
	CContainedEdit m_editInplace;

	CSimpleArray<CString> m_arrFieldNames;
	CSimpleArray<BOOL> m_arrFieldTimeProps;
};

#endif // _SQLBUILDDLG_H__
