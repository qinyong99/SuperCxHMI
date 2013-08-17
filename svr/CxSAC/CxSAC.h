/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Aug 11 11:01:46 2013
 */
/* Compiler settings for D:\spcx\svr\CxSAC\CxSAC.idl:
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

#ifndef __CxSAC_h__
#define __CxSAC_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ICxDataServer_FWD_DEFINED__
#define __ICxDataServer_FWD_DEFINED__
typedef interface ICxDataServer ICxDataServer;
#endif 	/* __ICxDataServer_FWD_DEFINED__ */


#ifndef __ICxUserManager_FWD_DEFINED__
#define __ICxUserManager_FWD_DEFINED__
typedef interface ICxUserManager ICxUserManager;
#endif 	/* __ICxUserManager_FWD_DEFINED__ */


#ifndef __ICxBrowseServerAddressSpace_FWD_DEFINED__
#define __ICxBrowseServerAddressSpace_FWD_DEFINED__
typedef interface ICxBrowseServerAddressSpace ICxBrowseServerAddressSpace;
#endif 	/* __ICxBrowseServerAddressSpace_FWD_DEFINED__ */


#ifndef __ICxTagGroup_FWD_DEFINED__
#define __ICxTagGroup_FWD_DEFINED__
typedef interface ICxTagGroup ICxTagGroup;
#endif 	/* __ICxTagGroup_FWD_DEFINED__ */


#ifndef __ICxDataClientCallback_FWD_DEFINED__
#define __ICxDataClientCallback_FWD_DEFINED__
typedef interface ICxDataClientCallback ICxDataClientCallback;
#endif 	/* __ICxDataClientCallback_FWD_DEFINED__ */


#ifndef __ICxEventSubscription_FWD_DEFINED__
#define __ICxEventSubscription_FWD_DEFINED__
typedef interface ICxEventSubscription ICxEventSubscription;
#endif 	/* __ICxEventSubscription_FWD_DEFINED__ */


#ifndef __ICxEventSink_FWD_DEFINED__
#define __ICxEventSink_FWD_DEFINED__
typedef interface ICxEventSink ICxEventSink;
#endif 	/* __ICxEventSink_FWD_DEFINED__ */


#ifndef __ICxServerSynchronization_FWD_DEFINED__
#define __ICxServerSynchronization_FWD_DEFINED__
typedef interface ICxServerSynchronization ICxServerSynchronization;
#endif 	/* __ICxServerSynchronization_FWD_DEFINED__ */


#ifndef __ICxSyncNotify_FWD_DEFINED__
#define __ICxSyncNotify_FWD_DEFINED__
typedef interface ICxSyncNotify ICxSyncNotify;
#endif 	/* __ICxSyncNotify_FWD_DEFINED__ */


#ifndef __ITimeSched_FWD_DEFINED__
#define __ITimeSched_FWD_DEFINED__
typedef interface ITimeSched ITimeSched;
#endif 	/* __ITimeSched_FWD_DEFINED__ */


#ifndef __IEventSched_FWD_DEFINED__
#define __IEventSched_FWD_DEFINED__
typedef interface IEventSched IEventSched;
#endif 	/* __IEventSched_FWD_DEFINED__ */


#ifndef __CxDataServer_FWD_DEFINED__
#define __CxDataServer_FWD_DEFINED__

#ifdef __cplusplus
typedef class CxDataServer CxDataServer;
#else
typedef struct CxDataServer CxDataServer;
#endif /* __cplusplus */

#endif 	/* __CxDataServer_FWD_DEFINED__ */


#ifndef ___ITimeSchedEvents_FWD_DEFINED__
#define ___ITimeSchedEvents_FWD_DEFINED__
typedef interface _ITimeSchedEvents _ITimeSchedEvents;
#endif 	/* ___ITimeSchedEvents_FWD_DEFINED__ */


#ifndef ___IEventSchedEvents_FWD_DEFINED__
#define ___IEventSchedEvents_FWD_DEFINED__
typedef interface _IEventSchedEvents _IEventSchedEvents;
#endif 	/* ___IEventSchedEvents_FWD_DEFINED__ */


#ifndef __TimeSched_FWD_DEFINED__
#define __TimeSched_FWD_DEFINED__

#ifdef __cplusplus
typedef class TimeSched TimeSched;
#else
typedef struct TimeSched TimeSched;
#endif /* __cplusplus */

#endif 	/* __TimeSched_FWD_DEFINED__ */


#ifndef __EventSched_FWD_DEFINED__
#define __EventSched_FWD_DEFINED__

#ifdef __cplusplus
typedef class EventSched EventSched;
#else
typedef struct EventSched EventSched;
#endif /* __cplusplus */

#endif 	/* __EventSched_FWD_DEFINED__ */


#ifndef __CxOPCServer_FWD_DEFINED__
#define __CxOPCServer_FWD_DEFINED__

#ifdef __cplusplus
typedef class CxOPCServer CxOPCServer;
#else
typedef struct CxOPCServer CxOPCServer;
#endif /* __cplusplus */

#endif 	/* __CxOPCServer_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_CxSAC_0000 */
/* [local] */ 

typedef DWORD CXHANDLE;

typedef 
enum tagCXBROWSETYPE
    {	CX_BRANCH	= 1,
	CX_LEAF	= CX_BRANCH + 1,
	CX_FLAT	= CX_LEAF + 1
    }	CXBROWSETYPE;

typedef 
enum tagCXBROWSEDIRECTION
    {	CX_BROWSE_UP	= 1,
	CX_BROWSE_DOWN	= CX_BROWSE_UP + 1,
	CX_BROWSE_TO	= CX_BROWSE_DOWN + 1
    }	CXBROWSEDIRECTION;

typedef 
enum tagCXSERVERSTATE
    {	CX_STATUS_RUNNING	= 1,
	CX_STATUS_FAILED	= CX_STATUS_RUNNING + 1,
	CX_STATUS_NOCONFIG	= CX_STATUS_FAILED + 1,
	CX_STATUS_SUSPENDED	= CX_STATUS_NOCONFIG + 1,
	CX_STATUS_TEST	= CX_STATUS_SUSPENDED + 1
    }	CXSERVERSTATE;

typedef struct  tagCXNETNODE
    {
    /* [string] */ LPWSTR szNode;
    /* [string] */ LPWSTR szComputer;
    /* [string] */ LPWSTR szBackupComputer;
    }	CXNETNODE;

typedef struct  tagCXITEMDEF
    {
    /* [string] */ LPWSTR szItemID;
    /* [string] */ LPWSTR szProp;
    CXHANDLE hClient;
    }	CXITEMDEF;

typedef struct  tagCXHDAITEM
    {
    DWORD dwCount;
    /* [size_is] */ FILETIME __RPC_FAR *pftTimeStamps;
    /* [size_is] */ VARIANT __RPC_FAR *pvDataValues;
    }	CXHDAITEM;

typedef struct  tagCXRUNPICSETTING
    {
    /* [string] */ LPWSTR szSplashImage;
    /* [string] */ LPWSTR szStartPicture;
    BOOL bFullScreen;
    BOOL bDisableSwitchFullScreen;
    BOOL bSystemMenu;
    /* [string] */ LPWSTR szCaption;
    BOOL bMaximizeBox;
    BOOL bMinimizeBox;
    BOOL bMainMenu;
    BOOL bThickFrame;
    /* [string] */ LPWSTR szExitPassword;
    BOOL bHighlightMouseObject;
    DWORD dwDisableKeyMask;
    BOOL bStopServerWhenExit;
    }	CXRUNPICSETTING;

typedef struct  tagCXLOGINUSERINFO
    {
    DWORD dwID;
    /* [string] */ LPWSTR szUserName;
    /* [string] */ LPWSTR szIP;
    FILETIME ftLoginTime;
    }	CXLOGINUSERINFO;

#define MAX_TAG_NAME 256
typedef struct  tagCXEVENTSTRUCT
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
    }	CXEVENTSTRUCT;

typedef struct  tagCXSERVERSTATUS
    {
    FILETIME ftStartTime;
    CXSERVERSTATE dwServerState;
    /* [string] */ LPWSTR szActiveProject;
    DWORD dwIOCount;
    DWORD dwLockIONumber;
    }	CXSERVERSTATUS;

typedef struct  tagCXDISPPARAMS
    {
    DWORD dwArgCount;
    /* [size_is] */ VARIANT __RPC_FAR *pvaArgs;
    }	CXDISPPARAMS;

#define CX_CONDITION_ENABLED				0x0001
#define CX_CONDITION_ACTIVE				0x0002
#define CX_CONDITION_ACKED				0x0004
#define CX_CHANGE_ACTIVE_STATE			0x0001
#define CX_CHANGE_ACK_STATE				0x0002
#define CX_CHANGE_ENABLE_STATE			0x0004
#define CX_CHANGE_QUALITY				0x0008
#define CX_CHANGE_SEVERITY				0x0010
#define CX_CHANGE_SUBCONDITION			0x0020
#define CX_CHANGE_MESSAGE				0x0040
#define CX_CHANGE_ATTRIBUTE				0x0080
#define CX_EVENT_SYSTEM					0x0001
#define CX_EVENT_ALARM					0x0002
#define CX_EVENT_OPERATION				0x0004
#define CX_EVENT_SOE						0x0008
#define CX_EVENT_ALL						(CX_EVENT_SYSTEM|CX_EVENT_ALARM|CX_EVENT_OPERATION|CX_EVENT_SOE)


extern RPC_IF_HANDLE __MIDL_itf_CxSAC_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_CxSAC_0000_v0_0_s_ifspec;

#ifndef __ICxDataServer_INTERFACE_DEFINED__
#define __ICxDataServer_INTERFACE_DEFINED__

