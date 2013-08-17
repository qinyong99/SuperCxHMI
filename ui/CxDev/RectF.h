// RectF.h: interface for the CRectF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECTF_H__CD0798B0_36E8_42FC_9E45_462AE2D82437__INCLUDED_)
#define AFX_RECTF_H__CD0798B0_36E8_42FC_9E45_462AE2D82437__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CArchive;

#define ROUND(f) (int)((f) >=0 ? f + .4999999 : f - .4999999) 

#ifdef POINTF
typedef struct tagPOINTF
{
    float  x;
    float  y;
} POINTF, *PPOINTF, NEAR *NPPOINTF, FAR *LPPOINTF;
#endif

typedef struct tagSIZEF
{
    float        cx;
    float        cy;
} SIZEF, *PSIZEF, *LPSIZEF;

typedef struct tagRECTF RECTF, *PRECTF, NEAR *NPRECTF, FAR *LPRECTF;

class CSizeF;
class CPointF;
class CRectF;

inline void NormalizeRectF(RECTF* pRect)
{
	float fTemp;
	if (pRect->left > pRect->right)
	{
		fTemp = pRect->left;
		pRect->left = pRect->right;
		pRect->right = fTemp;
	}
	if (pRect->top > pRect->bottom)
	{
		fTemp = pRect->top;
		pRect->top = pRect->bottom;
		pRect->bottom = fTemp;
	}
}

inline BOOL CopyRectF( LPRECTF lprcDst, CONST RECTF *lprcSrc)
{
	lprcDst->left = lprcSrc->left;
	lprcDst->right = lprcSrc->right;
	lprcDst->top = lprcSrc->top;
	lprcDst->bottom = lprcSrc->bottom;
	return TRUE;
}

inline BOOL IsRectFEmpty(CONST RECTF *lprc)
{
	return (lprc->left >= lprc->right || lprc->top >= lprc->bottom);
}

inline BOOL PtInRectF(CONST RECTF *lprc,  POINTF pt)
{
	return (pt.x >= lprc->left &&
		pt.x < lprc->right &&
		pt.y >= lprc->top &&
		pt.y < lprc->bottom);
}

inline BOOL EqualRectF(CONST RECTF *lprc1, CONST RECTF *lprc2)
{
	return lprc1->left == lprc2->left &&
		lprc1->top == lprc2->top &&
		lprc1->right == lprc2->right &&
		lprc1->bottom == lprc2->bottom;
}

inline BOOL InflateRectF(LPRECTF lprc, float dx, float dy)
{
	lprc->left -= dx;
	lprc->top -= dy;
	lprc->right += dx;
	lprc->bottom += dy;
}

inline BOOL OffsetRectF(LPRECTF lprc, float dx, float dy)
{
	lprc->left += dx;
	lprc->right += dx;
	lprc->bottom += dy;
	lprc->top += dy;
	return TRUE;
}

inline BOOL UnionRectF(LPRECTF lprcDst, CONST RECTF *lprcSrc1, CONST RECTF *lprcSrc2)
{
	if(lprcSrc1 == NULL || lprcSrc2 == NULL || lprcDst == NULL)
		return FALSE;

	if(lprcSrc1->left == lprcSrc1->right || lprcSrc1->bottom == lprcSrc1->top)
	{
		::CopyRectF(lprcDst, lprcSrc2);
		return TRUE;
	}

	if(lprcSrc2->left == lprcSrc2->right || lprcSrc2->bottom == lprcSrc2->top)
	{
		::CopyRectF(lprcDst, lprcSrc1);
		return TRUE;
	}

	lprcDst->left = lprcSrc1->left > lprcSrc2->left ? lprcSrc2->left : lprcSrc1->left;
	lprcDst->right = lprcSrc1->right > lprcSrc2->right ? lprcSrc1->right : lprcSrc2->right;
	lprcDst->top = lprcSrc1->top > lprcSrc2->top ? lprcSrc2->top : lprcSrc1->top;
	lprcDst->bottom = lprcSrc1->bottom > lprcSrc2->bottom ? lprcSrc1->bottom : lprcSrc2->bottom;

	return TRUE;
}

