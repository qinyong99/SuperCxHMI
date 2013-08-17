// DevObj.cpp: implementation of the CDevObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"
//#include "DynamicSet.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//根据两个点求相对角度
int CalculateAngle(POINT ptOrigin, POINT ptCal)
{
	int dx = ptCal.x - ptOrigin.x;
	int dy = ptOrigin.y - ptCal.y;
	if (dx == 0)
	{
		if (dy < 0)
			return 270;
		return 90;
	}
	return int(atan2(dy, dx) * 57.29578) % 360;
}

float CalculateAngle(POINTF ptOrigin, POINTF ptCal)
{
	float dx = ptCal.x - ptOrigin.x;
	float dy = ptOrigin.y - ptCal.y;
	if (dx == 0)
	{
		if (dy < 0)
			return 270;
		return 90;
	}
	return float(fmod(atan2(dy, dx) * 57.29578, 360));
}

POINTF CalculateRotatePoint(POINTF ptOrgin, POINTF ptCal, float fAngle)
{
	fAngle = -CalculateAngle(ptOrgin, ptCal) + fAngle;
	float RR = (float)sqrt((ptOrgin.x - ptCal.x) * (ptOrgin.x - ptCal.x) +
			(ptOrgin.y - ptCal.y) * (ptOrgin.y - ptCal.y));

	POINTF ptRet;
	ptRet.x =  ptOrgin.x + float(RR * cos(fAngle * 0.0174533));
	ptRet.y =  ptOrgin.y + float(RR * sin(fAngle * 0.0174533));

	return ptRet;
}

CImageList CDevObj::c_imageGeneralHandle;
CImageList CDevObj::c_imageRotateHandle;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CDevObj, CCmdTarget, 0)

CDevObj::CDevObj(CDevObjDoc* pContainer)
{
	m_dwVer = 0;
	m_dwSubObjNum = -1;
	m_pDocument = NULL;
	m_dwObjectNumber = 0;
	m_nLayer = 0;
	m_bLocked = FALSE;
	m_nPrivilege = 0;
	m_bVisible = TRUE;
	m_dwMiscStatus = 0;

	m_pDC = NULL;

	m_pGroupObj = NULL;

	if (pContainer != NULL)
		ASSERT_VALID(pContainer);
	
	if (pContainer != NULL)
		pContainer->AddObject(this);

	ASSERT(m_pDocument == pContainer);
	ASSERT_VALID(this);
}

CDevObj::~CDevObj()
{
}

