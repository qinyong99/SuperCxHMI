// DataNotation.h: interface for the CDataNotation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATANOTATION_H__CD6A541E_0135_4E8A_B7C8_4C9A87E55D77__INCLUDED_)
#define AFX_DATANOTATION_H__CD6A541E_0135_4E8A_B7C8_4C9A87E55D77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartObject.h"

class CTrendCtrl;

class CDataNotation : public CChartObject
{
public:
	CDataNotation();
	virtual ~CDataNotation();

	CString m_strText;

	double m_dXVal;
	double m_dYVal;
	int	m_nCurve;
	int m_nIndex;

	POINT m_ptLast;

	BOOL m_bPositioned;
	
	virtual void DoDraw(HDC hdcDest);
	virtual BOOL CanDelete() { return TRUE; }

	CTrendCtrl* m_pTrend;
};

#endif // !defined(AFX_DATANOTATION_H__CD6A541E_0135_4E8A_B7C8_4C9A87E55D77__INCLUDED_)