inline BOOL IntersectRectF(LPRECTF lprcDst, CONST RECTF *lprcSrc1, CONST RECTF *lprcSrc2)
{
	memset(lprcDst, 0, sizeof(RECTF));

	RECTF rc1, rc2;
	memcpy(&rc1, lprcSrc1, sizeof(rc1));
	memcpy(&rc2, lprcSrc2, sizeof(rc2));
	NormalizeRectF(&rc1);
	NormalizeRectF(&rc2);

	RECTF rect;
	rect.left = max(rc1.left, rc2.left);
	rect.right = min(rc1.right, rc2.right);
	rect.top = max(rc1.top, rc2.top);
	rect.bottom = min(rc1.bottom, rc2.bottom);
	if (rect.left >= rect.right || rect.top >= rect.bottom)
		return FALSE;

	memcpy(lprcDst, &rect, sizeof(rect));
	return TRUE;
}

class CSizeF : public tagSIZEF
{
public:

// Constructors
	// construct an uninitialized size
	CSizeF();
	// create from two integers
	CSizeF(float initCX, float initCY);
	// create from another size
	CSizeF(SIZEF initSize);
	// create from a point
	CSizeF(POINTF initPt);
	// create from a DWORD: cx = LOWORD(dw) cy = HIWORD(dw)
	CSizeF(DWORD dwSize);

// Operations
	BOOL operator==(SIZEF size) const;
	BOOL operator!=(SIZEF size) const;
	void operator+=(SIZEF size);
	void operator-=(SIZEF size);

// Operators returning CSizeF values
	CSizeF operator+(SIZEF size) const;
	CSizeF operator-(SIZEF size) const;
	CSizeF operator-() const;

// Operators returning CPointF values
	CPointF operator+(POINTF point) const;
	CPointF operator-(POINTF point) const;

// Operators returning CRectF values
	CRectF operator+(const RECTF* lpRect) const;
	CRectF operator-(const RECTF* lpRect) const;
};

/////////////////////////////////////////////////////////////////////////////
// CPointF - A 2-D point, similar to Windows POINTF structure.

class CPointF : public tagPOINTF
{
public:
// Constructors

	// create an uninitialized point
	CPointF();
	// create from two integers
	CPointF(float initX, float initY);
	// create from another point
	CPointF(POINTF initPt);
	// create from a size
	CPointF(SIZEF initSize);
	// create from a dword: x = LOWORD(dw) y = HIWORD(dw)
	CPointF(DWORD dwPoint);

// Operations

// translate the point
	void Offset(float xOffset, float yOffset);
	void Offset(POINTF point);
	void Offset(SIZEF size);

	BOOL operator==(POINTF point) const;
	BOOL operator!=(POINTF point) const;
	void operator+=(SIZEF size);
	void operator-=(SIZEF size);
	void operator+=(POINTF point);
	void operator-=(POINTF point);

// Operators returning CPointF values
	CPointF operator+(SIZEF size) const;
	CPointF operator-(SIZEF size) const;
	CPointF operator-() const;
	CPointF operator+(POINTF point) const;

// Operators returning CSizeF values
	CSizeF operator-(POINTF point) const;

// Operators returning CRectF values
	CRectF operator+(const RECTF* lpRect) const;
	CRectF operator-(const RECTF* lpRect) const;
};

/////////////////////////////////////////////////////////////////////////////
// CRectF - A 2-D rectangle, similar to Windows RECTF structure.

typedef const RECTF* LPCRECTF;    // pointer to read/only RECTF

class CRectF : public tagRECTF 
{
public:

// Constructors

	// uninitialized rectangle
	CRectF();
	// from left, top, right, and bottom
	CRectF(float l, float t, float r, float b);
	// copy constructor
	CRectF(const RECTF& srcRect);
	CRectF(const RECT& rc);
	// from a pointer to another rect
	CRectF(LPCRECTF lpSrcRect);
	// from a point and size
	CRectF(POINTF point, SIZEF size);
	// from two points
	CRectF(POINTF topLeft, POINTF bottomRight);

// Attributes (in addition to RECTF members)

	// retrieves the width
	float Width() const;
	// returns the height
	float Height() const;
	// returns the size
	CSizeF Size() const;
	// reference to the top-left point
	CPointF& TopLeft();
	// reference to the bottom-right point
	CPointF& BottomRight();
	// const reference to the top-left point
	const CPointF& TopLeft() const;
	// const reference to the bottom-right point
	const CPointF& BottomRight() const;
	// the geometric center point of the rectangle
	CPointF CenterPoint() const;
	// swap the left and right
	void SwapLeftRight();
	static void SwapLeftRight(LPRECTF lpRect);

