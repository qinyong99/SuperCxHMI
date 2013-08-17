// RectF.cpp: implementation of the CRectF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"
#include "RectF.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CRectF::InflateRect(LPCRECTF lpRect)
{
	left -= lpRect->left;
	top -= lpRect->top;
	right += lpRect->right;
	bottom += lpRect->bottom;
}

void CRectF::InflateRect(float l, float t, float r, float b)
{
	left -= l;
	top -= t;
	right += r;
	bottom += b;
}

void CRectF::DeflateRect(LPCRECTF lpRect)
{
	left += lpRect->left;
	top += lpRect->top;
	right -= lpRect->right;
	bottom -= lpRect->bottom;
}

void CRectF::DeflateRect(float l, float t, float r, float b)
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}