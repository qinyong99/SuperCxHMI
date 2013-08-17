// TrendDispatchImpl.h: interface for the CTrendDispatchImpl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __TRENDDISPATCHIMPL_H_
#define __TRENDDISPATCHIMPL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class T, class IT, const IID* piid, const GUID* plibid = &CComModule::m_libid, WORD wMajor = 1,
WORD wMinor = 0, class tihclass = CComTypeInfoHolder>
class ATL_NO_VTABLE CTrendDispatchImpl : public IDispatchImpl<IT, piid, plibid, wMajor, wMinor, tihclass>
{
public:
	CTrendDispatchImpl();
	virtual ~CTrendDispatchImpl();

public:
	STDMETHOD(AddSerial)(/*[in]*/ BSTR szSource, /*[out, retval]*/ ISerial** ppSerial);
	STDMETHOD(GetSerialsCount)(/*[out, retval]*/ long* plCount);
	STDMETHOD(GetSerial)(/*[in]*/ long lIndex, /*[out, retval]*/ ISerial** ppSerial);
	STDMETHOD(RemoveSerial)(/*[in]*/ long lIndex);
	STDMETHOD(ScrollBack)(/*[in, optional]*/ VARIANT TimeSpan);
	STDMETHOD(ScrollForward)(/*[in, optional]*/ VARIANT TimeSpan);
	STDMETHOD(ScrollForwardMost)();
	STDMETHOD(Scroll)(BOOL bHorizon, BOOL bUpOrRight);
	STDMETHOD(Zoom)(/*[in]*/ float fYHi, /*[in]*/ float fYLo, /*[in]*/ float fXHi, /*[in]*/ float fXLo);
	STDMETHOD(ZoomIn)();
	STDMETHOD(ZoomOut)();
	STDMETHOD(ScrollTo)(VARIANT FromTime, /*[in, optional]*/ VARIANT ToTime);
	STDMETHOD(PrintOut)();
	