void CDevObj::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	if (ar.IsStoring())
	{
		ASSERT_VALID(m_pDocument);  //文档不能为空

		m_dwVer = _SUPERCX_VER;
		ar << m_dwVer;

		ar << m_dwObjectNumber;   // save the object number
		ar << m_dwSubObjNum;
		ar << m_nLayer;
		ar << m_bLocked;
		ar << m_bVisible;
		ar << m_strDisplayName;
		ar << m_strToolTip;
		ar << m_nPrivilege;
		ar << m_dwMiscStatus;
		ar << m_strDescription;
	}
	else
	{
		if (m_pDocument == NULL)
		{
			CDevObjDoc* pContainerDoc = (CDevObjDoc*)ar.m_pDocument;
			ASSERT_VALID(pContainerDoc);
			ASSERT_KINDOF(CDevObjDoc, pContainerDoc);
			m_pDocument = pContainerDoc;
			ASSERT(pContainerDoc == m_pDocument);
		}
		ASSERT(m_pDocument != NULL);  //文档不能为空

		if (m_pDocument->m_dwVer == _SUPERCX_OLDVER)
			m_dwVer = _SUPERCX_OLDVER;
		else
			ar >> m_dwVer;
		
		ar >> m_dwObjectNumber;
		ar >> m_dwSubObjNum;
		ar >> m_nLayer;
		if (m_dwVer >= _SUPERCX_VER_0203)
			ar >> m_bLocked;
		ar >> m_bVisible;
		ar >> m_strDisplayName;
		ar >> m_strToolTip;
		ar >> m_nPrivilege;
		ar >> m_dwMiscStatus;

		if (m_dwVer != _SUPERCX_OLDVER)
			ar >> m_strDescription;

		CNamedDevobjMap* pMap = m_pDocument->GetNamedObjectsMap();
		CString strName = GetDisplayName(); 
		CDevObj* pSearchObj = NULL;
		ASSERT(!pMap->Lookup(strName, pSearchObj));
		pMap->SetAt(strName, this);
		
		if (m_dwObjectNumber >= m_pDocument->m_dwNextObjectNumber)
			m_pDocument->m_dwNextObjectNumber = m_dwObjectNumber + 1;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDevObj diagnostics

#ifdef _DEBUG
void CDevObj::AssertValid() const
{
	CObject::AssertValid();
	if (m_pDocument != NULL)
		m_pDocument->AssertValid();
}

void CDevObj::Dump(CDumpContext& dc) const
{
	CCmdTarget::Dump(dc);

	dc << "m_pDocument = " << (void*)m_pDocument;
	dc << "\n";
}
#endif //_DEBUG

///////////////////////////////////////////////////////////


//以矩形的缺省处理
int CDevObj::GetHandleCount(TrackerState state)
{
	if (!IsDrawVisible())	
		return 0;

	ASSERT_VALID(this);
	if (state == resize || state == general)
		return 8;
	else if (state == rotate)
		return 5;
	
	return 0;
}

//返回把柄中心逻辑坐标
//HandleAsHandle是这个把柄以什么把柄的处理方式，用在移动把柄时
CPoint CDevObj::GetHandle(int nHandle, TrackerState state, CLayoutView* pView)
{
	ASSERT_VALID(this);

	int x, y, xCenter, yCenter;
	CRect rect = GetPositionRect().GetRECT();
	xCenter = (rect.left + rect.right) / 2;
	yCenter = (rect.top + rect.bottom) / 2;

	if (state == resize || state == general)
	{
		switch (nHandle)
		{
		case 1:
			x = rect.left;
			y = rect.top;
			break;

		case 2:
			x = xCenter;
			y = rect.top;
			break;

		case 3:
			x = rect.right;
			y = rect.top;
			break;

		case 4:
			x = rect.right;
			y = yCenter;
			break;

		case 5:
			x = rect.right;
			y = rect.bottom;
			break;

		case 6:
			x = xCenter;
			y = rect.bottom;
			break;

		case 7:
			x = rect.left;
			y = rect.bottom;
			break;

		case 8:
			x = rect.left;
			y = yCenter;
			break;

		default:
			ASSERT(FALSE);
		}
	}
	else if (state == rotate)
	{
		CPoint ptBase = GetRotateBasePoint();
	//	rect.InflateRect(16, 16);
		switch (nHandle)
		{
		case 1:
			x = ptBase.x;
			y = ptBase.y;
			break;

		case 2:
			x = rect.left;
			y = rect.top;
			break;

		case 3:
			x = rect.right;
			y = rect.top;
			break;

		case 4:
			x = rect.right;
			y = rect.bottom;
			break;

		case 5:
			x = rect.left;
			y = rect.bottom;
			break;

		default:
			ASSERT(FALSE);
		}
	}
	else
		ASSERT(FALSE);

	return CPoint(x, y);
}

//鼠标在把柄上光标的形状
HCURSOR CDevObj::GetHandleCursor(int nHandle, TrackerState state)
{
	ASSERT_VALID(this);

	LPCTSTR id;
	if (state == resize || state == general)
	{
		switch (nHandle)
		{
		case 1:
		case 5:
			id = IDC_SIZENWSE;
			break;

		case 2:
		case 6:
			id = IDC_SIZENS;
			break;

		case 3:
		case 7:
			id = IDC_SIZENESW;
			break;

		case 4:
		case 8:
			id = IDC_SIZEWE;
			break;
			default:
			ASSERT(FALSE);
		}
	}
	else if (state == rotate)
	{
		if (nHandle == 1)
			id = IDC_CROSS;
		else
		{
			return AfxGetApp()->LoadCursor(IDC_ROTATE);;
		}
	}
	else
		ASSERT(FALSE);


	return AfxGetApp()->LoadStandardCursor(id);
}

//point为逻辑坐标
BOOL CDevObj::HitTest(CPoint point, CLayoutView* pView)
{
	if (IsDrawVisible())
	//	return GetBoundingRect().PtInRect(point);
		return  (point.x >= m_position.left && point.x < m_position.right &&
				point.y >= m_position.top && point.y < m_position.bottom);

	return FALSE;
}

//注意：如果bSelected，从左上角开始顺时钟加一，0为没有击中
//如果!bSelected，0为没有击中
//point为逻辑坐标
int CDevObj::HitTestHandle(CPoint point, CLayoutView* pView, TrackerState state)
{
	ASSERT_VALID(this);
	ASSERT(pView != NULL);

	pView->DocToClient(point);
	int nHandleCount = GetHandleCount(state);
	for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
	{
		CPoint handle = GetHandle(nHandle, state, pView);
		pView->DocToClient(handle);
		int nWidth = 5;
		if (state == rotate)
			nWidth = 7;
		if (point.x >= handle.x - nWidth && point.x < handle.x + nWidth &&
				point.y >= handle.y - nWidth && point.y < handle.y + nWidth)
				return nHandle;
	}
		
	return 0;
}

// rect 为逻辑坐标
BOOL CDevObj::Intersects(const CRect& rect)
{
	ASSERT_VALID(this);

	if (IsDrawVisible())
	{
		CRectF fixed = m_position;
		fixed.NormalizeRect();
		CRectF rectT = rect;
		rectT.NormalizeRect();
		return !(rectT & fixed).IsRectEmpty();
	}
	
	return FALSE;
}

//移动把柄到指定位置
void CDevObj::MoveHandleTo(int nHandle, TrackerState state, CPoint point, CLayoutView* pView)
{
	ASSERT_VALID(this);

	if (state == resize || state == general)
	{
		CRect position = m_position.GetRECT();
		int nOldWidth = position.Width();
		int nOldHeight = position.Height();
		int nNewWidth;
		int nNewHeight;

		switch (nHandle)
		{
		case 1:
			position.top = point.y;
			position.left = point.x;
			if (IsUniformScale())
			{
				nNewWidth = position.Width();
				nNewHeight = position.Height();	
				if (abs(nNewHeight * nOldWidth) > abs(nNewWidth * nOldHeight))
				{
					if (nOldHeight != 0)
						position.left = position.right - (nOldWidth * nNewHeight) / nOldHeight;
				}
				else if (abs(nNewHeight * nOldWidth) < abs(nNewWidth * nOldHeight))
				{
					if (nOldWidth != 0)
						position.top = position.bottom - (nOldHeight * nNewWidth) / nOldWidth;
				}
			}
			break;

		case 2:
			position.top = point.y;
			if (IsUniformScale())
			{
				nNewHeight = position.Height();	
				if (nOldHeight != 0)
				{
					int nCenter = (position.left + position.right) / 2;
					int nNewHalfWidth = nOldWidth * nNewHeight / (2 * nOldHeight);
					position.left = nCenter - abs(nNewHalfWidth);
					position.right = nCenter + abs(nNewHalfWidth);
				}
			}
			break;

		case 3:
			position.top = point.y;
			position.right = point.x;
			if (IsUniformScale())
			{
				nNewWidth = position.Width();
				nNewHeight = position.Height();	
				if (abs(nNewHeight * nOldWidth) > abs(nNewWidth * nOldHeight))
				{
					if (nOldHeight != 0)
						position.right = position.left + (nOldWidth * nNewHeight) / nOldHeight;
				}
				else if (abs(nNewHeight * nOldWidth) < abs(nNewWidth * nOldHeight))
				{
					if (nOldWidth != 0)
						position.top = position.bottom - (nOldHeight * nNewWidth) / nOldWidth;
				}
			}
			break;

		case 4:
			position.right = point.x;
			if (IsUniformScale())
			{
				nNewWidth = position.Width();	
				if (nOldHeight != 0)
				{
					int nCenter = (position.top + position.bottom) / 2;
					int nNewHalfHeight = nOldHeight * nNewWidth / (2 * nOldWidth);
					position.top = nCenter - abs(nNewHalfHeight);
					position.bottom = nCenter + abs(nNewHalfHeight);
				}
			}
			break;

		case 5:
			position.right = point.x;
			position.bottom = point.y;
			if (IsUniformScale())
			{
				nNewWidth = position.Width();
				nNewHeight = position.Height();	
				if (abs(nNewHeight * nOldWidth) > abs(nNewWidth * nOldHeight))
				{
					if (nOldHeight != 0)
						position.right = position.left + (nOldWidth * nNewHeight) / nOldHeight;
				}
				else if (abs(nNewHeight * nOldWidth) < abs(nNewWidth * nOldHeight))
				{
					if (nOldWidth != 0)
						position.bottom = position.top + (nOldHeight * nNewWidth) / nOldWidth;
				}
			}
			break;

		case 6:
			position.bottom = point.y;
			if (IsUniformScale())
			{
				nNewHeight = position.Height();	
				if (nOldHeight != 0)
				{
					int nCenter = (position.left + position.right) / 2;
					int nNewHalfWidth = nOldWidth * nNewHeight / (2 * nOldHeight);
					position.left = nCenter - abs(nNewHalfWidth);
					position.right = nCenter + abs(nNewHalfWidth);
				}
			}
			break;

		case 7:
			position.left = point.x;
			position.bottom = point.y;
			if (IsUniformScale())
			{
				nNewWidth = position.Width();
				nNewHeight = position.Height();	
				if (abs(nNewHeight * nOldWidth) > abs(nNewWidth * nOldHeight))
				{
					if (nOldHeight != 0)
						position.left = position.right - (nOldWidth * nNewHeight) / nOldHeight;
				}
				else if (abs(nNewHeight * nOldWidth) < abs(nNewWidth * nOldHeight))
				{
					if (nOldWidth != 0)
						position.bottom = position.top + (nOldHeight * nNewWidth) / nOldWidth;
				}
			}
			break;

		case 8:
			position.left = point.x;
			if (IsUniformScale())
			{
				nNewWidth = position.Width();	
				if (nOldHeight != 0)
				{
					int nCenter = (position.top + position.bottom) / 2;
					int nNewHalfHeight = nOldHeight * nNewWidth / (2 * nOldWidth);
					position.top = nCenter - abs(nNewHalfHeight);
					position.bottom = nCenter + abs(nNewHalfHeight);
				}
			}
			break;

		default:
			ASSERT(FALSE);
			}
		MoveTo(CRectF(position), FALSE, pView);
	}
	else if (state == rotate)
	{
		if (nHandle == 1)
			SetRotateBasePoint(point);
	}
}

//刷新界面（与DrawCli例子不同）
void CDevObj::Invalidate(CLayoutView* pView)
{
	ASSERT_VALID(this);
	
	m_pDocument->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, this);


//	if (pView == NULL)
//		m_pDocument->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, this);
//	else 
//		pView->OnUpdate(NULL, HINT_UPDATE_DRAWOBJ, this);

}

