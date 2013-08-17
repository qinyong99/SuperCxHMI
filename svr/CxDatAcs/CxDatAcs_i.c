/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Dec 12 17:18:21 2008
 */
/* Compiler settings for D:\WorkDir1\CxDatAcs\CxDatAcs.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
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

const IID IID_ICxMutiDataAccess = {0xA947C3CB,0x33C9,0x407c,{0xB7,0x52,0x55,0x1D,0x94,0x17,0xDC,0xF8}};


const IID IID_ICxDataAccess = {0x16D09780,0x1548,0x4D76,{0x85,0x8C,0xBB,0x6A,0xE1,0x4D,0xCA,0x29}};


const IID IID_ICxDataRecord = {0x7E956574,0xEBEB,0x4A98,{0xA9,0x33,0x0B,0xA1,0x81,0x0B,0x71,0xCD}};


const IID LIBID_CXDATACSLib = {0x511B285E,0x998E,0x4319,{0xB6,0xF9,0xA6,0x01,0x43,0x61,0x44,0xA7}};


#ifdef __cplusplus
}
#endif

