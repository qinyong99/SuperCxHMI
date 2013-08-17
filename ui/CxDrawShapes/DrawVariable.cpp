// DrawVariable.cpp : implementation file

#include "stdafx.h"
#include "CxDrawShapes.h"
#include "DrawVariable.h"

HRESULT CDrawVariable::DoLoad(DWORD dwVer, LPSTREAM pStm)
{
	pStm->Read(&m_rcPosition, sizeof(m_rcPosition), NULL);

	m_value.ReadFromStream(pStm);
	pStm->Read(&m_type, sizeof(m_type), NULL);

	m_bstrDescription.ReadFromStream(pStm);
	
	return CCxDrawObjectBase::DoLoad(dwVer, pStm);
}

HRESULT CDrawVariable::DoSave(DWORD dwVer, LPSTREAM pStm)
{
	pStm->Write(&m_rcPosition, sizeof(m_rcPosition), NULL);

	m_value.WriteToStream(pStm);
	pStm->Write(&m_type, sizeof(m_type), NULL);
	
	m_bstrDescription.WriteToStream(pStm);
	
	return CCxDrawObjectBase::DoSave(dwVer, pStm);
}

int CDrawVariable::GetDynamicState(DISPID dispid)
{
	int nState = CCxDrawObjectBase::GetDynamicState(dispid);

	//只有水平位置，垂直位置，旋转角度支持鼠标输入
	if (dispid == CXPROPID_Value)
		nState |= 0x1;
	
	return nState;
}

STDMETHODIMP CDrawVariable::InitRun(BOOL* pbHasDynamic, ICxScriptEngineSite* pScriptSite)
{
	m_vaOrigin = m_value;

	return ICxDrawObjectImpl<CDrawVariable>::InitRun(pbHasDynamic, pScriptSite);
}

STDMETHODIMP CDrawVariable::ParseDynamic()
{
	m_bParsingDynamic = TRUE;
	HRESULT hr = ICxDrawObjectImpl<CDrawVariable>::ParseDynamic();
	m_bParsingDynamic = FALSE;

	return hr;
}

//////////////////////////////////////////////////////////////
//功能: 取对象名字串
//////////////////////////////////////////////////////////////
STDMETHODIMP CDrawVariable::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("Variable");
	return S_OK;
}

STDMETHODIMP CDrawVariable::InitNew()
{
	m_type = PropertyTypeUnknown;

	m_rcPosition.left = m_rcPosition.top = 0;
	m_rcPosition.right = m_rcPosition.bottom = 100;

	return ICxDrawObjectImpl<CDrawVariable>::InitNew();
}

STDMETHODIMP CDrawVariable::get_Value(/*[out, retval]*/ CXVARIABLETYPE *pVal)
{
// 	ATLASSERT(m_value.vt != VT_EMPTY);
	::VariantCopy(pVal, &m_value);

	return S_OK;
}

STDMETHODIMP CDrawVariable::put_Value(/*[in]*/ CXVARIABLETYPE newVal)
{
	CComVariant var = newVal;

	HRESULT hr = S_OK;
	switch (m_type) 
	{
	case PropertyTypeColor:
		hr = var.ChangeType(VT_I4);
		break;
	case PropertyTypeBool:
		hr = var.ChangeType(VT_BOOL);
		break;
	case PropertyTypeString:
		hr = var.ChangeType(VT_BSTR);
		break;
	case PropertyTypeNumeric:
		hr = var.ChangeType(VT_R8);
		break;
	default:
		break;
	}
	if (FAILED(hr))
	{
		// 如果是解析动态的情况下，使用原始值
		if (m_bParsingDynamic)
			var = m_vaOrigin;
//		return hr;
	}
	
	if (m_value != var)
	{
		m_value = var;

		if (!m_bParsingDynamic && ClientIsRunMode())
		{
			ICxDynamicObject* pDyn = GetMouseInputDynamic(CXPROPID_Value);
			if (pDyn != NULL)
			{
				m_bParsingDynamic = TRUE;
				pDyn->InvertParseConnection();
				m_bParsingDynamic = FALSE;
			}
		}

		Fire_Event(1);
		
		var.ChangeType(VT_BOOL);
		if (var.boolVal != VARIANT_FALSE)
			Fire_Event(2);
		else
			Fire_Event(3);
	}
	
	return hr;
}

STDMETHODIMP CDrawVariable::get_Type(/*[out, retval]*/ enumPropertyType *pVal)
{
	*pVal = m_type;
	
	return S_OK;
}

STDMETHODIMP CDrawVariable::put_Type(/*[in]*/ enumPropertyType newVal)
{
	m_type = newVal;
	
	return S_OK;
}