CDevObj::TrackerState CDevObj::GetTrackerState(TrackerState state)
{
	return resize;
}

//对象移动到指定位置（与DrawCli例子不同）
void CDevObj::MoveTo(const CRectF &position, BOOL bUniformScale, CLayoutView* pView)
{
	ASSERT_VALID(this);

	if (position == m_position)
		return;

	m_position = position;
}

void CDevObj::Rotate(int lAngle, CLayoutView* pView)
{
}

void CDevObj::RotateAt(CPointF point, float fAngle, CLayoutView* pView)
{
	CPointF ptCenter = m_position.CenterPoint();
	CPointF ptNew = CalculateRotatePoint(point, ptCenter, fAngle);
	m_position.OffsetRect(ptNew - ptCenter);
}

void CDevObj::ScaleAt(CPointF point, float fHorScale, float fVerScale, CLayoutView* pView)
{
	m_position.OffsetRect(-point);
	m_position.left *= fHorScale;
	m_position.right *= fHorScale;
	m_position.top *= fVerScale;
	m_position.bottom *= fVerScale;
	m_position.OffsetRect(point);
}

//绘制跟踪框
void CDevObj::DrawTracker(CDC* pDC, TrackerState state, CLayoutView* pView)
{
	ASSERT_VALID(this);

	int nHandleCount = GetHandleCount(state);
	for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
	{
		CPoint handle = GetHandle(nHandle, state, pView);
		if (pView != NULL)
			pView->DocToClient(handle);
		if (state == rotate)
		{
			ASSERT(nHandle >= 1 && nHandle <= 5);
			if (nHandle == 1)
			{
				pDC->SetROP2(R2_NOTXORPEN);
				CBrush* pOldBrush = pDC->SelectObject(CBrush::FromHandle((HBRUSH )GetStockObject(BLACK_BRUSH)));
				pDC->Ellipse(handle.x - 2, handle.y - 2, handle.x + 2, handle.y + 2);
				pDC->SelectObject(pOldBrush);
				pDC->Ellipse(handle.x - 7, handle.y - 7, handle.x + 7, handle.y + 7);
			}
			else
			{
				handle -= CPoint(5, 5);
				if (m_bLocked)
					c_imageGeneralHandle.Draw(pDC, 5, handle, ILD_NORMAL);
				else 
					c_imageRotateHandle.Draw(pDC, nHandle - 1, handle, ILD_NORMAL);
			}
		}
		else if (GetTrackerState(CDevTool::c_enumTrackerState) == resize)
		{
			handle -= CPoint(5, 5);
			int nImageStart = 6;
			if (pView != NULL)
				if (pView->m_pFocusObj == this && pView->m_selection.Find(pView->m_pFocusObj) != NULL)
					nImageStart = 0;

			if (m_bLocked)
				c_imageGeneralHandle.Draw(pDC, 5 + nImageStart, handle, ILD_NORMAL);
			else if (nHandle > 8)
				c_imageGeneralHandle.Draw(pDC, 4 + nImageStart, handle, ILD_NORMAL);
			else if (nHandle < 5)
				c_imageGeneralHandle.Draw(pDC, nHandle - 1 + nImageStart, handle, ILD_NORMAL);
			else
				c_imageGeneralHandle.Draw(pDC, nHandle - 5 + nImageStart, handle, ILD_NORMAL);
		}
		else
		{
			handle -= CPoint(5, 5);
			int nImageStart = 6;
			if (pView != NULL)
				if (pView->m_pFocusObj == this && pView->m_selection.Find(pView->m_pFocusObj) != NULL)
					nImageStart = 0;

			if (m_bLocked)
				c_imageGeneralHandle.Draw(pDC, 5 + nImageStart, handle, ILD_NORMAL);
			else 
				c_imageGeneralHandle.Draw(pDC, 4 + nImageStart, handle, ILD_NORMAL);
		}
	}
}

