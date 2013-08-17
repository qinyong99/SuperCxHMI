// AlmSumDispatchImpl.h: interface for the CAlmSumDispatchImpl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ALMSUMDISPATCHIMPL_H_
#define __ALMSUMDISPATCHIMPL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CxAlmSumCtrl.h"

template <class T, class IT, const IID* piid, const GUID* plibid = &CComModule::m_libid, WORD wMajor = 1,
WORD wMinor = 0, class tihclass = CComTypeInfoHolder>
class ATL_NO_VTABLE CAlmSumDispatchImpl : public IDispatchImpl<IT, piid, plibid, wMajor, wMinor, tihclass>
{
public:
	CAlmSumDispatchImpl();
	virtual ~CAlmSumDispatchImpl();

public:
	STDMETHOD(get_BackColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_BackColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_HeaderForeColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_HeaderForeColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_HeaderBackColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_HeaderBackColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_ShowGrid)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowGrid)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_AutoScroll)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_AutoScroll)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ShowScrollBar)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowScrollBar)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ShowHeader)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ShowHeader)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_SortOrder)(/*[out, retval]*/ enumSortOrder *pVal);
	STDMETHOD(put_SortOrder)(/*[in]*/ enumSortOrder newVal);
	STDMETHOD(get_SecondSortColumn)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_SecondSortColumn)(/*[in]*/ long newVal);
	STDMETHOD(get_SortColumn)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_SortColumn)(/*[in]*/ long newVal);
	STDMETHOD(get_PriorityFilterList)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_PriorityFilterList)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_AreaFilterList)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_AreaFilterList)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_NodeFilterList)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_NodeFilterList)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_AckEnabled)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_AckEnabled)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ContextMenuEnabled)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ContextMenuEnabled)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_OnlineConfigureEnabled)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_OnlineConfigureEnabled)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(put_HeaderFont)(IFontDisp* pFont);
	STDMETHOD(putref_HeaderFont)(IFontDisp* pFont);
	STDMETHOD(get_HeaderFont)(IFontDisp** ppFont);
	STDMETHOD(get_RowHeight)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_RowHeight)(/*[in]*/ long newVal);
	STDMETHOD(get_RunMode)(/*[out, retval]*/ VARIANT_BOOL *pVal);
		
	STDMETHOD(SetStatusFont)(/*[in]*/ long lStatus, /*[out, retval]*/ IFontDisp* pFont);
	STDMETHOD(GetStatusFont)(/*[in]*/ long lStatus, /*[out, retval]*/ IFontDisp** ppFont);
	STDMETHOD(GetPriorityLabel)(/*[in]*/ long lPrioritys, /*[out, retval]*/ BSTR* pbstrLabel);
	STDMETHOD(GetPriorityCount)(/*[out, retval]*/ long* plCount);
	STDMETHOD(GetStatusLabel)(/*[in]*/ long lStatus, /*[out, retval]*/ BSTR* pbstrLabel);
	STDMETHOD(GetStatusCount)(/*[out, retval]*/ long* plCount);
	STDMETHOD(GetPriorityColor)(/*[in]*/ long lPriority, /*[out, retval]*/ OLE_COLOR* pclrColor);
	STDMETHOD(SetPriorityColor)(/*[in]*/ long lPriority, /*[in]*/ OLE_COLOR clrColor);
	STDMETHOD(GetStatusColor)(/*[in]*/ long lStatus, /*[out, retval]*/ OLE_COLOR* pclrColor);
	STDMETHOD(SetStatusColor)(/*[in]*/ long lStatus, /*[in]*/ OLE_COLOR clrColor);
	STDMETHOD(GetShowColumn)(/*[in]*/ long lIndex, /*[out]*/ long* plColumn);
	STDMETHOD(GetShowColumnCount)(/*[in]*/ long* plCount);
	STDMETHOD(ShowColumn)(/*[in]*/ long lColumn, /*[in]*/ BOOL bShow);
	STDMETHOD(IsColumnShow)(/*[in]*/ long lColumn);
	STDMETHOD(GetColumnLabel)(/*[int]*/ long lColumn, /*[out]*/ BSTR* pbstrLabel);
	STDMETHOD(GetColumnCount)(/*[out]*/ long* plCount);
	STDMETHOD(Sort)();
	STDMETHOD(EditProperties)();
	STDMETHOD(AcknowledgeAllAlarms)();
	STDMETHOD(AcknowledgePageAlarms)();
};

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>()
{
	
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::~CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>()
{
	
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::GetColumnCount(long *plCount)
{
	*plCount = NUM_FIELDS;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::GetColumnLabel(long lColumn, BSTR *pbstrLabel)
{
	USES_CONVERSION;
	*pbstrLabel = ::SysAllocString(T2COLE(_gszColumnLabel[lColumn]));

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::IsColumnShow(long lColumn)
{
	T* pT = static_cast<T*>(this);

	int nCount = pT->m_apColumns.GetSize();
	CColumn* pCol;
	for (int i = 1; i < nCount; i++)
	{
		pCol = pT->m_apColumns[i];
		if (pCol->m_enumAlarmField == (enumAlarmField)lColumn)
			return S_OK;
	}

	return S_FALSE;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::ShowColumn(long lColumn, BOOL bShow)
{
	T* pT = static_cast<T*>(this);

	if (bShow)
	{
		if (pT->IsColumnShow(lColumn) == S_OK)
			return S_OK;

		CColumn* pCol = new CColumn((enumAlarmField)lColumn);
		pT->m_apColumns.Add(pCol);
		pT->RecalIdealColumnWidth(pCol);
	}
	else
	{
		int nCount = pT->m_apColumns.GetSize();
		CColumn* pCol;
		for (int i = 0; i < nCount; i++)
		{
			pCol = pT->m_apColumns[i];
			if (pCol->m_enumAlarmField == (enumAlarmField)lColumn)
			{
				delete pT->m_apColumns[i];
				pT->m_apColumns.RemoveAt(i);
				break;
			}
		}
	}

	pT->UpdateScrollbarVer(TRUE, FALSE);
	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::GetShowColumnCount(long *plCount)
{
	T* pT = static_cast<T*>(this);

	*plCount = pT->m_apColumns.GetSize();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::GetShowColumn(long lIndex, long *plColumn)
{
	T* pT = static_cast<T*>(this);

	*plColumn = pT->m_apColumns[lIndex]->m_enumAlarmField;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::SetStatusColor(long lStatus, OLE_COLOR clrColor)
{
	T* pT = static_cast<T*>(this);

	pT->m_apForeFont[lStatus]->m_clr = clrColor;
	for (int i = 0; i < (int)pT->m_apAlarms.GetCount(); i ++)
	{
		pT->m_apAlarms[i]->UpdateDrawStyle();
	}
	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::GetStatusColor(long lStatus, OLE_COLOR *pclrColor)
{
	T* pT = static_cast<T*>(this);
	*pclrColor = pT->m_apForeFont[lStatus]->m_clr;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::SetPriorityColor(long lPriority, OLE_COLOR clrColor)
{
	T* pT = static_cast<T*>(this);

	pT->m_apBackColor[lPriority]->m_clr = clrColor;
	for (int i = 0; i < (int)pT->m_apAlarms.GetCount(); i ++)
	{
		pT->m_apAlarms[i]->UpdateDrawStyle();
	}
	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::GetPriorityColor(long lPriority, OLE_COLOR *pclrColor)
{
	T* pT = static_cast<T*>(this);
	
	*pclrColor = pT->m_apBackColor[lPriority]->m_clr;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::GetStatusCount(long *plCount)
{
	*plCount = NUM_STATUSS;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::GetStatusLabel(long lStatus, BSTR *pbstrLabel)
{
	USES_CONVERSION;
	*pbstrLabel = ::SysAllocString(T2COLE(_gszStatusLabel[lStatus]));

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::GetPriorityCount(long *plCount)
{
	*plCount = NUM_PRIORITYS;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::GetPriorityLabel(long lPrioritys, BSTR *pbstrLabel)
{
	T* pT = static_cast<T*>(this);

	USES_CONVERSION;
	*pbstrLabel = ::SysAllocString(T2COLE(_gszPriorityLabel[lPrioritys]));

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::GetStatusFont(long lStatus, IFontDisp **ppFont)
{
	T* pT = static_cast<T*>(this);

	ATLASSERT(pT->m_apForeFont[lStatus]->m_spFont != NULL);
	pT->m_apForeFont[lStatus]->m_spFont.CopyTo(ppFont);

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::SetStatusFont(long lStatus, IFontDisp *pFont)
{
	T* pT = static_cast<T*>(this);

	pT->m_apForeFont[lStatus]->m_spFont = pFont;
	for (int i = 0; i < (int)pT->m_apAlarms.GetCount(); i ++)
	{
		pT->m_apAlarms[i]->UpdateDrawStyle();
	}
	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_ShowScrollBar(VARIANT_BOOL *pVal)
{
	T* pT = static_cast<T*>(this);
	*pVal = pT->m_bShowScrollBar;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::AcknowledgeAllAlarms()
{
	T* pT = static_cast<T*>(this);
	pT->AckAllAlarms();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::AcknowledgePageAlarms()
{
	T* pT = static_cast<T*>(this);
	pT->AckPageAlarms();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_ShowScrollBar(VARIANT_BOOL newVal)
{
	T* pT = static_cast<T*>(this);

	if (pT->m_bShowScrollBar == newVal)
		return S_OK;

	pT->m_bShowScrollBar = newVal;
	pT->RecalcRects();
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_ShowHeader(VARIANT_BOOL *pVal)
{
	T* pT = static_cast<T*>(this);
	*pVal = pT->m_bShowHeader;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_ShowHeader(VARIANT_BOOL newVal)
{
	T* pT = static_cast<T*>(this);

	if (pT->m_bShowHeader == newVal)
		return S_OK;

	pT->m_bShowHeader = newVal;
	pT->RecalcRects();
	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_AutoScroll(VARIANT_BOOL *pVal)
{
	T* pT = static_cast<T*>(this);
	*pVal = pT->m_bAutoScroll;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_AutoScroll(VARIANT_BOOL newVal)
{
	T* pT = static_cast<T*>(this);
	pT->m_bAutoScroll = newVal;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_ShowGrid(VARIANT_BOOL *pVal)
{
	T* pT = static_cast<T*>(this);
	
	*pVal = pT->m_bShowGrid;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_ShowGrid(VARIANT_BOOL newVal)
{
	T* pT = static_cast<T*>(this);

	if (pT->m_bShowGrid == newVal)
		return S_OK;

	pT->m_bShowGrid = newVal;
	pT->FireViewChange();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_SortOrder(/*[out, retval]*/ enumSortOrder *pVal)
{
	T* pT = static_cast<T*>(this);
	*pVal = pT->m_bSortAscending ? SortOrderAscending : SortOrderDescending;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_SortOrder(/*[in]*/ enumSortOrder newVal)
{
	T* pT = static_cast<T*>(this);
	pT->m_bSortAscending = (newVal == SortOrderAscending);

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_SecondSortColumn(/*[out, retval]*/ long *pVal)
{
	T* pT = static_cast<T*>(this);
	*pVal = pT->m_nSecondSortField;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_SecondSortColumn(/*[in]*/ long newVal)
{
	T* pT = static_cast<T*>(this);
	pT->m_nSecondSortField = (enumAlarmField)newVal;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_SortColumn(/*[out, retval]*/ long *pVal)
{
	T* pT = static_cast<T*>(this);
	*pVal = pT->m_nSortField;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_SortColumn(/*[in]*/ long newVal)
{
	T* pT = static_cast<T*>(this);
	pT->m_nSortField = (enumAlarmField)newVal;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::Sort()
{
	T* pT = static_cast<T*>(this);
	CAlarm* pSelectAlarm = pT->GetSelectAlarm();
	pT->ReSortAlarms();
	pT->SelectAlarm(pSelectAlarm);

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_PriorityFilterList(/*[out, retval]*/ BSTR *pVal)
{
	T* pT = static_cast<T*>(this);
	*pVal = pT->m_bstrPriorityFilter.Copy();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_PriorityFilterList(/*[in]*/ BSTR newVal)
{
	T* pT = static_cast<T*>(this);

	if (pT->m_bstrPriorityFilter == newVal)
		return S_OK;

	pT->m_bstrPriorityFilter = newVal;
	pT->ReFilterAlarms();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_AreaFilterList(/*[out, retval]*/ BSTR *pVal)
{
	T* pT = static_cast<T*>(this);
	*pVal = pT->m_bstrAreaFilter.Copy();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_AreaFilterList(/*[in]*/ BSTR newVal)
{
	T* pT = static_cast<T*>(this);

	if (pT->m_bstrAreaFilter == newVal)
		return S_OK;

	pT->m_bstrAreaFilter = newVal;
	pT->ReFilterAlarms();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_NodeFilterList(/*[out, retval]*/ BSTR *pVal)
{
	T* pT = static_cast<T*>(this);
	*pVal = pT->m_bstrNodeFilter.Copy();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_NodeFilterList(/*[in]*/ BSTR newVal)
{
	T* pT = static_cast<T*>(this);

	if (pT->m_bstrNodeFilter == newVal)
		return S_OK;

	pT->m_bstrNodeFilter = newVal;
	pT->ReFilterAlarms();

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_AckEnabled(/*[out, retval]*/ VARIANT_BOOL *pVal)
{
	T* pT = static_cast<T*>(this);
	*pVal = pT->m_bEnableAck;

	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_AckEnabled(/*[in]*/ VARIANT_BOOL newVal)
{
	T* pT = static_cast<T*>(this);
	pT->m_bEnableAck = newVal;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_ContextMenuEnabled(/*[out, retval]*/ VARIANT_BOOL *pVal)
{
	T* pT = static_cast<T*>(this);
	*pVal = pT->m_bEnableContextMenu;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_ContextMenuEnabled(/*[in]*/ VARIANT_BOOL newVal)
{
	T* pT = static_cast<T*>(this);
	pT->m_bEnableContextMenu = newVal;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_OnlineConfigureEnabled(/*[out, retval]*/ VARIANT_BOOL *pVal)
{
	T* pT = static_cast<T*>(this);
	*pVal = pT->m_bEnableOnlineConfigure;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_OnlineConfigureEnabled(/*[in]*/ VARIANT_BOOL newVal)
{
	T* pT = static_cast<T*>(this);
	pT->m_bEnableOnlineConfigure = newVal;
		
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::EditProperties()
{
	T* pT = static_cast<T*>(this);
	return pT->OnEditProperties();
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_BackColor(OLE_COLOR *pVal)
{
	T* pT = static_cast<T*>(this);
	
	*pVal = pT->m_clrBack;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_BackColor(OLE_COLOR newVal)
{
	T* pT = static_cast<T*>(this);
	
	pT->m_clrBack = newVal;
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_HeaderBackColor(OLE_COLOR *pVal)
{
	T* pT = static_cast<T*>(this);
	
	*pVal = pT->m_clrHeaderBack;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_HeaderBackColor(OLE_COLOR newVal)
{
	T* pT = static_cast<T*>(this);
	
	pT->m_clrHeaderBack = newVal;
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_HeaderForeColor(OLE_COLOR *pVal)
{
	T* pT = static_cast<T*>(this);
	
	*pVal = pT->m_clrHeaderFore;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_HeaderForeColor(OLE_COLOR newVal)
{
	T* pT = static_cast<T*>(this);
	
	pT->m_clrHeaderFore = newVal;
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_HeaderFont(IFontDisp** ppFont)
{
	T* pT = static_cast<T*>(this);
	
	*ppFont = pT->m_spHeaderFont;
	if (*ppFont != NULL)
		(*ppFont)->AddRef();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_HeaderFont(IFontDisp* pFont)
{
	T* pT = static_cast<T*>(this);
	
	if (pFont)
	{
		CComQIPtr<IFont, &IID_IFont> p(pFont);
		if (p)
		{
			// 如果想等即返回
			CComQIPtr<IFont, &IID_IFont> pOld(pT->m_spHeaderFont);
			if (p->IsEqual(pOld) == S_OK)
				return S_OK;
			
			pT->m_spHeaderFont = 0;
			
			CComPtr<IFont> pFont;
			p->Clone(&pFont);
			if (pFont)
				pFont->QueryInterface(IID_IFontDisp, (void**) &pT->m_spHeaderFont);
		}
	}
	else
	{
		if (pT->m_spHeaderFont == NULL)
			return S_OK;
		
		pT->m_spHeaderFont = 0;
	}
	
	pT->RecalIdealColumnWidth();
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::putref_HeaderFont(IFontDisp* pFont)
{
	T* pT = static_cast<T*>(this);
	
	pT->m_spHeaderFont = pFont;
	pT->RecalIdealColumnWidth();
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_RowHeight(long *pVal)
{
	T* pT = static_cast<T*>(this);
	
	*pVal = pT->m_lRowHeight;
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::put_RowHeight(long newVal)
{
	T* pT = static_cast<T*>(this);
	
	if (newVal < 1)
		return E_INVALIDARG;

	pT->m_lRowHeight = newVal;
	pT->UpdateScrollbarVer(FALSE, TRUE);
	pT->FireViewChange();
	
	return S_OK;
}

template <class T, class IT, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
STDMETHODIMP CAlmSumDispatchImpl<T, IT, piid, plibid, wMajor, wMinor, tihclass>::get_RunMode(/*[out, retval]*/ VARIANT_BOOL *pVal)
{
	T* pT = static_cast<T*>(this);
	*pVal = pT->ClientIsRunMode() ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

#endif // __ALMSUMDISPATCHIMPL_H_