HRESULT CDrawVariable::DoDraw(HDC hdcDest)
{
	if (ClientIsRunMode())
		return S_OK;
	
	HBRUSH hBrush = ::CreateSolidBrush(RGB(255, 255, 0));
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdcDest, hBrush);

	int nLeft = ROUND(m_rcPosition.left);
	int nTop = ROUND(m_rcPosition.top);
	int nRight = ROUND(m_rcPosition.right);
	int nBottom = ROUND(m_rcPosition.bottom);
	int nWidth = nRight - nLeft;
	int nHeight = nBottom - nTop;

	POINT points[6];
	points[0].x = nLeft + nWidth / 5;
	points[0].y = nTop;
	points[1].x = nRight;
	points[1].y = nTop;
	points[2].x = nRight;
	points[2].y = nBottom;
	points[3].x = nLeft + nWidth / 5;
	points[3].y = nBottom;
	points[4].x = nLeft;
	points[4].y = nBottom - 2 * nHeight / 5;
	points[5].x = nLeft;
	points[5].y = nTop + 2 * nHeight / 5;

	Polygon(hdcDest, points, 6);

	::SelectObject(hdcDest, hOldBrush);
	::DeleteObject((HGDIOBJ)hBrush);
	
	int nXCenter = nLeft + nWidth * 3 / 20; 
	int nYCenter = (nTop + nBottom) / 2; 
	int nRadius = min(nWidth, nHeight) / 10;
	Ellipse(hdcDest, nXCenter - nRadius, nYCenter - nRadius,
		nXCenter + nRadius, nYCenter + nRadius);

	return S_OK;
}

STDMETHODIMP CDrawVariable::GetPositionRect(RECTF* pPositionRect)
{
	pPositionRect->left = m_rcPosition.left;
	pPositionRect->top = m_rcPosition.top;
	pPositionRect->right = m_rcPosition.right;
	pPositionRect->bottom = m_rcPosition.bottom;

	return S_OK;
}

STDMETHODIMP CDrawVariable::GetBoundingRect(RECT* pBoundingRect)
{
	pBoundingRect->left = ROUND(m_rcPosition.left);
	pBoundingRect->top = ROUND(m_rcPosition.top);
	pBoundingRect->right = ROUND(m_rcPosition.right);
	pBoundingRect->bottom = ROUND(m_rcPosition.bottom);

	return S_OK;
}

////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawVariable::MoveHandleTo(long nHandle, POINT point)
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

STDMETHODIMP CDrawVariable::RotateAt(POINTF point, float fAngle)
{
	POINTF ptCenter;
	ptCenter.x = (m_rcPosition.left + m_rcPosition.right) / 2;
	ptCenter.y = (m_rcPosition.top + m_rcPosition.bottom) / 2;
	POINTF ptNew = CalculateRotatePoint(point, ptCenter, fAngle);
	
	OffsetRectF(&m_rcPosition, ptNew.x - ptCenter.x, ptNew.y - ptCenter.y);

	return S_OK;
}

STDMETHODIMP CDrawVariable::ScaleAt(POINTF point, float fHorScale, float fVerScale)
{
	::OffsetRectF(&m_rcPosition, -point.x, -point.y);

	m_rcPosition.left = m_rcPosition.left * fHorScale;
	m_rcPosition.right = m_rcPosition.right * fHorScale;
	m_rcPosition.top = m_rcPosition.top * fVerScale;
	m_rcPosition.bottom  = m_rcPosition.bottom * fVerScale;

	::OffsetRectF(&m_rcPosition, point.x, point.y);

	return S_OK;
}

STDMETHODIMP CDrawVariable::DrawRotateAtShape(HDC hdcDest, POINTF point, long lAngle)
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

STDMETHODIMP CDrawVariable::DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
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

STDMETHODIMP CDrawVariable::DrawTrackerShape(HDC hdcDest, POINT* pPoints, int nCount)
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

STDMETHODIMP CDrawVariable::MoveTo(RECTF rect, BOOL bUniformScale)
{
	NormalizeRect(&rect);
	m_rcPosition.left = rect.left;
	m_rcPosition.top = rect.top;
	m_rcPosition.right = rect.right;
	m_rcPosition.bottom = rect.bottom;

	return S_OK;
}

STDMETHODIMP CDrawVariable::GetHandleCount(long* pHandleCount)
{
	*pHandleCount = 0;
	return S_OK;
}

STDMETHODIMP CDrawVariable::HitTest(POINT point, BOOL* bHitTest)
{
	if (!ClientIsRunMode() && 
		point.x >= m_rcPosition.left && point.x < m_rcPosition.right &&
		point.y >= m_rcPosition.top && point.y < m_rcPosition.bottom)
		*bHitTest = TRUE;
	else
		*bHitTest = FALSE;

	return S_OK;
}

STDMETHODIMP CDrawVariable::Intersects(RECT rect, BOOL* bIntersects)
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