void CDevObj::DrawTrackerHandleShape(CDC* pDC, int nHandle, TrackerState state, CPoint point, CLayoutView* pView)
{
	CRect position = GetPositionRect().GetRECT();

	if (state == resize || state == general)
	{
		int nOldWidth = position.Width();
		int nOldHeight = position.Height();
		int nNewWidth;
		int nNewHeight;

		switch (nHandle)
		{
		case 1:
			position.top = point.y;
			position.left = point.x;
			if (IsUniformScale())
			{
				nNewWidth = position.Width();
				nNewHeight = position.Height();	
				if (abs(nNewHeight * nOldWidth) > abs(nNewWidth * nOldHeight))
				{
					if (nOldHeight != 0)
						position.left = position.right - (nOldWidth * nNewHeight) / nOldHeight;
				}
				else if (abs(nNewHeight * nOldWidth) < abs(nNewWidth * nOldHeight))
				{
					if (nOldWidth != 0)
						position.top = position.bottom - (nOldHeight * nNewWidth) / nOldWidth;
				}
			}
			break;

		case 2:
			position.top = point.y;
			if (IsUniformScale())
			{
				nNewHeight = position.Height();	
				if (nOldHeight != 0)
				{
					int nCenter = (position.left + position.right) / 2;
					int nNewHalfWidth = nOldWidth * nNewHeight / (2 * nOldHeight);
					position.left = nCenter - nNewHalfWidth;
					position.right = nCenter + nNewHalfWidth;
				}
			}
			break;

		case 3:
			position.top = point.y;
			position.right = point.x;
			if (IsUniformScale())
			{
				nNewWidth = position.Width();
				nNewHeight = position.Height();	
				if (abs(nNewHeight * nOldWidth) > abs(nNewWidth * nOldHeight))
				{
					if (nOldHeight != 0)
						position.right = position.left + (nOldWidth * nNewHeight) / nOldHeight;
				}
				else if (abs(nNewHeight * nOldWidth) < abs(nNewWidth * nOldHeight))
				{
					if (nOldWidth != 0)
						position.top = position.bottom - (nOldHeight * nNewWidth) / nOldWidth;
				}
			}
			break;

		case 4:
			position.right = point.x;
			if (IsUniformScale())
			{
				nNewWidth = position.Width();	
				if (nOldHeight != 0)
				{
					int nCenter = (position.top + position.bottom) / 2;
					int nNewHalfHeight = nOldHeight * nNewWidth / (2 * nOldWidth);
					position.top = nCenter - nNewHalfHeight;
					position.bottom = nCenter + nNewHalfHeight;
				}
			}
			break;

		case 5:
			position.right = point.x;
			position.bottom = point.y;
			if (IsUniformScale())
			{
				nNewWidth = position.Width();
				nNewHeight = position.Height();	
				if (abs(nNewHeight * nOldWidth) > abs(nNewWidth * nOldHeight))
				{
					if (nOldHeight != 0)
						position.right = position.left + (nOldWidth * nNewHeight) / nOldHeight;
				}
				else if (abs(nNewHeight * nOldWidth) < abs(nNewWidth * nOldHeight))
				{
					if (nOldWidth != 0)
						position.bottom = position.top + (nOldHeight * nNewWidth) / nOldWidth;
				}
			}
			break;

		case 6:
			position.bottom = point.y;
			if (IsUniformScale())
			{
				nNewHeight = position.Height();	
				if (nOldHeight != 0)
				{
					int nCenter = (position.left + position.right) / 2;
					int nNewHalfWidth = nOldWidth * nNewHeight / (2 * nOldHeight);
					position.left = nCenter - nNewHalfWidth;
					position.right = nCenter + nNewHalfWidth;
				}
			}
			break;

		case 7:
			position.left = point.x;
			position.bottom = point.y;
			if (IsUniformScale())
			{
				nNewWidth = position.Width();
				nNewHeight = position.Height();	
				if (abs(nNewHeight * nOldWidth) > abs(nNewWidth * nOldHeight))
				{
					if (nOldHeight != 0)
						position.left = position.right - (nOldWidth * nNewHeight) / nOldHeight;
				}
				else if (abs(nNewHeight * nOldWidth) < abs(nNewWidth * nOldHeight))
				{
					if (nOldWidth != 0)
						position.bottom = position.top + (nOldHeight * nNewWidth) / nOldWidth;
				}
			}
			break;

		case 8:
			position.left = point.x;
			if (IsUniformScale())
			{
				nNewWidth = position.Width();	
				if (nOldHeight != 0)
				{
					int nCenter = (position.top + position.bottom) / 2;
					int nNewHalfHeight = nOldHeight * nNewWidth / (2 * nOldWidth);
					position.top = nCenter - nNewHalfHeight;
					position.bottom = nCenter + nNewHalfHeight;
				}
			}
			break;

		default:
			ASSERT(FALSE);
		}

		pDC->SetROP2(R2_NOTXORPEN);	
		pDC->Rectangle(position);
		
	}
	else if (state == rotate)
	{
		ASSERT(nHandle >= 1 && nHandle <= 5);
		CPoint ptBase;
		pDC->SetROP2(R2_NOTXORPEN);
		if (nHandle == 1)
		{
			ptBase = point;
		}
		else
		{
		//	pDC->SetROP2(R2_BLACK);
			ptBase = GetRotateBasePoint();
		}
		CBrush* pOldBrush = pDC->SelectObject(CBrush::FromHandle((HBRUSH )GetStockObject(BLACK_BRUSH)));
		CSize size2(2, 2);
		pDC->DPtoLP(&size2);
		pDC->Ellipse(ptBase.x - size2.cx, ptBase.y - size2.cy, ptBase.x + size2.cx, ptBase.y + size2.cy);
		pDC->SelectObject(pOldBrush);
		CSize size7(7, 7);
		pDC->DPtoLP(&size7);
		pDC->Ellipse(ptBase.x - size7.cx, ptBase.y - size7.cy, ptBase.x + size7.cx, ptBase.y + size7.cy);
		if (nHandle == 1)
		{
			CSize size8(8, 8);
			pDC->DPtoLP(&size8);
		//	pDC->SetROP2(R2_BLACK);
			ptBase.x = (position.left + position.right) / 2;
			ptBase.y = (position.top + position.bottom) / 2;
			pDC->MoveTo(ptBase.x - size7.cx, ptBase.y);
			pDC->LineTo(ptBase.x + size8.cx, ptBase.y);
			pDC->MoveTo(ptBase.x, ptBase.y - size7.cy);
			pDC->LineTo(ptBase.x, ptBase.y + size8.cy);
		}
	}
}

