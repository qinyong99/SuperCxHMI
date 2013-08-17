//////////////////////////////////////////////////////////////////////////////
// DrawLine.cpp : Implementation of CDrawLine
// 所属项目: AutoCx的园角矩形对象
// 创建日期: 2001.10.8
// 创 建 人：卢文江 
// 功能描述：
// 修改记录：
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxDrawShapes.h"
#include "DrawLine.h"
#include "math.h"

void CDrawLine::AddObjectToPath()
{
	m_pObjectPath->AddLine(0, 0, 200, 200);
}

HRESULT CDrawLine::DoDraw(HDC hdcDest)
{
	using namespace Gdiplus;

	Graphics graphics(hdcDest);
	DrawObjectEdge(&graphics, m_pObjectPath);

	return S_OK;
}

HRESULT CDrawLine::DoPrint(HDC hdcDest)
{
	using namespace Gdiplus;
	
	Graphics graphics(hdcDest);
	graphics.SetPageUnit(UnitPixel);
	DrawObjectEdge(&graphics, m_pObjectPath);
	
	return S_OK;
}

HRESULT CDrawLine::DoLoad(DWORD dwVer, LPSTREAM pStm)
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

HRESULT CDrawLine::DoSave(DWORD dwVer, LPSTREAM pStm)
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

HRESULT CDrawLine::DrawTrackerShape(HDC hdcDest, POINT* points, int ptcount)
{
	if (points != NULL && ptcount != 0)
	{
		MoveToEx(hdcDest, points[0].x, points[0].y, NULL);
		LineTo(hdcDest, ROUND(points[1].x), ROUND(points[1].y));
		return S_OK;
	}

	return ICxDrawObjectImpl<CDrawLine>::DrawTrackerShape(hdcDest, points, ptcount);
}

STDMETHODIMP CDrawLine::DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
{
	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawLine>::DrawTrackerHandleShape(hdcDest, nHandle, point);

	using namespace Gdiplus;
	Point points[2];
	m_pObjectPath->GetPathPoints(points, 2);

	::MoveToEx(hdcDest, point.x, point.y, NULL);
	if (nHandle == 1)
		::LineTo(hdcDest, points[1].X, points[1].Y);
	else if (nHandle == 2)
		::LineTo(hdcDest, points[0].X, points[0].Y);

	return S_OK;
}

/////////////////////////////////////////////////////////////
//功能: 移动手柄点至新位置
//参数: nHandle[in] 手柄编号
//		point[in] 新的手柄位置
/////////////////////////////////////////////////////////////
STDMETHODIMP CDrawLine::MoveHandleTo(long nHandle, POINT point)
{
	using namespace Gdiplus;

	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawLine>::MoveHandleTo(nHandle, point);

	PointF points[2];
	m_pObjectPath->GetPathPoints(points, 2);
	points[nHandle - 1].X = float(point.x);
	points[nHandle - 1].Y = float(point.y);
	m_pObjectPath->Reset();
	m_pObjectPath->AddLine(points[0].X, points[0].Y, points[1].X, points[1].Y);
	RectF rcPos;
	m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
	m_ptRotateBase.X =  rcPos.X + rcPos.Width / 2;
	m_ptRotateBase.Y =  rcPos.Y + rcPos.Height / 2;

	return S_OK;
}

