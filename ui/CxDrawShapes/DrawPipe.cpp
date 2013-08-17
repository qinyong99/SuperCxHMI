// DrawPipe.cpp : Implementation of CDrawPipe
#include "stdafx.h"
#include "CxDrawShapes.h"
//#include "CxDrawPipe_i.c"
#include "DrawPipe.h"
#include <math.h>

/////////////////////////////////////////////////////////////////////////////
// CDrawPipe

///////////////////////////////////////////////////////////////////
//功 能：
///////////////////////////////////////////////////////////////////
inline int GetFixLenth(POINTF* point1, POINTF* point2, int nLenth, POINTF* point)
{	
	double nRR = (point2->x - point1->x) * (point2->x - point1->x) +
		(point2->y - point1->y) * (point2->y - point1->y);
	double nrr = nLenth * nLenth;
	if (nRR <= nrr)
	{
		point->x = point2->x;
		point->y = point2->y;
		return int(sqrt(nRR));
	}
    float drR = (float)sqrt(nrr / nRR);
	point->x = point1->x + (point2->x - point1->x) * drR;
	point->y = point1->y + (point2->y - point1->y) * drR;

	return nLenth;
}

HRESULT CDrawPipe::DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
{
	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawPipe>::DrawTrackerHandleShape(hdcDest, nHandle, point);

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

inline void DrawLine(HDC hdcDest, POINTF* pPoint1, POINTF* pPoint2, BOOL bSwitch)
{
	if (bSwitch)
	{
		::MoveToEx(hdcDest, (int)pPoint1->x, (int)pPoint1->y, NULL);
		::LineTo(hdcDest, (int)pPoint2->x, (int)pPoint2->y);
	}
}

HRESULT CDrawPipe::DoDraw(HDC hdcDest)
{
	using namespace Gdiplus;
	
	SIZE szZoomNum, szZoomDenom;
	::GetViewportExtEx(hdcDest, &szZoomNum);
	::GetWindowExtEx(hdcDest, &szZoomDenom);

//	float f = (float)(szZoomNum.cx / (float)szZoomDenom.cx);
//	BOOL bZoomInX = (int(f + 0.5f) != int(f + 1.0f));
//	f = (float)(szZoomNum.cy / (float)szZoomDenom.cy);
//	BOOL bZoomInY = (int(f + 0.5f) != int(f + 1.0f));

	BOOL bZoomInX = (szZoomNum.cx > szZoomDenom.cx);
	BOOL bZoomInY = (szZoomNum.cy > szZoomDenom.cy);
	BOOL bNotZoomIn = !(bZoomInX && bZoomInY);

//	if (m_bNeedRecalGeometry || 
//		m_szLastZoomNum.cx != szZoomNum.cx || m_szLastZoomNum.cy != szZoomNum.cy ||
//		m_szLastZoomDenom.cx != szZoomDenom.cx || m_szLastZoomDenom.cy != szZoomDenom.cy)
	
	if (m_bNeedRecalGeometry)
	{
//		m_szLastZoomNum = szZoomNum;
//		m_szLastZoomDenom = szZoomDenom;

		RecalGeometry();
		m_bNeedRecalGeometry = FALSE;
	}

	HGDIOBJ hOldPen = SelectObject(hdcDest, GetStockObject(NULL_PEN));

	int nEdgeWidth = ROUND(m_fEdgeWidth);

	RECT* pRect;
	for (int i = nEdgeWidth - 1; i >= 0; i--)
	{
		COLORREF clrPen = m_apLines[i].clrColor;
			
		int j;

		//画同色的线
		CPen pen;
		CPen penDouble;
	
		pen.CreatePen(PS_SOLID, 1, clrPen);
		penDouble.CreatePen(PS_SOLID, 2, clrPen);
		
		for (j = 0; j < m_apLines[i].nCount; j++)
		{
			pRect = &m_apLines[i].pLines[j];

			if (bNotZoomIn || i % 2 == 0)
			{
				SelectObject(hdcDest, pen);
			}
			else
			{
				SelectObject(hdcDest, penDouble);
			}
	
			MoveToEx(hdcDest, pRect->left, pRect->top, NULL);
			LineTo(hdcDest, pRect->right, pRect->bottom);
		}

		//画同色的接头
		SelectObject(hdcDest, penDouble);
		for (j = 0; j < m_apCircles[i].nCount; j++)
		{
			pRect = &m_apCircles[i].pCircles[j];
			Ellipse(hdcDest, pRect->left, pRect->top, pRect->right, pRect->bottom);
		}

		SelectObject(hdcDest, hOldPen);
	}
	
	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);
			
	if (m_bFlowShow)	//绘制流动效果
	{
		//创建画笔
		COLORREF clrLiquid;
		OleTranslateColor(m_clrLiquid, 0, &clrLiquid);

		CPen pen;
		pen.CreatePen(PS_SOLID, nEdgeWidth, clrLiquid);
		SelectObject(hdcDest, pen);

		int nSenLen = nEdgeWidth * 2 + 4; //液体长度
		float fr0 = (float)nSenLen;
		float frr0 = fr0 * fr0;
		BOOL bSwitch = m_nFlowStartLenth == 0 || m_nFlowStartLenth > nSenLen;
		int nLenth = m_nFlowStartLenth % nSenLen;

		POINTF point1, point2, point;
		int nPoint;
		float fRR, frR, fR;
	
		if (m_enumFlowDirection == FlowDirectionForward) 	//正向流动
		{
			nPoint = 0;
			while (nPoint < nCount - 1) 
			{
				point1.x = (float)pPoints[nPoint].X;
				point1.y = (float)pPoints[nPoint].Y;
				
				point2.x = (float)pPoints[nPoint + 1].X;
				point2.y = (float)pPoints[nPoint + 1].Y;

				float fx21 = point2.x - point1.x;
				float fy21 = point2.y - point1.y;
				fRR = fx21 * fx21 + fy21 * fy21;
				fR = (float)sqrt(fRR);
				//上一次剩余长度
				if (nLenth > 0)
				{
					if (nLenth <= fR)
					{
						frR = nLenth / fR;
						point.x = point1.x + fx21 * frR;
						point.y = point1.y + fy21 * frR;
						DrawLine(hdcDest, &point1, &point, bSwitch);
						bSwitch = !bSwitch;
						point1 = point;
					}
					//线段不够，到下一条线段
					else
					{
						DrawLine(hdcDest, &point1, &point2, bSwitch);
						nLenth -= (int)fR;
						nPoint++;
						continue;
					}
				}
			
				frR = fr0 / fR;
				float frx = frR * fx21;
				float fry = frR * fy21;
				fR -= (float)nLenth;
				int n = (int)(fR / fr0);
				nLenth = (int)(fr0 + n * fr0 - fR);
				while (n--)
				{
					point.x = point1.x + frx;
					point.y = point1.y + fry;
					DrawLine(hdcDest, &point1, &point, bSwitch);
					bSwitch = !bSwitch;
					point1 = point;
				}
				if (nLenth > 0)
				{
					DrawLine(hdcDest, &point1, &point2, bSwitch);
				}
				nPoint++;
			}
		}
		else
		{
			nPoint = nCount - 1;
			while (nPoint > 0) 
			{
				point1.x = (float)pPoints[nPoint].X;
				point1.y = (float)pPoints[nPoint].Y;
				
				point2.x = (float)pPoints[nPoint - 1].X;
				point2.y = (float)pPoints[nPoint - 1].Y;
				
				float fx21 = point2.x - point1.x;
				float fy21 = point2.y - point1.y;
				fRR = fx21 * fx21 + fy21 * fy21;
				fR = (float)sqrt(fRR);
				//上一次剩余长度
				if (nLenth > 0)
				{
					if (nLenth <= fR)
					{
						frR = nLenth / fR;
						point.x = point1.x + fx21 * frR;
						point.y = point1.y + fy21 * frR;
						DrawLine(hdcDest, &point1, &point, bSwitch);
						bSwitch = !bSwitch;
						point1 = point;
					}
					//线段不够，到下一条线段
					else
					{
						DrawLine(hdcDest, &point1, &point2, bSwitch);
						nLenth -= (int)fR;
						nPoint--;
						continue;
					}
				}
				
				frR = fr0 / fR;
				float frx = frR * fx21;
				float fry = frR * fy21;
				fR -= (float)nLenth;
				int n = (int)(fR / fr0);
				nLenth = (int)(fr0 + n * fr0 - fR);
				while (n--)
				{
					point.x = point1.x + frx;
					point.y = point1.y + fry;
					DrawLine(hdcDest, &point1, &point, bSwitch);
					bSwitch = !bSwitch;
					point1 = point;
				}
				if (nLenth > 0)
				{
					DrawLine(hdcDest, &point1, &point2, bSwitch);
				}
				nPoint--;
			}
		}

		SelectObject(hdcDest, hOldPen);
	}

	delete [] pPoints;
	
	return S_OK;
}

