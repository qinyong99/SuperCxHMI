/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Aug 11 11:02:35 2013
 */
/* Compiler settings for D:\spcx\svr\CxDatSes\CxDatSes.idl:
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

#ifndef __CxDatSes_h__
#define __CxDatSes_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ICxDataSession_FWD_DEFINED__
#define __ICxDataSession_FWD_DEFINED__
typedef interface ICxDataSession ICxDataSession;
#endif 	/* __ICxDataSession_FWD_DEFINED__ */


#ifndef __ICxAutoValue_FWD_DEFINED__
#define __ICxAutoValue_FWD_DEFINED__
typedef interface ICxAutoValue ICxAutoValue;
#endif 	/* __ICxAutoValue_FWD_DEFINED__ */


#ifndef __ICxAutoHistory_FWD_DEFINED__
#define __ICxAutoHistory_FWD_DEFINED__
typedef interface ICxAutoHistory ICxAutoHistory;
#endif 	/* __ICxAutoHistory_FWD_DEFINED__ */


#ifndef __ICxAutoDataSession_FWD_DEFINED__
#define __ICxAutoDataSession_FWD_DEFINED__
typedef interface ICxAutoDataSession ICxAutoDataSession;
#endif 	/* __ICxAutoDataSession_FWD_DEFINED__ */


#ifndef __ICxCommomFunctions_FWD_DEFINED__
#define __ICxCommomFunctions_FWD_DEFINED__
typedef interface ICxCommomFunctions ICxCommomFunctions;
#endif 	/* __ICxCommomFunctions_FWD_DEFINED__ */


#ifndef __ICxAutoValue_FWD_DEFINED__
#define __ICxAutoValue_FWD_DEFINED__
typedef interface ICxAutoValue ICxAutoValue;
#endif 	/* __ICxAutoValue_FWD_DEFINED__ */


#ifndef __ICxAutoHistory_FWD_DEFINED__
#define __ICxAutoHistory_FWD_DEFINED__
typedef interface ICxAutoHistory ICxAutoHistory;
#endif 	/* __ICxAutoHistory_FWD_DEFINED__ */


#ifndef __ICxCommomFunctions_FWD_DEFINED__
#define __ICxCommomFunctions_FWD_DEFINED__
typedef interface ICxCommomFunctions ICxCommomFunctions;
#endif 	/* __ICxCommomFunctions_FWD_DEFINED__ */


#ifndef __CxDataSession_FWD_DEFINED__
#define __CxDataSession_FWD_DEFINED__

#ifdef __cplusplus
typedef class CxDataSession CxDataSession;
#else
typedef struct CxDataSession CxDataSession;
#endif /* __cplusplus */

#endif 	/* __CxDataSession_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_CxDatSes_0000 */
/* [local] */ 

#define WM_ALARM_NOTIFY    WM_USER + 256
#undef ExitWindows
typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_CxDatSes_0000_0001
    {	ReadTagMode_Normal	= 0,
	ReadTagMode_Add	= 1,
	ReadTagMode_Update	= 2
    }	enumReadTagMode;



extern RPC_IF_HANDLE __MIDL_itf_CxDatSes_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_CxDatSes_0000_v0_0_s_ifspec;

#ifndef __ICxDataSession_INTERFACE_DEFINED__
#define __ICxDataSession_INTERFACE_DEFINED__

