// PolyLineInRect.h: 
// 绘制由矩形裁减的多边形
//////////////////////////////////////////////////////////////////////

#ifndef __POLYLINEINRECT_H_
#define __POLYLINEINRECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 得到相对于矩形点所在区域
inline int GetAreaToRect(POINT point, RECT rect)
{
	if (point.x < rect.left)
	{
		if (point.y < rect.top)
			return 1;
		else if (point.y > rect.bottom)
			return 7;
		else
			return 8;
	}
	else if (point.x > rect.right)
	{
		if (point.y < rect.top)
			return 3;
		else if (point.y > rect.bottom)
			return 5;
		else
			return 4;
	}
	else
	{
		if (point.y < rect.top)
			return 2;
		else if (point.y > rect.bottom)
			return 6;
		else
			return 0;
	}
}

#define INPUTX TRUE
#define INPUTY FALSE

// 计算直线上的第三个点的横座标或者纵座标
BOOL GetThirdPointOnline(POINT point1, POINT point2, POINT& point, BOOL bInputX)
{
	if (bInputX)
	{
		if (point2.x != point1.x)
		{
			point.y = (point.x - point1.x) * (point2.y - point1.y) / (point2.x - point1.x) + point1.y;
			return TRUE;
		}
	}
	else 
	{
		if (point2.y != point1.y)
		{
			point.x = (point.y - point1.y) * (point2.x - point1.x) / (point2.y - point1.y) + point1.x;
			return TRUE;
		}
	}

	return FALSE;
}