HRESULT CDrawPipe::DoLoad(DWORD dwVer, LPSTREAM pStm)
{
	pStm->Read(&m_clrLiquid, sizeof(m_clrLiquid), NULL);
	pStm->Read(&m_bFlowShow, sizeof(m_bFlowShow), NULL);
	pStm->Read(&m_enumFlowDirection, sizeof(m_enumFlowDirection), NULL);
	pStm->Read(&m_enumFlowSpeed, sizeof(m_enumFlowSpeed), NULL);

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

HRESULT CDrawPipe::DoSave(DWORD dwVer, LPSTREAM pStm)
{
	pStm->Write(&m_clrLiquid, sizeof(m_clrLiquid), NULL);
	pStm->Write(&m_bFlowShow, sizeof(m_bFlowShow), NULL);
	pStm->Write(&m_enumFlowDirection, sizeof(m_enumFlowDirection), NULL);
	pStm->Write(&m_enumFlowSpeed, sizeof(m_enumFlowSpeed), NULL);

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

STDMETHODIMP CDrawPipe::InitRun(BOOL* pbHasDynamic, ICxScriptEngineSite* pScriptSite)
{
	if (m_bFlowShow)
		m_spDrawObjectSite->SetTimer(50);

	return ICxDrawObjectImpl<CDrawPipe>::InitRun(pbHasDynamic, pScriptSite);
}

///////////////////////////////////////////////////////////////
//功能：运行时进行动态刷新
//返回：成功返回s_ok
//描述：在此实现管道液体流动效果
///////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPipe::OnTimer()
{
	if (m_enumFlowSpeed != FlowSpeedSleep && (m_nTimeFactor % (12 / m_enumFlowSpeed)/*m_dwRefreshInterval*/) == 0)
	{
		m_nFlowStartLenth += (int)m_enumFlowSpeed * 2;
		int nSegLen = ROUND(m_fEdgeWidth) * 2 + 4;
		m_nFlowStartLenth = m_nFlowStartLenth % (nSegLen * 2);
		if (m_enumFlowSpeed != FlowSpeedSleep)
		{	
//			if (m_hPipeRgn == NULL)
//				UpdateRegion();
//			m_spDrawObjectSite->InvalidateRgn(m_hPipeRgn, FALSE);

			FireViewChangeEx();
		}
	}
	m_nTimeFactor += 1;	

	return S_OK;
}

HRESULT CDrawPipe::FireViewChangeEx()
{
	using namespace Gdiplus;

	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);
	
	for (int i = 0; i < nCount - 1; i++)
	{
		RECT rect = {pPoints[i].X, pPoints[i].Y, pPoints[i + 1].X, pPoints[i + 1].Y};
		int nEdgeWidth = ROUND(m_fEdgeWidth);
		InflateRect(&rect, nEdgeWidth, nEdgeWidth);
		if (m_spDrawObjectSite)
			m_spDrawObjectSite->InvalidateRect(&rect, FALSE);
	}

	delete [] pPoints;
	
	return S_OK;
}

BOOL CDrawPipe::ReAllocWidth(int nWidth)
{
	ATLASSERT((nWidth >= m_nAllocWidth));
	if (nWidth < m_nAllocWidth)
		return FALSE;
	
	LINESARRAY* apLines = new LINESARRAY[nWidth];
	if (apLines == NULL)
		return FALSE;
	
	memcpy(apLines, m_apLines, sizeof(LINESARRAY) * m_nAllocWidth);
	memset(apLines + m_nAllocWidth, 0, sizeof(LINESARRAY) * (nWidth - m_nAllocWidth));
	
	m_apLines = apLines;
	
	CIRCLEARRAY* apCircles = new CIRCLEARRAY[nWidth];
	if (apCircles == NULL)
		return FALSE;
	
	memcpy(apCircles, m_apCircles, sizeof(CIRCLEARRAY) * m_nAllocWidth);
	memset(apCircles + m_nAllocWidth, 0, sizeof(CIRCLEARRAY) * (nWidth - m_nAllocWidth));
	
	m_apCircles = apCircles;
	
	m_nAllocWidth = nWidth;
	
	return TRUE;
}

void CDrawPipe::RecalGeometry()
{
	using namespace Gdiplus;

	int nWidth = ROUND(m_fEdgeWidth);

	if (m_nAllocWidth < nWidth + 2)
		ReAllocWidth(nWidth + 2);
			
	// 计算颜色跨度
	COLORREF clrEdgeColor;
	OleTranslateColor(m_clrEdgeColor, 0, &clrEdgeColor);
	BYTE R1 = GetRValue(clrEdgeColor);
	BYTE G1 = GetGValue(clrEdgeColor);
	BYTE B1 = GetBValue(clrEdgeColor);
	BYTE R_setp;
	BYTE G_setp;
	BYTE B_setp;
	if (nWidth > 1)
	{
		COLORREF clrHightLightEdge;
		OleTranslateColor(m_clrHighLightEdge, 0, &clrHightLightEdge);
		
		R_setp = BYTE((GetRValue(clrHightLightEdge) - R1) / (nWidth - 1));
		G_setp = BYTE((GetGValue(clrHightLightEdge) - G1) / (nWidth - 1));
		B_setp = BYTE((GetBValue(clrHightLightEdge) - B1) / (nWidth - 1));
	}
	else
	{
		R_setp = G_setp = B_setp = 0;
	}
	
	//计算要用到的颜色值,及初始化其它值
	for (int i = nWidth - 1; i >= 0 ; i--)
	{
		m_apLines[i].clrColor = RGB(R1, G1, B1);
		R1 += R_setp;
		G1 += G_setp;
		B1 += B_setp;
	}

	// 得到管道端点坐标
	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);

	// 初始化
	for (i = 0; i < m_nAllocWidth; i++)
	{
		m_apLines[i].nCount = 0;
		m_apCircles[i].nCount = 0;
	}
	
	// 计算接头的同心圆
	if (m_enumLineJoinStyle == LineJoinStyleSquare)
	{
		for (i = 1; i < nCount - 1; i++)
		{
			for (int j = 0; j < nWidth; j++)
			{
				RECT rect;
				rect.left   = pPoints[i].X - j;
				rect.right  = pPoints[i].X + j;
				rect.top    = pPoints[i].Y - j;
				rect.bottom = pPoints[i].Y + j;
				if ((pPoints[i].Y -  pPoints[i - 1].Y) * (pPoints[i + 1].X - pPoints[i].X)
					!= (pPoints[i].X -  pPoints[i - 1].X) * (pPoints[i + 1].Y - pPoints[i].Y))
					AddCircle(rect, j);
			}
		}
	}
	else
	{
		for (i = 0; i < nCount; i++)
		{
			for (int j = 0; j < nWidth; j++)
			{
				RECT rect;
				rect.left   = pPoints[i].X - j;
				rect.right  = pPoints[i].X + j;
				rect.top    = pPoints[i].Y - j;
				rect.bottom = pPoints[i].Y + j;
				AddCircle(rect, j);
			}
		}
	}

	// 计算管道线段	
	for (i = 0; i < nCount - 1; i++)
	{
		CalcuLineSegment(pPoints[i].X, pPoints[i].Y, pPoints[i+1].X,
			pPoints[i+1].Y);
	}

	delete [] pPoints;
}

