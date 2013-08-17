/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Jan 21 13:34:15 2000
 */
/* Compiler settings for opcbc.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, app_config, c_ext
    error checks: allocation ref bounds_check enum stub_data 
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

const IID IID_IOPCBatchServer = {0x8BB4ED50,0xB314,0x11d3,{0xB3,0xEA,0x00,0xC0,0x4F,0x8E,0xCE,0xAA}};


const IID IID_IEnumOPCBatchSummary = {0xa8080da2,0xe23e,0x11d2,{0xaf,0xa7,0x00,0xc0,0x4f,0x53,0x94,0x21}};


const IID IID_IOPCEnumerationSets = {0xa8080da3,0xe23e,0x11d2,{0xaf,0xa7,0x00,0xc0,0x4f,0x53,0x94,0x21}};


const IID LIBID_OPC_BATCH = {0xa8080da4,0xe23e,0x11d2,{0xaf,0xa7,0x00,0xc0,0x4f,0x53,0x94,0x21}};


#ifdef __cplusplus
}
#endif

