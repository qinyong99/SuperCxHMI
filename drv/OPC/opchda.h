/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Sep 14 14:21:44 2000
 */
/* Compiler settings for opchda.idl:
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

#ifndef __opchda_h__
#define __opchda_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IOPCHDA_Browser_FWD_DEFINED__
#define __IOPCHDA_Browser_FWD_DEFINED__
typedef interface IOPCHDA_Browser IOPCHDA_Browser;
#endif 	/* __IOPCHDA_Browser_FWD_DEFINED__ */


#ifndef __IOPCHDA_Server_FWD_DEFINED__
#define __IOPCHDA_Server_FWD_DEFINED__
typedef interface IOPCHDA_Server IOPCHDA_Server;
#endif 	/* __IOPCHDA_Server_FWD_DEFINED__ */


#ifndef __IOPCHDA_SyncRead_FWD_DEFINED__
#define __IOPCHDA_SyncRead_FWD_DEFINED__
typedef interface IOPCHDA_SyncRead IOPCHDA_SyncRead;
#endif 	/* __IOPCHDA_SyncRead_FWD_DEFINED__ */


#ifndef __IOPCHDA_SyncUpdate_FWD_DEFINED__
#define __IOPCHDA_SyncUpdate_FWD_DEFINED__
typedef interface IOPCHDA_SyncUpdate IOPCHDA_SyncUpdate;
#endif 	/* __IOPCHDA_SyncUpdate_FWD_DEFINED__ */


#ifndef __IOPCHDA_SyncAnnotations_FWD_DEFINED__
#define __IOPCHDA_SyncAnnotations_FWD_DEFINED__
typedef interface IOPCHDA_SyncAnnotations IOPCHDA_SyncAnnotations;
#endif 	/* __IOPCHDA_SyncAnnotations_FWD_DEFINED__ */


#ifndef __IOPCHDA_AsyncRead_FWD_DEFINED__
#define __IOPCHDA_AsyncRead_FWD_DEFINED__
typedef interface IOPCHDA_AsyncRead IOPCHDA_AsyncRead;
#endif 	/* __IOPCHDA_AsyncRead_FWD_DEFINED__ */


#ifndef __IOPCHDA_AsyncUpdate_FWD_DEFINED__
#define __IOPCHDA_AsyncUpdate_FWD_DEFINED__
typedef interface IOPCHDA_AsyncUpdate IOPCHDA_AsyncUpdate;
#endif 	/* __IOPCHDA_AsyncUpdate_FWD_DEFINED__ */


#ifndef __IOPCHDA_AsyncAnnotations_FWD_DEFINED__
#define __IOPCHDA_AsyncAnnotations_FWD_DEFINED__
typedef interface IOPCHDA_AsyncAnnotations IOPCHDA_AsyncAnnotations;
#endif 	/* __IOPCHDA_AsyncAnnotations_FWD_DEFINED__ */


#ifndef __IOPCHDA_Playback_FWD_DEFINED__
#define __IOPCHDA_Playback_FWD_DEFINED__
typedef interface IOPCHDA_Playback IOPCHDA_Playback;
#endif 	/* __IOPCHDA_Playback_FWD_DEFINED__ */


#ifndef __IOPCHDA_DataCallback_FWD_DEFINED__
#define __IOPCHDA_DataCallback_FWD_DEFINED__
typedef interface IOPCHDA_DataCallback IOPCHDA_DataCallback;
#endif 	/* __IOPCHDA_DataCallback_FWD_DEFINED__ */


#ifndef __IOPCHDA_Server_FWD_DEFINED__
#define __IOPCHDA_Server_FWD_DEFINED__
typedef interface IOPCHDA_Server IOPCHDA_Server;
#endif 	/* __IOPCHDA_Server_FWD_DEFINED__ */


#ifndef __IOPCHDA_Browser_FWD_DEFINED__
#define __IOPCHDA_Browser_FWD_DEFINED__
typedef interface IOPCHDA_Browser IOPCHDA_Browser;
#endif 	/* __IOPCHDA_Browser_FWD_DEFINED__ */


#ifndef __IOPCHDA_SyncRead_FWD_DEFINED__
#define __IOPCHDA_SyncRead_FWD_DEFINED__
typedef interface IOPCHDA_SyncRead IOPCHDA_SyncRead;
#endif 	/* __IOPCHDA_SyncRead_FWD_DEFINED__ */


#ifndef __IOPCHDA_SyncUpdate_FWD_DEFINED__
#define __IOPCHDA_SyncUpdate_FWD_DEFINED__
typedef interface IOPCHDA_SyncUpdate IOPCHDA_SyncUpdate;
#endif 	/* __IOPCHDA_SyncUpdate_FWD_DEFINED__ */


#ifndef __IOPCHDA_SyncAnnotations_FWD_DEFINED__
#define __IOPCHDA_SyncAnnotations_FWD_DEFINED__
typedef interface IOPCHDA_SyncAnnotations IOPCHDA_SyncAnnotations;
#endif 	/* __IOPCHDA_SyncAnnotations_FWD_DEFINED__ */


#ifndef __IOPCHDA_AsyncRead_FWD_DEFINED__
#define __IOPCHDA_AsyncRead_FWD_DEFINED__
typedef interface IOPCHDA_AsyncRead IOPCHDA_AsyncRead;
#endif 	/* __IOPCHDA_AsyncRead_FWD_DEFINED__ */


#ifndef __IOPCHDA_AsyncUpdate_FWD_DEFINED__
#define __IOPCHDA_AsyncUpdate_FWD_DEFINED__
typedef interface IOPCHDA_AsyncUpdate IOPCHDA_AsyncUpdate;
#endif 	/* __IOPCHDA_AsyncUpdate_FWD_DEFINED__ */


#ifndef __IOPCHDA_AsyncAnnotations_FWD_DEFINED__
#define __IOPCHDA_AsyncAnnotations_FWD_DEFINED__
typedef interface IOPCHDA_AsyncAnnotations IOPCHDA_AsyncAnnotations;
#endif 	/* __IOPCHDA_AsyncAnnotations_FWD_DEFINED__ */


#ifndef __IOPCHDA_Playback_FWD_DEFINED__
#define __IOPCHDA_Playback_FWD_DEFINED__
typedef interface IOPCHDA_Playback IOPCHDA_Playback;
#endif 	/* __IOPCHDA_Playback_FWD_DEFINED__ */


#ifndef __IOPCHDA_DataCallback_FWD_DEFINED__
#define __IOPCHDA_DataCallback_FWD_DEFINED__
typedef interface IOPCHDA_DataCallback IOPCHDA_DataCallback;
#endif 	/* __IOPCHDA_DataCallback_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_opchda_0000 */
/* [local] */ 

typedef 
enum tagOPCHDA_SERVERSTATUS
    {	OPCHDA_UP	= 1,
	OPCHDA_DOWN	= OPCHDA_UP + 1,
	OPCHDA_INDETERMINATE	= OPCHDA_DOWN + 1
    }	OPCHDA_SERVERSTATUS;

typedef 
enum tagOPCHDA_BROWSEDIRECTION
    {	OPCHDA_BROWSE_UP	= 1,
	OPCHDA_BROWSE_DOWN	= OPCHDA_BROWSE_UP + 1,
	OPCHDA_BROWSE_DIRECT	= OPCHDA_BROWSE_DOWN + 1
    }	OPCHDA_BROWSEDIRECTION;

typedef 
enum tagOPCHDA_BROWSETYPE
    {	OPCHDA_BRANCH	= 1,
	OPCHDA_LEAF	= OPCHDA_BRANCH + 1,
	OPCHDA_FLAT	= OPCHDA_LEAF + 1,
	OPCHDA_ITEMS	= OPCHDA_FLAT + 1
    }	OPCHDA_BROWSETYPE;

typedef 
enum tagOPCHDA_ANNOTATIONCAPABILITIES
    {	OPCHDA_READANNOTATIONCAP	= 0x1,
	OPCHDA_INSERTANNOTATIONCAP	= 0x2
    }	OPCHDA_ANNOTATIONCAPABILITIES;

typedef 
enum tagOPCHDA_UPDATECAPABILITIES
    {	OPCHDA_INSERTCAP	= 0x1,
	OPCHDA_REPLACECAP	= 0x2,
	OPCHDA_INSERTREPLACECAP	= 0x4,
	OPCHDA_DELETERAWCAP	= 0x8,
	OPCHDA_DELETEATTIMECAP	= 0x10
    }	OPCHDA_UPDATECAPABILITIES;

typedef 
enum tagOPCHDA_OPERATORCODES
    {	OPCHDA_EQUAL	= 1,
	OPCHDA_LESS	= OPCHDA_EQUAL + 1,
	OPCHDA_LESSEQUAL	= OPCHDA_LESS + 1,
	OPCHDA_GREATER	= OPCHDA_LESSEQUAL + 1,
	OPCHDA_GREATEREQUAL	= OPCHDA_GREATER + 1,
	OPCHDA_NOTEQUAL	= OPCHDA_GREATEREQUAL + 1
    }	OPCHDA_OPERATORCODES;