	// convert between CRectF and LPRECTF/LPCRECTF (no need for &)
	operator LPRECTF();
	operator LPCRECTF() const;

	// returns TRUE if rectangle has no area
	BOOL IsRectFEmpty() const;
	// returns TRUE if rectangle is at (0,0) and has no area
	BOOL IsRectNull() const;
	// returns TRUE if point is within rectangle
	BOOL PtInRectF(POINTF point) const;

// Operations

	// set rectangle from left, top, right, and bottom
	void SetRect(float x1, float y1, float x2, float y2);
	void SetRect(POINTF topLeft, POINTF bottomRight);
	// empty the rectangle
	void SetRectEmpty();
	// copy from another rectangle
	void CopyRectF(LPCRECTF lpSrcRect);
	// TRUE if exactly the same as another rectangle
	BOOL EqualRect(LPCRECTF lpRect) const;

	// inflate rectangle's width and height without
	// moving its top or left
	void InflateRect(float x, float y);
	void InflateRect(SIZEF size);
	void InflateRect(LPCRECTF lpRect);
	void InflateRect(float l, float t, float r, float b);
	// deflate the rectangle's width and height without
	// moving its top or left
	void DeflateRect(float x, float y);
	void DeflateRect(SIZEF size);
	void DeflateRect(LPCRECTF lpRect);
	void DeflateRect(float l, float t, float r, float b);

	// translate the rectangle by moving its top and left
	void OffsetRect(float x, float y);
	void OffsetRect(SIZEF size);
	void OffsetRect(POINTF point);
	void NormalizeRect();

	// set this rectangle to intersection of two others
	BOOL IntersectRect(LPCRECTF lpRect1, LPCRECTF lpRect2);

	// set this rectangle to bounding union of two others
	BOOL UnionRect(LPCRECTF lpRect1, LPCRECTF lpRect2);

	// set this rectangle to minimum of two others
	BOOL SubtractRect(LPCRECTF lpRectSrc1, LPCRECTF lpRectSrc2);

	BOOL IsRectEmpty() {return left == right || bottom == top;};

// Additional Operations
	void operator=(const RECTF& srcRect);
	void operator=(const RECT& src);
	BOOL operator==(const RECTF& rect) const;
	BOOL operator!=(const RECTF& rect) const;
	void operator+=(POINTF point);
	void operator+=(SIZEF size);
	void operator+=(LPCRECTF lpRect);
	void operator-=(POINTF point);
	void operator-=(SIZEF size);
	void operator-=(LPCRECTF lpRect);
	void operator&=(const RECTF& rect);
	void operator|=(const RECTF& rect);

// Operators returning CRectF values
	CRectF operator+(POINTF point) const;
	CRectF operator-(POINTF point) const;
	CRectF operator+(LPCRECTF lpRect) const;
	CRectF operator+(SIZEF size) const;
	CRectF operator-(SIZEF size) const;
	CRectF operator-(LPCRECTF lpRect) const;
	CRectF operator&(const RECTF& rect2) const;
	CRectF operator|(const RECTF& rect2) const;
	CRectF MulDiv(float nMultiplier, float nDivisor) const;

	RECT GetRECT();
};

// CSizeF
inline CSizeF::CSizeF()
	{ /* random filled */ }
inline CSizeF::CSizeF(float initCX, float initCY)
	{ cx = initCX; cy = initCY; }
inline CSizeF::CSizeF(SIZEF initSize)
	{ *(SIZEF*)this = initSize; }
inline CSizeF::CSizeF(POINTF initPt)
	{ *(POINTF*)this = initPt; }
inline CSizeF::CSizeF(DWORD dwSize)
	{
		cx = (short)LOWORD(dwSize);
		cy = (short)HIWORD(dwSize);
	}
inline BOOL CSizeF::operator==(SIZEF size) const
	{ return (cx == size.cx && cy == size.cy); }
inline BOOL CSizeF::operator!=(SIZEF size) const
	{ return (cx != size.cx || cy != size.cy); }
