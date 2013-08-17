// DropPalette.cpp: implementation of the CDropPalette class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DropPalette.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include <math.h>

#define NUM_LEVELS			7
#define TAN30				0.57735026918962F
#define YOFFSET				(1.5F * TAN30)
#define PI					3.14159265358979

static const float cfxFOOffset[] = { -0.5, -1.0, -0.5, 0.5, 1.0, 0.5 };
static const float cfyFOOffset[] = { YOFFSET, 0.0, -YOFFSET, -YOFFSET, 0.0, YOFFSET };

static const COLORREF crWhite = RGB (255, 255, 255);
static const COLORREF crBlack = RGB (0, 0, 0);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDropPalette::CDropPalette()
{
	m_rcPosition = CRect(0,0,0,0);
	m_pParent = NULL;
	m_bPopup = FALSE;
	m_bCustomSelect = FALSE;
	strCustom = "Custom...";
	m_nRows = 16;
	m_nCols = 9;
	bHasCustomBar = TRUE;
	nMainBorderSize = 5;
	nCellBorderSize = 1;
	nBorderType = BORDER_SINGLE_OUT;
	m_crCurColor = RGB(0,0,0);
	ConvertRGBToHSL (m_crCurColor, &m_dblHue, &m_dblSat, &m_dblLum);
	rcSep = CRect(0,0,0,0);
	CFont newFont;
	HFONT hFont = NULL;
#ifdef WIN32
	hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
#endif
	if (hFont == NULL)
		hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
	newFont.Attach(hFont);
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	newFont.GetLogFont(&lf);
	fText.CreateFontIndirect(&lf);
}

CDropPalette::~CDropPalette()
{
	fText.DeleteObject();
}
/////////////////////////////////////////////////////////////////////////////
// CPrintBaseJob serialization

void CDropPalette::InitColorPalette()
{
	ComputeColors();
	CRect rcPos = GetRect();
	rcPos.DeflateRect(nMainBorderSize, nMainBorderSize, nMainBorderSize, nMainBorderSize);
	rcCustom.SetRectEmpty();
	if (bHasCustomBar)
	{
		rcSep = rcPos;
		rcSep.top = rcPos.bottom -22;
		rcSep.bottom = rcSep.top +4;

		rcCustom = rcPos;
		rcCustom.top = rcPos.bottom -18;
		rcCustomSave = rcCustom;
		rcPos.bottom -= 24;
	}
	AdjustCells(rcPos);
}

void CDropPalette::InvalRect(CRect rcPos)
{
	rcPos.InflateRect(2,2,2,2);
	m_pParent->InvalidateRect(rcPos);
}

void CDropPalette::Invalidate(BOOL bRedraw /* = FALSE */)
{
	m_pParent->Invalidate(bRedraw);
}

void CDropPalette::SetHLS(double hue, double luminance, double saturation)
{
	if (hue != -1)
		m_dblHue = hue;
	
	if (saturation != -1)
		m_dblSat = saturation;
	
	if (luminance != -1)
		m_dblLum = luminance;
	
	COLORREF crColor = GetRGBFromHLSExtend(m_dblHue, m_dblSat, m_dblLum);
	SetRGB(crColor);
}

void CDropPalette::GetHLS(double *hue, double *luminance, double *saturation)
{
	ConvertRGBToHSL(m_crCurColor, &m_dblHue, &m_dblSat, &m_dblLum);

	*hue		= m_dblHue;
	*luminance	= m_dblLum;
	*saturation = m_dblSat;
}