STDMETHODIMP CDrawPipe::InitNew()
{
	ICxDrawObjectImpl<CDrawPipe>::InitNew();

	m_fEdgeWidth = 8.0;
	m_clrHighLightEdge = RGB(255, 255, 255);
	m_clrEdgeColor = RGB(128, 128, 128);

	m_bFlowShow = FALSE;
	m_enumFlowDirection = FlowDirectionForward;
	m_enumFlowSpeed = FlowSpeedMiddle;
	m_clrLiquid = RGB(0, 0, 255); //兰色

	m_enumEdgeStyle = EdgeStylePipe;
	m_enumLineJoinStyle = LineJoinStyleSquare;

	return S_OK;
}

STDMETHODIMP CDrawPipe::Intersects(RECT rect, BOOL* pbIntersects)
{
	using namespace Gdiplus;

	int nWidth = ROUND(m_fEdgeWidth);

	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);
	for (int i = 0; i < nCount - 1; i++)
	{
		double Distance = sqrt((pPoints[i].X - pPoints[i + 1].X) * (pPoints[i].X - pPoints[i + 1].X) +
			(pPoints[i].Y - pPoints[i + 1].Y) * (pPoints[i].Y - pPoints[i + 1].Y));
		long OffsetX = (long)((pPoints[i + 1].Y - pPoints[i].Y) * (nWidth + 4)/ Distance);
		long OffsetY = (long)((-pPoints[i + 1].X + pPoints[i].X) * (nWidth + 4) / Distance);

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

//void CDrawPipe::UpdateRegion()
//{
//	using namespace Gdiplus;
//
//	if (m_hPipeRgn)
//	{
//		::DeleteObject(m_hPipeRgn);
//		m_hPipeRgn = NULL;
//	}
//
//	int nCount = m_pObjectPath->GetPointCount();
//	Point* pPoints = new Point[nCount];
//	m_pObjectPath->GetPathPoints(pPoints, nCount);
//	
//	for (int i = 0; i < nCount - 1; i++)
//	{
//		double Distance = sqrt((pPoints[i].X - pPoints[i + 1].X) * (pPoints[i].X - pPoints[i + 1].X) +
//			(pPoints[i].Y - pPoints[i + 1].Y) * (pPoints[i].Y - pPoints[i + 1].Y));
//		long OffsetX = (long)((pPoints[i + 1].Y - pPoints[i].Y) * m_nEdgeWidth / Distance);
//		long OffsetY = (long)((-pPoints[i + 1].X + pPoints[i].X) * m_nEdgeWidth / Distance);
//		
//		POINT poly[4];
//		poly[1].x = pPoints[i].X + OffsetX;
//		poly[1].y = pPoints[i].Y + OffsetY;
//		poly[2].x = pPoints[i].X - OffsetX;
//		poly[2].y = pPoints[i].Y - OffsetY;
//		poly[3].x = pPoints[i + 1].X - OffsetX;
//		poly[3].y = pPoints[i + 1].Y - OffsetY;
//		poly[0].x = pPoints[i + 1].X + OffsetX;
//		poly[0].y = pPoints[i + 1].Y + OffsetY;
//		
//		HRGN hRgn = ::CreatePolygonRgn(poly, 4, ALTERNATE);
//		if (NULL == m_hPipeRgn)
//		{
//			m_hPipeRgn = hRgn;
//			continue;
//		}
//		
//		CombineRgn(m_hPipeRgn, m_hPipeRgn, hRgn, RGN_OR);
//		DeleteObject(hRgn);
//	}
//
//	if (m_enumLineJoinStyle == SquareJoin)
//	{
//		for (i = 1; i < nCount - 1; i++)
//		{
//			RECT rect;
//			rect.left   = pPoints[i].X - m_nEdgeWidth;
//			rect.right  = pPoints[i].X + m_nEdgeWidth;
//			rect.top    = pPoints[i].Y - m_nEdgeWidth;
//			rect.bottom = pPoints[i].Y + m_nEdgeWidth;
//
//			HRGN hRgn = ::CreateEllipticRgnIndirect(&rect);
//			int n = CombineRgn(m_hPipeRgn, m_hPipeRgn, hRgn, RGN_OR);
//			
//			DeleteObject(hRgn);
//		}
//	}
//	else
//	{
//		for (i = 0; i < nCount; i++)
//		{
//			RECT rect;
//			rect.left   = pPoints[i].X - m_nEdgeWidth;
//			rect.right  = pPoints[i].X + m_nEdgeWidth;
//			rect.top    = pPoints[i].Y - m_nEdgeWidth;
//			rect.bottom = pPoints[i].Y + m_nEdgeWidth;
//			
//			HRGN hRgn = ::CreateEllipticRgnIndirect(&rect);
//			CombineRgn(m_hPipeRgn, m_hPipeRgn, hRgn, RGN_OR);
//			
//			DeleteObject(hRgn);
//		}
//	}
//	
//	delete [] pPoints;


STDMETHODIMP CDrawPipe::HitTest(POINT point, BOOL* pbHitTest)
{
	using namespace Gdiplus;
	
	float fPenWidth = (float)2 * m_fEdgeWidth;
	if (fPenWidth < 4)
		fPenWidth = 4;

	Pen EdgePen(m_clrEdgeColor, fPenWidth);
	*pbHitTest = m_pObjectPath->IsOutlineVisible(point.x, point.y, &EdgePen);

	return S_OK;
}


////////////////////////////////////////////////////////////////////////
//功能：IDrawPipe接口实现函数，给管道对象中轴线增加一点
//参数：pdispPoint(输入)	实现IFloatPoint接口的对象的IDispatch接口
//返回：成功S_OK
////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPipe::AddPoint(IFloatPoint *pPoint)
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
	pPoints[nCount] = point;
	m_pObjectPath->Reset();
	RectF rcPos;
	m_pObjectPath->AddLines(pPoints, nCount + 1);
	m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
	m_ptRotateBase.X =  rcPos.X + rcPos.Width / 2;
	m_ptRotateBase.Y =  rcPos.Y + rcPos.Height / 2;

	delete [] pPoints;
	
//	UpdateRegion();
	m_bNeedRecalGeometry = TRUE;
	FireViewChange();

	return S_OK;
}

