// DrawRoundRectangle.cpp : Implementation of CDrawRoundRectangle
// 所属项目: AutoCx的园角矩形对象
// 创建日期: 2001.10.8
// 创 建 人：卢文江 
// 功能描述：
// 修改记录：
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxDrawShapes.h"
#include "DrawRoundRectangle.h"

STDMETHODIMP CDrawRoundRectangle::InitNew()
{
	m_roundness.x = m_roundness.y = 32;	
	return ICxDrawObjectImpl<CDrawRoundRectangle>::InitNew();
}

void CDrawRoundRectangle::InitRoundRectPath(Gdiplus::GraphicsPath**	ppObjectPath,
		int nLeftRect, int nTopRect, int nRightRect, 
		int nBottomRect, int nWidth, int nHeight)
{
	int W =  nWidth/2;
	int H =  nHeight/2;

	if (2*W >= nRightRect - nLeftRect)
		W = (nRightRect - nLeftRect)/2;

	if (2*H >= nBottomRect - nTopRect)
		H = (nBottomRect - nTopRect)/2;

	using namespace Gdiplus;
	*ppObjectPath = new GraphicsPath;
	(*ppObjectPath)->AddArc(nLeftRect, nTopRect, 2*W, 2*H, 180, 90);
	(*ppObjectPath)->AddArc(nRightRect- 2*W, nTopRect,  2*W, 2*H, 270, 90);
	(*ppObjectPath)->AddArc(nRightRect- 2*W, nBottomRect-2*H,2*W, 2*H, 0, 90);
	(*ppObjectPath)->AddArc(nLeftRect, nBottomRect- 2*H, 2*W, 2*H, 90, 90);
	(*ppObjectPath)->CloseFigure();
}

void CDrawRoundRectangle::AddObjectToPath()
{
	delete m_pObjectPath;
	InitRoundRectPath(&m_pObjectPath, 0, 0, 200, 200, int(m_roundness.x), int(m_roundness.y));
}

HRESULT CDrawRoundRectangle::DoLoad(DWORD dwVer, LPSTREAM pStm)
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

	pStm->Read(&m_roundness, sizeof(m_roundness), NULL);
	
	return CCxDrawObjectBase::DoLoad(dwVer, pStm);
}

HRESULT CDrawRoundRectangle::DoSave(DWORD dwVer, LPSTREAM pStm)
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

	pStm->Write(&m_roundness, sizeof(m_roundness), NULL);
	
	return CCxDrawObjectBase::DoSave(dwVer, pStm);
}

STDMETHODIMP CDrawRoundRectangle::MoveTo(RECTF rect, BOOL bUniformScale)
{
	//如果小于零则表示对象以发生旋转
	if (m_roundness.x >= 0 && m_roundness.y >= 0)
	{	
		using namespace Gdiplus;
		RectF rcPos;
		m_pObjectPath->GetBounds(&rcPos, NULL, NULL);

		float fScaleX;
		float fScaleY;

		if (rcPos.Width == 0)
			fScaleX = 1;
		else
			fScaleX = (rect.right - rect.left) / rcPos.Width;
		if (rcPos.Height == 0)
			fScaleY = 1;
		else
			fScaleY = (rect.bottom - rect.top) / rcPos.Height;

		if (fScaleX == 0)
			fScaleX = 1 / rcPos.Width;
		if (fScaleY == 0)
			fScaleY = 1 / rcPos.Height;
		if (fScaleX < 0)
			fScaleX = -fScaleX;
		if (fScaleY < 0)
			fScaleY = -fScaleY;

		m_roundness.x *= fScaleX;
		m_roundness.y *= fScaleY;
	}

	return ICxDrawObjectImpl<CDrawRoundRectangle>::MoveTo(rect, bUniformScale);
}

STDMETHODIMP CDrawRoundRectangle::DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
{
	RECTF rectF;
	RECT rect;
	GetPositionRect(&rectF);
	rect.left = ROUND(rectF.left);
	rect.right = ROUND(rectF.right);
	rect.top = ROUND(rectF.top);	
	rect.bottom = ROUND(rectF.bottom);

	if (nHandle == -9)
	{
		if (point.x > rect.right - 1)
			point.x = rect.right - 1;
		else if (point.x < (rect.left + rect.right) / 2)
			point.x = (rect.left + rect.right) / 2;
		if (point.y > rect.bottom - 1)
			point.y = rect.bottom - 1;
		else if (point.y < (rect.top + rect.bottom) / 2)
			point.y = (rect.top + rect.bottom) / 2;
		int nWidth = 2 * (rect.right - point.x);
		int nHeight = 2 * (rect.bottom - point.y);
		::RoundRect(hdcDest, rect.left, rect.top, rect.right, 
				rect.bottom, nWidth, nHeight);
		
		return S_OK;
	}
	
	return ICxDrawObjectImpl<CDrawRoundRectangle>::DrawTrackerHandleShape(hdcDest, nHandle, point);
}