void CDropPalette::SetRGB(COLORREF ref)
{
	ConvertRGBToHSL (ref, &m_dblHue, &m_dblSat, &m_dblLum);
	CColorCell* pOldSelCell = NULL;
	CColorCell* pSelCell = NULL;
	BOOL bCurSel = FALSE;
	BOOL bOldSel = FALSE;
	for (int i = 0; i < FO_TOTAL_CELLS; i ++)
	{
		if (m_crCells[i].GetCellColor() == m_crCurColor)
		{
			bOldSel = TRUE;
			pOldSelCell = &m_crCells[i];
			if(bOldSel && bCurSel)
			{
				break;
			}
		}
		if (m_crCells[i].GetCellColor() == ref)
		{
			bCurSel = TRUE;
			pSelCell = &m_crCells[i];
			if(bOldSel && bCurSel)
			{
				break;
			}
		}
	}
	
	if(pSelCell != NULL)
	{
		m_crCurColor = pSelCell->GetCellColor();
		InvalRect(pSelCell->GetRect());
	}
	else
	{
		m_crCurColor = ref;
		InvalRect(rcCustomSave);
	}

	if(pOldSelCell != NULL)
	{
		InvalRect(pOldSelCell->GetRect());
	}
}

void CDropPalette::UpdateAll()
{
	InitColorPalette();
}

void CDropPalette::Create(CWindow *pWnd,COLORREF crDefault,CRect &rcPos,BOOL bPopup)
{
	m_rcPosition = rcPos;
	m_pParent = pWnd;
	m_crCurColor = crDefault;
	m_bPopup = bPopup;
	InitColorPalette();
}

CRect CDropPalette::GetOldColorRect()
{
	for (int i = 0; i < FO_TOTAL_CELLS; i ++)
	{
		if (m_crCells[i].GetCellColor() == m_crCurColor)
		{
			return m_crCells[i].GetRect();
		}
	}
	return CRect(0,0,0,0);
}

CColorCell* CDropPalette::HitTest(CPoint point, BOOL& bChange)
{
	bChange = FALSE;

	CColorCell* pSelCell = NULL;
	CColorCell* pHitCell = NULL;
	BOOL bHit = FALSE;
	BOOL bSel = FALSE;
	for (int i = 0; i < FO_TOTAL_CELLS; i ++)
	{
		if (m_crCells[i].GetCellColor() == m_crCurColor)
		{
			bSel = TRUE;
			pSelCell = &m_crCells[i];
			if (bSel && bHit)
			{
				break;
			}
		}
		if (m_crCells[i].HitTest(point))
		{
			bHit = TRUE;
			pHitCell = &m_crCells[i];
			if (bSel && bHit)
			{
				break;
			}
		}
	}
	
	if (pHitCell != pSelCell)
	{
		if (pHitCell != NULL)
		{
			bChange = TRUE;

			m_crCurColor = pHitCell->GetCellColor();
			InvalRect(pHitCell->GetRect());
		}

		if (pSelCell != NULL)
		{
			InvalRect(pSelCell->GetRect());
		}
	}

	return pHitCell;
}

void CDropPalette::DrawRect(CDCHandle dc, RECT rect)
{
	dc.MoveTo(rect.left, rect.top);
	dc.LineTo(rect.right - 1, rect.top);
	dc.LineTo(rect.right - 1, rect.bottom - 1);
	dc.LineTo(rect.left, rect.bottom - 1);
	dc.LineTo(rect.left, rect.top);
}

void CDropPalette::DrawRect(CDCHandle dc, RECT rect, COLORREF color)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, color);
	HPEN hPenOld = dc.SelectPen(pen);
	
	DrawRect(dc, rect);
	
	dc.SelectPen(hPenOld);
}

void CDropPalette::DrawRect(CDCHandle dc, int x, int y, int w, int h)
{
	dc.MoveTo(x, y);
	dc.LineTo(x + w, y);
	dc.LineTo(x + w, y + h);
	dc.LineTo(x, y + h);
	dc.LineTo(x, y);
}

void CDropPalette::DrawRect(CDCHandle dc, int x, int y, int w, int h, COLORREF color)
{	
	color;
	DrawRect(dc, x, y, w, h);
}

void CDropPalette::DrawCircle(CDCHandle dc, int x, int y, int radius)
{
	dc.Arc(x - radius, y - radius, x + radius, y + radius, x, y - radius, x, y - radius);
}

void CDropPalette::DrawCircle(CDCHandle dc, int x, int y, int radius, COLORREF color)
{
	CPen pen;
	pen.CreatePen(PS_SOLID,1,color);

	HPEN hPenOld = (CPen)dc.SelectPen(pen);
	DrawCircle(dc, x, y, radius);

	dc.SelectPen(hPenOld);
}

