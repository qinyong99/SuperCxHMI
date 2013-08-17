// DrawMenu.cpp : Implementation of CDrawMenu
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "DrawMenu.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawMenu

HRESULT CDrawMenu::DoLoad(DWORD dwVer, LPSTREAM pStm)
{
	if (dwVer > _SUPERCX_VER_0200)
	{
		pStm->Read(&m_bButtonDisplayed, sizeof(m_bButtonDisplayed), NULL);
		m_bstrButtonCaption.ReadFromStream(pStm);
	}
	else
	{
		m_bButtonDisplayed = FALSE;
	}

	int nCount;
	pStm->Read(&nCount, sizeof(nCount), NULL);
	for (int i = 0; i < nCount; i++)
	{
		CMenuItem item;
		item.m_bstrText.ReadFromStream(pStm);
		pStm->Read(&item.m_dwState, sizeof(DWORD), NULL);
		if (dwVer > _SUPERCX_VER_0203)
		{
			item.m_bstrID.ReadFromStream(pStm);
			pStm->Read(&item.m_dwShiftState, sizeof(DWORD), NULL);
			pStm->Read(&item.m_dwKeyCode, sizeof(DWORD), NULL);
		}
		m_arrItems.Add(item);
	}

	pStm->Read(&m_rcPosition, sizeof(m_rcPosition), NULL);

	m_bstrDescription.ReadFromStream(pStm);
	
	return CCxDrawObjectBase::DoLoad(dwVer, pStm);
}

HRESULT CDrawMenu::DoSave(DWORD dwVer, LPSTREAM pStm)
{
	pStm->Write(&m_bButtonDisplayed, sizeof(m_bButtonDisplayed), NULL);
	m_bstrButtonCaption.WriteToStream(pStm);

	int nCount = m_arrItems.GetSize();
	pStm->Write(&nCount, sizeof(nCount), NULL);
	for (int i = 0; i < nCount; i++)
	{
		CMenuItem& item = m_arrItems[i];
		item.m_bstrText.WriteToStream(pStm);
		pStm->Write(&item.m_dwState, sizeof(DWORD), NULL);
		if (dwVer > _SUPERCX_VER_0203)
		{
			item.m_bstrID.WriteToStream(pStm);
			pStm->Write(&item.m_dwShiftState, sizeof(DWORD), NULL);
			pStm->Write(&item.m_dwKeyCode, sizeof(DWORD), NULL);
		}
	}
	
	pStm->Write(&m_rcPosition, sizeof(m_rcPosition), NULL);

	m_bstrDescription.WriteToStream(pStm);
	
	return CCxDrawObjectBase::DoSave(dwVer, pStm);
}

//////////////////////////////////////////////////////////////
//功能: 取对象名字串
//////////////////////////////////////////////////////////////
STDMETHODIMP CDrawMenu::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("Menu");
	return S_OK;
}

STDMETHODIMP CDrawMenu::InitNew()
{
	m_rcPosition.left = m_rcPosition.top = 0;
	m_rcPosition.right = m_rcPosition.bottom = 100;

	m_bButtonDisplayed = TRUE;
	m_bstrButtonCaption = L"菜单";

	return ICxDrawObjectImpl<CDrawMenu>::InitNew();
}

