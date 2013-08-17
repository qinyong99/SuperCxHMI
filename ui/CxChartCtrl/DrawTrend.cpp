// DrawChart.cpp : Implementation of CDrawTrend
#include "stdafx.h"
#include "CxChartCtrl.h"
#include "DrawTrend.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawTrend

STDMETHODIMP CDrawTrend::IsConnected(BSTR bstrPropertyName, VARIANT_BOOL* pbHasConnection)
{
	DISPID dispid;
	GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);
	
	if (HasDynamic(dispid))
		*pbHasConnection = VARIANT_TRUE;
	else
		*pbHasConnection = VARIANT_FALSE;
	
	return S_OK;
}

STDMETHODIMP CDrawTrend::ConnectObject(BSTR bstrPropertyName, IUnknown* punkObject)
{
	DISPID dispid;
	GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);
	
	AddDynamic(dispid, punkObject);
	
	return S_OK;
}

STDMETHODIMP CDrawTrend::ConnectDirect(BSTR bstrPropertyName, BSTR bstrDataSource)
{
	DISPID dispid;
	GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);
	
	return S_OK;
}

STDMETHODIMP CDrawTrend::GetConnectObject(BSTR bstrPropertyName, IUnknown** ppunkObject)
{
	DISPID dispid;
	GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);
	
	GetDynamic(dispid, ppunkObject);
	
	return S_OK;
}

STDMETHODIMP CDrawTrend::Disconnect(BSTR bstrPropertyName)
{
	DISPID dispid;
	GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);
	
	RemoveDynamic(dispid);
	
	return S_OK;
}