void CDevObj::DrawRotateAtShape(CDC* pDC, CPointF point, long lAngle)
{
	pDC->SetROP2(R2_NOTXORPEN);	
	CRectF position = m_position;
	CPointF ptCenter = position.CenterPoint();
	CPointF ptNew = CalculateRotatePoint(point, ptCenter, float(lAngle));
	position.OffsetRect(ptNew - ptCenter);
	pDC->Rectangle(&position.GetRECT());
}

void CDevObj::DrawTrackerShape(CDC* pDC)
{
	CRect rect = m_position.GetRECT();
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.top);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.left, rect.bottom);
	pDC->LineTo(rect.left, rect.top);
}

CPoint CDevObj::GetRotateBasePoint()
{
	return CPoint(0, 0);
}

void CDevObj::SetRotateBasePoint(CPoint point)
{
}

///////////////////////////////////////////////////////////
// 复制

void CDevObj::CopyToClipboard(CArchive& ar, IStorage* pStorage)
{
	ar << GetDisplayName();
	ar << m_dwSubObjNum;
	ar << m_nLayer;
	ar << m_bLocked;
	ar << m_bVisible;
	ar << m_strToolTip;
	ar << m_nPrivilege;
	ar << m_dwMiscStatus;
}

void RemoveRightDigits(CString& str)
{
	USES_CONVERSION;
	
	LPWSTR psz = T2W(str);
	int nLen = wcslen(psz);
	if (nLen > 0)
	{
		LPWSTR p = psz + nLen;
		while (iswdigit(*(p - 1)))
			p--;
		*p = '\0';
		str = psz;
	}
}

