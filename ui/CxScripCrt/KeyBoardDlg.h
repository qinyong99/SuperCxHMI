// KeyBoardDlg.h: interface for the CKeyBoardDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYBOARDDLG_H__2431BEE6_738B_4575_8837_4DE0BFE51179__INCLUDED_)
#define AFX_KEYBOARDDLG_H__2431BEE6_738B_4575_8837_4DE0BFE51179__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

class CKeyBoardDlg : public CDialogImpl<CKeyBoardDlg>  
{
public:
	CKeyBoardDlg();
	virtual ~CKeyBoardDlg();

	enum { IDD = IDD_KEYBORAD };

BEGIN_MSG_MAP(CKeyBoardDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_RANGE_HANDLER(IDC_BUTTON1, IDC_BUTTON30, OnClickedSymbols)
	MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedSymbols(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetTextOutWindow(HWND hWndTextOut);

	CEdit m_edit; 
};

#endif // !defined(AFX_KEYBOARDDLG_H__2431BEE6_738B_4575_8837_4DE0BFE51179__INCLUDED_)
