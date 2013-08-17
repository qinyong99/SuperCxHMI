// DrawAlmSum.cpp : Implementation of CDrawAlmSum
#include "stdafx.h"
#include "CxAlmSumCtrl.h"
#include "Alarm.h"
#include "DrawHeader.h"
#include "DrawGrid.h"
#include "DrawScrollBar.h"
#include "DrawAlmSum.h"


/////////////////////////////////////////////////////////////////////////////
// CDrawAlmSum


STDMETHODIMP CDrawAlmSum::IsConnected(BSTR bstrPropertyName, VARIANT_BOOL* pbHasConnection)
{
	DISPID dispid;
	GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);
	
	if (HasDynamic(dispid))
		*pbHasConnection = VARIANT_TRUE;
	else
		*pbHasConnection = VARIANT_FALSE;
	
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::ConnectObject(BSTR bstrPropertyName, IUnknown* punkObject)
{
	DISPID dispid;
	GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);
	
	AddDynamic(dispid, punkObject);
	
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::ConnectDirect(BSTR bstrPropertyName, BSTR bstrDataSource)
{
	DISPID dispid;
	GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);
	
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::GetConnectObject(BSTR bstrPropertyName, IUnknown** ppunkObject)
{
	DISPID dispid;
	GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);
	
	GetDynamic(dispid, ppunkObject);
	
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::Disconnect(BSTR bstrPropertyName)
{
	DISPID dispid;
	GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);
	
	RemoveDynamic(dispid);
	
	return S_OK;
}

HWND CDrawAlmSum::GetClientWindow()
{
	HWND hWnd = NULL;
	if (m_spDrawObjectSite != NULL)
		m_spDrawObjectSite->GetWindow(&hWnd);

	return hWnd;
}

void CDrawAlmSum::GetDefaultServer(BSTR* pbstrComputer)
{
	if (m_spDrawObjectSite != NULL)
		m_spDrawObjectSite->GetDefaultServer(pbstrComputer);
}

