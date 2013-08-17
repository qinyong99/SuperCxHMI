// opchda.h : interface of the CWTSvrTestView class

#ifndef __OPCHDAIMPL_H_
#define __OPCHDAIMPL_H_

template<class T>
class ATL_NO_VTABLE IOPCHDA_ServerImpl : public IOPCHDA_Server
{
public:
	STDMETHOD(GetItemAttributes)(
		DWORD	*pdwCount,
		DWORD	**ppdwAttrID,
		LPWSTR	**ppszAttrName,
		LPWSTR	**ppszAttrDesc,
		VARTYPE	**ppvtAttrDataType)
	{
		return S_OK;
	}
	STDMETHOD(GetAggregates)(
		DWORD	*pdwCount,
		DWORD	**ppdwAggrID,
		LPWSTR	**ppszAggrName,
		LPWSTR	**ppszAggrDesc)
	{
		return S_OK;
	}
	STDMETHOD(GetHistorianStatus)(
		OPCHDA_SERVERSTATUS		*pwStatus,
		FILETIME				**pftCurrentTime,
		FILETIME				**pftStartTime,
		WORD					*pwMajorVersion,
		WORD					*pwMinorVersion,
		WORD					*pwBuildNumber,
		DWORD					*pdwMaxReturnValues,
		LPWSTR					*ppszStatusString,
		LPWSTR					*ppszVendorInfo)
	{
		return S_OK;
	}
	STDMETHOD(GetItemHandles)(
		DWORD		dwCount,
		LPWSTR		*pszItemID,
		OPCHANDLE	*phClient,
		OPCHANDLE	**pphServer,
		HRESULT		**ppErrors)
	{
		return S_OK;
	}
	STDMETHOD(ReleaseItemHandles)(
		DWORD		dwCount,
		OPCHANDLE	*phServer,
		HRESULT		**ppErrors)
	{
		return S_OK;
	}
	STDMETHOD(ValidateItemIDs)(
		DWORD	dwCount,
		LPWSTR	*pszItemID,
		HRESULT **ppErrors)
	{
		return S_OK;
	}
	STDMETHOD(CreateBrowse)(
		DWORD					dwCount,
		DWORD					*pdwAttrID,
		OPCHDA_OPERATORCODES	*pOperator,
		VARIANT					*vFilter,
		IOPCHDA_Browser			**pphBrowser,
		HRESULT					**ppErrors)
	{
		return S_OK;
	}
};

template<class T>
class ATL_NO_VTABLE IOPCHDA_SyncReadImpl : public IOPCHDA_SyncRead
{
public:
	STDMETHOD(ReadRaw)(
		OPCHDA_TIME *htStartTime,
		OPCHDA_TIME *htEndTime,
		DWORD	dwNumValues,
		BOOL	bBounds,
		DWORD	dwNumItems,
		OPCHANDLE * phServer, 
		OPCHDA_ITEM ** ppItemValues,
		HRESULT ** ppErrors)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE ReadProcessed( 
            OPCHDA_TIME *htStartTime,
            OPCHDA_TIME *htEndTime,
            FILETIME ftResampleInterval,
            DWORD dwNumItems,
            OPCHANDLE *phServer,
            OPCHDA_AGGREGATE *haAggregate,
            OPCHDA_ITEM **ppItemValues,
            HRESULT **ppErrors)
	{
		return S_OK;
	}
        
	STDMETHOD(ReadAtTime)(
		DWORD dwNumTimeStamps,
		FILETIME *ftTimeStamps,
		DWORD dwNumItems,
		OPCHANDLE * phServer, 
		OPCHDA_ITEM ** ppItemValues,
		HRESULT ** ppErrors)
	{
		return S_OK;
	}

	STDMETHOD(ReadModified)(
		OPCHDA_TIME *htStartTime,
		OPCHDA_TIME *htEndTime,
		DWORD dwNumValues,
		DWORD dwNumItems,
		OPCHANDLE				*phServer, 
		OPCHDA_MODIFIEDITEM		** ppItemValues,
		HRESULT					**ppErrors)
	{
		return S_OK;
	}

	STDMETHOD(ReadAttribute)(
		OPCHDA_TIME *htStartTime,
		OPCHDA_TIME *htEndTime,
		OPCHANDLE hServer, 
		DWORD dwNumAttributes,
		DWORD				*pdwAttributeIDs, 
		OPCHDA_ATTRIBUTE	**ppAttributeValues,
		HRESULT				**ppErrors)
	{
		return S_OK;
	}
};

#endif //__OPCHDAIMPL_H_