STDMETHODIMP CDrawTrend::EditProperties()
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
		HWND hWnd = NULL;

		CComPtr<ICxDrawObjectSite> spDrawObjectSite;
		spDrawObject->GetDrawObjectSite(&spDrawObjectSite);
		if (spDrawObjectSite != NULL)
			spDrawObjectSite->GetWindow(&hWnd);
	
		hr = OleCreatePropertyFrame(
			hWnd,       // Parent window of the property frame
			0,          // Horizontal position of the property frame
			0,          // Vertical position of the property frame
			L"趋势",// Property frame caption
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

STDMETHODIMP CDrawTrend::EditSerials()
{
	IUnknown* pUnk = GetUnknown();
	
	CComQIPtr<ICxDrawObject> spDrawObject(pUnk);
	if (!spDrawObject)
		return E_NOINTERFACE;

	HWND hWnd = NULL;
	
	CComPtr<ICxDrawObjectSite> spDrawObjectSite;
	spDrawObject->GetDrawObjectSite(&spDrawObjectSite);
	if (spDrawObjectSite != NULL)
		spDrawObjectSite->GetWindow(&hWnd);
	
	CLSID guidPage = CLSID_CxTrendCtrlPage;
	
	OleCreatePropertyFrame(
		hWnd,       // Parent window of the property frame
		0,          // Horizontal position of the property frame
		0,          // Vertical position of the property frame
		L"趋势",// Property frame caption
		1,          // Number of objects
		&pUnk,      // Array of IUnknown pointers for objects
		1,    // Number of property pages
		&guidPage,    // Array of CLSIDs for property pages
		NULL,       // Locale identifier
		0,          // Reserved - 0
		NULL        // Reserved - 0
		);
	
	return S_OK;
}

STDMETHODIMP CDrawTrend::GetConnectState(DISPID dispid, int* pState)
{
	*pState = GetDynamicState(dispid);
	
	return S_OK;
}

STDMETHODIMP CDrawTrend::get_HorizontalPosition(float *pVal)
{
	*pVal = m_rcPosition.left;
	return S_OK;
}

STDMETHODIMP CDrawTrend::put_HorizontalPosition(float newVal)
{
	FireViewChange();
	m_rcPosition.right = newVal + m_rcPosition.right - m_rcPosition.left;
	m_rcPosition.left = newVal;
	RecalcRects();
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawTrend::get_VerticalPosition(float *pVal)
{
	*pVal = m_rcPosition.top;
	return S_OK;
}

STDMETHODIMP CDrawTrend::put_VerticalPosition(float newVal)
{
	FireViewChange();
	m_rcPosition.bottom = newVal + m_rcPosition.bottom - m_rcPosition.top;
	m_rcPosition.top = newVal;
	RecalcRects();
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawTrend::get_Width(float *pVal)
{
	*pVal = m_rcPosition.right - m_rcPosition.left;
	return S_OK;
}

STDMETHODIMP CDrawTrend::put_Width(float newVal)
{
	FireViewChange();
	if (newVal < 0)
		return S_FALSE;

	m_rcPosition.right = m_rcPosition.left + newVal;
	RecalcRects();
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawTrend::get_Height(float *pVal)
{
	*pVal = m_rcPosition.bottom - m_rcPosition.top;
	return S_OK;
}

STDMETHODIMP CDrawTrend::put_Height(float newVal)
{
	FireViewChange();
	if(newVal < 0)
		return S_FALSE;

	m_rcPosition.bottom = m_rcPosition.top + newVal;
	RecalcRects();
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawTrend::get_Name(/*[out, retval]*/ BSTR *pbstrName)
{
	*pbstrName = GetClientName();

	return S_OK;
}
STDMETHODIMP CDrawTrend::put_Name(/*[in]*/ BSTR bstrName)
{
	SetClientName(bstrName);

	return S_OK;
}

STDMETHODIMP CDrawTrend::get_Visible(/*[out, retval]*/ VARIANT_BOOL *pVal)
{
	*pVal = GetClientVisibility() ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CDrawTrend::put_Visible(/*[in]*/ VARIANT_BOOL newVal)
{
	VARIANT_BOOL oldVal = GetClientVisibility() ? VARIANT_TRUE : VARIANT_FALSE;
	if (oldVal == newVal)
		return S_OK;
	
	SetClientVisibility(newVal != VARIANT_FALSE);
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawTrend::get_Locked(/*[out, retval]*/ VARIANT_BOOL *pVal)
{
	*pVal = GetClientLocked() ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CDrawTrend::put_Locked(/*[in]*/ VARIANT_BOOL newVal)
{
	VARIANT_BOOL oldVal = GetClientLocked() ? VARIANT_TRUE : VARIANT_FALSE;
	if (oldVal == newVal)
		return S_OK;
	
	SetClientLocked(newVal != VARIANT_FALSE);
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawTrend::get_Privilege(/*[out, retval]*/ int *pVal)
{
	*pVal = GetClientPrivilege();
	return S_OK;
}

STDMETHODIMP CDrawTrend::put_Privilege(/*[in]*/ int newVal)
{
	SetClientPrivilege(newVal);
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawTrend::get_Layer(/*[out, retval]*/ int *pVal)
{
	*pVal = GetClientLayer();
	return S_OK;
}

STDMETHODIMP CDrawTrend::put_Layer(/*[in]*/ int newVal)
{
	int oldVal = GetClientLayer();
	if (oldVal == newVal)
		return S_OK;
	
	SetClientLayer(newVal);
	FireViewChange();
	return S_OK;
}

STDMETHODIMP CDrawTrend::get_MousePointer(/*[out, retval]*/ enumMousePointer *pVal)
{
	*pVal = m_enumMousePointer;
	return S_OK;
}

STDMETHODIMP CDrawTrend::put_MousePointer(/*[in]*/ enumMousePointer newVal)
{
	m_enumMousePointer = newVal;
	return S_OK;
}

STDMETHODIMP CDrawTrend::get_Description(/*[out, retval]*/ BSTR *pVal)
{
	*pVal = m_bstrDescription;
	return S_OK;
}

STDMETHODIMP CDrawTrend::put_Description(/*[in]*/ BSTR newVal)
{
	m_bstrDescription = newVal;
	return S_OK;
}

STDMETHODIMP CDrawTrend::get_ToolTipText(/*[out, retval]*/ BSTR *pbstrText)
{
	*pbstrText = GetClientToolTipText();
	return S_OK;
}

STDMETHODIMP CDrawTrend::put_ToolTipText(/*[in]*/ BSTR bstrText)
{
	SetClientToolTipText(bstrText);
	return S_OK;
}

STDMETHODIMP CDrawTrend::InitNew()
{
	CChart::InitNew();

//	AddSerial(L"Tag", NULL);

	return ICxDrawObjectImpl<CDrawTrend>::InitNew();
}

////////////////////////////////////////////////////////////////////////
//功能：ICxPersistStreamInit接口实现函数，从流对象中取本对象
//参数：pStm(输入)		要取之的流对象接口
//		fClearDirty		?
//		pMap(输入)		？
//返回：成功S_OK
////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawTrend::SaveOldVer(LPSTREAM pStm, BOOL fClearDirty)
{
	HRESULT hr = CChart::Save(pStm, fClearDirty);
	if (FAILED(hr))
		return hr;

	pStm->Write(&m_rcPosition, sizeof(m_rcPosition), NULL);
	hr = ICxDrawObjectImpl<CDrawTrend>::SaveOldVer(pStm,  fClearDirty);
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
STDMETHODIMP CDrawTrend::LoadOldVer(LPSTREAM pStm)
{
	HRESULT hr = CChart::Load(pStm);
	if (FAILED(hr))
		return hr;

	pStm->Read(&m_rcPosition, sizeof(m_rcPosition), NULL);
	hr = ICxDrawObjectImpl<CDrawTrend>::LoadOldVer(pStm);
	if (FAILED(hr))
		return hr;
		
	return S_OK;
}

HRESULT CDrawTrend::DoLoad(DWORD dwVer, LPSTREAM pStm)
{
	DWORD dwSubVer;
	pStm->Read(&dwSubVer, sizeof(DWORD), NULL);

	ReadFontFromStream(&m_spCaptionFont, pStm);
	pStm->Read(&m_clrCaption, sizeof(m_clrCaption), NULL);

	return LoadOldVer(pStm);
}

HRESULT CDrawTrend::DoSave(DWORD dwVer, LPSTREAM pStm)
{
	DWORD dwSubVer = 0x0100;
	pStm->Write(&dwSubVer, sizeof(DWORD), NULL);

	SaveFontToStream(m_spCaptionFont, pStm);
	pStm->Write(&m_clrCaption, sizeof(m_clrCaption), NULL);

	return SaveOldVer(pStm, TRUE);
}

STDMETHODIMP CDrawTrend::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("Trend");
	return S_OK;
}

HRESULT CDrawTrend::DoDraw(HDC hdcDest)
{
	CChart::DoDraw(hdcDest);

	return S_OK;
}

STDMETHODIMP CDrawTrend::GetPositionRect(RECTF* pPositionRect)
{
	pPositionRect->left = m_rcPosition.left;
	pPositionRect->top = m_rcPosition.top;
	pPositionRect->right = m_rcPosition.right;
	pPositionRect->bottom = m_rcPosition.bottom;

	return S_OK;
}

STDMETHODIMP CDrawTrend::GetBoundingRect(RECT* pBoundingRect)
{
	pBoundingRect->left = ROUND(m_rcPosition.left);
	pBoundingRect->top = ROUND(m_rcPosition.top);
	pBoundingRect->right = ROUND(m_rcPosition.right);
	pBoundingRect->bottom = ROUND(m_rcPosition.bottom);

	return S_OK;
}

////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDrawTrend::MoveHandleTo(long nHandle, POINT point)
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

STDMETHODIMP CDrawTrend::RotateAt(POINTF point, float fAngle)
{
	POINTF ptCenter;
	ptCenter.x = (m_rcPosition.left + m_rcPosition.right) / 2;
	ptCenter.y = (m_rcPosition.top + m_rcPosition.bottom) / 2;
	POINTF ptNew = CalculateRotatePoint(point, ptCenter, fAngle);
	
	OffsetRectF(&m_rcPosition, ptNew.x - ptCenter.x, ptNew.y - ptCenter.y);

	RecalcRects();

	return S_OK;
}

STDMETHODIMP CDrawTrend::ScaleAt(POINTF point, float fHorScale, float fVerScale)
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

STDMETHODIMP CDrawTrend::DrawRotateAtShape(HDC hdcDest, POINTF point, long lAngle)
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

STDMETHODIMP CDrawTrend::DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
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

STDMETHODIMP CDrawTrend::DrawTrackerShape(HDC hdcDest, POINT* pPoints, int nCount)
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

STDMETHODIMP CDrawTrend::MoveTo(RECTF rect, BOOL bUniformScale)
{
	NormalizeRect(&rect);
	m_rcPosition.left = rect.left;
	m_rcPosition.top = rect.top;
	m_rcPosition.right = rect.right;
	m_rcPosition.bottom = rect.bottom;

	RecalcRects();

	return S_OK;
}

STDMETHODIMP CDrawTrend::GetHandleCount(long* pHandleCount)
{
	*pHandleCount = 0;
	return S_OK;
}

STDMETHODIMP CDrawTrend::HitTest(POINT point, BOOL* bHitTest)
{
	if (point.x >= m_rcPosition.left && point.x < m_rcPosition.right &&
		point.y >= m_rcPosition.top && point.y < m_rcPosition.bottom)
		*bHitTest = TRUE;
	else
		*bHitTest = FALSE;

	return S_OK;
}

STDMETHODIMP CDrawTrend::Intersects(RECT rect, BOOL* bIntersects)
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

STDMETHODIMP CDrawTrend::InitRun(BOOL* pbHasDynamic, ICxScriptEngineSite* pScriptSite)
{
	if (m_apSerials.GetSize() > 0)
		m_spDrawObjectSite->SetTimer(500);
	
//	Zoom(360, 380, 400, 150);

	m_spScriptSite = pScriptSite;

	ConnectToDataServer();
	OnRefreshData(TRUE);

	HRESULT hr = ICxDrawObjectImpl<CDrawTrend>::InitRun(pbHasDynamic, pScriptSite);
	*pbHasDynamic = TRUE;

	return hr;
}

STDMETHODIMP CDrawTrend::FakeParseDynamic()
{
	BOOL bChanged = FALSE;

	for (int i = 0; i < m_apSerials.GetSize(); i++)
	{
		CSerial* pSerial = m_apSerials[i];
		CComBSTR bstrOld = pSerial->m_bstrTag;
		m_spScriptSite->PreTreatExpression(pSerial->m_bstrSource, &pSerial->m_bstrTag);
		if (lstrcmpiW(bstrOld, pSerial->m_bstrTag) != 0)
			bChanged = TRUE;
		//	pSerial->RemoveAllValues();
	}

	if (bChanged)
	{
		ConnectToDataServer();
		OnRefreshData(TRUE);
	}
	
//	FireViewChange();
	
	return ICxDrawObjectImpl<CDrawTrend>::FakeParseDynamic();
}

BOOL CDrawTrend::OnRefreshSerialData(CSerial* pSerial)
{
	CComVariant varExpressionValue;
	//求表达式值
	m_spScriptSite->EvaluateExpression(pSerial->m_bstrSource, &varExpressionValue);
	if (varExpressionValue.vt != VT_EMPTY)
	{
		DATAPOINT datapoint;

		COleDateTime dtTimeStamp = COleDateTime::GetCurrentTime();
		datapoint.dXVal = dtTimeStamp.m_dt;
		varExpressionValue.ChangeType(VT_R8);
		datapoint.dYVal = varExpressionValue.dblVal;
		pSerial->AddValue(datapoint);

		return TRUE;
	}

	return FALSE;
}

void CDrawTrend::GetDefaultServer(BSTR* pbstrComputer)
{
	if (m_spDrawObjectSite != NULL)
		m_spDrawObjectSite->GetDefaultServer(pbstrComputer);
}

STDMETHODIMP CDrawTrend::OnTimer()
{
	int n = m_dwRefreshInterval / 100;
	if (n < 1)
		n = 1;
	if ((m_nTimeFactor %= n) == 0)
	{
		if (OnRefreshData())
			FireViewChange();
	}

	m_nTimeFactor += 1;

//	static int n = 0;
//	if ((n %= m_dwRefreshInterval) == 0)
//	{
//		int nCount = m_apSerials.GetSize();
//		for (int i = 0; i < nCount; i++)
//		{
//			CComVariant varExpressionValue;
//			//求表达式值
//			m_spScriptSite->EvaluateExpression(m_apSerials[i]->m_bstrSource, &varExpressionValue);
//			varExpressionValue.ChangeType(VT_R8);
//			m_apSerials[i]->AddValue(varExpressionValue.dblVal);
//
//			
//		}
//	}
//	
//	n += 100;
//
	
	return S_OK;
}

BOOL CDrawTrend::RecalcRects()
{
	m_rect.left = ROUND(m_rcPosition.left);
	m_rect.top = ROUND(m_rcPosition.top);
	m_rect.right = ROUND(m_rcPosition.right);
	m_rect.bottom = ROUND(m_rcPosition.bottom);

	return CChart::RecalcRects();
}

HRESULT CDrawTrend::FireViewChange()
{
	return CCxDrawObjectBase::FireViewChange();
}

BOOL CDrawTrend::ClientIsRunMode()
{
	return CCxDrawObjectBase::ClientIsRunMode();
}

