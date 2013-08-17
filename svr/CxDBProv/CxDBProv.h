/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Aug 11 11:02:41 2013
 */
/* Compiler settings for D:\spcx\svr\CxDBProv\CxDBProv.idl:
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

#ifndef __CxDBProv_h__
#define __CxDBProv_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ICxDataSource_FWD_DEFINED__
#define __ICxDataSource_FWD_DEFINED__
typedef interface ICxDataSource ICxDataSource;
#endif 	/* __ICxDataSource_FWD_DEFINED__ */


#ifndef __HistoryData_FWD_DEFINED__
#define __HistoryData_FWD_DEFINED__

#ifdef __cplusplus
typedef class HistoryData HistoryData;
#else
typedef struct HistoryData HistoryData;
#endif /* __cplusplus */

#endif 	/* __HistoryData_FWD_DEFINED__ */


#ifndef __DataConnectionPage_FWD_DEFINED__
#define __DataConnectionPage_FWD_DEFINED__

#ifdef __cplusplus
typedef class DataConnectionPage DataConnectionPage;
#else
typedef struct DataConnectionPage DataConnectionPage;
#endif /* __cplusplus */

#endif 	/* __DataConnectionPage_FWD_DEFINED__ */


#ifndef __RealtimeData_FWD_DEFINED__
#define __RealtimeData_FWD_DEFINED__

#ifdef __cplusplus
typedef class RealtimeData RealtimeData;
#else
typedef struct RealtimeData RealtimeData;
#endif /* __cplusplus */

#endif 	/* __RealtimeData_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __ICxDataSource_INTERFACE_DEFINED__
#define __ICxDataSource_INTERFACE_DEFINED__

/* interface ICxDataSource */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ICxDataSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8019A860-86A3-4553-B861-EC24D13EF9E3")
    ICxDataSource : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetDataSession( 
            /* [in] */ REFIID riid,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppSession) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetUseLocalSetup( 
            /* [in] */ BOOL __RPC_FAR *pbUse) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxDataSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxDataSource __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxDataSource __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxDataSource __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDataSession )( 
            ICxDataSource __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppSession);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetUseLocalSetup )( 
            ICxDataSource __RPC_FAR * This,
            /* [in] */ BOOL __RPC_FAR *pbUse);
        
        END_INTERFACE
    } ICxDataSourceVtbl;

    interface ICxDataSource
    {
        CONST_VTBL struct ICxDataSourceVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxDataSource_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxDataSource_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxDataSource_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxDataSource_GetDataSession(This,riid,ppSession)	\
    (This)->lpVtbl -> GetDataSession(This,riid,ppSession)

#define ICxDataSource_SetUseLocalSetup(This,pbUse)	\
    (This)->lpVtbl -> SetUseLocalSetup(This,pbUse)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICxDataSource_GetDataSession_Proxy( 
    ICxDataSource __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppSession);


void __RPC_STUB ICxDataSource_GetDataSession_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSource_SetUseLocalSetup_Proxy( 
    ICxDataSource __RPC_FAR * This,
    /* [in] */ BOOL __RPC_FAR *pbUse);


void __RPC_STUB ICxDataSource_SetUseLocalSetup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxDataSource_INTERFACE_DEFINED__ */



#ifndef __CXDBPROVLib_LIBRARY_DEFINED__
#define __CXDBPROVLib_LIBRARY_DEFINED__

/* library CXDBPROVLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_CXDBPROVLib;

EXTERN_C const CLSID CLSID_HistoryData;

#ifdef __cplusplus

class DECLSPEC_UUID("F8CA6B76-DB06-4D83-B862-DE7BBCE1789F")
HistoryData;
#endif

EXTERN_C const CLSID CLSID_DataConnectionPage;

#ifdef __cplusplus

class DECLSPEC_UUID("D470437F-5131-40E1-AFFE-9B41A31DAF56")
DataConnectionPage;
#endif

EXTERN_C const CLSID CLSID_RealtimeData;

#ifdef __cplusplus

class DECLSPEC_UUID("29E5101C-58AA-41D3-B36D-AC98093D1754")
RealtimeData;
#endif
#endif /* __CXDBPROVLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
