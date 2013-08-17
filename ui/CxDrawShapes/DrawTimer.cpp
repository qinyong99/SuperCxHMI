// DrawTimer.cpp : Implementation of CDrawTimer
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "DrawTimer.h"


////////////////////////////////////////////////////////////////////////////
// CDrawTimer

HRESULT CDrawTimer::DoLoad(DWORD dwVer, LPSTREAM pStm)
{
	pStm->Read(&m_rcPosition, sizeof(m_rcPosition), NULL);

	pStm->Read(&m_bEnabled, sizeof(m_bEnabled), NULL);
	pStm->Read(&m_nInterval, sizeof(m_nInterval), NULL);

	m_bstrDescription.ReadFromStream(pStm);
	
	return CCxDrawObjectBase::DoLoad(dwVer, pStm);
}

HRESULT CDrawTimer::DoSave(DWORD dwVer, LPSTREAM pStm)
{
	pStm->Write(&m_rcPosition, sizeof(m_rcPosition), NULL);

	pStm->Write(&m_bEnabled, sizeof(m_bEnabled), NULL);
	pStm->Write(&m_nInterval, sizeof(m_nInterval), NULL);
	
	m_bstrDescription.WriteToStream(pStm);
	
	return CCxDrawObjectBase::DoSave(dwVer, pStm);
}

//////////////////////////////////////////////////////////////
//功能: 取对象名字串
//////////////////////////////////////////////////////////////
STDMETHODIMP CDrawTimer::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("Timer");
	return S_OK;
}

STDMETHODIMP CDrawTimer::InitNew()
{
	m_bEnabled = VARIANT_TRUE;
	m_nInterval = 0;

	m_rcPosition.left = m_rcPosition.top = 0;
	m_rcPosition.right = m_rcPosition.bottom = 100;

	return ICxDrawObjectImpl<CDrawTimer>::InitNew();
}

HRESULT CDrawTimer::DoDraw(HDC hdcDest)
{
	if (ClientIsRunMode())
		return S_OK;
	
	HBRUSH hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdcDest, hBrush);

	int nLeft = ROUND(m_rcPosition.left);
	int nTop = ROUND(m_rcPosition.top);
	int nRight = ROUND(m_rcPosition.right);
	int nBottom = ROUND(m_rcPosition.bottom);

	Ellipse(hdcDest, nLeft, nTop, nRight, nBottom);
	
	MoveToEx(hdcDest, nLeft + (nRight - nLeft) / 4, nBottom - (nBottom - nTop) / 4, NULL);
	LineTo(hdcDest, (nLeft + nRight) / 2, (nTop + nBottom) / 2);
	LineTo(hdcDest, nRight, (nTop + nBottom) / 2);
	
	::SelectObject(hdcDest, hOldBrush);
	::DeleteObject((HGDIOBJ)hBrush);
	
	return S_OK;
}

STDMETHODIMP CDrawTimer::GetPositionRect(RECTF* pPositionRect)
{
	pPositionRect->left = m_rcPosition.left;
	pPositionRect->top = m_rcPosition.top;
	pPositionRect->right = m_rcPosition.right;
	pPositionRect->bottom = m_rcPosition.bottom;

	return S_OK;
}

STDMETHODIMP CDrawTimer::GetBoundingRect(RECT* pBoundingRect)
{
	pBoundingRect->left = ROUND(m_rcPosition.left);
	pBoundingRect->top = ROUND(m_rcPosition.top);
	pBoundingRect->right = ROUND(m_rcPosition.right);
	pBoundingRect->bottom = ROUND(m_rcPosition.bottom);

	return S_OK;
}

////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawTimer::MoveHandleTo(long nHandle, POINT point)
{
	DefaultMoveHandleTo(&m_rcPosition, -nHandle, point);
	NormalizeRect(&m_rcPosition);

	return S_OK;
}

inline BOOL OffsetRectF(RECTF* pRect, float fOffsetX, float fOffsetY)
{
	pRect->left += fOffsetX;
	pRect->right += fOffsetX;
	pRect->top += fOffsetY;
	pRect->bottom += fOffsetY;

	return TRUE;
}

STDMETHODIMP CDrawTimer::RotateAt(POINTF point, float fAngle)
{
	POINTF ptCenter;
	ptCenter.x = (m_rcPosition.left + m_rcPosition.right) / 2;
	ptCenter.y = (m_rcPosition.top + m_rcPosition.bottom) / 2;
	POINTF ptNew = CalculateRotatePoint(point, ptCenter, fAngle);
	
	OffsetRectF(&m_rcPosition, ptNew.x - ptCenter.x, ptNew.y - ptCenter.y);

	return S_OK;
}

STDMETHODIMP CDrawTimer::ScaleAt(POINTF point, float fHorScale, float fVerScale)
{
	::OffsetRectF(&m_rcPosition, -point.x, -point.y);

	m_rcPosition.left = m_rcPosition.left * fHorScale;
	m_rcPosition.right = m_rcPosition.right * fHorScale;
	m_rcPosition.top = m_rcPosition.top * fVerScale;
	m_rcPosition.bottom  = m_rcPosition.bottom * fVerScale;

	::OffsetRectF(&m_rcPosition, point.x, point.y);

	return S_OK;
}

