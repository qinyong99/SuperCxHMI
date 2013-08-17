

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


// {7DE5B060-E089-11d2-A5E6-000086339399}
const GUID CATID_OPCHDAServer10 = 
{ 0x7de5b060, 0xe089, 0x11d2, { 0xa5, 0xe6, 0x0, 0x0, 0x86, 0x33, 0x93, 0x99 } };



#ifdef __cplusplus

}

#endif

