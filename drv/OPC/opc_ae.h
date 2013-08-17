/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Jun 03 11:08:36 1999
 */
/* Compiler settings for opc_ae.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __opc_ae_h__
#define __opc_ae_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IOPCEventServer_FWD_DEFINED__
#define __IOPCEventServer_FWD_DEFINED__
typedef interface IOPCEventServer IOPCEventServer;
#endif 	/* __IOPCEventServer_FWD_DEFINED__ */


#ifndef __IOPCEventSubscriptionMgt_FWD_DEFINED__
#define __IOPCEventSubscriptionMgt_FWD_DEFINED__
typedef interface IOPCEventSubscriptionMgt IOPCEventSubscriptionMgt;
#endif 	/* __IOPCEventSubscriptionMgt_FWD_DEFINED__ */


#ifndef __IOPCEventAreaBrowser_FWD_DEFINED__
#define __IOPCEventAreaBrowser_FWD_DEFINED__
typedef interface IOPCEventAreaBrowser IOPCEventAreaBrowser;
#endif 	/* __IOPCEventAreaBrowser_FWD_DEFINED__ */


#ifndef __IOPCEventSink_FWD_DEFINED__
#define __IOPCEventSink_FWD_DEFINED__
typedef interface IOPCEventSink IOPCEventSink;
#endif 	/* __IOPCEventSink_FWD_DEFINED__ */


#ifndef __IOPCEventServer_FWD_DEFINED__
#define __IOPCEventServer_FWD_DEFINED__
typedef interface IOPCEventServer IOPCEventServer;
#endif 	/* __IOPCEventServer_FWD_DEFINED__ */


#ifndef __IOPCEventSubscriptionMgt_FWD_DEFINED__
#define __IOPCEventSubscriptionMgt_FWD_DEFINED__
typedef interface IOPCEventSubscriptionMgt IOPCEventSubscriptionMgt;
#endif 	/* __IOPCEventSubscriptionMgt_FWD_DEFINED__ */


#ifndef __IOPCEventAreaBrowser_FWD_DEFINED__
#define __IOPCEventAreaBrowser_FWD_DEFINED__
typedef interface IOPCEventAreaBrowser IOPCEventAreaBrowser;
#endif 	/* __IOPCEventAreaBrowser_FWD_DEFINED__ */


#ifndef __IOPCEventSink_FWD_DEFINED__
#define __IOPCEventSink_FWD_DEFINED__
typedef interface IOPCEventSink IOPCEventSink;
#endif 	/* __IOPCEventSink_FWD_DEFINED__ */


#ifndef __OPCEventServerCATID_FWD_DEFINED__
#define __OPCEventServerCATID_FWD_DEFINED__
typedef interface OPCEventServerCATID OPCEventServerCATID;
#endif 	/* __OPCEventServerCATID_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __OPCEventServerCATID_INTERFACE_DEFINED__
#define __OPCEventServerCATID_INTERFACE_DEFINED__

/* interface OPCEventServerCATID */
/* [auto_handle][unique][helpstring][uuid] */ 



extern RPC_IF_HANDLE OPCEventServerCATID_v0_0_c_ifspec;
extern RPC_IF_HANDLE OPCEventServerCATID_v0_0_s_ifspec;
#endif /* __OPCEventServerCATID_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_opc_ae_0209 */
/* [local] */ 

typedef DWORD OPCHANDLE;

typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_opc_ae_0209_0001
    {	OPCAE_BROWSE_UP	= 1,
	OPCAE_BROWSE_DOWN	= OPCAE_BROWSE_UP + 1,
	OPCAE_BROWSE_TO	= OPCAE_BROWSE_DOWN + 1
    }	OPCAEBROWSEDIRECTION;

typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_opc_ae_0209_0002
    {	OPC_AREA	= 1,
	OPC_SOURCE	= OPC_AREA + 1
    }	OPCAEBROWSETYPE;

typedef /* [public][public][public] */ 
enum __MIDL___MIDL_itf_opc_ae_0209_0003
    {	OPCAE_STATUS_RUNNING	= 1,
	OPCAE_STATUS_FAILED	= OPCAE_STATUS_RUNNING + 1,
	OPCAE_STATUS_NOCONFIG	= OPCAE_STATUS_FAILED + 1,
	OPCAE_STATUS_SUSPENDED	= OPCAE_STATUS_NOCONFIG + 1,
	OPCAE_STATUS_TEST	= OPCAE_STATUS_SUSPENDED + 1
    }	OPCEVENTSERVERSTATE;

typedef /* [public][public] */ struct  __MIDL___MIDL_itf_opc_ae_0209_0004
    {
    WORD wChangeMask;
    WORD wNewState;
    /* [string] */ LPWSTR szSource;
    FILETIME ftTime;
    /* [string] */ LPWSTR szMessage;
    DWORD dwEventType;
    DWORD dwEventCategory;
    DWORD dwSeverity;
    /* [string] */ LPWSTR szConditionName;
    /* [string] */ LPWSTR szSubconditionName;
    WORD wQuality;
    WORD wReserved;
    BOOL bAckRequired;
    FILETIME ftActiveTime;
    DWORD dwCookie;
    DWORD dwNumEventAttrs;
    /* [size_is] */ VARIANT __RPC_FAR *pEventAttributes;
    /* [string] */ LPWSTR szActorID;
    }	ONEVENTSTRUCT;

