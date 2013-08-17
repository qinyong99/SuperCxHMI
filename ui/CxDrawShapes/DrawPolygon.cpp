// DrawPolygon.cpp : Implementation of CDrawPolygon
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "DrawPolygon.h"

void CDrawPolygon::AddObjectToPath()
{
}

HRESULT CDrawPolygon::DoLoad(DWORD dwVer, LPSTREAM pStm)
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

HRESULT CDrawPolygon::DoSave(DWORD dwVer, LPSTREAM pStm)
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

HRESULT CDrawPolygon::DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
{
	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawPolygon>::DrawTrackerHandleShape(hdcDest, nHandle, point);

	using namespace Gdiplus;
	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	ATLASSERT(nHandle >= 1 && nHandle <= nCount);
//	if (pPoints[nHandle - 1].X != point.x && pPoints[nHandle - 1].Y != point.y)
	{
		nHandle -= 1;
		::MoveToEx(hdcDest, point.x, point.y, NULL);
		if (nHandle > 0)
			::LineTo(hdcDest, pPoints[nHandle - 1].X, pPoints[nHandle - 1].Y);
		::MoveToEx(hdcDest, point.x, point.y, NULL);
		if (nHandle < nCount - 1)
			::LineTo(hdcDest, pPoints[nHandle + 1].X, pPoints[nHandle + 1].Y);
		if (nHandle == 0)
		{
			::MoveToEx(hdcDest, point.x, point.y, NULL);
			::LineTo(hdcDest, pPoints[nCount - 1].X, pPoints[nCount - 1].Y);
		}
		else if (nHandle == nCount - 1)
		{
			::MoveToEx(hdcDest, point.x, point.y, NULL);
			::LineTo(hdcDest, pPoints[0].X, pPoints[0].Y);
		}
	}
	delete [] pPoints;

	return S_OK;
}

HRESULT CDrawPolygon::DrawTrackerShape(HDC hdcDest, POINT* points, int ptcount)
{
	using namespace Gdiplus;
	if (points != NULL && ptcount != 0)
		::Polygon(hdcDest, points, ptcount);
	else
		ICxDrawObjectImpl<CDrawPolygon>::DrawTrackerShape(hdcDest, points, ptcount);
	
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//功能: 接口, 增加点
////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPolygon::AddPoint(IFloatPoint *pPoint)
{
	using namespace Gdiplus;

	PointF point;
	float fTemp;
	pPoint->get_X(&fTemp);
	point.X = fTemp;
	pPoint->get_Y(&fTemp);
	point.Y = fTemp;

	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount + 1];
	m_pObjectPath->GetPathPoints(pPoints, nCount);
//	BYTE* pTypes = new BYTE[nCount + 1];
//	m_pObjectPath->GetPathTypes(pTypes, nCount);
	pPoints[nCount] = point;
//	pTypes[nCount] = 1;
	m_pObjectPath->Reset();
	RectF rcPos;
	if (nCount > 1)
		m_pObjectPath->AddPolygon(pPoints, nCount + 1);
	else
		m_pObjectPath->AddLines(pPoints, nCount + 1);
	m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
	m_ptRotateBase.X =  rcPos.X + rcPos.Width / 2;
	m_ptRotateBase.Y =  rcPos.Y + rcPos.Height / 2;

	delete [] pPoints;
	
	FireViewChange();

	return S_OK;
}


STDMETHODIMP CDrawPolygon::GetNumberOfPoints(long *pVal)
{
	using namespace Gdiplus;
	*pVal = m_pObjectPath->GetPointCount();

	return S_OK;
}

STDMETHODIMP CDrawPolygon::DeletePoint(long lIndex)
{
	using namespace Gdiplus;

	FireViewChange();
	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount - 1];
	m_pObjectPath->GetPathPoints(pPoints, nCount - 1);
	m_pObjectPath->Reset();
	if (nCount > 3)
		m_pObjectPath->AddPolygon(pPoints, nCount - 1);
	else
		m_pObjectPath->AddLines(pPoints, nCount - 1);

	RectF rcPos;
	m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
	m_ptRotateBase.X =  rcPos.X + rcPos.Width / 2;
	m_ptRotateBase.Y =  rcPos.Y + rcPos.Height / 2;

	delete [] pPoints;
	
	return S_OK;
}