/* interface ICxDataServer */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_ICxDataServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5175CCA2-F0EF-4354-80BF-2FE15EADA1F8")
    ICxDataServer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE UserLogin( 
            /* [string][in] */ LPCWSTR szUserName,
            /* [string][in] */ LPCWSTR szPassword,
            /* [string][in] */ LPCWSTR szIP,
            /* [in] */ BOOL bLocal,
            /* [out][in] */ DWORD __RPC_FAR *pdwCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUserAccessRight( 
            /* [string][out] */ LPWSTR __RPC_FAR *pSecurityAreas,
            /* [out] */ DWORD __RPC_FAR *pdwPrivilege) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UserLogout( 
            /* [out][in] */ DWORD __RPC_FAR *pdwCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNetNodes( 
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ CXNETNODE __RPC_FAR *__RPC_FAR *ppNodes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateTagGroup( 
            /* [string][in] */ LPCWSTR szName,
            /* [in] */ CXHANDLE hClientGroup,
            /* [out] */ CXHANDLE __RPC_FAR *phServerGroup,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateTagBrowser( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsTagAvailableProperty( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryTagAvailableProperties( 
            /* [in] */ LPWSTR szItemID,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppNames) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateEventSubscription( 
            /* [in] */ CXHANDLE hClientSubscription,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AckEventCondition( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ LPCWSTR __RPC_FAR *pszSource,
            /* [size_is][in] */ LPCWSTR __RPC_FAR *pszConditionName,
            /* [size_is][in] */ FILETIME __RPC_FAR *pftActiveTime,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwCookie,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LogUserOperation( 
            /* [string][in] */ LPCWSTR szSource,
            /* [string][in] */ LPCWSTR szMessage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LogSOEEvent( 
            /* [in] */ FILETIME ftTime,
            /* [string][in] */ LPCWSTR szSource,
            /* [string][in] */ LPCWSTR szMessage,
            /* [in] */ DWORD dwSeverity) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BrowseSecurityAreas( 
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BrowseEventAreas( 
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BrowseDevices( 
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BrowseTagLogTabs( 
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BrowseTagsOfLogTab( 
            /* [in] */ LPCWSTR szLogTab,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DownloadFile( 
            /* [in] */ LPCWSTR szFile,
            /* [out][in] */ FILETIME __RPC_FAR *pftFileTime,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ BYTE __RPC_FAR *__RPC_FAR *ppBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetActiveState( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateSynchronization( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SwitchMasterComputer( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetServerCurrentTime( 
            /* [out] */ FILETIME __RPC_FAR *pftTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LogDownloadRecipe( 
            /* [in] */ LPCWSTR szFile,
            /* [in] */ FILETIME ftStartTime,
            /* [in] */ FILETIME ftEndTime,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ LPCWSTR __RPC_FAR *pszItem,
            /* [size_is][in] */ VARIANT __RPC_FAR *pvaValues) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadEventHistory( 
            /* [in] */ FILETIME ftStartTime,
            /* [in] */ FILETIME ftEndTime,
            /* [in] */ DWORD dwEventType,
            /* [in] */ DWORD dwLowSeverity,
            /* [in] */ DWORD dwHighSeverity,
            /* [in] */ DWORD dwNumAreas,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreaList,
            /* [in] */ DWORD dwNumSources,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszSourceList,
            /* [in] */ DWORD dwNumActors,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszActorList,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ CXEVENTSTRUCT __RPC_FAR *__RPC_FAR *ppEvents) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEventCount( 
            /* [in] */ BOOL bAcked,
            /* [in] */ DWORD dwLowSeverity,
            /* [in] */ DWORD dwHighSeverity,
            /* [in] */ DWORD dwNumAreas,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreaList,
            /* [out] */ DWORD __RPC_FAR *pdwCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateUserManager( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRunPictureSetting( 
            /* [out] */ CXRUNPICSETTING __RPC_FAR *pSetting) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsModified( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetServerStatus( 
            /* [out] */ CXSERVERSTATUS __RPC_FAR *__RPC_FAR *ppStatus) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDeviceStatus( 
            /* [string][in] */ LPCWSTR szDevice,
            /* [string][out] */ LPWSTR __RPC_FAR *pszType,
            /* [out] */ DWORD __RPC_FAR *pdwState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDeviceStatus( 
            /* [string][in] */ LPCWSTR szDevice,
            /* [in] */ DWORD dwState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLoginUsersInfo( 
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ CXLOGINUSERINFO __RPC_FAR *__RPC_FAR *ppInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE VerifySoftLock( 
            /* [in] */ BOOL bAllowDemoStatus,
            /* [out] */ DWORD __RPC_FAR *pdwStatus,
            /* [out][in] */ DWORD __RPC_FAR *pdwEncryptData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateObject( 
            /* [in] */ VARIANT vaClassValue,
            /* [out] */ VARIANT __RPC_FAR *pvaObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ObjectBatchCall( 
            /* [in] */ IDispatch __RPC_FAR *pObject,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ LPCWSTR __RPC_FAR *ppszFunctions,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwFlags,
            /* [size_is][in] */ CXDISPPARAMS __RPC_FAR *pDispParams,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvarResultValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadTagPropertiesValue( 
            /* [in] */ DWORD dwTagType,
            /* [string][in] */ LPCWSTR szTagFilter,
            /* [in] */ DWORD dwNumProps,
            /* [size_is][in] */ LPCWSTR __RPC_FAR *pszPropList,
            /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppVartypes,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppSizes,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvaValues) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SaveTagStatus( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxDataServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxDataServer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxDataServer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UserLogin )( 
            ICxDataServer __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szUserName,
            /* [string][in] */ LPCWSTR szPassword,
            /* [string][in] */ LPCWSTR szIP,
            /* [in] */ BOOL bLocal,
            /* [out][in] */ DWORD __RPC_FAR *pdwCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUserAccessRight )( 
            ICxDataServer __RPC_FAR * This,
            /* [string][out] */ LPWSTR __RPC_FAR *pSecurityAreas,
            /* [out] */ DWORD __RPC_FAR *pdwPrivilege);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UserLogout )( 
            ICxDataServer __RPC_FAR * This,
            /* [out][in] */ DWORD __RPC_FAR *pdwCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNetNodes )( 
            ICxDataServer __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ CXNETNODE __RPC_FAR *__RPC_FAR *ppNodes);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateTagGroup )( 
            ICxDataServer __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szName,
            /* [in] */ CXHANDLE hClientGroup,
            /* [out] */ CXHANDLE __RPC_FAR *phServerGroup,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateTagBrowser )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsTagAvailableProperty )( 
            ICxDataServer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryTagAvailableProperties )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ LPWSTR szItemID,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppNames);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateEventSubscription )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ CXHANDLE hClientSubscription,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AckEventCondition )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ LPCWSTR __RPC_FAR *pszSource,
            /* [size_is][in] */ LPCWSTR __RPC_FAR *pszConditionName,
            /* [size_is][in] */ FILETIME __RPC_FAR *pftActiveTime,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwCookie,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LogUserOperation )( 
            ICxDataServer __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szSource,
            /* [string][in] */ LPCWSTR szMessage);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LogSOEEvent )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ FILETIME ftTime,
            /* [string][in] */ LPCWSTR szSource,
            /* [string][in] */ LPCWSTR szMessage,
            /* [in] */ DWORD dwSeverity);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BrowseSecurityAreas )( 
            ICxDataServer __RPC_FAR * This,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BrowseEventAreas )( 
            ICxDataServer __RPC_FAR * This,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BrowseDevices )( 
            ICxDataServer __RPC_FAR * This,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BrowseTagLogTabs )( 
            ICxDataServer __RPC_FAR * This,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BrowseTagsOfLogTab )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ LPCWSTR szLogTab,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DownloadFile )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ LPCWSTR szFile,
            /* [out][in] */ FILETIME __RPC_FAR *pftFileTime,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ BYTE __RPC_FAR *__RPC_FAR *ppBuffer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetActiveState )( 
            ICxDataServer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateSynchronization )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SwitchMasterComputer )( 
            ICxDataServer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetServerCurrentTime )( 
            ICxDataServer __RPC_FAR * This,
            /* [out] */ FILETIME __RPC_FAR *pftTime);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LogDownloadRecipe )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ LPCWSTR szFile,
            /* [in] */ FILETIME ftStartTime,
            /* [in] */ FILETIME ftEndTime,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ LPCWSTR __RPC_FAR *pszItem,
            /* [size_is][in] */ VARIANT __RPC_FAR *pvaValues);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadEventHistory )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ FILETIME ftStartTime,
            /* [in] */ FILETIME ftEndTime,
            /* [in] */ DWORD dwEventType,
            /* [in] */ DWORD dwLowSeverity,
            /* [in] */ DWORD dwHighSeverity,
            /* [in] */ DWORD dwNumAreas,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreaList,
            /* [in] */ DWORD dwNumSources,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszSourceList,
            /* [in] */ DWORD dwNumActors,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszActorList,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ CXEVENTSTRUCT __RPC_FAR *__RPC_FAR *ppEvents);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEventCount )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ BOOL bAcked,
            /* [in] */ DWORD dwLowSeverity,
            /* [in] */ DWORD dwHighSeverity,
            /* [in] */ DWORD dwNumAreas,
            /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreaList,
            /* [out] */ DWORD __RPC_FAR *pdwCount);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateUserManager )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRunPictureSetting )( 
            ICxDataServer __RPC_FAR * This,
            /* [out] */ CXRUNPICSETTING __RPC_FAR *pSetting);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsModified )( 
            ICxDataServer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetServerStatus )( 
            ICxDataServer __RPC_FAR * This,
            /* [out] */ CXSERVERSTATUS __RPC_FAR *__RPC_FAR *ppStatus);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDeviceStatus )( 
            ICxDataServer __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szDevice,
            /* [string][out] */ LPWSTR __RPC_FAR *pszType,
            /* [out] */ DWORD __RPC_FAR *pdwState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDeviceStatus )( 
            ICxDataServer __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szDevice,
            /* [in] */ DWORD dwState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLoginUsersInfo )( 
            ICxDataServer __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ CXLOGINUSERINFO __RPC_FAR *__RPC_FAR *ppInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *VerifySoftLock )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ BOOL bAllowDemoStatus,
            /* [out] */ DWORD __RPC_FAR *pdwStatus,
            /* [out][in] */ DWORD __RPC_FAR *pdwEncryptData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateObject )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ VARIANT vaClassValue,
            /* [out] */ VARIANT __RPC_FAR *pvaObject);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ObjectBatchCall )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pObject,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ LPCWSTR __RPC_FAR *ppszFunctions,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwFlags,
            /* [size_is][in] */ CXDISPPARAMS __RPC_FAR *pDispParams,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvarResultValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadTagPropertiesValue )( 
            ICxDataServer __RPC_FAR * This,
            /* [in] */ DWORD dwTagType,
            /* [string][in] */ LPCWSTR szTagFilter,
            /* [in] */ DWORD dwNumProps,
            /* [size_is][in] */ LPCWSTR __RPC_FAR *pszPropList,
            /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppVartypes,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppSizes,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvaValues);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveTagStatus )( 
            ICxDataServer __RPC_FAR * This);
        
        END_INTERFACE
    } ICxDataServerVtbl;

    interface ICxDataServer
    {
        CONST_VTBL struct ICxDataServerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxDataServer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxDataServer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxDataServer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxDataServer_UserLogin(This,szUserName,szPassword,szIP,bLocal,pdwCookie)	\
    (This)->lpVtbl -> UserLogin(This,szUserName,szPassword,szIP,bLocal,pdwCookie)

#define ICxDataServer_GetUserAccessRight(This,pSecurityAreas,pdwPrivilege)	\
    (This)->lpVtbl -> GetUserAccessRight(This,pSecurityAreas,pdwPrivilege)

#define ICxDataServer_UserLogout(This,pdwCookie)	\
    (This)->lpVtbl -> UserLogout(This,pdwCookie)

#define ICxDataServer_GetNetNodes(This,pdwCount,ppNodes)	\
    (This)->lpVtbl -> GetNetNodes(This,pdwCount,ppNodes)

#define ICxDataServer_CreateTagGroup(This,szName,hClientGroup,phServerGroup,riid,ppUnk)	\
    (This)->lpVtbl -> CreateTagGroup(This,szName,hClientGroup,phServerGroup,riid,ppUnk)

#define ICxDataServer_CreateTagBrowser(This,riid,ppUnk)	\
    (This)->lpVtbl -> CreateTagBrowser(This,riid,ppUnk)

#define ICxDataServer_IsTagAvailableProperty(This)	\
    (This)->lpVtbl -> IsTagAvailableProperty(This)

#define ICxDataServer_QueryTagAvailableProperties(This,szItemID,pdwCount,ppNames)	\
    (This)->lpVtbl -> QueryTagAvailableProperties(This,szItemID,pdwCount,ppNames)

#define ICxDataServer_CreateEventSubscription(This,hClientSubscription,riid,ppUnk)	\
    (This)->lpVtbl -> CreateEventSubscription(This,hClientSubscription,riid,ppUnk)

#define ICxDataServer_AckEventCondition(This,dwCount,pszSource,pszConditionName,pftActiveTime,pdwCookie,ppErrors)	\
    (This)->lpVtbl -> AckEventCondition(This,dwCount,pszSource,pszConditionName,pftActiveTime,pdwCookie,ppErrors)

#define ICxDataServer_LogUserOperation(This,szSource,szMessage)	\
    (This)->lpVtbl -> LogUserOperation(This,szSource,szMessage)

#define ICxDataServer_LogSOEEvent(This,ftTime,szSource,szMessage,dwSeverity)	\
    (This)->lpVtbl -> LogSOEEvent(This,ftTime,szSource,szMessage,dwSeverity)

#define ICxDataServer_BrowseSecurityAreas(This,ppIEnumString)	\
    (This)->lpVtbl -> BrowseSecurityAreas(This,ppIEnumString)

#define ICxDataServer_BrowseEventAreas(This,ppIEnumString)	\
    (This)->lpVtbl -> BrowseEventAreas(This,ppIEnumString)

#define ICxDataServer_BrowseDevices(This,ppIEnumString)	\
    (This)->lpVtbl -> BrowseDevices(This,ppIEnumString)

#define ICxDataServer_BrowseTagLogTabs(This,ppIEnumString)	\
    (This)->lpVtbl -> BrowseTagLogTabs(This,ppIEnumString)

#define ICxDataServer_BrowseTagsOfLogTab(This,szLogTab,ppIEnumString)	\
    (This)->lpVtbl -> BrowseTagsOfLogTab(This,szLogTab,ppIEnumString)

#define ICxDataServer_DownloadFile(This,szFile,pftFileTime,pdwCount,ppBuffer)	\
    (This)->lpVtbl -> DownloadFile(This,szFile,pftFileTime,pdwCount,ppBuffer)

#define ICxDataServer_GetActiveState(This)	\
    (This)->lpVtbl -> GetActiveState(This)

#define ICxDataServer_CreateSynchronization(This,riid,ppUnk)	\
    (This)->lpVtbl -> CreateSynchronization(This,riid,ppUnk)

#define ICxDataServer_SwitchMasterComputer(This)	\
    (This)->lpVtbl -> SwitchMasterComputer(This)

#define ICxDataServer_GetServerCurrentTime(This,pftTime)	\
    (This)->lpVtbl -> GetServerCurrentTime(This,pftTime)

#define ICxDataServer_LogDownloadRecipe(This,szFile,ftStartTime,ftEndTime,dwCount,pszItem,pvaValues)	\
    (This)->lpVtbl -> LogDownloadRecipe(This,szFile,ftStartTime,ftEndTime,dwCount,pszItem,pvaValues)

#define ICxDataServer_ReadEventHistory(This,ftStartTime,ftEndTime,dwEventType,dwLowSeverity,dwHighSeverity,dwNumAreas,pszAreaList,dwNumSources,pszSourceList,dwNumActors,pszActorList,pdwCount,ppEvents)	\
    (This)->lpVtbl -> ReadEventHistory(This,ftStartTime,ftEndTime,dwEventType,dwLowSeverity,dwHighSeverity,dwNumAreas,pszAreaList,dwNumSources,pszSourceList,dwNumActors,pszActorList,pdwCount,ppEvents)

#define ICxDataServer_GetEventCount(This,bAcked,dwLowSeverity,dwHighSeverity,dwNumAreas,pszAreaList,pdwCount)	\
    (This)->lpVtbl -> GetEventCount(This,bAcked,dwLowSeverity,dwHighSeverity,dwNumAreas,pszAreaList,pdwCount)

#define ICxDataServer_CreateUserManager(This,riid,ppUnk)	\
    (This)->lpVtbl -> CreateUserManager(This,riid,ppUnk)

#define ICxDataServer_GetRunPictureSetting(This,pSetting)	\
    (This)->lpVtbl -> GetRunPictureSetting(This,pSetting)

#define ICxDataServer_IsModified(This)	\
    (This)->lpVtbl -> IsModified(This)

#define ICxDataServer_GetServerStatus(This,ppStatus)	\
    (This)->lpVtbl -> GetServerStatus(This,ppStatus)

#define ICxDataServer_GetDeviceStatus(This,szDevice,pszType,pdwState)	\
    (This)->lpVtbl -> GetDeviceStatus(This,szDevice,pszType,pdwState)

#define ICxDataServer_SetDeviceStatus(This,szDevice,dwState)	\
    (This)->lpVtbl -> SetDeviceStatus(This,szDevice,dwState)

#define ICxDataServer_GetLoginUsersInfo(This,pdwCount,ppInfo)	\
    (This)->lpVtbl -> GetLoginUsersInfo(This,pdwCount,ppInfo)

#define ICxDataServer_VerifySoftLock(This,bAllowDemoStatus,pdwStatus,pdwEncryptData)	\
    (This)->lpVtbl -> VerifySoftLock(This,bAllowDemoStatus,pdwStatus,pdwEncryptData)

#define ICxDataServer_CreateObject(This,vaClassValue,pvaObject)	\
    (This)->lpVtbl -> CreateObject(This,vaClassValue,pvaObject)

#define ICxDataServer_ObjectBatchCall(This,pObject,dwCount,ppszFunctions,pdwFlags,pDispParams,ppvarResultValues,ppErrors)	\
    (This)->lpVtbl -> ObjectBatchCall(This,pObject,dwCount,ppszFunctions,pdwFlags,pDispParams,ppvarResultValues,ppErrors)

#define ICxDataServer_ReadTagPropertiesValue(This,dwTagType,szTagFilter,dwNumProps,pszPropList,ppVartypes,ppSizes,pdwCount,ppvaValues)	\
    (This)->lpVtbl -> ReadTagPropertiesValue(This,dwTagType,szTagFilter,dwNumProps,pszPropList,ppVartypes,ppSizes,pdwCount,ppvaValues)

#define ICxDataServer_SaveTagStatus(This)	\
    (This)->lpVtbl -> SaveTagStatus(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICxDataServer_UserLogin_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szUserName,
    /* [string][in] */ LPCWSTR szPassword,
    /* [string][in] */ LPCWSTR szIP,
    /* [in] */ BOOL bLocal,
    /* [out][in] */ DWORD __RPC_FAR *pdwCookie);


void __RPC_STUB ICxDataServer_UserLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_GetUserAccessRight_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [string][out] */ LPWSTR __RPC_FAR *pSecurityAreas,
    /* [out] */ DWORD __RPC_FAR *pdwPrivilege);


