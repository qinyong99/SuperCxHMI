// opc_aeImpl.h : interface of the CWTSvrTestView class

#ifndef __OPC_AEIMPL_H_
#define __OPC_AEIMPL_H_

template<class T>
class ATL_NO_VTABLE IOPCEventServerImpl : public IOPCEventServer
{
public:
	STDMETHOD(GetStatus)(
			OPCEVENTSERVERSTATUS **ppEventServerStatus)
	{
		return S_OK;
	}
	STDMETHOD(CreateEventSubscription)(
			BOOL bActive,
			DWORD dwBufferTime, 
			DWORD dwMaxSize,
			OPCHANDLE hClientSubscription,
			REFIID riid,
			LPUNKNOWN * ppUnk,
			DWORD *pdwRevisedBufferTime,
			DWORD *pdwRevisedMaxSize)
	{
		return S_OK;
	}
	STDMETHOD(QueryAvailableFilters)(
			DWORD* pdwFilterMask)
	{
		return S_OK;
	}
	STDMETHOD(QueryEventCategories)(
			DWORD  dwEventType,	
			DWORD* pdwCount, 
			DWORD** ppdwEventCategories,
			LPWSTR** ppszEventCategoryDescs)
	{
		return S_OK;
	}
	STDMETHOD(QueryConditionNames)(
			DWORD  dwEventCategory, 
			DWORD* pdwCount, 
			LPWSTR** ppszConditionNames)
	{
		return S_OK;
	}
	STDMETHOD(QuerySubConditionNames)(
			LPWSTR szConditionName, 
			DWORD* pdwCount, 
			LPWSTR** ppszSubConditionNames)
	{
		return S_OK;
	}
	STDMETHOD(QuerySourceConditions)(
			LPWSTR szSource, 
			DWORD* pdwCount, 
			LPWSTR** ppszConditionNames)
	{
		return S_OK;
	}
	STDMETHOD(QueryEventAttributes)(
			DWORD dwEventCategory, 
			DWORD* pdwCount, 
			DWORD** ppdwAttrIDs,
			LPWSTR** ppszAttrDescs,
			VARTYPE** ppvtAttrTypes)
	{
		return S_OK;
	}
	STDMETHOD(TranslateToItemIDs)(
			LPWSTR szSource,
			DWORD dwEventCategory,
 			LPWSTR szConditionName,
 			LPWSTR szSubconditionName,
			DWORD  dwCount, 
			DWORD* pdwAssocAttrIDs, 
			LPWSTR** ppszAttrItemIDs,
			LPWSTR** ppszNodeNames,
			CLSID** ppCLSIDs)
	{
		return S_OK;
	}
	STDMETHOD(GetConditionState)(
			LPWSTR szSource,
			LPWSTR szConditionName,
			DWORD dwNumEventAttrs,
			DWORD* pdwAttributeIDs,
			OPCCONDITIONSTATE ** ppConditionState)
	{
		return S_OK;
	}
	STDMETHOD(EnableConditionByArea)(
			DWORD dwNumAreas, 
			LPWSTR* pszAreas)
	{
		return S_OK;
	}
	STDMETHOD(EnableConditionBySource)(
			DWORD dwNumSources, 
			LPWSTR* pszSources)
	{
		return S_OK;
	}
	STDMETHOD(DisableConditionByArea)(
			DWORD dwNumAreas, 
			LPWSTR* pszAreas)
	{
		return S_OK;
	}
	STDMETHOD(DisableConditionBySource)(
			DWORD dwNumSources, 
			LPWSTR* pszSources)
	{
		return S_OK;
	}
	STDMETHOD(AckCondition)(
			DWORD dwCount,
			LPWSTR szAcknowledgerID ,
			LPWSTR szComment ,
			LPWSTR* pszSource,
			LPWSTR* pszConditionName,
			FILETIME* pftActiveTime,
			DWORD* pdwCookie,
			HRESULT **ppErrors)
	{
		return S_OK;
	}
	STDMETHOD(CreateAreaBrowser)(
			REFIID riid,
			LPUNKNOWN* ppUnk)
	{
		return S_OK;
	}

};