typedef 
enum tagOPCHDA_EDITTYPE
    {	OPCHDA_INSERT	= 1,
	OPCHDA_REPLACE	= OPCHDA_INSERT + 1,
	OPCHDA_INSERTREPLACE	= OPCHDA_REPLACE + 1,
	OPCHDA_DELETE	= OPCHDA_INSERTREPLACE + 1
    }	OPCHDA_EDITTYPE;

typedef 
enum tagOPCHDA_AGGREGATE
    {	OPCHDA_NOAGGREGATE	= 0,
	OPCHDA_INTERPOLATIVE	= OPCHDA_NOAGGREGATE + 1,
	OPCHDA_TOTAL	= OPCHDA_INTERPOLATIVE + 1,
	OPCHDA_AVERAGE	= OPCHDA_TOTAL + 1,
	OPCHDA_TIMEAVERAGE	= OPCHDA_AVERAGE + 1,
	OPCHDA_COUNT	= OPCHDA_TIMEAVERAGE + 1,
	OPCHDA_STDEV	= OPCHDA_COUNT + 1,
	OPCHDA_MINIMUMACTUALTIME	= OPCHDA_STDEV + 1,
	OPCHDA_MINIMUM	= OPCHDA_MINIMUMACTUALTIME + 1,
	OPCHDA_MAXIMUMACTUALTIME	= OPCHDA_MINIMUM + 1,
	OPCHDA_MAXIMUM	= OPCHDA_MAXIMUMACTUALTIME + 1,
	OPCHDA_START	= OPCHDA_MAXIMUM + 1,
	OPCHDA_END	= OPCHDA_START + 1,
	OPCHDA_DELTA	= OPCHDA_END + 1,
	OPCHDA_REGSLOPE	= OPCHDA_DELTA + 1,
	OPCHDA_REGCONST	= OPCHDA_REGSLOPE + 1,
	OPCHDA_REGDEV	= OPCHDA_REGCONST + 1,
	OPCHDA_VARIANCE	= OPCHDA_REGDEV + 1,
	OPCHDA_RANGE	= OPCHDA_VARIANCE + 1,
	OPCHDA_DURATIONGOOD	= OPCHDA_RANGE + 1,
	OPCHDA_DURATIONBAD	= OPCHDA_DURATIONGOOD + 1,
	OPCHDA_PERCENTGOOD	= OPCHDA_DURATIONBAD + 1,
	OPCHDA_PERCENTBAD	= OPCHDA_PERCENTGOOD + 1,
	OPCHDA_WORSTQUALITY	= OPCHDA_PERCENTBAD + 1,
	OPCHDA_ANNOTATIONS	= OPCHDA_WORSTQUALITY + 1
    }	OPCHDA_AGGREGATE;

typedef DWORD OPCHANDLE;

typedef struct  tagOPCHDA_ANNOTATION
    {
    OPCHANDLE hClient;
    DWORD dwNumValues;
    /* [size_is] */ FILETIME __RPC_FAR *ftTimeStamps;
    /* [string][size_is] */ LPWSTR __RPC_FAR *szAnnotation;
    /* [size_is] */ FILETIME __RPC_FAR *ftAnnotationTime;
    /* [string][size_is] */ LPWSTR __RPC_FAR *szUser;
    }	OPCHDA_ANNOTATION;

typedef struct  tagOPCHDA_MODIFIEDITEM
    {
    OPCHANDLE hClient;
    DWORD dwCount;
    /* [size_is] */ FILETIME __RPC_FAR *pftTimeStamps;
    /* [size_is] */ DWORD __RPC_FAR *pdwQualities;
    /* [size_is] */ VARIANT __RPC_FAR *pvDataValues;
    /* [size_is] */ FILETIME __RPC_FAR *pftModificationTime;
    /* [size_is] */ OPCHDA_EDITTYPE __RPC_FAR *pEditType;
    /* [size_is] */ LPWSTR __RPC_FAR *szUser;
    }	OPCHDA_MODIFIEDITEM;

typedef struct  tagOPCHDA_ATTRIBUTE
    {
    OPCHANDLE hClient;
    DWORD dwNumValues;
    DWORD dwAttributeID;
    /* [size_is] */ FILETIME __RPC_FAR *ftTimeStamps;
    /* [size_is] */ VARIANT __RPC_FAR *vAttributeValues;
    }	OPCHDA_ATTRIBUTE;

typedef struct  tagOPCHDA_TIME
    {
    BOOL bString;
    /* [string] */ LPWSTR szTime;
    FILETIME ftTime;
    }	OPCHDA_TIME;

typedef struct  tagOPCHDA_ITEM
    {
    OPCHANDLE hClient;
    OPCHDA_AGGREGATE haAggregate;
    DWORD dwCount;
    /* [size_is] */ FILETIME __RPC_FAR *pftTimeStamps;
    /* [size_is] */ DWORD __RPC_FAR *pdwQualities;
    /* [size_is] */ VARIANT __RPC_FAR *pvDataValues;
    }	OPCHDA_ITEM;

#define    OPCHDA_DATA_TYPE			0x01
#define    OPCHDA_DESCRIPTION		0x02
#define    OPCHDA_ENG_UNITS			0x03
#define    OPCHDA_STEPPED			0x04
#define    OPCHDA_ARCHIVING			0x05
#define    OPCHDA_DERIVE_EQUATION	0x06
#define    OPCHDA_NODE_NAME			0x07
#define    OPCHDA_PROCESS_NAME		0x08
#define    OPCHDA_SOURCE_NAME		0x09
#define    OPCHDA_SOURCE_TYPE		0x0a
#define    OPCHDA_NORMAL_MAXIMUM		0x0b
#define    OPCHDA_NORMAL_MINIMUM		0x0c
#define    OPCHDA_ITEMID				0x0d
#define    OPCHDA_MAX_TIME_INT			0x0e
#define    OPCHDA_MIN_TIME_INT			0x0f
#define    OPCHDA_EXCEPTION_DEV			0x10
#define    OPCHDA_EXCEPTION_DEV_TYPE		0x11
#define    OPCHDA_HIGH_ENTRY_LIMIT		0x12
#define    OPCHDA_LOW_ENTRY_LIMIT		0x13
#define    OPCHDA_EXTRADATA		0x0001
#define    OPCHDA_INTERPOLATED	0x0002
#define    OPCHDA_RAW			0x0004
#define    OPCHDA_CALCULATED		0x0008
#define    OPCHDA_NOBOUND		0x0010
#define    OPCHDA_NODATA			0x0020
#define    OPCHDA_DATALOST		0x0040
#define    OPCHDA_CONVERSION		0x0080


extern RPC_IF_HANDLE __MIDL_itf_opchda_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_opchda_0000_v0_0_s_ifspec;

#ifndef __IOPCHDA_Browser_INTERFACE_DEFINED__
#define __IOPCHDA_Browser_INTERFACE_DEFINED__