void __RPC_STUB ICxDataServer_GetUserAccessRight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_UserLogout_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [out][in] */ DWORD __RPC_FAR *pdwCookie);


void __RPC_STUB ICxDataServer_UserLogout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_GetNetNodes_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ CXNETNODE __RPC_FAR *__RPC_FAR *ppNodes);


void __RPC_STUB ICxDataServer_GetNetNodes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_CreateTagGroup_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szName,
    /* [in] */ CXHANDLE hClientGroup,
    /* [out] */ CXHANDLE __RPC_FAR *phServerGroup,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);


void __RPC_STUB ICxDataServer_CreateTagGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_CreateTagBrowser_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);


void __RPC_STUB ICxDataServer_CreateTagBrowser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_IsTagAvailableProperty_Proxy( 
    ICxDataServer __RPC_FAR * This);


void __RPC_STUB ICxDataServer_IsTagAvailableProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_QueryTagAvailableProperties_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ LPWSTR szItemID,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppNames);


void __RPC_STUB ICxDataServer_QueryTagAvailableProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_CreateEventSubscription_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ CXHANDLE hClientSubscription,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);


void __RPC_STUB ICxDataServer_CreateEventSubscription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_AckEventCondition_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ LPCWSTR __RPC_FAR *pszSource,
    /* [size_is][in] */ LPCWSTR __RPC_FAR *pszConditionName,
    /* [size_is][in] */ FILETIME __RPC_FAR *pftActiveTime,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwCookie,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB ICxDataServer_AckEventCondition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_LogUserOperation_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szSource,
    /* [string][in] */ LPCWSTR szMessage);


