// opcdaimpl.h : interface of the CWTSvrTestView class

#ifndef __OPCDAIMPL_H_
#define __OPCDAIMPL_H_

template<class T>
class ATL_NO_VTABLE IOPCBrowseServerAddressSpaceImpl : public IOPCBrowseServerAddressSpace
{
public:
	STDMETHOD(QueryOrganization)(
		OPCNAMESPACETYPE* pNameSpaceType)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(ChangeBrowsePosition)(
		OPCBROWSEDIRECTION dwBrowseDirection,
		LPCWSTR szString)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(BrowseOPCItemIDs)(
		OPCBROWSETYPE dwBrowseFilterType,
		LPCWSTR szFilterCriteria,  
		VARTYPE vtDataTypeFilter,     
		DWORD dwAccessRightsFilter,
		LPENUMSTRING* ppIEnumString)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetItemID)(
		LPWSTR szItemDataID,
		LPWSTR* szItemID)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(BrowseAccessPaths)(
		LPCWSTR szItemID,
		LPENUMSTRING* ppIEnumString)
	{
		return E_NOTIMPL;
	}
};



#endif //__OPCDAIMPL_H_