/* interface ICxDataSession */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ICxDataSession;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("33F87734-E676-4E73-B6B8-4E8E4F294D8C")
    ICxDataSession : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetDataSourceCount( 
            /* [out] */ DWORD __RPC_FAR *pdwCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDataSourceInfo( 
            /* [in] */ DWORD dwIndex,
            /* [string][in] */ LPWSTR szSource,
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispSource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTagGroup( 
            /* [string][in] */ LPCWSTR szTagGroup,
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispGroup) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetReadTagMode( 
            /* [in] */ enumReadTagMode iMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CommitTags( 
            BOOL bAutoRefresh) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ResetTags( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTagRange( 
            /* [string][in] */ LPWSTR szTag,
            /* [out] */ VARIANT __RPC_FAR *pvaMin,
            /* [out] */ VARIANT __RPC_FAR *pvaMax) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadTag( 
            /* [string][in] */ LPWSTR szTag,
            /* [out] */ VARIANT __RPC_FAR *pvaValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteTag( 
            /* [string][in] */ LPWSTR szTag,
            /* [in] */ VARIANT __RPC_FAR *pvaValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RequestTag( 
            /* [in] */ LPWSTR szTag,
            /* [out] */ DWORD __RPC_FAR *pdwCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelRequestTag( 
            DWORD dwNumCookies,
            DWORD __RPC_FAR *pdwCookies) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadTagByCookie( 
            /* [in] */ DWORD dwCookie,
            /* [out] */ VARIANT __RPC_FAR *pvaValue,
            /* [out] */ FILETIME __RPC_FAR *pftTimeStamp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteTagByCookie( 
            /* [in] */ DWORD dwCookie,
            /* [in] */ VARIANT __RPC_FAR *pvaValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTagValueType( 
            /* [in] */ DWORD dwNumCookies,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwCookies,
            /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppVartypes,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppSizes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CacheTagHistoryData( 
            /* [out][in] */ FILETIME __RPC_FAR *pftStartTime,
            /* [out][in] */ FILETIME __RPC_FAR *pftEndTime,
            /* [in] */ DWORD dwNumValues,
            /* [in] */ BOOL bBounds,
            /* [in] */ LPCWSTR szLogTab,
            /* [in] */ DWORD dwNumCookies,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwCookies) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTagHistoryDataCache( 
            /* [in] */ DWORD dwCookie,
            /* [out][in] */ DWORD __RPC_FAR *pdwNumValues,
            /* [size_is][size_is][out] */ FILETIME __RPC_FAR *__RPC_FAR *ppftTimeStamps,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvDataValues) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadTagHistoryData( 
            /* [in] */ DWORD dwCookie,
            /* [in] */ BOOL bBounds,
            /* [out][in] */ FILETIME __RPC_FAR *pftStartTime,
            /* [out][in] */ FILETIME __RPC_FAR *pftEndTime,
            /* [out][in] */ DWORD __RPC_FAR *pdwNumValues,
            /* [size_is][size_is][out] */ FILETIME __RPC_FAR *__RPC_FAR *ppftTimeStamps,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvDataValues) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RefreshTags( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTagProperties( 
            /* [in] */ LPWSTR szTag,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwPropertyIDs,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvData,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUserAccessRight( 
            /* [in] */ LPWSTR __RPC_FAR *pszUserName,
            /* [in] */ LPCWSTR szPassword,
            /* [string][out] */ LPWSTR __RPC_FAR *pSecurityAreas,
            /* [out] */ DWORD __RPC_FAR *pdwPrivilege) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AcknowledgeAlarm( 
            /* [in] */ LPCWSTR szTag) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LogUserOperation( 
            /* [string][in] */ LPCWSTR szSource,
            /* [string][in] */ LPCWSTR szMessage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LogSOEEvent( 
            /* [in] */ FILETIME ftTime,
            /* [string][in] */ LPCWSTR szSource,
            /* [string][in] */ LPCWSTR szMessage,
            /* [in] */ DWORD dwSeverity) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetThisNode( 
            /* [string][in] */ LPCWSTR szComputer,
            /* [in] */ IUnknown __RPC_FAR *pUnk,
            /* [in] */ BOOL bOnlyOnce) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetThisNodeName( 
            /* [string][out] */ LPWSTR __RPC_FAR *ppszName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FetchPictureFile( 
            /* [in] */ LPCWSTR szFile,
            /* [out][in] */ FILETIME __RPC_FAR *pftFileTime,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ BYTE __RPC_FAR *__RPC_FAR *ppBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SubscribeAlarm( 
            /* [in] */ LPCWSTR szNode,
            /* [in] */ BOOL bSubscribe) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetNotifyWindow( 
            /* [in] */ HWND hWnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNetNodes( 
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppNames) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTagLogTabs( 
            /* [in] */ LPCWSTR szNode,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppNames) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTagsOfLogTab( 
            /* [in] */ LPCWSTR szNode,
            /* [in] */ LPCWSTR szLogTab,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppNames) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCommomFunctions( 
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispSource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNodeServer( 
            /* [in] */ LPCWSTR szNode,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StartServerMonitor( 
            /* [in] */ LPCWSTR szServer,
            /* [in] */ HWND hNotifyWnd,
            /* [in] */ DWORD dwMessage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSACProxy( 
            /* [in] */ BOOL bLocalUsed,
            /* [in] */ BOOL bRemoteUsed,
            /* [in] */ long lPort) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetAutoLogin( 
            /* [in] */ BOOL bLogin,
            /* [in] */ BOOL bPrompt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateServerObject( 
            /* [in] */ VARIANT vaClassValue,
            /* [out] */ VARIANT __RPC_FAR *pvaObject,
            /* [in] */ BOOL bAllowAsLocal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCreateServerObjectAsLocal( 
            /* [in] */ BOOL bAsLocal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDefaultUserPassword( 
            /* [in] */ LPCWSTR szUser,
            /* [in] */ LPCWSTR szPassword) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDeviceStatus( 
            /* [string][in] */ LPCWSTR szDevice,
            /* [out] */ DWORD __RPC_FAR *pdwState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDeviceStatus( 
            /* [string][in] */ LPCWSTR szDevice,
            /* [in] */ DWORD dwState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ThisNodeIsLocal( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddAsynObjectCall( 
            /* [string][in] */ LPCWSTR szID,
            /* [in] */ IDispatch __RPC_FAR *pdispObject,
            /* [string][in] */ LPCWSTR szFuncName,
            /* [in] */ DWORD dwArgCount,
            /* [in] */ VARIANT __RPC_FAR *pvaArgs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAsynObjectCallValue( 
            /* [string][in] */ LPCWSTR szID,
            /* [out] */ VARIANT __RPC_FAR *pvaValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetAsynObjectCallValue( 
            /* [string][in] */ LPCWSTR szID,
            /* [out] */ VARIANT __RPC_FAR *pvaValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetAsynObjectCallEnable( 
            /* [in] */ IDispatch __RPC_FAR *pdispObject,
            /* [in] */ BOOL bEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InvokeAsynObjectCallAtOnce( 
            /* [in] */ IDispatch __RPC_FAR *pdispObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SaveTagStatus( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BindObjectsToTagGroup( 
            /* [in] */ IUnknown __RPC_FAR *__RPC_FAR *ppObjects,
            /* [in] */ DWORD dwCount,
            /* [in] */ LPCWSTR szGroup) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxDataSessionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxDataSession __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxDataSession __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDataSourceCount )( 
            ICxDataSession __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwCount);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDataSourceInfo )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ DWORD dwIndex,
            /* [string][in] */ LPWSTR szSource,
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispSource);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTagGroup )( 
            ICxDataSession __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szTagGroup,
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispGroup);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetReadTagMode )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ enumReadTagMode iMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CommitTags )( 
            ICxDataSession __RPC_FAR * This,
            BOOL bAutoRefresh);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ResetTags )( 
            ICxDataSession __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTagRange )( 
            ICxDataSession __RPC_FAR * This,
            /* [string][in] */ LPWSTR szTag,
            /* [out] */ VARIANT __RPC_FAR *pvaMin,
            /* [out] */ VARIANT __RPC_FAR *pvaMax);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadTag )( 
            ICxDataSession __RPC_FAR * This,
            /* [string][in] */ LPWSTR szTag,
            /* [out] */ VARIANT __RPC_FAR *pvaValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteTag )( 
            ICxDataSession __RPC_FAR * This,
            /* [string][in] */ LPWSTR szTag,
            /* [in] */ VARIANT __RPC_FAR *pvaValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RequestTag )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ LPWSTR szTag,
            /* [out] */ DWORD __RPC_FAR *pdwCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CancelRequestTag )( 
            ICxDataSession __RPC_FAR * This,
            DWORD dwNumCookies,
            DWORD __RPC_FAR *pdwCookies);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadTagByCookie )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ DWORD dwCookie,
            /* [out] */ VARIANT __RPC_FAR *pvaValue,
            /* [out] */ FILETIME __RPC_FAR *pftTimeStamp);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteTagByCookie )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ DWORD dwCookie,
            /* [in] */ VARIANT __RPC_FAR *pvaValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTagValueType )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ DWORD dwNumCookies,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwCookies,
            /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppVartypes,
            /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppSizes);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CacheTagHistoryData )( 
            ICxDataSession __RPC_FAR * This,
            /* [out][in] */ FILETIME __RPC_FAR *pftStartTime,
            /* [out][in] */ FILETIME __RPC_FAR *pftEndTime,
            /* [in] */ DWORD dwNumValues,
            /* [in] */ BOOL bBounds,
            /* [in] */ LPCWSTR szLogTab,
            /* [in] */ DWORD dwNumCookies,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwCookies);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTagHistoryDataCache )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ DWORD dwCookie,
            /* [out][in] */ DWORD __RPC_FAR *pdwNumValues,
            /* [size_is][size_is][out] */ FILETIME __RPC_FAR *__RPC_FAR *ppftTimeStamps,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvDataValues);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadTagHistoryData )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ DWORD dwCookie,
            /* [in] */ BOOL bBounds,
            /* [out][in] */ FILETIME __RPC_FAR *pftStartTime,
            /* [out][in] */ FILETIME __RPC_FAR *pftEndTime,
            /* [out][in] */ DWORD __RPC_FAR *pdwNumValues,
            /* [size_is][size_is][out] */ FILETIME __RPC_FAR *__RPC_FAR *ppftTimeStamps,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvDataValues);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RefreshTags )( 
            ICxDataSession __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTagProperties )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ LPWSTR szTag,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ DWORD __RPC_FAR *pdwPropertyIDs,
            /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvData,
            /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUserAccessRight )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ LPWSTR __RPC_FAR *pszUserName,
            /* [in] */ LPCWSTR szPassword,
            /* [string][out] */ LPWSTR __RPC_FAR *pSecurityAreas,
            /* [out] */ DWORD __RPC_FAR *pdwPrivilege);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AcknowledgeAlarm )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ LPCWSTR szTag);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LogUserOperation )( 
            ICxDataSession __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szSource,
            /* [string][in] */ LPCWSTR szMessage);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LogSOEEvent )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ FILETIME ftTime,
            /* [string][in] */ LPCWSTR szSource,
            /* [string][in] */ LPCWSTR szMessage,
            /* [in] */ DWORD dwSeverity);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetThisNode )( 
            ICxDataSession __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szComputer,
            /* [in] */ IUnknown __RPC_FAR *pUnk,
            /* [in] */ BOOL bOnlyOnce);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetThisNodeName )( 
            ICxDataSession __RPC_FAR * This,
            /* [string][out] */ LPWSTR __RPC_FAR *ppszName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FetchPictureFile )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ LPCWSTR szFile,
            /* [out][in] */ FILETIME __RPC_FAR *pftFileTime,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ BYTE __RPC_FAR *__RPC_FAR *ppBuffer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SubscribeAlarm )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ LPCWSTR szNode,
            /* [in] */ BOOL bSubscribe);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetNotifyWindow )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ HWND hWnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNetNodes )( 
            ICxDataSession __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppNames);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTagLogTabs )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ LPCWSTR szNode,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppNames);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTagsOfLogTab )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ LPCWSTR szNode,
            /* [in] */ LPCWSTR szLogTab,
            /* [out] */ DWORD __RPC_FAR *pdwCount,
            /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppNames);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCommomFunctions )( 
            ICxDataSession __RPC_FAR * This,
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispSource);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNodeServer )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ LPCWSTR szNode,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartServerMonitor )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ LPCWSTR szServer,
            /* [in] */ HWND hNotifyWnd,
            /* [in] */ DWORD dwMessage);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSACProxy )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ BOOL bLocalUsed,
            /* [in] */ BOOL bRemoteUsed,
            /* [in] */ long lPort);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAutoLogin )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ BOOL bLogin,
            /* [in] */ BOOL bPrompt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateServerObject )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ VARIANT vaClassValue,
            /* [out] */ VARIANT __RPC_FAR *pvaObject,
            /* [in] */ BOOL bAllowAsLocal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCreateServerObjectAsLocal )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ BOOL bAsLocal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDefaultUserPassword )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ LPCWSTR szUser,
            /* [in] */ LPCWSTR szPassword);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDeviceStatus )( 
            ICxDataSession __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szDevice,
            /* [out] */ DWORD __RPC_FAR *pdwState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDeviceStatus )( 
            ICxDataSession __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szDevice,
            /* [in] */ DWORD dwState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ThisNodeIsLocal )( 
            ICxDataSession __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddAsynObjectCall )( 
            ICxDataSession __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szID,
            /* [in] */ IDispatch __RPC_FAR *pdispObject,
            /* [string][in] */ LPCWSTR szFuncName,
            /* [in] */ DWORD dwArgCount,
            /* [in] */ VARIANT __RPC_FAR *pvaArgs);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAsynObjectCallValue )( 
            ICxDataSession __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szID,
            /* [out] */ VARIANT __RPC_FAR *pvaValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAsynObjectCallValue )( 
            ICxDataSession __RPC_FAR * This,
            /* [string][in] */ LPCWSTR szID,
            /* [out] */ VARIANT __RPC_FAR *pvaValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAsynObjectCallEnable )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pdispObject,
            /* [in] */ BOOL bEnable);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeAsynObjectCallAtOnce )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pdispObject);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveTagStatus )( 
            ICxDataSession __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BindObjectsToTagGroup )( 
            ICxDataSession __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *__RPC_FAR *ppObjects,
            /* [in] */ DWORD dwCount,
            /* [in] */ LPCWSTR szGroup);
        
        END_INTERFACE
    } ICxDataSessionVtbl;

    interface ICxDataSession
    {
        CONST_VTBL struct ICxDataSessionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxDataSession_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxDataSession_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxDataSession_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxDataSession_GetDataSourceCount(This,pdwCount)	\
    (This)->lpVtbl -> GetDataSourceCount(This,pdwCount)

#define ICxDataSession_GetDataSourceInfo(This,dwIndex,szSource,ppdispSource)	\
    (This)->lpVtbl -> GetDataSourceInfo(This,dwIndex,szSource,ppdispSource)

#define ICxDataSession_GetTagGroup(This,szTagGroup,ppdispGroup)	\
    (This)->lpVtbl -> GetTagGroup(This,szTagGroup,ppdispGroup)

#define ICxDataSession_SetReadTagMode(This,iMode)	\
    (This)->lpVtbl -> SetReadTagMode(This,iMode)

#define ICxDataSession_CommitTags(This,bAutoRefresh)	\
    (This)->lpVtbl -> CommitTags(This,bAutoRefresh)

#define ICxDataSession_ResetTags(This)	\
    (This)->lpVtbl -> ResetTags(This)

#define ICxDataSession_GetTagRange(This,szTag,pvaMin,pvaMax)	\
    (This)->lpVtbl -> GetTagRange(This,szTag,pvaMin,pvaMax)

#define ICxDataSession_ReadTag(This,szTag,pvaValue)	\
    (This)->lpVtbl -> ReadTag(This,szTag,pvaValue)

#define ICxDataSession_WriteTag(This,szTag,pvaValue)	\
    (This)->lpVtbl -> WriteTag(This,szTag,pvaValue)

#define ICxDataSession_RequestTag(This,szTag,pdwCookie)	\
    (This)->lpVtbl -> RequestTag(This,szTag,pdwCookie)

#define ICxDataSession_CancelRequestTag(This,dwNumCookies,pdwCookies)	\
    (This)->lpVtbl -> CancelRequestTag(This,dwNumCookies,pdwCookies)

#define ICxDataSession_ReadTagByCookie(This,dwCookie,pvaValue,pftTimeStamp)	\
    (This)->lpVtbl -> ReadTagByCookie(This,dwCookie,pvaValue,pftTimeStamp)

#define ICxDataSession_WriteTagByCookie(This,dwCookie,pvaValue)	\
    (This)->lpVtbl -> WriteTagByCookie(This,dwCookie,pvaValue)

#define ICxDataSession_GetTagValueType(This,dwNumCookies,pdwCookies,ppVartypes,ppSizes)	\
    (This)->lpVtbl -> GetTagValueType(This,dwNumCookies,pdwCookies,ppVartypes,ppSizes)

#define ICxDataSession_CacheTagHistoryData(This,pftStartTime,pftEndTime,dwNumValues,bBounds,szLogTab,dwNumCookies,pdwCookies)	\
    (This)->lpVtbl -> CacheTagHistoryData(This,pftStartTime,pftEndTime,dwNumValues,bBounds,szLogTab,dwNumCookies,pdwCookies)

#define ICxDataSession_GetTagHistoryDataCache(This,dwCookie,pdwNumValues,ppftTimeStamps,ppvDataValues)	\
    (This)->lpVtbl -> GetTagHistoryDataCache(This,dwCookie,pdwNumValues,ppftTimeStamps,ppvDataValues)

#define ICxDataSession_ReadTagHistoryData(This,dwCookie,bBounds,pftStartTime,pftEndTime,pdwNumValues,ppftTimeStamps,ppvDataValues)	\
    (This)->lpVtbl -> ReadTagHistoryData(This,dwCookie,bBounds,pftStartTime,pftEndTime,pdwNumValues,ppftTimeStamps,ppvDataValues)

#define ICxDataSession_RefreshTags(This)	\
    (This)->lpVtbl -> RefreshTags(This)

#define ICxDataSession_GetTagProperties(This,szTag,dwCount,pdwPropertyIDs,ppvData,ppErrors)	\
    (This)->lpVtbl -> GetTagProperties(This,szTag,dwCount,pdwPropertyIDs,ppvData,ppErrors)

#define ICxDataSession_GetUserAccessRight(This,pszUserName,szPassword,pSecurityAreas,pdwPrivilege)	\
    (This)->lpVtbl -> GetUserAccessRight(This,pszUserName,szPassword,pSecurityAreas,pdwPrivilege)

#define ICxDataSession_AcknowledgeAlarm(This,szTag)	\
    (This)->lpVtbl -> AcknowledgeAlarm(This,szTag)

#define ICxDataSession_LogUserOperation(This,szSource,szMessage)	\
    (This)->lpVtbl -> LogUserOperation(This,szSource,szMessage)

#define ICxDataSession_LogSOEEvent(This,ftTime,szSource,szMessage,dwSeverity)	\
    (This)->lpVtbl -> LogSOEEvent(This,ftTime,szSource,szMessage,dwSeverity)

#define ICxDataSession_SetThisNode(This,szComputer,pUnk,bOnlyOnce)	\
    (This)->lpVtbl -> SetThisNode(This,szComputer,pUnk,bOnlyOnce)

#define ICxDataSession_GetThisNodeName(This,ppszName)	\
    (This)->lpVtbl -> GetThisNodeName(This,ppszName)

#define ICxDataSession_FetchPictureFile(This,szFile,pftFileTime,pdwCount,ppBuffer)	\
    (This)->lpVtbl -> FetchPictureFile(This,szFile,pftFileTime,pdwCount,ppBuffer)

#define ICxDataSession_SubscribeAlarm(This,szNode,bSubscribe)	\
    (This)->lpVtbl -> SubscribeAlarm(This,szNode,bSubscribe)

#define ICxDataSession_SetNotifyWindow(This,hWnd)	\
    (This)->lpVtbl -> SetNotifyWindow(This,hWnd)

#define ICxDataSession_GetNetNodes(This,pdwCount,ppNames)	\
    (This)->lpVtbl -> GetNetNodes(This,pdwCount,ppNames)

#define ICxDataSession_GetTagLogTabs(This,szNode,pdwCount,ppNames)	\
    (This)->lpVtbl -> GetTagLogTabs(This,szNode,pdwCount,ppNames)

#define ICxDataSession_GetTagsOfLogTab(This,szNode,szLogTab,pdwCount,ppNames)	\
    (This)->lpVtbl -> GetTagsOfLogTab(This,szNode,szLogTab,pdwCount,ppNames)

#define ICxDataSession_GetCommomFunctions(This,ppdispSource)	\
    (This)->lpVtbl -> GetCommomFunctions(This,ppdispSource)

#define ICxDataSession_GetNodeServer(This,szNode,riid,ppUnk)	\
    (This)->lpVtbl -> GetNodeServer(This,szNode,riid,ppUnk)

#define ICxDataSession_StartServerMonitor(This,szServer,hNotifyWnd,dwMessage)	\
    (This)->lpVtbl -> StartServerMonitor(This,szServer,hNotifyWnd,dwMessage)

#define ICxDataSession_SetSACProxy(This,bLocalUsed,bRemoteUsed,lPort)	\
    (This)->lpVtbl -> SetSACProxy(This,bLocalUsed,bRemoteUsed,lPort)

#define ICxDataSession_SetAutoLogin(This,bLogin,bPrompt)	\
    (This)->lpVtbl -> SetAutoLogin(This,bLogin,bPrompt)

#define ICxDataSession_CreateServerObject(This,vaClassValue,pvaObject,bAllowAsLocal)	\
    (This)->lpVtbl -> CreateServerObject(This,vaClassValue,pvaObject,bAllowAsLocal)

#define ICxDataSession_SetCreateServerObjectAsLocal(This,bAsLocal)	\
    (This)->lpVtbl -> SetCreateServerObjectAsLocal(This,bAsLocal)

#define ICxDataSession_SetDefaultUserPassword(This,szUser,szPassword)	\
    (This)->lpVtbl -> SetDefaultUserPassword(This,szUser,szPassword)

#define ICxDataSession_GetDeviceStatus(This,szDevice,pdwState)	\
    (This)->lpVtbl -> GetDeviceStatus(This,szDevice,pdwState)

#define ICxDataSession_SetDeviceStatus(This,szDevice,dwState)	\
    (This)->lpVtbl -> SetDeviceStatus(This,szDevice,dwState)

#define ICxDataSession_ThisNodeIsLocal(This)	\
    (This)->lpVtbl -> ThisNodeIsLocal(This)

#define ICxDataSession_AddAsynObjectCall(This,szID,pdispObject,szFuncName,dwArgCount,pvaArgs)	\
    (This)->lpVtbl -> AddAsynObjectCall(This,szID,pdispObject,szFuncName,dwArgCount,pvaArgs)

#define ICxDataSession_GetAsynObjectCallValue(This,szID,pvaValue)	\
    (This)->lpVtbl -> GetAsynObjectCallValue(This,szID,pvaValue)

#define ICxDataSession_SetAsynObjectCallValue(This,szID,pvaValue)	\
    (This)->lpVtbl -> SetAsynObjectCallValue(This,szID,pvaValue)

#define ICxDataSession_SetAsynObjectCallEnable(This,pdispObject,bEnable)	\
    (This)->lpVtbl -> SetAsynObjectCallEnable(This,pdispObject,bEnable)

#define ICxDataSession_InvokeAsynObjectCallAtOnce(This,pdispObject)	\
    (This)->lpVtbl -> InvokeAsynObjectCallAtOnce(This,pdispObject)

#define ICxDataSession_SaveTagStatus(This)	\
    (This)->lpVtbl -> SaveTagStatus(This)

#define ICxDataSession_BindObjectsToTagGroup(This,ppObjects,dwCount,szGroup)	\
    (This)->lpVtbl -> BindObjectsToTagGroup(This,ppObjects,dwCount,szGroup)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICxDataSession_GetDataSourceCount_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwCount);