void __RPC_STUB ICxDataServer_LogUserOperation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_LogSOEEvent_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ FILETIME ftTime,
    /* [string][in] */ LPCWSTR szSource,
    /* [string][in] */ LPCWSTR szMessage,
    /* [in] */ DWORD dwSeverity);


void __RPC_STUB ICxDataServer_LogSOEEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_BrowseSecurityAreas_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);


void __RPC_STUB ICxDataServer_BrowseSecurityAreas_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_BrowseEventAreas_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);


void __RPC_STUB ICxDataServer_BrowseEventAreas_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_BrowseDevices_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);


void __RPC_STUB ICxDataServer_BrowseDevices_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_BrowseTagLogTabs_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);


void __RPC_STUB ICxDataServer_BrowseTagLogTabs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_BrowseTagsOfLogTab_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ LPCWSTR szLogTab,
    /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString);


void __RPC_STUB ICxDataServer_BrowseTagsOfLogTab_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_DownloadFile_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ LPCWSTR szFile,
    /* [out][in] */ FILETIME __RPC_FAR *pftFileTime,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ BYTE __RPC_FAR *__RPC_FAR *ppBuffer);


void __RPC_STUB ICxDataServer_DownloadFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_GetActiveState_Proxy( 
    ICxDataServer __RPC_FAR * This);


void __RPC_STUB ICxDataServer_GetActiveState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_CreateSynchronization_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);


void __RPC_STUB ICxDataServer_CreateSynchronization_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_SwitchMasterComputer_Proxy( 
    ICxDataServer __RPC_FAR * This);


void __RPC_STUB ICxDataServer_SwitchMasterComputer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_GetServerCurrentTime_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [out] */ FILETIME __RPC_FAR *pftTime);


void __RPC_STUB ICxDataServer_GetServerCurrentTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_LogDownloadRecipe_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ LPCWSTR szFile,
    /* [in] */ FILETIME ftStartTime,
    /* [in] */ FILETIME ftEndTime,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ LPCWSTR __RPC_FAR *pszItem,
    /* [size_is][in] */ VARIANT __RPC_FAR *pvaValues);


void __RPC_STUB ICxDataServer_LogDownloadRecipe_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_ReadEventHistory_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ FILETIME ftStartTime,
    /* [in] */ FILETIME ftEndTime,
    /* [in] */ DWORD dwEventType,
    /* [in] */ DWORD dwLowSeverity,
    /* [in] */ DWORD dwHighSeverity,
    /* [in] */ DWORD dwNumAreas,
    /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreaList,
    /* [in] */ DWORD dwNumSources,
    /* [size_is][in] */ LPWSTR __RPC_FAR *pszSourceList,
    /* [in] */ DWORD dwNumActors,
    /* [size_is][in] */ LPWSTR __RPC_FAR *pszActorList,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ CXEVENTSTRUCT __RPC_FAR *__RPC_FAR *ppEvents);


void __RPC_STUB ICxDataServer_ReadEventHistory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_GetEventCount_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ BOOL bAcked,
    /* [in] */ DWORD dwLowSeverity,
    /* [in] */ DWORD dwHighSeverity,
    /* [in] */ DWORD dwNumAreas,
    /* [size_is][in] */ LPWSTR __RPC_FAR *pszAreaList,
    /* [out] */ DWORD __RPC_FAR *pdwCount);


void __RPC_STUB ICxDataServer_GetEventCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_CreateUserManager_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);


void __RPC_STUB ICxDataServer_CreateUserManager_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_GetRunPictureSetting_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [out] */ CXRUNPICSETTING __RPC_FAR *pSetting);


void __RPC_STUB ICxDataServer_GetRunPictureSetting_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_IsModified_Proxy( 
    ICxDataServer __RPC_FAR * This);


void __RPC_STUB ICxDataServer_IsModified_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_GetServerStatus_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [out] */ CXSERVERSTATUS __RPC_FAR *__RPC_FAR *ppStatus);


void __RPC_STUB ICxDataServer_GetServerStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_GetDeviceStatus_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szDevice,
    /* [string][out] */ LPWSTR __RPC_FAR *pszType,
    /* [out] */ DWORD __RPC_FAR *pdwState);


void __RPC_STUB ICxDataServer_GetDeviceStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_SetDeviceStatus_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szDevice,
    /* [in] */ DWORD dwState);


void __RPC_STUB ICxDataServer_SetDeviceStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_GetLoginUsersInfo_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ CXLOGINUSERINFO __RPC_FAR *__RPC_FAR *ppInfo);


void __RPC_STUB ICxDataServer_GetLoginUsersInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_VerifySoftLock_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ BOOL bAllowDemoStatus,
    /* [out] */ DWORD __RPC_FAR *pdwStatus,
    /* [out][in] */ DWORD __RPC_FAR *pdwEncryptData);


void __RPC_STUB ICxDataServer_VerifySoftLock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_CreateObject_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ VARIANT vaClassValue,
    /* [out] */ VARIANT __RPC_FAR *pvaObject);


void __RPC_STUB ICxDataServer_CreateObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_ObjectBatchCall_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pObject,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ LPCWSTR __RPC_FAR *ppszFunctions,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwFlags,
    /* [size_is][in] */ CXDISPPARAMS __RPC_FAR *pDispParams,
    /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvarResultValues,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB ICxDataServer_ObjectBatchCall_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_ReadTagPropertiesValue_Proxy( 
    ICxDataServer __RPC_FAR * This,
    /* [in] */ DWORD dwTagType,
    /* [string][in] */ LPCWSTR szTagFilter,
    /* [in] */ DWORD dwNumProps,
    /* [size_is][in] */ LPCWSTR __RPC_FAR *pszPropList,
    /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppVartypes,
    /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppSizes,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvaValues);


void __RPC_STUB ICxDataServer_ReadTagPropertiesValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataServer_SaveTagStatus_Proxy( 
    ICxDataServer __RPC_FAR * This);


void __RPC_STUB ICxDataServer_SaveTagStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxDataServer_INTERFACE_DEFINED__ */


#ifndef __ICxUserManager_INTERFACE_DEFINED__
#define __ICxUserManager_INTERFACE_DEFINED__