STDMETHODIMP CDrawTimer::DrawRotateAtShape(HDC hdcDest, POINTF point, long lAngle)
{
	POINTF ptCenter;
	RECTF rcPosition = m_rcPosition;
	ptCenter.x = (rcPosition.left + rcPosition.right) / 2;
	ptCenter.y = (rcPosition.top + rcPosition.bottom) / 2;
	POINTF ptNew = CalculateRotatePoint(point, ptCenter, float(lAngle));
	::OffsetRectF(&rcPosition, ptNew.x - ptCenter.x, ptNew.y - ptCenter.y);
	::Rectangle(hdcDest, ROUND(rcPosition.left), ROUND(rcPosition.top), ROUND(rcPosition.right), ROUND(rcPosition.bottom));

	return S_OK;
}

STDMETHODIMP CDrawTimer::DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
{
	RECTF rect;
	rect.left = (float)m_rcPosition.left;
	rect.top = (float)m_rcPosition.top;
	rect.right = (float)m_rcPosition.right;
	rect.bottom = (float)m_rcPosition.bottom;
	DefaultMoveHandleTo(&rect, -nHandle, point);
	::Rectangle(hdcDest, ROUND(rect.left), ROUND(rect.top), ROUND(rect.right), ROUND(rect.bottom));
	return S_OK;
}

STDMETHODIMP CDrawTimer::DrawTrackerShape(HDC hdcDest, POINT* pPoints, int nCount)
{
	RECT rect;
	rect.left = ROUND(m_rcPosition.left);
	rect.top = ROUND(m_rcPosition.top);
	rect.right = ROUND(m_rcPosition.right);
	rect.bottom = ROUND(m_rcPosition.bottom);
	
	MoveToEx(hdcDest, rect.left, rect.top, NULL);
	LineTo(hdcDest, rect.right, rect.top);
	LineTo(hdcDest, rect.right, rect.bottom);
	LineTo(hdcDest, rect.left, rect.bottom);
	LineTo(hdcDest, rect.left, rect.top);

	return S_OK;
}

STDMETHODIMP CDrawTimer::MoveTo(RECTF rect, BOOL bUniformScale)
{
	NormalizeRect(&rect);
	m_rcPosition.left = rect.left;
	m_rcPosition.top = rect.top;
	m_rcPosition.right = rect.right;
	m_rcPosition.bottom = rect.bottom;

	return S_OK;
}

STDMETHODIMP CDrawTimer::GetHandleCount(long* pHandleCount)
{
	*pHandleCount = 0;
	return S_OK;
}

STDMETHODIMP CDrawTimer::HitTest(POINT point, BOOL* bHitTest)
{
	if (!ClientIsRunMode() && 
		point.x >= m_rcPosition.left && point.x < m_rcPosition.right &&
		point.y >= m_rcPosition.top && point.y < m_rcPosition.bottom)
		*bHitTest = TRUE;
	else
		*bHitTest = FALSE;

	return S_OK;
}

STDMETHODIMP CDrawTimer::Intersects(RECT rect, BOOL* bIntersects)
{
	RECT fixed;
	
	fixed.left = ROUND(m_rcPosition.left);
	fixed.top = ROUND(m_rcPosition.top);
	fixed.right = ROUND(m_rcPosition.right);
	fixed.bottom = ROUND(m_rcPosition.bottom);
	
	fixed.right++;
	fixed.bottom++;
	RECT temp;
	IntersectRect(&temp, &fixed, &rect);
	if (IsRectEmpty(&temp))
		*bIntersects = FALSE;
	else
		*bIntersects = TRUE;

	return S_OK;
}

STDMETHODIMP CDrawTimer::put_Enabled(VARIANT_BOOL newVal)
{
	if (m_bEnabled == newVal)
		return S_OK;
	
	m_bEnabled = newVal;
	ResetTimer();
	
	return S_OK;
}

STDMETHODIMP CDrawTimer::get_Enabled(VARIANT_BOOL *pVal)
{
	*pVal = m_bEnabled;
	
	return S_OK;
}

STDMETHODIMP CDrawTimer::get_Interval(int *pVal)
{
	*pVal = m_nInterval;

	return S_OK;
}

STDMETHODIMP CDrawTimer::put_Interval(int newVal)
{
	if (m_nInterval == newVal)
		return S_OK;
		
	m_nInterval = newVal;
	ResetTimer();

	return S_OK;
}

STDMETHODIMP CDrawTimer::InitRun(BOOL* pbHasDynamic, ICxScriptEngineSite* pScriptSite)
{
	m_bRun = TRUE;
	ResetTimer();

	return ICxDrawObjectImpl<CDrawTimer>::InitRun(pbHasDynamic, pScriptSite);
}

LRESULT CDrawTimer::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ATLASSERT(m_uTimer != -1);
	Fire_Event(1);

	return 0;
}

void CDrawTimer::ResetTimer()
{
	if (!m_bRun)
		return;

	if (m_uTimer != -1)
	{
		KillTimer(m_uTimer);
		m_uTimer = -1;
	}

	if (m_bEnabled != VARIANT_FALSE && m_nInterval != 0)
	{
		if (m_hWnd == NULL)
		{
			RECT rcPos = { 0, 0, 0, 0 };
			if (!Create(NULL, rcPos, WS_OVERLAPPED))
			{
				ATLASSERT(FALSE);
				DWORD dw = GetLastError();
					
				return;
			}
		}

		m_uTimer = SetTimer(1, m_nInterval);
	}
}

void CDrawTimer::FinalRelease()
{
	if (m_uTimer != -1)
	{
		if (KillTimer(m_uTimer))
			m_uTimer = -1;
		ATLASSERT(m_uTimer == -1);
	}

	if (m_hWnd != NULL)
	{
		DestroyWindow();
		m_hWnd = NULL;
	}
}
