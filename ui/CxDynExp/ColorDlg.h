// ColorDlg.h: interface for the CColorDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COLORDLG_H_
#define __COLORDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"

class CLookupLevel
{
public:
	CComVariant m_vaInput1;
	CComVariant m_vaInput2;
	CComVariant m_vaOutput;
public:
	CLookupLevel()
	{
	}
	
	CLookupLevel(const CLookupLevel& level)
	{
		InternalCopy(level);
	}
	
	CLookupLevel& operator = (CLookupLevel& level)
	{
		InternalCopy(level);
		return *this;
	}
	
	void InternalCopy(const CLookupLevel& level)
	{
		m_vaInput1 = level.m_vaInput1;
		m_vaInput2 = level.m_vaInput2;
		m_vaOutput = level.m_vaOutput;
	}
};

#include "SimpleArray2.h"
typedef CSimpleArray2<CLookupLevel> CLookupLevelArray;

typedef CContainedWindowT<CListViewCtrl> CContainedList;
typedef CContainedWindowT<CEdit> CContainedEdit;

class CDynamicExpert;

class CColorDlg :
	public CDialogImpl<CColorDlg> 
{
public:
	CColorDlg();
	virtual ~CColorDlg();
	
	enum { IDD = IDD_COLOR };
	
	BEGIN_MSG_MAP(CColorDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
		MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
		COMMAND_HANDLER(IDC_EXACTMATCH, BN_CLICKED, OnClickedExactmatch)
		COMMAND_HANDLER(IDC_RANGECOMPARE, BN_CLICKED, OnClickedRangecompare)
		COMMAND_HANDLER(IDC_INSERT_ROW, BN_CLICKED, OnClickedInsertRow)
		COMMAND_HANDLER(IDC_DELETE_ROW, BN_CLICKED, OnClickedDeleteRow)
		COMMAND_HANDLER(IDC_EDIT_ROW, BN_CLICKED, OnClickedEditRow)
	ALT_MSG_MAP(1)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDownList)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDownList)
		MESSAGE_HANDLER(WM_KEYDOWN,	OnKeyDownList)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCodeList)
	ALT_MSG_MAP(2) 
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocusEdit)
		MESSAGE_HANDLER(WM_KEYDOWN,	OnKeyDownEdit)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCodeEdit)
	END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClickedExactmatch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedRangecompare(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedInsertRow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedDeleteRow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedEditRow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMeasureItem(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnLButtonDownList(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetDlgCodeList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDownList(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnGetDlgCodeEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	
	LRESULT OnKeyDownEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocusEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	virtual void OnShow();
	virtual void OnOK();

	void RemoveAllLevels();
	int GetLevelsCount();
	void AddLevel(VARIANT* pInput1, VARIANT* pInput2, VARIANT* pOutput);
	void GetLevel(int iIndex, VARIANT *pInput1, VARIANT *pInput2, VARIANT *pOutput);
	
	void GetCurrentColor(DISPID dispid, OLE_COLOR* pColor);

	BOOL OnEditCell(int nItem, int nColumn);
	void LoadLevelItems();
	
	CDynamicExpert* m_pExpert;
	CComPtr<IUnknown> m_spUnkDynamic;

	BOOL m_bExactmatch;

	CContainedList m_list;
	int m_nSelectColumn;
	int m_nEditItem;
	int m_nEditColumn;

	CContainedEdit m_editInplace;
	
	CFont m_font;
	CFont m_fontBold;
	CPen m_penGrid;

	CLookupLevelArray m_arrLevels;
};


#endif // __COLORDLG_H_