////////////////////////////////////////////////////////////////////////
//功能：IDrawPipe接口实现函数，在管道对象中轴线上减少一点
//参数：1Index(输入)	要删除点索引号
//返回：成功S_OK
////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPipe::DeletePoint(long lIndex)
{
	using namespace Gdiplus;

	m_bNeedRecalGeometry = TRUE;
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

////////////////////////////////////////////////////////////////////////
//功能：IDrawPipe接口实现函数，取管道对象中轴线上一点
//参数：1Index(输入)		要取点的索引号
//		ppdispPoint(输出)	返回的点对象(实现IFloatPoint接口)的IDispatch接口
//返回：成功S_OK
////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPipe::GetPointAt(long lIndex, IFloatPoint **ppPoint)
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

////////////////////////////////////////////////////////////////////////
//功能：IDrawPipe接口实现函数，取管道对象中轴线总点数
//参数：pVal(输出)	返回的点数
//返回：成功S_OK
////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPipe::GetNumberOfPoints(long *pVal)
{
	using namespace Gdiplus;
	*pVal = m_pObjectPath->GetPointCount();

	return S_OK;
}

////////////////////////////////////////////////////////////////////////
//功能：ICxDrawObject接口实现函数，取指定的控制点
//参数：nHandle(输入)		控制点编号
//		pHandlePoint(输出)	控制点坐标
//返回：成功S_OK
////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPipe::GetHandle(long nHandle, POINT* pHandlePoint)
{
	using namespace Gdiplus;

	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawPipe>::GetHandle(nHandle, pHandlePoint);

	int nCount = m_pObjectPath->GetPointCount();
	Point* pPoints = new Point[nCount];
	m_pObjectPath->GetPathPoints(pPoints, nCount);
	pHandlePoint->x = pPoints[nHandle - 1].X;
	pHandlePoint->y = pPoints[nHandle - 1].Y;
	delete [] pPoints;
	return S_OK;
}

////////////////////////////////////////////////////////////////////////
//功能：ICxDrawObject接口实现函数，取控制点总数（中轴线上的点)
//参数：pHandleCount(输出)	点的总数
//返回：成功S_OK
////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPipe::GetHandleCount(long* pHandleCount)
{
	*pHandleCount = m_pObjectPath->GetPointCount();

	return S_OK;
}

////////////////////////////////////////////////////////////////////////
//功能：ICxDrawObject接口实现函数，移动管道中轴线上一点到指定位置
//参数：nHandle(输入)	要移动的点
//		point			移到的新位置
//返回：成功S_OK
////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPipe::MoveHandleTo(long nHandle, POINT point)
{	
	using namespace Gdiplus;

	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawPipe>::MoveHandleTo(nHandle, point);

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

	m_bNeedRecalGeometry = TRUE;

	return S_OK;
}

STDMETHODIMP CDrawPipe::MoveTo(RECTF rect, BOOL bUniformScale)
{
	HRESULT hr = ICxDrawObjectImpl<CDrawPipe>::MoveTo(rect, bUniformScale);
	
	m_bNeedRecalGeometry = TRUE;

	return hr;
}

STDMETHODIMP CDrawPipe::Rotate(long lAngle)
{
	HRESULT hr = ICxDrawObjectImpl<CDrawPipe>::Rotate(lAngle);

	m_bNeedRecalGeometry = TRUE;
	
	return hr;
}

STDMETHODIMP CDrawPipe::RotateAt(POINTF point, float fAngle)
{
	HRESULT hr = ICxDrawObjectImpl<CDrawPipe>::RotateAt(point, fAngle);
	
	m_bNeedRecalGeometry = TRUE;
	
	return hr;
}

////////////////////////////////////////////////////////////////////////
//功能：ICxDrawObject接口实现函数，获得移动控制点时的光标形状
//参数：nHandle(输入)		要移动的点号
//		pHandleCursor(输出)	输出的光标柄
//返回：成功S_OK
////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawPipe::GetHandleCursor(long nHandle, HCURSOR* pHandleCursor)
{
	if (nHandle < 0)
		return ICxDrawObjectImpl<CDrawPipe>::GetHandleCursor(nHandle, pHandleCursor);

	*pHandleCursor = LoadCursor(NULL, IDC_CROSS);
	return S_OK;
}

STDMETHODIMP CDrawPipe::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("Pipe");
	return S_OK;
}

