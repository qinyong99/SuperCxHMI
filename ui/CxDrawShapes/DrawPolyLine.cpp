// DrawPolyline.cpp : Implementation of CDrawPolyline
// 所属项目: AutoCx的矩形对象
// 创建日期: 2001.10.8
// 创 建 人：卢文江 
// 功能描述：
// 修改记录：
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxDrawShapes.h"
#include "DrawPolyline.h"

void CDrawPolyline::AddObjectToPath()
{
//	m_pObjectPath->AddLines(m_points, m_nPoints);
}

HRESULT CDrawPolyline::DoDraw(HDC hdcDest)
{
	using namespace Gdiplus;

	Graphics graphics(hdcDest);
	DrawObjectEdge(&graphics, m_pObjectPath);

	return S_OK;
}

HRESULT CDrawPolyline::DoPrint(HDC hdcDest)
{
	using namespace Gdiplus;
	
	Graphics graphics(hdcDest);
	graphics.SetPageUnit(UnitPixel);
	DrawObjectEdge(&graphics, m_pObjectPath);
	
	return S_OK;
}

HRESULT CDrawPolyline::DoLoad(DWORD dwVer, LPSTREAM pStm)
{
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

HRESULT CDrawPolyline::DoSave(DWORD dwVer, LPSTREAM pStm)
{
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

STDMETHODIMP CDrawPolyline::Intersects(RECT rect, BOOL* pbIntersects)
{
	using namespace Gdiplus;

	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);
	for (int i = 0; i < nCount - 1; i++)
	{
		double Distance = sqrt((pPoints[i].X - pPoints[i + 1].X) * (pPoints[0].X - pPoints[i + 1].X) +
						(pPoints[i].Y - pPoints[i + 1].Y) * (pPoints[0].Y - pPoints[i + 1].Y));
		long OffsetX = (long)((pPoints[i + 1].Y - pPoints[i].Y) * (i + m_fEdgeWidth / 2) / Distance);
		long OffsetY = (long)((-pPoints[i + 1].X + pPoints[i].X) * (4 + m_fEdgeWidth / 2) / Distance);

		POINT poly[4];
		poly[1].x = pPoints[i].X + OffsetX;
		poly[1].y = pPoints[i].Y + OffsetY;
		poly[2].x = pPoints[i].X - OffsetX;
		poly[2].y = pPoints[i].Y - OffsetY;
		poly[3].x = pPoints[i + 1].X - OffsetX;
		poly[3].y = pPoints[i + 1].Y - OffsetY;
		poly[0].x = pPoints[i + 1].X + OffsetX;
		poly[0].y = pPoints[i + 1].Y + OffsetY;
		HRGN hRgn = ::CreatePolygonRgn(poly, 4, ALTERNATE);
		*pbIntersects = ::RectInRegion(hRgn, &rect);
		::DeleteObject(hRgn);
		if (*pbIntersects)
			break;
	}
	
	delete [] pPoints;

	return S_OK;
}

STDMETHODIMP CDrawPolyline::HitTest(POINT point, BOOL* pbHitTest)
{
	using namespace Gdiplus;
	
	DashStyle dsPen = (DashStyle)m_enumEdgeStyle;
	if (m_enumEdgeStyle == EdgeStylePipe)
		dsPen = Gdiplus::DashStyleSolid;
	float fPenWidth = m_fEdgeWidth;
	if (fPenWidth < 4)
		fPenWidth = 4;

	if (m_enumEdgeStyle == EdgeStyleTransparence)
		fPenWidth = 0;

	Pen EdgePen(m_clrEdgeColor, fPenWidth);
	if (m_enumLineJoinStyle == LineJoinStyleRound)
	{
		EdgePen.SetLineJoin(LineJoinRound);
		EdgePen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);
	}
	else if (m_enumLineJoinStyle == LineJoinStyleSquare)
	{
		EdgePen.SetLineJoin(LineJoinMiter);
		EdgePen.SetLineCap(LineCapSquare, LineCapSquare, DashCapFlat);
	}
	EdgePen.SetDashStyle(dsPen);

	*pbHitTest = m_pObjectPath->IsOutlineVisible(point.x, point.y, &EdgePen);

	return S_OK;
}

HRESULT CDrawPolyline::DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
{
	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawPolyline>::DrawTrackerHandleShape(hdcDest, nHandle, point);

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
	}
	delete [] pPoints;

	return S_OK;
}