void CDropPalette::DrawLine(CDCHandle dc, CPoint& start, CPoint& end)
{
	dc.MoveTo(start);
	dc.LineTo(end);
}

void CDropPalette::DrawOtherBorder(CDCHandle dc,CRect &rcPos,UINT nType)
{
	CRect rcImage;
	rcImage = rcPos;
	if (TRUE)
	{
		switch(nType)
		{
		case BORDER_NONE:
			{
			}
			break;
		case BORDER_SINGLE_IN:
			{
				rcPos.InflateRect(2,2,2,2);
				dc.Draw3dRect(rcImage,  RGB(0,0,0),RGB(255,255,255));
				rcImage.InflateRect(1,1,1,1);
				dc.Draw3dRect(rcImage,  globalData.m_crBtnShadow,globalData.m_crBtnFace);
			}
			break;
		case BORDER_SINGLE_OUT:
			{
				dc.Draw3dRect(rcImage,  RGB(255,255,255),RGB(0,0,0));
				rcImage.DeflateRect(1,1,1,1);
				dc.Draw3dRect(rcImage, globalData.m_crBtnFace,globalData.m_crBtnShadow);
			}
			break;
			
		case BORDER_DOUBLE_IN:
			{
				dc.Draw3dRect(rcImage,  globalData.m_crBtnShadow,RGB(255,255,255));
				rcImage.DeflateRect(1,1,1,1);
				dc.Draw3dRect(rcImage,  RGB(0,0,0),globalData.m_crBtnFace);
				rcImage.DeflateRect(2,2,2,2);
				dc.Draw3dRect(rcImage,  RGB(255,255,255),RGB(0,0,0));
				rcImage.DeflateRect(1,1,1,1);
				dc.Draw3dRect(rcImage, globalData.m_crBtnFace, globalData.m_crBtnShadow);
			}
			break;
		case BORDER_DOUBLE_OUT:
			{
				dc.Draw3dRect(rcImage,  RGB(255,255,255),RGB(0,0,0));
				rcImage.DeflateRect(1,1,1,1);
				dc.Draw3dRect(rcImage, globalData.m_crBtnFace ,globalData.m_crBtnShadow);
				rcImage.DeflateRect(2,2,2,2);
				dc.Draw3dRect(rcImage,  globalData.m_crBtnShadow,RGB(255,255,255));
				rcImage.DeflateRect(1,1,1,1);
				dc.Draw3dRect(rcImage,  RGB(0,0,0),globalData.m_crBtnFace);
			}
			break;
			
		case BORDER_SUNKEN:
			{
				dc.Draw3dRect(rcPos, globalData.m_crBtn3dShadow, globalData.m_crBtn3dHiLight);
				rcPos.DeflateRect(1,1,1,1);
				dc.Draw3dRect(rcPos, globalData.m_crBtnDkShadow, globalData.m_crBtnLight);
			}
			break;
			
		case BORDER_HIGH:
			{
				dc.Draw3dRect(rcPos, globalData.m_crBtn3dHiLight, globalData.m_crBtnDkShadow);
				rcPos.DeflateRect(1,1,1,1);
				dc.Draw3dRect(rcPos, globalData.m_crBtnLight, globalData.m_crBtn3dShadow);
			}
			break;
			
		case BORDER_FLAT:
			{
				DrawRect(dc, rcPos, globalData.m_crBtnDkShadow);
				rcPos.DeflateRect(1,1,1,1);
				DrawRect(dc, rcPos, globalData.m_crBtnDkShadow);
			}
			break;
			
		case BORDER_LOW:
			{
				dc.Draw3dRect(rcPos, globalData.m_crBtn3dShadow, globalData.m_crBtn3dHiLight);
			}
			break;
			
		case BORDER_UP:
			{
				dc.Draw3dRect(rcPos, globalData.m_crBtn3dHiLight, globalData.m_crBtn3dShadow);
			}
			break;
			
		case BORDER_SIMPLE:
			{
				DrawRect(dc, rcPos, globalData.m_crBtnDkShadow);;
			}
			break;
			
		case BORDER_FRAME:
			{
				dc.Draw3dRect(rcPos,globalData.m_crBtn3dHiLight, globalData.m_crBtnDkShadow);
				rcPos.DeflateRect(1,1,1,1);
				DrawRect(dc, rcPos, globalData.m_cr3dFace);
				rcPos.DeflateRect(1,1,1,1);
				dc.Draw3dRect(rcPos, globalData.m_crBtnDkShadow, globalData.m_crBtn3dHiLight);
			}
			break;
			
		case BORDER_THIN_FRAME:
			{
				dc.Draw3dRect(rcPos,globalData.m_crBtn3dHiLight, globalData.m_crBtnDkShadow);
				rcPos.DeflateRect(1,1,1,1);
				dc.Draw3dRect(rcPos, globalData.m_crBtnDkShadow, globalData.m_crBtn3dHiLight);
			}
			break;
			
		case BORDER_LIGHT_FRAME:
			{
				dc.Draw3dRect(rcPos,globalData.m_crBtn3dHiLight, globalData.m_crBtn3dShadow);
				rcPos.DeflateRect(1,1,1,1);
				DrawRect(dc, rcPos, globalData.m_cr3dFace);
				rcPos.DeflateRect(1,1,1,1);
				dc.Draw3dRect(rcPos, globalData.m_crBtn3dShadow, globalData.m_crBtn3dHiLight);
			}
			break;
			
		case BORDER_THIN_LIGHT_FRAME:
			{
				dc.Draw3dRect(rcPos,globalData.m_crBtn3dHiLight, globalData.m_crBtn3dShadow);
				rcPos.DeflateRect(1,1,1,1);
				dc.Draw3dRect(rcPos, globalData.m_crBtn3dShadow, globalData.m_crBtn3dHiLight);
			}
			break;
			
		case BORDER_ETCHED:
			{
				dc.Draw3dRect(rcPos, globalData.m_crBtn3dShadow, globalData.m_crBtn3dHiLight);
				rcPos.DeflateRect(1,1,1,1);
				dc.Draw3dRect(rcPos,globalData.m_crBtn3dHiLight, globalData.m_crBtn3dShadow);
			}
			break;
			
		case BORDER_ETCHED2:
			{
				dc.Draw3dRect(rcPos, globalData.m_crBtn3dShadow, globalData.m_crBtn3dHiLight);
				rcPos.DeflateRect(1,1,1,1);
				DrawRect(dc, rcPos, globalData.m_crBtnDkShadow);
				rcPos.DeflateRect(1,1,1,1);
				dc.Draw3dRect(rcPos,globalData.m_crBtn3dHiLight, globalData.m_crBtn3dShadow);
			}
			break;
			
		default:
			break;
		}
	}
}

