// ChartObject.h: interface for the CChartObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTOBJECT_H__ADC0C563_0D42_4223_AA34_03D624853234__INCLUDED_)
#define AFX_CHARTOBJECT_H__ADC0C563_0D42_4223_AA34_03D624853234__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CChartObject  
{
public:
	CChartObject();
	virtual ~CChartObject();

	virtual BOOL CanDelete() { return FALSE; }
	virtual void DoDraw(HDC hdcDest) { return; }

	RECT m_rect;

	BOOL m_bVisible;
};

#endif // !defined(AFX_CHARTOBJECT_H__ADC0C563_0D42_4223_AA34_03D624853234__INCLUDED_)