STDMETHODIMP CDrawPipe::put_EdgeStyle(enumEdgeStyle newVal)
{
	return S_OK;
}	

STDMETHODIMP CDrawPipe::get_EdgeWidth(float *pVal)
{
	*pVal = m_fEdgeWidth * 2;
	return S_OK;
}

STDMETHODIMP CDrawPipe::put_EdgeWidth(float newVal)
{
	if (newVal < 1 || newVal >60)
		return S_OK;
	newVal = float(newVal) / 2;
	if (m_fEdgeWidth == newVal)
		return S_OK;

	m_bNeedRecalGeometry = TRUE;
	FireViewChange();
	m_fEdgeWidth = newVal;
	FireViewChange();
//	UpdateRegion();

	return S_OK;
}


///////////////////////////////////////////////////////////////////////////
// 功能：向园表表数据中增加一系例园
// 参数：x,y为要加入园的园心坐标
//		 index为对应颜色索引号
///////////////////////////////////////////////////////////////////////////
void CDrawPipe::AddCircle(RECT& rect, int index)
{
	//调整空间
	CIRCLEARRAY& aCircle = m_apCircles[index];
	if ((int)(aCircle.nCount) >= aCircle.nArraySize )
	{	//已没有足够的空间了
		int size = aCircle.nArraySize + 32;
		RECT* pRects = new RECT[size];
		if (pRects == NULL)
			throw "管道制作内存不足!";
		::memcpy(pRects , aCircle.pCircles, aCircle.nCount * sizeof(RECT));
		delete [] aCircle.pCircles;
		aCircle.pCircles = pRects;
		aCircle.nArraySize = size;
		ATLASSERT(aCircle.pCircles != NULL);
	}

	//保存参数
	aCircle.pCircles[aCircle.nCount] = rect;
	aCircle.nCount++;
}

