// ToolPane.h: interface for the CToolPane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLPANE_H__80B19E27_CE02_415E_8F97_217465051FC2__INCLUDED_)
#define AFX_TOOLPANE_H__80B19E27_CE02_415E_8F97_217465051FC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

#define ID_TOOL									150
#define ID_PICKER_FROM_TIME						151
#define ID_PICKER_TO_TIME						152
#define ID_EDIT_ZOOM_RADIO						153

class CTrendCtrl;

typedef CContainedWindowT<CEdit> CContainedEdit;
typedef CContainedWindowT<CListViewCtrl> CContainedList;

class CToolPane : public CPaneContainerImpl<CToolPane>
{
public:
    DECLARE_WND_CLASS_EX(_T("CX_ToolPane"), 0, -1)

	CToolPane();
	virtual ~CToolPane();
		
	typedef CPaneContainerImpl<CToolPane> baseClass;
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
		MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
		COMMAND_ID_HANDLER(ID_SCROLL_BACK, OnScrollBack)
		COMMAND_ID_HANDLER(ID_TIME_UPDATE, OnTimeUpdate)
		COMMAND_ID_HANDLER(ID_SCROLL_FORWARD, OnScrollForward)
		COMMAND_ID_HANDLER(ID_SCROLL_FORWARD_MOST, OnScrollForwardMost)
		COMMAND_ID_HANDLER(ID_ZOOM_RADIO, DoNothing);
		COMMAND_RANGE_HANDLER(ID_ZOOM_1600, ID_ZOOM_12_5, OnZoomRadio);
		COMMAND_ID_HANDLER(ID_ZOOM_IN, OnZoomIn);
		COMMAND_ID_HANDLER(ID_ZOOM_OUT, OnZoomOut);
		COMMAND_ID_HANDLER(ID_CROSS_CURSOR, OnCrossCursor);
		COMMAND_ID_HANDLER(ID_PRINT, OnPrint);
		COMMAND_ID_HANDLER(ID_EDIT_SERIALS, OnEditSerials);
		COMMAND_ID_HANDLER(ID_MODE_REAL, OnModeRealtime)
		COMMAND_ID_HANDLER(ID_MODE_HIST, OnModeHistory)
		COMMAND_ID_HANDLER(ID_EDIT_PROPERTIES, OnEditProperties)
		NOTIFY_HANDLER(ID_TOOL, TBN_DROPDOWN, OnDropDown)
		NOTIFY_CODE_HANDLER(TTN_NEEDTEXT, OnToolbarNeedText)
		CHAIN_MSG_MAP(baseClass)
	ALT_MSG_MAP(1) 
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocusEdit)
		MESSAGE_HANDLER(WM_KEYDOWN,	OnKeyDownEdit)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCodeEdit)
	ALT_MSG_MAP(2) 
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDownList)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMeasureItem(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnTimeUpdate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnScrollBack(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnScrollForward(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnScrollForwardMost(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT DoNothing(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnZoomRadio(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnZoomIn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnZoomOut(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCrossCursor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPrint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditSerials(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnModeRealtime(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnModeHistory(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditProperties(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnDropDown(WPARAM idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnGetDlgCodeEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	
	LRESULT OnKeyDownEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocusEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnLButtonDownList(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);

	LRESULT OnToolbarNeedText(WPARAM, LPNMHDR pnmh, BOOL&);

	void CreateButtons();

	void DrawListItem(LPDRAWITEMSTRUCT lpdis);

	void Zoom(float fFactor);

	void CalcSize();
	void UpdateLayout(int cxWidth, int cyHeight);
	void UpdateLayout();
	void DrawPaneTitle(CDCHandle dc);

	void UpdateZoomRadioEdit();
	void UpdateToolBar();
	
	CToolBarCtrl m_wndToolBar;
	CDateTimePickerCtrl m_dtpcFromTimePicker;
	CDateTimePickerCtrl m_dtpcToTimePicker;
	CContainedEdit m_editZoomRadio;

	CContainedList m_wndList;

	COleDateTimeSpan m_dtsScrollTimeSpan;
	float m_fScaleFactor;

	CTrendCtrl* m_pTrend;
};

#endif // !defined(AFX_TOOLPANE_H__80B19E27_CE02_415E_8F97_217465051FC2__INCLUDED_)
