// FontComboBox.cpp: implementation of the CFontComboBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FontComboBox.h"

void _InitMaskFromBitmap(CBitmap* pbmp, CBitmap* pbmpMask)
{
	BITMAP bmp;
	pbmp->GetBitmap(bmp);
	pbmpMask->CreateBitmap(bmp.bmWidth, bmp.bmHeight, 1, 1, NULL);

	CDC dcDst;
	dcDst.CreateCompatibleDC(NULL);
	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);
	HBITMAP hOldDst = dcDst.SelectBitmap(pbmpMask->m_hBitmap);
	HBITMAP hOldSrc = dcSrc.SelectBitmap(pbmp->m_hBitmap);

	COLORREF oldBkColor = dcSrc.SetBkColor(dcSrc.GetPixel(0, 0));
	dcDst.BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, dcSrc, 0, 0, NOTSRCCOPY);
	dcSrc.SetBkColor(oldBkColor);

	dcDst.SelectBitmap(hOldDst);
	dcSrc.SelectBitmap(hOldSrc);
}

#define DSx     0x00660046L
#define DSna    0x00220326L

void _DrawMaskedBitmap(CDCHandle& dc, CBitmap* pbmp, CBitmap* pbmpMask,
	int x, int y, int cx, int cy)
{
	COLORREF oldBkColor = dc.SetBkColor(RGB(255, 255, 255));
	COLORREF oldTextColor = dc.SetTextColor(RGB(0, 0, 0));

	CDC dcCompat;
	dcCompat.CreateCompatibleDC(dc);
	HBITMAP hBmpSave = dcCompat.SelectBitmap(pbmp->m_hBitmap);
	dc.BitBlt(x, y, cx, cy, dcCompat, 0, 0, DSx);
	dcCompat.SelectBitmap(pbmpMask->m_hBitmap);
	dc.BitBlt(x, y, cx, cy, dcCompat, 0, 0, DSna);
	dcCompat.SelectBitmap(pbmp->m_hBitmap);
	dc.BitBlt(x, y, cx, cy, dcCompat, 0, 0, DSx);
	dcCompat.SelectBitmap(hBmpSave);

	dc.SetBkColor(oldBkColor);
	dc.SetTextColor(oldTextColor);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFontComboBox::CFontComboBox()
{

}

CFontComboBox::~CFontComboBox()
{

}