inline void CSizeF::operator+=(SIZEF size)
	{ cx += size.cx; cy += size.cy; }
inline void CSizeF::operator-=(SIZEF size)
	{ cx -= size.cx; cy -= size.cy; }
inline CSizeF CSizeF::operator+(SIZEF size) const
	{ return CSizeF(cx + size.cx, cy + size.cy); }
inline CSizeF CSizeF::operator-(SIZEF size) const
	{ return CSizeF(cx - size.cx, cy - size.cy); }
inline CSizeF CSizeF::operator-() const
	{ return CSizeF(-cx, -cy); }
inline CPointF CSizeF::operator+(POINTF point) const
	{ return CPointF(cx + point.x, cy + point.y); }
inline CPointF CSizeF::operator-(POINTF point) const
	{ return CPointF(cx - point.x, cy - point.y); }
inline CRectF CSizeF::operator+(const RECTF* lpRect) const
	{ return CRectF(lpRect) + *this; }
inline CRectF CSizeF::operator-(const RECTF* lpRect) const
	{ return CRectF(lpRect) - *this; }

// CPointF
inline CPointF::CPointF()
	{ /* random filled */ }
inline CPointF::CPointF(float initX, float initY)
	{ x = initX; y = initY; }
#if !defined(_AFX_CORE_IMPL) || !defined(_AFXDLL) || defined(_DEBUG)
inline CPointF::CPointF(POINTF initPt)
	{ *(POINTF*)this = initPt; }
#endif
inline CPointF::CPointF(SIZEF initSize)
	{ *(SIZEF*)this = initSize; }
inline CPointF::CPointF(DWORD dwPoint)
	{
		x = (short)LOWORD(dwPoint);
		y = (short)HIWORD(dwPoint);
	}
inline void CPointF::Offset(float xOffset, float yOffset)
	{ x += xOffset; y += yOffset; }
inline void CPointF::Offset(POINTF point)
	{ x += point.x; y += point.y; }
inline void CPointF::Offset(SIZEF size)
	{ x += size.cx; y += size.cy; }
inline BOOL CPointF::operator==(POINTF point) const
	{ return (x == point.x && y == point.y); }
inline BOOL CPointF::operator!=(POINTF point) const
	{ return (x != point.x || y != point.y); }
inline void CPointF::operator+=(SIZEF size)
	{ x += size.cx; y += size.cy; }
inline void CPointF::operator-=(SIZEF size)
	{ x -= size.cx; y -= size.cy; }
inline void CPointF::operator+=(POINTF point)
	{ x += point.x; y += point.y; }
inline void CPointF::operator-=(POINTF point)
	{ x -= point.x; y -= point.y; }
inline CPointF CPointF::operator+(SIZEF size) const
	{ return CPointF(x + size.cx, y + size.cy); }
inline CPointF CPointF::operator-(SIZEF size) const
	{ return CPointF(x - size.cx, y - size.cy); }
inline CPointF CPointF::operator-() const
	{ return CPointF(-x, -y); }
inline CPointF CPointF::operator+(POINTF point) const
	{ return CPointF(x + point.x, y + point.y); }
inline CSizeF CPointF::operator-(POINTF point) const
	{ return CSizeF(x - point.x, y - point.y); }
inline CRectF CPointF::operator+(const RECTF* lpRect) const
	{ return CRectF(lpRect) + *this; }
inline CRectF CPointF::operator-(const RECTF* lpRect) const
	{ return CRectF(lpRect) - *this; }

// CRectF
inline CRectF::CRectF()
	{ /* random filled */ }
inline CRectF::CRectF(const RECT& rc)
	{left = (float)rc.left; top = (float)rc.top; right = (float)rc.right; bottom = (float)rc.bottom;}
inline CRectF::CRectF(float l, float t, float r, float b)
	{ left = l; top = t; right = r; bottom = b; }
inline CRectF::CRectF(const RECTF& srcRect)
{ ::CopyRectF(this, &srcRect); }
inline CRectF::CRectF(LPCRECTF lpSrcRect)
	{ ::CopyRectF(this, lpSrcRect); }
inline CRectF::CRectF(POINTF point, SIZEF size)
	{ right = (left = point.x) + size.cx; bottom = (top = point.y) + size.cy; }
