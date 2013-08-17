// DialogSizeHelper.h: interface for the CDialogSizeHelper class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DIALOGSIZEHELPER_H__
#define _DIALOGSIZEHELPER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDialogSizeHelper : _DialogSizeHelper
{
public:
	static void GetDialogSize2(const DLGTEMPLATE* pTemplate, SIZE* pSize)
	{
		// If the dialog has a font we use it otherwise we default
		// to the system font.
		if (HasFont(pTemplate))
		{
			TCHAR szFace[LF_FACESIZE];
			WORD  wFontSize = 0;
			GetFont(pTemplate, szFace, &wFontSize);
			GetSizeInDialogUnits(pTemplate, pSize);
			ConvertDialogUnitsToPixels2(szFace, wFontSize, pSize);
		}
		else
		{
			GetSizeInDialogUnits(pTemplate, pSize);
			LONG nDlgBaseUnits = GetDialogBaseUnits();
			pSize->cx = MulDiv(pSize->cx, LOWORD(nDlgBaseUnits), 4);
			pSize->cy = MulDiv(pSize->cy, HIWORD(nDlgBaseUnits), 8);
		}
	}
	static void ConvertDialogUnitsToPixels2(LPCTSTR pszFontFace, WORD wFontSize, SIZE* pSizePixel)
	{
		// Attempt to create the font to be used in the dialog box
		UINT cxSysChar, cySysChar;
		LOGFONT lf;
		HDC hDC = ::GetDC(NULL);
		int cxDlg = pSizePixel->cx;
		int cyDlg = pSizePixel->cy;
		
		ZeroMemory(&lf, sizeof(LOGFONT));
		lf.lfHeight = -MulDiv(wFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
		lf.lfWeight = FW_NORMAL;
		lf.lfCharSet = DEFAULT_CHARSET;
		lstrcpy(lf.lfFaceName, pszFontFace);
		
		HFONT hNewFont = CreateFontIndirect(&lf);
		if (hNewFont != NULL)
		{
			TEXTMETRIC  tm;
			SIZE        size;
			HFONT       hFontOld = (HFONT)SelectObject(hDC, hNewFont);
			GetTextMetrics(hDC, &tm);
			cySysChar = tm.tmHeight/* + tm.tmExternalLeading*/;
			::GetTextExtentPoint(hDC,
				_T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), 52,
				&size);
			cxSysChar = (size.cx + 26) / 52;
			SelectObject(hDC, hFontOld);
			DeleteObject(hNewFont);
		}
		else
		{
			// Could not create the font so just use the system's values
			cxSysChar = LOWORD(GetDialogBaseUnits());
			cySysChar = HIWORD(GetDialogBaseUnits());
		}
		::ReleaseDC(NULL, hDC);
		
		// Translate dialog units to pixels
		pSizePixel->cx = MulDiv(cxDlg, cxSysChar, 4);
		pSizePixel->cy = MulDiv(cyDlg, cySysChar, 8);
	}

};

#endif // _DIALOGSIZEHELPER_H__
