// Chart.h: interface for the CChart class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CHART_H_
#define __CHART_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Serial.h"

#include "../../svr/CxDatSes/CxDatSes.h"

class CChart : public CPrintJobInfo
{
public:
	CChart();
	virtual ~CChart();

public:	
	STDMETHOD(InitNew)();
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty); 
	STDMETHOD(Load)(LPSTREAM pStm);

	void OnPrint();
	
	void DoDraw(HDC hdcDest);

	void UpdateSerialSourceProp(CSerial* pSerial);
	HRESULT ConnectToDataServer(CSerial* pAddSerial = NULL, CSerial* pRemoveSerial = NULL);
	BOOL OnRefreshData(BOOL bForce = FALSE);
	
	COLORREF GetNotUsedSerialColor();

	void InitSerialDemoVaules();

	virtual HRESULT FireViewChange();
	virtual BOOL ClientIsRunMode();
	virtual BOOL RecalcRects();
	virtual BOOL OnRefreshSerialData(CSerial* pSeria);
	virtual void GetDefaultServer(BSTR* pbstrComputer) {}

	int RecalcLegendsRect(RECT rect);

	virtual bool PrintPage(UINT nPage, HDC hDC);
			
public:
	BOOL m_bInitPrint;
	CPrinter m_printer;
	CDevMode m_devmode;
	CRect m_rectMargin;

	CComPtr<ICxDataSession> m_spDataSession;

	CComBSTR m_bstrTitle;
	CComPtr<IFontDisp> m_spCaptionFont;
	OLE_COLOR m_clrCaption;
		
	enumTrendMode m_enumTrendMode;
	
	VARIANT_BOOL m_bTransparency;
	OLE_COLOR m_clrFore;
	OLE_COLOR m_clrBack;
	OLE_COLOR m_clrFrame;
	enumFrameStyle m_enumFrameStyle;

	VARIANT_BOOL m_bShowMutiTimeAxis;
	VARIANT_BOOL m_bShowMutiValueAxis;
	VARIANT_BOOL m_bShowMutiGrid;
	VARIANT_BOOL m_bShowLegend;
	
	enumScrollDirection m_enumScrollDirection;
	enumZoomMode m_enumZoomMode;
	
	float m_fScrollPercentage;

	DWORD m_dwRefreshInterval; //刷新间隔以毫秒为单位
	
	CSimpleValArray<CComObject<CSerial> *> m_apSerials;

	CComObject<CValueAxis>* m_pValueAxis;	//横座标轴
	CComObject<CTimeAxis>* m_pTimeAxis;	//纵座标轴
	CComObject<CGrid>* m_pGrid; //网格

	RECT m_rcSerial;
	RECT m_rect;
};

#endif // __CHART_H_