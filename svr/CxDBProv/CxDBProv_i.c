/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Aug 11 11:02:41 2013
 */
/* Compiler settings for D:\spcx\svr\CxDBProv\CxDBProv.idl:
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

const IID IID_ICxDataSource = {0x8019A860,0x86A3,0x4553,{0xB8,0x61,0xEC,0x24,0xD1,0x3E,0xF9,0xE3}};


const IID LIBID_CXDBPROVLib = {0x46430F2A,0xFCD1,0x4991,{0x92,0xA6,0x7B,0x87,0xA0,0x6E,0x25,0x9E}};


const CLSID CLSID_HistoryData = {0xF8CA6B76,0xDB06,0x4D83,{0xB8,0x62,0xDE,0x7B,0xBC,0xE1,0x78,0x9F}};


const CLSID CLSID_DataConnectionPage = {0xD470437F,0x5131,0x40E1,{0xAF,0xFE,0x9B,0x41,0xA3,0x1D,0xAF,0x56}};


const CLSID CLSID_RealtimeData = {0x29E5101C,0x58AA,0x41D3,{0xB3,0x6D,0xAC,0x98,0x09,0x3D,0x17,0x54}};


#ifdef __cplusplus
}
#endif