/* interface IOPCHDA_Browser */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCHDA_Browser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1F1217B1-DEE0-11d2-A5E5-000086339399")
    IOPCHDA_Browser : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetEnum( 
            /* [in] */ OPCHDA_BROWSETYPE dwBrowseType,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ChangeBrowsePosition( 
            /* [in] */ OPCHDA_BROWSEDIRECTION dwBrowseDirection,
            /* [string][in] */ LPCWSTR szString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetItemID( 
            /* [string][in] */ LPCWSTR szNode,
            /* [string][out] */ LPWSTR __RPC_FAR *pszItemID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBranchPosition( 
            /* [string][out] */ LPWSTR __RPC_FAR *pszBranchPos) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCHDA_BrowserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCHDA_Browser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCHDA_Browser __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCHDA_Browser __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEnum )( 
            IOPCHDA_Browser __RPC_FAR * This,
            /* [in] */ OPCHDA_BROWSETYPE dwBrowseType,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ChangeBrowsePosition )( 
            IOPCHDA_Browser __RPC_FAR * This,
            /* [in] */ OPCHDA_BROWSEDIRECTION dwBrowseDirection,
            /* [string][in] */ LPCWSTR szString);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItemID )( 
            IOPCHDA_Browser __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szNode,
            /* [string][out] */ LPWSTR __RPC_FAR *pszItemID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBranchPosition )( 
            IOPCHDA_Browser __RPC_FAR * This,
            /* [string][out] */ LPWSTR __RPC_FAR *pszBranchPos);
        
        END_INTERFACE
    } IOPCHDA_BrowserVtbl;

    interface IOPCHDA_Browser
    {
        CONST_VTBL struct IOPCHDA_BrowserVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCHDA_Browser_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCHDA_Browser_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCHDA_Browser_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCHDA_Browser_GetEnum(This,dwBrowseType,ppIEnumString)	\
    (This)->lpVtbl -> GetEnum(This,dwBrowseType,ppIEnumString)

#define IOPCHDA_Browser_ChangeBrowsePosition(This,dwBrowseDirection,szString)	\
    (This)->lpVtbl -> ChangeBrowsePosition(This,dwBrowseDirection,szString)

#define IOPCHDA_Browser_GetItemID(This,szNode,pszItemID)	\
    (This)->lpVtbl -> GetItemID(This,szNode,pszItemID)

#define IOPCHDA_Browser_GetBranchPosition(This,pszBranchPos)	\
    (This)->lpVtbl -> GetBranchPosition(This,pszBranchPos)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCHDA_Browser_GetEnum_Proxy( 
    IOPCHDA_Browser __RPC_FAR * This,
    /* [in] */ OPCHDA_BROWSETYPE dwBrowseType,
    /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);


void __RPC_STUB IOPCHDA_Browser_GetEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_Browser_ChangeBrowsePosition_Proxy( 
    IOPCHDA_Browser __RPC_FAR * This,
    /* [in] */ OPCHDA_BROWSEDIRECTION dwBrowseDirection,
    /* [string][in] */ LPCWSTR szString);


void __RPC_STUB IOPCHDA_Browser_ChangeBrowsePosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_Browser_GetItemID_Proxy( 
    IOPCHDA_Browser __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szNode,
    /* [string][out] */ LPWSTR __RPC_FAR *pszItemID);


void __RPC_STUB IOPCHDA_Browser_GetItemID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_Browser_GetBranchPosition_Proxy( 
    IOPCHDA_Browser __RPC_FAR * This,
    /* [string][out] */ LPWSTR __RPC_FAR *pszBranchPos);


void __RPC_STUB IOPCHDA_Browser_GetBranchPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCHDA_Browser_INTERFACE_DEFINED__ */


#ifndef __IOPCHDA_Server_INTERFACE_DEFINED__
#define __IOPCHDA_Server_INTERFACE_DEFINED__

/* interface IOPCHDA_Server */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCHDA_Server;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1F1217B0-DEE0-11d2-A5E5-000086339399")
    IOPCHDA_Server : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetItemAttributes( 
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwAttrID,
            /* [string][size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAttrName,
            /* [string][size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAttrDesc,
            /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppvtAttrDataType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAggregates( 
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwAggrID,
            /* [string][size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAggrName,
            /* [string][size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAggrDesc) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetHistorianStatus( 
            /* [out] */ OPCHDA_SERVERSTATUS __RPC_FAR *pwStatus,
            /* [out] */ FILETIME __RPC_FAR *__RPC_FAR *pftCurrentTime,
            /* [out] */ FILETIME __RPC_FAR *__RPC_FAR *pftStartTime,
            /* [out] */ WORD __RPC_FAR *pwMajorVersion,
            /* [out] */ WORD __RPC_FAR *pwMinorVersion,
            /* [out] */ WORD __RPC_FAR *pwBuildNumber,
            /* [out] */ DWORD __RPC_FAR *pdwMaxReturnValues,
            /* [string][out] */ LPWSTR __RPC_FAR *ppszStatusString,
            /* [string][out] */ LPWSTR __RPC_FAR *ppszVendorInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetItemHandles( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszItemID,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClient,
            /* [size_is][size_is][out] */ OPCHANDLE __RPC_FAR *__RPC_FAR *pphServer,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReleaseItemHandles( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ValidateItemIDs( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszItemID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateBrowse( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwAttrID,
            /* [size_is][in] */ OPCHDA_OPERATORCODES __RPC_FAR *pOperator,
            /* [size_is][in] */ VARIANT __RPC_FAR *vFilter,
            /* [out] */ IOPCHDA_Browser __RPC_FAR *__RPC_FAR *pphBrowser,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCHDA_ServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCHDA_Server __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCHDA_Server __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCHDA_Server __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItemAttributes )( 
            IOPCHDA_Server __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwAttrID,
            /* [string][size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAttrName,
            /* [string][size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAttrDesc,
            /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppvtAttrDataType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAggregates )( 
            IOPCHDA_Server __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwAggrID,
            /* [string][size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAggrName,
            /* [string][size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAggrDesc);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHistorianStatus )( 
            IOPCHDA_Server __RPC_FAR * This,
            /* [out] */ OPCHDA_SERVERSTATUS __RPC_FAR *pwStatus,
            /* [out] */ FILETIME __RPC_FAR *__RPC_FAR *pftCurrentTime,
            /* [out] */ FILETIME __RPC_FAR *__RPC_FAR *pftStartTime,
            /* [out] */ WORD __RPC_FAR *pwMajorVersion,
            /* [out] */ WORD __RPC_FAR *pwMinorVersion,
            /* [out] */ WORD __RPC_FAR *pwBuildNumber,
            /* [out] */ DWORD __RPC_FAR *pdwMaxReturnValues,
            /* [string][out] */ LPWSTR __RPC_FAR *ppszStatusString,
            /* [string][out] */ LPWSTR __RPC_FAR *ppszVendorInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItemHandles )( 
            IOPCHDA_Server __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszItemID,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClient,
            /* [size_is][size_is][out] */ OPCHANDLE __RPC_FAR *__RPC_FAR *pphServer,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReleaseItemHandles )( 
            IOPCHDA_Server __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateItemIDs )( 
            IOPCHDA_Server __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszItemID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateBrowse )( 
            IOPCHDA_Server __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwAttrID,
            /* [size_is][in] */ OPCHDA_OPERATORCODES __RPC_FAR *pOperator,
            /* [size_is][in] */ VARIANT __RPC_FAR *vFilter,
            /* [out] */ IOPCHDA_Browser __RPC_FAR *__RPC_FAR *pphBrowser,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        END_INTERFACE
    } IOPCHDA_ServerVtbl;

    interface IOPCHDA_Server
    {
        CONST_VTBL struct IOPCHDA_ServerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCHDA_Server_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCHDA_Server_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCHDA_Server_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCHDA_Server_GetItemAttributes(This,pdwCount,ppdwAttrID,ppszAttrName,ppszAttrDesc,ppvtAttrDataType)	\
    (This)->lpVtbl -> GetItemAttributes(This,pdwCount,ppdwAttrID,ppszAttrName,ppszAttrDesc,ppvtAttrDataType)

#define IOPCHDA_Server_GetAggregates(This,pdwCount,ppdwAggrID,ppszAggrName,ppszAggrDesc)	\
    (This)->lpVtbl -> GetAggregates(This,pdwCount,ppdwAggrID,ppszAggrName,ppszAggrDesc)

#define IOPCHDA_Server_GetHistorianStatus(This,pwStatus,pftCurrentTime,pftStartTime,pwMajorVersion,pwMinorVersion,pwBuildNumber,pdwMaxReturnValues,ppszStatusString,ppszVendorInfo)	\
    (This)->lpVtbl -> GetHistorianStatus(This,pwStatus,pftCurrentTime,pftStartTime,pwMajorVersion,pwMinorVersion,pwBuildNumber,pdwMaxReturnValues,ppszStatusString,ppszVendorInfo)

#define IOPCHDA_Server_GetItemHandles(This,dwCount,pszItemID,phClient,pphServer,ppErrors)	\
    (This)->lpVtbl -> GetItemHandles(This,dwCount,pszItemID,phClient,pphServer,ppErrors)

#define IOPCHDA_Server_ReleaseItemHandles(This,dwCount,phServer,ppErrors)	\
    (This)->lpVtbl -> ReleaseItemHandles(This,dwCount,phServer,ppErrors)

#define IOPCHDA_Server_ValidateItemIDs(This,dwCount,pszItemID,ppErrors)	\
    (This)->lpVtbl -> ValidateItemIDs(This,dwCount,pszItemID,ppErrors)

#define IOPCHDA_Server_CreateBrowse(This,dwCount,pdwAttrID,pOperator,vFilter,pphBrowser,ppErrors)	\
    (This)->lpVtbl -> CreateBrowse(This,dwCount,pdwAttrID,pOperator,vFilter,pphBrowser,ppErrors)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCHDA_Server_GetItemAttributes_Proxy( 
    IOPCHDA_Server __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwAttrID,
    /* [string][size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAttrName,
    /* [string][size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAttrDesc,
    /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppvtAttrDataType);


void __RPC_STUB IOPCHDA_Server_GetItemAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_Server_GetAggregates_Proxy( 
    IOPCHDA_Server __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppdwAggrID,
    /* [string][size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAggrName,
    /* [string][size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszAggrDesc);


void __RPC_STUB IOPCHDA_Server_GetAggregates_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_Server_GetHistorianStatus_Proxy( 
    IOPCHDA_Server __RPC_FAR * This,
    /* [out] */ OPCHDA_SERVERSTATUS __RPC_FAR *pwStatus,
    /* [out] */ FILETIME __RPC_FAR *__RPC_FAR *pftCurrentTime,
    /* [out] */ FILETIME __RPC_FAR *__RPC_FAR *pftStartTime,
    /* [out] */ WORD __RPC_FAR *pwMajorVersion,
    /* [out] */ WORD __RPC_FAR *pwMinorVersion,
    /* [out] */ WORD __RPC_FAR *pwBuildNumber,
    /* [out] */ DWORD __RPC_FAR *pdwMaxReturnValues,
    /* [string][out] */ LPWSTR __RPC_FAR *ppszStatusString,
    /* [string][out] */ LPWSTR __RPC_FAR *ppszVendorInfo);


void __RPC_STUB IOPCHDA_Server_GetHistorianStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_Server_GetItemHandles_Proxy( 
    IOPCHDA_Server __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ LPWSTR __RPC_FAR *pszItemID,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClient,
    /* [size_is][size_is][out] */ OPCHANDLE __RPC_FAR *__RPC_FAR *pphServer,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_Server_GetItemHandles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_Server_ReleaseItemHandles_Proxy( 
    IOPCHDA_Server __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_Server_ReleaseItemHandles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_Server_ValidateItemIDs_Proxy( 
    IOPCHDA_Server __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ LPWSTR __RPC_FAR *pszItemID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_Server_ValidateItemIDs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_Server_CreateBrowse_Proxy( 
    IOPCHDA_Server __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwAttrID,
    /* [size_is][in] */ OPCHDA_OPERATORCODES __RPC_FAR *pOperator,
    /* [size_is][in] */ VARIANT __RPC_FAR *vFilter,
    /* [out] */ IOPCHDA_Browser __RPC_FAR *__RPC_FAR *pphBrowser,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_Server_CreateBrowse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCHDA_Server_INTERFACE_DEFINED__ */


#ifndef __IOPCHDA_SyncRead_INTERFACE_DEFINED__
#define __IOPCHDA_SyncRead_INTERFACE_DEFINED__

/* interface IOPCHDA_SyncRead */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCHDA_SyncRead;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1F1217B2-DEE0-11d2-A5E5-000086339399")
    IOPCHDA_SyncRead : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ReadRaw( 
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumValues,
            /* [in] */ BOOL bBounds,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ OPCHDA_ITEM __RPC_FAR *__RPC_FAR *ppItemValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadProcessed( 
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ FILETIME ftResampleInterval,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ OPCHDA_AGGREGATE __RPC_FAR *haAggregate,
            /* [size_is][size_is][out] */ OPCHDA_ITEM __RPC_FAR *__RPC_FAR *ppItemValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadAtTime( 
            /* [in] */ DWORD dwNumTimeStamps,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ OPCHDA_ITEM __RPC_FAR *__RPC_FAR *ppItemValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadModified( 
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumValues,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ OPCHDA_MODIFIEDITEM __RPC_FAR *__RPC_FAR *ppItemValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadAttribute( 
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ OPCHANDLE hServer,
            /* [in] */ DWORD dwNumAttributes,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwAttributeIDs,
            /* [size_is][size_is][out] */ OPCHDA_ATTRIBUTE __RPC_FAR *__RPC_FAR *ppAttributeValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCHDA_SyncReadVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCHDA_SyncRead __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCHDA_SyncRead __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCHDA_SyncRead __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadRaw )( 
            IOPCHDA_SyncRead __RPC_FAR * This,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumValues,
            /* [in] */ BOOL bBounds,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ OPCHDA_ITEM __RPC_FAR *__RPC_FAR *ppItemValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadProcessed )( 
            IOPCHDA_SyncRead __RPC_FAR * This,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ FILETIME ftResampleInterval,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ OPCHDA_AGGREGATE __RPC_FAR *haAggregate,
            /* [size_is][size_is][out] */ OPCHDA_ITEM __RPC_FAR *__RPC_FAR *ppItemValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadAtTime )( 
            IOPCHDA_SyncRead __RPC_FAR * This,
            /* [in] */ DWORD dwNumTimeStamps,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ OPCHDA_ITEM __RPC_FAR *__RPC_FAR *ppItemValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadModified )( 
            IOPCHDA_SyncRead __RPC_FAR * This,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumValues,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ OPCHDA_MODIFIEDITEM __RPC_FAR *__RPC_FAR *ppItemValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadAttribute )( 
            IOPCHDA_SyncRead __RPC_FAR * This,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ OPCHANDLE hServer,
            /* [in] */ DWORD dwNumAttributes,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwAttributeIDs,
            /* [size_is][size_is][out] */ OPCHDA_ATTRIBUTE __RPC_FAR *__RPC_FAR *ppAttributeValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        END_INTERFACE
    } IOPCHDA_SyncReadVtbl;

    interface IOPCHDA_SyncRead
    {
        CONST_VTBL struct IOPCHDA_SyncReadVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCHDA_SyncRead_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCHDA_SyncRead_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCHDA_SyncRead_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCHDA_SyncRead_ReadRaw(This,htStartTime,htEndTime,dwNumValues,bBounds,dwNumItems,phServer,ppItemValues,ppErrors)	\
    (This)->lpVtbl -> ReadRaw(This,htStartTime,htEndTime,dwNumValues,bBounds,dwNumItems,phServer,ppItemValues,ppErrors)

#define IOPCHDA_SyncRead_ReadProcessed(This,htStartTime,htEndTime,ftResampleInterval,dwNumItems,phServer,haAggregate,ppItemValues,ppErrors)	\
    (This)->lpVtbl -> ReadProcessed(This,htStartTime,htEndTime,ftResampleInterval,dwNumItems,phServer,haAggregate,ppItemValues,ppErrors)

#define IOPCHDA_SyncRead_ReadAtTime(This,dwNumTimeStamps,ftTimeStamps,dwNumItems,phServer,ppItemValues,ppErrors)	\
    (This)->lpVtbl -> ReadAtTime(This,dwNumTimeStamps,ftTimeStamps,dwNumItems,phServer,ppItemValues,ppErrors)

#define IOPCHDA_SyncRead_ReadModified(This,htStartTime,htEndTime,dwNumValues,dwNumItems,phServer,ppItemValues,ppErrors)	\
    (This)->lpVtbl -> ReadModified(This,htStartTime,htEndTime,dwNumValues,dwNumItems,phServer,ppItemValues,ppErrors)

#define IOPCHDA_SyncRead_ReadAttribute(This,htStartTime,htEndTime,hServer,dwNumAttributes,pdwAttributeIDs,ppAttributeValues,ppErrors)	\
    (This)->lpVtbl -> ReadAttribute(This,htStartTime,htEndTime,hServer,dwNumAttributes,pdwAttributeIDs,ppAttributeValues,ppErrors)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCHDA_SyncRead_ReadRaw_Proxy( 
    IOPCHDA_SyncRead __RPC_FAR * This,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
    /* [in] */ DWORD dwNumValues,
    /* [in] */ BOOL bBounds,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][size_is][out] */ OPCHDA_ITEM __RPC_FAR *__RPC_FAR *ppItemValues,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_SyncRead_ReadRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_SyncRead_ReadProcessed_Proxy( 
    IOPCHDA_SyncRead __RPC_FAR * This,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
    /* [in] */ FILETIME ftResampleInterval,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ OPCHDA_AGGREGATE __RPC_FAR *haAggregate,
    /* [size_is][size_is][out] */ OPCHDA_ITEM __RPC_FAR *__RPC_FAR *ppItemValues,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_SyncRead_ReadProcessed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_SyncRead_ReadAtTime_Proxy( 
    IOPCHDA_SyncRead __RPC_FAR * This,
    /* [in] */ DWORD dwNumTimeStamps,
    /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][size_is][out] */ OPCHDA_ITEM __RPC_FAR *__RPC_FAR *ppItemValues,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_SyncRead_ReadAtTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_SyncRead_ReadModified_Proxy( 
    IOPCHDA_SyncRead __RPC_FAR * This,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
    /* [in] */ DWORD dwNumValues,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][size_is][out] */ OPCHDA_MODIFIEDITEM __RPC_FAR *__RPC_FAR *ppItemValues,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_SyncRead_ReadModified_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_SyncRead_ReadAttribute_Proxy( 
    IOPCHDA_SyncRead __RPC_FAR * This,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
    /* [in] */ OPCHANDLE hServer,
    /* [in] */ DWORD dwNumAttributes,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwAttributeIDs,
    /* [size_is][size_is][out] */ OPCHDA_ATTRIBUTE __RPC_FAR *__RPC_FAR *ppAttributeValues,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_SyncRead_ReadAttribute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCHDA_SyncRead_INTERFACE_DEFINED__ */


#ifndef __IOPCHDA_SyncUpdate_INTERFACE_DEFINED__
#define __IOPCHDA_SyncUpdate_INTERFACE_DEFINED__

/* interface IOPCHDA_SyncUpdate */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCHDA_SyncUpdate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1F1217B3-DEE0-11d2-A5E5-000086339399")
    IOPCHDA_SyncUpdate : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QueryCapabilities( 
            /* [out] */ OPCHDA_UPDATECAPABILITIES __RPC_FAR *pCapabilities) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Insert( 
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Replace( 
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
            /* [size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InsertReplace( 
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
            /* [size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteRaw( 
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteAtTime( 
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCHDA_SyncUpdateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCHDA_SyncUpdate __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCHDA_SyncUpdate __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCHDA_SyncUpdate __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryCapabilities )( 
            IOPCHDA_SyncUpdate __RPC_FAR * This,
            /* [out] */ OPCHDA_UPDATECAPABILITIES __RPC_FAR *pCapabilities);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Insert )( 
            IOPCHDA_SyncUpdate __RPC_FAR * This,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Replace )( 
            IOPCHDA_SyncUpdate __RPC_FAR * This,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
            /* [size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertReplace )( 
            IOPCHDA_SyncUpdate __RPC_FAR * This,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
            /* [size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteRaw )( 
            IOPCHDA_SyncUpdate __RPC_FAR * This,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteAtTime )( 
            IOPCHDA_SyncUpdate __RPC_FAR * This,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        END_INTERFACE
    } IOPCHDA_SyncUpdateVtbl;

    interface IOPCHDA_SyncUpdate
    {
        CONST_VTBL struct IOPCHDA_SyncUpdateVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCHDA_SyncUpdate_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCHDA_SyncUpdate_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCHDA_SyncUpdate_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCHDA_SyncUpdate_QueryCapabilities(This,pCapabilities)	\
    (This)->lpVtbl -> QueryCapabilities(This,pCapabilities)

#define IOPCHDA_SyncUpdate_Insert(This,dwNumItems,phServer,ftTimeStamps,vDataValues,pdwQualities,ppErrors)	\
    (This)->lpVtbl -> Insert(This,dwNumItems,phServer,ftTimeStamps,vDataValues,pdwQualities,ppErrors)

#define IOPCHDA_SyncUpdate_Replace(This,dwNumItems,phServer,ftTimeStamps,vDataValues,pdwQualities,ppErrors)	\
    (This)->lpVtbl -> Replace(This,dwNumItems,phServer,ftTimeStamps,vDataValues,pdwQualities,ppErrors)

#define IOPCHDA_SyncUpdate_InsertReplace(This,dwNumItems,phServer,ftTimeStamps,vDataValues,pdwQualities,ppErrors)	\
    (This)->lpVtbl -> InsertReplace(This,dwNumItems,phServer,ftTimeStamps,vDataValues,pdwQualities,ppErrors)

#define IOPCHDA_SyncUpdate_DeleteRaw(This,htStartTime,htEndTime,dwNumItems,phServer,ppErrors)	\
    (This)->lpVtbl -> DeleteRaw(This,htStartTime,htEndTime,dwNumItems,phServer,ppErrors)

#define IOPCHDA_SyncUpdate_DeleteAtTime(This,dwNumItems,phServer,ftTimeStamps,ppErrors)	\
    (This)->lpVtbl -> DeleteAtTime(This,dwNumItems,phServer,ftTimeStamps,ppErrors)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCHDA_SyncUpdate_QueryCapabilities_Proxy( 
    IOPCHDA_SyncUpdate __RPC_FAR * This,
    /* [out] */ OPCHDA_UPDATECAPABILITIES __RPC_FAR *pCapabilities);


void __RPC_STUB IOPCHDA_SyncUpdate_QueryCapabilities_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_SyncUpdate_Insert_Proxy( 
    IOPCHDA_SyncUpdate __RPC_FAR * This,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
    /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_SyncUpdate_Insert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_SyncUpdate_Replace_Proxy( 
    IOPCHDA_SyncUpdate __RPC_FAR * This,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
    /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
    /* [size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_SyncUpdate_Replace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_SyncUpdate_InsertReplace_Proxy( 
    IOPCHDA_SyncUpdate __RPC_FAR * This,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
    /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
    /* [size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_SyncUpdate_InsertReplace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_SyncUpdate_DeleteRaw_Proxy( 
    IOPCHDA_SyncUpdate __RPC_FAR * This,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_SyncUpdate_DeleteRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_SyncUpdate_DeleteAtTime_Proxy( 
    IOPCHDA_SyncUpdate __RPC_FAR * This,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
    /* [size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_SyncUpdate_DeleteAtTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCHDA_SyncUpdate_INTERFACE_DEFINED__ */


#ifndef __IOPCHDA_SyncAnnotations_INTERFACE_DEFINED__
#define __IOPCHDA_SyncAnnotations_INTERFACE_DEFINED__

/* interface IOPCHDA_SyncAnnotations */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCHDA_SyncAnnotations;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1F1217B4-DEE0-11d2-A5E5-000086339399")
    IOPCHDA_SyncAnnotations : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QueryCapabilities( 
            /* [out] */ OPCHDA_ANNOTATIONCAPABILITIES __RPC_FAR *pCapabilities) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Read( 
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ OPCHDA_ANNOTATION __RPC_FAR *__RPC_FAR *ppAnnotationValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Insert( 
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ OPCHDA_ANNOTATION __RPC_FAR *pAnnotationValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCHDA_SyncAnnotationsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCHDA_SyncAnnotations __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCHDA_SyncAnnotations __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCHDA_SyncAnnotations __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryCapabilities )( 
            IOPCHDA_SyncAnnotations __RPC_FAR * This,
            /* [out] */ OPCHDA_ANNOTATIONCAPABILITIES __RPC_FAR *pCapabilities);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Read )( 
            IOPCHDA_SyncAnnotations __RPC_FAR * This,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ OPCHDA_ANNOTATION __RPC_FAR *__RPC_FAR *ppAnnotationValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Insert )( 
            IOPCHDA_SyncAnnotations __RPC_FAR * This,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ OPCHDA_ANNOTATION __RPC_FAR *pAnnotationValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        END_INTERFACE
    } IOPCHDA_SyncAnnotationsVtbl;

    interface IOPCHDA_SyncAnnotations
    {
        CONST_VTBL struct IOPCHDA_SyncAnnotationsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCHDA_SyncAnnotations_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCHDA_SyncAnnotations_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCHDA_SyncAnnotations_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCHDA_SyncAnnotations_QueryCapabilities(This,pCapabilities)	\
    (This)->lpVtbl -> QueryCapabilities(This,pCapabilities)

#define IOPCHDA_SyncAnnotations_Read(This,htStartTime,htEndTime,dwNumItems,phServer,ppAnnotationValues,ppErrors)	\
    (This)->lpVtbl -> Read(This,htStartTime,htEndTime,dwNumItems,phServer,ppAnnotationValues,ppErrors)

#define IOPCHDA_SyncAnnotations_Insert(This,dwNumItems,phServer,ftTimeStamps,pAnnotationValues,ppErrors)	\
    (This)->lpVtbl -> Insert(This,dwNumItems,phServer,ftTimeStamps,pAnnotationValues,ppErrors)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCHDA_SyncAnnotations_QueryCapabilities_Proxy( 
    IOPCHDA_SyncAnnotations __RPC_FAR * This,
    /* [out] */ OPCHDA_ANNOTATIONCAPABILITIES __RPC_FAR *pCapabilities);


void __RPC_STUB IOPCHDA_SyncAnnotations_QueryCapabilities_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_SyncAnnotations_Read_Proxy( 
    IOPCHDA_SyncAnnotations __RPC_FAR * This,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][size_is][out] */ OPCHDA_ANNOTATION __RPC_FAR *__RPC_FAR *ppAnnotationValues,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_SyncAnnotations_Read_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_SyncAnnotations_Insert_Proxy( 
    IOPCHDA_SyncAnnotations __RPC_FAR * This,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
    /* [size_is][in] */ OPCHDA_ANNOTATION __RPC_FAR *pAnnotationValues,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_SyncAnnotations_Insert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCHDA_SyncAnnotations_INTERFACE_DEFINED__ */


#ifndef __IOPCHDA_AsyncRead_INTERFACE_DEFINED__
#define __IOPCHDA_AsyncRead_INTERFACE_DEFINED__

/* interface IOPCHDA_AsyncRead */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCHDA_AsyncRead;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1F1217B5-DEE0-11d2-A5E5-000086339399")
    IOPCHDA_AsyncRead : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ReadRaw( 
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumValues,
            /* [in] */ BOOL bBounds,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AdviseRaw( 
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [in] */ FILETIME ftUpdateInterval,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadProcessed( 
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ FILETIME ftResampleInterval,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ OPCHDA_AGGREGATE __RPC_FAR *haAggregate,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AdviseProcessed( 
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [in] */ FILETIME ftResampleInterval,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ OPCHDA_AGGREGATE __RPC_FAR *haAggregate,
            /* [in] */ DWORD dwNumIntervals,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadAtTime( 
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ DWORD dwNumTimeStamps,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadModified( 
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumValues,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadAttribute( 
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ OPCHANDLE hServer,
            /* [in] */ DWORD dwNumAttributes,
            /* [size_is][in] */ DWORD __RPC_FAR *dwAttributeIDs,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Cancel( 
            /* [in] */ DWORD dwCancelID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCHDA_AsyncReadVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCHDA_AsyncRead __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCHDA_AsyncRead __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCHDA_AsyncRead __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadRaw )( 
            IOPCHDA_AsyncRead __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumValues,
            /* [in] */ BOOL bBounds,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AdviseRaw )( 
            IOPCHDA_AsyncRead __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [in] */ FILETIME ftUpdateInterval,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadProcessed )( 
            IOPCHDA_AsyncRead __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ FILETIME ftResampleInterval,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ OPCHDA_AGGREGATE __RPC_FAR *haAggregate,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AdviseProcessed )( 
            IOPCHDA_AsyncRead __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [in] */ FILETIME ftResampleInterval,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ OPCHDA_AGGREGATE __RPC_FAR *haAggregate,
            /* [in] */ DWORD dwNumIntervals,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadAtTime )( 
            IOPCHDA_AsyncRead __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ DWORD dwNumTimeStamps,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadModified )( 
            IOPCHDA_AsyncRead __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumValues,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadAttribute )( 
            IOPCHDA_AsyncRead __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ OPCHANDLE hServer,
            /* [in] */ DWORD dwNumAttributes,
            /* [size_is][in] */ DWORD __RPC_FAR *dwAttributeIDs,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Cancel )( 
            IOPCHDA_AsyncRead __RPC_FAR * This,
            /* [in] */ DWORD dwCancelID);
        
        END_INTERFACE
    } IOPCHDA_AsyncReadVtbl;

    interface IOPCHDA_AsyncRead
    {
        CONST_VTBL struct IOPCHDA_AsyncReadVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCHDA_AsyncRead_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCHDA_AsyncRead_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCHDA_AsyncRead_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCHDA_AsyncRead_ReadRaw(This,dwTransactionID,htStartTime,htEndTime,dwNumValues,bBounds,dwNumItems,phServer,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> ReadRaw(This,dwTransactionID,htStartTime,htEndTime,dwNumValues,bBounds,dwNumItems,phServer,pdwCancelID,ppErrors)

#define IOPCHDA_AsyncRead_AdviseRaw(This,dwTransactionID,htStartTime,ftUpdateInterval,dwNumItems,phServer,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> AdviseRaw(This,dwTransactionID,htStartTime,ftUpdateInterval,dwNumItems,phServer,pdwCancelID,ppErrors)

#define IOPCHDA_AsyncRead_ReadProcessed(This,dwTransactionID,htStartTime,htEndTime,ftResampleInterval,dwNumItems,phServer,haAggregate,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> ReadProcessed(This,dwTransactionID,htStartTime,htEndTime,ftResampleInterval,dwNumItems,phServer,haAggregate,pdwCancelID,ppErrors)

#define IOPCHDA_AsyncRead_AdviseProcessed(This,dwTransactionID,htStartTime,ftResampleInterval,dwNumItems,phServer,haAggregate,dwNumIntervals,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> AdviseProcessed(This,dwTransactionID,htStartTime,ftResampleInterval,dwNumItems,phServer,haAggregate,dwNumIntervals,pdwCancelID,ppErrors)

#define IOPCHDA_AsyncRead_ReadAtTime(This,dwTransactionID,dwNumTimeStamps,ftTimeStamps,dwNumItems,phServer,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> ReadAtTime(This,dwTransactionID,dwNumTimeStamps,ftTimeStamps,dwNumItems,phServer,pdwCancelID,ppErrors)

#define IOPCHDA_AsyncRead_ReadModified(This,dwTransactionID,htStartTime,htEndTime,dwNumValues,dwNumItems,phServer,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> ReadModified(This,dwTransactionID,htStartTime,htEndTime,dwNumValues,dwNumItems,phServer,pdwCancelID,ppErrors)

#define IOPCHDA_AsyncRead_ReadAttribute(This,dwTransactionID,htStartTime,htEndTime,hServer,dwNumAttributes,dwAttributeIDs,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> ReadAttribute(This,dwTransactionID,htStartTime,htEndTime,hServer,dwNumAttributes,dwAttributeIDs,pdwCancelID,ppErrors)

#define IOPCHDA_AsyncRead_Cancel(This,dwCancelID)	\
    (This)->lpVtbl -> Cancel(This,dwCancelID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncRead_ReadRaw_Proxy( 
    IOPCHDA_AsyncRead __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
    /* [in] */ DWORD dwNumValues,
    /* [in] */ BOOL bBounds,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_AsyncRead_ReadRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncRead_AdviseRaw_Proxy( 
    IOPCHDA_AsyncRead __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [in] */ FILETIME ftUpdateInterval,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_AsyncRead_AdviseRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncRead_ReadProcessed_Proxy( 
    IOPCHDA_AsyncRead __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
    /* [in] */ FILETIME ftResampleInterval,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ OPCHDA_AGGREGATE __RPC_FAR *haAggregate,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_AsyncRead_ReadProcessed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncRead_AdviseProcessed_Proxy( 
    IOPCHDA_AsyncRead __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [in] */ FILETIME ftResampleInterval,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ OPCHDA_AGGREGATE __RPC_FAR *haAggregate,
    /* [in] */ DWORD dwNumIntervals,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_AsyncRead_AdviseProcessed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncRead_ReadAtTime_Proxy( 
    IOPCHDA_AsyncRead __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [in] */ DWORD dwNumTimeStamps,
    /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_AsyncRead_ReadAtTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncRead_ReadModified_Proxy( 
    IOPCHDA_AsyncRead __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
    /* [in] */ DWORD dwNumValues,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_AsyncRead_ReadModified_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncRead_ReadAttribute_Proxy( 
    IOPCHDA_AsyncRead __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
    /* [in] */ OPCHANDLE hServer,
    /* [in] */ DWORD dwNumAttributes,
    /* [size_is][in] */ DWORD __RPC_FAR *dwAttributeIDs,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_AsyncRead_ReadAttribute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncRead_Cancel_Proxy( 
    IOPCHDA_AsyncRead __RPC_FAR * This,
    /* [in] */ DWORD dwCancelID);


void __RPC_STUB IOPCHDA_AsyncRead_Cancel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCHDA_AsyncRead_INTERFACE_DEFINED__ */


#ifndef __IOPCHDA_AsyncUpdate_INTERFACE_DEFINED__
#define __IOPCHDA_AsyncUpdate_INTERFACE_DEFINED__

/* interface IOPCHDA_AsyncUpdate */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCHDA_AsyncUpdate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1F1217B6-DEE0-11d2-A5E5-000086339399")
    IOPCHDA_AsyncUpdate : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QueryCapabilities( 
            /* [out] */ OPCHDA_UPDATECAPABILITIES __RPC_FAR *pCapabilities) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Insert( 
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Replace( 
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InsertReplace( 
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteRaw( 
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteAtTime( 
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Cancel( 
            /* [in] */ DWORD dwCancelID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCHDA_AsyncUpdateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCHDA_AsyncUpdate __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCHDA_AsyncUpdate __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCHDA_AsyncUpdate __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryCapabilities )( 
            IOPCHDA_AsyncUpdate __RPC_FAR * This,
            /* [out] */ OPCHDA_UPDATECAPABILITIES __RPC_FAR *pCapabilities);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Insert )( 
            IOPCHDA_AsyncUpdate __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Replace )( 
            IOPCHDA_AsyncUpdate __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertReplace )( 
            IOPCHDA_AsyncUpdate __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteRaw )( 
            IOPCHDA_AsyncUpdate __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteAtTime )( 
            IOPCHDA_AsyncUpdate __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Cancel )( 
            IOPCHDA_AsyncUpdate __RPC_FAR * This,
            /* [in] */ DWORD dwCancelID);
        
        END_INTERFACE
    } IOPCHDA_AsyncUpdateVtbl;

    interface IOPCHDA_AsyncUpdate
    {
        CONST_VTBL struct IOPCHDA_AsyncUpdateVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCHDA_AsyncUpdate_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCHDA_AsyncUpdate_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCHDA_AsyncUpdate_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCHDA_AsyncUpdate_QueryCapabilities(This,pCapabilities)	\
    (This)->lpVtbl -> QueryCapabilities(This,pCapabilities)

#define IOPCHDA_AsyncUpdate_Insert(This,dwTransactionID,dwNumItems,phServer,ftTimeStamps,vDataValues,pdwQualities,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> Insert(This,dwTransactionID,dwNumItems,phServer,ftTimeStamps,vDataValues,pdwQualities,pdwCancelID,ppErrors)

#define IOPCHDA_AsyncUpdate_Replace(This,dwTransactionID,dwNumItems,phServer,ftTimeStamps,vDataValues,pdwQualities,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> Replace(This,dwTransactionID,dwNumItems,phServer,ftTimeStamps,vDataValues,pdwQualities,pdwCancelID,ppErrors)

#define IOPCHDA_AsyncUpdate_InsertReplace(This,dwTransactionID,dwNumItems,phServer,ftTimeStamps,vDataValues,pdwQualities,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> InsertReplace(This,dwTransactionID,dwNumItems,phServer,ftTimeStamps,vDataValues,pdwQualities,pdwCancelID,ppErrors)

#define IOPCHDA_AsyncUpdate_DeleteRaw(This,dwTransactionID,htStartTime,htEndTime,dwNumItems,phServer,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> DeleteRaw(This,dwTransactionID,htStartTime,htEndTime,dwNumItems,phServer,pdwCancelID,ppErrors)

#define IOPCHDA_AsyncUpdate_DeleteAtTime(This,dwTransactionID,dwNumItems,phServer,ftTimeStamps,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> DeleteAtTime(This,dwTransactionID,dwNumItems,phServer,ftTimeStamps,pdwCancelID,ppErrors)

#define IOPCHDA_AsyncUpdate_Cancel(This,dwCancelID)	\
    (This)->lpVtbl -> Cancel(This,dwCancelID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncUpdate_QueryCapabilities_Proxy( 
    IOPCHDA_AsyncUpdate __RPC_FAR * This,
    /* [out] */ OPCHDA_UPDATECAPABILITIES __RPC_FAR *pCapabilities);


void __RPC_STUB IOPCHDA_AsyncUpdate_QueryCapabilities_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncUpdate_Insert_Proxy( 
    IOPCHDA_AsyncUpdate __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
    /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_AsyncUpdate_Insert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncUpdate_Replace_Proxy( 
    IOPCHDA_AsyncUpdate __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
    /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_AsyncUpdate_Replace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncUpdate_InsertReplace_Proxy( 
    IOPCHDA_AsyncUpdate __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
    /* [size_is][in] */ VARIANT __RPC_FAR *vDataValues,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwQualities,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_AsyncUpdate_InsertReplace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncUpdate_DeleteRaw_Proxy( 
    IOPCHDA_AsyncUpdate __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_AsyncUpdate_DeleteRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncUpdate_DeleteAtTime_Proxy( 
    IOPCHDA_AsyncUpdate __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_AsyncUpdate_DeleteAtTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncUpdate_Cancel_Proxy( 
    IOPCHDA_AsyncUpdate __RPC_FAR * This,
    /* [in] */ DWORD dwCancelID);


void __RPC_STUB IOPCHDA_AsyncUpdate_Cancel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCHDA_AsyncUpdate_INTERFACE_DEFINED__ */


#ifndef __IOPCHDA_AsyncAnnotations_INTERFACE_DEFINED__
#define __IOPCHDA_AsyncAnnotations_INTERFACE_DEFINED__

/* interface IOPCHDA_AsyncAnnotations */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCHDA_AsyncAnnotations;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1F1217B7-DEE0-11d2-A5E5-000086339399")
    IOPCHDA_AsyncAnnotations : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QueryCapabilities( 
            /* [out] */ OPCHDA_ANNOTATIONCAPABILITIES __RPC_FAR *pCapabilities) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Read( 
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Insert( 
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ OPCHDA_ANNOTATION __RPC_FAR *pAnnotationValues,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Cancel( 
            /* [in] */ DWORD dwCancelID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCHDA_AsyncAnnotationsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCHDA_AsyncAnnotations __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCHDA_AsyncAnnotations __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCHDA_AsyncAnnotations __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryCapabilities )( 
            IOPCHDA_AsyncAnnotations __RPC_FAR * This,
            /* [out] */ OPCHDA_ANNOTATIONCAPABILITIES __RPC_FAR *pCapabilities);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Read )( 
            IOPCHDA_AsyncAnnotations __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Insert )( 
            IOPCHDA_AsyncAnnotations __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
            /* [size_is][in] */ OPCHDA_ANNOTATION __RPC_FAR *pAnnotationValues,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Cancel )( 
            IOPCHDA_AsyncAnnotations __RPC_FAR * This,
            /* [in] */ DWORD dwCancelID);
        
        END_INTERFACE
    } IOPCHDA_AsyncAnnotationsVtbl;

    interface IOPCHDA_AsyncAnnotations
    {
        CONST_VTBL struct IOPCHDA_AsyncAnnotationsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCHDA_AsyncAnnotations_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCHDA_AsyncAnnotations_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCHDA_AsyncAnnotations_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCHDA_AsyncAnnotations_QueryCapabilities(This,pCapabilities)	\
    (This)->lpVtbl -> QueryCapabilities(This,pCapabilities)

#define IOPCHDA_AsyncAnnotations_Read(This,dwTransactionID,htStartTime,htEndTime,dwNumItems,phServer,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> Read(This,dwTransactionID,htStartTime,htEndTime,dwNumItems,phServer,pdwCancelID,ppErrors)

#define IOPCHDA_AsyncAnnotations_Insert(This,dwTransactionID,dwNumItems,phServer,ftTimeStamps,pAnnotationValues,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> Insert(This,dwTransactionID,dwNumItems,phServer,ftTimeStamps,pAnnotationValues,pdwCancelID,ppErrors)

#define IOPCHDA_AsyncAnnotations_Cancel(This,dwCancelID)	\
    (This)->lpVtbl -> Cancel(This,dwCancelID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncAnnotations_QueryCapabilities_Proxy( 
    IOPCHDA_AsyncAnnotations __RPC_FAR * This,
    /* [out] */ OPCHDA_ANNOTATIONCAPABILITIES __RPC_FAR *pCapabilities);


void __RPC_STUB IOPCHDA_AsyncAnnotations_QueryCapabilities_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncAnnotations_Read_Proxy( 
    IOPCHDA_AsyncAnnotations __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_AsyncAnnotations_Read_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncAnnotations_Insert_Proxy( 
    IOPCHDA_AsyncAnnotations __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ FILETIME __RPC_FAR *ftTimeStamps,
    /* [size_is][in] */ OPCHDA_ANNOTATION __RPC_FAR *pAnnotationValues,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_AsyncAnnotations_Insert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_AsyncAnnotations_Cancel_Proxy( 
    IOPCHDA_AsyncAnnotations __RPC_FAR * This,
    /* [in] */ DWORD dwCancelID);


void __RPC_STUB IOPCHDA_AsyncAnnotations_Cancel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCHDA_AsyncAnnotations_INTERFACE_DEFINED__ */


#ifndef __IOPCHDA_Playback_INTERFACE_DEFINED__
#define __IOPCHDA_Playback_INTERFACE_DEFINED__

/* interface IOPCHDA_Playback */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCHDA_Playback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1F1217B8-DEE0-11d2-A5E5-000086339399")
    IOPCHDA_Playback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ReadRawWithUpdate( 
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumValues,
            /* [in] */ FILETIME ftUpdateDuration,
            /* [in] */ FILETIME ftUpdateInterval,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadProcessedWithUpdate( 
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ FILETIME ftResampleInterval,
            /* [in] */ DWORD dwNumIntervals,
            /* [in] */ FILETIME ftUpdateInterval,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ OPCHDA_AGGREGATE __RPC_FAR *haAggregate,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Cancel( 
            /* [in] */ DWORD dwCancelID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCHDA_PlaybackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCHDA_Playback __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCHDA_Playback __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCHDA_Playback __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadRawWithUpdate )( 
            IOPCHDA_Playback __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ DWORD dwNumValues,
            /* [in] */ FILETIME ftUpdateDuration,
            /* [in] */ FILETIME ftUpdateInterval,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadProcessedWithUpdate )( 
            IOPCHDA_Playback __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
            /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
            /* [in] */ FILETIME ftResampleInterval,
            /* [in] */ DWORD dwNumIntervals,
            /* [in] */ FILETIME ftUpdateInterval,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ OPCHDA_AGGREGATE __RPC_FAR *haAggregate,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Cancel )( 
            IOPCHDA_Playback __RPC_FAR * This,
            /* [in] */ DWORD dwCancelID);
        
        END_INTERFACE
    } IOPCHDA_PlaybackVtbl;

    interface IOPCHDA_Playback
    {
        CONST_VTBL struct IOPCHDA_PlaybackVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCHDA_Playback_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCHDA_Playback_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCHDA_Playback_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCHDA_Playback_ReadRawWithUpdate(This,dwTransactionID,htStartTime,htEndTime,dwNumValues,ftUpdateDuration,ftUpdateInterval,dwNumItems,phServer,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> ReadRawWithUpdate(This,dwTransactionID,htStartTime,htEndTime,dwNumValues,ftUpdateDuration,ftUpdateInterval,dwNumItems,phServer,pdwCancelID,ppErrors)

#define IOPCHDA_Playback_ReadProcessedWithUpdate(This,dwTransactionID,htStartTime,htEndTime,ftResampleInterval,dwNumIntervals,ftUpdateInterval,dwNumItems,phServer,haAggregate,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> ReadProcessedWithUpdate(This,dwTransactionID,htStartTime,htEndTime,ftResampleInterval,dwNumIntervals,ftUpdateInterval,dwNumItems,phServer,haAggregate,pdwCancelID,ppErrors)

#define IOPCHDA_Playback_Cancel(This,dwCancelID)	\
    (This)->lpVtbl -> Cancel(This,dwCancelID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCHDA_Playback_ReadRawWithUpdate_Proxy( 
    IOPCHDA_Playback __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
    /* [in] */ DWORD dwNumValues,
    /* [in] */ FILETIME ftUpdateDuration,
    /* [in] */ FILETIME ftUpdateInterval,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_Playback_ReadRawWithUpdate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_Playback_ReadProcessedWithUpdate_Proxy( 
    IOPCHDA_Playback __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htStartTime,
    /* [out][in] */ OPCHDA_TIME __RPC_FAR *htEndTime,
    /* [in] */ FILETIME ftResampleInterval,
    /* [in] */ DWORD dwNumIntervals,
    /* [in] */ FILETIME ftUpdateInterval,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ OPCHDA_AGGREGATE __RPC_FAR *haAggregate,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCHDA_Playback_ReadProcessedWithUpdate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_Playback_Cancel_Proxy( 
    IOPCHDA_Playback __RPC_FAR * This,
    /* [in] */ DWORD dwCancelID);


void __RPC_STUB IOPCHDA_Playback_Cancel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCHDA_Playback_INTERFACE_DEFINED__ */


#ifndef __IOPCHDA_DataCallback_INTERFACE_DEFINED__
#define __IOPCHDA_DataCallback_INTERFACE_DEFINED__

/* interface IOPCHDA_DataCallback */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOPCHDA_DataCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1F1217B9-DEE0-11d2-A5E5-000086339399")
    IOPCHDA_DataCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnDataChange( 
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHDA_ITEM __RPC_FAR *pItemValues,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnReadComplete( 
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHDA_ITEM __RPC_FAR *pItemValues,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnReadModifiedComplete( 
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHDA_MODIFIEDITEM __RPC_FAR *pItemValues,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnReadAttributeComplete( 
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ OPCHANDLE hClient,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHDA_ATTRIBUTE __RPC_FAR *pAttributeValues,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnReadAnnotations( 
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHDA_ANNOTATION __RPC_FAR *pAnnotationValues,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnInsertAnnotations( 
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClients,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnPlayback( 
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHDA_ITEM __RPC_FAR *__RPC_FAR *ppItemValues,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnUpdateComplete( 
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClients,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnCancelComplete( 
            /* [in] */ DWORD dwCancelID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCHDA_DataCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCHDA_DataCallback __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCHDA_DataCallback __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCHDA_DataCallback __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnDataChange )( 
            IOPCHDA_DataCallback __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHDA_ITEM __RPC_FAR *pItemValues,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnReadComplete )( 
            IOPCHDA_DataCallback __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHDA_ITEM __RPC_FAR *pItemValues,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnReadModifiedComplete )( 
            IOPCHDA_DataCallback __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHDA_MODIFIEDITEM __RPC_FAR *pItemValues,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnReadAttributeComplete )( 
            IOPCHDA_DataCallback __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ OPCHANDLE hClient,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHDA_ATTRIBUTE __RPC_FAR *pAttributeValues,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnReadAnnotations )( 
            IOPCHDA_DataCallback __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHDA_ANNOTATION __RPC_FAR *pAnnotationValues,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnInsertAnnotations )( 
            IOPCHDA_DataCallback __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClients,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnPlayback )( 
            IOPCHDA_DataCallback __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ OPCHDA_ITEM __RPC_FAR *__RPC_FAR *ppItemValues,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUpdateComplete )( 
            IOPCHDA_DataCallback __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID,
            /* [in] */ HRESULT hrStatus,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClients,
            /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnCancelComplete )( 
            IOPCHDA_DataCallback __RPC_FAR * This,
            /* [in] */ DWORD dwCancelID);
        
        END_INTERFACE
    } IOPCHDA_DataCallbackVtbl;

    interface IOPCHDA_DataCallback
    {
        CONST_VTBL struct IOPCHDA_DataCallbackVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCHDA_DataCallback_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCHDA_DataCallback_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCHDA_DataCallback_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCHDA_DataCallback_OnDataChange(This,dwTransactionID,hrStatus,dwNumItems,pItemValues,phrErrors)	\
    (This)->lpVtbl -> OnDataChange(This,dwTransactionID,hrStatus,dwNumItems,pItemValues,phrErrors)

#define IOPCHDA_DataCallback_OnReadComplete(This,dwTransactionID,hrStatus,dwNumItems,pItemValues,phrErrors)	\
    (This)->lpVtbl -> OnReadComplete(This,dwTransactionID,hrStatus,dwNumItems,pItemValues,phrErrors)

#define IOPCHDA_DataCallback_OnReadModifiedComplete(This,dwTransactionID,hrStatus,dwNumItems,pItemValues,phrErrors)	\
    (This)->lpVtbl -> OnReadModifiedComplete(This,dwTransactionID,hrStatus,dwNumItems,pItemValues,phrErrors)

#define IOPCHDA_DataCallback_OnReadAttributeComplete(This,dwTransactionID,hrStatus,hClient,dwNumItems,pAttributeValues,phrErrors)	\
    (This)->lpVtbl -> OnReadAttributeComplete(This,dwTransactionID,hrStatus,hClient,dwNumItems,pAttributeValues,phrErrors)

#define IOPCHDA_DataCallback_OnReadAnnotations(This,dwTransactionID,hrStatus,dwNumItems,pAnnotationValues,phrErrors)	\
    (This)->lpVtbl -> OnReadAnnotations(This,dwTransactionID,hrStatus,dwNumItems,pAnnotationValues,phrErrors)

#define IOPCHDA_DataCallback_OnInsertAnnotations(This,dwTransactionID,hrStatus,dwCount,phClients,phrErrors)	\
    (This)->lpVtbl -> OnInsertAnnotations(This,dwTransactionID,hrStatus,dwCount,phClients,phrErrors)

#define IOPCHDA_DataCallback_OnPlayback(This,dwTransactionID,hrStatus,dwNumItems,ppItemValues,phrErrors)	\
    (This)->lpVtbl -> OnPlayback(This,dwTransactionID,hrStatus,dwNumItems,ppItemValues,phrErrors)

#define IOPCHDA_DataCallback_OnUpdateComplete(This,dwTransactionID,hrStatus,dwCount,phClients,phrErrors)	\
    (This)->lpVtbl -> OnUpdateComplete(This,dwTransactionID,hrStatus,dwCount,phClients,phrErrors)

#define IOPCHDA_DataCallback_OnCancelComplete(This,dwCancelID)	\
    (This)->lpVtbl -> OnCancelComplete(This,dwCancelID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCHDA_DataCallback_OnDataChange_Proxy( 
    IOPCHDA_DataCallback __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [in] */ HRESULT hrStatus,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHDA_ITEM __RPC_FAR *pItemValues,
    /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);


void __RPC_STUB IOPCHDA_DataCallback_OnDataChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_DataCallback_OnReadComplete_Proxy( 
    IOPCHDA_DataCallback __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [in] */ HRESULT hrStatus,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHDA_ITEM __RPC_FAR *pItemValues,
    /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);


void __RPC_STUB IOPCHDA_DataCallback_OnReadComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_DataCallback_OnReadModifiedComplete_Proxy( 
    IOPCHDA_DataCallback __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [in] */ HRESULT hrStatus,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHDA_MODIFIEDITEM __RPC_FAR *pItemValues,
    /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);


void __RPC_STUB IOPCHDA_DataCallback_OnReadModifiedComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_DataCallback_OnReadAttributeComplete_Proxy( 
    IOPCHDA_DataCallback __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [in] */ HRESULT hrStatus,
    /* [in] */ OPCHANDLE hClient,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHDA_ATTRIBUTE __RPC_FAR *pAttributeValues,
    /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);


void __RPC_STUB IOPCHDA_DataCallback_OnReadAttributeComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_DataCallback_OnReadAnnotations_Proxy( 
    IOPCHDA_DataCallback __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [in] */ HRESULT hrStatus,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHDA_ANNOTATION __RPC_FAR *pAnnotationValues,
    /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);


void __RPC_STUB IOPCHDA_DataCallback_OnReadAnnotations_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_DataCallback_OnInsertAnnotations_Proxy( 
    IOPCHDA_DataCallback __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [in] */ HRESULT hrStatus,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClients,
    /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);


void __RPC_STUB IOPCHDA_DataCallback_OnInsertAnnotations_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_DataCallback_OnPlayback_Proxy( 
    IOPCHDA_DataCallback __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [in] */ HRESULT hrStatus,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ OPCHDA_ITEM __RPC_FAR *__RPC_FAR *ppItemValues,
    /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);


void __RPC_STUB IOPCHDA_DataCallback_OnPlayback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_DataCallback_OnUpdateComplete_Proxy( 
    IOPCHDA_DataCallback __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID,
    /* [in] */ HRESULT hrStatus,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClients,
    /* [size_is][in] */ HRESULT __RPC_FAR *phrErrors);


void __RPC_STUB IOPCHDA_DataCallback_OnUpdateComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCHDA_DataCallback_OnCancelComplete_Proxy( 
    IOPCHDA_DataCallback __RPC_FAR * This,
    /* [in] */ DWORD dwCancelID);


void __RPC_STUB IOPCHDA_DataCallback_OnCancelComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCHDA_DataCallback_INTERFACE_DEFINED__ */



#ifndef __OPCHDA_LIBRARY_DEFINED__
#define __OPCHDA_LIBRARY_DEFINED__

/* library OPCHDA */
/* [helpstring][version][uuid] */ 












EXTERN_C const IID LIBID_OPCHDA;
#endif /* __OPCHDA_LIBRARY_DEFINED__ */

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
