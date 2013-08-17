// DrawChord.cpp : Implementation of CDrawChord
// 所属项目: AutoCx的弓形对象
// 创建日期: 2001.10.8
// 创 建 人：卢文江 
// 功能描述：
// 修改记录：
/////////////////////////////////////////////////////////////////////////////
 

#include "stdafx.h"
#include "CxDrawShapes.h"
#include "DrawChord.h"
#include "math.h"

#define PI 3.1415926
#define GETPOINTNO (nCount / 2) 

void CDrawChord::InitChordPath(Gdiplus::GraphicsPath** ppObjectPath,
		POINT* points)
{
	POINTF ptTemp[3];
	for (int i = 0; i < 3; i++)
	{
		ptTemp[i].x = float(points[i].x);
		ptTemp[i].y = float(points[i].y);
	}
	InitChordPath(ppObjectPath, ptTemp);
}

void CDrawChord::InitChordPath(Gdiplus::GraphicsPath** ppObjectPath,
		POINTF* points)
{
	using namespace Gdiplus;

	POINTF ptMiddleSE;
	POINTF ptAcross;
	POINTF ptCenter;
	float fStartEndDx = points[0].x - points[1].x;
	float fStartEndDy = points[0].y - points[1].y;
	float fMiddleStartDx = points[2].x - points[0].x;
	float fMiddleStartDy = points[2].y - points[0].y;
	float fStartAngle; 
	float fEndAngle;
	float fSweepAngle;
	float k1;

	BOOL bIsReSetSweepAngle = FALSE;

	if (fStartEndDx != 0)
	{
		//fCondY > 0 : 中点Y坐标在起点-终点直线之上方
		//		 < 0 : 下方
		//		 = 0 : 直线之上(三点共线)
		float fCondY = fMiddleStartDy - fMiddleStartDx * fStartEndDy / fStartEndDx;

		//if (fCondY == 0)
		//	return FALSE;

		//判定弧的方向
		if (fCondY * fStartEndDx < 0)
			bIsReSetSweepAngle = true;
			
	}
	else	//起点-终点连线平行Y轴
	{
		if (fMiddleStartDx < 0)
			bIsReSetSweepAngle = true;
	}

	ptMiddleSE.x = (points[1].x + points[0].x) / 2;
	ptMiddleSE.y = (points[1].y + points[0].y) / 2;

	if (fStartEndDx == 0)
	{
		ptAcross.x = points[2].x;
		ptAcross.y = ptMiddleSE.y;
	}
	else if(fStartEndDy == 0)
	{
		ptAcross.x = ptMiddleSE.x;
		ptAcross.y = points[2].y;
	}
	else
	{
		k1 = (points[1].y - points[0].y) / (points[1].x - points[0].x);
		
		ptAcross.x = ((ptMiddleSE.y - points[2].y + k1 * points[2].x ) * k1 + ptMiddleSE.x ) / (k1 * k1 + 1);
		ptAcross.y = ((ptMiddleSE.x - points[2].x + ptMiddleSE.y * k1 ) * k1 + points[2].y ) / (k1 * k1 + 1);
	}

	float oldx1, oldy1, oldx2, oldy2, oldx3, oldy3;
	float a, b, c;
	float d,e;
	float xt,yt;

	oldx1 = points[0].x;
	oldy1 = points[0].y;
	oldx3 = points[1].x;
	oldy3 = points[1].y;
	oldx2 = ptAcross.x;
	oldy2 = ptAcross.y;

	a = oldx1 - oldx2;
	b = oldy1 - oldy2;
	c = oldx2 - oldx3;
	d = oldy2 - oldy3;
	e = a * d -b * c;

	if ((e > 0 ? e : -e) < 0.001)
		e = 1;

	xt = (float)((pow(oldx1, 2.0) + pow(oldy1, 2.0) - pow(oldx2, 2.0) -pow(oldy2, 2.0)) / 2.0);
	yt = (float)((pow(oldx2, 2.0) + pow(oldy2, 2.0) - pow(oldx3, 2.0) -pow(oldy3, 2.0)) / 2.0);
	ptCenter.x = (xt * d - yt * b) / e;
	ptCenter.y = (yt * a - xt * c) / e;
	float fRadius = (float)sqrt(pow((oldx1 - ptCenter.x), 2.0) + pow((oldy1 - ptCenter.y), 2.0));

	float fDistance = (float)sqrt(pow((ptAcross.x - ptMiddleSE.x), 2.0) + pow((ptAcross.x - ptMiddleSE.x), 2.0));
	if (fRadius < fDistance)
		m_MostSphere = TRUE;
	else
		m_MostSphere = FALSE;
	//求起点,终点相对于以园心为原点的坐标系下的旋转角度
	fStartAngle = 360 - CalculateAngle(ptCenter, points[0]);
	fEndAngle = 360 - CalculateAngle(ptCenter, points[1]);
	//起点-终点的夹角(0<m_lSweepAngle<360)
	fSweepAngle = (fEndAngle - fStartAngle);
	if (fSweepAngle < 0) 
		fSweepAngle += 360;
	
//	if (fabs(fSweepAngle) < 0.5f)
//		fSweepAngle = 0.1f;

	if (bIsReSetSweepAngle == TRUE)
       fSweepAngle = (float)fabs(fSweepAngle) - 360;

	*ppObjectPath = new Gdiplus::GraphicsPath;
	(*ppObjectPath)->StartFigure();
	(*ppObjectPath)->AddArc(ptCenter.x - fRadius, ptCenter.y - fRadius, fRadius * 2, 
		fRadius * 2, fStartAngle, fSweepAngle / 2);
	(*ppObjectPath)->AddArc(ptCenter.x - fRadius, ptCenter.y - fRadius, fRadius * 2, 
		fRadius * 2, fStartAngle + fSweepAngle / 2, fSweepAngle / 2);
	(*ppObjectPath)->CloseFigure();

	m_ptStart.x = points[0].x;
	m_ptStart.y = points[0].y;
	m_ptEnd.x = points[1].x;
	m_ptEnd.y = points[1].y;
	m_ptMiddle.x = ptAcross.x;
	m_ptMiddle.y = ptAcross.y;
}

