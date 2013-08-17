/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 3.01.75 */
/* at Mon Aug 10 18:36:09 1998
 */
/* Compiler settings for opc_ae.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IOPCEventServer = {0x65168851,0x5783,0x11D1,{0x84,0xA0,0x00,0x60,0x8C,0xB8,0xA7,0xE9}};


const IID IID_IOPCEventSubscriptionMgt = {0x65168855,0x5783,0x11D1,{0x84,0xA0,0x00,0x60,0x8C,0xB8,0xA7,0xE9}};


const IID IID_IOPCEventAreaBrowser = {0x65168857,0x5783,0x11D1,{0x84,0xA0,0x00,0x60,0x8C,0xB8,0xA7,0xE9}};


const IID IID_IOPCEventSink = {0x6516885F,0x5783,0x11D1,{0x84,0xA0,0x00,0x60,0x8C,0xB8,0xA7,0xE9}};


const IID LIBID_OPC_AE = {0x65168844,0x5783,0x11D1,{0x84,0xA0,0x00,0x60,0x8C,0xB8,0xA7,0xE9}};


const IID IID_OPCEventServerCATID = {0x58E13251,0xAC87,0x11d1,{0x84,0xD5,0x00,0x60,0x8C,0xB8,0xA7,0xE9}};


#ifdef __cplusplus
}
#endif