template<class T>
class ATL_NO_VTABLE IOPCEventSubscriptionMgtImpl : public IOPCEventSubscriptionMgt
{
public:
	STDMETHOD(SetFilter)(
			DWORD  dwEventType, 
			DWORD dwNumCategories,		
			DWORD* pdwEventCategories, 
			DWORD dwLowSeverity,
			DWORD dwHighSeverity,
			DWORD dwNumAreas,		
			LPWSTR* pszAreaList,
			DWORD dwNumSources,
			LPWSTR* pszSourceList)
	{
		return S_OK;
	}
	STDMETHOD(GetFilter)(
			DWORD* pdwEventType, 
			DWORD* pdwNumCategories,		
			DWORD** ppdwEventCategories, 
			DWORD* pdwLowSeverity,
			DWORD* pdwHighSeverity,
			DWORD* pdwNumAreas,		
			LPWSTR** ppszAreaList,
			DWORD* pdwNumSources,
			LPWSTR** ppszSourceList)
	{
		return S_OK;
	}
	STDMETHOD(SelectReturnedAttributes)(
			DWORD dwEventCategory,		
			DWORD dwCount,		
			DWORD* dwAttributeIDs)
	{
		return S_OK;
	}
	STDMETHOD(GetReturnedAttributes)(
			DWORD dwEventCategory,		
			DWORD * pdwCount,		
			DWORD** ppdwAttributeIDs)
	{
		return S_OK;
	}
	STDMETHOD(Refresh)(
			DWORD dwConnection)
	{
		return S_OK;
	}
	STDMETHOD(CancelRefresh)(
			DWORD dwConnection)
	{
		return S_OK;
	}
	STDMETHOD(GetState)(
			BOOL	* pbActive, 
			DWORD * pdwBufferTime,  
			DWORD * pdwMaxSize,
			OPCHANDLE * phClientSubscription)
	{
		return S_OK;
	}
	STDMETHOD(SetState)(
			BOOL *  pbActive, 
			DWORD * pdwBufferTime,
			DWORD * pdwMaxSize,
			OPCHANDLE hClientSubscription,
			DWORD * pdwRevisedBufferTime,
			DWORD * pdwRevisedMaxSize)
	{
		return S_OK;
	}

};

/*
template <class T>
class ATL_NO_VTABLE IOPCEventSinkCP :
	public IConnectionPointImpl<T, &IID_IOPCEventSink, CComDynamicUnkArray>
{
public:
	void Fire_OPCEvent(
			BOOL bRefresh,
			BOOL bLastRefresh,
			DWORD dwCount,
			ONEVENTSTRUCT* pEvents)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IOPCEventSink* pSink = reinterpret_cast<IOPCEventSink*>(sp.p);
			if (pSink != NULL)
			{
				pSink->OnEvent(pT->m_hClientSubscription, bRefresh, bLastRefresh, dwCount, pEvents);
			}
		}
	}

	void Fire_OPCEvent(
			DWORD dwConnection, 
			BOOL bRefresh,
			BOOL bLastRefresh,
			DWORD dwCount,
			ONEVENTSTRUCT* pEvents)
	{
		T* pT = static_cast<T*>(this);
		
		pT->Lock();
		CComPtr<IUnknown> sp = (IUnknown *)dwConnection;
		pT->Unlock();
		IOPCEventSink* pSink = reinterpret_cast<IOPCEventSink*>(sp.p);
		if (pSink != NULL)
		{
			pSink->OnEvent(pT->m_hClientSubscription, bRefresh, bLastRefresh, dwCount, pEvents);
		}
	}
};
*/

#endif //__OPC_AEIMPL_H_
