// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif

#include <atlimpl.cpp>

/////////////////////////////////////////////////////////////////////////////////////////////////
// Emboss         - Creates a 3D embossed effect
// Taken directly from an article by Zafir Anjum entitled
// "Emboss text and other shape on your bitmap"
/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Returns                - A new bitmap containing the resulting effect
// hBitmap                - Bitmap that contains the basic text & shapes
// hbmBackGnd             - Contains the color image 
// hPal                   - Handle of palette associated with hbmBackGnd
// bRaised                - True if raised effect is desired. False for sunken effect
// xDest          - x coordinate - used to offset hBitmap
// yDest          - y coordinate - used to offset hBitmap
// clrHightlight  - Color used for the highlight edge
// clrShadow              - Color used for the shadow
//
// Note                   - 1. Neither of the bitmap handles passed in should be selected 
//                          in a device context.
//                          2. The pixel at 0,0 in hBitmap is considered the background color
//
/////////////////////////////////////////////////////////////////////////////////////////////////

static HBITMAP Emboss(HBITMAP hBitmap, HBITMAP hbmBackGnd, HPALETTE hPal, BOOL bRaised, int xDest, int yDest, COLORREF clrHighlight, COLORREF clrShadow )
{
    const DWORD PSDPxax = 0x00B8074A;
    BITMAP   bmInfo;
    HBITMAP  hbmOld, hbmShadow, hbmHighlight, hbmResult, hbmOldMem;
    HBRUSH   hbrPat;
    HDC      hDC, hColorDC, hMonoDC, hMemDC;

    if (!bRaised)
    {
        // Swap the highlight and shadow color
        COLORREF clrTemp = clrShadow;
        clrShadow = clrHighlight;
        clrHighlight = clrTemp;
    }

    // We create two monochrome bitmaps. One of them will contain the
    // highlighted edge and the other will contain the shadow. These
    // bitmaps are then used to paint the highlight and shadow on the
    // background image.

    hbmResult = NULL ;
    hDC = GetDC(NULL) ;

    // Create a compatible DCs
    hMemDC = ::CreateCompatibleDC(hDC);
    hMonoDC	= ::CreateCompatibleDC(hDC);
    hColorDC = ::CreateCompatibleDC(hDC);

    if (hMemDC == NULL || hMonoDC == NULL || hColorDC == NULL)
    {
            if (hMemDC) 
				DeleteDC(hMemDC);
            if (hMonoDC) 
				DeleteDC(hMonoDC);
            if (hColorDC) 
				DeleteDC(hColorDC);

            return NULL;
    }

    // Select the background image into memory DC so that we can draw it
    hbmOldMem = (HBITMAP)::SelectObject(hMemDC, hbmBackGnd);
    
    // Get dimensions of the background image
    BITMAP bm;
    ::GetObject(hbmBackGnd, sizeof(bm), &bm);


    // Create the monochrome and compatible color bitmaps 
    GetObject(hBitmap, sizeof(BITMAP), (LPSTR) &bmInfo) ;
    hbmShadow		= CreateBitmap(bmInfo.bmWidth, bmInfo.bmHeight, 1, 1, NULL) ;
    hbmHighlight	= CreateBitmap(bmInfo.bmWidth, bmInfo.bmHeight, 1, 1, NULL) ;
    hbmResult		= CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight) ;

    hbmOld = (HBITMAP)SelectObject(hColorDC, hBitmap) ;

    // Set background color of bitmap for mono conversion
    // We assume that the pixel in the top left corner has the background color
    SetBkColor(hColorDC, GetPixel(hColorDC, 0, 0)) ;

    // Create the highlight bitmap.
    hbmHighlight = (HBITMAP)SelectObject(hMonoDC, (HGDIOBJ) hbmHighlight);
    PatBlt(hMonoDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, WHITENESS);
    BitBlt(hMonoDC, 0, 0, bmInfo.bmWidth - 1, bmInfo.bmHeight - 1, hColorDC, 1, 1, SRCCOPY);
    BitBlt(hMonoDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, hColorDC, 0, 0, MERGEPAINT);
    hbmHighlight = (HBITMAP)SelectObject(hMonoDC, (HGDIOBJ) hbmHighlight);


    // create the shadow bitmap
    hbmShadow = (HBITMAP)SelectObject(hMonoDC, (HGDIOBJ) hbmShadow);
    PatBlt(hMonoDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, WHITENESS) ;
    BitBlt(hMonoDC, 1, 1, bmInfo.bmWidth-1, bmInfo.bmHeight-1, hColorDC, 0, 0, SRCCOPY);
    BitBlt(hMonoDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, hColorDC, 0, 0, MERGEPAINT);
    hbmShadow = (HBITMAP)SelectObject(hMonoDC, (HGDIOBJ) hbmShadow);


    // Now let's start working on the final image
    SelectObject(hColorDC, hbmResult);
    // Select and realize the palette if one is supplied
    if (hPal && GetDeviceCaps(hDC, RASTERCAPS) & RC_PALETTE)
    {
		::SelectPalette(hColorDC, hPal, FALSE);
		::RealizePalette(hColorDC);
    }

    // Draw the background image
    BitBlt(hColorDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0,SRCCOPY);

    // Restore the old bitmap in the hMemDC
    ::SelectObject(hMemDC, hbmOldMem);


    // Set the background and foreground color for the raster operations
    SetBkColor(hColorDC, RGB(255,255,255));
    SetTextColor(hColorDC, RGB(0,0,0));

    // blt the highlight edge
    hbrPat = CreateSolidBrush(clrHighlight) ;
    hbrPat = (HBRUSH)SelectObject(hColorDC, hbrPat) ;
    hbmHighlight = (HBITMAP)SelectObject(hMonoDC, (HGDIOBJ) hbmHighlight);
    BitBlt(hColorDC, xDest, yDest, bmInfo.bmWidth, bmInfo.bmHeight,
            hMonoDC, 0, 0, PSDPxax);
    DeleteObject(SelectObject(hColorDC, hbrPat));
    hbmHighlight = (HBITMAP)SelectObject(hMonoDC, (HGDIOBJ) hbmHighlight);

    // blt the shadow edge
    hbrPat = CreateSolidBrush(clrShadow);

    hbrPat = (HBRUSH)SelectObject(hColorDC, hbrPat) ;
    hbmShadow = (HBITMAP)SelectObject( hMonoDC, (HGDIOBJ) hbmShadow);
    BitBlt( hColorDC, xDest, yDest, bmInfo.bmWidth, bmInfo.bmHeight,
            hMonoDC, 0, 0, PSDPxax) ;
    DeleteObject(SelectObject(hColorDC, hbrPat)) ;
    hbmShadow = (HBITMAP)SelectObject(hMonoDC, (HGDIOBJ) hbmShadow);

    // select old bitmap into color DC 
    SelectObject(hColorDC, hbmOld);

    DeleteObject((HGDIOBJ)hbmShadow);
    DeleteObject((HGDIOBJ)hbmHighlight) ;

