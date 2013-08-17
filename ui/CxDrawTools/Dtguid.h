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

EXTERN_C const CLSID CLSID_DrawLine;
EXTERN_C const CLSID CLSID_DrawEllipse;
EXTERN_C const CLSID CLSID_DrawRectangle;
EXTERN_C const CLSID CLSID_DrawRoundRectangle;
EXTERN_C const CLSID CLSID_DrawButton;
EXTERN_C const CLSID CLSID_DrawChart;
EXTERN_C const CLSID CLSID_DrawAlmSum;
EXTERN_C const CLSID CLSID_DrawDrawArc;
EXTERN_C const CLSID CLSID_DrawFloatPoint;
EXTERN_C const CLSID CLSID_DrawVariable;
EXTERN_C const CLSID CLSID_DrawTimer;
EXTERN_C const CLSID CLSID_DrawMenu;


#ifdef __cplusplus
}
#endif
