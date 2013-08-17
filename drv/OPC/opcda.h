/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.01.75 */
/* at Wed Oct 14 12:14:58 1998
 */
/* Compiler settings for opcda.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"
#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __opcda_h__
#define __opcda_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IOPCServer_FWD_DEFINED__
#define __IOPCServer_FWD_DEFINED__
typedef interface IOPCServer IOPCServer;
#endif 	/* __IOPCServer_FWD_DEFINED__ */


#ifndef __IOPCServerPublicGroups_FWD_DEFINED__
#define __IOPCServerPublicGroups_FWD_DEFINED__
typedef interface IOPCServerPublicGroups IOPCServerPublicGroups;
#endif 	/* __IOPCServerPublicGroups_FWD_DEFINED__ */


#ifndef __IOPCBrowseServerAddressSpace_FWD_DEFINED__
#define __IOPCBrowseServerAddressSpace_FWD_DEFINED__
typedef interface IOPCBrowseServerAddressSpace IOPCBrowseServerAddressSpace;
#endif 	/* __IOPCBrowseServerAddressSpace_FWD_DEFINED__ */


#ifndef __IOPCGroupStateMgt_FWD_DEFINED__
#define __IOPCGroupStateMgt_FWD_DEFINED__
typedef interface IOPCGroupStateMgt IOPCGroupStateMgt;
#endif 	/* __IOPCGroupStateMgt_FWD_DEFINED__ */


#ifndef __IOPCPublicGroupStateMgt_FWD_DEFINED__
#define __IOPCPublicGroupStateMgt_FWD_DEFINED__
typedef interface IOPCPublicGroupStateMgt IOPCPublicGroupStateMgt;
#endif 	/* __IOPCPublicGroupStateMgt_FWD_DEFINED__ */


#ifndef __IOPCSyncIO_FWD_DEFINED__
#define __IOPCSyncIO_FWD_DEFINED__
typedef interface IOPCSyncIO IOPCSyncIO;
#endif 	/* __IOPCSyncIO_FWD_DEFINED__ */


#ifndef __IOPCAsyncIO_FWD_DEFINED__
#define __IOPCAsyncIO_FWD_DEFINED__
typedef interface IOPCAsyncIO IOPCAsyncIO;
#endif 	/* __IOPCAsyncIO_FWD_DEFINED__ */


#ifndef __IOPCItemMgt_FWD_DEFINED__
#define __IOPCItemMgt_FWD_DEFINED__
typedef interface IOPCItemMgt IOPCItemMgt;
#endif 	/* __IOPCItemMgt_FWD_DEFINED__ */


#ifndef __IEnumOPCItemAttributes_FWD_DEFINED__
#define __IEnumOPCItemAttributes_FWD_DEFINED__
typedef interface IEnumOPCItemAttributes IEnumOPCItemAttributes;
#endif 	/* __IEnumOPCItemAttributes_FWD_DEFINED__ */


#ifndef __IOPCDataCallback_FWD_DEFINED__
#define __IOPCDataCallback_FWD_DEFINED__
typedef interface IOPCDataCallback IOPCDataCallback;
#endif 	/* __IOPCDataCallback_FWD_DEFINED__ */


#ifndef __IOPCAsyncIO2_FWD_DEFINED__
#define __IOPCAsyncIO2_FWD_DEFINED__
typedef interface IOPCAsyncIO2 IOPCAsyncIO2;
#endif 	/* __IOPCAsyncIO2_FWD_DEFINED__ */


#ifndef __IOPCItemProperties_FWD_DEFINED__
#define __IOPCItemProperties_FWD_DEFINED__
typedef interface IOPCItemProperties IOPCItemProperties;
#endif 	/* __IOPCItemProperties_FWD_DEFINED__ */


#ifndef __IOPCServer_FWD_DEFINED__
#define __IOPCServer_FWD_DEFINED__
typedef interface IOPCServer IOPCServer;
#endif 	/* __IOPCServer_FWD_DEFINED__ */


#ifndef __IOPCServerPublicGroups_FWD_DEFINED__
#define __IOPCServerPublicGroups_FWD_DEFINED__
typedef interface IOPCServerPublicGroups IOPCServerPublicGroups;
#endif 	/* __IOPCServerPublicGroups_FWD_DEFINED__ */


#ifndef __IOPCBrowseServerAddressSpace_FWD_DEFINED__
#define __IOPCBrowseServerAddressSpace_FWD_DEFINED__
typedef interface IOPCBrowseServerAddressSpace IOPCBrowseServerAddressSpace;
#endif 	/* __IOPCBrowseServerAddressSpace_FWD_DEFINED__ */


#ifndef __IOPCGroupStateMgt_FWD_DEFINED__
#define __IOPCGroupStateMgt_FWD_DEFINED__
typedef interface IOPCGroupStateMgt IOPCGroupStateMgt;
#endif 	/* __IOPCGroupStateMgt_FWD_DEFINED__ */


#ifndef __IOPCPublicGroupStateMgt_FWD_DEFINED__
#define __IOPCPublicGroupStateMgt_FWD_DEFINED__
typedef interface IOPCPublicGroupStateMgt IOPCPublicGroupStateMgt;
#endif 	/* __IOPCPublicGroupStateMgt_FWD_DEFINED__ */


#ifndef __IOPCSyncIO_FWD_DEFINED__
#define __IOPCSyncIO_FWD_DEFINED__
typedef interface IOPCSyncIO IOPCSyncIO;
#endif 	/* __IOPCSyncIO_FWD_DEFINED__ */


#ifndef __IOPCAsyncIO_FWD_DEFINED__
#define __IOPCAsyncIO_FWD_DEFINED__
typedef interface IOPCAsyncIO IOPCAsyncIO;
#endif 	/* __IOPCAsyncIO_FWD_DEFINED__ */


#ifndef __IOPCItemMgt_FWD_DEFINED__
#define __IOPCItemMgt_FWD_DEFINED__
typedef interface IOPCItemMgt IOPCItemMgt;
#endif 	/* __IOPCItemMgt_FWD_DEFINED__ */


#ifndef __IEnumOPCItemAttributes_FWD_DEFINED__
#define __IEnumOPCItemAttributes_FWD_DEFINED__
typedef interface IEnumOPCItemAttributes IEnumOPCItemAttributes;
#endif 	/* __IEnumOPCItemAttributes_FWD_DEFINED__ */


#ifndef __IOPCDataCallback_FWD_DEFINED__
#define __IOPCDataCallback_FWD_DEFINED__
typedef interface IOPCDataCallback IOPCDataCallback;
#endif 	/* __IOPCDataCallback_FWD_DEFINED__ */


#ifndef __IOPCAsyncIO2_FWD_DEFINED__
#define __IOPCAsyncIO2_FWD_DEFINED__
typedef interface IOPCAsyncIO2 IOPCAsyncIO2;
#endif 	/* __IOPCAsyncIO2_FWD_DEFINED__ */


#ifndef __IOPCItemProperties_FWD_DEFINED__
#define __IOPCItemProperties_FWD_DEFINED__
typedef interface IOPCItemProperties IOPCItemProperties;
#endif 	/* __IOPCItemProperties_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/****************************************
 * Generated header for interface: __MIDL_itf_opcda_0000
 * at Wed Oct 14 12:14:58 1998
 * using MIDL 3.01.75
 ****************************************/
/* [local] */ 


typedef 
enum tagOPCDATASOURCE
    {	OPC_DS_CACHE	= 1,
	OPC_DS_DEVICE	= OPC_DS_CACHE + 1
    }	OPCDATASOURCE;

typedef 
enum tagOPCBROWSETYPE
    {	OPC_BRANCH	= 1,
	OPC_LEAF	= OPC_BRANCH + 1,
	OPC_FLAT	= OPC_LEAF + 1
    }	OPCBROWSETYPE;

typedef 
enum tagOPCNAMESPACETYPE
    {	OPC_NS_HIERARCHIAL	= 1,
	OPC_NS_FLAT	= OPC_NS_HIERARCHIAL + 1
    }	OPCNAMESPACETYPE;

typedef 
enum tagOPCBROWSEDIRECTION
    {	OPC_BROWSE_UP	= 1,
	OPC_BROWSE_DOWN	= OPC_BROWSE_UP + 1,
	OPC_BROWSE_TO	= OPC_BROWSE_DOWN + 1
    }	OPCBROWSEDIRECTION;

