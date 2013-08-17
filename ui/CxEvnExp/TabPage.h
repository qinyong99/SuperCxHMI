// TabPage.h: 各子属性页的基类
//
//////////////////////////////////////////////////////////////////////

#ifndef __TABPAGE_H_
#define __TABPAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTabPage
{
public:
	CTabPage()
	{
	}
	virtual ~CTabPage()
	{
	}

	virtual void MoveTo(int nLeft, int nTop, int nWidth, int nHeight) = 0;
	virtual void Create(HWND hWndParent) = 0;
	virtual void Show(BOOL b) = 0;
	virtual void Apply() = 0;
};

template <class TBase>
class CTabPageImpl : 
	public CTabPage,
	public TBase
{
public:
	CTabPageImpl()
	{
	}
//	virtual ~CTabPageImpl()
//	{
//	}


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
			{
// 				SetFocus();
				TBase::OnShow();
			}
		}
	}
	virtual void Apply() 
	{
		TBase::OnOK();
	}
};

#endif //__TABPAGE_H_