//	ReleaseDC( NULL, hDC ) ;

	// Clean up.
	DeleteDC((HDC)hColorDC);
	DeleteDC((HDC)hMonoDC);
	DeleteDC((HDC)hMemDC);
	ReleaseDC(NULL, hDC);

    return hbmResult;
}

BOOL DrawPicture(HDC hDC, RECT rect, IPictureDisp* pPictureDisp, COLORREF clrBack, BOOL bDisable)
{
	if (pPictureDisp)
	{
		CComPtr<IPicture> pPicture;
		pPictureDisp->QueryInterface(IID_IPicture, (void**)&pPicture);
		HBITMAP hBitmap;
		pPicture->get_Handle((OLE_HANDLE FAR *)&hBitmap);

		BITMAP bm;
		GetObject(hBitmap, sizeof(bm), &bm);

		HDC hMemDC;
		hMemDC = CreateCompatibleDC(hDC);

		if (!bDisable)
		{
			HBITMAP hOldBitmap =  (HBITMAP)SelectObject(hMemDC, hBitmap);
			StretchBlt(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 
				hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			SelectObject(hMemDC, hOldBitmap);
		}
		else
		{
			HBITMAP hbmBack = CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight);
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbmBack);
			
			RECT rcTemp;
			rcTemp.left = rcTemp.top = 0;
			rcTemp.right = bm.bmWidth;
			rcTemp.bottom = bm.bmHeight;
		//	::SetBkColor(hMemDC, GetPixel(hDC, rect.left, rect.top));
			::SetBkColor(hMemDC, clrBack);
		//	::SetBkColor(hMemDC, GetSysColor(COLOR_3DFACE));
			::ExtTextOut(hMemDC, 0, 0, ETO_OPAQUE, &rcTemp, NULL, 0, NULL);
			
			// µÃµ½µñ¿ÌÎ»Í¼
			SelectObject(hMemDC, hOldBitmap);
			HBITMAP hbmEmboss = Emboss(hBitmap, hbmBack, NULL, FALSE, 0, 0,
				GetSysColor(COLOR_3DHILIGHT),
				GetSysColor(COLOR_3DSHADOW));
			
			SelectObject(hMemDC, hbmEmboss);
			StretchBlt(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 
				hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			
			SelectObject(hMemDC, hOldBitmap);
			DeleteObject(hbmBack);
			DeleteObject(hbmEmboss);
		}
	
		DeleteDC(hMemDC);

		return TRUE;
	}
	
	return FALSE;
}