#define OPC_READABLE    1
#define OPC_WRITEABLE   2
typedef 
enum tagOPCEUTYPE
    {	OPC_NOENUM	= 0,
	OPC_ANALOG	= OPC_NOENUM + 1,
	OPC_ENUMERATED	= OPC_ANALOG + 1
    }	OPCEUTYPE;

typedef 
enum tagOPCSERVERSTATE
    {	OPC_STATUS_RUNNING	= 1,
	OPC_STATUS_FAILED	= OPC_STATUS_RUNNING + 1,
	OPC_STATUS_NOCONFIG	= OPC_STATUS_FAILED + 1,
	OPC_STATUS_SUSPENDED	= OPC_STATUS_NOCONFIG + 1,
	OPC_STATUS_TEST	= OPC_STATUS_SUSPENDED + 1
    }	OPCSERVERSTATE;

typedef 
enum tagOPCENUMSCOPE
    {	OPC_ENUM_PRIVATE_CONNECTIONS	= 1,
	OPC_ENUM_PUBLIC_CONNECTIONS	= OPC_ENUM_PRIVATE_CONNECTIONS + 1,
	OPC_ENUM_ALL_CONNECTIONS	= OPC_ENUM_PUBLIC_CONNECTIONS + 1,
	OPC_ENUM_PRIVATE	= OPC_ENUM_ALL_CONNECTIONS + 1,
	OPC_ENUM_PUBLIC	= OPC_ENUM_PRIVATE + 1,
	OPC_ENUM_ALL	= OPC_ENUM_PUBLIC + 1
    }	OPCENUMSCOPE;

typedef DWORD OPCHANDLE;

typedef struct  tagOPCGROUPHEADER
    {
    DWORD dwSize;
    DWORD dwItemCount;
    OPCHANDLE hClientGroup;
    DWORD dwTransactionID;
    HRESULT hrStatus;
    }	OPCGROUPHEADER;

typedef struct  tagOPCITEMHEADER1
    {
    OPCHANDLE hClient;
    DWORD dwValueOffset;
    WORD wQuality;
    WORD wReserved;
    FILETIME ftTimeStampItem;
    }	OPCITEMHEADER1;

typedef struct  tagOPCITEMHEADER2
    {
    OPCHANDLE hClient;
    DWORD dwValueOffset;
    WORD wQuality;
    WORD wReserved;
    }	OPCITEMHEADER2;

typedef struct  tagOPCGROUPHEADERWRITE
    {
    DWORD dwItemCount;
    OPCHANDLE hClientGroup;
    DWORD dwTransactionID;
    HRESULT hrStatus;
    }	OPCGROUPHEADERWRITE;

typedef struct  tagOPCITEMHEADERWRITE
    {
    OPCHANDLE hClient;
    HRESULT dwError;
    }	OPCITEMHEADERWRITE;

typedef struct  tagOPCITEMSTATE
    {
    OPCHANDLE hClient;
    FILETIME ftTimeStamp;
    WORD wQuality;
    WORD wReserved;
    VARIANT vDataValue;
    }	OPCITEMSTATE;

typedef struct  tagOPCSERVERSTATUS
    {
    FILETIME ftStartTime;
    FILETIME ftCurrentTime;
    FILETIME ftLastUpdateTime;
    OPCSERVERSTATE dwServerState;
    DWORD dwGroupCount;
    DWORD dwBandWidth;
    WORD wMajorVersion;
    WORD wMinorVersion;
    WORD wBuildNumber;
    WORD wReserved;
    /* [string] */ LPWSTR szVendorInfo;
    }	OPCSERVERSTATUS;

typedef struct  tagOPCITEMDEF
    {
    /* [string] */ LPWSTR szAccessPath;
    /* [string] */ LPWSTR szItemID;
    BOOL bActive;
    OPCHANDLE hClient;
    DWORD dwBlobSize;
    /* [size_is] */ BYTE __RPC_FAR *pBlob;
    VARTYPE vtRequestedDataType;
    WORD wReserved;
    }	OPCITEMDEF;

typedef struct  tagOPCITEMATTRIBUTES
    {
    /* [string] */ LPWSTR szAccessPath;
    /* [string] */ LPWSTR szItemID;
    BOOL bActive;
    OPCHANDLE hClient;
    OPCHANDLE hServer;
    DWORD dwAccessRights;
    DWORD dwBlobSize;
    /* [size_is] */ BYTE __RPC_FAR *pBlob;
    VARTYPE vtRequestedDataType;
    VARTYPE vtCanonicalDataType;
    OPCEUTYPE dwEUType;
    VARIANT vEUInfo;
    }	OPCITEMATTRIBUTES;

typedef struct  tagOPCITEMRESULT
    {
    OPCHANDLE hServer;
    VARTYPE vtCanonicalDataType;
    WORD wReserved;
    DWORD dwAccessRights;
    DWORD dwBlobSize;
    /* [size_is] */ BYTE __RPC_FAR *pBlob;
    }	OPCITEMRESULT;

#define    OPC_QUALITY_MASK            0xC0
#define    OPC_STATUS_MASK             0xFC
#define    OPC_LIMIT_MASK              0x03
#define    OPC_QUALITY_BAD             0x00
#define    OPC_QUALITY_UNCERTAIN       0x40
#define    OPC_QUALITY_GOOD            0xC0
#define    OPC_QUALITY_CONFIG_ERROR    0x04
#define    OPC_QUALITY_NOT_CONNECTED   0x08
#define    OPC_QUALITY_DEVICE_FAILURE  0x0c
#define    OPC_QUALITY_SENSOR_FAILURE  0x10
#define    OPC_QUALITY_LAST_KNOWN      0x14
#define    OPC_QUALITY_COMM_FAILURE    0x18
#define    OPC_QUALITY_OUT_OF_SERVICE  0x1C
#define    OPC_QUALITY_LAST_USABLE     0x44
#define    OPC_QUALITY_SENSOR_CAL      0x50
#define    OPC_QUALITY_EGU_EXCEEDED    0x54
#define    OPC_QUALITY_SUB_NORMAL      0x58
#define    OPC_QUALITY_LOCAL_OVERRIDE  0xD8
#define    OPC_LIMIT_OK                0x00
#define    OPC_LIMIT_LOW               0x01
#define    OPC_LIMIT_HIGH              0x02
#define    OPC_LIMIT_CONST             0x03


extern RPC_IF_HANDLE __MIDL_itf_opcda_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_opcda_0000_v0_0_s_ifspec;

#ifndef __IOPCServer_INTERFACE_DEFINED__
#define __IOPCServer_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOPCServer
 * at Wed Oct 14 12:14:58 1998
 * using MIDL 3.01.75
 ****************************************/
/* [unique][uuid][object] */ 