inline CRectF::CRectF(POINTF topLeft, POINTF bottomRight)
	{ left = topLeft.x; top = topLeft.y;
		right = bottomRight.x; bottom = bottomRight.y; }
inline float CRectF::Width() const
	{ return right - left; }
inline float CRectF::Height() const
	{ return bottom - top; }
inline CSizeF CRectF::Size() const
	{ return CSizeF(right - left, bottom - top); }
inline CPointF& CRectF::TopLeft()
	{ return *((CPointF*)this); }
inline CPointF& CRectF::BottomRight()
	{ return *((CPointF*)this+1); }
inline const CPointF& CRectF::TopLeft() const
	{ return *((CPointF*)this); }
inline const CPointF& CRectF::BottomRight() const
	{ return *((CPointF*)this+1); }
inline CPointF CRectF::CenterPoint() const
	{ return CPointF((left+right)/2, (top+bottom)/2); }
inline void CRectF::SwapLeftRight()
	{ SwapLeftRight(LPRECTF(this)); }
inline void CRectF::SwapLeftRight(LPRECTF lpRect)
	{ float temp = lpRect->left; lpRect->left = lpRect->right; lpRect->right = temp; }
inline CRectF::operator LPRECTF()
	{ return this; }
inline void CRectF::operator=(const RECT& src)
	{left = float(src.left); top = float(src.top); right = float(src.right); bottom = float(src.bottom);};
inline CRectF::operator LPCRECTF() const
	{ return this; }
inline BOOL CRectF::IsRectFEmpty() const
	{ return ::IsRectFEmpty(this); }
inline BOOL CRectF::IsRectNull() const
	{ return (left == 0 && right == 0 && top == 0 && bottom == 0); }
inline BOOL CRectF::PtInRectF(POINTF point) const
	{ return ::PtInRectF(this, point); }
inline void CRectF::SetRect(float x1, float y1, float x2, float y2)
	{ left = x1; top = y1; right = x2; bottom = y2;}
inline void CRectF::SetRect(POINTF topLeft, POINTF bottomRight)
	{ SetRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y); }
inline void CRectF::SetRectEmpty()
	{ left = right = bottom= top = 0;}
inline void CRectF::CopyRectF(LPCRECTF lpSrcRect)
	{ ::CopyRectF(this, lpSrcRect); }
inline BOOL CRectF::EqualRect(LPCRECTF lpRect) const
	{ return ::EqualRectF(this, lpRect); }
inline void CRectF::InflateRect(float x, float y)
	{ ::InflateRectF(this, x, y); }
inline void CRectF::InflateRect(SIZEF size)
	{ ::InflateRectF(this, size.cx, size.cy); }
inline void CRectF::DeflateRect(float x, float y)
	{ ::InflateRectF(this, -x, -y); }
inline void CRectF::DeflateRect(SIZEF size)
	{ ::InflateRectF(this, -size.cx, -size.cy); }
inline void CRectF::OffsetRect(float x, float y)
	{ ::OffsetRectF(this, x, y); }
inline void CRectF::OffsetRect(POINTF point)
	{ ::OffsetRectF(this, point.x, point.y); }
inline void CRectF::OffsetRect(SIZEF size)
	{ ::OffsetRectF(this, size.cx, size.cy); }
inline BOOL CRectF::IntersectRect(LPCRECTF lpRect1, LPCRECTF lpRect2)
	{ return ::IntersectRectF(this, lpRect1, lpRect2);}
inline BOOL CRectF::UnionRect(LPCRECTF lpRect1, LPCRECTF lpRect2)
	{ return ::UnionRectF(this, lpRect1, lpRect2); }
inline void CRectF::operator=(const RECTF& srcRect)
	{ ::CopyRectF(this, &srcRect); }
inline BOOL CRectF::operator==(const RECTF& rect) const
	{ return ::EqualRectF(this, &rect); }
inline BOOL CRectF::operator!=(const RECTF& rect) const
	{ return !::EqualRectF(this, &rect); }
inline void CRectF::operator+=(POINTF point)
	{ ::OffsetRectF(this, point.x, point.y); }
inline void CRectF::operator+=(SIZEF size)
	{ ::OffsetRectF(this, size.cx, size.cy); }
inline void CRectF::operator+=(LPCRECTF lpRect)
	{ InflateRect(lpRect); }