/* interface ICxUserManager */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_ICxUserManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("395EB050-7AF2-4af8-9B30-8835A911769D")
    ICxUserManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE BrowseUsers( 
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumDesc) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUserInfo( 
            /* [string][in] */ LPCWSTR szName,
            /* [string][out] */ LPWSTR __RPC_FAR *pszDesc,
            /* [string][out] */ LPWSTR __RPC_FAR *pszPassword,
            /* [string][out] */ LPWSTR __RPC_FAR *pszGroup,
            /* [out] */ DWORD __RPC_FAR *pdwPrivilege,
            /* [string][out] */ LPWSTR __RPC_FAR *pszSecurityAreas) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetUserInfo( 
            /* [string][in] */ LPCWSTR szName,
            /* [string][in] */ LPCWSTR szNewName,
            /* [string][in] */ LPCWSTR szDesc,
            /* [string][in] */ LPCWSTR szPassword,
            /* [string][in] */ LPCWSTR szGroup,
            /* [in] */ DWORD dwPrivilege,
            /* [string][in] */ LPCWSTR szSecurityAreas) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SaveUsersInfo( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxUserManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxUserManager __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxUserManager __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxUserManager __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BrowseUsers )( 
            ICxUserManager __RPC_FAR * This,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumDesc);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUserInfo )( 
            ICxUserManager __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szName,
            /* [string][out] */ LPWSTR __RPC_FAR *pszDesc,
            /* [string][out] */ LPWSTR __RPC_FAR *pszPassword,
            /* [string][out] */ LPWSTR __RPC_FAR *pszGroup,
            /* [out] */ DWORD __RPC_FAR *pdwPrivilege,
            /* [string][out] */ LPWSTR __RPC_FAR *pszSecurityAreas);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetUserInfo )( 
            ICxUserManager __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szName,
            /* [string][in] */ LPCWSTR szNewName,
            /* [string][in] */ LPCWSTR szDesc,
            /* [string][in] */ LPCWSTR szPassword,
            /* [string][in] */ LPCWSTR szGroup,
            /* [in] */ DWORD dwPrivilege,
            /* [string][in] */ LPCWSTR szSecurityAreas);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveUsersInfo )( 
            ICxUserManager __RPC_FAR * This);
        
        END_INTERFACE
    } ICxUserManagerVtbl;

    interface ICxUserManager
    {
        CONST_VTBL struct ICxUserManagerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxUserManager_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxUserManager_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxUserManager_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxUserManager_BrowseUsers(This,ppIEnumString,ppIEnumDesc)	\
    (This)->lpVtbl -> BrowseUsers(This,ppIEnumString,ppIEnumDesc)

#define ICxUserManager_GetUserInfo(This,szName,pszDesc,pszPassword,pszGroup,pdwPrivilege,pszSecurityAreas)	\
    (This)->lpVtbl -> GetUserInfo(This,szName,pszDesc,pszPassword,pszGroup,pdwPrivilege,pszSecurityAreas)

#define ICxUserManager_SetUserInfo(This,szName,szNewName,szDesc,szPassword,szGroup,dwPrivilege,szSecurityAreas)	\
    (This)->lpVtbl -> SetUserInfo(This,szName,szNewName,szDesc,szPassword,szGroup,dwPrivilege,szSecurityAreas)

#define ICxUserManager_SaveUsersInfo(This)	\
    (This)->lpVtbl -> SaveUsersInfo(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICxUserManager_BrowseUsers_Proxy( 
    ICxUserManager __RPC_FAR * This,
    /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString,
    /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumDesc);


void __RPC_STUB ICxUserManager_BrowseUsers_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxUserManager_GetUserInfo_Proxy( 
    ICxUserManager __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szName,
    /* [string][out] */ LPWSTR __RPC_FAR *pszDesc,
    /* [string][out] */ LPWSTR __RPC_FAR *pszPassword,
    /* [string][out] */ LPWSTR __RPC_FAR *pszGroup,
    /* [out] */ DWORD __RPC_FAR *pdwPrivilege,
    /* [string][out] */ LPWSTR __RPC_FAR *pszSecurityAreas);


void __RPC_STUB ICxUserManager_GetUserInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxUserManager_SetUserInfo_Proxy( 
    ICxUserManager __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szName,
    /* [string][in] */ LPCWSTR szNewName,
    /* [string][in] */ LPCWSTR szDesc,
    /* [string][in] */ LPCWSTR szPassword,
    /* [string][in] */ LPCWSTR szGroup,
    /* [in] */ DWORD dwPrivilege,
    /* [string][in] */ LPCWSTR szSecurityAreas);


void __RPC_STUB ICxUserManager_SetUserInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxUserManager_SaveUsersInfo_Proxy( 
    ICxUserManager __RPC_FAR * This);


void __RPC_STUB ICxUserManager_SaveUsersInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxUserManager_INTERFACE_DEFINED__ */


#ifndef __ICxBrowseServerAddressSpace_INTERFACE_DEFINED__
#define __ICxBrowseServerAddressSpace_INTERFACE_DEFINED__

/* interface ICxBrowseServerAddressSpace */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_ICxBrowseServerAddressSpace;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("03D28780-A2B3-4232-8417-0A6435F3A7AB")
    ICxBrowseServerAddressSpace : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ChangeBrowsePosition( 
            /* [in] */ CXBROWSEDIRECTION dwBrowseDirection,
            /* [string][in] */ LPCWSTR szString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BrowseItemIDs( 
            /* [in] */ CXBROWSETYPE dwBrowseFilterType,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString,
            /* [out] */ LPENUMVARIANT __RPC_FAR *ppIEnumAttributes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetItemID( 
            /* [in] */ LPWSTR szItemDataID,
            /* [string][out] */ LPWSTR __RPC_FAR *szItemID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxBrowseServerAddressSpaceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxBrowseServerAddressSpace __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxBrowseServerAddressSpace __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxBrowseServerAddressSpace __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ChangeBrowsePosition )( 
            ICxBrowseServerAddressSpace __RPC_FAR * This,
            /* [in] */ CXBROWSEDIRECTION dwBrowseDirection,
            /* [string][in] */ LPCWSTR szString);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BrowseItemIDs )( 
            ICxBrowseServerAddressSpace __RPC_FAR * This,
            /* [in] */ CXBROWSETYPE dwBrowseFilterType,
            /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString,
            /* [out] */ LPENUMVARIANT __RPC_FAR *ppIEnumAttributes);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItemID )( 
            ICxBrowseServerAddressSpace __RPC_FAR * This,
            /* [in] */ LPWSTR szItemDataID,
            /* [string][out] */ LPWSTR __RPC_FAR *szItemID);
        
        END_INTERFACE
    } ICxBrowseServerAddressSpaceVtbl;

    interface ICxBrowseServerAddressSpace
    {
        CONST_VTBL struct ICxBrowseServerAddressSpaceVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxBrowseServerAddressSpace_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxBrowseServerAddressSpace_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxBrowseServerAddressSpace_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxBrowseServerAddressSpace_ChangeBrowsePosition(This,dwBrowseDirection,szString)	\
    (This)->lpVtbl -> ChangeBrowsePosition(This,dwBrowseDirection,szString)

#define ICxBrowseServerAddressSpace_BrowseItemIDs(This,dwBrowseFilterType,ppIEnumString,ppIEnumAttributes)	\
    (This)->lpVtbl -> BrowseItemIDs(This,dwBrowseFilterType,ppIEnumString,ppIEnumAttributes)

#define ICxBrowseServerAddressSpace_GetItemID(This,szItemDataID,szItemID)	\
    (This)->lpVtbl -> GetItemID(This,szItemDataID,szItemID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICxBrowseServerAddressSpace_ChangeBrowsePosition_Proxy( 
    ICxBrowseServerAddressSpace __RPC_FAR * This,
    /* [in] */ CXBROWSEDIRECTION dwBrowseDirection,
    /* [string][in] */ LPCWSTR szString);


void __RPC_STUB ICxBrowseServerAddressSpace_ChangeBrowsePosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxBrowseServerAddressSpace_BrowseItemIDs_Proxy( 
    ICxBrowseServerAddressSpace __RPC_FAR * This,
    /* [in] */ CXBROWSETYPE dwBrowseFilterType,
    /* [out] */ LPENUMSTRING __RPC_FAR *ppIEnumString,
    /* [out] */ LPENUMVARIANT __RPC_FAR *ppIEnumAttributes);


void __RPC_STUB ICxBrowseServerAddressSpace_BrowseItemIDs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxBrowseServerAddressSpace_GetItemID_Proxy( 
    ICxBrowseServerAddressSpace __RPC_FAR * This,
    /* [in] */ LPWSTR szItemDataID,
    /* [string][out] */ LPWSTR __RPC_FAR *szItemID);


void __RPC_STUB ICxBrowseServerAddressSpace_GetItemID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxBrowseServerAddressSpace_INTERFACE_DEFINED__ */


#ifndef __ICxTagGroup_INTERFACE_DEFINED__
#define __ICxTagGroup_INTERFACE_DEFINED__

/* interface ICxTagGroup */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_ICxTagGroup;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8B7A9876-593F-41f8-843E-BF539B08C4AB")
    ICxTagGroup : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AddRemoveItems( 
            /* [in] */ BOOL bRemoveAll,
            /* [in] */ DWORD dwAddCount,
            /* [size_is][in] */ CXITEMDEF __RPC_FAR *pAddItemArray,
            /* [size_is][size_is][out] */ CXHANDLE __RPC_FAR *__RPC_FAR *pphAddServerItems,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppAddErrors,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvaReadValues,
            /* [size_is][size_is][out] */ FILETIME __RPC_FAR *__RPC_FAR *ppftReadTimeStamps,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppReadErrors,
            /* [in] */ DWORD dwRemoveCount,
            /* [size_is][in] */ CXHANDLE __RPC_FAR *phRemoveServerItems,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppRemoveErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddItems( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ CXITEMDEF __RPC_FAR *pItemArray,
            /* [size_is][size_is][out] */ CXHANDLE __RPC_FAR *__RPC_FAR *pphServerItems,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ValidateItems( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ CXITEMDEF __RPC_FAR *pItemArray,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveItems( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ CXHANDLE __RPC_FAR *phServerItems,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadItems( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ CXHANDLE __RPC_FAR *phServerItems,
            /* [size_is][size_is][out] */ CXHANDLE __RPC_FAR *__RPC_FAR *pphClientItems,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvaValues,
            /* [size_is][size_is][out] */ FILETIME __RPC_FAR *__RPC_FAR *ppftTimeStamps,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteItems( 
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ CXHANDLE __RPC_FAR *phServerItems,
            /* [size_is][out][in] */ VARIANT __RPC_FAR *pvaValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetItemRangeByName( 
            /* [string][in] */ LPCWSTR szItemID,
            /* [string][in] */ LPCWSTR szProp,
            /* [out] */ VARIANT __RPC_FAR *pvaMin,
            /* [out] */ VARIANT __RPC_FAR *pvaMax) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetItemPropertiesByName( 
            /* [in] */ LPCWSTR szItemID,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwPropertyIDs,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvData,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadItemByName( 
            /* [string][in] */ LPCWSTR szItemID,
            /* [string][in] */ LPCWSTR szProp,
            /* [out] */ VARIANT __RPC_FAR *pvaValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteItemByName( 
            /* [string][in] */ LPCWSTR szItemID,
            /* [string][in] */ LPCWSTR szProp,
            /* [out][in] */ VARIANT __RPC_FAR *pvaValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Refresh( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEnable( 
            /* [in] */ BOOL bEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadItemsHistory( 
            /* [out][in] */ FILETIME __RPC_FAR *pftStartTime,
            /* [out][in] */ FILETIME __RPC_FAR *pftEndTime,
            /* [in] */ DWORD dwNumValues,
            /* [in] */ BOOL bBounds,
            /* [in] */ LPCWSTR szLogTab,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ CXHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ CXHDAITEM __RPC_FAR *__RPC_FAR *ppItemValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryItemsValueType( 
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ CXHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppVartypes,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppSizes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddAllItems( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxTagGroupVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxTagGroup __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxTagGroup __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxTagGroup __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddRemoveItems )( 
            ICxTagGroup __RPC_FAR * This,
            /* [in] */ BOOL bRemoveAll,
            /* [in] */ DWORD dwAddCount,
            /* [size_is][in] */ CXITEMDEF __RPC_FAR *pAddItemArray,
            /* [size_is][size_is][out] */ CXHANDLE __RPC_FAR *__RPC_FAR *pphAddServerItems,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppAddErrors,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvaReadValues,
            /* [size_is][size_is][out] */ FILETIME __RPC_FAR *__RPC_FAR *ppftReadTimeStamps,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppReadErrors,
            /* [in] */ DWORD dwRemoveCount,
            /* [size_is][in] */ CXHANDLE __RPC_FAR *phRemoveServerItems,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppRemoveErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddItems )( 
            ICxTagGroup __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ CXITEMDEF __RPC_FAR *pItemArray,
            /* [size_is][size_is][out] */ CXHANDLE __RPC_FAR *__RPC_FAR *pphServerItems,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateItems )( 
            ICxTagGroup __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ CXITEMDEF __RPC_FAR *pItemArray,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveItems )( 
            ICxTagGroup __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ CXHANDLE __RPC_FAR *phServerItems,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadItems )( 
            ICxTagGroup __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ CXHANDLE __RPC_FAR *phServerItems,
            /* [size_is][size_is][out] */ CXHANDLE __RPC_FAR *__RPC_FAR *pphClientItems,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvaValues,
            /* [size_is][size_is][out] */ FILETIME __RPC_FAR *__RPC_FAR *ppftTimeStamps,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteItems )( 
            ICxTagGroup __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ CXHANDLE __RPC_FAR *phServerItems,
            /* [size_is][out][in] */ VARIANT __RPC_FAR *pvaValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItemRangeByName )( 
            ICxTagGroup __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szItemID,
            /* [string][in] */ LPCWSTR szProp,
            /* [out] */ VARIANT __RPC_FAR *pvaMin,
            /* [out] */ VARIANT __RPC_FAR *pvaMax);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItemPropertiesByName )( 
            ICxTagGroup __RPC_FAR * This,
            /* [in] */ LPCWSTR szItemID,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwPropertyIDs,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvData,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadItemByName )( 
            ICxTagGroup __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szItemID,
            /* [string][in] */ LPCWSTR szProp,
            /* [out] */ VARIANT __RPC_FAR *pvaValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteItemByName )( 
            ICxTagGroup __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szItemID,
            /* [string][in] */ LPCWSTR szProp,
            /* [out][in] */ VARIANT __RPC_FAR *pvaValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh )( 
            ICxTagGroup __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEnable )( 
            ICxTagGroup __RPC_FAR * This,
            /* [in] */ BOOL bEnable);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadItemsHistory )( 
            ICxTagGroup __RPC_FAR * This,
            /* [out][in] */ FILETIME __RPC_FAR *pftStartTime,
            /* [out][in] */ FILETIME __RPC_FAR *pftEndTime,
            /* [in] */ DWORD dwNumValues,
            /* [in] */ BOOL bBounds,
            /* [in] */ LPCWSTR szLogTab,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ CXHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ CXHDAITEM __RPC_FAR *__RPC_FAR *ppItemValues,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryItemsValueType )( 
            ICxTagGroup __RPC_FAR * This,
            /* [in] */ DWORD dwNumItems,
            /* [size_is][in] */ CXHANDLE __RPC_FAR *phServer,
            /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppVartypes,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppSizes);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddAllItems )( 
            ICxTagGroup __RPC_FAR * This);
        
        END_INTERFACE
    } ICxTagGroupVtbl;

    interface ICxTagGroup
    {
        CONST_VTBL struct ICxTagGroupVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxTagGroup_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxTagGroup_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxTagGroup_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxTagGroup_AddRemoveItems(This,bRemoveAll,dwAddCount,pAddItemArray,pphAddServerItems,ppAddErrors,ppvaReadValues,ppftReadTimeStamps,ppReadErrors,dwRemoveCount,phRemoveServerItems,ppRemoveErrors)	\
    (This)->lpVtbl -> AddRemoveItems(This,bRemoveAll,dwAddCount,pAddItemArray,pphAddServerItems,ppAddErrors,ppvaReadValues,ppftReadTimeStamps,ppReadErrors,dwRemoveCount,phRemoveServerItems,ppRemoveErrors)

#define ICxTagGroup_AddItems(This,dwCount,pItemArray,pphServerItems,ppErrors)	\
    (This)->lpVtbl -> AddItems(This,dwCount,pItemArray,pphServerItems,ppErrors)

#define ICxTagGroup_ValidateItems(This,dwCount,pItemArray,ppErrors)	\
    (This)->lpVtbl -> ValidateItems(This,dwCount,pItemArray,ppErrors)

#define ICxTagGroup_RemoveItems(This,dwCount,phServerItems,ppErrors)	\
    (This)->lpVtbl -> RemoveItems(This,dwCount,phServerItems,ppErrors)

#define ICxTagGroup_ReadItems(This,dwCount,phServerItems,pphClientItems,ppvaValues,ppftTimeStamps,ppErrors)	\
    (This)->lpVtbl -> ReadItems(This,dwCount,phServerItems,pphClientItems,ppvaValues,ppftTimeStamps,ppErrors)

#define ICxTagGroup_WriteItems(This,dwCount,phServerItems,pvaValues,ppErrors)	\
    (This)->lpVtbl -> WriteItems(This,dwCount,phServerItems,pvaValues,ppErrors)

#define ICxTagGroup_GetItemRangeByName(This,szItemID,szProp,pvaMin,pvaMax)	\
    (This)->lpVtbl -> GetItemRangeByName(This,szItemID,szProp,pvaMin,pvaMax)

#define ICxTagGroup_GetItemPropertiesByName(This,szItemID,dwCount,pdwPropertyIDs,ppvData,ppErrors)	\
    (This)->lpVtbl -> GetItemPropertiesByName(This,szItemID,dwCount,pdwPropertyIDs,ppvData,ppErrors)

#define ICxTagGroup_ReadItemByName(This,szItemID,szProp,pvaValue)	\
    (This)->lpVtbl -> ReadItemByName(This,szItemID,szProp,pvaValue)

#define ICxTagGroup_WriteItemByName(This,szItemID,szProp,pvaValue)	\
    (This)->lpVtbl -> WriteItemByName(This,szItemID,szProp,pvaValue)

#define ICxTagGroup_Refresh(This)	\
    (This)->lpVtbl -> Refresh(This)

#define ICxTagGroup_SetEnable(This,bEnable)	\
    (This)->lpVtbl -> SetEnable(This,bEnable)

#define ICxTagGroup_ReadItemsHistory(This,pftStartTime,pftEndTime,dwNumValues,bBounds,szLogTab,dwNumItems,phServer,ppItemValues,ppErrors)	\
    (This)->lpVtbl -> ReadItemsHistory(This,pftStartTime,pftEndTime,dwNumValues,bBounds,szLogTab,dwNumItems,phServer,ppItemValues,ppErrors)

#define ICxTagGroup_QueryItemsValueType(This,dwNumItems,phServer,ppVartypes,ppSizes)	\
    (This)->lpVtbl -> QueryItemsValueType(This,dwNumItems,phServer,ppVartypes,ppSizes)

#define ICxTagGroup_AddAllItems(This)	\
    (This)->lpVtbl -> AddAllItems(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICxTagGroup_AddRemoveItems_Proxy( 
    ICxTagGroup __RPC_FAR * This,
    /* [in] */ BOOL bRemoveAll,
    /* [in] */ DWORD dwAddCount,
    /* [size_is][in] */ CXITEMDEF __RPC_FAR *pAddItemArray,
    /* [size_is][size_is][out] */ CXHANDLE __RPC_FAR *__RPC_FAR *pphAddServerItems,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppAddErrors,
    /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvaReadValues,
    /* [size_is][size_is][out] */ FILETIME __RPC_FAR *__RPC_FAR *ppftReadTimeStamps,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppReadErrors,
    /* [in] */ DWORD dwRemoveCount,
    /* [size_is][in] */ CXHANDLE __RPC_FAR *phRemoveServerItems,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppRemoveErrors);


void __RPC_STUB ICxTagGroup_AddRemoveItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxTagGroup_AddItems_Proxy( 
    ICxTagGroup __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ CXITEMDEF __RPC_FAR *pItemArray,
    /* [size_is][size_is][out] */ CXHANDLE __RPC_FAR *__RPC_FAR *pphServerItems,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB ICxTagGroup_AddItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxTagGroup_ValidateItems_Proxy( 
    ICxTagGroup __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ CXITEMDEF __RPC_FAR *pItemArray,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB ICxTagGroup_ValidateItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxTagGroup_RemoveItems_Proxy( 
    ICxTagGroup __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ CXHANDLE __RPC_FAR *phServerItems,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB ICxTagGroup_RemoveItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxTagGroup_ReadItems_Proxy( 
    ICxTagGroup __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ CXHANDLE __RPC_FAR *phServerItems,
    /* [size_is][size_is][out] */ CXHANDLE __RPC_FAR *__RPC_FAR *pphClientItems,
    /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvaValues,
    /* [size_is][size_is][out] */ FILETIME __RPC_FAR *__RPC_FAR *ppftTimeStamps,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB ICxTagGroup_ReadItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxTagGroup_WriteItems_Proxy( 
    ICxTagGroup __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ CXHANDLE __RPC_FAR *phServerItems,
    /* [size_is][out][in] */ VARIANT __RPC_FAR *pvaValues,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB ICxTagGroup_WriteItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxTagGroup_GetItemRangeByName_Proxy( 
    ICxTagGroup __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szItemID,
    /* [string][in] */ LPCWSTR szProp,
    /* [out] */ VARIANT __RPC_FAR *pvaMin,
    /* [out] */ VARIANT __RPC_FAR *pvaMax);


void __RPC_STUB ICxTagGroup_GetItemRangeByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxTagGroup_GetItemPropertiesByName_Proxy( 
    ICxTagGroup __RPC_FAR * This,
    /* [in] */ LPCWSTR szItemID,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwPropertyIDs,
    /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvData,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB ICxTagGroup_GetItemPropertiesByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxTagGroup_ReadItemByName_Proxy( 
    ICxTagGroup __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szItemID,
    /* [string][in] */ LPCWSTR szProp,
    /* [out] */ VARIANT __RPC_FAR *pvaValue);


void __RPC_STUB ICxTagGroup_ReadItemByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxTagGroup_WriteItemByName_Proxy( 
    ICxTagGroup __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szItemID,
    /* [string][in] */ LPCWSTR szProp,
    /* [out][in] */ VARIANT __RPC_FAR *pvaValue);


void __RPC_STUB ICxTagGroup_WriteItemByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxTagGroup_Refresh_Proxy( 
    ICxTagGroup __RPC_FAR * This);


void __RPC_STUB ICxTagGroup_Refresh_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxTagGroup_SetEnable_Proxy( 
    ICxTagGroup __RPC_FAR * This,
    /* [in] */ BOOL bEnable);


void __RPC_STUB ICxTagGroup_SetEnable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxTagGroup_ReadItemsHistory_Proxy( 
    ICxTagGroup __RPC_FAR * This,
    /* [out][in] */ FILETIME __RPC_FAR *pftStartTime,
    /* [out][in] */ FILETIME __RPC_FAR *pftEndTime,
    /* [in] */ DWORD dwNumValues,
    /* [in] */ BOOL bBounds,
    /* [in] */ LPCWSTR szLogTab,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ CXHANDLE __RPC_FAR *phServer,
    /* [size_is][size_is][out] */ CXHDAITEM __RPC_FAR *__RPC_FAR *ppItemValues,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB ICxTagGroup_ReadItemsHistory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxTagGroup_QueryItemsValueType_Proxy( 
    ICxTagGroup __RPC_FAR * This,
    /* [in] */ DWORD dwNumItems,
    /* [size_is][in] */ CXHANDLE __RPC_FAR *phServer,
    /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppVartypes,
    /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppSizes);


void __RPC_STUB ICxTagGroup_QueryItemsValueType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxTagGroup_AddAllItems_Proxy( 
    ICxTagGroup __RPC_FAR * This);


void __RPC_STUB ICxTagGroup_AddAllItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxTagGroup_INTERFACE_DEFINED__ */


#ifndef __ICxDataClientCallback_INTERFACE_DEFINED__
#define __ICxDataClientCallback_INTERFACE_DEFINED__

/* interface ICxDataClientCallback */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_ICxDataClientCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("03131DD2-23D7-4296-A76E-085535090119")
    ICxDataClientCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnDataChange( 
            /* [in] */ CXHANDLE hClientGroup,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ CXHANDLE __RPC_FAR *phClientItems,
            /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
            /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
            /* [size_is][in] */ HRESULT __RPC_FAR *pErrors) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxDataClientCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxDataClientCallback __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxDataClientCallback __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxDataClientCallback __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnDataChange )( 
            ICxDataClientCallback __RPC_FAR * This,
            /* [in] */ CXHANDLE hClientGroup,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ CXHANDLE __RPC_FAR *phClientItems,
            /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
            /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
            /* [size_is][in] */ HRESULT __RPC_FAR *pErrors);
        
        END_INTERFACE
    } ICxDataClientCallbackVtbl;

    interface ICxDataClientCallback
    {
        CONST_VTBL struct ICxDataClientCallbackVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxDataClientCallback_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxDataClientCallback_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxDataClientCallback_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxDataClientCallback_OnDataChange(This,hClientGroup,dwCount,phClientItems,pvValues,pftTimeStamps,pErrors)	\
    (This)->lpVtbl -> OnDataChange(This,hClientGroup,dwCount,phClientItems,pvValues,pftTimeStamps,pErrors)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICxDataClientCallback_OnDataChange_Proxy( 
    ICxDataClientCallback __RPC_FAR * This,
    /* [in] */ CXHANDLE hClientGroup,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ CXHANDLE __RPC_FAR *phClientItems,
    /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
    /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
    /* [size_is][in] */ HRESULT __RPC_FAR *pErrors);


void __RPC_STUB ICxDataClientCallback_OnDataChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxDataClientCallback_INTERFACE_DEFINED__ */


#ifndef __ICxEventSubscription_INTERFACE_DEFINED__
#define __ICxEventSubscription_INTERFACE_DEFINED__

/* interface ICxEventSubscription */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_ICxEventSubscription;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("32EC5CDE-4995-4ba4-B7E1-87D8864B71D8")
    ICxEventSubscription : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Refresh( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelRefresh( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectReturnedAttributes( 
            /* [in] */ DWORD dwCount,
            /* [in] */ DWORD __RPC_FAR *dwAttributeIDs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadPoolItems( 
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ CXEVENTSTRUCT __RPC_FAR *__RPC_FAR *ppEvents) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEnable( 
            /* [in] */ BOOL bEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetFilter( 
            /* [in] */ DWORD dwEventType,
            /* [in] */ DWORD dwNumCategories,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwEventCategories,
            /* [in] */ DWORD dwLowSeverity,
            /* [in] */ DWORD dwHighSeverity,
            /* [in] */ DWORD dwNumAreas,
            /* [size_is][in] */ LPCWSTR __RPC_FAR *pszAreaList,
            /* [in] */ DWORD dwNumSources,
            /* [size_is][in] */ LPCWSTR __RPC_FAR *pszSourceList) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxEventSubscriptionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxEventSubscription __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxEventSubscription __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxEventSubscription __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh )( 
            ICxEventSubscription __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CancelRefresh )( 
            ICxEventSubscription __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectReturnedAttributes )( 
            ICxEventSubscription __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [in] */ DWORD __RPC_FAR *dwAttributeIDs);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadPoolItems )( 
            ICxEventSubscription __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ CXEVENTSTRUCT __RPC_FAR *__RPC_FAR *ppEvents);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEnable )( 
            ICxEventSubscription __RPC_FAR * This,
            /* [in] */ BOOL bEnable);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFilter )( 
            ICxEventSubscription __RPC_FAR * This,
            /* [in] */ DWORD dwEventType,
            /* [in] */ DWORD dwNumCategories,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwEventCategories,
            /* [in] */ DWORD dwLowSeverity,
            /* [in] */ DWORD dwHighSeverity,
            /* [in] */ DWORD dwNumAreas,
            /* [size_is][in] */ LPCWSTR __RPC_FAR *pszAreaList,
            /* [in] */ DWORD dwNumSources,
            /* [size_is][in] */ LPCWSTR __RPC_FAR *pszSourceList);
        
        END_INTERFACE
    } ICxEventSubscriptionVtbl;

    interface ICxEventSubscription
    {
        CONST_VTBL struct ICxEventSubscriptionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxEventSubscription_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxEventSubscription_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxEventSubscription_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxEventSubscription_Refresh(This)	\
    (This)->lpVtbl -> Refresh(This)

#define ICxEventSubscription_CancelRefresh(This)	\
    (This)->lpVtbl -> CancelRefresh(This)

#define ICxEventSubscription_SelectReturnedAttributes(This,dwCount,dwAttributeIDs)	\
    (This)->lpVtbl -> SelectReturnedAttributes(This,dwCount,dwAttributeIDs)

#define ICxEventSubscription_ReadPoolItems(This,pdwCount,ppEvents)	\
    (This)->lpVtbl -> ReadPoolItems(This,pdwCount,ppEvents)

#define ICxEventSubscription_SetEnable(This,bEnable)	\
    (This)->lpVtbl -> SetEnable(This,bEnable)

#define ICxEventSubscription_SetFilter(This,dwEventType,dwNumCategories,pdwEventCategories,dwLowSeverity,dwHighSeverity,dwNumAreas,pszAreaList,dwNumSources,pszSourceList)	\
    (This)->lpVtbl -> SetFilter(This,dwEventType,dwNumCategories,pdwEventCategories,dwLowSeverity,dwHighSeverity,dwNumAreas,pszAreaList,dwNumSources,pszSourceList)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICxEventSubscription_Refresh_Proxy( 
    ICxEventSubscription __RPC_FAR * This);


void __RPC_STUB ICxEventSubscription_Refresh_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxEventSubscription_CancelRefresh_Proxy( 
    ICxEventSubscription __RPC_FAR * This);


void __RPC_STUB ICxEventSubscription_CancelRefresh_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxEventSubscription_SelectReturnedAttributes_Proxy( 
    ICxEventSubscription __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [in] */ DWORD __RPC_FAR *dwAttributeIDs);


void __RPC_STUB ICxEventSubscription_SelectReturnedAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxEventSubscription_ReadPoolItems_Proxy( 
    ICxEventSubscription __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ CXEVENTSTRUCT __RPC_FAR *__RPC_FAR *ppEvents);


void __RPC_STUB ICxEventSubscription_ReadPoolItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxEventSubscription_SetEnable_Proxy( 
    ICxEventSubscription __RPC_FAR * This,
    /* [in] */ BOOL bEnable);


void __RPC_STUB ICxEventSubscription_SetEnable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxEventSubscription_SetFilter_Proxy( 
    ICxEventSubscription __RPC_FAR * This,
    /* [in] */ DWORD dwEventType,
    /* [in] */ DWORD dwNumCategories,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwEventCategories,
    /* [in] */ DWORD dwLowSeverity,
    /* [in] */ DWORD dwHighSeverity,
    /* [in] */ DWORD dwNumAreas,
    /* [size_is][in] */ LPCWSTR __RPC_FAR *pszAreaList,
    /* [in] */ DWORD dwNumSources,
    /* [size_is][in] */ LPCWSTR __RPC_FAR *pszSourceList);


void __RPC_STUB ICxEventSubscription_SetFilter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxEventSubscription_INTERFACE_DEFINED__ */


#ifndef __ICxEventSink_INTERFACE_DEFINED__
#define __ICxEventSink_INTERFACE_DEFINED__

/* interface ICxEventSink */
/* [object][unique][helpstring][uuid] */ 


EXTERN_C const IID IID_ICxEventSink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1C8E194F-4A82-4179-9EFD-AB0290A96449")
    ICxEventSink : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnEvent( 
            /* [in] */ CXHANDLE hClientSubscription,
            /* [in] */ BOOL bRefresh,
            /* [in] */ BOOL bLastRefresh,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ CXEVENTSTRUCT __RPC_FAR *pEvents) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxEventSinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxEventSink __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxEventSink __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxEventSink __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnEvent )( 
            ICxEventSink __RPC_FAR * This,
            /* [in] */ CXHANDLE hClientSubscription,
            /* [in] */ BOOL bRefresh,
            /* [in] */ BOOL bLastRefresh,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ CXEVENTSTRUCT __RPC_FAR *pEvents);
        
        END_INTERFACE
    } ICxEventSinkVtbl;

    interface ICxEventSink
    {
        CONST_VTBL struct ICxEventSinkVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxEventSink_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxEventSink_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxEventSink_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxEventSink_OnEvent(This,hClientSubscription,bRefresh,bLastRefresh,dwCount,pEvents)	\
    (This)->lpVtbl -> OnEvent(This,hClientSubscription,bRefresh,bLastRefresh,dwCount,pEvents)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICxEventSink_OnEvent_Proxy( 
    ICxEventSink __RPC_FAR * This,
    /* [in] */ CXHANDLE hClientSubscription,
    /* [in] */ BOOL bRefresh,
    /* [in] */ BOOL bLastRefresh,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ CXEVENTSTRUCT __RPC_FAR *pEvents);


void __RPC_STUB ICxEventSink_OnEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxEventSink_INTERFACE_DEFINED__ */


#ifndef __ICxServerSynchronization_INTERFACE_DEFINED__
#define __ICxServerSynchronization_INTERFACE_DEFINED__

/* interface ICxServerSynchronization */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_ICxServerSynchronization;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4039BE2B-117F-4507-989A-80C05AF446A7")
    ICxServerSynchronization : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SyncStorage( 
            DWORD dwConnection) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxServerSynchronizationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxServerSynchronization __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxServerSynchronization __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxServerSynchronization __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SyncStorage )( 
            ICxServerSynchronization __RPC_FAR * This,
            DWORD dwConnection);
        
        END_INTERFACE
    } ICxServerSynchronizationVtbl;

    interface ICxServerSynchronization
    {
        CONST_VTBL struct ICxServerSynchronizationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxServerSynchronization_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxServerSynchronization_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxServerSynchronization_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxServerSynchronization_SyncStorage(This,dwConnection)	\
    (This)->lpVtbl -> SyncStorage(This,dwConnection)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICxServerSynchronization_SyncStorage_Proxy( 
    ICxServerSynchronization __RPC_FAR * This,
    DWORD dwConnection);


void __RPC_STUB ICxServerSynchronization_SyncStorage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxServerSynchronization_INTERFACE_DEFINED__ */


#ifndef __ICxSyncNotify_INTERFACE_DEFINED__
#define __ICxSyncNotify_INTERFACE_DEFINED__

/* interface ICxSyncNotify */
/* [object][unique][helpstring][uuid] */ 


EXTERN_C const IID IID_ICxSyncNotify;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("26C64741-B037-4f04-AD32-EBE3BA4C0B2E")
    ICxSyncNotify : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE TransmitFile( 
            /* [in] */ LPCWSTR szFile,
            /* [in] */ FILETIME ftFileTime,
            /* [in] */ DWORD dwCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateStorage( 
            /* [in] */ LPCWSTR szFileName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteStorage( 
            /* [in] */ LPCWSTR szFileName,
            /* [in] */ LPCWSTR szTabNames,
            /* [in] */ DWORD dwPara) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateStream( 
            /* [in] */ LPCWSTR szName,
            /* [out] */ CXHANDLE __RPC_FAR *phStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteStream( 
            /* [in] */ CXHANDLE hStream,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ BYTE __RPC_FAR *pBuffer,
            /* [in] */ DWORD dwBackCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnSync( 
            /* [in] */ BOOL bDroped) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxSyncNotifyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxSyncNotify __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxSyncNotify __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxSyncNotify __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TransmitFile )( 
            ICxSyncNotify __RPC_FAR * This,
            /* [in] */ LPCWSTR szFile,
            /* [in] */ FILETIME ftFileTime,
            /* [in] */ DWORD dwCount);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateStorage )( 
            ICxSyncNotify __RPC_FAR * This,
            /* [in] */ LPCWSTR szFileName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteStorage )( 
            ICxSyncNotify __RPC_FAR * This,
            /* [in] */ LPCWSTR szFileName,
            /* [in] */ LPCWSTR szTabNames,
            /* [in] */ DWORD dwPara);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateStream )( 
            ICxSyncNotify __RPC_FAR * This,
            /* [in] */ LPCWSTR szName,
            /* [out] */ CXHANDLE __RPC_FAR *phStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteStream )( 
            ICxSyncNotify __RPC_FAR * This,
            /* [in] */ CXHANDLE hStream,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ BYTE __RPC_FAR *pBuffer,
            /* [in] */ DWORD dwBackCount);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSync )( 
            ICxSyncNotify __RPC_FAR * This,
            /* [in] */ BOOL bDroped);
        
        END_INTERFACE
    } ICxSyncNotifyVtbl;

    interface ICxSyncNotify
    {
        CONST_VTBL struct ICxSyncNotifyVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxSyncNotify_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxSyncNotify_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxSyncNotify_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxSyncNotify_TransmitFile(This,szFile,ftFileTime,dwCount)	\
    (This)->lpVtbl -> TransmitFile(This,szFile,ftFileTime,dwCount)

#define ICxSyncNotify_CreateStorage(This,szFileName)	\
    (This)->lpVtbl -> CreateStorage(This,szFileName)

#define ICxSyncNotify_DeleteStorage(This,szFileName,szTabNames,dwPara)	\
    (This)->lpVtbl -> DeleteStorage(This,szFileName,szTabNames,dwPara)

#define ICxSyncNotify_CreateStream(This,szName,phStream)	\
    (This)->lpVtbl -> CreateStream(This,szName,phStream)

#define ICxSyncNotify_WriteStream(This,hStream,dwCount,pBuffer,dwBackCount)	\
    (This)->lpVtbl -> WriteStream(This,hStream,dwCount,pBuffer,dwBackCount)

#define ICxSyncNotify_OnSync(This,bDroped)	\
    (This)->lpVtbl -> OnSync(This,bDroped)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICxSyncNotify_TransmitFile_Proxy( 
    ICxSyncNotify __RPC_FAR * This,
    /* [in] */ LPCWSTR szFile,
    /* [in] */ FILETIME ftFileTime,
    /* [in] */ DWORD dwCount);


void __RPC_STUB ICxSyncNotify_TransmitFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxSyncNotify_CreateStorage_Proxy( 
    ICxSyncNotify __RPC_FAR * This,
    /* [in] */ LPCWSTR szFileName);


void __RPC_STUB ICxSyncNotify_CreateStorage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxSyncNotify_DeleteStorage_Proxy( 
    ICxSyncNotify __RPC_FAR * This,
    /* [in] */ LPCWSTR szFileName,
    /* [in] */ LPCWSTR szTabNames,
    /* [in] */ DWORD dwPara);


void __RPC_STUB ICxSyncNotify_DeleteStorage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxSyncNotify_CreateStream_Proxy( 
    ICxSyncNotify __RPC_FAR * This,
    /* [in] */ LPCWSTR szName,
    /* [out] */ CXHANDLE __RPC_FAR *phStream);


void __RPC_STUB ICxSyncNotify_CreateStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxSyncNotify_WriteStream_Proxy( 
    ICxSyncNotify __RPC_FAR * This,
    /* [in] */ CXHANDLE hStream,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ BYTE __RPC_FAR *pBuffer,
    /* [in] */ DWORD dwBackCount);


void __RPC_STUB ICxSyncNotify_WriteStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxSyncNotify_OnSync_Proxy( 
    ICxSyncNotify __RPC_FAR * This,
    /* [in] */ BOOL bDroped);


void __RPC_STUB ICxSyncNotify_OnSync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxSyncNotify_INTERFACE_DEFINED__ */


#ifndef __ITimeSched_INTERFACE_DEFINED__
#define __ITimeSched_INTERFACE_DEFINED__

/* interface ITimeSched */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITimeSched;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51BA5EDB-DC1B-42CE-8F8A-C3C479CAE02A")
    ITimeSched : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ITimeSchedVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITimeSched __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITimeSched __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITimeSched __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITimeSched __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITimeSched __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITimeSched __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITimeSched __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } ITimeSchedVtbl;

    interface ITimeSched
    {
        CONST_VTBL struct ITimeSchedVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITimeSched_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITimeSched_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITimeSched_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITimeSched_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITimeSched_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITimeSched_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITimeSched_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITimeSched_INTERFACE_DEFINED__ */


#ifndef __IEventSched_INTERFACE_DEFINED__
#define __IEventSched_INTERFACE_DEFINED__

/* interface IEventSched */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IEventSched;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("16E0227C-D0DA-4B9A-BFF4-0DEFBA07F854")
    IEventSched : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IEventSchedVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEventSched __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEventSched __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEventSched __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IEventSched __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IEventSched __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IEventSched __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IEventSched __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IEventSchedVtbl;

    interface IEventSched
    {
        CONST_VTBL struct IEventSchedVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEventSched_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEventSched_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEventSched_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEventSched_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IEventSched_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IEventSched_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IEventSched_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEventSched_INTERFACE_DEFINED__ */



#ifndef __CXSACLib_LIBRARY_DEFINED__
#define __CXSACLib_LIBRARY_DEFINED__

/* library CXSACLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_CXSACLib;

EXTERN_C const CLSID CLSID_CxDataServer;

#ifdef __cplusplus

class DECLSPEC_UUID("01682EE0-9654-4EE0-A48F-E44D28B19CE0")
CxDataServer;
#endif

#ifndef ___ITimeSchedEvents_DISPINTERFACE_DEFINED__
#define ___ITimeSchedEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ITimeSchedEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ITimeSchedEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6888DB60-4797-49e4-B336-B3CF1CC48546")
    _ITimeSchedEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ITimeSchedEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _ITimeSchedEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _ITimeSchedEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _ITimeSchedEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _ITimeSchedEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _ITimeSchedEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _ITimeSchedEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _ITimeSchedEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _ITimeSchedEventsVtbl;

    interface _ITimeSchedEvents
    {
        CONST_VTBL struct _ITimeSchedEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ITimeSchedEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ITimeSchedEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _ITimeSchedEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _ITimeSchedEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _ITimeSchedEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _ITimeSchedEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _ITimeSchedEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ITimeSchedEvents_DISPINTERFACE_DEFINED__ */


#ifndef ___IEventSchedEvents_DISPINTERFACE_DEFINED__
#define ___IEventSchedEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IEventSchedEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IEventSchedEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A1C97A37-6619-455c-9471-3B9130B157CD")
    _IEventSchedEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IEventSchedEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _IEventSchedEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _IEventSchedEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _IEventSchedEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _IEventSchedEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _IEventSchedEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _IEventSchedEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _IEventSchedEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _IEventSchedEventsVtbl;

    interface _IEventSchedEvents
    {
        CONST_VTBL struct _IEventSchedEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IEventSchedEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IEventSchedEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IEventSchedEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IEventSchedEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IEventSchedEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IEventSchedEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IEventSchedEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IEventSchedEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_TimeSched;

#ifdef __cplusplus

class DECLSPEC_UUID("B5F22D21-2FFB-4D9E-B85E-CAB2BD83D0D4")
TimeSched;
#endif

EXTERN_C const CLSID CLSID_EventSched;

#ifdef __cplusplus

class DECLSPEC_UUID("85054ABD-074C-43C0-91C6-6097335F8035")
EventSched;
#endif

EXTERN_C const CLSID CLSID_CxOPCServer;

#ifdef __cplusplus

class DECLSPEC_UUID("D5743C70-FFEB-43B8-9ACA-FB99D692E1C4")
CxOPCServer;
#endif
#endif /* __CXSACLib_LIBRARY_DEFINED__ */

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
