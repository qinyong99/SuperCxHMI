// Axis.h: Definition of the CValueAxis class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXIS_H__58A183F2_C50F_45F3_BBFF_BB56A042636D__INCLUDED_)
#define AFX_AXIS_H__58A183F2_C50F_45F3_BBFF_BB56A042636D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

#include <float.h>

class CChart;

#define MIN TRUE
#define MAX FALSE

double AutoScale(double dValue, BOOL bDirection);

typedef struct tagDataPoint
{
	double dXVal;
	double dYVal;
} DATAPOINT, *LPDATAPOINT;

/////////////////////////////////////////////////////////////////////////////
// Axis

class CAxis 
{
public:
	CAxis();
	
	CComBSTR m_bstrDisplayFormat;

	double m_dMinVal;
	double m_dMaxVal;

	RECT m_rect;

	CComBSTR m_bstrTitle;
	long m_lNumTicks;
	VARIANT_BOOL m_bShowAxis;
	VARIANT_BOOL m_bShowTitle;
	OLE_COLOR m_clrAxis;
	OLE_COLOR m_clrTitle;
	CComPtr<IFontDisp> m_spTickFont;
	CComPtr<IFontDisp> m_spTitleFont;
	
	CChart* m_pChart;

	virtual void ScrollPercentage(BOOL bUpOrRight, float fPercentage);
	virtual void Scroll(double dRange);
	virtual void Scale(double dMin, double dMax);

	STDMETHOD(InitNew)();
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty); 
	STDMETHOD(Load)(LPSTREAM pStm);

	virtual void GetPointForValue(DATAPOINT* value, POINT* point, int nCount = 1) = 0;
	virtual double GetValueForPos(float fPos) = 0;
};

/////////////////////////////////////////////////////////////////////////////
// CValueAxis

class CValueAxis : 
	public CAxis,
	public IDispatchImpl<IValueAxis, &IID_IValueAxis, &LIBID_CXCHARTCTRLLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot
{
public:
	CValueAxis() 
	{
		m_dViewMaxVal			= -DBL_MAX;
		m_dViewMinVal			= DBL_MAX;
	}

BEGIN_COM_MAP(CValueAxis)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IValueAxis)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CValueAxis) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_NOT_AGGREGATABLE(CValueAxis)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAxis
public:
	STDMETHOD(get_DecimalDigits)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_DecimalDigits)(/*[in]*/ long newVal);
	STDMETHOD(get_WholeDigits)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_WholeDigits)(/*[in]*/ long newVal);
	STDMETHOD(get_TitleColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_TitleColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_AxisColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_AxisColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_ShowTitle)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowTitle)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ShowAxis)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowAxis)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Title)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Title)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_NumTicks)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_NumTicks)(/*[in]*/ long newVal);
	STDMETHOD(get_MinValue)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_MinValue)(/*[in]*/ double newVal);
	STDMETHOD(get_MaxValue)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_MaxValue)(/*[in]*/ double newVal);
	STDMETHOD(put_TickFont)(IFontDisp* pFont);
	STDMETHOD(putref_TickFont)(IFontDisp* pFont);
	STDMETHOD(get_TickFont)(IFontDisp** ppFont);
	STDMETHOD(put_TitleFont)(IFontDisp* pFont);
	STDMETHOD(putref_TitleFont)(IFontDisp* pFont);
	STDMETHOD(get_TitleFont)(IFontDisp** ppFont);
	
	void RecalcRect(RECT& rect);
	void DoDraw(HDC hdcDest);

	STDMETHOD(InitNew)();
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty); 
	STDMETHOD(Load)(LPSTREAM pStm);

	virtual void ScrollPercentage(BOOL bUpOrRight, float fPercentage);
	virtual void Scroll(double dRange);
	virtual void Scale(double dMin, double dMax);

	virtual void GetPointForValue(DATAPOINT* value, POINT* point, int nCount = 1);
	virtual double GetValueForPos(float fPos);

	void UpdateDisplayFormat();
	
	double m_dViewMinVal; //显示
	double m_dViewMaxVal;

	int m_iWholeDigits; 
	int m_iDecimalDigits; 
};

/////////////////////////////////////////////////////////////////////////////
// CTimeAxis

class CTimeAxis : 
	public CAxis,
	public IDispatchImpl<ITimeAxis, &IID_ITimeAxis, &LIBID_CXCHARTCTRLLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot
{
public:
	CTimeAxis() 
	{
		//初始化时间
		m_dMaxVal = (DATE)COleDateTime::GetCurrentTime();
		m_dMinVal = m_dMaxVal - (double)COleDateTimeSpan(0, 0, 10, 0);
	}
BEGIN_COM_MAP(CTimeAxis)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ITimeAxis)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CTimeAxis) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_NOT_AGGREGATABLE(CTimeAxis)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ITimeAxis
public:
	STDMETHOD(get_TimeSpan)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_TimeSpan)(/*[in]*/ double newVal);
	STDMETHOD(get_DisplayFormat)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_DisplayFormat)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_TitleColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_TitleColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_AxisColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_AxisColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_ShowTitle)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowTitle)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ShowAxis)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowAxis)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Title)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Title)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_NumTicks)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_NumTicks)(/*[in]*/ long newVal);
	STDMETHOD(get_MinValue)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_MinValue)(/*[in]*/ double newVal);
	STDMETHOD(get_MaxValue)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_MaxValue)(/*[in]*/ double newVal);
	STDMETHOD(put_TickFont)(IFontDisp* pFont);
	STDMETHOD(putref_TickFont)(IFontDisp* pFont);
	STDMETHOD(get_TickFont)(IFontDisp** ppFont);
	STDMETHOD(put_TitleFont)(IFontDisp* pFont);
	STDMETHOD(putref_TitleFont)(IFontDisp* pFont);
	STDMETHOD(get_TitleFont)(IFontDisp** ppFont);

	void RecalcRect(RECT& rect);
	void DoDraw(HDC hdcDest);

	STDMETHOD(InitNew)();
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty); 
	STDMETHOD(Load)(LPSTREAM pStm);

	void ScrollForward(COleDateTimeSpan dtsSpan);  //向前卷动
	void ScrollBack(COleDateTimeSpan dtsSpan); //向后卷动
	void ScrollTo(COleDateTime dtFrom, COleDateTime dtTo);
	void ScrollForwardMost(); //卷动到最后

	virtual void GetPointForValue(DATAPOINT* value, POINT* point, int nCount = 1);
	virtual double GetValueForPos(float fPos);
};

#endif // !defined(AFX_AXIS_H__58A183F2_C50F_45F3_BBFF_BB56A042636D__INCLUDED_)