HRESULT CDrawMenu::DoDraw(HDC hdcDest)
{
	if (m_bButtonDisplayed)
	{
		USES_CONVERSION;

		RECT rect;
		rect.left = ROUND(m_rcPosition.left);
		rect.top = ROUND(m_rcPosition.top);
		rect.right = ROUND(m_rcPosition.right);
		rect.bottom = ROUND(m_rcPosition.bottom);

		// 画边框
		if (m_enumButtonState == ButtonStateDown)
		{
			::DrawEdge(hdcDest, &rect, BDR_SUNKENOUTER, BF_RECT | BF_ADJUST);
		}
		else
		{
			if (m_bMouseOn || !ClientIsRunMode())
				::DrawEdge(hdcDest, &rect, BDR_RAISEDINNER, BF_RECT | BF_ADJUST);
		}

		COLORREF clrMenu = GetSysColor(COLOR_MENU);
		COLORREF clrMenuText = GetSysColor(COLOR_MENUTEXT);
			
		COLORREF clrOld = SetTextColor(hdcDest, clrMenuText);
		int iOldMode = SetBkMode(hdcDest, TRANSPARENT);
		
		if (m_fontMenu.m_hFont == NULL)
			GetSystemSettings();
		CFontHandle fontOld = (HFONT)SelectObject(hdcDest, m_fontMenu);

		LPTSTR szCaption = W2T(m_bstrButtonCaption);
		int cbCaption = lstrlen(szCaption);
			
		CBrush brushBack;
		brushBack.CreateSolidBrush(clrMenu);
		::FillRect(hdcDest, &rect, brushBack);

		::DrawText(hdcDest, szCaption, cbCaption, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		SetTextColor(hdcDest, clrOld);
		SetBkMode(hdcDest, iOldMode);
		SelectObject(hdcDest, fontOld);
	}
	else
	{
		if (ClientIsRunMode())
			return S_OK;

		int nLeft = ROUND(m_rcPosition.left);
		int nTop = ROUND(m_rcPosition.top);
		int nRight = ROUND(m_rcPosition.right);
		int nBottom = ROUND(m_rcPosition.bottom);
		
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdcDest, (HBRUSH)GetStockObject(BLACK_BRUSH));
		Rectangle(hdcDest, nLeft, nTop, nLeft + (nRight - nLeft) / 4, nBottom);
		
		RECT rect;
		rect.left = nLeft + (nRight - nLeft) / 4;
		rect.top = nTop;
		rect.right = nRight;
		rect.bottom = nBottom;
		
		::SelectObject(hdcDest, (HBRUSH)GetStockObject(WHITE_BRUSH));
		Rectangle(hdcDest, rect.left, nTop, nRight, nBottom);
		
		int iOldBkMode = SetBkMode(hdcDest, TRANSPARENT);
		HFONT hOldFont = (HFONT)::SelectObject(hdcDest, (HFONT)GetStockObject(DEFAULT_GUI_FONT));
		
		DrawText(hdcDest, _T("菜单"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
		SetBkMode(hdcDest, iOldBkMode);
		::SelectObject(hdcDest, hOldBrush);
		::SelectObject(hdcDest, hOldFont);
	}
	
	
	return S_OK;
}

STDMETHODIMP CDrawMenu::GetPositionRect(RECTF* pPositionRect)
{
	pPositionRect->left = m_rcPosition.left;
	pPositionRect->top = m_rcPosition.top;
	pPositionRect->right = m_rcPosition.right;
	pPositionRect->bottom = m_rcPosition.bottom;

	return S_OK;
}

STDMETHODIMP CDrawMenu::GetBoundingRect(RECT* pBoundingRect)
{
	pBoundingRect->left = ROUND(m_rcPosition.left);
	pBoundingRect->top = ROUND(m_rcPosition.top);
	pBoundingRect->right = ROUND(m_rcPosition.right);
	pBoundingRect->bottom = ROUND(m_rcPosition.bottom);

	return S_OK;
}

////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawMenu::MoveHandleTo(long nHandle, POINT point)
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

STDMETHODIMP CDrawMenu::RotateAt(POINTF point, float fAngle)
{
	POINTF ptCenter;
	ptCenter.x = (m_rcPosition.left + m_rcPosition.right) / 2;
	ptCenter.y = (m_rcPosition.top + m_rcPosition.bottom) / 2;
	POINTF ptNew = CalculateRotatePoint(point, ptCenter, fAngle);
	
	OffsetRectF(&m_rcPosition, ptNew.x - ptCenter.x, ptNew.y - ptCenter.y);

	return S_OK;
}

STDMETHODIMP CDrawMenu::ScaleAt(POINTF point, float fHorScale, float fVerScale)
{
	::OffsetRectF(&m_rcPosition, -point.x, -point.y);

	m_rcPosition.left = m_rcPosition.left * fHorScale;
	m_rcPosition.right = m_rcPosition.right * fHorScale;
	m_rcPosition.top = m_rcPosition.top * fVerScale;
	m_rcPosition.bottom  = m_rcPosition.bottom * fVerScale;

	::OffsetRectF(&m_rcPosition, point.x, point.y);

	return S_OK;
}

STDMETHODIMP CDrawMenu::DrawRotateAtShape(HDC hdcDest, POINTF point, long lAngle)
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

STDMETHODIMP CDrawMenu::DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
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

STDMETHODIMP CDrawMenu::DrawTrackerShape(HDC hdcDest, POINT* pPoints, int nCount)
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

STDMETHODIMP CDrawMenu::MoveTo(RECTF rect, BOOL bUniformScale)
{
	NormalizeRect(&rect);
	m_rcPosition.left = rect.left;
	m_rcPosition.top = rect.top;
	m_rcPosition.right = rect.right;
	m_rcPosition.bottom = rect.bottom;

	return S_OK;
}

STDMETHODIMP CDrawMenu::GetHandleCount(long* pHandleCount)
{
	*pHandleCount = 0;
	return S_OK;
}

STDMETHODIMP CDrawMenu::HitTest(POINT point, BOOL* bHitTest)
{
	if ((!ClientIsRunMode() || m_bButtonDisplayed) && 
		point.x >= m_rcPosition.left && point.x < m_rcPosition.right &&
		point.y >= m_rcPosition.top && point.y < m_rcPosition.bottom)
		*bHitTest = TRUE;
	else 
		*bHitTest = FALSE;

	return S_OK;
}

STDMETHODIMP CDrawMenu::Intersects(RECT rect, BOOL* bIntersects)
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

void CDrawMenu::AddMenuItems(HMENU hMenu, int& nIndex, int nDepth)
{
	USES_CONVERSION;
	HTREEITEM hItem = NULL;
	
	while (nIndex < m_arrItems.GetSize())
	{
		CMenuItem& item = m_arrItems[nIndex];
		
		int nItemDepth = 0;
		LPCWSTR p = item.m_bstrText;
		while (*p++ == '.')
			nItemDepth++;

		if (nItemDepth < nDepth)
		{
			nIndex--;
			break;
		}

		int nNextItemDepth = nItemDepth;
		if (nIndex + 1 < m_arrItems.GetSize())
		{
			nNextItemDepth = 0;
			CMenuItem& itemNext = m_arrItems[nIndex + 1];
			LPCWSTR p = itemNext.m_bstrText;
			while (*p++ == '.')
				nNextItemDepth++;
		}

		if (nNextItemDepth > nItemDepth)
		{
			HMENU hNewMenu = CreatePopupMenu();
			ATLASSERT(hNewMenu != NULL);
					
			AddMenuItems(hNewMenu, ++nIndex, nDepth + 1);
			AppendMenu(hMenu, MF_POPUP, (UINT)hNewMenu, OLE2T(item.m_bstrText + nItemDepth));
		}
		else
		{
			CComBSTR bstrItemText = item.m_bstrText + nItemDepth; 
			if (bstrItemText == L"-")
			{
				AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
			}
			else
			{
				UINT uFlags = MF_STRING;
				if ((item.m_dwState & MenuStateDisable) != 0)
					uFlags |= MF_GRAYED;
				if ((item.m_dwState & MenuStateCheck) != 0)
					uFlags |= MF_CHECKED;
				AppendMenu(hMenu, uFlags, nIndex + 1, OLE2T(bstrItemText));
			}
		}
	
		nIndex++;
	}
}

STDMETHODIMP CDrawMenu::Popup(/*[in, optional] */VARIANT X, /*[in, optional] */VARIANT Y)
{
	USES_CONVERSION;

	int nCount = m_arrItems.GetSize();
	if (nCount == 0)
		return S_FALSE;

	HMENU hMenu = CreatePopupMenu();
	if (hMenu == NULL) 
		return E_FAIL;

	int nIndex = 0;
	AddMenuItems(hMenu, nIndex, 0);

	HWND hWndParent = GetClientWindow();

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hWndParent, &pt);

	if (X.vt != VT_ERROR)
	{
		VariantChangeType(&X, &X, 0, VT_I4);
		pt.x = X.intVal;
	}
	if (Y.vt != VT_ERROR)
	{
		VariantChangeType(&Y, &Y, 0, VT_I4);
		pt.y = Y.intVal;
	}

	ClientToScreen(hWndParent, &pt);
	
	Fire_Event(2);

	// 2011.4.25: 鼠标按下时弹出菜单时，释放鼠标捕获
	SetCapture(FALSE);

	int nRet = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD, 
		pt.x, pt.y, 0, hWndParent, 0);

	if (nRet > 0 && nRet <= m_arrItems.GetSize())
	{
		CMenuItem& item = m_arrItems[nRet - 1];
		Fire_MenuClick(nRet, item.m_bstrID);
	}

	DestroyMenu(hMenu);

	Fire_Event(3);

	return S_OK;
}