HRESULT CDrawAlmSum::OnEditProperties()
{
	IUnknown* pUnk = GetUnknown();
	
	CComQIPtr<ICxDrawObject> spDrawObject(pUnk);
	if (!spDrawObject)
		return E_NOINTERFACE;
	
	// Get list of property pages
	CAUUID cauuid = {0};
	HRESULT hr = spDrawObject->GetPages(&cauuid);
	
	// Use the system-supplied property frame
	if (SUCCEEDED(hr) && (cauuid.cElems != 0))
	{
		hr = OleCreatePropertyFrame(
			GetClientWindow(),       // Parent window of the property frame
			0,          // Horizontal position of the property frame
			0,          // Vertical position of the property frame
			L"报警表",// Property frame caption
			1,          // Number of objects
			&pUnk,      // Array of IUnknown pointers for objects
			cauuid.cElems,    // Number of property pages
			cauuid.pElems,    // Array of CLSIDs for property pages
			NULL,       // Locale identifier
			0,          // Reserved - 0
			NULL        // Reserved - 0
			);
		
		// Free array allocated by GetPages
		CoTaskMemFree(cauuid.pElems);
	}
	
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::GetConnectState(DISPID dispid, int* pState)
{
	*pState = GetDynamicState(dispid);
	
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::get_HorizontalPosition(float *pVal)
{
	*pVal = m_rcPosition.left;
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::put_HorizontalPosition(float newVal)
{
	FireViewChange();
	m_rcPosition.right = newVal + m_rcPosition.right - m_rcPosition.left;
	m_rcPosition.left = newVal;
	RecalcRects();
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::get_VerticalPosition(float *pVal)
{
	*pVal = m_rcPosition.top;
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::put_VerticalPosition(float newVal)
{
	FireViewChange();
	m_rcPosition.bottom = newVal + m_rcPosition.bottom - m_rcPosition.top;
	m_rcPosition.top = newVal;
	RecalcRects();
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::get_Width(float *pVal)
{
	*pVal = m_rcPosition.right - m_rcPosition.left;
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::put_Width(float newVal)
{
	FireViewChange();
	if (newVal < 0)
		return S_FALSE;

	m_rcPosition.right = m_rcPosition.left + newVal;
	RecalcRects();
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::get_Height(float *pVal)
{
	*pVal = m_rcPosition.bottom - m_rcPosition.top;
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::put_Height(float newVal)
{
	FireViewChange();
	if(newVal < 0)
		return S_FALSE;

	m_rcPosition.bottom = m_rcPosition.top + newVal;
	RecalcRects();
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::get_Name(/*[out, retval]*/ BSTR *pbstrName)
{
	*pbstrName = GetClientName();

	return S_OK;
}
STDMETHODIMP CDrawAlmSum::put_Name(/*[in]*/ BSTR bstrName)
{
	SetClientName(bstrName);

	return S_OK;
}

STDMETHODIMP CDrawAlmSum::get_Visible(/*[out, retval]*/ VARIANT_BOOL *pVal)
{
	*pVal = GetClientVisibility() ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::put_Visible(/*[in]*/ VARIANT_BOOL newVal)
{
	VARIANT_BOOL oldVal = GetClientVisibility() ? VARIANT_TRUE : VARIANT_FALSE;
	if (oldVal == newVal)
		return S_OK;
	
	SetClientVisibility(newVal != VARIANT_FALSE);
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::get_Privilege(/*[out, retval]*/ int *pVal)
{
	*pVal = GetClientPrivilege();
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::put_Privilege(/*[in]*/ int newVal)
{
	SetClientPrivilege(newVal);
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::get_Layer(/*[out, retval]*/ int *pVal)
{
	*pVal = GetClientLayer();
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::put_Layer(/*[in]*/ int newVal)
{
	int oldVal = GetClientLayer();
	if (oldVal == newVal)
		return S_OK;
	
	SetClientLayer(newVal);
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::get_Locked(/*[out, retval]*/ VARIANT_BOOL *pVal)
{
	*pVal = GetClientLocked() ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::put_Locked(/*[in]*/ VARIANT_BOOL newVal)
{
	VARIANT_BOOL oldVal = GetClientLocked() ? VARIANT_TRUE : VARIANT_FALSE;
	if (oldVal == newVal)
		return S_OK;
	
	SetClientLocked(newVal != VARIANT_FALSE);
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::get_MousePointer(/*[out, retval]*/ enumMousePointer *pVal)
{
	*pVal = m_enumMousePointer;
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::put_MousePointer(/*[in]*/ enumMousePointer newVal)
{
	m_enumMousePointer = newVal;
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::get_Description(/*[out, retval]*/ BSTR *pVal)
{
	*pVal = m_bstrDescription;
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::put_Description(/*[in]*/ BSTR newVal)
{
	m_bstrDescription = newVal;
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::get_ToolTipText(/*[out, retval]*/ BSTR *pbstrText)
{
	*pbstrText = GetClientToolTipText();
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::put_ToolTipText(/*[in]*/ BSTR bstrText)
{
	SetClientToolTipText(bstrText);
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::GetPositionRect(RECTF* pPositionRect)
{
	pPositionRect->left = m_rcPosition.left;
	pPositionRect->top = m_rcPosition.top;
	pPositionRect->right = m_rcPosition.right;
	pPositionRect->bottom = m_rcPosition.bottom;

	return S_OK;
}

STDMETHODIMP CDrawAlmSum::GetBoundingRect(RECT* pBoundingRect)
{
	pBoundingRect->left = ROUND(m_rcPosition.left);
	pBoundingRect->top = ROUND(m_rcPosition.top);
	pBoundingRect->right = ROUND(m_rcPosition.right);
	pBoundingRect->bottom = ROUND(m_rcPosition.bottom);

	return S_OK;
}

////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawAlmSum::MoveHandleTo(long nHandle, POINT point)
{
	DefaultMoveHandleTo(&m_rcPosition, -nHandle, point);
	NormalizeRect(&m_rcPosition);

	RecalcRects();

	return S_OK;
}

BOOL OffsetRectF(RECTF* pRect, float fOffsetX, float fOffsetY)
{
	pRect->left += fOffsetX;
	pRect->right += fOffsetX;
	pRect->top += fOffsetY;
	pRect->bottom += fOffsetY;

	return TRUE;
}

STDMETHODIMP CDrawAlmSum::RotateAt(POINTF point, float fAngle)
{
	POINTF ptCenter;
	ptCenter.x = (m_rcPosition.left + m_rcPosition.right) / 2;
	ptCenter.y = (m_rcPosition.top + m_rcPosition.bottom) / 2;
	POINTF ptNew = CalculateRotatePoint(point, ptCenter, fAngle);
	
	OffsetRectF(&m_rcPosition, ptNew.x - ptCenter.x, ptNew.y - ptCenter.y);

	return S_OK;
}

STDMETHODIMP CDrawAlmSum::ScaleAt(POINTF point, float fHorScale, float fVerScale)
{
	::OffsetRectF(&m_rcPosition, -point.x, -point.y);

	m_rcPosition.left = m_rcPosition.left * fHorScale;
	m_rcPosition.right = m_rcPosition.right * fHorScale;
	m_rcPosition.top = m_rcPosition.top * fVerScale;
	m_rcPosition.bottom  = m_rcPosition.bottom * fVerScale;

	::OffsetRectF(&m_rcPosition, point.x, point.y);

	RecalcRects();

	return S_OK;
}

STDMETHODIMP CDrawAlmSum::DrawRotateAtShape(HDC hdcDest, POINTF point, long lAngle)
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

STDMETHODIMP CDrawAlmSum::DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
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

STDMETHODIMP CDrawAlmSum::DrawTrackerShape(HDC hdcDest, POINT* pPoints, int nCount)
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

STDMETHODIMP CDrawAlmSum::MoveTo(RECTF rect, BOOL bUniformScale)
{
	NormalizeRect(&rect);
	m_rcPosition.left = rect.left;
	m_rcPosition.top = rect.top;
	m_rcPosition.right = rect.right;
	m_rcPosition.bottom = rect.bottom;

	RecalcRects();

	return S_OK;
}

STDMETHODIMP CDrawAlmSum::GetHandleCount(long* pHandleCount)
{
	*pHandleCount = 0;
	return S_OK;
}

STDMETHODIMP CDrawAlmSum::HitTest(POINT point, BOOL* bHitTest)
{
	if (point.x >= m_rcPosition.left && point.x < m_rcPosition.right &&
		point.y >= m_rcPosition.top && point.y < m_rcPosition.bottom)
		*bHitTest = TRUE;
	else
		*bHitTest = FALSE;

	return S_OK;
}

STDMETHODIMP CDrawAlmSum::Intersects(RECT rect, BOOL* bIntersects)
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

BOOL CDrawAlmSum::RecalcRects()
{
	m_rect.left = ROUND(m_rcPosition.left);
	m_rect.top = ROUND(m_rcPosition.top);
	m_rect.right = ROUND(m_rcPosition.right);
	m_rect.bottom = ROUND(m_rcPosition.bottom);
	
	return CAlmSum::RecalcRects();
}

HRESULT CDrawAlmSum::FireViewChange()
{
	return CCxDrawObjectBase::FireViewChange();
}

BOOL CDrawAlmSum::ClientIsRunMode()
{
	return CCxDrawObjectBase::ClientIsRunMode();
}

void CDrawAlmSum::FinalRelease()
{
	if (m_hDC != NULL)
	{
		m_spDrawObjectSite->ReleaseDC(m_hDC);
		m_hDC = NULL;
	}
}

void CDrawAlmSum::SetCapture(BOOL bCapture, CDrawCtrl* pCtrl)
{
	ATLASSERT(m_spDrawObjectSite != NULL);
	m_spDrawObjectSite->SetCapture(bCapture);

	CAlmSum::SetCapture(bCapture, pCtrl);
}

HDC CDrawAlmSum::GetDC()
{
	ATLASSERT(m_spDrawObjectSite != NULL);
	if (m_hDC == NULL)
		m_spDrawObjectSite->GetDC(&m_hDC);

	return m_hDC;
}

void CDrawAlmSum::ReleaseDC(HDC hDC)
{
	ATLASSERT(m_spDrawObjectSite != NULL);
	if (m_hDC != NULL)
		m_spDrawObjectSite->ReleaseDC(hDC);
	m_hDC = NULL;
}

STDMETHODIMP CDrawAlmSum::InitNew()
{
	CAlmSum::InitNew();

// 	GetAmbientBackColor(m_clrBack);
	
	return ICxDrawObjectImpl<CDrawAlmSum>::InitNew();
}

////////////////////////////////////////////////////////////////////////
//功能：ICxPersistStreamInit接口实现函数，从流对象中取本对象
//参数：pStm(输入)		要取之的流对象接口
//		fClearDirty		?
//		pMap(输入)		？
//返回：成功S_OK
////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawAlmSum::SaveOldVer(LPSTREAM pStm, BOOL fClearDirty)
{
	pStm->Write(&m_rcPosition, sizeof(m_rcPosition), NULL);
	HRESULT hr = ICxDrawObjectImpl<CDrawAlmSum>::SaveOldVer(pStm,  fClearDirty);
	if (FAILED(hr))
		return hr;
	
	hr = CAlmSum::Save(pStm, fClearDirty);
	if (FAILED(hr))
		return hr;
	
	return S_OK;
}

////////////////////////////////////////////////////////////////////////
//功能：ICxPersistStreamInit接口实现函数，存对象到指定流对象
//参数：pStm(输入)		要存入的流对象接口
//		pMap(输入)		？
//返回：成功S_OK
////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawAlmSum::LoadOldVer(LPSTREAM pStm)
{
	pStm->Read(&m_rcPosition, sizeof(m_rcPosition), NULL);
	HRESULT hr = ICxDrawObjectImpl<CDrawAlmSum>::LoadOldVer(pStm);
	if (FAILED(hr))
		return hr;
	
	hr = CAlmSum::Load(pStm);
	if (FAILED(hr))
		return hr;
	
	return S_OK;
}

HRESULT CDrawAlmSum::DoLoad(DWORD dwVer, LPSTREAM pStm)
{
	DWORD dwSubVer;
	pStm->Read(&dwSubVer, sizeof(DWORD), NULL);
	
	return LoadOldVer(pStm);
	
	//	return CCxDrawObjectBase::DoLoad(dwVer, pStm);
}

HRESULT CDrawAlmSum::DoSave(DWORD dwVer, LPSTREAM pStm)
{
	DWORD dwSubVer = 0x0100;
	pStm->Write(&dwSubVer, sizeof(DWORD), NULL);
	
	return SaveOldVer(pStm, TRUE);
	
	//	return CCxDrawObjectBase::DoSave(dwVer, pStm);
}

STDMETHODIMP CDrawAlmSum::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("AlmSum");
	return S_OK;
}

HRESULT CDrawAlmSum::DoDraw(HDC hdcDest)
{
	CAlmSum::DoDraw(hdcDest);

	return S_OK;
}

STDMETHODIMP CDrawAlmSum::InitRun(BOOL* pbHasDynamic, ICxScriptEngineSite* pScriptSite)
{
	ReConnectNetNodes();
	
	return ICxDrawObjectImpl<CDrawAlmSum>::InitRun(pbHasDynamic, pScriptSite);
}

STDMETHODIMP CDrawAlmSum::OnWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL bSubObj, LRESULT *plResult)
{
	BOOL bHandle = FALSE;
	if (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)
	{
		DoWindowMouseMessage(msg, wParam, lParam, bHandle);
	}
	else if (msg == WM_KEYDOWN)
	{
		DoKeyDown(msg, wParam, lParam, bHandle);
	}

	if (bHandle)
		return S_OK;

	return ICxDrawObjectImpl<CDrawAlmSum>::OnWindowMessage(msg, wParam, lParam, bSubObj, plResult);
}

void CDrawAlmSum::OnAlarmAcked(LPCTSTR szSource)
{
	USES_CONVERSION;
	Fire_AlarmAcknowledge(T2OLE(szSource));
}
