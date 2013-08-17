// Label.h: interface for the CLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LABEL_H__748622C6_44F8_40E1_823B_67BA40C57C8F__INCLUDED_)
#define AFX_LABEL_H__748622C6_44F8_40E1_823B_67BA40C57C8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartObject.h"

class CLabel : public CChartObject 
{
public:
	CLabel();
	virtual ~CLabel();

	CString m_strText;
	
	virtual void DoDraw(HDC hdcDest);
};

#endif // !defined(AFX_LABEL_H__748622C6_44F8_40E1_823B_67BA40C57C8F__INCLUDED_)