VOID CDrawMenu::Fire_MenuClick(int Item, BSTR ID)
{
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[2];
	int nConnections = m_vec.GetSize();
	
	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			pvars[0] = ID;
			pvars[1] = Item;
			DISPPARAMS disp = { pvars, NULL, 2, 0 };
			pDispatch->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
		}
	}
	delete[] pvars;
}

HWND CDrawMenu::GetClientWindow()
{
	HWND hWnd = NULL;
	if (m_spDrawObjectSite != NULL)
		m_spDrawObjectSite->GetWindow(&hWnd);
	
	return hWnd;
}

STDMETHODIMP CDrawMenu::AddItem(/*[in] */BSTR Item, /*[in, optional] */VARIANT Index, 
								/*[in, optional] */VARIANT State, /*[in, optional] */VARIANT ID,
								/*[in, optional] */VARIANT AccelShift, /*[in, optional] */VARIANT AccelCode)
{
	CMenuItem item;
	item.m_bstrText = Item;

	int iIndex = m_arrItems.GetSize();
	if (Index.vt != VT_ERROR)
	{
		CComVariant var = Index;
		var.ChangeType(VT_I4);

		iIndex = var.intVal - 1;
		if (iIndex < 1)
			iIndex = 0;
		if (iIndex > m_arrItems.GetSize())
			iIndex = m_arrItems.GetSize();
	}

	if (State.vt != VT_ERROR)
	{
		CComVariant var = State;
		var.ChangeType(VT_I4);
		item.m_dwState = var.intVal;
	}

	if (ID.vt != VT_ERROR)
	{
		CComVariant var = ID;
		var.ChangeType(VT_BSTR);
		item.m_bstrID = var.bstrVal;
	}

	if (AccelShift.vt != VT_ERROR)
	{
		CComVariant var = AccelShift;
		var.ChangeType(VT_I4);
		item.m_dwShiftState = var.intVal;
	}

	if (AccelCode.vt != VT_ERROR)
	{
		CComVariant var = AccelCode;
		var.ChangeType(VT_I4);
		item.m_dwKeyCode = var.intVal;
	}

	int nItemDepth = 0;
	LPCWSTR p = item.m_bstrText;
	while (*p++ == '.')
		nItemDepth++;
	
	int nPrevItemDepth;
	if (iIndex > 0 && iIndex <= m_arrItems.GetSize())
	{
		CMenuItem& itemPrev = m_arrItems[iIndex - 1];

		nPrevItemDepth = 0;

		p = itemPrev.m_bstrText;
		while (*p++ == '.')
			nPrevItemDepth++;
	}
	else
	{
		nPrevItemDepth = -1;
	}
	
	// 增加的深度不能超过上一级别＋1
	if (nItemDepth > nPrevItemDepth + 1)
		return E_INVALIDARG;

	m_arrItems.InsertAt(iIndex, item);

	return S_OK;
}

