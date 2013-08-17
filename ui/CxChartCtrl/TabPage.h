// TabPage.h: 各子属性页的基类
//
//////////////////////////////////////////////////////////////////////

#ifndef __TABPAGE_H_
#define __TABPAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlapp.h>
#include <atlmisc.h>

#include "CustCtrl.h"

class CTabPage
{
public:
	CTabPage()
	{
	}

	virtual void MoveTo(int nLeft, int nTop, int nWidth, int nHeight) = 0;
	virtual void Create(HWND hWndParent) = 0;
	virtual void Show(BOOL b) = 0;
	virtual HRESULT SetSerialObjects(ULONG nObjects, IUnknown **ppUnk) = 0;
	virtual HRESULT ApplySerial() = 0;
	virtual void Enable(BOOL b) = 0;

	CString m_strCaption;
};

template <class TBase/*, const TCHAR* szCaption*/>
class CTabPageImpl : 
	public CTabPage,
	public TBase
{
public:
	CTabPageImpl()
	{
	}

	virtual void Create(HWND hWndParent)
	{
		CDialogImpl<TBase>::Create(hWndParent, NULL);
	}
	virtual void MoveTo(int nLeft, int nTop, int nWidth, int nHeight)
	{ 
		if (IsWindow())
			SetWindowPos(HWND_TOP, nLeft, nTop, nWidth, nHeight, SWP_SHOWWINDOW);
	}
	virtual void Show(BOOL b)
	{
		if (IsWindow())
		{
			ShowWindow(b ? SW_SHOW : SW_HIDE);
			if (b)
				SetFocus();
		}
	}
	virtual void Enable(BOOL b)
	{
		if (IsWindow())
			EnableWindow(b);
	}
	virtual HRESULT SetSerialObjects(ULONG nObjects, IUnknown **ppUnk)
	{
		return TBase::SetObjects(nObjects, ppUnk);
	}
	virtual HRESULT ApplySerial()
	{
		return TBase::Apply();
	}
};

#endif //__TABPAGE_H_