HRESULT CDrawPolyline::DrawTrackerShape(HDC hdcDest, POINT* points, int ptcount)
{
	using namespace Gdiplus;
	if (points != NULL && ptcount != 0)
	{
		::Polyline(hdcDest, points, ptcount);
	}
	else
	{
		int nCount = m_pObjectPath->GetPointCount();
		Point* pPoints = new Point[nCount];
		POINT* pPOINTS = new POINT[nCount];
		m_pObjectPath->GetPathPoints(pPoints, nCount);
		for (int i = 0; i < nCount; i++)
		{
			pPOINTS[i].x = pPoints[i].X;
			pPOINTS[i].y = pPoints[i].Y;
		}
		::Polyline(hdcDest, (POINT*)pPOINTS, nCount);
		delete [] pPoints;
		delete [] pPOINTS;
	}
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//功能：增加点
////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPolyline::AddPoint(IFloatPoint *pPoint)
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
	m_pObjectPath->AddLines(pPoints, nCount + 1);
	m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
	m_ptRotateBase.X =  rcPos.X + rcPos.Width / 2;
	m_ptRotateBase.Y =  rcPos.Y + rcPos.Height / 2;

	delete [] pPoints;
	
	FireViewChange();

	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////////////////////
//功能: 取对象手柄点的坐标值
//参数: nHandle[in] 对象手柄点的编号
//      pHandlePoint[out] 返回指定手柄点的坐标
//      pHandleAsHandle[out] 返回手柄点数目
////////////////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPolyline::GetHandle(long nHandle, POINT* pHandlePoint)
{
	using namespace Gdiplus;

	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawPolyline>::GetHandle(nHandle, pHandlePoint);

	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);
	pHandlePoint->x = pPoints[nHandle - 1].X;
	pHandlePoint->y = pPoints[nHandle - 1].Y;
	delete [] pPoints;
	return S_OK;
}

////////////////////////////////////////////////////////////////////////////////////////
//功能：取对象手柄数目
////////////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPolyline::GetHandleCount(long* pHandleCount)
{
	*pHandleCount = m_pObjectPath->GetPointCount();
	
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////////////////
//功能: 移动对象手柄至指定位置
//参数: nHandle[in] 指定手柄
//		point[in] 新位置
/////////////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPolyline::MoveHandleTo(long nHandle, POINT point)
{	
	using namespace Gdiplus;

	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawPolyline>::MoveHandleTo(nHandle, point);

	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);
	pPoints[nHandle - 1].X = float(point.x);
	pPoints[nHandle - 1].Y = float(point.y);
	m_pObjectPath->Reset();
	m_pObjectPath->AddLines(pPoints, nCount);
	RectF rcPos;
	m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
	m_ptRotateBase.X =  rcPos.X + rcPos.Width / 2;
	m_ptRotateBase.Y =  rcPos.Y + rcPos.Height / 2;

	delete [] pPoints;
	return S_OK;
}

STDMETHODIMP CDrawPolyline::GetPointAt(long lIndex, IFloatPoint **ppPoint)
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

////////////////////////////////////////////////////////////////
//功能: 取对象名字字串
////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPolyline::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("Polyline");
	return S_OK;
}

////////////////////////////////////////////////////////////////////////////////
//功能: 取对象手柄的光标句柄
////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPolyline::GetHandleCursor(long nHandle, HCURSOR* pHandleCursor)
{
	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawPolyline>::GetHandleCursor(nHandle, pHandleCursor);

	*pHandleCursor = LoadCursor(NULL, IDC_CROSS);
	return S_OK;
}

STDMETHODIMP CDrawPolyline::GetNumberOfPoints(long *pVal)
{
	using namespace Gdiplus;
	*pVal = m_pObjectPath->GetPointCount();

	return S_OK;
}


STDMETHODIMP CDrawPolyline::DeletePoint(long lIndex)
{
	using namespace Gdiplus;

	FireViewChange();

	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount - 1];
	m_pObjectPath->GetPathPoints(pPoints, nCount - 1);
	m_pObjectPath->Reset();
	m_pObjectPath->AddLines(pPoints, nCount - 1);
	RectF rcPos;
	m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
	m_ptRotateBase.X =  rcPos.X + rcPos.Width / 2;
	m_ptRotateBase.Y =  rcPos.Y + rcPos.Height / 2;

	delete [] pPoints;
	
	return S_OK;
}