void CDropPalette::DrawLine(CDCHandle dc, CPoint& start, CPoint& end, COLORREF color)
{
	CPen pen;
	pen.CreatePen(PS_SOLID,1,color);
	CPen oldPen = (CPen)dc.SelectPen((HPEN)pen);
	
	DrawLine(dc, start, end);
	
	dc.SelectPen((HPEN)oldPen);
}

void CDropPalette::DrawLine(CDCHandle dc, int xStart, int yStart, int xEnd, int yEnd)
{
	dc.MoveTo(xStart, yStart);
	dc.LineTo(xEnd, yEnd);
}

void CDropPalette::DrawLine(CDCHandle dc, int xStart, int yStart, int xEnd, int yEnd, COLORREF color)
{
	CPen pen;
	pen.CreatePen(PS_SOLID,1,color);
	CPen oldPen = (CPen)dc.SelectPen((HPEN)pen);
	
	DrawLine(dc, xStart, yStart, xEnd, yEnd);
	
	dc.SelectPen((HPEN)oldPen);
}

void CDropPalette::OnDraw(CDCHandle dc)
{
	CRect rectClient;
	rectClient = m_rcPosition;
	
	dc.FillSolidRect(rectClient, globalData.m_crBtnFace);

	DrawOtherBorder(dc,rectClient,nBorderType);
	
	CColorCell* pSelCell = NULL;
	
	for (int i = 0; i < FO_TOTAL_CELLS; i ++)
	{
		
		m_crCells[i].OnDraw(dc);
		
		if (m_crCells[i].GetCellColor() == m_crCurColor)
		{
			pSelCell = &m_crCells[i];
		}
	}
	
	if (pSelCell != NULL)
	{
		pSelCell->OnDrawSelect (dc);
		if(bHasCustomBar)
		{
			CRect rcX = rcSep;
			rcX.bottom -= 2;
			dc.Draw3dRect(rcX, globalData.m_crBtnShadow, RGB(255,255,255));
			OnCustomDraw(dc, FALSE);
			if (m_bCustomSelect)
			{
				OnCustomDrawSelect(dc);
			}
		}
	}
	else
	{
		if(bHasCustomBar)
		{
			CRect rcX = rcSep;
			rcX.bottom -= 2;
			dc.Draw3dRect(rcX, globalData.m_crBtnShadow, RGB(255,255,255));
			OnCustomDraw(dc,TRUE);
			if (m_bCustomSelect)
			{
				OnCustomDrawSelect(dc);
			}
		}
	}
}

