/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Dec 12 17:18:21 2008
 */
/* Compiler settings for D:\WorkDir1\CxDatAcs\CxDatAcs.idl:
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

#ifndef __CxDatAcs_h__
#define __CxDatAcs_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ICxMutiDataAccess_FWD_DEFINED__
#define __ICxMutiDataAccess_FWD_DEFINED__
typedef interface ICxMutiDataAccess ICxMutiDataAccess;
#endif 	/* __ICxMutiDataAccess_FWD_DEFINED__ */


#ifndef __ICxDataAccess_FWD_DEFINED__
#define __ICxDataAccess_FWD_DEFINED__
typedef interface ICxDataAccess ICxDataAccess;
#endif 	/* __ICxDataAccess_FWD_DEFINED__ */


#ifndef __ICxDataRecord_FWD_DEFINED__
#define __ICxDataRecord_FWD_DEFINED__
typedef interface ICxDataRecord ICxDataRecord;
#endif 	/* __ICxDataRecord_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_CxDatAcs_0000 */
/* [local] */ 


enum __MIDL___MIDL_itf_CxDatAcs_0000_0001
    {	OPENMODE_CREATE	= 0,
	OPENMODE_READ	= 0x1,
	OPENMODE_WRITE	= 0x2,
	OPENMODE_ISEXIST	= 0x3,
	OPENMODE_CLOSE	= 0x4,
	OPENMODE_READ2	= 0x5
    };




extern RPC_IF_HANDLE __MIDL_itf_CxDatAcs_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_CxDatAcs_0000_v0_0_s_ifspec;

#ifndef __ICxMutiDataAccess_INTERFACE_DEFINED__
#define __ICxMutiDataAccess_INTERFACE_DEFINED__