typedef /* [public][public] */ struct  __MIDL___MIDL_itf_opc_ae_0209_0005
    {
    FILETIME ftStartTime;
    FILETIME ftCurrentTime;
    FILETIME ftLastUpdateTime;
    OPCEVENTSERVERSTATE dwServerState;
    WORD wMajorVersion;
    WORD wMinorVersion;
    WORD wBuildNumber;
    WORD wReserved;
    /* [string] */ LPWSTR szVendorInfo;
    }	OPCEVENTSERVERSTATUS;

typedef /* [public][public] */ struct  __MIDL___MIDL_itf_opc_ae_0209_0006
    {
    WORD wState;
    WORD wReserved1;
    LPWSTR szActiveSubCondition;
    LPWSTR szASCDefinition;
    DWORD dwASCSeverity;
    LPWSTR szASCDescription;
    WORD wQuality;
    WORD wReserved2;
    FILETIME ftLastAckTime;
    FILETIME ftSubCondLastActive;
    FILETIME ftCondLastActive;
    FILETIME ftCondLastInactive;
    LPWSTR szAcknowledgerID;
    LPWSTR szComment;
    DWORD dwNumSCs;
    /* [size_is] */ LPWSTR __RPC_FAR *pszSCNames;
    /* [size_is] */ LPWSTR __RPC_FAR *pszSCDefinitions;
    /* [size_is] */ DWORD __RPC_FAR *pdwSCSeverities;
    /* [size_is] */ LPWSTR __RPC_FAR *pszSCDescriptions;
    DWORD dwNumEventAttrs;
    /* [size_is] */ VARIANT __RPC_FAR *pEventAttributes;
    /* [size_is] */ HRESULT __RPC_FAR *pErrors;
    }	OPCCONDITIONSTATE;



extern RPC_IF_HANDLE __MIDL_itf_opc_ae_0209_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_opc_ae_0209_v0_0_s_ifspec;

#ifndef __IOPCEventServer_INTERFACE_DEFINED__
#define __IOPCEventServer_INTERFACE_DEFINED__

/* interface IOPCEventServer */
/* [object][unique][helpstring][uuid] */ 


