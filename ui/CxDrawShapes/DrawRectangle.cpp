// DrawRectangle.cpp : implementation file

#include "stdafx.h"
#include "CxDrawShapes.h"
#include "DrawRectangle.h"

void CDrawRectangle::AddObjectToPath()
{
	m_pObjectPath->AddRectangle(Gdiplus::Rect(0, 0, 200, 200));
}

HRESULT CDrawRectangle::DoLoad(DWORD dwVer, LPSTREAM pStm)
{
	pStm->Read(&m_enumFillStyle, sizeof(m_enumFillStyle), NULL);
	pStm->Read(&m_enumHorFillDirection, sizeof(m_enumHorFillDirection), NULL);
	pStm->Read(&m_enumVerFillDirection, sizeof(m_enumVerFillDirection), NULL);
	pStm->Read(&m_nHorFillPercent, sizeof(m_nHorFillPercent), NULL);
	pStm->Read(&m_nVerFillPercent, sizeof(m_nVerFillPercent), NULL);
	m_BrushImageFileName.ReadFromStream(pStm);
	pStm->Read(&m_clrFillColor, sizeof(m_clrFillColor), NULL);
	pStm->Read(&m_clrHighLightFill, sizeof(m_clrHighLightFill), NULL);
	
	pStm->Read(&m_enumEdgeStyle, sizeof(m_enumEdgeStyle), NULL);
	pStm->Read(&m_fEdgeWidth, sizeof(m_fEdgeWidth), NULL);
	pStm->Read(&m_enumLineJoinStyle, sizeof(m_enumLineJoinStyle), NULL);
	
	pStm->Read(&m_clrEdgeColor, sizeof(m_clrEdgeColor), NULL);
	pStm->Read(&m_clrHighLightEdge, sizeof(m_clrHighLightEdge), NULL);
	
	pStm->Read(&m_ptRotateBase, sizeof(m_ptRotateBase), NULL);
	pStm->Read(&m_enumScaleBase, sizeof(m_enumScaleBase), NULL);
	
	pStm->Read(&m_enumMousePointer, sizeof(m_enumMousePointer), NULL);
	m_bstrDescription.ReadFromStream(pStm);
	
	return CCxDrawObjectBase::DoLoad(dwVer, pStm);
}

HRESULT CDrawRectangle::DoSave(DWORD dwVer, LPSTREAM pStm)
{
	pStm->Write(&m_enumFillStyle, sizeof(m_enumFillStyle), NULL);
	pStm->Write(&m_enumHorFillDirection, sizeof(m_enumHorFillDirection), NULL);
	pStm->Write(&m_enumVerFillDirection, sizeof(m_enumVerFillDirection), NULL);
	pStm->Write(&m_nHorFillPercent, sizeof(m_nHorFillPercent), NULL);
	pStm->Write(&m_nVerFillPercent, sizeof(m_nVerFillPercent), NULL);
	m_BrushImageFileName.WriteToStream(pStm);
	pStm->Write(&m_clrFillColor, sizeof(m_clrFillColor), NULL);
	pStm->Write(&m_clrHighLightFill, sizeof(m_clrHighLightFill), NULL);
	
	pStm->Write(&m_enumEdgeStyle, sizeof(m_enumEdgeStyle), NULL);
	pStm->Write(&m_fEdgeWidth, sizeof(m_fEdgeWidth), NULL);
	pStm->Write(&m_enumLineJoinStyle, sizeof(m_enumLineJoinStyle), NULL);
	
	pStm->Write(&m_clrEdgeColor, sizeof(m_clrEdgeColor), NULL);
	pStm->Write(&m_clrHighLightEdge, sizeof(m_clrHighLightEdge), NULL);
	
	pStm->Write(&m_ptRotateBase, sizeof(m_ptRotateBase), NULL);
	pStm->Write(&m_enumScaleBase, sizeof(m_enumScaleBase), NULL);
	
	pStm->Write(&m_enumMousePointer, sizeof(m_enumMousePointer), NULL);
	m_bstrDescription.WriteToStream(pStm);
	
	return CCxDrawObjectBase::DoSave(dwVer, pStm);
}

//////////////////////////////////////////////////////////////
//功能: 取对象名字串
//////////////////////////////////////////////////////////////
STDMETHODIMP CDrawRectangle::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("Rectangle");
	return S_OK;
}
