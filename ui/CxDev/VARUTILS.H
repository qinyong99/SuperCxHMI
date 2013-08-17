const int VTI_EMPTY = 0;
const int VTI_BOOL = 1;
const int VTI_UI1 = 2;
const int VTI_UI2 = 3;
const int VTI_UI4 = 4;
const int VTI_UI8 = 5;
const int VTI_I1 = 6;
const int VTI_I2 = 7;
const int VTI_I4 = 8;
const int VTI_I8 = 9;
const int VTI_R4 = 10;
const int VTI_R8 = 11;
const int VTI_BSTR = 12;
const int VTI_CY = 13;
const int VTI_DATE = 14;
const int VTI_ERROR = 15;
const int VTI_COLOR = 16;
const int VTI_FONT = 17;
const int VTI_UNKNOWN = 18;
const int NUM_VARTYPES = 19;

const VARIANT_BOOL VAR_TRUE = ~VARIANT_FALSE;
const VARIANT_BOOL VAR_FALSE = VARIANT_FALSE;

LPCTSTR InvokeKindToString( INVOKEKIND invokekind );
LPCTSTR VTIToString( int iType );
VARTYPE VTIToVT( int iType );
LPCTSTR VTToString( VARTYPE vt );
int VTToVTI( VARTYPE vt );

CString VariantToString( const VARIANT& var );

//#define ARSHIFT(a, b) ((a) >> (b))  
//#define MAX(a, b) ((a) > (b) ? (a) : (b)) 
//#define MIN(a, b) ((a) < (b) ? (a) : (b)) 
//#define MIN_MAX(a, l, h) ((a) < (l) ? (l) : ((a) > (h) ? (h) : (a))) 
//#define ABS(a) ((a) < 0 ? (-(a)) : (a)) 
//#define ROUND(f) (I32)((f)>=0? f + .4999999 : f - .4999999 ) 
//#define DIV2(x)     ((x)>0?(x)>>1:-(-(x))>>1) 
//#define DIV_ROUND(x,d)     ((x)>0?((x)+(d)/2)/(d):-(((-(x))+(d)/2)/(d))) 
//#define ROUND_SHIFT(a,shift) ( (a)>0?((a)+(1<<((shift)-1)))>>(shift):-((-(a)+(1<<((shift)-1)))>>(shift)))  
