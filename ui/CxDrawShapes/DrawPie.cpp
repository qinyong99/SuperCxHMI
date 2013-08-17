// DrawPie.cpp : Implementation of CDrawPie
// 所属项目: AutoCx的扇形对象
// 创建日期: 2001.10.8
// 创 建 人：卢文江 
// 功能描述：
// 修改记录：
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxDrawShapes.h"
#include "DrawPie.h"
#include "math.h"

#define DegToRad 0.0174533
#define RadToDeg 57.29578

void CDrawPie::InitPiePath(Gdiplus::GraphicsPath** ppObjectPath,
		POINT* points, enumPieType type)
{
	float fRadius = (float)sqrt((points[2].x - points[0].x) * (points[2].x - points[0].x) + 
			(points[2].y - points[0].y) * (points[2].y - points[0].y)); 

	float fStartAngle = -(float)CalculateAngle(points[0], points[1]);
	float fEndAngle = -(float)CalculateAngle(points[0], points[2]);
	float fSweepAngle = fEndAngle - fStartAngle;
	if (fSweepAngle < 0)
		fSweepAngle += 360;
	if ((type == PieTypeWedge && fabs(fSweepAngle) > 180) || (type == PieTypeCutOut && fabs(fSweepAngle) < 180))	
		fSweepAngle = float(fabs(fSweepAngle) - 360);

	*ppObjectPath = new Gdiplus::GraphicsPath;
	if (fSweepAngle == 0)
		fSweepAngle = 0.1f;
	(*ppObjectPath)->AddPie(points[0].x - fRadius, points[0].y - fRadius, 
			2 * fRadius, 2 * fRadius, fStartAngle, fSweepAngle);
}

HRESULT CDrawPie::DoLoad(DWORD dwVer, LPSTREAM pStm)
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

	pStm->Read(&m_enumPieType, sizeof(m_enumPieType), NULL);
	
	pStm->Read(&m_enumMousePointer, sizeof(m_enumMousePointer), NULL);
	m_bstrDescription.ReadFromStream(pStm);
	
	return CCxDrawObjectBase::DoLoad(dwVer, pStm);
}

HRESULT CDrawPie::DoSave(DWORD dwVer, LPSTREAM pStm)
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

	pStm->Write(&m_enumPieType, sizeof(m_enumPieType), NULL);
	
	pStm->Write(&m_enumMousePointer, sizeof(m_enumMousePointer), NULL);
	m_bstrDescription.WriteToStream(pStm);
	
	return CCxDrawObjectBase::DoSave(dwVer, pStm);
}

HRESULT CDrawPie::DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
{
	using namespace Gdiplus;

	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawPie>::DrawTrackerHandleShape(hdcDest, nHandle, point);

	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINT points[3];

	switch (nHandle)
	{
		case 1:
			points[0] = point;	
			points[1].x =  pPoints[1].X;
			points[1].y =  pPoints[1].Y;
			points[2].x =  pPoints[nCount - 1].X;
			points[2].y =  pPoints[nCount - 1].Y;
			break;
		case 2:
			points[0].x =  pPoints[0].X;
			points[0].y =  pPoints[0].Y;
			points[1].x =  pPoints[nCount - 1].X;
			points[1].y =  pPoints[nCount - 1].Y;
			points[2] = point;
			break;
		case 3:
			points[0].x =  pPoints[0].X;
			points[0].y =  pPoints[0].Y;
			points[1].x =  pPoints[1].X;
			points[1].y =  pPoints[1].Y;
			points[2] = point;
			break;
		default:
			return S_FALSE;
	}
	delete [] pPoints;
	GraphicsPath* pObjectPath = NULL;
	InitPiePath(&pObjectPath, points, m_enumPieType);
	PathData pathData;
	pObjectPath->GetPathData(&pathData);
	DrawPathDataShape(hdcDest, pathData);
	delete pObjectPath;

	return S_OK;
}

HRESULT CDrawPie::DrawTrackerShape(HDC hdcDest, POINT* points, int nPoint)
{
	using namespace Gdiplus;

	if (nPoint == 2)
	{
		MoveToEx(hdcDest, points[0].x, points[0].y, NULL);
		LineTo(hdcDest, points[1].x, points[1].y);
	}
	else if (nPoint == 3)
	{
		GraphicsPath* pObjectPath = NULL;
		InitPiePath(&pObjectPath, points, m_enumPieType);
		PathData pathData;
		pObjectPath->GetPathData(&pathData);
		DrawPathDataShape(hdcDest, pathData);
		delete pObjectPath;

	}
	else
	{
		return ICxDrawObjectImpl<CDrawPie>::DrawTrackerShape(hdcDest, points, nPoint);
	}

	return S_OK;
}

