// AlmSumCtrl.cpp : Implementation of CAlmSumCtrl

#include "stdafx.h"
#include "CxAlmSumCtrl.h"
#include "AlmSumCtrl.h"
#pragma comment(lib, "comctl32.lib")



/////////////////////////////////////////////////////////////////////////////
// CAlmSumCtrl

BOOL CAlmSumCtrl::PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet)
{
	if(pMsg->message == WM_KEYDOWN && 
		(pMsg->wParam == VK_LEFT || 
		pMsg->wParam == VK_RIGHT ||
		pMsg->wParam == VK_UP ||
		pMsg->wParam == VK_DOWN))
	{
		hRet = S_FALSE;
		return TRUE;
	}
	
	return FALSE;
}

HRESULT CAlmSumCtrl::OnDraw(ATL_DRAWINFO& di)
{
	HDC& hdcDest = di.hdcDraw;
	
	CAlmSum::DoDraw(hdcDest);

	return S_OK;
}	

STDMETHODIMP CAlmSumCtrl::InitNew()
{
	CAlmSum::InitNew();

	return IPersistStreamInitImpl<CAlmSumCtrl>::InitNew();
}

////////////////////////////////////////////////////////////////////////
//功能：ICxPersistStreamInit接口实现函数，从流对象中取本对象
//参数：pStm(输入)		要取之的流对象接口
//		fClearDirty		?
//		pMap(输入)		？
//返回：成功S_OK
////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAlmSumCtrl::Save(LPSTREAM pStm, BOOL fClearDirty)
{
	HRESULT hr = IPersistStreamInitImpl<CAlmSumCtrl>::Save(pStm,  fClearDirty);
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
STDMETHODIMP CAlmSumCtrl::Load(LPSTREAM pStm)
{
	HRESULT hr = IPersistStreamInitImpl<CAlmSumCtrl>::Load(pStm);
	if (FAILED(hr))
		return hr;
	
	hr = CAlmSum::Load(pStm);
	if (FAILED(hr))
		return hr;
	
	return S_OK;
}

LRESULT CAlmSumCtrl::OnSize (UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)		
{
	RecalcRects();
	
	return 0;
}

// 不进行缺省绘制
LRESULT CAlmSumCtrl::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1;
}

LRESULT CAlmSumCtrl::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (ClientIsRunMode())
		ReConnectNetNodes();
	
	return 0;
}

HRESULT CAlmSumCtrl::FireViewChange()
{
	return CComControlBase::FireViewChange();
}

// 是否运行模式
BOOL CAlmSumCtrl::ClientIsRunMode()
{
	BOOL bUserMode = TRUE;
	HRESULT hRet = GetAmbientUserMode(bUserMode);
	
	return bUserMode;
}

HDC CAlmSumCtrl::GetDC()
{
	if (m_hDC == NULL)
		m_hDC = ::GetDC(m_hWnd);
	
	return ::GetDC(m_hWnd);
}

void CAlmSumCtrl::ReleaseDC(HDC hDC)
{
	if (m_hDC != NULL)
		::ReleaseDC(m_hWnd, hDC);
	m_hDC = NULL;
}

void CAlmSumCtrl::OnFinalMessage(HWND /*hWnd*/)
{
	if (m_hDC != NULL)
		::ReleaseDC(m_hWnd, m_hDC);
	m_hDC = NULL;
}

BOOL CAlmSumCtrl::RecalcRects()
{
	if (!IsWindow())
		return FALSE;
	
	RECT rcClient;
	GetClientRect(&rcClient);
	
	m_rect = rcClient;
//	m_rect.right--;
//	m_rect.bottom--;

	return CAlmSum::RecalcRects();
}

void CAlmSumCtrl::SetCapture(BOOL bCapture, CDrawCtrl* pCtrl)
{
	if (bCapture)
		CWindow::SetCapture();
	else
		ReleaseCapture();

	CAlmSum::SetCapture(bCapture, pCtrl);
}

HWND CAlmSumCtrl::GetClientWindow()
{
	return m_hWnd;
}

HRESULT CAlmSumCtrl::OnEditProperties()
{
	IUnknown* pUnk = GetUnknown();
	
	CComQIPtr<ISpecifyPropertyPages> spSpec(pUnk);
	if (!spSpec)
		return E_NOINTERFACE;
	
	// Get list of property pages
	CAUUID cauuid = {0};
	HRESULT hr = spSpec->GetPages(&cauuid);
	
	// Use the system-supplied property frame
	if (SUCCEEDED(hr) && (cauuid.cElems != 0))
	{
		hr = OleCreatePropertyFrame(
			m_hWnd,       // Parent window of the property frame
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