void __RPC_STUB ICxDataSession_GetDataSourceCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetDataSourceInfo_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ DWORD dwIndex,
    /* [string][in] */ LPWSTR szSource,
    /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispSource);


void __RPC_STUB ICxDataSession_GetDataSourceInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetTagGroup_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szTagGroup,
    /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispGroup);


void __RPC_STUB ICxDataSession_GetTagGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_SetReadTagMode_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ enumReadTagMode iMode);


void __RPC_STUB ICxDataSession_SetReadTagMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_CommitTags_Proxy( 
    ICxDataSession __RPC_FAR * This,
    BOOL bAutoRefresh);


void __RPC_STUB ICxDataSession_CommitTags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_ResetTags_Proxy( 
    ICxDataSession __RPC_FAR * This);


void __RPC_STUB ICxDataSession_ResetTags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetTagRange_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [string][in] */ LPWSTR szTag,
    /* [out] */ VARIANT __RPC_FAR *pvaMin,
    /* [out] */ VARIANT __RPC_FAR *pvaMax);


void __RPC_STUB ICxDataSession_GetTagRange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_ReadTag_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [string][in] */ LPWSTR szTag,
    /* [out] */ VARIANT __RPC_FAR *pvaValue);


void __RPC_STUB ICxDataSession_ReadTag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_WriteTag_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [string][in] */ LPWSTR szTag,
    /* [in] */ VARIANT __RPC_FAR *pvaValue);