/* interface ICxMutiDataAccess */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ICxMutiDataAccess;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A947C3CB-33C9-407c-B752-551D9417DCF8")
    ICxMutiDataAccess : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetDataAccess( 
            LPCWSTR lpszName,
            ICxDataAccess __RPC_FAR *__RPC_FAR *ppDataAccess) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetDataFile( 
            LPCWSTR lpszFileName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetOpenMode( 
            BOOL bReadOnly) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxMutiDataAccessVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxMutiDataAccess __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxMutiDataAccess __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxMutiDataAccess __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDataAccess )( 
            ICxMutiDataAccess __RPC_FAR * This,
            LPCWSTR lpszName,
            ICxDataAccess __RPC_FAR *__RPC_FAR *ppDataAccess);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDataFile )( 
            ICxMutiDataAccess __RPC_FAR * This,
            LPCWSTR lpszFileName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOpenMode )( 
            ICxMutiDataAccess __RPC_FAR * This,
            BOOL bReadOnly);
        
        END_INTERFACE
    } ICxMutiDataAccessVtbl;

    interface ICxMutiDataAccess
    {
        CONST_VTBL struct ICxMutiDataAccessVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxMutiDataAccess_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxMutiDataAccess_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxMutiDataAccess_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxMutiDataAccess_GetDataAccess(This,lpszName,ppDataAccess)	\
    (This)->lpVtbl -> GetDataAccess(This,lpszName,ppDataAccess)

#define ICxMutiDataAccess_SetDataFile(This,lpszFileName)	\
    (This)->lpVtbl -> SetDataFile(This,lpszFileName)

#define ICxMutiDataAccess_SetOpenMode(This,bReadOnly)	\
    (This)->lpVtbl -> SetOpenMode(This,bReadOnly)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxMutiDataAccess_GetDataAccess_Proxy( 
    ICxMutiDataAccess __RPC_FAR * This,
    LPCWSTR lpszName,
    ICxDataAccess __RPC_FAR *__RPC_FAR *ppDataAccess);


void __RPC_STUB ICxMutiDataAccess_GetDataAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxMutiDataAccess_SetDataFile_Proxy( 
    ICxMutiDataAccess __RPC_FAR * This,
    LPCWSTR lpszFileName);


void __RPC_STUB ICxMutiDataAccess_SetDataFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxMutiDataAccess_SetOpenMode_Proxy( 
    ICxMutiDataAccess __RPC_FAR * This,
    BOOL bReadOnly);


void __RPC_STUB ICxMutiDataAccess_SetOpenMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxMutiDataAccess_INTERFACE_DEFINED__ */


#ifndef __ICxDataAccess_INTERFACE_DEFINED__
#define __ICxDataAccess_INTERFACE_DEFINED__

/* interface ICxDataAccess */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ICxDataAccess;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("16D09780-1548-4D76-858C-BB6AE14DCA29")
    ICxDataAccess : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Open( 
            long lOpenMode) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddRecord( 
            ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE InsertRecord( 
            int lIndex,
            ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetCount( 
            long __RPC_FAR *plCount) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetRecord( 
            long lIndex,
            ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetRecordByID( 
            DWORD dwID,
            ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetRecordByName( 
            LPCWSTR lpszName,
            ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DeleteRecord( 
            DWORD dwID) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetUniqueRecordName( 
            LPCWSTR lpszBaseName,
            LPWSTR lpszName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetDataFile( 
            LPCWSTR lpszFileName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Update( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DeleteAllRecord( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Encrypt( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxDataAccessVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxDataAccess __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxDataAccess __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxDataAccess __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            ICxDataAccess __RPC_FAR * This,
            long lOpenMode);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Close )( 
            ICxDataAccess __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddRecord )( 
            ICxDataAccess __RPC_FAR * This,
            ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertRecord )( 
            ICxDataAccess __RPC_FAR * This,
            int lIndex,
            ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCount )( 
            ICxDataAccess __RPC_FAR * This,
            long __RPC_FAR *plCount);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRecord )( 
            ICxDataAccess __RPC_FAR * This,
            long lIndex,
            ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRecordByID )( 
            ICxDataAccess __RPC_FAR * This,
            DWORD dwID,
            ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRecordByName )( 
            ICxDataAccess __RPC_FAR * This,
            LPCWSTR lpszName,
            ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteRecord )( 
            ICxDataAccess __RPC_FAR * This,
            DWORD dwID);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUniqueRecordName )( 
            ICxDataAccess __RPC_FAR * This,
            LPCWSTR lpszBaseName,
            LPWSTR lpszName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDataFile )( 
            ICxDataAccess __RPC_FAR * This,
            LPCWSTR lpszFileName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Update )( 
            ICxDataAccess __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteAllRecord )( 
            ICxDataAccess __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Encrypt )( 
            ICxDataAccess __RPC_FAR * This);
        
        END_INTERFACE
    } ICxDataAccessVtbl;

    interface ICxDataAccess
    {
        CONST_VTBL struct ICxDataAccessVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxDataAccess_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxDataAccess_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxDataAccess_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxDataAccess_Open(This,lOpenMode)	\
    (This)->lpVtbl -> Open(This,lOpenMode)

#define ICxDataAccess_Close(This)	\
    (This)->lpVtbl -> Close(This)

#define ICxDataAccess_AddRecord(This,ppDataRecord)	\
    (This)->lpVtbl -> AddRecord(This,ppDataRecord)

#define ICxDataAccess_InsertRecord(This,lIndex,ppDataRecord)	\
    (This)->lpVtbl -> InsertRecord(This,lIndex,ppDataRecord)

#define ICxDataAccess_GetCount(This,plCount)	\
    (This)->lpVtbl -> GetCount(This,plCount)

#define ICxDataAccess_GetRecord(This,lIndex,ppDataRecord)	\
    (This)->lpVtbl -> GetRecord(This,lIndex,ppDataRecord)

#define ICxDataAccess_GetRecordByID(This,dwID,ppDataRecord)	\
    (This)->lpVtbl -> GetRecordByID(This,dwID,ppDataRecord)

#define ICxDataAccess_GetRecordByName(This,lpszName,ppDataRecord)	\
    (This)->lpVtbl -> GetRecordByName(This,lpszName,ppDataRecord)

#define ICxDataAccess_DeleteRecord(This,dwID)	\
    (This)->lpVtbl -> DeleteRecord(This,dwID)

#define ICxDataAccess_GetUniqueRecordName(This,lpszBaseName,lpszName)	\
    (This)->lpVtbl -> GetUniqueRecordName(This,lpszBaseName,lpszName)

#define ICxDataAccess_SetDataFile(This,lpszFileName)	\
    (This)->lpVtbl -> SetDataFile(This,lpszFileName)

#define ICxDataAccess_Update(This)	\
    (This)->lpVtbl -> Update(This)

#define ICxDataAccess_DeleteAllRecord(This)	\
    (This)->lpVtbl -> DeleteAllRecord(This)

#define ICxDataAccess_Encrypt(This)	\
    (This)->lpVtbl -> Encrypt(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataAccess_Open_Proxy( 
    ICxDataAccess __RPC_FAR * This,
    long lOpenMode);


void __RPC_STUB ICxDataAccess_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataAccess_Close_Proxy( 
    ICxDataAccess __RPC_FAR * This);


void __RPC_STUB ICxDataAccess_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataAccess_AddRecord_Proxy( 
    ICxDataAccess __RPC_FAR * This,
    ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord);


void __RPC_STUB ICxDataAccess_AddRecord_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataAccess_InsertRecord_Proxy( 
    ICxDataAccess __RPC_FAR * This,
    int lIndex,
    ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord);


void __RPC_STUB ICxDataAccess_InsertRecord_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataAccess_GetCount_Proxy( 
    ICxDataAccess __RPC_FAR * This,
    long __RPC_FAR *plCount);


void __RPC_STUB ICxDataAccess_GetCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataAccess_GetRecord_Proxy( 
    ICxDataAccess __RPC_FAR * This,
    long lIndex,
    ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord);


void __RPC_STUB ICxDataAccess_GetRecord_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataAccess_GetRecordByID_Proxy( 
    ICxDataAccess __RPC_FAR * This,
    DWORD dwID,
    ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord);


void __RPC_STUB ICxDataAccess_GetRecordByID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataAccess_GetRecordByName_Proxy( 
    ICxDataAccess __RPC_FAR * This,
    LPCWSTR lpszName,
    ICxDataRecord __RPC_FAR *__RPC_FAR *ppDataRecord);


void __RPC_STUB ICxDataAccess_GetRecordByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataAccess_DeleteRecord_Proxy( 
    ICxDataAccess __RPC_FAR * This,
    DWORD dwID);


void __RPC_STUB ICxDataAccess_DeleteRecord_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataAccess_GetUniqueRecordName_Proxy( 
    ICxDataAccess __RPC_FAR * This,
    LPCWSTR lpszBaseName,
    LPWSTR lpszName);


void __RPC_STUB ICxDataAccess_GetUniqueRecordName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataAccess_SetDataFile_Proxy( 
    ICxDataAccess __RPC_FAR * This,
    LPCWSTR lpszFileName);


void __RPC_STUB ICxDataAccess_SetDataFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataAccess_Update_Proxy( 
    ICxDataAccess __RPC_FAR * This);


void __RPC_STUB ICxDataAccess_Update_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataAccess_DeleteAllRecord_Proxy( 
    ICxDataAccess __RPC_FAR * This);


void __RPC_STUB ICxDataAccess_DeleteAllRecord_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataAccess_Encrypt_Proxy( 
    ICxDataAccess __RPC_FAR * This);


void __RPC_STUB ICxDataAccess_Encrypt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxDataAccess_INTERFACE_DEFINED__ */


#ifndef __ICxDataRecord_INTERFACE_DEFINED__
#define __ICxDataRecord_INTERFACE_DEFINED__

/* interface ICxDataRecord */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ICxDataRecord;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7E956574-EBEB-4A98-A933-0BA1810B71CD")
    ICxDataRecord : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetData( 
            long pData) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetData( 
            long pData) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetField( 
            long lField,
            VARIANT __RPC_FAR *pvaField) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetField( 
            long lField,
            VARIANT vaField) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetFieldByName( 
            LPCWSTR szFieldName,
            VARIANT __RPC_FAR *pvaField) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetFieldByName( 
            LPCWSTR szFieldName,
            VARIANT vaField) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateObject( 
            BOOL bNew,
            REFIID riid,
            IUnknown __RPC_FAR *__RPC_FAR *ppUnk) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ObjectToData( 
            IUnknown __RPC_FAR *pUnk) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DataToObject( 
            IUnknown __RPC_FAR *pUnk) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxDataRecordVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxDataRecord __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxDataRecord __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxDataRecord __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetData )( 
            ICxDataRecord __RPC_FAR * This,
            long pData);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetData )( 
            ICxDataRecord __RPC_FAR * This,
            long pData);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetField )( 
            ICxDataRecord __RPC_FAR * This,
            long lField,
            VARIANT __RPC_FAR *pvaField);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetField )( 
            ICxDataRecord __RPC_FAR * This,
            long lField,
            VARIANT vaField);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFieldByName )( 
            ICxDataRecord __RPC_FAR * This,
            LPCWSTR szFieldName,
            VARIANT __RPC_FAR *pvaField);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFieldByName )( 
            ICxDataRecord __RPC_FAR * This,
            LPCWSTR szFieldName,
            VARIANT vaField);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateObject )( 
            ICxDataRecord __RPC_FAR * This,
            BOOL bNew,
            REFIID riid,
            IUnknown __RPC_FAR *__RPC_FAR *ppUnk);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ObjectToData )( 
            ICxDataRecord __RPC_FAR * This,
            IUnknown __RPC_FAR *pUnk);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DataToObject )( 
            ICxDataRecord __RPC_FAR * This,
            IUnknown __RPC_FAR *pUnk);
        
        END_INTERFACE
    } ICxDataRecordVtbl;

    interface ICxDataRecord
    {
        CONST_VTBL struct ICxDataRecordVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxDataRecord_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxDataRecord_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxDataRecord_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxDataRecord_GetData(This,pData)	\
    (This)->lpVtbl -> GetData(This,pData)

#define ICxDataRecord_SetData(This,pData)	\
    (This)->lpVtbl -> SetData(This,pData)

#define ICxDataRecord_GetField(This,lField,pvaField)	\
    (This)->lpVtbl -> GetField(This,lField,pvaField)

#define ICxDataRecord_SetField(This,lField,vaField)	\
    (This)->lpVtbl -> SetField(This,lField,vaField)

#define ICxDataRecord_GetFieldByName(This,szFieldName,pvaField)	\
    (This)->lpVtbl -> GetFieldByName(This,szFieldName,pvaField)

#define ICxDataRecord_SetFieldByName(This,szFieldName,vaField)	\
    (This)->lpVtbl -> SetFieldByName(This,szFieldName,vaField)

#define ICxDataRecord_CreateObject(This,bNew,riid,ppUnk)	\
    (This)->lpVtbl -> CreateObject(This,bNew,riid,ppUnk)

#define ICxDataRecord_ObjectToData(This,pUnk)	\
    (This)->lpVtbl -> ObjectToData(This,pUnk)

#define ICxDataRecord_DataToObject(This,pUnk)	\
    (This)->lpVtbl -> DataToObject(This,pUnk)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataRecord_GetData_Proxy( 
    ICxDataRecord __RPC_FAR * This,
    long pData);


void __RPC_STUB ICxDataRecord_GetData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataRecord_SetData_Proxy( 
    ICxDataRecord __RPC_FAR * This,
    long pData);


void __RPC_STUB ICxDataRecord_SetData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataRecord_GetField_Proxy( 
    ICxDataRecord __RPC_FAR * This,
    long lField,
    VARIANT __RPC_FAR *pvaField);


void __RPC_STUB ICxDataRecord_GetField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataRecord_SetField_Proxy( 
    ICxDataRecord __RPC_FAR * This,
    long lField,
    VARIANT vaField);


void __RPC_STUB ICxDataRecord_SetField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataRecord_GetFieldByName_Proxy( 
    ICxDataRecord __RPC_FAR * This,
    LPCWSTR szFieldName,
    VARIANT __RPC_FAR *pvaField);


void __RPC_STUB ICxDataRecord_GetFieldByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataRecord_SetFieldByName_Proxy( 
    ICxDataRecord __RPC_FAR * This,
    LPCWSTR szFieldName,
    VARIANT vaField);


void __RPC_STUB ICxDataRecord_SetFieldByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataRecord_CreateObject_Proxy( 
    ICxDataRecord __RPC_FAR * This,
    BOOL bNew,
    REFIID riid,
    IUnknown __RPC_FAR *__RPC_FAR *ppUnk);


void __RPC_STUB ICxDataRecord_CreateObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataRecord_ObjectToData_Proxy( 
    ICxDataRecord __RPC_FAR * This,
    IUnknown __RPC_FAR *pUnk);


void __RPC_STUB ICxDataRecord_ObjectToData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICxDataRecord_DataToObject_Proxy( 
    ICxDataRecord __RPC_FAR * This,
    IUnknown __RPC_FAR *pUnk);


void __RPC_STUB ICxDataRecord_DataToObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxDataRecord_INTERFACE_DEFINED__ */



#ifndef __CXDATACSLib_LIBRARY_DEFINED__
#define __CXDATACSLib_LIBRARY_DEFINED__

/* library CXDATACSLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_CXDATACSLib;
#endif /* __CXDATACSLib_LIBRARY_DEFINED__ */

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