STDMETHODIMP CDrawMenu::SelectItem(/*[in] */int Index)
{
	if (Index < 1 || Index > m_arrItems.GetSize())
		return E_INVALIDARG;

	CMenuItem& item = m_arrItems[Index - 1];
	Fire_MenuClick(Index, item.m_bstrID);

	return S_OK;
}

STDMETHODIMP CDrawMenu::RemoveItem(/*[in] */int Index)
{
	if (Index < 1 || Index > m_arrItems.GetSize())
		return E_INVALIDARG;

	CMenuItem& item = m_arrItems[Index - 1];

	int nItemDepth = 0;
	LPCWSTR p = item.m_bstrText;
	while (*p++ == '.')
		nItemDepth++;

	while (Index < m_arrItems.GetSize())
	{
		CMenuItem& itemNext = m_arrItems[Index];

		int nNextItemDepth = 0;
		p = itemNext.m_bstrText;
		while (*p++ == '.')
			nNextItemDepth++;

		// 删除子项目
		if (nNextItemDepth <= nItemDepth)
			break;

		m_arrItems.RemoveAt(Index);
	}

	m_arrItems.RemoveAt(Index - 1);

	return S_OK;
}

STDMETHODIMP CDrawMenu::GetItemState(/*[in] */int Index, /*[out, retval] */long* pVal)
{
	if (Index < 1 || Index > m_arrItems.GetSize())
		return E_INVALIDARG;

	CMenuItem& item = m_arrItems[Index - 1];
	*pVal = item.m_dwState;

	return S_OK;
}