inline void CRectF::operator-=(POINTF point)
	{ ::OffsetRectF(this, -point.x, -point.y); }
inline void CRectF::operator-=(SIZEF size)
	{ ::OffsetRectF(this, -size.cx, -size.cy); }
inline void CRectF::operator-=(LPCRECTF lpRect)
	{ DeflateRect(lpRect); }
inline void CRectF::operator&=(const RECTF& rect)
	{ ::IntersectRectF(this, this, &rect); }
inline void CRectF::operator|=(const RECTF& rect)
	{ ::UnionRectF(this, this, &rect); }
inline CRectF CRectF::operator+(POINTF pt) const
	{ CRectF rect(*this); ::OffsetRectF(&rect, pt.x, pt.y); return rect; }
inline CRectF CRectF::operator-(POINTF pt) const
	{ CRectF rect(*this); ::OffsetRectF(&rect, -pt.x, -pt.y); return rect; }
inline CRectF CRectF::operator+(SIZEF size) const
	{ CRectF rect(*this); ::OffsetRectF(&rect, size.cx, size.cy); return rect; }
inline CRectF CRectF::operator-(SIZEF size) const
	{ CRectF rect(*this); ::OffsetRectF(&rect, -size.cx, -size.cy); return rect; }
inline CRectF CRectF::operator+(LPCRECTF lpRect) const
	{ CRectF rect(this); rect.InflateRect(lpRect); return rect; }
inline CRectF CRectF::operator-(LPCRECTF lpRect) const
	{ CRectF rect(this); rect.DeflateRect(lpRect); return rect; }
inline CRectF CRectF::operator&(const RECTF& rect2) const
	{ CRectF rect; ::IntersectRectF(&rect, this, &rect2);
		return rect; }
inline CRectF CRectF::operator|(const RECTF& rect2) const
	{ CRectF rect; ::UnionRectF(&rect, this, &rect2);
		return rect; }
inline BOOL CRectF::SubtractRect(LPCRECTF lpRectSrc1, LPCRECTF lpRectSrc2)
	{ 
		left = lpRectSrc1->left;
		top = lpRectSrc1->top;
		right = lpRectSrc1->right;
		bottom = lpRectSrc1->bottom;
		NormalizeRect();

		RECTF rect;
		if(!IntersectRectF(&rect, lpRectSrc1, lpRectSrc2))
			return FALSE;

		if(rect.bottom == bottom && rect.top == top)
		{
			if(rect.left > left)
				right = rect.right;
			else
				left = rect.right;
		}
		
		if(left == rect.left && right == rect.right)
		{
			if(rect.top > top)
				bottom = rect.top;
			else
				top = rect.top;
		};

		return TRUE;
	}
inline void CRectF::NormalizeRect() 
	{NormalizeRectF(this);};
inline RECT CRectF::GetRECT()
	{
		RECT rcTmp;
		rcTmp.left = ROUND(left);
		rcTmp.top = ROUND(top);
		rcTmp.right = ROUND(right);
		rcTmp.bottom = ROUND(bottom);
		return rcTmp;
	};

inline CArchive& AFXAPI operator >> (CArchive& ar, CRectF& rc)
{
	ar >> rc.left;
	ar >> rc.top;
	ar >> rc.right;
	ar >> rc.bottom;
	return ar;
};

inline CArchive& AFXAPI operator >> (CArchive& ar, CPointF& pt)
{
	ar >> pt.x;
	ar >> pt.y;
	return ar;
};

inline CArchive& AFXAPI operator >> (CArchive& ar, CSizeF& size)
{
	ar >> size.cx;
	ar >> size.cy;
	return ar;
};

inline CArchive& AFXAPI operator << (CArchive& ar, CRectF& rc)
{
	ar << rc.left;
	ar << rc.top;
	ar << rc.right;
	ar << rc.bottom;
	return ar;
};

inline CArchive& AFXAPI operator << (CArchive& ar, CPointF& pt)
{
	ar << pt.x;
	ar << pt.y;
	return ar;
};

inline CArchive& AFXAPI operator << (CArchive& ar, CSizeF& size)
{
	ar << size.cx;
	ar << size.cy;
	return ar;
};

#endif // !defined(AFX_RECTF_H__CD0798B0_36E8_42FC_9E45_462AE2D82437__INCLUDED_)

