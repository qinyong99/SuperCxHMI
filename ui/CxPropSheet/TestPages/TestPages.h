/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Oct 26 17:45:23 2001
 */
/* Compiler settings for D:\WorkDir\CxPropSheet\TestPages\TestPages.idl:
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

#ifndef __TestPages_h__
#define __TestPages_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __TestPage1_FWD_DEFINED__
#define __TestPage1_FWD_DEFINED__

#ifdef __cplusplus
typedef class TestPage1 TestPage1;
#else
typedef struct TestPage1 TestPage1;
#endif /* __cplusplus */

#endif 	/* __TestPage1_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __TESTPAGESLib_LIBRARY_DEFINED__
#define __TESTPAGESLib_LIBRARY_DEFINED__

/* library TESTPAGESLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_TESTPAGESLib;

EXTERN_C const CLSID CLSID_TestPage1;

#ifdef __cplusplus

class DECLSPEC_UUID("4081C3E5-DC50-497D-9711-28C0A3DAA0D7")
TestPage1;
#endif
#endif /* __TESTPAGESLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