///////////////////////////////////////////////////////////////
//功能: 获取对象手柄的数目
//////////////////////////////////////////////////////////////
STDMETHODIMP CDrawLine::GetHandleCount(long* pHandleCount)
{
	*pHandleCount = 2;
	
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//功能: 取对象手柄点的坐标值
//参数: nHandle[in] 对象手柄点的编号
//      pHandlePoint[out] 返回指定手柄点的坐标
//      pHandleAsHandle[out] 返回手柄点数目
////////////////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawLine::GetHandle(long nHandle, POINT* pHandlePoint)
{
	using namespace Gdiplus;

	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawLine>::GetHandle(nHandle, pHandlePoint);

	Point points[2];
	m_pObjectPath->GetPathPoints(points, 2);
	pHandlePoint->x = points[nHandle - 1].X;
	pHandlePoint->y = points[nHandle - 1].Y;
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////
//功能: 取指定点的手柄光标
//参数: nHandle[in] 对象手柄编号
//		pHandleCursor[out] 光标句柄
///////////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawLine::GetHandleCursor(long nHandle, HCURSOR* pHandleCursor)
{
	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawLine>::GetHandleCursor(nHandle, pHandleCursor);

	*pHandleCursor = LoadCursor(NULL, IDC_CROSS);
	return S_OK;
}

STDMETHODIMP CDrawLine::HitTest(POINT point, BOOL* pbHitTest)
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

///////////////////////////////////////////////////////////
//功能: 判定指定矩形是否与对象区域相交
//参数: rect[in] 指定矩形
//      pIntersected[out] 相交返回ture, 否则返回false
///////////////////////////////////////////////////////////
STDMETHODIMP CDrawLine::Intersects(RECT rect, BOOL* bIntersects)
{
	using namespace Gdiplus;
	Point points[2];
	m_pObjectPath->GetPathPoints(points, 2);

	double Distance = sqrt((points[0].X - points[1].X) * (points[0].X - points[1].X) +
					(points[0].Y - points[1].Y) * (points[0].Y - points[1].Y));
	long OffsetX = (long)((points[1].Y - points[0].Y) * (4 + m_fEdgeWidth / 2) / Distance);
	long OffsetY = (long)((-points[1].X + points[0].X) * (4 + m_fEdgeWidth / 2) / Distance);

	POINT poly[4];
	poly[1].x = points[0].X + OffsetX;
	poly[1].y = points[0].Y + OffsetY;
	poly[2].x = points[0].X - OffsetX;
	poly[2].y = points[0].Y - OffsetY;
	poly[3].x = points[1].X - OffsetX;
	poly[3].y = points[1].Y - OffsetY;
	poly[0].x = points[1].X + OffsetX;
	poly[0].y = points[1].Y + OffsetY;
	HRGN hRgn = ::CreatePolygonRgn(poly, 4, ALTERNATE);
	*bIntersects = ::RectInRegion(hRgn, &rect);
	::DeleteObject(hRgn);

	return S_OK;
}

//////////////////////////////////////////////////////////
//功能: 取对象名字
//参数: pbsrtClassName[in] 返回对象名字字串
//////////////////////////////////////////////////////////
STDMETHODIMP CDrawLine::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("Line");
	return S_OK;
}

STDMETHODIMP CDrawLine::get_StartX(float *pVal)
{
	using namespace Gdiplus;

	PointF points[2];
	m_pObjectPath->GetPathPoints(points, 2);

	*pVal = points[0].X;
	return S_OK;
}

STDMETHODIMP CDrawLine::put_StartX(float newVal)
{
	using namespace Gdiplus;
	PointF points[2];
	m_pObjectPath->GetPathPoints(points, 2);
	if (points[0].X == newVal)
		return S_OK;

	FireViewChange();
	m_pObjectPath->Reset();
	points[0].X = newVal;
	m_pObjectPath->AddLine(points[0].X, points[0].Y, points[1].X, points[1].Y);
	RectF rcPos;
	m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
	m_ptRotateBase.X =  rcPos.X + rcPos.Width / 2;
	m_ptRotateBase.Y =  rcPos.Y + rcPos.Height / 2;

	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawLine::get_StartY(float *pVal)
{
	using namespace Gdiplus;

	PointF points[2];
	m_pObjectPath->GetPathPoints(points, 2);

	*pVal = points[0].Y;
	return S_OK;
}

STDMETHODIMP CDrawLine::put_StartY(float newVal)
{
	using namespace Gdiplus;
	PointF points[2];
	m_pObjectPath->GetPathPoints(points, 2);
	if (points[0].Y == newVal)
		return S_OK;

	FireViewChange();
	m_pObjectPath->Reset();
	points[0].Y = newVal;
	m_pObjectPath->AddLine(points[0].X, points[0].Y, points[1].X, points[1].Y);
	RectF rcPos;
	m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
	m_ptRotateBase.X =  rcPos.X + rcPos.Width / 2;
	m_ptRotateBase.Y =  rcPos.Y + rcPos.Height / 2;

	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawLine::get_EndX(float *pVal)
{
	using namespace Gdiplus;

	PointF points[2];
	m_pObjectPath->GetPathPoints(points, 2);

	*pVal = points[1].X;
	return S_OK;
}

STDMETHODIMP CDrawLine::put_EndX(float newVal)
{
	using namespace Gdiplus;
	PointF points[2];
	m_pObjectPath->GetPathPoints(points, 2);
	if (points[1].X == newVal)
		return S_OK;
	
	FireViewChange();
	m_pObjectPath->Reset();
	points[1].X = newVal;
	m_pObjectPath->AddLine(points[0].X, points[0].Y, points[1].X, points[1].Y);
	RectF rcPos;
	m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
	m_ptRotateBase.X =  rcPos.X + rcPos.Width / 2;
	m_ptRotateBase.Y =  rcPos.Y + rcPos.Height / 2;

	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawLine::get_EndY(float *pVal)
{
	using namespace Gdiplus;

	PointF points[2];
	m_pObjectPath->GetPathPoints(points, 2);

	*pVal = points[1].Y;
	return S_OK;
}

STDMETHODIMP CDrawLine::put_EndY(float newVal)
{
	using namespace Gdiplus;
	PointF points[2];
	m_pObjectPath->GetPathPoints(points, 2);
	if (points[1].Y == newVal)
		return S_OK;

	FireViewChange();
	m_pObjectPath->Reset();
	points[1].Y = newVal;
	m_pObjectPath->AddLine(points[0].X, points[0].Y, points[1].X, points[1].Y);
	RectF rcPos;
	m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
	m_ptRotateBase.X =  rcPos.X + rcPos.Width / 2;
	m_ptRotateBase.Y =  rcPos.Y + rcPos.Height / 2;

	FireViewChange();
	return S_OK;
}


