#ifndef __CXDRAWTOOL_H_
#define __CXDRAWTOOL_H_

#include <math.h>
///////////////////////////////////////////////////////////////
//功 能：计算正好是30度倍数的点
//0.5236 30
inline void Calc30Or45DegreeDuploidPoint(POINT ptOrigin, POINT& ptCal)
{
	int dx = ptCal.x - ptOrigin.x;
	int dy = ptOrigin.y - ptCal.y;
	if (dx == 0 && dy == 0)
		return;

	double dDuploid30 = int((atan2(dy, dx) + (dy >= 0 ? 0.2618 : -0.2618)) / 0.5236) * 0.5236;
	double dOffset30 = fabs(dDuploid30 - atan2(dy, dx));

	double dDuploid45 = int((atan2(dy, dx) + (dy >= 0 ? 0.3927 : -0.3927)) / 0.7854) * 0.7854;
	double dOffset45 = fabs(dDuploid45 - atan2(dy, dx));

	double dDegree = dOffset30 <= dOffset45 ? dDuploid30 : dDuploid45;
	if (abs(dy) < abs(dx))
		ptCal.y = int(ptOrigin.y - dx * tan(dDegree) + 0.5);
	else
		ptCal.x = int(ptOrigin.x + dy / tan(dDegree) + 0.5);
}

template <class T>
class CProxy_ICxDrawToolEvents : public IConnectionPointImpl<T, &IID_ICxDrawToolEvents, CComDynamicUnkArray>
{
public:

//ICxDrawToolEvents : IUnknown
public:
	HRESULT Fire_OnViewChange(RECT* pInvalidRect = NULL)
	{
		T* pT = (T*)this;
		pT->Lock();
		HRESULT ret;
		IUnknown** pp = m_vec.begin();
		while (pp < m_vec.end())
		{
			if (*pp != NULL)
			{
				ICxDrawToolEvents* pCxDrawToolEvents = reinterpret_cast<ICxDrawToolEvents*>(*pp);
				ret = pCxDrawToolEvents->OnViewChange(pInvalidRect);
			}
			pp++;
		}
		pT->Unlock();
		return ret;
	}

	HRESULT Fire_OnCreate()
	{
		T* pT = (T*)this;
		pT->Lock();
		HRESULT ret;
		IUnknown** pp = m_vec.begin();
		while (pp < m_vec.end())
		{
			if (*pp != NULL)
			{
				ICxDrawToolEvents* pCxDrawToolEvents = reinterpret_cast<ICxDrawToolEvents*>(*pp);
				ret = pCxDrawToolEvents->OnCreate();
			}
			pp++;
		}
		pT->Unlock();
		return ret;
	}

	HRESULT Fire_OnCancel()
	{
		T* pT = (T*)this;
		pT->Lock();
		HRESULT ret;
		IUnknown** pp = m_vec.begin();
		while (pp < m_vec.end())
		{
			if (*pp != NULL)
			{
				ICxDrawToolEvents* pCxDrawToolEvents = reinterpret_cast<ICxDrawToolEvents*>(*pp);
				ret = pCxDrawToolEvents->OnCancel();
			}
			pp++;
		}
		pT->Unlock();
		return ret;
	}
};
///////////////////////////////////////////////////////////
class ATL_NO_VTABLE CCxDrawToolBase
{
public:
	CCxDrawToolBase()
	{
	}
	~CCxDrawToolBase()
	{
	}
public:
	HRESULT CaptureMouse()
	{
		ATLASSERT(m_pDrawToolSite != NULL);
		return m_pDrawToolSite->CaptureMouse();
	}
	HRESULT ReleaseMouse()
	{
		ATLASSERT(m_pDrawToolSite != NULL);
		return m_pDrawToolSite->ReleaseMouse();
	}

	BOOL PressCursorKey(UINT nChar)
	{
		SIZE szGrid;
		szGrid.cx = 1;
		szGrid.cy = 1;
		m_pDrawToolSite->GetGridSpace(&szGrid);
			
		POINT ptOffset;
		ptOffset.x = ptOffset.y = 0;

		switch (nChar)
		{
		case VK_LEFT:
			ptOffset.x = -szGrid.cx;
			break;
		case VK_UP:
			ptOffset.y = -szGrid.cy;
			break;
		case VK_RIGHT:
			ptOffset.x = szGrid.cx;
			break;
		case VK_DOWN:
			ptOffset.y = szGrid.cy;
			break;
		default:
			break;
		}

		POINT point;
		GetCursorPos(&point);
		point.x += ptOffset.x;
		point.y += ptOffset.y;
		SetCursorPos(point.x, point.y);
		
		return (ptOffset.x != 0 || ptOffset.y != 0);
	}

// Attributes
public:	
	ICxDrawToolSite* m_pDrawToolSite;
};

////////////////////////////////////////////////////////////
template <class T>
class ATL_NO_VTABLE ICxDrawToolImpl : public ICxDrawTool
{
public:
	STDMETHOD(SetDrawToolSite)(ICxDrawToolSite* pDrawToolSite)
	{
		T* pT = static_cast<T*>(this);
		pT->m_pDrawToolSite = pDrawToolSite;

		return S_OK;
	}
	STDMETHOD(GetBoundingRect)(RECT* pBoundingRect)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(DoDraw)(HDC hdcDest)
	{
		return S_OK;
	}
	STDMETHOD(DoLButtonDblClk)(UINT nFlags, POINT point)
	{
		return S_OK;
	}
	STDMETHOD(DoMouseMove)(UINT nFlags, POINT point)
	{
		SetCursor(LoadCursor(NULL, IDC_CROSS));

		return S_OK;
	}
	STDMETHOD(DoLButtonUp)(UINT nFlags, POINT point)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(DoLButtonDown)(UINT nFlags, POINT point)
	{
		return S_OK;
	}
	STDMETHOD(DoRButtonDown)(UINT nFlags, POINT point)
	{
		return DoCancel();
	}
	STDMETHOD(DoChar)(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		return S_OK;
	}
	STDMETHOD(DoKeyDown)(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		return S_OK;
	}
	STDMETHOD(DoCancel)()
	{
		return S_OK;
	}
	STDMETHOD(DoInit)()
	{
		return S_OK;
	}
};


#endif // __CXDRAWTOOL_H_
