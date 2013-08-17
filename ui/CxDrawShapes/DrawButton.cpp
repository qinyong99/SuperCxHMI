// DrawButton.cpp : Implementation of CDrawButton
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "DrawButton.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawButton

STDMETHODIMP CDrawButton::get_HorizontalPosition(float *pVal)
{
	*pVal = m_rcPosition.left;
	return S_OK;
}

STDMETHODIMP CDrawButton::put_HorizontalPosition(float newVal)
{
	if (m_rcPosition.left == newVal)
		return S_OK;

	FireViewChange();
	m_rcPosition.right = newVal + m_rcPosition.right - m_rcPosition.left;
	m_rcPosition.left = newVal;
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawButton::get_VerticalPosition(float *pVal)
{
	*pVal = m_rcPosition.top;
	return S_OK;
}

STDMETHODIMP CDrawButton::put_VerticalPosition(float newVal)
{
	if (m_rcPosition.top == newVal)
		return S_OK;

	FireViewChange();
	m_rcPosition.bottom = newVal + m_rcPosition.bottom - m_rcPosition.top;
	m_rcPosition.top = newVal;
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawButton::get_Width(float *pVal)
{
	*pVal = m_rcPosition.right - m_rcPosition.left;
	return S_OK;
}

STDMETHODIMP CDrawButton::put_Width(float newVal)
{
	if (newVal < 0)
		return S_FALSE;

	if (m_rcPosition.right == m_rcPosition.left + newVal)
		return S_OK;

	FireViewChange();
	
	m_rcPosition.right = m_rcPosition.left + newVal;
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawButton::get_Height(float *pVal)
{
	*pVal = m_rcPosition.bottom - m_rcPosition.top;
	return S_OK;
}

STDMETHODIMP CDrawButton::put_Height(float newVal)
{
	if (newVal < 0)
		return S_FALSE;
	
	if (m_rcPosition.bottom == m_rcPosition.top + newVal)
		return S_OK;

	FireViewChange();

	m_rcPosition.bottom = m_rcPosition.top + newVal;
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawButton::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("Button");
	return S_OK;
}

STDMETHODIMP CDrawButton::InitNew()
{
	m_rcPosition.left = m_rcPosition.top = 0;
	m_rcPosition.right = m_rcPosition.bottom = 100;

	GetDefaultFontDisp(&m_pFont);
	m_clrBackground = 0x80000000 | COLOR_BTNFACE;
	m_clrForeground = 0x80000000 | COLOR_BTNTEXT;

	m_bEnabled = VARIANT_TRUE;

//	GetDefaultBackgroundColor(m_clrBackground);
//	GetDefaultForegroundColor(m_clrForeground);

	return ICxDrawObjectImpl<CDrawButton>::InitNew();
}

HRESULT CDrawButton::DoLoad(DWORD dwVer, LPSTREAM pStm)
{
	pStm->Read(&m_rcPosition, sizeof(m_rcPosition), NULL);
	m_bstrCaption.ReadFromStream(pStm);
	pStm->Read(&m_clrBackground, sizeof(m_clrBackground), NULL);
	pStm->Read(&m_clrForeground, sizeof(m_clrForeground), NULL);

	pStm->Read(&m_enumMousePointer, sizeof(m_enumMousePointer), NULL);
	m_bstrDescription.ReadFromStream(pStm);

	pStm->Read(&m_bEnabled, sizeof(VARIANT_BOOL), NULL);
	
	BOOL b;
	pStm->Read(&b, sizeof(b), NULL);
	if (b)
	{
		CComPtr<IUnknown> spUnk;
		OleLoadFromStream(pStm, IID_IUnknown, (void **)&spUnk);
		if (spUnk != NULL)
			spUnk->QueryInterface(IID_IFontDisp, (void **)&m_pFont);
	}

	pStm->Read(&b, sizeof(b), NULL);
	if (b)
	{
		CComPtr<IUnknown> spUnk;
		OleLoadFromStream(pStm, IID_IUnknown, (void **)&spUnk);
		if (spUnk != NULL)
			spUnk->QueryInterface(IID_IPictureDisp, (void **)&m_pPictureNormal);
	}

	pStm->Read(&b, sizeof(b), NULL);
	if (b)
	{
		CComPtr<IUnknown> spUnk;
		OleLoadFromStream(pStm, IID_IUnknown, (void **)&spUnk);
		if (spUnk != NULL)
			spUnk->QueryInterface(IID_IPictureDisp, (void **)&m_pPictureHover);
	}

	pStm->Read(&b, sizeof(b), NULL);
	if (b)
	{
		CComPtr<IUnknown> spUnk;
		OleLoadFromStream(pStm, IID_IUnknown, (void **)&spUnk);
		if (spUnk != NULL)
			spUnk->QueryInterface(IID_IPictureDisp, (void **)&m_pPicturePushed);
	}

	pStm->Read(&b, sizeof(b), NULL);
	if (b)
	{
		CComPtr<IUnknown> spUnk;
		OleLoadFromStream(pStm, IID_IUnknown, (void **)&spUnk);
		if (spUnk != NULL)
			spUnk->QueryInterface(IID_IPictureDisp, (void **)&m_pPictureDisabled);
	}

	return CCxDrawObjectBase::DoLoad(dwVer, pStm);
}

HRESULT CDrawButton::DoSave(DWORD dwVer, LPSTREAM pStm)
{
	pStm->Write(&m_rcPosition, sizeof(m_rcPosition), NULL);
	m_bstrCaption.WriteToStream(pStm);
	pStm->Write(&m_clrBackground, sizeof(m_clrBackground), NULL);
	pStm->Write(&m_clrForeground, sizeof(m_clrForeground), NULL);
	
	pStm->Write(&m_enumMousePointer, sizeof(m_enumMousePointer), NULL);
	m_bstrDescription.WriteToStream(pStm);

	pStm->Write(&m_bEnabled, sizeof(VARIANT_BOOL), NULL);

	BOOL b = (m_pFont != NULL);
	pStm->Write(&b, sizeof(b), NULL);
	if (b)
	{
		CComPtr<IPersistStream> spStream;
		m_pFont->QueryInterface(IID_IPersistStream, (void**)&spStream);
		if (spStream != NULL)
			OleSaveToStream(spStream, pStm);
		else
			WriteClassStm(pStm, CLSID_NULL);
	}

	b = (m_pPictureNormal != NULL);
	pStm->Write(&b, sizeof(b), NULL);
	if (b)
	{
		CComPtr<IPersistStream> spStream;
		m_pPictureNormal->QueryInterface(IID_IPersistStream, (void**)&spStream);
		if (spStream != NULL)
			OleSaveToStream(spStream, pStm);
		else
			WriteClassStm(pStm, CLSID_NULL);
	}

	b = (m_pPictureHover != NULL);
	pStm->Write(&b, sizeof(b), NULL);
	if (b)
	{
		CComPtr<IPersistStream> spStream;
		m_pPictureHover->QueryInterface(IID_IPersistStream, (void**)&spStream);
		if (spStream != NULL)
			OleSaveToStream(spStream, pStm);
		else
			WriteClassStm(pStm, CLSID_NULL);
	}

	b = (m_pPicturePushed != NULL);
	pStm->Write(&b, sizeof(b), NULL);
	if (b)
	{
		CComPtr<IPersistStream> spStream;
		m_pPicturePushed->QueryInterface(IID_IPersistStream, (void**)&spStream);
		if (spStream != NULL)
			OleSaveToStream(spStream, pStm);
		else
			WriteClassStm(pStm, CLSID_NULL);
	}

	b = (m_pPictureDisabled != NULL);
	pStm->Write(&b, sizeof(b), NULL);
	if (b)
	{
		CComPtr<IPersistStream> spStream;
		m_pPictureDisabled->QueryInterface(IID_IPersistStream, (void**)&spStream);
		if (spStream != NULL)
			OleSaveToStream(spStream, pStm);
		else
			WriteClassStm(pStm, CLSID_NULL);
	}
	
	return CCxDrawObjectBase::DoSave(dwVer, pStm);
}

STDMETHODIMP CDrawButton::SaveOldVer(LPSTREAM pStm, BOOL fClearDirty)
{
	pStm->Write(&m_rcPosition, sizeof(m_rcPosition), NULL);
	HRESULT hr = ICxDrawObjectImpl<CDrawButton>::SaveOldVer(pStm,  fClearDirty);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

STDMETHODIMP CDrawButton::LoadOldVer(LPSTREAM pStm)
{
	pStm->Read(&m_rcPosition, sizeof(m_rcPosition), NULL);
	HRESULT hr = ICxDrawObjectImpl<CDrawButton>::LoadOldVer(pStm);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT CDrawButton::DoDraw(HDC hdcDest)
{
	USES_CONVERSION;

	RECT rect;
	rect.left = ROUND(m_rcPosition.left);
	rect.top = ROUND(m_rcPosition.top);
	rect.right = ROUND(m_rcPosition.right);
	rect.bottom = ROUND(m_rcPosition.bottom);

	COLORREF colBack,colFore;
	OleTranslateColor(m_clrForeground, 0, &colFore);
	OleTranslateColor(m_clrBackground, 0, &colBack);

	RECT rcPicture = rect;
	::InflateRect(&rcPicture, -1, -1);
	// 有图片绘制图片
	if (m_bEnabled == VARIANT_FALSE)
	{
		if (DrawPicture(hdcDest, rcPicture, m_pPictureDisabled))
			return S_OK;
	}
	if (m_enumButtonState == ButtonStateUp)
	{
		if (m_bMouseOn)
		{
			if (DrawPicture(hdcDest, rcPicture, m_pPictureHover))
				return S_OK;
		}
		else
		{
	//		OLE_COLOR clrBack;
	//		if (m_bEnabled == VARIANT_FALSE)
	//			GetAmbientBackColor(&clrBack);
			if (DrawPicture(hdcDest, rcPicture, m_pPictureNormal, colBack, m_bEnabled == VARIANT_FALSE))
				return S_OK;
		}
	}
	else
	{
		if (DrawPicture(hdcDest, rcPicture, m_pPicturePushed))
			return S_OK;
	}

	BOOL bHasDrawPicture = FALSE;

	// 画边框
	if (m_enumButtonState == ButtonStateDown)
	{
		::OffsetRect(&rcPicture, 1, 1);
		if (DrawPicture(hdcDest, rcPicture, m_pPictureNormal))
			bHasDrawPicture = TRUE;

		rect.left++;
		rect.top++;
		::DrawEdge(hdcDest, &rect, EDGE_SUNKEN, BF_RECT | BF_ADJUST);
	}
	else
	{
		::OffsetRect(&rcPicture, -1, -1);
		if (DrawPicture(hdcDest, rcPicture, m_pPictureNormal))
			bHasDrawPicture = TRUE;

		rect.right--;
		rect.bottom--;
		::DrawEdge(hdcDest, &rect, EDGE_RAISED, BF_RECT | BF_ADJUST);
	}
	
	if (bHasDrawPicture)
		return S_OK;

	CBrush brushBack;
	brushBack.CreateSolidBrush(colBack);

	COLORREF colOld = SetTextColor(hdcDest, colFore);
	int iOldMode = SetBkMode(hdcDest, TRANSPARENT);
	LPTSTR szCaption = W2T(m_bstrCaption);
	int cbCaption = lstrlen(szCaption);
	HFONT hOldFont = NULL;
	if (m_pFont)
	{
		CComPtr<IFont> pFont;
		m_pFont->QueryInterface(IID_IFont, (void**)&pFont);
		HFONT hfont;
		pFont->get_hFont(&hfont);
		hOldFont = (HFONT) SelectObject(hdcDest, hfont);
	}

	::FillRect(hdcDest, &rect, brushBack);

//	if (m_enumButtonState == ButtonStateDown)
//		::OffsetRect(&rect, 1, 1);

	SIZE szExtent;
	POINT pt;
	::GetTextExtentPoint32(hdcDest, szCaption, cbCaption, &szExtent);
	if (szExtent.cx > rect.right - rect.left)
		szExtent.cx = rect.right - rect.left;
	if (szExtent.cy > rect.bottom - rect.top)
		szExtent.cy = rect.bottom - rect.top;
	pt.x = (rect.left + rect.right - szExtent.cx) / 2;
	pt.y = (rect.top + rect.bottom - szExtent.cy) / 2;

	if (m_bEnabled != VARIANT_FALSE)
	{
		RECT rcTemp;
		rcTemp.left = pt.x;
		rcTemp.top = pt.y;
		rcTemp.right = rcTemp.left + szExtent.cx;
		rcTemp.bottom = rcTemp.top + szExtent.cy;
		::DrawText(hdcDest, szCaption, cbCaption, &rcTemp, DT_SINGLELINE | DT_VCENTER);
//		::TextOut(hdcDest, pt.x, pt.y, szCaption, cbCaption);
	}
	else
	{
		::DrawState(hdcDest, (HBRUSH)NULL, 0, (LPARAM)szCaption, (WPARAM)cbCaption, 
				pt.x, pt.y, szExtent.cx, szExtent.cy, DST_TEXT | DSS_DISABLED);
	}

//	::DrawText(hdcDest, szCaption, cbCaption, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	iOldMode = SetBkMode(hdcDest, iOldMode);
	if (hOldFont)
		SelectObject(hdcDest, hOldFont);

	return S_OK;
}

STDMETHODIMP CDrawButton::OnMouseLeave()
{
	m_bMouseOn = FALSE;
	FireViewChange();
	return ICxDrawObjectImpl<CDrawButton>::OnMouseLeave();
}

STDMETHODIMP CDrawButton::OnWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL bSubObj, LRESULT *plResult)
{
	if (m_bEnabled != VARIANT_TRUE)
	{
		BOOL bMouseInput = ProcessMouseInput(msg, wParam, lParam, GetUnknown());
		//如果是子对象，并且没有处理鼠标输入，返回S_FALSE;
		return bSubObj && !bMouseInput ? S_FALSE : S_OK;
	}
	
	switch (msg)
	{
	case WM_MOUSEMOVE:
		{
			POINT point;
			point.x = GET_X_LPARAM(lParam);
			point.y = GET_Y_LPARAM(lParam);
			BOOL bHit;
			HitTest(point, &bHit);
			if (bHit)
			{
				m_bMouseOn = TRUE;
				m_spDrawObjectSite->TrackMouseLeave();
				FireViewChange();
			}
			else
			{
				m_bMouseOn = FALSE;
				FireViewChange();
			}
			if (m_enumButtonState == ButtonStateDown)
			{
				if (!bHit)
				{
					m_enumButtonState = ButtonStateUp;
					FireViewChange();
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		m_enumButtonState = ButtonStateDown;
		FireViewChange();
		break;
	case WM_LBUTTONUP:
	case WM_KILLFOCUS:
		m_enumButtonState = ButtonStateUp;
		FireViewChange();
		break;
	}
	return ICxDrawObjectImpl<CDrawButton>::OnWindowMessage(msg, wParam, lParam, bSubObj, plResult);
}

STDMETHODIMP CDrawButton::GetPositionRect(RECTF* pPositionRect)
{
	pPositionRect->left = m_rcPosition.left;
	pPositionRect->top = m_rcPosition.top;
	pPositionRect->right = m_rcPosition.right;
	pPositionRect->bottom = m_rcPosition.bottom;

	return S_OK;
}

STDMETHODIMP CDrawButton::GetBoundingRect(RECT* pBoundingRect)
{
	pBoundingRect->left = ROUND(m_rcPosition.left);
	pBoundingRect->top = ROUND(m_rcPosition.top);
	pBoundingRect->right = ROUND(m_rcPosition.right);
	pBoundingRect->bottom = ROUND(m_rcPosition.bottom);

	return S_OK;
}

////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawButton::MoveHandleTo(long nHandle, POINT point)
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

STDMETHODIMP CDrawButton::RotateAt(POINTF point, float fAngle)
{
	POINTF ptCenter;
	ptCenter.x = (m_rcPosition.left + m_rcPosition.right) / 2;
	ptCenter.y = (m_rcPosition.top + m_rcPosition.bottom) / 2;
	POINTF ptNew = CalculateRotatePoint(point, ptCenter, fAngle);
	
	OffsetRectF(&m_rcPosition, ptNew.x - ptCenter.x, ptNew.y - ptCenter.y);

	return S_OK;
}

STDMETHODIMP CDrawButton::ScaleAt(POINTF point, float fHorScale, float fVerScale)
{
	::OffsetRectF(&m_rcPosition, -point.x, -point.y);

	m_rcPosition.left = m_rcPosition.left * fHorScale;
	m_rcPosition.right = m_rcPosition.right * fHorScale;
	m_rcPosition.top = m_rcPosition.top * fVerScale;
	m_rcPosition.bottom  = m_rcPosition.bottom * fVerScale;

	::OffsetRectF(&m_rcPosition, point.x, point.y);

	return S_OK;
}

STDMETHODIMP CDrawButton::DrawRotateAtShape(HDC hdcDest, POINTF point, long lAngle)
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

STDMETHODIMP CDrawButton::DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
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

STDMETHODIMP CDrawButton::DrawTrackerShape(HDC hdcDest, POINT* pPoints, int nCount)
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

STDMETHODIMP CDrawButton::MoveTo(RECTF rect, BOOL bUniformScale)
{
	NormalizeRect(&rect);
	m_rcPosition.left = rect.left;
	m_rcPosition.top = rect.top;
	m_rcPosition.right = rect.right;
	m_rcPosition.bottom = rect.bottom;

	return S_OK;
}

STDMETHODIMP CDrawButton::GetHandleCount(long* pHandleCount)
{
	*pHandleCount = 0;
	return S_OK;
}

STDMETHODIMP CDrawButton::HitTest(POINT point, BOOL* bHitTest)
{
	if (point.x >= m_rcPosition.left && point.x < m_rcPosition.right &&
		point.y >= m_rcPosition.top && point.y < m_rcPosition.bottom)
		*bHitTest = TRUE;
	else
		*bHitTest = FALSE;

	return S_OK;
}

STDMETHODIMP CDrawButton::Intersects(RECT rect, BOOL* bIntersects)
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

STDMETHODIMP CDrawButton::get_Caption(BSTR *pVal)
{
	*pVal = m_bstrCaption.Copy();

	return S_OK;
}

STDMETHODIMP CDrawButton::put_Caption(BSTR newVal)
{
	if (m_bstrCaption == newVal)
		return S_OK;

	m_bstrCaption = newVal;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawButton::put_BackColor(OLE_COLOR clrBackground)
{
	if (m_clrBackground == clrBackground)
		return S_OK;

	m_clrBackground = clrBackground;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawButton::get_BackColor(OLE_COLOR* pclrBackground)
{
	*pclrBackground = m_clrBackground;

	return S_OK;
}

STDMETHODIMP CDrawButton::put_ForeColor(OLE_COLOR clrForeground)
{
	if (m_clrForeground == clrForeground)
		return S_OK;
	
	m_clrForeground = clrForeground;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawButton::get_ForeColor(OLE_COLOR* pclrForeground)
{
	*pclrForeground = m_clrForeground;

	return S_OK;
}

STDMETHODIMP CDrawButton::put_Font(IFontDisp* pFont)
{
	if (pFont)
	{
		CComQIPtr<IFont, &IID_IFont> p(pFont);
		if (p)
		{
			// 如果想等即返回
			CComQIPtr<IFont, &IID_IFont> pOld(m_pFont);
			if (p->IsEqual(pOld) == S_OK)
				return S_OK;

			m_pFont = 0;

			CComPtr<IFont> pFont;
			p->Clone(&pFont);
			if (pFont)
				pFont->QueryInterface(IID_IFontDisp, (void**) &m_pFont);
		}
	}
	else
	{
		if (m_pFont == NULL)
			return S_OK;

		m_pFont = 0;
	}
	
	FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CDrawButton::putref_Font(IFontDisp* pFont)
{
	m_pFont = pFont;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawButton::get_Font(IFontDisp** ppFont)
{
	*ppFont = m_pFont;
	if (*ppFont != NULL)
		(*ppFont)->AddRef();
	
	return S_OK;
}

STDMETHODIMP CDrawButton::put_Enabled(VARIANT_BOOL newVal)
{
	BOOL bNewVal = (newVal != VARIANT_FALSE ? VARIANT_TRUE : VARIANT_FALSE);
	if (m_bEnabled == bNewVal)
		return S_OK;
	
	m_bEnabled = bNewVal;

	// 复位状态
	m_enumButtonState = ButtonStateUp;
	m_bMouseOn = FALSE;

	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawButton::get_Enabled(VARIANT_BOOL *pVal)
{
	*pVal = m_bEnabled;

	return S_OK;
}

STDMETHODIMP CDrawButton::put_PictureNormal(IPictureDisp* pPicture)
{
	m_pPictureNormal = 0;
	if (pPicture)
	{
		CComQIPtr<IPersistStream, &IID_IPersistStream> p(pPicture);
		if (p)
		{
			ULARGE_INTEGER l;
			p->GetSizeMax(&l);
			HGLOBAL hGlob = GlobalAlloc(GHND, l.LowPart);
			if (hGlob)
			{
				CComPtr<IStream> spStream;
				CreateStreamOnHGlobal(hGlob, TRUE, &spStream);
				if (spStream)
				{
					if (SUCCEEDED(p->Save(spStream, FALSE)))
					{
						LARGE_INTEGER l;
						l.QuadPart = 0;
						spStream->Seek(l, STREAM_SEEK_SET, NULL);
						OleLoadPicture(spStream, l.LowPart, FALSE, IID_IPictureDisp, (void**)&m_pPictureNormal);
					}
					spStream.Release();
				}
				GlobalFree(hGlob);
			}
		}
	}
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawButton::putref_PictureNormal(IPictureDisp* pPicture)
{
	m_pPictureNormal = pPicture;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawButton::get_PictureNormal(IPictureDisp** ppPicture)
{
	*ppPicture = m_pPictureNormal;
	if (*ppPicture != NULL)
		(*ppPicture)->AddRef();
	return S_OK;
}

STDMETHODIMP CDrawButton::put_PictureHover(IPictureDisp* pPicture)
{
	m_pPictureHover = 0;
	if (pPicture)
	{
		CComQIPtr<IPersistStream, &IID_IPersistStream> p(pPicture);
		if (p)
		{
			ULARGE_INTEGER l;
			p->GetSizeMax(&l);
			HGLOBAL hGlob = GlobalAlloc(GHND, l.LowPart);
			if (hGlob)
			{
				CComPtr<IStream> spStream;
				CreateStreamOnHGlobal(hGlob, TRUE, &spStream);
				if (spStream)
				{
					if (SUCCEEDED(p->Save(spStream, FALSE)))
					{
						LARGE_INTEGER l;
						l.QuadPart = 0;
						spStream->Seek(l, STREAM_SEEK_SET, NULL);
						OleLoadPicture(spStream, l.LowPart, FALSE, IID_IPictureDisp, (void**)&m_pPictureHover);
					}
					spStream.Release();
				}
				GlobalFree(hGlob);
			}
		}
	}
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawButton::putref_PictureHover(IPictureDisp* pPicture)
{
	m_pPictureHover = pPicture;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawButton::get_PictureHover(IPictureDisp** ppPicture)
{
	*ppPicture = m_pPictureHover;
	if (*ppPicture != NULL)
		(*ppPicture)->AddRef();
	return S_OK;
}

STDMETHODIMP CDrawButton::put_PicturePushed(IPictureDisp* pPicture)
{
	m_pPicturePushed = 0;
	if (pPicture)
	{
		CComQIPtr<IPersistStream, &IID_IPersistStream> p(pPicture);
		if (p)
		{
			ULARGE_INTEGER l;
			p->GetSizeMax(&l);
			HGLOBAL hGlob = GlobalAlloc(GHND, l.LowPart);
			if (hGlob)
			{
				CComPtr<IStream> spStream;
				CreateStreamOnHGlobal(hGlob, TRUE, &spStream);
				if (spStream)
				{
					if (SUCCEEDED(p->Save(spStream, FALSE)))
					{
						LARGE_INTEGER l;
						l.QuadPart = 0;
						spStream->Seek(l, STREAM_SEEK_SET, NULL);
						OleLoadPicture(spStream, l.LowPart, FALSE, IID_IPictureDisp, (void**)&m_pPicturePushed);
					}
					spStream.Release();
				}
				GlobalFree(hGlob);
			}
		}
	}
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawButton::putref_PicturePushed(IPictureDisp* pPicture)
{
	m_pPicturePushed = pPicture;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawButton::get_PicturePushed(IPictureDisp** ppPicture)
{
	*ppPicture = m_pPicturePushed;
	if (*ppPicture != NULL)
		(*ppPicture)->AddRef();
	return S_OK;
}

STDMETHODIMP CDrawButton::put_PictureDisabled(IPictureDisp* pPicture)
{
	m_pPictureDisabled = 0;
	if (pPicture)
	{
		CComQIPtr<IPersistStream, &IID_IPersistStream> p(pPicture);
		if (p)
		{
			ULARGE_INTEGER l;
			p->GetSizeMax(&l);
			HGLOBAL hGlob = GlobalAlloc(GHND, l.LowPart);
			if (hGlob)
			{
				CComPtr<IStream> spStream;
				CreateStreamOnHGlobal(hGlob, TRUE, &spStream);
				if (spStream)
				{
					if (SUCCEEDED(p->Save(spStream, FALSE)))
					{
						LARGE_INTEGER l;
						l.QuadPart = 0;
						spStream->Seek(l, STREAM_SEEK_SET, NULL);
						OleLoadPicture(spStream, l.LowPart, FALSE, IID_IPictureDisp, (void**)&m_pPictureDisabled);
					}
					spStream.Release();
				}
				GlobalFree(hGlob);
			}
		}
	}
	FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CDrawButton::putref_PictureDisabled(IPictureDisp* pPicture)
{
	m_pPictureDisabled = pPicture;
	FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CDrawButton::get_PictureDisabled(IPictureDisp** ppPicture)
{
	*ppPicture = m_pPictureDisabled;
	if (*ppPicture != NULL)
		(*ppPicture)->AddRef();
	return S_OK;
}