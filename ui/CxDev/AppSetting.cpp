// AppSetting.cpp: implementation of the CAppSetting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// class CAppSetting
//////////////////////////////////////////////////////////////////////

CAppSetting::CAppSetting(LPCTSTR lpszSection)
{
	m_strSection = lpszSection;
}

CAppSetting::~CAppSetting()
{

}

//////////////////////////////////////////////////////////////////////
// class CFormSetting
//////////////////////////////////////////////////////////////////////

CFormSetting::CFormSetting(LPCTSTR lpszSection) : 
	CAppSetting(lpszSection), 
	m_clrFore(RGB(0, 0, 0))
{
	if (m_strSection == _T("MainForm"))
	{
		m_nWidth = GetSystemMetrics(SM_CXSCREEN);
		m_nHeight = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		m_nHeight = 300;
		m_nWidth = 400;
	}

//	HDC hDC = ::GetDC(NULL);
//	m_nWidth = GetSystemMetrics(SM_CXSCREEN) * 1440 / GetDeviceCaps(hDC, LOGPIXELSX);
//	m_nHeight = GetSystemMetrics(SM_CYSCREEN) * 1440 / GetDeviceCaps(hDC, LOGPIXELSY);
//	ReleaseDC(NULL, hDC);
	  
	m_clrBack = 0x80000000 | COLOR_BTNFACE; //GetSysColor(COLOR_BTNFACE);
}

BOOL CFormSetting::LoadSettings()	
{
	int nTemp;
		
	m_nWidth = theApp.GetProfileInt(m_strSection, _T("Width"), m_nWidth);
	m_nHeight = theApp.GetProfileInt(m_strSection, _T("Height"), m_nHeight);
	  
	nTemp = theApp.GetProfileInt(m_strSection, _T("BackgroundColor"), 0x80000000 | COLOR_BTNFACE);
	m_clrBack = (COLORREF)nTemp;

	nTemp = theApp.GetProfileInt(m_strSection, _T("ForegroundColor"), 0x80000000 | COLOR_WINDOWTEXT);
	m_clrFore = (COLORREF)nTemp;

	return TRUE;
}

BOOL CFormSetting::SaveSettings()	
{
	int nTemp;

	theApp.WriteProfileInt(m_strSection, _T("Width"), m_nWidth);
	theApp.WriteProfileInt(m_strSection, _T("Height"), m_nHeight);

	nTemp = (int)m_clrBack;
	theApp.WriteProfileInt(m_strSection, _T("BackgroundColor"), nTemp);

	nTemp = (int)m_clrFore;
	theApp.WriteProfileInt(m_strSection, _T("ForegroundColor"), nTemp);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// class CObjectSetting
//////////////////////////////////////////////////////////////////////

CObjectSetting::CObjectSetting(LPCTSTR lpszSection) : 
	CAppSetting(lpszSection)
{
	m_clrEdgeColor = RGB(0, 0, 0); // 默认黑色
	m_clrFillColor = RGB(0, 0, 255); // 默认绿色
	m_clrFontColor = 0x80000000 | COLOR_WINDOWTEXT;
}
	
BOOL CObjectSetting::LoadSettings()	
{
	m_clrEdgeColor = theApp.GetProfileInt(m_strSection, _T("EdgeColor"), m_clrEdgeColor); 
	m_clrFillColor = theApp.GetProfileInt(m_strSection, _T("FillColor"), m_clrFillColor); 
	m_clrFontColor = theApp.GetProfileInt(m_strSection, _T("FontColor"), m_clrFontColor); 
	
	return TRUE;
}

BOOL CObjectSetting::SaveSettings()	
{
	theApp.WriteProfileInt(m_strSection, _T("EdgeColor"), m_clrEdgeColor);
	theApp.WriteProfileInt(m_strSection, _T("FillColor"), m_clrFillColor);
	theApp.WriteProfileInt(m_strSection, _T("FontColor"), m_clrFontColor);
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// class CObjectSetting
//////////////////////////////////////////////////////////////////////

CGenSetting::CGenSetting(LPCTSTR lpszSection) : 
	CAppSetting(lpszSection), 
	m_bGrid(TRUE),
	m_clrGrid(RGB(0, 0, 0))
{
	m_nGridSpaceWidth = 10;
	m_nGridSpaceHeight = 10;

	m_bAlignToGrid = TRUE;
	m_bShowScriptSymbol = TRUE;
}

BOOL CGenSetting::LoadSettings()	
{
	int nTemp;
	
	nTemp = theApp.GetProfileInt(m_strSection, _T("GridEnable"), 1);
	m_bGrid = (BOOL)nTemp;
	
	nTemp = theApp.GetProfileInt(m_strSection, _T("GridColor"), 0);
	m_clrGrid = (COLORREF)nTemp;

	m_nGridSpaceWidth = theApp.GetProfileInt(m_strSection, _T("GridSpaceWidth"), 10);
	m_nGridSpaceHeight = theApp.GetProfileInt(m_strSection, _T("GridSpaceHeight"), 10);
	
	nTemp = theApp.GetProfileInt(m_strSection, _T("AlignToGrid"), 1);
	m_bAlignToGrid = (BOOL)nTemp;

	BYTE* pb = NULL;
	UINT nLen = 0;
	if (theApp.GetProfileBinary(m_strSection, _T("PageMargin"), &pb, &nLen))
	{
		ASSERT(nLen == sizeof(CRect));
		memcpy(&m_rectPageMargin, pb, sizeof(CRect));
		delete pb;
	}
	else
		m_rectPageMargin.SetRect(900, 720, 900, 720);

	m_bShowScriptSymbol = theApp.GetProfileInt(m_strSection, _T("ShowScriptSymbol"), 1) != 0;

	return TRUE;
}

BOOL CGenSetting::SaveSettings()	
{
	int nTemp;

	nTemp = (int)m_bGrid;
	theApp.WriteProfileInt(m_strSection, _T("GridEnable"), nTemp);
	
	nTemp = (int)m_clrGrid;
	theApp.WriteProfileInt(m_strSection, _T("GridColor"), nTemp);

	theApp.WriteProfileInt(m_strSection, _T("GridSpaceWidth"), m_nGridSpaceWidth);
	theApp.WriteProfileInt(m_strSection, _T("GridSpaceHeight"), m_nGridSpaceHeight);

	nTemp = (int)m_bAlignToGrid;
	theApp.WriteProfileInt(m_strSection, _T("AlignToGrid"), nTemp);

	theApp.WriteProfileBinary(m_strSection, _T("PageMargin"), (BYTE*)&m_rectPageMargin,
		sizeof(CRect));

	theApp.WriteProfileInt(m_strSection, _T("ShowScriptSymbol"), m_bShowScriptSymbol ? 1 : 0);
	
	return TRUE;
}