/////////////////////////////////////////////////////////////////////////
//功能: 移动对象指定手柄至新的位置
//参数: nHandle[in] 手柄编号
//		point[in] 手柄新位置
/////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawRoundRectangle::MoveHandleTo(long nHandle, POINT point)
{
	RECTF rectF;
	RECT rect;
	GetPositionRect(&rectF);
	rect.left = ROUND(rectF.left);
	rect.right = ROUND(rectF.right);
	rect.top = ROUND(rectF.top);	
	rect.bottom = ROUND(rectF.bottom);

	if (nHandle == -9)
	{
		if (point.x > rect.right - 1)
			point.x = rect.right - 1;
		else if (point.x < (rect.left + rect.right) / 2)
			point.x = (rect.left + rect.right) / 2;
		if (point.y > rect.bottom - 1)
			point.y = rect.bottom - 1;
		else if (point.y < (rect.top + rect.bottom) / 2)
			point.y = (rect.top + rect.bottom) / 2;
		m_roundness.x = float(2 * (rect.right - point.x));
		m_roundness.y = float(2 * (rect.bottom - point.y));
	
		delete m_pObjectPath;
		InitRoundRectPath(&m_pObjectPath, rect.left, rect.top, rect.right, 
				rect.bottom, ROUND(m_roundness.x), ROUND(m_roundness.y));
		MoveTo(rectF, FALSE);
	
		return S_OK;
	}

	return ICxDrawObjectImpl<CDrawRoundRectangle>::MoveHandleTo(nHandle, point);
}

/////////////////////////////////////////////////////////////////////////////////////////////
//功能: 取对象手柄点的坐标值
//参数: nHandle[in] 对象手柄点的编号
//      pHandlePoint[out] 返回指定手柄点的坐标
//      pHandleAsHandle[out] 返回手柄点数目
////////////////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawRoundRectangle::GetHandle(long nHandle, POINT* pHandlePoint)
{
	RECTF rcPos;
	GetPositionRect(&rcPos);

	if (nHandle == -9)
	{
		ATLASSERT(m_roundness.x >= 0 && m_roundness.y >= 0);
		pHandlePoint->x = int(rcPos.right - m_roundness.x / 2);
		pHandlePoint->y = int(rcPos.bottom - m_roundness.y / 2);
		return S_OK;
	}

	return ICxDrawObjectImpl<CDrawRoundRectangle>::GetHandle(nHandle, pHandlePoint);
}

////////////////////////////////////////////////////////////////////////////
//功能: 获取对象手柄数目
//参数: pHandleCount[in] 返回对象手柄数目
///////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawRoundRectangle::GetHandleCount(long* pHandleCount)
{
	if (m_roundness.x >= 0 && m_roundness.y >= 0)
	{
		RECTF rcPos;
		GetPositionRect(&rcPos);

		if (rcPos.right - rcPos.left > 1  && rcPos.bottom - rcPos.top > 1)
		{
			*pHandleCount = -9;
			return S_OK;
		}
	}

	*pHandleCount = -8;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////////////
//功能: 取光标
//参数: nHandle[in] 对象手柄点的编号
//      pHandleCursor[out] 光标句柄
/////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawRoundRectangle::GetHandleCursor(long nHandle, HCURSOR* pHandleCursor)
{
	if (nHandle == -9)
	{
		*pHandleCursor = LoadCursor(NULL, IDC_CROSS);
		return S_OK;
	}
	return ICxDrawObjectImpl<CDrawRoundRectangle>::GetHandleCursor(nHandle, pHandleCursor);
}

STDMETHODIMP CDrawRoundRectangle::Rotate(long lAngle)
{
	m_roundness.x = m_roundness.y = -1;
	return ICxDrawObjectImpl<CDrawRoundRectangle>::Rotate(lAngle);
}

STDMETHODIMP CDrawRoundRectangle::RotateAt(POINTF point, float fAngle)
{
	m_roundness.x = m_roundness.y = -1;
	return ICxDrawObjectImpl<CDrawRoundRectangle>::RotateAt(point, fAngle);
}

////////////////////////////////////////////////////////////////////
//功能: 取对象名字串
////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawRoundRectangle::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("RoundRect");
	return S_OK;
}

STDMETHODIMP CDrawRoundRectangle::get_RoundNessX(/*[out, retval]*/ float *pVal)
{
	*pVal = m_roundness.x;
	return S_OK;
}

STDMETHODIMP CDrawRoundRectangle::put_RoundNessX(/*[in]*/ float newVal)
{
	m_roundness.x = newVal;
	ReSetObjectPath();
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawRoundRectangle::get_RoundNessY(/*[out, retval]*/ float *pVal)
{
	*pVal = m_roundness.y;
	return S_OK;
}

STDMETHODIMP CDrawRoundRectangle::put_RoundNessY(/*[in]*/ float newVal)
{
	m_roundness.y = newVal;
	ReSetObjectPath();
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawRoundRectangle::put_RotateAngle(float newVal)
{
	using namespace Gdiplus;
	if (ClientIsRunMode())
	{
		m_fRotateAngle = newVal;
		ReSetObjectPath();
	}
	else
	{
		m_roundness.x = m_roundness.y = -1;
		Matrix mtTansMatrix;
		mtTansMatrix.RotateAt(float(newVal), m_ptRotateBase);
		m_pObjectPath->Transform(&mtTansMatrix);
	}
	FireViewChange();
	return S_OK;
}