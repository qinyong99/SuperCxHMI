// DropPalette.h: interface for the CDropPalette class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DROPPALETTE_H__
#define _DROPPALETTE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorCell.h"

const int FO_TOTAL_CELLS = 144;

class CDropPalette  
{
public:
	CDropPalette();
	virtual ~CDropPalette();

	void Create(CWindow*pWnd, COLORREF crDefault, CRect &rcPos,BOOL bPopup = FALSE);
	void InitColorPalette();

	void AdjustCells(CRect rcPos);
	void ComputeColors();
	int GetAngleFromPoint(int nX, int nY);
	
	void OnDraw(CDCHandle dc);

	void UpdateAll();

	COLORREF GetRGB() const	{return m_crCurColor; }
	void SetRGB(COLORREF ref);

	void SetHLS (double hue,double luminance, double saturation);
	void GetHLS (double *hue,double *luminance, double *saturation);

	CRect GetOldColorRect();

	virtual CColorCell* HitTest(CPoint point, BOOL& bChange);

	BOOL IsPopupStyle() const		{ return m_bPopup; }
	void SetPopupStyle(const BOOL bPopup)	{ m_bPopup = bPopup; }

	void OnSelectOK(WPARAM wParam, LPARAM lParam);
	void OnSelectCancel(WPARAM wParam, LPARAM lParam);
	
public:
	void SetWnd(CWindow *pWnd) { m_pParent = pWnd; }

	void Invalidate(BOOL bRedraw = FALSE);
	void InvalRect(CRect rcPos);
	
	// Capture the mouse.
	void CaptureMouse(BOOL bCapture);
	
	// WM_CHAR message.
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	// WM_LBUTTONDOWN message.
	void OnLButtonDown(UINT nFlags, CPoint point); 
	
	// WM_LBUTTONDBLCLK message.
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	
	// WM_RBUTTONDOWN message.
	void OnRButtonDown(UINT nFlags, CPoint point); 
	
	// WM_LBUTTONUP message.
	void OnLButtonUp(UINT nFlags, CPoint point);
	
	// WM_MOUSEMOVE message.
	void OnMouseMove(UINT nFlags, CPoint point);

	// WM_KEYDOWN message.
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	// Cancel mode.
	virtual void OnCancelMode();

	// Get Rectangle
	CRect GetRect() const						{ return m_rcPosition; }

	// Set Rectangle
	void  SetRect(const CRect rc)				{ m_rcPosition = rc; }

	// Set Custom Text
	void  SetCustomText(const CString strText )	{ strCustom = strText; }

	// Get Custom Text
	CString GetCustomText() const				{ return strCustom; }

	// Set Custom Bar
	void SetCustomBar(const BOOL bCustom);

	// Set Main Border Size
	void SetMainBorderSize(const int nBorder);
 
	// Get Main Border Size
	int  GetMainBorderSize() const				{ return nMainBorderSize; }

	// Set Cell Border Size
	void SetCellBorderSize(const int nBorder);

	// Get Cell Border Size
	int GetCellBorderSize() const				{ return nCellBorderSize; }

	// Set Border Type
	void SetBorderType(const UINT nType)		{ nBorderType = nType; }

	// Get Border Type
	UINT GetBorderType() const					{ return nBorderType; }

	// Update Custom
	void UpdateCustom();

	// Get rgb from hue.
	BYTE GetRGBFromHue(float rm1, float rm2, float rh);

	// Get rgb value from hls.
	COLORREF GetRGBFromHLS( double H, double L, double S );

	// Get rgb value from hls.
	COLORREF GetRGBFromHLSExtend( double H, double L, double S );

	// Convert rgb value to hsl.
	void ConvertRGBToHSL( COLORREF rgb, double *H, double *S, double *L );

public:
	
	// Draw border for the control.
	// Draw Other Border
	virtual void DrawOtherBorder(CDCHandle dc,CRect &rcPos,UINT nType);

	// Draw Custom
	virtual void OnCustomDraw(CDCHandle dc,BOOL bHasColor);

	// Draw Custom Selected
	virtual void OnCustomDrawSelect(CDCHandle dc);
	
	// Draw a rectangle
	void DrawRect(CDCHandle dc, RECT rect);

	// Draw a rectangle
	void DrawRect(CDCHandle dc, int x, int y, int w, int h);

	// Draw a rectangle	
	void DrawRect(CDCHandle dc, RECT rect, COLORREF color);

	// Draw a rectangle
	void DrawRect(CDCHandle dc, int x, int y, int w, int h, COLORREF color);
	
	// Draw a circle
	void DrawCircle(CDCHandle dc, int x, int y, int radius);

	//Draw a circle
	void DrawCircle(CDCHandle dc, int x, int y, int radius, COLORREF color);
	
	// Draw a line
	void DrawLine(CDCHandle dc, CPoint& start, CPoint& end);

	// Draw a line
	void DrawLine(CDCHandle dc, int xStart, int yStart, int xEnd, int yEnd);

	// Draw a line
	void DrawLine(CDCHandle dc, CPoint& start, CPoint& end, COLORREF color);

	// Draw a line
	void DrawLine(CDCHandle dc, int xStart, int yStart, int xEnd, int yEnd, COLORREF color);
	
public:
	// The Rectangle of Cells
	CRect				rcCells;
	
	// The Rectangle of Position
	CRect				m_rcPosition;

	// The Rectangle of Custom
	CRect				rcCustom;

	// the Rectangle of Custom Saved
	CRect				rcCustomSave;

	// Sep Rectangle
	CRect				rcSep;

	// Current Color
	COLORREF			m_crCurColor;

	// The pointer to parent window
	CWindow*				m_pParent;

	// Is it popup style?
	BOOL				m_bPopup;

	// Is it custom selected?
	BOOL				m_bCustomSelect;

	// Custom string
	CString				strCustom;

	// Rows
	int					m_nRows;

	// Columns
	int					m_nCols;

	// Has it custom bar
	BOOL				bHasCustomBar;

	// Main Border Size
	int					nMainBorderSize;

	// Cell Border Size
	int					nCellBorderSize;

	// Border Type
	UINT				nBorderType;

	//
	double				m_dblLum;

	//
	double				m_dblSat;

	//
	double				m_dblHue;

	// Text Font
	CFont				fText;

protected:
	CColorCell m_crCells[FO_TOTAL_CELLS];
};

#endif // _DROPPALETTE_H__
