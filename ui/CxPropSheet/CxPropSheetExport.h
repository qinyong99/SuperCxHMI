#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )

DllImport HRESULT CxCreatePropertyFrame(
  HWND hwndOwner,    //Parent window of property sheet dialog box
  UINT x,            //Horizontal position for dialog box
  UINT y,            //Vertical position for dialog box
  LPCOLESTR lpszCaption,
                     //Pointer to the dialog box caption
  ULONG cObjects,    //Number of object pointers in lplpUnk
  LPUNKNOWN FAR* lplpUnk,
                     //Pointer to the objects for property sheet
  ULONG cPages,      //Number of property pages in lpPageClsID
  LPCLSID lpPageClsID,
                     //Array of CLSIDs for each property page
  LCID lcid,         //Locale identifier for property sheet locale
  DWORD dwReserved,  //Reserved
  LPVOID lpvReserved //Reserved
);

