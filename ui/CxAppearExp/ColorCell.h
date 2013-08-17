// ColorCell.h: interface for the CColorCell class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _COLORCELL_H__
#define _COLORCELL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_SELECTCOLOROK					WM_USER+220	// Select color ok.
#define WM_SELECTCOLORCANCEL				WM_USER+221	// Select color cancel.
#define WM_SELECTCOLORCUSTOM				WM_USER+222	// Select color custom.
#define WM_SELECTCOLORCHANGE				WM_USER+223 // Select color change.

enum BorderShape 
{
	BORDER_NONE = 0,
	BORDER_SINGLE_IN,
	BORDER_SINGLE_OUT,
	BORDER_DOUBLE_IN,
	BORDER_DOUBLE_OUT,
	BORDER_SUNKEN,	
	BORDER_LOW,	
	BORDER_UP,	
	BORDER_HIGH,
	BORDER_FLAT,
	BORDER_FRAME,
	BORDER_LIGHT_FRAME,	
	BORDER_THIN_FRAME,
	BORDER_THIN_LIGHT_FRAME,
	BORDER_ETCHED,	
	BORDER_ETCHED2,	
	BORDER_SIMPLE
};	

struct _GLOBAL_DATA
{
	_GLOBAL_DATA();
	~_GLOBAL_DATA();
	
	// Button color.
	COLORREF m_crBtnFace;
	COLORREF m_crBtnHilite;
	COLORREF m_crBtnShadow;
	COLORREF m_crBtnText;
	COLORREF m_crBtnDkShadow;
	COLORREF m_crBtn3dShadow;
	COLORREF m_crBtn3dHiLight;
	COLORREF m_crBtnLight;
	COLORREF m_cr3dFace;
	COLORREF m_crAcCaption;
	COLORREF m_crInAcCaption;
	COLORREF m_crInAcCaptionText;
	COLORREF m_crDesktop;
	
	// Window color.
	COLORREF m_crWindowFrame;
	COLORREF m_crHilite;
	COLORREF m_crWindowText;
	
	// Text color.
	COLORREF m_crTextGrayed;
	COLORREF m_crTextHilite;
	COLORREF m_crTextHot;
	
	// Size of window.
	int m_cxEdge;
	int m_cyEdge;
	int m_cxBorder;
	
	// Screen size.
	int m_cxScreen;
	int m_cyScreen;
	
	// Scroll size.
	int m_cxVScroll;
	
	// Frame size.
	int m_cxFrame;
	int m_cyFrame;
	
	int m_cxFixedFrame;
	
	// Is nt.
	BOOL	bNT4;
	int		m_nBitsPerPixel;
	
	CSize GetScreenDimensions();
	void OnUpdateMetrics();
	void OnSysColorChange();
};

extern _GLOBAL_DATA globalData;

class CColorCell  
{
public:
	CColorCell();
	virtual ~CColorCell();
	
	BOOL HitTest(CPoint point);
	void OnDraw(CDCHandle dc);
	void OnDrawSelect(CDCHandle dc);
	void SetRect(const CRect &rcPos){ m_rcPosition = rcPos; }
	CRect GetRect() const;
	void SetCellColor(const COLORREF &cr);
	COLORREF GetCellColor() const { return m_crCell; }
	void AdjustCellPosition(int x, int y, int nWidth);
	void SetFirstType(const BOOL bFirst) {m_bAtFirstType = bFirst; }
	int MoreNear(float fNear);
	
	void SetCellBorderSize(const int nBorder) { m_nCellBorderSize = nBorder; }
	
private:
	CRect m_rcPosition;
	COLORREF m_crCell;		//当前颜色
	int m_nCellBorderSize;	//当前边界尺寸
	CPoint m_Points[6];
	BOOL m_bAtFirstType;
};

#endif // _COLORCELL_H__