void __RPC_STUB ICxDataSession_WriteTag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_RequestTag_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ LPWSTR szTag,
    /* [out] */ DWORD __RPC_FAR *pdwCookie);


void __RPC_STUB ICxDataSession_RequestTag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_CancelRequestTag_Proxy( 
    ICxDataSession __RPC_FAR * This,
    DWORD dwNumCookies,
    DWORD __RPC_FAR *pdwCookies);


void __RPC_STUB ICxDataSession_CancelRequestTag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_ReadTagByCookie_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ DWORD dwCookie,
    /* [out] */ VARIANT __RPC_FAR *pvaValue,
    /* [out] */ FILETIME __RPC_FAR *pftTimeStamp);


void __RPC_STUB ICxDataSession_ReadTagByCookie_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_WriteTagByCookie_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ DWORD dwCookie,
    /* [in] */ VARIANT __RPC_FAR *pvaValue);


void __RPC_STUB ICxDataSession_WriteTagByCookie_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetTagValueType_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ DWORD dwNumCookies,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwCookies,
    /* [size_is][size_is][out] */ VARTYPE __RPC_FAR *__RPC_FAR *ppVartypes,
    /* [size_is][size_is][out] */ DWORD __RPC_FAR *__RPC_FAR *ppSizes);


void __RPC_STUB ICxDataSession_GetTagValueType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_CacheTagHistoryData_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [out][in] */ FILETIME __RPC_FAR *pftStartTime,
    /* [out][in] */ FILETIME __RPC_FAR *pftEndTime,
    /* [in] */ DWORD dwNumValues,
    /* [in] */ BOOL bBounds,
    /* [in] */ LPCWSTR szLogTab,
    /* [in] */ DWORD dwNumCookies,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwCookies);


