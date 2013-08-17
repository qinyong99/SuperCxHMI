// 1/8/99 - acc - Fix Server20 CATID to have proper value

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __GUID_DEFINED__
#define __GUID_DEFINED__

typedef struct _GUID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} GUID;

#endif // __GUID_DEFINED__

const GUID CATID_OPCDAServer10 = 
{ 0x63d5f430, 0xcfe4, 0x11d1, { 0xb2, 0xc8, 0x0, 0x60, 0x8, 0x3b, 0xa1, 0xfb } };
// {63D5F430-CFE4-11d1-B2C8-0060083BA1FB}

const GUID CATID_OPCDAServer20 = 
{ 0x63d5f432, 0xcfe4, 0x11d1, { 0xb2, 0xc8, 0x0, 0x60, 0x8, 0x3b, 0xa1, 0xfb } };
// {63D5F432-CFE4-11d1-B2C8-0060083BA1FB}


#ifdef __cplusplus
}
#endif
