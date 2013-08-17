// BitmapButton2.h: interface for the CBitmapButton2 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _BITMAPBUTTON2_H__
#define _BITMAPBUTTON2_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBitmapButton2 : public CBitmapButtonImpl<CBitmapButton2>
{
public:
	DECLARE_WND_SUPERCLASS(_T("WTL_BitmapButton2"), GetWndClassName())

	CBitmapButton2(DWORD dwExtendedStyle = BMPBTN_AUTOSIZE, HIMAGELIST hImageList = NULL) : 
		CBitmapButtonImpl<CBitmapButton2>(dwExtendedStyle, hImageList)
	{
		m_bCheckBox = FALSE;
		m_bSelected = FALSE;
	}

	typedef CBitmapButtonImpl<CBitmapButton2> baseClass;

	BEGIN_MSG_MAP(CBitmapButton2)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	void DoPaint(CDCHandle dc)
	{
		RECT rect;
		GetClientRect(&rect);

		dc.FillRect(&rect, (HBRUSH)LongToPtr(COLOR_3DFACE + 1));

		ATLASSERT(m_ImageList.m_hImageList != NULL);	// image list must be set
		ATLASSERT(m_nImage[0] != -1);			// main bitmap must be set

		// set bitmap according to the current button state
		int nImage = -1;
		bool bHover = IsHoverMode();
		if(m_fPressed == 1 || (m_bCheckBox && m_bSelected))
		{
			if (!IsWindowEnabled())
				nImage = m_nImage[_nImageDisabled];
			else
				nImage = m_nImage[_nImagePushed];
		}
		else if((!bHover && m_fFocus == 1) || (bHover && m_fMouseOver == 1))
			nImage = m_nImage[_nImageFocusOrHover];
		else if(!IsWindowEnabled())
			nImage = m_nImage[_nImageDisabled];
		if(nImage == -1)	// not there, use default one
			nImage = m_nImage[_nImageNormal];

		// draw the button image
		int xyPos = 0;
		if((m_fPressed == 1) && ((m_dwExtendedStyle & (BMPBTN_AUTO3D_SINGLE | BMPBTN_AUTO3D_DOUBLE)) != 0) && (m_nImage[_nImagePushed] == -1))
			xyPos = 1;
		m_ImageList.Draw(dc, nImage, xyPos, xyPos, ILD_NORMAL);

		// draw 3D border if required
		if((m_dwExtendedStyle & (BMPBTN_AUTO3D_SINGLE | BMPBTN_AUTO3D_DOUBLE)) != 0)
		{
			if(m_fPressed == 1)
				dc.DrawEdge(&rect, ((m_dwExtendedStyle & BMPBTN_AUTO3D_SINGLE) != 0) ? BDR_SUNKENOUTER : EDGE_SUNKEN, BF_RECT);
			else if(!bHover || m_fMouseOver == 1)
				dc.DrawEdge(&rect, ((m_dwExtendedStyle & BMPBTN_AUTO3D_SINGLE) != 0) ? BDR_RAISEDINNER : EDGE_RAISED, BF_RECT);

			if(!bHover && m_fFocus == 1)
			{
				::InflateRect(&rect, -2 * ::GetSystemMetrics(SM_CXEDGE), -2 * ::GetSystemMetrics(SM_CYEDGE));
				dc.DrawFocusRect(&rect);
			}
		}
	}

	void SetSelected(BOOL b)
	{
		m_bSelected = b;
		m_fPressed = b ? 1 : 0;
	}

	BOOL GetSelected()
	{
		return m_bSelected;
	}

	BOOL SubclassWindow(HWND hWnd)
	{
		BOOL bRet = CWindowImpl< CBitmapButton2, CButton, CControlWinTraits>::SubclassWindow(hWnd);
		if (bRet)
		{
			DWORD dwStyle = GetButtonStyle();
			if (dwStyle == BS_CHECKBOX)
				m_bCheckBox = TRUE;

			Init();
		}

		return bRet;
	}

	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		LRESULT lRet = 0;
		bool bHover = IsHoverMode();
		ATLTRACE("OnLButtonUp1\n");
//		if(!bHover)
//			lRet = DefWindowProc(uMsg, wParam, lParam);
		ATLTRACE("OnLButtonUp2\n");
		if(::GetCapture() == m_hWnd)
		{
			::ReleaseCapture();

//			if(bHover && m_fPressed == 1)
			::SendMessage(GetParent(), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
			
		}
		return 0/*lRet*/;
	}

	LRESULT OnCaptureChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_fPressed == 1)
		{
			ATLTRACE("OnCaptureChanged\n");

			if (m_bCheckBox)
			{
				if (m_bSelected)
				{
					m_fPressed = 0;
					m_bSelected = FALSE;
				}
				else
				{
					ATLTRACE("m_bSelected = TRUE;\n");
					m_bSelected = TRUE;
				}
			}
			else
			{
				m_fPressed = 0;
			}
			
			Invalidate();
			UpdateWindow();
		}

		return DefWindowProc(uMsg, wParam, lParam);
	}

	BOOL m_bCheckBox;
	BOOL m_bSelected;
};

#endif // _BITMAPBUTTON2_H__