void __RPC_STUB ICxDataSession_CacheTagHistoryData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetTagHistoryDataCache_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ DWORD dwCookie,
    /* [out][in] */ DWORD __RPC_FAR *pdwNumValues,
    /* [size_is][size_is][out] */ FILETIME __RPC_FAR *__RPC_FAR *ppftTimeStamps,
    /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvDataValues);


void __RPC_STUB ICxDataSession_GetTagHistoryDataCache_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_ReadTagHistoryData_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ DWORD dwCookie,
    /* [in] */ BOOL bBounds,
    /* [out][in] */ FILETIME __RPC_FAR *pftStartTime,
    /* [out][in] */ FILETIME __RPC_FAR *pftEndTime,
    /* [out][in] */ DWORD __RPC_FAR *pdwNumValues,
    /* [size_is][size_is][out] */ FILETIME __RPC_FAR *__RPC_FAR *ppftTimeStamps,
    /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvDataValues);


void __RPC_STUB ICxDataSession_ReadTagHistoryData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_RefreshTags_Proxy( 
    ICxDataSession __RPC_FAR * This);


void __RPC_STUB ICxDataSession_RefreshTags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetTagProperties_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ LPWSTR szTag,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ DWORD __RPC_FAR *pdwPropertyIDs,
    /* [size_is][size_is][out] */ VARIANT __RPC_FAR *__RPC_FAR *ppvData,
    /* [size_is][size_is][out] */ HRESULT __RPC_FAR *__RPC_FAR *ppErrors);


void __RPC_STUB ICxDataSession_GetTagProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetUserAccessRight_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ LPWSTR __RPC_FAR *pszUserName,
    /* [in] */ LPCWSTR szPassword,
    /* [string][out] */ LPWSTR __RPC_FAR *pSecurityAreas,
    /* [out] */ DWORD __RPC_FAR *pdwPrivilege);


void __RPC_STUB ICxDataSession_GetUserAccessRight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_AcknowledgeAlarm_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ LPCWSTR szTag);


void __RPC_STUB ICxDataSession_AcknowledgeAlarm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_LogUserOperation_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szSource,
    /* [string][in] */ LPCWSTR szMessage);


void __RPC_STUB ICxDataSession_LogUserOperation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_LogSOEEvent_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ FILETIME ftTime,
    /* [string][in] */ LPCWSTR szSource,
    /* [string][in] */ LPCWSTR szMessage,
    /* [in] */ DWORD dwSeverity);


void __RPC_STUB ICxDataSession_LogSOEEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_SetThisNode_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szComputer,
    /* [in] */ IUnknown __RPC_FAR *pUnk,
    /* [in] */ BOOL bOnlyOnce);


void __RPC_STUB ICxDataSession_SetThisNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetThisNodeName_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [string][out] */ LPWSTR __RPC_FAR *ppszName);


void __RPC_STUB ICxDataSession_GetThisNodeName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_FetchPictureFile_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ LPCWSTR szFile,
    /* [out][in] */ FILETIME __RPC_FAR *pftFileTime,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ BYTE __RPC_FAR *__RPC_FAR *ppBuffer);


void __RPC_STUB ICxDataSession_FetchPictureFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_SubscribeAlarm_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ LPCWSTR szNode,
    /* [in] */ BOOL bSubscribe);


void __RPC_STUB ICxDataSession_SubscribeAlarm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_SetNotifyWindow_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ HWND hWnd);


void __RPC_STUB ICxDataSession_SetNotifyWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetNetNodes_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppNames);


void __RPC_STUB ICxDataSession_GetNetNodes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetTagLogTabs_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ LPCWSTR szNode,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppNames);


void __RPC_STUB ICxDataSession_GetTagLogTabs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetTagsOfLogTab_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ LPCWSTR szNode,
    /* [in] */ LPCWSTR szLogTab,
    /* [out] */ DWORD __RPC_FAR *pdwCount,
    /* [size_is][size_is][out] */ LPWSTR __RPC_FAR *__RPC_FAR *ppNames);


void __RPC_STUB ICxDataSession_GetTagsOfLogTab_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetCommomFunctions_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispSource);


void __RPC_STUB ICxDataSession_GetCommomFunctions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetNodeServer_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ LPCWSTR szNode,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ LPUNKNOWN __RPC_FAR *ppUnk);


void __RPC_STUB ICxDataSession_GetNodeServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_StartServerMonitor_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ LPCWSTR szServer,
    /* [in] */ HWND hNotifyWnd,
    /* [in] */ DWORD dwMessage);


void __RPC_STUB ICxDataSession_StartServerMonitor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_SetSACProxy_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ BOOL bLocalUsed,
    /* [in] */ BOOL bRemoteUsed,
    /* [in] */ long lPort);


void __RPC_STUB ICxDataSession_SetSACProxy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_SetAutoLogin_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ BOOL bLogin,
    /* [in] */ BOOL bPrompt);


void __RPC_STUB ICxDataSession_SetAutoLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_CreateServerObject_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ VARIANT vaClassValue,
    /* [out] */ VARIANT __RPC_FAR *pvaObject,
    /* [in] */ BOOL bAllowAsLocal);


void __RPC_STUB ICxDataSession_CreateServerObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_SetCreateServerObjectAsLocal_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ BOOL bAsLocal);


void __RPC_STUB ICxDataSession_SetCreateServerObjectAsLocal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_SetDefaultUserPassword_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ LPCWSTR szUser,
    /* [in] */ LPCWSTR szPassword);


void __RPC_STUB ICxDataSession_SetDefaultUserPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetDeviceStatus_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szDevice,
    /* [out] */ DWORD __RPC_FAR *pdwState);


void __RPC_STUB ICxDataSession_GetDeviceStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_SetDeviceStatus_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szDevice,
    /* [in] */ DWORD dwState);


void __RPC_STUB ICxDataSession_SetDeviceStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_ThisNodeIsLocal_Proxy( 
    ICxDataSession __RPC_FAR * This);


void __RPC_STUB ICxDataSession_ThisNodeIsLocal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_AddAsynObjectCall_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szID,
    /* [in] */ IDispatch __RPC_FAR *pdispObject,
    /* [string][in] */ LPCWSTR szFuncName,
    /* [in] */ DWORD dwArgCount,
    /* [in] */ VARIANT __RPC_FAR *pvaArgs);


void __RPC_STUB ICxDataSession_AddAsynObjectCall_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_GetAsynObjectCallValue_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szID,
    /* [out] */ VARIANT __RPC_FAR *pvaValue);


void __RPC_STUB ICxDataSession_GetAsynObjectCallValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_SetAsynObjectCallValue_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [string][in] */ LPCWSTR szID,
    /* [out] */ VARIANT __RPC_FAR *pvaValue);


void __RPC_STUB ICxDataSession_SetAsynObjectCallValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_SetAsynObjectCallEnable_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pdispObject,
    /* [in] */ BOOL bEnable);


void __RPC_STUB ICxDataSession_SetAsynObjectCallEnable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_InvokeAsynObjectCallAtOnce_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pdispObject);


void __RPC_STUB ICxDataSession_InvokeAsynObjectCallAtOnce_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_SaveTagStatus_Proxy( 
    ICxDataSession __RPC_FAR * This);


void __RPC_STUB ICxDataSession_SaveTagStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICxDataSession_BindObjectsToTagGroup_Proxy( 
    ICxDataSession __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *__RPC_FAR *ppObjects,
    /* [in] */ DWORD dwCount,
    /* [in] */ LPCWSTR szGroup);