int CDropPalette::GetAngleFromPoint(int nX, int nY)
{
	double dAngle = atan2(nY, nX);
	
	return (int)(dAngle * 180.0/PI);
}

void CDropPalette::ComputeColors()
{
	CRect rectClient;
	rectClient = m_rcPosition;
	rectClient.bottom = rectClient.bottom - 24;
	
	// Normalize to squere:
	if (rectClient.Height () < rectClient.Width ())
	{
		rectClient.DeflateRect((rectClient.Width () - rectClient.Height ()) / 2, 0);
	}
	else
	{
		rectClient.DeflateRect(0, (rectClient.Height () - rectClient.Width ()) / 2);
	}
	
	//ATL_ASSERT (abs (rectClient.Height () - rectClient.Width ()) <= 1);
	
	//每行高即每个单元的尺寸
	int nCellSize = rectClient.Height () / (2 * NUM_LEVELS - 1) + 1;
	
	//中心点位置
	int x = (rectClient.left + rectClient.right) / 2;
	int y = (rectClient.top + rectClient.bottom) / 2;

	// Add center cell(中心单元)
	m_crCells[0].SetCellColor(crWhite);
	m_crCells[0].AdjustCellPosition(x,y,nCellSize);
	// for each level(每一行)
	int nNumber = 1;
	for (int nLevel = 1; nLevel < NUM_LEVELS; nLevel++)
	{
		// store the level start position
		int nPosX = x + (nCellSize * nLevel);
		int nPosY = y;
		
		// for each side
		for (int nSide = 0; nSide < NUM_LEVELS - 1; nSide++)
		{
			// set the deltas for the side
			int nDx = static_cast<int>(static_cast<float>(nCellSize) * cfxFOOffset[nSide]);
			int nDy = static_cast<int>(static_cast<float>(nCellSize) * cfyFOOffset[nSide]);
			
			// for each cell per side
			for (int nCell = 0; nCell < nLevel; nCell++)
			{
				int nAngle = GetAngleFromPoint(nPosX - x, nPosY - y);
				
				// TODO: Set the luminance and saturation properly
				double L = 1. * (NUM_LEVELS - nLevel) / NUM_LEVELS + .1;
				
				m_crCells[nNumber].SetCellColor(GetRGBFromHLSExtend ((float) nAngle, L, 1.0F));
				m_crCells[nNumber].AdjustCellPosition(nPosX,nPosY,nCellSize);
				nNumber++;
				// offset the position
				nPosX += nDx;
				nPosY += nDy;
			}
		}
	}
	m_crCells[nNumber].SetCellColor(globalData.m_crBtnFace);
	nNumber++;
	m_crCells[nNumber].SetCellColor(crWhite);

	int nRGBOffset = 255 / (15 + 2);
	
	int nRGB = 255 - nRGBOffset;
	
	for (int i = 0; i < 15; i++)
	{
		COLORREF color = RGB(nRGB, nRGB, nRGB);
		m_crCells[nNumber].SetCellColor(color);
		nNumber ++;
		nRGB -= nRGBOffset;
	}
	m_crCells[nNumber].SetCellColor(crBlack);
}