	STDMETHOD(get_FrameStyle)(/*[out, retval]*/ enumFrameStyle *pVal);
	STDMETHOD(put_FrameStyle)(/*[in]*/ enumFrameStyle newVal);
	STDMETHOD(get_ShowLegend)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowLegend)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_RefreshInterval)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_RefreshInterval)(/*[in]*/ long newVal);
	STDMETHOD(get_ZoomMode)(/*[out, retval]*/ enumZoomMode *pVal);
	STDMETHOD(put_ZoomMode)(/*[in]*/ enumZoomMode newVal);
	STDMETHOD(get_ScrollDirection)(/*[out, retval]*/ enumScrollDirection *pVal);
	STDMETHOD(put_ScrollDirection)(/*[in]*/ enumScrollDirection newVal);
	STDMETHOD(get_FrameColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_FrameColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_ShowMutiGrid)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowMutiGrid)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ShowMutiValueAxis)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowMutiValueAxis)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ShowMutiTimeAxis)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowMutiTimeAxis)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Transparency)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Transparency)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ForeColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_ForeColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_BackColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_BackColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_ScrollPercentage)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_ScrollPercentage)(/*[in]*/ float newVal);
	STDMETHOD(get_TrendMode)(/*[out, retval]*/ enumTrendMode *pVal);
	STDMETHOD(put_TrendMode)(/*[in]*/ enumTrendMode newVal);
	STDMETHOD(get_Caption)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Caption)(/*[in]*/ BSTR newVal);

	STDMETHOD(put_CaptionFont)(IFontDisp* pFont);
	STDMETHOD(putref_CaptionFont)(IFontDisp* pFont);
	STDMETHOD(get_CaptionFont)(IFontDisp** ppFont);
	STDMETHOD(get_CaptionColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_CaptionColor)(/*[in]*/ OLE_COLOR newVal);

	STDMETHOD(get_RunMode)(/*[out, retval]*/ VARIANT_BOOL *pVal);

	STDMETHOD(get_TimeAxis)(/*[out, retval]*/ ITimeAxis* *pVal);
	STDMETHOD(get_ValueAxis)(/*[out, retval]*/ IValueAxis * *pVal);
	STDMETHOD(get_Grid)(/*[out, retval]*/ IGrid* *pVal);
};

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>()
{
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::~CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>()
{

}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::PrintOut()
{
	T* pT = static_cast<T*>(this);
	pT->OnPrint();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::AddSerial(BSTR szSource, ISerial** ppSerial)
{
	T* pT = static_cast<T*>(this);

	USES_CONVERSION;

	CComObject<CSerial>* pSerial = new CComObject<CSerial>;
	pSerial->m_pChart = pT;
	pSerial->m_bstrSource = szSource;
	pSerial->m_bstrTag = szSource;
	pSerial->InitNew();
	
	pSerial->AddRef();
	pT->m_apSerials.Add(pSerial);
	
	pT->RecalcRects();

	if (pT->ClientIsRunMode())
	{
		pT->ConnectToDataServer(pSerial);

		if (pT->m_enumTrendMode == TrendModeHistory)
			pSerial->RefreshHistData();
	//	InitSerialDemoVaules();
	}

	pT->FireViewChange();
	
	if (ppSerial != NULL)
		pSerial->QueryInterface(IID_ISerial, (void **)ppSerial);
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::GetSerialsCount(long *plCount)
{
	T* pT = static_cast<T*>(this);

	*plCount = pT->m_apSerials.GetSize();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::GetSerial(long lIndex, ISerial** ppSerial)
{
	T* pT = static_cast<T*>(this);

	if (lIndex < 0 || lIndex >= pT->m_apSerials.GetSize())
		return S_FALSE;
	
	pT->m_apSerials[lIndex]->QueryInterface(IID_ISerial, (void **)ppSerial);
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::RemoveSerial(long lIndex)
{
	T* pT = static_cast<T*>(this);

	if (lIndex < 0 || lIndex >= pT->m_apSerials.GetSize())
		return S_FALSE;
	
	CSerial* pSerial = pT->m_apSerials[lIndex];
	if (pSerial->m_dwTagCookie != 0)
		pT->ConnectToDataServer(NULL, pSerial);
	
	pSerial->Release();
	pT->m_apSerials.RemoveAt(lIndex);
	
	pT->RecalcRects();
	pT->InitSerialDemoVaules();
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_Transparency(VARIANT_BOOL *pVal)
{
	T* pT = static_cast<T*>(this);

	*pVal = pT->m_bTransparency;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_Transparency(VARIANT_BOOL newVal)
{
	T* pT = static_cast<T*>(this);

	pT->m_bTransparency = newVal;
	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_ShowMutiGrid(/*[out, retval]*/ VARIANT_BOOL *pVal)
{
	T* pT = static_cast<T*>(this);
	
	*pVal = pT->m_bShowMutiGrid;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_ShowMutiGrid(/*[in]*/ VARIANT_BOOL newVal)
{
	T* pT = static_cast<T*>(this);
	
	pT->m_bShowMutiGrid = newVal;
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_ShowMutiTimeAxis(VARIANT_BOOL *pVal)
{
	T* pT = static_cast<T*>(this);
	
	*pVal = pT->m_bShowMutiTimeAxis;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_ShowMutiTimeAxis(VARIANT_BOOL newVal)
{
	T* pT = static_cast<T*>(this);

	pT->m_bShowMutiTimeAxis = newVal;
	pT->RecalcRects();
	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_ShowMutiValueAxis(VARIANT_BOOL *pVal)
{
	T* pT = static_cast<T*>(this);

	*pVal = pT->m_bShowMutiValueAxis;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_ShowMutiValueAxis(VARIANT_BOOL newVal)
{
	T* pT = static_cast<T*>(this);

	pT->m_bShowMutiValueAxis = newVal;
	pT->RecalcRects();
	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_FrameColor(OLE_COLOR *pVal)
{
	T* pT = static_cast<T*>(this);

	*pVal = pT->m_clrFrame;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_FrameColor(OLE_COLOR newVal)
{
	T* pT = static_cast<T*>(this);

	pT->m_clrFrame = newVal;
	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_ScrollDirection(enumScrollDirection *pVal)
{
	T* pT = static_cast<T*>(this);

	*pVal = pT->m_enumScrollDirection;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_ScrollDirection(enumScrollDirection newVal)
{
	T* pT = static_cast<T*>(this);

	if (pT->m_enumScrollDirection == newVal)
		return S_OK;
	
	pT->m_enumScrollDirection = newVal;
	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_ZoomMode(enumZoomMode *pVal)
{
	T* pT = static_cast<T*>(this);

	*pVal = pT->m_enumZoomMode;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_ZoomMode(enumZoomMode newVal)
{
	T* pT = static_cast<T*>(this);

	if (pT->m_enumZoomMode == newVal)
		return S_OK;
	
//	m_wndToolPane.Zoom(1);
	pT->m_enumZoomMode = newVal;

	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_RefreshInterval(long *pVal)
{
	T* pT = static_cast<T*>(this);

	*pVal = pT->m_dwRefreshInterval;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_RefreshInterval(long newVal)
{
	T* pT = static_cast<T*>(this);

	newVal = newVal / 100 * 100;
	if (newVal < 100)
		newVal = 100;

	pT->m_dwRefreshInterval = newVal;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_ShowLegend(VARIANT_BOOL *pVal)
{
	T* pT = static_cast<T*>(this);

	*pVal = pT->m_bShowLegend;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_ShowLegend(VARIANT_BOOL newVal)
{
	T* pT = static_cast<T*>(this);

	pT->m_bShowLegend = newVal;
	pT->RecalcRects();
	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_FrameStyle(enumFrameStyle *pVal)
{
	T* pT = static_cast<T*>(this);

	*pVal = pT->m_enumFrameStyle;
	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_FrameStyle(enumFrameStyle newVal)
{
	T* pT = static_cast<T*>(this);

	pT->m_enumFrameStyle = newVal;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_ScrollPercentage(float *pVal)
{
	T* pT = static_cast<T*>(this);

	*pVal = pT->m_fScrollPercentage;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_ScrollPercentage(float newVal)
{
	T* pT = static_cast<T*>(this);

	pT->m_fScrollPercentage = newVal;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::ScrollForward(/*[in, optional]*/ VARIANT TimeSpan)
{
	T* pT = static_cast<T*>(this);

	double dTimeSpan;
	if (TimeSpan.vt != VT_ERROR)
	{
		CComVariant var = TimeSpan;
		HRESULT hr = var.ChangeType(VT_R8);
		if (FAILED(hr))
			return hr;
		dTimeSpan = var.dblVal;
	}
	else
	{
		pT->m_pTimeAxis->get_TimeSpan(&dTimeSpan);
	}

	if (!pT->m_bShowMutiTimeAxis)
	{
		pT->m_pTimeAxis->ScrollForward(COleDateTimeSpan(dTimeSpan));
		
		if (pT->m_enumTrendMode == TrendModeHistory)
			pT->OnRefreshData(TRUE);
	}
	else
	{
		int nCount = pT->m_apSerials.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			pT->m_apSerials[i]->ScrollForward(COleDateTimeSpan(dTimeSpan));
		}
	}

	pT->FireViewChange();
	

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::ScrollBack(/*[in, optional]*/ VARIANT TimeSpan)
{
	T* pT = static_cast<T*>(this);

	double dTimeSpan;
	if (TimeSpan.vt != VT_ERROR)
	{
		CComVariant var = TimeSpan;
		HRESULT hr = var.ChangeType(VT_R8);
		if (FAILED(hr))
			return hr;
		dTimeSpan = var.dblVal;
	}
	else
	{
		pT->m_pTimeAxis->get_TimeSpan(&dTimeSpan);
	}

	if (!pT->m_bShowMutiTimeAxis)
	{
		pT->m_pTimeAxis->ScrollBack(COleDateTimeSpan(dTimeSpan));

		if (pT->m_enumTrendMode == TrendModeHistory)
			pT->OnRefreshData(TRUE);
	}
	else
	{
		int nCount = pT->m_apSerials.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			pT->m_apSerials[i]->ScrollBack(COleDateTimeSpan(dTimeSpan));
		}
	}

	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::ScrollForwardMost()
{
	T* pT = static_cast<T*>(this);

	if (!pT->m_bShowMutiTimeAxis)
	{
		pT->m_pTimeAxis->ScrollForwardMost();

		if (pT->m_enumTrendMode == TrendModeHistory)
			pT->OnRefreshData(TRUE);
	}
	else
	{
		int nCount = pT->m_apSerials.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			pT->m_apSerials[i]->ScrollForwardMost();
		}
	}
	
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::Scroll(BOOL bHorizon, BOOL bUpOrRight)
{
	T* pT = static_cast<T*>(this);

	if (bHorizon && !pT->m_bShowMutiTimeAxis)
	{
		pT->m_pTimeAxis->ScrollPercentage(bUpOrRight, pT->m_fScrollPercentage);
		if (pT->m_enumTrendMode == TrendModeHistory)
			pT->OnRefreshData(TRUE);
	}
	else if (!bHorizon && !pT->m_bShowMutiValueAxis)
	{
		pT->m_pValueAxis->ScrollPercentage(bUpOrRight, pT->m_fScrollPercentage);
		if (pT->m_enumTrendMode == TrendModeHistory)
			pT->OnRefreshData(TRUE);
	}
	else
	{
		int nCount = pT->m_apSerials.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			pT->m_apSerials[i]->Scroll(bHorizon, bUpOrRight, pT->m_fScrollPercentage);
		}
	}
	
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::ScrollTo(VARIANT FromTime, /*[in, optional]*/ VARIANT ToTime)
{
	T* pT = static_cast<T*>(this);

	DATE dtFrom;
	CComVariant var = FromTime;
	HRESULT hr = var.ChangeType(VT_DATE);
	if (FAILED(hr))
		return hr;
	dtFrom = var.date;
	
	DATE dtTo;
	if (ToTime.vt != VT_ERROR)
	{
		CComVariant var = ToTime;
		HRESULT hr = var.ChangeType(VT_DATE);
		if (FAILED(hr))
			return hr;
		dtTo = var.date;
	}
	else
	{
		double dTimeSpan;
		pT->m_pTimeAxis->get_TimeSpan(&dTimeSpan);
		dtTo = dtFrom + dTimeSpan;
	}

	if (!pT->m_bShowMutiTimeAxis)
	{
		pT->m_pTimeAxis->ScrollTo(COleDateTime(dtFrom), COleDateTime(dtTo));

		if (pT->m_enumTrendMode == TrendModeHistory)
			pT->OnRefreshData(TRUE);
	}
	else
	{
		int nCount = pT->m_apSerials.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			pT->m_apSerials[i]->ScrollTo(COleDateTime(dtFrom), COleDateTime(dtTo));
		}
	}
	
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::Zoom(float fYHi, float fYLo, float fXHi, float fXLo)
{
	T* pT = static_cast<T*>(this);

	RECTF rectDest;
	rectDest.left = fXLo;
	rectDest.top = fYHi;
	rectDest.right = fXHi;
	rectDest.bottom = fYLo;
	
	RECTF rectSource;
	rectSource.left = (float)pT->m_rcSerial.left;
	rectSource.top = (float)pT->m_rcSerial.top;
	rectSource.right = (float)pT->m_rcSerial.right;
	rectSource.bottom = (float)pT->m_rcSerial.bottom;
	float fXZoom = pT->m_enumZoomMode == ZoomModeVertical ? 1 : (rectDest.right - rectDest.left) / (rectSource.right - rectSource.left);
	float fYZoom = pT->m_enumZoomMode == ZoomModeHorizontal ? 1 : (rectDest.bottom - rectDest.top) / (rectSource.bottom - rectSource.top);
	
	if (pT->m_enumZoomMode == ZoomModeHorizontal)
	{
//		m_wndToolPane.m_fScaleFactor *= fXZoom;	
		rectDest.top = rectSource.top;
		rectDest.bottom = rectSource.bottom;
	}
	else if (pT->m_enumZoomMode == ZoomModeVertical)
	{
//		m_wndToolPane.m_fScaleFactor *= fYZoom;	
		rectDest.left = rectSource.left;
		rectDest.right = rectSource.right;
	}
	else
	{
		//选用两者的大值放大
		if (fXZoom >= fYZoom)
		{
//			m_wndToolPane.m_fScaleFactor *= fXZoom;
			
			float fYCenter = (rectDest.top + rectDest.bottom) / 2;
			float fHeightHalf = fXZoom * (rectSource.bottom - rectSource.top) / 2;
			rectDest.top = fYCenter - fHeightHalf;
			rectDest.bottom = fYCenter + fHeightHalf;
		}
		else
		{
//			m_wndToolPane.m_fScaleFactor *= fYZoom;
			
			float fXCenter = (rectDest.left + rectDest.right) / 2;
			float fWidthHalf = fYZoom * (rectSource.right - rectSource.left) / 2;
			rectDest.left = fXCenter - fWidthHalf;
			rectDest.right = fXCenter + fWidthHalf;
		}
	}

	if (!pT->m_bShowMutiTimeAxis && !pT->m_bShowMutiValueAxis)
	{
		double dXMax = pT->m_pTimeAxis->GetValueForPos(rectDest.right);
		double dXMin = pT->m_pTimeAxis->GetValueForPos(rectDest.left);
		pT->m_pTimeAxis->Scale(dXMin, dXMax);

		double dYMax = pT->m_pValueAxis->GetValueForPos(rectDest.top);
		double dYMin = pT->m_pValueAxis->GetValueForPos(rectDest.bottom);
		pT->m_pValueAxis->Scale(dYMin, dYMax);

		if (pT->m_enumTrendMode == TrendModeHistory)
			pT->OnRefreshData(TRUE);
	}
	else if (!pT->m_bShowMutiTimeAxis && pT->m_bShowMutiValueAxis)
	{
		double dXMax = pT->m_pTimeAxis->GetValueForPos(rectDest.right);
		double dXMin = pT->m_pTimeAxis->GetValueForPos(rectDest.left);
		pT->m_pTimeAxis->Scale(dXMin, dXMax);

		int nCount = pT->m_apSerials.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			pT->m_apSerials[i]->Zoom(FALSE, rectDest.top, rectDest.bottom);
		}
	}
	else if (pT->m_bShowMutiTimeAxis && !pT->m_bShowMutiValueAxis)
	{
		double dYMax = pT->m_pValueAxis->GetValueForPos(rectDest.top);
		double dYMin = pT->m_pValueAxis->GetValueForPos(rectDest.bottom);
		pT->m_pValueAxis->Scale(dYMin, dYMax);

		int nCount = pT->m_apSerials.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			pT->m_apSerials[i]->Zoom(TRUE, rectDest.right, rectDest.left);
		}
	}
	else
	{
		// 每条画笔缩放
		int nCount = pT->m_apSerials.GetSize();
		for (int i = 0; i < nCount; i++)
		{
	//		pT->m_apSerials[i]->Zoom(fYHi, fYLo, fXHi, fXLo);
			pT->m_apSerials[i]->Zoom(rectDest.top, rectDest.bottom, rectDest.right, rectDest.left);
		}
	}

//	m_wndToolPane.UpdateZoomRadioEdit();

	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::ZoomIn()
{
	T* pT = static_cast<T*>(this);

	CRect rect = pT->m_rcSerial;
	return Zoom(rect.top + (float)rect.Height() / 4, rect.bottom - (float)rect.Height() / 4,
			rect.right - (float)rect.Width() / 4, rect.left + (float)rect.Width() / 4);
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::ZoomOut()
{
	T* pT = static_cast<T*>(this);

	CRect rect = pT->m_rcSerial;
	return Zoom(rect.top - (float)rect.Height() / 2, rect.bottom + (float)rect.Height() / 2,
		rect.right + (float)rect.Width() / 2, rect.left - (float)rect.Width() / 2);
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_TrendMode(enumTrendMode *pVal)
{
	T* pT = static_cast<T*>(this);

	*pVal = pT->m_enumTrendMode;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_TrendMode(enumTrendMode newVal)
{
	T* pT = static_cast<T*>(this);

	if (pT->m_enumTrendMode == newVal)
		return S_OK;

	pT->m_enumTrendMode = newVal;
//	m_wndToolPane.UpdateToolBar();

	for (int i = 0; i < pT->m_apSerials.GetSize(); i++)
	{
		CSerial* pSerial = pT->m_apSerials[i];
		pSerial->RemoveAllValues();
	}

	pT->OnRefreshData(TRUE);

	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_Caption(BSTR *pVal)
{
	T* pT = static_cast<T*>(this);
	
	*pVal = pT->m_bstrTitle.Copy();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_Caption(BSTR newVal)
{
	T* pT = static_cast<T*>(this);

	pT->m_bstrTitle = newVal;
	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_BackColor(OLE_COLOR *pVal)
{
	T* pT = static_cast<T*>(this);

	*pVal = pT->m_clrBack;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_BackColor(OLE_COLOR newVal)
{
	T* pT = static_cast<T*>(this);

	pT->m_clrBack = newVal;
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_ForeColor(OLE_COLOR *pVal)
{
	T* pT = static_cast<T*>(this);
	
	*pVal = pT->m_clrFore;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_ForeColor(OLE_COLOR newVal)
{
	T* pT = static_cast<T*>(this);

	pT->m_clrFore = newVal;
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_CaptionFont(IFontDisp** ppFont)
{
	T* pT = static_cast<T*>(this);
	
	*ppFont = pT->m_spCaptionFont;
	if (*ppFont != NULL)
		(*ppFont)->AddRef();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_CaptionFont(IFontDisp* pFont)
{
	T* pT = static_cast<T*>(this);
	
	if (pFont)
	{
		CComQIPtr<IFont, &IID_IFont> p(pFont);
		if (p)
		{
			// 如果想等即返回
			CComQIPtr<IFont, &IID_IFont> pOld(pT->m_spCaptionFont);
			if (p->IsEqual(pOld) == S_OK)
				return S_OK;
			
			pT->m_spCaptionFont = 0;
			
			CComPtr<IFont> pFont;
			p->Clone(&pFont);
			if (pFont)
				pFont->QueryInterface(IID_IFontDisp, (void**) &pT->m_spCaptionFont);
		}
	}
	else
	{
		if (pT->m_spCaptionFont == NULL)
			return S_OK;
		
		pT->m_spCaptionFont = 0;
	}
	
	pT->RecalcRects();
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::putref_CaptionFont(IFontDisp* pFont)
{
	T* pT = static_cast<T*>(this);
	
	pT->m_spCaptionFont = pFont;
	pT->RecalcRects();
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_CaptionColor(OLE_COLOR *pVal)
{
	T* pT = static_cast<T*>(this);
	
	*pVal = pT->m_clrCaption;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_CaptionColor(OLE_COLOR newVal)
{
	T* pT = static_cast<T*>(this);
	
	pT->m_clrCaption = newVal;
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_RunMode(/*[out, retval]*/ VARIANT_BOOL *pVal)
{
	T* pT = static_cast<T*>(this);
	*pVal = pT->ClientIsRunMode() ? VARIANT_TRUE : VARIANT_FALSE;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_TimeAxis(/*[out, retval]*/ ITimeAxis* *pVal)
{
	T* pT = static_cast<T*>(this);

	ATLASSERT(pT->m_pTimeAxis != NULL);
	pT->m_pTimeAxis->QueryInterface(IID_ITimeAxis, (void **)pVal);

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_ValueAxis(/*[out, retval]*/ IValueAxis * *pVal)
{
	T* pT = static_cast<T*>(this);
	
	ATLASSERT(pT->m_pValueAxis != NULL);
	pT->m_pValueAxis->QueryInterface(IID_IValueAxis, (void **)pVal);
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CTrendDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_Grid(/*[out, retval]*/ IGrid* *pVal)
{
	T* pT = static_cast<T*>(this);
	
	ATLASSERT(pT->m_pGrid != NULL);
	pT->m_pGrid->QueryInterface(IID_IGrid, (void **)pVal);
	
	return S_OK;
}

#endif // __TRENDDISPATCHIMPL_H_