void __RPC_STUB ICxDataSession_BindObjectsToTagGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxDataSession_INTERFACE_DEFINED__ */


#ifndef __ICxAutoValue_INTERFACE_DEFINED__
#define __ICxAutoValue_INTERFACE_DEFINED__

/* interface ICxAutoValue */
/* [unique][helpstring][uuid][dual][object] */ 


EXTERN_C const IID IID_ICxAutoValue;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B3C0A6A0-26D2-4ffe-8993-5CC14D570168")
    ICxAutoValue : public IDispatch
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_TimeStamp( 
            /* [retval][out] */ DATE __RPC_FAR *TimeStamp) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DataValue( 
            /* [retval][out] */ VARIANT __RPC_FAR *DataValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxAutoValueVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxAutoValue __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxAutoValue __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxAutoValue __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICxAutoValue __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICxAutoValue __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICxAutoValue __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICxAutoValue __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TimeStamp )( 
            ICxAutoValue __RPC_FAR * This,
            /* [retval][out] */ DATE __RPC_FAR *TimeStamp);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DataValue )( 
            ICxAutoValue __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *DataValue);
        
        END_INTERFACE
    } ICxAutoValueVtbl;

    interface ICxAutoValue
    {
        CONST_VTBL struct ICxAutoValueVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxAutoValue_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxAutoValue_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxAutoValue_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxAutoValue_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICxAutoValue_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICxAutoValue_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICxAutoValue_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICxAutoValue_get_TimeStamp(This,TimeStamp)	\
    (This)->lpVtbl -> get_TimeStamp(This,TimeStamp)

#define ICxAutoValue_get_DataValue(This,DataValue)	\
    (This)->lpVtbl -> get_DataValue(This,DataValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE ICxAutoValue_get_TimeStamp_Proxy( 
    ICxAutoValue __RPC_FAR * This,
    /* [retval][out] */ DATE __RPC_FAR *TimeStamp);


void __RPC_STUB ICxAutoValue_get_TimeStamp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ICxAutoValue_get_DataValue_Proxy( 
    ICxAutoValue __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *DataValue);


void __RPC_STUB ICxAutoValue_get_DataValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxAutoValue_INTERFACE_DEFINED__ */


#ifndef __ICxAutoHistory_INTERFACE_DEFINED__
#define __ICxAutoHistory_INTERFACE_DEFINED__

/* interface ICxAutoHistory */
/* [unique][helpstring][uuid][dual][object] */ 


EXTERN_C const IID IID_ICxAutoHistory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D98019CF-99AD-4a1e-BEC7-F1E2A7CF93FE")
    ICxAutoHistory : public IDispatch
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ LONG __RPC_FAR *Count) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ int Index,
            /* [retval][out] */ ICxAutoValue __RPC_FAR *__RPC_FAR *Value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxAutoHistoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxAutoHistory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxAutoHistory __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxAutoHistory __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICxAutoHistory __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICxAutoHistory __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICxAutoHistory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICxAutoHistory __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            ICxAutoHistory __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *Count);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Item )( 
            ICxAutoHistory __RPC_FAR * This,
            /* [in] */ int Index,
            /* [retval][out] */ ICxAutoValue __RPC_FAR *__RPC_FAR *Value);
        
        END_INTERFACE
    } ICxAutoHistoryVtbl;

    interface ICxAutoHistory
    {
        CONST_VTBL struct ICxAutoHistoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxAutoHistory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxAutoHistory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxAutoHistory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxAutoHistory_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICxAutoHistory_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICxAutoHistory_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICxAutoHistory_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICxAutoHistory_get_Count(This,Count)	\
    (This)->lpVtbl -> get_Count(This,Count)

#define ICxAutoHistory_get_Item(This,Index,Value)	\
    (This)->lpVtbl -> get_Item(This,Index,Value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ICxAutoHistory_get_Count_Proxy( 
    ICxAutoHistory __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *Count);


void __RPC_STUB ICxAutoHistory_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICxAutoHistory_get_Item_Proxy( 
    ICxAutoHistory __RPC_FAR * This,
    /* [in] */ int Index,
    /* [retval][out] */ ICxAutoValue __RPC_FAR *__RPC_FAR *Value);


void __RPC_STUB ICxAutoHistory_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxAutoHistory_INTERFACE_DEFINED__ */


#ifndef __ICxAutoDataSession_INTERFACE_DEFINED__
#define __ICxAutoDataSession_INTERFACE_DEFINED__

/* interface ICxAutoDataSession */
/* [unique][helpstring][uuid][dual][object] */ 


EXTERN_C const IID IID_ICxAutoDataSession;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("278A04DB-60AA-48bc-97A0-EA5FFDCF15F3")
    ICxAutoDataSession : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AutoRequestTag( 
            /* [in] */ BSTR Tag,
            /* [retval][out] */ LONG __RPC_FAR *Cookie) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AutoCommitTags( 
            /* [in] */ BOOL AutoRefresh) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AutoCancelRequestTag( 
            /* [in] */ LONG Cookie) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AutoReadTagHistoryData( 
            /* [in] */ LONG Cookie,
            /* [in] */ BOOL Bounds,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [in] */ LONG NumValues,
            /* [retval][out] */ ICxAutoHistory __RPC_FAR *__RPC_FAR *History) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxAutoDataSessionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxAutoDataSession __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxAutoDataSession __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxAutoDataSession __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICxAutoDataSession __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICxAutoDataSession __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICxAutoDataSession __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICxAutoDataSession __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AutoRequestTag )( 
            ICxAutoDataSession __RPC_FAR * This,
            /* [in] */ BSTR Tag,
            /* [retval][out] */ LONG __RPC_FAR *Cookie);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AutoCommitTags )( 
            ICxAutoDataSession __RPC_FAR * This,
            /* [in] */ BOOL AutoRefresh);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AutoCancelRequestTag )( 
            ICxAutoDataSession __RPC_FAR * This,
            /* [in] */ LONG Cookie);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AutoReadTagHistoryData )( 
            ICxAutoDataSession __RPC_FAR * This,
            /* [in] */ LONG Cookie,
            /* [in] */ BOOL Bounds,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [in] */ LONG NumValues,
            /* [retval][out] */ ICxAutoHistory __RPC_FAR *__RPC_FAR *History);
        
        END_INTERFACE
    } ICxAutoDataSessionVtbl;

    interface ICxAutoDataSession
    {
        CONST_VTBL struct ICxAutoDataSessionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxAutoDataSession_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxAutoDataSession_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxAutoDataSession_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxAutoDataSession_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICxAutoDataSession_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICxAutoDataSession_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICxAutoDataSession_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICxAutoDataSession_AutoRequestTag(This,Tag,Cookie)	\
    (This)->lpVtbl -> AutoRequestTag(This,Tag,Cookie)

#define ICxAutoDataSession_AutoCommitTags(This,AutoRefresh)	\
    (This)->lpVtbl -> AutoCommitTags(This,AutoRefresh)

#define ICxAutoDataSession_AutoCancelRequestTag(This,Cookie)	\
    (This)->lpVtbl -> AutoCancelRequestTag(This,Cookie)

#define ICxAutoDataSession_AutoReadTagHistoryData(This,Cookie,Bounds,StartTime,EndTime,NumValues,History)	\
    (This)->lpVtbl -> AutoReadTagHistoryData(This,Cookie,Bounds,StartTime,EndTime,NumValues,History)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxAutoDataSession_AutoRequestTag_Proxy( 
    ICxAutoDataSession __RPC_FAR * This,
    /* [in] */ BSTR Tag,
    /* [retval][out] */ LONG __RPC_FAR *Cookie);


void __RPC_STUB ICxAutoDataSession_AutoRequestTag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxAutoDataSession_AutoCommitTags_Proxy( 
    ICxAutoDataSession __RPC_FAR * This,
    /* [in] */ BOOL AutoRefresh);


void __RPC_STUB ICxAutoDataSession_AutoCommitTags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxAutoDataSession_AutoCancelRequestTag_Proxy( 
    ICxAutoDataSession __RPC_FAR * This,
    /* [in] */ LONG Cookie);


void __RPC_STUB ICxAutoDataSession_AutoCancelRequestTag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxAutoDataSession_AutoReadTagHistoryData_Proxy( 
    ICxAutoDataSession __RPC_FAR * This,
    /* [in] */ LONG Cookie,
    /* [in] */ BOOL Bounds,
    /* [in] */ DATE StartTime,
    /* [in] */ DATE EndTime,
    /* [in] */ LONG NumValues,
    /* [retval][out] */ ICxAutoHistory __RPC_FAR *__RPC_FAR *History);


void __RPC_STUB ICxAutoDataSession_AutoReadTagHistoryData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxAutoDataSession_INTERFACE_DEFINED__ */


#ifndef __ICxCommomFunctions_INTERFACE_DEFINED__
#define __ICxCommomFunctions_INTERFACE_DEFINED__

/* interface ICxCommomFunctions */
/* [unique][helpstring][uuid][dual][object] */ 


EXTERN_C const IID IID_ICxCommomFunctions;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E6EC9619-741A-4bab-94B4-B2C9AAD9D000")
    ICxCommomFunctions : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DownloadRecipe( 
            /* [in] */ BSTR RecipeName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlaySound( 
            /* [in] */ BSTR Sound,
            /* [optional][in] */ VARIANT Flags,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAlarmCount( 
            /* [optional][in] */ VARIANT Acked,
            /* [optional][in] */ VARIANT AreaList,
            /* [optional][in] */ VARIANT LowPriority,
            /* [optional][in] */ VARIANT HighPriority,
            /* [retval][out] */ long __RPC_FAR *Count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAlarmCountEx( 
            /* [optional][in] */ VARIANT NodeList,
            /* [optional][in] */ VARIANT Acked,
            /* [optional][in] */ VARIANT AreaList,
            /* [optional][in] */ VARIANT LowPriority,
            /* [optional][in] */ VARIANT HighPriority,
            /* [retval][out] */ long __RPC_FAR *Count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AcknowledgeAlarm( 
            BSTR Source) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LogUserOperation( 
            BSTR Source,
            BSTR Message) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LogSOEEvent( 
            BSTR Source,
            BSTR Message,
            /* [optional][in] */ VARIANT Year,
            /* [optional][in] */ VARIANT Month,
            /* [optional][in] */ VARIANT Day,
            /* [optional][in] */ VARIANT Hour,
            /* [optional][in] */ VARIANT Minute,
            /* [optional][in] */ VARIANT Second,
            /* [optional][in] */ VARIANT Millisecond,
            /* [optional][in] */ VARIANT Severity) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExitWindows( 
            /* [optional][in] */ VARIANT Flags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadTag( 
            /* [in] */ BSTR Tag,
            /* [retval][out] */ VARIANT __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteTag( 
            /* [in] */ BSTR Tag,
            /* [in] */ VARIANT Value,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDeviceStatus( 
            BSTR Device,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetDeviceStatus( 
            BSTR Device,
            long State) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveTagStatus( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetSystemTime( 
            /* [optional][in] */ VARIANT Year,
            /* [optional][in] */ VARIANT Month,
            /* [optional][in] */ VARIANT Day,
            /* [optional][in] */ VARIANT Hour,
            /* [optional][in] */ VARIANT Minute,
            /* [optional][in] */ VARIANT Second,
            /* [optional][in] */ VARIANT Millisecond) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICxCommomFunctionsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICxCommomFunctions __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICxCommomFunctions __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICxCommomFunctions __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICxCommomFunctions __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICxCommomFunctions __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICxCommomFunctions __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICxCommomFunctions __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DownloadRecipe )( 
            ICxCommomFunctions __RPC_FAR * This,
            /* [in] */ BSTR RecipeName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PlaySound )( 
            ICxCommomFunctions __RPC_FAR * This,
            /* [in] */ BSTR Sound,
            /* [optional][in] */ VARIANT Flags,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAlarmCount )( 
            ICxCommomFunctions __RPC_FAR * This,
            /* [optional][in] */ VARIANT Acked,
            /* [optional][in] */ VARIANT AreaList,
            /* [optional][in] */ VARIANT LowPriority,
            /* [optional][in] */ VARIANT HighPriority,
            /* [retval][out] */ long __RPC_FAR *Count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAlarmCountEx )( 
            ICxCommomFunctions __RPC_FAR * This,
            /* [optional][in] */ VARIANT NodeList,
            /* [optional][in] */ VARIANT Acked,
            /* [optional][in] */ VARIANT AreaList,
            /* [optional][in] */ VARIANT LowPriority,
            /* [optional][in] */ VARIANT HighPriority,
            /* [retval][out] */ long __RPC_FAR *Count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AcknowledgeAlarm )( 
            ICxCommomFunctions __RPC_FAR * This,
            BSTR Source);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LogUserOperation )( 
            ICxCommomFunctions __RPC_FAR * This,
            BSTR Source,
            BSTR Message);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LogSOEEvent )( 
            ICxCommomFunctions __RPC_FAR * This,
            BSTR Source,
            BSTR Message,
            /* [optional][in] */ VARIANT Year,
            /* [optional][in] */ VARIANT Month,
            /* [optional][in] */ VARIANT Day,
            /* [optional][in] */ VARIANT Hour,
            /* [optional][in] */ VARIANT Minute,
            /* [optional][in] */ VARIANT Second,
            /* [optional][in] */ VARIANT Millisecond,
            /* [optional][in] */ VARIANT Severity);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ExitWindows )( 
            ICxCommomFunctions __RPC_FAR * This,
            /* [optional][in] */ VARIANT Flags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadTag )( 
            ICxCommomFunctions __RPC_FAR * This,
            /* [in] */ BSTR Tag,
            /* [retval][out] */ VARIANT __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteTag )( 
            ICxCommomFunctions __RPC_FAR * This,
            /* [in] */ BSTR Tag,
            /* [in] */ VARIANT Value,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDeviceStatus )( 
            ICxCommomFunctions __RPC_FAR * This,
            BSTR Device,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDeviceStatus )( 
            ICxCommomFunctions __RPC_FAR * This,
            BSTR Device,
            long State);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveTagStatus )( 
            ICxCommomFunctions __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSystemTime )( 
            ICxCommomFunctions __RPC_FAR * This,
            /* [optional][in] */ VARIANT Year,
            /* [optional][in] */ VARIANT Month,
            /* [optional][in] */ VARIANT Day,
            /* [optional][in] */ VARIANT Hour,
            /* [optional][in] */ VARIANT Minute,
            /* [optional][in] */ VARIANT Second,
            /* [optional][in] */ VARIANT Millisecond);
        
        END_INTERFACE
    } ICxCommomFunctionsVtbl;

    interface ICxCommomFunctions
    {
        CONST_VTBL struct ICxCommomFunctionsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICxCommomFunctions_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICxCommomFunctions_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICxCommomFunctions_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICxCommomFunctions_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICxCommomFunctions_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICxCommomFunctions_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICxCommomFunctions_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICxCommomFunctions_DownloadRecipe(This,RecipeName)	\
    (This)->lpVtbl -> DownloadRecipe(This,RecipeName)

#define ICxCommomFunctions_PlaySound(This,Sound,Flags,pVal)	\
    (This)->lpVtbl -> PlaySound(This,Sound,Flags,pVal)

#define ICxCommomFunctions_GetAlarmCount(This,Acked,AreaList,LowPriority,HighPriority,Count)	\
    (This)->lpVtbl -> GetAlarmCount(This,Acked,AreaList,LowPriority,HighPriority,Count)

#define ICxCommomFunctions_GetAlarmCountEx(This,NodeList,Acked,AreaList,LowPriority,HighPriority,Count)	\
    (This)->lpVtbl -> GetAlarmCountEx(This,NodeList,Acked,AreaList,LowPriority,HighPriority,Count)

#define ICxCommomFunctions_AcknowledgeAlarm(This,Source)	\
    (This)->lpVtbl -> AcknowledgeAlarm(This,Source)

#define ICxCommomFunctions_LogUserOperation(This,Source,Message)	\
    (This)->lpVtbl -> LogUserOperation(This,Source,Message)

#define ICxCommomFunctions_LogSOEEvent(This,Source,Message,Year,Month,Day,Hour,Minute,Second,Millisecond,Severity)	\
    (This)->lpVtbl -> LogSOEEvent(This,Source,Message,Year,Month,Day,Hour,Minute,Second,Millisecond,Severity)

#define ICxCommomFunctions_ExitWindows(This,Flags)	\
    (This)->lpVtbl -> ExitWindows(This,Flags)

#define ICxCommomFunctions_ReadTag(This,Tag,pVal)	\
    (This)->lpVtbl -> ReadTag(This,Tag,pVal)

#define ICxCommomFunctions_WriteTag(This,Tag,Value,pVal)	\
    (This)->lpVtbl -> WriteTag(This,Tag,Value,pVal)

#define ICxCommomFunctions_GetDeviceStatus(This,Device,pVal)	\
    (This)->lpVtbl -> GetDeviceStatus(This,Device,pVal)

#define ICxCommomFunctions_SetDeviceStatus(This,Device,State)	\
    (This)->lpVtbl -> SetDeviceStatus(This,Device,State)

#define ICxCommomFunctions_SaveTagStatus(This)	\
    (This)->lpVtbl -> SaveTagStatus(This)

#define ICxCommomFunctions_SetSystemTime(This,Year,Month,Day,Hour,Minute,Second,Millisecond)	\
    (This)->lpVtbl -> SetSystemTime(This,Year,Month,Day,Hour,Minute,Second,Millisecond)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxCommomFunctions_DownloadRecipe_Proxy( 
    ICxCommomFunctions __RPC_FAR * This,
    /* [in] */ BSTR RecipeName);


void __RPC_STUB ICxCommomFunctions_DownloadRecipe_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxCommomFunctions_PlaySound_Proxy( 
    ICxCommomFunctions __RPC_FAR * This,
    /* [in] */ BSTR Sound,
    /* [optional][in] */ VARIANT Flags,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ICxCommomFunctions_PlaySound_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxCommomFunctions_GetAlarmCount_Proxy( 
    ICxCommomFunctions __RPC_FAR * This,
    /* [optional][in] */ VARIANT Acked,
    /* [optional][in] */ VARIANT AreaList,
    /* [optional][in] */ VARIANT LowPriority,
    /* [optional][in] */ VARIANT HighPriority,
    /* [retval][out] */ long __RPC_FAR *Count);


void __RPC_STUB ICxCommomFunctions_GetAlarmCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxCommomFunctions_GetAlarmCountEx_Proxy( 
    ICxCommomFunctions __RPC_FAR * This,
    /* [optional][in] */ VARIANT NodeList,
    /* [optional][in] */ VARIANT Acked,
    /* [optional][in] */ VARIANT AreaList,
    /* [optional][in] */ VARIANT LowPriority,
    /* [optional][in] */ VARIANT HighPriority,
    /* [retval][out] */ long __RPC_FAR *Count);


void __RPC_STUB ICxCommomFunctions_GetAlarmCountEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxCommomFunctions_AcknowledgeAlarm_Proxy( 
    ICxCommomFunctions __RPC_FAR * This,
    BSTR Source);


void __RPC_STUB ICxCommomFunctions_AcknowledgeAlarm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxCommomFunctions_LogUserOperation_Proxy( 
    ICxCommomFunctions __RPC_FAR * This,
    BSTR Source,
    BSTR Message);


void __RPC_STUB ICxCommomFunctions_LogUserOperation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxCommomFunctions_LogSOEEvent_Proxy( 
    ICxCommomFunctions __RPC_FAR * This,
    BSTR Source,
    BSTR Message,
    /* [optional][in] */ VARIANT Year,
    /* [optional][in] */ VARIANT Month,
    /* [optional][in] */ VARIANT Day,
    /* [optional][in] */ VARIANT Hour,
    /* [optional][in] */ VARIANT Minute,
    /* [optional][in] */ VARIANT Second,
    /* [optional][in] */ VARIANT Millisecond,
    /* [optional][in] */ VARIANT Severity);


void __RPC_STUB ICxCommomFunctions_LogSOEEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxCommomFunctions_ExitWindows_Proxy( 
    ICxCommomFunctions __RPC_FAR * This,
    /* [optional][in] */ VARIANT Flags);


void __RPC_STUB ICxCommomFunctions_ExitWindows_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxCommomFunctions_ReadTag_Proxy( 
    ICxCommomFunctions __RPC_FAR * This,
    /* [in] */ BSTR Tag,
    /* [retval][out] */ VARIANT __RPC_FAR *pVal);


void __RPC_STUB ICxCommomFunctions_ReadTag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxCommomFunctions_WriteTag_Proxy( 
    ICxCommomFunctions __RPC_FAR * This,
    /* [in] */ BSTR Tag,
    /* [in] */ VARIANT Value,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ICxCommomFunctions_WriteTag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxCommomFunctions_GetDeviceStatus_Proxy( 
    ICxCommomFunctions __RPC_FAR * This,
    BSTR Device,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICxCommomFunctions_GetDeviceStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxCommomFunctions_SetDeviceStatus_Proxy( 
    ICxCommomFunctions __RPC_FAR * This,
    BSTR Device,
    long State);


void __RPC_STUB ICxCommomFunctions_SetDeviceStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxCommomFunctions_SaveTagStatus_Proxy( 
    ICxCommomFunctions __RPC_FAR * This);


void __RPC_STUB ICxCommomFunctions_SaveTagStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICxCommomFunctions_SetSystemTime_Proxy( 
    ICxCommomFunctions __RPC_FAR * This,
    /* [optional][in] */ VARIANT Year,
    /* [optional][in] */ VARIANT Month,
    /* [optional][in] */ VARIANT Day,
    /* [optional][in] */ VARIANT Hour,
    /* [optional][in] */ VARIANT Minute,
    /* [optional][in] */ VARIANT Second,
    /* [optional][in] */ VARIANT Millisecond);


void __RPC_STUB ICxCommomFunctions_SetSystemTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICxCommomFunctions_INTERFACE_DEFINED__ */



#ifndef __CXDATSESLib_LIBRARY_DEFINED__
#define __CXDATSESLib_LIBRARY_DEFINED__

/* library CXDATSESLib */
/* [helpstring][version][uuid] */ 





EXTERN_C const IID LIBID_CXDATSESLib;

EXTERN_C const CLSID CLSID_CxDataSession;

#ifdef __cplusplus

class DECLSPEC_UUID("8E7CEF47-487F-44B6-870A-977EEEADE956")
CxDataSession;
#endif
#endif /* __CXDATSESLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  HWND_UserSize(     unsigned long __RPC_FAR *, unsigned long            , HWND __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  HWND_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, HWND __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  HWND_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, HWND __RPC_FAR * ); 
void                      __RPC_USER  HWND_UserFree(     unsigned long __RPC_FAR *, HWND __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