EXTERN_C const IID IID_IOPCServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("39c13a4d-011e-11d0-9675-0020afd8adb3")
    IOPCServer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AddGroup( 
            /* [string][in] */ LPCWSTR szName,
            /* [in] */ BOOL bActive,
            /* [in] */ DWORD dwRequestedUpdateRate,
            /* [in] */ OPCHANDLE hClientGroup,
            /* [in][unique] */ LONG __RPC_FAR *pTimeBias,
            /* [in][unique] */ FLOAT __RPC_FAR *pPercentDeadband,
            /* [in] */ DWORD dwLCID,
            /* [out] */ OPCHANDLE __RPC_FAR *phServerGroup,
            /* [out] */ DWORD __RPC_FAR *pRevisedUpdateRate,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetErrorString( 
            /* [in] */ HRESULT dwError,
            /* [in] */ LCID dwLocale,
            /* [string][out] */ LPWSTR __RPC_FAR *ppString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetGroupByName( 
            /* [string][in] */ LPCWSTR szName,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [out] */ OPCSERVERSTATUS __RPC_FAR *__RPC_FAR *ppServerStatus) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveGroup( 
            /* [in] */ OPCHANDLE hServerGroup,
            /* [in] */ BOOL bForce) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateGroupEnumerator( 
            /* [in] */ OPCENUMSCOPE dwScope,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCServer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCServer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddGroup )( 
            IOPCServer __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szName,
            /* [in] */ BOOL bActive,
            /* [in] */ DWORD dwRequestedUpdateRate,
            /* [in] */ OPCHANDLE hClientGroup,
            /* [in][unique] */ LONG __RPC_FAR *pTimeBias,
            /* [in][unique] */ FLOAT __RPC_FAR *pPercentDeadband,
            /* [in] */ DWORD dwLCID,
            /* [out] */ OPCHANDLE __RPC_FAR *phServerGroup,
            /* [out] */ DWORD __RPC_FAR *pRevisedUpdateRate,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetErrorString )( 
            IOPCServer __RPC_FAR * This,
            /* [in] */ HRESULT dwError,
            /* [in] */ LCID dwLocale,
            /* [string][out] */ LPWSTR __RPC_FAR *ppString);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetGroupByName )( 
            IOPCServer __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szName,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStatus )( 
            IOPCServer __RPC_FAR * This,
            /* [out] */ OPCSERVERSTATUS __RPC_FAR *__RPC_FAR *ppServerStatus);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveGroup )( 
            IOPCServer __RPC_FAR * This,
            /* [in] */ OPCHANDLE hServerGroup,
            /* [in] */ BOOL bForce);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateGroupEnumerator )( 
            IOPCServer __RPC_FAR * This,
            /* [in] */ OPCENUMSCOPE dwScope,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);
        
        END_INTERFACE
    } IOPCServerVtbl;

    interface IOPCServer
    {
        CONST_VTBL struct IOPCServerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCServer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCServer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCServer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCServer_AddGroup(This,szName,bActive,dwRequestedUpdateRate,hClientGroup,pTimeBias,pPercentDeadband,dwLCID,phServerGroup,pRevisedUpdateRate,riid,ppUnk)	\
    (This)->lpVtbl -> AddGroup(This,szName,bActive,dwRequestedUpdateRate,hClientGroup,pTimeBias,pPercentDeadband,dwLCID,phServerGroup,pRevisedUpdateRate,riid,ppUnk)

#define IOPCServer_GetErrorString(This,dwError,dwLocale,ppString)	\
    (This)->lpVtbl -> GetErrorString(This,dwError,dwLocale,ppString)

#define IOPCServer_GetGroupByName(This,szName,riid,ppUnk)	\
    (This)->lpVtbl -> GetGroupByName(This,szName,riid,ppUnk)

#define IOPCServer_GetStatus(This,ppServerStatus)	\
    (This)->lpVtbl -> GetStatus(This,ppServerStatus)

#define IOPCServer_RemoveGroup(This,hServerGroup,bForce)	\
    (This)->lpVtbl -> RemoveGroup(This,hServerGroup,bForce)

#define IOPCServer_CreateGroupEnumerator(This,dwScope,riid,ppUnk)	\
    (This)->lpVtbl -> CreateGroupEnumerator(This,dwScope,riid,ppUnk)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCServer_AddGroup_Proxy( 
    IOPCServer __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szName,
    /* [in] */ BOOL bActive,
    /* [in] */ DWORD dwRequestedUpdateRate,
    /* [in] */ OPCHANDLE hClientGroup,
    /* [in][unique] */ LONG __RPC_FAR *pTimeBias,
    /* [in][unique] */ FLOAT __RPC_FAR *pPercentDeadband,
    /* [in] */ DWORD dwLCID,
    /* [out] */ OPCHANDLE __RPC_FAR *phServerGroup,
    /* [out] */ DWORD __RPC_FAR *pRevisedUpdateRate,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);


void __RPC_STUB IOPCServer_AddGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCServer_GetErrorString_Proxy( 
    IOPCServer __RPC_FAR * This,
    /* [in] */ HRESULT dwError,
    /* [in] */ LCID dwLocale,
    /* [string][out] */ LPWSTR __RPC_FAR *ppString);


void __RPC_STUB IOPCServer_GetErrorString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCServer_GetGroupByName_Proxy( 
    IOPCServer __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szName,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);


void __RPC_STUB IOPCServer_GetGroupByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCServer_GetStatus_Proxy( 
    IOPCServer __RPC_FAR * This,
    /* [out] */ OPCSERVERSTATUS __RPC_FAR *__RPC_FAR *ppServerStatus);


void __RPC_STUB IOPCServer_GetStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCServer_RemoveGroup_Proxy( 
    IOPCServer __RPC_FAR * This,
    /* [in] */ OPCHANDLE hServerGroup,
    /* [in] */ BOOL bForce);


void __RPC_STUB IOPCServer_RemoveGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCServer_CreateGroupEnumerator_Proxy( 
    IOPCServer __RPC_FAR * This,
    /* [in] */ OPCENUMSCOPE dwScope,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);


void __RPC_STUB IOPCServer_CreateGroupEnumerator_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCServer_INTERFACE_DEFINED__ */


#ifndef __IOPCServerPublicGroups_INTERFACE_DEFINED__
#define __IOPCServerPublicGroups_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOPCServerPublicGroups
 * at Wed Oct 14 12:14:58 1998
 * using MIDL 3.01.75
 ****************************************/
/* [unique][uuid][object] */ 



EXTERN_C const IID IID_IOPCServerPublicGroups;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("39c13a4e-011e-11d0-9675-0020afd8adb3")
    IOPCServerPublicGroups : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetPublicGroupByName( 
            /* [string][in] */ LPCWSTR szName,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemovePublicGroup( 
            /* [in] */ OPCHANDLE hServerGroup,
            /* [in] */ BOOL bForce) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCServerPublicGroupsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCServerPublicGroups __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCServerPublicGroups __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCServerPublicGroups __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPublicGroupByName )( 
            IOPCServerPublicGroups __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szName,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemovePublicGroup )( 
            IOPCServerPublicGroups __RPC_FAR * This,
            /* [in] */ OPCHANDLE hServerGroup,
            /* [in] */ BOOL bForce);
        
        END_INTERFACE
    } IOPCServerPublicGroupsVtbl;

    interface IOPCServerPublicGroups
    {
        CONST_VTBL struct IOPCServerPublicGroupsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCServerPublicGroups_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCServerPublicGroups_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCServerPublicGroups_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCServerPublicGroups_GetPublicGroupByName(This,szName,riid,ppUnk)	\
    (This)->lpVtbl -> GetPublicGroupByName(This,szName,riid,ppUnk)

#define IOPCServerPublicGroups_RemovePublicGroup(This,hServerGroup,bForce)	\
    (This)->lpVtbl -> RemovePublicGroup(This,hServerGroup,bForce)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCServerPublicGroups_GetPublicGroupByName_Proxy( 
    IOPCServerPublicGroups __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szName,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);


void __RPC_STUB IOPCServerPublicGroups_GetPublicGroupByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCServerPublicGroups_RemovePublicGroup_Proxy( 
    IOPCServerPublicGroups __RPC_FAR * This,
    /* [in] */ OPCHANDLE hServerGroup,
    /* [in] */ BOOL bForce);


void __RPC_STUB IOPCServerPublicGroups_RemovePublicGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCServerPublicGroups_INTERFACE_DEFINED__ */


#ifndef __IOPCBrowseServerAddressSpace_INTERFACE_DEFINED__
#define __IOPCBrowseServerAddressSpace_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOPCBrowseServerAddressSpace
 * at Wed Oct 14 12:14:58 1998
 * using MIDL 3.01.75
 ****************************************/
/* [unique][uuid][object] */ 



EXTERN_C const IID IID_IOPCBrowseServerAddressSpace;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("39c13a4f-011e-11d0-9675-0020afd8adb3")
    IOPCBrowseServerAddressSpace : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QueryOrganization( 
            /* [out] */ OPCNAMESPACETYPE __RPC_FAR *pNameSpaceType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ChangeBrowsePosition( 
            /* [in] */ OPCBROWSEDIRECTION dwBrowseDirection,
            /* [string][in] */ LPCWSTR szString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BrowseOPCItemIDs( 
            /* [in] */ OPCBROWSETYPE dwBrowseFilterType,
            /* [string][in] */ LPCWSTR szFilterCriteria,
            /* [in] */ VARTYPE vtDataTypeFilter,
            /* [in] */ DWORD dwAccessRightsFilter,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetItemID( 
            /* [in] */ LPWSTR szItemDataID,
            /* [string][out] */ LPWSTR __RPC_FAR *szItemID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BrowseAccessPaths( 
            /* [string][in] */ LPCWSTR szItemID,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCBrowseServerAddressSpaceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCBrowseServerAddressSpace __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCBrowseServerAddressSpace __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCBrowseServerAddressSpace __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryOrganization )( 
            IOPCBrowseServerAddressSpace __RPC_FAR * This,
            /* [out] */ OPCNAMESPACETYPE __RPC_FAR *pNameSpaceType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ChangeBrowsePosition )( 
            IOPCBrowseServerAddressSpace __RPC_FAR * This,
            /* [in] */ OPCBROWSEDIRECTION dwBrowseDirection,
            /* [string][in] */ LPCWSTR szString);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BrowseOPCItemIDs )( 
            IOPCBrowseServerAddressSpace __RPC_FAR * This,
            /* [in] */ OPCBROWSETYPE dwBrowseFilterType,
            /* [string][in] */ LPCWSTR szFilterCriteria,
            /* [in] */ VARTYPE vtDataTypeFilter,
            /* [in] */ DWORD dwAccessRightsFilter,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItemID )( 
            IOPCBrowseServerAddressSpace __RPC_FAR * This,
            /* [in] */ LPWSTR szItemDataID,
            /* [string][out] */ LPWSTR __RPC_FAR *szItemID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BrowseAccessPaths )( 
            IOPCBrowseServerAddressSpace __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szItemID,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);
        
        END_INTERFACE
    } IOPCBrowseServerAddressSpaceVtbl;

    interface IOPCBrowseServerAddressSpace
    {
        CONST_VTBL struct IOPCBrowseServerAddressSpaceVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCBrowseServerAddressSpace_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCBrowseServerAddressSpace_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCBrowseServerAddressSpace_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCBrowseServerAddressSpace_QueryOrganization(This,pNameSpaceType)	\
    (This)->lpVtbl -> QueryOrganization(This,pNameSpaceType)

#define IOPCBrowseServerAddressSpace_ChangeBrowsePosition(This,dwBrowseDirection,szString)	\
    (This)->lpVtbl -> ChangeBrowsePosition(This,dwBrowseDirection,szString)

#define IOPCBrowseServerAddressSpace_BrowseOPCItemIDs(This,dwBrowseFilterType,szFilterCriteria,vtDataTypeFilter,dwAccessRightsFilter,ppIEnumString)	\
    (This)->lpVtbl -> BrowseOPCItemIDs(This,dwBrowseFilterType,szFilterCriteria,vtDataTypeFilter,dwAccessRightsFilter,ppIEnumString)

#define IOPCBrowseServerAddressSpace_GetItemID(This,szItemDataID,szItemID)	\
    (This)->lpVtbl -> GetItemID(This,szItemDataID,szItemID)

#define IOPCBrowseServerAddressSpace_BrowseAccessPaths(This,szItemID,ppIEnumString)	\
    (This)->lpVtbl -> BrowseAccessPaths(This,szItemID,ppIEnumString)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCBrowseServerAddressSpace_QueryOrganization_Proxy( 
    IOPCBrowseServerAddressSpace __RPC_FAR * This,
    /* [out] */ OPCNAMESPACETYPE __RPC_FAR *pNameSpaceType);


void __RPC_STUB IOPCBrowseServerAddressSpace_QueryOrganization_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCBrowseServerAddressSpace_ChangeBrowsePosition_Proxy( 
    IOPCBrowseServerAddressSpace __RPC_FAR * This,
    /* [in] */ OPCBROWSEDIRECTION dwBrowseDirection,
    /* [string][in] */ LPCWSTR szString);


void __RPC_STUB IOPCBrowseServerAddressSpace_ChangeBrowsePosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCBrowseServerAddressSpace_BrowseOPCItemIDs_Proxy( 
    IOPCBrowseServerAddressSpace __RPC_FAR * This,
    /* [in] */ OPCBROWSETYPE dwBrowseFilterType,
    /* [string][in] */ LPCWSTR szFilterCriteria,
    /* [in] */ VARTYPE vtDataTypeFilter,
    /* [in] */ DWORD dwAccessRightsFilter,
    /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);


void __RPC_STUB IOPCBrowseServerAddressSpace_BrowseOPCItemIDs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCBrowseServerAddressSpace_GetItemID_Proxy( 
    IOPCBrowseServerAddressSpace __RPC_FAR * This,
    /* [in] */ LPWSTR szItemDataID,
    /* [string][out] */ LPWSTR __RPC_FAR *szItemID);


void __RPC_STUB IOPCBrowseServerAddressSpace_GetItemID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCBrowseServerAddressSpace_BrowseAccessPaths_Proxy( 
    IOPCBrowseServerAddressSpace __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szItemID,
    /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);


void __RPC_STUB IOPCBrowseServerAddressSpace_BrowseAccessPaths_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCBrowseServerAddressSpace_INTERFACE_DEFINED__ */


#ifndef __IOPCGroupStateMgt_INTERFACE_DEFINED__
#define __IOPCGroupStateMgt_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOPCGroupStateMgt
 * at Wed Oct 14 12:14:58 1998
 * using MIDL 3.01.75
 ****************************************/
/* [unique][uuid][object] */ 



EXTERN_C const IID IID_IOPCGroupStateMgt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("39c13a50-011e-11d0-9675-0020afd8adb3")
    IOPCGroupStateMgt : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetState( 
            /* [out] */ DWORD __RPC_FAR *pUpdateRate,
            /* [out] */ BOOL __RPC_FAR *pActive,
            /* [string][out] */ LPWSTR __RPC_FAR *ppName,
            /* [out] */ LONG __RPC_FAR *pTimeBias,
            /* [out] */ FLOAT __RPC_FAR *pPercentDeadband,
            /* [out] */ DWORD __RPC_FAR *pLCID,
            /* [out] */ OPCHANDLE __RPC_FAR *phClientGroup,
            /* [out] */ OPCHANDLE __RPC_FAR *phServerGroup) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetState( 
            /* [in][unique] */ DWORD __RPC_FAR *pRequestedUpdateRate,
            /* [out] */ DWORD __RPC_FAR *pRevisedUpdateRate,
            /* [in][unique] */ BOOL __RPC_FAR *pActive,
            /* [in][unique] */ LONG __RPC_FAR *pTimeBias,
            /* [in][unique] */ FLOAT __RPC_FAR *pPercentDeadband,
            /* [in][unique] */ DWORD __RPC_FAR *pLCID,
            /* [in][unique] */ OPCHANDLE __RPC_FAR *phClientGroup) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetName( 
            /* [string][in] */ LPCWSTR szName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CloneGroup( 
            /* [string][in] */ LPCWSTR szName,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCGroupStateMgtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCGroupStateMgt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCGroupStateMgt __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCGroupStateMgt __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetState )( 
            IOPCGroupStateMgt __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pUpdateRate,
            /* [out] */ BOOL __RPC_FAR *pActive,
            /* [string][out] */ LPWSTR __RPC_FAR *ppName,
            /* [out] */ LONG __RPC_FAR *pTimeBias,
            /* [out] */ FLOAT __RPC_FAR *pPercentDeadband,
            /* [out] */ DWORD __RPC_FAR *pLCID,
            /* [out] */ OPCHANDLE __RPC_FAR *phClientGroup,
            /* [out] */ OPCHANDLE __RPC_FAR *phServerGroup);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetState )( 
            IOPCGroupStateMgt __RPC_FAR * This,
            /* [in][unique] */ DWORD __RPC_FAR *pRequestedUpdateRate,
            /* [out] */ DWORD __RPC_FAR *pRevisedUpdateRate,
            /* [in][unique] */ BOOL __RPC_FAR *pActive,
            /* [in][unique] */ LONG __RPC_FAR *pTimeBias,
            /* [in][unique] */ FLOAT __RPC_FAR *pPercentDeadband,
            /* [in][unique] */ DWORD __RPC_FAR *pLCID,
            /* [in][unique] */ OPCHANDLE __RPC_FAR *phClientGroup);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetName )( 
            IOPCGroupStateMgt __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloneGroup )( 
            IOPCGroupStateMgt __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szName,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);
        
        END_INTERFACE
    } IOPCGroupStateMgtVtbl;

    interface IOPCGroupStateMgt
    {
        CONST_VTBL struct IOPCGroupStateMgtVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCGroupStateMgt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCGroupStateMgt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCGroupStateMgt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCGroupStateMgt_GetState(This,pUpdateRate,pActive,ppName,pTimeBias,pPercentDeadband,pLCID,phClientGroup,phServerGroup)	\
    (This)->lpVtbl -> GetState(This,pUpdateRate,pActive,ppName,pTimeBias,pPercentDeadband,pLCID,phClientGroup,phServerGroup)

#define IOPCGroupStateMgt_SetState(This,pRequestedUpdateRate,pRevisedUpdateRate,pActive,pTimeBias,pPercentDeadband,pLCID,phClientGroup)	\
    (This)->lpVtbl -> SetState(This,pRequestedUpdateRate,pRevisedUpdateRate,pActive,pTimeBias,pPercentDeadband,pLCID,phClientGroup)

#define IOPCGroupStateMgt_SetName(This,szName)	\
    (This)->lpVtbl -> SetName(This,szName)

#define IOPCGroupStateMgt_CloneGroup(This,szName,riid,ppUnk)	\
    (This)->lpVtbl -> CloneGroup(This,szName,riid,ppUnk)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCGroupStateMgt_GetState_Proxy( 
    IOPCGroupStateMgt __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pUpdateRate,
    /* [out] */ BOOL __RPC_FAR *pActive,
    /* [string][out] */ LPWSTR __RPC_FAR *ppName,
    /* [out] */ LONG __RPC_FAR *pTimeBias,
    /* [out] */ FLOAT __RPC_FAR *pPercentDeadband,
    /* [out] */ DWORD __RPC_FAR *pLCID,
    /* [out] */ OPCHANDLE __RPC_FAR *phClientGroup,
    /* [out] */ OPCHANDLE __RPC_FAR *phServerGroup);


void __RPC_STUB IOPCGroupStateMgt_GetState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCGroupStateMgt_SetState_Proxy( 
    IOPCGroupStateMgt __RPC_FAR * This,
    /* [in][unique] */ DWORD __RPC_FAR *pRequestedUpdateRate,
    /* [out] */ DWORD __RPC_FAR *pRevisedUpdateRate,
    /* [in][unique] */ BOOL __RPC_FAR *pActive,
    /* [in][unique] */ LONG __RPC_FAR *pTimeBias,
    /* [in][unique] */ FLOAT __RPC_FAR *pPercentDeadband,
    /* [in][unique] */ DWORD __RPC_FAR *pLCID,
    /* [in][unique] */ OPCHANDLE __RPC_FAR *phClientGroup);


void __RPC_STUB IOPCGroupStateMgt_SetState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCGroupStateMgt_SetName_Proxy( 
    IOPCGroupStateMgt __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szName);


void __RPC_STUB IOPCGroupStateMgt_SetName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCGroupStateMgt_CloneGroup_Proxy( 
    IOPCGroupStateMgt __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szName,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);


void __RPC_STUB IOPCGroupStateMgt_CloneGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCGroupStateMgt_INTERFACE_DEFINED__ */


#ifndef __IOPCPublicGroupStateMgt_INTERFACE_DEFINED__
#define __IOPCPublicGroupStateMgt_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOPCPublicGroupStateMgt
 * at Wed Oct 14 12:14:58 1998
 * using MIDL 3.01.75
 ****************************************/
/* [unique][uuid][object] */ 



EXTERN_C const IID IID_IOPCPublicGroupStateMgt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("39c13a51-011e-11d0-9675-0020afd8adb3")
    IOPCPublicGroupStateMgt : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetState( 
            /* [out] */ BOOL __RPC_FAR *pPublic) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveToPublic( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCPublicGroupStateMgtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCPublicGroupStateMgt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCPublicGroupStateMgt __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCPublicGroupStateMgt __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetState )( 
            IOPCPublicGroupStateMgt __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pPublic);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MoveToPublic )( 
            IOPCPublicGroupStateMgt __RPC_FAR * This);
        
        END_INTERFACE
    } IOPCPublicGroupStateMgtVtbl;

    interface IOPCPublicGroupStateMgt
    {
        CONST_VTBL struct IOPCPublicGroupStateMgtVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCPublicGroupStateMgt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCPublicGroupStateMgt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCPublicGroupStateMgt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCPublicGroupStateMgt_GetState(This,pPublic)	\
    (This)->lpVtbl -> GetState(This,pPublic)

#define IOPCPublicGroupStateMgt_MoveToPublic(This)	\
    (This)->lpVtbl -> MoveToPublic(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCPublicGroupStateMgt_GetState_Proxy( 
    IOPCPublicGroupStateMgt __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pPublic);


void __RPC_STUB IOPCPublicGroupStateMgt_GetState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCPublicGroupStateMgt_MoveToPublic_Proxy( 
    IOPCPublicGroupStateMgt __RPC_FAR * This);


void __RPC_STUB IOPCPublicGroupStateMgt_MoveToPublic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCPublicGroupStateMgt_INTERFACE_DEFINED__ */


#ifndef __IOPCSyncIO_INTERFACE_DEFINED__
#define __IOPCSyncIO_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOPCSyncIO
 * at Wed Oct 14 12:14:58 1998
 * using MIDL 3.01.75
 ****************************************/
/* [unique][uuid][object] */ 



EXTERN_C const IID IID_IOPCSyncIO;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("39c13a52-011e-11d0-9675-0020afd8adb3")
    IOPCSyncIO : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Read( 
            /* [in] */ OPCDATASOURCE dwSource,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ OPCITEMSTATE __RPC_FAR *__RPC_FAR *ppItemValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Write( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ VARIANT __RPC_FAR *pItemValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCSyncIOVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCSyncIO __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCSyncIO __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCSyncIO __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Read )( 
            IOPCSyncIO __RPC_FAR * This,
            /* [in] */ OPCDATASOURCE dwSource,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ OPCITEMSTATE __RPC_FAR *__RPC_FAR *ppItemValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Write )( 
            IOPCSyncIO __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ VARIANT __RPC_FAR *pItemValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        END_INTERFACE
    } IOPCSyncIOVtbl;

    interface IOPCSyncIO
    {
        CONST_VTBL struct IOPCSyncIOVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCSyncIO_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCSyncIO_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCSyncIO_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCSyncIO_Read(This,dwSource,dwCount,phServer,ppItemValues,ppErrors)	\
    (This)->lpVtbl -> Read(This,dwSource,dwCount,phServer,ppItemValues,ppErrors)

#define IOPCSyncIO_Write(This,dwCount,phServer,pItemValues,ppErrors)	\
    (This)->lpVtbl -> Write(This,dwCount,phServer,pItemValues,ppErrors)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCSyncIO_Read_Proxy( 
    IOPCSyncIO __RPC_FAR * This,
    /* [in] */ OPCDATASOURCE dwSource,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][size_is][out] */ OPCITEMSTATE __RPC_FAR *__RPC_FAR *ppItemValues,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCSyncIO_Read_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCSyncIO_Write_Proxy( 
    IOPCSyncIO __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ VARIANT __RPC_FAR *pItemValues,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCSyncIO_Write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCSyncIO_INTERFACE_DEFINED__ */


#ifndef __IOPCAsyncIO_INTERFACE_DEFINED__
#define __IOPCAsyncIO_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOPCAsyncIO
 * at Wed Oct 14 12:14:58 1998
 * using MIDL 3.01.75
 ****************************************/
/* [unique][uuid][object] */ 



EXTERN_C const IID IID_IOPCAsyncIO;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("39c13a53-011e-11d0-9675-0020afd8adb3")
    IOPCAsyncIO : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Read( 
            /* [in] */ DWORD dwConnection,
            /* [in] */ OPCDATASOURCE dwSource,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pTransactionID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Write( 
            /* [in] */ DWORD dwConnection,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ VARIANT __RPC_FAR *pItemValues,
            /* [out] */ DWORD __RPC_FAR *pTransactionID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Refresh( 
            /* [in] */ DWORD dwConnection,
            /* [in] */ OPCDATASOURCE dwSource,
            /* [out] */ DWORD __RPC_FAR *pTransactionID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Cancel( 
            /* [in] */ DWORD dwTransactionID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCAsyncIOVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCAsyncIO __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCAsyncIO __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCAsyncIO __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Read )( 
            IOPCAsyncIO __RPC_FAR * This,
            /* [in] */ DWORD dwConnection,
            /* [in] */ OPCDATASOURCE dwSource,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [out] */ DWORD __RPC_FAR *pTransactionID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Write )( 
            IOPCAsyncIO __RPC_FAR * This,
            /* [in] */ DWORD dwConnection,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ VARIANT __RPC_FAR *pItemValues,
            /* [out] */ DWORD __RPC_FAR *pTransactionID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh )( 
            IOPCAsyncIO __RPC_FAR * This,
            /* [in] */ DWORD dwConnection,
            /* [in] */ OPCDATASOURCE dwSource,
            /* [out] */ DWORD __RPC_FAR *pTransactionID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Cancel )( 
            IOPCAsyncIO __RPC_FAR * This,
            /* [in] */ DWORD dwTransactionID);
        
        END_INTERFACE
    } IOPCAsyncIOVtbl;

    interface IOPCAsyncIO
    {
        CONST_VTBL struct IOPCAsyncIOVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCAsyncIO_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCAsyncIO_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCAsyncIO_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCAsyncIO_Read(This,dwConnection,dwSource,dwCount,phServer,pTransactionID,ppErrors)	\
    (This)->lpVtbl -> Read(This,dwConnection,dwSource,dwCount,phServer,pTransactionID,ppErrors)

#define IOPCAsyncIO_Write(This,dwConnection,dwCount,phServer,pItemValues,pTransactionID,ppErrors)	\
    (This)->lpVtbl -> Write(This,dwConnection,dwCount,phServer,pItemValues,pTransactionID,ppErrors)

#define IOPCAsyncIO_Refresh(This,dwConnection,dwSource,pTransactionID)	\
    (This)->lpVtbl -> Refresh(This,dwConnection,dwSource,pTransactionID)

#define IOPCAsyncIO_Cancel(This,dwTransactionID)	\
    (This)->lpVtbl -> Cancel(This,dwTransactionID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCAsyncIO_Read_Proxy( 
    IOPCAsyncIO __RPC_FAR * This,
    /* [in] */ DWORD dwConnection,
    /* [in] */ OPCDATASOURCE dwSource,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [out] */ DWORD __RPC_FAR *pTransactionID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCAsyncIO_Read_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCAsyncIO_Write_Proxy( 
    IOPCAsyncIO __RPC_FAR * This,
    /* [in] */ DWORD dwConnection,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ VARIANT __RPC_FAR *pItemValues,
    /* [out] */ DWORD __RPC_FAR *pTransactionID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCAsyncIO_Write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCAsyncIO_Refresh_Proxy( 
    IOPCAsyncIO __RPC_FAR * This,
    /* [in] */ DWORD dwConnection,
    /* [in] */ OPCDATASOURCE dwSource,
    /* [out] */ DWORD __RPC_FAR *pTransactionID);


void __RPC_STUB IOPCAsyncIO_Refresh_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCAsyncIO_Cancel_Proxy( 
    IOPCAsyncIO __RPC_FAR * This,
    /* [in] */ DWORD dwTransactionID);


void __RPC_STUB IOPCAsyncIO_Cancel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCAsyncIO_INTERFACE_DEFINED__ */


#ifndef __IOPCItemMgt_INTERFACE_DEFINED__
#define __IOPCItemMgt_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOPCItemMgt
 * at Wed Oct 14 12:14:58 1998
 * using MIDL 3.01.75
 ****************************************/
/* [unique][uuid][object] */ 



EXTERN_C const IID IID_IOPCItemMgt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("39c13a54-011e-11d0-9675-0020afd8adb3")
    IOPCItemMgt : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AddItems( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCITEMDEF __RPC_FAR *pItemArray,
            /* [size_is][size_is][out] */ OPCITEMRESULT __RPC_FAR *__RPC_FAR *ppAddResults,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ValidateItems( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCITEMDEF __RPC_FAR *pItemArray,
            /* [in] */ BOOL bBlobUpdate,
            /* [size_is][size_is][out] */ OPCITEMRESULT __RPC_FAR *__RPC_FAR *ppValidationResults,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveItems( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetActiveState( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [in] */ BOOL bActive,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetClientHandles( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClient,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDatatypes( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ VARTYPE __RPC_FAR *pRequestedDatatypes,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateEnumerator( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCItemMgtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCItemMgt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCItemMgt __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCItemMgt __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddItems )( 
            IOPCItemMgt __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCITEMDEF __RPC_FAR *pItemArray,
            /* [size_is][size_is][out] */ OPCITEMRESULT __RPC_FAR *__RPC_FAR *ppAddResults,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateItems )( 
            IOPCItemMgt __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCITEMDEF __RPC_FAR *pItemArray,
            /* [in] */ BOOL bBlobUpdate,
            /* [size_is][size_is][out] */ OPCITEMRESULT __RPC_FAR *__RPC_FAR *ppValidationResults,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveItems )( 
            IOPCItemMgt __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetActiveState )( 
            IOPCItemMgt __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [in] */ BOOL bActive,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetClientHandles )( 
            IOPCItemMgt __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClient,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDatatypes )( 
            IOPCItemMgt __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ VARTYPE __RPC_FAR *pRequestedDatatypes,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateEnumerator )( 
            IOPCItemMgt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);
        
        END_INTERFACE
    } IOPCItemMgtVtbl;

    interface IOPCItemMgt
    {
        CONST_VTBL struct IOPCItemMgtVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCItemMgt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCItemMgt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCItemMgt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCItemMgt_AddItems(This,dwCount,pItemArray,ppAddResults,ppErrors)	\
    (This)->lpVtbl -> AddItems(This,dwCount,pItemArray,ppAddResults,ppErrors)

#define IOPCItemMgt_ValidateItems(This,dwCount,pItemArray,bBlobUpdate,ppValidationResults,ppErrors)	\
    (This)->lpVtbl -> ValidateItems(This,dwCount,pItemArray,bBlobUpdate,ppValidationResults,ppErrors)

#define IOPCItemMgt_RemoveItems(This,dwCount,phServer,ppErrors)	\
    (This)->lpVtbl -> RemoveItems(This,dwCount,phServer,ppErrors)

#define IOPCItemMgt_SetActiveState(This,dwCount,phServer,bActive,ppErrors)	\
    (This)->lpVtbl -> SetActiveState(This,dwCount,phServer,bActive,ppErrors)

#define IOPCItemMgt_SetClientHandles(This,dwCount,phServer,phClient,ppErrors)	\
    (This)->lpVtbl -> SetClientHandles(This,dwCount,phServer,phClient,ppErrors)

#define IOPCItemMgt_SetDatatypes(This,dwCount,phServer,pRequestedDatatypes,ppErrors)	\
    (This)->lpVtbl -> SetDatatypes(This,dwCount,phServer,pRequestedDatatypes,ppErrors)

#define IOPCItemMgt_CreateEnumerator(This,riid,ppUnk)	\
    (This)->lpVtbl -> CreateEnumerator(This,riid,ppUnk)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCItemMgt_AddItems_Proxy( 
    IOPCItemMgt __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCITEMDEF __RPC_FAR *pItemArray,
    /* [size_is][size_is][out] */ OPCITEMRESULT __RPC_FAR *__RPC_FAR *ppAddResults,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCItemMgt_AddItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCItemMgt_ValidateItems_Proxy( 
    IOPCItemMgt __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCITEMDEF __RPC_FAR *pItemArray,
    /* [in] */ BOOL bBlobUpdate,
    /* [size_is][size_is][out] */ OPCITEMRESULT __RPC_FAR *__RPC_FAR *ppValidationResults,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCItemMgt_ValidateItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCItemMgt_RemoveItems_Proxy( 
    IOPCItemMgt __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCItemMgt_RemoveItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCItemMgt_SetActiveState_Proxy( 
    IOPCItemMgt __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [in] */ BOOL bActive,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCItemMgt_SetActiveState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCItemMgt_SetClientHandles_Proxy( 
    IOPCItemMgt __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClient,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCItemMgt_SetClientHandles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCItemMgt_SetDatatypes_Proxy( 
    IOPCItemMgt __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ VARTYPE __RPC_FAR *pRequestedDatatypes,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCItemMgt_SetDatatypes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCItemMgt_CreateEnumerator_Proxy( 
    IOPCItemMgt __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);


void __RPC_STUB IOPCItemMgt_CreateEnumerator_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCItemMgt_INTERFACE_DEFINED__ */


#ifndef __IEnumOPCItemAttributes_INTERFACE_DEFINED__
#define __IEnumOPCItemAttributes_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IEnumOPCItemAttributes
 * at Wed Oct 14 12:14:58 1998
 * using MIDL 3.01.75
 ****************************************/
/* [unique][uuid][object] */ 



EXTERN_C const IID IID_IEnumOPCItemAttributes;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("39c13a55-011e-11d0-9675-0020afd8adb3")
    IEnumOPCItemAttributes : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG celt,
            /* [size_is][size_is][out] */ OPCITEMATTRIBUTES __RPC_FAR *__RPC_FAR *ppItemArray,
            /* [out] */ ULONG __RPC_FAR *pceltFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG celt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumOPCItemAttributes __RPC_FAR *__RPC_FAR *ppEnumItemAttributes) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumOPCItemAttributesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEnumOPCItemAttributes __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEnumOPCItemAttributes __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEnumOPCItemAttributes __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Next )( 
            IEnumOPCItemAttributes __RPC_FAR * This,
            /* [in] */ ULONG celt,
            /* [size_is][size_is][out] */ OPCITEMATTRIBUTES __RPC_FAR *__RPC_FAR *ppItemArray,
            /* [out] */ ULONG __RPC_FAR *pceltFetched);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Skip )( 
            IEnumOPCItemAttributes __RPC_FAR * This,
            /* [in] */ ULONG celt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IEnumOPCItemAttributes __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IEnumOPCItemAttributes __RPC_FAR * This,
            /* [out] */ IEnumOPCItemAttributes __RPC_FAR *__RPC_FAR *ppEnumItemAttributes);
        
        END_INTERFACE
    } IEnumOPCItemAttributesVtbl;

    interface IEnumOPCItemAttributes
    {
        CONST_VTBL struct IEnumOPCItemAttributesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumOPCItemAttributes_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumOPCItemAttributes_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumOPCItemAttributes_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumOPCItemAttributes_Next(This,celt,ppItemArray,pceltFetched)	\
    (This)->lpVtbl -> Next(This,celt,ppItemArray,pceltFetched)

#define IEnumOPCItemAttributes_Skip(This,celt)	\
    (This)->lpVtbl -> Skip(This,celt)

#define IEnumOPCItemAttributes_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumOPCItemAttributes_Clone(This,ppEnumItemAttributes)	\
    (This)->lpVtbl -> Clone(This,ppEnumItemAttributes)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IEnumOPCItemAttributes_Next_Proxy( 
    IEnumOPCItemAttributes __RPC_FAR * This,
    /* [in] */ ULONG celt,
    /* [size_is][size_is][out] */ OPCITEMATTRIBUTES __RPC_FAR *__RPC_FAR *ppItemArray,
    /* [out] */ ULONG __RPC_FAR *pceltFetched);


void __RPC_STUB IEnumOPCItemAttributes_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumOPCItemAttributes_Skip_Proxy( 
    IEnumOPCItemAttributes __RPC_FAR * This,
    /* [in] */ ULONG celt);


void __RPC_STUB IEnumOPCItemAttributes_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumOPCItemAttributes_Reset_Proxy( 
    IEnumOPCItemAttributes __RPC_FAR * This);


void __RPC_STUB IEnumOPCItemAttributes_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumOPCItemAttributes_Clone_Proxy( 
    IEnumOPCItemAttributes __RPC_FAR * This,
    /* [out] */ IEnumOPCItemAttributes __RPC_FAR *__RPC_FAR *ppEnumItemAttributes);


void __RPC_STUB IEnumOPCItemAttributes_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumOPCItemAttributes_INTERFACE_DEFINED__ */


#ifndef __IOPCDataCallback_INTERFACE_DEFINED__
#define __IOPCDataCallback_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOPCDataCallback
 * at Wed Oct 14 12:14:58 1998
 * using MIDL 3.01.75
 ****************************************/
/* [unique][uuid][object] */ 



EXTERN_C const IID IID_IOPCDataCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("39c13a70-011e-11d0-9675-0020afd8adb3")
    IOPCDataCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnDataChange( 
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup,
            /* [in] */ HRESULT hrMasterquality,
            /* [in] */ HRESULT hrMastererror,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
            /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
            /* [size_is][in] */ WORD __RPC_FAR *pwQualities,
            /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
            /* [size_is][in] */ HRESULT __RPC_FAR *pErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnReadComplete( 
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup,
            /* [in] */ HRESULT hrMasterquality,
            /* [in] */ HRESULT hrMastererror,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
            /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
            /* [size_is][in] */ WORD __RPC_FAR *pwQualities,
            /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
            /* [size_is][in] */ HRESULT __RPC_FAR *pErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnWriteComplete( 
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup,
            /* [in] */ HRESULT hrMastererr,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *pClienthandles,
            /* [size_is][in] */ HRESULT __RPC_FAR *pErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnCancelComplete( 
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCDataCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCDataCallback __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCDataCallback __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCDataCallback __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnDataChange )( 
            IOPCDataCallback __RPC_FAR * This,
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup,
            /* [in] */ HRESULT hrMasterquality,
            /* [in] */ HRESULT hrMastererror,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
            /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
            /* [size_is][in] */ WORD __RPC_FAR *pwQualities,
            /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
            /* [size_is][in] */ HRESULT __RPC_FAR *pErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnReadComplete )( 
            IOPCDataCallback __RPC_FAR * This,
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup,
            /* [in] */ HRESULT hrMasterquality,
            /* [in] */ HRESULT hrMastererror,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
            /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
            /* [size_is][in] */ WORD __RPC_FAR *pwQualities,
            /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
            /* [size_is][in] */ HRESULT __RPC_FAR *pErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnWriteComplete )( 
            IOPCDataCallback __RPC_FAR * This,
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup,
            /* [in] */ HRESULT hrMastererr,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *pClienthandles,
            /* [size_is][in] */ HRESULT __RPC_FAR *pErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnCancelComplete )( 
            IOPCDataCallback __RPC_FAR * This,
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup);
        
        END_INTERFACE
    } IOPCDataCallbackVtbl;

    interface IOPCDataCallback
    {
        CONST_VTBL struct IOPCDataCallbackVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCDataCallback_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCDataCallback_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCDataCallback_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCDataCallback_OnDataChange(This,dwTransid,hGroup,hrMasterquality,hrMastererror,dwCount,phClientItems,pvValues,pwQualities,pftTimeStamps,pErrors)	\
    (This)->lpVtbl -> OnDataChange(This,dwTransid,hGroup,hrMasterquality,hrMastererror,dwCount,phClientItems,pvValues,pwQualities,pftTimeStamps,pErrors)

#define IOPCDataCallback_OnReadComplete(This,dwTransid,hGroup,hrMasterquality,hrMastererror,dwCount,phClientItems,pvValues,pwQualities,pftTimeStamps,pErrors)	\
    (This)->lpVtbl -> OnReadComplete(This,dwTransid,hGroup,hrMasterquality,hrMastererror,dwCount,phClientItems,pvValues,pwQualities,pftTimeStamps,pErrors)

#define IOPCDataCallback_OnWriteComplete(This,dwTransid,hGroup,hrMastererr,dwCount,pClienthandles,pErrors)	\
    (This)->lpVtbl -> OnWriteComplete(This,dwTransid,hGroup,hrMastererr,dwCount,pClienthandles,pErrors)

#define IOPCDataCallback_OnCancelComplete(This,dwTransid,hGroup)	\
    (This)->lpVtbl -> OnCancelComplete(This,dwTransid,hGroup)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCDataCallback_OnDataChange_Proxy( 
    IOPCDataCallback __RPC_FAR * This,
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup,
    /* [in] */ HRESULT hrMasterquality,
    /* [in] */ HRESULT hrMastererror,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
    /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
    /* [size_is][in] */ WORD __RPC_FAR *pwQualities,
    /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
    /* [size_is][in] */ HRESULT __RPC_FAR *pErrors);


void __RPC_STUB IOPCDataCallback_OnDataChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCDataCallback_OnReadComplete_Proxy( 
    IOPCDataCallback __RPC_FAR * This,
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup,
    /* [in] */ HRESULT hrMasterquality,
    /* [in] */ HRESULT hrMastererror,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
    /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
    /* [size_is][in] */ WORD __RPC_FAR *pwQualities,
    /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
    /* [size_is][in] */ HRESULT __RPC_FAR *pErrors);


void __RPC_STUB IOPCDataCallback_OnReadComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCDataCallback_OnWriteComplete_Proxy( 
    IOPCDataCallback __RPC_FAR * This,
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup,
    /* [in] */ HRESULT hrMastererr,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *pClienthandles,
    /* [size_is][in] */ HRESULT __RPC_FAR *pErrors);


void __RPC_STUB IOPCDataCallback_OnWriteComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCDataCallback_OnCancelComplete_Proxy( 
    IOPCDataCallback __RPC_FAR * This,
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup);


void __RPC_STUB IOPCDataCallback_OnCancelComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCDataCallback_INTERFACE_DEFINED__ */


#ifndef __IOPCAsyncIO2_INTERFACE_DEFINED__
#define __IOPCAsyncIO2_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOPCAsyncIO2
 * at Wed Oct 14 12:14:58 1998
 * using MIDL 3.01.75
 ****************************************/
/* [unique][uuid][object] */ 



EXTERN_C const IID IID_IOPCAsyncIO2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("39c13a71-011e-11d0-9675-0020afd8adb3")
    IOPCAsyncIO2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Read( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [in] */ DWORD dwTransactionID,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Write( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ VARIANT __RPC_FAR *pItemValues,
            /* [in] */ DWORD dwTransactionID,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Refresh2( 
            /* [in] */ OPCDATASOURCE dwSource,
            /* [in] */ DWORD dwTransactionID,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Cancel2( 
            /* [in] */ DWORD dwCancelID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEnable( 
            /* [in] */ BOOL bEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEnable( 
            /* [out] */ BOOL __RPC_FAR *pbEnable) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCAsyncIO2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCAsyncIO2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCAsyncIO2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCAsyncIO2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Read )( 
            IOPCAsyncIO2 __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [in] */ DWORD dwTransactionID,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Write )( 
            IOPCAsyncIO2 __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
            /* [size_is][in] */ VARIANT __RPC_FAR *pItemValues,
            /* [in] */ DWORD dwTransactionID,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh2 )( 
            IOPCAsyncIO2 __RPC_FAR * This,
            /* [in] */ OPCDATASOURCE dwSource,
            /* [in] */ DWORD dwTransactionID,
            /* [out] */ DWORD __RPC_FAR *pdwCancelID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Cancel2 )( 
            IOPCAsyncIO2 __RPC_FAR * This,
            /* [in] */ DWORD dwCancelID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEnable )( 
            IOPCAsyncIO2 __RPC_FAR * This,
            /* [in] */ BOOL bEnable);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEnable )( 
            IOPCAsyncIO2 __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pbEnable);
        
        END_INTERFACE
    } IOPCAsyncIO2Vtbl;

    interface IOPCAsyncIO2
    {
        CONST_VTBL struct IOPCAsyncIO2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCAsyncIO2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCAsyncIO2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCAsyncIO2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCAsyncIO2_Read(This,dwCount,phServer,dwTransactionID,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> Read(This,dwCount,phServer,dwTransactionID,pdwCancelID,ppErrors)

#define IOPCAsyncIO2_Write(This,dwCount,phServer,pItemValues,dwTransactionID,pdwCancelID,ppErrors)	\
    (This)->lpVtbl -> Write(This,dwCount,phServer,pItemValues,dwTransactionID,pdwCancelID,ppErrors)

#define IOPCAsyncIO2_Refresh2(This,dwSource,dwTransactionID,pdwCancelID)	\
    (This)->lpVtbl -> Refresh2(This,dwSource,dwTransactionID,pdwCancelID)

#define IOPCAsyncIO2_Cancel2(This,dwCancelID)	\
    (This)->lpVtbl -> Cancel2(This,dwCancelID)

#define IOPCAsyncIO2_SetEnable(This,bEnable)	\
    (This)->lpVtbl -> SetEnable(This,bEnable)

#define IOPCAsyncIO2_GetEnable(This,pbEnable)	\
    (This)->lpVtbl -> GetEnable(This,pbEnable)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCAsyncIO2_Read_Proxy( 
    IOPCAsyncIO2 __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [in] */ DWORD dwTransactionID,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCAsyncIO2_Read_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCAsyncIO2_Write_Proxy( 
    IOPCAsyncIO2 __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phServer,
    /* [size_is][in] */ VARIANT __RPC_FAR *pItemValues,
    /* [in] */ DWORD dwTransactionID,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCAsyncIO2_Write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCAsyncIO2_Refresh2_Proxy( 
    IOPCAsyncIO2 __RPC_FAR * This,
    /* [in] */ OPCDATASOURCE dwSource,
    /* [in] */ DWORD dwTransactionID,
    /* [out] */ DWORD __RPC_FAR *pdwCancelID);


void __RPC_STUB IOPCAsyncIO2_Refresh2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCAsyncIO2_Cancel2_Proxy( 
    IOPCAsyncIO2 __RPC_FAR * This,
    /* [in] */ DWORD dwCancelID);


void __RPC_STUB IOPCAsyncIO2_Cancel2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCAsyncIO2_SetEnable_Proxy( 
    IOPCAsyncIO2 __RPC_FAR * This,
    /* [in] */ BOOL bEnable);


void __RPC_STUB IOPCAsyncIO2_SetEnable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCAsyncIO2_GetEnable_Proxy( 
    IOPCAsyncIO2 __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pbEnable);


void __RPC_STUB IOPCAsyncIO2_GetEnable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCAsyncIO2_INTERFACE_DEFINED__ */


#ifndef __IOPCItemProperties_INTERFACE_DEFINED__
#define __IOPCItemProperties_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IOPCItemProperties
 * at Wed Oct 14 12:14:58 1998
 * using MIDL 3.01.75
 ****************************************/
/* [unique][uuid][object] */ 



EXTERN_C const IID IID_IOPCItemProperties;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("39c13a72-011e-11d0-9675-0020afd8adb3")
    IOPCItemProperties : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QueryAvailableProperties( 
            /* [in] */ LPWSTR szItemID,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppPropertyIDs,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppDescriptions,
            /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppvtDataTypes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetItemProperties( 
            /* [in] */ LPWSTR szItemID,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwPropertyIDs,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvData,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LookupItemIDs( 
            /* [in] */ LPWSTR szItemID,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwPropertyIDs,
            /* [size_is][size_is][string][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszNewItemIDs,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOPCItemPropertiesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOPCItemProperties __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOPCItemProperties __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOPCItemProperties __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryAvailableProperties )( 
            IOPCItemProperties __RPC_FAR * This,
            /* [in] */ LPWSTR szItemID,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppPropertyIDs,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppDescriptions,
            /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppvtDataTypes);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItemProperties )( 
            IOPCItemProperties __RPC_FAR * This,
            /* [in] */ LPWSTR szItemID,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwPropertyIDs,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvData,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LookupItemIDs )( 
            IOPCItemProperties __RPC_FAR * This,
            /* [in] */ LPWSTR szItemID,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwPropertyIDs,
            /* [size_is][size_is][string][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszNewItemIDs,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        END_INTERFACE
    } IOPCItemPropertiesVtbl;

    interface IOPCItemProperties
    {
        CONST_VTBL struct IOPCItemPropertiesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOPCItemProperties_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOPCItemProperties_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOPCItemProperties_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOPCItemProperties_QueryAvailableProperties(This,szItemID,pdwCount,ppPropertyIDs,ppDescriptions,ppvtDataTypes)	\
    (This)->lpVtbl -> QueryAvailableProperties(This,szItemID,pdwCount,ppPropertyIDs,ppDescriptions,ppvtDataTypes)

#define IOPCItemProperties_GetItemProperties(This,szItemID,dwCount,pdwPropertyIDs,ppvData,ppErrors)	\
    (This)->lpVtbl -> GetItemProperties(This,szItemID,dwCount,pdwPropertyIDs,ppvData,ppErrors)

#define IOPCItemProperties_LookupItemIDs(This,szItemID,dwCount,pdwPropertyIDs,ppszNewItemIDs,ppErrors)	\
    (This)->lpVtbl -> LookupItemIDs(This,szItemID,dwCount,pdwPropertyIDs,ppszNewItemIDs,ppErrors)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOPCItemProperties_QueryAvailableProperties_Proxy( 
    IOPCItemProperties __RPC_FAR * This,
    /* [in] */ LPWSTR szItemID,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppPropertyIDs,
    /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppDescriptions,
    /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppvtDataTypes);


void __RPC_STUB IOPCItemProperties_QueryAvailableProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCItemProperties_GetItemProperties_Proxy( 
    IOPCItemProperties __RPC_FAR * This,
    /* [in] */ LPWSTR szItemID,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwPropertyIDs,
    /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvData,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCItemProperties_GetItemProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOPCItemProperties_LookupItemIDs_Proxy( 
    IOPCItemProperties __RPC_FAR * This,
    /* [in] */ LPWSTR szItemID,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwPropertyIDs,
    /* [size_is][size_is][string][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppszNewItemIDs,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB IOPCItemProperties_LookupItemIDs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOPCItemProperties_INTERFACE_DEFINED__ */



#ifndef __OPCDA_LIBRARY_DEFINED__
#define __OPCDA_LIBRARY_DEFINED__

/****************************************
 * Generated header for library: OPCDA
 * at Wed Oct 14 12:14:58 1998
 * using MIDL 3.01.75
 ****************************************/
/* [helpstring][version][uuid] */ 















EXTERN_C const IID LIBID_OPCDA;
#endif /* __OPCDA_LIBRARY_DEFINED__ */

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