STDMETHODIMP CDrawPolygon::Intersects(RECT rect, BOOL* pbIntersects)
{
	using namespace Gdiplus;

	RectF rcPosition;
	m_pObjectPath->GetBounds(&rcPosition, NULL, NULL);
	
	if (rcPosition.Width == 0)
	{
		if (rcPosition.Y <= rect.bottom &&  rcPosition.Y + rcPosition.Height >= rect.top &&
			rcPosition.X <= rect.right &&  rcPosition.X >= rect.left)
		{
			*pbIntersects = TRUE;
			return S_OK;
		}
	}
	if (rcPosition.Height == 0)
	{
		if (rcPosition.X <= rect.right &&  rcPosition.X + rcPosition.Width >= rect.left &&
			rcPosition.Y <= rect.bottom &&  rcPosition.Y >= rect.top) 
		{
			*pbIntersects = TRUE;
			return S_OK;
		}
	}

	return ICxDrawObjectImpl<CDrawPolygon>::Intersects(rect, pbIntersects);
}

////////////////////////////////////////////////////////////////////////
//功能: 获取对象指定手柄点坐标
//参数: nHandle[in] 手柄编号
//      pHandlePoint[out] 返回手柄点位置
//		pHandleAsHandle[out] 返回
////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPolygon::GetHandle(long nHandle, POINT* pHandlePoint)
{
	using namespace Gdiplus;

	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawPolygon>::GetHandle(nHandle, pHandlePoint);

	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);
	pHandlePoint->x = pPoints[nHandle - 1].X;
	pHandlePoint->y = pPoints[nHandle - 1].Y;
	delete [] pPoints;
	return S_OK;
}

////////////////////////////////////////////////////////////////////////////////////////
//功能: 取对象手柄数目
////////////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPolygon::GetHandleCount(long* pHandleCount)
{
	*pHandleCount = m_pObjectPath->GetPointCount();
	
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
//功能: 移动对象手柄至指定点
//参数: nHandle[in] 对象手柄号
//		point[in] 指定点
//////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPolygon::MoveHandleTo(long nHandle, POINT point)
{	
	using namespace Gdiplus;

	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawPolygon>::MoveHandleTo(nHandle, point);

	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);
	pPoints[nHandle - 1].X = float(point.x);
	pPoints[nHandle - 1].Y = float(point.y);
	m_pObjectPath->Reset();
	if (nCount > 2)
		m_pObjectPath->AddPolygon(pPoints, nCount);
	else
		m_pObjectPath->AddLines(pPoints, nCount);
	RectF rcPos;
	m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
	m_ptRotateBase.X =  rcPos.X + rcPos.Width / 2;
	m_ptRotateBase.Y =  rcPos.Y + rcPos.Height / 2;

	delete [] pPoints;
	return S_OK;
}


/////////////////////////////////////////////////////////////////////
//功能: 取对象名字串
/////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPolygon::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("Polygon");
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//功能: 取手柄光标
///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPolygon::GetHandleCursor(long nHandle, HCURSOR* pHandleCursor)
{
	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawPolygon>::GetHandleCursor(nHandle, pHandleCursor);

	*pHandleCursor = LoadCursor(NULL, IDC_CROSS);
	return S_OK;
}

STDMETHODIMP CDrawPolygon::GetPointAt(long lIndex, IFloatPoint **ppPoint)
{
	using namespace Gdiplus;
	
	IFloatPoint* pFloatPoint = NULL;
	HRESULT hr = CoCreateInstance(CLSID_FloatPoint, NULL, CLSCTX_INPROC_SERVER, IID_IFloatPoint, (void**)&pFloatPoint);
	if (FAILED(hr))
		return hr;
	
	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);
	pFloatPoint->put_X(pPoints[lIndex].X);
	pFloatPoint->put_Y(pPoints[lIndex].Y);
	delete [] pPoints;
	
	*ppPoint = pFloatPoint;
	
	return S_OK;
}