///////////////////////////////////////////////////////////////////////////
// 功能：向线表数据中增加一根线
// 参数：x1,y1,x2,y2为要加入线的两端点坐标
//		 index为对应颜色索引号
///////////////////////////////////////////////////////////////////////////
void CDrawPipe::AddLine(int x1, int y1, int x2, int y2, int index)
{
	//调整空间
	LINESARRAY& aLine = m_apLines[index];
	if ((int)(aLine.nCount) >= aLine.nArraySize)
	{	//已没有足够的空间了
		int size = aLine.nArraySize + 64;
		RECT* pRects = new RECT[size];
		if (pRects == NULL)
			throw "管道制作内存不足!";
		::memcpy(pRects , aLine.pLines, aLine.nCount * sizeof(RECT));
		delete [] aLine.pLines;
		aLine.pLines = pRects;
		aLine.nArraySize = size;
	}

	//保存参数
	aLine.pLines[aLine.nCount].left = x1;
	aLine.pLines[aLine.nCount].top = y1;
	aLine.pLines[aLine.nCount].right = x2;
	aLine.pLines[aLine.nCount].bottom = y2;
	aLine.nCount++;
}

//////////////////////////////////////////////////////////////////
//功能：计算线段12对应柱体的各线段构成
//参数：x1,y1		中轴线的开始点
//		x2,y2		中轴线的结束点。
//返回：无
/////////////////////////////////////////////////////////////////////////////////////////
#define DISTANCE sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1))
#define DRAWCOLOR (int)xy
void CDrawPipe::CalcuLineSegment(int x1, int y1, int x2, int y2)
{
	double nRadius = (double)m_fEdgeWidth;

	int x21 = x2 - x1;
	int y21 = y2 - y1;
	int ax21 = abs(x21);
	int ay21 = abs(y21);
	
	//确定垂线上x,y每次循环的增量
	int dx, dy;
	if ((x21 > 0 && y21 > 0) || (x21 < 0) && (y21 < 0))
	{
		dx = 1;
		dy = -1;
	}
	else
	{
		dx = 1;
		dy = 1;
	}

	//按从中心到两边的顺序计算管道两头上的点坐标
	int x = x1;
	int y = y1;
	int m;
	double xy;
	if (abs(x21) < abs(y21))
	{	//沿x方向增加，y方向随动
		m = int(ay21 / 2 + 0.5);
		for (;;)
		{
			xy = DISTANCE;
			if (xy > nRadius)
				return;
			AddLine(x, y, x + x21, y + y21, DRAWCOLOR);
			if (int(xy) == int(nRadius + 0.5))
				return;
			AddLine(2 * x1 - x, 2 * y1 - y, x1 + x2 - x, y1 + y2 - y, DRAWCOLOR);
			x += dx;
			m += ax21;
			if (m > ay21)
			{
				xy = DISTANCE;
				if (xy > nRadius)
					return;
				AddLine(x, y, x + x21, y + y21, DRAWCOLOR);
				if (int(xy) == int(nRadius + 0.5))
					return;
				AddLine(2 * x1 - x, 2 * y1 - y, x1 + x2 - x, y1 + y2 - y, DRAWCOLOR);
				y += dy;
				m -= ay21;
			}
		}
	}
	else
	{	//沿y方向增加，x方向随动
		m = int(ax21 / 2 + 0.5);
		for (;;)
		{
			xy = DISTANCE;
			if (xy > nRadius)
				return;
			AddLine(x, y, x + x21, y + y21, DRAWCOLOR);
			if (int(xy) == int(nRadius + 0.5))
				return;
			AddLine(2 * x1 - x, 2 * y1 - y, x1 + x2 - x, y1 + y2 - y, DRAWCOLOR);
			y += dy;
			m += ay21;
			if (m > ax21)
			{
				xy = DISTANCE;
				if (xy > nRadius)
					return;
				AddLine(x, y, x + x21, y + y21, DRAWCOLOR);
				if (int(xy) == int(nRadius + 0.5))
					return;
				AddLine(2 * x1 - x, 2 * y1 - y, x1 + x2 - x, y1 + y2 - y, DRAWCOLOR);
				x += dx;
				m -= ax21;
			}
		}
	}
}

