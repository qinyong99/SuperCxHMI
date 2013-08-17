#if !defined(AFX_KEYBOARDDLG_H__708FA1CA_34CC_407B_B727_87B7BC2C8656__INCLUDED_)
#define AFX_KEYBOARDDLG_H__708FA1CA_34CC_407B_B727_87B7BC2C8656__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyBoardDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardDlg dialog

class CKeyBoardDlg : public CDialog
{
// Construction
public:
	CKeyBoardDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKeyBoardDlg)
	enum { IDD = IDD_KEYBOARD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	static CKeyBoardDlg* c_pKeyBoardDlg;
	static HWND m_hwndFocusWindow;
	static HANDLE m_hEvent;

	static HHOOK m_hHookMouse;

	static WNDPROC m_pfnOldWndProc;

	DWORD m_dwThreadID;
	HANDLE m_hThread;

	static BOOL m_bOnlyNumber;
	static BOOL m_bDefaultPos;
	static int m_nXPos;
	static int m_nYPos;

	static BOOL m_bShift;
	static BOOL m_bControl;
	static BOOL m_bMenu;
	
	static void ShowSoftKeyBoard(CWnd* pParentWnd = NULL, BOOL bOnlyNumber = FALSE, BOOL bDefaultPos = TRUE, int nXPos = -1, int nYPos = -1);
	static void HideSoftKeyBoard();

	void ReleaseFocus();

	static LRESULT CALLBACK _ButtonSubclassProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK ButtonSubclassProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK _MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	
	DWORD WINAPI MessageLoopProc();

	static DWORD WINAPI _MessageLoopProc(void* pv)
	{
		return ((CKeyBoardDlg*)pv)->MessageLoopProc();
	} 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyBoardDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	~CKeyBoardDlg();
	virtual void PostNcDestroy();

	// Generated message map functions
	//{{AFX_MSG(CKeyBoardDlg)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	afx_msg void OnKeyCommand(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDDLG_H__708FA1CA_34CC_407B_B727_87B7BC2C8656__INCLUDED_)
