// AlmSum.h: interface for the CAlmSum class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ALMSUM_H_
#define __ALMSUM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "atlcoll.h"

#include "Alarm.h"
#include "DrawHeader.h"
#include "DrawGrid.h"
#include "DrawScrollBar.h"
#include "NetNode.h"

#include "../../svr/CxDatSes/CxDatSes.h"

class CAlarmSinkWnd;

class CAlmSum
{
public:
	CAlmSum();
	virtual ~CAlmSum();

	int GetColumnWidth(int nCol);
	void SetColumnWidth(int nCol, int nWidth);

	void RecalIdealColumnWidth(CColumn* pColumn = NULL);

	// IPersistStreamInit
public:
	STDMETHOD(InitNew)();
	STDMETHOD(Load)(LPSTREAM pStm);
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty);

public:
	virtual BOOL RecalcRects();
	virtual HRESULT FireViewChange();
	virtual BOOL ClientIsRunMode();
	virtual void SetCapture(BOOL bCapture, CDrawCtrl* pCtrl = NULL);
	virtual HDC GetDC() = 0;
	virtual void ReleaseDC(HDC hDC) = 0;
	virtual HWND GetClientWindow() = 0;
	virtual IUnknown* GetObjectUnknown() = 0;
	virtual void GetDefaultServer(BSTR* pbstrComputer) {}

	virtual HRESULT OnEditProperties();
	virtual void OnContextMenu();

	void Release();

	LRESULT DoKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT DoWindowMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT DoMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	void DoDraw(HDC hdcDest);
	void DrawItem(HDC hdcDest, int nItem, RECT rcItem, RECT rcClip);
	void DrawHeader(HDC hdcDest);
	void DrawScrollBar(HDC hdcDest, BOOL bHor = TRUE);

	void InitDemo();
	void ReConnectNetNodes();

	CAlarmSinkWnd* m_pAlarmSinkWnd;

	int GetFirstVisibleRow();
	int GetFirstVisibleColumn();

	void DoHScroll(UINT nSBCode, UINT nPos, CDrawScrollBar* pScrollBar);
	void DoVScroll(UINT nSBCode, UINT nPos, CDrawScrollBar* pScrollBar);

	void AddAlarm(CAlarm* pAlarm);
	void AddAlarm(CAtlArray<CAlarm *>& apAlarms, CAlarm* pAlarm, enumAlarmField nSortField);
	void ReSortAlarms();
	void UpdateScrollbarVer(BOOL bHor, BOOL bVer);

	BOOL FilterAlarm(CAlarm* pAlarm);
	void ReFilterAlarms();

	void AckAlarm(CAlarm* pAlarm);
	void AckAllAlarms();
	void AckPageAlarms();

	CAlarm* GetSelectAlarm();
	void SelectAlarm(CAlarm* pAlarm);
	void SelectAlarm(int nIndex);

	void EnsureAlarmVisible(int nIndex);
	void EnsureAlarmVisible(CAlarm* pAlarm);

	void PopupPropertyPage(GUID guidPage);

	virtual void OnAlarmAcked(LPCTSTR szSource) {};

	void OnAlarm(DWORD dwCount, CXEVENTSTRUCT* pEvents);

	CDrawCtrl* m_pCtrlWithCapture;

	CComPtr<IFontDisp> m_spHeaderFont;
	long m_lRowHeight;

	CPen m_penGrid;
	CFont m_fontWinnding;
	
	VARIANT_BOOL m_bShowHeader;
	VARIANT_BOOL m_bAutoScroll;
	
	VARIANT_BOOL m_bShowScrollBar;
	VARIANT_BOOL m_bShowGrid;

	OLE_COLOR m_clrBack;
	OLE_COLOR m_clrHeaderFore;
	OLE_COLOR m_clrHeaderBack;

	VARIANT_BOOL m_bEnableAck;
	VARIANT_BOOL m_bEnableContextMenu;
	VARIANT_BOOL m_bEnableOnlineConfigure;

	CSimpleValArray<CColumn *> m_apColumns;
	CSimpleValArray<CForeFont *> m_apForeFont;
	CSimpleValArray<CBackColor *> m_apBackColor;
	
	CAtlArray<CAlarm *> m_apAlarms;

	int m_nSelectAlarm;
	
	CDrawHeader m_header;
	CDrawScrollBar m_scrollbarHor;
	CDrawScrollBar m_scrollbarVer;
	CDrawGrid m_grid;

	RECT m_rect;

	int m_nRowsPerWheelNotch;

	enumAlarmField m_nSortField;
	enumAlarmField m_nSecondSortField;
	BOOL m_bSortAscending;
	
	CComBSTR m_bstrNodeFilter;
	CComBSTR m_bstrAreaFilter;
	CComBSTR m_bstrPriorityFilter;
};

class CAlarmSinkWnd : public CWindowImpl<CAlarmSinkWnd, CWindow, CNullTraits>
{
public:
	CAlarmSinkWnd(CAlmSum* pAlmSum, LPCWSTR szServer)
	{
		m_pAlmSum = pAlmSum;
		m_bstrServer = szServer;
	}
	
	DECLARE_WND_CLASS(_T("Alarm Notification Sink Window"))
		
	BEGIN_MSG_MAP(CAlarmSinkWnd)
		MESSAGE_HANDLER(WM_ALARM_NOTIFY, OnAlarmNotify)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()
		
	LRESULT OnAlarmNotify(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		DWORD dwCount = wParam;
		CXEVENTSTRUCT* pEvents = (CXEVENTSTRUCT *)lParam;

		m_pAlmSum->OnAlarm(dwCount, pEvents);
		
		return 0;
	}

	LRESULT OnDestroy(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_spDataSession->SetNotifyWindow(NULL);
		
		return 0;
	}
	
	BOOL SubscribeAlarm(LPCWSTR szNodeFilter)
	{
		HRESULT hr;
		if (m_spDataSession == NULL)
		{
			hr = m_spDataSession.CoCreateInstance(CLSID_CxDataSession);
			if (FAILED(hr))
				return FALSE;

			m_spDataSession->SetThisNode(m_bstrServer, NULL, TRUE);
			m_spDataSession->SetNotifyWindow(m_hWnd);
		}

		DWORD dwCount = 0;
		LPWSTR*	pNames = NULL;
		
		hr = m_spDataSession->GetNetNodes(&dwCount, &pNames);
		if (SUCCEEDED(hr))
		{
			for (DWORD dw = 0; dw < dwCount; dw++)
			{
				BOOL bSubscribe = StrInStrlist(szNodeFilter, pNames[dw]);
				m_spDataSession->SubscribeAlarm(pNames[dw], bSubscribe);

				if (pNames[dw] != NULL)
					CoTaskMemFree(pNames[dw]);
			}

			if (pNames != NULL)
				CoTaskMemFree(pNames);
		}

		return TRUE;
	}

	void AckAlarm(LPCWSTR szTag)
	{
		m_spDataSession->AcknowledgeAlarm(szTag);
	}

	CAlmSum* m_pAlmSum;
	CComBSTR m_bstrServer;
	CComPtr<ICxDataSession> m_spDataSession;
};

#endif // __ALMSUM_H_