EXTERN_C const IID IID_IOPCEventServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("65168851-5783-11D1-84A0-00608CB8A7E9")
    IOPCEventServer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [out] */ OPCEVENTSERVERSTATUS __RPC_FAR *__RPC_FAR *ppEventServerStatus) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateEventSubscription( 
            /* [in] */ BOOL bActive,
            /* [in] */ DWORD dwBufferTime,
            /* [in] */ DWORD dwMaxSize,
            /* [in] */ OPCHANDLE hClientSubscription,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk,
            /* [out] */ DWORD __RPC_FAR *pdwRevisedBufferTime,
            /* [out] */ DWORD __RPC_FAR *pdwRevisedMaxSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryAvailableFilters( 
            /* [out] */ DWORD __RPC_FAR *pdwFilterMask) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryEventCategories( 
            /* [in] */ DWORD dwEventType,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwEventCategories,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszEventCategoryDescs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryConditionNames( 
            /* [in] */ DWORD dwEventCategory,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszConditionNames) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QuerySubConditionNames( 
            /* [in] */ LPWSTR szConditionName,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszSubConditionNames) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QuerySourceConditions( 
            /* [in] */ LPWSTR szSource,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszConditionNames) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryEventAttributes( 
            /* [in] */ DWORD dwEventCategory,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwAttrIDs,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAttrDescs,
            /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppvtAttrTypes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TranslateToItemIDs( 
            /* [in] */ LPWSTR szSource,
            /* [in] */ DWORD dwEventCategory,
            /* [in] */ LPWSTR szConditionName,
            /* [in] */ LPWSTR szSubconditionName,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwAssocAttrIDs,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAttrItemIDs,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszNodeNames,
            /* [size_is][size_is][out] */ CLSID __RPC_FAR *__RPC_FAR *ppCLSIDs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetConditionState( 
            /* [in] */ LPWSTR szSource,
            /* [in] */ LPWSTR szConditionName,
            /* [in] */ DWORD dwNumEventAttrs,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwAttributeIDs,
            /* [out] */ OPCCONDITIONSTATE __RPC_FAR *__RPC_FAR *ppConditionState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnableConditionByArea( 
            /* [in] */ DWORD dwNumAreas,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreas) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnableConditionBySource( 
            /* [in] */ DWORD dwNumSources,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszSources) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DisableConditionByArea( 
            /* [in] */ DWORD dwNumAreas,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreas) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DisableConditionBySource( 
            /* [in] */ DWORD dwNumSources,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszSources) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AckCondition( 
            /* [in] */ DWORD dwCount,
            /* [string][in] */ LPWSTR szAcknowledgerID,
            /* [string][in] */ LPWSTR szComment,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszSource,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszConditionName,
            /* [size_is][in] */ FILETIME __RPC_FAR *pftActiveTime,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwCookie,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateAreaBrowser( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCEventServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCEventServer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCEventServer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStatus )( 
            IOPCEventServer __RPC_FAR * This,
            /* [out] */ OPCEVENTSERVERSTATUS __RPC_FAR *__RPC_FAR *ppEventServerStatus);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateEventSubscription )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ BOOL bActive,
            /* [in] */ DWORD dwBufferTime,
            /* [in] */ DWORD dwMaxSize,
            /* [in] */ OPCHANDLE hClientSubscription,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk,
            /* [out] */ DWORD __RPC_FAR *pdwRevisedBufferTime,
            /* [out] */ DWORD __RPC_FAR *pdwRevisedMaxSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryAvailableFilters )( 
            IOPCEventServer __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwFilterMask);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryEventCategories )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ DWORD dwEventType,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwEventCategories,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszEventCategoryDescs);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryConditionNames )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ DWORD dwEventCategory,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszConditionNames);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QuerySubConditionNames )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ LPWSTR szConditionName,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszSubConditionNames);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QuerySourceConditions )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ LPWSTR szSource,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszConditionNames);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryEventAttributes )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ DWORD dwEventCategory,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwAttrIDs,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAttrDescs,
            /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppvtAttrTypes);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TranslateToItemIDs )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ LPWSTR szSource,
            /* [in] */ DWORD dwEventCategory,
            /* [in] */ LPWSTR szConditionName,
            /* [in] */ LPWSTR szSubconditionName,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwAssocAttrIDs,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAttrItemIDs,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszNodeNames,
            /* [size_is][size_is][out] */ CLSID __RPC_FAR *__RPC_FAR *ppCLSIDs);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetConditionState )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ LPWSTR szSource,
            /* [in] */ LPWSTR szConditionName,
            /* [in] */ DWORD dwNumEventAttrs,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwAttributeIDs,
            /* [out] */ OPCCONDITIONSTATE __RPC_FAR *__RPC_FAR *ppConditionState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnableConditionByArea )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ DWORD dwNumAreas,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreas);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnableConditionBySource )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ DWORD dwNumSources,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszSources);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DisableConditionByArea )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ DWORD dwNumAreas,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreas);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DisableConditionBySource )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ DWORD dwNumSources,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszSources);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AckCondition )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [string][in] */ LPWSTR szAcknowledgerID,
            /* [string][in] */ LPWSTR szComment,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszSource,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszConditionName,
            /* [size_is][in] */ FILETIME __RPC_FAR *pftActiveTime,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwCookie,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateAreaBrowser )( 
            IOPCEventServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);
        
        END_INTERFACE
    } IOPCEventServerVtbl;

    interface IOPCEventServer
    {
        CONST_VTBL struct IOPCEventServerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCEventServer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCEventServer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCEventServer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCEventServer_GetStatus(This,ppEventServerStatus)	\
    (This)->lpVtbl -> GetStatus(This,ppEventServerStatus)

#define IOPCEventServer_CreateEventSubscription(This,bActive,dwBufferTime,dwMaxSize,hClientSubscription,riid,ppUnk,pdwRevisedBufferTime,pdwRevisedMaxSize)	\
    (This)->lpVtbl -> CreateEventSubscription(This,bActive,dwBufferTime,dwMaxSize,hClientSubscription,riid,ppUnk,pdwRevisedBufferTime,pdwRevisedMaxSize)

#define IOPCEventServer_QueryAvailableFilters(This,pdwFilterMask)	\
    (This)->lpVtbl -> QueryAvailableFilters(This,pdwFilterMask)

#define IOPCEventServer_QueryEventCategories(This,dwEventType,pdwCount,ppdwEventCategories,ppszEventCategoryDescs)	\
    (This)->lpVtbl -> QueryEventCategories(This,dwEventType,pdwCount,ppdwEventCategories,ppszEventCategoryDescs)

#define IOPCEventServer_QueryConditionNames(This,dwEventCategory,pdwCount,ppszConditionNames)	\
    (This)->lpVtbl -> QueryConditionNames(This,dwEventCategory,pdwCount,ppszConditionNames)

#define IOPCEventServer_QuerySubConditionNames(This,szConditionName,pdwCount,ppszSubConditionNames)	\
    (This)->lpVtbl -> QuerySubConditionNames(This,szConditionName,pdwCount,ppszSubConditionNames)

#define IOPCEventServer_QuerySourceConditions(This,szSource,pdwCount,ppszConditionNames)	\
    (This)->lpVtbl -> QuerySourceConditions(This,szSource,pdwCount,ppszConditionNames)

#define IOPCEventServer_QueryEventAttributes(This,dwEventCategory,pdwCount,ppdwAttrIDs,ppszAttrDescs,ppvtAttrTypes)	\
    (This)->lpVtbl -> QueryEventAttributes(This,dwEventCategory,pdwCount,ppdwAttrIDs,ppszAttrDescs,ppvtAttrTypes)

#define IOPCEventServer_TranslateToItemIDs(This,szSource,dwEventCategory,szConditionName,szSubconditionName,dwCount,pdwAssocAttrIDs,ppszAttrItemIDs,ppszNodeNames,ppCLSIDs)	\
    (This)->lpVtbl -> TranslateToItemIDs(This,szSource,dwEventCategory,szConditionName,szSubconditionName,dwCount,pdwAssocAttrIDs,ppszAttrItemIDs,ppszNodeNames,ppCLSIDs)

#define IOPCEventServer_GetConditionState(This,szSource,szConditionName,dwNumEventAttrs,pdwAttributeIDs,ppConditionState)	\
    (This)->lpVtbl -> GetConditionState(This,szSource,szConditionName,dwNumEventAttrs,pdwAttributeIDs,ppConditionState)

#define IOPCEventServer_EnableConditionByArea(This,dwNumAreas,pszAreas)	\
    (This)->lpVtbl -> EnableConditionByArea(This,dwNumAreas,pszAreas)

#define IOPCEventServer_EnableConditionBySource(This,dwNumSources,pszSources)	\
    (This)->lpVtbl -> EnableConditionBySource(This,dwNumSources,pszSources)

#define IOPCEventServer_DisableConditionByArea(This,dwNumAreas,pszAreas)	\
    (This)->lpVtbl -> DisableConditionByArea(This,dwNumAreas,pszAreas)

#define IOPCEventServer_DisableConditionBySource(This,dwNumSources,pszSources)	\
    (This)->lpVtbl -> DisableConditionBySource(This,dwNumSources,pszSources)

#define IOPCEventServer_AckCondition(This,dwCount,szAcknowledgerID,szComment,pszSource,pszConditionName,pftActiveTime,pdwCookie,ppErrors)	\
    (This)->lpVtbl -> AckCondition(This,dwCount,szAcknowledgerID,szComment,pszSource,pszConditionName,pftActiveTime,pdwCookie,ppErrors)

#define IOPCEventServer_CreateAreaBrowser(This,riid,ppUnk)	\
    (This)->lpVtbl -> CreateAreaBrowser(This,riid,ppUnk)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCEventServer_GetStatus_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [out] */ OPCEVENTSERVERSTATUS __RPC_FAR *__RPC_FAR *ppEventServerStatus);