void CDropPalette::AdjustCells(CRect rcPos)
{
	CRect rcTotal = rcPos;
	rcCells = rcTotal;
	
	// Determine the total width & height budgets.
	double MaxPixWide = double(rcTotal.Width());
	double maxPixHigh = double(rcTotal.Height());
	
	double dX;
	dX = 0;
	
	int *widthArray = new int[16+1];
	{
		double dlta = (double)(1/(double)16);
		for ( int i = 0 ; i < 16+1 ; i++ ) 
		{
			int x = rcTotal.left + int(double((MaxPixWide)*dlta*i));
			widthArray[i] = x;
		}
	}
	int *heightArray = new int[9+1];
	{
		double dlta = double(1/(double)9);
		for ( int i = 0 ; i < 9+1 ; i++ ) 
		{
			heightArray[i] = rcTotal.top + int(double((maxPixHigh)*dlta*i));
		}
	}
	
	CRect rcTemp;
	CRect rcOther;
	int nDay, nWeek;
	for (int x = 128 ; x < FO_TOTAL_CELLS ; x++ )
	{
		nDay = x % 16;
		nWeek = x / 16 ;
		rcTemp =  CRect(widthArray[nDay],heightArray[nWeek],widthArray[nDay+1],heightArray[nWeek+1]);
		m_crCells[x].SetFirstType(FALSE);
		m_crCells[x].SetRect(&rcTemp);
	}
	delete []heightArray;
	delete []widthArray;
}

void CDropPalette::CaptureMouse(BOOL bCapture)
{
	if (bCapture)
	{
		m_pParent->SetCapture();
	}
	else
	{
		ReleaseCapture();
	}
}

void CDropPalette::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	nChar; 
	nRepCnt; 
	nFlags; 
	
}

void CDropPalette::OnLButtonUp(UINT nFlags, CPoint point) 
{
	nFlags;
	if(m_bCustomSelect)
	{
		m_bCustomSelect = FALSE;
		InvalRect(rcCustom);
	}

	BOOL bChange = FALSE;
	CColorCell* pSel = HitTest(point, bChange);
	if (pSel != NULL && bChange)
	{
		OnSelectOK((WPARAM)(pSel->GetCellColor()),0);
	}

	if(rcCustom.PtInRect(point))
	{
		if(bHasCustomBar)
		{
			m_pParent->PostMessage(WM_SELECTCOLORCUSTOM, (WPARAM)(m_crCurColor), 0 );
			if(m_bPopup)
			{
				m_pParent->ShowWindow(SW_HIDE);
			}
		}
	}
}

void CDropPalette::OnSelectOK(WPARAM wParam, LPARAM lParam)
{
	m_pParent->PostMessage(WM_SELECTCOLOROK, wParam, lParam );
	if (bHasCustomBar)
	{
		UpdateCustom();
	}
	if (m_bPopup)
	{
		m_pParent->ShowWindow(SW_HIDE);
	}
}

void CDropPalette::OnSelectCancel(WPARAM wParam, LPARAM lParam)
{
	m_pParent->PostMessage(WM_SELECTCOLORCANCEL, wParam, lParam);
	if (bHasCustomBar)
	{
		UpdateCustom();
	}
	if (m_bPopup)
	{
		m_pParent->ShowWindow(SW_HIDE);
	}
}

void CDropPalette::OnMouseMove(UINT nFlags, CPoint point) 
{
	BOOL bChange = FALSE;

	CColorCell* pSel = HitTest(point, bChange);
	if (pSel != NULL && bChange)
	{
		OnSelectOK((WPARAM)(pSel->GetCellColor()),0);
	}
	else if(rcCustom.PtInRect(point))
	{
		if(bHasCustomBar)
		{
			m_bCustomSelect = TRUE;
			InvalRect(rcCustom);
		}
	}

}

