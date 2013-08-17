// Serial.h: Definition of the CSerial class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIAL_H__350ACB92_A782_4E12_ABD9_BB88DCFC1843__INCLUDED_)
#define AFX_SERIAL_H__350ACB92_A782_4E12_ABD9_BB88DCFC1843__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

#include "OleDateTime.h"

#include "Axis.h"
#include "Legend.h"
#include "Grid.h"

class CChart;

/////////////////////////////////////////////////////////////////////////////
// CSerial

void DrawMarker(HDC hdc, int x, int y, int mode, int n);

class CSerial : 
	public IDispatchImpl<ISerial, &IID_ISerial, &LIBID_CXCHARTCTRLLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot
{
public:
	CSerial() 
	{
		m_pChart = NULL;
	
		m_dValTotal = 0;
		m_dMaxVal = -DBL_MAX;
		m_dMinVal = +DBL_MAX;
		m_dwTagCookie = 0;
	}
BEGIN_COM_MAP(CSerial)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISerial)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CSerial) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_NOT_AGGREGATABLE(CSerial)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ISerial
public:
	STDMETHOD(get_Source)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Source)(/*[in]*/ BSTR newVal);

	STDMETHOD(get_ValueAxis)(/*[out, retval]*/ IValueAxis* *pVal);
	STDMETHOD(get_TimeAxis)(/*[out, retval]*/ ITimeAxis* *pVal);
	STDMETHOD(get_Legend)(/*[out, retval]*/ ILegend* *pVal);
	STDMETHOD(get_Grid)(/*[out, retval]*/ IGrid* *pVal);

	STDMETHOD(get_MaxNumPoints)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_MaxNumPoints)(/*[in]*/ long newVal);

	STDMETHOD(get_MaxNumDisplayPoints)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_MaxNumDisplayPoints)(/*[in]*/ long newVal);

	STDMETHOD(get_LineStyle)(/*[out, retval]*/ enumLineStyle *pVal);
	STDMETHOD(put_LineStyle)(/*[in]*/ enumLineStyle newVal);
	STDMETHOD(get_LineColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_LineColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_LineWidth)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_LineWidth)(/*[in]*/ long newVal);

	STDMETHOD(get_MinValueLimit)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_MinValueLimit)(/*[in]*/ double newVal);
	STDMETHOD(get_MaxValueLimit)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_MaxValueLimit)(/*[in]*/ double newVal);

public:
	STDMETHOD(get_Show)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Show)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_MarkerStyle)(/*[out, retval]*/ enumMarkerStyle *pVal);
	STDMETHOD(put_MarkerStyle)(/*[in]*/ enumMarkerStyle newVal);
	void FinalRelease()
	{
		if (m_pTimeAxis)
			m_pTimeAxis->Release();
		if (m_pValueAxis)
			m_pValueAxis->Release();
		if (m_pLegend)
			m_pLegend->Release();
		if (m_pGrid)
			m_pGrid->Release();
	}

	STDMETHOD(InitNew)();
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty); 
	STDMETHOD(Load)(LPSTREAM pStm);

	void AddValue(DATAPOINT& newVal);
	void RemoveAllValues();

	void AdjustLegendHeight(int nHeight);
	void CalcLegendLargestWidth(int& nWidth);
	void RecalcLegend(RECT& rect);
	void RecalcTimeAxis(RECT& rect);
	void RecalcValueAxis(RECT& rect);

	void DoDraw(HDC hdcDest, RECT& rect, BOOL bTimeAxis, BOOL bValueAxis, BOOL bGrid, BOOL bLegend);
	void DoDrawCurve(HDC hdcDest, RECT& rect);

	void InitDemoValues();

	void Scroll(BOOL bHorizon, BOOL bUpOrRight, float fPercentage);  //卷动
	void ScrollForward(COleDateTimeSpan dtsSpan);  //向前卷动
	void ScrollBack(COleDateTimeSpan dtsSpan); //向后卷动
	void ScrollTo(COleDateTime dtFrom, COleDateTime dtTo); //卷动到时间段
	void ScrollForwardMost(); //卷动到最后
	void Zoom(float fYHi, float fYLo, float fXHi, float fXLo); //缩放
	void Zoom(BOOL bHorizon, float fHi, float fLo);

	void RefreshHistData(); //刷新历史数据
	BOOL IsShowCurrentData();

	void GetIndexByXVal(long& nIndex, double x);

	CComBSTR m_bstrSource;
	CComBSTR m_bstrTag;
	DWORD m_dwTagCookie; //数据源对应的甜饼

	enumLineStyle m_enumLineStyle;
	OLE_COLOR m_clrLineColor;
	long m_lLineWidth;
	enumMarkerStyle m_enumMarkerStyle;

	CComObject<CValueAxis>* m_pValueAxis;	//横座标轴
	CComObject<CTimeAxis>* m_pTimeAxis;	//纵座标轴
	CComObject<CLegend>* m_pLegend; //标识
	CComObject<CGrid>* m_pGrid; //网格

	CValueAxis* GetValueAxis();
	CTimeAxis* GetTimeAxis();
	
	CSimpleArray<DATAPOINT> m_values;

	int m_nMaxNumDisplayPoints;
	int m_nMaxNumPoints;

	VARIANT_BOOL m_bShow;

	CChart* m_pChart;

	CComBSTR m_bstrSourceDesc; // 数据源描述
	CComBSTR m_bstrSourceUnit; // 数据源单位
	double m_dValTotal; //值和
	double m_dMaxVal;
	double m_dMinVal;

	double m_dMaxValLimit;
	double m_dMinValLimit;
};

#endif // !defined(AFX_SERIAL_H__350ACB92_A782_4E12_ABD9_BB88DCFC1843__INCLUDED_)