BOOL CDevObj::CreateFromClipboard(CArchive& ar, IStorage* pStorage)
{ 
	CString strName;
	ar >> strName;
	
	if (!PutDisplayName(strName))
	{
		CString strBaseName;
	
		if (!IsBaseObject())
		{
			strBaseName = strName;
			RemoveRightDigits(strBaseName);
		}
		
		m_pDocument->CreateUniqueObjectName(this, (IsBaseObject() || strBaseName.IsEmpty()) ? LPCTSTR(NULL) : strBaseName); // 使用源对象名字作为基本名
	}

	ar >> m_dwSubObjNum;
	ar >> m_nLayer;
	ar >> m_bLocked;
	ar >> m_bVisible;
	ar >> m_strToolTip;
	ar >> m_nPrivilege;
	ar >> m_dwMiscStatus;
// 	m_dwObjectNumber = GetNewObjectNumber();
	
	return TRUE; 
}

//////////////////////////////////////////////////////////////////////////////////////
//功能：从存贮装载动态属性
//////////////////////////////////////////////////////////////////////////////////////
HRESULT CDevObj::LoadDynamicProperty(IStream *pStm)
{
	HRESULT hr;

	if(!pStm)
	{
		return E_POINTER;
	}

	int nSize;
	pStm->Read(&nSize, sizeof(nSize), NULL);
	for(int nIndex = 0; nIndex < nSize; nIndex++)
	{
		DISPID dispid;
		pStm->Read(&dispid, sizeof(DISPID), NULL);
		IUnknownPtr pObject;
		hr = OleLoadFromStream(pStm, IID_IUnknown, (void**)&pObject);
		AddDynamic(dispid, pObject);
		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
//功能：往存贮保存动态属性
//////////////////////////////////////////////////////////////////////////////////////
HRESULT CDevObj::SaveDynamicProperty(IStream *pStm)
{
	HRESULT hr;

	if(!pStm)
	{
		return E_POINTER;
	}

	int nSize = m_arLinkInfo.GetSize();;
	pStm->Write(&nSize, sizeof(nSize), NULL);
	for(int nIndex = 0; nIndex < nSize; nIndex++)
	{
		pStm->Write(&m_arLinkInfo[nIndex].dispid, sizeof(DISPID), NULL);
		ICxDynamicObject* pObject = m_arLinkInfo[nIndex].pDynamicObject;
		if (pObject != NULL)
		{
			IPersistStreamPtr spStream;			
			hr = pObject->QueryInterface(IID_IPersistStream, (void**)&spStream);
			if (FAILED(hr))
				return hr;
			if (spStream != NULL)
			{	
				hr = OleSaveToStream(spStream, pStm);
				if (FAILED(hr))
					return hr;
			}
		}
	}

	return S_OK;
}

void CDevObj::SerializeDynamicProperty(CArchive& ar)
{
	CArchiveStream stm(&ar);
	LPSTREAM pStream = &stm;

	if (ar.IsStoring())
	{
		SaveDynamicProperty(pStream);
	}
	else
	{
		LoadDynamicProperty(pStream);
	}
}

BOOL CDevObj::GetPropertyPageCLSIDs( CArray< CLSID,
	CLSID& >& aclsidPages )
{
/*	ISpecifyPropertyPagesPtr pSpecify;
	CAUUID pages;
	HRESULT hResult;
	ULONG iPage;
//	CLSID clsid;

	pSpecify = m_lpObject;
	if( pSpecify != NULL )
	{
		pages.cElems = 0;
		pages.pElems = NULL;
		hResult = pSpecify->GetPages( &pages );
		if( FAILED( hResult ) )
		{
			TRACE("ISpecifyPropertyPages::GetPages() failed\n");
			return( FALSE );
		}

		for( iPage = 0; iPage < pages.cElems; iPage++ )
		{
			aclsidPages.Add( pages.pElems[iPage] );
		}

		CoTaskMemFree( pages.pElems );
	}

   if( m_pExtendedControl != NULL )
   {
	  clsid = __uuidof( TCExtPage );
	  aclsidPages.Add( clsid );
   }
*/
	return TRUE;
}

//得到唯一的对象编号，目的是用于序列化
DWORD CDevObj::GetNewObjectNumber()
{
	ASSERT_VALID(this);

	CDevObjDoc* pDoc = m_pDocument;
	ASSERT_VALID(pDoc);
	DWORD dwNextObjectNumber = pDoc->m_dwNextObjectNumber;

	for (;;)
	{
		//保证对象编号没有被其它对象使用
		CDevObjList* pObjList = pDoc->GetObjects();
		POSITION posObj = pObjList->GetHeadPosition();
		CDevObj* pObj = NULL;
		BOOL bCollision = FALSE;
		while (posObj != NULL)
		{
			pObj = pObjList->GetNext(posObj);
			if (pObj->m_dwObjectNumber == dwNextObjectNumber)
			{
				bCollision = TRUE;
				break;
			}
		}
		if (!bCollision)
			break;  // no item using m_dwNextObjectNumber

		// m_dwNextObjectNumber is in use, bump to next one!
		++dwNextObjectNumber;
	}

	pDoc->m_dwNextObjectNumber = dwNextObjectNumber + 1;
	return dwNextObjectNumber;
}

//依据对象编号产生对象名称
void CDevObj::GetObjectName(LPTSTR lpszObjectName) const
{
	ASSERT_VALID(this);
	ASSERT(lpszObjectName != NULL);

	wsprintf(lpszObjectName, _T("CDevObj %lu"), m_dwObjectNumber);
}

////////////////////////////////////////////////////////////////////////////
//功 能：释放对象
//描 述：与void CDevObj::Delete(BOOL bAutoDelete)的差别在于只处理
//与对象本身相关的东西
////////////////////////////////////////////////////////////////////////////
void CDevObj::Release()
{
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
		if (m_arLinkInfo[i].pDynamicObject != NULL)
		{
			m_arLinkInfo[i].pDynamicObject->Release();	
			m_arLinkInfo[i].pDynamicObject = NULL;
		}
}

////////////////////////////////////////////////////////////////////////////
//功 能：删除对象
//描 述：部分代码参照 void COleClientItem::Delete(BOOL bAutoDelete)
////////////////////////////////////////////////////////////////////////////
void CDevObj::Delete(BOOL bAutoDelete)
{	
	USES_CONVERSION;
	ASSERT_VALID(this);

	CDevObjDoc* pDoc = m_pDocument;

	//先把对象名字从快速索引表中删除
	VERIFY(pDoc->GetNamedObjectsMap()->RemoveKey(GetDisplayName()));
	pDoc->OnSymbolChanged(UPDATE_SYMBOL_OBJECT);

	Release();      // first close it
	
	if (pDoc != NULL && pDoc->m_bCompoundFile)
	{
		// cleanup docfile storage first
		CDevObjDoc* pDoc = m_pDocument;
		ASSERT_VALID(pDoc);

		if (pDoc->m_lpRootStg != NULL)
		{
			// get item name
			TCHAR szObjectName[64];
			GetObjectName(szObjectName);

			// attempt to remove it from the storage, ignore errors
			pDoc->m_lpRootStg->DestroyElement(T2COLE(szObjectName));
		}
	}

	if (bAutoDelete)
	{
		//是组合对象的一个子对象
		if (m_pGroupObj != NULL)
			m_pGroupObj->RemoveObject(this);

		//是文档中的一个根对象 
		else if (pDoc != NULL)
			pDoc->RemoveObject(this);

		InternalRelease();  // remove the item from memory
	}
}

BEGIN_INTERFACE_MAP(CDevObj, CCmdTarget)
	INTERFACE_PART(CDevObj, IID_IPropertyNotifySink, PropertyNotifySink)
END_INTERFACE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CDevObj::XPropertyNotifySink
///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_( ULONG ) CDevObj::XPropertyNotifySink::AddRef()
{
	METHOD_PROLOGUE(CDevObj, PropertyNotifySink)

	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CDevObj::XPropertyNotifySink::Release()
{
	METHOD_PROLOGUE(CDevObj, PropertyNotifySink)

    return pThis->ExternalRelease();
}

STDMETHODIMP CDevObj::XPropertyNotifySink::QueryInterface(
	REFIID iid, LPVOID* ppvObj )
{
	METHOD_PROLOGUE(CDevObj, PropertyNotifySink)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CDevObj::XPropertyNotifySink::OnChanged(
	DISPID dispid )
{
	AfxMessageBox("bb");
   	return S_OK;
}

STDMETHODIMP CDevObj::XPropertyNotifySink::OnRequestEdit(
	DISPID dispid)
{
	return S_OK;
}

////////////////////////////////////////////////////////////////////////
//原IDynamicSet接口函数
HRESULT CDevObj::AddDynamic(DISPID dispid, IUnknown *pUnknown)
{
	VARIANT_BOOL bHasConnected;
	HasDynamic(dispid, &bHasConnected);
	if (bHasConnected == VARIANT_TRUE)
	{
		RemoveDynamic(dispid);
	}

	if (pUnknown == NULL)
		return S_OK;

	LINKINFO linkinfo;
	memset(&linkinfo, 0, sizeof(LINKINFO));
	linkinfo.dispid = dispid;
	//在此增加引用
	ICxDynamicObject* pDynamicObject = NULL;
	pUnknown->QueryInterface(IID_ICxDynamicObject, (void **)&pDynamicObject);
	if (pDynamicObject == NULL)
		return S_FALSE;

	linkinfo.pDynamicObject = pDynamicObject;
	m_arLinkInfo.Add(linkinfo);

	return S_OK;
}

HRESULT CDevObj::HasDynamic(DISPID dispid, VARIANT_BOOL *pConnected)
{
	*pConnected = VARIANT_FALSE;
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		if (m_arLinkInfo[i].dispid == dispid)
		{
			*pConnected = VARIANT_TRUE;
			break;
		}
	}

	return S_OK;
}

HRESULT CDevObj::GetDynamic(DISPID dispid, IUnknown **ppUnknown)
{
	*ppUnknown = NULL;
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		if (m_arLinkInfo[i].dispid == dispid)
		{
			m_arLinkInfo[i].pDynamicObject->QueryInterface(IID_IUnknown, (void **)ppUnknown);
			break;
		}
	}

	return S_OK;
}

HRESULT CDevObj::RemoveDynamic(DISPID dispid)
{
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		if (m_arLinkInfo[i].dispid == dispid)
		{
			m_arLinkInfo[i].pDynamicObject->Release();
			m_arLinkInfo.RemoveAt(i);
			break;
		}
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////
//功能: 设置对象的显示名
///////////////////////////////////////////////////////////////////
BOOL CDevObj::PutDisplayName(LPCSTR strDisplayName, BOOL bValidName)
{
	USES_CONVERSION;

	if (m_pDocument == NULL)
		return FALSE;

	if (bValidName && !m_pDocument->IsValidObjectName(CString(strDisplayName)))
		return FALSE;

	//如已有该名对象存在
	CDevObj* pSelectObj = NULL;
	CNamedDevobjMap* pMap = m_pDocument->GetNamedObjectsMap();
	if (pMap->LookupNoCase(strDisplayName, pSelectObj) && pSelectObj != this)
		return FALSE;

	//新名与旧名同
	if (m_strDisplayName == strDisplayName)
	{
		if (pSelectObj == NULL)
		{
			pMap->SetAt(strDisplayName, this);
			m_pDocument->OnSymbolChanged(UPDATE_SYMBOL_OBJECT);
		}
		return TRUE;
	}
	
	//新名字没有被其它对象使用
	pMap->RemoveKey(m_strDisplayName);
	m_strDisplayName = strDisplayName;
	pMap->SetAt(strDisplayName, this);
	m_pDocument->OnSymbolChanged(UPDATE_SYMBOL_OBJECT);

	return TRUE;
}

HRESULT CDevObj::PutProperty(DISPID dwDispID, VARIANT* pVar)
{
	IDispatchPtr pDisp = GetObjectUnknown();

	DISPPARAMS dispparams = {NULL, NULL, 1, 1};
	dispparams.rgvarg = pVar;
	DISPID dispidPut = DISPID_PROPERTYPUT;
	dispparams.rgdispidNamedArgs = &dispidPut;
	
	if (pVar->vt == VT_UNKNOWN || pVar->vt == VT_DISPATCH || 
		(pVar->vt & VT_ARRAY) || (pVar->vt & VT_BYREF))
	{
		HRESULT hr = pDisp->Invoke(dwDispID, IID_NULL,
			LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUTREF,
			&dispparams, NULL, NULL, NULL);
		if (SUCCEEDED(hr))
			return hr;
	}
	
	return pDisp->Invoke(dwDispID, IID_NULL,
		LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT,
		&dispparams, NULL, NULL, NULL);
}

////////////////////////////////////////////////////////////////////////////////////
//CMapNoCaseStringToPtr

IMPLEMENT_DYNAMIC(CMapNoCaseStringToPtr, CMapStringToPtr)


// Construction
CMapNoCaseStringToPtr::CMapNoCaseStringToPtr(int nBlockSize) : 
	CMapStringToPtr(nBlockSize),
	m_mapNocase(nBlockSize)
{
}
	
// add a new (key, value) pair
void CMapNoCaseStringToPtr::SetAt(LPCTSTR key, void* newValue)
{
	CMapStringToPtr::SetAt(key, newValue);

	CString strKey = key;
	strKey.MakeUpper();
	m_mapNocase.SetAt(strKey, newValue);
}
	
// removing existing (key, ?) pair
BOOL CMapNoCaseStringToPtr::RemoveKey(LPCTSTR key)
{
	int bRet = CMapStringToPtr::RemoveKey(key);
	
	CString strKey = key;
	strKey.MakeUpper();
	VERIFY(m_mapNocase.RemoveKey(strKey) == bRet); 

	return bRet;
}

void CMapNoCaseStringToPtr::RemoveAll()
{
	CMapStringToPtr::RemoveAll();
	m_mapNocase.RemoveAll();
}

BOOL CMapNoCaseStringToPtr::LookupNoCase(LPCTSTR key, void*& rValue) const
{
	CString strKey = key;
	strKey.MakeUpper();
	return m_mapNocase.Lookup(strKey, rValue);
}