void CDropPalette::OnCancelMode()
{
	
}
void CDropPalette::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	BOOL bChange = FALSE;
	CColorCell* pSel = HitTest(point, bChange);
	if (pSel != NULL/* && bChange*/) // 2005.11.25
	{
		COLORREF clr = pSel->GetCellColor();

		CHOOSECOLOR cc;
		memset(&cc, 0, sizeof CHOOSECOLOR);
		cc.lStructSize = sizeof CHOOSECOLOR;
		cc.hwndOwner = m_pParent->m_hWnd;
		cc.rgbResult = clr;
		cc.lpCustColors = &clr;
		cc.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;
		if(ChooseColor(&cc))
		{			
			pSel->SetCellColor(cc.rgbResult);
			m_pParent->Invalidate();	
			OnSelectOK((WPARAM)(cc.rgbResult),0);
		}
	}
}

void CDropPalette::OnLButtonDown(UINT nFlags, CPoint point) 
{
	nFlags; 
	point; 
	
}

void CDropPalette::OnRButtonDown(UINT nFlags, CPoint point) 
{
	nFlags; 
	point; 
}

void CDropPalette::OnCustomDraw(CDCHandle dc,BOOL bHasColor)
{
	dc.FillSolidRect(rcCustom,globalData.m_crBtnFace);
	rcCustom = rcCustomSave;
	if (bHasColor)
	{
		CRect rcItem;
		rcItem = rcCustom;
		rcItem.left = rcItem.right -21;
		rcCustom.right = rcItem.left-2;
		
		CRect rcTemp;
		rcTemp = rcCustom;
		rcTemp.DeflateRect(2,2,2,2);
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, globalData.m_crBtnShadow);
		CPen pOldPen = (CPen)dc.SelectPen((HPEN)pen);
		
		CString strText;
		strText = strCustom;
		if (strText.GetLength())
		{
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(0);
			dc.DrawText(strText, strText.GetLength(), rcTemp, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		}
		
		dc.SelectPen((HPEN)pOldPen);
		dc.Draw3dRect(rcItem,globalData.m_crBtnShadow,RGB(255,255,255));
		rcItem.DeflateRect(4,4,4,4);
		dc.FillSolidRect(rcItem,m_crCurColor);
		dc.Draw3dRect(rcItem,globalData.m_crBtnShadow,globalData.m_crBtnShadow);
	}
	else
	{
		CRect rcPos;
		rcPos = rcCustom;
		rcPos.DeflateRect(2,2,2,2);
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, globalData.m_crBtnShadow);
		CPen pOldPen = (CPen)dc.SelectPen((HPEN)pen);
		
		CString strText;
		strText = strCustom;
		if (strText.GetLength())
		{
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(0);
			dc.DrawText(strText, strText.GetLength(), rcPos, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		}
		
		dc.SelectPen((HPEN)pOldPen);
	}
}

void CDropPalette::OnCustomDrawSelect(CDCHandle dc)
{
	CRgn rgnOne, rgnTwo, rgnThree;
	
	CRect rcPos;
	rcPos = rcCustom;
	rcPos.DeflateRect(1,1,1,1);
	
	rgnTwo.CreateRectRgnIndirect(&rcPos);
	dc.FrameRgn(rgnTwo, (HBRUSH)::GetStockObject(WHITE_BRUSH), 2, 2);
	
	rcPos.InflateRect(2,2,2,2);
	
	rgnThree.CreateRectRgnIndirect(&rcPos);
	dc.FrameRgn(rgnThree, (HBRUSH)::GetStockObject(BLACK_BRUSH), 1, 1);
}

void CDropPalette::SetCustomBar(const BOOL bCustom)
{
	bHasCustomBar = bCustom;
}

void CDropPalette::SetMainBorderSize(const int nBorder)
{
	nMainBorderSize = nBorder;
}