STDMETHODIMP CDrawMenu::SetItemState(/*[in] */int Index, /*[in] */long State)
{
	if (Index < 1 || Index > m_arrItems.GetSize())
		return E_INVALIDARG;

	CMenuItem& item = m_arrItems[Index - 1];
	item.m_dwState = State;

	return S_OK;
}

STDMETHODIMP CDrawMenu::GetItemText(/*[in] */int Index, /*[out, retval] */BSTR* pVal)
{
	if (Index < 1 || Index > m_arrItems.GetSize())
		return E_INVALIDARG;

	CMenuItem& item = m_arrItems[Index - 1];
	*pVal = item.m_bstrText.Copy();

	return S_OK;
}

STDMETHODIMP CDrawMenu::SetItemText(/*[in] */int Index, /*[in] */BSTR Item)
{
	if (Index < 1 || Index > m_arrItems.GetSize())
		return E_INVALIDARG;

	CMenuItem& item = m_arrItems[Index - 1];
	item.m_bstrText = Item;

	return S_OK;
}

STDMETHODIMP CDrawMenu::GetItemID(/*[in] */int Index, /*[out, retval] */BSTR* pVal)
{
	if (Index < 1 || Index > m_arrItems.GetSize())
		return E_INVALIDARG;
	
	CMenuItem& item = m_arrItems[Index - 1];
	*pVal = item.m_bstrID.Copy();
	
	return S_OK;
}

STDMETHODIMP CDrawMenu::SetItemID(/*[in] */int Index, /*[in] */BSTR ID)
{
	if (Index < 1 || Index > m_arrItems.GetSize())
		return E_INVALIDARG;
	
	CMenuItem& item = m_arrItems[Index - 1];
	item.m_bstrID = ID;
	
	return S_OK;
}

STDMETHODIMP CDrawMenu::GetItemAccelShift(/*[in] */int Index, /*[out, retval] */long* pVal)
{
	if (Index < 1 || Index > m_arrItems.GetSize())
		return E_INVALIDARG;
	
	CMenuItem& item = m_arrItems[Index - 1];
	*pVal = item.m_dwShiftState;
	
	return S_OK;
}

STDMETHODIMP CDrawMenu::SetItemAccelShift(/*[in] */int Index, /*[in] */long Shift)
{
	if (Index < 1 || Index > m_arrItems.GetSize())
		return E_INVALIDARG;
	
	CMenuItem& item = m_arrItems[Index - 1];
	item.m_dwShiftState = Shift;
	
	return S_OK;
}

STDMETHODIMP CDrawMenu::GetItemAccelKeyCode(/*[in] */int Index, /*[out, retval] */long* pVal)
{
	if (Index < 1 || Index > m_arrItems.GetSize())
		return E_INVALIDARG;
	
	CMenuItem& item = m_arrItems[Index - 1];
	*pVal = item.m_dwKeyCode;
	
	return S_OK;
}

STDMETHODIMP CDrawMenu::SetItemAccelKeyCode(/*[in] */int Index, /*[in] */long KeyCode)
{
	if (Index < 1 || Index > m_arrItems.GetSize())
		return E_INVALIDARG;
	
	CMenuItem& item = m_arrItems[Index - 1];
	item.m_dwKeyCode = KeyCode;
	
	return S_OK;
}

STDMETHODIMP CDrawMenu::get_ItemCount(/*[out, retval] */long *pVal)
{
	*pVal = m_arrItems.GetSize();
	return S_OK;
}