STDMETHODIMP CDrawPipe::get_Flow(VARIANT_BOOL *pVal)
{
	*pVal = m_bFlowShow ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CDrawPipe::put_Flow(VARIANT_BOOL newVal)
{
	BOOL bNewVal = (newVal != VARIANT_FALSE);
	if (m_bFlowShow == bNewVal)
		return S_OK;

	m_bFlowShow = bNewVal;
	if (!m_bFlowShow)
		m_spDrawObjectSite->SetTimer(0);
	else
		m_spDrawObjectSite->SetTimer(50);

	m_bNeedRecalGeometry = TRUE;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawPipe::get_FlowDirection(enumFlowDirection *pVal)
{
	*pVal = m_enumFlowDirection;
	
	return S_OK;
}

STDMETHODIMP CDrawPipe::put_FlowDirection(enumFlowDirection newVal)
{
	if (m_enumFlowDirection == newVal)
		return S_OK;

	m_enumFlowDirection = newVal;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawPipe::get_FlowSpeed(enumFlowSpeed *pVal)
{
	*pVal = m_enumFlowSpeed;

	return S_OK;
}

STDMETHODIMP CDrawPipe::put_FlowSpeed(enumFlowSpeed newVal)
{
	m_enumFlowSpeed = newVal;
	
	return S_OK;
}

STDMETHODIMP CDrawPipe::get_LiquidColor(OLE_COLOR *pVal)
{
	*pVal = m_clrLiquid;

	return S_OK;
}

STDMETHODIMP CDrawPipe::put_LiquidColor(OLE_COLOR newVal)
{
	if (m_clrLiquid == newVal)
		return S_OK;

	m_clrLiquid = newVal;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawPipe::put_EdgeColor(OLE_COLOR newVal)
{
	if (m_clrEdgeColor == newVal)
		return S_OK;

	m_clrEdgeColor = newVal;
	
	m_bNeedRecalGeometry = TRUE;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawPipe::put_HighLightEdgeColor(/*[in]*/ OLE_COLOR newVal)
{
	if (m_clrHighLightEdge == newVal)
		return S_OK;
	
	m_clrHighLightEdge = newVal;
	
	m_bNeedRecalGeometry = TRUE;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawPipe::put_JoinStyle(/*[in]*/ enumLineJoinStyle newVal)
{
	if (m_enumLineJoinStyle == newVal)
		return S_OK;
	
	m_enumLineJoinStyle = newVal;
	m_bNeedRecalGeometry = TRUE;
	FireViewChange();

	return S_OK;
}