////////////////////////////////////////////////////////////////
//功能: 取对象手柄数目
///////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPie::GetHandleCount(long* pHandleCount)
{
	*pHandleCount = 3;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//功能: 取对象手柄点的坐标值
//参数: nHandle[in] 对象手柄点的编号
//      pHandlePoint[out] 返回指定手柄点的坐标
//      pHandleAsHandle[out] 返回手柄点数目
////////////////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPie::GetHandle(long nHandle, POINT* pHandlePoint)
{
	using namespace Gdiplus;

	if (nHandle < 0)
	{
		return ICxDrawObjectImpl<CDrawPie>::GetHandle(nHandle, pHandlePoint);
	}

	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	switch (nHandle)
	{
		case 1:
		{
			pHandlePoint->x = pPoints[0].X;
			pHandlePoint->y = pPoints[0].Y;
			break;
		}
		case 2:
			pHandlePoint->x = pPoints[1].X;
			pHandlePoint->y = pPoints[1].Y;
			break;
		case 3:
		{
			pHandlePoint->x = pPoints[nCount - 1].X;
			pHandlePoint->y = pPoints[nCount - 1].Y;
			break;
		}
		default:
			return S_FALSE;
	}
	delete [] pPoints;

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////
//功能: 获取手柄点光标
//////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPie::GetHandleCursor(long nHandle, HCURSOR* pHandleCursor)
{
	if (nHandle < 0)
	{
		return ICxDrawObjectImpl<CDrawPie>::GetHandleCursor(nHandle, pHandleCursor);
	}
	*pHandleCursor = LoadCursor(NULL, IDC_CROSS);
	return S_OK;
}

///////////////////////////////////////////////////////////////////
//功能: 移动手柄点至新位置
//参数: nHandle[in] 手柄点编号
//		point[in] 新位置点坐标
//////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPie::MoveHandleTo(long nHandle, POINT point)
{
	using namespace Gdiplus;

	if (nHandle < 0)
	{
		return ICxDrawObjectImpl<CDrawPie>::MoveHandleTo(nHandle, point);
	}

	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINT points[3];

	switch (nHandle)
	{
		case 1:
			points[0] = point;	
			points[1].x =  pPoints[1].X;
			points[1].y =  pPoints[1].Y;
			points[2].x =  pPoints[nCount - 1].X;
			points[2].y =  pPoints[nCount - 1].Y;
			break;
		case 2:
			points[0].x =  pPoints[0].X;
			points[0].y =  pPoints[0].Y;
			points[1].x =  pPoints[nCount - 1].X;
			points[1].y =  pPoints[nCount - 1].Y;
			points[2] = point;
			break;
		case 3:
			points[0].x =  pPoints[0].X;
			points[0].y =  pPoints[0].Y;
			points[1].x =  pPoints[1].X;
			points[1].y =  pPoints[1].Y;
			points[2] = point;
			break;
		default:
			return S_FALSE;
	}
	delete [] pPoints;
	delete m_pObjectPath;
	InitPiePath(&m_pObjectPath, points, m_enumPieType);
	ReSetRotateBasePoint();

	return S_OK;
}

STDMETHODIMP CDrawPie::MoveHandlesTo(POINT* pPoints, int nCount)
{
	ATLASSERT(nCount == 3);

	delete m_pObjectPath;
	InitPiePath(&m_pObjectPath, pPoints, m_enumPieType);
	ReSetRotateBasePoint();
	//FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawPie::InitNew()
{
	m_enumPieType = PieTypeWedge;
	return ICxDrawObjectImpl<CDrawPie>::InitNew();
}


STDMETHODIMP CDrawPie::Intersects(RECT rect, BOOL* pbIntersects)
{
	using namespace Gdiplus;

	RectF rcPosition;
	m_pObjectPath->GetBounds(&rcPosition, NULL, NULL);
	
	if (rcPosition.Width < 0.5)
	{
		if (rcPosition.Y <= rect.bottom &&  rcPosition.Y + rcPosition.Height >= rect.top &&
			rcPosition.X <= rect.right &&  rcPosition.X >= rect.left)
		{
			*pbIntersects = TRUE;
			return S_OK;
		}
	}
	if (rcPosition.Height < 0.5)
	{
		if (rcPosition.X <= rect.right &&  rcPosition.X + rcPosition.Width >= rect.left &&
			rcPosition.Y <= rect.bottom &&  rcPosition.Y >= rect.top) 
		{
			*pbIntersects = TRUE;
			return S_OK;
		}
	}

	return ICxDrawObjectImpl<CDrawPie>::Intersects(rect, pbIntersects);
}

STDMETHODIMP CDrawPie::LoadOldVer(LPSTREAM pStm)
{
	pStm->Read(&m_enumPieType, sizeof(m_enumPieType), NULL);
	return ICxDrawObjectImpl<CDrawPie>::LoadOldVer(pStm);
}

STDMETHODIMP CDrawPie::SaveOldVer(LPSTREAM pStm, BOOL fClearDirty)
{
	pStm->Write(&m_enumPieType, sizeof(m_enumPieType), NULL);
	return ICxDrawObjectImpl<CDrawPie>::SaveOldVer(pStm, fClearDirty);
}

////////////////////////////////////////////////////////////////
//功能: 取对象名
////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPie::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("Pie");
	return S_OK;
}


STDMETHODIMP CDrawPie::get_PieType(enumPieType *pVal)
{
	*pVal = m_enumPieType;
	return S_OK;
}

STDMETHODIMP CDrawPie::put_PieType(enumPieType newVal)
{
	if (m_enumPieType == newVal)
		return S_OK;

	m_enumPieType = newVal;

	using namespace Gdiplus;

	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINT points[3];
	points[0].x =  pPoints[0].X;
	points[0].y =  pPoints[0].Y;
	points[1].x =  pPoints[1].X;
	points[1].y =  pPoints[1].Y;
	points[2].x =  pPoints[nCount - 1].X;
	points[2].y =  pPoints[nCount - 1].Y;

	delete pPoints;

	FireViewChange();
	MoveHandlesTo(points, 3);
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawPie::get_StartAngle(float *pVal)
{
	using namespace Gdiplus;

	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINTF points[3];
	points[0].x =  pPoints[0].X;
	points[0].y =  pPoints[0].Y;
	points[1].x =  pPoints[1].X;
	points[1].y =  pPoints[1].Y;
	points[2].x =  pPoints[nCount - 1].X;
	points[2].y =  pPoints[nCount - 1].Y;

	*pVal = -CalculateAngle(points[0], points[1]);

	return S_OK;
}

STDMETHODIMP CDrawPie::put_StartAngle(float fStartAngle)
{
	using namespace Gdiplus;

	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINTF points[3];
	points[0].x =  pPoints[0].X;
	points[0].y =  pPoints[0].Y;
	points[1].x =  pPoints[1].X;
	points[1].y =  pPoints[1].Y;
	points[2].x =  pPoints[nCount - 1].X;
	points[2].y =  pPoints[nCount - 1].Y;

	float fRadius = (float)sqrt((points[2].x - points[0].x) * (points[2].x - points[0].x) + 
			(points[2].y - points[0].y) * (points[2].y - points[0].y)); 

	float fEndAngle = -CalculateAngle(points[0], points[2]);
	float fSweepAngle = fEndAngle - fStartAngle;
	if (fSweepAngle < 0)
		fSweepAngle += 360;
	if ((m_enumPieType == PieTypeWedge && fabs(fSweepAngle) > 180) || 
		(m_enumPieType == PieTypeCutOut && fabs(fSweepAngle) < 180))	
		fSweepAngle = float(fabs(fSweepAngle) - 360);

	if (fabs(fSweepAngle) < 0.1)
		fSweepAngle = 0.1f;

	FireViewChange();

	m_pObjectPath->Reset();
	m_pObjectPath->AddPie(points[0].x - fRadius, points[0].y - fRadius, 
			2 * fRadius, 2 * fRadius, fStartAngle, fSweepAngle);

	delete pPoints;

	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawPie::get_SweepAngle(float *pVal)
{
	using namespace Gdiplus;

	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINTF points[3];
	points[0].x =  pPoints[0].X;
	points[0].y =  pPoints[0].Y;
	points[1].x =  pPoints[1].X;
	points[1].y =  pPoints[1].Y;
	points[2].x =  pPoints[nCount - 1].X;
	points[2].y =  pPoints[nCount - 1].Y;

	float fStartAngle = -CalculateAngle(points[0], points[1]);
	float fEndAngle = -CalculateAngle(points[0], points[2]);
	float fSweepAngle = fEndAngle - fStartAngle;
	if (fSweepAngle < 0)
		fSweepAngle += 360;
	if ((m_enumPieType == PieTypeWedge && fabs(fSweepAngle) > 180) || 
		(m_enumPieType == PieTypeCutOut && fabs(fSweepAngle) < 180))	
		fSweepAngle = float(fabs(fSweepAngle) - 360);

	delete pPoints;

	*pVal = fSweepAngle;
	return S_OK;
}

STDMETHODIMP CDrawPie::put_SweepAngle(float fSweepAngle)
{
	using namespace Gdiplus;

	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINTF points[3];
	points[0].x =  pPoints[0].X;
	points[0].y =  pPoints[0].Y;
	points[1].x =  pPoints[1].X;
	points[1].y =  pPoints[1].Y;
	points[2].x =  pPoints[nCount - 1].X;
	points[2].y =  pPoints[nCount - 1].Y;

	float fRadius = (float)sqrt((points[2].x - points[0].x) * (points[2].x - points[0].x) + 
			(points[2].y - points[0].y) * (points[2].y - points[0].y)); 

	float fStartAngle = -CalculateAngle(points[0], points[1]);
	if ((m_enumPieType == PieTypeWedge && fabs(fSweepAngle) > 180) || 
		(m_enumPieType == PieTypeCutOut && fabs(fSweepAngle) < 180))	
		fSweepAngle = float(fabs(fSweepAngle) - 360);

	if (fabs(fSweepAngle) < 0.1)
		fSweepAngle = 0.1f;

	FireViewChange();
	m_pObjectPath->Reset();
	m_pObjectPath->AddPie(points[0].x - fRadius, points[0].y - fRadius, 
			2 * fRadius, 2 * fRadius, fStartAngle, fSweepAngle);

	delete pPoints;

	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawPie::get_CenterX(float *pVal)
{
	using namespace Gdiplus;

	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);
	*pVal = pPoints[0].X;

	delete pPoints;

	return S_OK;
}

STDMETHODIMP CDrawPie::put_CenterX(float newVal)
{
	using namespace Gdiplus;

	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);
	if (newVal == pPoints[0].X)
		return S_OK;

	FireViewChange();
	float fOffset = newVal - pPoints[0].X;

	RECTF rcPos;
	GetPositionRect(&rcPos);
	rcPos.left += fOffset;	
	rcPos.right += fOffset;
	MoveTo(rcPos, FALSE);

	delete pPoints;

	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawPie::get_CenterY(float *pVal)
{
	using namespace Gdiplus;

	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);
	*pVal = pPoints[0].Y;

	delete pPoints;

	return S_OK;
}

STDMETHODIMP CDrawPie::put_CenterY(float newVal)
{
	using namespace Gdiplus;

	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	if (newVal == pPoints[0].Y)
		return S_OK;
	
	FireViewChange();

	float fOffset = newVal - pPoints[0].Y;

	RECTF rcPos;
	GetPositionRect(&rcPos);
	rcPos.top += fOffset;	
	rcPos.bottom += fOffset;
	MoveTo(rcPos, FALSE);

	delete pPoints;

	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawPie::get_Radius(float *pVal)
{
	using namespace Gdiplus;

	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	float fRadius1 = (float)sqrt((pPoints[1].X - pPoints[0].X) * (pPoints[1].X - pPoints[0].X) + 
			(pPoints[1].Y - pPoints[0].Y) * (pPoints[1].Y - pPoints[0].Y)); 
	float fRadius2 = (float)sqrt((pPoints[nCount - 1].X - pPoints[0].X) * (pPoints[nCount - 1].X - pPoints[0].X) + 
			(pPoints[nCount - 1].Y - pPoints[0].Y) * (pPoints[nCount - 1].Y - pPoints[0].Y)); 
	*pVal = (fRadius1 + fRadius2) / 2;

	delete pPoints;

	return S_OK;
}

STDMETHODIMP CDrawPie::put_Radius(float newVal)
{
	using namespace Gdiplus;

	FireViewChange();

	int nCount = m_pObjectPath->GetPointCount();
	PointF* pPoints = new PointF[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINTF points[3];
	points[0].x =  pPoints[0].X;
	points[0].y =  pPoints[0].Y;
	points[1].x =  pPoints[1].X;
	points[1].y =  pPoints[1].Y;
	points[2].x =  pPoints[nCount - 1].X;
	points[2].y =  pPoints[nCount - 1].Y;

	float fStartAngle = -CalculateAngle(points[0], points[1]);
	float fEndAngle = -CalculateAngle(points[0], points[2]);
	float fSweepAngle = fEndAngle - fStartAngle;
	if (fSweepAngle < 0)
		fSweepAngle += 360;
	if ((m_enumPieType == PieTypeWedge && fabs(fSweepAngle) > 180) || 
		(m_enumPieType == PieTypeCutOut && fabs(fSweepAngle) < 180))	
		fSweepAngle = float(fabs(fSweepAngle) - 360);

	if (fSweepAngle == 0)
		fSweepAngle = 0.1f;

	m_pObjectPath->Reset();
	m_pObjectPath->AddPie(points[0].x - newVal, points[0].y - newVal, 
			2 * newVal, 2 * newVal, fStartAngle, fSweepAngle);

	delete pPoints;

	FireViewChange();
	return S_OK;
}