void __RPC_STUB IOPCEventServer_GetStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_CreateEventSubscription_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [in] */ BOOL bActive,
    /* [in] */ DWORD dwBufferTime,
    /* [in] */ DWORD dwMaxSize,
    /* [in] */ OPCHANDLE hClientSubscription,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk,
    /* [out] */ DWORD __RPC_FAR *pdwRevisedBufferTime,
    /* [out] */ DWORD __RPC_FAR *pdwRevisedMaxSize);


void __RPC_STUB IOPCEventServer_CreateEventSubscription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_QueryAvailableFilters_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwFilterMask);


void __RPC_STUB IOPCEventServer_QueryAvailableFilters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_QueryEventCategories_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [in] */ DWORD dwEventType,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwEventCategories,
    /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszEventCategoryDescs);


void __RPC_STUB IOPCEventServer_QueryEventCategories_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_QueryConditionNames_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [in] */ DWORD dwEventCategory,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszConditionNames);


void __RPC_STUB IOPCEventServer_QueryConditionNames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_QuerySubConditionNames_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [in] */ LPWSTR szConditionName,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszSubConditionNames);


void __RPC_STUB IOPCEventServer_QuerySubConditionNames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_QuerySourceConditions_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [in] */ LPWSTR szSource,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszConditionNames);


void __RPC_STUB IOPCEventServer_QuerySourceConditions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_QueryEventAttributes_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [in] */ DWORD dwEventCategory,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwAttrIDs,
    /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAttrDescs,
    /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppvtAttrTypes);


void __RPC_STUB IOPCEventServer_QueryEventAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_TranslateToItemIDs_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [in] */ LPWSTR szSource,
    /* [in] */ DWORD dwEventCategory,
    /* [in] */ LPWSTR szConditionName,
    /* [in] */ LPWSTR szSubconditionName,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwAssocAttrIDs,
    /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAttrItemIDs,
    /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszNodeNames,
    /* [size_is][size_is][out] */ CLSID __RPC_FAR *__RPC_FAR *ppCLSIDs);


void __RPC_STUB IOPCEventServer_TranslateToItemIDs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_GetConditionState_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [in] */ LPWSTR szSource,
    /* [in] */ LPWSTR szConditionName,
    /* [in] */ DWORD dwNumEventAttrs,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwAttributeIDs,
    /* [out] */ OPCCONDITIONSTATE __RPC_FAR *__RPC_FAR *ppConditionState);


void __RPC_STUB IOPCEventServer_GetConditionState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_EnableConditionByArea_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [in] */ DWORD dwNumAreas,
    /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreas);


void __RPC_STUB IOPCEventServer_EnableConditionByArea_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_EnableConditionBySource_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [in] */ DWORD dwNumSources,
    /* [size_is][in] */ LPWSTR __RPC_FAR *pszSources);


void __RPC_STUB IOPCEventServer_EnableConditionBySource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_DisableConditionByArea_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [in] */ DWORD dwNumAreas,
    /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreas);


void __RPC_STUB IOPCEventServer_DisableConditionByArea_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_DisableConditionBySource_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [in] */ DWORD dwNumSources,
    /* [size_is][in] */ LPWSTR __RPC_FAR *pszSources);