STDMETHODIMP CDrawMenu::Clear()
{
	m_arrItems.RemoveAll();
	
	return S_OK;
}

STDMETHODIMP CDrawMenu::get_ButtonDisplayed(/*[out, retval] */VARIANT_BOOL *pVal)
{
	*pVal = m_bButtonDisplayed ? VARIANT_TRUE : VARIANT_FALSE;
	
	return S_OK;
}

STDMETHODIMP CDrawMenu::put_ButtonDisplayed(/*[in] */VARIANT_BOOL newVal)
{
	m_bButtonDisplayed = (newVal != VARIANT_FALSE);
	FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CDrawMenu::get_ButtonCaption(/*[out, retval] */BSTR *pVal)
{
	*pVal = m_bstrButtonCaption.Copy();
	
	return S_OK;
}

STDMETHODIMP CDrawMenu::put_ButtonCaption(/*[in] */BSTR newVal)
{
	m_bstrButtonCaption = newVal;
	
	return S_OK;
}

STDMETHODIMP CDrawMenu::OnMouseLeave()
{
	if (m_bMouseOn)
	{
		m_bMouseOn = FALSE;
		FireViewChange();
	}

	return ICxDrawObjectImpl<CDrawMenu>::OnMouseLeave();
}

STDMETHODIMP CDrawMenu::OnWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL bSubObj, LRESULT *plResult)
{
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
		{
			POINT point;
			point.x = GET_X_LPARAM(lParam);
			point.y = GET_Y_LPARAM(lParam);
			BOOL bHit;
			HitTest(point, &bHit);
			
			if (bHit)
			{
				m_enumButtonState = ButtonStateDown;
				FireViewChange();
				
 				if (m_arrItems.GetSize() > 0)
				{
					CComVariant varX = m_rcPosition.left;
					CComVariant varY = m_rcPosition.bottom;
					Popup(varX, varY);

					m_enumButtonState = ButtonStateUp;
					FireViewChange();
					msg = WM_LBUTTONUP;
				}
				else
				{
					Fire_MenuClick(0, NULL);
//					return S_OK;
				}
			}
			
		}
		break;
	case WM_LBUTTONUP:
	case WM_KILLFOCUS:
		m_enumButtonState = ButtonStateUp;
		FireViewChange();
		break;
	}

	return ICxDrawObjectImpl<CDrawMenu>::OnWindowMessage(msg, wParam, lParam, bSubObj, plResult);
}

void CDrawMenu::GetSystemSettings()
{
	// refresh our font
	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
	LOGFONT logfont;
	memset(&logfont, 0, sizeof(LOGFONT));
	if (m_fontMenu.m_hFont != NULL)
		m_fontMenu.GetLogFont(logfont);
	if (logfont.lfHeight != info.lfMenuFont.lfHeight ||
		logfont.lfWidth != info.lfMenuFont.lfWidth ||
		logfont.lfEscapement != info.lfMenuFont.lfEscapement ||
		logfont.lfOrientation != info.lfMenuFont.lfOrientation ||
		logfont.lfWeight != info.lfMenuFont.lfWeight ||
		logfont.lfItalic != info.lfMenuFont.lfItalic ||
		logfont.lfUnderline != info.lfMenuFont.lfUnderline ||
		logfont.lfStrikeOut != info.lfMenuFont.lfStrikeOut ||
		logfont.lfCharSet != info.lfMenuFont.lfCharSet ||
		logfont.lfOutPrecision != info.lfMenuFont.lfOutPrecision ||
		logfont.lfClipPrecision != info.lfMenuFont.lfClipPrecision ||
		logfont.lfQuality != info.lfMenuFont.lfQuality ||
		logfont.lfPitchAndFamily != info.lfMenuFont.lfPitchAndFamily ||
		lstrcmp(logfont.lfFaceName, info.lfMenuFont.lfFaceName) != 0)
	{
		HFONT hFontMenu = ::CreateFontIndirect(&info.lfMenuFont);
		ATLASSERT(hFontMenu != NULL);
		if (hFontMenu != NULL)
		{
			if (m_fontMenu.m_hFont != NULL)
				m_fontMenu.DeleteObject();
			m_fontMenu.Attach(hFontMenu);
		}
	}
}