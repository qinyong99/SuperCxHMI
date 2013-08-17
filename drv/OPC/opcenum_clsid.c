//
// (c) Copyright 1998 The OPC Foundation
// ALL RIGHTS RESERVED.
//
//
// CONTENTS:
//	Definition of the global class ID for the OPC Server Lister (OPCENUM.EXE)
//
// Modification Log:
//	Vers    Date   By    Notes
//	----  -------- ---   -----
//  2.0   10/02/98 acc   
//
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


const CLSID CLSID_OPCServerList = {0x13486D51,0x4821,0x11D2,{0xA4,0x94,0x3C,0xB3,0x06,0xC1,0x00,0x00}};


#ifdef __cplusplus
}
#endif