void CDropPalette::UpdateCustom()
{
	InvalRect(rcCustomSave);
}

void CDropPalette::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	nFlags;
	nRepCnt;
	// Determine if the CONTROL key is pressed...
	int ks = GetKeyState(VK_CONTROL);
	ks >>= ((sizeof(int)*8)-1);

	switch(nChar) 
	{
	case VK_RETURN:
		{
			if(m_bPopup)
			{
				OnSelectOK((WPARAM)m_crCurColor,0L);
			}
		}
		break;
	case VK_CANCEL:
	case VK_ESCAPE:
		{
			if(m_bPopup)
			{
				OnSelectCancel((WPARAM)m_crCurColor, 0L);
			}
		}
		break;
	}
}

void CDropPalette::SetCellBorderSize(const int nBorder)
{
	nCellBorderSize = nBorder;
	for (int i = 0; i < FO_TOTAL_CELLS; i ++)
	{
		m_crCells[i].SetCellBorderSize(nBorder);
	}
}

BYTE CDropPalette::GetRGBFromHue(float rm1, float rm2, float rh)
{
	if (rh > 360.0f)
		rh -= 360.0f;
	else if (rh < 0.0f)
		rh += 360.0f;
	
	if (rh <  60.0f)
		rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;   
	else if (rh < 180.0f)
		rm1 = rm2;
	else if (rh < 240.0f)
		rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;      
	
	return static_cast<BYTE>(rm1 * 255);
}

COLORREF CDropPalette::GetRGBFromHLS(double H, double L, double S)
{
	double r, g, b;
	double m1, m2;
	
	if (S == 0) 
	{
		r = g = b = L;
	} 
	else 
	{
		if (L <= 0.5)
			m2 = L * (1.0 + S);
		else
			m2 = L + S - L * S;
		m1 = 2.0 * L - m2;
		r = GetRGBFromHue((float)m1, (float)m2, (float)(H + 1.0 / 3.0));
		g = GetRGBFromHue((float)m1, (float)m2, (float)H);
		b = GetRGBFromHue((float)m1, (float)m2, (float)(H - 1.0 / 3.0));
	}
	return RGB((BYTE)(r * 255), (BYTE)(g * 255), (BYTE)(b * 255));
}

COLORREF CDropPalette::GetRGBFromHLSExtend(double H, double L, double S)
{
	WORD R, G, B; // RGB component values
	
	if (S == 0.0)
	{
		R = G = B = unsigned char(L * 255.0);
	}
	else
	{
		float rm1, rm2;
		
		if (L <= 0.5f)
			rm2 = (float)(L + L * S);
		else
			rm2 = (float)(L + S - L * S);
		
		rm1 = (float)(2.0f * L - rm2);
		
		R = GetRGBFromHue(rm1, rm2, (float)(H + 120.0f));
		G = GetRGBFromHue(rm1, rm2, (float)(H));
		B = GetRGBFromHue(rm1, rm2, (float)(H - 120.0f));
	}
	
	return RGB(R, G, B);
}

void CDropPalette::ConvertRGBToHSL(COLORREF rgb, double *H, double *S, double *L)
{   
	double delta;
	double r = (double)GetRValue(rgb) / 255;
	double g = (double)GetGValue(rgb) / 255;
	double b = (double)GetBValue(rgb) / 255;   
	double cmax = max(r, max(g, b));
	double cmin = min(r, min(g, b));   
	*L= (cmax + cmin) / 2.0;   
	
	if (cmax == cmin) 
	{
		*S = 0;      
		*H = 0; // it's really undefined   
	} 
	else 
	{
		if (*L < 0.5) 
			*S = (cmax - cmin) / (cmax + cmin);      
		else
			*S = (cmax - cmin) / (2.0 - cmax - cmin);      
		
		delta = cmax - cmin;
		if (r == cmax) 
			*H = (g - b) / delta;      
		else if (g == cmax)
			*H = 2.0 + (b - r) / delta;
		else          
			*H = 4.0+(r - g) / delta;
		*H /= 6.0; 
		if (*H < 0.0)
			*H += 1;  
	}
}