// 绘制由矩形裁减的矩形
inline void PolyLineInRect(HDC hdc, const POINT* pPoints, int nPoint, RECT rect)
{
	POINT point, ptTemp;
	int nArea;
	POINT ptLast = pPoints[0];
	int nAreaLast = GetAreaToRect(ptLast, rect);
	
	for (int i = 1; i < nPoint; i++)
	{	
		point = pPoints[i];
		nArea = GetAreaToRect(point, rect);

		BOOL bDraw = FALSE;

		int nAreaTemp = -1;
		if (nAreaLast == 0)
		{
			::MoveToEx(hdc, ptLast.x, ptLast.y, NULL);

			if (nArea == 0)
			{
				::LineTo(hdc, point.x, point.y);
				ptLast = point;
				nAreaLast = nArea;
				continue;
			}

			nAreaTemp = nArea;
		}
		else if (nArea == 0)
		{
			::MoveToEx(hdc, point.x, point.y, NULL);
			nAreaTemp = nAreaLast;
		}

		if (nAreaTemp != -1)
		{
			switch (nAreaTemp)
			{
			case 1:
				ptTemp.x = rect.left;
				if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTX))
				{
					if (ptTemp.y >= rect.top && ptTemp.y <= rect.bottom)
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
						break;
					}
				}

				ptTemp.y = rect.top;
				if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTY))
				{
					if (ptTemp.x >= rect.left && ptTemp.x <= rect.right)
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
						break;
					}
				}
				break;
			case 2:
				ptTemp.y = rect.top;
				if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTY))
				{
					if (ptTemp.x >= rect.left && ptTemp.x <= rect.right)
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
						break;
					}
				}
				break;
			case 3:
				ptTemp.x = rect.right;
				if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTX))
				{
					if (ptTemp.y >= rect.top && ptTemp.y <= rect.bottom)
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
						break;
					}
				}
				
				ptTemp.y = rect.top;
				if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTY))
				{
					if (ptTemp.x >= rect.left && ptTemp.x <= rect.right)
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
						break;
					}
				}
				break;
			case 4:
				ptTemp.x = rect.right;
				if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTX))
				{
					if (ptTemp.y >= rect.top && ptTemp.y <= rect.bottom)
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
						break;
					}
				}
				break;
			case 5:
				ptTemp.x = rect.right;
				if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTX))
				{
					if (ptTemp.y >= rect.top && ptTemp.y <= rect.bottom)
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
						break;
					}
				}
				
				ptTemp.y = rect.bottom;
				if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTY))
				{
					if (ptTemp.x >= rect.left && ptTemp.x <= rect.right)
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
						break;
					}
				}
				break;
			case 6:
				ptTemp.y = rect.bottom;
				if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTY))
				{
					if (ptTemp.x >= rect.left && ptTemp.x <= rect.right)
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
						break;
					}
				}
				break;
			case 7:
				ptTemp.x = rect.left;
				if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTX))
				{
					if (ptTemp.y >= rect.top && ptTemp.y <= rect.bottom)
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
						break;
					}
				}
				
				ptTemp.y = rect.bottom;
				if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTY))
				{
					if (ptTemp.x >= rect.left && ptTemp.x <= rect.right)
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
						break;
					}
				}
				break;
			case 8:
				ptTemp.x = rect.left;
				if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTX))
				{
					if (ptTemp.y >= rect.top && ptTemp.y <= rect.bottom)
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
						break;
					}
				}
				break;
			default:
				ATLASSERT(FALSE);
			}
		}
		else
		{
			switch (nAreaLast)
			{
			case 1:
				if (nArea == 4 || nArea == 5 || nArea == 6)
					bDraw = TRUE;
				break;
			case 2:
				if (nArea != 1 && nArea != 2 && nArea != 3)
					bDraw = TRUE;
				break;
			case 3:
				if (nArea == 6 || nArea == 7 || nArea == 8)
					bDraw = TRUE;
				break;
			case 4:
				if (nArea != 3 && nArea != 4 && nArea != 5)
					bDraw = TRUE;
				break;
			case 5:
				if (nArea == 1 || nArea == 2 || nArea == 8)
					bDraw = TRUE;
				break;
			case 6:
				if (nArea != 5 && nArea != 6 && nArea != 7)
					bDraw = TRUE;
				break;
			case 7:
				if (nArea == 2 || nArea == 3 || nArea == 4)
					bDraw = TRUE;
				break;
			case 8:
				if (nArea != 1 && nArea != 8 && nArea != 7)
					bDraw = TRUE;
				break;
			default:
				ATLASSERT(FALSE);
			}
		}

		if (bDraw)
		{
			BOOL bMoveTo = TRUE;
			ptTemp.x = rect.left;
			if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTX))
			{
				if (ptTemp.y >= rect.top && ptTemp.y <= rect.bottom)
				{
					if (bMoveTo)
					{
						::MoveToEx(hdc, ptTemp.x, ptTemp.y, NULL);
						bMoveTo = FALSE;
					}
					else
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
					}
				}
			}
			
			ptTemp.x = rect.right;
			if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTX))
			{
				if (ptTemp.y >= rect.top && ptTemp.y <= rect.bottom)
				{
					if (bMoveTo)
					{
						::MoveToEx(hdc, ptTemp.x, ptTemp.y, NULL);
						bMoveTo = FALSE;
					}
					else
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
					}
				}
			}
			
			ptTemp.y = rect.top;
			if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTY))
			{
				if (ptTemp.x >= rect.left && ptTemp.x <= rect.right)
				{
					if (bMoveTo)
					{
						::MoveToEx(hdc, ptTemp.x, ptTemp.y, NULL);
						bMoveTo = FALSE;
					}
					else
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
					}
				}
			}
			
			ptTemp.y = rect.bottom;
			if (GetThirdPointOnline(point, ptLast, ptTemp, INPUTY))
			{
				if (ptTemp.x >= rect.left && ptTemp.x <= rect.right)
				{
					if (bMoveTo)
					{
						::MoveToEx(hdc, ptTemp.x, ptTemp.y, NULL);
						bMoveTo = FALSE;
					}
					else
					{
						::LineTo(hdc, ptTemp.x, ptTemp.y);
					}
				}
			}
		}

		ptLast = point;
		nAreaLast = nArea;
	}
}

#endif //__POLYLINEINRECT_H_