HRESULT CDrawChord::DoLoad(DWORD dwVer, LPSTREAM pStm)
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

HRESULT CDrawChord::DoSave(DWORD dwVer, LPSTREAM pStm)
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

HRESULT CDrawChord::DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
{
	using namespace Gdiplus;

	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawChord>::DrawTrackerHandleShape(hdcDest, nHandle, point);

	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINT points[3];
	points[0].x =  pPoints[0].X;
	points[0].y =  pPoints[0].Y;
	points[1].x =  pPoints[nCount - 1].X;
	points[1].y =  pPoints[nCount - 1].Y;
	
	points[2].x =  pPoints[GETPOINTNO].X;
	points[2].y =  pPoints[GETPOINTNO].Y;

	switch (nHandle)
	{
		case 1:
			points[0] = point;	
			break;
		case 2:
			points[1] = point;
			break;
		case 3:
			points[2] = point;
			break;
		default:
			return S_FALSE;
	}
	delete [] pPoints;
	GraphicsPath* pObjectPath = NULL;
	InitChordPath(&pObjectPath, points);
	PathData pathData;
	pObjectPath->GetPathData(&pathData);
	DrawPathDataShape(hdcDest, pathData);
	delete pObjectPath;

	return S_OK;
}

HRESULT CDrawChord::DrawTrackerShape(HDC hdcDest, POINT* points, int nPoint)
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
		InitChordPath(&pObjectPath, points);
		PathData pathData;
		pObjectPath->GetPathData(&pathData);
		DrawPathDataShape(hdcDest, pathData);
		delete pObjectPath;

	}
	else
	{
		return ICxDrawObjectImpl<CDrawChord>::DrawTrackerShape(hdcDest, points, nPoint);
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////
//功能: 根据起始点，中心点，结束点确定弧的方向
//说明: 通过Center点及Start点在原坐标系下建立直线方程,根据End点坐标值确定
//End点在相对直线的位置(上方或下方)，接合Center及Start点置可确定弧的方向
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CDrawChord::ReCalGeometry()
{
	bool bIsReSetSweepAngle = false;
	fPOINT fptMiddleSE;
	fPOINT fptMiddleSM;
	double dStartEndDx = m_ptStart.x - m_ptEnd.x;
	double dStartEndDy = m_ptStart.y - m_ptEnd.y;
	double dMiddleStartDx = m_ptMiddle.x - m_ptStart.x;
	double dMiddleStartDy = m_ptMiddle.y - m_ptStart.y;
		
	//三点共线且垂直于Y或X轴,不画
	if ((int)dStartEndDx == 0 && (int)dMiddleStartDx == 0) 
		return FALSE;

	if ((int)dStartEndDx != 0)
	{
		//fCondY > 0 : 中点Y坐标在起点-终点直线之上方
		//		 < 0 : 下方
		//		 = 0 : 直线之上(三点共线)
		int fCondY = int(dMiddleStartDy - dMiddleStartDx * dStartEndDy / dStartEndDx);

		if (fCondY == 0)
			return FALSE;

		//判定弧的方向
		if (fCondY * dStartEndDx < 0)
			bIsReSetSweepAngle = true;
			
	}
	else	//起点-终点连线平行Y轴
	{
		if (dMiddleStartDx * dStartEndDx < 0)
			bIsReSetSweepAngle = true;
	}

	//求起点-终点连线正交线的斜率
	double fSlopeSE = 0.0 - dStartEndDx / dStartEndDy;
	//起点-中点连线正交线的斜率
	double fSlopeSM = 0.0 - dMiddleStartDx / dMiddleStartDy;

	//按起点-结束点的中垂线及起点-中心点的中垂线的交点为园心点
	fptMiddleSE.x = (m_ptStart.x + m_ptEnd.x) / 2;
	fptMiddleSE.y = (m_ptStart.y + m_ptEnd.y) / 2;
	fptMiddleSM.x = (m_ptStart.x + m_ptMiddle.x) / 2;
	fptMiddleSM.y = (m_ptStart.y + m_ptMiddle.y) / 2;
	m_ptCenter.x = (fptMiddleSM.y - fptMiddleSE.y + fSlopeSE * fptMiddleSE.x - fSlopeSM * fptMiddleSM.x)
					/ (fSlopeSE - fSlopeSM);
	m_ptCenter.y = fSlopeSM * (m_ptCenter.x - fptMiddleSM.x) + fptMiddleSM.y;
	m_lRadius = (float)sqrt(pow(m_ptStart.x - m_ptCenter.x, 2) + pow(m_ptStart.y - m_ptCenter.y, 2));			
	
	//限制园弧大小
//	if (m_lRadius > 200)
//		return FALSE;

	//求起点,终点相对于以园心为原点的坐标系下的旋转角度
	long sdx = long(m_ptStart.x - m_ptCenter.x);
	long sdy = long(m_ptStart.y - m_ptCenter.y);
	m_lStartAngle = float(atan2(sdy, sdx) * 180 / PI);
	if (m_lStartAngle < 0)
		m_lStartAngle = 360 + m_lStartAngle;
	long edx = long(m_ptEnd.x - m_ptCenter.x);
	long edy = long(m_ptEnd.y - m_ptCenter.y);
	long lEndAngle = long(atan2(edy, edx) * 180 / PI);
	if (lEndAngle < 0)
		lEndAngle = 360 + lEndAngle;
	//起点-终点的夹角(0<m_lSweepAngle<360)
	m_lSweepAngle = lEndAngle - m_lStartAngle > 0 ? lEndAngle - m_lStartAngle : 
													lEndAngle - m_lStartAngle + 360;
	if (bIsReSetSweepAngle == true)
			m_lSweepAngle = (float)fabs(m_lSweepAngle) - 360;

	//包含chord的外接园的正方形
	m_rcRect.left =long(m_ptCenter.x - m_lRadius); 
	m_rcRect.right = long(m_ptCenter.x + m_lRadius); 
	m_rcRect.top = long(m_ptCenter.y - m_lRadius); 
	m_rcRect.bottom = long(m_ptCenter.y + m_lRadius);

	return TRUE;
}

STDMETHODIMP CDrawChord::MoveHandlesTo(POINT* pPoints, int nCount)
{
	ATLASSERT(nCount == 3);

	delete m_pObjectPath;
	InitChordPath(&m_pObjectPath, pPoints);
	ReSetRotateBasePoint();
	//FireViewChange();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////////
//功能: 移动手柄至指点位置
//参数: nHandle[in] 手柄点编号
//		point[in] 新位置点坐标
/////////////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawChord::MoveHandleTo(long nHandle, POINT point)
{
	using namespace Gdiplus;

	if (nHandle < 0)
	{
		return ICxDrawObjectImpl<CDrawChord>::MoveHandleTo(nHandle, point);
	}

	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINT points[3];

	switch (nHandle)
	{
		case 1:
			points[0] = point;	
			points[1].x =  pPoints[nCount - 1].X;
			points[1].y =  pPoints[nCount - 1].Y;
			points[2].x =  pPoints[GETPOINTNO].X;
			points[2].y =  pPoints[GETPOINTNO].Y;
			break;
		case 2:
			points[0].x =  pPoints[0].X;
			points[0].y =  pPoints[0].Y;
			points[1] =  point;
			points[2].x =  pPoints[GETPOINTNO].X;
			points[2].y =  pPoints[GETPOINTNO].Y;
			break;
		case 3:
			points[0].x =  pPoints[0].X;
			points[0].y =  pPoints[0].Y;
			points[1].x =  pPoints[nCount - 1].X;
			points[1].y =  pPoints[nCount - 1].Y;
			points[2] = point;
			break;
		default:
			return S_FALSE;
	}
	delete [] pPoints;
	delete m_pObjectPath;
	InitChordPath(&m_pObjectPath, points);
	ReSetRotateBasePoint();

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////////
//功能: 获取对象手柄数目
//////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawChord::GetHandleCount(long* pHandleCount)
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
STDMETHODIMP CDrawChord::GetHandle(long nHandle, POINT* pHandlePoint)
{
	using namespace Gdiplus;

	if (nHandle < 0)
	{
		return ICxDrawObjectImpl<CDrawChord>::GetHandle(nHandle, pHandlePoint);
	}

	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINT points[3];
	points[0].x =  pPoints[0].X;
	points[0].y =  pPoints[0].Y;
	points[1].x =  pPoints[nCount - 1].X;
	points[1].y =  pPoints[nCount - 1].Y;
	
	points[2].x =  pPoints[GETPOINTNO].X;
	points[2].y =  pPoints[GETPOINTNO].Y;

	delete [] pPoints;

	switch (nHandle)
	{
		case 1:
			{
				pHandlePoint->x = (long)points[0].x; 
				pHandlePoint->y = (long)points[0].y; 
				break;
			}
		case 2:
			{
				pHandlePoint->x = (long)points[1].x; 
				pHandlePoint->y = (long)points[1].y; 
				break;
			}
		case 3:
			{
				pHandlePoint->x = (long)points[2].x ; 
				pHandlePoint->y = (long)points[2].y ; 
				break;
			}
		default:
			return S_FALSE;
	}
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////
//功能: 取手柄位置光标句柄
///////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawChord::GetHandleCursor(long nHandle, HCURSOR* pHandleCursor)
{
	if (nHandle < 0)
	{
		return ICxDrawObjectImpl<CDrawChord>::GetHandleCursor(nHandle, pHandleCursor);
	}
	*pHandleCursor = LoadCursor(NULL, IDC_CROSS);
	return S_OK;
}

STDMETHODIMP CDrawChord::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("Chord");
	return S_OK;
}

STDMETHODIMP CDrawChord::get_StartX(long *pVal)
{
	POINT point;
	GetHandle(1, &point);

	*pVal = (long)point.x;

	return S_OK;
}

STDMETHODIMP CDrawChord::put_StartX(long newVal)
{
	using namespace Gdiplus;

	FireViewChange();
	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINT point;

	point.x = newVal;	
	point.y = pPoints[0].Y;

	delete [] pPoints;
	MoveHandleTo(1, point);
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawChord::get_StartY(long *pVal)
{
	POINT point;
	GetHandle(1, &point);

	*pVal = (long)point.y;

	return S_OK;
}

STDMETHODIMP CDrawChord::put_StartY(long newVal)
{
	using namespace Gdiplus;

	FireViewChange();
	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINT point;

	point.x = pPoints[0].X;	
	point.y = newVal;

	delete [] pPoints;
	MoveHandleTo(1, point);
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawChord::get_EndX(long *pVal)
{
	POINT point;
	GetHandle(2, &point);

	*pVal = (long)point.x;

	return S_OK;
}

STDMETHODIMP CDrawChord::put_EndX(long newVal)
{
	using namespace Gdiplus;

	FireViewChange();

	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINT point;

	point.x = newVal;	
	point.y = pPoints[nCount - 1].Y;

	delete [] pPoints;
	MoveHandleTo(2, point);
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawChord::get_EndY(long *pVal)
{
	POINT point;
	GetHandle(2, &point);

	*pVal = (long)point.y;

	return S_OK;
}

STDMETHODIMP CDrawChord::put_EndY(long newVal)
{
	using namespace Gdiplus;

	FireViewChange();
	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINT point;

	point.x = pPoints[nCount - 1].X;	
	point.y = newVal;

	delete [] pPoints;
	MoveHandleTo(2, point);
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawChord::get_MiddleX(long *pVal)
{
	POINT point;
	GetHandle(3, &point);

	*pVal = (long)point.x;

	return S_OK;
}

STDMETHODIMP CDrawChord::put_MiddleX(long newVal)
{
	using namespace Gdiplus;

	FireViewChange();
	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINT point;

	point.x = newVal;	
	point.y = pPoints[GETPOINTNO].Y;

	delete [] pPoints;
	MoveHandleTo(3, point);
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawChord::get_MiddleY(long *pVal)
{
	POINT point;
	GetHandle(3, &point);

	*pVal = (long)point.y;

	return S_OK;
}

STDMETHODIMP CDrawChord::put_MiddleY(long newVal)
{
	using namespace Gdiplus;

	FireViewChange();
	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	POINT point;

	point.x = pPoints[GETPOINTNO].X;	
	point.y = newVal;

	delete [] pPoints;
	MoveHandleTo(3, point);
	FireViewChange();

	return S_OK;
}