void __RPC_STUB IOPCEventServer_DisableConditionBySource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_AckCondition_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [string][in] */ LPWSTR szAcknowledgerID,
    /* [string][in] */ LPWSTR szComment,
    /* [size_is][in] */ LPWSTR __RPC_FAR *pszSource,
    /* [size_is][in] */ LPWSTR __RPC_FAR *pszConditionName,
    /* [size_is][in] */ FILETIME __RPC_FAR *pftActiveTime,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwCookie,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCEventServer_AckCondition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventServer_CreateAreaBrowser_Proxy( 
    IOPCEventServer __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);


void __RPC_STUB IOPCEventServer_CreateAreaBrowser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCEventServer_INTERFACE_DEFINED__ */


#ifndef __IOPCEventSubscriptionMgt_INTERFACE_DEFINED__
#define __IOPCEventSubscriptionMgt_INTERFACE_DEFINED__

/* interface IOPCEventSubscriptionMgt */
/* [object][unique][helpstring][uuid] */ 


EXTERN_C const IID IID_IOPCEventSubscriptionMgt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("65168855-5783-11D1-84A0-00608CB8A7E9")
    IOPCEventSubscriptionMgt : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetFilter( 
            /* [in] */ DWORD dwEventType,
            /* [in] */ DWORD dwNumCategories,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwEventCategories,
            /* [in] */ DWORD dwLowSeverity,
            /* [in] */ DWORD dwHighSeverity,
            /* [in] */ DWORD dwNumAreas,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreaList,
            /* [in] */ DWORD dwNumSources,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszSourceList) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFilter( 
            /* [out] */ DWORD __RPC_FAR *pdwEventType,
            /* [out] */ DWORD __RPC_FAR *pdwNumCategories,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwEventCategories,
            /* [out] */ DWORD __RPC_FAR *pdwLowSeverity,
            /* [out] */ DWORD __RPC_FAR *pdwHighSeverity,
            /* [out] */ DWORD __RPC_FAR *pdwNumAreas,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAreaList,
            /* [out] */ DWORD __RPC_FAR *pdwNumSources,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszSourceList) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectReturnedAttributes( 
            /* [in] */ DWORD dwEventCategory,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ DWORD __RPC_FAR *dwAttributeIDs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetReturnedAttributes( 
            /* [in] */ DWORD dwEventCategory,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwAttributeIDs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Refresh( 
            /* [in] */ DWORD dwConnection) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelRefresh( 
            /* [in] */ DWORD dwConnection) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetState( 
            /* [out] */ BOOL __RPC_FAR *pbActive,
            /* [out] */ DWORD __RPC_FAR *pdwBufferTime,
            /* [out] */ DWORD __RPC_FAR *pdwMaxSize,
            /* [out] */ OPCHANDLE __RPC_FAR *phClientSubscription) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetState( 
            /* [in][unique] */ BOOL __RPC_FAR *pbActive,
            /* [in][unique] */ DWORD __RPC_FAR *pdwBufferTime,
            /* [in][unique] */ DWORD __RPC_FAR *pdwMaxSize,
            /* [in] */ OPCHANDLE hClientSubscription,
            /* [out] */ DWORD __RPC_FAR *pdwRevisedBufferTime,
            /* [out] */ DWORD __RPC_FAR *pdwRevisedMaxSize) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCEventSubscriptionMgtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCEventSubscriptionMgt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCEventSubscriptionMgt __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCEventSubscriptionMgt __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFilter )( 
            IOPCEventSubscriptionMgt __RPC_FAR * This,
            /* [in] */ DWORD dwEventType,
            /* [in] */ DWORD dwNumCategories,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwEventCategories,
            /* [in] */ DWORD dwLowSeverity,
            /* [in] */ DWORD dwHighSeverity,
            /* [in] */ DWORD dwNumAreas,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreaList,
            /* [in] */ DWORD dwNumSources,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszSourceList);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFilter )( 
            IOPCEventSubscriptionMgt __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwEventType,
            /* [out] */ DWORD __RPC_FAR *pdwNumCategories,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwEventCategories,
            /* [out] */ DWORD __RPC_FAR *pdwLowSeverity,
            /* [out] */ DWORD __RPC_FAR *pdwHighSeverity,
            /* [out] */ DWORD __RPC_FAR *pdwNumAreas,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAreaList,
            /* [out] */ DWORD __RPC_FAR *pdwNumSources,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszSourceList);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectReturnedAttributes )( 
            IOPCEventSubscriptionMgt __RPC_FAR * This,
            /* [in] */ DWORD dwEventCategory,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ DWORD __RPC_FAR *dwAttributeIDs);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetReturnedAttributes )( 
            IOPCEventSubscriptionMgt __RPC_FAR * This,
            /* [in] */ DWORD dwEventCategory,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwAttributeIDs);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh )( 
            IOPCEventSubscriptionMgt __RPC_FAR * This,
            /* [in] */ DWORD dwConnection);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CancelRefresh )( 
            IOPCEventSubscriptionMgt __RPC_FAR * This,
            /* [in] */ DWORD dwConnection);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetState )( 
            IOPCEventSubscriptionMgt __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pbActive,
            /* [out] */ DWORD __RPC_FAR *pdwBufferTime,
            /* [out] */ DWORD __RPC_FAR *pdwMaxSize,
            /* [out] */ OPCHANDLE __RPC_FAR *phClientSubscription);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetState )( 
            IOPCEventSubscriptionMgt __RPC_FAR * This,
            /* [in][unique] */ BOOL __RPC_FAR *pbActive,
            /* [in][unique] */ DWORD __RPC_FAR *pdwBufferTime,
            /* [in][unique] */ DWORD __RPC_FAR *pdwMaxSize,
            /* [in] */ OPCHANDLE hClientSubscription,
            /* [out] */ DWORD __RPC_FAR *pdwRevisedBufferTime,
            /* [out] */ DWORD __RPC_FAR *pdwRevisedMaxSize);
        
        END_INTERFACE
    } IOPCEventSubscriptionMgtVtbl;

    interface IOPCEventSubscriptionMgt
    {
        CONST_VTBL struct IOPCEventSubscriptionMgtVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCEventSubscriptionMgt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCEventSubscriptionMgt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCEventSubscriptionMgt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCEventSubscriptionMgt_SetFilter(This,dwEventType,dwNumCategories,pdwEventCategories,dwLowSeverity,dwHighSeverity,dwNumAreas,pszAreaList,dwNumSources,pszSourceList)	\
    (This)->lpVtbl -> SetFilter(This,dwEventType,dwNumCategories,pdwEventCategories,dwLowSeverity,dwHighSeverity,dwNumAreas,pszAreaList,dwNumSources,pszSourceList)

#define IOPCEventSubscriptionMgt_GetFilter(This,pdwEventType,pdwNumCategories,ppdwEventCategories,pdwLowSeverity,pdwHighSeverity,pdwNumAreas,ppszAreaList,pdwNumSources,ppszSourceList)	\
    (This)->lpVtbl -> GetFilter(This,pdwEventType,pdwNumCategories,ppdwEventCategories,pdwLowSeverity,pdwHighSeverity,pdwNumAreas,ppszAreaList,pdwNumSources,ppszSourceList)

#define IOPCEventSubscriptionMgt_SelectReturnedAttributes(This,dwEventCategory,dwCount,dwAttributeIDs)	\
    (This)->lpVtbl -> SelectReturnedAttributes(This,dwEventCategory,dwCount,dwAttributeIDs)

#define IOPCEventSubscriptionMgt_GetReturnedAttributes(This,dwEventCategory,pdwCount,ppdwAttributeIDs)	\
    (This)->lpVtbl -> GetReturnedAttributes(This,dwEventCategory,pdwCount,ppdwAttributeIDs)

#define IOPCEventSubscriptionMgt_Refresh(This,dwConnection)	\
    (This)->lpVtbl -> Refresh(This,dwConnection)

#define IOPCEventSubscriptionMgt_CancelRefresh(This,dwConnection)	\
    (This)->lpVtbl -> CancelRefresh(This,dwConnection)

#define IOPCEventSubscriptionMgt_GetState(This,pbActive,pdwBufferTime,pdwMaxSize,phClientSubscription)	\
    (This)->lpVtbl -> GetState(This,pbActive,pdwBufferTime,pdwMaxSize,phClientSubscription)

#define IOPCEventSubscriptionMgt_SetState(This,pbActive,pdwBufferTime,pdwMaxSize,hClientSubscription,pdwRevisedBufferTime,pdwRevisedMaxSize)	\
    (This)->lpVtbl -> SetState(This,pbActive,pdwBufferTime,pdwMaxSize,hClientSubscription,pdwRevisedBufferTime,pdwRevisedMaxSize)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCEventSubscriptionMgt_SetFilter_Proxy( 
    IOPCEventSubscriptionMgt __RPC_FAR * This,
    /* [in] */ DWORD dwEventType,
    /* [in] */ DWORD dwNumCategories,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwEventCategories,
    /* [in] */ DWORD dwLowSeverity,
    /* [in] */ DWORD dwHighSeverity,
    /* [in] */ DWORD dwNumAreas,
    /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreaList,
    /* [in] */ DWORD dwNumSources,
    /* [size_is][in] */ LPWSTR __RPC_FAR *pszSourceList);


void __RPC_STUB IOPCEventSubscriptionMgt_SetFilter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventSubscriptionMgt_GetFilter_Proxy( 
    IOPCEventSubscriptionMgt __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwEventType,
    /* [out] */ DWORD __RPC_FAR *pdwNumCategories,
    /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwEventCategories,
    /* [out] */ DWORD __RPC_FAR *pdwLowSeverity,
    /* [out] */ DWORD __RPC_FAR *pdwHighSeverity,
    /* [out] */ DWORD __RPC_FAR *pdwNumAreas,
    /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAreaList,
    /* [out] */ DWORD __RPC_FAR *pdwNumSources,
    /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszSourceList);


void __RPC_STUB IOPCEventSubscriptionMgt_GetFilter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventSubscriptionMgt_SelectReturnedAttributes_Proxy( 
    IOPCEventSubscriptionMgt __RPC_FAR * This,
    /* [in] */ DWORD dwEventCategory,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ DWORD __RPC_FAR *dwAttributeIDs);


void __RPC_STUB IOPCEventSubscriptionMgt_SelectReturnedAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventSubscriptionMgt_GetReturnedAttributes_Proxy( 
    IOPCEventSubscriptionMgt __RPC_FAR * This,
    /* [in] */ DWORD dwEventCategory,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwAttributeIDs);


void __RPC_STUB IOPCEventSubscriptionMgt_GetReturnedAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventSubscriptionMgt_Refresh_Proxy( 
    IOPCEventSubscriptionMgt __RPC_FAR * This,
    /* [in] */ DWORD dwConnection);


void __RPC_STUB IOPCEventSubscriptionMgt_Refresh_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventSubscriptionMgt_CancelRefresh_Proxy( 
    IOPCEventSubscriptionMgt __RPC_FAR * This,
    /* [in] */ DWORD dwConnection);


void __RPC_STUB IOPCEventSubscriptionMgt_CancelRefresh_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventSubscriptionMgt_GetState_Proxy( 
    IOPCEventSubscriptionMgt __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pbActive,
    /* [out] */ DWORD __RPC_FAR *pdwBufferTime,
    /* [out] */ DWORD __RPC_FAR *pdwMaxSize,
    /* [out] */ OPCHANDLE __RPC_FAR *phClientSubscription);


void __RPC_STUB IOPCEventSubscriptionMgt_GetState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventSubscriptionMgt_SetState_Proxy( 
    IOPCEventSubscriptionMgt __RPC_FAR * This,
    /* [in][unique] */ BOOL __RPC_FAR *pbActive,
    /* [in][unique] */ DWORD __RPC_FAR *pdwBufferTime,
    /* [in][unique] */ DWORD __RPC_FAR *pdwMaxSize,
    /* [in] */ OPCHANDLE hClientSubscription,
    /* [out] */ DWORD __RPC_FAR *pdwRevisedBufferTime,
    /* [out] */ DWORD __RPC_FAR *pdwRevisedMaxSize);


void __RPC_STUB IOPCEventSubscriptionMgt_SetState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCEventSubscriptionMgt_INTERFACE_DEFINED__ */


#ifndef __IOPCEventAreaBrowser_INTERFACE_DEFINED__
#define __IOPCEventAreaBrowser_INTERFACE_DEFINED__

/* interface IOPCEventAreaBrowser */
/* [object][unique][helpstring][uuid] */ 


EXTERN_C const IID IID_IOPCEventAreaBrowser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("65168857-5783-11D1-84A0-00608CB8A7E9")
    IOPCEventAreaBrowser : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ChangeBrowsePosition( 
            /* [in] */ OPCAEBROWSEDIRECTION dwBrowseDirection,
            /* [string][in] */ LPCWSTR szString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BrowseOPCAreas( 
            /* [in] */ OPCAEBROWSETYPE dwBrowseFilterType,
            /* [string][in] */ LPCWSTR szFilterCriteria,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetQualifiedAreaName( 
            /* [in] */ LPCWSTR szAreaName,
            /* [string][out] */ LPWSTR __RPC_FAR *pszQualifiedAreaName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetQualifiedSourceName( 
            /* [in] */ LPCWSTR szSourceName,
            /* [string][out] */ LPWSTR __RPC_FAR *pszQualifiedSourceName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCEventAreaBrowserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCEventAreaBrowser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCEventAreaBrowser __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCEventAreaBrowser __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ChangeBrowsePosition )( 
            IOPCEventAreaBrowser __RPC_FAR * This,
            /* [in] */ OPCAEBROWSEDIRECTION dwBrowseDirection,
            /* [string][in] */ LPCWSTR szString);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BrowseOPCAreas )( 
            IOPCEventAreaBrowser __RPC_FAR * This,
            /* [in] */ OPCAEBROWSETYPE dwBrowseFilterType,
            /* [string][in] */ LPCWSTR szFilterCriteria,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetQualifiedAreaName )( 
            IOPCEventAreaBrowser __RPC_FAR * This,
            /* [in] */ LPCWSTR szAreaName,
            /* [string][out] */ LPWSTR __RPC_FAR *pszQualifiedAreaName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetQualifiedSourceName )( 
            IOPCEventAreaBrowser __RPC_FAR * This,
            /* [in] */ LPCWSTR szSourceName,
            /* [string][out] */ LPWSTR __RPC_FAR *pszQualifiedSourceName);
        
        END_INTERFACE
    } IOPCEventAreaBrowserVtbl;

    interface IOPCEventAreaBrowser
    {
        CONST_VTBL struct IOPCEventAreaBrowserVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCEventAreaBrowser_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCEventAreaBrowser_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCEventAreaBrowser_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCEventAreaBrowser_ChangeBrowsePosition(This,dwBrowseDirection,szString)	\
    (This)->lpVtbl -> ChangeBrowsePosition(This,dwBrowseDirection,szString)

#define IOPCEventAreaBrowser_BrowseOPCAreas(This,dwBrowseFilterType,szFilterCriteria,ppIEnumString)	\
    (This)->lpVtbl -> BrowseOPCAreas(This,dwBrowseFilterType,szFilterCriteria,ppIEnumString)

#define IOPCEventAreaBrowser_GetQualifiedAreaName(This,szAreaName,pszQualifiedAreaName)	\
    (This)->lpVtbl -> GetQualifiedAreaName(This,szAreaName,pszQualifiedAreaName)

#define IOPCEventAreaBrowser_GetQualifiedSourceName(This,szSourceName,pszQualifiedSourceName)	\
    (This)->lpVtbl -> GetQualifiedSourceName(This,szSourceName,pszQualifiedSourceName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCEventAreaBrowser_ChangeBrowsePosition_Proxy( 
    IOPCEventAreaBrowser __RPC_FAR * This,
    /* [in] */ OPCAEBROWSEDIRECTION dwBrowseDirection,
    /* [string][in] */ LPCWSTR szString);


void __RPC_STUB IOPCEventAreaBrowser_ChangeBrowsePosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventAreaBrowser_BrowseOPCAreas_Proxy( 
    IOPCEventAreaBrowser __RPC_FAR * This,
    /* [in] */ OPCAEBROWSETYPE dwBrowseFilterType,
    /* [string][in] */ LPCWSTR szFilterCriteria,
    /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);


void __RPC_STUB IOPCEventAreaBrowser_BrowseOPCAreas_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventAreaBrowser_GetQualifiedAreaName_Proxy( 
    IOPCEventAreaBrowser __RPC_FAR * This,
    /* [in] */ LPCWSTR szAreaName,
    /* [string][out] */ LPWSTR __RPC_FAR *pszQualifiedAreaName);


void __RPC_STUB IOPCEventAreaBrowser_GetQualifiedAreaName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCEventAreaBrowser_GetQualifiedSourceName_Proxy( 
    IOPCEventAreaBrowser __RPC_FAR * This,
    /* [in] */ LPCWSTR szSourceName,
    /* [string][out] */ LPWSTR __RPC_FAR *pszQualifiedSourceName);


void __RPC_STUB IOPCEventAreaBrowser_GetQualifiedSourceName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCEventAreaBrowser_INTERFACE_DEFINED__ */


#ifndef __IOPCEventSink_INTERFACE_DEFINED__
#define __IOPCEventSink_INTERFACE_DEFINED__

/* interface IOPCEventSink */
/* [object][unique][helpstring][uuid] */ 


EXTERN_C const IID IID_IOPCEventSink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6516885F-5783-11D1-84A0-00608CB8A7E9")
    IOPCEventSink : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnEvent( 
            /* [in] */ OPCHANDLE hClientSubscription,
            /* [in] */ BOOL bRefresh,
            /* [in] */ BOOL bLastRefresh,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ ONEVENTSTRUCT __RPC_FAR *pEvents) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCEventSinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCEventSink __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCEventSink __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCEventSink __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnEvent )( 
            IOPCEventSink __RPC_FAR * This,
            /* [in] */ OPCHANDLE hClientSubscription,
            /* [in] */ BOOL bRefresh,
            /* [in] */ BOOL bLastRefresh,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ ONEVENTSTRUCT __RPC_FAR *pEvents);
        
        END_INTERFACE
    } IOPCEventSinkVtbl;

    interface IOPCEventSink
    {
        CONST_VTBL struct IOPCEventSinkVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCEventSink_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCEventSink_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCEventSink_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCEventSink_OnEvent(This,hClientSubscription,bRefresh,bLastRefresh,dwCount,pEvents)	\
    (This)->lpVtbl -> OnEvent(This,hClientSubscription,bRefresh,bLastRefresh,dwCount,pEvents)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCEventSink_OnEvent_Proxy( 
    IOPCEventSink __RPC_FAR * This,
    /* [in] */ OPCHANDLE hClientSubscription,
    /* [in] */ BOOL bRefresh,
    /* [in] */ BOOL bLastRefresh,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ ONEVENTSTRUCT __RPC_FAR *pEvents);


void __RPC_STUB IOPCEventSink_OnEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCEventSink_INTERFACE_DEFINED__ */



#ifndef __OPC_AE_LIBRARY_DEFINED__
#define __OPC_AE_LIBRARY_DEFINED__

/* library OPC_AE */
/* [helpstring][version][uuid] */ 







EXTERN_C const IID LIBID_OPC_AE;

#ifndef __OPCEventServerCATID_INTERFACE_DEFINED__
#define __OPCEventServerCATID_INTERFACE_DEFINED__

/* interface OPCEventServerCATID */
/* [auto_handle][unique][helpstring][uuid] */ 


EXTERN_C const IID IID_OPCEventServerCATID;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("58E13251-AC87-11d1-84D5-00608CB8A7E9")
    OPCEventServerCATID
    {
    public:
        BEGIN_INTERFACE
        END_INTERFACE
    };
    
#else 	/* C style interface */

    typedef struct OPCEventServerCATIDVtbl
    {
        BEGIN_INTERFACE
        
        END_INTERFACE
    } OPCEventServerCATIDVtbl;

    interface OPCEventServerCATID
    {
        CONST_VTBL struct OPCEventServerCATIDVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __OPCEventServerCATID_INTERFACE_DEFINED__ */

#endif /* __OPC_AE_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
