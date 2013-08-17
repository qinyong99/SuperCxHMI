// ShapeGenPage.cpp : Implementation of CShapeGenPage
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "ShapeGenPage.h"

/////////////////////////////////////////////////////////////////////////////
// CShapeGenPage

LRESULT CShapeGenPage::OnHorFillSpin(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	CComVariant temp = m_editHorizontalFillPercent.GetText();
	temp.ChangeType(VT_I4);
	LPNMUPDOWN lpUpdown = (LPNMUPDOWN)pnmh;
	if (lpUpdown->iDelta < 0  && temp.lVal < 100)
		temp.lVal += 1; 
	else if (lpUpdown->iPos > 0 && temp.lVal > 0)
		temp.lVal -= 1;
	m_editHorizontalFillPercent.SetText(temp);
	
	m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
	
	return 0;
}

LRESULT CShapeGenPage::OnVerFillSpin(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	CComVariant temp = m_editVerticalFillPercent.GetText();
	temp.ChangeType(VT_I4);
	LPNMUPDOWN lpUpdown = (LPNMUPDOWN)pnmh;
	if (lpUpdown->iDelta < 0  && temp.lVal < 100)
		temp.lVal += 1; 
	else if (lpUpdown->iPos > 0 && temp.lVal > 0)
		temp.lVal -= 1;
	m_editVerticalFillPercent.SetText(temp);
	
	m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
	
	return 0;
}

LRESULT CShapeGenPage::OnEdgeWidthSpin(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	CComVariant temp = m_editEdgeWidth.GetText();
	temp.ChangeType(VT_I4);
	LPNMUPDOWN lpUpdown = (LPNMUPDOWN)pnmh;
	if (lpUpdown->iDelta < 0  && temp.lVal < 60)
		temp.lVal += 1; 
	else if (lpUpdown->iPos > 0 && temp.lVal > 1)
		temp.lVal -= 1;
	m_editEdgeWidth.SetText(temp);
	
	m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
	
	return 0;
}